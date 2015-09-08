#include <linux/kernel.h>  
#include <linux/errno.h>  
#include <linux/module.h>  
#include <linux/fs.h>  
#include <linux/miscdevice.h>  
#include <asm/io.h>  
#include <linux/interrupt.h>  
#include <linux/irq.h>  
#include <linux/gpio.h>  
#include <mach/gpio.h>
#include <asm-generic/siginfo.h>  
#include <linux/init.h>  
#include <asm/signal.h>  
#include <linux/timer.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/platform_device.h>
#include <linux/of_gpio.h>
#include <asm/uaccess.h>  

static int DEVICE_IRQ_PIN;// AT91_PIN_PC29
static int major = 0;
static int minor = 0;
static dev_t dev;
static int irq = 0;
static struct fasync_struct *async;//声明fasync_struct
static irqreturn_t Protector_interrupt(int irq, void *dev_id)  
{  
    kill_fasync(&async, SIGIO, POLL_IN);  //向打开设备文件的进程发出SIGIO信号  
    return (IRQ_HANDLED);  
}  
static int protector_fasync(int fd, struct file *filp, int mode)
{
	printk("application  fasync!\n");  
	return fasync_helper(fd, filp, mode, &async);
}
static int protector_open(struct inode *inode, struct file *filp)
{
	int ret = 0;
	
	ret = gpio_request(DEVICE_IRQ_PIN, "protecter");
	if(ret)
	{
		printk(KERN_WARNING "Protector: Can't obtain gpio!\n");
		return -1;
	}
	gpio_direction_input(DEVICE_IRQ_PIN);
	irq = gpio_to_irq(DEVICE_IRQ_PIN);
	if((ret=request_irq(irq, Protector_interrupt, IRQF_TRIGGER_FALLING, "protector", NULL))<0)
	{
		printk(KERN_WARNING "Protector: Can't alloc irq!\n");
		return -1;
	}
	return 0;	
}
static int protector_close(struct inode *inode, struct file *filp)
{
	disable_irq(irq);
	free_irq(irq, NULL);
	return protector_fasync(-1, filp, 0);
}
static struct file_operations protector_fops = {
	.open = protector_open,
	.release = protector_close,
	.fasync = protector_fasync,
};
static struct class *protecter_class;

static int PowerOffProtecter_probe (struct platform_device *protecter)
{
	int status = 0;
	struct cdev *protector_cdev;
	struct device_node *np = protecter->dev.of_node;
	
	printk("IAM IN.....\n");
	DEVICE_IRQ_PIN = of_get_named_gpio(np, "irq_pin", 0);
	if(-ENOENT==DEVICE_IRQ_PIN)
	{
		printk("Protecter: can't get gpio!\n");
		return -1;
	}
	if(major){
		dev = MKDEV(major, minor);
		status = register_chrdev_region(dev, 1, "protecter");
	}
	else{
		status = alloc_chrdev_region(&dev, minor, 1 , "protecter");
		major = MAJOR(dev);
	}
	if(status<0)
	{
		return status;
	}
	protector_cdev = cdev_alloc();
	if(NULL==protector_cdev){
		printk(KERN_WARNING "Protecter: can't alloc cdev!\n");
		return -1;
	}
	cdev_init(protector_cdev, &protector_fops);
	protector_cdev->ops = &protector_fops;
	status = cdev_add(protector_cdev, dev, 1);
	if(status){
		printk(KERN_NOTICE "Error %d add Protector.\n", status);
		
		return -1;
	}
	protecter_class = class_create(THIS_MODULE, "Protecter");
	if(IS_ERR(protecter_class))
	{
		unregister_chrdev_region(major, 1);
		return PTR_ERR(protecter_class);
	}
	if(minor<2)
	{
		device_create(protecter_class, NULL, MKDEV(major,0), NULL, "protector");
		//status = PTR_ERR_OR_ZERO()
	}
	return 0;
}
static int PowerOffProtecter_remove(struct platform_device *device)
{
	device_destroy(protecter_class, dev);
	class_destroy(protecter_class);
	return 0;
}
static const struct of_device_id PowerOff_Protecter_dt_ids[] = {
	{.compatible = "PowerOff, Protecter"},
	{},
};


struct platform_driver PowerOff_Protecter_driver = {
	.driver = {
		.name = "Protecter",
		.owner = THIS_MODULE,
		.of_match_table = of_match_ptr(PowerOff_Protecter_dt_ids),
	},
	.probe = PowerOffProtecter_probe,
	.remove = PowerOffProtecter_remove,
};
static int __init PowerOff_Protecter_init(void)
{
	int status;
	status = platform_driver_register(&PowerOff_Protecter_driver);
	return status;
}
static void __exit PowerOff_Protecter_exit(void)
{
	platform_driver_unregister(&PowerOff_Protecter_driver);	
}
module_init(PowerOff_Protecter_init);
module_exit(PowerOff_Protecter_exit);
MODULE_DESCRIPTION("PowerOff Protecter Driver");
MODULE_AUTHOR("taoxianchong@gooagoo.com");
MODULE_LICENSE("GPL");
