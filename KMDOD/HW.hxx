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
#ifdef __cplusplus
}
#endif

//平台定义 
#define BDD_DRIVCE_NAME            L"\\Device\\PrismDriver"
#define BDD_DEVICE_KMDSYNLINK      L"\\DosDevices\\PrismDriver"




#define BDD_OPLAT_NAME          "Prism GPU" 
#define BDD_OPLAT_NAME          "UNKNOWN"
#define BDD_ADAPTE_NAME         "UNKNOWN"
#define BDD_BIOS_NAME           "UNKNOWN"

#define BDD_DRIVER_PICE      (0) 
#define BDD_DRIVER_APIC      (1) 
#define BDD_DRIVER_PRESENT_MODE           BDD_DRIVER_PICE  

#define BDD_DRIVER_VENDORID    0x1414
#define BDD_DRIVER_DEVICE_ID   0x0010






#endif // _HW_HXX_
