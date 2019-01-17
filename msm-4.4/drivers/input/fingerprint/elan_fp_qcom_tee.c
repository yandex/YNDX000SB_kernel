#include <linux/module.h>
#include <linux/input.h>
#include <linux/input/mt.h>
#include <linux/interrupt.h>
#include <linux/platform_device.h>
#include <linux/async.h>
#include <linux/delay.h>
#include <linux/gpio.h>
#include <linux/kthread.h>
#include <linux/miscdevice.h>
#include <linux/uaccess.h>
#include <linux/version.h>
#include <linux/slab.h>
#include <linux/wait.h>
#include <linux/io.h>
#include <linux/ioctl.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/syscalls.h>
#include <linux/err.h>
#include <linux/cdev.h>
#include <linux/types.h>
#include <linux/wakelock.h>
#include <linux/sched.h>
#include <linux/mutex.h>
#include <linux/pm.h>
#include <linux/time.h>
#include <linux/namei.h>
#include <linux/mount.h>
#include <linux/poll.h>
#include <linux/of_gpio.h>
#include "elan_fp_qcom_tee.h"

#define VERSION_LOG	"2.2.1"

static int elan_debug = 1;
#define ELAN_DEBUG(format, args ...) \
do { \
    if (elan_debug) \
        printk("[ELAN] " format, ##args); \
    } while(0)

#define KEY_FP_INT			KEY_POWER //KEY_WAKEUP // change by customer & framework support
#define KEY_FP_INT2			KEY_1 // change by customer & framework support
#define SET_SPI_OWNER       (0)
#if SET_SPI_OWNER
#include <soc/qcom/scm.h>
#endif

#if defined(CONFIG_FB)
#include <linux/notifier.h>
#include <linux/fb.h>
#include <linux/signal.h>
#endif

static int factory_status = 0;
static DEFINE_MUTEX(elan_factory_mutex);
static DECLARE_WAIT_QUEUE_HEAD(elan_poll_wq);
static int elan_work_flag = 0;
#if defined(CONFIG_FB)
static struct notifier_block fb_notif;
static int pid_fp = -1;
static int display_status = -1;
#endif
#define VDD_POWER_GPIO 1
struct elan_data  {
	int 					int_gpio;
	int						irq;
	int 					rst_gpio;
	int						irq_is_disable;
	struct miscdevice		elan_dev;	/* char device for ioctl */
	struct platform_device	*pdev;
	struct input_dev		*input_dev;
	spinlock_t				irq_lock;
	struct wake_lock		wake_lock;
    struct wake_lock	    hal_wake_lock;
#if VDD_POWER_GPIO
    int 					vdd_gpio;
#endif
};

void elan_irq_enable(void *_fp)
{
	struct elan_data *fp = _fp;	
	unsigned long irqflags = 0;
	ELAN_DEBUG("IRQ Enable = %d.\n", fp->irq);
  
	spin_lock_irqsave(&fp->irq_lock, irqflags);
	if (fp->irq_is_disable) 
	{
		enable_irq(fp->irq);
		fp->irq_is_disable = 0; 
	}
	spin_unlock_irqrestore(&fp->irq_lock, irqflags);
}

void elan_irq_disable(void *_fp)
{
	struct elan_data *fp = _fp;
	unsigned long irqflags;
	ELAN_DEBUG("IRQ Disable = %d.\n", fp->irq);

	spin_lock_irqsave(&fp->irq_lock, irqflags);
	if (!fp->irq_is_disable)
	{
		fp->irq_is_disable = 1; 
		disable_irq_nosync(fp->irq);
	}
	spin_unlock_irqrestore(&fp->irq_lock, irqflags);
}

static ssize_t show_drv_version_value(struct device *dev, struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "%s\n", VERSION_LOG);
}
static DEVICE_ATTR(drv_version, S_IRUGO, show_drv_version_value, NULL);

