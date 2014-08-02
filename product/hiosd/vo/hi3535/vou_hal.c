/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : vou_hal.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2011/06/28
  Description   :
  History       :
  1.Date        : 2011/06/28
    Author      : n00168968    
    Modification: Created file
  2.Date        : 2012/06/19
    Author      : l00181524    
    Modification: Modify file

******************************************************************************/
#ifdef __LINUX_OS__
#include <linux/kernel.h>       /* printk */
#include <asm/hardware.h>       /* IO_ADDRESS */
#include <asm/io.h>             /* ioremap_nocache */
#include <linux/delay.h>        /* for ndelay(ns) */
#else
#include <common.h>
#include <command.h>
#include <malloc.h>
//#include <devices.h>
#include <version.h>
#include <net.h>
#include <asm/io.h>
#include <asm/arch/platform.h>
#include <asm/sizes.h>
#endif

#include "vou_hal.h"

#ifdef __cplusplus
#if __cplusplus
    extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

/****************************************************************************
 * MACRO DEFINITION                                                         *
 ****************************************************************************/
#define HAL_PRINT printf
#define IO_ADDRESS(x) (x)

#define HI_INVALID_LAYER (-1)

/***************************************/
#define CRG_REGS_ADDR   0x20030000
#define CRG_REGS_SIZE   0X10000

#define CRG_PERCTL2_ADDR (0x0008 + CRG_REGS_ADDR)
#define CRG_PERCTL3_ADDR (0x000C + CRG_REGS_ADDR)
#define CRG_PERCTL4_ADDR (0x0010 + CRG_REGS_ADDR)
#define CRG_PERCTL5_ADDR (0x0014 + CRG_REGS_ADDR)
#define CRG_PERCTL6_ADDR (0x0018 + CRG_REGS_ADDR)
#define CRG_PERCTL7_ADDR (0x001c + CRG_REGS_ADDR)


#define CRG_PERCTL13_ADDR (0x0034 + CRG_REGS_ADDR)
#define CRG_PERCTL15_ADDR (0x003C + CRG_REGS_ADDR)

#define HDMI_REGS_ADDR  0x201f0000L
#define HDMI_REGS_SIZE  0X10000

#define HDMI_DE_DLY_ADDR        (0x32*4 + HDMI_REGS_ADDR)
#define HDMI_DE_CTRL_ADDR       (0x33*4 + HDMI_REGS_ADDR)
#define HDMI_DE_TOP_ADDR        (0x34*4 + HDMI_REGS_ADDR)
#define HDMI_DE_CNTH_ADDR       (0x37*4 + HDMI_REGS_ADDR)
#define HDMI_DE_CNTL_ADDR       (0x36*4 + HDMI_REGS_ADDR)
#define HDMI_DE_LINH_ADDR       (0x39*4 + HDMI_REGS_ADDR)
#define HDMI_DE_LINL_ADDR       (0x38*4 + HDMI_REGS_ADDR)
#define HDMI_HBIT_2HSYNC1_ADDR  (0x40*4 + HDMI_REGS_ADDR)
#define HDMI_HBIT_2HSYNC2_ADDR  (0x41*4 + HDMI_REGS_ADDR)
#define HDMI_FLD2_HS_OFSTH_ADDR (0x43*4 + HDMI_REGS_ADDR)
#define HDMI_FLD2_HS_OFSTL_ADDR (0x42*4 + HDMI_REGS_ADDR)
#define HDMI_HWIDTH1_ADDR       (0x44*4 + HDMI_REGS_ADDR)
#define HDMI_HWIDTH2_ADDR       (0x45*4 + HDMI_REGS_ADDR)
#define HDMI_VBIT_TO_VSYNC_ADDR (0x46*4 + HDMI_REGS_ADDR)
#define HDMI_VWIDTH_ADDR        (0x47*4 + HDMI_REGS_ADDR)
#define HDMI_VID_MODE_ADDR      (0x4a*4 + HDMI_REGS_ADDR)

/****************************************************************************
 * EXTERN VARIABLES                                                         *
 ****************************************************************************/

/****************************************************************************
 * GLOBAL VARIABLES                                                         *
 ****************************************************************************/
 volatile VOU_REGS_S *pVoReg = NULL;

/****************************************************************************
 * EXTERN FUNCTION DECLARATION                                              *
 ****************************************************************************/


/****************************************************************************
 * INTERNAL FUNCTION DECLARATION                                            *
 ****************************************************************************/

/****************************************************************************
 * VOU FUNCTION                                                             *
 *            ---- Assistant Function Area                                  *
 ****************************************************************************/


/*****************************************************************************
 Prototype         : Hal initial
 Description     :

*****************************************************************************/    
HI_VOID HAL_VOU_Init(HI_VOID)
{
#if 1
    pVoReg = (volatile VOU_REGS_S *)IO_ADDRESS(VO_BASE_ADDR);
#else
    pVoReg = (volatile VOU_REGS_S *)ioremap_nocache(VO_BASE_ADDR,sizeof(VOU_REGS_S));
#endif
}

HI_VOID HAL_VOU_Exit(HI_VOID)
{
#if 0
    iounmap(pVoReg);
#endif
}

static inline void HI_RegSetBit(unsigned long value, unsigned long offset,
    unsigned long addr)
{
	unsigned long t, mask;

	mask = 1 << offset;
	t = readl(addr);
	t &= ~mask;
	t |= (value << offset) & mask;
	writel(t, addr);
}

static inline void HI_RegWrite32(unsigned long value, unsigned long mask,
    unsigned long addr)
{
	unsigned long t;

	t = readl(addr);
	t &= ~mask;
	t |= value & mask;
	writel(t, addr);
}

HI_VOID HAL_WriteReg(HI_U32 *pAddress, HI_U32 Value)
{
    *(volatile HI_U32*)pAddress = Value;
    return;
}

HI_U32 HAL_ReadReg(HI_U32 *pAddress)
{
    return *(volatile HI_U32*)(pAddress);
}

HI_S32 HDMI_AV_Config_sep(HI_VOID)
{    
    HI_U8 u8DeDly = 0;
    HI_U8 u8DeTop = 0;
    HI_U8 u8DeCnt = 0;
    HI_U8 u8DeLin = 0;
    HI_U8 u8DeGen = 0;
    HI_U8 u8Hbit2Hsync = 0;
    HI_U8 u8Field2Ofst = 0;
    HI_U8 u8HWidth     = 0;
    HI_U8 u8Vbit2Vsync = 0;
    HI_U8 u8VWidth     = 0;
    HI_U8 u8Syncext    = 0;
    HI_U8 u8Demux      = 0;
    HI_U8 u8Upsmp      = 0;
    HI_U8 u8Csc        = 1;
    HI_RegWrite32(0x28, 0xffffffff, IO_ADDRESS(0x205d1804));
    HI_RegWrite32(0x60, 0xffffffff, IO_ADDRESS(0x205d1820));
    HI_RegWrite32(0xa9, 0xffffffff, IO_ADDRESS(0x205d1808));
    HI_RegWrite32(0x40, 0xffffffff, IO_ADDRESS(0x205d180c));

    HI_RegWrite32(u8DeDly & 0xff, 0xffffffff, IO_ADDRESS(HDMI_DE_DLY_ADDR));
    HI_RegWrite32((u8DeGen << 6) | ((u8DeDly >> 8) & 0xff), 0xffffffff, IO_ADDRESS(HDMI_DE_CTRL_ADDR));
    HI_RegWrite32(u8DeTop, 0xffffffff, IO_ADDRESS(HDMI_DE_TOP_ADDR));
    HI_RegWrite32(u8DeCnt >> 8, 0xffffffff, IO_ADDRESS(HDMI_DE_CNTH_ADDR));
    HI_RegWrite32(u8DeCnt & 0xff, 0xffffffff, IO_ADDRESS(HDMI_DE_CNTL_ADDR));
    HI_RegWrite32(u8DeLin >> 8, 0xffffffff, IO_ADDRESS(HDMI_DE_LINH_ADDR));
    HI_RegWrite32(u8DeLin & 0xff, 0xffffffff, IO_ADDRESS(HDMI_DE_LINL_ADDR));
    HI_RegWrite32(u8Hbit2Hsync & 0xff, 0xffffffff, IO_ADDRESS(HDMI_HBIT_2HSYNC1_ADDR));
    HI_RegWrite32(u8Hbit2Hsync >> 8, 0xffffffff, IO_ADDRESS(HDMI_HBIT_2HSYNC2_ADDR));
    HI_RegWrite32(u8Field2Ofst >> 8, 0xffffffff, IO_ADDRESS(HDMI_FLD2_HS_OFSTH_ADDR));
    HI_RegWrite32(u8Field2Ofst, 0xff, IO_ADDRESS(HDMI_FLD2_HS_OFSTL_ADDR));
    HI_RegWrite32(u8HWidth & 0xff, 0xffffffff, IO_ADDRESS(HDMI_HWIDTH1_ADDR));
    HI_RegWrite32(u8HWidth >> 8, 0xffffffff, IO_ADDRESS(HDMI_HWIDTH2_ADDR));
    HI_RegWrite32(u8Vbit2Vsync, 0xffffffff, IO_ADDRESS(HDMI_VBIT_TO_VSYNC_ADDR));
    HI_RegWrite32(u8VWidth, 0xffffffff, IO_ADDRESS(HDMI_VWIDTH_ADDR));
    HI_RegWrite32((u8Csc << 3) | (u8Upsmp << 2) | (u8Demux << 1) | u8Syncext,
        0xffffffff, IO_ADDRESS(HDMI_VID_MODE_ADDR));

    HI_RegWrite32(0x21, 0xffffffff, IO_ADDRESS(0x205d04bc));
    HI_RegWrite32(0x35, 0xffffffff, IO_ADDRESS(0x205d0020));

    return 0;
}


HI_S32 SYS_HAL_SelVoBt1120PinConifg(HI_VOID)
{
    HI_RegWrite32(0x1, 0xffffffff, IO_ADDRESS(0x200f0154));
    HI_RegWrite32(0x1, 0xffffffff, IO_ADDRESS(0x200f0158));
    HI_RegWrite32(0x1, 0xffffffff, IO_ADDRESS(0x200f015c));
    HI_RegWrite32(0x1, 0xffffffff, IO_ADDRESS(0x200f0160));
    HI_RegWrite32(0x1, 0xffffffff, IO_ADDRESS(0x200f0164));
    HI_RegWrite32(0x1, 0xffffffff, IO_ADDRESS(0x200f0168));
    HI_RegWrite32(0x1, 0xffffffff, IO_ADDRESS(0x200f016c));
    HI_RegWrite32(0x1, 0xffffffff, IO_ADDRESS(0x200f0170));
    HI_RegWrite32(0x1, 0xffffffff, IO_ADDRESS(0x200f0174));
    HI_RegWrite32(0x1, 0xffffffff, IO_ADDRESS(0x200f0178));
    HI_RegWrite32(0x1, 0xffffffff, IO_ADDRESS(0x200f017c));
    HI_RegWrite32(0x1, 0xffffffff, IO_ADDRESS(0x200f0180));
    HI_RegWrite32(0x1, 0xffffffff, IO_ADDRESS(0x200f0184));
    HI_RegWrite32(0x1, 0xffffffff, IO_ADDRESS(0x200f0188));
    HI_RegWrite32(0x1, 0xffffffff, IO_ADDRESS(0x200f018c));
    HI_RegWrite32(0x1, 0xffffffff, IO_ADDRESS(0x200f0190));
    HI_RegWrite32(0x1, 0xffffffff, IO_ADDRESS(0x200f0194));

    return 0;
}

HI_S32 SYS_HAL_SelVoVgaPinConifg(HI_VOID)
{
    HI_RegWrite32(0x0, 0xffffffff, IO_ADDRESS(0x200f0000));
    HI_RegWrite32(0x0, 0xffffffff, IO_ADDRESS(0x200f0004));
    return 0;
}

HI_S32 SYS_HAL_SelVoHdmiPinConifg(HI_VOID)
{

    HI_RegWrite32(0x1, 0xffffffff, IO_ADDRESS(0x200f00f4));
    HI_RegWrite32(0x1, 0xffffffff, IO_ADDRESS(0x200f00f8));
    HI_RegWrite32(0x1, 0xffffffff, IO_ADDRESS(0x200f00fc));
    HI_RegWrite32(0x1, 0xffffffff, IO_ADDRESS(0x200f0100));
    return 0;
}



HI_S32 SYS_HAL_SetVoHdmiReset(HI_BOOL bReset)
{
    if (bReset)
    {
        HI_RegWrite32(0x01, 0xffffffff, IO_ADDRESS(CRG_PERCTL15_ADDR));
    }
    else
    {
        HI_RegWrite32(0x0c, 0xffffffff, IO_ADDRESS(CRG_PERCTL15_ADDR));
    }
    return 0;
}

HI_S32 SYS_HAL_SelVoBt1120Clk(HI_U32 u32ClkSel)
{
    //HI_RegSetBit(u32ClkSel, 27, IO_ADDRESS(CRG_PERCTL14_ADDR));

    return 0;
}
///////////////////////////////////////////
HI_S32 SYS_HAL_VouHdDacClkEn(HI_BOOL pbClkEn)
{
    volatile HI_U32 u32Tmp = (pbClkEn == HI_TRUE) ? 1 : 0;

    HI_RegSetBit(u32Tmp, 6, IO_ADDRESS(CRG_PERCTL13_ADDR));


    return 0;
}
HI_S32 SYS_HAL_SelVoHdDacClk(HI_U32 u32ClkSel)
{
    HI_RegSetBit(u32ClkSel, 18, IO_ADDRESS(CRG_PERCTL13_ADDR));

    return 0;
}

#if 0
HI_S32 SYS_HAL_SelVoHddateClk(HI_U32 u32ClkSel)
{
    //HI_RegSetBit(u32ClkSel, 24, IO_ADDRESS(CRG_PERCTL14_ADDR));

    return 0;
}
#endif
HI_S32 SYS_HAL_SelVoHDMIClk(HI_U32 u32ClkSel)
{
    HI_RegSetBit(u32ClkSel, 11, IO_ADDRESS(CRG_PERCTL15_ADDR));

    return 0;
}

HI_S32 SYS_HAL_VouSdDacClkEn(HI_BOOL pbClkEn)
{
    volatile HI_U32 u32Tmp = (pbClkEn == HI_TRUE) ? 1 : 0;

    HI_RegSetBit(u32Tmp, 7, IO_ADDRESS(CRG_PERCTL13_ADDR));

    return 0;
}

HI_S32 SYS_HAL_SelVoClk(HI_U32 u32ClkSel)
{
    HI_RegWrite32(u32ClkSel << 14, 0x3 << 14, IO_ADDRESS(CRG_PERCTL13_ADDR));
    return 0;
}

HI_S32 SYS_HAL_SelVoSdClkDiv(HI_U32 u32ClkSel)
{
    HI_RegSetBit(u32ClkSel, 13, IO_ADDRESS(CRG_PERCTL13_ADDR));

    return 0;
}

HI_S32 SYS_HAL_VouBusResetSel(HI_BOOL bReset)
{
    volatile HI_U32 u32Tmp = (bReset == HI_TRUE) ? 1 : 0;

    HI_RegSetBit(u32Tmp, 0, IO_ADDRESS(CRG_PERCTL13_ADDR));

    return 0;
}

HI_S32 SYS_HAL_VouDevClkEn(HI_S32 s32VoDev, HI_BOOL pbClkEn)
{
    volatile HI_U32 u32Tmp = (pbClkEn == HI_TRUE) ? 1 : 0;  
    volatile HI_U32 u32Bit;

    if (s32VoDev >= 0 && s32VoDev <= 2)
    {
        u32Bit = 9 + s32VoDev;  
        HI_RegSetBit(u32Tmp, u32Bit, IO_ADDRESS(CRG_PERCTL13_ADDR));
    }
    else
    {
        return -1;
    }

    return 0;
}

HI_S32 SYS_HAL_VouHdOutClkSel(HI_U32 u32ClkSel)
{
    HI_RegSetBit(u32ClkSel, 12, IO_ADDRESS(CRG_PERCTL13_ADDR));
    return 0;
}

HI_S32 SYS_HAL_VouBusClkEn(HI_BOOL pbClkEn)
{
    volatile HI_U32 u32Tmp = (pbClkEn == HI_TRUE) ? 1 : 0;

    HI_RegSetBit(u32Tmp, 8, IO_ADDRESS(CRG_PERCTL13_ADDR));

    return 0;
}

HI_S32 SYS_HAL_HdmiResetSel(HI_BOOL bReset)
{
    volatile HI_U32 u32Tmp = (bReset == HI_TRUE) ? 1 : 0;
     
    HI_RegSetBit(u32Tmp, 0, IO_ADDRESS(CRG_PERCTL15_ADDR));
    HI_RegSetBit(u32Tmp, 1, IO_ADDRESS(CRG_PERCTL15_ADDR));
        
    return 0;
}

HI_S32 SYS_HAL_HdmiPixelClkEn(HI_BOOL pbClkEn)
{
    volatile HI_U32 u32Tmp = (pbClkEn == HI_TRUE) ? 1 : 0;
          
    HI_RegSetBit(u32Tmp, 3, IO_ADDRESS(CRG_PERCTL15_ADDR));
        
    return 0;
}

HI_S32 SYS_HAL_HdmiBusClkEn(HI_BOOL pbClkEn)
{
    volatile HI_U32 u32Tmp = (pbClkEn == HI_TRUE) ? 1 : 0;
          
    HI_RegSetBit(u32Tmp, 2, IO_ADDRESS(CRG_PERCTL15_ADDR));
        
    return 0;
}
#if 0
HI_S32 SYS_HAL_SelVoHdClk(HI_S32 VoDev, HI_U32 u32Pll)
{
    return 0;
}
#endif
HI_S32 SYS_HAL_SetVoPllFrac(HI_S32 s32Pll, HI_U32 u32BitsSet)
{
    if (0 == s32Pll)
    {
        HI_RegWrite32(u32BitsSet << 0, 0xffffff << 0, IO_ADDRESS(CRG_PERCTL4_ADDR));        
        
        /* 如果采用小数分频系数，要将对应的模式设为小数模式 */
        if (0 == u32BitsSet)
        {
            HI_RegSetBit(1, 24, IO_ADDRESS(CRG_PERCTL5_ADDR));
        }
        else
        {
            HI_RegSetBit(0, 24, IO_ADDRESS(CRG_PERCTL5_ADDR));
        }
    }
    else if(1 == s32Pll)
    {
        HI_RegWrite32(u32BitsSet << 0, 0xffffff << 0, IO_ADDRESS(CRG_PERCTL6_ADDR));        
        
        /* 如果采用小数分频系数，要将对应的模式设为小数模式 */
        if (0 == u32BitsSet)
        {
            HI_RegSetBit(1, 24, IO_ADDRESS(CRG_PERCTL7_ADDR));
        }
        else
        {
            HI_RegSetBit(0, 24, IO_ADDRESS(CRG_PERCTL7_ADDR));
        }
    }
    else 
    {
        return -1;
    }
    return 0;
}

HI_S32 SYS_HAL_SetVoPllPostdiv1(HI_S32 s32Pll, HI_U32 u32BitsSet)
{
    if (0 == s32Pll)
    {
        HI_RegWrite32(u32BitsSet << 24, 0x7 << 24, IO_ADDRESS(CRG_PERCTL4_ADDR));
    }
    else if(1 == s32Pll)
    {
        HI_RegWrite32(u32BitsSet << 24, 0x7 << 24, IO_ADDRESS(CRG_PERCTL6_ADDR));
    }
    else
    {
        return -1;
    }
    return 0;
}

HI_S32 SYS_HAL_SetVoPllPostdiv2(HI_S32 s32Pll, HI_U32 u32BitsSet)
{
    if (0 == s32Pll)
    {
        HI_RegWrite32(u32BitsSet << 28, 0x7 << 28, IO_ADDRESS(CRG_PERCTL4_ADDR));
    }
    else if(1 == s32Pll)
    {
        HI_RegWrite32(u32BitsSet << 28, 0x7 << 28, IO_ADDRESS(CRG_PERCTL6_ADDR));
    }
    else
    {
        return -1;
    }
    return 0;
}
HI_S32 SYS_HAL_SetVoPllRefdiv(HI_S32 s32Pll, HI_U32 u32BitsSet)
{
    if (0 == s32Pll)
    {
        HI_RegWrite32(u32BitsSet << 12, 0x3f << 12, IO_ADDRESS(CRG_PERCTL5_ADDR));
    }
    else if(1 == s32Pll)
    {
        HI_RegWrite32(u32BitsSet << 12, 0x3f << 12, IO_ADDRESS(CRG_PERCTL7_ADDR));
    }
    else
    {
        return -1;
    }
    return 0;
}
HI_S32 SYS_HAL_SetVoPllFbdiv(HI_S32 s32Pll, HI_U32 u32BitsSet)
{
    if (0 == s32Pll)
    {
        HI_RegWrite32(u32BitsSet << 0, 0xfff << 0, IO_ADDRESS(CRG_PERCTL5_ADDR));
    }
    else if(1 == s32Pll)
    {
        HI_RegWrite32(u32BitsSet << 0, 0xfff << 0, IO_ADDRESS(CRG_PERCTL7_ADDR));
    }
    else
    {
        return -1;
    }
    return 0;
}


#if 0
HI_S32 SYS_HAL_VouSdResetSel(HI_BOOL bReset)
{
    HI_U32 u32Tmp = (bReset == HI_TRUE) ? 1 : 0;

    HI_RegSetBit(u32Tmp, 2, IO_ADDRESS(CRG_PERCTL13_ADDR));

    return 0;
}

HI_S32 SYS_HAL_VouHdResetSel(HI_S32 s32VoDev, HI_BOOL bReset)
{
    HI_U32 u32Tmp = (bReset == HI_TRUE) ? 1 : 0;
    
    if (0 == s32VoDev)
    {
        HI_RegSetBit(u32Tmp, 1, IO_ADDRESS(CRG_PERCTL13_ADDR));
    }
    else
    {
        return -1;
    }

    return 0;
}

HI_S32 SYS_HAL_VouHdDateResetSel(HI_BOOL bReset)
{
    return 0;
}





HI_S32 SYS_HAL_VouSdDateClkEn(HI_S32 s32VoDev, HI_BOOL pbClkEn)
{
    HI_U32 u32Tmp = (pbClkEn == HI_TRUE) ? 1 : 0;  

    HI_RegSetBit(u32Tmp, 13, IO_ADDRESS(CRG_PERCTL13_ADDR));

    return 0;
}


HI_S32 SYS_HAL_VouHdDateClkEn(HI_BOOL pbClkEn)
{
    return 0;
}




HI_S32 SYS_HAL_VouHdDacClkEn(HI_S32 Vodev, HI_BOOL pbClkEn)
{
    HI_U32 u32Tmp = (pbClkEn == HI_TRUE) ? 1 : 0;

    if (0 == Vodev)
    {
        HI_RegSetBit(u32Tmp, 9, IO_ADDRESS(CRG_PERCTL13_ADDR));
    }
    else if (1 == Vodev)
    {
        HI_RegSetBit(u32Tmp, 10, IO_ADDRESS(CRG_PERCTL13_ADDR));
    }
    else
    {
        ;
    }

    return 0;
}


#endif

HI_VOID HAL_SYS_Control(HI_VOID)
{
    volatile U_VOCTRL VOCTRL;

    /* outstand */
    VOCTRL.u32 = pVoReg->VOCTRL.u32;
    VOCTRL.u32 = 0x80000000;
    pVoReg->VOCTRL.u32 = VOCTRL.u32;
}


HI_U32 inline HAL_GetXDC_BUMA(HI_S32 s32Value)
{
    HI_U32 u32AbsValue = s32Value << 2;

    if(s32Value >= 0)
    {
        return u32AbsValue & 0x7ff;
    }
    /*0~10bit有效，第10bit为符号位*/
    else
    {
        u32AbsValue = (~((-1)* u32AbsValue) +1);
        //return ( (((~u32AbsValue)+1)& 0xFF) | 0x100 );
        u32AbsValue = ((u32AbsValue & 0x7ff) | 0x400);
        return u32AbsValue;
    }
}

HI_U32 inline HAL_Conver_CscCoef(HI_S32 s32Value)
{
    HI_S32 s32Result = ((s32Value << 10)/ 1000 );

    /* 保留问题! 当CSC矩阵计算出来的结果s32Value是-1、-2等值时，
    *   s32Result将是0，换句话说0.001转换成二进制是0x0000 0000 01
    *   按注释掉的代码计算，配寄存器的值是0x1000，这个值会导致图像为绿色(50 2 50 50)
    */
    if (s32Result < 0)
    //if (s32Value < 0)
    {
        s32Result = (~((-1)*s32Result) + 1);

        /* 5.8 format */
        s32Result = ((s32Result & 0x7fff) | 0x4000);
    }
    else
    {
        s32Result = s32Result & 0x7fff;
    }
    
    return s32Result;
}


HI_U32 Vou_GetAbsAddr(HAL_DISP_LAYER_E enLayer, HI_U32 pReg)
{
    HI_U32 RegAbsAddr;
    switch(enLayer)
    {
        case HAL_DISP_LAYER_VHD0:
        {
            RegAbsAddr = (pReg) + enLayer * VHD_REG_LEN;
            break;
        }
        case HAL_DISP_LAYER_VHD1:
        {
            RegAbsAddr = (pReg) + (enLayer + 2) * VHD_REG_LEN;
            break;
        }
        case HAL_DISP_LAYER_VP:
        {
            RegAbsAddr = (pReg) + (enLayer - 1) * VHD_REG_LEN;//从VHD0开始映射，VSD0层归为高清层，相当于3531的VHD2
            break;
        }        
        case HAL_DISP_LAYER_VSD0:
        {
            RegAbsAddr = (pReg) + (enLayer + 1) * VHD_REG_LEN;
            break;
        }
        case HAL_DISP_LAYER_GFX0:
        case HAL_DISP_LAYER_GFX1:   /* fall through */
        case HAL_DISP_LAYER_HC0:    /* G2 */
        {
            RegAbsAddr = ((pReg)) + (enLayer - HAL_DISP_LAYER_GFX0) * GFX_REG_LEN;
            break;
        }
        case HAL_DISP_LAYER_GFX4:   /* no G3, so this needs to and 1 */
        {
            RegAbsAddr = ((pReg)) + (enLayer - HAL_DISP_LAYER_GFX0 + 1) * GFX_REG_LEN;
            break;
        }
        case HAL_DISP_LAYER_WBC_G0:
        case HAL_DISP_LAYER_WBC_G4:
        {
            RegAbsAddr = ((pReg)) + (enLayer - HAL_DISP_LAYER_WBC_G0) * WBC_REG_LEN;
            break;
        }
        case HAL_DISP_LAYER_WBC_D:
        {
            RegAbsAddr = (pReg) + (enLayer - HAL_DISP_LAYER_WBC_D) * WBC_REG_LEN;
            break;
        }
         default:
        {
            HAL_PRINT("Error channel id found in %s: L%d\n",__FUNCTION__, __LINE__);
            return 0;
        }
    }
    
    return RegAbsAddr;
}

