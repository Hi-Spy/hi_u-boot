/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : vou_drv.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2011/06/28
  Description   : vou_drv.c header file
  History       :
  1.Date        : 2011/06/28
    Author      : z00185248/n00168968
    Modification: Created file

******************************************************************************/

#ifndef __VOU_DRV_H__
#define __VOU_DRV_H__

#include "hi_type.h"
#include <hi3535_vo.h>
#include "vou_def.h"

#ifdef __cplusplus
#if __cplusplus
    extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

#define VOU_WBC2_FINISHED_INT    0          /* 是否使用WBC完成中断控制宏 */

#define VOU_INTCLEAR_ALL        0xffffffff
#define RGB(r,g,b)   (((r & 0xff) << 16) | ((g & 0xff) << 8) | (b & 0xff))
#define RGB_RED(c)   ((c & 0xff0000) >> 16)
#define RGB_GREEN(c) ((c & 0xff00) >> 8)
#define RGB_BLUE(c)  (c & 0xff)

#define YUV(y,u,v)   (((y & 0xff) << 16) | ((u & 0xff) << 8) | (v & 0xff))
#define YUV_Y(c)     ((c & 0xff0000) >> 16)
#define YUV_U(c)     ((c & 0xff00) >> 8)
#define YUV_V(c)     (c & 0xff)


typedef HI_S32 VO_DEV;
typedef HI_S32 VO_LAYER;
typedef HI_S32 VO_CHN;
typedef HI_S32 VO_WBC;
typedef HI_S32 GRAPHIC_LAYER;
typedef HI_S32 VO_INTF_TYPE_E;


typedef enum hiVOU_INT_MODE_E
{
    VOU_INT_MODE_FRAME      = 0x0,
    VOU_INT_MODE_FIELD      = 0x1,
    VOU_INT_MODE_BUTT
} VOU_INT_MODE_E;

/* vou mixer prio id */
typedef enum tagVOU_MIX_PRIO_E
{
    VOU_MIX_PRIO0 = 0,
    VOU_MIX_PRIO1,
    VOU_MIX_PRIO2,
    VOU_MIX_PRIO3,
    VOU_MIX_PRIO4,
    VOU_MIX_PRIO5,
    VOU_MIX_PRIO_CLR,
    VOU_MIX_BUTT 
}VOU_MIX_PRIO_E;

typedef enum hiVOU_LAYER_PIXERL_FORMAT_E
{
    /* for video surface interface */
    VOU_LAYER_PIXERL_FORMAT_SPYCbCr_420       = 0x3,
    VOU_LAYER_PIXERL_FORMAT_SPYCbCr_422       = 0x4,
    VOU_LAYER_PIXERL_FORMAT_BUTT
} VOU_LAYER_PIXERL_FORMAT_E;

typedef enum hiVOU_INTF_PIXERL_FORMAT_E
{
    /* for device interface */
    VOU_INTF_PIXERL_FORMAT_YCBCR422          = 0x0,
    VOU_INTF_PIXERL_FORMAT_YCBCR444          = 0xC,
    VOU_INTF_PIXERL_FORMAT_RGB888            = 0xC,
    VOU_INTF_PIXERL_FORMAT_BUTT
} VOU_INTF_PIXERL_FORMAT_E;

typedef enum hiVO_CSC_RANGE_E
{
    VO_CSC_DEF_VAL          = 50,
    VO_CSC_LUMA_MAX         = 100,
    VO_CSC_LUMA_MIN         = 0,

    VO_CSC_CONT_MAX         = 100,
    VO_CSC_CONT_MIN         = 0,

    VO_CSC_HUE_MAX         = 100,
    VO_CSC_HUE_MIN         = 0,

    VO_CSC_SAT_MAX         = 100,
    VO_CSC_SAT_MIN         = 0,

}VO_CSC_RANGE_E;


/* 如果后期开放ACM参数配置，则移到hi_comm_vo.h中 */
typedef struct hiVOU_ACM_BLKINFO_S
{
    HI_U32 bAcmBlkEn;
    
    HI_U8  u8UIndex;
    HI_U8  u8VIndex;
    
    HI_S8  s8UOffsetA;
    HI_S8  s8UOffsetB;
    HI_S8  s8UOffsetC;
    HI_S8  s8UOffsetD;
    
    HI_S8  s8VOffsetA;
    HI_S8  s8VOffsetB;
    HI_S8  s8VOffsetC;
    HI_S8  s8VOffsetD;
} VOU_ACM_BLKINFO_S;

/* vou zoom mode */
typedef enum 
{
    VOU_DISP_ZMEMODE_HORL = 0,  
    VOU_DISP_ZMEMODE_HORC,  
    VOU_DISP_ZMEMODE_VERL,
    VOU_DISP_ZMEMODE_VERC,
    
    VOU_DISP_ZMEMODE_HOR,
    VOU_DISP_ZMEMODE_VER,
    VOU_DISP_ZMEMODE_ALPHA,
    VOU_DISP_ZMEMODE_ALPHAV,
    VOU_DISP_ZMEMODE_VERT,
    VOU_DISP_ZMEMODE_VERB,
    
    VOU_DISP_ZMEMODE_ALL,
    VOU_DISP_ZMEMODE_NONL,
    VOU_DISP_ZMEMODE_BUTT
      
}VOU_DISP_ZMEMODE_E;

