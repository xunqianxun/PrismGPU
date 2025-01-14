/***************************************************************\
	Author : Guoqi Li
	Date   : 2025/1/9
	Funcation: HeadWare define and handle
\***************************************************************/

#include "HW.hxx"

HWDEVICEINFO h_DEVICEINFO;

NTSTATUS AnalysizeEdid(
	_In_  BYTE* m_EDID,
   	_Inout_ HWEDIDINFO* h_EDID)
{
	h_EDID->FACID = m_EDID[0x08] << 8 | m_EDID[0x09];
	h_EDID->PRODUCTID = m_EDID[0x0b] << 8 | m_EDID[0x0a];
	h_EDID->SNCODE = (m_EDID[0x0f] << 24) | (m_EDID[0x0e] << 16) | (m_EDID[0x0d] << 8) | m_EDID[0x0c];
	h_EDID->PRODWEEK = m_EDID[0x10];
	h_EDID->PRODYEAR = m_EDID[0x11];
	h_EDID->VERSION = m_EDID[0x12];
	h_EDID->REVERSION = m_EDID[0x13];
	h_EDID->CLOCKKHZ = ((m_EDID[0x37] << 8) | m_EDID[0x36]) * 10;
	h_EDID->HACTIVEPIXEL = ((m_EDID[0x3a] >> 4) << 8) | m_EDID[0x38];
	h_EDID->HNOACTIVEPIXEL = ((m_EDID[0x3a] & 0xf) << 8) | m_EDID[0x39];
	h_EDID->VACTIVEPIXEL = ((m_EDID[0x3d] >> 4) << 8) | m_EDID[0x3b];
	h_EDID->VNOACTIVEPIXEL = ((m_EDID[0x3d] & 0xf) << 8) | m_EDID[0x3c];
	h_EDID->HFORNTPROCH = ((m_EDID[0x41] >> 6) << 8) | m_EDID[0x3e];
	h_EDID->HSYNCPULSE = (((m_EDID[0x41] >> 4) & 0x3) << 8) | m_EDID[0x3f];
	h_EDID->VFORNTPROCH = (((m_EDID[0x41] >> 2) & 0x3) << 4) | (m_EDID[0x40] >> 4);
	h_EDID->VSYNCPULSE = (m_EDID[0x41] & 0x3) | (m_EDID[0x40] & 0xf);
	h_EDID->HIMAGESIZE = ((m_EDID[0x44] >> 4) << 8) | m_EDID[0x42];
	h_EDID->VIMAGESIZE = ((m_EDID[0x44] & 0xf) << 8) | m_EDID[0x43];
	h_EDID->HBOARDER = m_EDID[0x45];
	h_EDID->VBOARDER = m_EDID[0x46];
	h_EDID->FFLAGE.INTERSECTSCAN = (m_EDID[0x47] >> 7);
	h_EDID->FFLAGE.STEREOMODE = ((m_EDID[0x47] >> 4) & 0x6) | (m_EDID[0x47] & 0x1);
	h_EDID->FFLAGE.SYNCTYPE = (m_EDID[0x47] >> 1) & 0xf;
	if (h_EDID->FFLAGE.SYNCTYPE & 0xc && h_EDID->FFLAGE.SYNCTYPE & 0x2) h_EDID->HSYNCSITUATION = 1;
	if (h_EDID->FFLAGE.SYNCTYPE & 0x8 && h_EDID->FFLAGE.SYNCTYPE & 0x1) h_EDID->VSYNCSITUATION = 1;

	h_EDID->HACTIVEPIXEL = DISP_HORIZON_PIXEL;
	h_EDID->VACTIVEPIXEL = DISP_VERTICAL_LINE;

	return TRUE;
}