static ssize_t elan_debug_value(struct device *dev, struct device_attribute *attr, char *buf)
{
	if(elan_debug){
		elan_debug=0;
	} else {
		elan_debug=1;
	}
	return sprintf(buf, "[ELAN] elan debug %d\n", elan_debug);
}
static DEVICE_ATTR(elan_debug, S_IRUGO, elan_debug_value, NULL);

static struct attribute *elan_attributes[] = {
	&dev_attr_drv_version.attr,
	&dev_attr_elan_debug.attr,
	NULL
};

static struct attribute_group elan_attr_group = {
	.attrs = elan_attributes,
};

static void elan_reset(struct elan_data *fp)
{
	/* Developement platform */
	gpio_set_value(fp->rst_gpio, 0);
	mdelay(5);
	gpio_set_value(fp->rst_gpio, 1);
	mdelay(50);
}

#if defined(CONFIG_FB)

static int send_sig_to_pid(int sig, pid_t pid)
{
    struct siginfo info;

    info.si_signo = sig;
    info.si_errno = 0;
    info.si_code = SI_USER; 
    info.si_pid = get_current()->pid;
    info.si_uid = from_kuid_munged(current_user_ns(), current_uid());

    return kill_proc_info(sig, &info, pid);
}

static int fb_notifier_callback(struct notifier_block *self, unsigned long event, void *data)
{
    struct fb_event *evdata = data;
    int *blank ;

    if (event != 0x10)
    	return 0;

    if(evdata == NULL)
    {
    	pr_err("%s data is NULL\n",__func__);
    	return 0;
    }
    blank = evdata->data;
    if(blank == NULL)
    {
    	pr_err("%s blank is NULL\n",__func__);
    	return 0;
    }
    ELAN_DEBUG("%s fb notifier callback event = %lu, evdata->data = %d\n",__func__, event, *blank);
    if (evdata && evdata->data) {
        if (event == 0x10) {
            if (*blank == FB_BLANK_UNBLANK) {
                display_status = 0;
                if(pid_fp != -1)
                    send_sig_to_pid(SIGUSR2,pid_fp);
                ELAN_DEBUG("Display On\n");
            }
            else if (*blank == FB_BLANK_POWERDOWN) {
                display_status = 1;
                if(pid_fp != -1)
                    send_sig_to_pid(SIGUSR2,pid_fp);
                ELAN_DEBUG("Display Off\n");
            }
        }
    }
    return 0;
}
#endif

