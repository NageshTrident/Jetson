/*                                                                            
 *                                                                            
 *      pixcitty.c.tmp     External       31 MAY 2018        
 *                                                                            
 *      Copyright (C)  2009-2018  EPIX, Inc.  Released under GPL. 
 *                                                                            
 *      Frame Grabber Driver: LINUX tty driver access to Camera Link serial   
 *                                                                            
 *      THIS IS A MACHINE GENERATED COPY                                      
 */                                                                           
#define PIXCI
#define P_AARCH64
#include <linux/version.h>
#include <linux/module.h>
#include <linux/param.h>
#include <linux/sched.h>
#include <linux/kernel.h>
#include <linux/pci.h>
#include <linux/delay.h>
#include <linux/slab.h>
#include <asm/uaccess.h>
#include <asm/current.h>
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,0))
  #include <asm/irq.h>
  #include <linux/interrupt.h>
  #if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,32))
    #include <linux/irqreturn.h>
  #endif
  #if !defined(UTS_RELEASE)
    #if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,35))
      #include <generated/utsrelease.h>
    #else
      #include <linux/utsrelease.h>
    #endif
  #endif
#else
  EXPORT_NO_SYMBOLS;
#endif
#if defined(PIXCI)
  #include "pixcipub.h"
  MODULE_AUTHOR("EPIX, Inc.");
  MODULE_DESCRIPTION("PIXCI(R) 32 Bit TTY Driver" ". " "3.8.01" " " "[18.05.31.180500]" ". ");
  MODULE_SUPPORTED_DEVICE("PIXCI(R) Frame Grabbers");
  #if defined(MODULE_LICENSE)
    MODULE_LICENSE("GPL");
  #endif
#endif
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,0))
    #include <linux/tty.h>
    #include <linux/tty_driver.h>
    #include <linux/tty_flip.h>
    #include <asm/termios.h>
    static struct tty_driver *pxdrv_tty = NULL;
    static int pxdrv_ntty = 0;
#endif
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3,8,0))
    static struct tty_port *pxdrv_ttyport = NULL;