//本质上来说不应该有这一步，因为在真正的GPU中的HDMI中一定是双向的一定是可以读取EDID信息的，在本设备中不具备这样的功能，因此采用这种方式来初始化设备。
//但是通过驱动来给设备提供显示的规格信息的函数是有的，当操作系统更改了输出分辨率，缩放情况等，显卡也会接收到这些信息对输出形式进行更改，当然那个也要借助EDID信息，
//因为系统对于分辨率的更改要在显示器的规定范围之内。
NTSTATUS InitHardware(
	_In_ DXGKRNL_INTERFACE* pDxgkInterface,
	_In_ UINT64             REGPBASE,
	_In_ HWEDIDINFO*         EDIDINFO)
{
	CRTCTIMING h_CrTiTIming;

	NTSTATUS State = { 0 };

	RtlZeroMemory(&h_CrTiTIming, sizeof(h_CrTiTIming));

	
	h_CrTiTIming.PIEXLWIDTH = EDIDINFO->HACTIVEPIXEL;
	h_CrTiTIming.PIEXLWIDTH = EDIDINFO->VACTIVEPIXEL;
	h_CrTiTIming.PIEXLCLOCKKHZ = EDIDINFO->CLOCKKHZ;

	h_CrTiTIming.HORIZONTOTAL = EDIDINFO->HACTIVEPIXEL + EDIDINFO->HNOACTIVEPIXEL;
	h_CrTiTIming.HORIZONBOARDLEFT = EDIDINFO->HBOARDER;
	h_CrTiTIming.HORIZONACTIVE = EDIDINFO->HACTIVEPIXEL;
	h_CrTiTIming.HORIZONBOARDRIGHT = EDIDINFO->HBOARDER;
	h_CrTiTIming.HORIZONFRONT = EDIDINFO->HFORNTPROCH;
	h_CrTiTIming.HORIZONSYNCWIDTH = EDIDINFO->HSYNCPULSE;
	h_CrTiTIming.VERTICALTOTAL = EDIDINFO->VACTIVEPIXEL + EDIDINFO->VNOACTIVEPIXEL;
	h_CrTiTIming.VERTICALBOARDTOP = EDIDINFO->VBOARDER;
	h_CrTiTIming.VERTICALACTIVE = EDIDINFO->VACTIVEPIXEL;
	h_CrTiTIming.VERTICALBOARDBOTTOM = EDIDINFO->VBOARDER;
	h_CrTiTIming.VERTICALFRONT = EDIDINFO->VFORNTPROCH;
	h_CrTiTIming.VERTICALSYNCWIDTH = EDIDINFO->HSYNCPULSE;

	h_CrTiTIming.FLAGS.HSYNCPOSTIVE = EDIDINFO->HSYNCSITUATION;
	h_CrTiTIming.FLAGS.VSYNCPOSTIVE = EDIDINFO->VSYNCSITUATION;

	h_CrTiTIming.FLAGS.INTERLANCE = EDIDINFO->FFLAGE.INTERSECTSCAN;
	

	RtlCopyMemory(&h_DEVICEINFO.CrTcTimg, &h_CrTiTIming, sizeof(h_CrTiTIming));



	State = InitPHYForHDMI(REGPBASE, &h_CrTiTIming);

	if (State != STATUS_SUCCESS)
	{
		DbgPrint("Init HardWare Fault\n");
	}
	return STATUS_SUCCESS;
}


 UINT32 ReadSpace(
	_In_ UINT64  PBaseAddr,
	_In_ UINT32  BaseOffset)
{

	return	*((volatile UINT32*)(PBaseAddr + BaseOffset));
}

 NTSTATUS WriteSpace(
	_In_ UINT64  PBaseAddr,
	_In_ UINT32  BaseOffset,
	_In_ UINT32  Bit32InDate)
{
	NTSTATUS State = STATUS_SUCCESS;

	*((volatile UINT32*)(PBaseAddr + BaseOffset)) = Bit32InDate;

	return State;
}


 NTSTATUS InitPHYForHDMI(
	_In_ UINT64 PBaseAddr,
	_In_ CRTCTIMING* PCrTrTiming)
{
	NTSTATUS State = STATUS_SUCCESS;

	WriteSpace(PBaseAddr, 0x0, 0x3);
	WriteSpace(PBaseAddr, 0x54, (DISP_HORIZON_PIXEL * DISP_PIXEL_SIZE));
	WriteSpace(PBaseAddr, 0x58, (DISP_HORIZON_PIXEL * DISP_PIXEL_SIZE));
	WriteSpace(PBaseAddr, 0x5C, 0x00000000);
	WriteSpace(PBaseAddr, 0x50, DISP_VERTICAL_LINE);


	return State;
}

