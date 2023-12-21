/**                                                                           
 *                                                                            
 *      pixciv4l.c         External       28 AUGUST 2020        
 *                                                                            
 *      Copyright (C)  2009-2020  EPIX, Inc.  Released under GPL. 
 *                                                                            
 *      Frame Grabber Driver: LINUX v4l driver access to Video 4 Linux 2 (V4L2) 
 *                                                                            
 *      THIS IS A MACHINE GENERATED COPY                                      
 **/                                                                          
#define PIXCI
#define P_AARCH64
#include <linux/types.h>
#include <linux/version.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/param.h>
#include <linux/sched.h>
#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/slab.h>
#include <linux/kmod.h>
#include <linux/mutex.h>
#include <linux/workqueue.h>
#include <linux/pci.h>
#include <linux/videodev2.h>
#include <linux/v4l2-dv-timings.h>
#include <media/v4l2-device.h>
#include <media/v4l2-dev.h>
#include <media/v4l2-ioctl.h>
#include <media/v4l2-ctrls.h>
#include <media/v4l2-event.h>
#include <media/videobuf2-dma-contig.h>
#include <media/videobuf2-vmalloc.h>
#if defined(PIXCI)
  #include <asm/irq.h>
  #include <linux/interrupt.h>
  #if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,32))
    #include <linux/irqreturn.h>
  #endif
  #define XCDRV_IDV4LN "PIXCI(R)" " V4L2 Driver"
  MODULE_AUTHOR("EPIX, Inc.");
  MODULE_DESCRIPTION(XCDRV_IDV4LN);
  MODULE_SUPPORTED_DEVICE("PIXCI(R) Frame Grabbers");
  MODULE_VERSION("3.8.01" "-" "20.08.28.142201");
  #if defined(MODULE_LICENSE)
    MODULE_LICENSE("GPL");
  #endif
  #include "pixcipub.h"
  #include "pixciv4l.h"
  #if LINUX_VERSION_CODE >= KERNEL_VERSION(4,0,0)
      MODULE_SOFTDEP("pre: " "PIXCI(R)");
  #elif (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,31))
    static const char __module_depends[]
    __used
    __attribute__((section(".modinfo"))) =
    "depends=""PIXCI(R)";
  #endif
#endif
/**
 * Parameters set via insmod.
 *
 * The read_mode can be changed via VIDIOC_S_CTRL.
 * Changing others on-the-fly would require re-registering the driver,
 * or cause the image data format to be changed as a side effect.
 **/
/**
 * The /dev/videoN index, -1 for autodetect default
 **/
static int video_nr = -1;
module_param(video_nr, int, 0644);
MODULE_PARM_DESC(video_nr, "videoX start number, -1 is autodetect");
/**
 * Support streaming modes?
 * 0x01 (bit): Support MMAP
 * 0x02 (bit): Support USERPTR
 *
 * Number of streaming buffers?
 * While many V4L2 drivers default to 32 buffers,
 * we must consider high resolution, high bit depth cameras;
 * a handful of buffers should be enough w. the
 * proper application software.
 **/
#define STREAM_MEMMAP 0x01
#define STREAM_USERPTR 0x02
static unsigned stream_mode = STREAM_MEMMAP;
module_param(stream_mode, uint, 0644);
MODULE_PARM_DESC(stream_mode, "select streaming mode(s), bit 0: memmap");
static unsigned stream_buffers = 5;
module_param(stream_buffers, uint, 0644);
MODULE_PARM_DESC(stream_buffers, "number of stream buffers, default");
/**
 * Non Stream video capture modes.
 * 1 : Non Stream Read - Snap.
 * In btn trigger mode will also trigger camera with each read()
 * 2: Non Stream Read - Live, Newest
 * 3: Non Stream Read - Live, Oldest
 * Currently takes affect only after close & re-open of V4L2.
 **/
#define READMODE_SNAP 1
#define READMODE_LIVENEW 2
#define READMODE_LIVEOLD 3
static unsigned read_mode = READMODE_SNAP;
module_param(read_mode, uint, 0644);
MODULE_PARM_DESC(read_mode, "for non-stream read() mode, snap vs live and discard policies, (1, 2, 3)");
/**
 * 0x01 (bit): If capture card provides DMAed field counter,
 * choose whether the field counter should be retained
 * (but will cause the image/pixel format to be non-standard)
 *
 * 0x02 (bit): Force description of image data to be 8-bit monochrome;
 * does not modify the image data.
 **/
#define DATA_FIELDSTAMP 0x01
#define DATA_RAWBYTES 0x02
static unsigned data_mode = 0;
module_param(data_mode, uint, 0644);
MODULE_PARM_DESC(data_mode, "bit 0: retain field counter in image data, bit 1: label image data as 8 bit monochrome");
/**
 * Verbose options
 **/
#define VERBOSE_FORMAT 0x001
static unsigned verbose = VERBOSE_FORMAT;
module_param(verbose, uint, 0644);
/**
 * Bit 0: In streaming mode, optionally notify V4L2 code of
 * 'buffer done' from IRQ lower half; reducing latency albeitt
 * at risk of losing interrupts if V4L2 code takes too long.
 *
 * (For all options, 0 should be common, safe, default).
 **/
#define CAPT_BUFDONE_IRQ 0x01
static unsigned capt_options = 0;
module_param(capt_options, uint, 0644);
/**
 *
 * Compile time (if even then) parameters
 *
 **/
#define READLIVEOLD_RSVDBUFFERS 3
#define READLIVENEW_MINBUFFERS 3
#define SUPPORT_FH 1
#define NLAYERCTRL 1
#define MAXCTRL (12+NLAYERCTRL)
#define CID_CUSTOM_BASE (V4L2_CID_USER_BASE | 0xf000)
#define CUSTOM_FOURCC 0x80000000
static LIST_HEAD(pxdrvv4l2_devlist);
struct pxdrvv4l2_dev
{
    struct list_head devlist;
    struct v4l2_device v4l2_dev;
    struct v4l2_fh v4l2_fh;
    struct video_device video_dev;
    struct mutex mutex;
    int pxdrv_unit;
    void *pxdrv_plp;
    struct pci_dev *pcidev;
    struct pxdrv_v4l2format pxdrv_v4l2fmt;
    struct pxdrv_v4l2dope pxdrv_v4l2dope;
    struct pxdrv_v4l2status pxdrv_v4l2status;
    u32 v4l2_fourcc;
    v4l2_std_id v4l2_stdid;
    u32 pxdrv_framebuffers;
    struct v4l2_ctrl_handler ctrl_handler;
    struct v4l2_ctrl *ctrls[MAXCTRL];
    char *ctrlnames[MAXCTRL];
    char **ctrlmenus[MAXCTRL];
    int ctrlfgn;
    struct semaphore readmutex;
    int readstatus;
    int readsnapped;
    wait_queue_head_t readqueue;
    int readmode;
    spinlock_t readlock;
    struct pxdrvv4l2_fgbuffer *readlist;
    int readlistcnt;
    struct vb2_queue vb_vidq;
    #if LINUX_VERSION_CODE < KERNEL_VERSION(4,5,0)
    struct vb2_alloc_ctx *alloc_ctx;
    #endif
    struct list_head bufqueue;
    struct list_head bufprequeue;
    spinlock_t bufqueuelock;
    spinlock_t bufprequeuelock;
    int bufstreamon;
    int buftrackgen;
    struct workqueue_struct *bufwork;
};
struct pxdrvv4l2_buffer
{
    struct vb2_buffer vb;
    struct list_head list;
    int tracker;
    u32 ticks[2];
    u32 vfield;
    struct work_struct work;
    int workstatus;
};
struct pxdrvv4l2_fgbuffer
{
    struct list_head list;
    int fgbufindex;
};
#if !defined(list_last_entry)
#define list_last_entry(ptr, type, member) list_entry((ptr)->prev, type, member)
#endif
static void
pxdrvv4l2_reportfault(struct pxdrvv4l2_dev *dev)
{
    int fault = pxdrv_v4l2getlastfault(dev, dev->pxdrv_plp);
    if (fault > 0)
 v4l2_err(&dev->v4l2_dev, "fault (%d)\n", fault);
}
/**
 * Utility.
 * Convert PIXCI/XCLIB color space 'hints' to
 * V4L2 standard fourcc codes.
 * See fourcc info as per linuxtv.org.
 * Some codes may not be defined in older Linux distributions.
 *
 * For some formats, lacking a V4L2 standard fourcc codes,
 * build a custom fourcc based on our hint code,
 * multitap code, number of bits, and bitpack flag.
 * We must make use of the unused nibble in multitap to make
 * everything fit in 32 bits.
 * And explicitly set high bit so as to, hopefully, keep this
 * distinct from future official fourcc codes.
 * Some custom codes are expected to be eventually replaced
 * by official fourcc codes - when/if they are defined.
 *
 * (Alternately, use data_mode (DATA_RAWBYTES) to force
 * description of pixel data as 8 bit monochrome, perhaps with
 * increasing the number of 'pixels' per line).
 **/
