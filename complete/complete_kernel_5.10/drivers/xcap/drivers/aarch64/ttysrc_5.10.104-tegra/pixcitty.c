/**                                                                               
 *                                                                                
 *      pixcitty.c         External       2-MAY-2023         
 *                                                                                
 *      Copyright (C)  2009-2023  EPIX, Inc.  Released under GPL.
 *                                                                                
 *      Frame Grabber Driver: LINUX tty driver access to Camera Link serial   
 *                                                                               
 *      THIS IS A MACHINE GENERATED COPY                                         
 **/                                                                             
#define PIXCI
#define PIXCI_O_SHIPPED
#define P_AARCH64
#include <linux/version.h>
#include <linux/module.h>
#include <linux/param.h>
#include <linux/sched.h>
#include <linux/kernel.h>
#include <linux/pci.h>
#include <linux/delay.h>
#include <linux/slab.h>
#include <linux/bitmap.h>
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
  #define XCDRV_IDTTYN "PIXCI(R)" " TTY Driver"
  MODULE_AUTHOR("EPIX, Inc.");
  MODULE_DESCRIPTION(XCDRV_IDTTYN);
  #if defined(MODULE_SUPPORTED_DEVICE)
    MODULE_SUPPORTED_DEVICE("PIXCI(R) Frame Grabbers");
  #endif
  MODULE_VERSION("3.8.01" "-" "23.05.02.134412");
  /**
   * At most, this is interpreted as labeling this file as GPL, not
   * the driver and/or its binary blob to which it is dynamically linked.
   **/
  #if defined(MODULE_LICENSE)
    MODULE_LICENSE("GPL");
  #endif
  #if LINUX_VERSION_CODE >= KERNEL_VERSION(4,0,0)
    MODULE_SOFTDEP("pre: " "PIXCI(R)");
  #elif (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,31))
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
#endif
#if !defined(true)
    #define true 1
#endif
#if !defined(false)
    #define false 0
#endif
/**
 * Parameters set via insmod.
 *
 * /dev/tty node major number
 * Use 0 for auto generate.
 * Use -1 for same as PIXCI major number,
 * but with (obviously) different minor numbers
 * (option for historic behavior).
 **/
static unsigned tty_major = 0;
module_param(tty_major, int, 0644);
MODULE_PARM_DESC(tty_major, "tty node's major number, 0 for autogenerate, -1 for frame grabber's");
/**
 * The default baud rate.
 * The default default is 9600; the only baud
 * rate supported in early Camera Link specs.
 * Only Camera Link baud rates are supported;
 * invalid values silently ignored and replaced with 9600.
 **/
static int default_baudrate = 9600;
module_param(default_baudrate, int, 0644);
MODULE_PARM_DESC(default_baudrate, "default baud rate");
/**
 * Compile time option.
 *
 * Select /dev node name
 * /dev/ttyPIXCI...
 * vs
 * /dev/ttySPIXCI...
 *
 * Typically, terminal applications only list /dev/ttyS* devices;
 * thus the change from the former to the latter.
 **/
#define DEVNAME_TTYS 1
/**
 *
 **/
