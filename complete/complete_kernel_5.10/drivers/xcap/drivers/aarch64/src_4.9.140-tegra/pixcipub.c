/**                                                                               
 *                                                                                
 *      pixcipub.c     External        24-JAN-2023               
 *                                                                                
 *      Copyright (C)  2006-2023  EPIX, Inc.  Released under GPL.
 *                                                                                
 *      Frame Grabber Driver: Linux Device Driver wrappers.                       
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
#include <linux/ioport.h>
#include <linux/fs.h>
#include <linux/mm.h>
#include <linux/delay.h>
#include <asm/uaccess.h>
#include <asm/current.h>
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4,13,0))
#include <linux/uaccess.h>
#include <linux/sched/signal.h>
#endif
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4,0,0))
#include <linux/timekeeping.h>
#endif
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,0))
  #include <asm/irq.h>
  #include <linux/moduleparam.h>
  #include <linux/interrupt.h>
  #include <linux/dma-mapping.h>
  #include <linux/slab.h>
  #if !defined(UTS_RELEASE)
    #if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,35))
      #include <generated/utsrelease.h>
    #else
      #include <linux/utsrelease.h>
    #endif
  #endif
  #if !defined(SA_SHIRQ)
    #define SA_SHIRQ IRQF_SHARED
  #endif
#else
  EXPORT_NO_SYMBOLS;
#endif
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3,7,0))
  #if !defined(VM_RESERVED)
    #define VM_RESERVED VM_IO
  #endif
#endif
#if defined(PIXCI)
  #include "pixcipub.h"
  char *PIXCIPARM = "";
  #if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,0))
    module_param(PIXCIPARM, charp, 0);
  #else
    MODULE_PARM(PIXCIPARM, "s");
  #endif
  MODULE_AUTHOR("EPIX, Inc.");
  MODULE_DESCRIPTION("PIXCI(R) 64 Bit Driver" ". " "3.8.01" " " "[23.01.24.115411]" ". " "Copyright(C) 2013 EPIX, Inc.");
  #if defined(MODULE_SUPPORTED_DEVICE)
    MODULE_SUPPORTED_DEVICE("PIXCI(R) Frame Grabbers");
  #endif
  #if defined(MODULE_LICENSE)
    #if defined(PIXCI_O_SHIPPED)
       /**
 * When compiled against/with a binary blob.
 * At the most, this is interpreted as labeling *this* file
 * as GPL, not the binary blob to which it is linked.
 **/
       MODULE_LICENSE("GPL");
    #else
       /**
 * As a precompiled driver.
 **/
       MODULE_LICENSE("Proprietary");
    #endif
  #endif
#endif
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,0))
  #define PIXCI_TTY_EXTERNAL
  #if defined(PIXCI_TTY_EXTERNAL)
    void (*pxdrv_ttyinsertflipchar)(void *atty, int c, int brkflg, int overrunflg) = NULL;
    void (*pxdrv_ttyprobe) (void *atty, int major, int unit, int ntty);
  #endif
  #define PIXCI_V4L2_EXTERNAL
  #if defined(PIXCI_V4L2_EXTERNAL)
    void (*pxdrv_v4l2newframe)(void *v4l2, int unit, int buffer, int vfield, int tracker, int status, u32 ticks[]);
    void (*pxdrv_v4l2probe) (void *v4l2, int unit, struct pci_dev *pcidev);
  #endif