HI_U32 Vou_GetChnAbsAddr(HAL_DISP_OUTPUTCHANNEL_E enChan, HI_U32 pReg)
{
    volatile HI_U32 RegAbsAddr;
    switch(enChan)
    {
        case HAL_DISP_CHANNEL_DHD0:
        case HAL_DISP_CHANNEL_DHD1:            
        case HAL_DISP_CHANNEL_DSD0:
        {
            RegAbsAddr =pReg + enChan * DHD_REG_LEN;
            break;
        }
        default:
        {  
            HAL_PRINT("Error channel id found in %s: L%d\n",__FUNCTION__, __LINE__);
            return 0;
        }
    }
     return RegAbsAddr;
}

 //for get Intf abs addr
HI_U32 Vou_GetIntfAbsAddr(HAL_DISP_INTF_E enIntf, HI_U32 pReg)
{
    volatile HI_U32 RegAbsAddr;
    switch(enIntf)
    {
        case HAL_DISP_INTF_HDMI:
        {
            RegAbsAddr =pReg ;
            break;
        }      
        case HAL_DISP_INTF_VGA: 
        {
            RegAbsAddr =pReg + INTF_REG_LEN;
            break;
        }        
        case HAL_DISP_INTF_BT1120: 
        {
            RegAbsAddr =pReg + 2 * INTF_REG_LEN;
            break;
        }        
        case HAL_DISP_INTF_CVBS: 
        {
            RegAbsAddr =pReg + 3 * INTF_REG_LEN;
            break;
        }      
        default:
        {  
            HAL_PRINT("Error intf id found in %s: L%d\n",__FUNCTION__, __LINE__);
            return 0;
        }
    }
     return RegAbsAddr;
}

HI_U32 HAL_GetBkg_BUMA(HI_U8 u8Bkg_YUV)
{
    HI_U32 u32Value = 0;
    return (u32Value|u8Bkg_YUV) << 2;
}
HI_VOID HAL_SYS_SetOutstanding(HI_VOID)
{
    volatile U_VOAXICTRL VOAXICTRL;

    VOAXICTRL.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->VOAXICTRL.u32));
    VOAXICTRL.bits.m0_wr_ostd = 3;
    HAL_WriteReg((HI_U32*)&(pVoReg->VOAXICTRL.u32), VOAXICTRL.u32); 

    return ;
}


HI_VOID HAL_SYS_SetRdBusId(HI_U32 bMode)
{
    
    return ;
}


//设置显示通道开关状态
HI_BOOL HAL_DISP_SetIntfEnable(HAL_DISP_OUTPUTCHANNEL_E enChan, HI_BOOL enIntf)
{
    volatile U_DHD0_CTRL DHD0_CTRL;     
    volatile HI_U32 addr_REG;
    if((CHANNEL_DHD_START <= enChan) && (enChan <= CHANNEL_DSD_END))
    {
        addr_REG = Vou_GetChnAbsAddr(enChan,(HI_U32)&(pVoReg->DHD0_CTRL.u32));
        DHD0_CTRL.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        DHD0_CTRL.bits.intf_en = enIntf;
        HAL_WriteReg((HI_U32*)addr_REG, DHD0_CTRL.u32); 
    }
    else
    {
        HAL_PRINT("Error channel id found in %s: L%d\n",__FUNCTION__, __LINE__);
        return HI_FALSE;
    }
    return HI_TRUE;
}

HI_BOOL HAL_DISP_GetIntfEnable(HAL_DISP_OUTPUTCHANNEL_E enChan, HI_BOOL *pbIntfEn)
{
    volatile U_DHD0_CTRL DHD0_CTRL;    
    
    volatile HI_U32 addr_REG;
    if((CHANNEL_DHD_START <= enChan) && (enChan <= CHANNEL_DSD_END))
    {
        addr_REG = Vou_GetChnAbsAddr(enChan,(HI_U32)&(pVoReg->DHD0_CTRL.u32));
        DHD0_CTRL.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        *pbIntfEn = DHD0_CTRL.bits.intf_en;
    }
    else
    {
        HAL_PRINT("Error channel id found in %s: L%d\n",__FUNCTION__, __LINE__);
        return HI_FALSE;
    }
    return HI_TRUE;    
}

HI_BOOL HAL_DISP_GetIntState(HAL_DISP_OUTPUTCHANNEL_E enChan, HI_BOOL *pbBottom)
{
    volatile U_DHD0_STATE DHD0_STATE;    
    
    volatile HI_U32 addr_REG;
    if((CHANNEL_DHD_START <= enChan) && (enChan <= CHANNEL_DSD_END))
    {
        addr_REG = Vou_GetChnAbsAddr(enChan,(HI_U32)&(pVoReg->DHD0_STATE.u32));
        DHD0_STATE.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        *pbBottom = DHD0_STATE.bits.bottom_field;
    }
    else
    {
        HAL_PRINT("Error channel id found in %s: L%d\n",__FUNCTION__, __LINE__);
        return HI_FALSE;
    }
    return HI_TRUE;
}

/* 获取接口显示是否为消隐区状态*/
HI_BOOL HAL_DISP_GetIntStateVblank(HAL_DISP_OUTPUTCHANNEL_E enChan, HI_BOOL *pbVblank)
{
    volatile U_DHD0_STATE DHD0_STATE;       
    volatile HI_U32 addr_REG;
    if((CHANNEL_DHD_START <= enChan) && (enChan <= CHANNEL_DSD_END))
    {
        addr_REG = Vou_GetChnAbsAddr(enChan,(HI_U32)&(pVoReg->DHD0_STATE.u32));
        DHD0_STATE.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        *pbVblank = DHD0_STATE.bits.vblank;
        }
        else
        {
        HAL_PRINT("Error channel id found in %s: L%d\n",__FUNCTION__, __LINE__);
        return HI_FALSE;
        }
    return HI_TRUE;
}

/* 获取接口显示是否为后消隐区状态*/
HI_BOOL HAL_DISP_GetIntStateVbackBlank(HAL_DISP_OUTPUTCHANNEL_E enChan, HI_BOOL *pbVbackBlank)
{
    volatile U_DHD0_STATE DHD0_STATE;         
    volatile HI_U32 addr_REG;
    if((CHANNEL_DHD_START <= enChan) && (enChan <= CHANNEL_DSD_END))
    {
        addr_REG = Vou_GetChnAbsAddr(enChan,(HI_U32)&(pVoReg->DHD0_STATE.u32));
        DHD0_STATE.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        *pbVbackBlank = DHD0_STATE.bits.vback_blank;
    }
        else
        {
        HAL_PRINT("Error channel id found in %s: L%d\n",__FUNCTION__, __LINE__);
        return HI_FALSE;
    }
    return HI_TRUE;
}

//配置接口时序
HI_BOOL HAL_DISP_SetIntfSync(HAL_DISP_OUTPUTCHANNEL_E enChan,
                                     HAL_DISP_SYNCINFO_S *pstSyncInfo)
{   
    volatile U_DHD0_CTRL DHD0_CTRL;
    volatile U_DHD0_VSYNC DHD0_VSYNC;    
    volatile U_DHD0_HSYNC1 DHD0_HSYNC1;
    volatile U_DHD0_HSYNC2 DHD0_HSYNC2; 
    volatile U_DHD0_VPLUS DHD0_VPLUS;
    volatile U_DHD0_PWR DHD0_PWR;
    volatile HI_U32 addr_REG;
    if((CHANNEL_DHD_START <= enChan) && (enChan <= CHANNEL_DSD_END))
    {
        addr_REG = Vou_GetChnAbsAddr(enChan,(HI_U32)&(pVoReg->DHD0_CTRL.u32));
        DHD0_CTRL.u32 = HAL_ReadReg((HI_U32*)addr_REG);        
        DHD0_CTRL.bits.iop   = pstSyncInfo->bIop;        
        HAL_WriteReg((HI_U32*)addr_REG, DHD0_CTRL.u32);   
            
        addr_REG = Vou_GetChnAbsAddr(enChan,(HI_U32)&(pVoReg->DHD0_HSYNC1.u32));
        DHD0_HSYNC1.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        DHD0_HSYNC1.bits.hact = pstSyncInfo->u16Hact -1;
        DHD0_HSYNC1.bits.hbb  = (pstSyncInfo->u16Hbb)-1;
        HAL_WriteReg((HI_U32*)addr_REG, DHD0_HSYNC1.u32); 

        addr_REG = Vou_GetChnAbsAddr(enChan,(HI_U32)&(pVoReg->DHD0_HSYNC2.u32));
        DHD0_HSYNC2.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        DHD0_HSYNC2.bits.hmid = (pstSyncInfo->u16Hmid == 0) ? 0 : pstSyncInfo->u16Hmid -1;
        DHD0_HSYNC2.bits.hfb  = (pstSyncInfo->u16Hfb)-1;
        HAL_WriteReg((HI_U32*)addr_REG, DHD0_HSYNC2.u32); 
        

        //Config VHD interface veritical timming
        addr_REG = Vou_GetChnAbsAddr(enChan,(HI_U32)&(pVoReg->DHD0_VSYNC.u32));
        DHD0_VSYNC.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        DHD0_VSYNC.bits.vact = pstSyncInfo->u16Vact  -1;
        DHD0_VSYNC.bits.vbb = pstSyncInfo->u16Vbb - 1;
        DHD0_VSYNC.bits.vfb =  pstSyncInfo->u16Vfb - 1;
        HAL_WriteReg((HI_U32*)addr_REG, DHD0_VSYNC.u32); 
        
        //Config VHD interface veritical bottom timming,no use in progressive mode
        addr_REG = Vou_GetChnAbsAddr(enChan,(HI_U32)&(pVoReg->DHD0_VPLUS.u32));
        DHD0_VPLUS.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        DHD0_VPLUS.bits.bvact = pstSyncInfo->u16Bvact - 1;
        DHD0_VPLUS.bits.bvbb = pstSyncInfo->u16Bvbb - 1;
        DHD0_VPLUS.bits.bvfb =  pstSyncInfo->u16Bvfb - 1;
        HAL_WriteReg((HI_U32*)addr_REG, DHD0_VPLUS.u32); 

        //Config VHD interface veritical bottom timming, 
        addr_REG = Vou_GetChnAbsAddr(enChan,(HI_U32)&(pVoReg->DHD0_PWR.u32));
        DHD0_PWR.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        DHD0_PWR.bits.hpw = pstSyncInfo->u16Hpw - 1;
        DHD0_PWR.bits.vpw = pstSyncInfo->u16Vpw - 1;
        HAL_WriteReg((HI_U32*)addr_REG, DHD0_PWR.u32); 
    }
    else
    {
         HAL_PRINT("Error channel id found in %s: L%d\n",__FUNCTION__, __LINE__);
         return HI_FALSE;
    }

    return HI_TRUE;
}

/* 设置输出接口的外同步极性*/
HI_BOOL HAL_DISP_SetIntfSyncInv(HAL_DISP_INTF_E enIntf, HAL_DISP_SYNCINV_S *pstInv)
{
    volatile U_INTF_SYNC_INV INTF_SYNC_INV;    
    volatile HI_U32 addr_REG;
    addr_REG = Vou_GetIntfAbsAddr(enIntf,(HI_U32)&(pVoReg->INTF_SYNC_INV.u32));
    INTF_SYNC_INV.u32 = HAL_ReadReg((HI_U32*)addr_REG);
    INTF_SYNC_INV.bits.f_inv = pstInv->u32f_inv;
    INTF_SYNC_INV.bits.vs_inv = pstInv->u32vs_inv;     
    INTF_SYNC_INV.bits.hs_inv = pstInv->u32hs_inv;
    INTF_SYNC_INV.bits.dv_inv = pstInv->u32dv_inv; 
    HAL_WriteReg((HI_U32*)addr_REG, INTF_SYNC_INV.u32); 
    return HI_TRUE;
}
HI_BOOL  HAL_DISP_GetDispIoP(HAL_DISP_OUTPUTCHANNEL_E enChan,HI_U32 *pu32iop)
{

    U_DHD0_CTRL DHD0_CTRL;
    
    volatile  HI_U32 addr_REG;
    if((CHANNEL_DHD_START <= enChan) && (enChan <= CHANNEL_DSD_END))
    {
        addr_REG = Vou_GetChnAbsAddr(enChan,(HI_U32)&(pVoReg->DHD0_CTRL.u32));
        DHD0_CTRL.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        *pu32iop = DHD0_CTRL.bits.iop;
    }
    else
    {
        HAL_PRINT("Error channel id found in %s: L%d\n",__FUNCTION__, __LINE__);
        return HI_FALSE;
    }
    return HI_TRUE;
}

/* 设置输出接口的mux数据选择*/
HI_BOOL  HAL_DISP_SetIntfMuxSel(HAL_DISP_OUTPUTCHANNEL_E enChan,HAL_DISP_INTF_E enIntf)
{
    volatile U_VO_MUX VO_MUX;
    if(enChan > HAL_DISP_CHANNEL_DSD0)
     {
        HAL_PRINT("Error channel id found in %s: L%d\n",__FUNCTION__, __LINE__);
        return HI_FALSE;
    }

    VO_MUX.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->VO_MUX.u32));    
    switch(enIntf)
    {
        case HAL_DISP_INTF_LCD:
        {
            VO_MUX.bits.digital_sel = 2;
            VO_MUX.bits.lcd_sel = enChan; 
            break;
        }
        case HAL_DISP_INTF_BT1120:
        {
            VO_MUX.bits.digital_sel = 0;
            VO_MUX.bits.bt1120_sel = enChan; 
            break;
        }        
        case HAL_DISP_INTF_BT656:
        {
            VO_MUX.bits.digital_sel = 1;
            VO_MUX.bits.bt656_sel = enChan; 
            break;
        }
        case HAL_DISP_INTF_HDMI:
        {
            VO_MUX.bits.hdmi_sel = enChan;
            break;
        }
        case HAL_DISP_INTF_VGA:
        {
            VO_MUX.bits.vga_sel = enChan;
            break;
        }
        case HAL_DISP_INTF_HDDATE:
        {
            VO_MUX.bits.hddate_sel = enChan;
            break;
        }

        case HAL_DISP_INTF_CVBS:
        {
            VO_MUX.bits.sddate_sel = 4;
            break;
        }

        default:
        {
            HAL_PRINT("Error! DP_DISP_SetIntfMuxSel Wrong Select\n");
            return HI_FALSE;
        }
    }
    
    HAL_WriteReg((HI_U32*)&(pVoReg->VO_MUX.u32), VO_MUX.u32); 
    return HI_TRUE;
}

/* 设置输出接口的dac使能*/
HI_BOOL HAL_DISP_SetIntfDacEnable(HAL_DISP_INTF_E enIntf, HI_BOOL bDacEnable)
{
    volatile U_VO_DAC_CTRL VO_DAC_CTRL;    
    volatile U_VO_DAC_C_CTRL VO_DAC_C_CTRL;    
    volatile U_VO_DAC_R_CTRL VO_DAC_R_CTRL;    
    volatile U_VO_DAC_G_CTRL VO_DAC_G_CTRL; 
    volatile U_VO_DAC_B_CTRL VO_DAC_B_CTRL;
    
    switch(enIntf)
    {
        case(HAL_DISP_INTF_VGA):
            VO_DAC_CTRL.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->VO_DAC_CTRL.u32));
            VO_DAC_CTRL.bits.en_bg = bDacEnable;
            HAL_WriteReg((HI_U32*)&(pVoReg->VO_DAC_CTRL.u32), VO_DAC_CTRL.u32);
            
            VO_DAC_R_CTRL.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->VO_DAC_R_CTRL.u32));
            VO_DAC_R_CTRL.bits.en_r = bDacEnable;
            HAL_WriteReg((HI_U32*)&(pVoReg->VO_DAC_R_CTRL.u32), VO_DAC_R_CTRL.u32);
            
            VO_DAC_G_CTRL.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->VO_DAC_G_CTRL.u32));
            VO_DAC_G_CTRL.bits.en_g = bDacEnable;
            HAL_WriteReg((HI_U32*)&(pVoReg->VO_DAC_G_CTRL.u32), VO_DAC_G_CTRL.u32);
            
            VO_DAC_B_CTRL.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->VO_DAC_B_CTRL.u32));
            VO_DAC_B_CTRL.bits.en_b = bDacEnable;
            HAL_WriteReg((HI_U32*)&(pVoReg->VO_DAC_B_CTRL.u32), VO_DAC_B_CTRL.u32);

            break;
        case(HAL_DISP_INTF_CVBS):
            VO_DAC_CTRL.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->VO_DAC_CTRL.u32));
            VO_DAC_CTRL.bits.en_bg = bDacEnable;
            HAL_WriteReg((HI_U32*)&(pVoReg->VO_DAC_CTRL.u32), VO_DAC_CTRL.u32);
            
            VO_DAC_C_CTRL.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->VO_DAC_C_CTRL.u32));
            VO_DAC_C_CTRL.bits.en_c = bDacEnable;
            HAL_WriteReg((HI_U32*)&(pVoReg->VO_DAC_C_CTRL.u32), VO_DAC_C_CTRL.u32);

            break;
        default:
            break;
    }
    return HI_TRUE;
}

//for hi3531
HI_BOOL HAL_DISP_VgaDacEn(HI_BOOL bEn)
{
    volatile U_VO_MUX_DAC VO_MUX_DAC;
    if(bEn)
    {
        VO_MUX_DAC.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->VO_MUX_DAC.u32));
        VO_MUX_DAC.bits.dac2_sel = 9;          
        VO_MUX_DAC.bits.dac1_sel = 8;              
        VO_MUX_DAC.bits.dac0_sel = 7;        
        HAL_WriteReg((HI_U32*)&(pVoReg->VO_MUX_DAC.u32), VO_MUX_DAC.u32);
    }    
    return HI_TRUE;
}

HI_BOOL HAL_DISP_CvbsDacEn(HAL_DISP_OUTPUTCHANNEL_E enChan, HI_BOOL bEn)
{
    volatile U_VO_MUX_DAC VO_MUX_DAC;

    VO_MUX_DAC.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->VO_MUX_DAC.u32));
    if (HAL_DISP_CHANNEL_DSD0 == enChan)
    {        
        VO_MUX_DAC.bits.dac3_sel = 0;
    }
    else
    {
        HAL_PRINT("Error channel id found in %s: L%d\n",__FUNCTION__, __LINE__);
        return HI_FALSE;
    }
    
    HAL_WriteReg((HI_U32*)&(pVoReg->VO_MUX_DAC.u32), VO_MUX_DAC.u32);
    return HI_TRUE;
}

/* 设置dac电流增益 */
HI_BOOL HAL_DISP_SetVgaGc(HI_U32 u32Value)
{
    volatile U_VO_DAC_R_CTRL VO_DAC_R_CTRL;    
    volatile U_VO_DAC_G_CTRL VO_DAC_G_CTRL;
    volatile U_VO_DAC_B_CTRL VO_DAC_B_CTRL;

    VO_DAC_R_CTRL.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->VO_DAC_R_CTRL.u32));
    VO_DAC_R_CTRL.bits.ct_r = u32Value;
    HAL_WriteReg((HI_U32*)&(pVoReg->VO_DAC_R_CTRL.u32), VO_DAC_R_CTRL.u32);
    
    VO_DAC_G_CTRL.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->VO_DAC_G_CTRL.u32));
    VO_DAC_G_CTRL.bits.ct_g = u32Value;
    HAL_WriteReg((HI_U32*)&(pVoReg->VO_DAC_G_CTRL.u32), VO_DAC_G_CTRL.u32);

    VO_DAC_B_CTRL.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->VO_DAC_B_CTRL.u32));
    VO_DAC_B_CTRL.bits.ct_b = u32Value;
    HAL_WriteReg((HI_U32*)&(pVoReg->VO_DAC_B_CTRL.u32), VO_DAC_B_CTRL.u32);
    
    return HI_TRUE;
}

HI_BOOL HAL_DISP_SetCvbsGc(HAL_DISP_OUTPUTCHANNEL_E enChan, HI_U32 u32Value)
{
    volatile U_VO_DAC_C_CTRL VO_DAC_C_CTRL;

    VO_DAC_C_CTRL.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->VO_DAC_C_CTRL.u32));
    if (HAL_DISP_CHANNEL_DSD0 == enChan)
    {        
        VO_DAC_C_CTRL.bits.ct_c = u32Value;
    }    
    else
    {
        HAL_PRINT("Error channel id found in %s: L%d\n",__FUNCTION__, __LINE__);
        return HI_FALSE;
    }
    
    HAL_WriteReg((HI_U32*)&(pVoReg->VO_DAC_C_CTRL.u32), VO_DAC_C_CTRL.u32);
    return HI_TRUE;
}


/* 设置输出接口的CSC*/
HI_BOOL HAL_DISP_SetIntfCSCEn(HAL_DISP_INTF_E enIntf,HI_BOOL bCscEn)
{   
    U_INTF_CSC_IDC INTF_CSC_IDC;
    volatile  HI_U32 addr_REG;
    addr_REG = Vou_GetIntfAbsAddr(enIntf,(HI_U32)&(pVoReg->INTF_CSC_IDC.u32));
    INTF_CSC_IDC.u32 = HAL_ReadReg((HI_U32*)addr_REG);
    INTF_CSC_IDC.bits.csc_en = bCscEn;
    HAL_WriteReg((HI_U32*)addr_REG, INTF_CSC_IDC.u32);     
    return HI_TRUE;
}

HI_BOOL HAL_DISP_SetIntfCscCoef(HAL_DISP_INTF_E enIntf,CscCoef_S *pstCoef)
{    
    U_INTF_CSC_IDC INTF_CSC_IDC;
    U_INTF_CSC_ODC INTF_CSC_ODC;
    U_INTF_CSC_IODC INTF_CSC_IODC;
    U_INTF_CSC_P0  INTF_CSC_P0;
    U_INTF_CSC_P1  INTF_CSC_P1;
    U_INTF_CSC_P2  INTF_CSC_P2;
    U_INTF_CSC_P3  INTF_CSC_P3;
    U_INTF_CSC_P4  INTF_CSC_P4;    
    volatile  HI_U32 addr_REG;
    addr_REG = Vou_GetIntfAbsAddr(enIntf,(HI_U32)&(pVoReg->INTF_CSC_IDC.u32));    
    INTF_CSC_IDC.u32 = HAL_ReadReg((HI_U32*)addr_REG);
    INTF_CSC_IDC.bits.cscidc1 = HAL_GetXDC_BUMA(pstCoef->csc_in_dc1);
//    DHD0_CSC_IDC.bits.cscidc0 = HAL_GetXDC_BUMA(pstCoef->csc_in_dc0);
    INTF_CSC_IDC.bits.cscidc0 = HAL_GetXDC_BUMA(pstCoef->csc_in_dc2);
    HAL_WriteReg((HI_U32*)addr_REG, INTF_CSC_IDC.u32); 

    addr_REG = Vou_GetIntfAbsAddr(enIntf,(HI_U32)&(pVoReg->INTF_CSC_ODC.u32));    
    INTF_CSC_ODC.bits.cscodc1 = HAL_GetXDC_BUMA(pstCoef->csc_out_dc1);
    //DHD0_CSC_ODC.bits.cscodc0 = HAL_GetXDC_BUMA(pstCoef->csc_out_dc0);    
    INTF_CSC_ODC.bits.cscodc0 = HAL_GetXDC_BUMA(pstCoef->csc_out_dc2);
    HAL_WriteReg((HI_U32*)addr_REG, INTF_CSC_ODC.u32); 
    
    addr_REG = Vou_GetIntfAbsAddr(enIntf,(HI_U32)&(pVoReg->INTF_CSC_IODC.u32));
//    DHD0_CSC_IODC.bits.cscidc2 = HAL_GetXDC_BUMA(pstCoef->csc_in_dc2);
//    DHD0_CSC_IODC.bits.cscodc2 = HAL_GetXDC_BUMA(pstCoef->csc_out_dc2);
    INTF_CSC_IODC.bits.cscidc2 = HAL_GetXDC_BUMA(pstCoef->csc_in_dc0);
    INTF_CSC_IODC.bits.cscodc2 = HAL_GetXDC_BUMA(pstCoef->csc_out_dc0);
    HAL_WriteReg((HI_U32*)addr_REG, INTF_CSC_IODC.u32); 

    addr_REG = Vou_GetIntfAbsAddr(enIntf,(HI_U32)&(pVoReg->INTF_CSC_P0.u32));
    INTF_CSC_P0.bits.cscp00 = HAL_Conver_CscCoef(pstCoef->csc_coef00);
    INTF_CSC_P0.bits.cscp01 = HAL_Conver_CscCoef(pstCoef->csc_coef01);
    HAL_WriteReg((HI_U32*)addr_REG, INTF_CSC_P0.u32); 

    addr_REG = Vou_GetIntfAbsAddr(enIntf,(HI_U32)&(pVoReg->INTF_CSC_P1.u32));
    INTF_CSC_P1.bits.cscp02 = HAL_Conver_CscCoef(pstCoef->csc_coef02);
    INTF_CSC_P1.bits.cscp10 = HAL_Conver_CscCoef(pstCoef->csc_coef10);
    HAL_WriteReg((HI_U32*)addr_REG, INTF_CSC_P1.u32); 

    addr_REG = Vou_GetIntfAbsAddr(enIntf,(HI_U32)&(pVoReg->INTF_CSC_P2.u32));
    INTF_CSC_P2.bits.cscp11 = HAL_Conver_CscCoef(pstCoef->csc_coef11);
    INTF_CSC_P2.bits.cscp12 = HAL_Conver_CscCoef(pstCoef->csc_coef12);
    HAL_WriteReg((HI_U32*)addr_REG, INTF_CSC_P2.u32); 

    addr_REG = Vou_GetIntfAbsAddr(enIntf,(HI_U32)&(pVoReg->INTF_CSC_P3.u32));
    INTF_CSC_P3.bits.cscp20 = HAL_Conver_CscCoef(pstCoef->csc_coef20);
    INTF_CSC_P3.bits.cscp21 = HAL_Conver_CscCoef(pstCoef->csc_coef21);
    HAL_WriteReg((HI_U32*)addr_REG, INTF_CSC_P3.u32); 

    addr_REG = Vou_GetIntfAbsAddr(enIntf,(HI_U32)&(pVoReg->INTF_CSC_P4.u32));
    INTF_CSC_P4.bits.cscp22 = HAL_Conver_CscCoef(pstCoef->csc_coef22);
    HAL_WriteReg((HI_U32*)addr_REG, INTF_CSC_P4.u32); 

    return HI_TRUE;
}