typedef struct hiVOU_ACM_S
{
    VOU_ACM_BLKINFO_S stAcmBlk[4];  // 4这个值的宏暂时被定义在vou_chn.c中
} VOU_ACM_S;

typedef enum hiVOU_DEV_TYPE_E
{
    VOU_HD_HW_DEV = 0,
    VOU_SD_HW_DEV,
    VOU_CAS_DEV,
    VOU_VIRT_DEV,
    VOU_DEV_TYPE_BUTT,
} VOU_DEV_TYPE_E;

typedef struct hiVOU_DEV_CAPABILITY_S
{
    VOU_DEV_TYPE_E  enVoDevType;             /* VO设备类型 */
    HI_BOOL         bHdVpssRciv;             /* 接收图像策略是否需要通过VPSS接收 */
    HI_BOOL         bSdTdeDisp;              /* 显示图像策略是否需要通过TDE拼接 */
    HI_BOOL         bSupportWbc;             /* 设置支持wbc功能 */
    HI_BOOL         bSptGetScreenFrm;        /* 是否支持获取屏幕接口 */
} VOU_DEV_CAPABILITY_S;

/* 将Dev、LAYER的枚举放在此处是为了不让mkp层的代码直接使用枚举变量，而必须通过调用相应的能力集接口来实现 */
/* vou device enumeration */
/* 物理显示通路枚举，需要与HAL层的显示通路枚举一一对应 */
#if 0
typedef enum hiVO_DEV_E
{
    VOU_DEV_DHD0  = 0,                 /* high definition device */
    VOU_DEV_DHD1  = 1,                 /* assistant device */
    VOU_DEV_DSD0  = 2,                 /* spot device */
    //VOU_DEV_DSD1  = 2,
    //VOU_DEV_DSD2  = 4,
    //VOU_DEV_DSD3  = 5,
    //VOU_DEV_DSD4  = 6,
    //VOU_DEV_DSD5  = 7,
    VOU_DEV_BUTT
} VO_DEV_E;


/*  所有物理资源层的枚举，需要与HAL层的资源枚举一一对应*/
typedef enum
{
    VOU_LAYER_VHD0  = 0,
    //VOU_LAYER_VHD1  = 1,
    //VOU_LAYER_VHD2  = 2,
    VOU_LAYER_VHD1  = 1,    
    VOU_LAYER_VP    = 2,
    VOU_LAYER_VSD0  = 3,
    //VOU_LAYER_VSD2  = 5,
    //VOU_LAYER_VSD3  = 6,
    //VOU_LAYER_VSD4  = 7,
    VOU_LAYER_G0    = 4,   /* fb0 */
    VOU_LAYER_G1    = 5,   /* fb2 */ 
    VOU_LAYER_HC0   = 6,   /* G2 is HC0  --- fb3 */ 
    VOU_LAYER_G4    = 7,   /* fb1 and no G3*/ 
    //VOU_LAYER_HC1   = 14,
    VOU_LAYER_WBC_G0 = 8,
    VOU_LAYER_WBC_G4 = 9,
    VOU_LAYER_WBC_D = 10,
    VOU_LAYER_BUTT
}VOU_LAYER_E;

#define VOU_LAYER_G2 VOU_LAYER_HC0
#endif

typedef enum
{
    VOU_SW_LAYER_VHD0  = 0,
    VOU_SW_LAYER_VHD1  = 1,    
    VOU_SW_LAYER_VP    = 2,
    //VOU_SW_LAYER_VHD2  = 2,     /* 暂时软件层面上，HD2和SD5一致，和硬件特性保持一致，以后再改 */
    VOU_SW_LAYER_VSD0  = 3,
    //VOU_SW_LAYER_VSD1  = 2,
    //VOU_SW_LAYER_VSD2  = 5,
    //VOU_SW_LAYER_VSD3  = 6,
    //VOU_SW_LAYER_VSD4  = 7,
    //VOU_SW_LAYER_CAS1  = 8,
    //VOU_SW_LAYER_CAS2  = 9,
    VOU_SW_LAYER_VIRT0 = 4,
    VOU_SW_LAYER_VIRT1 = 5,
    VOU_SW_LAYER_VIRT2 = 6,
    VOU_SW_LAYER_VIRT3 = 7,
    VOU_SW_LAYER_BUTT
}VOU_SW_LAYER_E;

