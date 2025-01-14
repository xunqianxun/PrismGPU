/***************************************************************\
	Author : Guoqi Li 
	Date   : 2025/1/9 
	Funcation: HeadWare define head file 
\***************************************************************/

#ifndef _HW_HXX_
#define _HW_HXX_

#ifdef __cplusplus
extern "C" {
#endif
	// Standard C-runtime headers
#include <stddef.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>


#include <initguid.h>

// NTOS headers
#include <ntddk.h>

#ifndef FAR
#define FAR
#endif

// Windows headers
#include <windef.h>
#include <winerror.h>

// Windows GDI headers
#include <wingdi.h>

// Windows DDI headers
#include <winddi.h>
#include <ntddvdeo.h>

#include <d3dkmddi.h>
#include <d3dkmthk.h>

#include <ntstrsafe.h>
#include <ntintsafe.h>

#include <dispmprt.h>
#ifdef __cplusplus
}
#endif

//平台定义 
#define BDD_DRIVCE_NAME            L"\\Device\\PrismDriver"
#define BDD_DEVICE_KMDSYNLINK      L"\\DosDevices\\PrismDriver"

//其中\为链接这个宏定义，因为宏定义一般都是一行，使用\来链接多行。
//...表示不定类型的参数，什么类型对的参数都可以
//do while(0)代表只执行一次

//#define DPF(lvl, fmt, ...)      \
//do { \
//    if ( g_eDebugLevel & lvl ) { \
//        DebugPrint(fmt, __VA_ARGS__); \
//    } \
//} while (0);


#define BDD_DEVICE_NAME        L"PrismGPU"
#define BDD_OPLAT_NAME          "PrismGPU" 
#define BDD_ODAC_NAME           "UNKNOWN"
#define BDD_ADAPTE_NAME         "UNKNOWN"
#define BDD_BIOS_NAME           "UNKNOWN"

#define BDD_DRIVER_PICE      (0) 
#define BDD_DRIVER_APIC      (1) 
#define BDD_DRIVER_PRESENT_MODE           BDD_DRIVER_PICE  

#define BDD_DRIVER_VENDORID    0x1414
#define BDD_DRIVER_DEVICE_ID   0x0010

#define BDD_DRIVER_REG_LENGTH  6*1024 
#define BDD_DRIVER_MEM_LENGTH  64*1024*1024

#define BDD_DRIVER_MAXCHILD 1 //暂时使用过预定义的方式

#define DISP_HORIZON_PIXEL		(1280)
#define DISP_VERTICAL_LINE		(720)
#define DISP_PIXEL_SIZE			(4)

#define REGISTER_PATH     L"\\Registry\\Machine\\Software\\KMDOD\\UserParams"


typedef struct
{
	UINT32 PIEXLWIDTH;
	UINT32 PIEXLHEIGHT;
	UINT32 HORIZONTOTAL; //水平扫描总数
	UINT32 HORIZONBOARDLEFT; //左边框像素量
	UINT32 HORIZONACTIVE; //可显示的水平像素量
	UINT32 HORIZONBOARDRIGHT; //右边框像素量
	UINT32 HORIZONFRONT;
	UINT32 HORIZONSYNCWIDTH; //水平脉冲宽度

	UINT32 VERTICALTOTAL;
	UINT32 VERTICALBOARDTOP;
	UINT32 VERTICALACTIVE;
	UINT32 VERTICALBOARDBOTTOM;
	UINT32 VERTICALFRONT;
	UINT32 VERTICALSYNCWIDTH;
	UINT32 PIEXLCLOCKKHZ;
	struct
	{
		UINT32 INTERLANCE : 1; //标识支持交错扫描
		UINT32 DOUBLESCAN : 1; //启用双扫描模式
		UINT32 PIEXLREPRTITION : 4; //显示时像素的重复次数
		UINT32 HSYNCPOSTIVE : 1; //水平脉冲正
		UINT32 VSYNCPOSTIVE : 1; //垂直脉冲正
		UINT32 EXCLUSIVE3D : 1; //只支持3d模式
		UINT32 SUBSIMPLE3D : 1;
		UINT32 USEIN3DONLY : 1; //仅在3D使用
		UINT32 STEREO3DPREFENCE : 1;
		UINT32 YONLY : 1;
		UINT32 YCBCR420SUPPORT : 1;
		UINT32 DTDCOUNTER : 5;

	}FLAGS;

}CRTCTIMING;