/*对VGA Sharp参数进行设置*/
/*1.hsp_hf0*/
HI_BOOL HAL_DISP_SetIntfHspH0Coef(HAL_DISP_INTF_E enIntf,HspHfCoef_S *pstVgaHspCoef)
{
    U_INTF_HSPCFG0 INTF_HSPCFG0;
    U_INTF_HSPCFG1 INTF_HSPCFG1;
    U_INTF_HSPCFG5 INTF_HSPCFG5;
    U_INTF_HSPCFG6 INTF_HSPCFG6;
    volatile  HI_U32 addr_REG;
    
    addr_REG = Vou_GetIntfAbsAddr(enIntf,(HI_U32)&(pVoReg->INTF_HSPCFG0.u32));    
    INTF_HSPCFG0.u32 = HAL_ReadReg((HI_U32*)addr_REG);
    INTF_HSPCFG0.bits.hsp_hf0_tmp0 = pstVgaHspCoef->vga_hsp_tmp0;
    INTF_HSPCFG0.bits.hsp_hf0_tmp1 = pstVgaHspCoef->vga_hsp_tmp1;
    INTF_HSPCFG0.bits.hsp_hf0_tmp2 = pstVgaHspCoef->vga_hsp_tmp2;
    INTF_HSPCFG0.bits.hsp_hf0_tmp3 = pstVgaHspCoef->vga_hsp_tmp3;
    HAL_WriteReg((HI_U32*)addr_REG, INTF_HSPCFG0.u32); 

    addr_REG = Vou_GetIntfAbsAddr(enIntf,(HI_U32)&(pVoReg->INTF_HSPCFG1.u32));    
    INTF_HSPCFG1.u32 = HAL_ReadReg((HI_U32*)addr_REG);
    INTF_HSPCFG1.bits.hsp_hf0_coring = pstVgaHspCoef->vga_hsp_coring;
    HAL_WriteReg((HI_U32*)addr_REG, INTF_HSPCFG1.u32); 

    addr_REG = Vou_GetIntfAbsAddr(enIntf,(HI_U32)&(pVoReg->INTF_HSPCFG5.u32));
    INTF_HSPCFG5.u32 = HAL_ReadReg((HI_U32*)addr_REG);
    INTF_HSPCFG5.bits.hsp_hf0_gainneg = pstVgaHspCoef->vga_hsp_gainneg;
    INTF_HSPCFG5.bits.hsp_hf0_gainpos = pstVgaHspCoef->vga_hsp_gainpos;
    HAL_WriteReg((HI_U32*)addr_REG, INTF_HSPCFG5.u32); 

    addr_REG = Vou_GetIntfAbsAddr(enIntf,(HI_U32)&(pVoReg->INTF_HSPCFG6.u32));    
    INTF_HSPCFG6.u32 = HAL_ReadReg((HI_U32*)addr_REG);
    INTF_HSPCFG6.bits.hsp_hf0_adpshoot_en = pstVgaHspCoef ->vga_hsp_adpshooten;
    INTF_HSPCFG6.bits.hsp_hf0_winsize = pstVgaHspCoef->vga_hsp_winsize;
    INTF_HSPCFG6.bits.hsp_hf0_mixratio = pstVgaHspCoef->vga_hsp_mixratio;
    INTF_HSPCFG6.bits.hsp_hf0_underth = pstVgaHspCoef->vga_hsp_underth;
    INTF_HSPCFG6.bits.hsp_hf0_overth = pstVgaHspCoef->vga_hsp_overth;
    HAL_WriteReg((HI_U32*)addr_REG, INTF_HSPCFG6.u32); 
    return HI_TRUE;
}

/*2.hsp_hf1*/
HI_BOOL HAL_DISP_SetIntfHspH1Coef(HAL_DISP_INTF_E enIntf,HspHfCoef_S *pstVgaHspCoef)
{   
    U_INTF_HSPCFG7 INTF_HSPCFG7;
    U_INTF_HSPCFG8 INTF_HSPCFG8;
    U_INTF_HSPCFG12 INTF_HSPCFG12;
    U_INTF_HSPCFG13 INTF_HSPCFG13;
    volatile  HI_U32 addr_REG;
    addr_REG = Vou_GetIntfAbsAddr(enIntf,(HI_U32)&(pVoReg->INTF_HSPCFG7.u32));    
    INTF_HSPCFG7.u32 = HAL_ReadReg((HI_U32*)addr_REG);
    INTF_HSPCFG7.bits.hsp_hf1_tmp0 = pstVgaHspCoef->vga_hsp_tmp0;
    INTF_HSPCFG7.bits.hsp_hf1_tmp1 = pstVgaHspCoef->vga_hsp_tmp1;
    INTF_HSPCFG7.bits.hsp_hf1_tmp2 = pstVgaHspCoef->vga_hsp_tmp2;
    INTF_HSPCFG7.bits.hsp_hf1_tmp3 = pstVgaHspCoef->vga_hsp_tmp3;
    HAL_WriteReg((HI_U32*)addr_REG, INTF_HSPCFG7.u32); 

    addr_REG = Vou_GetIntfAbsAddr(enIntf,(HI_U32)&(pVoReg->INTF_HSPCFG8.u32));    
    INTF_HSPCFG8.u32 = HAL_ReadReg((HI_U32*)addr_REG);
    INTF_HSPCFG8.bits.hsp_hf1_coring = pstVgaHspCoef->vga_hsp_coring;
    HAL_WriteReg((HI_U32*)addr_REG, INTF_HSPCFG8.u32); 

    addr_REG = Vou_GetIntfAbsAddr(enIntf,(HI_U32)&(pVoReg->INTF_HSPCFG12.u32));    
    INTF_HSPCFG12.u32 = HAL_ReadReg((HI_U32*)addr_REG);
    INTF_HSPCFG12.bits.hsp_hf1_gainneg = pstVgaHspCoef->vga_hsp_gainneg;
    INTF_HSPCFG12.bits.hsp_hf1_gainpos = pstVgaHspCoef->vga_hsp_gainpos;
    HAL_WriteReg((HI_U32*)addr_REG, INTF_HSPCFG12.u32); 

    addr_REG = Vou_GetIntfAbsAddr(enIntf,(HI_U32)&(pVoReg->INTF_HSPCFG13.u32));    
    INTF_HSPCFG13.u32 = HAL_ReadReg((HI_U32*)addr_REG);
    INTF_HSPCFG13.bits.hsp_hf1_adpshoot_en = pstVgaHspCoef ->vga_hsp_adpshooten;
    INTF_HSPCFG13.bits.hsp_hf1_winsize = pstVgaHspCoef->vga_hsp_winsize;
    INTF_HSPCFG13.bits.hsp_hf1_mixratio = pstVgaHspCoef->vga_hsp_mixratio;
    INTF_HSPCFG13.bits.hsp_hf1_underth = pstVgaHspCoef->vga_hsp_underth;
    INTF_HSPCFG13.bits.hsp_hf1_overth = pstVgaHspCoef->vga_hsp_overth;
    HAL_WriteReg((HI_U32*)addr_REG, INTF_HSPCFG13.u32); 
    return HI_TRUE;
}

/* 3.hsp_en水平锐化使能*/
HI_BOOL HAL_DISP_SetIntfHspEn(HAL_DISP_INTF_E enIntf,HI_BOOL bHspEn)
{
    U_INTF_HSPCFG1 INTF_HSPCFG1;    
    volatile  HI_U32 addr_REG;
    addr_REG = Vou_GetIntfAbsAddr(enIntf,(HI_U32)&(pVoReg->INTF_HSPCFG1.u32));    
    INTF_HSPCFG1.u32 = HAL_ReadReg((HI_U32*)addr_REG);
    INTF_HSPCFG1.bits.hsp_en = bHspEn;
    HAL_WriteReg((HI_U32*)addr_REG, INTF_HSPCFG1.u32); 
    return HI_TRUE;
}

/* 4.Vga 水平LTI使能*/
HI_BOOL HAL_DISP_SetIntfHspLTIEn(HAL_DISP_INTF_E enIntf,HI_BOOL bHspLTIEn)
{
    U_INTF_HSPCFG14 INTF_HSPCFG14;    
    volatile  HI_U32 addr_REG;
    addr_REG = Vou_GetIntfAbsAddr(enIntf,(HI_U32)&(pVoReg->INTF_HSPCFG14.u32));    
    INTF_HSPCFG14.u32 = HAL_ReadReg((HI_U32*)addr_REG);
    INTF_HSPCFG14.bits.hsp_ltih_en = bHspLTIEn;
    HAL_WriteReg((HI_U32*)addr_REG, INTF_HSPCFG14.u32); 
    return HI_TRUE;
}

/* 5.Vga 水平CTI使能*/
HI_BOOL HAL_DISP_SetIntfHspCTIEn(HAL_DISP_INTF_E enIntf,HI_BOOL bHspCTIEn)
{
    U_INTF_HSPCFG14 INTF_HSPCFG14;    
    volatile  HI_U32 addr_REG;
    addr_REG = Vou_GetIntfAbsAddr(enIntf,(HI_U32)&(pVoReg->INTF_HSPCFG14.u32));    
    INTF_HSPCFG14.u32 = HAL_ReadReg((HI_U32*)addr_REG);
    INTF_HSPCFG14.bits.hsp_ctih_en = bHspCTIEn;
    HAL_WriteReg((HI_U32*)addr_REG, INTF_HSPCFG14.u32); 
    return HI_TRUE;
}

/* 6.hsp_h0_en高频滤波0使能*/
HI_BOOL HAL_DISP_SetIntfHspH0En(HAL_DISP_INTF_E enIntf,HI_BOOL bHspH0En)
{
    U_INTF_HSPCFG14 INTF_HSPCFG14;    
    volatile  HI_U32 addr_REG;
    addr_REG = Vou_GetIntfAbsAddr(enIntf,(HI_U32)&(pVoReg->INTF_HSPCFG14.u32));    
    INTF_HSPCFG14.u32 = HAL_ReadReg((HI_U32*)addr_REG);
    INTF_HSPCFG14.bits.hsp_h0_en = bHspH0En;
    HAL_WriteReg((HI_U32*)addr_REG, INTF_HSPCFG14.u32); 
    return HI_TRUE;
}

/* 7.hsp_h1_en高频滤波1使能*/
HI_BOOL HAL_DISP_SetIntfHspH1En(HAL_DISP_INTF_E enIntf,HI_BOOL bHspH1En)
{
    U_INTF_HSPCFG14 INTF_HSPCFG14;    
    volatile  HI_U32 addr_REG;
    addr_REG = Vou_GetIntfAbsAddr(enIntf,(HI_U32)&(pVoReg->INTF_HSPCFG14.u32));    
    INTF_HSPCFG14.u32 = HAL_ReadReg((HI_U32*)addr_REG);
    INTF_HSPCFG14.bits.hsp_h1_en = bHspH1En;
    HAL_WriteReg((HI_U32*)addr_REG, INTF_HSPCFG14.u32); 
    return HI_TRUE;
}

/* 8.Vga亮度色度增益参数*/
HI_BOOL HAL_DISP_SetIntfHspCoef(HAL_DISP_INTF_E enIntf,HspCoef_S *pstVgaHspCoef)
{
    U_INTF_HSPCFG14 INTF_HSPCFG14;     
    U_INTF_HSPCFG15 INTF_HSPCFG15;    
    volatile  HI_U32 addr_REG;    
    addr_REG = Vou_GetIntfAbsAddr(enIntf,(HI_U32)&(pVoReg->INTF_HSPCFG14.u32));    
    INTF_HSPCFG14.u32 = HAL_ReadReg((HI_U32*)addr_REG);
    INTF_HSPCFG14.bits.hsp_hf_shootdiv = pstVgaHspCoef->vga_hsp_hf_shootdiv;
    INTF_HSPCFG14.bits.hsp_lti_ratio = pstVgaHspCoef->vga_hsp_lti_ratio;
    INTF_HSPCFG14.bits.hsp_ldti_gain = pstVgaHspCoef->vga_hsp_ldti_gain;
    INTF_HSPCFG14.bits.hsp_cdti_gain = pstVgaHspCoef->vga_hsp_cdti_gain;
    HAL_WriteReg((HI_U32*)addr_REG, INTF_HSPCFG14.u32); 

    addr_REG = Vou_GetIntfAbsAddr(enIntf,(HI_U32)&(pVoReg->INTF_HSPCFG15.u32));    
    INTF_HSPCFG15.u32 = HAL_ReadReg((HI_U32*)addr_REG);
    INTF_HSPCFG15.bits.hsp_peak_ratio = pstVgaHspCoef->vga_hsp_peak_ratio;
    INTF_HSPCFG15.bits.hsp_glb_overth = pstVgaHspCoef->vga_hsp_glb_overth;
    INTF_HSPCFG15.bits.hsp_glb_underth = pstVgaHspCoef->vga_hsp_glb_unferth;
    HAL_WriteReg((HI_U32*)addr_REG, INTF_HSPCFG15.u32); 
    return HI_TRUE;
}


/*3535只要设置hdmi的接口，且hdmi只能绑定在INTF上面， CVBS的钳位固定在标清通道上，且3535没有钳位只能开关*/
HI_BOOL  HAL_DISP_SetIntfClip(HAL_DISP_INTF_E enIntf,
                                        HI_BOOL enClip,
                                        HAL_DISP_CLIP_S *pstClipData)
{   
    volatile U_INTF_CLIP0_L    INTF_CLIP0_L;
    volatile U_INTF_CLIP0_H    INTF_CLIP0_H;    

    volatile  HI_U32 addr_REG;
    addr_REG = Vou_GetIntfAbsAddr(enIntf,(HI_U32)&(pVoReg->INTF_CLIP0_L.u32));
    INTF_CLIP0_L.u32 = HAL_ReadReg((HI_U32*)addr_REG);
    INTF_CLIP0_L.bits.clip_cl2 = pstClipData->u16ClipLow_y;
    INTF_CLIP0_L.bits.clip_cl1 = pstClipData->u16ClipLow_cb;
    INTF_CLIP0_L.bits.clip_cl0 = pstClipData->u16ClipLow_cr;        
    HAL_WriteReg((HI_U32*)addr_REG, INTF_CLIP0_L.u32); 

    addr_REG = Vou_GetIntfAbsAddr(enIntf,(HI_U32)&(pVoReg->INTF_CLIP0_H.u32));
    INTF_CLIP0_H.u32 = HAL_ReadReg((HI_U32*)addr_REG);
    INTF_CLIP0_H.bits.clip_ch2 = pstClipData->u16ClipHigh_y;
    INTF_CLIP0_H.bits.clip_ch1 = pstClipData->u16ClipHigh_cb;
    INTF_CLIP0_H.bits.clip_ch0 = pstClipData->u16ClipHigh_cr;        
    HAL_WriteReg((HI_U32*)addr_REG, INTF_CLIP0_H.u32); 
    
    return HI_TRUE;
}

//设置显示通道中断产生模式
HI_BOOL HAL_DISP_SetVtThdMode(HAL_DISP_OUTPUTCHANNEL_E enChan, HI_U32 uFieldMode)
{
    volatile U_DHD0_VTTHD  DHD0_VTTHD;
    volatile  HI_U32 addr_REG;
    if((CHANNEL_DHD_START <= enChan) && (enChan <= CHANNEL_DSD_END))
    {
        addr_REG = Vou_GetChnAbsAddr(enChan,(HI_U32)&(pVoReg->DHD0_VTTHD.u32));
        DHD0_VTTHD.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        DHD0_VTTHD.bits.thd1_mode = uFieldMode;
        HAL_WriteReg((HI_U32*)addr_REG, DHD0_VTTHD.u32); 
    }
    else
    {
        HAL_PRINT("Error channel id found in %s: L%d\n",__FUNCTION__, __LINE__);
        return HI_FALSE;
    }
    return HI_TRUE;
}


//获取显示通道中断产生模式
HI_BOOL HAL_DISP_GetVtThdMode(HAL_DISP_OUTPUTCHANNEL_E enChan, HI_U32 *pu32FieldMode)
{
    volatile U_DHD0_VTTHD  DHD0_VTTHD;
    volatile  HI_U32 addr_REG;
    if((CHANNEL_DHD_START <= enChan) && (enChan <= CHANNEL_DSD_END))
    {
        addr_REG = Vou_GetChnAbsAddr(enChan,(HI_U32)&(pVoReg->DHD0_VTTHD.u32));
        DHD0_VTTHD.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        *pu32FieldMode = DHD0_VTTHD.bits.thd1_mode;
    }
    else
    {
        HAL_PRINT("Error channel id found in %s: L%d\n",__FUNCTION__, __LINE__);
        return HI_FALSE;
    }
    return HI_TRUE;
}


//设置显示通道中断产生门限
HI_BOOL HAL_DISP_SetVtThd(HAL_DISP_OUTPUTCHANNEL_E enChan, HI_U32 vtthd)
{    
    volatile U_DHD0_VTTHD  DHD0_VTTHD;
    volatile  HI_U32 addr_REG;
    if((CHANNEL_DHD_START <= enChan) && (enChan <= CHANNEL_DSD_END))
    {
        addr_REG = Vou_GetChnAbsAddr(enChan,(HI_U32)&(pVoReg->DHD0_VTTHD.u32));
        DHD0_VTTHD.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        DHD0_VTTHD.bits.vtmgthd1 = vtthd;
        HAL_WriteReg((HI_U32*)addr_REG, DHD0_VTTHD.u32); 
    }
    else
    {
        HAL_PRINT("Error channel id found in %s: L%d\n",__FUNCTION__, __LINE__);
        return HI_FALSE; 
    }
    return HI_TRUE;
}


HI_BOOL HAL_DISP_SetGammaEnable(HAL_DISP_OUTPUTCHANNEL_E enChn, HI_U32 uGmmEn)
{
    volatile U_DHD0_CTRL  DHD0_CTRL;
    volatile  HI_U32 addr_REG;
    if((CHANNEL_DHD_START <= enChn) && (enChn <= CHANNEL_DHD_END))
    {
        addr_REG = Vou_GetChnAbsAddr(enChn,(HI_U32)&(pVoReg->DHD0_CTRL.u32));
        DHD0_CTRL.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        DHD0_CTRL.bits.gmm_en = uGmmEn;
        HAL_WriteReg((HI_U32*)addr_REG, DHD0_CTRL.u32); 
    }   
    else
    {
        HAL_PRINT("Error channel id found in %s: L%d\n",__FUNCTION__, __LINE__);
        return HI_FALSE; 
    }
    return HI_TRUE;

    return HI_TRUE;
}
HI_BOOL HAL_DISP_SetGammaAddr(HAL_DISP_OUTPUTCHANNEL_E enChn, HI_U32 uGmmAddr)
{
    volatile U_DHD0_GMM_COEFAD DHD0_GMM_COEFAD;    
    volatile  HI_U32 addr_REG;
    if((CHANNEL_DHD_START <= enChn) && (enChn <= CHANNEL_DHD_END))
    {       
        addr_REG = Vou_GetChnAbsAddr(enChn,(HI_U32)&(pVoReg->DHD0_GMM_COEFAD.u32));
        DHD0_GMM_COEFAD.u32 = HAL_ReadReg((HI_U32*)addr_REG);       
        DHD0_GMM_COEFAD.u32 = uGmmAddr;
        HAL_WriteReg((HI_U32*)addr_REG, DHD0_GMM_COEFAD.u32); 
    }
    else
    {
        HAL_PRINT("Error channel id found in %s: L%d\n",__FUNCTION__, __LINE__);
        return HI_FALSE;
    }

    return HI_TRUE;
}


/* Vou 设置接口的参数更新 update */
HI_BOOL HAL_DISP_SetDispParaUpd(HAL_DISP_OUTPUTCHANNEL_E enChan,
                                  HAL_DISP_COEFMODE_E enMode)
{
    volatile U_DHD0_PARAUP DHD0_PARAUP;
    volatile  HI_U32 addr_REG; 

    if((enMode == HAL_DISP_COEFMODE_GAM) && ((CHANNEL_DHD_START <= enChan) && (enChan <= CHANNEL_DHD_END)))
    {
         addr_REG = Vou_GetChnAbsAddr(enChan,(HI_U32)&(pVoReg->DHD0_PARAUP.u32));
         DHD0_PARAUP.u32 = HAL_ReadReg((HI_U32*)addr_REG);       
         DHD0_PARAUP.bits.dhd0_gmm_upd = 1;
         HAL_WriteReg((HI_U32*)addr_REG, DHD0_PARAUP.u32); 
    }
    else
    {
        HAL_PRINT("Error channel id found in %s: L%d\n",__FUNCTION__, __LINE__);
        return HI_FALSE;
    }
    return HI_TRUE;
}

//打开、关闭中断
HI_BOOL HAL_DISP_SetIntMask(HI_U32 u32MaskEn)
{
    volatile U_VOINTMSK VOINTMSK;

    /* Dispaly interrupt mask enable */
    VOINTMSK.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->VOINTMSK.u32));
    VOINTMSK.u32 = VOINTMSK.u32 | u32MaskEn;
    HAL_WriteReg((HI_U32*)&(pVoReg->VOINTMSK.u32), VOINTMSK.u32); 

    return HI_TRUE;
}

HI_BOOL  HAL_DISP_ClrIntMask(HI_U32 u32MaskEn)
{
    volatile U_VOINTMSK VOINTMSK;

    /* Dispaly interrupt mask enable */
    VOINTMSK.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->VOINTMSK.u32));
    VOINTMSK.u32 = VOINTMSK.u32 & (~u32MaskEn);
    HAL_WriteReg((HI_U32*)&(pVoReg->VOINTMSK.u32), VOINTMSK.u32);

    return HI_TRUE;
}

//获取中断状态
HI_U32 HAL_DISP_GetIntStatus(HI_U32 u32IntMsk)
{
    volatile U_VOMSKINTSTA VOMSKINTSTA;
    
    /* read interrupt status */
    VOMSKINTSTA.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->VOMSKINTSTA.u32));
    
    return (VOMSKINTSTA.u32 & u32IntMsk);
}

//清除中断状态
HI_BOOL HAL_DISP_ClearIntStatus(HI_U32 u32IntMsk)
{
    /* read interrupt status */
    HAL_WriteReg((HI_U32*)&(pVoReg->VOMSKINTSTA.u32), u32IntMsk);
    return HI_TRUE;
}

//获取中断状态
HI_U32 HAL_DISP_GetRawIntStatus(HI_U32 u32IntMsk)
{
    volatile U_VOINTSTA VOINTSTA;

    /* read interrupt status */
    VOINTSTA.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->VOINTSTA.u32));

    return (VOINTSTA.u32 & u32IntMsk);
}

HI_BOOL HAL_DISP_SetClkGateEnable(HI_U32 u32Data)
{
    volatile U_VOCTRL VOCTRL;
    
    VOCTRL.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->VOCTRL.u32));
    VOCTRL.bits.vo_ck_gt_en = u32Data;
    HAL_WriteReg((HI_U32*)&(pVoReg->VOCTRL.u32), VOCTRL.u32);
    return HI_TRUE;
}

HI_VOID HAL_DISP_DATE_OutCtrl(HAL_DISP_OUTPUTCHANNEL_E enChan, HI_U32 u32OutCtrl)
{
    if (HAL_DISP_CHANNEL_DSD0 == enChan)
    {
        U_DATE_COEFF21 DateOutCtrl, tmp;

        tmp.u32 = u32OutCtrl;
        DateOutCtrl.u32 = pVoReg->DATE_COEFF21.u32;
        DateOutCtrl.bits.dac0_in_sel = tmp.bits.dac0_in_sel;
        DateOutCtrl.bits.dac1_in_sel = tmp.bits.dac1_in_sel;
        DateOutCtrl.bits.dac2_in_sel = tmp.bits.dac2_in_sel;
        DateOutCtrl.bits.dac3_in_sel = tmp.bits.dac3_in_sel;
        DateOutCtrl.bits.dac4_in_sel = tmp.bits.dac4_in_sel;
        DateOutCtrl.bits.dac5_in_sel = tmp.bits.dac5_in_sel;
        pVoReg->DATE_COEFF21.u32 = DateOutCtrl.u32;
    }
    return;
}

HI_BOOL HAL_DISP_SetDateCoeff(HAL_DISP_OUTPUTCHANNEL_E enChan, HI_U32 u32Data)
{
    U_DATE_COEFF0 DATE_COEFF0;

    if (HAL_DISP_CHANNEL_DSD0 == enChan)
    {
        DATE_COEFF0.u32 = pVoReg->DATE_COEFF0.u32;
        DATE_COEFF0.u32 = u32Data;
        pVoReg->DATE_COEFF0.u32 = DATE_COEFF0.u32;
    }
    else
    {
        HAL_PRINT("Error channel id found in %s: L%d\n",__FUNCTION__, __LINE__);
        return HI_FALSE;
    }
    
    return HI_TRUE;
}

HI_BOOL HAL_DISP_DATE_Enable_VDACDetect(HAL_DISP_OUTPUTCHANNEL_E enChan, HI_BOOL bEnable, HI_BOOL  bNTSC)
{
    U_DATE_DACDET1  DATE_DACDET1;
    U_DATE_DACDET2  DATE_DACDET2;

    if (HAL_DISP_CHANNEL_DSD0 == enChan)
    {
        DATE_DACDET1.u32 = pVoReg->DATE_DACDET1.u32;
        DATE_DACDET2.u32 = pVoReg->DATE_DACDET2.u32;
        if (bEnable)
        {
            if (bNTSC == HI_FALSE)
            {
                DATE_DACDET1.bits.det_line = 0x14a;//330                
            }
            else
            {
                DATE_DACDET1.bits.det_line = 0x118;//280
            }            
            DATE_DACDET2.bits.vdac_det_en = 1; 
        }
	 else
	 {
            DATE_DACDET2.bits.vdac_det_en = 0; 
	 }
        pVoReg->DATE_DACDET1.u32 = DATE_DACDET1.u32;
        pVoReg->DATE_DACDET2.u32 = DATE_DACDET2.u32;
    }
    else
    {
        HAL_PRINT("Error channel id found in %s: L%d\n",__FUNCTION__, __LINE__);
        return HI_FALSE;
    }
    
    return HI_TRUE;
}

