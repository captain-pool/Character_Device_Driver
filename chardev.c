#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/uaccess.h>
#include<linux/fs.h>
#include<linux/unistd.h>
#include<asm/uaccess.h>
#include<linux/device.h>
#include<linux/string.h>
#include<linux/mutex.h>
#define DEVICE_NAME "chrdev01"
#define CLASS "chrdev"
MODULE_AUTHOR("Adrish Dey<rickdey1998@gmail.com>");
MODULE_VERSION("1.0");
MODULE_LICENSE("GPL");
static  DEFINE_MUTEX(dev_mutex);
static int dev_open(struct inode*,struct file*);
static int dev_close(struct inode*,struct file*);
static ssize_t dev_read(struct file*,char*,size_t,loff_t*);
static ssize_t dev_write(struct file*,const char*,size_t,loff_t*);
static struct file_operations fops={
.owner=THIS_MODULE,
.read=dev_read,
.release=dev_close,
.open=dev_open,
.write=dev_write,
};
static int Major,devOC=0;
static struct class* dev_class;
static struct device* dev;
static char msg[256],*msgPtr;
static int __init mod_init(void)
{
Major=register_chrdev(0,DEVICE_NAME,&fops);
if(Major<0)
{
printk(KERN_ALERT"Error Registering Device");
return -EPERM;
}
dev_class=class_create(THIS_MODULE,CLASS);
if(IS_ERR(dev))
{
printk(KERN_ALERT"Error Creating Device Class!");
unregister_chrdev(Major,DEVICE_NAME);
return PTR_ERR(dev);
}
dev=device_create(dev_class,NULL,MKDEV(Major,0),NULL,DEVICE_NAME);
if(IS_ERR(dev))
{
printk(KERN_ALERT"Error Creating Device");
class_destroy(dev_class);
unregister_chrdev(Major,DEVICE_NAME);
return PTR_ERR(dev);
}
mutex_init(&dev_mutex);
return 0;
}
static void __exit mod_exit(void)
{
mutex_destroy(&dev_mutex);
device_destroy(dev_class,MKDEV(Major,0));
class_unregister(dev_class);
class_destroy(dev_class);
unregister_chrdev(Major,DEVICE_NAME);
printk(KERN_INFO"Clean Up Done!");
}
static int dev_open(struct inode* inode,struct file* file)
{
if(!mutex_trylock(&dev_mutex))
{
printk(KERN_ALERT"Device Busy!");
return -EBUSY;
}
devOC++;
sprintf(msg,"Default Txt");
msgPtr=msg;
return 0;
}
static int dev_close(struct inode* inode,struct file* file)
{
devOC--;
printk(KERN_INFO"Device Closed: %d",devOC);
mutex_unlock(&dev_mutex);
return 0;
}
static ssize_t dev_read(struct file* file,char* buff,size_t length,loff_t* offset)
{
int bytes_read=0;
while(*msgPtr&&length--)
{
put_user(*(msgPtr++),buff++);
bytes_read++;
}
return bytes_read++;
}
static ssize_t dev_write(struct file* file,const char*buff,size_t length,loff_t *offset)
{
copy_from_user(msg,buff,length);
printk("Data Recieved: %s",msg);
return length;
}
module_init(mod_init);
module_exit(mod_exit);