typedef struct tagVO_SYNC_INFO_S
{
    HI_BOOL  bSynm;     /* sync mode(0:timing,as BT.656; 1:signal,as LCD) */
    HI_BOOL  bIop;      /* interlaced or progressive display(0:i; 1:p) */
    HI_U8    u8Intfb;   /* interlace bit width while output */

    HI_U16   u16Vact ;  /* vertical active area */
    HI_U16   u16Vbb;    /* vertical back blank porch */
    HI_U16   u16Vfb;    /* vertical front blank porch */

    HI_U16   u16Hact;   /* herizontal active area */
    HI_U16   u16Hbb;    /* herizontal back blank porch */
    HI_U16   u16Hfb;    /* herizontal front blank porch */
    HI_U16   u16Hmid;   /* bottom herizontal active area */

    HI_U16   u16Bvact;  /* bottom vertical active area */
    HI_U16   u16Bvbb;   /* bottom vertical back blank porch */
    HI_U16   u16Bvfb;   /* bottom vertical front blank porch */

    HI_U16   u16Hpw;    /* horizontal pulse width */
    HI_U16   u16Vpw;    /* vertical pulse width */

    HI_BOOL  bIdv;      /* inverse data valid of output */
    HI_BOOL  bIhs;      /* inverse horizontal synch signal */
    HI_BOOL  bIvs;      /* inverse vertical synch signal */

} VO_SYNC_INFO_S;

typedef enum hiVOU_LAYER_DDR_E
{
    VOU_LAYER_DDR0 = 0,
    VOU_LAYER_DDR1 = 1,
    VOU_LAYER_DDR_BUTT
}VOU_LAYER_DDR_E;

typedef struct hiVO_CSC_S
{
    VO_CSC_MATRIX_E enCscMatrix;
    HI_U32 u32Luma;                     /* luminance:   0 ~ 100 default: 50 */
    HI_U32 u32Contrast;                 /* contrast :   0 ~ 100 default: 50 */
    HI_U32 u32Hue;                      /* hue      :   0 ~ 100 default: 50 */
    HI_U32 u32Satuature;                /* satuature:   0 ~ 100 default: 50 */
} VO_CSC_S;

typedef struct hiVO_VGA_PARAM_S
{
    VO_CSC_S stCSC;
    HI_U32 u32Gain;                     /* current gain of VGA signals. [0, 64). default:0x30 */
} VO_VGA_PARAM_S;

typedef enum hiVO_SCREEN_HFILTER_E
{
	VO_SCREEN_HFILTER_DEF	= 0,
	VO_SCREEN_HFILTER_8M,
	VO_SCREEN_HFILTER_6M,
	VO_SCREEN_HFILTER_5M,
	VO_SCREEN_HFILTER_4M,
	VO_SCREEN_HFILTER_3M,
	VO_SCREEN_HFILTER_2M,
	VO_SCREEN_HFILTER_BUTT
    
} VO_SCREEN_HFILTER_E;

typedef enum hiVO_SCREEN_VFILTER_E
{
	VO_SCREEN_VFILTER_DEF	= 0,
	VO_SCREEN_VFILTER_8M,
	VO_SCREEN_VFILTER_6M,
	VO_SCREEN_VFILTER_5M,
	VO_SCREEN_VFILTER_4M,
	VO_SCREEN_VFILTER_3M,
	VO_SCREEN_VFILTER_2M,	
	VO_SCREEN_VFILTER_BUTT
    
} VO_SCREEN_VFILTER_E;

typedef enum hiVO_CAS_RGN_E
{
    VO_CAS_64_RGN = 0,
    VO_CAS_32_RGN,
    VO_CAS_RGN_BUTT,
} VO_CAS_RGN_E;

typedef enum hiVO_WBC_SOURCE_TYPE_E
{
    VO_WBC_SOURCE_DEV      = 0x0,
    VO_WBC_SOURCE_VIDEO    = 0x1,    
    VO_WBC_SOURCE_GRAPHIC  = 0x2,
    
    VO_WBC_SOURCE_BUTT
} VO_WBC_SOURCE_TYPE_E;

typedef struct hiVO_WBC_SOURCE_S
{
    VO_WBC_SOURCE_TYPE_E enSourceType;
    HI_U32 u32SourceId;
} VO_WBC_SOURCE_S;

typedef enum hiVO_CAS_MODE_E
{
    VO_CAS_MODE_SINGLE = 0,
    VO_CAS_MODE_DUAL,
    VO_CAS_MODE_BUTT,
} VO_CAS_MODE_E;

typedef struct hiSIZE_S
{
    HI_U32 u32Width;
    HI_U32 u32Height;
} SIZE_S;

