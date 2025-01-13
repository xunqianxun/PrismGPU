/***************************************************************\
	Author : Guoqi Li
	Date   : 2025/1/9
	Funcation: HeadWare define and handle
\***************************************************************/

#include "HW.hxx"
#include "bdd.hxx"
#include <minwindef.h>

static int
ANALYZEEDID(
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