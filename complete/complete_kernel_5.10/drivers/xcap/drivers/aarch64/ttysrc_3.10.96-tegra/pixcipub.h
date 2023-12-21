/*
 *
 *	pixcipub.h	External	10-May-2018
 *
 *	Copyright (C)  2006-2014  EPIX, Inc.  All rights reserved.
 *
 *	Frame Grabber Driver: Linux Device Driver wrappers,
 *	allowing partial compilation of the frame grabber driver
 *	in the field.
 *
 */

#if !defined(__EPIX_PXDRVLNX_DEFINED)
#define __EPIX_PXDRVLNX_DEFINED



/*
 * Wrappers: PXDRV binary blob to Linux API.
 */
struct pci_dev; 	// existent(?) and very opaque
struct task_struct;	// existent(?) and very opaque
struct semaphore;	// existent(?) and very opaque
struct timer_list;	// existent(?) and very opaque
struct pt_regs; 	// existent(?) and very opaque

#if defined(P_I386)|defined(P_I86_64)
    #define _cfunregparm0   __attribute__((regparm(0)))
#elif defined(P_ARMHF) | defined(P_AARCH64)
    #define _cfunregparm0
#else
    ??
#endif


void*  _cfunregparm0 wrapper_kmalloc(size_t size);
void   _cfunregparm0 wrapper_kfree(void *ptr);
struct semaphore  * _cfunregparm0 wrapper_kmalloc_semaphore(int value);
struct timer_list * _cfunregparm0 wrapper_kmalloc_timer_list(void);
#if 0
ulong  _cfunregparm0 wrapper_get_free_pages(int nowarn, uint order, int dma32);
void   _cfunregparm0 wrapper_free_pages(ulong adrs, uint order);
ulong  _cfunregparm0 wrapper_virt_to_bus(void *p);
#endif
ulong  _cfunregparm0 wrapper_virt_to_phys(void *p);
int    _cfunregparm0 wrapper_request_region(ulong start, ulong len, char *name);
void   _cfunregparm0 wrapper_release_region(ulong start, ulong len);
int    _cfunregparm0 wrapper_request_mem_region(ulong start, ulong len, char *name);
void   _cfunregparm0 wrapper_release_mem_region(ulong start, ulong len);
void*  _cfunregparm0 wrapper_ioremap(ulong adrs, ulong size);
void   _cfunregparm0 wrapper_iounmap(volatile void *adrs);
int    _cfunregparm0 wrapper_copy_to_user(void *to, const void *from, ulong count);
int    _cfunregparm0 wrapper_copy_from_user(void *to, const void *from, ulong count);
int    _cfunregparm0 wrapper_put_user_long(long value, long *ptr);
int    _cfunregparm0 wrapper_put_user_int(int value, int *ptr);
int    _cfunregparm0 wrapper_pci_present(void);
int    _cfunregparm0 wrapper_pci_write_config_dword(struct pci_dev *dev, int where, u32 value);
int    _cfunregparm0 wrapper_pci_read_config_dword(struct pci_dev *dev, int where, u32 *value);
int    _cfunregparm0 wrapper_pci_enable_device(struct pci_dev *dev);
void   _cfunregparm0 wrapper_pci_set_master(struct pci_dev *dev);
void   _cfunregparm0 wrapper_pci_clear_master(struct pci_dev *dev);
struct pci_dev * _cfunregparm0 wrapper_pci_get_device(uint vendor, uint device, struct pci_dev *last);
void   _cfunregparm0 wrapper_pci_dev_put(struct pci_dev *dev);
int    _cfunregparm0 wrapper_pci_resource_flags_io(struct pci_dev *dev, int index);
ulong  _cfunregparm0 wrapper_pci_resource_start(struct pci_dev *dev, int index);
ulong  _cfunregparm0 wrapper_pci_resource_len(struct pci_dev *dev, int index);
int    _cfunregparm0 wrapper_pci_irq_vector(struct pci_dev *dev, int nr);
int    _cfunregparm0 wrapper_pci_alloc_irq_vectors(struct pci_dev *dev, uint minvecs, uint maxvecs, int legacy, int msi, int msix);
void   _cfunregparm0 wrapper_pci_free_irq_vectors(struct pci_dev *dev);
int    _cfunregparm0 wrapper_pci_dev_bus_number(struct pci_dev *dev);
void   _cfunregparm0 wrapper_down(struct semaphore *sem);
int    _cfunregparm0 wrapper_down_trylock(struct semaphore *sem);
int    _cfunregparm0 wrapper_down_interruptible(struct semaphore *sem);
void   _cfunregparm0 wrapper_up(struct semaphore *sem);
#if (LINUX_VERSION_CODE < KERNEL_VERSION(4,15,0))
void   _cfunregparm0 wrapper_add_timer(struct timer_list *timer, void (*func)(ulong data), ulong data, ulong millis);
#else
void   _cfunregparm0 wrapper_add_timer_4_15(struct timer_list *timer, void (*func)(struct timer_list *), ulong millis);
#endif
void   _cfunregparm0 wrapper_init_timer(struct timer_list *timer);
void   _cfunregparm0 wrapper_del_timer_sync(struct timer_list *timer);
ulong  _cfunregparm0 wrapper_jiffies(void);
u64    _cfunregparm0 wrapper_get_jiffies_64(void);
ulong  _cfunregparm0 wrapper_HZ(void);
char*  _cfunregparm0 wrapper_UTS_RELEASE(void);
ulong  _cfunregparm0 wrapper_num_physpages(void);
int    _cfunregparm0 wrapper_send_sig(int sig, struct task_struct *task, int z);
struct task_struct * _cfunregparm0 wrapper_get_current(void);
void*  _cfunregparm0 wrapper_dma_alloc_coherent(int nowarn,struct pci_dev *dev, size_t order, dma_addr_t *dma_handlep, int dma32);
void   _cfunregparm0 wrapper_dma_free_coherent(struct pci_dev *dev, size_t order, void *vaddr, dma_addr_t dma_handle);
u32    _cfunregparm0 wrapper_inl(ulong port);
void   _cfunregparm0 wrapper_outl(u32 data, ulong port);
u32    _cfunregparm0 wrapper_readl(void *adrs);
void   _cfunregparm0 wrapper_writel(u32 data, void *adrs);
#if defined(CONFIG_X86_64)|defined(CONFIG_ARM64)
u64    _cfunregparm0 wrapper_readq(void *adrs);
void   _cfunregparm0 wrapper_writeq(u64 data, void *adrs);
#endif
void   _cfunregparm0 wrapper_disable_irq(int irq);
void   _cfunregparm0 wrapper_enable_irq(int irq);
void   _cfunregparm0 wrapper_schedule(void);
void   _cfunregparm0 wrapper_gettimeofday(time_t *ssecp, long *usecp);
void   _cfunregparm0 wrapper_mdelay(ulong msecs);
void   _cfunregparm0 wrapper_udelay(ulong usecs);
void   _cfunregparm0 wrapper_msleep(uint msecs);

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,19))
int _cfunregparm0 wrapper_request_irq(uint irq,
		       irqreturn_t (*handler)(int, void*),
		       int shareirq, const char* dev_name, void *dev_id);