/* we ONLY define picture format used, all unused will be deleted!*/
typedef enum hiPIXEL_FORMAT_E
{   
    PIXEL_FORMAT_RGB_1BPP = 0,
    PIXEL_FORMAT_RGB_2BPP,
    PIXEL_FORMAT_RGB_4BPP,
    PIXEL_FORMAT_RGB_8BPP,
    PIXEL_FORMAT_RGB_444,
    PIXEL_FORMAT_RGB_4444,
    PIXEL_FORMAT_RGB_555,
    PIXEL_FORMAT_RGB_565,
    PIXEL_FORMAT_RGB_1555,

    /*  9 reserved */
    PIXEL_FORMAT_RGB_888,
    PIXEL_FORMAT_RGB_8888,
    PIXEL_FORMAT_RGB_PLANAR_888,
    PIXEL_FORMAT_RGB_BAYER_8BPP,
    PIXEL_FORMAT_RGB_BAYER_10BPP,
    PIXEL_FORMAT_RGB_BAYER_12BPP,
    PIXEL_FORMAT_RGB_BAYER_14BPP,
    PIXEL_FORMAT_RGB_BAYER,         /* 16 bpp */

    PIXEL_FORMAT_YUV_A422,
    PIXEL_FORMAT_YUV_A444,

    PIXEL_FORMAT_YUV_PLANAR_422,
    PIXEL_FORMAT_YUV_PLANAR_420,
    PIXEL_FORMAT_YUV_PLANAR_444,

    PIXEL_FORMAT_YUV_SEMIPLANAR_422,
    PIXEL_FORMAT_YUV_SEMIPLANAR_420,
    PIXEL_FORMAT_YUV_SEMIPLANAR_444,

    PIXEL_FORMAT_UYVY_PACKAGE_422,
    PIXEL_FORMAT_YUYV_PACKAGE_422,
    PIXEL_FORMAT_VYUY_PACKAGE_422,
    PIXEL_FORMAT_YCbCr_PLANAR,
    
    PIXEL_FORMAT_BUTT   
} PIXEL_FORMAT_E;


typedef struct hiVO_WBC_ATTR_S
{
    SIZE_S              stTargetSize;        /* WBC Zoom target size */ 
    PIXEL_FORMAT_E      enPixelFormat;       /* the pixel format of WBC output */
    HI_U32              u32FrameRate;        /* frame rate control */    
} VO_WBC_ATTR_S;

typedef enum hiVO_WBC_MODE_E
{
    VO_WBC_MODE_NOMAL = 0,                  /* In this mode, wbc will capture frames according to dev frame rate
                                                    and wbc frame rate */
    VO_WBC_MODE_DROP_REPEAT,                /* In this mode, wbc will drop dev repeat frame, and capture the real frame
                                                    according to video layer's display rate and wbc frame rate */
    VO_WBC_MODE_PROG_TO_INTL,               /* In this mode, wbc will drop dev repeat frame which repeats more than 3 times,
                                                     and change two progressive frames to one interlace frame */
    
    VO_WBC_MODE_BUTT,
} VO_WBC_MODE_E;

typedef struct hiVO_PUB_ATTR_S
{
    HI_U32                   u32BgColor;          /* Background color of a device, in RGB format. */
    VO_INTF_TYPE_E           enIntfType;          /* Type of a VO interface */
    VO_INTF_SYNC_E           enIntfSync;          /* Type of a VO interface timing */
    VO_SYNC_INFO_S           stSyncInfo;          /* Information about VO interface timings */
} VO_PUB_ATTR_S;

typedef struct hiVO_CAS_ATTR_S
{
    HI_BOOL         bSlave;
    VO_CAS_RGN_E    enCasRgn;
    VO_CAS_MODE_E   enCasMode;
} VO_CAS_ATTR_S;

typedef struct hiVO_VIDEO_LAYER_ATTR_S
{
    RECT_S stDispRect;                  /* Display resolution */
    SIZE_S stImageSize;                 /* Canvas size of the video layer */
    HI_U32 u32DispFrmRt;                /* Display frame rate */
    PIXEL_FORMAT_E enPixFormat;         /* Pixel format of the video layer */
    HI_BOOL bDoubleFrame;               /* Whether to double frames */  
    HI_BOOL bClusterMode;               /* Whether to take Cluster way to use memory*/ 
} VO_VIDEO_LAYER_ATTR_S;

typedef struct hiBORDER_S
{
    HI_U32 u32TopWidth;
    HI_U32 u32BottomWidth;
    HI_U32 u32LeftWidth;
    HI_U32 u32RightWidth;
    HI_U32 u32Color;
} BORDER_S;

typedef struct hiVO_BORDER_S
{    
    HI_BOOL bBorderEn;                   /*do Frame or not*/  
    BORDER_S stBorder;
}VO_BORDER_S;

HI_BOOL VOU_DRV_Is_Progressive(VO_DEV VoDev);