static long elan_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	struct elan_data *fp = filp->private_data;
    int keycode;
    int wake_lock_arg;

	ELAN_DEBUG("%s() : cmd = [%04X]\n", __func__, cmd);

	switch(cmd)
	{
		case ID_IOCTL_RESET: //6
			elan_reset(fp);
			ELAN_DEBUG("[IOCTL] RESET\n");
			break;

		case ID_IOCTL_POLL_INIT: //20
            elan_work_flag = 0;
			ELAN_DEBUG("[IOCTL] POLL INIT\n");
			break;

		case ID_IOCTL_POLL_EXIT: //23
            elan_work_flag = 1;
            wake_up(&elan_poll_wq);
			ELAN_DEBUG("[IOCTL] POLL EXIT\n");
			break;

        case ID_IOCTL_INPUT_KEYCODE: //22
			keycode =(int __user)arg;
			ELAN_DEBUG("[IOCTL] KEYCODE DOWN & UP, keycode = %d \n", keycode);
			if (!keycode) {
				ELAN_DEBUG("Keycode %d not defined, ignored\n", (int __user)arg);
				break ;
			}
			input_report_key(fp->input_dev, keycode, 1); // Added for KEY Event
			input_sync(fp->input_dev);
			input_report_key(fp->input_dev, keycode, 0); // Added for KEY Event
			input_sync(fp->input_dev);
			break;	
			
		case ID_IOCTL_SET_KEYCODE: //24
			keycode =(int __user)arg;
			ELAN_DEBUG("[IOCTL] SET KEYCODE, keycode = %d \n", keycode);
			if (!keycode) {
				ELAN_DEBUG("Keycode %d not defined, ignored\n", (int __user)arg);
				break ;
			}
			input_set_capability(fp->input_dev, EV_KEY, keycode); 
			set_bit(keycode, fp->input_dev->keybit);	
			break;

        case ID_IOCTL_INPUT_KEYCODE_DOWN: //28
            keycode =(int __user)arg;
            ELAN_DEBUG("[IOCTL] KEYCODE DOWN, keycode = %d \n", keycode);
            if(!keycode) {
                ELAN_DEBUG("Keycode %d not defined, ignored\n", (int __user)arg);
				break ;
            }
            input_report_key(fp->input_dev, keycode, 1);
			input_sync(fp->input_dev);
            break;

        case ID_IOCTL_INPUT_KEYCODE_UP: //29
            keycode =(int __user)arg;
            ELAN_DEBUG("[IOCTL] KEYCODE UP, keycode = %d \n", keycode);
            if(!keycode) {
                ELAN_DEBUG("Keycode %d not defined, ignored\n", (int __user)arg);
				break ;
            }
            input_report_key(fp->input_dev, keycode, 0);
			input_sync(fp->input_dev);
            break;

        case ID_IOCTL_READ_FACTORY_STATUS: //26
            mutex_lock(&elan_factory_mutex);
            ELAN_DEBUG("[IOCTL] READ factory_status = %d\n", factory_status);
            mutex_unlock(&elan_factory_mutex);
            return factory_status;
            break;

        case ID_IOCTL_WRITE_FACTORY_STATUS: //27
            mutex_lock(&elan_factory_mutex);
            factory_status = (int __user)arg;
            ELAN_DEBUG("[IOCTL] WRITE factory_status = %d\n", factory_status);
            mutex_unlock(&elan_factory_mutex);
            break;

        case ID_IOCTL_INT_STATUS: //40
            return gpio_get_value(fp->int_gpio);

        case ID_IOCTL_WAKE_LOCK_UNLOCK: //41
            wake_lock_arg = (int __user)arg;
            if(!wake_lock_arg)
            {
                wake_unlock(&fp->hal_wake_lock);
                ELAN_DEBUG("[IOCTL] HAL WAKE UNLOCK = %d\n", wake_lock_arg);
            }
            else if(wake_lock_arg)
            {
                wake_lock(&fp->hal_wake_lock);
                ELAN_DEBUG("[IOCTL] HAL WAKE LOCK = %d\n", wake_lock_arg);
            }
            else
                ELAN_DEBUG("[IOCTL] ERROR WAKE LOCK ARGUMENT\n");
            break;

		case ID_IOCTL_EN_IRQ: //55
			elan_irq_enable(fp);
			ELAN_DEBUG("[IOCTL] ENABLE IRQ\n");
			break;

		case ID_IOCTL_DIS_IRQ: //66
			elan_irq_disable(fp);
			ELAN_DEBUG("[IOCTL] DISABLE IRQ\n");
			break;

        case ID_IOCTL_SET_IRQ_TYPE: //91
            ELAN_DEBUG("[IOCTL] SET IRQ TYPE\n");
            irq_set_irq_type(fp->irq, IRQF_TRIGGER_FALLING | IRQF_NO_SUSPEND | IRQF_ONESHOT);
            break;

        case ID_IOCTL_DISPLAY_STATUS: //93
            ELAN_DEBUG("[IOCTL] DISPLAY_STATUS = %d\n", display_status);
            return display_status;

        case ID_IOCTL_DISPLAY_NOTIFY: //94
            break;
            
        case ID_IOCTL_SET_PID: //94
            pid_fp = (int __user)arg;
            ELAN_DEBUG("[IOCTL] ID_IOCTL_SET_PID = %d\n", pid_fp);
            break;

		default:
			ELAN_DEBUG("INVALID COMMAND\n");
			break;
	}
	return 0;
}