HI_VOID HAL_DISP_SetRegUp (HAL_DISP_OUTPUTCHANNEL_E enChan)
{
    volatile U_DHD0_CTRL DHD0_CTRL;    
    volatile  HI_U32 addr_REG; 
    if(enChan > HAL_DISP_CHANNEL_DSD0)
    {
        HAL_PRINT("Error,VDP_DISP_SetIntfEnable Select Wrong CHANNEL ID\n");
        return ;
    }
    addr_REG = Vou_GetChnAbsAddr(enChan,(HI_U32)&(pVoReg->DHD0_CTRL.u32));
    DHD0_CTRL.u32 = HAL_ReadReg((HI_U32*)addr_REG);
    DHD0_CTRL.bits.regup = 0x1;
    HAL_WriteReg((HI_U32*)addr_REG, DHD0_CTRL.u32); 
    return;
}


/* 设置寄存器帧场更新模式*/
HI_BOOL HAL_VIDEO_SetLayerUpMode(HAL_DISP_LAYER_E enLayer, HI_U32 bUpMode)
{
    U_V0_CTRL V0_CTRL;
    volatile  HI_U32 addr_REG;   
    if((LAYER_VHD_START <= enLayer)&&(enLayer <= LAYER_VSD_END))
    {
        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->V0_CTRL.u32));
        V0_CTRL.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        V0_CTRL.bits.vup_mode = bUpMode;
        HAL_WriteReg((HI_U32*)addr_REG, V0_CTRL.u32); 
    }
    else
    {
        HAL_PRINT("Error layer id found in %s: L%d\n",__FUNCTION__, __LINE__);
        return HI_FALSE;
    }
    return HI_TRUE;
}

HI_BOOL HAL_VIDEO_SetIfirMode(HAL_DISP_LAYER_E enLayer, HAL_IFIRMODE_E enMode)
{
    volatile U_V0_CTRL V0_CTRL;
    volatile  HI_U32 addr_REG;
    if((LAYER_VHD_START <= enLayer)&&(enLayer <= LAYER_VSD_END))
    {
        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->V0_CTRL.u32));
        V0_CTRL.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        V0_CTRL.bits.ifir_mode = enMode;
        HAL_WriteReg((HI_U32*)addr_REG, V0_CTRL.u32); 
    }
    else
    {
        HAL_PRINT("Error layer id found in %s: L%d\n",__FUNCTION__, __LINE__);
        return HI_FALSE;
    }
    return HI_TRUE;
}

HI_BOOL HAL_VIDEO_SetIfirCoef(HAL_DISP_LAYER_E enLayer, HI_S32 * s32Coef)
{
    volatile U_V0_IFIRCOEF01 V0_IFIRCOEF01;
    volatile U_V0_IFIRCOEF23 V0_IFIRCOEF23;
    volatile U_V0_IFIRCOEF45 V0_IFIRCOEF45;
    volatile U_V0_IFIRCOEF67 V0_IFIRCOEF67;   
    
    volatile  HI_U32 addr_REG;
    if((LAYER_VHD_START <= enLayer)&&(enLayer <= LAYER_VSD_END))
    {

        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->V0_IFIRCOEF01.u32));
        V0_IFIRCOEF01.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        V0_IFIRCOEF01.bits.coef0 = s32Coef[0];
        V0_IFIRCOEF01.bits.coef1 = s32Coef[1];
        HAL_WriteReg((HI_U32*)addr_REG, V0_IFIRCOEF01.u32); 

        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->V0_IFIRCOEF23.u32));
        V0_IFIRCOEF23.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        V0_IFIRCOEF23.bits.coef2 = s32Coef[2];
        V0_IFIRCOEF23.bits.coef3 = s32Coef[3];
        HAL_WriteReg((HI_U32*)addr_REG, V0_IFIRCOEF23.u32); 

        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->V0_IFIRCOEF45.u32));
        V0_IFIRCOEF45.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        V0_IFIRCOEF45.bits.coef4 = s32Coef[4];
        V0_IFIRCOEF45.bits.coef5 = s32Coef[5];
        HAL_WriteReg((HI_U32*)addr_REG, V0_IFIRCOEF45.u32); 

        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->V0_IFIRCOEF67.u32));
        V0_IFIRCOEF67.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        V0_IFIRCOEF67.bits.coef6 = s32Coef[6];
        V0_IFIRCOEF67.bits.coef7 = s32Coef[7];
        HAL_WriteReg((HI_U32*)addr_REG, V0_IFIRCOEF67.u32);
    }
    else
    {
        HAL_PRINT("Error layer id found in %s: L%d\n",__FUNCTION__, __LINE__);
        return HI_FALSE;
    }


    return HI_TRUE;
}

//设置视频显示区域窗口
HI_BOOL HAL_VIDEO_SetLayerDispRect(HAL_DISP_LAYER_E enLayer, RECT_S *pstRect)
{
    volatile U_V0_DFPOS V0_DFPOS;
    volatile U_V0_DLPOS V0_DLPOS;    
    
    volatile U_G0_DFPOS G0_DFPOS;
    volatile U_G0_DLPOS G0_DLPOS;    
    volatile  HI_U32 addr_REG;
    
    if((LAYER_VHD_START <= enLayer)&&(enLayer <= LAYER_VSD_END))
    {
        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->V0_DFPOS.u32));
        V0_DFPOS.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        V0_DFPOS.bits.disp_xfpos = pstRect->s32X;
        V0_DFPOS.bits.disp_yfpos = pstRect->s32Y;
        HAL_WriteReg((HI_U32*)addr_REG, V0_DFPOS.u32); 

        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->V0_DLPOS.u32));
        V0_DLPOS.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        V0_DLPOS.bits.disp_xlpos = pstRect->s32X +pstRect->u32Width -1;
        V0_DLPOS.bits.disp_ylpos = pstRect->s32Y + pstRect->u32Height -1;
        HAL_WriteReg((HI_U32*)addr_REG, V0_DLPOS.u32);         
    }
    else if((LAYER_GFX_START <= enLayer)&&(enLayer <= LAYER_GFX_END))
    {
        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->G0_DFPOS.u32));
        G0_DFPOS.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        G0_DFPOS.bits.disp_xfpos = pstRect->s32X;
        G0_DFPOS.bits.disp_yfpos = pstRect->s32Y;
        HAL_WriteReg((HI_U32*)addr_REG, G0_DFPOS.u32); 

        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->G0_DLPOS.u32));
        G0_DLPOS.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        G0_DLPOS.bits.disp_xlpos = pstRect->s32X +pstRect->u32Width -1;
        G0_DLPOS.bits.disp_ylpos = pstRect->s32Y + pstRect->u32Height -1;
        HAL_WriteReg((HI_U32*)addr_REG, G0_DLPOS.u32); 
    }
    else
    {
        HAL_PRINT("Error layer id found in %s: L%d\n",__FUNCTION__, __LINE__);
        return HI_FALSE;
    }
    
    return HI_TRUE;
}

//设置视频图像显示区域窗口
HI_BOOL HAL_VIDEO_SetLayerVideoRect(HAL_DISP_LAYER_E enLayer, RECT_S *pstRect)
{
    volatile U_V0_VFPOS V0_VFPOS;    
    volatile U_V0_VLPOS V0_VLPOS;
    
    volatile U_G0_VFPOS G0_VFPOS;    
    volatile U_G0_VLPOS G0_VLPOS;
    volatile  HI_U32 addr_REG;
    
    if((LAYER_VHD_START <= enLayer)&&(enLayer <= LAYER_VSD_END))
    {
        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->V0_VFPOS.u32));
        V0_VFPOS.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        V0_VFPOS.bits.video_xfpos = pstRect->s32X;
        V0_VFPOS.bits.video_yfpos = pstRect->s32Y;
        HAL_WriteReg((HI_U32*)addr_REG, V0_VFPOS.u32); 

        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->V0_VLPOS.u32));
        V0_VLPOS.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        V0_VLPOS.bits.video_xlpos = pstRect->s32X +pstRect->u32Width -1;
        V0_VLPOS.bits.video_ylpos = pstRect->s32Y + pstRect->u32Height -1;
        HAL_WriteReg((HI_U32*)addr_REG, V0_VLPOS.u32); 
    }    
    else if((LAYER_GFX_START <= enLayer)&&(enLayer <= LAYER_GFX_END))
    {
        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->G0_VFPOS.u32));
        G0_VFPOS.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        G0_VFPOS.bits.video_xfpos = pstRect->s32X;
        G0_VFPOS.bits.video_yfpos = pstRect->s32Y;
        HAL_WriteReg((HI_U32*)addr_REG, G0_VFPOS.u32); 

        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->G0_VLPOS.u32));
        G0_VLPOS.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        G0_VLPOS.bits.video_xlpos = pstRect->s32X +pstRect->u32Width -1;
        G0_VLPOS.bits.video_ylpos = pstRect->s32Y + pstRect->u32Height -1;
        HAL_WriteReg((HI_U32*)addr_REG, G0_VLPOS.u32);   

    }
    else
    {
        HAL_PRINT("Error layer id found in %s: L%d\n",__FUNCTION__, __LINE__);
        return HI_FALSE;
    }
    
    return HI_TRUE;
}


HI_BOOL HAL_VIDEO_SetMultiAreaLAddr  (HAL_DISP_LAYER_E enLayer,HI_U32 area_num,HI_U32 u32LAddr, HI_U16 stride)
{
    volatile U_V0_P0LADDR V0_P0LADDR;
    volatile U_V0_P0STRIDE V0_P0STRIDE;
    volatile  HI_U32 addr_REG;
    HI_U32 u32_pxaddr_addr = 0;
    HI_U32 u32_pxstride_addr = 0;

    if((LAYER_VHD_START <= enLayer)&&(enLayer <= LAYER_VSD_END))
    {
        u32_pxaddr_addr   = (HI_U32)&(pVoReg->V0_P0LADDR.u32) + area_num * MULTI_AREA_CFG_LEN;
        u32_pxstride_addr = (HI_U32)&(pVoReg->V0_P0STRIDE.u32) + area_num * MULTI_AREA_CFG_LEN;

        addr_REG = Vou_GetAbsAddr(enLayer,u32_pxaddr_addr);
        V0_P0LADDR.u32   = u32LAddr;
        HAL_WriteReg((HI_U32*)addr_REG, V0_P0LADDR.u32); 

        addr_REG = Vou_GetAbsAddr(enLayer,u32_pxstride_addr);
        V0_P0STRIDE.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        V0_P0STRIDE.bits.surface_stride = stride;
        HAL_WriteReg((HI_U32*)addr_REG, V0_P0STRIDE.u32); 
    }
    else
    {
        HAL_PRINT("Error layer id found in %s: L%d\n",__FUNCTION__, __LINE__);
        return HI_FALSE;
    }

    return HI_TRUE;
}

HI_BOOL HAL_VIDEO_SetMultiAreaCAddr  (HAL_DISP_LAYER_E enLayer,HI_U32 area_num,HI_U32 u32CAddr, HI_U16 stride)
{
    volatile U_V0_P0CADDR V0_P0CADDR;
    volatile U_V0_P0STRIDE V0_P0STRIDE;
    volatile  HI_U32 addr_REG;
    HI_U32 u32_pxaddr_addr = 0;
    HI_U32 u32_pxstride_addr = 0;

    if((LAYER_VHD_START <= enLayer)&&(enLayer <= LAYER_VSD_END))
    {
        u32_pxaddr_addr   = (HI_U32)&(pVoReg->V0_P0CADDR.u32) + area_num * MULTI_AREA_CFG_LEN;
        u32_pxstride_addr = (HI_U32)&(pVoReg->V0_P0STRIDE.u32) + area_num * MULTI_AREA_CFG_LEN;

        addr_REG = Vou_GetAbsAddr(enLayer,u32_pxaddr_addr);
        V0_P0CADDR.u32   = u32CAddr;
        HAL_WriteReg((HI_U32*)addr_REG, V0_P0CADDR.u32); 

        addr_REG = Vou_GetAbsAddr(enLayer,u32_pxstride_addr);
        V0_P0STRIDE.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        V0_P0STRIDE.bits.surface_cstride = stride;
        HAL_WriteReg((HI_U32*)addr_REG, V0_P0STRIDE.u32); 
    }
    else
    {
        HAL_PRINT("Error layer id found in %s: L%d\n",__FUNCTION__, __LINE__);
        return HI_FALSE;
    }

    return HI_TRUE;
}


HI_BOOL HAL_VIDEO_SetMultiAreaEnable(HAL_DISP_LAYER_E enLayer,HI_U32 area_num,HI_U32 bEnable)
{
    volatile U_V0_16REGIONENL  V0_16REGIONENL;
    volatile HI_U32    regionen_value;
    volatile  HI_U32 addr_REG;
    
    if((LAYER_VHD_START <= enLayer)&&(enLayer <= LAYER_VSD_END))
    {
        if(area_num < 16)
        {
            addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->V0_16REGIONENL.u32));

            V0_16REGIONENL.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            if(bEnable)
            {
                regionen_value = ((bEnable << area_num ) | V0_16REGIONENL.u32);
            }
            else
            {
                regionen_value = ~(1 << area_num ) & V0_16REGIONENL.u32;
            }
            HAL_WriteReg((HI_U32*)addr_REG, regionen_value); 
        }       
        else
        {
            HAL_PRINT("Error area num found in %s: L%d\n",__FUNCTION__, __LINE__);
            return HI_FALSE;
        }
    }    
    else
    {
        HAL_PRINT("Error layer id found in %s: L%d\n",__FUNCTION__, __LINE__);
        return HI_FALSE;
    }
    
    return HI_TRUE;
}


HI_BOOL HAL_VIDEO_GetMultiAreaEnable(HAL_DISP_LAYER_E enLayer,HI_U32 area_num,HI_U32 *pu32Enable)
{
    volatile U_V0_16REGIONENL V0_16REGIONENL;
    volatile  HI_U32 addr_REG;
    
    if((LAYER_VHD_START <= enLayer)&&(enLayer <= LAYER_VSD_END))
    {
        if(area_num <32)
        {
            addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->V0_16REGIONENL.u32));

            V0_16REGIONENL.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            *pu32Enable = (1 << area_num ) & V0_16REGIONENL.u32;
        }        
        else
        {
            HAL_PRINT("Error area num found in %s: L%d\n",__FUNCTION__, __LINE__);
            return HI_FALSE;
        }
    }    
    else
    {
        HAL_PRINT("Error layer id found in %s: L%d\n",__FUNCTION__, __LINE__);
        return HI_FALSE;
    }
    
    return HI_TRUE;
}


//disable all area
HI_BOOL HAL_VIDEO_SetAllAreaDisable  (HAL_DISP_LAYER_E enLayer)
{
    volatile  HI_U32 addr_REG;

    if((LAYER_VHD_START <= enLayer)&&(enLayer <= LAYER_VSD_END))
    {
        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->V0_16REGIONENL.u32));
        HAL_WriteReg((HI_U32*)addr_REG, 0x0); 
    }
    else
    {
        HAL_PRINT("Error layer id found in %s: L%d\n",__FUNCTION__, __LINE__);
        return HI_FALSE;
    }

    return HI_TRUE;
}

/*多区域的分辨率只要配置宽度即可，高度已经有显示的原始位置和终点位置，stride确定*/
HI_BOOL HAL_VIDEO_SetMultiAreaReso(HAL_DISP_LAYER_E enLayer,HI_U32 area_num,
                                              HI_U32 u32Width)
{
    U_V0_P0RESO  V0_P0RESO;
    HI_U32 pstReso;

    volatile  HI_U32 addr_REG;

    
    if((LAYER_VHD_START <= enLayer)&&(enLayer <= LAYER_VSD_END))
    {
        pstReso  = (HI_U32)&(pVoReg->V0_P0RESO.u32)  + area_num * MULTI_AREA_CFG_LEN;

        //cfg resolution
        addr_REG = Vou_GetAbsAddr(enLayer,pstReso);
        V0_P0RESO.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        V0_P0RESO.bits.w = u32Width -1;
        HAL_WriteReg((HI_U32*)addr_REG, V0_P0RESO.u32); 
    }   
    else
    {
        HAL_PRINT("Error layer id found in %s: L%d\n",__FUNCTION__, __LINE__);
        return HI_FALSE;
    }

    return HI_TRUE;
}


HI_BOOL HAL_VIDEO_SetMultiAreaRect(HAL_DISP_LAYER_E enLayer,HI_U32 area_num,RECT_S *pstVideoAreaRect)
{
    U_V0_P0VFPOS V0_P0VFPOS;
    U_V0_P0VLPOS V0_P0VLPOS;

    HI_U32 pstVfpos;
    HI_U32 pstVlpos;

    volatile  HI_U32 addr_REG;

    if((LAYER_VHD_START <= enLayer)&&(enLayer <= LAYER_VSD_END))
    {
        pstVfpos = (HI_U32)&(pVoReg->V0_P0VFPOS.u32) + area_num * MULTI_AREA_CFG_LEN;
        pstVlpos = (HI_U32)&(pVoReg->V0_P0VLPOS.u32) + area_num * MULTI_AREA_CFG_LEN;

        //cfg fpos
        addr_REG = Vou_GetAbsAddr(enLayer,pstVfpos);
        V0_P0VFPOS.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        V0_P0VFPOS.bits.video_xfpos = pstVideoAreaRect->s32X;
        V0_P0VFPOS.bits.video_yfpos = pstVideoAreaRect->s32Y;
        HAL_WriteReg((HI_U32*)addr_REG, V0_P0VFPOS.u32); 

        //cfg lpos
        addr_REG = Vou_GetAbsAddr(enLayer,pstVlpos);
        V0_P0VLPOS.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        V0_P0VLPOS.bits.video_xlpos = pstVideoAreaRect->s32X + pstVideoAreaRect->u32Width -1;
        V0_P0VLPOS.bits.video_ylpos = pstVideoAreaRect->s32Y + pstVideoAreaRect->u32Height -1;
        HAL_WriteReg((HI_U32*)addr_REG, V0_P0VLPOS.u32); 
    }
    else
    {
        HAL_PRINT("Error layer id found in %s: L%d\n",__FUNCTION__, __LINE__);
        return HI_FALSE;
    }

    return HI_TRUE;
}

HI_BOOL HAL_LAYER_EnableLayer(HAL_DISP_LAYER_E enLayer, HI_U32 bEnable)
                                                           
{
    volatile U_V0_CTRL V0_CTRL;
    volatile U_G0_CTRL G0_CTRL;
    
    volatile  HI_U32 addr_REG;
    if((LAYER_VHD_START <= enLayer)&&(enLayer <= LAYER_VSD_END))
    {
        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->V0_CTRL.u32));
        V0_CTRL.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        V0_CTRL.bits.surface_en = bEnable;
        HAL_WriteReg((HI_U32*)addr_REG, V0_CTRL.u32); 
    }
    else if((LAYER_GFX_START <= enLayer)&&(enLayer <= LAYER_GFX_END))
    {
        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->G0_CTRL.u32));
        G0_CTRL.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        G0_CTRL.bits.surface_en = bEnable;
        HAL_WriteReg((HI_U32*)addr_REG, G0_CTRL.u32); 
    }
    else
    {
        HAL_PRINT("Error layer id found in %s: L%d\n",__FUNCTION__, __LINE__);
        return HI_FALSE;
    }

    return HI_TRUE;
}

HI_BOOL HAL_LAYER_GetLayerEnable(HAL_DISP_LAYER_E enLayer,HI_U32 *pu32Enable)
{
    volatile U_V0_CTRL V0_CTRL;
    volatile U_G0_CTRL G0_CTRL;

    volatile  HI_U32 addr_REG;

    if((LAYER_VHD_START <= enLayer)&&(enLayer <= LAYER_VSD_END))
    {
        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->V0_CTRL.u32));
        V0_CTRL.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        *pu32Enable = V0_CTRL.bits.surface_en;
    }
    else if((LAYER_GFX_START <= enLayer) && (enLayer <= LAYER_GFX_END))
    {
        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->G0_CTRL.u32));
        G0_CTRL.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        *pu32Enable = G0_CTRL.bits.surface_en;
    }
    else
    {
        HAL_PRINT("Error layer id found in %s: L%d\n",__FUNCTION__, __LINE__);
        return HI_FALSE;
    }

    return HI_TRUE;
}



HI_BOOL HAL_LAYER_SetLayerDataFmt(HAL_DISP_LAYER_E enLayer,
                                            HAL_DISP_PIXEL_FORMAT_E  enDataFmt)
{
    volatile U_V0_CTRL V0_CTRL;
    volatile U_G0_CTRL G0_CTRL;
    volatile  HI_U32 addr_REG;
    if((LAYER_VHD_START <= enLayer)&&(enLayer <= LAYER_VSD_END))
    {
        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->V0_CTRL.u32));
        V0_CTRL.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        V0_CTRL.bits.ifmt = enDataFmt;
        HAL_WriteReg((HI_U32*)addr_REG, V0_CTRL.u32); 
    }   
    else if((LAYER_GFX_START <= enLayer)&&(enLayer <= LAYER_GFX_END))
    {
        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->G0_CTRL.u32));
        G0_CTRL.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        G0_CTRL.bits.ifmt = enDataFmt;
        HAL_WriteReg((HI_U32*)addr_REG, G0_CTRL.u32); 
    }
    else
    {
        HAL_PRINT("Error layer id found in %s: L%d\n",__FUNCTION__, __LINE__);
        return HI_FALSE;
    }

    return HI_TRUE;
}


HI_BOOL HAL_LAYER_GetLayerDataFmt(HAL_DISP_LAYER_E enLayer,HI_U32 *pu32Fmt)
{
    volatile U_V0_CTRL V0_CTRL;
    volatile U_G0_CTRL G0_CTRL;
    volatile  HI_U32 addr_REG;
    
    if((LAYER_VHD_START <= enLayer)&&(enLayer <= LAYER_VSD_END))
    {
        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->V0_CTRL.u32));
        V0_CTRL.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        *pu32Fmt = V0_CTRL.bits.ifmt;
    }   
    else if((LAYER_GFX_START <= enLayer)&&(enLayer <= LAYER_GFX_END))
    {
        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->G0_CTRL.u32));
        G0_CTRL.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        *pu32Fmt = G0_CTRL.bits.ifmt;
    }
    else
    {
        HAL_PRINT("Error layer id found in %s: L%d\n",__FUNCTION__, __LINE__);
        return HI_FALSE;
    }

    return HI_TRUE;
}