HI_VOID VOU_DRV_WriteReg(HI_U32 u32Addr, HI_U32 u32Value);
HI_VOID VOU_DRV_ReadReg(HI_U32 u32Addr, HI_U32 *pu32Value);
HI_VOID VOU_DRV_DacSwitch(HI_U32 u32DacId, HI_BOOL bOpen);
HI_VOID VOU_DRV_BoardInit(HI_VOID);
HI_VOID VOU_DRV_BoardDeInit(HI_VOID);
HI_VOID VOU_DRV_IntRegUpMode(VO_LAYER VoLayer, VOU_INT_MODE_E IntMode);
HI_VOID VOU_DRV_SetDevIntfType(VO_DEV VoDev, VO_INTF_TYPE_E enIntfType);
HI_VOID VOU_DRV_SetDevBkGrd(VO_DEV VoDev, HI_U32 u32BgColor);
HI_VOID VOU_DRV_SetDevOutSync(VO_DEV VoDev, VO_INTF_SYNC_E enVoOutMode);
HI_VOID VOU_DRV_SetDevVtth(VO_DEV VoDev, HI_U32 u32Vtth);
HI_VOID VOU_DRV_GetDevVtth(VO_DEV VoDev, HI_U32* pu32Vtth);
HI_BOOL VOU_DRV_GetDevEnable(VO_DEV VoDev);
HI_VOID VOU_DRV_GetDevDispMode(VO_DEV VoDev, HI_BOOL *pbInterace);
HI_VOID VOU_DRV_GetDevIntState(VO_DEV VoDev, HI_BOOL *pbBottom);
HI_VOID VOU_DRV_GetDevIntStateVblank(VO_DEV VoDev, HI_BOOL *pbVblank);
HI_VOID VOU_DRV_GetDevIntStateVbackBlank(VO_DEV VoDev, HI_BOOL *pbVbackBlank);
HI_VOID VOU_DRV_SetUsrSyncTiming(VO_DEV VoDev, VO_SYNC_INFO_S *pstCfg);
HI_VOID VOU_DRV_DateSetting(VO_DEV VoDev, VO_INTF_SYNC_E enOutSync);
HI_VOID VOU_DRV_HDateSetting(VO_DEV VoDev, VO_INTF_SYNC_E enOutSync);
HI_VOID VOU_DRV_GammaConfig(VO_DEV VoDev);
HI_VOID VOU_DRV_GammaEnable(VO_DEV VoDev, HI_BOOL bEnable);


/* interrupt relative                                                       */
HI_VOID VOU_DRV_DevIntEnable(VO_DEV VoDev, HI_BOOL Enable);
HI_VOID VOU_DRV_IntClear(HI_U32 u32IntClear);
HI_VOID VOU_DRV_IntDisableAll(HI_VOID);
HI_U32 VOU_DRV_IntGetStatus(HI_VOID);
HI_U32 VOU_DRV_IntGetRawStatus(HI_VOID);
HI_VOID VOU_DRV_IntSetMode(VO_DEV VoDev, VOU_INT_MODE_E IntMode);
HI_VOID VOU_DRV_IntGetMode(VO_DEV VoDev, VOU_INT_MODE_E *pIntMode);
HI_VOID VOU_DRV_IntSetVTTH(VO_DEV VoDev, HI_U32 Threshold);


/* surface relative                                                         */
HI_VOID VOU_DRV_SetLayerDdr(VO_LAYER VoLayer, VOU_LAYER_DDR_E enDdrSelId);
HI_VOID VOU_DRV_LayerEnable(VO_LAYER VoLayer, HI_BOOL Enable);
HI_BOOL VOU_DRV_GetLayerEnable(VO_LAYER VoLayer);
HI_VOID VOU_DRV_SetLayerAlpha(VO_LAYER VoLayer, HI_U32 Alpha);
HI_VOID VOU_DRV_SetLayerBkGrd(VO_LAYER VoLayer, HI_U32 u32BkGrd);
HI_VOID VOU_DRV_SetLayerDataFmt(VO_LAYER VoLayer, VOU_LAYER_PIXERL_FORMAT_E enDataFmt);
HI_VOID VOU_DRV_SetImageInReso(VO_LAYER VoLayer, RECT_S *pstRect);
HI_VOID VOU_DRV_SetImageOutReso(VO_LAYER VoLayer, RECT_S *pstRect);
HI_VOID VOU_DRV_SetLayerDispRect(VO_LAYER VoLayer, RECT_S *pstLayerRect);
HI_VOID VOU_DRV_SetLayerVideoRect(VO_LAYER VoLayer, RECT_S *pstLayerRect);
HI_VOID VOU_DRV_SetMultiAreaEnable(VO_LAYER VoLayer,
                                              HI_U32 u32area_num,
                                              HI_BOOL bEnable);
HI_BOOL VOU_DRV_GetMultiAreaEnable(VO_LAYER VoLayer,
                                              HI_U32 u32area_num);

HI_VOID VOU_DRV_DisableAllArea(VO_LAYER VoLayer);
HI_VOID VOU_DRV_SetMultiAddrStride(
                        VO_LAYER VoLayer,
                        HI_U32      u32Area_num,
                        HI_U32      u32LayerLumaAddr,
                        HI_U32      u32LayerChromaAddr,
                        HI_U32      u32LayerLumaStride,
                        HI_U32      u32LayerChromaStride);
HI_VOID VOU_DRV_MultiAreaReso(
                        VO_LAYER VoLayer,
                        HI_U32 u32area_num,
                        HI_U32 u32Width, 
                        HI_U32 u32Height);
HI_VOID VOU_DRV_MultiAreaRect(
                        VO_LAYER VoLayer,
                        HI_U32 u32area_num,
                        RECT_S *pstRect);
