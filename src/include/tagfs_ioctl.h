#ifndef __TAGFS_DEV_H__
#define __TAGFS_DEV_H__ 1

    #include <asm/ioctl.h>
#ifdef __KERNEL__
    #define PRINT(fmt,args...) printk(KERN_INFO  fmt,##args)
#else
    #define PRINT(fmt, args...) printf(fmt,##args)
#endif

/*
 * @file_name 
 * @tags list of tags separated by /
 * @s_filename filename length
 * @s_tags tags length
 * */
    struct ioctl_data{
        char *file_name;
        char *tags;
        unsigned long s_filename;
        unsigned long s_tags;
    };
    
    #define ALL         1
    #define FILENAME    2
    #define TAGS        3

    #define DATA_SIZE sizeof(struct ioctl_data)

    #define TAGFS_TOUCH _IOC(_IOC_NONE, 'k', 0 ,DATA_SIZE)
    #define TAGFS_ADD_TAG _IOC(_IOC_NONE, 'k', 1 ,DATA_SIZE)
    #define TAGFS_REMOVE_TAG _IOC(_IOC_NONE, 'k', 2 ,DATA_SIZE)
    #define TAGFS_FIND_TAGS _IOC(_IOC_NONE, 'k', 3 ,DATA_SIZE)
    #define TAGFS_RM _IOC(_IOC_NONE, 'k', 4 ,DATA_SIZE)

void print_data(struct ioctl_data *args, unsigned what)
{
    switch(what){
        case (ALL):
            PRINT("filename =%s(size=%ld) tags=%s(size=%ld)\n",args->file_name,args->s_filename,args->tags,args->s_tags);
            break;
        case (TAGS):
            PRINT("tags=%s(size=%ld)\n",args->tags,args->s_tags);
            break;
        case (FILENAME):
            PRINT("filename =%s(size=%ld)\n",args->file_name,args->s_filename);
            break;
    }
}
#endif