static unsigned int elan_poll(struct file *file, poll_table *wait)
{
	int mask=0;
    ELAN_DEBUG("%s()\n",__func__);

	wait_event_interruptible(elan_poll_wq, elan_work_flag > 0);
	if(elan_work_flag > 0)
		mask = elan_work_flag;

	elan_work_flag = 0;
	return mask;
}

static int elan_open(struct inode *inode, struct file *filp)
{
	struct elan_data *fp = container_of(filp->private_data, struct elan_data, elan_dev);	
	filp->private_data = fp;
	ELAN_DEBUG("%s()\n", __func__);
	return 0;
}

static int elan_close(struct inode *inode, struct file *filp)
{
	ELAN_DEBUG("%s()\n", __func__);
	return 0;
}

static const struct file_operations elan_fops = {
	.owner 			= THIS_MODULE,
	.open 			= elan_open,
	.unlocked_ioctl = elan_ioctl,
	.poll			= elan_poll,
	.release 		= elan_close,
};

#if SET_SPI_OWNER
static int set_pipe_ownership(void)
{
	const u32 TZ_BLSP_MODIFY_OWNERSHIP_ID = 3;
	const u32 TZBSP_TZ_ID = 3;
	int rc;
	struct scm_desc desc = {
		.arginfo = SCM_ARGS(2),
		.args[0] = 3,
		.args[1] = TZBSP_TZ_ID,
	};

	rc = scm_call2(SCM_SIP_FNID(SCM_SVC_TZ, TZ_BLSP_MODIFY_OWNERSHIP_ID), &desc);

	if(rc || desc.ret[0])
	{
		ELAN_DEBUG("%s() FAIL\n", __func__);
		return -EINVAL;
	}
	ELAN_DEBUG("%s() Success\n", __func__);
	return 0;
}
#endif

static irqreturn_t elan_irq_handler(int irq, void *dev_id)
{
	struct elan_data *fp = (struct elan_data *)dev_id;

	ELAN_DEBUG("%s()\n", __func__);
	wake_lock_timeout(&fp->wake_lock, msecs_to_jiffies(1000));
    if(fp == NULL)
		return IRQ_NONE;
    elan_work_flag = 1;
    wake_up(&elan_poll_wq);

	return IRQ_HANDLED;
}

static int elan_setup_cdev(struct elan_data *fp)
{
	
	fp->elan_dev.minor = MISC_DYNAMIC_MINOR;
	fp->elan_dev.name = "elan_fp";
	fp->elan_dev.fops = &elan_fops;
	fp->elan_dev.mode = S_IFREG|S_IRWXUGO; 
	if (misc_register(&fp->elan_dev) < 0) {
  		ELAN_DEBUG("misc_register failed\n");
		return -1;		
	}
  	else {
		ELAN_DEBUG("misc_register finished\n");		
	}
	return 0;
}

static int elan_sysfs_create(struct elan_data *sysfs)
{
	struct elan_data *fp = platform_get_drvdata(sysfs->pdev);
	int ret = 0;
	
	/* Register sysfs */
	ret = sysfs_create_group(&fp->pdev->dev.kobj, &elan_attr_group);
	if (ret) {
		ELAN_DEBUG("create sysfs attributes failed, ret = %d\n", ret);
		goto fail_un;
	}
	return 0;
fail_un:
	/* Remove sysfs */
	sysfs_remove_group(&fp->pdev->dev.kobj, &elan_attr_group);
		
	return ret;
}