HI_VOID VOU_DRV_LoadCoef(HI_VOID *pVirtAddr, HI_U32 u32PhyAddr);
HI_VOID VOU_DRV_LayerAccConfig(VO_LAYER VoLayer);
HI_VOID VOU_DRV_LayerAccCtrl(VO_LAYER VoLayer, HI_BOOL bEnable, HI_U32 u32AccMode);
HI_VOID VOU_DRV_LayerAcmConfig(VO_LAYER VoLayer, VOU_ACM_S *pstAcmAttr);
HI_VOID VOU_DRV_DefLayerAcmConfig(VO_LAYER VoLayer);
HI_S32 VOU_DRV_GraphicsSetCscCoef(VO_LAYER VoLayer);
HI_VOID VOU_DRV_LayerCSCEnable(VO_LAYER VoLayer, HI_BOOL bCscEn);
HI_VOID VOU_DRV_LayerCSCConfig(VO_LAYER VoLayer, VO_CSC_S *pstCsc);
HI_VOID VOU_DRV_VGACscConfig(VO_CSC_S *pstVgaCsc);
HI_S32  VOU_DRV_CheckVgaParam(VO_VGA_PARAM_S * pstVgaParam);
HI_VOID VOU_DRV_GetVgaDefParam(VO_VGA_PARAM_S *pstVgaParam);
HI_VOID VOU_DRV_SetIntfHspDefParam(VO_INTF_TYPE_E enIntf, HI_BOOL bShrpenEn ,HI_S32 s32Strength);
HI_VOID VOU_DRV_SetVgaGc(HI_U32 u32Value);
HI_S32  VOU_DRV_CheckVideoCsc(VO_LAYER VoLayer,VO_CSC_S *pstCSC);


HI_VOID VOU_DRV_LayerZoomEnable(VO_LAYER VoLayer, HI_BOOL Enable);
HI_VOID VOU_DRV_LayerVChZoomEnable(VO_LAYER VoLayer, HI_BOOL Enable);
HI_VOID VOU_DRV_LayerVChZoomFir(VO_LAYER VoLayer, HI_BOOL Enable);
HI_VOID VOU_DRV_LayerHChZoomFir(VO_LAYER VoLayer, HI_BOOL Enable);
HI_VOID VOU_DRV_SetZoomReso(VO_LAYER VoLayer, HI_U32 u32InWidth,
    HI_U32 u32InHeight, HI_U32 u32OutWidth, HI_U32 u32OutHeight);
HI_VOID VOU_DRV_SetZoomVerType(VO_LAYER VoLayer, HI_U32 uVerType);
HI_VOID VOU_DRV_SetZoomHfirOrder(VO_LAYER VoLayer, HI_U32 uHfirOrder);
HI_VOID VOU_DRV_ZoomConfig(VO_LAYER VoLayer, VO_SCREEN_HFILTER_E enHFilter, VO_SCREEN_VFILTER_E enVFilter);
HI_VOID VOU_DRV_SetHLOffset(VO_WBC VoWbc, VOU_DISP_ZMEMODE_E enMode, HI_S32 s32Phase);
HI_VOID VOU_DRV_ZoomFieldOffset(VO_LAYER VoLayer);
HI_VOID VOU_DRV_GetZoomPhase(HI_U32 u32Source,HI_U32 u32Target,HI_S32 * ps32ZoomPhase);


/* mixer relative                                                           */
HI_VOID VOU_DRV_LayerBindDev(VO_LAYER VoLayer, VO_DEV VoDev);
HI_VOID VOU_DRV_SetLayerPriority(VO_LAYER VoLayer, HI_U32 u32Priority);
HI_VOID VOU_DRV_GetLayerPriority(VO_LAYER VoLayer, HI_U32 *pu32Priority);

HI_VOID VOU_DRV_DefLayerBindDev(HI_VOID);

HI_VOID VOU_DRV_SetCasRgn(VO_CAS_RGN_E enVoCasRgn);
HI_VOID VOU_DRV_SetCasMode(HI_BOOL bSlave, VO_CAS_MODE_E enVoCasMode, HI_BOOL bEven);
HI_VOID VOU_DRV_SetCasSlaveMode(HI_BOOL bSlave);
HI_S32  VOU_DRV_GetCasMode(VO_CAS_MODE_E *penVoCasMode, HI_BOOL *pbEven);
HI_VOID VOU_DRV_SetCasEnable(HI_BOOL bEnable);
HI_VOID VOU_DRV_SetCasPosEnalbe(HI_U32 u32Pos, HI_BOOL bEnable);
HI_VOID VOU_DRV_ClearAllCasPosEn(HI_VOID);
HI_VOID VOU_DRV_SetCasPattern(HI_U32 u32Pattern);

