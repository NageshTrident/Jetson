/*                                                                                  
 *                                                                                  
 *      pixcipub.c     External        31 MAY 2018             
 *                                                                                  
 *      Copyright (C)  2006-2018  EPIX, Inc.  All rights reserved. 
 *                                                                                  
 *      Frame Grabber Driver: Linux Device Driver wrappers.                         
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
  MODULE_DESCRIPTION("PIXCI(R) 32 Bit Driver" ". " "3.8.01" " " "[18.05.31.180450]" ". " "Copyright(C) 2013 EPIX, Inc.");
  MODULE_SUPPORTED_DEVICE("PIXCI(R) Frame Grabbers");
  #if defined(MODULE_LICENSE)
    #if defined(P_AARCH64) & (LINUX_VERSION_CODE == KERNEL_VERSION(3,10,96))
       MODULE_LICENSE("GPL");
    #else
       MODULE_LICENSE("Proprietary");
    #endif
  #endif
#endif
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,0))
  #define PIXCI_TTY_EXTERNAL
  #if defined(PIXCI_TTY_EXTERNAL)
    void (*pxdrv_ttyinsertflipchar)(void *atty, int c, int brkflg, int overrunflg) = NULL;
    int pxdrv_ntty = 0;
    int pxdrv_major = 0;
  #endif
  #define PIXCI_V4L_EXTERNAL
  #if defined(PIXCI_V4L_EXTERNAL)
    void (*pxdrv_v4l2newframe)(void *av4l2, int unit, int bufferN);
  #endif
#endif
static void __exit cleanup_pxdrvlnx(void)
{
    int major = wrapped_cleanup_pxdrvlnx(PAGE_SIZE);
    if (major != 0)
 unregister_chrdev(major, "PIXCI(R)");
    printk(KERN_INFO "pixci" ": cleanup\n");
}
static int __init init_pxdrvlnx(void)
{
    extern struct file_operations pxdrv_fops;
    int r, major;
    int nunit = -1;
    printk(KERN_INFO "pixci" ": " "PIXCI(R) 64 Bit Driver" " V" "3.8.01" " " "[18.05.31.180450]" " [" UTS_RELEASE "]" " initialization\n");
    if (PIXCIPARM)
 printk(KERN_INFO "pixci" ": Configuration Parameters = %s\n", PIXCIPARM);
    r = major = register_chrdev(0, "PIXCI(R)", &pxdrv_fops);
    if (r < 0)
 return(r);
    #if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,0))
 r = wrapped_init_pxdrvlnx(major, PIXCIPARM, HZ, PAGE_SIZE, IRQ_HANDLED, IRQ_NONE, &nunit);
    #else
 r = wrapped_init_pxdrvlnx(r, PIXCIPARM, HZ, PAGE_SIZE, 0, 0, &nunit);
    #endif
    if (nunit >= 0)
 printk(KERN_INFO "pixci" ": %d unit(s) detected\n", nunit);
    if (r < 0)
 return(r);
    #if defined(PIXCI_TTY_EXTERNAL)
 pxdrv_ntty = nunit;
 pxdrv_major = major;
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
 virt = wrapped_pxdrv_mmapdope1(NULL, vma->vm_pgoff<<PAGE_SHIFT, NULL, NULL);
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
 virt = wrapped_pxdrv_mmapdope1(vma->vm_file, vma->vm_pgoff<<PAGE_SHIFT, NULL, NULL);
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
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4,13,0))
    int pxdrv_vma_fault(struct vm_fault *vmf)
    {
 void *virt;
 virt = wrapped_pxdrv_mmapdope1(vmf->vma->vm_file, vmf->vma->vm_pgoff<<PAGE_SHIFT, NULL, NULL);
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
     dma_addr_t dmaadrs;
     ulong size;
     void *virtadrs;
     int r;
     virtadrs = wrapped_pxdrv_mmapdope1(vma->vm_file, vma->vm_pgoff<<PAGE_SHIFT, &dmaadrs, &size);
     if (virtadrs == NULL)
  return(-EIO);
     if (vma->vm_end-vma->vm_start > size)
  return(-EIO);
     r = dma_mmap_coherent(&wrapped_pxdrv_mmapdope2()->dev, vma, virtadrs, dmaadrs, vma->vm_end-vma->vm_start);
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
void* _cfunregparm0 wrapper_ioremap(ulong adrs, ulong size)
{
    return(ioremap(adrs, size));
}
void _cfunregparm0 wrapper_iounmap(volatile void *adrs)
{
    iounmap(adrs);
}
int _cfunregparm0 wrapper_copy_to_user(void *to, const void *from, ulong count)
{
    return(__copy_to_user(to, from, count));
}
int _cfunregparm0 wrapper_copy_from_user(void *to, const void *from, ulong count)
{
    return(__copy_from_user(to, from, count));
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
#if 0
void _cfunregparm0 wrapper_pci_set_master(struct pci_dev *dev)
{
    pci_set_master(dev);
}
void _cfunregparm0 wrapper_pci_clear_master(struct pci_dev *dev)
{
    pci_clear_master(dev);
}
#endif
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
#if (LINUX_VERSION_CODE < KERNEL_VERSION(4,15,0))
void _cfunregparm0 wrapper_add_timer(struct timer_list *timer, void (*func)(ulong data), ulong data, ulong micros)
{
    timer->data = data;
    timer->function = func;
    if (micros < 100000)
 timer->expires = (( (HZ*micros+500)/1000)+500) /1000;
    else
 timer->expires = ( (HZ*(micros/1000))+500) /1000;
    timer->expires = timer->expires > 1? timer->expires: 1;
    timer->expires += jiffies;
    add_timer(timer);
}
#else
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
ulong _cfunregparm0 wrapper_virt_to_bus(void *p)
{
    return(virt_to_bus(p));
}
#endif
ulong _cfunregparm0 wrapper_virt_to_phys(void *p)
{
    return(virt_to_phys(p));
}
void *
_cfunregparm0 wrapper_dma_alloc_coherent(int nowarn, struct pci_dev *dev, size_t order, dma_addr_t *dma_handlep, int dma32)
{
    #if defined(P_ARMHF) | defined(P_AARCH64)
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
     vaddr = dma_alloc_coherent(&dev->dev, PAGE_SIZE*((size_t)1<<order), dma_handlep, (nowarn? WRAPPER_GFP_NOWARN: 0) );
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
    #if defined(P_ARMHF) | defined(P_AARCH64)
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
 _cfunregparm0 wrapper_kmalloc_timer_list(void)
{
    return((struct timer_list *)kmalloc(sizeof(struct timer_list), GFP_KERNEL));
}
int _cfunregparm0 wrapper_pci_resource_flags_io(struct pci_dev *dev, int index)
{
    return(pci_resource_flags(dev, index)&IORESOURCE_IO);
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
    #else
 return(totalram_pages);
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
void _cfunregparm0 wrapper_schedule(void) { schedule(); }
void _cfunregparm0 wrapper_gettimeofday(time_t *ssecp, long *usecp)
{
    struct timeval t;
    do_gettimeofday(&t);
    *ssecp = t.tv_sec;
    *usecp = t.tv_usec;
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
void _cfunregparm0 wrapper_v4l2newframe(void *av4l2, int unit, int bufferN)
{
    #if defined(PIXCI_V4L_EXTERNAL)
 void (*newframe)(void *v4l2, int unit, int bufferN);
 if ((newframe = pxdrv_v4l2newframe))
     (*newframe)(av4l2, unit, bufferN);
    #endif
}
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
    }
    int pxdrv_ttyinit(int *nttyp, int *majorp, void (*insertflipchar)(void *tty, int c, int brkflag, int overrunflg))
    {
 if (pxdrv_ttyinsertflipchar)
     return(-1);
 pxdrv_ttyinsertflipchar = insertflipchar;
 if (nttyp)
     *nttyp = pxdrv_ntty;
 if (majorp)
     *majorp = pxdrv_major;
 return(0);
    }
    void *pxdrv_ttyopen(void *tty, int index)
    {
 return(wrapped_ttyopen(tty, index));
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
#if defined(PIXCI_V4L_EXTERNAL)
    EXPORT_SYMBOL(pxdrv_v4l2init);
    EXPORT_SYMBOL(pxdrv_v4l2getformat);
    EXPORT_SYMBOL(pxdrv_v4l2controllabel);
    EXPORT_SYMBOL(pxdrv_v4l2control);
    int pxdrv_v4l2init(int *nvideop, void (*newframe)(void *v4l2, int unit, int bufferN))
    {
     return(-1);
    }
    void pxdrv_v4l2cleanup(void (*newframe)(void *v4l2, int unit, int bufferN))
    {
 pxdrv_v4l2newframe = NULL;
    }
    int pxdrv_v4l2getformat(int unit, struct pxdrv_v4l2format *fmtp, size_t cnt)
    {
 return(wrapped_v4l2getformat(unit, fmtp, cnt));
    }
    int pxdrv_v4l2getstatus(int unit, struct pxdrv_v4l2status *fmtp, size_t cnt)
    {
 return(wrapped_v4l2getstatus(unit, fmtp, cnt));
    }
    int pxdrv_v4l2controllabel(int unit, int ctrlIndex, char *buf, size_t bufsize) {
 return(wrapped_v4l2controllabel(unit, ctrlIndex, buf, bufsize));
    }
    int pxdrv_v4l2control(int unit, int ctrlIndex, int op, int value) {
 return(wrapped_v4l2control(unit, ctrlIndex, op, value));
    }
#endif
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
