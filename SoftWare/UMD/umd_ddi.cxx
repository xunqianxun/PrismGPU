/***************************************************************\
	Author : Guoqi Li
	Date   : 2025/1/9
	Funcation: User mode DDI
\***************************************************************/

#include<Process.h>
#include<minwindef.h>



HINSTANCE g_hDLL;

BOOL WINAPI DllMain(
	HINSTANCE hinstDLL,  // handle to DLL module
	DWORD fwReason,     // reason for calling function
	LPVOID lpvReserved)  // reserved
{
    switch (fwReason)
    {
    case(DLL_PROCESS_ATTACH):
    {
        g_hDLL = hinstDLL;
    } break;

    case(DLL_PROCESS_DETACH):
    {
        g_hDLL = NULL;
        return TRUE;
    }

    default: break;
    }

    return TRUE;
}