#define NEWPROBE 0
#define OLDPROBE 1
#define OLDNOPROBE 0
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,0))
    #include <linux/tty.h>
    #include <linux/tty_driver.h>
    #include <linux/tty_flip.h>
    #include <asm/termios.h>
    static LIST_HEAD(pxdrvtty_devlist);
    struct pxdrvtty_dev
    {
 struct list_head devlist;
 int ntty;
 struct tty_driver *ttydrv;
 unsigned long *ttydyndev;
 #if (LINUX_VERSION_CODE >= KERNEL_VERSION(3,8,0))
     struct tty_port *ttyport;
 #endif
 void *pxtty;
 int pxunit;
 int pxmajor;
    };
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
static void
pxdrvtty_wakeup(void *atty)
{
    struct tty_struct *tty = (struct tty_struct *)atty;
    if (!tty)
 return;
    tty_wakeup(tty);
}
#if (LINUX_VERSION_CODE < KERNEL_VERSION(3,0,0))
static void pxdrvtty_reflectbaud(struct tty_struct *tty, int nbaud)
#else
static void pxdrvtty_reflectbaud(struct tty_struct *tty, int nbaud)
#endif
{
    int cflags = CS8 | CREAD | HUPCL | CLOCAL;
    #if 0 && (LINUX_VERSION_CODE >= KERNEL_VERSION(3,8,0))
 tty_termios_encode_baud_rate(&tty->termios, nbaud, nbaud);
    #else
 int cbaud = 0;
 switch (nbaud) {
   default:
   case 9600: cbaud = B9600; break;
   case 19200: cbaud = B19200; break;
   case 38400: cbaud = B38400; break;
   case 57600: cbaud = B57600; break;
   case 115200: cbaud = B115200; break;
   case 230400: cbaud = B230400; break;
   case 460800: cbaud = B460800; break;
   case 921600: cbaud = B921600; break;
 }
 if (cbaud) {
     #if (LINUX_VERSION_CODE < KERNEL_VERSION(3,7,0))
  tty->termios->c_cflag = cbaud | cflags;
     #elif (LINUX_VERSION_CODE < KERNEL_VERSION(3,8,0))
  mutex_lock(&tty->termios_mutex);
  tty->termios.c_cflag = cbaud | cflags;
  tty->termios->c_ispeed = nbaud;
  tty->termios->c_ospeed = nbaud;
  mutex_unlock(&tty->termios_mutex);
     #else
  tty->termios.c_cflag = cbaud | cflags;
  tty->termios.c_ispeed = nbaud;
  tty->termios.c_ospeed = nbaud;
     #endif
 }
    #endif
}
static int pxdrvtty_open(struct tty_struct *tty, struct file *filp)
{
    int r;
    #if NEWPROBE
 struct pxdrvtty_dev *dev = (struct pxdrvtty_dev*)tty->driver_data;
    #endif
    #if OLDNOPROBE|OLDPROBE
 struct pxdrvtty_dev *dev;
 if (list_empty(&pxdrvtty_devlist))
     return(-ENODEV);
 dev = (struct pxdrvtty_dev *)pxdrvtty_devlist.next;
    #endif
    if (tty->index < 0 || tty->index >= dev->ntty)
 return(-EINVAL);
    #if 0
 #if (LINUX_VERSION_CODE >= KERNEL_VERSION(3,8,0))
     r = tty_port_open(&dev->ttyport[tty->index], tty, filp);
     if (r)
  return(r);
 #endif
 dev->pxtty = pxdrv_ttyopen(tty, tty->index+dev->pxunit, tty_get_baud_rate(tty));
 #if OLDNOPROBE|OLDPROBE
  tty->driver_data = dev->pxtty;
 #endif
 if (!dev->pxtty) {
  #if (LINUX_VERSION_CODE >= KERNEL_VERSION(3,8,0))
      tty_port_close(&dev->ttyport[tty->index], tty, filp);
  #endif
  return(-ENODEV);
 }
 #if (LINUX_VERSION_CODE < KERNEL_VERSION(3,10,0))
  tty->low_latency = 1;
 #endif
 return(0);
    #else
 r = pxdrv_ttyopen(tty, tty->index+dev->pxunit, &dev->pxtty, 0);
 if (r)
     return(r);
 #if OLDNOPROBE|OLDPROBE
     tty->driver_data = dev->pxtty;
 #endif
 #if (LINUX_VERSION_CODE >= KERNEL_VERSION(3,8,0))
     r = tty_port_open(&dev->ttyport[tty->index], tty, filp);
     if (r) {
  pxdrv_ttyclose(tty, dev->pxtty);
  dev->pxtty = NULL;
  #if OLDNOPROBE|OLDPROBE
  tty->driver_data = NULL;
  #endif
  return(r);
     }
 #endif
 #if (LINUX_VERSION_CODE < KERNEL_VERSION(3,10,0))
     tty->low_latency = 1;
 #endif
 if (1) {
     int nbaud = pxdrv_ttysettermios(tty, dev->pxtty, tty_get_baud_rate(tty));
     pxdrvtty_reflectbaud(tty, nbaud);
 }
 return(0);
    #endif
}
static void pxdrvtty_close(struct tty_struct *tty, struct file *filp)
{
    #if NEWPROBE
 struct pxdrvtty_dev *dev = (struct pxdrvtty_dev*)tty->driver_data;
    #endif
    #if OLDNOPROBE|OLDPROBE
 struct pxdrvtty_dev *dev;
 if (list_empty(&pxdrvtty_devlist))
     return;
 dev = (struct pxdrvtty_dev *)pxdrvtty_devlist.next;
    #endif
    if (!dev->pxtty)
 return;
    if (tty->index < 0 || tty->index >= dev->ntty)
 return;
    pxdrv_ttyclose(tty, dev->pxtty);
    dev->pxtty = NULL;
    #if (LINUX_VERSION_CODE < KERNEL_VERSION(3,8,0))
 tty->closing = 1;
 tty->flags |= TTY_IO_ERROR;
    #else
 tty_port_close(&dev->ttyport[tty->index], tty, filp);
    #endif
}
static int pxdrvtty_write(struct tty_struct *tty, const unsigned char *buffer, int count)
{
    #if NEWPROBE
 struct pxdrvtty_dev *dev = (struct pxdrvtty_dev*)tty->driver_data;
 void *pxtty = dev->pxtty;
    #endif
    #if OLDNOPROBE|OLDPROBE
 void *pxtty = tty->driver_data;
    #endif
    if (!pxtty)
 return(0);
    {
 int i = pxdrv_ttywrite(tty, pxtty, buffer, count);
 return(i>=0? i: 0);
    }
}
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(5,14,0))
static uint pxdrvtty_writeroom(struct tty_struct *tty)
#else
static int pxdrvtty_writeroom(struct tty_struct *tty)
#endif
{
    #if NEWPROBE
 struct pxdrvtty_dev *dev = (struct pxdrvtty_dev*)tty->driver_data;
 void *pxtty = dev->pxtty;
    #endif
    #if OLDNOPROBE|OLDPROBE
 void *pxtty = tty->driver_data;
    #endif
    if (!pxtty)
 return(0);
    {
 int i = pxdrv_ttywriteroom(tty, pxtty);
 return(i>=0? i: 0);
    }
}
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(5,14,0))
static uint pxdrvtty_charsinbuffer(struct tty_struct *tty)
#else
static int pxdrvtty_charsinbuffer(struct tty_struct *tty)
#endif
{
    #if NEWPROBE
 struct pxdrvtty_dev *dev = (struct pxdrvtty_dev*)tty->driver_data;
 void *pxtty = dev->pxtty;
    #endif
    #if OLDNOPROBE|OLDPROBE
 void *pxtty = tty->driver_data;
    #endif
    if (!pxtty)
 return(0);
    {
 int i = pxdrv_ttycharsinbuffer(tty, pxtty);
 return(i>=0? i: 0);
    }
}
#if (LINUX_VERSION_CODE < KERNEL_VERSION(3,0,0))
static void pxdrvtty_settermios(struct tty_struct *tty, struct termios *old)
#else
static void pxdrvtty_settermios(struct tty_struct *tty, struct ktermios *old)
#endif
{
    #if NEWPROBE
 struct pxdrvtty_dev *dev = (struct pxdrvtty_dev*)tty->driver_data;
 void *pxtty = dev->pxtty;
    #endif
    #if OLDNOPROBE|OLDPROBE
 void *pxtty = tty->driver_data;
    #endif
    if (!pxtty)
 return;
    {
 int nbaud = pxdrv_ttysettermios(tty, pxtty, tty_get_baud_rate(tty));
 pxdrvtty_reflectbaud(tty, nbaud);
    }
}
static void pxdrvtty_flushbuffer(struct tty_struct *tty)
{
    #if NEWPROBE
 struct pxdrvtty_dev *dev = (struct pxdrvtty_dev*)tty->driver_data;
 void *pxtty = dev->pxtty;
    #endif
    #if OLDNOPROBE|OLDPROBE
 void *pxtty = tty->driver_data;
    #endif
    if (!pxtty)
 return;
    pxdrv_ttyflushbuffer(tty, pxtty);
}
#if 0
static void pxdrvtty_remove(struct tty_driver *self, struct tty_struct *tty)
{
    self->ttys[tty->index] = NULL;
}
static int pxdrvtty_install(struct tty_driver *self, struct tty_struct *tty)
{
    self->ttys[tty->index] = tty;
}
static struct tty_struct *pxdrvtty_lookup(struct tty_driver *self, struct file *filp, int idx)
{
    return(self->ttys[idx];
}
#endif
static struct tty_operations pxdrvtty_ops = {
    .open = pxdrvtty_open,
    .close = pxdrvtty_close,
    .write = pxdrvtty_write,
    .write_room = pxdrvtty_writeroom,
    .chars_in_buffer = pxdrvtty_charsinbuffer,
    .set_termios = pxdrvtty_settermios,
    .flush_buffer = pxdrvtty_flushbuffer,
    #if 0
    .lookup = pxdrvtty_lookup,
    .install = pxdrvtty_install,
    .remove = pxdrvtty_remove,
    #endif
};
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3,8,0))
  static struct tty_port_operations pxdrvtty_port_ops = {};
