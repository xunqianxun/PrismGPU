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

//ƽ̨���� 
#define BDD_DRIVCE_NAME            L"\\Device\\PrismDriver"
#define BDD_DEVICE_KMDSYNLINK      L"\\DosDevices\\PrismDriver"

//����\Ϊ��������궨�壬��Ϊ�궨��һ�㶼��һ�У�ʹ��\�����Ӷ��С�
//...��ʾ�������͵Ĳ�����ʲô���ͶԵĲ���������
//do while(0)����ִֻ��һ��

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

#define BDD_DRIVER_MAXCHILD 1 //��ʱʹ�ù�Ԥ����ķ�ʽ

#define DISP_HORIZON_PIXEL		(1280)
#define DISP_VERTICAL_LINE		(720)
#define DISP_PIXEL_SIZE			(4)

#define REGISTER_PATH     L"\\Registry\\Machine\\Software\\KMDOD\\UserParams"


typedef struct
{
	UINT32 PIEXLWIDTH;
	UINT32 PIEXLHEIGHT;
	UINT32 HORIZONTOTAL; //ˮƽɨ������
	UINT32 HORIZONBOARDLEFT; //��߿�������
	UINT32 HORIZONACTIVE; //����ʾ��ˮƽ������
	UINT32 HORIZONBOARDRIGHT; //�ұ߿�������
	UINT32 HORIZONFRONT;
	UINT32 HORIZONSYNCWIDTH; //ˮƽ������

	UINT32 VERTICALTOTAL;
	UINT32 VERTICALBOARDTOP;
	UINT32 VERTICALACTIVE;
	UINT32 VERTICALBOARDBOTTOM;
	UINT32 VERTICALFRONT;
	UINT32 VERTICALSYNCWIDTH;
	UINT32 PIEXLCLOCKKHZ;
	struct
	{
		UINT32 INTERLANCE : 1; //��ʶ֧�ֽ���ɨ��
		UINT32 DOUBLESCAN : 1; //����˫ɨ��ģʽ
		UINT32 PIEXLREPRTITION : 4; //��ʾʱ���ص��ظ�����
		UINT32 HSYNCPOSTIVE : 1; //ˮƽ������
		UINT32 VSYNCPOSTIVE : 1; //��ֱ������
		UINT32 EXCLUSIVE3D : 1; //ֻ֧��3dģʽ
		UINT32 SUBSIMPLE3D : 1;
		UINT32 USEIN3DONLY : 1; //����3Dʹ��
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
	UINT64 HACTIVEPIXEL;// ˮƽ��Ч����������ʾ����Ŀ�ȣ���λ�����أ�
	UINT64 HNOACTIVEPIXEL;// ˮƽ�հ���������������ʾ������һ֡��ʼ�Ŀհ����򣬵�λ�����أ�
	UINT64 VACTIVEPIXEL;// ��ֱ��Ч����������ʾ����ĸ߶ȣ���λ�����أ�
	UINT64 VNOACTIVEPIXEL;// ��ֱ�հ���������������ʾ������һ֡��ʼ�Ŀհ����򣬵�λ�����أ�
	UINT64 HFORNTPROCH;// ˮƽǰ�أ�ˮƽͬ���źŵ�ǰ��ʱ�䣬��λ�����أ�
	UINT64 HSYNCPULSE; // ˮƽͬ�������ȣ�ˮƽͬ���źŵĳ���ʱ�䣬��λ�����أ�
	UINT64 VFORNTPROCH;// ��ֱǰ�أ���ֱͬ���źŵ�ǰ��ʱ�䣬��λ�����أ�
	UINT64 VSYNCPULSE;// ��ֱͬ�������ȣ���ֱͬ���źŵĳ���ʱ�䣬��λ�����أ�
	UINT16 HIMAGESIZE; // ��ʾ����ˮƽͼ��ߴ磨��λ�����ף�
	UINT16 VIMAGESIZE;// ��ʾ���Ĵ�ֱͼ��ߴ磨��λ�����ף�
	UINT16 HBOARDER; // ˮƽ�߿��ȣ���λ�����أ�
	UINT16 VBOARDER; // ��ֱ�߿��ȣ���λ�����أ�
	UINT8  HSYNCSITUATION; // ˮƽͬ���źŵļ��ԣ�1��ʾ�����ԣ�0��ʾ�����ԣ�
	UINT8  VSYNCSITUATION;// ��ֱͬ���źŵļ��ԣ�1��ʾ�����ԣ�0��ʾ�����ԣ�
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