//设置层色彩空间转换参数
HI_BOOL HAL_LAYER_SetCscCoef(HAL_DISP_LAYER_E enLayer, CscCoef_S *pstCscCoef)
{
    U_G0_CSC_IDC G0_CSC_IDC;
    U_G0_CSC_ODC  G0_CSC_ODC;    
    U_G0_CSC_IODC  G0_CSC_IODC;
    U_V0_CSC_IDC  V0_CSC_IDC;
    U_V0_CSC_ODC  V0_CSC_ODC;    
    U_V0_CSC_IODC  V0_CSC_IODC;
    
    volatile U_V0_CSC_P0 V0_CSC_P0;
    volatile U_V0_CSC_P1 V0_CSC_P1;
    volatile U_V0_CSC_P2 V0_CSC_P2;
    volatile U_V0_CSC_P3 V0_CSC_P3;
    volatile U_V0_CSC_P4 V0_CSC_P4;

    volatile U_G0_CSC_P0 G0_CSC_P0;
    volatile U_G0_CSC_P1 G0_CSC_P1;
    volatile U_G0_CSC_P2 G0_CSC_P2;
    volatile U_G0_CSC_P3 G0_CSC_P3;
    volatile U_G0_CSC_P4 G0_CSC_P4;
    
    volatile  HI_U32 addr_REG;

    if((LAYER_VHD_START <= enLayer) && (enLayer <= LAYER_VSD_END))
    {
        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->V0_CSC_IDC.u32));
        V0_CSC_IDC.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        V0_CSC_IDC.bits.cscidc1 = HAL_GetXDC_BUMA(pstCscCoef->csc_in_dc1);
      // V0_CSC_IDC.bits.cscidc0 = HAL_GetXDC_BUMA(pstCscCoef->csc_in_dc0);        
        V0_CSC_IDC.bits.cscidc0 = HAL_GetXDC_BUMA(pstCscCoef->csc_in_dc2);
        HAL_WriteReg((HI_U32*)addr_REG, V0_CSC_IDC.u32); 

        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->V0_CSC_ODC.u32));
        V0_CSC_ODC.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        V0_CSC_ODC.bits.cscodc1 = HAL_GetXDC_BUMA(pstCscCoef->csc_out_dc1);
        //V0_CSC_ODC.bits.cscodc0 = HAL_GetXDC_BUMA(pstCscCoef->csc_out_dc0);        
        V0_CSC_ODC.bits.cscodc0 = HAL_GetXDC_BUMA(pstCscCoef->csc_out_dc2);
        HAL_WriteReg((HI_U32*)addr_REG, V0_CSC_ODC.u32);
        
        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->V0_CSC_IODC.u32));
        V0_CSC_IODC.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        //V0_CSC_IODC.bits.cscodc2 = HAL_GetXDC_BUMA(pstCscCoef->csc_out_dc2);
        //V0_CSC_IODC.bits.cscidc2 = HAL_GetXDC_BUMA(pstCscCoef->csc_in_dc2);
        
        V0_CSC_IODC.bits.cscodc2 = HAL_GetXDC_BUMA(pstCscCoef->csc_out_dc0);
        V0_CSC_IODC.bits.cscidc2 = HAL_GetXDC_BUMA(pstCscCoef->csc_in_dc0);
        HAL_WriteReg((HI_U32*)addr_REG, V0_CSC_IODC.u32);
        
        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->V0_CSC_P0.u32));
        V0_CSC_P0.u32 = HAL_ReadReg((HI_U32*)addr_REG);        
        V0_CSC_P0.bits.cscp00 = HAL_Conver_CscCoef(pstCscCoef->csc_coef00);
        V0_CSC_P0.bits.cscp01 = HAL_Conver_CscCoef(pstCscCoef->csc_coef01);
        HAL_WriteReg((HI_U32*)addr_REG, V0_CSC_P0.u32); 

        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->V0_CSC_P1.u32));
        V0_CSC_P1.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        V0_CSC_P1.bits.cscp02 = HAL_Conver_CscCoef(pstCscCoef->csc_coef02);
        V0_CSC_P1.bits.cscp10 = HAL_Conver_CscCoef(pstCscCoef->csc_coef10);
        HAL_WriteReg((HI_U32*)addr_REG, V0_CSC_P1.u32); 

        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->V0_CSC_P2.u32));
        V0_CSC_P2.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        V0_CSC_P2.bits.cscp11 = HAL_Conver_CscCoef(pstCscCoef->csc_coef11);
        V0_CSC_P2.bits.cscp12 = HAL_Conver_CscCoef(pstCscCoef->csc_coef12);
        HAL_WriteReg((HI_U32*)addr_REG, V0_CSC_P2.u32); 

        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->V0_CSC_P3.u32));
        V0_CSC_P3.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        V0_CSC_P3.bits.cscp20 = HAL_Conver_CscCoef(pstCscCoef->csc_coef20);
        V0_CSC_P3.bits.cscp21 = HAL_Conver_CscCoef(pstCscCoef->csc_coef21);
        HAL_WriteReg((HI_U32*)addr_REG, V0_CSC_P3.u32); 

        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->V0_CSC_P4.u32));
        V0_CSC_P4.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        V0_CSC_P4.bits.cscp22 = HAL_Conver_CscCoef(pstCscCoef->csc_coef22);
        HAL_WriteReg((HI_U32*)addr_REG, V0_CSC_P4.u32); 
    }    
    else if((LAYER_GFX_START <= enLayer) && (enLayer <= LAYER_GFX_END))
    {
        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->G0_CSC_IDC.u32));
        G0_CSC_IDC.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        G0_CSC_IDC.bits.cscidc1 = HAL_GetXDC_BUMA(pstCscCoef->csc_in_dc1);
        //G0_CSC_IDC.bits.cscidc0 = HAL_GetXDC_BUMA(pstCscCoef->csc_in_dc0);        
        G0_CSC_IDC.bits.cscidc0 = HAL_GetXDC_BUMA(pstCscCoef->csc_in_dc2);
        HAL_WriteReg((HI_U32*)addr_REG, G0_CSC_IDC.u32); 

        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->G0_CSC_ODC.u32));
        G0_CSC_ODC.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        G0_CSC_ODC.bits.cscodc1 = HAL_GetXDC_BUMA(pstCscCoef->csc_out_dc1);
        //G0_CSC_ODC.bits.cscodc0 = HAL_GetXDC_BUMA(pstCscCoef->csc_out_dc0);        
        G0_CSC_ODC.bits.cscodc0 = HAL_GetXDC_BUMA(pstCscCoef->csc_out_dc2);
        HAL_WriteReg((HI_U32*)addr_REG, G0_CSC_ODC.u32);
        
        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->G0_CSC_IODC.u32));
        G0_CSC_IODC.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        //G0_CSC_IODC.bits.cscodc2 = HAL_GetXDC_BUMA(pstCscCoef->csc_out_dc2);
        //G0_CSC_IODC.bits.cscidc2 = HAL_GetXDC_BUMA(pstCscCoef->csc_in_dc2);
        G0_CSC_IODC.bits.cscodc2 = HAL_GetXDC_BUMA(pstCscCoef->csc_out_dc0);
        G0_CSC_IODC.bits.cscidc2 = HAL_GetXDC_BUMA(pstCscCoef->csc_in_dc0);
        HAL_WriteReg((HI_U32*)addr_REG, G0_CSC_IODC.u32);
        
        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->G0_CSC_P0.u32));
        G0_CSC_P0.u32 = HAL_ReadReg((HI_U32*)addr_REG);        
        G0_CSC_P0.bits.cscp00 = HAL_Conver_CscCoef(pstCscCoef->csc_coef00);
        G0_CSC_P0.bits.cscp01 = HAL_Conver_CscCoef(pstCscCoef->csc_coef01);
        HAL_WriteReg((HI_U32*)addr_REG, G0_CSC_P0.u32); 

        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->G0_CSC_P1.u32));
        G0_CSC_P1.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        G0_CSC_P1.bits.cscp02 = HAL_Conver_CscCoef(pstCscCoef->csc_coef02);
        G0_CSC_P1.bits.cscp10 = HAL_Conver_CscCoef(pstCscCoef->csc_coef10);
        HAL_WriteReg((HI_U32*)addr_REG, G0_CSC_P1.u32); 

        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->G0_CSC_P2.u32));
        G0_CSC_P2.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        G0_CSC_P2.bits.cscp11 = HAL_Conver_CscCoef(pstCscCoef->csc_coef11);
        G0_CSC_P2.bits.cscp12 = HAL_Conver_CscCoef(pstCscCoef->csc_coef12);
        HAL_WriteReg((HI_U32*)addr_REG, G0_CSC_P2.u32); 

        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->G0_CSC_P3.u32));
        G0_CSC_P3.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        G0_CSC_P3.bits.cscp20 = HAL_Conver_CscCoef(pstCscCoef->csc_coef20);
        G0_CSC_P3.bits.cscp21 = HAL_Conver_CscCoef(pstCscCoef->csc_coef21);
        HAL_WriteReg((HI_U32*)addr_REG, G0_CSC_P3.u32); 

        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->G0_CSC_P4.u32));
        G0_CSC_P4.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        G0_CSC_P4.bits.cscp22 = HAL_Conver_CscCoef(pstCscCoef->csc_coef22);
        HAL_WriteReg((HI_U32*)addr_REG, G0_CSC_P4.u32); 
    }
    else
    {
        HAL_PRINT("Error layer id found in %s: L%d\n",__FUNCTION__, __LINE__);
        return HI_FALSE;
    }

    return HI_TRUE;
}

HI_BOOL HAL_LAYER_SetCscMode(HAL_DISP_LAYER_E enLayer, HI_BOOL bIsHCModeBy709)
{
    U_G0_CSC_IDC G0_CSC_IDC;          
    volatile  HI_U32 addr_REG;

    
     //对于3535 HC层只有模式选择，但是不会对其他的CSC参数有修改   
    if(HAL_DISP_LAYER_HC0 == enLayer) 
    {
        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->G0_CSC_IDC.u32));
        G0_CSC_IDC.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        if (HI_TRUE == bIsHCModeBy709)
        {
            G0_CSC_IDC.bits.csc_mode = 0x7; /* 111： RGB2YUV_709 */
        }
        else
        {
            G0_CSC_IDC.bits.csc_mode = 0x6; /* 110： RGB2YUV_601 */
        }
        HAL_WriteReg((HI_U32*)addr_REG, G0_CSC_IDC.u32); 
    }
    else
    {
        
    }

    return HI_TRUE;
}


/*配置层CSC使能*/
HI_BOOL HAL_LAYER_SetCscEn(HAL_DISP_LAYER_E enLayer, HI_BOOL bCscEn)
{
    U_V0_CSC_IDC  V0_CSC_IDC;
    U_G0_CSC_IDC G0_CSC_IDC;
    volatile  HI_U32 addr_REG;

    if((LAYER_VHD_START <= enLayer)&&(enLayer <= LAYER_VSD_END))
    {
        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->V0_CSC_IDC.u32));
        V0_CSC_IDC.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        V0_CSC_IDC.bits.csc_en = bCscEn;
        HAL_WriteReg((HI_U32*)addr_REG, V0_CSC_IDC.u32); 
    }
    else if((LAYER_GFX_START <= enLayer)&&(enLayer <= LAYER_GFX_END))
    {
        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->G0_CSC_IDC.u32));
        G0_CSC_IDC.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        G0_CSC_IDC.bits.csc_en = bCscEn;
        HAL_WriteReg((HI_U32*)addr_REG, G0_CSC_IDC.u32); 
    }
    else
    {
        HAL_PRINT("Error, Wrong layer ID!\n");
        return HI_FALSE;
    }
    
    return HI_TRUE;
}



//配置源窗口
HI_BOOL HAL_LAYER_SetLayerInRect(HAL_DISP_LAYER_E enLayer, RECT_S *pstRect)
{
    U_V0_IRESO V0_IRESO;
    U_G0_IRESO G0_IRESO;
    //U_G0SFPOS G0SFPOS;
    volatile  HI_U32 addr_REG;
    
    if((LAYER_VHD_START <= enLayer)&&(enLayer <= LAYER_VSD_END))
    {
        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->V0_IRESO.u32));
        V0_IRESO.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        V0_IRESO.bits.iw = pstRect->u32Width- 1;
        V0_IRESO.bits.ih = pstRect->u32Height- 1;
        HAL_WriteReg((HI_U32*)addr_REG, V0_IRESO.u32); 
    }   
    else if((LAYER_GFX_START <= enLayer)&&(enLayer <= LAYER_GFX_END))
    {
        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->G0_IRESO.u32));
        G0_IRESO.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        G0_IRESO.bits.iw = pstRect->u32Width- 1;
        G0_IRESO.bits.ih = pstRect->u32Height- 1;
        HAL_WriteReg((HI_U32*)addr_REG, G0_IRESO.u32); 
    }
    else
    {
        HAL_PRINT("Error layer id found in %s: L%d\n",__FUNCTION__, __LINE__);
        return HI_FALSE;
    }

    return HI_TRUE;
}


//配置输出窗口
HI_BOOL HAL_LAYER_SetLayerOutRect(HAL_DISP_LAYER_E enLayer, RECT_S *pstRect)
{
    U_V0_ORESO V0_ORESO;
    //U_G0_ORESO G0_ORESO;
    //U_G0SFPOS G0SFPOS;
    volatile  HI_U32 addr_REG;
    
    if((LAYER_VHD_START <= enLayer)&&(enLayer <= LAYER_VSD_END))
    {
        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->V0_ORESO.u32));
        V0_ORESO.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        V0_ORESO.bits.ow = pstRect->u32Width- 1;
        V0_ORESO.bits.oh = pstRect->u32Height- 1;
        HAL_WriteReg((HI_U32*)addr_REG, V0_ORESO.u32); 
    }   
    else if((LAYER_GFX_START <= enLayer)&&(enLayer <= LAYER_GFX_END))
    {
        #if 0
        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->G0_IRESO.u32));
        G0_IRESO.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        G0_IRESO.bits.iw = pstRect->u32Width- 1;
        G0_IRESO.bits.ih = pstRect->u32Height- 1;
        HAL_WriteReg((HI_U32*)addr_REG, G0_IRESO.u32); 
        #endif
    }
    else
    {
        HAL_PRINT("Error layer id found in %s: L%d\n",__FUNCTION__, __LINE__);
        return HI_FALSE;
    }
    return HI_FALSE;
}

HI_BOOL HAL_LAYER_SetLayerGalpha(HAL_DISP_LAYER_E enLayer,
                                     HI_U8 u8Alpha0)
{
    volatile U_V0_CBMPARA V0_CBMPARA;
    volatile U_G0_CBMPARA G0_CBMPARA;

    volatile  HI_U32 addr_REG;
    if((LAYER_VHD_START <= enLayer)&&(enLayer <= LAYER_VSD_END))
    {
        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->V0_CBMPARA.u32));
        V0_CBMPARA.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        V0_CBMPARA.bits.galpha = u8Alpha0;
        HAL_WriteReg((HI_U32*)addr_REG, V0_CBMPARA.u32); 
    }
    else if((LAYER_GFX_START <= enLayer)&&(enLayer <= LAYER_GFX_END))
    {
        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->G0_CBMPARA.u32));
        G0_CBMPARA.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        G0_CBMPARA.bits.galpha = u8Alpha0;
        HAL_WriteReg((HI_U32*)addr_REG, G0_CBMPARA.u32); 
    }
    else
    {
        HAL_PRINT("Error layer id %d found in %s: L%d\n",enLayer,__FUNCTION__, __LINE__);
        return HI_FALSE;
    }    

    return HI_TRUE;    
}

//查询层全局alpha
HI_BOOL HAL_LAYER_GetLayerGalpha(HAL_DISP_LAYER_E enLayer,HI_U8 *pu8Alpha0)
{
    volatile U_V0_CBMPARA V0_CBMPARA;
    volatile U_G0_CBMPARA G0_CBMPARA;    
    volatile  HI_U32 addr_REG;
    
    if((LAYER_VHD_START <= enLayer)&&(enLayer <= LAYER_VSD_END))
    {
        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->V0_CBMPARA.u32));
        V0_CBMPARA.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        *pu8Alpha0 = V0_CBMPARA.bits.galpha;
    }
    else if((LAYER_GFX_START <= enLayer)&&(enLayer <= LAYER_GFX_END))
    {
        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->G0_CBMPARA.u32));
        G0_CBMPARA.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        *pu8Alpha0 = G0_CBMPARA.bits.galpha;
    }
    else
    {
        HAL_PRINT("Error layer id found in %s: L%d\n",__FUNCTION__, __LINE__);
        return HI_FALSE;
    }

    return HI_TRUE;
}

HI_BOOL HAL_LAYER_GetCoefAddr(HAL_DISP_LAYER_E enLayer,
                                   HAL_DISP_COEFMODE_E enMode,
                                   HI_U32 *pu32Addr)
{
    volatile U_V0_HLCOEFAD V0_HLCOEFAD;
    volatile U_V0_HCCOEFAD V0_HCCOEFAD;
    volatile U_V0_VLCOEFAD V0_VLCOEFAD;
    volatile U_V0_VCCOEFAD V0_VCCOEFAD;
    volatile U_WBC_DHD0_HLCOEFAD WBC_DHD0_HLCOEFAD;
    volatile U_WBC_DHD0_HCCOEFAD WBC_DHD0_HCCOEFAD;
    volatile U_WBC_DHD0_VLCOEFAD WBC_DHD0_VLCOEFAD;
    volatile U_WBC_DHD0_VCCOEFAD WBC_DHD0_VCCOEFAD;

    volatile  HI_U32 addr_REG;

    if(enLayer <= HAL_DISP_LAYER_VSD0)
    {
        if(enMode == HAL_DISP_COEFMODE_HORL)
        {
            addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->V0_HLCOEFAD.u32));
            V0_HLCOEFAD.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            *pu32Addr = V0_HLCOEFAD.bits.coef_addr;
        }        
        else if(enMode == HAL_DISP_COEFMODE_HORC)
        {
            addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->V0_HCCOEFAD.u32));
            V0_HCCOEFAD.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            *pu32Addr = V0_HCCOEFAD.bits.coef_addr;
        }
        else if(enMode == HAL_DISP_COEFMODE_VERL)
        {
            addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->V0_VLCOEFAD.u32));
            V0_VLCOEFAD.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            *pu32Addr = V0_VLCOEFAD.bits.coef_addr;
        }        
        else if(enMode == HAL_DISP_COEFMODE_VERC)
        {
            addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->V0_VCCOEFAD.u32));
            V0_VCCOEFAD.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            *pu32Addr = V0_VCCOEFAD.bits.coef_addr;
        }
        else
        {
            HAL_PRINT("Error coef found in %s: L%d\n",__FUNCTION__, __LINE__);
            return HI_FALSE;
        }
    }   
    else if(enLayer == HAL_DISP_LAYER_WBC_D)
    {
        if(enMode == HAL_DISP_COEFMODE_HORL)
        {
            addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->WBC_DHD0_HLCOEFAD.u32));
            WBC_DHD0_HLCOEFAD.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            *pu32Addr = WBC_DHD0_HLCOEFAD.bits.coef_addr;
        }
        else if(enMode == HAL_DISP_COEFMODE_HORC)
        {
            addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->WBC_DHD0_HCCOEFAD.u32));
            WBC_DHD0_HCCOEFAD.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            *pu32Addr = WBC_DHD0_HCCOEFAD.bits.coef_addr;
        }
        else if(enMode == HAL_DISP_COEFMODE_VERL)
        {
            addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->WBC_DHD0_VLCOEFAD.u32));
            WBC_DHD0_VLCOEFAD.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            *pu32Addr = WBC_DHD0_VLCOEFAD.bits.coef_addr;
        }        
        else if(enMode == HAL_DISP_COEFMODE_VERC)
        {
            addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->WBC_DHD0_VCCOEFAD.u32));
            WBC_DHD0_VCCOEFAD.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            *pu32Addr = WBC_DHD0_VCCOEFAD.bits.coef_addr;
        }
        else
        {
            HAL_PRINT("Error coef found in %s: L%d\n",__FUNCTION__, __LINE__);
            return HI_FALSE;
        }
    }
    else
    {
        HAL_PRINT("Error layer id found in %s: L%d\n",__FUNCTION__, __LINE__);
        return HI_FALSE;
    }

    return HI_FALSE;
}


/* Vou set coef or lut read address */
HI_BOOL HAL_LAYER_SetCoefAddr(HAL_DISP_LAYER_E enLayer,
                                   HAL_DISP_COEFMODE_E enMode,
                                   HI_U32 u32Addr)
{
    volatile U_V0_HLCOEFAD V0_HLCOEFAD;
    volatile U_V0_HCCOEFAD V0_HCCOEFAD;
    volatile U_V0_VLCOEFAD V0_VLCOEFAD;
    volatile U_V0_VCCOEFAD V0_VCCOEFAD;
    volatile U_WBC_DHD0_HLCOEFAD WBC_DHD0_HLCOEFAD;
    volatile U_WBC_DHD0_HCCOEFAD WBC_DHD0_HCCOEFAD;
    volatile U_WBC_DHD0_VLCOEFAD WBC_DHD0_VLCOEFAD;
    volatile U_WBC_DHD0_VCCOEFAD WBC_DHD0_VCCOEFAD;
    
    volatile  HI_U32 addr_REG;

    if(enLayer <= HAL_DISP_LAYER_VSD0)
    {
        if(enMode == HAL_DISP_COEFMODE_HORL)
        {
            addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->V0_HLCOEFAD.u32));
            V0_HLCOEFAD.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            V0_HLCOEFAD.bits.coef_addr = u32Addr;
            HAL_WriteReg((HI_U32*)addr_REG, V0_HLCOEFAD.u32); 
        }    
        else if(enMode == HAL_DISP_COEFMODE_HORC)
        {
            addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->V0_HCCOEFAD.u32));
            V0_HCCOEFAD.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            V0_HCCOEFAD.bits.coef_addr = u32Addr;
            HAL_WriteReg((HI_U32*)addr_REG, V0_HCCOEFAD.u32); 
        }  
        else if(enMode == HAL_DISP_COEFMODE_VERL)
        {
            addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->V0_VLCOEFAD.u32));
            V0_VLCOEFAD.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            V0_VLCOEFAD.bits.coef_addr = u32Addr;
            HAL_WriteReg((HI_U32*)addr_REG, V0_VLCOEFAD.u32); 
        }
        else if(enMode == HAL_DISP_COEFMODE_VERC)
        {
            addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->V0_VCCOEFAD.u32));
            V0_VCCOEFAD.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            V0_VCCOEFAD.bits.coef_addr = u32Addr;
            HAL_WriteReg((HI_U32*)addr_REG, V0_VCCOEFAD.u32); 
        }       
        else
        {
            HAL_PRINT("Error coef found in %s: L%d\n",__FUNCTION__, __LINE__);
            return HI_FALSE;
        }
    }
    
    else if(enLayer == HAL_DISP_LAYER_WBC_D)
    {
        if(enMode == HAL_DISP_COEFMODE_HORL)
        {
            addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->WBC_DHD0_HLCOEFAD.u32));
            WBC_DHD0_HLCOEFAD.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            WBC_DHD0_HLCOEFAD.bits.coef_addr = u32Addr;
            HAL_WriteReg((HI_U32*)addr_REG, WBC_DHD0_HLCOEFAD.u32); 
        }    
        else if(enMode == HAL_DISP_COEFMODE_HORC)
        {
            addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->WBC_DHD0_HCCOEFAD.u32));
            WBC_DHD0_HCCOEFAD.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            WBC_DHD0_HCCOEFAD.bits.coef_addr = u32Addr;
            HAL_WriteReg((HI_U32*)addr_REG, WBC_DHD0_HCCOEFAD.u32); 
        }  
        else if(enMode == HAL_DISP_COEFMODE_VERL)
        {
            addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->WBC_DHD0_VLCOEFAD.u32));
            WBC_DHD0_VLCOEFAD.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            WBC_DHD0_VLCOEFAD.bits.coef_addr = u32Addr;
            HAL_WriteReg((HI_U32*)addr_REG, WBC_DHD0_VLCOEFAD.u32); 
        }
        else if(enMode == HAL_DISP_COEFMODE_VERC)
        {
            addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->WBC_DHD0_VCCOEFAD.u32));
            WBC_DHD0_VCCOEFAD.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            WBC_DHD0_VCCOEFAD.bits.coef_addr = u32Addr;
            HAL_WriteReg((HI_U32*)addr_REG, WBC_DHD0_VCCOEFAD.u32); 
        }
        else
        {
            HAL_PRINT("Error coef found in %s: L%d\n",__FUNCTION__, __LINE__);
            return HI_FALSE;
        }
    }
    else
    {
        HAL_PRINT("Error layer id found in %s: L%d\n",__FUNCTION__, __LINE__);
        return HI_FALSE;
    }

    return HI_FALSE;
}


/* Vou 设置视频层的参数更新 update */
HI_BOOL HAL_LAYER_SetLayerParaUpd(HAL_DISP_LAYER_E enLayer,
                                  HAL_DISP_COEFMODE_E enMode)
{
    U_V0_PARAUP V0_PARAUP;
    U_WBC_DHD0_PARAUP WBC_DHD0_PARAUP;
    volatile  HI_U32 addr_REG;

    if(enLayer <= HAL_DISP_LAYER_VSD0)
    {
        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->V0_PARAUP.u32));        
        V0_PARAUP.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        if(enMode == HAL_DISP_COEFMODE_HORL || enMode == HAL_DISP_COEFMODE_HORC || enMode == HAL_DISP_COEFMODE_ALL)
        {
            V0_PARAUP.bits.v0_hlcoef_upd = 0x1;            
            V0_PARAUP.bits.v0_hccoef_upd = 0x1;
        }

        if(enMode == HAL_DISP_COEFMODE_VERL || enMode == HAL_DISP_COEFMODE_VERC || enMode == HAL_DISP_COEFMODE_ALL)
        {
            V0_PARAUP.bits.v0_vlcoef_upd = 0x1;            
            V0_PARAUP.bits.v0_vccoef_upd = 0x1;
        }        
        HAL_WriteReg((HI_U32*)addr_REG, V0_PARAUP.u32); 
    }   
    else if(enLayer == HAL_DISP_LAYER_WBC_D)
    {
        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->WBC_DHD0_PARAUP.u32));        
        WBC_DHD0_PARAUP.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        if(enMode == HAL_DISP_COEFMODE_HORL || enMode == HAL_DISP_COEFMODE_HORC || enMode == HAL_DISP_COEFMODE_ALL)
        {
            WBC_DHD0_PARAUP.bits.wbc_hlcoef_upd = 0x1;            
            WBC_DHD0_PARAUP.bits.wbc_hccoef_upd = 0x1;
        }

        if(enMode == HAL_DISP_COEFMODE_VERL || enMode == HAL_DISP_COEFMODE_VERC || enMode == HAL_DISP_COEFMODE_ALL)
        {
            WBC_DHD0_PARAUP.bits.wbc_vlcoef_upd = 0x1;            
            WBC_DHD0_PARAUP.bits.wbc_vccoef_upd = 0x1;
        }        
        HAL_WriteReg((HI_U32*)addr_REG, WBC_DHD0_PARAUP.u32); 
    }
    else
    {
        HAL_PRINT("Error, Wrong coef update layer select\n");
        return HI_FALSE;
    }   

    return HI_TRUE;
}      

/* Vou zoom enable */
HI_BOOL HAL_LAYER_SetZmeEnable(HAL_DISP_LAYER_E enLayer,
                                    HAL_DISP_ZMEMODE_E enMode,
                                    HI_U32 bEnable)
{
    volatile U_V0_HSP V0_HSP;
    volatile U_V0_VSP V0_VSP;
    //U_G0HSP G0HSP;
   // U_G0VSP G0VSP;
    volatile U_WBC_DHD0_ZME_HSP WBC_DHD0_ZME_HSP;
    volatile U_WBC_DHD0_ZME_VSP WBC_DHD0_ZME_VSP;

    volatile  HI_U32 addr_REG;

    if(enLayer <= HAL_DISP_LAYER_VSD0)
    {
        /* layer zoom enable */
        if((enMode == HAL_DISP_ZMEMODE_HORL)||(enMode == HAL_DISP_ZMEMODE_HOR)||(enMode == HAL_DISP_ZMEMODE_ALL))
        {
            addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->V0_HSP.u32));
            V0_HSP.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            V0_HSP.bits.hlmsc_en = bEnable;            
            V0_HSP.bits.hlfir_en = 1;
            HAL_WriteReg((HI_U32*)addr_REG, V0_HSP.u32); 
        }

        if((enMode == HAL_DISP_ZMEMODE_HORC)||(enMode == HAL_DISP_ZMEMODE_HOR)||(enMode == HAL_DISP_ZMEMODE_ALL))
        {
            addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->V0_HSP.u32));
            V0_HSP.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            V0_HSP.bits.hchmsc_en = bEnable;
            V0_HSP.bits.hchfir_en = 1;
            HAL_WriteReg((HI_U32*)addr_REG, V0_HSP.u32);
        }

        if((enMode == HAL_DISP_ZMEMODE_VERL)||(enMode == HAL_DISP_ZMEMODE_VER)||(enMode == HAL_DISP_ZMEMODE_ALL))
        {
            addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->V0_VSP.u32));
            V0_VSP.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            V0_VSP.bits.vlmsc_en = bEnable;
            V0_VSP.bits.vlfir_en = 1;
            HAL_WriteReg((HI_U32*)addr_REG, V0_VSP.u32); 
        }

        if((enMode == HAL_DISP_ZMEMODE_VERC)||(enMode == HAL_DISP_ZMEMODE_VER)||(enMode == HAL_DISP_ZMEMODE_ALL))
        {
            addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->V0_VSP.u32));
            V0_VSP.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            V0_VSP.bits.vchmsc_en = bEnable;
            V0_VSP.bits.vchfir_en = 1;
            HAL_WriteReg((HI_U32*)addr_REG, V0_VSP.u32); 
        }
    }
    else if(enLayer == HAL_DISP_LAYER_WBC_D)
    {
        /* VSD layer zoom enable */
        if((enMode == HAL_DISP_ZMEMODE_HORL)||(enMode == HAL_DISP_ZMEMODE_HOR)||(enMode == HAL_DISP_ZMEMODE_ALL))
        {
            addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->WBC_DHD0_ZME_HSP.u32));
            WBC_DHD0_ZME_HSP.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            WBC_DHD0_ZME_HSP.bits.hlmsc_en = bEnable;
            WBC_DHD0_ZME_HSP.bits.hlfir_en = 1;
            HAL_WriteReg((HI_U32*)addr_REG, WBC_DHD0_ZME_HSP.u32); 
        }

        if((enMode == HAL_DISP_ZMEMODE_HORC)||(enMode == HAL_DISP_ZMEMODE_HOR)||(enMode == HAL_DISP_ZMEMODE_ALL))
        {
            addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->WBC_DHD0_ZME_HSP.u32));
            WBC_DHD0_ZME_HSP.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            WBC_DHD0_ZME_HSP.bits.hchmsc_en = bEnable;
            WBC_DHD0_ZME_HSP.bits.hchfir_en = 1;
            HAL_WriteReg((HI_U32*)addr_REG, WBC_DHD0_ZME_HSP.u32); 
        }

        if((enMode == HAL_DISP_ZMEMODE_VERL)||(enMode == HAL_DISP_ZMEMODE_VER)||(enMode == HAL_DISP_ZMEMODE_ALL))
        {
            addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->WBC_DHD0_ZME_VSP.u32));
            WBC_DHD0_ZME_VSP.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            WBC_DHD0_ZME_VSP.bits.vlmsc_en = bEnable;
            WBC_DHD0_ZME_VSP.bits.vlfir_en = 1;
            HAL_WriteReg((HI_U32*)addr_REG, WBC_DHD0_ZME_VSP.u32); 
        }

        if((enMode == HAL_DISP_ZMEMODE_VERC)||(enMode == HAL_DISP_ZMEMODE_VER)||(enMode == HAL_DISP_ZMEMODE_ALL))
        {
            addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->WBC_DHD0_ZME_VSP.u32));
            WBC_DHD0_ZME_VSP.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            WBC_DHD0_ZME_VSP.bits.vchmsc_en = bEnable;
            WBC_DHD0_ZME_VSP.bits.vchfir_en = 1;
            HAL_WriteReg((HI_U32*)addr_REG, WBC_DHD0_ZME_VSP.u32); 
        }
    }
    else
    {
        HAL_PRINT("Error layer id found in %s: L%d\n",__FUNCTION__, __LINE__);
        return HI_FALSE;
    }
    return HI_TRUE;
}