#endif
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,31))
static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=""PIXCI(R)";
#elif (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,0))
static const char __module_depends[]
__attribute_used__
__attribute__((section(".modinfo"))) =
"depends=""PIXCI(R)";
#endif
static void
pxdrvtty_insertflipchar(void *atty, int c, int brkflg, int overrunflg)
{
    struct tty_struct *tty = (struct tty_struct *)atty;
    if (!tty)
 return;
    #if defined(TTY_DRIVER_NO_DEVFS)
 if (tty->flip.count >= TTY_FLIPBUF_SIZE)
     tty_flip_buffer_push(tty);
 tty_insert_flip_char(tty, c, (brkflg?TTY_BREAK:0) | (overrunflg?TTY_OVERRUN:0) | ((!brkflg&!overrunflg)?TTY_NORMAL:0));
 tty_flip_buffer_push(tty);
    #elif (LINUX_VERSION_CODE < KERNEL_VERSION(3,10,0))
 tty_buffer_request_room(tty, 1);
 tty_insert_flip_char(tty, c, (brkflg?TTY_BREAK:0) | (overrunflg?TTY_OVERRUN:0) | ((!brkflg&!overrunflg)?TTY_NORMAL:0));
 tty_flip_buffer_push(tty);
    #else
 tty_buffer_request_room(tty->port, 1);
 tty_insert_flip_char(tty->port, c, (brkflg?TTY_BREAK:0) | (overrunflg?TTY_OVERRUN:0) | ((!brkflg&!overrunflg)?TTY_NORMAL:0));
 tty_flip_buffer_push(tty->port);
    #endif
}
static void __exit cleanup_pxdrvttylnx(void)
{
    int i;
    pxdrv_ttycleanup(pxdrvtty_insertflipchar);
    #if (LINUX_VERSION_CODE >= KERNEL_VERSION(3,8,0))
 if (pxdrv_ttyport) {
     for (i = 0; i < pxdrv_ntty; i++) {
  tty_port_destroy(&pxdrv_ttyport[i]);
     }
     kfree(pxdrv_ttyport);
     pxdrv_ttyport = NULL;
 }
    #else
 if (pxdrv_tty != NULL) {
     for (i = 0; i < pxdrv_ntty; i++)
  tty_unregister_device(pxdrv_tty, i);
 }
    #endif
    if (pxdrv_tty != NULL) {
 tty_unregister_driver(pxdrv_tty);
 put_tty_driver(pxdrv_tty);
 pxdrv_tty = NULL;
    }
    printk(KERN_INFO "pixci_tty" ": cleanup\n");
}
static struct tty_operations pxdrvtty_ops;
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3,8,0))
static struct tty_port_operations pxdrvtty_port_ops = {};
#endif
static int __init init_pxdrvttylnx(void)
{
    int r, major = 0, i;
    int ntty = 0;
    pxdrv_ntty = 0;
    pxdrv_ttyinit(&ntty, &major, pxdrvtty_insertflipchar);
    if (ntty <= 0)
 return(-ENODEV);
    #if (LINUX_VERSION_CODE < KERNEL_VERSION(3,8,0))
 pxdrv_tty = alloc_tty_driver(ntty);
 if (!pxdrv_tty)
     return(-ENOMEM);
    #else
 pxdrv_tty = tty_alloc_driver(ntty, TTY_DRIVER_REAL_RAW|TTY_EXCLUSIVE);
 if (IS_ERR(pxdrv_tty))
     return(PTR_ERR(pxdrv_tty));
    #endif
    pxdrv_tty->owner = THIS_MODULE;
    pxdrv_tty->driver_name = "ttyPIXCI";
    pxdrv_tty->name = "ttyPIXCI";
    pxdrv_tty->name_base = 0;
    pxdrv_tty->major = major;
    pxdrv_tty->minor_start = 256+1;
    pxdrv_tty->type = TTY_DRIVER_TYPE_SERIAL;
    pxdrv_tty->subtype = SERIAL_TYPE_NORMAL;
    #if defined(TTY_DRIVER_NO_DEVFS)
 pxdrv_tty->devfs_name = "tts/ttyPIXCI%d";
 pxdrv_tty->flags = TTY_DRIVER_REAL_RAW
    | TTY_DRIVER_NO_DEVFS
    | TTY_EXCLUSIVE;
    #elif (LINUX_VERSION_CODE < KERNEL_VERSION(3,8,0))
 pxdrv_tty->flags = TTY_DRIVER_REAL_RAW
     | TTY_DRIVER_DYNAMIC_DEV
     | TTY_EXCLUSIVE;
    #else
    #endif
    pxdrv_tty->init_termios = tty_std_termios;
    pxdrv_tty->init_termios.c_iflag = 0;
    pxdrv_tty->init_termios.c_oflag = 0;
    pxdrv_tty->init_termios.c_cflag = B9600 | CS8 | CREAD | HUPCL | CLOCAL;
    pxdrv_tty->init_termios.c_lflag = 0;
    tty_set_operations(pxdrv_tty, &pxdrvtty_ops);
    #if (LINUX_VERSION_CODE >= KERNEL_VERSION(3,8,0))
 pxdrv_ttyport = (struct tty_port*)kmalloc(sizeof(struct tty_port)*ntty, GFP_KERNEL);
 if (!pxdrv_ttyport) {
     r = -ENOMEM;
     goto err1;
 }
 memset(pxdrv_ttyport, 0, sizeof(struct tty_port)*ntty);
 for (i = 0; i < ntty; i++) {
     tty_port_init(&pxdrv_ttyport[i]);
     pxdrv_ttyport[i].ops = &pxdrvtty_port_ops;
     tty_port_link_device(&pxdrv_ttyport[i], pxdrv_tty, i);
 }
    #endif
    r = tty_register_driver(pxdrv_tty);
    if (r) {
 goto err2;
    }
    #if (LINUX_VERSION_CODE < KERNEL_VERSION(3,8,0))
 for (i = 0; i < ntty; i++)
     tty_register_device(pxdrv_tty, i, NULL);
    #endif
    pxdrv_ntty = ntty;
    if (1) {
 printk(KERN_INFO "pixci_tty" ": " "PIXCI(R) 64 Bit Driver" " V" "3.8.01" " " "[18.05.31.180500]" " [" UTS_RELEASE "]" " initialization\n");
    }
    return(0);
err2:
    #if (LINUX_VERSION_CODE >= KERNEL_VERSION(3,8,0))
 if (pxdrv_ttyport) {
     for (i = 0; i < ntty; i++)
  tty_port_destroy(&pxdrv_ttyport[i]);
     kfree(pxdrv_ttyport);
     pxdrv_ttyport = NULL;
 }
    #endif
err1:
    if (pxdrv_tty) {
 put_tty_driver(pxdrv_tty);
 pxdrv_tty = NULL;
    }
    return(r);
}
module_init(init_pxdrvttylnx);
module_exit(cleanup_pxdrvttylnx);
static int pxdrvtty_open(struct tty_struct *tty, struct file *filp) {
    int r;
    if (tty->index < 0 || tty->index >= pxdrv_ntty)
 return(-EINVAL);
    #if (LINUX_VERSION_CODE >= KERNEL_VERSION(3,8,0))
 r = tty_port_open(&pxdrv_ttyport[tty->index], tty, filp);
 if (r)
     return(r);
    #endif
    tty->driver_data = pxdrv_ttyopen(tty, tty->index);
    if (!tty->driver_data) {
 #if (LINUX_VERSION_CODE >= KERNEL_VERSION(3,8,0))
     tty_port_close(&pxdrv_ttyport[tty->index], tty, filp);
 #endif
 return(-ENODEV);
    }
    #if (LINUX_VERSION_CODE < KERNEL_VERSION(3,10,0))
 tty->low_latency = 1;
    #endif
    return(0);
}
static void pxdrvtty_close(struct tty_struct *tty, struct file *filp) {
    void *driver_data = tty->driver_data;
    if (!driver_data)
 return;
    if (tty->index < 0 || tty->index > pxdrv_ntty)
 return;
    pxdrv_ttyclose(tty, driver_data);
    #if (LINUX_VERSION_CODE < KERNEL_VERSION(3,8,0))
 tty->closing = 1;
 tty->flags |= TTY_IO_ERROR;
    #else
 tty_port_close(&pxdrv_ttyport[tty->index], tty, filp);
    #endif
}
static int pxdrvtty_write(struct tty_struct *tty, const unsigned char *buffer, int count) {
    void *driver_data = tty->driver_data;
    int i;
    if (!driver_data)
 return(0);
    i = pxdrv_ttywrite(tty, driver_data, buffer, count);
    return(i>=0? i: 0);
}
static int pxdrvtty_writeroom(struct tty_struct *tty) {
    void *driver_data = tty->driver_data;
    int i;
    if (!driver_data)
 return(0);
    i = pxdrv_ttywriteroom(tty, driver_data);
    return(i>=0? i: 0);
}
static int pxdrvtty_charsinbuffer(struct tty_struct *tty) {
    void *driver_data = tty->driver_data;
    int i;
    if (!driver_data)
 return(0);
    i = pxdrv_ttycharsinbuffer(tty, driver_data);
    return(i>=0? i: 0);
}
#if (LINUX_VERSION_CODE < KERNEL_VERSION(3,0,0))
static void pxdrvtty_settermios(struct tty_struct *tty, struct termios *old)
#else
static void pxdrvtty_settermios(struct tty_struct *tty, struct ktermios *old)
#endif
{
    void *driver_data = tty->driver_data;
    if (!driver_data)
 return;
    {
 int nbaud = pxdrv_ttysettermios(tty, driver_data, tty_get_baud_rate(tty));
 int cbaud = 0;
 switch (nbaud) {
   case 9600: cbaud = B9600; break;
   case 19200: cbaud = B19200; break;
   case 38400: cbaud = B38400; break;
   case 57600: cbaud = B57600; break;
   case 115200: cbaud = B115200; break;
   case 230400: cbaud = B230400; break;
   case 460800: cbaud = B460800; break;
   case 921600: cbaud = B921600; break;
   #if (LINUX_VERSION_CODE < KERNEL_VERSION(3,8,0))
     default: cbaud = tty->termios->c_cflag & CBAUD; break;
  #else
     default: cbaud = tty->termios.c_cflag & CBAUD; break;
   #endif
 }
 #if (LINUX_VERSION_CODE < KERNEL_VERSION(3,7,0))
     if (cbaud)
  tty->termios->c_cflag = cbaud | CS8 | CREAD | HUPCL | CLOCAL;
 #elif (LINUX_VERSION_CODE < KERNEL_VERSION(3,8,0))
     mutex_lock(&tty->termios_mutex);
     if (cbaud)
  tty->termios.c_cflag = cbaud | CS8 | CREAD | HUPCL | CLOCAL;
     mutex_unlock(&tty->termios_mutex);
 #else
     if (cbaud)
  tty->termios.c_cflag = cbaud | CS8 | CREAD | HUPCL | CLOCAL;
 #endif
    }
}
static void pxdrvtty_flushbuffer(struct tty_struct *tty)
{
    void *driver_data = tty->driver_data;
    if (!driver_data)
 return;
    pxdrv_ttyflushbuffer(tty, driver_data);
}
static struct tty_operations pxdrvtty_ops = {
    .open = pxdrvtty_open,
    .close = pxdrvtty_close,
    .write = pxdrvtty_write,
    .write_room = pxdrvtty_writeroom,
    .chars_in_buffer = pxdrvtty_charsinbuffer,
    .set_termios = pxdrvtty_settermios,
    .flush_buffer = pxdrvtty_flushbuffer,
};