HI_VOID VOU_DRV_Open(VO_DEV VoDev);
HI_VOID VOU_DRV_Close(VO_DEV VoDev);
HI_VOID VOU_DRV_Enable(VO_DEV VoDev);
HI_VOID VOU_DRV_Disable(VO_DEV VoDev);
HI_VOID VOU_DRV_EnableLayer(VO_LAYER VoLayer);
HI_VOID VOU_DRV_DisableLayer(VO_LAYER VoLayer);

HI_VOID VOU_DRV_EnableWbc(VO_WBC VoWbc, HI_BOOL bEnable);
HI_VOID VOU_DRV_EnableWbcStop(VO_WBC VoWbc);
HI_S32 VOU_DRV_CheckWbcDataSrc(VO_WBC VoWbc, VO_WBC_SOURCE_S *pstWbcSource);
HI_S32 VOU_DRV_CheckWbcAttr(VO_WBC VoWbc, VO_WBC_ATTR_S *pstWbcAttr);
HI_S32 VOU_DRV_CheckWbcMode(VO_WBC VoWbc, VO_WBC_MODE_E enMode);
HI_VOID VOU_DRV_SetWbcOReso(VO_WBC VoWbc, HI_U32 u32Width, HI_U32 u32Height);
HI_VOID VOU_DRV_SetWbcOutFmt(VO_WBC VoWbc, PIXEL_FORMAT_E enDataFmt);
HI_U32 VOU_DRV_SetWbcDataSource(VO_WBC VoWbc, VO_WBC_SOURCE_S *pstDataSource);
HI_VOID VOU_DRV_SetWbcAddrStride(
                        VO_WBC      VoWbc,
                        HI_U32      u32LayerLumaAddr,
                        HI_U32      u32LayerChromaAddr,
                        HI_U32      u32LayerLumaStride,
                        HI_U32      u32LayerChromaStride);
HI_VOID VOU_DRV_GetWbcHalLayer(VO_WBC VoWbc,VO_LAYER *pLayerWbc);

HI_VOID VOU_DRV_DefaultSetting(HI_VOID);
HI_S32  VOU_DRV_CheckDevId(VO_DEV VoDev);
HI_S32  VOU_DRV_CheckCasPatten(HI_S32 s32Patten);
HI_S32  VOU_DRV_DevCapabilityInit(VO_DEV VoDev);
HI_VOID VOU_DRV_DevInfoInit(HI_VOID);
HI_VOID VOU_DRV_LayerInfoInit(HI_VOID);
HI_S32  VOU_DRV_DevBindInit(VO_DEV VoDev, HI_U32 *pu32VoLayerNum, VO_LAYER *pVoLayer);
HI_BOOL VOU_DRV_IsVirtDev(VO_DEV VoDev);
HI_BOOL VOU_DRV_IsCasDev(VO_DEV VoDev);
HI_BOOL VOU_DRV_IsPhyDevHD(VO_DEV VoDev);
HI_BOOL VOU_DRV_IsPhyDevSD(VO_DEV VoDev);
HI_BOOL VOU_DRV_IsDevSupportCas(VO_DEV VoDev);
HI_BOOL VOU_DRV_IsDevSupportWbc(VO_DEV VoDev);
HI_BOOL VOU_DRV_IsDevSupportGetScrnFrm(VO_DEV VoDev);
HI_BOOL VOU_DRV_IsDevSetCascade(VO_DEV VoDev);
HI_BOOL VOU_DRV_IsChipSupportPip(HI_VOID);
HI_BOOL VOU_DRV_IsChipSupportWbc(HI_VOID);
HI_BOOL VOU_DRV_IsChipSupportVGA(HI_VOID);
HI_BOOL VOU_DRV_IsChipSupportCas(HI_VOID);
HI_BOOL VOU_DRV_IsLayerSupportZoom(VO_LAYER VoLayer);
HI_BOOL VOU_DRV_IsHdVpssRciv(VO_DEV VoDev);
HI_BOOL VOU_DRV_IsSdTdeDisp(VO_DEV VoDev);
HI_S32 VOU_DRV_GetCoefSize(VO_DEV VoDev);
HI_BOOL VOU_DRV_IsPipD1Limit(VO_LAYER VoLayer);

/*查询当前设备的输出接口是否支持负载检测*/
HI_BOOL VOU_DRV_IsDevSupportLoadDetect(VO_DEV VoDev);
HI_S32 VOU_DRV_LoadDetectInit(VO_DEV VoDev);
HI_S32 VOU_DRV_LoadDetectDeInit(VO_DEV VoDev);
HI_BOOL VOU_DRV_CheckDetectInt(VO_DEV VoDev, HI_U32 u32IntStatus);
HI_S32 VOU_DRV_EnableLoadDetect(VO_DEV VoDev);
HI_S32 VOU_DRV_DisableLoadDetect(VO_DEV VoDev);

	
HI_S32  VOU_DRV_CheckDevPubAttr(VO_DEV VoDev, VO_PUB_ATTR_S *pstPubAttr);
HI_S32  VOU_DRV_SetPubAttr(VO_DEV VoDev, VO_PUB_ATTR_S *pstVoPubAttr);
HI_VOID VOU_DRV_SetDispMaxSize(VO_DEV VoDev, HI_U32 u32MaxWidth, HI_U32 u32MaxHeight);
HI_VOID  VOU_DRV_SetDevClk(VO_DEV VoDev);