HI_BOOL HAL_LAYER_SetZmeFirEnable(HAL_DISP_LAYER_E enLayer, HAL_DISP_ZMEMODE_E enMode, HI_U32 bEnable)
{
    U_V0_HSP V0_HSP;    
    U_V0_VSP V0_VSP;
    U_WBC_DHD0_ZME_HSP WBC_DHD0_ZME_HSP;
    U_WBC_DHD0_ZME_VSP WBC_DHD0_ZME_VSP;

    volatile  HI_U32 addr_REG;
    if(enLayer <= HAL_DISP_LAYER_VSD0)
    {
        /* layer zoom enable */
        if((enMode == HAL_DISP_ZMEMODE_HORL)||(enMode == HAL_DISP_ZMEMODE_HOR)||(enMode == HAL_DISP_ZMEMODE_ALL))
        {
            addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->V0_HSP.u32));
            V0_HSP.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            V0_HSP.bits.hlfir_en = bEnable;
            HAL_WriteReg((HI_U32*)addr_REG, V0_HSP.u32); 
        }

        if((enMode == HAL_DISP_ZMEMODE_HORC)||(enMode == HAL_DISP_ZMEMODE_HOR)||(enMode == HAL_DISP_ZMEMODE_ALL))
        {
            addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->V0_HSP.u32));
            V0_HSP.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            V0_HSP.bits.hchfir_en = bEnable;
            HAL_WriteReg((HI_U32*)addr_REG, V0_HSP.u32); 
        }

        if((enMode == HAL_DISP_ZMEMODE_VERL)||(enMode == HAL_DISP_ZMEMODE_VER)||(enMode == HAL_DISP_ZMEMODE_ALL))
        {
            addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->V0_VSP.u32));
            V0_VSP.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            V0_VSP.bits.vlfir_en = bEnable;
            HAL_WriteReg((HI_U32*)addr_REG, V0_VSP.u32); 
        }

        if((enMode == HAL_DISP_ZMEMODE_VERC)||(enMode == HAL_DISP_ZMEMODE_VER)||(enMode == HAL_DISP_ZMEMODE_ALL))
        {
            addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->V0_VSP.u32));
            V0_VSP.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            V0_VSP.bits.vchfir_en = bEnable;
            HAL_WriteReg((HI_U32*)addr_REG, V0_VSP.u32); 
        }
    }    
    else if(enLayer == HAL_DISP_LAYER_WBC_D)
    {
        /* VSD layer zoom enable */
        if((enMode == HAL_DISP_ZMEMODE_HORL)||(enMode == HAL_DISP_ZMEMODE_HOR)||(enMode == HAL_DISP_ZMEMODE_ALL))
        {
            addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->WBC_DHD0_ZME_HSP.u32));
            WBC_DHD0_ZME_HSP.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            WBC_DHD0_ZME_HSP.bits.hlfir_en = bEnable;
            HAL_WriteReg((HI_U32*)addr_REG, WBC_DHD0_ZME_HSP.u32); 
        }

        if((enMode == HAL_DISP_ZMEMODE_HORC)||(enMode == HAL_DISP_ZMEMODE_HOR)||(enMode == HAL_DISP_ZMEMODE_ALL))
        {
            addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->WBC_DHD0_ZME_HSP.u32));
            WBC_DHD0_ZME_HSP.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            WBC_DHD0_ZME_HSP.bits.hchfir_en = bEnable;
            HAL_WriteReg((HI_U32*)addr_REG, WBC_DHD0_ZME_HSP.u32); 
        }

        if((enMode == HAL_DISP_ZMEMODE_VERL)||(enMode == HAL_DISP_ZMEMODE_VER)||(enMode == HAL_DISP_ZMEMODE_ALL))
        {
            addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->WBC_DHD0_ZME_VSP.u32));
            WBC_DHD0_ZME_VSP.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            WBC_DHD0_ZME_VSP.bits.vlfir_en = bEnable;
            HAL_WriteReg((HI_U32*)addr_REG, WBC_DHD0_ZME_VSP.u32); 
        }

        if((enMode == HAL_DISP_ZMEMODE_VERC)||(enMode == HAL_DISP_ZMEMODE_VER)||(enMode == HAL_DISP_ZMEMODE_ALL))
        {
            addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->WBC_DHD0_ZME_VSP.u32));
            WBC_DHD0_ZME_VSP.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            WBC_DHD0_ZME_VSP.bits.vchfir_en = bEnable;
            HAL_WriteReg((HI_U32*)addr_REG, WBC_DHD0_ZME_VSP.u32); 
        }
    }
    else
    {
        HAL_PRINT("Error layer id found in %s: L%d\n",__FUNCTION__, __LINE__);
        return HI_FALSE;
    }

    return HI_TRUE;
}

HI_BOOL HAL_LAYER_SetZmeMscEnable(HAL_DISP_LAYER_E enLayer, HAL_DISP_ZMEMODE_E enMode, HI_U32 bEnable)
{
    U_V0_HSP V0_HSP;    
    U_V0_VSP V0_VSP;
    U_WBC_DHD0_ZME_HSP WBC_DHD0_ZME_HSP;
    U_WBC_DHD0_ZME_VSP WBC_DHD0_ZME_VSP;

    volatile  HI_U32 addr_REG;
    if(enLayer <= HAL_DISP_LAYER_VSD0)
    {
        /* layer zoom enable */
        if((enMode == HAL_DISP_ZMEMODE_HORL)||(enMode == HAL_DISP_ZMEMODE_HOR)||(enMode == HAL_DISP_ZMEMODE_ALL))
        {
            addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->V0_HSP.u32));
            V0_HSP.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            V0_HSP.bits.hlmsc_en = bEnable;
            HAL_WriteReg((HI_U32*)addr_REG, V0_HSP.u32); 
        }

        if((enMode == HAL_DISP_ZMEMODE_HORC)||(enMode == HAL_DISP_ZMEMODE_HOR)||(enMode == HAL_DISP_ZMEMODE_ALL))
        {
            addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->V0_HSP.u32));
            V0_HSP.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            V0_HSP.bits.hchmsc_en = bEnable;
            HAL_WriteReg((HI_U32*)addr_REG, V0_HSP.u32); 
        }

        if((enMode == HAL_DISP_ZMEMODE_VERL)||(enMode == HAL_DISP_ZMEMODE_VER)||(enMode == HAL_DISP_ZMEMODE_ALL))
        {
            addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->V0_VSP.u32));
            V0_VSP.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            V0_VSP.bits.vlmsc_en = bEnable;
            HAL_WriteReg((HI_U32*)addr_REG, V0_VSP.u32); 
        }

        if((enMode == HAL_DISP_ZMEMODE_VERC)||(enMode == HAL_DISP_ZMEMODE_VER)||(enMode == HAL_DISP_ZMEMODE_ALL))
        {
            addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->V0_VSP.u32));
            V0_VSP.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            V0_VSP.bits.vchmsc_en = bEnable;
            HAL_WriteReg((HI_U32*)addr_REG, V0_VSP.u32); 
        }
    }  
    else if(enLayer == HAL_DISP_LAYER_WBC_D)
    {
        /* VSD layer zoom enable */
        if((enMode == HAL_DISP_ZMEMODE_HORL)||(enMode == HAL_DISP_ZMEMODE_HOR)||(enMode == HAL_DISP_ZMEMODE_ALL))
        {
            addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->WBC_DHD0_ZME_HSP.u32));
            WBC_DHD0_ZME_HSP.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            WBC_DHD0_ZME_HSP.bits.hlmsc_en = bEnable;
            HAL_WriteReg((HI_U32*)addr_REG, WBC_DHD0_ZME_HSP.u32); 
        }

        if((enMode == HAL_DISP_ZMEMODE_HORC)||(enMode == HAL_DISP_ZMEMODE_HOR)||(enMode == HAL_DISP_ZMEMODE_ALL))
        {
            addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->WBC_DHD0_ZME_HSP.u32));
            WBC_DHD0_ZME_HSP.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            WBC_DHD0_ZME_HSP.bits.hchmsc_en = bEnable;
            HAL_WriteReg((HI_U32*)addr_REG, WBC_DHD0_ZME_HSP.u32); 
        }

        if((enMode == HAL_DISP_ZMEMODE_VERL)||(enMode == HAL_DISP_ZMEMODE_VER)||(enMode == HAL_DISP_ZMEMODE_ALL))
        {
            addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->WBC_DHD0_ZME_VSP.u32));
            WBC_DHD0_ZME_VSP.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            WBC_DHD0_ZME_VSP.bits.vlmsc_en = bEnable;
            HAL_WriteReg((HI_U32*)addr_REG, WBC_DHD0_ZME_VSP.u32); 
        }

        if((enMode == HAL_DISP_ZMEMODE_VERC)||(enMode == HAL_DISP_ZMEMODE_VER)||(enMode == HAL_DISP_ZMEMODE_ALL))
        {
            addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->WBC_DHD0_ZME_VSP.u32));
            WBC_DHD0_ZME_VSP.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            WBC_DHD0_ZME_VSP.bits.vchmsc_en = bEnable;
            HAL_WriteReg((HI_U32*)addr_REG, WBC_DHD0_ZME_VSP.u32); 
        }
    }
    else
    {
        HAL_PRINT("Error layer id found in %s: L%d\n",__FUNCTION__, __LINE__);
        return HI_FALSE;
    }

    return HI_TRUE;
}

HI_BOOL HAL_LAYER_SetHfirOrder(HAL_DISP_LAYER_E enLayer, HI_U32 uHfirOrder)
{
    U_V0_HSP V0_HSP;    
    U_WBC_DHD0_ZME_HSP WBC_DHD0_ZME_HSP;
    volatile  HI_U32 addr_REG;
    if(enLayer <= HAL_DISP_LAYER_VSD0)
    {
        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->V0_HSP.u32));
        V0_HSP.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        V0_HSP.bits.hfir_order = uHfirOrder;
        HAL_WriteReg((HI_U32*)addr_REG, V0_HSP.u32); 
    }
    else if(enLayer == HAL_DISP_LAYER_WBC_D)
    {
        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->WBC_DHD0_ZME_HSP.u32));
        WBC_DHD0_ZME_HSP.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        WBC_DHD0_ZME_HSP.bits.hfir_order = uHfirOrder;
        HAL_WriteReg((HI_U32*)addr_REG, WBC_DHD0_ZME_HSP.u32); 
    }
    else
    {
        HAL_PRINT("Error layer id found in %s: L%d\n",__FUNCTION__, __LINE__);
        return HI_FALSE;
    }

    return HI_TRUE;
}


HI_BOOL HAL_LAYER_SetZmeVerTap(HAL_DISP_LAYER_E enLayer, HAL_DISP_ZMEMODE_E enMode, HI_U32 uVerTap)
{
    U_V0_VSP V0_VSP;    
    U_WBC_DHD0_ZME_VSP WBC_DHD0_ZME_VSP;
    volatile HI_U32 addr_REG;

    if (  (enMode == HAL_DISP_ZMEMODE_HORL) || (enMode == HAL_DISP_ZMEMODE_HOR) \
       || (enMode == HAL_DISP_ZMEMODE_HORC))
    {
        HAL_PRINT("Error! HAL_SetZmeVerTap enMode is error!\n");
    }


    if(enLayer <= HAL_DISP_LAYER_VSD0)
    {        
        if ((enMode == HAL_DISP_ZMEMODE_VERC) || (enMode == HAL_DISP_ZMEMODE_VER) || (enMode == HAL_DISP_ZMEMODE_ALL))
        {
            addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->V0_VSP.u32));

            V0_VSP.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            V0_VSP.bits.vsc_chroma_tap = uVerTap;
            HAL_WriteReg((HI_U32*)addr_REG, V0_VSP.u32); 
        }
    }
    else if(enLayer == HAL_DISP_LAYER_WBC_D)
    {       
        if ((enMode == HAL_DISP_ZMEMODE_VERC) || (enMode == HAL_DISP_ZMEMODE_VER) || (enMode == HAL_DISP_ZMEMODE_ALL))
        {
            addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->WBC_DHD0_ZME_VSP.u32));
            WBC_DHD0_ZME_VSP.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            WBC_DHD0_ZME_VSP.bits.vsc_chroma_tap = uVerTap;
            HAL_WriteReg((HI_U32*)addr_REG, WBC_DHD0_ZME_VSP.u32); 
        }
    }
    else
    {
        HAL_PRINT("Error layer id found in %s: L%d\n",__FUNCTION__, __LINE__);
        return HI_FALSE;
    }

    return HI_TRUE;
}

/*缩放的输入数据格式 */
HI_BOOL HAL_LAYER_SetZmeVerType(HAL_DISP_LAYER_E enLayer, HI_U32 uVerType)
{
    U_V0_VSP V0_VSP;    
    U_WBC_DHD0_ZME_VSP WBC_DHD0_ZME_VSP;

    volatile  HI_U32 addr_REG;
    if(enLayer <= HAL_DISP_LAYER_VSD0)
    {
        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->V0_VSP.u32));
        V0_VSP.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        V0_VSP.bits.zme_in_fmt = uVerType;
        HAL_WriteReg((HI_U32*)addr_REG, V0_VSP.u32); 
    }
    else if(enLayer == HAL_DISP_LAYER_WBC_D)
    {
        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->WBC_DHD0_ZME_VSP.u32));
        WBC_DHD0_ZME_VSP.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        WBC_DHD0_ZME_VSP.bits.zme_in_fmt = uVerType;
        HAL_WriteReg((HI_U32*)addr_REG, WBC_DHD0_ZME_VSP.u32); 
    }

    return HI_TRUE;
}

/* Vou set zoom inital phase */
HI_BOOL  HAL_LAYER_SetZmePhase(HAL_DISP_LAYER_E enLayer,
                                   HAL_DISP_ZMEMODE_E enMode,
                                   HI_S32 s32Phase)
{                                    
    U_V0_HLOFFSET V0_HLOFFSET;
    U_V0_HCOFFSET V0_HCOFFSET;
    U_V0_VOFFSET V0_VOFFSET;
    //U_G0HOFFSET G0HOFFSET;
    //U_G0VOFFSET G0VOFFSET;
    U_WBC_DHD0_ZME_HLOFFSET WBC_DHD0_ZME_HLOFFSET;
    U_WBC_DHD0_ZME_HCOFFSET WBC_DHD0_ZME_HCOFFSET;
    U_WBC_DHD0_ZME_VOFFSET  WBC_DHD0_ZME_VOFFSET;

    volatile  HI_U32 addr_REG;

    if(enLayer <= HAL_DISP_LAYER_VSD0)
    {
        /* layer zoom enable */
        if((enMode == HAL_DISP_ZMEMODE_HORL) || (enMode == HAL_DISP_ZMEMODE_HOR)||(enMode == HAL_DISP_ZMEMODE_ALL))
        {
            addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->V0_HLOFFSET.u32));
            V0_HLOFFSET.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            V0_HLOFFSET.bits.hor_loffset = s32Phase;
            HAL_WriteReg((HI_U32*)addr_REG, V0_HLOFFSET.u32); 
        }

        if((enMode == HAL_DISP_ZMEMODE_HORC) || (enMode == HAL_DISP_ZMEMODE_HOR)||(enMode == HAL_DISP_ZMEMODE_ALL))
        {
            addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->V0_HCOFFSET.u32));
            V0_HCOFFSET.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            V0_HCOFFSET.bits.hor_coffset = s32Phase;
            HAL_WriteReg((HI_U32*)addr_REG, V0_HCOFFSET.u32); 
        }

        if((enMode == HAL_DISP_ZMEMODE_VERL)||(enMode == HAL_DISP_ZMEMODE_VER)||(enMode == HAL_DISP_ZMEMODE_ALL))
        {
            addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->V0_VOFFSET.u32));
            V0_VOFFSET.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            V0_VOFFSET.bits.vluma_offset = s32Phase;
            HAL_WriteReg((HI_U32*)addr_REG, V0_VOFFSET.u32); 
        }

        if((enMode == HAL_DISP_ZMEMODE_VERC)||(enMode == HAL_DISP_ZMEMODE_VER)||(enMode == HAL_DISP_ZMEMODE_ALL))
        {
            addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->V0_VOFFSET.u32));
            V0_VOFFSET.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            V0_VOFFSET.bits.vchroma_offset = s32Phase;
            HAL_WriteReg((HI_U32*)addr_REG, V0_VOFFSET.u32); 
        }
    }   
    else if(enLayer == HAL_DISP_LAYER_WBC_D)
    {
        if((enMode == HAL_DISP_ZMEMODE_HORL) || (enMode == HAL_DISP_ZMEMODE_HOR)||(enMode == HAL_DISP_ZMEMODE_ALL))
        {
            addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->WBC_DHD0_ZME_HLOFFSET.u32));
            WBC_DHD0_ZME_HLOFFSET.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            WBC_DHD0_ZME_HLOFFSET.bits.hor_loffset = s32Phase;
            HAL_WriteReg((HI_U32*)addr_REG, WBC_DHD0_ZME_HLOFFSET.u32); 
        }

        if((enMode == HAL_DISP_ZMEMODE_HORC) || (enMode == HAL_DISP_ZMEMODE_HOR)||(enMode == HAL_DISP_ZMEMODE_ALL))
        {
            addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->WBC_DHD0_ZME_HCOFFSET.u32));
            WBC_DHD0_ZME_HCOFFSET.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            WBC_DHD0_ZME_HCOFFSET.bits.hor_coffset = s32Phase;
            HAL_WriteReg((HI_U32*)addr_REG, WBC_DHD0_ZME_HCOFFSET.u32); 
        }

        if((enMode == HAL_DISP_ZMEMODE_VERL)||(enMode == HAL_DISP_ZMEMODE_VER)||(enMode == HAL_DISP_ZMEMODE_ALL))
        {
            addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->WBC_DHD0_ZME_VOFFSET.u32));
            WBC_DHD0_ZME_VOFFSET.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            WBC_DHD0_ZME_VOFFSET.bits.vluma_offset = s32Phase;
            HAL_WriteReg((HI_U32*)addr_REG, WBC_DHD0_ZME_VOFFSET.u32); 
        }

        if((enMode == HAL_DISP_ZMEMODE_VERC)||(enMode == HAL_DISP_ZMEMODE_VER)||(enMode == HAL_DISP_ZMEMODE_ALL))
        {
            addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->WBC_DHD0_ZME_VOFFSET.u32));
            WBC_DHD0_ZME_VOFFSET.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            WBC_DHD0_ZME_VOFFSET.bits.vchroma_offset = s32Phase;
            HAL_WriteReg((HI_U32*)addr_REG, WBC_DHD0_ZME_VOFFSET.u32); 
        }
    }
    else
    {
        HAL_PRINT("Error layer id found in %s: L%d\n",__FUNCTION__, __LINE__);
        return HI_FALSE;
    }

    return HI_TRUE;
}


HI_BOOL HAL_LAYER_SetVerRatio(HAL_DISP_LAYER_E enLayer, HI_U32 uRatio)
{
    volatile U_V0_VSR V0_VSR;
    //volatile U_G0VSR G0VSR;
    volatile U_WBC_DHD0_ZME_VSR WBC_DHD0_ZME_VSR;

    volatile HI_U32 addr_REG;
    if(enLayer <= HAL_DISP_LAYER_VSD0)
    {
        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->V0_VSR.u32));
        V0_VSR.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        V0_VSR.bits.vratio = uRatio;
        HAL_WriteReg((HI_U32*)addr_REG, V0_VSR.u32); 
    }
    else if(enLayer == HAL_DISP_LAYER_WBC_D)
    {
        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->WBC_DHD0_ZME_VSR.u32));
        WBC_DHD0_ZME_VSR.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        WBC_DHD0_ZME_VSR.bits.vratio = uRatio;
        HAL_WriteReg((HI_U32*)addr_REG, WBC_DHD0_ZME_VSR.u32); 
    }
    else
    {
        HAL_PRINT("Error layer id found in %s: L%d\n",__FUNCTION__, __LINE__);
        return HI_FALSE;
    }
    
    return HI_TRUE;
}

HI_BOOL HAL_LAYER_SetHorRatio(HAL_DISP_LAYER_E enLayer, HI_U32 uRatio)
{
    volatile U_V0_HSP V0_HSP;
    //volatile U_G0HSP G0HSP;
    volatile U_WBC_DHD0_ZME_HSP WBC_DHD0_ZME_HSP;

    volatile HI_U32 addr_REG;
    if(enLayer <= HAL_DISP_LAYER_VSD0)
    {
        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->V0_HSP.u32));
        V0_HSP.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        V0_HSP.bits.hratio = uRatio;
        HAL_WriteReg((HI_U32*)addr_REG, V0_HSP.u32); 
    }
    else if(enLayer == HAL_DISP_LAYER_WBC_D)
    {
        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->WBC_DHD0_ZME_HSP.u32));
        WBC_DHD0_ZME_HSP.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        WBC_DHD0_ZME_HSP.bits.hratio = uRatio;
        HAL_WriteReg((HI_U32*)addr_REG, WBC_DHD0_ZME_HSP.u32); 
    }
    else
    {
        HAL_PRINT("Error layer id found in %s: L%d\n",__FUNCTION__, __LINE__);
        return HI_FALSE;
    }
    
    return HI_TRUE;
}

HI_BOOL  HAL_LAYER_SetRegUp(HAL_DISP_LAYER_E enLayer)
{
    U_V0_UPD V0_UPD;
    U_G0_UPD G0_UPD;
    U_WBC_DHD0_UPD WBC_DHD0_UPD;    

    volatile HI_U32 addr_REG;
    if(enLayer <= HAL_DISP_LAYER_VSD0)
    {
        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->V0_UPD.u32));
        V0_UPD.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        /* layer register update */
        V0_UPD.bits.regup = 0x1;
        HAL_WriteReg((HI_U32*)addr_REG, V0_UPD.u32); 
    }
    else if((LAYER_GFX_START <= enLayer) && (enLayer <= LAYER_GFX_END))
    {
        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->G0_UPD.u32));
        G0_UPD.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        /* layer register update */
        G0_UPD.bits.regup = 0x1;
        HAL_WriteReg((HI_U32*)addr_REG, G0_UPD.u32); 
    }
    else if(enLayer == HAL_DISP_LAYER_WBC_D)
    {
        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->WBC_DHD0_UPD.u32));
        WBC_DHD0_UPD.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        /* G1 layer register update */
        WBC_DHD0_UPD.bits.regup = 0x1;
        HAL_WriteReg((HI_U32*)addr_REG, WBC_DHD0_UPD.u32); 
    }
    else
    {
        HAL_PRINT("Error layer id found in %s: L%d\n",__FUNCTION__, __LINE__);
        return HI_FALSE;
    }

    return HI_TRUE;
}

/*配置图形层源地址*/
HI_BOOL HAL_GRAPHIC_SetGfxAddr(HAL_DISP_LAYER_E enLayer, HI_U32 u32LAddr)
{
    U_G0_ADDR G0_ADDR;
    volatile  HI_U32 addr_REG;
    //printk("Vou_SetGfxAddr %d\n",enLayer);

    if((LAYER_GFX_START <= enLayer)&&(enLayer <= LAYER_GFX_END))
    {
        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->G0_ADDR.u32));
        G0_ADDR.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        G0_ADDR.u32 = u32LAddr;
        HAL_WriteReg((HI_U32*)addr_REG, G0_ADDR.u32); 

    }
    else
    {
        HAL_PRINT("Error layer id found in %s: L%d\n",__FUNCTION__, __LINE__);
        return HI_FALSE;
    }
    
    return HI_TRUE;
}

/*查询图形层源地址*/
HI_BOOL HAL_GRAPHIC_GetGfxAddr(HAL_DISP_LAYER_E enLayer, HI_U32 *pu32GfxAddr)
{
    volatile  HI_U32 addr_REG;
    if((LAYER_GFX_START <= enLayer)&&(enLayer <= LAYER_GFX_END))
    {
        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->G0_ADDR.u32));
    }
    else
    {
        HAL_PRINT("Error layer id found in %s: L%d\n",__FUNCTION__, __LINE__);
        return HI_FALSE;
    }

    *pu32GfxAddr = HAL_ReadReg((HI_U32*)addr_REG);
    return HI_TRUE;
}

/*配置图形层stride*/
HI_BOOL HAL_GRAPHIC_SetGfxStride(HAL_DISP_LAYER_E enLayer, HI_U16 u16pitch)
{
    volatile U_G0_STRIDE G0_STRIDE;
    volatile  HI_U32 addr_REG;
    if((LAYER_GFX_START <= enLayer)&&(enLayer <= LAYER_GFX_END))
    {
        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->G0_STRIDE.u32));
        G0_STRIDE.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        G0_STRIDE.bits.surface_stride = u16pitch;
        HAL_WriteReg((HI_U32*)addr_REG, G0_STRIDE.u32); 
    }
    else
    {
        HAL_PRINT("Error layer id found in %s: L%d\n",__FUNCTION__, __LINE__);
        return HI_FALSE;
    }
    return HI_TRUE;
}

