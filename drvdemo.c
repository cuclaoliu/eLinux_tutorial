/*******drivedemo.c*****/
#ifndef __KERNEL__
#define __KERNEL__
#endif
#ifndef MODULE
#define MODULE
#endif
/***********头文件******************************/
#include <linux/mm.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h> /* printk() */
#include <linux/fs.h>     /* file operations */
#include <linux/errno.h>  /* error codes */
#include <linux/types.h>  /* size_t */
#include <linux/proc_fs.h>
#include <linux/fcntl.h> /* O_ACCMODE */
#include <linux/uaccess.h>

/***********定义常量、变量、函数******************/
#define DEVICE_NAME "drvdemo"
#define drvdemo_MAJOR 267
#define drvdemo_MINOR 0
#define MAX_BUF_LEN 1024
static char drv_buf[MAX_BUF_LEN];
static int WRI_LENGTH = 0;

static ssize_t demo_write(struct file *, const char *, size_t, loff_t *);
static ssize_t demo_read(struct file *, char *, size_t, loff_t *);
static int demo_open(struct inode *, struct file *);
static void reverse_buffer(void);
//static long demo_ioctl(struct file *, unsigned int, unsigned long);
/***********定义驱动接口结构体***********************/
static struct file_operations demo_fops = {
        owner : THIS_MODULE,
        open : demo_open,
        read : demo_read,
        write : demo_write,
        //llseek:demo_llseek(),
        //ioctl:demo_ioctl,
        //release:demo_release,
    };

/***********子函数**************************************/
/*******将缓冲区内容逆序****/
static void reverse_buffer(void){
    int i;
    char tmp;
    for (i=0; i < (WRI_LENGTH >> 1); i++) {
        tmp = drv_buf[WRI_LENGTH - 1-i];
        drv_buf[WRI_LENGTH - 1-i] = drv_buf[i];
        drv_buf[i] = tmp;
    }
    printk("buffer content reversed.\n");
}
/***********demo_open()函数**************/
static int demo_open(struct inode* node, struct file *filp){
    printk("device opened.\n");
    printk("driver buffer address: %p.\n", drv_buf);
    return 0;
}
/***********demo_read()函数***********************/
static ssize_t demo_read(struct file *filp, char *buffer, size_t count, loff_t *ppos){
    if (count > WRI_LENGTH)
        count = WRI_LENGTH;
    copy_to_user(buffer, drv_buf, count);
    printk("user read data from driver\n");
    return count;
}
/**********demo_write()函数***********************/
static ssize_t demo_write(struct file *filp, const char *buffer, size_t count, loff_t *ppos){
    if (count > MAX_BUF_LEN)
        count = MAX_BUF_LEN;
    WRI_LENGTH = count;
    copy_from_user(drv_buf , buffer, count);
    printk("user write data to driver\n");
    reverse_buffer();
    return count;
}
/**********demo_init()函数***********************/
static int __init demo_init(void){
    int result;
    result = register_chrdev(drvdemo_MAJOR, "drvdemo", &demo_fops);
    if (result < 0) {
        printk(DEVICE_NAME " initialized failure\n");
        return result;
    } else {
        printk(DEVICE_NAME " initialized\n");
        return 0;
    }
}
/**********demo_exit()函数***********************/
static void __exit demo_exit(void){
    unregister_chrdev(drvdemo_MAJOR, "drvdemo");
    // kfree(demo_devices);
    printk(DEVICE_NAME " unloaded\n");
}
/*********demo_ioctl***********
static long demo_ioctl(struct file *filep, unsigned int cmd, unsigned long arg){
    switch(cmd){
        case REG0_W:
            if (1 == (int)arg){
                reverse();
                printk("reverse driver buffer\n");
            }
        default:
            printk("do nothing\n");
    }
    return 0;
}*/
/**********系统调用模块接口***********************/
module_init(demo_init);
module_exit(demo_exit);
/******* *******/
MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("Laoliu@CUC");
MODULE_DESCRIPTION("char driver demo");