#endif
#if defined(PIXCI)
#define PIXCI_REGISTER_DRIVER 1
#if PIXCI_REGISTER_DRIVER
static const struct pci_device_id pixci_ids[] = {
    { PCI_DEVICE( 0x8086, 0x1223) },
    { PCI_DEVICE( 0x109E, 0x0350) },
    { PCI_DEVICE( 0x109E, 0x036E) },
    { PCI_DEVICE( 0x109E, 0x0878) },
    { PCI_DEVICE( 0x14F1, 0x8800) },
    { PCI_DEVICE( 0x14F1, 0x8801) },
    { PCI_DEVICE( 0x14F1, 0x8802) },
    { PCI_DEVICE( 0x14F1, 0x8803) },
    { PCI_DEVICE( 0x14F1, 0x8804) },
    { PCI_DEVICE( 0x165A, 0xEA02) },
    { PCI_DEVICE( 0x165A, 0xEA03) },
    { PCI_DEVICE( 0x165A, 0xA310) },
    { PCI_DEVICE( 0x165A, 0xA110) },
    { PCI_DEVICE( 0x165A, 0xA117) },
    { PCI_DEVICE( 0x10E8, 0x82B1) },
    { PCI_DEVICE( 0x10E8, 0x80D6) },
    { PCI_DEVICE( 0x10E8, 0x817F) },
    { PCI_DEVICE( 0x165A, 0xD200) },
    { PCI_DEVICE( 0x165A, 0xD20A) },
    { PCI_DEVICE( 0x165A, 0xD300) },
    { PCI_DEVICE( 0x165A, 0xC100) },
    { PCI_DEVICE( 0x165A, 0xC10A) },
    { PCI_DEVICE( 0x165A, 0xC200) },
    { PCI_DEVICE( 0x165A, 0xED30) },
    { PCI_DEVICE( 0x165A, 0xE001) },
    { PCI_DEVICE( 0x165A, 0xF001) },
    { PCI_DEVICE( 0x165A, 0xEC01) },
    { PCI_DEVICE( 0x165A, 0xECF1) },
    { PCI_DEVICE( 0x165A, 0xECB1) },
    { PCI_DEVICE( 0x165A, 0xECB2) },
    { PCI_DEVICE( 0x165A, 0xECB3) },
    { PCI_DEVICE( 0x165A, 0xEB01) },
    { PCI_DEVICE( 0x165A, 0xEB13) },
    { PCI_DEVICE( 0x165A, 0xEB14) },
    { PCI_DEVICE( 0x165A, 0xEB41) },
    { PCI_DEVICE( 0x165A, 0xEB43) },
    { PCI_DEVICE( 0x165A, 0xEB21) },
    { PCI_DEVICE( 0x165A, 0xEB31) },
    { PCI_DEVICE( 0x165A, 0xEB22) },
    { PCI_DEVICE( 0x165A, 0xEB23) },
    { PCI_DEVICE( 0x165A, 0xEB25) },
    { PCI_DEVICE( 0x165A, 0xEB26) },
    { PCI_DEVICE( 0x165A, 0xEB11) },
    { PCI_DEVICE( 0x165A, 0xED01) },
    { PCI_DEVICE( 0x165A, 0xFD01) },
    { PCI_DEVICE( 0x165A, 0xFDF1) },
    { PCI_DEVICE( 0x165A, 0xF002) },
    { PCI_DEVICE( 0x165A, 0xED04) },
    { PCI_DEVICE( 0x165A, 0xE004) },
    { PCI_DEVICE( 0x165A, 0xE008) },
    { PCI_DEVICE( 0x165A, 0xE0C8) },
    { PCI_DEVICE( 0x165A, 0xED08) },
    { PCI_DEVICE( 0x165A, 0xE0A1) },
    { PCI_DEVICE( 0x165A, 0xF0A1) },
    { PCI_DEVICE( 0x165A, 0xF0F1) },
    { PCI_DEVICE( 0x165A, 0xEDA1) },
    { PCI_DEVICE( 0x165A, 0xE204) },
    { PCI_DEVICE( 0x165A, 0xE404) },
    { PCI_DEVICE( 0x165A, 0xE304) },
    { PCI_DEVICE( 0x165A, 0xE504) },
    { PCI_DEVICE( 0x165A, 0xE704) },
    { PCI_DEVICE( 0x165A, 0xE604) },
    { PCI_DEVICE( 0x165A, 0xE512) },
    { PCI_DEVICE( 0x165A, 0xE712) },
    { PCI_DEVICE( 0x165A, 0xE612) },
    { PCI_DEVICE( 0x165A, 0xC000) },
    { PCI_DEVICE( 0x165A, 0xC001) },
    { PCI_DEVICE( 0x165A, 0xC004) },
    { PCI_DEVICE( 0x165A, 0xC024) },
    { PCI_DEVICE( 0x165A, 0xC002) },
    { PCI_DEVICE( 0x165A, 0xC012) },
    { PCI_DEVICE( 0x165A, 0xC011) },
    { PCI_DEVICE( 0x165A, 0xC014) },
    { PCI_DEVICE( 0x165A, 0xF0B1) },
    { PCI_DEVICE( 0x165A, 0xD201) },
    { PCI_DEVICE( 0x165A, 0xC301) },
    { PCI_DEVICE( 0x165A, 0xC300) },
    { 0 },
};
MODULE_DEVICE_TABLE(pci, pixci_ids);
static int pxdrv_probe(struct pci_dev *dev, const struct pci_device_id *idp)
{
    int r = wrapped_pxdrv_probe(dev, idp->vendor, idp->device);
    if (r >= 0)
 printk(KERN_INFO "pixci" ": PCI Device ID %x:%x found\n", idp->vendor, idp->device);
    return(r);
}
static void pxdrv_remove(struct pci_dev *dev)
{
    printk(KERN_INFO "pixci" ": removed\n");
    wrapped_pxdrv_remove(dev);
}
static struct pci_driver pixci_driver = {
    .name = "PIXCI(R)",
    .id_table = pixci_ids,
    .probe = pxdrv_probe,
    .remove = pxdrv_remove,
};
#endif
#endif
static void __exit cleanup_pxdrvlnx(void)
{
    int major = wrapped_cleanup_pxdrvlnx(PAGE_SIZE);
    if (major != 0)
 unregister_chrdev(major, "PIXCI(R)");
    #if PIXCI_REGISTER_DRIVER
 pci_unregister_driver(&pixci_driver);
    #endif
    printk(KERN_INFO "pixci" ": cleanup\n");
}
static int __init init_pxdrvlnx(void)
{
    extern struct file_operations pxdrv_fops;
    int r, major;
    int nunit = -1;
    printk(KERN_INFO "pixci" ": " "PIXCI(R) 64 Bit Driver" " V" "3.8.01" " " "[23.01.24.115411]" " initialization\n");
    if (PIXCIPARM)
 printk(KERN_INFO "pixci" ": Configuration Parameters = %s\n", PIXCIPARM);
    r = major = register_chrdev(0, "PIXCI(R)", &pxdrv_fops);
    if (r < 0)
 return(r);
    #if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,0))
 r = wrapped_init_pxdrvlnx(major, PIXCIPARM, HZ, PAGE_SIZE, IRQ_HANDLED, IRQ_NONE, &nunit, PIXCI_REGISTER_DRIVER);
    #else
 r = wrapped_init_pxdrvlnx(r, PIXCIPARM, HZ, PAGE_SIZE, 0, 0, &nunit, PIXCI_REGISTER_DRIVER);
    #endif
    #if !PIXCI_REGISTER_DRIVER
 if (nunit >= 0)
     printk(KERN_INFO "pixci" ": %d unit(s) found\n", nunit);
    #endif
    if (r < 0) {
 unregister_chrdev(major, "PIXCI(R)");
 return(r);
    }
    #if PIXCI_REGISTER_DRIVER
 r = pci_register_driver(&pixci_driver);
 if (r < 0) {
     unregister_chrdev(major, "PIXCI(R)");
     wrapped_cleanup_pxdrvlnx(PAGE_SIZE);
     return(r);
 }
    #endif
    #if defined(PIXCI_TTY_EXTERNAL)
    #endif
    return(0);
}
module_init(init_pxdrvlnx);
module_exit(cleanup_pxdrvlnx);
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,18))
    long pxdrv_ioctl(struct file *filep, uint cmd, ulong arg)
    {
 return(wrapped_pxdrv_ioctl(NULL, filep, cmd, arg, 0));
    }
    long pxdrv_ioctl32(struct file *filep, uint cmd, ulong arg)
    {
 return(wrapped_pxdrv_ioctl(NULL, filep, cmd, arg, 32));
    }
#else
int pxdrv_ioctl(struct inode *inodep, struct file *filep, uint cmd, ulong arg)
    {
 return(wrapped_pxdrv_ioctl(inodep, filep, cmd, arg, 0));
    }