void _cfunregparm0 wrapper_free_irq(uint irq, void *dev_id);
#elif (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,0))
int _cfunregparm0 wrapper_request_irq(uint irq,
		       int (*handler)(int, void*, struct pt_regs *),
		       int shareirq, const char* dev_name, void *dev_id);
void _cfunregparm0 wrapper_free_irq(uint irq, void *dev_id);
#else
int _cfunregparm0 wrapper_request_irq(uint irq,
		       void (*handler)(int, void*, struct pt_regs *),
		       int shareirq, const char* dev_name, void *dev_id);
void _cfunregparm0 wrapper_free_irq(uint irq, void *dev_id);
#endif


/*
 * Wrappers: Linux API to PXDRV binary blob.
 */
int  _cfunregparm0 wrapped_init_pxdrvlnx(int major, char *parms, ulong hz, size_t page_size, int irq_handled, int irq_none, int *nunitp);
int  _cfunregparm0 wrapped_cleanup_pxdrvlnx(size_t page_size);
int  _cfunregparm0 wrapped_pxdrv_ioctl(void *inodep, void *filep, uint cmd, ulong arg, int compat);
int  _cfunregparm0 wrapped_pxdrv_open(void *inodep, void *filep);
int  _cfunregparm0 wrapped_pxdrv_release(void *inodep, void *filep);
int  _cfunregparm0 wrapped_pxdrv_mmapdope0(void *filep, ulong start, ulong end, ulong pgoff);
void*_cfunregparm0 wrapped_pxdrv_mmapdope1(void *filep, ulong adrs, dma_addr_t *dma_handlep, ulong *sizep);
struct pci_dev*_cfunregparm0 wrapped_pxdrv_mmapdope2(void);

/*
 * Wrappers: PXDRV binary blob to PXDRV tty driver to Linux tty driver.
 */
