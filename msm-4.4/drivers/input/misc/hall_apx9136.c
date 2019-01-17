#include <linux/module.h>
#include <linux/err.h>
#include <linux/delay.h>
#include <linux/interrupt.h>
#include <linux/init.h>
#include <linux/input.h>
#include <linux/platform_device.h>
#include <linux/workqueue.h>
#include <linux/gpio_event.h>
#include <linux/gpio.h>
#include <linux/of_platform.h>
#include <linux/of_gpio.h>
#define LID_DEBUG 1
#define CONVERSION_TIME_MS 50

#if LID_DEBUG
#define LID_INFO(format, arg...) \
pr_info("hall_sensor: [%s] " format , __func__ , ##arg)
#else
#define LID_INFO(format, arg...) do { } while (0)
#endif
#define LID_NOTICE(format, arg...)	\
pr_notice("hall_sensor: [%s] " format , __func__ , ##arg)
#define LID_ERR(format, arg...)	\
pr_err("hall_sensor: [%s] " format , __func__ , ##arg)
/*
* global variable
*/
static struct input_dev *lid_indev;
static struct workqueue_struct *lid_wq;
static int hall_sensor_irq;
struct delayed_work lid_hall_sensor_work;

struct anpec_apx9136_platform_data
{
	u32 hall_gpio;
	u32 hall_flag;
};

static struct anpec_apx9136_platform_data anpec_pdata;

/*
* functions declaration
*/
static ssize_t show_lid_status(struct device *class,struct device_attribute *attr, char *buf);
static void lid_report_function(struct work_struct *dat);
static int  hall_sensor_devicetree_config(struct platform_device *ppdev);
static int hall_sensor_get_devtree_pdata(struct device *dev);
//static int lid_input_device_create(void);
static DEVICE_ATTR(lid_status, S_IWUSR | S_IRUGO, show_lid_status, NULL);

/* Attribute Descriptor */
static struct attribute *lid_attrs[] = {
	&dev_attr_lid_status.attr,
	NULL
};


/* Attribute group */
static struct attribute_group lid_attr_group = {
	.attrs = lid_attrs,
};

static ssize_t show_lid_status(struct device *class , struct device_attribute *attr, char *buf)
{
	char *s = buf;
	s += sprintf(buf, "%u\n",gpio_get_value(anpec_pdata.hall_gpio ) ? 1 : 0);
	return s - buf;
}

static irqreturn_t lid_interrupt_handler(int irq, void *dev_id)
{
	if (irq == hall_sensor_irq) {
		LID_NOTICE("LID interrupt handler...gpio: %d..\n",
		gpio_get_value(anpec_pdata.hall_gpio ));
		queue_delayed_work(lid_wq, &lid_hall_sensor_work, 0);
	}
		return IRQ_HANDLED;
}

static void lid_report_function(struct work_struct *dat)
{
	int value = 0;
	if (!lid_indev) {
		LID_ERR("LID input device doesn't exist\n");
		return;
	}

	msleep(CONVERSION_TIME_MS);
	value = gpio_get_value(anpec_pdata.hall_gpio) ? 1 : 0;
	input_report_switch(lid_indev, SW_LID, !value);
	input_sync(lid_indev);
	LID_NOTICE("SW_LID report value = %d\n", value);
}

static int lid_input_device_create(void)
{
	int err = 0;
	lid_indev = input_allocate_device();

	if (!lid_indev) {
		LID_ERR("lid_indev allocation fails\n");
		err = -ENOMEM;
		goto exit;
	}

	lid_indev->name = "lid_input";
	lid_indev->phys = "/dev/input/lid_indev";
	set_bit(EV_SW, lid_indev->evbit);
	set_bit(SW_LID, lid_indev->swbit);
	err = input_register_device(lid_indev);

	if (err) {
		LID_ERR("lid_indev registration fails\n");
		goto exit_input_free;
	}

	return 0;
	
	exit_input_free:
	input_free_device(lid_indev);
	lid_indev = NULL;
	exit:
	return err;
}

static int hall_sensor_get_devtree_pdata(struct device *dev)
{
	struct device_node *np = dev->of_node;
	anpec_pdata.hall_gpio = of_get_named_gpio_flags(np,"apx9136,irq-gpio", 0, &anpec_pdata.hall_flag);
	LID_NOTICE("#####  apx9136_get_devtree: %d..\n",anpec_pdata.hall_gpio);

	if (anpec_pdata.hall_gpio  <= 0  )
		goto err1;

	return 0;
err1:
	return anpec_pdata.hall_gpio;


}

static int  hall_sensor_devicetree_config(struct platform_device *ppdev)
{
	int err;

	if (ppdev->dev.of_node)
	{
		err = hall_sensor_get_devtree_pdata(&ppdev->dev);

		if (err)
			{
				LID_ERR("########    get hall dtsi failed ,,err=%d", err);
				return err;
			}

	} 
	else 
	{
				LID_ERR("########   no dtsi \n");
				return -ENOSYS;
	}

	return 0;
}

static int hall_sensor_probe(struct platform_device *pdev)
{
	int ret = 0;
	int irq = 0;	
	unsigned long irqflags;

	if (!pdev)
		return -EINVAL;

	ret = sysfs_create_group(&pdev->dev.kobj, &lid_attr_group);

	if (ret) {
		LID_ERR("Unable to create sysfs, error: %d\n",
		ret);
		goto fail_sysfs;
	}

	ret = lid_input_device_create();

	if (ret) {
		LID_ERR("Unable to register input device, error: %d\n",ret);
		goto fail_create;
	}

	lid_wq = create_singlethread_workqueue("lid_wq");
	if(!lid_wq){
		LID_ERR("Unable to create workqueue\n");
		goto fail_create;
	}

	ret=hall_sensor_devicetree_config(pdev);
	if (ret) {
		LID_ERR("Unable to get GPIO , error: %d\n",ret);
		goto fail_sysfs;
	}

	if (!gpio_is_valid(anpec_pdata.hall_gpio )) {
		LID_ERR("Invalid GPIO %d\n", anpec_pdata.hall_gpio );
		goto fail_create;
	}

	ret = gpio_request(anpec_pdata.hall_gpio , "LID");
	if (ret < 0) {
		LID_ERR("Failed to request GPIO %d\n",
		anpec_pdata.hall_gpio );
		goto fail_create;
	}

	ret = gpio_direction_input(anpec_pdata.hall_gpio );
	if (ret < 0) {
		LID_ERR("Failed to configure direction for GPIO %d\n",anpec_pdata.hall_gpio );
		goto fail_free;
	}

	irq = gpio_to_irq(anpec_pdata.hall_gpio );
	hall_sensor_irq = irq;
	if (irq < 0) {
		LID_ERR("Unable to get irq number for GPIO %d\n" , anpec_pdata.hall_gpio );
		goto fail_free;
	}

	irqflags = IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING;
	ret = request_any_context_irq(irq, lid_interrupt_handler,
								irqflags, "hall_sensor",
								lid_indev);
	if (ret < 0) {
	LID_ERR("Unable to claim irq %d\n", irq);
		goto fail_free;
	}

	device_init_wakeup(&pdev->dev, 1);
	enable_irq_wake(irq);
#if 1
	INIT_DELAYED_WORK(&lid_hall_sensor_work,lid_report_function);
#else
	INIT_DEFERRABLE_WORK_ONSTACK(&lid_hall_sensor_work,lid_report_function);
#endif
	return 0;

	fail_free:
	gpio_free(anpec_pdata.hall_gpio );
	fail_create:
	sysfs_remove_group(&pdev->dev.kobj, &lid_attr_group);
	fail_sysfs:
	return ret;
}

static int hall_sensor_remove(struct platform_device *pdev)
{
	sysfs_remove_group(&pdev->dev.kobj, &lid_attr_group);
	free_irq(hall_sensor_irq, NULL);
	cancel_delayed_work_sync(&lid_hall_sensor_work);

	if (gpio_is_valid(anpec_pdata.hall_gpio ))
		gpio_free(anpec_pdata.hall_gpio );

	input_unregister_device(lid_indev);
	device_init_wakeup(&pdev->dev, 0);
	
	return 0;
}	

static struct of_device_id hall_sensor_of_match[] = {
	{ .compatible = "hall_sensor", },
	{ },
};

static struct platform_driver hall_device_driver = {
	.probe		= hall_sensor_probe,
	.remove		= hall_sensor_remove,
	.driver		= {
		.name	= "platform_hall",
		.owner	= THIS_MODULE,
	//	.pm	= &gpio_keys_pm_ops,
		.of_match_table = hall_sensor_of_match,
	}
};

static int __init hall_init(void)
{
	printk( "function= %s  \n", __func__);
	return platform_driver_register(&hall_device_driver);
}

static void __exit hall_exit(void)
{
	dump_stack();
	printk( "function= %s  \n", __func__);
	return;
}
  
module_init(hall_init);
module_exit(hall_exit);

MODULE_DESCRIPTION("Apx9136 Hall Sensor Driver");
MODULE_LICENSE("GPL");
