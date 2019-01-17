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
#define SAR_DEBUG 0
#define CONVERSION_TIME_MS 50

#if SAR_DEBUG
#define SAR_INFO(format, arg...) \
pr_info("sar_sensor: [%s] " format , __func__ , ##arg)
#else
#define SAR_INFO(format, arg...) do { } while (0)
#endif
#define SAR_NOTICE(format, arg...)	\
pr_notice("sar_sensor: [%s] " format , __func__ , ##arg)
#define SAR_ERR(format, arg...)	\
pr_err("sar_sensor: [%s] " format , __func__ , ##arg)
/*
* global variable
*/
static struct input_dev *sar_sensor_indev;
static struct workqueue_struct *sar_sensor_wq;
static int sar_sensor_irq;
struct delayed_work sar_sensor_work;
static bool interrupt_always_on = false;
static bool interrupt_enable = true;
static bool phone_call_on = false;
static bool data_on = false;
static bool probe_init = true;
static int gpio_status = 1;

struct jiengtai_jt06_platform_data
{
    u32 sar_gpio;
    u32 sar_flag;
};

static struct jiengtai_jt06_platform_data jiengtai_pdata;

/*
* functions declaration
*/
static ssize_t show_sar_status(struct device *class,struct device_attribute *attr, char *buf);
static ssize_t show_interrupt_status(struct device *class, struct device_attribute *attr, char *buf);
static ssize_t set_interrupt_always_on(struct device *class, struct device_attribute *attr, const char *buf, size_t count);
static ssize_t set_phone_call_on(struct device *class, struct device_attribute *attr, char *buf);
static ssize_t set_phone_call_off(struct device *class, struct device_attribute *attr, char *buf);
static ssize_t set_data_on(struct device *class, struct device_attribute *attr, char *buf);
static ssize_t set_data_off(struct device *class, struct device_attribute *attr, char *buf);
static void config_interrupt(void);
static void sar_interrupt_enable(bool enable);
static void sar_report_function(struct work_struct *dat);
static int  sar_sensor_devicetree_config(struct platform_device *ppdev);
static int sar_sensor_get_devtree_pdata(struct device *dev);

static DEVICE_ATTR(sar_status, S_IWUSR | S_IRUGO, show_sar_status, NULL);
static DEVICE_ATTR(interrupt_always_on, S_IWUSR | S_IRUGO, show_interrupt_status, set_interrupt_always_on);
static DEVICE_ATTR(call_state_on, S_IWUSR | S_IRUGO, set_phone_call_on, NULL);
static DEVICE_ATTR(call_state_off, S_IWUSR | S_IRUGO, set_phone_call_off, NULL);
static DEVICE_ATTR(data_state_on, S_IWUSR | S_IRUGO, set_data_on, NULL);
static DEVICE_ATTR(data_state_off, S_IWUSR | S_IRUGO, set_data_off, NULL);

/* Attribute Descriptor */
static struct attribute *sar_attrs[] = {
    &dev_attr_sar_status.attr,
    &dev_attr_interrupt_always_on.attr,
    &dev_attr_call_state_on.attr,
    &dev_attr_call_state_off.attr,
    &dev_attr_data_state_on.attr,
    &dev_attr_data_state_off.attr,
    NULL
};


/* Attribute group */
static struct attribute_group sar_attr_group = {
    .attrs = sar_attrs,
};

static ssize_t show_sar_status(struct device *class , struct device_attribute *attr, char *buf)
{
    char *s = buf;
    s += sprintf(buf, "%u\n",gpio_get_value(jiengtai_pdata.sar_gpio ) ? 1 : 0);
    return s - buf;
}

static ssize_t show_interrupt_status(struct device *class, struct device_attribute *attr, char *buf)
{
    char *s = buf;
    s += sprintf(buf, "%u\n", interrupt_always_on ? 1 : 0);
    return s - buf;
}

static ssize_t set_interrupt_always_on(struct device *class, struct device_attribute *attr, const char *buf, size_t count)
{
    int value = 0;

    sscanf(buf, "%d", &value);

    if (value == 0) {
        interrupt_always_on = false;
        SAR_NOTICE("Set sar sensor interrupt to dynamic enable/disable.\n");
        config_interrupt();
    }
    else if (value == 1) {
        interrupt_always_on = true;
        SAR_NOTICE("Set sar sensor interrupt to always on.\n");
        config_interrupt();
    }
    else {
        SAR_NOTICE("Receiving unknow value.\n");
    }
    
    return count;
}

static ssize_t set_phone_call_on(struct device *class, struct device_attribute *attr, char *buf)
{
    char *s = buf;
    
    SAR_INFO("set_phone_call_on\n");
    if (phone_call_on != true) {
        phone_call_on = true;
        config_interrupt();
    }
    s += sprintf(buf, "%u\n", interrupt_enable ? 1 : 0);
    
    return s - buf;
}

static ssize_t set_phone_call_off(struct device *class, struct device_attribute *attr, char *buf)
{
    char *s = buf;
    
    SAR_INFO("set_phone_call_off\n");
    if (phone_call_on != false) {
        phone_call_on = false;
        config_interrupt();
    }
    s += sprintf(buf, "%u\n", interrupt_enable ? 1 : 0);
    
    return s - buf;
}

static ssize_t set_data_on(struct device *class, struct device_attribute *attr, char *buf)
{
    char *s = buf;
    
    SAR_INFO("set_data_on\n");
    if (data_on != true) {
        data_on = true;
        config_interrupt();
    }
    s += sprintf(buf, "%u\n", interrupt_enable ? 1 : 0);
    
    return s - buf;
}

static ssize_t set_data_off(struct device *class, struct device_attribute *attr, char *buf)
{
    char *s = buf;
    
    SAR_INFO("set_data_off\n");
    if (data_on != false) {
        data_on = false;
        config_interrupt();
    }
    s += sprintf(buf, "%u\n", interrupt_enable ? 1 : 0);
    
    return s - buf;
}

static void config_interrupt(void)
{
    if (interrupt_always_on == true) {
        sar_interrupt_enable(true);
    }
    else {
        if ( (phone_call_on == false) && (data_on == false) ) {
            sar_interrupt_enable(false);
        }
        else {
            sar_interrupt_enable(true);
        }
    }
}

static void sar_interrupt_enable(bool enable)
{
    if (enable != interrupt_enable) {
        if (enable) {
            enable_irq(sar_sensor_irq);
            enable_irq_wake(sar_sensor_irq);
            SAR_NOTICE("Enable SAR sensor interrupt.\n");
        }
        else {
            disable_irq_wake(sar_sensor_irq);
            disable_irq_nosync(sar_sensor_irq);
            SAR_NOTICE("Disable SAR sensor interrupt.\n");
        }
        interrupt_enable = enable;
        
        //After enable interrupt, we need to check SAR sensor GPIO status.
        //After disable interrupt, we force to set SAR sensor is Far.
        if (probe_init == true) {
            probe_init = false;
        }
        else {
            SAR_NOTICE("Force check SAR sensor GPIO status.\n");
            queue_delayed_work(sar_sensor_wq, &sar_sensor_work, 0);
        }
    }
}

static irqreturn_t sar_interrupt_handler(int irq, void *dev_id)
{
    if (irq == sar_sensor_irq) {
        SAR_INFO("SAR sensor interrupt handler...gpio: %d..\n",
        gpio_get_value(jiengtai_pdata.sar_gpio ));
        queue_delayed_work(sar_sensor_wq, &sar_sensor_work, 0);
    }
    return IRQ_HANDLED;
}

static void sar_report_function(struct work_struct *dat)
{
    int value = 0;
    if (!sar_sensor_indev) {
        SAR_ERR("SAR sensor input device doesn't exist\n");
        return;
    }

    if (interrupt_enable == false) {
        value = 1;//SAR sensor gpio: 1=Far, 0=Near
    }
    else {
        msleep(CONVERSION_TIME_MS);
        value = gpio_get_value(jiengtai_pdata.sar_gpio) ? 1 : 0;
    }

    if (value != gpio_status) {
        input_report_switch(sar_sensor_indev, SW_SAR_SENSOR, !value);//Active-Low: !value; Active-High: value
        input_sync(sar_sensor_indev);
        SAR_NOTICE("SAR sensor SW_SAR_SENSOR report value = %d\n", !value);
        gpio_status = value;
    }
}

static int sar_input_device_create(void)
{
    int err = 0;
    sar_sensor_indev = input_allocate_device();

    if (!sar_sensor_indev) {
        SAR_ERR("sar_sensor_indev allocation fails\n");
        err = -ENOMEM;
        goto exit;
    }

    sar_sensor_indev->name = "sar_sensor_input";
    sar_sensor_indev->phys = "/dev/input/sar_sensor_indev";
    set_bit(EV_SW, sar_sensor_indev->evbit);
    set_bit(SW_SAR_SENSOR, sar_sensor_indev->swbit);
    err = input_register_device(sar_sensor_indev);

    if (err) {
        SAR_ERR("sar_sensor_indev registration fails\n");
        goto exit_input_free;
    }

    return 0;

    exit_input_free:
    input_free_device(sar_sensor_indev);
    sar_sensor_indev = NULL;
    exit:
    return err;
}

static int sar_sensor_get_devtree_pdata(struct device *dev)
{
    struct device_node *np = dev->of_node;
    jiengtai_pdata.sar_gpio = of_get_named_gpio_flags(np,"jt06,irq-gpio", 0, &jiengtai_pdata.sar_flag);
    SAR_NOTICE("#####  jt06_get_devtree: %d..\n",jiengtai_pdata.sar_gpio);

    if (jiengtai_pdata.sar_gpio  <= 0  )
        goto err1;

    return 0;
err1:
    return jiengtai_pdata.sar_gpio;


}

static int  sar_sensor_devicetree_config(struct platform_device *ppdev)
{
    int err;

    if (ppdev->dev.of_node)
    {
        err = sar_sensor_get_devtree_pdata(&ppdev->dev);

        if (err)
        {
            SAR_ERR("########    get sar sensor dtsi failed, err=%d\n", err);
            return err;
        }
    }
    else
    {
        SAR_ERR("########   no dtsi \n");
        return -ENOSYS;
    }

    return 0;
}

static int sar_sensor_probe(struct platform_device *pdev)
{
    int ret = 0;
    int irq = 0;
    unsigned long irqflags;

    if (!pdev)
        return -EINVAL;

    ret = sysfs_create_group(&pdev->dev.kobj, &sar_attr_group);

    if (ret) {
        SAR_ERR("Unable to create sysfs, error: %d\n",ret);
        goto fail_sysfs;
    }

    ret = sar_input_device_create();

    if (ret) {
        SAR_ERR("Unable to register input device, error: %d\n",ret);
        goto fail_create;
    }

    sar_sensor_wq = create_singlethread_workqueue("sar_sensor_wq");
    if(!sar_sensor_wq){
        SAR_ERR("Unable to create workqueue\n");
        goto fail_create;
    }

    ret=sar_sensor_devicetree_config(pdev);
    if (ret) {
        SAR_ERR("Unable to get GPIO , error: %d\n",ret);
        goto fail_sysfs;
    }

    if (!gpio_is_valid(jiengtai_pdata.sar_gpio )) {
        SAR_ERR("Invalid GPIO %d\n", jiengtai_pdata.sar_gpio );
        goto fail_create;
    }

    ret = gpio_request(jiengtai_pdata.sar_gpio , "SAR");
    if (ret < 0) {
        SAR_ERR("Failed to request GPIO %d\n",
        jiengtai_pdata.sar_gpio );
        goto fail_create;
    }

    ret = gpio_direction_input(jiengtai_pdata.sar_gpio );
    if (ret < 0) {
        SAR_ERR("Failed to configure direction for GPIO %d\n",jiengtai_pdata.sar_gpio );
        goto fail_free;
    }

    irq = gpio_to_irq(jiengtai_pdata.sar_gpio );
    sar_sensor_irq = irq;
    if (irq < 0) {
        SAR_ERR("Unable to get irq number for GPIO %d\n" , jiengtai_pdata.sar_gpio );
        goto fail_free;
    }

    irqflags = IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING;
    ret = request_any_context_irq(irq, sar_interrupt_handler,irqflags, "sar_sensor",
            sar_sensor_indev);
    if (ret < 0) {
    SAR_ERR("Unable to claim irq %d\n", irq);
        goto fail_free;
    }

    device_init_wakeup(&pdev->dev, 1);
    enable_irq_wake(irq);
    sar_interrupt_enable(false);

//Fix stack error
#if 1
    INIT_DELAYED_WORK(&sar_sensor_work,sar_report_function);
#else
    INIT_DEFERRABLE_WORK_ONSTACK(&sar_sensor_work,sar_report_function);
#endif

    return 0;

    fail_free:
    gpio_free(jiengtai_pdata.sar_gpio );
    fail_create:
    sysfs_remove_group(&pdev->dev.kobj, &sar_attr_group);
    fail_sysfs:
    return ret;
}

static int sar_sensor_remove(struct platform_device *pdev)
{
    sysfs_remove_group(&pdev->dev.kobj, &sar_attr_group);
    free_irq(sar_sensor_irq, NULL);
    cancel_delayed_work_sync(&sar_sensor_work);

    if (gpio_is_valid(jiengtai_pdata.sar_gpio ))
        gpio_free(jiengtai_pdata.sar_gpio );

    input_unregister_device(sar_sensor_indev);
    device_init_wakeup(&pdev->dev, 0);

    return 0;
}

static struct of_device_id sar_sensor_of_match[] = {
    { .compatible = "sar_sensor", },
    { },
};

static struct platform_driver sar_device_driver = {
    .probe      = sar_sensor_probe,
    .remove     = sar_sensor_remove,
    .driver     = {
        .name   = "platform_sar",
        .owner  = THIS_MODULE,
        //.pm   = &gpio_keys_pm_ops,
        .of_match_table = sar_sensor_of_match,
    }
};

static int __init sar_init(void)
{
    printk( "function= %s  \n", __func__);
    return platform_driver_register(&sar_device_driver);
}

static void __exit sar_exit(void)
{
    dump_stack();
    printk( "function= %s  \n", __func__);
    return;
}

module_init(sar_init);
module_exit(sar_exit);

MODULE_DESCRIPTION("Iqs231a Sar Sensor Driver");
MODULE_LICENSE("GPL");