static u32
pxdrvv4l2_pixelhintToFourCC(u32 pixelhint, u32 multitap, u32 pixiebits, u32 bitpack, u32 framestampsize)
{
    u32 fourcc;
    if (multitap == 0
     && bitpack == 0
     && (framestampsize == 0 || (data_mode&DATA_FIELDSTAMP) == 0) ) {
 if (pixiebits == 8) {
     switch (pixelhint) {
       case PXHINTGREY: return(V4L2_PIX_FMT_GREY);
       case PXHINTCBYCRY: return(V4L2_PIX_FMT_UYVY);
       case PXHINTYCBYCR: return(V4L2_PIX_FMT_YUYV);
       case PXHINTRGB: return(V4L2_PIX_FMT_RGB24);
       case PXHINTRGBX: return(V4L2_PIX_FMT_RGB32);
       case PXHINTBGR: return(V4L2_PIX_FMT_BGR24);
       case PXHINTBGRX: return(V4L2_PIX_FMT_BGR32);
       case PXHINTBAYER3: return(V4L2_PIX_FMT_SBGGR8);
       case PXHINTBAYER2: return(V4L2_PIX_FMT_SGBRG8);
       case PXHINTBAYER1: return(V4L2_PIX_FMT_SGRBG8);
       case PXHINTBAYER0: return(V4L2_PIX_FMT_SRGGB8);
     }
 }
 if (pixiebits == 10) {
     switch (pixelhint) {
       case PXHINTGREY: return(V4L2_PIX_FMT_Y10);
       case PXHINTBAYER3: return(V4L2_PIX_FMT_SBGGR10);
       case PXHINTBAYER2: return(V4L2_PIX_FMT_SGBRG10);
       case PXHINTBAYER1: return(V4L2_PIX_FMT_SGRBG10);
       case PXHINTBAYER0: return(V4L2_PIX_FMT_SRGGB10);
     }
 }
 if (pixiebits == 12) {
     switch (pixelhint) {
       case PXHINTGREY: return(V4L2_PIX_FMT_Y12);
       case PXHINTBAYER3: return(V4L2_PIX_FMT_SBGGR12);
       case PXHINTBAYER2: return(V4L2_PIX_FMT_SGBRG12);
       case PXHINTBAYER1: return(V4L2_PIX_FMT_SGRBG12);
       case PXHINTBAYER0: return(V4L2_PIX_FMT_SRGGB12);
     }
 }
 if (pixiebits == 16) {
     switch (pixelhint) {
       case PXHINTGREY: return(V4L2_PIX_FMT_Y16);
       case PXHINTBAYER3: return(V4L2_PIX_FMT_SBGGR16);
       case PXHINTBAYER2: return(V4L2_PIX_FMT_SGBRG16);
       case PXHINTBAYER1: return(V4L2_PIX_FMT_SGRBG16);
       case PXHINTBAYER0: return(V4L2_PIX_FMT_SRGGB16);
     }
 }
    }
    fourcc = CUSTOM_FOURCC
      | pixelhint
      | (multitap<<16)
      | (pixiebits<<24)
      | (bitpack?(1<<27): 0)
      | ((framestampsize!=0&&(data_mode&DATA_FIELDSTAMP)!=0)?(1<<28): 0);
    return(fourcc);
}
static int
pxdrvv4l2_videoclassToStdId(int videoclass)
{
    switch (videoclass & ~PXSCN_YC) {
      case PXSCN_RS170: return(0);
      case PXSCN_NTSC: return(V4L2_STD_NTSC_M);
      case PXSCN_NTSC_J: return(V4L2_STD_NTSC_M_JP);
      case PXSCN_CCIR: return(0);
      case PXSCN_PAL: return(V4L2_STD_PAL);
      case PXSCN_PAL_M: return(V4L2_STD_PAL_M);
      case PXSCN_PAL_N: return(V4L2_STD_PAL_N);
      case PXSCN_SECAM: return(V4L2_STD_SECAM_B);
      case PXSCN_PAL_N_COMBO: return(V4L2_STD_PAL_Nc);
      case PXSCN_PAL_60: return(V4L2_STD_PAL_60);
      case PXSCN_NTSC_443: return(V4L2_STD_NTSC_443);
    }
    return(0);
}
static void
pxdrvv4l2_reportformat(struct pxdrvv4l2_dev *dev, int doparm, int doformat, int dodope, int doinfo, int dostatus)
{
    #define HEX 0x8000
    #define DEC 0x0000
    #define OFS 0x7FFF
    static struct mosinfo { u16 offs; char *name; } *mosp, mosfmt[] = {
 { offsetof(struct pxdrv_v4l2format, videoclass ) | DEC, "videoclass\t" },
 { offsetof(struct pxdrv_v4l2format, imagexdim ) | DEC, "imagexdim\t" },
 { offsetof(struct pxdrv_v4l2format, imageydim ) | DEC, "imageydim\t" },
 { offsetof(struct pxdrv_v4l2format, imageylace ) | DEC, "imageylace\t" },
 { offsetof(struct pxdrv_v4l2format, imagelinelen ) | DEC, "imagelinelen\t" },
 { offsetof(struct pxdrv_v4l2format, imagesize ) | DEC, "imagesize\t" },
 { offsetof(struct pxdrv_v4l2format, imagemultitap ) | HEX, "imagemultitap" },
 { offsetof(struct pxdrv_v4l2format, pixiebits ) | DEC, "pixiebits\t" },
 { offsetof(struct pxdrv_v4l2format, pixies ) | DEC, "pixies\t" },
 { offsetof(struct pxdrv_v4l2format, pixelhint ) | HEX, "pixelhint\t" },
 { offsetof(struct pxdrv_v4l2format, pixelbitpack ) | DEC, "pixelbitpack\t" },
 { offsetof(struct pxdrv_v4l2format, pixelaspect[0] ) | DEC, "pixelaspect[0]" },
 { offsetof(struct pxdrv_v4l2format, pixelaspect[1] ) | DEC, "pixelaspect[1]" },
 { offsetof(struct pxdrv_v4l2format, framebuffers ) | DEC, "framebuffers\t" },
 { offsetof(struct pxdrv_v4l2format, framesize ) | DEC, "framesize\t" },
 { offsetof(struct pxdrv_v4l2format, framestampsize ) | DEC, "framestampsize" },
 { offsetof(struct pxdrv_v4l2format, framerate[0] ) | DEC, "framerate[0]\t" },
 { offsetof(struct pxdrv_v4l2format, framerate[1] ) | DEC, "framerate[1]\t" },
 { offsetof(struct pxdrv_v4l2format, video_fieldmod ) | DEC, "video_fieldmod" },
 { offsetof(struct pxdrv_v4l2format, video_fieldmap ) | DEC, "video_fieldmap" },
 { offsetof(struct pxdrv_v4l2format, video_phyldmap ) | DEC, "video_phyldmap" },
 { offsetof(struct pxdrv_v4l2format, video_vidmode ) | DEC, "video_vidmode" },
 { offsetof(struct pxdrv_v4l2format, video_periodicity) | DEC, "video_periodicity" },
 { 0, NULL},
    };
    static struct mosinfo mosdope[] = {
 { offsetof(struct pxdrv_v4l2dope, model ) | HEX, "model\t" },
 { offsetof(struct pxdrv_v4l2dope, framequeues ) | DEC, "framequeues\t" },
 { offsetof(struct pxdrv_v4l2dope, framealign ) | DEC, "framealign\t" },
 { offsetof(struct pxdrv_v4l2dope, dma32 ) | DEC, "dma32\t"},
 { offsetof(struct pxdrv_v4l2dope, ticks[0] ) | DEC, "ticks[0]"},
 { offsetof(struct pxdrv_v4l2dope, ticks[1] ) | DEC, "ticks[1]"},
 { offsetof(struct pxdrv_v4l2dope, ticku[0] ) | DEC, "ticku[0]"},
 { offsetof(struct pxdrv_v4l2dope, ticku[1] ) | DEC, "ticku[1]"},
 { 0, NULL},
    };
    static struct mosinfo mosstat[] = {
 { offsetof(struct pxdrv_v4l2status, no_signal ) | DEC, "no_signal\t"},
 { offsetof(struct pxdrv_v4l2status, no_color ) | DEC, "no_color\t"},
 { offsetof(struct pxdrv_v4l2status, no_htiming ) | DEC, "no_htiming\t"},
 { offsetof(struct pxdrv_v4l2status, no_vtiming ) | DEC, "no_vtiming\t"},
 { offsetof(struct pxdrv_v4l2status, colorkill ) | DEC, "colorkill\t"},
 { offsetof(struct pxdrv_v4l2status, macrovision ) | DEC, "macrovision\t"},
 { 0, NULL},
    };
    char *s1 = "\t ...";
    char *s2 = "fmt :";
    char *s3 = "dope:";
    char *s4 = "stat:";
    char *s5 = "parm:";
    if (doparm) {
 char *name = dev? dev->v4l2_dev.name: XCDRV_IDV4LN;
 printk(KERN_INFO "%s: %s %s: video_nr\t\t= %d\n", name, s1, s5, video_nr);
 printk(KERN_INFO "%s: %s %s: stream_mode\t\t= 0x%x\n", name, s1, s5, stream_mode);
 printk(KERN_INFO "%s: %s %s: stream_buffers\t= %d\n", name, s1, s5, stream_buffers);
 printk(KERN_INFO "%s: %s %s: data_mode\t\t= 0x%x\n", name, s1, s5, data_mode);
 printk(KERN_INFO "%s: %s %s: read_mode\t\t= %d\n", name, s1, s5, read_mode);
 printk(KERN_INFO "%s: %s %s: capt_options\t= 0x%x\n", name, s1, s5, capt_options);
    }
    if (dev && dodope) {
 v4l2_info(&dev->v4l2_dev, "%s %s unit\t\t\t= %u\n", s1, s3, dev->pxdrv_unit);
 for (mosp = mosdope; mosp->name; mosp++)
     v4l2_info(&dev->v4l2_dev, "%s %s %s\t= %u\n", s1, s3, mosp->name, ((u32*)&dev->pxdrv_v4l2dope)[(mosp->offs&OFS)/sizeof(u32)]);
    }
    if (dev && doformat) {
 for (mosp = mosfmt; mosp->name; mosp++) {
     if (mosp->offs & HEX)
  v4l2_info(&dev->v4l2_dev, "%s %s %s\t= 0x%x\n", s1, s2, mosp->name, ((u32*)&dev->pxdrv_v4l2fmt)[(mosp->offs&OFS)/sizeof(u32)]);
     else
  v4l2_info(&dev->v4l2_dev, "%s %s %s\t= %u\n", s1, s2, mosp->name, ((u32*)&dev->pxdrv_v4l2fmt)[(mosp->offs&OFS)/sizeof(u32)]);
 }
    }
    if (dev && doinfo) {
 #define CtoC(c) ((c)>=0x20 && (c)<0x7F? (c): ' ')
 v4l2_info(&dev->v4l2_dev, "%s v4l2: fourcc\t\t= 0x%.8x (%c%c%c%c)\n",
      s1, dev->v4l2_fourcc,
      CtoC((dev->v4l2_fourcc>>24)&0xFF), CtoC((dev->v4l2_fourcc>>16)&0xFF),
      CtoC((dev->v4l2_fourcc>>8)&0xFF), CtoC((dev->v4l2_fourcc>>0)&0xFF));
    }
    if (dev && dostatus) {
 for (mosp = mosstat; mosp->name; mosp++)
     v4l2_info(&dev->v4l2_dev, "%s %s %s\t= %u\n", s1, s4, mosp->name, ((u8*)&dev->pxdrv_v4l2status)[(mosp->offs&OFS)/sizeof(u8)]);
    }
    #undef HEX
    #undef DEC
    #undef OFS
}
static int
pxdrvv4l2_log_status(struct file *filep, void *fh)
{
    struct pxdrvv4l2_dev *dev = video_drvdata(filep);
    pxdrvv4l2_reportfault(dev);
    pxdrvv4l2_reportformat(dev, true, true, true, true, true);
    return(v4l2_ctrl_log_status(filep, fh));
}
static int
pxdrvv4l2_isbusy(struct pxdrvv4l2_dev *dev)
{
    if (dev->bufstreamon)
 return(true);
    if (dev->readsnapped)
 return(true);
    return(false);
}
static int
pxdrvv4l2_isinitialized(struct pxdrvv4l2_dev *dev)
{
    if (dev->pxdrv_v4l2fmt.imagexdim == 0
     || dev->pxdrv_v4l2fmt.imageydim == 0)
 return(false);
    return(true);
}
static int
pxdrvv4l2_querycap(struct file *file, void *priv, struct v4l2_capability *cap)
{
    struct pxdrvv4l2_dev *dev = video_drvdata(file);
    memset(cap, 0, sizeof(*cap));
    strlcpy(cap->driver, "pixci" "_v4l2", sizeof(cap->driver));
    snprintf(cap->card, sizeof(cap->card), "%s: %d", "PIXCI(R)", dev->pxdrv_unit);
    snprintf(cap->bus_info, sizeof(cap->bus_info), "platform: %s", dev->v4l2_dev.name);
    #if 1
 cap->device_caps = V4L2_CAP_VIDEO_CAPTURE
     | V4L2_CAP_READWRITE
     | (stream_mode? V4L2_CAP_STREAMING: 0);
 cap->capabilities = cap->device_caps | V4L2_CAP_DEVICE_CAPS;
 cap->version = KERNEL_VERSION(3, 8, 01);
    #endif
    return(0);
}
static int
pxdrvv4l2_enum_fmt_vid_cap(struct file *file, void *priv, struct v4l2_fmtdesc *fmtp)
{
    struct pxdrvv4l2_dev *dev = video_drvdata(file);
    if (!pxdrvv4l2_isinitialized(dev))
 return(-EFAULT);
    if (fmtp->index > 0)
 return(-EINVAL);
    if (fmtp->type != V4L2_BUF_TYPE_VIDEO_CAPTURE)
 return(-EINVAL);
    fmtp->pixelformat = dev->v4l2_fourcc;
    strlcpy(fmtp->description, "Default", sizeof(fmtp->description));
    fmtp->flags = 0;
    return((fmtp->pixelformat&CUSTOM_FOURCC)? 1: 0);
}
static int
pxdrvv4l2_g_fmt_vid_cap(struct file *file, void *priv, struct v4l2_format *fmtp)
{
    struct pxdrvv4l2_dev *dev = video_drvdata(file);
    if (!pxdrvv4l2_isinitialized(dev))
 return(-EFAULT);
    if (fmtp->type != V4L2_BUF_TYPE_VIDEO_CAPTURE)
 return(-EINVAL);
    fmtp->fmt.pix.width = dev->pxdrv_v4l2fmt.imagexdim;
    fmtp->fmt.pix.height = dev->pxdrv_v4l2fmt.imageydim;
    fmtp->fmt.pix.field = dev->pxdrv_v4l2fmt.imageylace==0? V4L2_FIELD_NONE: V4L2_FIELD_INTERLACED;
    fmtp->fmt.pix.field = V4L2_FIELD_NONE;
    fmtp->fmt.pix.pixelformat = dev->v4l2_fourcc;
    fmtp->fmt.pix.bytesperline = 0;
    fmtp->fmt.pix.bytesperline = dev->pxdrv_v4l2fmt.imagelinelen;
    fmtp->fmt.pix.sizeimage = dev->pxdrv_v4l2fmt.imagesize
       + ((data_mode&DATA_FIELDSTAMP)? dev->pxdrv_v4l2fmt.framestampsize: 0);
    fmtp->fmt.pix.priv = 0;
    fmtp->fmt.pix.colorspace = V4L2_COLORSPACE_SRGB;
    if (dev->pxdrv_v4l2dope.model == PIXCI_SV4
     || dev->pxdrv_v4l2dope.model == PIXCI_SV5)
 fmtp->fmt.pix.colorspace = V4L2_COLORSPACE_BT878;
    #if LINUX_VERSION_CODE >= KERNEL_VERSION(4,5,0)
 fmtp->fmt.pix.flags = 0;
 fmtp->fmt.pix.ycbcr_enc = 0;
 fmtp->fmt.pix.quantization = V4L2_QUANTIZATION_DEFAULT;
 fmtp->fmt.pix.xfer_func = 0;
    #endif
    return(0);
}
static int
pxdrvv4l2_try_fmt_vid_cap(struct file *file, void *priv, struct v4l2_format *fmtp)
{
    struct pxdrvv4l2_dev *dev = video_drvdata(file);
    struct v4l2_format f;
    if (!pxdrvv4l2_isinitialized(dev))
 return(-EFAULT);
    if (fmtp->type != V4L2_BUF_TYPE_VIDEO_CAPTURE)
 return(-EINVAL);
    memset(&f, 0, sizeof(f));
    f.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    pxdrvv4l2_g_fmt_vid_cap(file, priv, &f);
    if (0) {
 __u32 bytesperline;
 bytesperline = dev->pxdrv_v4l2fmt.imagelinelen;
 if ( ( f.fmt.pix.field != fmtp->fmt.pix.field
     && V4L2_FIELD_ANY != fmtp->fmt.pix.field)
  || f.fmt.pix.pixelformat != fmtp->fmt.pix.pixelformat
  || f.fmt.pix.colorspace != fmtp->fmt.pix.colorspace
  || f.fmt.pix.sizeimage != fmtp->fmt.pix.sizeimage
  || ( f.fmt.pix.bytesperline != fmtp->fmt.pix.bytesperline
      && 0 != fmtp->fmt.pix.bytesperline
      && bytesperline != fmtp->fmt.pix.bytesperline) ) {
     return(-EINVAL);
 }
    }
    *fmtp = f;
    return(0);
}
static int
pxdrvv4l2_s_fmt_vid_cap(struct file *file, void *priv, struct v4l2_format *fmtp)
{
    struct pxdrvv4l2_dev *dev = video_drvdata(file);
    if (!pxdrvv4l2_isinitialized(dev))
 return(-EFAULT);
    if (fmtp->type != V4L2_BUF_TYPE_VIDEO_CAPTURE)
 return(-EINVAL);
    if (pxdrvv4l2_isbusy(dev))
 return(-EBUSY);
    return(0);
}
static int
pxdrvv4l2_enum_framesizes(struct file *file, void *fh, struct v4l2_frmsizeenum *fsize)
{
    struct pxdrvv4l2_dev *dev = video_drvdata(file);
    if (!pxdrvv4l2_isinitialized(dev))
 return(-EFAULT);
    if (fsize->index)
 return(-EINVAL);
    if (fsize->pixel_format != dev->v4l2_fourcc)
 return(-EINVAL);
    fsize->type = V4L2_FRMSIZE_TYPE_DISCRETE;
    fsize->discrete.width = dev->pxdrv_v4l2fmt.imagexdim;
    fsize->discrete.height = dev->pxdrv_v4l2fmt.imageydim;
    return(0);
}
static int
pxdrvv4l2_enum_frameintervals(struct file *file, void *priv, struct v4l2_frmivalenum *fival)
{
    struct pxdrvv4l2_dev *dev = video_drvdata(file);
    if (!pxdrvv4l2_isinitialized(dev))
 return(-EFAULT);
    if (fival->index)
 return(-EINVAL);
    fival->type = V4L2_FRMIVAL_TYPE_DISCRETE;
    fival->width = dev->pxdrv_v4l2fmt.imagexdim;
    fival->height = dev->pxdrv_v4l2fmt.imageydim;
    fival->pixel_format = dev->v4l2_fourcc;
    fival->stepwise.step = (struct v4l2_fract) {1, 1};
    fival->discrete.numerator = dev->pxdrv_v4l2fmt.framerate[1];
    fival->discrete.denominator = dev->pxdrv_v4l2fmt.framerate[0];
    return(0);
}
static int
pxdrvv4l2_g_parm(struct file *file, void *priv, struct v4l2_streamparm *parmp)
{
    struct pxdrvv4l2_dev *dev = video_drvdata(file);
    if (!pxdrvv4l2_isinitialized(dev))
 return(-EFAULT);
    if (parmp->type != V4L2_BUF_TYPE_VIDEO_CAPTURE)
 return(-EINVAL);
    parmp->parm.capture.capability = V4L2_CAP_TIMEPERFRAME;
    parmp->parm.capture.capability = 0;
    parmp->parm.capture.capturemode = V4L2_MODE_HIGHQUALITY;
    parmp->parm.capture.timeperframe.numerator = dev->pxdrv_v4l2fmt.framerate[1];
    parmp->parm.capture.timeperframe.denominator = dev->pxdrv_v4l2fmt.framerate[0];
    if (stream_mode)
 parmp->parm.capture.readbuffers = 1;
    else
 parmp->parm.capture.readbuffers = dev->pxdrv_framebuffers;
    return(0);
}
static int
pxdrvv4l2_s_parm(struct file *file, void *priv, struct v4l2_streamparm *parm)
{
    struct pxdrvv4l2_dev *dev = video_drvdata(file);
    if (!pxdrvv4l2_isinitialized(dev))
 return(-EFAULT);
    return(0);
}
static int
pxdrvv4l2_enum_input(struct file *file, void *priv, struct v4l2_input *inputp)
{
    struct pxdrvv4l2_dev *dev = video_drvdata(file);
    if (!pxdrvv4l2_isinitialized(dev))
 return(-EFAULT);
    if (inputp->index > 0)
 return(-EINVAL);
    inputp->type = V4L2_INPUT_TYPE_CAMERA;
    strlcpy(inputp->name, "Video", sizeof(inputp->name));
    inputp->std = dev->v4l2_stdid;
    inputp->status = 0;
    if (pxdrv_v4l2getstatus(dev, dev->pxdrv_plp, &dev->pxdrv_v4l2status, sizeof(dev->pxdrv_v4l2status)) >= 0) {
 if (dev->pxdrv_v4l2status.no_signal)
     inputp->status |= V4L2_IN_ST_NO_SIGNAL;
 if (dev->pxdrv_v4l2status.no_color)
     inputp->status |= V4L2_IN_ST_NO_COLOR;
 if (dev->pxdrv_v4l2status.no_htiming)
     inputp->status |= V4L2_IN_ST_NO_H_LOCK;
 if (dev->pxdrv_v4l2status.no_vtiming)
     ;
 if (dev->pxdrv_v4l2status.colorkill)
     inputp->status |= V4L2_IN_ST_COLOR_KILL;
 if (dev->pxdrv_v4l2status.macrovision)
     inputp->status |= V4L2_IN_ST_MACROVISION;
    }
    inputp->tuner = 0;
    inputp->audioset = 0;
    if (inputp->std)
 inputp->capabilities = V4L2_IN_CAP_STD;
    else {
 inputp->capabilities = 0;
    }
    return(0);
}
static int
pxdrvv4l2_s_input(struct file *file, void *priv, unsigned int i)
{
    struct pxdrvv4l2_dev *dev = video_drvdata(file);
    if (!pxdrvv4l2_isinitialized(dev))
 return(-EFAULT);
    if (i > 0)
 return(-EINVAL);
    if (pxdrvv4l2_isbusy(dev))
 return(-EBUSY);
    return(0);
}
static int
pxdrvv4l2_g_input(struct file *file, void *priv, unsigned int *inputp)
{
    struct pxdrvv4l2_dev *dev = video_drvdata(file);
    if (!pxdrvv4l2_isinitialized(dev))
 return(-EFAULT);
    *inputp = 0;
    return(0);
}
static int
pxdrvv4l2_g_std(struct file *file, void *fh, v4l2_std_id *normp)
{
    struct pxdrvv4l2_dev *dev = video_drvdata(file);
    if (!pxdrvv4l2_isinitialized(dev))
 return(-EFAULT);
    if (dev->v4l2_stdid == 0)
 return(-EINVAL);
    *normp = dev->v4l2_stdid;
    return(0);
}
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,5,0)
static int
pxdrvv4l2_s_std(struct file *file, void *fh, v4l2_std_id norm)
#else
static int
pxdrvv4l2_s_std(struct file *file, void *fh, v4l2_std_id *norm)
#endif
{
    struct pxdrvv4l2_dev *dev = video_drvdata(file);
    if (!pxdrvv4l2_isinitialized(dev))
 return(-EFAULT);
    if (dev->v4l2_stdid == 0)
 return(-EINVAL);
    #if LINUX_VERSION_CODE >= KERNEL_VERSION(4,5,0)
 if (norm != dev->v4l2_stdid)
     return(-EINVAL);
    #else
 if (*norm != dev->v4l2_stdid)
     return(-EINVAL);
    #endif
    return(0);
}
static int
pxdrvv4l2_querystd(struct file *file, void *fh, v4l2_std_id *normp)
{
    struct pxdrvv4l2_dev *dev = video_drvdata(file);
    if (!pxdrvv4l2_isinitialized(dev))
 return(-EFAULT);
    if (dev->v4l2_stdid == 0)
 return(-EINVAL);
    *normp = dev->v4l2_stdid;
    return(0);
}
#if LINUX_VERSION_CODE < KERNEL_VERSION(5,3,0)
static int
pxdrvv4l2_cropcap(struct file *file, void *__fh, struct v4l2_cropcap *cap)
{
    struct pxdrvv4l2_dev *dev = video_drvdata(file);
    if (!pxdrvv4l2_isinitialized(dev))
 return(-EFAULT);
    if (cap->type != V4L2_BUF_TYPE_VIDEO_CAPTURE)
 return(-EINVAL);
    cap->bounds.left = 0;
    cap->bounds.top = 0;
    cap->bounds.width = dev->pxdrv_v4l2fmt.imagexdim;
    cap->bounds.height = dev->pxdrv_v4l2fmt.imageydim;
    memcpy(&cap->defrect, &cap->bounds, sizeof(struct v4l2_rect));
    cap->pixelaspect.numerator = dev->pxdrv_v4l2fmt.pixelaspect[1];
    cap->pixelaspect.denominator = dev->pxdrv_v4l2fmt.pixelaspect[0];
    return(0);
}
#else
static int
pxdrvv4l2_g_pixelaspect(struct file *file, void *priv, int type, struct v4l2_fract *fractp)
{
    struct pxdrvv4l2_dev *dev = video_drvdata(file);
    if (!pxdrvv4l2_isinitialized(dev))
 return(-EFAULT);
    fractp->numerator = dev->pxdrv_v4l2fmt.pixelaspect[1];
    fractp->denominator = dev->pxdrv_v4l2fmt.pixelaspect[0];
    return(0);
}
#endif
#if LINUX_VERSION_CODE < KERNEL_VERSION(5,3,0)
static int
pxdrvv4l2_g_crop(struct file *file, void *__fh, struct v4l2_crop *crop)
{
    struct pxdrvv4l2_dev *dev = video_drvdata(file);
    if (!pxdrvv4l2_isinitialized(dev))
 return(-EFAULT);
    if (crop->type != V4L2_BUF_TYPE_VIDEO_CAPTURE)
 return(-EINVAL);
    crop->c.left = 0;
    crop->c.top = 0;
    crop->c.width = dev->pxdrv_v4l2fmt.imagexdim;
    crop->c.height = dev->pxdrv_v4l2fmt.imageydim;
    return(0);
}
static int
pxdrvv4l2_s_crop(struct file *file, void *__fh, const struct v4l2_crop *crop)
{
    struct pxdrvv4l2_dev *dev = video_drvdata(file);
    if (!pxdrvv4l2_isinitialized(dev))
 return(-EFAULT);
    if (crop->type != V4L2_BUF_TYPE_VIDEO_CAPTURE)
 return(-EINVAL);
    return(-EINVAL);
}
#endif
static int
pxdrvv4l2_s_ctrl(struct v4l2_ctrl *ctrl)
{
    struct pxdrvv4l2_dev *dev = container_of(ctrl->handler, struct pxdrvv4l2_dev, ctrl_handler);
    int r;
    if ((ctrl->id-CID_CUSTOM_BASE) < 0
     || (ctrl->id-CID_CUSTOM_BASE) >= (dev->ctrlfgn+NLAYERCTRL))
 return(-EINVAL);
    if (ctrl->id-CID_CUSTOM_BASE == 0) {
 read_mode = ctrl->val;
 return(0);
    }
    r = pxdrv_v4l2control(dev, dev->pxdrv_plp, ctrl->id-CID_CUSTOM_BASE-NLAYERCTRL, PXDAI_II_CTRL_SET, ctrl->val);
    return(r);
}
static int
pxdrvv4l2_g_volatile_ctrl(struct v4l2_ctrl *ctrl)
{
    struct pxdrvv4l2_dev *dev = container_of(ctrl->handler, struct pxdrvv4l2_dev, ctrl_handler);
    int r;
    if ((ctrl->id-CID_CUSTOM_BASE) < 0
     || (ctrl->id-CID_CUSTOM_BASE) >= (dev->ctrlfgn+NLAYERCTRL))
 return(-EINVAL);
    if (ctrl->id-CID_CUSTOM_BASE == 0) {
 ctrl->val = dev->readmode;
 return(0);
    }
    r = pxdrv_v4l2control(dev, dev->pxdrv_plp, ctrl->id-CID_CUSTOM_BASE, PXDAI_II_CTRL_GET, ctrl->val);
    ctrl->val = r;
    return(0);
}
static int
#if LINUX_VERSION_CODE < KERNEL_VERSION(4,5,0)
pxdrvv4l2_queue_setup(struct vb2_queue *vq,
        const struct v4l2_format *fmt,
        unsigned int *num_buffers,
        unsigned int *num_planes,
        unsigned int sizes[],
        void *alloc_ctxs[])
