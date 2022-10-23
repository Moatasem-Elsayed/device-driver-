#include <linux/module.h>
#include <linux/init.h>
#include <linux/moduleparam.h>
#include <linux/fs.h> //for registration as a file
#include <linux/cdev.h>
#include <linux/gpio.h>
#include <linux/pwm.h>
#include <linux/kernel.h>
#include <linux/string.h>
/* Meta Information */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Moatasem ");
MODULE_DESCRIPTION("A hello world Psuedo device driver");
dev_t device_number;
struct cdev st_characterDevice;
struct class *myclass;
struct device *mydevice;

#define SIZE 15
#define SIZE_READ 3
#define LED_PIN 2
#define TOUCH_PIN 3
static unsigned char buffer[SIZE] = "";

/********PWM ***********/
struct pwm_device *pwm0 = NULL;
u32 pwm_on_time = 500000000;
/********************/
ssize_t driver_write(struct file *file, const char __user *user_buff, size_t count, loff_t *offs)
{
    int not_copied;
    long value = 0;
    /* Get amount of data to copy */
    printk("%s: the count to write %d \n", __func__, count);
    printk("%s: the offs %lld \n", __func__, *offs);
    /***********************************************************/
    if (count + *offs > SIZE)
    {
        count = SIZE - *offs;
    }
    if (!count)
    {
        printk("no space left");
        return -1;
    }
    not_copied = copy_from_user(&buffer[*offs], user_buff, count);
    if (kstrtol_from_user(user_buff, count - 1, 10, &value) == 0) // takes with ms
    {
        printk("driver_write - the value is %ld\n", value);
    }
    else
    {
        printk("faild- the value is %ld  length %d  buff %s\n", value, strlen(user_buff), user_buff);
    }
    /* Set PWM on time */
    if (value > 1000) //
        printk("Invalid Value\n");
    else
        pwm_config(pwm0, (int)value * 1000000, 1000000000);
    if (not_copied)
    {
        return -1;
    }
    *offs = count;
    /***********************************************************/
    printk("%s: already done  %d \n", __func__, count);
    printk("%s: message: %s \n", __func__, buffer);
    return count;
}

ssize_t driver_read(struct file *file, char __user *user_buffer, size_t count, loff_t *offs)
{
    int not_copied;
    char tmp[SIZE_READ] = "";
    /* Get amount of data to copy */
    printk("%s: the count to read %d \n", __func__, count);
    printk("%s: the offs %lld \n", __func__, *offs);
    /***********************************************************/
    if (count + *offs > SIZE_READ)
    {
        count = SIZE_READ - *offs;
    }

    not_copied = copy_to_user(user_buffer, &tmp[*offs], count);
    if (not_copied)
    {
        return -1;
    }
    *offs = count;
    /***********************************************************/
    printk("%s: not copied %d \n", __func__, not_copied);
    printk("%s: message: %s \n", __func__, user_buffer);
    return count;
}

static int driver_open(struct inode *device_file, struct file *instance)
{
    printk("%s dev_nr - open was called!\n", __FUNCTION__);
    return 0;
}
static int driver_close(struct inode *device_file, struct file *instance)
{
    printk("dev_nr - close was called!\n");
    return 0;
}
struct file_operations fops =
    {
        .owner = THIS_MODULE,
        .open = driver_open,
        .release = driver_close,
        .read = driver_read,
        .write = driver_write

};
static int __init hellodriver_init(void)
{
    int retval;

    printk("hello kernel\n");
    // 1- allocate device number
    //  register_chrdev(major_number, "mytest_driver", &fops);
    retval = alloc_chrdev_region(&device_number, 0, 1, "test_devicenumber");
    if (retval == 0)
    {
        printk("%s retval=0 - registered Device number Major: %d, Minor: %d\n", __FUNCTION__, MAJOR(device_number), MINOR(device_number)); //(unsigned int) ((device_number) >> 20))
    }
    else
    {
        printk("could not register device number");
        return -1;
    }
    // 2- define driver character or block or network
    cdev_init(&st_characterDevice, &fops);
    retval = cdev_add(&st_characterDevice, device_number, 1);
    if (retval != 0)
    {
        printk("Registering of device to kernel failed!\n");
        goto CHARACTER_ERROR;
    }
    // 3- generate file ( class -device)
    if ((myclass = class_create(THIS_MODULE, "test_class")) == NULL)
    {
        printk("Device class can not be created!\n");
        goto CLASS_ERROR;
    }
    mydevice = device_create(myclass, NULL, device_number, NULL, "test_file");
    if (mydevice == NULL)
    {
        printk("Device class can not be created!\n");
        goto DEVICE_ERROR;
    }
    printk("device driver is created \n");
    pwm0 = pwm_request(0, "thepwm");
    if (pwm0 == NULL)
    {
        printk("Could not get PWM0!\n");
        goto GPIO_REQUEST_ERROR;
    }

    pwm_config(pwm0, pwm_on_time, 1000000000); // 1 GHZ pulse every 500MHZ
    pwm_enable(pwm0);
    return 0;

GPIO_REQUEST_ERROR:
    device_destroy(myclass, device_number);
DEVICE_ERROR:
    class_destroy(myclass);
CLASS_ERROR:
    cdev_del(&st_characterDevice);
CHARACTER_ERROR:
    unregister_chrdev_region((device_number), 1);
    return -1;
}

static void __exit hellodriver_exit(void)
{
    pwm_disable(pwm0);
    pwm_free(pwm0);
    cdev_del(&st_characterDevice);
    device_destroy(myclass, device_number);
    class_destroy(myclass);
    unregister_chrdev_region((device_number), 1);

    printk("good bye\n");
    printk("good bye\n");
}
module_init(hellodriver_init);
module_exit(hellodriver_exit);