//此处get EDID信息只能使用这种方式来做，因为目前硬件的HDMI接口还是单向的不能从moniter上接收EDID information
 NTSTATUS GetHDMIEdidInform(
	_In_ UINT64 PRegMem,
	_In_ UINT32 PRegOffset,
	_In_ UINT32 EdidSize,
	_Inout_ UINT32* EdOut)
{ 
	BYTE PhysicalEDID[256] = {

		0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x41,0x0C,0x5D,0x09,0x48,0x05,0x00,0x00,
		0x09,0x21,0x01,0x03,0x80,0x3C,0x22,0x78,0x2A,0xB3,0x75,0xAC,0x52,0x4E,0xA0,0x26,
		0x0F,0x50,0x54,0xBF,0xEF,0x00,0xD1,0xC0,0xB3,0x00,0x95,0x00,0x81,0x80,0x81,0x40,
		0x81,0xC0,0x01,0x01,0x01,0x01,0x56,0x5E,0x00,0xA0,0xA0,0xA0,0x29,0x50,0x30,0x20,
		0x35,0x00,0x55,0x50,0x21,0x00,0x00,0x1E,0x00,0x00,0x00,0xFF,0x00,0x55,0x48,0x42,
		0x32,0x33,0x30,0x39,0x30,0x30,0x31,0x33,0x35,0x32,0x00,0x00,0x00,0xFC,0x00,0x50,
		0x48,0x4C,0x20,0x32,0x37,0x36,0x42,0x39,0x0A,0x20,0x20,0x20,0x00,0x00,0x00,0xFD,
		0x00,0x30,0x4B,0x1E,0x72,0x1E,0x00,0x0A,0x20,0x20,0x20,0x20,0x20,0x20,0x01,0x1B,
		0x02,0x03,0x1E,0xF1,0x4B,0x10,0x1F,0x05,0x14,0x04,0x13,0x03,0x12,0x02,0x11,0x01,
		0x23,0x09,0x07,0x07,0x83,0x01,0x00,0x00,0x65,0x03,0x0C,0x00,0x10,0x00,0xA0,0x73,
		0x00,0x6A,0xA0,0xA0,0x29,0x50,0x08,0x20,0x35,0x00,0x55,0x50,0x21,0x00,0x00,0x1A,
		0x2A,0x44,0x80,0xA0,0x70,0x38,0x27,0x40,0x30,0x20,0x35,0x00,0x55,0x50,0x21,0x00,
		0x00,0x1A,0x02,0x3A,0x80,0x18,0x71,0x38,0x2D,0x40,0x58,0x2C,0x45,0x00,0x55,0x50,
		0x21,0x00,0x00,0x1E,0x01,0x1D,0x00,0x72,0x51,0xD0,0x1E,0x20,0x6E,0x28,0x55,0x00,
		0x55,0x50,0x21,0x00,0x00,0x1E,0xF0,0x3C,0x00,0xD0,0x51,0xA0,0x35,0x50,0x60,0x88,
		0x3A,0x00,0x55,0x50,0x21,0x00,0x00,0x1C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x97
	};

	RtlCopyMemory(EdOut, PhysicalEDID, EdidSize);

	return STATUS_SUCCESS;
}


 NTSTATUS GetMoniterEdid(
	_In_ UINT64 PRegBaseAddr,
	_Inout_ UINT32* MonitorEDID,
	_In_ UINT32 EdiLength)
{
	NTSTATUS Status = { 0 };

	Status = GetHDMIEdidInform(PRegBaseAddr, 0, EdiLength, MonitorEDID);

	return Status;
}