HI_S32  VOU_DRV_GetCasPhyDev(HI_VOID);
HI_S32  VOU_DRV_GetCasPhyLayer(HI_VOID);
HI_S32  VOU_DRV_GetOddCasDev(HI_VOID);
HI_S32  VOU_DRV_GetEvenCasDev(HI_VOID);
HI_U32  VOU_DRV_GetWbcIntMsk(HI_VOID);
HI_S32  VOU_DRV_GetDefDispBlkNum(VO_DEV VoDev);

HI_S32  VOU_DRV_GetInterruptDev(HI_U32 IntStatus, VO_DEV *pVoDev);

HI_VOID VOU_DRV_SetCasAttr(VO_CAS_ATTR_S *pstVoCasAttr);

HI_S32  VOU_DRV_Dev2Layer(VO_DEV VoDev);
//HI_S32  VOU_DRV_VtThd2Dev(VOU_INT_MASK_E enIntMskVtThd);
HI_S32  VOU_DRV_VirtDev2VirtId(VO_DEV VoDev);


HI_S32  VOU_DRV_CheckLayerAttr(VO_DEV VoDev, VO_LAYER VoLayer,
                 VO_VIDEO_LAYER_ATTR_S *pstVideoAttr);

HI_BOOL VOU_DRV_CheckHdmiEn(HI_VOID);
HI_BOOL VOU_DRV_AssertLayerVGS(VOU_SW_LAYER_E enVoLayer);

HI_VOID VOU_DRV_CrgClkInit(HI_VOID);
HI_VOID VOU_DRV_CrgClkExit(HI_VOID);
HI_VOID VOU_DRV_ResetAllCrgClk(HI_VOID);
HI_VOID VOU_DRV_SetLayerRegUp(VO_LAYER VoLayer);

HI_S32 VOU_DRV_GetVDACStatus(VO_DEV VoDev);

HI_VOID VOU_DRV_GetDefaultCsc(VO_LAYER VoLayer, VO_CSC_S *pstCsc);
HI_VOID VOU_DRV_InitDefaultCsc(VO_LAYER VoLayer);

HI_S32 VOU_DRV_GetVideoLayerBindDev(VO_LAYER VoLayer);

HI_U32 VOU_DRV_GetDevLayerNum(VO_DEV VoDev);
HI_U32 VOU_DRV_SetDevLayerNum(VO_DEV VoDev , HI_U32 u32LayerNum);

HI_VOID VOU_DRV_GetDevLayer(VO_DEV VoDev , VO_LAYER *pVoLayer);
HI_BOOL VOU_DRV_IsLayerEnlargeable(VO_LAYER VoLayer);
HI_S32 VOU_DRV_CheckVideoBindParam(VO_LAYER VoLayer, VO_DEV VoDev);

HI_S32 VOU_DRV_VideoBindDev(VO_LAYER VoLayer,VO_DEV VoDev);
HI_S32 VOU_DRV_VideoUnBindDev(VO_LAYER VoLayer,VO_DEV VoDev);

HI_BOOL VOU_DRV_IsLayerSizeLimit(VO_LAYER VoLayer);
HI_U32 VOU_DRV_GetLayerSize(VO_LAYER VoLayer);
HI_U32 VOU_DRV_GetLayerChnNum(VO_LAYER VoLayer);

HI_S32 VOU_DRV_SetVideoLayerBindDev(VO_LAYER VoLayer , VO_DEV VoDev);
HI_VOID VOU_DRV_WbcInfoInit(HI_VOID);
HI_S32 VOU_DRV_SetWbcSource(VO_WBC VoWbc,VO_WBC_SOURCE_S *pstWbcSource);
HI_VOID VOU_DRV_GetDevWbc(VO_DEV VoDev , VO_WBC *pVoWbc);
HI_S32 VOU_DRV_SetWbcMode(VO_WBC VoWbc, VO_WBC_MODE_E enMode);

HI_S32 VOU_DRV_CheckLayerId(VO_LAYER VoLayer);
HI_S32  VOU_DRV_CheckWbcId(VO_WBC  wbc);
HI_S32 VOU_DRV_SetWbcDev(VO_WBC VoWbc, VO_DEV VoDev);

HI_U32 VOU_DRV_CheckBorderParam(VO_LAYER VoLayer,VO_CHN VoChn,const VO_BORDER_S *pstBorder);
HI_S32 VOU_DRV_GetDevBaseLayer(VO_DEV VoDev);

HI_S32 VOU_DRV_SetLayerAttr(VO_LAYER VoLayer, VO_VIDEO_LAYER_ATTR_S *pstVoLayerAttr);

HI_BOOL VOU_DRV_IsSupportChnFiled(HI_VOID);
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* End of __VOU_DRV_H__ */