#endif
int pxdrv_open(struct inode *inodep, struct file *filep)
{
    return(wrapped_pxdrv_open(inodep, filep));
}
int pxdrv_release(struct inode *inodep, struct file *filep)
{
    return(wrapped_pxdrv_release(inodep, filep));
}
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,0))
void pxdrv_vma_open(struct vm_area_struct *vma)
{
    MOD_INC_USE_COUNT;
}
void pxdrv_vma_close(struct vm_area_struct *vma)
{
    MOD_DEC_USE_COUNT;
}
#endif
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,0)) && (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,26))
    struct page *
    pxdrv_vma_nopage(struct vm_area_struct *vma, ulong address, int *type)
    {
 struct page *page;
 void *virt;
 virt = wrapped_pxdrv_mmapdope1(NULL, vma->vm_pgoff<<PAGE_SHIFT, NULL, NULL, NULL, NULL);
 if (virt == NULL)
     return(NULL);
 page = virt_to_page(virt + address - vma->vm_start);
 get_page(page);
 if (type)
     *type = VM_FAULT_MINOR;
 return(page);
    }
    struct vm_operations_struct pxdrv_vmops = {
 nopage: pxdrv_vma_nopage,
    };
#endif
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,26)) && (LINUX_VERSION_CODE < KERNEL_VERSION(4,13,0))
    int pxdrv_vma_fault(struct vm_area_struct *vma, struct vm_fault *vmf)
    {
 void *virt;
 virt = wrapped_pxdrv_mmapdope1(vma->vm_file, vma->vm_pgoff<<PAGE_SHIFT, NULL, NULL, NULL, NULL);
 if (virt == NULL)
     return(VM_FAULT_SIGBUS);
 #if (LINUX_VERSION_CODE >= KERNEL_VERSION(4,10,0))
     vmf->page = virt_to_page(virt + (ulong)vmf->address - vma->vm_start);
 #else
     vmf->page = virt_to_page(virt + (ulong)vmf->virtual_address - vma->vm_start);
 #endif
 get_page(vmf->page);
 return(0);
    }
    struct vm_operations_struct pxdrv_vmops = {
 fault: pxdrv_vma_fault,
    };
#endif
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4,13,0)) && (LINUX_VERSION_CODE < KERNEL_VERSION(5,1,0))
    int pxdrv_vma_fault(struct vm_fault *vmf)
    {
 void *virt;
 virt = wrapped_pxdrv_mmapdope1(vmf->vma->vm_file, vmf->vma->vm_pgoff<<PAGE_SHIFT, NULL, NULL, NULL, NULL);
 if (virt == NULL)
     return(VM_FAULT_SIGBUS);
 vmf->page = virt_to_page(virt + (ulong)vmf->address - vmf->vma->vm_start);
 get_page(vmf->page);
 return(0);
    }
    struct vm_operations_struct pxdrv_vmops = {
 fault: pxdrv_vma_fault,
    };
#endif
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(5,1,0))
    vm_fault_t pxdrv_vma_fault(struct vm_fault *vmf)
    {
 void *virt;
 virt = wrapped_pxdrv_mmapdope1(vmf->vma->vm_file, vmf->vma->vm_pgoff<<PAGE_SHIFT, NULL, NULL, NULL, NULL);
 if (virt == NULL)
     return(VM_FAULT_SIGBUS);
 vmf->page = virt_to_page(virt + (ulong)vmf->address - vmf->vma->vm_start);
 get_page(vmf->page);
 return(0);
    }
    struct vm_operations_struct pxdrv_vmops = {
 fault: pxdrv_vma_fault,
    };
#endif
int pxdrv_mmap(struct file *filep, struct vm_area_struct *vma)
{
    #if defined(P_AARCH64) && LINUX_VERSION_CODE >= KERNEL_VERSION(4,4,15)
 #if 1|defined(ARCH_HAS_DMA_MMAPP_COHERENT)
     dma_addr_t dmaadrs, dmaoffset;
     ulong size;
     void *cpuadrs;
     struct pci_dev *pcidev = NULL;
     int r;
     cpuadrs = wrapped_pxdrv_mmapdope1(vma->vm_file, vma->vm_pgoff<<PAGE_SHIFT, &dmaadrs, &size, &pcidev, &dmaoffset);
     if (cpuadrs == NULL)
  return(-EIO);
     if (vma->vm_end-vma->vm_start > size)
  return(-EIO);
     vma->vm_pgoff = dmaoffset>>PAGE_SHIFT;
     r = dma_mmap_coherent(&pcidev->dev, vma, cpuadrs, dmaadrs, vma->vm_end-vma->vm_start);
     vma->vm_flags |= VM_DONTEXPAND
        | VM_DONTDUMP;
     return(r);
 #endif
    #endif
    #if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,0))
 if (wrapped_pxdrv_mmapdope0(filep, vma->vm_start, vma->vm_end, vma->vm_pgoff) == 'e') {
     vma->vm_flags |= VM_RESERVED;
     if (!remap_pfn_range(vma, vma->vm_start, vma->vm_pgoff, vma->vm_end-vma->vm_start, vma->vm_page_prot))
  return(0);
     return(-EAGAIN);
 } else {
     #if defined(__GFP_COMP)
  if (__GFP_COMP != 0) {
      vma->vm_flags |= VM_RESERVED;
      vma->vm_ops = &pxdrv_vmops;
      return(0);
  }
     #endif
 }
    #endif
    return(-EAGAIN);
}
struct file_operations pxdrv_fops = {
    #if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,18))
 unlocked_ioctl: pxdrv_ioctl,
 compat_ioctl: pxdrv_ioctl32,
    #else
 ioctl: pxdrv_ioctl,
    #endif
    open: pxdrv_open,
    release: pxdrv_release,
    mmap: pxdrv_mmap,
    owner: THIS_MODULE,
};
/****
 *
 * Wrappers for Linux kernel functions.
 * The pre-compiled driver guts calls these instead
 * of directly calling kernel functions.
 *
 ****/