#else
pxdrvv4l2_queue_setup(struct vb2_queue *vq,
        unsigned int *num_buffers,
        unsigned int *num_planes,
        unsigned int sizes[],
        struct device *alloc_devs[])
#endif
{
    struct pxdrvv4l2_dev *dev = vb2_get_drv_priv(vq);
    if (!pxdrvv4l2_isinitialized(dev))
 return(-EFAULT);
    #if LINUX_VERSION_CODE < KERNEL_VERSION(4,5,0)
 if (*num_buffers == 0)
     *num_buffers = stream_buffers;
 *num_buffers = min(*num_buffers, dev->pxdrv_v4l2dope.framequeues);
 sizes[0] = dev->pxdrv_v4l2fmt.framesize;
 *num_planes = 1;
 alloc_ctxs[0] = dev->alloc_ctx;
    #else
 if (*num_planes == 0) {
     *num_buffers = min(stream_buffers, dev->pxdrv_v4l2dope.framequeues);
     *num_planes = 1;
     sizes[0] = dev->pxdrv_v4l2fmt.framesize;
 } else {
     if (sizes[0] != dev->pxdrv_v4l2fmt.framesize)
  return(-EINVAL);
     if (*num_planes != 1)
  return(-EINVAL);
     if ((vq->num_buffers + *num_buffers) > dev->pxdrv_v4l2dope.framequeues)
  return(-EINVAL);
 }
    #endif
    return(0);
}
static void
pxdrvv4l2_buffer_worker(struct work_struct *worker)
{
    struct pxdrvv4l2_buffer *buf = container_of(worker, struct pxdrvv4l2_buffer, work);
    vb2_buffer_done(&buf->vb, buf->workstatus);
}
#if 0
static int
pxdrvv4l2_buffer_init(struct vb2_buffer *vb)
{
    struct pxdrvv4l2_dev *dev = vb2_get_drv_priv(vb->vb2_queue);
    struct pxdrvv4l2_buffer *buf = container_of(vb, struct pxdrvv4l2_buffer, vb);
    if (!pxdrvv4l2_isinitialized(dev))
 return(-EFAULT);
    buf->tracker = ++dev->buftrackgen;
    if (!(capt_options&CAPT_BUFDONE_IRQ))
 INIT_WORK(&buf->work, pxdrvv4l2_buffer_worker);
    return(0);
}
#endif
#if LINUX_VERSION_CODE < KERNEL_VERSION(4,5,0)
static int
#else
static void
#endif
pxdrvv4l2_buffer_finish(struct vb2_buffer *vb)
{
    struct pxdrvv4l2_dev *dev = vb2_get_drv_priv(vb->vb2_queue);
    #if LINUX_VERSION_CODE < KERNEL_VERSION(4,5,0)
 char *vbuf = vb2_plane_vaddr(vb, 0);
 if (dev->pxdrv_v4l2fmt.framestampsize != 0 && (data_mode&DATA_FIELDSTAMP)==0) {
     memcpy(vbuf, vbuf+dev->pxdrv_v4l2fmt.framestampsize,
     dev->pxdrv_v4l2fmt.imagesize);
 }
    #endif
    #if 0
 if (dev->pxdrv_v4l2dope.ticku[0] == 1
  && dev->pxdrv_v4l2dope.ticku[1] == 1) {
     vb->v4l2_buf.timestamp.tv_sec = buf->ticks[0];
     vb->v4l2_buf.timestamp.tv_usec = buf->ticks[1];
 } else if (dev->pxdrv_v4l2dope.ticku[0] == 1
  && dev->pxdrv_v4l2dope.ticku[1] == 1000) {
     vb->v4l2_buf.timestamp.tv_sec = buf->ticks[0];
     vb->v4l2_buf.timestamp.tv_usec = buf->ticks[1]/1000;
 } else if (dev->pxdrv_v4l2dope.ticku[0] == 1000000) {
     u64 usec = (1000000*((u64)buf->ticks[0]+((u64)buf->ticks[1]<<32))/HZ;
     vb->v4l2_buf.timestamp.tv_sec = usec/1000;
     vb->v4l2_buf.timestamp.tv_usec = usec - (u64)vb->v4l2_buf.timestamp.tv_sec*1000;
 }
    #endif
    #if LINUX_VERSION_CODE >= KERNEL_VERSION(4,5,0)
 struct pxdrvv4l2_buffer *buf = container_of(vb, struct pxdrvv4l2_buffer, vb);
 if (dev->pxdrv_v4l2dope.ticku[0] == 1
  && dev->pxdrv_v4l2dope.ticku[1] == 1) {
     vb->timestamp = ((u64)buf->ticks[0]+((u64)buf->ticks[1]<<32))*1000;
 } else if (dev->pxdrv_v4l2dope.ticku[0] == 1
  && dev->pxdrv_v4l2dope.ticku[1] == 1000) {
     vb->timestamp = (u64)buf->ticks[0]+((u64)buf->ticks[1]<<32);
 } else if (dev->pxdrv_v4l2dope.ticku[0] == 1000000) {
     vb->timestamp = (1000000000*((u64)buf->ticks[0]+((u64)buf->ticks[1]<<32) ))/HZ;
 }
    #endif
    #if 0
 if (dev->pxdrv_v4l2fmt.video_fieldmap == 3)
     buf->vfield >>= 1;
 vb->v4l2_buf.sequence = buf->vfield;
    #endif
    #if LINUX_VERSION_CODE < KERNEL_VERSION(4,5,0)
 return(0);
    #endif
}
static int
pxdrvv4l2_buffer_prepare(struct vb2_buffer *vb)
{
    struct pxdrvv4l2_dev *dev = vb2_get_drv_priv(vb->vb2_queue);
    struct pxdrvv4l2_buffer *buf = container_of(vb, struct pxdrvv4l2_buffer, vb);
    if (vb2_plane_size(vb, 0) < dev->pxdrv_v4l2fmt.framesize) {
 v4l2_err(&dev->v4l2_dev, "buffer too small (%lu < %lu)\n",
   (unsigned long) vb2_plane_size(vb, 0),
   (unsigned long) dev->pxdrv_v4l2fmt.framesize);
 return(-EINVAL);
    }
    vb2_set_plane_payload(vb, 0, dev->pxdrv_v4l2fmt.imagesize);
    #if LINUX_VERSION_CODE >= KERNEL_VERSION(4,5,0)
 if (dev->pxdrv_v4l2fmt.framestampsize != 0 && (data_mode&DATA_FIELDSTAMP)==0) {
     if (0 < vb->num_planes)
  vb->planes[0].data_offset = dev->pxdrv_v4l2fmt.framestampsize;
 }
    #endif
    if (dev->buftrackgen == 0)
 dev->buftrackgen++;
    buf->tracker = dev->buftrackgen++;
    if (!(capt_options&CAPT_BUFDONE_IRQ)) {
 INIT_WORK(&buf->work, pxdrvv4l2_buffer_worker);
    }
    return(0);
}
static void
pxdrvv4l2_buffer_queue(struct vb2_buffer *vb)
{
    int r;
    struct pxdrvv4l2_dev *dev = vb2_get_drv_priv(vb->vb2_queue);
    struct pxdrvv4l2_buffer *buf = container_of(vb, struct pxdrvv4l2_buffer, vb);
    unsigned long flags;
    if (!pxdrvv4l2_isinitialized(dev))
 return;
    pxdrvv4l2_reportfault(dev);
    if (dev->bufstreamon) {
 spin_lock_irqsave(&dev->bufqueuelock, flags);
 list_add_tail(&buf->list, &dev->bufqueue);
 spin_unlock_irqrestore(&dev->bufqueuelock, flags);
  r = pxdrv_v4l2video(dev, dev->pxdrv_plp, XCLIVE_LIVEQUEUE, 0, buf->tracker, vb2_dma_contig_plane_dma_addr(vb,0), 0);
 if (r < 0) {
     spin_lock_irqsave(&dev->bufqueuelock, flags);
     list_del(&buf->list);
     spin_unlock_irqrestore(&dev->bufqueuelock, flags);
     vb2_buffer_done(vb, VB2_BUF_STATE_QUEUED);
     v4l2_err(&dev->v4l2_dev, "live queue error (%d)\n", r);
 }
    } else {
 spin_lock_irqsave(&dev->bufprequeuelock, flags);
 list_add_tail(&buf->list, &dev->bufprequeue);
 spin_unlock_irqrestore(&dev->bufprequeuelock, flags);
    }
}
static int
pxdrvv4l2_start_streaming(struct vb2_queue *vq, unsigned int count)
{
    struct pxdrvv4l2_dev *dev = vb2_get_drv_priv(vq);
    struct pxdrvv4l2_buffer *buf;
    unsigned long flags;
    if (!pxdrvv4l2_isinitialized(dev))
 return(-EFAULT);
    dev->bufstreamon = 1;
    for (;;) {
 spin_lock_irqsave(&dev->bufprequeuelock, flags);
 if (list_empty(&dev->bufprequeue)) {
     spin_unlock_irqrestore(&dev->bufprequeuelock, flags);
     break;
 }
 buf = list_entry(dev->bufprequeue.next, struct pxdrvv4l2_buffer, list);
 list_del(&buf->list);
 spin_unlock_irqrestore(&dev->bufprequeuelock, flags);
 pxdrvv4l2_buffer_queue(&buf->vb);
    }
    return(0);
}
#if LINUX_VERSION_CODE < KERNEL_VERSION(4,5,0)
static int
#else
static void
#endif
pxdrvv4l2_stop_streaming(struct vb2_queue *vq)
{
    struct pxdrvv4l2_dev *dev = vb2_get_drv_priv(vq);
    struct pxdrvv4l2_buffer *buf;
    unsigned long flags;
    dev->bufstreamon = 0;
    pxdrv_v4l2video(dev, dev->pxdrv_plp, XCLIVE_ABORTLIVE, 0, 0, 0, 0);
    for (;;) {
 spin_lock_irqsave(&dev->bufqueuelock, flags);
 if (list_empty(&dev->bufqueue)) {
     spin_unlock_irqrestore(&dev->bufqueuelock, flags);
     break;
 }
 buf = list_entry(dev->bufqueue.next, struct pxdrvv4l2_buffer, list);
 list_del(&buf->list);
 spin_unlock_irqrestore(&dev->bufqueuelock, flags);
 vb2_buffer_done(&buf->vb, VB2_BUF_STATE_ERROR);
    }
    pxdrvv4l2_reportfault(dev);
    #if LINUX_VERSION_CODE < KERNEL_VERSION(4,5,0)
 return(0);
    #endif
}
static const struct v4l2_ctrl_ops pxdrvv4l2_ctrl_ops = {
    .s_ctrl = pxdrvv4l2_s_ctrl,
    .g_volatile_ctrl = pxdrvv4l2_g_volatile_ctrl,
};
static const char * const pxdrvv4l2_ctrl_read_mode_menu[] = {
    "",
    "Snap",
    "Live-Newest",
    "Live-Oldest",
    NULL,
};
static int
pxdrvv4l2_init_controls(struct pxdrvv4l2_dev *dev)
{
    struct v4l2_ctrl_config ctrlconfigs;
    struct v4l2_ctrl_handler *hdl = NULL;
    int r, ctrlfgn, i, j;
    char buf1[2];
    if (dev->v4l2_dev.ctrl_handler)
 return(0);
    for (ctrlfgn = 0; ctrlfgn < MAXCTRL-NLAYERCTRL; ctrlfgn++) {
 if (pxdrv_v4l2controllabel(dev, dev->pxdrv_plp, ctrlfgn, buf1, sizeof(buf1)-1) < 0)
     break;
    }
    dev->ctrlfgn = ctrlfgn;
    hdl = &dev->ctrl_handler;
    v4l2_ctrl_handler_init(hdl, ctrlfgn+NLAYERCTRL);
    #if NLAYERCTRL != 1
 #error
    #endif
    i = 0;
    memset(&ctrlconfigs, 0, sizeof(ctrlconfigs));
    ctrlconfigs.type = V4L2_CTRL_TYPE_MENU;
    ctrlconfigs.name = "Non-Streaming Read Mode";
    ctrlconfigs.min = 1;
    ctrlconfigs.max = 3;
    ctrlconfigs.def = dev->readmode!=0? dev->readmode: read_mode;
    ctrlconfigs.def = min(ctrlconfigs.max, max(ctrlconfigs.min, ctrlconfigs.def));
    ctrlconfigs.flags = (stream_mode)? V4L2_CTRL_FLAG_DISABLED: 0;
    ctrlconfigs.qmenu = pxdrvv4l2_ctrl_read_mode_menu;
    ctrlconfigs.id = CID_CUSTOM_BASE + i;
    ctrlconfigs.ops = &pxdrvv4l2_ctrl_ops;
    dev->ctrls[i] = v4l2_ctrl_new_custom(hdl, &ctrlconfigs, NULL);
    i++;
    for (j = 0; j < ctrlfgn && i < MAXCTRL; j++, i++) {
 char *p; int z;
 memset(&ctrlconfigs, 0, sizeof(ctrlconfigs));
 ctrlconfigs.step = 1;
 ctrlconfigs.type = V4L2_CTRL_TYPE_INTEGER;
 z = pxdrv_v4l2controllabel(dev, dev->pxdrv_plp, j, buf1, sizeof(buf1)-1);
 if (z > 0)
     dev->ctrlnames[i] = kmalloc(z, GFP_KERNEL);
 if (!dev->ctrlnames[i])
     continue;
 pxdrv_v4l2controllabel(dev, dev->pxdrv_plp, j, dev->ctrlnames[i], z-1);
 dev->ctrlnames[i][z-1] = 0;
 for (p = dev->ctrlnames[i]; *p && *p != ';'; p++) {
     if (*p == 'r')
  ctrlconfigs.flags |= V4L2_CTRL_FLAG_READ_ONLY;
     if (*p == 'v')
  ctrlconfigs.flags |= V4L2_CTRL_FLAG_VOLATILE;
     if (*p == 'M') {
  ctrlconfigs.type = V4L2_CTRL_TYPE_BITMASK;
  ctrlconfigs.step = 0;
     }
     if (*p == 'L') {
  ctrlconfigs.type = V4L2_CTRL_TYPE_MENU;
  ctrlconfigs.step = 0;
  ctrlconfigs.menu_skip_mask = 0;
     }
 }
 if (*p == ';')
     p++;
 ctrlconfigs.name = p;
 if (ctrlconfigs.type == V4L2_CTRL_TYPE_MENU) {
     int n; char **pp, *p2;
     for ( ; *p && *p != ';'; p++) ;
     if (*p == ';')
  *p++ = 0;
     for (n = 1, p2 = p; *p2; )
  if (*p2++ == ';')
      n++;
     dev->ctrlmenus[i] = kzalloc( (n+1) * sizeof(*pp), GFP_KERNEL );
     if (!dev->ctrlmenus[i])
  continue;
     for (pp = dev->ctrlmenus[i]; n--; ) {
  *pp++ = p;
  for ( ; *p && *p != ';'; p++) ;
  if (*p == ';')
      *p++ = 0;
     }
     ctrlconfigs.qmenu = (const char * const *)dev->ctrlmenus[i];
 }
 ctrlconfigs.min = pxdrv_v4l2control(dev, dev->pxdrv_plp, j, PXDAI_II_CTRL_MIN, 0);
 ctrlconfigs.max = pxdrv_v4l2control(dev, dev->pxdrv_plp, j, PXDAI_II_CTRL_MAX, 0);
 ctrlconfigs.def = pxdrv_v4l2control(dev, dev->pxdrv_plp, j, PXDAI_II_CTRL_GET, 0);
 ctrlconfigs.def = min(ctrlconfigs.max, max(ctrlconfigs.min, ctrlconfigs.def));
 ctrlconfigs.id = CID_CUSTOM_BASE + i;
 ctrlconfigs.ops = &pxdrvv4l2_ctrl_ops;
 dev->ctrls[i] = v4l2_ctrl_new_custom(hdl, &ctrlconfigs, NULL);
    }
    if ((r = hdl->error)) {
 v4l2_ctrl_handler_free(hdl);
 return(r);
    }
    dev->v4l2_dev.ctrl_handler = hdl;
    return(0);
}
static void
pxdrvv4l2_readmodeinit(struct pxdrvv4l2_dev *dev, int readmode)
{
    int i, r = 0, r1;
    dev->readmode = readmode;
    if (dev->readmode == READMODE_SNAP)
 return;
    if (dev->readmode == READMODE_LIVENEW) {
 if (dev->pxdrv_framebuffers < READLIVENEW_MINBUFFERS)
     v4l2_warn(&dev->v4l2_dev, "fewer than %d buffers for live read mode\n", READLIVENEW_MINBUFFERS);
    }
    if (dev->readmode == READMODE_LIVEOLD) {
 if (dev->pxdrv_framebuffers < READLIVEOLD_RSVDBUFFERS)
     v4l2_warn(&dev->v4l2_dev, "fewer than %d buffers for live read mode\n", READLIVEOLD_RSVDBUFFERS);
    }
    if (dev->readmode == READMODE_LIVENEW
     || dev->readmode == READMODE_LIVEOLD) {
 dev->readlist = kmalloc_array( (1+dev->pxdrv_framebuffers), sizeof(*dev->readlist),
           GFP_KERNEL | __GFP_ZERO);
 if (!dev->readlist) {
     v4l2_err(&dev->v4l2_dev, "can't allocate 'live' buffer list");
     dev->readmode = READMODE_SNAP;
     return;
 }
 INIT_LIST_HEAD(&dev->readlist[0].list);
 for (i = 1; i < dev->pxdrv_framebuffers; i++) {
     dev->readlist[i].fgbufindex = i;
     r1 = pxdrv_v4l2video(dev, dev->pxdrv_plp, XCLIVE_LIVEQUEUE, i, 0, 0, 0);
     r = min(r, r1);
 }
    }
    if (r < 0)
 v4l2_err(&dev->v4l2_dev, "live queue error (%d)\n", r);
}
static int
pxdrvv4l2_fh_open(struct file *filep)
{
    int r = 0, i, singular = 1;
    struct pxdrvv4l2_dev *dev = video_drvdata(filep);
    mutex_lock(&dev->mutex);
    #if SUPPORT_FH
 r = v4l2_fh_open(filep);
 if (r < 0)
     goto err_fh;
 singular = v4l2_fh_is_singular_file(filep);
    #endif
    if (singular || dev->pxdrv_plp == NULL) {
 dev->pxdrv_plp = pxdrv_v4l2open(dev, dev->pxdrv_unit, (stream_mode&STREAM_MEMMAP)? stream_buffers: -1);
 if (!dev->pxdrv_plp) {
     v4l2_err(&dev->v4l2_dev, "frame grabber initialization error\n");
     pxdrvv4l2_reportfault(dev);
     r = -ENODEV;
     goto err_pxdrv;
 }
 pxdrv_v4l2getformat(dev, dev->pxdrv_plp, &dev->pxdrv_v4l2fmt, sizeof(dev->pxdrv_v4l2fmt));
 pxdrv_v4l2getdope (dev, dev->pxdrv_plp, &dev->pxdrv_v4l2dope, sizeof(dev->pxdrv_v4l2dope));
 pxdrv_v4l2getstatus(dev, dev->pxdrv_plp, &dev->pxdrv_v4l2status, sizeof(dev->pxdrv_v4l2status));
 dev->v4l2_fourcc = pxdrvv4l2_pixelhintToFourCC(dev->pxdrv_v4l2fmt.pixelhint,
       dev->pxdrv_v4l2fmt.imagemultitap,
       dev->pxdrv_v4l2fmt.pixiebits,
       dev->pxdrv_v4l2fmt.pixelbitpack,
       dev->pxdrv_v4l2fmt.framestampsize);
 dev->v4l2_stdid = pxdrvv4l2_videoclassToStdId(dev->pxdrv_v4l2fmt.videoclass);
 dev->pxdrv_framebuffers = min(dev->pxdrv_v4l2dope.framequeues, dev->pxdrv_v4l2fmt.framebuffers);
 if ((data_mode&DATA_RAWBYTES)) {
    dev->v4l2_fourcc = V4L2_PIX_FMT_GREY;
    dev->pxdrv_v4l2fmt.imagexdim = dev->pxdrv_v4l2fmt.imagelinelen;
 }
 if (dev->pxdrv_v4l2fmt.imagexdim == 0
  || dev->pxdrv_v4l2fmt.imageydim == 0) {
     v4l2_err(&dev->v4l2_dev, "frame grabber not initialized, run 'pixcinit'\n");
     if ((verbose & VERBOSE_FORMAT))
  pxdrvv4l2_reportformat(dev, false, false, true, false, false);
 } else {
     if ((verbose & VERBOSE_FORMAT))
  pxdrvv4l2_reportformat(dev, false, true, true, true, true);
 }
 dev->buftrackgen = 0;
 #if 0
     pxdrvv4l2_init_controls(dev);
 #endif
 if (dev->v4l2_dev.ctrl_handler) {
  for (i = NLAYERCTRL; i < MAXCTRL; i++) {
      if (dev->ctrls[i]) {
   v4l2_ctrl_s_ctrl(dev->ctrls[i],
        pxdrv_v4l2control(dev, dev->pxdrv_plp, dev->ctrls[i]->id-CID_CUSTOM_BASE-NLAYERCTRL, PXDAI_II_CTRL_GET, 0));
      }
  }
 }
 pxdrvv4l2_reportfault(dev);
 if (!stream_mode) {
  pxdrvv4l2_readmodeinit(dev, read_mode);
 }
    }
    r = 0;
    goto err_fh;
err_pxdrv:
    #if SUPPORT_FH
 v4l2_fh_release(filep);
    #endif
err_fh:
    mutex_unlock(&dev->mutex);
    return(r);
}
static int
pxdrvv4l2_fh_release(struct file *filep)
{
    struct pxdrvv4l2_dev *dev = video_drvdata(filep);
    int singular = 1;
    mutex_lock(&dev->mutex);
    #if SUPPORT_FH
 singular = v4l2_fh_is_singular_file(filep);
    #endif
    if (stream_mode) {
 _vb2_fop_release(filep, NULL);
    } else {
 v4l2_fh_release(filep);
    }
    if (singular) {
 dev->readmode = 0;
 if (dev->pxdrv_plp) {
     pxdrvv4l2_reportfault(dev);
     pxdrv_v4l2close(dev, dev->pxdrv_plp);
     dev->pxdrv_plp = NULL;
 }
    }
    mutex_unlock(&dev->mutex);
    return(0);
}
static ssize_t
pxdrvv4l2_readsnap(struct file *filep, char __user *buf, size_t count, loff_t *f_pos)
{
    struct pxdrvv4l2_dev *dev = video_drvdata(filep);
    int r = 0;
    #if 0
 if (filep->f_flags & O_NONBLOCK)
  return(-EWOULDBLOCK);
    #endif
    if (down_interruptible(&dev->readmutex))
 return(-ERESTARTSYS);
    if (dev->readsnapped == 0) {
 dev->readstatus = 0;
 r = pxdrv_v4l2video(dev, dev->pxdrv_plp, XCLIVE_SNAP, 1, 0, 0, 0);
 if (r < 0) {
     up(&dev->readmutex);
     v4l2_err(&dev->v4l2_dev, "snap failed (%d)\n", r);
     return(-EIO);
 }
 dev->readsnapped++;
    }
    while (dev->readstatus == 0) {
 up(&dev->readmutex);
 if (filep->f_flags & O_NONBLOCK)
     return(-EAGAIN);
 if (wait_event_interruptible(dev->readqueue, (dev->readstatus != 0)))
     return(-ERESTARTSYS);
 if (down_interruptible(&dev->readmutex))
     return(-ERESTARTSYS);
    }
    dev->readsnapped = 0;
    if (dev->readstatus != V4L2NEWFRAME_ABORT) {
 r = pxdrv_v4l2imageio(dev, dev->pxdrv_plp, 1,
         (data_mode&DATA_FIELDSTAMP)==0? dev->pxdrv_v4l2fmt.framestampsize: 0,
         buf, count, 0, 1);
 if (r >= 0)
     r = count;
    } else
 r = 0;
    up(&dev->readmutex);
    return(r);
}
#if 0
static ssize_t
pxdrvv4l2_readlivenew(struct file *filep, char __user *databuf, size_t count, loff_t *f_pos)
{
    struct pxdrvv4l2_dev *dev = video_drvdata(filep);
    int r, readfgbufindex;
    int ret;
    unsigned long flags;
    if (down_interruptible(&dev->readmutex))
 return(-ERESTARTSYS);
    while (dev->readstatus == 0) {
 up(&dev->readmutex);
 if (filep->f_flags & O_NONBLOCK)
     return(-EAGAIN);
 if (wait_event_interruptible(dev->readqueue, (dev->readstatus != 0)))
     return(-ERESTARTSYS);
 if (down_interruptible(&dev->readmutex))
     return(-ERESTARTSYS);
    }
    spin_lock_irqsave(&dev->readlock, flags);
    readfgbufindex = dev->readfgbufindex;
    dev->readfgbufindex = 0;
    dev->readstatus = 0;
    spin_unlock_irqrestore(&dev->readlock, flags);
    up(&dev->readmutex);
    r = pxdrv_v4l2imageio(dev, dev->pxdrv_plp, readfgbufindex,
         (data_mode&DATA_FIELDSTAMP)==0? dev->pxdrv_v4l2fmt.framestampsize: 0,
         databuf, count, 0, 1);
    ret = (r >= 0)? count: 0;
    r = pxdrv_v4l2video(dev, dev->pxdrv_plp, XCLIVE_LIVEQUEUE, readfgbufindex, 0, 0, 0);
    if (r < 0)
 v4l2_err(&dev->v4l2_dev, "live queue error (%d)\n", r);
    return(ret);
}
#endif
static ssize_t
pxdrvv4l2_readlive(struct file *filep, char __user *databuf, size_t count, loff_t *f_pos)
{
    struct pxdrvv4l2_dev *dev = video_drvdata(filep);
    struct pxdrvv4l2_fgbuffer *fgbuf;
    int r;
    int ret;
    unsigned long flags;
    if (down_interruptible(&dev->readmutex))
 return(-ERESTARTSYS);
    while (dev->readlistcnt == 0) {
 up(&dev->readmutex);
 if (filep->f_flags & O_NONBLOCK)
     return(-EAGAIN);
 if (wait_event_interruptible(dev->readqueue, (dev->readlistcnt != 0)))
     return(-ERESTARTSYS);
 if (down_interruptible(&dev->readmutex))
     return(-ERESTARTSYS);
    }
    spin_lock_irqsave(&dev->readlock, flags);
    fgbuf = list_last_entry(&dev->readlist[0].list, struct pxdrvv4l2_fgbuffer, list);
    list_del(&fgbuf->list);
    dev->readlistcnt--;
    spin_unlock_irqrestore(&dev->readlock, flags);
    up(&dev->readmutex);
    r = pxdrv_v4l2imageio(dev, dev->pxdrv_plp, fgbuf->fgbufindex,
         (data_mode&DATA_FIELDSTAMP)==0? dev->pxdrv_v4l2fmt.framestampsize: 0,
         databuf, count, 0, 1);
    ret = (r >= 0)? count: 0;
    r = pxdrv_v4l2video(dev, dev->pxdrv_plp, XCLIVE_LIVEQUEUE, fgbuf->fgbufindex, 0, 0, 0);
    if (r < 0)
 v4l2_err(&dev->v4l2_dev, "live queue error (%d)\n", r);
    return(ret);
}
static ssize_t
pxdrvv4l2_read(struct file *filep, char __user *buf, size_t count, loff_t *f_pos)
{
    struct pxdrvv4l2_dev *dev = video_drvdata(filep);
    pxdrvv4l2_reportfault(dev);
    if (!pxdrvv4l2_isinitialized(dev))
 return(-EFAULT);
    if (dev->readmode == READMODE_SNAP)
 return(pxdrvv4l2_readsnap(filep, buf, count, f_pos));
    if (dev->readmode == READMODE_LIVEOLD
     || dev->readmode == READMODE_LIVENEW)
 return(pxdrvv4l2_readlive(filep, buf, count, f_pos));
    return(pxdrvv4l2_readsnap(filep, buf, count, f_pos));
}
static unsigned int
pxdrvv4l2_poll(struct file *filep, struct poll_table_struct *waitp)
{
    struct pxdrvv4l2_dev *dev = video_drvdata(filep);
    unsigned int mask = 0;
    down(&dev->readmutex);
    if (dev->readmode == READMODE_SNAP) {
 if (!dev->readsnapped) {
     up(&dev->readmutex);
     return(POLLERR);
 }
    }
    poll_wait(filep, &dev->readqueue, waitp);
    if (dev->readstatus || dev->readlistcnt > 0)
 mask |= POLLIN | POLLRDNORM;
    if (dev->readstatus == V4L2NEWFRAME_ABORT)
 mask |= POLLERR;
    up(&dev->readmutex);
    return(mask);
}
static const struct v4l2_file_operations pxdrvv4l2_rm_fops_fops = {
    .owner = THIS_MODULE,
    .open = pxdrvv4l2_fh_open,
    .release = pxdrvv4l2_fh_release,
    .read = pxdrvv4l2_read,
    .poll = pxdrvv4l2_poll,
    .unlocked_ioctl = video_ioctl2,
};
static const struct v4l2_ioctl_ops pxdrvv4l2_rm_fops_ioctl_ops = {
    .vidioc_querycap = pxdrvv4l2_querycap,
    .vidioc_enum_fmt_vid_cap = pxdrvv4l2_enum_fmt_vid_cap,
    .vidioc_g_fmt_vid_cap = pxdrvv4l2_g_fmt_vid_cap,
    .vidioc_try_fmt_vid_cap = pxdrvv4l2_try_fmt_vid_cap,
    .vidioc_s_fmt_vid_cap = pxdrvv4l2_s_fmt_vid_cap,
    .vidioc_enum_framesizes = pxdrvv4l2_enum_framesizes,
    .vidioc_enum_frameintervals = pxdrvv4l2_enum_frameintervals,
    .vidioc_enum_input = pxdrvv4l2_enum_input,
    #if LINUX_VERSION_CODE < KERNEL_VERSION(5,3,0)
    .vidioc_cropcap = pxdrvv4l2_cropcap,
    .vidioc_s_crop = pxdrvv4l2_s_crop,
    .vidioc_g_crop = pxdrvv4l2_g_crop,
    #else
    .vidioc_g_pixelaspect = pxdrvv4l2_g_pixelaspect,
    #endif
    .vidioc_g_input = pxdrvv4l2_g_input,
    .vidioc_s_input = pxdrvv4l2_s_input,
    .vidioc_g_std = pxdrvv4l2_g_std,
    .vidioc_s_std = pxdrvv4l2_s_std,
    .vidioc_querystd = pxdrvv4l2_querystd,
    .vidioc_g_parm = pxdrvv4l2_g_parm,
    .vidioc_s_parm = pxdrvv4l2_s_parm,
    .vidioc_log_status = pxdrvv4l2_log_status,
    .vidioc_subscribe_event = v4l2_ctrl_subscribe_event,
    .vidioc_unsubscribe_event = v4l2_event_unsubscribe,
};
static const struct video_device pxdrvv4l2_rm_template = {
    .name = "pixci",
    .fops = &pxdrvv4l2_rm_fops_fops,
    .ioctl_ops = &pxdrvv4l2_rm_fops_ioctl_ops,
    .release = video_device_release_empty,
    .tvnorms = V4L2_STD_UNKNOWN,
};
static const struct v4l2_file_operations pxdrvv4l2_sm_fops_fops = {
    .owner = THIS_MODULE,
    .open = pxdrvv4l2_fh_open,
    .release = pxdrvv4l2_fh_release,
    .read = vb2_fop_read,
    .poll = vb2_fop_poll,
    .unlocked_ioctl = video_ioctl2,
    .mmap = vb2_fop_mmap,
};
static const struct v4l2_ioctl_ops pxdrvv4l2_sm_fops_ioctl_ops = {
    .vidioc_querycap = pxdrvv4l2_querycap,
    .vidioc_enum_fmt_vid_cap = pxdrvv4l2_enum_fmt_vid_cap,
    .vidioc_g_fmt_vid_cap = pxdrvv4l2_g_fmt_vid_cap,
    .vidioc_try_fmt_vid_cap = pxdrvv4l2_try_fmt_vid_cap,
    .vidioc_s_fmt_vid_cap = pxdrvv4l2_s_fmt_vid_cap,
    .vidioc_enum_framesizes = pxdrvv4l2_enum_framesizes,
    .vidioc_enum_frameintervals = pxdrvv4l2_enum_frameintervals,
    .vidioc_reqbufs = vb2_ioctl_reqbufs,
    .vidioc_create_bufs = vb2_ioctl_create_bufs,
    .vidioc_prepare_buf = vb2_ioctl_prepare_buf,
    .vidioc_querybuf = vb2_ioctl_querybuf,
    .vidioc_qbuf = vb2_ioctl_qbuf,
    .vidioc_dqbuf = vb2_ioctl_dqbuf,
    .vidioc_expbuf = vb2_ioctl_expbuf,
    .vidioc_enum_input = pxdrvv4l2_enum_input,
    #if LINUX_VERSION_CODE < KERNEL_VERSION(5,3,0)
    .vidioc_cropcap = pxdrvv4l2_cropcap,
    .vidioc_s_crop = pxdrvv4l2_s_crop,
    .vidioc_g_crop = pxdrvv4l2_g_crop,
    #else
    .vidioc_g_pixelaspect = pxdrvv4l2_g_pixelaspect,
    #endif
    .vidioc_g_input = pxdrvv4l2_g_input,
    .vidioc_s_input = pxdrvv4l2_s_input,
    .vidioc_g_std = pxdrvv4l2_g_std,
    .vidioc_s_std = pxdrvv4l2_s_std,
    .vidioc_querystd = pxdrvv4l2_querystd,
    .vidioc_g_parm = pxdrvv4l2_g_parm,
    .vidioc_s_parm = pxdrvv4l2_s_parm,
    .vidioc_streamon = vb2_ioctl_streamon,
    .vidioc_streamoff = vb2_ioctl_streamoff,
    .vidioc_log_status = pxdrvv4l2_log_status,
    .vidioc_subscribe_event = v4l2_ctrl_subscribe_event,
    .vidioc_unsubscribe_event = v4l2_event_unsubscribe,
};
static const struct vb2_ops pxdrvv4l2_sm_video_qops = {
    .queue_setup = pxdrvv4l2_queue_setup,
    .buf_prepare = pxdrvv4l2_buffer_prepare,
    .buf_finish = pxdrvv4l2_buffer_finish,
    .buf_queue = pxdrvv4l2_buffer_queue,
    .start_streaming = pxdrvv4l2_start_streaming,
    .stop_streaming = pxdrvv4l2_stop_streaming,
};
static const struct video_device pxdrvv4l2_sm_template = {
    .name = "pixci",
    .fops = &pxdrvv4l2_sm_fops_fops,
    .ioctl_ops = &pxdrvv4l2_sm_fops_ioctl_ops,
    .release = video_device_release_empty,
    .tvnorms = V4L2_STD_UNKNOWN,
};
static void pxdrvv4l2_remove(struct pxdrvv4l2_dev *dev)
{
    int i;
    v4l2_info(&dev->v4l2_dev, "unregistering %s\n",
       video_device_node_name(&dev->video_dev));
    video_unregister_device(&dev->video_dev);
    v4l2_device_unregister (&dev->v4l2_dev);
    v4l2_ctrl_handler_free (&dev->ctrl_handler);
    #if LINUX_VERSION_CODE < KERNEL_VERSION(4,5,0)
 if (dev->alloc_ctx)
     vb2_dma_contig_cleanup_ctx(dev->alloc_ctx);
    #endif
    if (dev->bufwork) {
 flush_workqueue(dev->bufwork);
 destroy_workqueue(dev->bufwork);
    }
    if (dev->pxdrv_plp) {
 pxdrvv4l2_reportfault(dev);
 pxdrv_v4l2close(dev, dev->pxdrv_plp);
 dev->pxdrv_plp = NULL;
    }
    for (i = 0; i < MAXCTRL; i++) {
 if (dev->ctrlmenus[i])
     kfree(dev->ctrlmenus[i]);
 if (dev->ctrlnames[i])
     kfree(dev->ctrlnames[i]);
    }
    if (dev->readlist)
 kfree(dev->readlist);
    kfree(dev);
}
static int pxdrvv4l2_release(void)
{
    struct pxdrvv4l2_dev *dev;
    struct list_head *list;
    while (!list_empty(&pxdrvv4l2_devlist)) {
 list = pxdrvv4l2_devlist.next;
 list_del(list);
 dev = list_entry(list, struct pxdrvv4l2_dev, devlist);
 pxdrvv4l2_remove(dev);
    }
    return(0);
}
static int
pxdrvv4l2_create_instance(int unit, struct pci_dev *pcidev)
{
    struct pxdrvv4l2_dev *dev;
    struct video_device *vdev;
    struct vb2_queue *vbq = NULL;
    int r, i;
    dev = kzalloc(sizeof(*dev), GFP_KERNEL);
    if (!dev)
 return(-ENOMEM);
    snprintf(dev->v4l2_dev.name, sizeof(dev->v4l2_dev.name),
  "%s-%03d", "pixci_v4l", unit);
    r = v4l2_device_register(&pcidev->dev, &dev->v4l2_dev);
    if (r)
 goto free_dev;
    dev->pxdrv_unit = unit;
    dev->pcidev = pcidev;
    mutex_init(&dev->mutex);
    sema_init(&dev->readmutex, 1);
    spin_lock_init(&dev->readlock);
    init_waitqueue_head(&dev->readqueue);
    if (1 || stream_mode) {
 INIT_LIST_HEAD(&dev->bufqueue);
 INIT_LIST_HEAD(&dev->bufprequeue);
 spin_lock_init(&dev->bufqueuelock);
 spin_lock_init(&dev->bufprequeuelock);
    }
    dev->pxdrv_plp = pxdrv_v4l2open(dev, dev->pxdrv_unit, (stream_mode&STREAM_MEMMAP)? stream_buffers: -1);
    if (dev->pxdrv_plp) {
 pxdrv_v4l2getformat(dev, dev->pxdrv_plp, &dev->pxdrv_v4l2fmt, sizeof(dev->pxdrv_v4l2fmt));
 pxdrv_v4l2getdope (dev, dev->pxdrv_plp, &dev->pxdrv_v4l2dope, sizeof(dev->pxdrv_v4l2dope));
 pxdrvv4l2_init_controls(dev);
 pxdrvv4l2_reportfault(dev);
 pxdrv_v4l2close(dev, dev->pxdrv_plp);
 dev->pxdrv_plp = NULL;
    }
    if (stream_mode) {
 vbq = &dev->vb_vidq;
 vbq->type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
 vbq->io_modes = ((stream_mode&STREAM_MEMMAP)? VB2_MMAP: 0) | ((stream_mode&STREAM_USERPTR)? V4L2_MEMORY_USERPTR: 0) | VB2_READ;
 vbq->drv_priv = dev;
 vbq->buf_struct_size = sizeof(struct pxdrvv4l2_buffer);
 vbq->ops = &pxdrvv4l2_sm_video_qops;
 vbq->mem_ops = &vb2_dma_contig_memops;
 vbq->lock = &dev->mutex;
 #if LINUX_VERSION_CODE >= KERNEL_VERSION(4,5,0)
     vbq->dev = &pcidev->dev;
 #endif
 #if LINUX_VERSION_CODE >= KERNEL_VERSION(4,5,0)
 vbq->timestamp_flags = V4L2_BUF_FLAG_TIMESTAMP_MONOTONIC;
 vbq->timestamp_flags |= V4L2_BUF_FLAG_TSTAMP_SRC_EOF;
 #endif
 #if LINUX_VERSION_CODE >= KERNEL_VERSION(4,0,0)
     vbq->min_buffers_needed = 2;
 #endif
 if (dev->pxdrv_v4l2dope.dma32) {
     #if LINUX_VERSION_CODE >= KERNEL_VERSION(4,0,0)
  vbq->gfp_flags = GFP_DMA32;
     #else
  if (sizeof(vbq) == 8)
      v4l2_err(&dev->v4l2_dev, "can't support 32 bit device on current kernel's V4L2 core");
  r = -EINVAL;
  goto unreg_dev;
     #endif
 }
 r = vb2_queue_init(vbq);
 if (r)
     goto unreg_dev;
 #if LINUX_VERSION_CODE < KERNEL_VERSION(4,5,0)
     dev->alloc_ctx = vb2_dma_contig_init_ctx(&pcidev->dev);
     if (IS_ERR(dev->alloc_ctx)) {
  v4l2_err(&dev->v4l2_dev, "can't allocate buffer context");
  r = PTR_ERR(dev->alloc_ctx);
  goto unreg_dev;
     }
 #else
 #endif
 if (!(capt_options&CAPT_BUFDONE_IRQ))
     dev->bufwork = create_singlethread_workqueue(dev->v4l2_dev.name);
    }
    vdev = &dev->video_dev;
    strlcpy(vdev->name, "pixci" "_v4l2", sizeof(vdev->name));
    if (stream_mode)
 *vdev = pxdrvv4l2_sm_template;
    else {
 *vdev = pxdrvv4l2_rm_template;
    }
    vdev->v4l2_dev = &dev->v4l2_dev;
    if (stream_mode)
 vdev->queue = vbq;
    vdev->lock = &dev->mutex;
    vdev->release = video_device_release_empty;
    #if LINUX_VERSION_CODE >= KERNEL_VERSION(5,3,0)
 vdev->device_caps = V4L2_CAP_VIDEO_CAPTURE
      | V4L2_CAP_READWRITE
      | (stream_mode? V4L2_CAP_STREAMING: 0);
    #endif
    video_set_drvdata(vdev, dev);
    r = video_register_device(vdev, VFL_TYPE_GRABBER, video_nr);
    if (r < 0)
 goto unreg_dev;
    list_add_tail(&dev->devlist, &pxdrvv4l2_devlist);
    v4l2_info(&dev->v4l2_dev, "V4L2 device registered as %s\n",
          video_device_node_name(vdev));
    return(0);
unreg_dev:
    v4l2_device_unregister(&dev->v4l2_dev);
free_dev:
    #if LINUX_VERSION_CODE < KERNEL_VERSION(4,5,0)
 if (dev->alloc_ctx)
     vb2_dma_contig_cleanup_ctx(dev->alloc_ctx);
    #endif
    if (!(capt_options&CAPT_BUFDONE_IRQ)) {
 if (dev->bufwork)
     destroy_workqueue(dev->bufwork);
    }
    for (i = 0; i < MAXCTRL; i++) {
 if (dev->ctrlmenus[i])
     kfree(dev->ctrlmenus[i]);
 if (dev->ctrlnames[i])
     kfree(dev->ctrlnames[i]);
    }
    if (dev->readlist)
 kfree(dev->readlist);
    kfree(dev);
    return(r);
}
void
pxdrvv4l2_newframe(void *adev, int unit, int fgbufindex, int vfield, int tracker, int status, u32 ticks[2])
{
    struct pxdrvv4l2_dev *dev = (struct pxdrvv4l2_dev *)adev;
    struct pxdrvv4l2_buffer *buf;
    unsigned long flags;
    if (!dev)
 return;
    if (dev->bufstreamon) {
 if (status == V4L2NEWFRAME_ABORTALL) {
     for (;;) {
  spin_lock_irqsave(&dev->bufqueuelock, flags);
  if (list_empty(&dev->bufqueue)) {
      spin_unlock_irqrestore(&dev->bufqueuelock, flags);
      break;
  }
  buf = list_first_entry(&dev->bufqueue, struct pxdrvv4l2_buffer, list);
  list_del(&buf->list);
  spin_unlock_irqrestore(&dev->bufqueuelock, flags);
  vb2_buffer_done(&buf->vb, VB2_BUF_STATE_ERROR);
     }
     return;
 }
 if (status == V4L2NEWFRAME_EOC || V4L2NEWFRAME_ABORT) {
     spin_lock_irqsave(&dev->bufqueuelock, flags);
     if (!list_empty(&dev->bufqueue)) {
  list_for_each_entry(buf, &dev->bufqueue, list) {
      if (buf->tracker == tracker) {
   list_del(&buf->list);
   spin_unlock_irqrestore(&dev->bufqueuelock, flags);
   if (ticks) {
       buf->ticks[0] = ticks[0];
       buf->ticks[1] = ticks[1];
   } else {
       buf->ticks[0] = 0;
       buf->ticks[1] = 0;
   }
   buf->vfield = vfield;
   if (!(capt_options&CAPT_BUFDONE_IRQ)) {
       buf->workstatus = status==V4L2NEWFRAME_EOC? VB2_BUF_STATE_DONE: VB2_BUF_STATE_ERROR;
       queue_work(dev->bufwork, &buf->work);
   } else {
       vb2_buffer_done(&buf->vb, status==V4L2NEWFRAME_EOC? VB2_BUF_STATE_DONE: VB2_BUF_STATE_ERROR);
   }
   return;
      }
  }
     }
     spin_unlock_irqrestore(&dev->bufqueuelock, flags);
     v4l2_err(&dev->v4l2_dev, "can't find 'done' buffer in list");
     return;
 }
    }
    if (dev->readmode) {
 if (status == V4L2NEWFRAME_EOC || status == V4L2NEWFRAME_ABORT) {
     if (dev->readmode == READMODE_SNAP) {
  dev->readstatus = status;
  wake_up_interruptible(&dev->readqueue);
     }
     if (dev->readmode == READMODE_LIVEOLD
      || dev->readmode == READMODE_LIVENEW) {
  if (status == V4L2NEWFRAME_EOC) {
      struct pxdrvv4l2_fgbuffer *fgbuf = NULL;
      spin_lock_irqsave(&dev->readlock, flags);
      if (dev->readmode == READMODE_LIVENEW) {
   while (!list_empty(&dev->readlist[0].list)) {
       fgbuf = list_first_entry(&dev->readlist[0].list, struct pxdrvv4l2_fgbuffer, list);
       list_del(&fgbuf->list);
       dev->readlistcnt--;
   }
      }
      if (fgbufindex >= 1 && fgbufindex <= dev->pxdrv_framebuffers) {
   list_add(&dev->readlist[fgbufindex].list, &dev->readlist[0].list);
   dev->readlistcnt++;
      }
      if (dev->readmode == READMODE_LIVEOLD) {
   if (dev->readlistcnt > 0
    && (dev->pxdrv_framebuffers - dev->readlistcnt) < READLIVEOLD_RSVDBUFFERS) {
       fgbuf = list_last_entry(&dev->readlist[0].list, struct pxdrvv4l2_fgbuffer, list);
       list_del(&fgbuf->list);
       dev->readlistcnt--;
   }
      }
      spin_unlock_irqrestore(&dev->readlock, flags);
      wake_up_interruptible(&dev->readqueue);
      if (fgbuf != NULL)
   pxdrv_v4l2video(dev, dev->pxdrv_plp, XCLIVE_LIVEQUEUE, fgbuf->fgbufindex, 0, 0, 1);
  } else {
      pxdrv_v4l2video(dev, dev->pxdrv_plp, XCLIVE_LIVEQUEUE, fgbufindex, 0, 0, 1);
  }
     }
 }
    }
}
void
pxdrvv4l2_probe(void *adev, int unit, struct pci_dev *pcidev)
{
    int r = 0;
    char *s1 = "instance for unit";
    if (!adev) {
 r = pxdrvv4l2_create_instance(unit, pcidev);
 if (r < 0)
     printk(KERN_INFO "%s create %s %d failed (%d)\n", XCDRV_IDV4LN, s1, unit, r);
    } else {
 struct pxdrvv4l2_dev *dev = (struct pxdrvv4l2_dev *)adev;
 list_del(&dev->devlist);
 pxdrvv4l2_remove(dev);
    }
}
static void __exit
cleanup_pxdrvv4l2(void)
{
    pxdrvv4l2_release();
    pxdrv_v4l2cleanup(pxdrvv4l2_newframe, pxdrvv4l2_probe);
    printk(KERN_INFO "%s removed\n", XCDRV_IDV4LN);
}
static int __init
init_pxdrvv4l2(void)
{
    int r = 0;
    int nPixci = 0;
    printk(KERN_INFO "%s %s loaded\n", XCDRV_IDV4LN, "3.8.01" " " "[20.08.28.142201]" );
    pxdrvv4l2_reportformat(NULL, true, false, false, false, false);
    pxdrv_v4l2init(&nPixci, pxdrvv4l2_newframe, pxdrvv4l2_probe);
    return(r);
}
#undef XCDRV_IDV4LN
module_init(init_pxdrvv4l2);
module_exit(cleanup_pxdrvv4l2);