//NTSTATUS GetRegisterKeyValue(
//	_In_ LPCWSTR KeyPath,
//	_In_ LPCWSTR pKeyName,
//	_Out_ UINT32* KeyValue)
//{
//	NTSTATUS State = STATUS_SUCCESS;
//
//	UNICODE_STRING KeyPathName;
//
//	RtlInitUnicodeString(&KeyPathName, KeyPath);
//
//	HANDLE HandleRegister;
//	OBJECT_ATTRIBUTES ObjectAttr = { 0 };
//
//	InitializeObjectAttributes(&ObjectAttr, &KeyPathName, OBJ_CASE_INSENSITIVE, NULL, NULL);
//
//	State = ZwOpenKey(&HandleRegister, KEY_ALL_ACCESS, &ObjectAttr);
//
//	if (State != STATUS_SUCCESS)
//	{
//		return State;
//	}
//
//	UNICODE_STRING KeyValueName;
//
//	RtlInitUnicodeString(&KeyValueName, pKeyName);
//
//	ULONG KeySize;
//
//	State = ZwQueryValueKey(HandleRegister, &KeyValueName, KeyValuePartialInformation, NULL, 0, &KeySize);
//
//	if ((State != STATUS_SUCCESS) || (KeySize == 0))
//	{
//		DbgPrint("query register size information fault\n");
//	}
//
//	else
//	{
//		PKEY_VALUE_PARTIAL_INFORMATION KeyValueInform;
//
//		KeyValueInform = (PKEY_VALUE_PARTIAL_INFORMATION)ExAllocatePool(PagedPool, KeySize);
//
//		State = ZwQueryValueKey(HandleRegister, &KeyValueName, KeyValuePartialInformation, KeyValueInform, KeySize, &KeySize);
//
//		if (State != STATUS_SUCCESS)
//		{
//			DbgPrint("query register information fault\n");
//		}
//		else
//		{
//			switch (KeyValueInform->Type)
//			{
//			case REG_BINARY:
//			case REG_DWORD:
//				KeyValue[0] = *(PUINT32)KeyValueInform->Data; //这里的KeyValue[0]，相当于把这个指针实体化了
//				break;
//			case REG_SZ:
//				if ((KeyValueInform->Data[0] = '0') && ((KeyValueInform->Data[0] = 'x') || (KeyValueInform->Data[0] = 'X')))
//				{
//					KeyValue[0] = strtol((char*)KeyValueInform->Data, NULL, 16);
//				}
//				else
//				{
//					KeyValue[0] = strtol((char*)KeyValueInform->Data, NULL, 10);
//				}
//				break;
//			default:
//				DbgPrint("register key type desn't define\n");
//				KeyValue[0] = { 0 };
//				break;
//			}
//		}
//		if (KeyValueInform != NULL)
//		{
//			ExFreePool(KeyValueInform);
//		}
//	}
//	ZwClose(HandleRegister);
//
//	return State;
//
//}
//
//NTSTATUS SetRegisterValue(
//	_In_ LPCWSTR KeyPath,
//	_In_ LPCWSTR pKeyName,
//	_In_ UINT32 KeyValue)
//{
//	NTSTATUS State = STATUS_SUCCESS;
//
//	UNICODE_STRING KeyPathName;
//
//	RtlInitUnicodeString(&KeyPathName, KeyPath);
//	
//	HANDLE HandleRegister;
//	OBJECT_ATTRIBUTES ObjectAttr = { 0 };
//	ULONG ReturnInform;
//
//	InitializeObjectAttributes(&ObjectAttr, &KeyPathName, OBJ_CASE_INSENSITIVE, NULL, NULL);
//	//      打开或创建一个指定的注册表键
//	State = ZwCreateKey(&HandleRegister, KEY_ALL_ACCESS, &ObjectAttr, 0, NULL, REG_OPTION_NON_VOLATILE, &ReturnInform);
//
//	if (State != STATUS_SUCCESS)
//	{
//		DbgPrint("register key open fault\n");
//
//		return State;
//	}
//
//	UNICODE_STRING KeyName;
//
//	RtlInitUnicodeString(&KeyName, pKeyName);
//
//	ZwSetValueKey(HandleRegister, &KeyName, 0, REG_DWORD, &KeyValue, sizeof(KeyValue));
//
//	ZwClose(HandleRegister);
//
//}


//NTSTATUS CreatDevice(
//	_In_ PDRIVER_OBJECT pDeiverObject ,
//	_In_ LPCWSTR        pDeviceName,
//	_In_ LPCWSTR        pDeviceSymbleLink,
//	_Out_
//)



