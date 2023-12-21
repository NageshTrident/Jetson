/**                                                                               
 *                                                                                
 *      pixciv4l.h         External       24-JAN-2023            
 *                                                                                
 *      Copyright (C)  2019-2023  EPIX, Inc.  Released under GPL.
 *                                                                                
 *      Frame Grabber Driver: LINUX v4l driver access to Video 4 Linux 2 (V4L2)   
 *                                                                                
 *      THIS IS A MACHINE GENERATED COPY                                          
 **/                                                                              













/**
 * Hints on pixel interpretation from XCLIB's pximage.h
 */
#define PXHINTNONE	0x0000	/* unknown				*/
#define PXHINTFRAME	0x0010	/* raw frame buffer data		*/

				/* With pixies=1 (typically):		*/
#define PXHINTGREY	0x0001	/* grey (gray) scale monochrome 	*/
#define PXHINTGRAY	0x0001	/* gray (grey) scale monochrome 	*/
#define PXHINTINDEX	0x0011	/* index into unspecified palette	*/
#define PXHINTBAYER	0x0021	/* Bayer pattern RGB	G B G B ...	*/
				/* (raw)		R G R G ...	*/
				/*			or variation	*/
#define PXHINTBAYER0	0x1021	/* Raw Bayer w. Red at upper left	*/
#define PXHINTBAYER1	0x2021	/* Raw Bayer w. Grn-Red at upper left	*/
#define PXHINTBAYER2	0x3021	/* Raw Bayer w. Grn-Blu at upper left	*/
#define PXHINTBAYER3	0x4021	/* Raw Bayer w. Blue	at upper left	*/

				/* With pixies=2 (typically):		*/
#define PXHINTCOMPLEX	0x0002	/* complex, probably of monochrome	*/
#define PXHINTCBYCRY	0x0012	/* Cb,Y,Cr,Y,Cb,Y,Cr,Y,... (YCrCb,UYVY) */
#define PXHINTYCBYCR	0x0022	/* Y,Cb,Y,Cr,Y,Cb,Y,Cr,... (YCrCb,YUY2) */
#define PXHINTCRYCBY	0x0032	/* Cr,Y,Cb,Y,Cr,Y,Cb,Y,... (YCrCb,VYUY) */
#define PXHINTYCRYCB	0x0042	/* Y,Cr,Y,Cb,Y,Cr,Y,Cb,... (YCrCb,YVYU) */
#define PXHINTBGRG	0x0052	/* B,G,R,G,B,G,R,G,...			*/
#define PXHINTRGGB	0x0062	/* R,G,G,B,R,G,G,B,...			*/

				/* With pixies=3 (typically):		*/
#define PXHINTBGR	0x0003	/* B,G,R,... (RGB)			*/
#define PXHINTYCRCB	0x0013	/* Y,Cr,Cb,Y,Cr,Cb,... (YCrCb)		*/
#define PXHINTBSH	0x0023	/* Brightness,Saturation,Hue, (HSB)	*/
#define PXHINTRGB	0x0043	/* R,G,B,... (RGB)			*/
#define PXHINTYIQ	0x0053	/* Y,I,Q (YIQ)				*/
#define PXHINTCMY	0x0063	/* C,M,Y,... (CMY)			*/
/*efine PXHINTGRB	0x0073	   G,R,B,... (GRB)			*/

				/* With pixies=4 (typically):		*/
#define PXHINTBGRX	0x0004	/* B,G,R,Pad (RGB)			*/
#define PXHINTYCRCBX	0x0014	/* Y,Cr,Cb,Pad,Y,Cr,Cb,Pad  (YCrCb)	*/
#define PXHINTRGBX	0x0024	/* R,G,B,Pad (RGB)			*/
#define PXHINTBAYERX4	0x0034	/* Bayer pattern w. R Gr Gb B each pixel*/
#define PXHINTCMYK	0x0044	/* C,M,Y,K,... (CMYK)			*/
/*efine PXHINTGRBX	0x0074	   G,R,B,Pad (GRB)			*/

#define PXHINTPIXIES	0x000F	/* mask # of pixies per pixel for hint	*/

#define PXHINTUSER	0x7000	/* user-defined types: from PXHINTUSER	*/
#define PXHINTUSERN	0x0FFF	/* thru PXHINTUSER+PXHINTUSERN		*/



/**
 * Camera/format class constants from XCLIB's pxlibvs.h.
 */
#define PXSCN_UNKNOWN	    0x0000
#define PXSCN_AREASCAN	    0x0002
#define PXSCN_LINESCAN	    0x0001
#define PXSCN_RS170	    0x0102
#define PXSCN_NTSC	    0x0202
#define PXSCN_NTSC_J	    0x0302
#define PXSCN_CCIR	    0x0402
#define PXSCN_PAL	    0x0502
#define PXSCN_PAL_M	    0x0602
#define PXSCN_PAL_N	    0x0702
#define PXSCN_SECAM	    0x0802
#define PXSCN_PAL_N_COMBO   0x0902
#define PXSCN_PAL_60	    0x0A02
#define PXSCN_NTSC_443	    0x0B02
#define PXSCN_YC	    0x0010	// adjective for s-video
#define PXSCN_CUSTOM	    0x8000	// adjective


/**
 * Additional pixel formats; public,
 * may not be  defined in older videodev2.h.
 * We have seen contradictory values for some of these!
 */
#if !defined(V4L2_PIX_FMT_SBGGR16)
    #define V4L2_PIX_FMT_SBGGR16 v4l2_fourcc('B', 'G', '1', '6') /* 16	BGBG.. GRGR.. */
  //#define V4L2_PIX_FMT_SBGGR16 v4l2_fourcc('B', 'Y', 'R', '2') /* 16	BGBG.. GRGR.. */
#endif
#if !defined(V4L2_PIX_FMT_SGBRG16)
    #define V4L2_PIX_FMT_SGBRG16 v4l2_fourcc('G', 'B', '1', '6') /* 16	GBGB.. RGRG.. */
#endif
#if !defined(V4L2_PIX_FMT_SGRBG16)
  //#define V4L2_PIX_FMT_SGRBG16 v4l2_fourcc('B', 'A', '1', '6') /* 16	GRGR.. BGBG.. */
    #define V4L2_PIX_FMT_SGRBG16 v4l2_fourcc('G', 'R', '1', '6') /* 16	GRGR.. BGBG.. */
#endif
#if !defined(V4L2_PIX_FMT_SRGGB16)
    #define V4L2_PIX_FMT_SRGGB16 v4l2_fourcc('R', 'G', '1', '6') /* 16	RGRG.. GBGB.. */
#endif



/**
 * Video mode verbs.
 * Some may not be used in our v4l2 driver.
 */
#define XCLIVE_LIVE1		'l'
#define XCLIVE_LIVE2		'2'
#define XCLIVE_SNAP		's'
#define XCLIVE_TRIG		't'
#define XCLIVE_SEQ		'q'
#define XCLIVE_SEQTRIG		'Q'
#define XCLIVE_SNAPQUEUE	'S'
#define XCLIVE_LIVEQUEUE	'L'
#define XCLIVE_STOPLIVE 	'u'
#define XCLIVE_ABORTLIVE	'U'


/**
 * Partial list of model codes.
 */
#define PIXCI_SV2	0x0001
#define PIXCI_SV3	0x0002
#define PIXCI_SV4	0x0004
#define PIXCI_SV5	0x000A
