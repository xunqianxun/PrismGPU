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
	h_CrTiTIming.PIEXLHEIGHT = EDIDINFO->VACTIVEPIXEL;
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
	UINT64 Offset = 0x700;

	UINT32 REG1 = 0x00000000 | (0x0000001f);
	UINT32 REG2 = (PCrTrTiming->PIEXLHEIGHT & 0x00000fff) | ((PCrTrTiming->PIEXLWIDTH & 0x00000fff) << 12);

	WriteSpace(PBaseAddr , Offset + 0 , REG1); 
	WriteSpace(PBaseAddr , Offset + 4 , REG2); 
	WriteSpace(PBaseAddr , Offset + 8 , 0);
	WriteSpace(PBaseAddr , Offset + 12, 0); 

	return State;
}


 NTSTATUS GetMoniterEdid(
	_In_ UINT64 PRegBaseAddr,
	_Inout_ UINT32* MonitorEDID,
	_In_ UINT32 EdiLength)
{
	NTSTATUS Status = { 0 };
	BYTE PhysicalEDID[128] = { 0 };

	for (size_t i = 0; i < (EdiLength / 4); i++)
	{
		UINT32 ReadDate = ReadSpace(PRegBaseAddr, 4 * i);
		PhysicalEDID[i * 4 + 0] = (BYTE)(ReadDate & 0x000f);
		PhysicalEDID[i * 4 + 1] = (BYTE)(ReadDate & 0x00f0 >> 8);
		PhysicalEDID[i * 4 + 2] = (BYTE)(ReadDate & 0x0f00 >> 16);
		PhysicalEDID[i * 4 + 3] = (BYTE)(ReadDate & 0xf000 >> 24);
	}

	RtlCopyMemory(MonitorEDID, PhysicalEDID, EdiLength);

	return Status;
}