/*查询图形层stride*/
HI_BOOL HAL_GRAPHIC_GetGfxStride(HAL_DISP_LAYER_E enLayer, HI_U32 *pu32GfxStride)
{
    volatile  HI_U32 addr_REG;
    if((LAYER_GFX_START <= enLayer)&&(enLayer <= LAYER_GFX_END))
    {
        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->G0_STRIDE.u32));
    }
    else
    {
        HAL_PRINT("Error layer id found in %s: L%d\n",__FUNCTION__, __LINE__);
        return HI_FALSE;
    }

    *pu32GfxStride = HAL_ReadReg((HI_U32*)addr_REG);
    return HI_TRUE;
}
// add by h00180450
HI_BOOL HAL_GRAPHIC_GetLayerInRect(HAL_DISP_LAYER_E enLayer, HI_U32 *u32Width, HI_U32 *u32Height)
{
    U_G0_IRESO G0_IRESO;
    volatile  HI_U32 addr_REG;

    if((LAYER_GFX_START <= enLayer)&&(enLayer <= LAYER_GFX_END))
    {
        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->G0_IRESO.u32));
        G0_IRESO.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        *u32Width = G0_IRESO.bits.iw + 1;
        *u32Height = G0_IRESO.bits.ih + 1;
    }
    else
    {
        HAL_PRINT("Error layer id found in %s: L%d\n",__FUNCTION__, __LINE__);
        return HI_FALSE;
    }

    return HI_TRUE;
}
/*配置图形层数据扩展方式*/
HI_BOOL HAL_GRAPHIC_SetGfxExt(HAL_DISP_LAYER_E enLayer,
                                HAL_GFX_BITEXTEND_E enMode)
{
    U_G0_CTRL G0_CTRL;

    volatile  HI_U32 addr_REG;
    if((LAYER_GFX_START <= enLayer)&&(enLayer <= LAYER_GFX_END))
    {
        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->G0_CTRL.u32));
        G0_CTRL.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        G0_CTRL.bits.bitext = enMode;
        HAL_WriteReg((HI_U32*)addr_REG, G0_CTRL.u32); 

    }
    else
    {
        HAL_PRINT("Error layer id found in %s: L%d\n",__FUNCTION__, __LINE__);
        return HI_FALSE;
    }    

    return HI_TRUE;    
}

HI_BOOL HAL_GRAPHIC_SetGfxPreMult(HAL_DISP_LAYER_E enLayer, HI_U32 bEnable)
{
    U_G0_CBMPARA G0_CBMPARA;
    volatile  HI_U32 addr_REG;

    if((LAYER_GFX_START <= enLayer)&&(enLayer <= LAYER_GFX_END))
    {
        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->G0_CBMPARA.u32));
        G0_CBMPARA.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        G0_CBMPARA.bits.premult_en = bEnable;
        HAL_WriteReg((HI_U32*)addr_REG, G0_CBMPARA.u32); 
    }
    else
    {
        HAL_PRINT("Error layer id found in %s: L%d\n",__FUNCTION__, __LINE__);
        return HI_FALSE;
    }

    return HI_TRUE;    
}

HI_BOOL HAL_GRAPHIC_GetGfxPreMult(HAL_DISP_LAYER_E enLayer, HI_U32 *pbEnable)
{
    U_G0_CBMPARA G0_CBMPARA;
    volatile  HI_U32 addr_REG;

    if((LAYER_GFX_START <= enLayer)&&(enLayer <= LAYER_GFX_END))
    {
        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->G0_CBMPARA.u32));
        G0_CBMPARA.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        *pbEnable = G0_CBMPARA.bits.premult_en;
    }
    else
    {
        HAL_PRINT("Error layer id found in %s: L%d\n",__FUNCTION__, __LINE__);
        return HI_FALSE;
    }

    return HI_TRUE;    
}

HI_BOOL HAL_GRAPHIC_SetGfxPalpha(HAL_DISP_LAYER_E enLayer,
                                   HI_U32 bAlphaEn,HI_U32 bArange,
                                   HI_U8 u8Alpha0, HI_U8 u8Alpha1)
{
    U_G0_CBMPARA G0_CBMPARA;
    U_G0_CKEYMIN G0_CKEYMIN;
    U_G0_CKEYMAX G0_CKEYMAX;

    volatile HI_U32 addr_REG;
    if((LAYER_GFX_START <= enLayer)&&(enLayer <= LAYER_GFX_END))
    {
        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->G0_CBMPARA.u32));
        G0_CBMPARA.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        G0_CBMPARA.bits.palpha_en = bAlphaEn; 
        HAL_WriteReg((HI_U32*)addr_REG, G0_CBMPARA.u32); 

        if(HI_TRUE == bAlphaEn)
        {
	        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->G0_CKEYMIN.u32));
	        G0_CKEYMIN.u32 = HAL_ReadReg((HI_U32*)addr_REG);
	        G0_CKEYMIN.bits.va1 = u8Alpha1;
	        HAL_WriteReg((HI_U32*)addr_REG, G0_CKEYMIN.u32); 

	        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->G0_CKEYMAX.u32));
	        G0_CKEYMAX.u32 = HAL_ReadReg((HI_U32*)addr_REG);
	        G0_CKEYMAX.bits.va0 = u8Alpha0;                            
	        HAL_WriteReg((HI_U32*)addr_REG, G0_CKEYMAX.u32); 
        }
		else
		{
	        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->G0_CKEYMIN.u32));
	        G0_CKEYMIN.u32 = HAL_ReadReg((HI_U32*)addr_REG);
	        G0_CKEYMIN.bits.va1 = 0xff;
	        HAL_WriteReg((HI_U32*)addr_REG, G0_CKEYMIN.u32); 

	        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->G0_CKEYMAX.u32));
	        G0_CKEYMAX.u32 = HAL_ReadReg((HI_U32*)addr_REG);
	        G0_CKEYMAX.bits.va0 = 0xff;                            
	        HAL_WriteReg((HI_U32*)addr_REG, G0_CKEYMAX.u32); 
        }	
    }
    else
    {
        HAL_PRINT("Error layer id found in %s: L%d\n",__FUNCTION__, __LINE__);
        return HI_FALSE;
    }    

    return HI_TRUE;    
}

HI_BOOL HAL_GRAPHIC_SetGfxPalphaRange(HAL_DISP_LAYER_E enLayer, HI_U32 bArange)
{
    U_G0_CBMPARA G0_CBMPARA;

    volatile  HI_U32 addr_REG;
    if((LAYER_GFX_START <= enLayer)&&(enLayer <= LAYER_GFX_END))
    {
        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->G0_CBMPARA.u32));
        G0_CBMPARA.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        G0_CBMPARA.bits.palpha_range = bArange;            
        HAL_WriteReg((HI_U32*)addr_REG, G0_CBMPARA.u32); 
    }
    else
    {
        HAL_PRINT("Error layer id found in %s: L%d\n",__FUNCTION__, __LINE__);
        return HI_FALSE;
    }    

    return HI_TRUE;    
}

HI_BOOL HAL_GRAPHIC_SetGfxKeyEn(HAL_DISP_LAYER_E enLayer, HI_U32 u32KeyEnable)
{
    U_G0_CBMPARA G0_CBMPARA;

    volatile  HI_U32 addr_REG;
    if((HAL_DISP_LAYER_GFX0 <= enLayer)&&(enLayer <= HAL_DISP_LAYER_GFX4))
    {
        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->G0_CBMPARA.u32));
        G0_CBMPARA.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        G0_CBMPARA.bits.key_en = u32KeyEnable;
        HAL_WriteReg((HI_U32*)addr_REG, G0_CBMPARA.u32); 
    }
    else
    {
        HAL_PRINT("Error layer id %d not support colorkey in %s: L%d\n",
            (HI_S32)enLayer, __FUNCTION__, __LINE__);
        return HI_FALSE;
    }    

    return HI_TRUE;    
}

HI_BOOL HAL_GRAPHIC_SetGfxKeyMode(HAL_DISP_LAYER_E enLayer, HI_U32 u32KeyOut)
{
    U_G0_CBMPARA G0_CBMPARA;

    volatile  HI_U32 addr_REG;
    if((HAL_DISP_LAYER_GFX0 <= enLayer)&&(enLayer <= HAL_DISP_LAYER_GFX4))
    {
        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->G0_CBMPARA.u32));
        G0_CBMPARA.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        G0_CBMPARA.bits.key_mode = u32KeyOut;
        HAL_WriteReg((HI_U32*)addr_REG, G0_CBMPARA.u32); 
    }
    else
    {
        HAL_PRINT("Error layer id %d not support colorkey mode in %s: L%d\n",
            (HI_S32)enLayer, __FUNCTION__, __LINE__);
        return HI_FALSE;
    }    

    return HI_TRUE;    
}

HI_BOOL HAL_GRAPHIC_SetColorKeyValue(HAL_DISP_LAYER_E enLayer, 
        HAL_GFX_KEY_MAX_S stKeyMax, HAL_GFX_KEY_MIN_S stKeyMin)
{
    U_G0_CKEYMIN G0_CKEYMIN;
    U_G0_CKEYMAX G0_CKEYMAX;
    volatile  HI_U32 addr_REG;
    if((HAL_DISP_LAYER_GFX0 <= enLayer)&&(enLayer <= HAL_DISP_LAYER_GFX4))
    {
        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->G0_CKEYMAX.u32));
        G0_CKEYMAX.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        G0_CKEYMAX.bits.keyr_max = stKeyMax.u8KeyMax_R;
        G0_CKEYMAX.bits.keyg_max = stKeyMax.u8KeyMax_G;
        G0_CKEYMAX.bits.keyb_max = stKeyMax.u8KeyMax_B;
        HAL_WriteReg((HI_U32*)addr_REG, G0_CKEYMAX.u32); 

        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->G0_CKEYMIN.u32));
        G0_CKEYMIN.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        G0_CKEYMIN.bits.keyr_min = stKeyMin.u8KeyMin_R;
        G0_CKEYMIN.bits.keyg_min = stKeyMin.u8KeyMin_G;
        G0_CKEYMIN.bits.keyb_min = stKeyMin.u8KeyMin_B;
        HAL_WriteReg((HI_U32*)addr_REG, G0_CKEYMIN.u32); 
    }
    else
    {
        HAL_PRINT("Error layer id %d not support colorkey in %s: L%d\n",
            (HI_S32)enLayer, __FUNCTION__, __LINE__);
        return HI_FALSE;
    }    

    return HI_TRUE;    
}

HI_BOOL HAL_GRAPHIC_SetColorKeyMask(HAL_DISP_LAYER_E enLayer, HAL_GFX_MASK_S stMsk)
{
    U_G0_CMASK G0_CMASK;

    volatile  HI_U32 addr_REG;
    if((HAL_DISP_LAYER_GFX0 <= enLayer)&&(enLayer <= HAL_DISP_LAYER_GFX4))
    {
        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->G0_CMASK.u32));
        G0_CMASK.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        G0_CMASK.bits.kmsk_r = stMsk.u8Mask_r;
        G0_CMASK.bits.kmsk_g = stMsk.u8Mask_g;
        G0_CMASK.bits.kmsk_b = stMsk.u8Mask_b;
        HAL_WriteReg((HI_U32*)addr_REG, G0_CMASK.u32); 
    }
    else
    {
        HAL_PRINT("Error layer id %d not support colorkey mask in %s: L%d\n",
            (HI_S32)enLayer, __FUNCTION__, __LINE__);
        return HI_FALSE;
    }    

    return HI_TRUE;    
}


HI_BOOL HAL_GRAPHIC_SetGfxUpdMode(HAL_DISP_LAYER_E enLayer, HI_U32 u32Data)
{
    U_G0_CTRL G0_CTRL;

    volatile  HI_U32 addr_REG;
    if((LAYER_GFX_START <= enLayer)&&(enLayer <= LAYER_GFX_END))
    {
        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->G0_CTRL.u32));
        G0_CTRL.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        G0_CTRL.bits.upd_mode = u32Data;
        HAL_WriteReg((HI_U32*)addr_REG, G0_CTRL.u32); 

    }
    else
    {
        HAL_PRINT("Error layer id found in %s: L%d\n",__FUNCTION__, __LINE__);
        return HI_FALSE;
    }    

    return HI_TRUE;    
}


HI_BOOL HAL_GRAPHIC_SetGfxReadMode(HAL_DISP_LAYER_E enLayer, HI_U32 u32Data)
{
    U_G0_CTRL G0_CTRL;

    volatile  HI_U32 addr_REG;
    if((LAYER_GFX_START <= enLayer)&&(enLayer <= LAYER_GFX_END))
    {
        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->G0_CTRL.u32));
        G0_CTRL.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        G0_CTRL.bits.read_mode = u32Data;
        HAL_WriteReg((HI_U32*)addr_REG, G0_CTRL.u32); 

    }
    else
    {
        HAL_PRINT("Error layer id found in %s: L%d\n",__FUNCTION__, __LINE__);
        return HI_FALSE;
    }    

    return HI_TRUE;    
}

//for gfx decmpress
HI_BOOL HAL_GRAPHIC_SetGfxDcmpEnable(HAL_DISP_LAYER_E enLayer, HI_U32 bEnable)
{
    volatile U_G0_CTRL G0_CTRL;
    volatile  HI_U32 addr_REG;

    if((HAL_DISP_LAYER_GFX0 == enLayer) || (enLayer <= HAL_DISP_LAYER_GFX4))
    {
        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->G0_CTRL.u32));

        G0_CTRL.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        G0_CTRL.bits.dcmp_en = bEnable;
        HAL_WriteReg((HI_U32*)addr_REG, G0_CTRL.u32); 
    }
    else
    {
        return HI_FALSE;
    }

    return HI_TRUE;
}

HI_BOOL HAL_GRAPHIC_GetGfxDcmpEnableState(HAL_DISP_LAYER_E enLayer, HI_U32 *pbEnable)
{
    volatile U_G0_CTRL G0_CTRL;
    volatile  HI_U32 addr_REG;
    if((HAL_DISP_LAYER_GFX0 == enLayer) || (enLayer <= HAL_DISP_LAYER_GFX4))
    {
        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->G0_CTRL.u32));
        G0_CTRL.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        *pbEnable = G0_CTRL.bits.dcmp_en;
    }
    else
    {
        return HI_FALSE;
    }
    return HI_TRUE;
}
HI_BOOL HAL_GRAPHIC_SetGfxDcmpBank(HAL_DISP_LAYER_E enLayer, HI_U32 bank)
{
    #if 0
    volatile U_G0DCMPBANKWIDTH G0DCMPBANKWIDTH;
    volatile  HI_U32 addr_REG;

    if((LAYER_GFX_START <= enLayer)&&(enLayer <= LAYER_GFX_END))
    {
        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->G0DCMPBANKWIDTH.u32));
        G0DCMPBANKWIDTH.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        G0DCMPBANKWIDTH.bits.dcmp_bankwidth= bank - 1;
        HAL_WriteReg((HI_U32*)addr_REG, G0DCMPBANKWIDTH.u32); 
    }
    else
    {
        HAL_PRINT("Error layer id found in %s: L%d\n",__FUNCTION__, __LINE__);
        return HI_FALSE;
    }
    #endif
    return HI_TRUE;
}

HI_BOOL HAL_GRAPHIC_SetGfxDcmpStride(HAL_DISP_LAYER_E enLayer, HI_U32 stride)
{
    #if 0
    volatile U_G0DCMPSTRIDE G0DCMPSTRIDE;
    volatile  HI_U32 addr_REG;

    if((LAYER_GFX_START <= enLayer)&&(enLayer <= LAYER_GFX_END))
    {
        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->G0DCMPSTRIDE.u32));
        G0DCMPSTRIDE.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        G0DCMPSTRIDE.bits.dcmp_stride= stride;
        HAL_WriteReg((HI_U32*)addr_REG, G0DCMPSTRIDE.u32); 
    }
    else
    {
        HAL_PRINT("Error layer id found in %s: L%d\n",__FUNCTION__, __LINE__);
        return HI_FALSE;
    }
    #endif
    return HI_TRUE;
}
 #if 0
HI_BOOL HAL_GRAPHIC_SetGfxDcmpAddr(HAL_DISP_LAYER_E enLayer, HI_U32 addr_a, HI_U32 addr_r, HI_U32 addr_g, HI_U32 addr_b)
{
    #if 0
    volatile U_G0DCMPAADDR G0DCMPAADDR;
    volatile U_G0DCMPRADDR G0DCMPRADDR;
    volatile U_G0DCMPGADDR G0DCMPGADDR;
    volatile U_G0DCMPBADDR G0DCMPBADDR;
    volatile  HI_U32 addr_REG;

    if((LAYER_GFX_START <= enLayer)&&(enLayer <= LAYER_GFX_END))
    {
        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->G0DCMPAADDR.u32));
        G0DCMPAADDR.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        G0DCMPAADDR.bits.dcmp_addr_a= addr_a;
        HAL_WriteReg((HI_U32*)addr_REG, G0DCMPAADDR.u32); 

        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->G0DCMPRADDR.u32));
        G0DCMPRADDR.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        G0DCMPRADDR.bits.dcmp_addr_r= addr_r;
        HAL_WriteReg((HI_U32*)addr_REG, G0DCMPRADDR.u32); 

        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->G0DCMPGADDR.u32));
        G0DCMPGADDR.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        G0DCMPGADDR.bits.dcmp_addr_g= addr_g;
        HAL_WriteReg((HI_U32*)addr_REG, G0DCMPGADDR.u32); 

        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->G0DCMPBADDR.u32));
        G0DCMPBADDR.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        G0DCMPBADDR.bits.dcmp_addr_b= addr_b;
        HAL_WriteReg((HI_U32*)addr_REG, G0DCMPBADDR.u32); 
    }
    else
    {
        HAL_PRINT("Error layer id found in %s: L%d\n",__FUNCTION__, __LINE__);
        return HI_FALSE;
    }
    #endif
    return HI_TRUE;
}
 #endif
HI_BOOL HAL_GRAPHIC_SetGfxDcmpInterModeEn(HAL_DISP_LAYER_E enLayer, HI_U32 bEnable)
{
    #if 0
    volatile U_G0CTRL G0CTRL;
    volatile  HI_U32 addr_REG;

    if((LAYER_GFX_START <= enLayer)&&(enLayer <= LAYER_GFX_END))
    {
        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->G0CTRL.u32));

        G0CTRL.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        G0CTRL.bits.dcmp_inter = bEnable;
        HAL_WriteReg((HI_U32*)addr_REG, G0CTRL.u32); 
    }
    else
    {
        HAL_PRINT("Error layer id found in %s: L%d\n",__FUNCTION__, __LINE__);
        return HI_FALSE;
    }
    #endif
    return HI_TRUE;
} 

#if 0
/*配置内存检测区域首尾地址*/
HI_BOOL HAL_MDDRC_SetZoneAddr(HI_U32 u32Section, HI_U32 u32StartAddr, HI_U32 u32EndAddr)
{
    U_AWADDR_SRVLNC_START awaddr_srvlnc_start;   
    U_AWADDR_SRVLNC_END awaddr_srvlnc_end;
    volatile  HI_U32 addr_REG;
    //printk("Vou_SetGfxAddr %d\n",enLayer);

    if((0 <= u32Section)&&(u32Section <= 31))
    {
        addr_REG = (HI_U32)&(pMddrcReg->awaddr_srvlnc_start.u32) + 0x8 * u32Section;
        awaddr_srvlnc_start.u32 = u32StartAddr;
        //printk("HAL_MDDRC_SetZoneAddr:write_reg_addr_s=0x%x\n",addr_REG);
        HAL_WriteReg((HI_U32*)addr_REG, awaddr_srvlnc_start.u32); 
        addr_REG = (HI_U32)&(pMddrcReg->awaddr_srvlnc_end.u32) + 0x8 * u32Section;
        awaddr_srvlnc_end.u32 = u32EndAddr;
        //printk("HAL_MDDRC_SetZoneAddr:write_reg_addr_e=0x%x\n",addr_REG);
        HAL_WriteReg((HI_U32*)addr_REG, awaddr_srvlnc_end.u32);        
    }
    else
    {
        HAL_PRINT("Error Section found in %s: L%d\n",__FUNCTION__, __LINE__);
        return HI_FALSE;
    }
    
    return HI_TRUE;
}

HI_VOID HAL_MDDRC_GetStatus(HI_U32 *u32Status)
{
    volatile  HI_U32 addr_REG;

    addr_REG = (HI_U32)&(pMddrcReg->awaddr_srvlnc_status);
    *u32Status = HAL_ReadReg((HI_U32*)addr_REG);        
}

HI_BOOL HAL_GRAPHIC_SetGfxDcmpOffset(HAL_DISP_LAYER_E enLayer, HI_U32 offset)
{
    U_G0_DCMP_OFFSET DCMP_OFFSET;
    volatile  HI_U32 addr_REG;

    if((HAL_DISP_LAYER_GFX0 == enLayer)||(enLayer == HAL_DISP_LAYER_GFX4))
    {
        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->G0_DCMP_OFFSET.u32));
        DCMP_OFFSET.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        DCMP_OFFSET.bits.offset= offset;
        HAL_WriteReg((HI_U32*)addr_REG, DCMP_OFFSET.u32); 
    }
    else
    {
        HAL_PRINT("Error layer id found in %s: L%d\n",__FUNCTION__, __LINE__);
        return HI_FALSE;
    }

    return HI_TRUE;
}

HI_BOOL HAL_GRAPHIC_SetGfxDcmpAddr(HAL_DISP_LAYER_E enLayer, HI_U32 addr_AR, HI_U32 addr_GB)
{
    U_G0_ADDR  G0DCMP_ARADDR;
    U_G0_NADDR G0DCMP_GBADDR;
    volatile  HI_U32 addr_REG;

    if((HAL_DISP_LAYER_GFX0 == enLayer)||(enLayer == HAL_DISP_LAYER_GFX4))
    {
        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->G0_ADDR.u32));
        G0DCMP_ARADDR.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        G0DCMP_ARADDR.bits.surface_addr= addr_AR;
        HAL_WriteReg((HI_U32*)addr_REG, G0DCMP_ARADDR.u32); 

        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->G0_NADDR.u32));
        G0DCMP_GBADDR.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        G0DCMP_GBADDR.bits.surface_addr= addr_GB;
        HAL_WriteReg((HI_U32*)addr_REG, G0DCMP_GBADDR.u32); 
    }
    else
    {
        HAL_PRINT("Error layer id found in %s: L%d\n",__FUNCTION__, __LINE__);
        return HI_FALSE;
    }

    return HI_TRUE;
}

HI_BOOL HAL_GFX_WBC_SetWbcStride(HAL_DISP_LAYER_E enLayer, HI_U16 stride)
{
    U_WBC_G0_STRIDE WBC_STRIDE;
    volatile  HI_U32 addr_REG;

    if((HAL_DISP_LAYER_WBC_G0 == enLayer)||(enLayer == HAL_DISP_LAYER_WBC_G4))
    {
        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->WBC_G0_STRIDE.u32));
        WBC_STRIDE.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        WBC_STRIDE.bits.wbcstride = stride;
        HAL_WriteReg((HI_U32*)addr_REG, WBC_STRIDE.u32); 

    }
    else
    {
        HAL_PRINT("Error layer id found in %s: L%d\n",__FUNCTION__, __LINE__);
        return HI_FALSE;
    }

    return HI_TRUE;
}

HI_BOOL HAL_GFX_WBC_SetWbcOReso(HAL_DISP_LAYER_E enLayer, RECT_S *stRect)
{
    U_WBC_G0_ORESO WBC_ORESO;
    volatile  HI_U32 addr_REG;

    if((HAL_DISP_LAYER_WBC_G0 == enLayer)||(enLayer == HAL_DISP_LAYER_WBC_G4))
    {
        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->WBC_G0_ORESO.u32));
        WBC_ORESO.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        WBC_ORESO.bits.ow = stRect->u32Width - 1;
        WBC_ORESO.bits.oh = stRect->u32Height - 1;
        HAL_WriteReg((HI_U32*)addr_REG, WBC_ORESO.u32); 

    }
    else
    {
        HAL_PRINT("Error layer id found in %s: L%d\n",__FUNCTION__, __LINE__);
        return HI_FALSE;
    }

    return HI_TRUE;
}

HI_BOOL HAL_GFX_WBC_SetWbcCrop(HAL_DISP_LAYER_E enLayer, RECT_S *stRect)
{
    U_WBC_G0_FCROP       WBC_FCROP;
    U_WBC_G0_LCROP       WBC_LCROP;    
    volatile  HI_U32 addr_REG;

    if((HAL_DISP_LAYER_WBC_G0 == enLayer)||(enLayer == HAL_DISP_LAYER_WBC_G4))
    {
        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->WBC_G0_FCROP.u32));
        WBC_FCROP.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        WBC_FCROP.bits.wfcrop = stRect->s32X;
        WBC_FCROP.bits.hfcrop = stRect->s32Y;
        HAL_WriteReg((HI_U32*)addr_REG, WBC_FCROP.u32); 
        
        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->WBC_G0_LCROP.u32));
        WBC_LCROP.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        WBC_LCROP.bits.wlcrop = stRect->s32X + stRect->u32Width - 1;
        WBC_LCROP.bits.hlcrop = stRect->s32Y + stRect->u32Height - 1;
        HAL_WriteReg((HI_U32*)addr_REG, WBC_LCROP.u32); 
    }
    else
    {
        HAL_PRINT("Error layer id found in %s: L%d\n",__FUNCTION__, __LINE__);
        return HI_FALSE;
    }

    return HI_TRUE;
}

HI_BOOL HAL_GFX_WBC_SetWbcAddr(HAL_DISP_LAYER_E enLayer, HI_U32 u32ARAddr, HI_U32 u32GBAddr)
{
    U_WBC_G0_AR_ADDR     WBC_AR_ADDR;
    U_WBC_G0_GB_ADDR     WBC_GB_ADDR;
    volatile  HI_U32 addr_REG;

    if((HAL_DISP_LAYER_WBC_G0 == enLayer)||(enLayer == HAL_DISP_LAYER_WBC_G4))
    {
        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->WBC_G0_AR_ADDR.u32));
        WBC_AR_ADDR.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        WBC_AR_ADDR.bits.wbcaddr = u32ARAddr;
        HAL_WriteReg((HI_U32*)addr_REG, WBC_AR_ADDR.u32); 

        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->WBC_G0_GB_ADDR.u32));
        WBC_GB_ADDR.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        WBC_GB_ADDR.bits.wbccaddr = u32GBAddr;
        HAL_WriteReg((HI_U32*)addr_REG, WBC_GB_ADDR.u32); 
    }
    else
    {
        HAL_PRINT("Error layer id found in %s: L%d\n",__FUNCTION__, __LINE__);
        return HI_FALSE;
    }
    return HI_TRUE;
}