typedef struct
{
	UINT16 FACID;
	UINT16 PRODUCTID;
	UINT32 SNCODE;
	UINT8  PRODWEEK;
	UINT8  PRODYEAR;
	UINT8  VERSION;
	UINT8  REVERSION;
	UINT64 CLOCKKHZ;
	UINT64 HACTIVEPIXEL;// 水平有效像素数（显示区域的宽度，单位：像素）
	UINT64 HNOACTIVEPIXEL;// 水平空白区像素数（从显示区域到下一帧开始的空白区域，单位：像素）
	UINT64 VACTIVEPIXEL;// 垂直有效像素数（显示区域的高度，单位：像素）
	UINT64 VNOACTIVEPIXEL;// 垂直空白区像素数（从显示区域到下一帧开始的空白区域，单位：像素）
	UINT64 HFORNTPROCH;// 水平前沿（水平同步信号的前导时间，单位：像素）
	UINT64 HSYNCPULSE; // 水平同步脉冲宽度（水平同步信号的持续时间，单位：像素）
	UINT64 VFORNTPROCH;// 垂直前沿（垂直同步信号的前导时间，单位：像素）
	UINT64 VSYNCPULSE;// 垂直同步脉冲宽度（垂直同步信号的持续时间，单位：像素）
	UINT16 HIMAGESIZE; // 显示器的水平图像尺寸（单位：毫米）
	UINT16 VIMAGESIZE;// 显示器的垂直图像尺寸（单位：毫米）
	UINT16 HBOARDER; // 水平边框宽度（单位：像素）
	UINT16 VBOARDER; // 垂直边框宽度（单位：像素）
	UINT8  HSYNCSITUATION; // 水平同步信号的极性（1表示正极性，0表示负极性）
	UINT8  VSYNCSITUATION;// 垂直同步信号的极性（1表示正极性，0表示负极性）
	struct 
	{
		UINT8 INTERSECTSCAN;
		UINT8 STEREOMODE;
		UINT8 SYNCTYPE;
	}FFLAGE;
}HWEDIDINFO;

typedef struct {
	UINT64 REGPBASE;
	UINT64 MEMPBASE;
	HWEDIDINFO DEVICEHWEDIDINFO;
	CRTCTIMING CrTcTimg;

}HWDEVICEINFO;

extern HWDEVICEINFO h_DEVICEINFO;


//
// hw funcation define 
//

NTSTATUS AnalysizeEdid(
	_In_  BYTE* m_EDID,
	_Inout_ HWEDIDINFO* h_EDID);

NTSTATUS InitHardware(
	_In_ DXGKRNL_INTERFACE* pDxgkInterface,
	_In_ UINT64 REGPBASE,
	_In_ HWEDIDINFO* RDIDINFO);

NTSTATUS InitPHYForHDMI(
	_In_ UINT64 PBaseAddr,
	_In_ CRTCTIMING* PCrTrTiming);

 NTSTATUS WriteSpace(
	_In_ UINT64 PBaseAddr,
	_In_ UINT32 BaseOffset,
	_In_ UINT32 Bit32InDate);

 UINT32 ReadSpace(
	_In_ UINT64 PBaseAddr,
	_In_ UINT32 BaseOffset);

 NTSTATUS GetHDMIEdidInform(
	_In_ UINT64 PRegMem,
	_In_ UINT32 PRegOffset,
	_In_ UINT32 EdidSize,
	_Inout_ UINT32* EdOut);

NTSTATUS GetMoniterEdid(
	_In_ UINT64 PRegBaseAddr,
	_Inout_ UINT32* MonitorEDID,
	_In_ UINT32 EdiLength);

NTSTATUS GetRegisterKeyValue(
	_In_ LPCWSTR KeyPath,
	_In_ LPCWSTR pKeyName,
	_Out_ UINT32* KeyValue);

NTSTATUS SetRegisterValue(
	_In_ LPCWSTR KeyPath,
	_In_ LPCWSTR pKeyName,
	_In_ UINT32 KeyValue);


#endif // _HW_HXX_