static int elan_gpio_config(struct elan_data *fp)
{	
	int ret = 0;

    // Configure INT GPIO (Input)
	ret = gpio_request(fp->int_gpio, "elan-irq");
	if (ret < 0)
		ELAN_DEBUG("interrupt pin request gpio failed, ret = %d\n", ret);
	else {
		gpio_direction_input(fp->int_gpio);
		fp->irq = gpio_to_irq(fp->int_gpio);
        if(fp->irq < 0) {
            ELAN_DEBUG("gpio to irq failed, irq = %d\n", fp->irq);
            ret = -1;
        }
        else
            ELAN_DEBUG("gpio to irq success, irq = %d\n",fp->irq);
	}
	
	// Configure RST GPIO (Output)
	ret =  gpio_request(fp->rst_gpio, "elan-rst");
	if (ret < 0) {
		gpio_free(fp->int_gpio);
		free_irq(fp->irq, fp);
		ELAN_DEBUG("reset pin request gpio failed, ret = %d\n", ret);
	}
	else
        gpio_direction_output(fp->rst_gpio, 1);
#if VDD_POWER_GPIO
	// Configure VDD GPIO (Output)
	ret =  gpio_request(fp->vdd_gpio, "elan-vdd");
	if (ret < 0) {
		gpio_free(fp->rst_gpio);
		gpio_free(fp->int_gpio);
		free_irq(fp->irq, fp);
		ELAN_DEBUG("reset pin request gpio failed, ret = %d\n", ret);
	}
	else
        gpio_direction_output(fp->vdd_gpio, 1);
#endif
	return ret;
}

static int elan_dts_init(struct elan_data *fp, struct device_node *np)
{
	fp->rst_gpio = of_get_named_gpio(np, "elan,rst-gpio", 0);
	ELAN_DEBUG("rst_gpio = %d\n", fp->rst_gpio);
	if (fp->rst_gpio < 0)
		return fp->rst_gpio;
	
	fp->int_gpio = of_get_named_gpio(np, "elan,irq-gpio", 0);
	ELAN_DEBUG("int_gpio = %d\n", fp->int_gpio);
	if (fp->int_gpio < 0)
		return fp->int_gpio;
#if VDD_POWER_GPIO
	fp->vdd_gpio = of_get_named_gpio(np, "elan,vdd-gpio", 0);
		ELAN_DEBUG("vdd_gpio = %d\n", fp->vdd_gpio);
		if (fp->vdd_gpio < 0)
			return fp->vdd_gpio;
#endif
    return 0;
}

static int elan_probe(struct platform_device *pdev)
{	
	struct elan_data *fp = NULL;
	struct input_dev *input_dev = NULL;
	int ret = 0;
	
	ELAN_DEBUG("%s(), version = %s\n", __func__, VERSION_LOG);

	/* Allocate Device Data */
	fp = devm_kzalloc(&pdev->dev, sizeof(struct elan_data), GFP_KERNEL);
	if(!fp)
		ELAN_DEBUG("kzmalloc elan data failed\n");

	/* Init Input Device */
	input_dev = input_allocate_device();
	if (!input_dev)
		ELAN_DEBUG("alloc input_dev failed\n");

	fp->pdev = pdev;		

	platform_set_drvdata(pdev, fp);	

	input_dev->name = "elan";
	input_dev->id.bustype = BUS_SPI;
	input_dev->dev.parent = &pdev->dev;
	input_set_drvdata(input_dev, fp);	

	input_dev->evbit[0] = BIT_MASK(EV_SYN) | BIT_MASK(EV_KEY);
	input_set_capability(input_dev, EV_KEY, KEY_FP_INT); // change by customer, send key event to framework. KEY_xxx could be changed.
	input_set_capability(input_dev, EV_KEY, KEY_FP_INT2); // change by customer, send key event to framework. KEY_xxx could be changed.

	fp->input_dev = input_dev;	

	/* Init Sysfs */
	ret = elan_sysfs_create(fp);
	if(ret < 0)
		ELAN_DEBUG("sysfs create failed, ret = %d\n", ret);

	/* Init Char Device */
	ret = elan_setup_cdev(fp);
	if(ret < 0)
		ELAN_DEBUG("setup device failed, ret = %d\n", ret);

	/* Register Input Device */
	ret = input_register_device(input_dev);
	if(ret)
		ELAN_DEBUG("register input device failed, ret = %d\n", ret);

    ret = elan_dts_init(fp, pdev->dev.of_node);
    if(ret < 0)
		ELAN_DEBUG("device tree initial failed, ret = %d\n", ret);

	ret = elan_gpio_config(fp);
	if(ret < 0)
		ELAN_DEBUG("gpio config failed, ret = %d\n", ret);

    wake_lock_init(&fp->wake_lock, WAKE_LOCK_SUSPEND, "fp_wake_lock");
    wake_lock_init(&fp->hal_wake_lock, WAKE_LOCK_SUSPEND, "hal_fp_wake_lock");

	ret = request_irq(fp->irq, elan_irq_handler,
			IRQF_NO_SUSPEND | IRQF_TRIGGER_RISING | IRQF_ONESHOT, 
			pdev->dev.driver->name, fp);
	if(ret)
		ELAN_DEBUG("request irq failed, ret = %d\n", ret);

	irq_set_irq_wake(fp->irq, 1);

	spin_lock_init(&fp->irq_lock);
	
#if defined(CONFIG_FB)
	fb_notif.notifier_call = fb_notifier_callback;
	fb_register_client(&fb_notif);
#endif

	/* Set Spi to TZ */
#if SET_SPI_OWNER
	ret = set_pipe_ownership();
#endif

    ELAN_DEBUG("%s() End\n", __func__);
	return 0;
}