#endif
static int pxdrvtty_create_instance(int pxmajor, int pxunit, int ntty, int dynamicdev)
{
    struct pxdrvtty_dev *dev;
    int r;
    #if (LINUX_VERSION_CODE >= KERNEL_VERSION(3,8,0))
 int pi;
    #endif
    if (ntty <= 0)
 return(-EINVAL);
    dev = kzalloc(sizeof(*dev), GFP_KERNEL);
    if (!dev)
 return(-ENOMEM);
    dev->ntty = ntty;
    dev->pxunit = pxunit;
    dev->pxmajor = pxmajor;
    #if (LINUX_VERSION_CODE < KERNEL_VERSION(3,8,0))
 dev->ttydrv = alloc_tty_driver(ntty);
    #else
 dev->ttydrv = tty_alloc_driver(ntty, TTY_DRIVER_REAL_RAW);
    #endif
    if (!dev->ttydrv) {
 r = -ENOMEM;
 goto err0;
    }
    dev->ttydrv->owner = THIS_MODULE;
    dev->ttydrv->driver_name = "ttyPIXCI";
    #if !DEVNAME_TTYS
    dev->ttydrv->name = "ttyPIXCI";
    #else
    dev->ttydrv->name = "ttySPIXCI";
    #endif
    dev->ttydrv->name_base = 0;
    dev->ttydrv->major = tty_major==-1? pxmajor: tty_major;
    dev->ttydrv->minor_start = tty_major==-1? 256+1: 0;
    dev->ttydrv->type = TTY_DRIVER_TYPE_SERIAL;
    dev->ttydrv->subtype = SERIAL_TYPE_NORMAL;
    #if defined(TTY_DRIVER_NO_DEVFS)
 dev->ttydrv->devfs_name = "tts/ttyPIXCI%d";
 dev->ttydrv->flags = TTY_DRIVER_REAL_RAW
      | TTY_DRIVER_NO_DEVFS
      | TTY_EXCLUSIVE;
 dynamicdev = false;
    #elif (LINUX_VERSION_CODE < KERNEL_VERSION(3,8,0))
 dev->ttydrv->flags = TTY_DRIVER_REAL_RAW
      | (dynamicdev? TTY_DRIVER_DYNAMIC_DEV: 0)
      | TTY_EXCLUSIVE;
    #else
 dev->ttydrv->flags = (dynamicdev? TTY_DRIVER_DYNAMIC_DEV: 0);
    #endif
    dev->ttydrv->init_termios = tty_std_termios;
    dev->ttydrv->init_termios.c_iflag = 0;
    dev->ttydrv->init_termios.c_oflag = 0;
    dev->ttydrv->init_termios.c_lflag = 0;
    switch (default_baudrate) {
      default:
      case 9600: dev->ttydrv->init_termios.c_cflag = B9600; break;
      case 19200: dev->ttydrv->init_termios.c_cflag = B19200; break;
      case 38400: dev->ttydrv->init_termios.c_cflag = B38400; break;
      case 57600: dev->ttydrv->init_termios.c_cflag = B57600; break;
      case 115200: dev->ttydrv->init_termios.c_cflag = B115200; break;
      case 230400: dev->ttydrv->init_termios.c_cflag = B230400; break;
      case 460800: dev->ttydrv->init_termios.c_cflag = B460800; break;
      case 921600: dev->ttydrv->init_termios.c_cflag = B921600; break;
    }
    dev->ttydrv->init_termios.c_cflag |= CS8 | CREAD | HUPCL | CLOCAL;
    tty_set_operations(dev->ttydrv, &pxdrvtty_ops);
    #if NEWPROBE
    {
    }
    #endif
    #if (LINUX_VERSION_CODE >= KERNEL_VERSION(3,8,0))
 dev->ttyport = (struct tty_port*)kzalloc(sizeof(struct tty_port)*ntty, GFP_KERNEL);
 if (!dev->ttyport) {
     r = -ENOMEM;
     goto err1;
 }
 for (pi = 0; pi < ntty; pi++) {
     tty_port_init(&dev->ttyport[pi]);
     dev->ttyport[pi].ops = &pxdrvtty_port_ops;
     tty_port_link_device(&dev->ttyport[pi], dev->ttydrv, pi);
 }
    #endif
    #if !defined(TTY_DRIVER_NO_DEVFS)
 if (dynamicdev) {
     dev->ttydyndev = kzalloc(sizeof(unsigned long)*BITS_TO_LONGS(ntty), GFP_KERNEL);
     if (!dev->ttydyndev) {
  r = -ENOMEM;
  goto err2;
     }
 }
    #endif
    r = tty_register_driver(dev->ttydrv);
    if (r)
 goto err2;
    #if !defined(TTY_DRIVER_NO_DEVFS)
 #if OLDNOPROBE
     if (dynamicdev) {
  for (i = 0; i < ntty; i++) {
      tty_register_device(dev->ttydrv, i, NULL);
      set_bit(i, dev->ttydyndev);
  }
     }
 #endif
    #endif
    list_add_tail(&dev->devlist, &pxdrvtty_devlist);
    return(0);
err2:
    #if (LINUX_VERSION_CODE >= KERNEL_VERSION(3,8,0))
 if (dev->ttyport) {
     for (pi = 0; pi < ntty; pi++)
  tty_port_destroy(&dev->ttyport[pi]);
     kfree(dev->ttyport);
     dev->ttyport = NULL;
 }
    #endif
err1:
    if (dev->ttydrv) {
 #if (LINUX_VERSION_CODE >= KERNEL_VERSION(5,15,0))
     tty_driver_kref_put(dev->ttydrv);
 #else
     put_tty_driver(dev->ttydrv);
 #endif
 dev->ttydrv = NULL;
    }
err0:
    if (dev) {
 kfree(dev);
    }
    return(r);
}
static void pxdrvtty_remove_instance(struct pxdrvtty_dev *dev, int unregister)
{
    int i;
    #if !defined(TTY_DRIVER_NO_DEVFS)
 if (dev->ttydyndev) {
     for (i = 0; i < dev->ntty; i++)
  if (test_bit(i, dev->ttydyndev))
      tty_unregister_device(dev->ttydrv, i);
     kfree(dev->ttydyndev);
 }
    #endif
    #if (LINUX_VERSION_CODE >= KERNEL_VERSION(3,8,0))
 if (dev->ttyport) {
     for (i = 0; i < dev->ntty; i++)
  tty_port_destroy(&dev->ttyport[i]);
     kfree(dev->ttyport);
 }
    #endif
    if (dev->ttydrv) {
 if (unregister)
     tty_unregister_driver(dev->ttydrv);
 #if (LINUX_VERSION_CODE >= KERNEL_VERSION(5,15,0))
     tty_driver_kref_put(dev->ttydrv);
 #else
     put_tty_driver(dev->ttydrv);
 #endif
    }
    if (dev) {
 kfree(dev);
    }
}
void
pxdrvtty_probe(void *atty, int major, int unit, int ntty)
{
    #if OLDPROBE
    #if !defined(TTY_DRIVER_NO_DEVFS)
 if (!list_empty(&pxdrvtty_devlist)) {
     struct pxdrvtty_dev *dev = (struct pxdrvtty_dev *)pxdrvtty_devlist.next;
     if (dev->ttydyndev) {
  for ( ; ntty--; unit++) {
      if (unit >= 0 && unit < dev->ntty) {
   if (!atty) {
       tty_register_device(dev->ttydrv, unit, NULL);
       set_bit(unit, dev->ttydyndev);
   } else {
       tty_unregister_device(dev->ttydrv, unit);
       clear_bit(unit, dev->ttydyndev);
   }
      }
  }
     }
 }
    #endif
    #endif
    #if NEWPROBE
    {
 int r = 0;
 char *s1 = "instance for unit";
 if (!atty) {
     printk(KERN_INFO "%s create %s %d\n", XCDRV_IDTTYN, s1, unit);
     r = pxdrvtty_create_instance(major, unit, ntty, false);
     if (r < 0)
  printk(KERN_INFO "%s create %s %d failed (%d)\n", XCDRV_IDTTYN, s1, unit, r);
 } else {
     struct pxdrvtty_dev *dev;
 }
    }
    #endif
}
static int __init
init_pxdrvttylnx(void)
{
    #if NEWPROBE|1
 printk(KERN_INFO "%s %s loaded\n", XCDRV_IDTTYN, "3.8.01" " " "[23.05.02.134412]" );
    #else
 printk(KERN_INFO "%s %s loaded\n", XCDRV_IDTTYN, "3.8.01" " " "[23.05.02.134412]" );
    #endif
    #if NEWPROBE
    {
 int r;
 r = pxdrv_ttyinit(NULL, NULL, pxdrvtty_insertflipchar, pxdrvtty_probe, pxdrvtty_wakeup);
 return(r);
    }
    #endif
    #if OLDNOPROBE|OLDPROBE
    {
 int r, major;
 int ntty = 0;
 pxdrv_ttyinit(&ntty, &major, NULL, NULL, NULL);
 if (ntty <= 0)
     return(-ENODEV);
 #if OLDNOPROBE
     r = pxdrvtty_create_instance(major, 0, ntty, false);
     if (r >= 0)
  pxdrv_ttyinit(NULL, NULL, pxdrvtty_insertflipchar, NULL, pxdrvtty_wakeup);
 #else
     r = pxdrvtty_create_instance(major, 0, ntty, true);
     if (r >= 0)
  pxdrv_ttyinit(NULL, NULL, pxdrvtty_insertflipchar, pxdrvtty_probe, pxdrvtty_wakeup);
 #endif
 return(r);
    }
    #endif
}
static void __exit
cleanup_pxdrvttylnx(void)
{
    struct pxdrvtty_dev *dev;
    struct list_head *list;
    pxdrv_ttycleanup(pxdrvtty_insertflipchar);
    while (!list_empty(&pxdrvtty_devlist)) {
 list = pxdrvtty_devlist.next;
 list_del(list);
 dev = list_entry(list, struct pxdrvtty_dev, devlist);
 pxdrvtty_remove_instance(dev, true);
    }
    #if NEWPROBE|1
 printk(KERN_INFO "%s removed\n", XCDRV_IDTTYN);
    #else
 printk(KERN_INFO "pixci_tty" ": cleanup\n");
    #endif
}
module_init(init_pxdrvttylnx);
module_exit(cleanup_pxdrvttylnx);
#undef XCDRV_IDTTYN