void   _cfunregparm0 wrapper_ttyinsertflipchar(void *tty, int c, int brkflag, int overrunflg);



/*
 * Wrappers: Linux tty to PXDRV tty driver to PXDRV binary blob.
 */
void*_cfunregparm0 wrapped_ttyopen(void *tty, int index);
void _cfunregparm0 wrapped_ttyclose(void *tty, void *pxtty);
int  _cfunregparm0 wrapped_ttywrite(void *tty, void *pxtty, const void *buffer, int count);
int  _cfunregparm0 wrapped_ttywriteroom(void *tty, void *pxtty);
int  _cfunregparm0 wrapped_ttycharsinbuffer(void *tty, void *pxtty);
int  _cfunregparm0 wrapped_ttysettermios(void *tty, void *pxtty, int baud);
void _cfunregparm0 wrapped_ttyflushbuffer(void *tty, void *pxtty);

/*
 * Wrappers: Linux v4l2 to PXDRV v4l2 driver to PXDRV binary blob.
 */
struct pxdrv_v4l2format;
struct pxdrv_v4l2status;
int  _cfunregparm0 wrapped_v4l2init(int *nvideop, void (*newframe)(void *v4l2, int unit, int bufferN));
int  _cfunregparm0 wrapped_v4l2getformat(int unit, struct pxdrv_v4l2format *fmtp, size_t cnt);
int  _cfunregparm0 wrapped_v4l2getstatus(int unit, struct pxdrv_v4l2status *statusp, size_t cnt);
int  _cfunregparm0 wrapped_v4l2controllabel(int unit, int ctrlIndex, char *buf, size_t bufsize);
int  _cfunregparm0 wrapped_v4l2control(int unit, int ctrlIndex, int op, int value);


/*
 * Export functions for separate PXDRV tty driver.
 */
extern int   pxdrv_ttyinit(int *nttyp, int *major, void (*insertflipchar)(void *tty, int c, int brkflag, int overrunflg));
extern void  pxdrv_ttycleanup(void (*insertflipchar)(void *tty, int c, int brkflag, int overrunflg));
extern void *pxdrv_ttyopen(void *tty, int index);
extern void  pxdrv_ttyclose(void *tty, void *pxtty);
extern int   pxdrv_ttywrite(void *tty, void *pxtty, const unsigned char *buffer, int count);
extern int   pxdrv_ttywriteroom(void *tty, void *pxtty);
extern int   pxdrv_ttycharsinbuffer(void *tty, void *pxtty);
extern int   pxdrv_ttysettermios(void *tty, void *pxtty, int baud);
extern void  pxdrv_ttyflushbuffer(void *tty, void *pxtty);


/*
 * Interface structures for separate PXDRV v4l2 driver.
 *
 * As homogenous arrays of a common data type, the
 * use of these between the binary blob and the locally
 * compiled driver won't be affected by changes to packing rules.
 * But beware variations which add elements.
 */
struct pxdrv_v4l2status
{
    u8	 no_signal;
    u8	 no_color;
    u8	 no_htiming;
    u8	 no_vtiming;
    u8	 colorkill;
    u8	 macrovision;
};

struct pxdrv_v4l2format
{
    u32  model;
    u32  pixiebits;
    u32  pixies;
    u32  imageylace;
    u32  framestamp;
    u32  videoclass;
    u32  imagexdim;
    u32  imageydim;
    u32  pixelbitpack;
    u32  imagelinelen;
    u32  imagesize;
    u32  pixelhint;
    u32  imagemultitap;
    u32  framerate[2];
    u32  pixelwidth[2];
    u32  pixelheight[2];
};

/*
 * Export functions for separate PXDRV v4l2 driver
 */
#define PXDAI_II_CTRL_MIN   0x01
#define PXDAI_II_CTRL_MAX   0x02
#define PXDAI_II_CTRL_SET   0x03
#define PXDAI_II_CTRL_GET   0x04
extern int  pxdrv_v4l2init(int *nvideop, void (*newframe)(void *v4l2, int unit, int bufferN));
extern int  pxdrv_v4l2getformat(int unit, struct pxdrv_v4l2format *fmtp, size_t cnt);
extern int  pxdrv_v4l2getimagedata(int unit, int bufferN, void *bufp, size_t cnt, int inIrq);
extern int  pxdrv_v4l2controllabel(int unit, int ctrlIndex, char *buf, size_t bufsize);
extern int  pxdrv_v4l2control(int unit, int ctrlIndex, int op, int value);



#endif				/* !defined(__EPIX_PXDRVLNX_DEFINED) */
