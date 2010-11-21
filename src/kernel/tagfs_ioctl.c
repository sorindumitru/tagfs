#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <asm/uaccess.h>

#include "../include/tagfs_ioctl.h"
#define TAGFS_MAJOR	42

MODULE_DESCRIPTION("Tagfs test hash file");
MODULE_AUTHOR("Turtles");
MODULE_LICENSE("GPL");

struct tagfs_data {
	struct cdev tagfs_cdev;
}tagfs_cdev;

static int tagfs_open(struct inode *inode, struct file *file)
{
	struct tagfs_data *data = container_of(inode->i_cdev, struct tagfs_data,tagfs_cdev);
	file->private_data = data;
 	
	return 0;
}

static int tagfs_release (struct inode *inode, struct file *file)
{
	return 0;
}

static ssize_t  tagfs_read(struct file *file, char __user *buffer, size_t size, loff_t *offset)
{
	return 0;
}

static int tagfs_write(struct file *file, const char __user *buffer, size_t size, loff_t *offset)
{
	return 0;
}

/*
 * @ioc_data where to copy
 * @from userspace pointer
 * @what data to copy from userspace
 */
inline int copy_data_from_user( long unsigned int from ,struct ioctl_data *ioc_data, unsigned what)
{
    if ( copy_from_user(ioc_data,(struct ioctl_data*)from, DATA_SIZE ) )
        return -EFAULT;

    switch (what) {

        case (ALL):
            if ( copy_from_user(ioc_data->file_name,((struct ioctl_data*)from)->file_name,ioc_data->s_filename) )
                return -EFAULT;
            if ( copy_from_user(ioc_data->tags,((struct ioctl_data*)from)->tags,ioc_data->s_tags) )
                return -EFAULT;
            break;
        case (TAGS):
            if ( copy_from_user(ioc_data->tags,((struct ioctl_data*)from)->tags,ioc_data->s_tags) )
                return -EFAULT;
            break;
        case (FILENAME):
            if ( copy_from_user(ioc_data->file_name,((struct ioctl_data*)from)->file_name,ioc_data->s_filename) )
                return -EFAULT;
            break;
    }
    return 0;
}

static int tagfs_ioctl(struct inode *inode, struct file *file, unsigned int cmd, unsigned long arg)
{
    
    struct ioctl_data tagfs_ioctl_data;    
	switch(cmd){

		case TAGFS_TOUCH:
            if (copy_data_from_user(arg, &tagfs_ioctl_data, ALL))
                return -EFAULT;
            print_data(&tagfs_ioctl_data,ALL);
            //get tags and add in hash
		break;

		case TAGFS_ADD_TAG:
            if(copy_data_from_user(arg, &tagfs_ioctl_data, ALL))
                return -EFAULT;
            print_data(&tagfs_ioctl_data,ALL);
            //get tags and add in has
		break;
		
		case TAGFS_REMOVE_TAG:
            if (copy_data_from_user(arg, &tagfs_ioctl_data, ALL))
                return -EFAULT;
            print_data(&tagfs_ioctl_data,ALL);

		break;

        case TAGFS_FIND_TAGS:
            if (copy_data_from_user(arg, &tagfs_ioctl_data, TAGS))
                return -EFAULT;
            print_data(&tagfs_ioctl_data,TAGS);
        break;
		
        case TAGFS_RM:
            if (copy_data_from_user(arg, &tagfs_ioctl_data, FILENAME))
                return -EFAULT;
            print_data(&tagfs_ioctl_data,FILENAME);
		break;

        default:
            printk(KERN_ERR "Unsupported ioctl type");
		
	}
	return 0;
}
struct file_operations tagfs_op = {
	.owner = THIS_MODULE,
	.open  = tagfs_open,
	.read  = tagfs_read,
	.write = tagfs_write,
	.release = tagfs_release,
	.ioctl   = tagfs_ioctl
};

int __init_tagfs_ioctl( void )
{
	int error = register_chrdev_region(MKDEV(TAGFS_MAJOR,0),1,"tagfs_ioctl");
	printk(KERN_INFO "Init tagfs_ioctl dev");
	if ( error !=0 )
	{
		printk(KERN_ERR "Error regidtering device");
		return error;
	}
	//init tagfs_cdev 
	cdev_init(&tagfs_cdev.tagfs_cdev,&tagfs_op);
	cdev_add(&tagfs_cdev.tagfs_cdev,MKDEV(TAGFS_MAJOR,0),1);

	return 0;
}

void __exit_tagfs_ioctl( void )
{
	cdev_del(&tagfs_cdev.tagfs_cdev);
	unregister_chrdev_region(MKDEV(TAGFS_MAJOR,0),1);
	printk(KERN_INFO "Exit tagfs_ioctl dev");
}

module_init(__init_tagfs_ioctl);
module_exit(__exit_tagfs_ioctl);