static int elan_remove(struct platform_device *pdev)
{
	struct elan_data *fp = platform_get_drvdata(pdev);
	
	if (fp->irq)
		free_irq(fp->irq, fp);
#if VDD_POWER_GPIO
	gpio_free(fp->vdd_gpio);
#endif
	gpio_free(fp->int_gpio);
	gpio_free(fp->rst_gpio);
	
	misc_deregister(&fp->elan_dev);
	input_free_device(fp->input_dev);
	
#if defined(CONFIG_FB)
	fb_unregister_client(&fb_notif);
#endif
		
	kfree(fp);

	platform_set_drvdata(pdev, NULL);
		
	return 0;
}

#ifdef CONFIG_PM_SLEEP
static int elan_suspend(struct device *dev)
{
	ELAN_DEBUG("elan suspend!\n");
	return 0;
}

static int elan_resume(struct device *dev)
{
	ELAN_DEBUG("elan resume!\n");
	return 0;
}
#endif

static SIMPLE_DEV_PM_OPS(elan_pm_ops, elan_suspend, elan_resume);

#ifdef CONFIG_OF
static struct of_device_id elan_metallica_table[] = {
	{ .compatible = "elan,elan_fp",},
	{ },
};
#else
#define elan_metallica_table NULL
#endif

static struct platform_driver elan_driver = {
	.driver = {
		.name 	= "elan",
		.owner = THIS_MODULE,
		.pm 	= &elan_pm_ops,
		.of_match_table = elan_metallica_table,
	},
	.probe 	= elan_probe,
	.remove = elan_remove,
};

static int __init elan_init(void)
{
	int ret = 0;
	ELAN_DEBUG("%s() Start\n", __func__);

	ret = platform_driver_register(&elan_driver);
	if(ret < 0)
		ELAN_DEBUG("%s FAIL !\n", __func__);

    ELAN_DEBUG("%s() End\n", __func__);
	return 0;
}

static void __exit elan_exist(void)
{	
	platform_driver_unregister(&elan_driver);
}

module_init(elan_init);
module_exit(elan_exist);

MODULE_AUTHOR("Elan");
MODULE_DESCRIPTION("ELAN SPI FingerPrint driver");
MODULE_VERSION(VERSION_LOG);
MODULE_LICENSE("GPL");