int _cfunregparm0 wrapper_request_mem_region(ulong start, ulong len, char *name)
{
    return(request_mem_region(start, len, name) != NULL);
}
void _cfunregparm0 wrapper_release_mem_region(ulong start, ulong len)
{
    release_mem_region(start, len);
}
int _cfunregparm0 wrapper_request_region(ulong start, ulong len, char *name)
{
    return(request_region(start, len, name) != NULL);
}
void _cfunregparm0 wrapper_release_region(ulong start, ulong len)
{
    release_region(start, len);
}
void* _cfunregparm0 wrapper_ioremap(ulong adrs, ulong size, int flags)
{
    #if (LINUX_VERSION_CODE >= KERNEL_VERSION(5,6,0))
 return(ioremap(adrs, size));
    #else
 if (flags & IORESOURCE_CACHEABLE)
     return(ioremap(adrs, size));
 else
     return(ioremap_nocache(adrs, size));
    #endif
}
void _cfunregparm0 wrapper_iounmap(volatile void *adrs)
{
    iounmap(adrs);
}
int _cfunregparm0 wrapper_copy_to_user(void *to, const void *from, ulong count)
{
    return(copy_to_user(to, from, count));
}
int _cfunregparm0 wrapper_copy_from_user(void *to, const void *from, ulong count)
{
    return(copy_from_user(to, from, count));
}
int _cfunregparm0 wrapper_pci_write_config_dword(struct pci_dev *dev, int where, u32 value)
{
    return(pci_write_config_dword(dev, where, value));
}
int _cfunregparm0 wrapper_pci_read_config_dword(struct pci_dev *dev, int where, u32 *value)
{
    return(pci_read_config_dword(dev, where, value));
}
int _cfunregparm0 wrapper_pci_enable_device(struct pci_dev *dev)
{
    return(pci_enable_device(dev));
}
void _cfunregparm0 wrapper_pci_disable_device(struct pci_dev *dev)
{
    pci_disable_device(dev);
}
void* _cfunregparm0 wrapper_pci_iomap(struct pci_dev *dev, int bar, unsigned long max)
{
    return(pci_iomap(dev, bar, max));
}
void _cfunregparm0 wrapper_pci_iounmap(struct pci_dev *dev, void *p)
{
    pci_iounmap(dev, p);
}
int _cfunregparm0 wrapper_pci_request_region(struct pci_dev *dev, int bar)
{
    #if defined(HAVE_PCI_REQ_REGIONS)
 return(pci_request_region(dev, bar, "PIXCI(R)"));
    #else
 return(ENOSYS);
    #endif
}
void _cfunregparm0 wrapper_pci_release_region(struct pci_dev *dev, int bar)
{
    #if defined(HAVE_PCI_REQ_REGIONS)
 pci_release_region(dev, bar);
    #endif
}
void _cfunregparm0 wrapper_pci_set_master(struct pci_dev *dev)
{
    pci_set_master(dev);
}
void _cfunregparm0 wrapper_pci_clear_master(struct pci_dev *dev)
{
    #if (LINUX_VERSION_CODE <= KERNEL_VERSION(2,8,18))
 u16 data;
 if (pci_read_config_word(dev, PCI_COMMAND, &data) >= 0)
     pci_write_config_word(dev, PCI_COMMAND, data & ~PCI_COMMAND_MASTER);
    #else
 pci_clear_master(dev);
    #endif
}
struct pci_dev *
_cfunregparm0 wrapper_pci_get_device(uint vendor, uint device, struct pci_dev *last)
{
    #if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,0))
 return(pci_get_device(vendor, device, (struct pci_dev*)last));
    #else
 return(pci_find_device(vendor, device, (struct pci_dev*)last));
    #endif
}
void _cfunregparm0 wrapper_pci_dev_put(struct pci_dev *dev)
{
    #if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,0))
 pci_dev_put(dev);
    #endif
}
int _cfunregparm0 _cfunregparm0 wrapper_pci_present(void)
{
    #if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,0))
 return(1);
    #else
 return(pci_present());
    #endif
}
void _cfunregparm0 _cfunregparm0 wrapper_down(struct semaphore *sem)
{
    down(sem);
}
int _cfunregparm0 _cfunregparm0 wrapper_down_trylock(struct semaphore *sem)
{
    return(down_trylock(sem));
}
int _cfunregparm0 _cfunregparm0 wrapper_down_interruptible(struct semaphore *sem)
{
    return(down_interruptible(sem));
}
void _cfunregparm0 wrapper_up(struct semaphore *sem)
{
    up(sem);
}
void _cfunregparm0 wrapper_add_timer(struct timer_list *timer, void (*func_old)(ulong data), void (*func_new)(struct timer_list *), ulong data, ulong micros)
{
    #if (LINUX_VERSION_CODE < KERNEL_VERSION(4,15,0))
 timer->data = data;
 timer->function = func_old;
 if (micros < 100000)
     timer->expires = (( (HZ*micros+500)/1000)+500) /1000;
 else
     timer->expires = ( (HZ*(micros/1000))+500) /1000;
 timer->expires = timer->expires > 1? timer->expires: 1;
 timer->expires += jiffies;
 add_timer(timer);
    #else
 timer_setup(timer, func_new, 0);
 if (micros < 100000)
     timer->expires = (( (HZ*micros+500)/1000)+500) /1000;
 else
     timer->expires = ( (HZ*(micros/1000))+500) /1000;
 timer->expires = timer->expires > 1? timer->expires: 1;
 timer->expires += jiffies;
 add_timer(timer);
    #endif
}
#if 0
void _cfunregparm0 wrapper_add_timer_4_15(struct timer_list *timer, void (*func)(struct timer_list *), ulong micros)
{
    timer_setup(timer, func, 0);
    if (micros < 100000)
 timer->expires = (( (HZ*micros+500)/1000)+500) /1000;
    else
 timer->expires = ( (HZ*(micros/1000))+500) /1000;
    timer->expires = timer->expires > 1? timer->expires: 1;
    timer->expires += jiffies;
    add_timer(timer);
}
#endif
void _cfunregparm0 wrapper_init_timer(struct timer_list *timer)
{
    #if (LINUX_VERSION_CODE < KERNEL_VERSION(4,15,0))
    init_timer(timer);
    #endif
}
void _cfunregparm0 wrapper_del_timer_sync(struct timer_list *timer)
{
    del_timer_sync(timer);
}
int _cfunregparm0 wrapper_send_sig(int sig, struct task_struct *task, int z)
{
    return(send_sig(sig, task, z));
}
struct task_struct *
_cfunregparm0 wrapper_get_current(void)
{
    return(get_current());
}
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,19))
typedef irqreturn_t (*irqhandler_t)(int, void*);
#elif (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,0))
typedef irqreturn_t (*irqhandler_t)(int, void*, struct pt_regs *);
#else
typedef void (*irqhandler_t)(int, void*, struct pt_regs *);
#endif
int _cfunregparm0 wrapper_request_irq(uint irq, irqhandler_t handler,int shareirq, const char* dev_name, void *dev_id) {
    return(request_irq(irq, handler, shareirq?SA_SHIRQ:0, dev_name, dev_id));
}
void _cfunregparm0 wrapper_free_irq(uint irq, void *dev_id)
{
    free_irq(irq, dev_id);
}
int _cfunregparm0 wrapper_put_user_long(long value, long *ptr)
{
    return(__put_user(value, ptr));
}
int _cfunregparm0 wrapper_put_user_int(int value, int *ptr)
{
    return(__put_user(value, ptr));
}
#if (defined(P_I386)|defined(P_I86_64))&0
ulong _cfunregparm0 wrapper_get_free_pages(int nowarn, uint order, int dma32)
{
    #if defined(__GFP_NOWARN)
 #define WRAPPER_GFP_NOWARN __GFP_NOWARN
    #else
 #define WRAPPER_GFP_NOWARN 0
    #endif
    #if defined(__GFP_COMP)
 #define WRAPPER_GFP_COMP __GFP_COMP
    #else
 #define WRAPPER_GFP_COMP 0
    #endif
    #if defined(__GFP_DMA32)
 #define WRAPPER_GFP_DMA32 __GFP_DMA32
    #else
 #define WRAPPER_GFP_DMA32 0
    #endif
    ulong adrs = __get_free_pages( GFP_KERNEL
      | (nowarn? WRAPPER_GFP_NOWARN: 0)
      | (dma32? WRAPPER_GFP_DMA32: 0)
      | WRAPPER_GFP_COMP, order);
    printk(KERN_INFO "pixci" ": get_free_pages: adrs=%lx order=%ld _PAGE_SIZE=%ld\n", (ulong)adrs, (long)order, (long)PAGE_SIZE);
    return(adrs);
    #undef WRAPPER_GFP_NOWARN
    #undef WRAPPER_GFP_COMP
}
void _cfunregparm0 wrapper_free_pages(ulong adrs, uint order)
{
    free_pages(adrs, order);
    printk(KERN_INFO "pixci" ": free_pages: adrs=%lx order=%ld\n", (ulong)adrs, (long)order);
}
#endif
#if (defined(P_I386)|defined(P_I86_64))&0
ulong _cfunregparm0 wrapper_virt_to_bus(void *p)
{
    return(virt_to_bus(p));
}
#endif
#if (defined(P_I386)|defined(P_I86_64))&0
ulong _cfunregparm0 wrapper_virt_to_phys(void *p)
{
    return(virt_to_phys(p));
}
#endif
void *
_cfunregparm0 wrapper_dma_alloc_coherent(int nowarn, struct pci_dev *dev, size_t order, dma_addr_t *dma_handlep, int dma32)
{
    #if defined(P_ARMHF) | defined(P_AARCH64) | (LINUX_VERSION_CODE >= KERNEL_VERSION(6,0,0))
 #if defined(__GFP_NOWARN)
     #define WRAPPER_GFP_NOWARN __GFP_NOWARN
 #else
     #define WRAPPER_GFP_NOWARN 0
 #endif
 #if defined(__GFP_DMA32)
     #define WRAPPER_GFP_DMA32 __GFP_DMA32
 #else
     #define WRAPPER_GFP_DMA32 0
 #endif
 void *vaddr;
 #if 0
     if (dma_set_mask_and_coherent(&dev->dev, dma32? DMA_BIT_MASK(32): DMA_BIT_MASK(64))) {
  printk(KERN_ERR "pixci" ": set_mask_and_coherent failed\n");
  return(0);
     }
 #else
     if (dma_set_coherent_mask(&dev->dev, dma32? DMA_BIT_MASK(32): DMA_BIT_MASK(64))) {
  printk(KERN_ERR "pixci" ": dma_set_coherent_mask failed\n");
  return(0);
     }
 #endif
 #if 0
     vaddr = kmalloc(PAGE_SIZE*((size_t)1<<order), GFP_KERNEL|GFP_DMA|(dma32? WRAPPER_GFP_DMA32: 0));
     if (!vaddr)
  return(NULL);
     *dma_handlep = dma_map_single(&dev->dev, vaddr, PAGE_SIZE*((size_t)1<<order), DMA_FROM_DEVICE);
     printk(KERN_INFO "pixci" ": dma_map_single vaddr=%lx size=%lu\n", (ulong)(uintptr_t)vaddr, (ulong)(PAGE_SIZE*((size_t)1<<order)));
     if (dma_mapping_error(&dev->dev, *dma_handlep)) {
  kfree(vaddr);
  return(NULL);
     }
     return(vaddr);
 #else
     vaddr = dma_alloc_coherent(&dev->dev, PAGE_SIZE*((size_t)1<<order), dma_handlep, (nowarn? WRAPPER_GFP_NOWARN: 0) | GFP_KERNEL );
     printk(KERN_INFO "pixci" ": dma_alloc_coherent vaddr=%lx size=%lu\n", (ulong)vaddr, (ulong)(PAGE_SIZE*((size_t)1<<order)));
     return(vaddr);
 #endif
 #undef WRAPPER_GFP_NOWARN
    #else
 #if defined(__GFP_NOWARN)
     #define WRAPPER_GFP_NOWARN __GFP_NOWARN
 #else
     #define WRAPPER_GFP_NOWARN 0
 #endif
 #if defined(__GFP_COMP)
     #define WRAPPER_GFP_COMP __GFP_COMP
 #else
     #define WRAPPER_GFP_COMP 0
 #endif
 #if defined(__GFP_DMA32)
     #define WRAPPER_GFP_DMA32 __GFP_DMA32
 #else
     #define WRAPPER_GFP_DMA32 0
 #endif
 ulong adrs;
 adrs = __get_free_pages( GFP_KERNEL
    | (nowarn? WRAPPER_GFP_NOWARN: 0)
    | (dma32? WRAPPER_GFP_DMA32: 0)
    | WRAPPER_GFP_COMP, order);
 printk(KERN_INFO "pixci" ": get_free_pages: adrs=%lx order=%ld PAGE_SIZE=%ld\n", (ulong)adrs, (long)order, (long)PAGE_SIZE);
 *dma_handlep = virt_to_bus((void*)adrs);
 return((void*)adrs);
 #undef WRAPPER_GFP_NOWARN
 #undef WRAPPER_GFP_COMP
    #endif
}
void
_cfunregparm0 wrapper_dma_free_coherent(struct pci_dev *dev, size_t order, void *vaddr, dma_addr_t dma_handle)
{
    #if defined(P_ARMHF) | defined(P_AARCH64) | (LINUX_VERSION_CODE >= KERNEL_VERSION(6,0,0))
 #if 0
     dma_unmap_single(&dev->dev, dma_handle, PAGE_SIZE*((size_t)1<<order), DMA_FROM_DEVICE);
     printk(KERN_INFO "pixci" ": unmap_single vaddr=%lx size=%lu\n", (ulong)(uintptr_t)vaddr, (ulong)(PAGE_SIZE*((size_t)1<<order)));
     kfree(vaddr);
 #else
     dma_free_coherent(&dev->dev, PAGE_SIZE*((size_t)1<<order), vaddr, dma_handle);
     printk(KERN_INFO "pixci" ": dma_free_coherent vaddr=%lx size=%lu\n", (ulong)vaddr, (ulong)(PAGE_SIZE*((size_t)1<<order)));
 #endif
    #else
 free_pages((ulong)vaddr, order);
 printk(KERN_INFO "pixci" ": free_pages: adrs=%lx order=%ld\n", (ulong)vaddr, (long)order);
    #endif
}
struct semaphore *
 _cfunregparm0 wrapper_kmalloc_semaphore(int value)
{
    struct semaphore *sem = kmalloc(sizeof(struct semaphore), GFP_KERNEL);
    if (sem)
 sema_init(sem, value);
    return(sem);
}
struct timer_list *
 _cfunregparm0 wrapper_kmalloc_timer_list(size_t extra)
{
    return((struct timer_list *)kmalloc(sizeof(struct timer_list)+extra, GFP_KERNEL));
}
spinlock_t *
 _cfunregparm0 wrapper_kmalloc_spinlock(void)
{
    spinlock_t *sp = (spinlock_t *)kmalloc(sizeof(spinlock_t), GFP_KERNEL);
    if (sp)
 spin_lock_init(sp);
    return(sp);
}
ulong _cfunregparm0 wrapper_spin_lock_irqsave(spinlock_t *sp, ulong flags)
{
    spin_lock_irqsave(sp, flags);
    return(flags);
}
ulong _cfunregparm0 wrapper_spin_unlock_irqrestore(spinlock_t *sp, ulong flags)
{
    spin_unlock_irqrestore(sp, flags);
    return(flags);
}
void _cfunregparm0 wrapper_spin_lock(spinlock_t *sp)
{
    spin_lock(sp);
}
void _cfunregparm0 wrapper_spin_unlock(spinlock_t *sp)
{
    spin_unlock(sp);
}
int _cfunregparm0 wrapper_pci_resource_flags_io(struct pci_dev *dev, int index)
{
    return(pci_resource_flags(dev, index)&IORESOURCE_IO);
}
int _cfunregparm0 wrapper_pci_resource_flags(struct pci_dev *dev, int index)
{
    return(pci_resource_flags(dev, index));
}
ulong _cfunregparm0 wrapper_pci_resource_start(struct pci_dev *dev, int index)
{
    return(pci_resource_start(dev, index));
}
ulong _cfunregparm0 wrapper_pci_resource_len(struct pci_dev *dev, int index)
{
    return(pci_resource_len(dev, index));
}
int _cfunregparm0 wrapper_pci_irq_vector(struct pci_dev *dev, int nr)
{
    #if 1
 return(dev->irq);
    #else
 return(pci_irq_vector(dev, nr));
    #endif
}
int _cfunregparm0 wrapper_pci_alloc_irq_vectors(struct pci_dev *dev, uint minvecs, uint maxvecs, int legacy, int msi, int msix)
{
    #if 1
 return(0);
    #else
 return(pci_alloc_irq_vectors(dev, minvecs, maxvecs,
   (legacy? PCI_IRQ_LEGACY? 0)
        | (msi? PCI_IRQ_MSI: 0)
        | (msix? PCI_IRQ_MSIX: 0) );
    #endif
}
void _cfunregparm0 wrapper_pci_free_irq_vectors(struct pci_dev *dev)
{
    #if 1
 return;
    #else
 pci_free_irq_vectors(dev);
    #endif
}
int _cfunregparm0 wrapper_pci_dev_bus_number(struct pci_dev *dev)
{
    if (dev->bus)
 return(dev->bus->number);
    return(0);
}
ulong _cfunregparm0 wrapper_jiffies(void) { return(jiffies); }
u64 _cfunregparm0 wrapper_get_jiffies_64(void){ return(get_jiffies_64()); }
ulong _cfunregparm0 wrapper_HZ(void) { return(HZ); }
char * _cfunregparm0 wrapper_UTS_RELEASE(void) { return(UTS_RELEASE); }
ulong _cfunregparm0 wrapper_num_physpages(void)
{
    #if (LINUX_VERSION_CODE < KERNEL_VERSION(3,11,0))
 return(num_physpages);
    #elif (LINUX_VERSION_CODE < KERNEL_VERSION(5,0,0))
 return(totalram_pages);
    #else
 return(totalram_pages());
    #endif
}
u32 _cfunregparm0 wrapper_inl(ulong port) { return(inl(port)); }
void _cfunregparm0 wrapper_outl(u32 data, ulong port) { outl(data, port); }
u32 _cfunregparm0 wrapper_readl(void *adrs) { rmb(); return(readl(adrs));}
void _cfunregparm0 wrapper_writel(u32 data, void *adrs) { writel(data, adrs); wmb(); }
#if defined(CONFIG_X86_64)|defined(CONFIG_ARM64)
u64 _cfunregparm0 wrapper_readq(void *adrs) { rmb(); return(readq(adrs));}
void _cfunregparm0 wrapper_writeq(u64 data, void *adrs) { writeq(data, adrs); wmb(); }
#endif
void * _cfunregparm0 wrapper_kmalloc(size_t size) { return(kmalloc(size, GFP_KERNEL)); }
void _cfunregparm0 wrapper_kfree(void *ptr) { kfree(ptr); }
void _cfunregparm0 wrapper_disable_irq(int irq) { disable_irq(irq); }
void _cfunregparm0 wrapper_enable_irq(int irq) { enable_irq(irq); }
void _cfunregparm0 wrapper_synchronize_irq(int irq) { synchronize_irq(irq); }
void _cfunregparm0 wrapper_schedule(void) { schedule(); }
int _cfunregparm0 wrapper_gettimeofday(s64 *ssecp, long *usecp)
{
    #if (LINUX_VERSION_CODE < KERNEL_VERSION(5,0,0))
 struct timeval t;
 do_gettimeofday(&t);
 *ssecp = t.tv_sec;
 *usecp = t.tv_usec;
 return(0);
    #else
 return(-1);
    #endif
}
int _cfunregparm0 wrapper_getnstimeofday(s64 *ssecp, long *nsecp)
{
    #if (LINUX_VERSION_CODE >= KERNEL_VERSION(5,6,0))
 struct timespec64 t;
 ktime_get_real_ts64(&t);
 *ssecp = t.tv_sec;
 *nsecp = t.tv_nsec;
 return(0);
    #elif (LINUX_VERSION_CODE >= KERNEL_VERSION(5,0,0))
 struct timespec t;
 getnstimeofday(&t);
 *ssecp = t.tv_sec;
 *nsecp = t.tv_nsec;
 return(0);
    #endif
    return(-1);
}
int _cfunregparm0 wrapper_getrawmonotonic(s64 *ssecp, long *nsecp)
{
    #if (LINUX_VERSION_CODE >= KERNEL_VERSION(5,6,0))
 struct timespec64 t;
 ktime_get_raw_ts64(&t);
 *ssecp = t.tv_sec;
 *nsecp = t.tv_nsec;
 return(0);
    #elif (LINUX_VERSION_CODE >= KERNEL_VERSION(3,0,0))
 struct timespec t;
 getrawmonotonic(&t);
 *ssecp = t.tv_sec;
 *nsecp = t.tv_nsec;
 return(0);
    #endif
    return(-1);
}
void _cfunregparm0 wrapper_mdelay(ulong msecs) { mdelay(msecs); }
void _cfunregparm0 wrapper_udelay(ulong usecs) { udelay(usecs); }
void _cfunregparm0 wrapper_msleep(uint msecs)
{
    #if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,0))
 msleep(msecs);
    #else
 mdelay(msecs);
    #endif
}
void _cfunregparm0 wrapper_ttyinsertflipchar(void *atty, int c, int brkflg, int overrunflg)
{
    #if defined(PIXCI_TTY_EXTERNAL)
 void (*insertflipchar)(void *tty, int c, int brkflag, int overrunflg);
 if ((insertflipchar = pxdrv_ttyinsertflipchar))
     (*insertflipchar)(atty, c, brkflg, overrunflg);
    #endif
}
void _cfunregparm0 wrapper_ttyprobe(void *tty, int major, int unit, int ntty)
{
    #if defined(PIXCI_TTY_EXTERNAL)
 void (*probe)(void *tty, int major, int unit, int ntty);
 if ((probe = pxdrv_ttyprobe))
     (*probe)(tty, major, unit, ntty);
    #endif
}
void _cfunregparm0 wrapper_v4l2newframe(void *v4l2, int unit, int buffer, int vfield, int tracker, int status, u32 ticks[])
{
    #if defined(PIXCI_V4L2_EXTERNAL)
 void (*newframe)(void *v4l2, int unit, int buffer, int vfield, int tarcker, int status, u32 ticks[]);
 if ((newframe = pxdrv_v4l2newframe))
     (*newframe)(v4l2, unit, buffer, vfield, tracker, status, ticks);
    #endif
}
void _cfunregparm0 wrapper_v4l2probe(void *v4l2, int unit, struct pci_dev *pcidev)
{
    #if defined(PIXCI_V4L2_EXTERNAL)
 void (*probe)(void *v4l2, int unit, struct pci_dev *pcidev);
 if ((probe = pxdrv_v4l2probe))
     (*probe)(v4l2, unit, pcidev);
    #endif
}
/****
 *
 * Exported functions for use by external tty driver
 *
 ****/
#if defined(PIXCI_TTY_EXTERNAL)
    EXPORT_SYMBOL(pxdrv_ttyinit);
    EXPORT_SYMBOL(pxdrv_ttycleanup);
    EXPORT_SYMBOL(pxdrv_ttyopen);
    EXPORT_SYMBOL(pxdrv_ttyclose);
    EXPORT_SYMBOL(pxdrv_ttywrite);
    EXPORT_SYMBOL(pxdrv_ttywriteroom);
    EXPORT_SYMBOL(pxdrv_ttycharsinbuffer);
    EXPORT_SYMBOL(pxdrv_ttysettermios);
    EXPORT_SYMBOL(pxdrv_ttyflushbuffer);
    void pxdrv_ttycleanup(void (*insertflipchar)(void *tty, int c, int brkflag, int overrunflg))
    {
 pxdrv_ttyinsertflipchar = NULL;
 pxdrv_ttyprobe = NULL;
    }
    int pxdrv_ttyinit(int *nttyp, int *majorp,
        void (*insertflipchar)(void *tty, int c, int brkflag, int overrunflg),
        void (*probe)(void *tty, int major, int unit, int ntty))
    {
 if (pxdrv_ttyinsertflipchar)
     return(-1);
 if (insertflipchar)
     pxdrv_ttyinsertflipchar = insertflipchar;
 if (probe)
     pxdrv_ttyprobe = probe;
 return(wrapped_ttyinit(nttyp, majorp, insertflipchar, probe));
    }
    int pxdrv_ttyopen(void *tty, int index, void **pxttyp)
    {
 return(wrapped_ttyopen(tty, index, pxttyp));
    }
    void pxdrv_ttyclose(void *tty, void *pxtty)
    {
 wrapped_ttyclose(tty, pxtty);
    }
    int pxdrv_ttywrite(void *tty, void *pxtty, const unsigned char *buffer, int count)
    {
 return(wrapped_ttywrite(tty, pxtty, buffer, count));
    }
    int pxdrv_ttywriteroom(void *tty, void *pxtty)
    {
 return(wrapped_ttywriteroom(tty, pxtty));
    }
    int pxdrv_ttycharsinbuffer(void *tty, void *pxtty)
    {
 return(wrapped_ttycharsinbuffer(tty, pxtty));
    }
    int pxdrv_ttysettermios(void *tty, void *pxtty, int baud)
    {
 return(wrapped_ttysettermios(tty, pxtty, baud));
    }
    void pxdrv_ttyflushbuffer(void *tty, void *pxtty)
    {
 wrapped_ttyflushbuffer(tty, pxtty);
    }
#endif
/****
 *
 * Exported functions for use by external v4l driver
 *
 ****/
#if defined(PIXCI_V4L2_EXTERNAL)
    EXPORT_SYMBOL(pxdrv_v4l2init);
    EXPORT_SYMBOL(pxdrv_v4l2getformat);
    EXPORT_SYMBOL(pxdrv_v4l2getstatus);
    EXPORT_SYMBOL(pxdrv_v4l2getdope);
    EXPORT_SYMBOL(pxdrv_v4l2controllabel);
    EXPORT_SYMBOL(pxdrv_v4l2control);
    EXPORT_SYMBOL(pxdrv_v4l2imageio);
    EXPORT_SYMBOL(pxdrv_v4l2video);
    EXPORT_SYMBOL(pxdrv_v4l2getlastfault);
    EXPORT_SYMBOL(pxdrv_v4l2open);
    EXPORT_SYMBOL(pxdrv_v4l2cleanup);
    EXPORT_SYMBOL(pxdrv_v4l2close);
    int pxdrv_v4l2init(int *nvideop,
         void (*newframe)(void *v4l2, int unit, int buffer, int vfield, int tracker, int status, u32 ticks[]),
         void (*probe)(void *v4l2, int unit, struct pci_dev *pcidev))
    {
 if (pxdrv_v4l2newframe)
     return(-1);
 pxdrv_v4l2newframe = newframe;
 pxdrv_v4l2probe = probe;
 return(wrapped_v4l2init(nvideop, newframe, probe));
    }
    void pxdrv_v4l2cleanup(void (*newframe)(void *v4l2, int unit, int buffer, int vfield, int tracker, int status, u32 ticks[]),
      void (*probe)(void *v4l2, int unit,struct pci_dev *pcidev))
    {
 pxdrv_v4l2newframe = NULL;
 pxdrv_v4l2probe = NULL;
    }
    int pxdrv_v4l2getformat(void *v4l2, void *pxv4l2, struct pxdrv_v4l2format *fmtp, size_t cnt)
    {
 return(wrapped_v4l2getformat(v4l2, pxv4l2, fmtp, cnt));
    }
    int pxdrv_v4l2getdope(void *v4l2, void *pxv4l2, struct pxdrv_v4l2dope *dopep, size_t cnt)
    {
 return(wrapped_v4l2getdope(v4l2, pxv4l2, dopep, cnt));
    }
    int pxdrv_v4l2getstatus(void *v4l2, void *pxv4l2, struct pxdrv_v4l2status *statp, size_t cnt)
    {
 return(wrapped_v4l2getstatus(v4l2, pxv4l2, statp, cnt));
    }
    int pxdrv_v4l2controllabel(void *v4l2, void *pxv4l2, int ctrlIndex, char *buf, size_t bufsize)
    {
 return(wrapped_v4l2controllabel(v4l2, pxv4l2, ctrlIndex, buf, bufsize));
    }
    int pxdrv_v4l2control(void *v4l2, void *pxv4l2, int ctrlIndex, int op, int value)
    {
 return(wrapped_v4l2control(v4l2, pxv4l2, ctrlIndex, op, value));
    }
    int pxdrv_v4l2imageio(void *v4l2, void *pxv4l2, int bufferN, int stampOffset, void *bufp, size_t cnt, int inIrq, int userio)
    {
 return(wrapped_v4l2imageio(v4l2, pxv4l2, bufferN, stampOffset, bufp, cnt, inIrq, userio));
    }
    int pxdrv_v4l2video(void *v4l2, void *pxv4l2, int mode, int bufferN, int tracker, dma_addr_t adrs, int irq) {
 return(wrapped_v4l2video(v4l2, pxv4l2, mode, bufferN, tracker, adrs, irq));
    }
    int pxdrv_v4l2getlastfault(void *v4l2, void *pxv4l2) {
 return(wrapped_v4l2getlastfault(v4l2, pxv4l2));
    }
    void pxdrv_v4l2close(void *v4l2, void *pxv4l2) {
 wrapped_v4l2close(v4l2, pxv4l2);
    }
    void * pxdrv_v4l2open(void *v4l2, int index, int framequeues) {
 return(wrapped_v4l2open(v4l2, index, framequeues));
    }
#endif
/****
 *
 * Use of a binary blob compiled w. profiling
 * on a system where profiling is disabled
 * may require one of these stub functions.
 *
 * Also, stub functions that might be required for
 * similar reasons, such as stack checking.
 *
 ****/
#if 0
void __fentry__(void)
{
}
#endif
#if 0
void mcount(void)
{
}
#endif
#if 0
void __stack_chk_fail(void)
{
}
void __stack_chk_guard(void)
{
}
#endif
#if 0
void __ubsan_handle_out_of_bounds(void)
{
}
void __ubsan_handle_shift_out_of_bounds(void)
{
}
#endif
