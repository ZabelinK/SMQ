#include <linux/init.h>           // Macros used to mark up functions e.g. __init __exit
#include <linux/module.h>         // Core header for loading LKMs into the kernel
#include <linux/device.h>         // Header to support the kernel Driver Model
#include <linux/kernel.h>         // Contains types, macros, functions for the kernel
#include <linux/fs.h>             // Header for the Linux file system support
#include <linux/uaccess.h>          // Required for the copy to user function


#include "smq_operations_user.h"
#include "smq_operations_fn.h"


typedef int (*smq_operation_fn)(void *);

smq_operation_fn smq_operation_table[] = 
{
    [SMQ_GET_SH_NAME] = smq_get_sh_name_fn,
    [SMQ_ALLOC_MSG] = smq_alloc_msg_fn,
    [SMQ_FREE_MSG] = smq_free_buf_fn,
    [SMQ_SEND_MSG] = smq_send_msg_fn,
    [SMQ_RECV_MSG] = smq_recv_msg_fn,
    [SMQ_CREATE_NEW_GROUP] = smq_create_new_recipients_group_fn,
    [SMQ_SUBSCRIBE_ON_GROUP] = smq_subscribe_on_group_fn,
    [SMQ_UNSUBSCRIBE_FROM_GROUP] = smq_unsubsribe_from_group_fn,
};


#define DEVICE_NAME "SharedMQDev1"
#define  CLASS_NAME "SharedMQ1"        ///< The device class -- this is a character device driver

static char *name = "world";
static int    majorNumber;                  ///< Stores the device number -- determined automatically
static char   message[256] = {0};           ///< Memory for the string that is passed from userspace
static short  size_of_message;              ///< Used to remember the size of the string stored
static int    numberOpens = 0;              ///< Counts the number of times the device is opened
static struct class*  smqClass  = NULL; ///< The device-driver class struct pointer
static struct device* smqDevice = NULL; ///< The device-driver device struct pointer

static int     dev_open(struct inode * p_inode, struct file * p_file)
{
    return 0;
}

static int     dev_release(struct inode * p_inode, struct file * p_file)
{
    return 0;
}

static ssize_t dev_read(struct file * p_file, char * buf, size_t size, loff_t * off)
{
    return 0;
}

static ssize_t dev_write(struct file * p_file, const char * buf, size_t size, loff_t * off)
{
    return 0;
}

static long    dev_ioctl(struct file * p_file, unsigned int op_code, unsigned long param)
{
    enum smq_operations op = op_code;

    smq_operation_table[op_code](&param);

    printk(KERN_INFO "dev_ioctl called");
}

static struct file_operations fops =
{
    .open = dev_open,
    .release = dev_release,
    .read = dev_read,
    .write = dev_write,
    .compat_ioctl = dev_ioctl,
    .unlocked_ioctl = dev_ioctl,
};

//module_param(name, charp, S_IRUGO); 
//MODULE_PARM_DESC(name, "The name to display in /var/log/kern.log"); 

static int __init helloBBB_init(void){
    printk(KERN_EMERG "EBB: Hello %s from the BBB LKM!\n", name);

    majorNumber = register_chrdev(0, DEVICE_NAME, &fops);
    if (majorNumber<0){
      printk(KERN_ALERT "EBBChar failed to register a major number\n");
      return majorNumber;
   }
    printk(KERN_EMERG "smq end init1");
    smqClass = class_create(THIS_MODULE, CLASS_NAME);
    if (IS_ERR(smqClass)){                // Check for error and clean up if there is
      unregister_chrdev(majorNumber, DEVICE_NAME);
      printk(KERN_ALERT "Failed to register device class\n");
      return PTR_ERR(smqClass);          // Correct way to return an error on a pointer
    }
    printk(KERN_EMERG "smq end init2");
    printk(KERN_EMERG "smq end init3");

    smqDevice = device_create(smqClass, NULL, MKDEV(majorNumber, 0), NULL, DEVICE_NAME);
    if (IS_ERR(smqDevice)){               // Clean up if there is an error
      class_destroy(smqClass);           // Repeated code but the alternative is goto statements
      unregister_chrdev(majorNumber, DEVICE_NAME);
      printk(KERN_ALERT "Failed to create the device\n");
      return PTR_ERR(smqDevice);
    }
    printk(KERN_EMERG "smq end ini4");
    printk(KERN_EMERG "smq end init");

    return 0;
}
 

static void __exit helloBBB_exit(void){
    printk(KERN_EMERG "EBB: Goodbye %s from the BBB LKM!\n", name);
    device_destroy(smqClass, MKDEV(majorNumber, 0));     // remove the device
    class_unregister(smqClass);                          // unregister the device class
    class_destroy(smqClass);                             // remove the device class
    unregister_chrdev(majorNumber, DEVICE_NAME);       
}
 
/** @brief A module must use the module_init() module_exit() macros from linux/init.h, which
 *  identify the initialization function at insertion time and the cleanup function (as
 *  listed above)
 */
module_init(helloBBB_init);
module_exit(helloBBB_exit);
MODULE_LICENSE("GPL");         
MODULE_AUTHOR("Zabelin Kirill");      
MODULE_DESCRIPTION("Kernel SharedMQ");
MODULE_VERSION("0.1");              

