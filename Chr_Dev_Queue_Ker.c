#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/module.h>
#include<linux/kdev_t.h>
#include<linux/fs.h>
#include<linux/cdev.h>
#include<linux/device.h>
#include<linux/slab.h>
#include<linux/uaccess.h>
#include<linux/ioctl.h>
#include<linux/string.h>

#define WR_VALUE _IOW('a','a',int32_t*)
#define RD_VALUE _IOR('a','b',int32_t*)

dev_t dev = 0;
static struct class *dev_class;
static struct cdev  Chr_Dev_cdev;

static int32_t  cnt,l;
static  char c,q[20][255],*ptr;
static int front=-1,rear=-1;

static int      __init Chr_Dev_init(void);
static void     __exit Chr_Dev_exit(void);
static int      Chr_Dev_open(struct inode *inode, struct file *file);
static int      Chr_Dev_release(struct inode *inode, struct file *file);
static ssize_t  Chr_Dev_read(struct file *filp, char __user *buf, size_t len,loff_t * off);
static ssize_t  Chr_Dev_write(struct file *filp, const char *buf, size_t len, loff_t * off);
static long     Chr_Dev_ioctl(struct file *file, unsigned int cmd, unsigned long arg);

static struct file_operations fops =
{
        .owner          = THIS_MODULE,
        .read           = Chr_Dev_read,
        .write          = Chr_Dev_write,
        .open           = Chr_Dev_open,
        .unlocked_ioctl = Chr_Dev_ioctl,
        .release        = Chr_Dev_release,
};

static int Chr_Dev_open(struct inode *inode, struct file *file)
{
        printk(KERN_INFO "Device File Opened...!!!\n");
        return 0;
}

static int Chr_Dev_release(struct inode *inode, struct file *file)
{
        printk(KERN_INFO "Device File Closed...!!!\n");
        return 0;
}

static ssize_t Chr_Dev_read(struct file *filp, char __user *buf, size_t len, loff_t *off)
{      if(cnt>0)
        {cnt--;
        l=strlen((q[front]));// first element length        
            if(len>l)  len=l;
        ptr=(q[front++]);//ptr to frst element
            if(front==rear+1)  rear=front=-1;
        copy_to_user(buf,ptr,l);//passing ptr value frm kernel space to user space
        return l;}
        else{
                len=1; copy_to_user(buf," ",len);
                return len;
}}

static ssize_t Chr_Dev_write(struct file *filp, const char __user *buf, size_t len, loff_t *off)
    {
   if(copy_from_user(&c,buf+len-1,1)!=0) return -EFAULT;
   else{
           if(front==-1) front=0;
           rear++;
           strcpy(q[rear],buf); //enqueuing the user given string
           cnt++;
           return len; }
       }

static long Chr_Dev_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
         switch(cmd) {
                case RD_VALUE:
                       printk(KERN_INFO "ioctl()\n");
                       copy_to_user((int32_t*) arg, &cnt, sizeof(cnt));
                       break;
                      }
        return 0;
}

static int __init Chr_Dev_init(void)
{
        if((alloc_chrdev_region(&dev, 0, 1, "Chr_Dev_Dev")) <0){
                printk(KERN_INFO "Cannot allocate major number\n");
                return -1;
        }
        printk(KERN_INFO "Major = %d Minor = %d \n",MAJOR(dev), MINOR(dev));

        cdev_init(&Chr_Dev_cdev,&fops);

        if((cdev_add(&Chr_Dev_cdev,dev,1)) < 0){
            printk(KERN_INFO "Cannot add the device to the system\n");
            goto r_class;
        }
                       
        if((dev_class = class_create(THIS_MODULE,"Chr_Dev_class")) == NULL){
            printk(KERN_INFO "Cannot create the struct class\n");
            goto r_class;
        }

        if((device_create(dev_class,NULL,dev,NULL,"Chr_Dev_device")) == NULL){
            printk(KERN_INFO "Cannot create the Device 1\n");
            goto r_device;
        }
        printk(KERN_INFO "Device Driver Insert...Done!!!\n");
        return 0;

r_device:
        class_destroy(dev_class);
r_class:
        unregister_chrdev_region(dev,1);
        return -1;
}

static void __exit Chr_Dev_exit(void)
{
        device_destroy(dev_class,dev);
        class_destroy(dev_class);
        cdev_del(&Chr_Dev_cdev);
        unregister_chrdev_region(dev, 1);
        printk(KERN_INFO "Device Driver Remove...Done!!!\n");
}

module_init(Chr_Dev_init);
module_exit(Chr_Dev_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("SUN2208");
MODULE_DESCRIPTION("CHRDEVPRG-QUEUE");
MODULE_VERSION("1.0");