HI_BOOL HAL_GFX_WBC_SetWbcDataFmt(HAL_DISP_LAYER_E enLayer, HAL_DISP_PIXEL_FORMAT_E  enDataFmt)
{
    U_WBC_G0_CTRL     G0_CTRL;
    volatile  HI_U32 addr_REG;
    
    if(((HAL_DISP_LAYER_WBC_G0 == enLayer)||(enLayer == HAL_DISP_LAYER_WBC_G4)) && 
        (HAL_INPUTFMT_ARGB_8888 == enDataFmt))
    {
        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->WBC_G0_CTRL.u32));
        G0_CTRL.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        G0_CTRL.bits.format_out = 0x2;
        HAL_WriteReg((HI_U32*)addr_REG, G0_CTRL.u32); 
    }
    else
    {
        HAL_PRINT("Error layer id found in %s: L%d\n",__FUNCTION__, __LINE__);
        return HI_FALSE;
    }
    
    return HI_TRUE;
}


HI_BOOL HAL_GFX_WBC_SetWbcOffset(HAL_DISP_LAYER_E enLayer, HI_U32 offset)
{
    U_WBC_G0_OFFSET      WBC_OFFSET;
    volatile  HI_U32 addr_REG;

    if((HAL_DISP_LAYER_WBC_G0 == enLayer)||(enLayer == HAL_DISP_LAYER_WBC_G4))
    {
        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->WBC_G0_OFFSET.u32));
        WBC_OFFSET.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        WBC_OFFSET.bits.wbcoffset = offset;
        HAL_WriteReg((HI_U32*)addr_REG, WBC_OFFSET.u32); 
    }
    else
    {
        HAL_PRINT("Error layer id found in %s: L%d\n",__FUNCTION__, __LINE__);
        return HI_FALSE;
    }

    return HI_TRUE;
}

HI_BOOL HAL_GFX_WBC_SetWbcAutoStopEn(HAL_DISP_LAYER_E enLayer, HI_BOOL bStopEn)
{
    U_WBC_G0_CTRL        WBC_CTRL;
    volatile  HI_U32 addr_REG;

    if((HAL_DISP_LAYER_WBC_G0 == enLayer)||(enLayer == HAL_DISP_LAYER_WBC_G4))
    {
        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->WBC_G0_CTRL.u32));
        WBC_CTRL.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        WBC_CTRL.bits.auto_stop_en = bStopEn;
        HAL_WriteReg((HI_U32*)addr_REG, WBC_CTRL.u32); 
    }
    else
    {
        HAL_PRINT("Error layer id found in %s: L%d\n",__FUNCTION__, __LINE__);
        return HI_FALSE;
    }

    return HI_TRUE;
}

HI_BOOL HAL_GFX_WBC_SetWbcEnable(HAL_DISP_LAYER_E enLayer, HI_BOOL bEnable)
{
    U_WBC_G0_CTRL        WBC_CTRL;
    U_WBC_G0_CMP         WBC_CMP;
    volatile  HI_U32 addr_REG;

    if((HAL_DISP_LAYER_WBC_G0 == enLayer)||(enLayer == HAL_DISP_LAYER_WBC_G4))
    {
        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->WBC_G0_CTRL.u32));
        WBC_CTRL.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        WBC_CTRL.bits.wbc_en = bEnable;
        HAL_WriteReg((HI_U32*)addr_REG, WBC_CTRL.u32);
        
        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->WBC_G0_CMP.u32));
        WBC_CMP.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        WBC_CMP.bits.cmp_en = bEnable;
        HAL_WriteReg((HI_U32*)addr_REG, WBC_CMP.u32); 
    }
    else
    {
        HAL_PRINT("Error layer id found in %s: L%d\n",__FUNCTION__, __LINE__);
        return HI_FALSE;
    }

    return HI_TRUE;
}

HI_VOID HAL_GFX_WBC_GetWbcEnState(HAL_DISP_LAYER_E enLayer, HI_BOOL *pbEnable)
{
    U_WBC_G0_CTRL        WBC_CTRL;
    volatile  HI_U32 addr_REG;

    *pbEnable = HI_FALSE;
    if((HAL_DISP_LAYER_WBC_G0 == enLayer)||(enLayer == HAL_DISP_LAYER_WBC_G4))
    {
        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->WBC_G0_CTRL.u32));
        WBC_CTRL.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        if (1 == WBC_CTRL.bits.wbc_en)
        {
            *pbEnable = HI_TRUE;
        }
    }
    else
    {
        HAL_PRINT("Error layer id found in %s: L%d\n",__FUNCTION__, __LINE__);
    }

    return ;
}

HI_BOOL HAL_GFX_WBC_SetWbcRegUp(HAL_DISP_LAYER_E enLayer)
{
    U_WBC_G0_UPD         WBC_UPD;
    volatile  HI_U32 addr_REG;

    if((HAL_DISP_LAYER_WBC_G0 == enLayer)||(enLayer == HAL_DISP_LAYER_WBC_G4))
    {
        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->WBC_G0_UPD.u32));
        WBC_UPD.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        WBC_UPD.bits.regup = 0x1;
        HAL_WriteReg((HI_U32*)addr_REG, WBC_UPD.u32); 
    }
    else
    {
        HAL_PRINT("Error layer id found in %s: L%d\n",__FUNCTION__, __LINE__);
        return HI_FALSE;
    }

    return HI_TRUE;
}

HI_VOID HAL_GFX_WBC_GetWbcStopState(HAL_DISP_LAYER_E enLayer, HI_BOOL *pbWbcStop)
{
    U_WBC_G0_UPD         WBC_UPD;
    volatile  HI_U32 addr_REG;
    U_WBC_G0_STPSTACLR   WBC_STPSTACLR;

    *pbWbcStop = HI_FALSE;
    if((HAL_DISP_LAYER_WBC_G0 == enLayer)||(enLayer == HAL_DISP_LAYER_WBC_G4))
    {
        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->WBC_G0_UPD.u32));
        WBC_UPD.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        if (1 == WBC_UPD.bits.auto_stop_int)
        {
            *pbWbcStop = HI_TRUE;
            /* 获取完之后要清除 */
            addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->WBC_G0_STPSTACLR.u32));
            WBC_STPSTACLR.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            WBC_STPSTACLR.bits.stpsta_clr = 1;
            HAL_WriteReg((HI_U32*)addr_REG, WBC_STPSTACLR.u32); 
        }
    }
    else
    {
        HAL_PRINT("Error layer id found in %s: L%d\n",__FUNCTION__, __LINE__);
        return ;
    }

    return ;
}
#endif

HI_BOOL HAL_CBM_SetCbmAttr(HAL_DISP_LAYER_E enLayer, HAL_DISP_OUTPUTCHANNEL_E enChan)
{
    U_VHD_CORRESP VHD_CORRESP;
    U_GDC_CORRESP GDC_CORRESP;

    if(enLayer == HAL_DISP_LAYER_VP)
    {        
        VHD_CORRESP.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->VHD_CORRESP.u32));
        if(enChan == HAL_DISP_CHANNEL_DHD0)
        {
            VHD_CORRESP.bits.v1_corresp = 0x1;
        }       
        else if(enChan == HAL_DISP_CHANNEL_DHD1)
        {
            VHD_CORRESP.bits.v1_corresp = 0x2;
        }       
        else
        {
            HAL_PRINT("Error channel id found in %s: L%d\n",__FUNCTION__, __LINE__);
            return HI_FALSE;
        }
        
        HAL_WriteReg((HI_U32*)&(pVoReg->VHD_CORRESP.u32), VHD_CORRESP.u32); 
    }
    else if(enLayer == HAL_DISP_LAYER_HC0)
    {        
        GDC_CORRESP.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->GDC_CORRESP.u32));
        if(enChan == HAL_DISP_CHANNEL_DHD0)
        {
            GDC_CORRESP.bits.g2_corresp = 0x2;
        }
        else if(enChan == HAL_DISP_CHANNEL_DHD1)
        {
            GDC_CORRESP.bits.g2_corresp = 0x2;
        } 
        else if(enChan == HAL_DISP_CHANNEL_DSD0)
        {
            GDC_CORRESP.bits.g2_corresp = 0x4;
        }  
        else
        {
            HAL_PRINT("Error channel id found in %s: L%d\n",__FUNCTION__, __LINE__);
            return HI_FALSE;
        }
        
        HAL_WriteReg((HI_U32*)&(pVoReg->GDC_CORRESP.u32), GDC_CORRESP.u32); 
    }
    else
    {
        HAL_PRINT("Error layer id found in %s: L%d\n",__FUNCTION__, __LINE__);
        return HI_FALSE;
    }
    return HI_TRUE;
}

HI_BOOL HAL_CBM_ReleaseCbmAttr(HAL_DISP_LAYER_E enLayer)
{
    U_VHD_CORRESP VHD_CORRESP;
    U_GDC_CORRESP GDC_CORRESP;

    if(enLayer == HAL_DISP_LAYER_VP)
    {       
        VHD_CORRESP.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->VHD_CORRESP.u32));
        VHD_CORRESP.bits.v1_corresp = 0x0;        
        HAL_WriteReg((HI_U32*)&(pVoReg->VHD_CORRESP.u32), VHD_CORRESP.u32); 
    }
    else if(enLayer == HAL_DISP_LAYER_HC0)
    {
        GDC_CORRESP.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->GDC_CORRESP.u32));
        GDC_CORRESP.bits.g2_corresp = 0x0;        
        HAL_WriteReg((HI_U32*)&(pVoReg->GDC_CORRESP.u32), GDC_CORRESP.u32); 
    }
    else
    {
        HAL_PRINT("Error layer id found in %s: L%d\n",__FUNCTION__, __LINE__);
        return HI_FALSE;
    }
    return HI_TRUE;
}


HI_BOOL HAL_CBM_SetCbmBkg(HI_U32 bMixerId, HAL_DISP_BKCOLOR_S *pstBkg)
{
    U_CBM_BKG1 CBM_BKG1;
    //U_CBMBKG2 CBMBKG2;
    U_CBM_BKG2 CBM_BKG2;
    U_MIXDSD_BKG MIXDSD_BKG;

    if(bMixerId == HAL_CBMMIX1)
    {
        CBM_BKG1.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->CBM_BKG1.u32));
        CBM_BKG1.bits.cbm_bkgy1  = HAL_GetBkg_BUMA(pstBkg->u8Bkg_y);
        CBM_BKG1.bits.cbm_bkgcb1 = HAL_GetBkg_BUMA(pstBkg->u8Bkg_cb);
        CBM_BKG1.bits.cbm_bkgcr1 = HAL_GetBkg_BUMA(pstBkg->u8Bkg_cr); 
        HAL_WriteReg((HI_U32*)&(pVoReg->CBM_BKG1.u32), CBM_BKG1.u32); 
    }   
    else if(bMixerId == HAL_CBMMIX2)
    {
        CBM_BKG2.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->CBM_BKG2.u32));
        CBM_BKG2.bits.cbm_bkgy2  = HAL_GetBkg_BUMA(pstBkg->u8Bkg_y);
        CBM_BKG2.bits.cbm_bkgcb2 = HAL_GetBkg_BUMA(pstBkg->u8Bkg_cb);
        CBM_BKG2.bits.cbm_bkgcr2 = HAL_GetBkg_BUMA(pstBkg->u8Bkg_cr);        
        HAL_WriteReg((HI_U32*)&(pVoReg->CBM_BKG2.u32), CBM_BKG2.u32); 
    }  
    else if(bMixerId == HAL_CBMMIX3)
    {
        MIXDSD_BKG.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->MIXDSD_BKG.u32));
        MIXDSD_BKG.bits.mixer_bkgy  = HAL_GetBkg_BUMA(pstBkg->u8Bkg_y);
        MIXDSD_BKG.bits.mixer_bkgcb = HAL_GetBkg_BUMA(pstBkg->u8Bkg_cb);
        MIXDSD_BKG.bits.mixer_bkgcr = HAL_GetBkg_BUMA(pstBkg->u8Bkg_cr);        
        HAL_WriteReg((HI_U32*)&(pVoReg->MIXDSD_BKG.u32), MIXDSD_BKG.u32); 
    }
    else
    {
        HAL_PRINT("Error mix id found in %s: L%d\n",__FUNCTION__, __LINE__);
        return HI_FALSE;
    }

    return HI_TRUE;
}


HI_BOOL HAL_CBM_SetCbmMixerPrio(HAL_DISP_LAYER_E enLayer, HI_U8 u8Prio, HI_U8 u8MixerId)
{
    U_CBM_MIX1 CBM_MIX1;
    //U_CBMBKG2 CBMBKG2;
    U_CBM_MIX2 CBM_MIX2;
    U_MIXDSD_MIX MIXDSD_MIX;
    HI_U8  u8LayerId = 0;

    //---------------------------------------------------------------
    if(u8MixerId == HAL_CBMMIX1)
    {
        switch(enLayer)
        {
            case HAL_DISP_LAYER_VHD0:
            {
                u8LayerId = 0x1;
                break;
            }
            case HAL_DISP_LAYER_VP:
            {
                u8LayerId = 0x9;
                break;
            }
            case HAL_DISP_LAYER_GFX0:
            {
                u8LayerId = 0x2;
                break;
            }
            case HAL_DISP_LAYER_HC0:
            {
                u8LayerId = 0xa;
                break;
            }
            case HI_INVALID_LAYER:
            {
                u8LayerId = 0x0;                
                break;
            }
            default:
            {
                HAL_PRINT("Error layer id found in %s: L%d\n",__FUNCTION__, __LINE__);
                return HI_FALSE;
            }
        }
        //set mixer prio
        CBM_MIX1.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->CBM_MIX1.u32));
        switch(u8Prio)
        {
            case 6:
            {
                //clean all priority
                CBM_MIX1.bits.mixer_prio0 = 0;
                CBM_MIX1.bits.mixer_prio1 = 0;
                CBM_MIX1.bits.mixer_prio2 = 0;
                CBM_MIX1.bits.mixer_prio3 = 0;
                break;
            }
            case 0:
            {
                CBM_MIX1.bits.mixer_prio0 = u8LayerId;
                break;
            }
            case 1:
            {
                CBM_MIX1.bits.mixer_prio1 = u8LayerId;
                break;
            }
            case 2:
            {
                CBM_MIX1.bits.mixer_prio2 = u8LayerId;
                break;
            }
            case 3:
            {
                CBM_MIX1.bits.mixer_prio3 = u8LayerId;
                break;
            }
            default:
            {
                HAL_PRINT("Error priority id found in %s: L%d\n",__FUNCTION__, __LINE__);
                return HI_FALSE;
            }
        }
        HAL_WriteReg((HI_U32*)&(pVoReg->CBM_MIX1.u32), CBM_MIX1.u32); 
    }
    //---------------------------------------------------------------
    else if(u8MixerId == HAL_CBMMIX2)
    {
        //check layer availability
        switch(enLayer)
        {
            case HAL_DISP_LAYER_VHD1:
            {
                u8LayerId = 0x3;
                break;
            }            
            case HAL_DISP_LAYER_VP:
            {
                u8LayerId = 0x9;
                break;
            }
            case HAL_DISP_LAYER_GFX4:
            {
                u8LayerId = 0x4;
                break;
            }
            case HAL_DISP_LAYER_HC0:
            {
                u8LayerId = 0xa;
                break;
            }            
            case HI_INVALID_LAYER:
            {
                u8LayerId = 0x0;                
                break;
            }
            default:
            {
                HAL_PRINT("Error layer id %d found in %s: L%d\n",enLayer,__FUNCTION__, __LINE__);
                return HI_FALSE;
            }
        }
        //set mixer prio
        CBM_MIX2.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->CBM_MIX2.u32));
        switch(u8Prio)
        {
            case 6:
            {
                //clean all priority
                CBM_MIX2.bits.mixer_prio0 = 0;
                CBM_MIX2.bits.mixer_prio1 = 0;
                CBM_MIX2.bits.mixer_prio2 = 0;                
                CBM_MIX2.bits.mixer_prio3 = 0;
                break;
            }
            case 0:
            {
                CBM_MIX2.bits.mixer_prio0 = u8LayerId;
                break;
            }
            case 1:
            {
                CBM_MIX2.bits.mixer_prio1 = u8LayerId;
                break;
            }
            case 2:
            {
                CBM_MIX2.bits.mixer_prio2 = u8LayerId;
                break;
            }            
            case 3:
            {
                CBM_MIX2.bits.mixer_prio3 = u8LayerId;
                break;
            }
            default:
            {
                HAL_PRINT("Error priority id found in %s: L%d\n",__FUNCTION__, __LINE__);
                return HI_FALSE;
            }
        }
        HAL_WriteReg((HI_U32*)&(pVoReg->CBM_MIX2.u32), CBM_MIX2.u32); 
    }
    //---------------------------------------------------------------
    else if(u8MixerId == HAL_CBMMIX3)
    {
        //check layer availability
        switch(enLayer)
        {
            case HAL_DISP_LAYER_VSD0:
            {
                u8LayerId = 0x1;
                break;
            }
            case HAL_DISP_LAYER_GFX1:
            {
                u8LayerId = 0x2;
                break;
            }
            case HAL_DISP_LAYER_HC0:
            {
                u8LayerId = 0xa;
                break;
            }            
            case HI_INVALID_LAYER:
            {
                u8LayerId = 0x0;                
                break;
            }
            default:
            {
                HAL_PRINT("Error layer id found in %s: L%d\n",__FUNCTION__, __LINE__);
                return HI_FALSE;
            }
        }
        //set mixer prio
        MIXDSD_MIX.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->MIXDSD_MIX.u32));
        switch(u8Prio)
        {
            case 6:
            {
                //clean all priority
                MIXDSD_MIX.bits.mixer_prio0 = 0;
                MIXDSD_MIX.bits.mixer_prio1 = 0;
                MIXDSD_MIX.bits.mixer_prio2 = 0;
                break;
            }
            case 0:
            {
                MIXDSD_MIX.bits.mixer_prio0 = u8LayerId;
                break;
            }
            case 1:
            {
                MIXDSD_MIX.bits.mixer_prio1 = u8LayerId;
                break;
            }
            case 2:
            {
                MIXDSD_MIX.bits.mixer_prio2 = u8LayerId;
                break;
            }
            default:
            {
                HAL_PRINT("Error priority id found in %s: L%d\n",__FUNCTION__, __LINE__);
                return HI_FALSE;
            }
        }
        HAL_WriteReg((HI_U32*)&(pVoReg->MIXDSD_MIX.u32), MIXDSD_MIX.u32); 
    }
    else
    {
        HAL_PRINT("Error, u8MixerId %d Vou_SetCbmMixerPrio() Set a wrong CBMMIX\n",u8MixerId);
        return HI_FALSE;
    }
    
    return HI_TRUE;
}

HI_BOOL HAL_WBC_SetWbcEnable(HAL_DISP_LAYER_E enLayer, HI_U32 bEnable)
{
    volatile U_WBC_DHD0_CTRL WBC_DHD0_CTRL;
     volatile HI_U32 addr_REG;

    if(HAL_DISP_LAYER_WBC_D == enLayer) 
    {
        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->WBC_DHD0_CTRL.u32));
        WBC_DHD0_CTRL.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        WBC_DHD0_CTRL.bits.wbc_en = bEnable;
        HAL_WriteReg((HI_U32*)addr_REG, WBC_DHD0_CTRL.u32); 
    }
    else
    {
        HAL_PRINT("Error layer id found in %s: L%d\n",__FUNCTION__, __LINE__);
        return HI_FALSE;
    }

    return HI_TRUE;
}

/* 亮度地址设置*/
HI_BOOL HAL_WBC_SetWbcAddr(HAL_DISP_LAYER_E enLayer, HI_U32 u32Addr,HI_U16 u16Str)
{
    volatile U_WBC_DHD0_YADDR WBC_DHD0_YADDR;
    volatile U_WBC_DHD0_STRIDE WBC_DHD0_STRIDE;
    volatile HI_U32 addr_REG;

    if(HAL_DISP_LAYER_WBC_D == enLayer) 
    {
        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->WBC_DHD0_YADDR.u32));
        WBC_DHD0_YADDR.u32 = u32Addr;
        HAL_WriteReg((HI_U32*)addr_REG, WBC_DHD0_YADDR.u32); 

        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->WBC_DHD0_STRIDE.u32));
        WBC_DHD0_STRIDE.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        WBC_DHD0_STRIDE.bits.wbclstride = u16Str;
        HAL_WriteReg((HI_U32*)addr_REG, WBC_DHD0_STRIDE.u32); 
    }
    else
    {
        HAL_PRINT("Error layer id found in %s: L%d\n",__FUNCTION__, __LINE__);
        return HI_FALSE;
    }

    return HI_TRUE;
}

HI_BOOL HAL_WBC_SetWbcCAddr(HAL_DISP_LAYER_E enLayer, HI_U32 u32CAddr,HI_U16 u16CStr)
{
    volatile U_WBC_DHD0_CADDR WBC_DHD0_CADDR;
    volatile U_WBC_DHD0_STRIDE WBC_DHD0_STRIDE;
    volatile HI_U32 addr_REG;

    if(HAL_DISP_LAYER_WBC_D == enLayer) 
    {
        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->WBC_DHD0_CADDR.u32));
        WBC_DHD0_CADDR.u32 = u32CAddr;
        HAL_WriteReg((HI_U32*)addr_REG, WBC_DHD0_CADDR.u32); 

        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->WBC_DHD0_STRIDE.u32));
        WBC_DHD0_STRIDE.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        WBC_DHD0_STRIDE.bits.wbccstride = u16CStr;
        HAL_WriteReg((HI_U32*)addr_REG, WBC_DHD0_STRIDE.u32); 
    }
    else
    {
        HAL_PRINT("Error layer id found in %s: L%d\n",__FUNCTION__, __LINE__);
        return HI_FALSE;
    }

    return HI_TRUE;
}

HI_BOOL HAL_WBC_SetWbcOReso(HAL_DISP_LAYER_E enLayer,HI_U32 width,  HI_U32 height)
{    
    if(HAL_DISP_LAYER_WBC_D == enLayer)
    {
        volatile U_WBC_DHD0_ORESO WBC_DHD0_ORESO;
        WBC_DHD0_ORESO.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->WBC_DHD0_ORESO.u32));
        WBC_DHD0_ORESO.bits.ow = width - 1;
        WBC_DHD0_ORESO.bits.oh  = height - 1;
        HAL_WriteReg((HI_U32*)&(pVoReg->WBC_DHD0_ORESO.u32), WBC_DHD0_ORESO.u32);  
    }    
    else
    {
        HAL_PRINT("Error layer id found in %s: L%d\n",__FUNCTION__, __LINE__);
        return HI_FALSE;
    }
    return HI_TRUE;
}

HI_BOOL HAL_WBC_SetWbcOutFmt(HAL_DISP_LAYER_E enLayer, HAL_DISP_INTFDATAFMT_E enIntfFmt)
{
    volatile U_WBC_DHD0_CTRL WBC_DHD0_CTRL;
    volatile U_WBC_DHD0_ZME_VSP  WBC_DHD0_ZME_VSP;
    volatile HI_U32 addr_REG;

    if(HAL_DISP_LAYER_WBC_D == enLayer) 
    {
        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->WBC_DHD0_CTRL.u32));
        WBC_DHD0_CTRL.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        WBC_DHD0_CTRL.bits.format_out = enIntfFmt;
        HAL_WriteReg((HI_U32*)addr_REG, WBC_DHD0_CTRL.u32); 

        // for hi3531 WBC2 only!
        // set the zme output format(420) when set WBC output format(420)
        if(enIntfFmt == HAL_DISP_INTFDATAFMT_YCBCR422)
        {
            WBC_DHD0_ZME_VSP.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->WBC_DHD0_ZME_VSP.u32));
            WBC_DHD0_ZME_VSP.bits.zme_out_fmt = 0;//yuv 422 output for zme
            HAL_WriteReg((HI_U32*)&(pVoReg->WBC_DHD0_ZME_VSP.u32), WBC_DHD0_ZME_VSP.u32);
        }
        else if(enIntfFmt == HAL_DISP_INTFDATAFMT_YCBCR420)
        {
            WBC_DHD0_ZME_VSP.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->WBC_DHD0_ZME_VSP.u32));
            WBC_DHD0_ZME_VSP.bits.zme_out_fmt = 1;//yuv 422 output for zme
            HAL_WriteReg((HI_U32*)&(pVoReg->WBC_DHD0_ZME_VSP.u32), WBC_DHD0_ZME_VSP.u32);
        }
    }
    else
    {
        HAL_PRINT("Error layer id found in %s: L%d\n",__FUNCTION__, __LINE__);
        return HI_FALSE;
    }

    return HI_TRUE;
    
}

HI_BOOL HAL_WBC_GetWbcOutFmt(HAL_DISP_LAYER_E enLayer, HAL_DISP_INTFDATAFMT_E *penIntfFmt)
{
    volatile U_WBC_DHD0_CTRL WBC_DHD0_CTRL;
    volatile HI_U32 addr_REG;

    if(HAL_DISP_LAYER_WBC_D == enLayer) 
    {
        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->WBC_DHD0_CTRL.u32));
        WBC_DHD0_CTRL.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        *penIntfFmt = WBC_DHD0_CTRL.bits.format_out;
    }
    else
    {
        HAL_PRINT("Error layer id found in %s: L%d\n",__FUNCTION__, __LINE__);
        return HI_FALSE;
    }

    return HI_TRUE;
    
}
HI_BOOL HAL_WBC_SetWbcDataSource(HAL_DISP_LAYER_E enLayer, HI_U32 u32DateSource)
{
    volatile U_WBC_CORRESP WBC_CORRESP;
    volatile HI_U32 addr_REG;

    if(HAL_DISP_LAYER_WBC_D == enLayer) 
    {
        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->WBC_CORRESP.u32));
        WBC_CORRESP.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        WBC_CORRESP.bits.wbc_corresp = u32DateSource;
        HAL_WriteReg((HI_U32*)addr_REG, WBC_CORRESP.u32);         
    }
    else
    {
        HAL_PRINT("Error layer id found in %s: L%d\n",__FUNCTION__, __LINE__);
        return HI_FALSE;
    }

    return HI_TRUE;
    
}

#ifdef __cplusplus
#if __cplusplus
 }
#endif
#endif /* End of #ifdef __cplusplus */

