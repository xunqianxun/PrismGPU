/***************************************************************\
	Author : Guoqi Li 
	Date   : 2025/1/9 
	Funcation: HeadWare define head file 
\***************************************************************/

#ifndef _HW_HXX_
#define _HW_HXX_
#include <basetsd.h>
#include <sal.h>


#ifdef __cplusplus
extern "C" {
#endif
#ifdef __cplusplus
}
#endif

//平台定义 
#define BDD_DRIVCE_NAME            L"\\Device\\PrismDriver"
#define BDD_DEVICE_KMDSYNLINK      L"\\DosDevices\\PrismDriver"




#define BDD_OPLAT_NAME          "Prism GPU" 
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

typedef struct {
	UINT16 FACID;
	UINT16 PRODUCTID;
	UINT32 SNCODE;
	UINT8  PRODWEEK;
	UINT8  PRODYEAR;
	UINT8  VERSION;
	UINT8  REVERSION;
	UINT64 CLOCKKHZ;
	UINT64 HACTIVEPIXEL;
	UINT64 HNOACTIVEPIXEL;
	UINT64 VACTIVEPIXEL;
	UINT64 VNOACTIVEPIXEL;
	UINT64 HFORNTPROCH;
	UINT64 HSYNCPULSE;
	UINT64 VFORNTPROCH;
	UINT64 VSYNCPULSE;
	UINT16 HIMAGESIZE;
	UINT16 VIMAGESIZE;
	UINT16 HBOARDER;
	UINT16 VBOARDER;
	UINT8  HSYNCSITUATION;
	UINT8  VSYNCSITUATION;
	struct {
		UINT8 INTERSECTSCAN;
		UINT8 STEREOMODE;
		UINT8 SYNCTYPE;
	}FFLAGE;
}HWEDIDINFO;

typedef struct {
	UINT64 REGPBASE;
	UINT64 MEMPBASE;
	HWEDIDINFO h_EDIDINFO;


}HWDEVICEINFO;

extern HWDEVICEINFO h_DEVICEINFO;

//
// hw funcation define 
//


static int
ANALYZEEDID(
	_In_  BYTE* m_EDID,
	_Inout_ HWEDIDINFO* h_EDID);



#endif // _HW_HXX_