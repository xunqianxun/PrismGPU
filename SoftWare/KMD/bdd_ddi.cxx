﻿/******************************Module*Header*******************************\
* Module Name: BDD_DDI.cxx
*
* Basic Display Driver DDI entry points redirects
*
*
* Copyright (c) 2010 Microsoft Corporation
\**************************************************************************/

#include "BDD.hxx"


#pragma code_seg(push)
#pragma code_seg("INIT")
// BEGIN: Init Code

//
// Driver Entry point
//
//VOID KmdUnload(DRIVER_OBJECT* pdevice) {
//
//    //BddDdiUnload();
//
//    if (pdevice->DeviceObject) {
//        IoDeleteDevice(pdevice->DeviceObject);
//    }
//
//    DbgPrint("卸载KMD成功\n");
//}

extern "C"
NTSTATUS
DriverEntry(
    _In_  DRIVER_OBJECT*  pDriverObject,
    _In_  UNICODE_STRING* pRegistryPath) //代表了驱动在注册表中的参数所存放的位置
{
    PAGED_CODE();

    /*DbgPrint("安装KMD成功\n");

    pDriverObject->DriverUnload = KmdUnload;*/

    // Initialize DDI function pointers and dxgkrnl
    KMDDOD_INITIALIZATION_DATA InitialData = {0};

    InitialData.Version = DXGKDDI_INTERFACE_VERSION;

    //系统IRP回调及WDDM框架相关
    InitialData.DxgkDdiAddDevice                    = BddDdiAddDevice;     
    InitialData.DxgkDdiStartDevice                  = BddDdiStartDevice;
    InitialData.DxgkDdiStopDevice                   = BddDdiStopDevice;
    InitialData.DxgkDdiResetDevice                  = BddDdiResetDevice;
    InitialData.DxgkDdiRemoveDevice                 = BddDdiRemoveDevice;
    InitialData.DxgkDdiDispatchIoRequest            = BddDdiDispatchIoRequest;
    InitialData.DxgkDdiInterruptRoutine             = BddDdiInterruptRoutine;
    InitialData.DxgkDdiDpcRoutine                   = BddDdiDpcRoutine;
    InitialData.DxgkDdiControlInterrupt             = BddDdiControlInyerrupt;
    InitialData.DxgkDdiGetScanLine                  = BddDdiGetscanline;

    InitialData.DxgkDdiQueryChildRelations          = BddDdiQueryChildRelations;
    InitialData.DxgkDdiQueryChildStatus             = BddDdiQueryChildStatus;
    InitialData.DxgkDdiQueryDeviceDescriptor        = BddDdiQueryDeviceDescriptor;
    InitialData.DxgkDdiSetPowerState                = BddDdiSetPowerState;
    InitialData.DxgkDdiUnload                       = BddDdiUnload;
    InitialData.DxgkDdiQueryAdapterInfo             = BddDdiQueryAdapterInfo;
    InitialData.DxgkDdiSetPointerPosition           = BddDdiSetPointerPosition;
    InitialData.DxgkDdiSetPointerShape              = BddDdiSetPointerShape;
    InitialData.DxgkDdiIsSupportedVidPn             = BddDdiIsSupportedVidPn;
    InitialData.DxgkDdiRecommendFunctionalVidPn     = BddDdiRecommendFunctionalVidPn;
    InitialData.DxgkDdiEnumVidPnCofuncModality      = BddDdiEnumVidPnCofuncModality;
    InitialData.DxgkDdiSetVidPnSourceVisibility     = BddDdiSetVidPnSourceVisibility;
    InitialData.DxgkDdiCommitVidPn                  = BddDdiCommitVidPn;
    InitialData.DxgkDdiUpdateActiveVidPnPresentPath = BddDdiUpdateActiveVidPnPresentPath;
    InitialData.DxgkDdiRecommendMonitorModes        = BddDdiRecommendMonitorModes;
    InitialData.DxgkDdiQueryVidPnHWCapability       = BddDdiQueryVidPnHWCapability;
    InitialData.DxgkDdiPresentDisplayOnly           = BddDdiPresentDisplayOnly;
    InitialData.DxgkDdiStopDeviceAndReleasePostDisplayOwnership = BddDdiStopDeviceAndReleasePostDisplayOwnership;
    InitialData.DxgkDdiSystemDisplayEnable          = BddDdiSystemDisplayEnable;
    InitialData.DxgkDdiSystemDisplayWrite           = BddDdiSystemDisplayWrite;

    NTSTATUS Status = DxgkInitializeDisplayOnlyDriver(pDriverObject, pRegistryPath, &InitialData);
    if (!NT_SUCCESS(Status))
    {
        BDD_LOG_ERROR1("DxgkInitializeDisplayOnlyDriver failed with Status: 0x%I64x", Status);
        return Status;
    }

    //TODO:明天添加注册表相关的函数

    return Status;
}
// END: Init Code
#pragma code_seg(pop)

#pragma code_seg(push)
#pragma code_seg("PAGE")

//
// PnP DDIs
//





NTSTATUS
APIENTRY
BddDdiControlInyerrupt(
    _In_ CONST HANDLE                      hAdapter,
    _In_ CONST DXGK_INTERRUPT_TYPE         InterruptType, 
    _In_       BOOLEAN                     EnableInterrupt) 
{

    return STATUS_SUCCESS;

}

NTSTATUS
APIENTRY
BddDdiGetscanline(
    _In_    CONST HANDLE               hAdapter,
    _Inout_       DXGKARG_GETSCANLINE* pGetScanLine )
{

    return STATUS_SUCCESS;
}


VOID
BddDdiUnload(VOID)
{
    PAGED_CODE();
}



NTSTATUS
BddDdiAddDevice(
    _In_ DEVICE_OBJECT* pPhysicalDeviceObject,
    _Outptr_ PVOID*  ppDeviceContext)
{
    PAGED_CODE();

    if ((pPhysicalDeviceObject == NULL) ||
        (ppDeviceContext == NULL))
    {
        BDD_LOG_ERROR2("One of pPhysicalDeviceObject (0x%I64x), ppDeviceContext (0x%I64x) is NULL",
                        pPhysicalDeviceObject, ppDeviceContext);
        return STATUS_INVALID_PARAMETER;
    }
    *ppDeviceContext = NULL;

    BASIC_DISPLAY_DRIVER* pBDD = new(NonPagedPoolNx) BASIC_DISPLAY_DRIVER(pPhysicalDeviceObject);
    if (pBDD == NULL)
    {
        BDD_LOG_LOW_RESOURCE0("pBDD failed to be allocated");
        return STATUS_NO_MEMORY;
    }

    *ppDeviceContext = pBDD;

    return STATUS_SUCCESS;   //此函数实质上就是在为PDO设备分配一个上下文空间
}

NTSTATUS
BddDdiRemoveDevice(
    _In_  VOID* pDeviceContext)
{
    PAGED_CODE();

    BASIC_DISPLAY_DRIVER* pBDD = reinterpret_cast<BASIC_DISPLAY_DRIVER*>(pDeviceContext);

    if (pBDD)
    {
        delete pBDD;
        pBDD = NULL;
    }

    return STATUS_SUCCESS; //此函数用于delete已经创建的设备
}

NTSTATUS
BddDdiStartDevice(
    _In_  VOID*              pDeviceContext,
    _In_  DXGK_START_INFO*   pDxgkStartInfo,//DXGK_START_INFO结构体成员变量RequiredDmaQueueEntry保存了需要分配的DMA队列
                                            //预分配BUFFER的数量。 AdapterGuid和AdapterLuid则为适配器的标识。
    _In_  DXGKRNL_INTERFACE* pDxgkInterface,//通过系统框架间接获取硬件资源信息,其另外一部分功能是与系统框架驱动的通信。
    _Out_ ULONG*             pNumberOfViews,//为KMOD需要返回给系统框架的视频源数量。
    _Out_ ULONG*             pNumberOfChildren)//为在显卡上枚举到的显示器数量。
{
    PAGED_CODE();
    BDD_ASSERT_CHK(pDeviceContext != NULL);

    BASIC_DISPLAY_DRIVER* pBDD = reinterpret_cast<BASIC_DISPLAY_DRIVER*>(pDeviceContext);
    return pBDD->StartDevice(pDxgkStartInfo, pDxgkInterface, pNumberOfViews, pNumberOfChildren);
} //启动设备

NTSTATUS
BddDdiStopDevice(
    _In_  VOID* pDeviceContext)
{
    PAGED_CODE();
    BDD_ASSERT_CHK(pDeviceContext != NULL);

    BASIC_DISPLAY_DRIVER* pBDD = reinterpret_cast<BASIC_DISPLAY_DRIVER*>(pDeviceContext);
    return pBDD->StopDevice();
} //关闭设备


NTSTATUS
BddDdiDispatchIoRequest(
    _In_  VOID*                 pDeviceContext,
    _In_  ULONG                 VidPnSourceId,
    _In_  VIDEO_REQUEST_PACKET* pVideoRequestPacket)
{
    PAGED_CODE();
    BDD_ASSERT_CHK(pDeviceContext != NULL);

    BASIC_DISPLAY_DRIVER* pBDD = reinterpret_cast<BASIC_DISPLAY_DRIVER*>(pDeviceContext);
    if (!pBDD->IsDriverActive())
    {
        BDD_LOG_ASSERTION1("BDD (0x%I64x) is being called when not active!", pBDD);
        return STATUS_UNSUCCESSFUL;
    }
    return pBDD->DispatchIoRequest(VidPnSourceId, pVideoRequestPacket);
}//图形驱动的IO设备请求

NTSTATUS
BddDdiSetPowerState(
    _In_  VOID*              pDeviceContext,
    _In_  ULONG              HardwareUid,
    _In_  DEVICE_POWER_STATE DevicePowerState,
    _In_  POWER_ACTION       ActionType)
{
    PAGED_CODE();
    BDD_ASSERT_CHK(pDeviceContext != NULL);

    BASIC_DISPLAY_DRIVER* pBDD = reinterpret_cast<BASIC_DISPLAY_DRIVER*>(pDeviceContext);
    if (!pBDD->IsDriverActive())
    {
        // If the driver isn't active, SetPowerState can still be called, however in BDD's case
        // this shouldn't do anything, as it could for instance be called on BDD Fallback after
        // Fallback has been stopped and BDD PnP is being started. Fallback doesn't have control
        // of the hardware in this case.
        return STATUS_SUCCESS;
    }
    return pBDD->SetPowerState(HardwareUid, DevicePowerState, ActionType);
}

NTSTATUS
BddDdiQueryChildRelations(
    _In_                             VOID*                  pDeviceContext,
    _Out_writes_bytes_(ChildRelationsSize) DXGK_CHILD_DESCRIPTOR* pChildRelations,
    _In_                             ULONG                  ChildRelationsSize)
{
    PAGED_CODE();
    BDD_ASSERT_CHK(pDeviceContext != NULL);

    BASIC_DISPLAY_DRIVER* pBDD = reinterpret_cast<BASIC_DISPLAY_DRIVER*>(pDeviceContext);
    return pBDD->QueryChildRelations(pChildRelations, ChildRelationsSize);
}//枚举子设备函数

NTSTATUS
BddDdiQueryChildStatus(
    _In_    VOID*              pDeviceContext,
    _Inout_ DXGK_CHILD_STATUS* pChildStatus,
    _In_    BOOLEAN            NonDestructiveOnly)
{
    PAGED_CODE();
    BDD_ASSERT_CHK(pDeviceContext != NULL);

    BASIC_DISPLAY_DRIVER* pBDD = reinterpret_cast<BASIC_DISPLAY_DRIVER*>(pDeviceContext);
    return pBDD->QueryChildStatus(pChildStatus, NonDestructiveOnly);
}//子设备状态函数

NTSTATUS
BddDdiQueryDeviceDescriptor(
    _In_  VOID*                     pDeviceContext,
    _In_  ULONG                     ChildUid,
    _Inout_ DXGK_DEVICE_DESCRIPTOR* pDeviceDescriptor)
{
    PAGED_CODE();
    BDD_ASSERT_CHK(pDeviceContext != NULL);

    BASIC_DISPLAY_DRIVER* pBDD = reinterpret_cast<BASIC_DISPLAY_DRIVER*>(pDeviceContext);
    if (!pBDD->IsDriverActive())
    {
        // During stress testing of PnPStop, it is possible for BDD Fallback to get called to start then stop in quick succession.
        // The first call queues a worker thread item indicating that it now has a child device, the second queues a worker thread
        // item that it no longer has any child device. This function gets called based on the first worker thread item, but after
        // the driver has been stopped. Therefore instead of asserting like other functions, we only warn.
        BDD_LOG_WARNING1("BDD (0x%I64x) is being called when not active!", pBDD);
        return STATUS_UNSUCCESSFUL;
    }
    return pBDD->QueryDeviceDescriptor(ChildUid, pDeviceDescriptor);
}//子设备描述函数


//
// WDDM Display Only Driver DDIs
//

NTSTATUS
APIENTRY //apientry则表明此函数是应用程序的入口点，相当于c的main()函数。
BddDdiQueryAdapterInfo(
    _In_ CONST HANDLE                    hAdapter,
    _In_ CONST DXGKARG_QUERYADAPTERINFO* pQueryAdapterInfo)
{
    PAGED_CODE();
    BDD_ASSERT_CHK(hAdapter != NULL);

    BASIC_DISPLAY_DRIVER* pBDD = reinterpret_cast<BASIC_DISPLAY_DRIVER*>(hAdapter);
    return pBDD->QueryAdapterInfo(pQueryAdapterInfo);
}//获取GPU硬件的信息

NTSTATUS
APIENTRY
BddDdiSetPointerPosition(
    _In_ CONST HANDLE                      hAdapter,
    _In_ CONST DXGKARG_SETPOINTERPOSITION* pSetPointerPosition)
{
    PAGED_CODE();
    BDD_ASSERT_CHK(hAdapter != NULL);

    BASIC_DISPLAY_DRIVER* pBDD = reinterpret_cast<BASIC_DISPLAY_DRIVER*>(hAdapter);
    if (!pBDD->IsDriverActive())
    {
        BDD_LOG_ASSERTION1("BDD (0x%I64x) is being called when not active!", pBDD);
        return STATUS_UNSUCCESSFUL;
    }
    return pBDD->SetPointerPosition(pSetPointerPosition);
} //函数设置鼠标指针的位置和可见性状态。

NTSTATUS
APIENTRY
BddDdiSetPointerShape(
    _In_ CONST HANDLE                   hAdapter,
    _In_ CONST DXGKARG_SETPOINTERSHAPE* pSetPointerShape)
{
    PAGED_CODE();
    BDD_ASSERT_CHK(hAdapter != NULL);

    BASIC_DISPLAY_DRIVER* pBDD = reinterpret_cast<BASIC_DISPLAY_DRIVER*>(hAdapter);
    if (!pBDD->IsDriverActive())
    {
        BDD_LOG_ASSERTION1("BDD (0x%I64x) is being called when not active!", pBDD);
        return STATUS_UNSUCCESSFUL;
    }
    return pBDD->SetPointerShape(pSetPointerShape);
}//函数设置鼠标指针的外观和位置。




NTSTATUS
APIENTRY
BddDdiPresentDisplayOnly(
    _In_ CONST HANDLE                       hAdapter,
    _In_ CONST DXGKARG_PRESENT_DISPLAYONLY* pPresentDisplayOnly)
{
    PAGED_CODE();
    BDD_ASSERT_CHK(hAdapter != NULL);

    BASIC_DISPLAY_DRIVER* pBDD = reinterpret_cast<BASIC_DISPLAY_DRIVER*>(hAdapter);
    if (!pBDD->IsDriverActive())
    {
        BDD_LOG_ASSERTION1("BDD (0x%I64x) is being called when not active!", pBDD);
        return STATUS_UNSUCCESSFUL;
    }
    return pBDD->PresentDisplayOnly(pPresentDisplayOnly);
}//将屏幕图像呈现给内核模式仅显示驱动程序 （KMDOD） 的显示设备。

NTSTATUS
APIENTRY
BddDdiStopDeviceAndReleasePostDisplayOwnership(
    _In_  VOID*                          pDeviceContext,
    _In_  D3DDDI_VIDEO_PRESENT_TARGET_ID TargetId,
    _Out_ DXGK_DISPLAY_INFORMATION*      DisplayInfo)
{
    PAGED_CODE();
    BDD_ASSERT_CHK(pDeviceContext != NULL);

    BASIC_DISPLAY_DRIVER* pBDD = reinterpret_cast<BASIC_DISPLAY_DRIVER*>(pDeviceContext);
    return pBDD->StopDeviceAndReleasePostDisplayOwnership(TargetId, DisplayInfo);
}//由操作系统调用，以请求显示微型端口驱动程序重置显示设备并释放当前开机自检 （POST） 设备的所有权。

NTSTATUS
APIENTRY
BddDdiIsSupportedVidPn(
    _In_ CONST HANDLE                 hAdapter,
    _Inout_ DXGKARG_ISSUPPORTEDVIDPN* pIsSupportedVidPn)
{
    PAGED_CODE();
    BDD_ASSERT_CHK(hAdapter != NULL);

    BASIC_DISPLAY_DRIVER* pBDD = reinterpret_cast<BASIC_DISPLAY_DRIVER*>(hAdapter);
    if (!pBDD->IsDriverActive())
    {
        // This path might hit because win32k/dxgport doesn't check that an adapter is active when taking the adapter lock.
        // The adapter lock is the main thing BDD Fallback relies on to not be called while it's inactive. It is still a rare
        // timing issue around PnpStart/Stop and isn't expected to have any effect on the stability of the system.
        BDD_LOG_WARNING1("BDD (0x%I64x) is being called when not active!", pBDD);
        return STATUS_UNSUCCESSFUL;
    }
    return pBDD->IsSupportedVidPn(pIsSupportedVidPn);
}//指示 VidPN 是受支持 （TRUE） 还是不支持 （FALSE）。

NTSTATUS
APIENTRY
BddDdiRecommendFunctionalVidPn(
    _In_ CONST HANDLE                                  hAdapter,
    _In_ CONST DXGKARG_RECOMMENDFUNCTIONALVIDPN* CONST pRecommendFunctionalVidPn)
{
    PAGED_CODE();
    BDD_ASSERT_CHK(hAdapter != NULL);

    BASIC_DISPLAY_DRIVER* pBDD = reinterpret_cast<BASIC_DISPLAY_DRIVER*>(hAdapter);
    if (!pBDD->IsDriverActive())
    {
        BDD_LOG_ASSERTION1("BDD (0x%I64x) is being called when not active!", pBDD);
        return STATUS_UNSUCCESSFUL;
    }
    return pBDD->RecommendFunctionalVidPn(pRecommendFunctionalVidPn);
}//函数创建可在指定显示适配器上实现的功能性 VidPN。所谓VidPN就是 视频呈网状管理器。

NTSTATUS
APIENTRY
BddDdiRecommendVidPnTopology(
    _In_ CONST HANDLE                                 hAdapter,
    _In_ CONST DXGKARG_RECOMMENDVIDPNTOPOLOGY* CONST  pRecommendVidPnTopology)
{
    PAGED_CODE();
    BDD_ASSERT_CHK(hAdapter != NULL);

    BASIC_DISPLAY_DRIVER* pBDD = reinterpret_cast<BASIC_DISPLAY_DRIVER*>(hAdapter);
    if (!pBDD->IsDriverActive())
    {
        BDD_LOG_ASSERTION1("BDD (0x%I64x) is being called when not active!", pBDD);
        return STATUS_UNSUCCESSFUL;
    }
    return pBDD->RecommendVidPnTopology(pRecommendVidPnTopology);
}

NTSTATUS
APIENTRY
BddDdiRecommendMonitorModes(
    _In_ CONST HANDLE                                hAdapter,
    _In_ CONST DXGKARG_RECOMMENDMONITORMODES* CONST  pRecommendMonitorModes)
{
    PAGED_CODE();
    BDD_ASSERT_CHK(hAdapter != NULL);

    BASIC_DISPLAY_DRIVER* pBDD = reinterpret_cast<BASIC_DISPLAY_DRIVER*>(hAdapter);
    if (!pBDD->IsDriverActive())
    {
        BDD_LOG_ASSERTION1("BDD (0x%I64x) is being called when not active!", pBDD);
        return STATUS_UNSUCCESSFUL;
    }
    return pBDD->RecommendMonitorModes(pRecommendMonitorModes);
}//函数检查与特定视频当前目标关联的监视器源模式集，并可能将模式添加到该集。

NTSTATUS
APIENTRY
BddDdiEnumVidPnCofuncModality(
    _In_ CONST HANDLE                                 hAdapter,
    _In_ CONST DXGKARG_ENUMVIDPNCOFUNCMODALITY* CONST pEnumCofuncModality)
{
    PAGED_CODE();
    BDD_ASSERT_CHK(hAdapter != NULL);

    BASIC_DISPLAY_DRIVER* pBDD = reinterpret_cast<BASIC_DISPLAY_DRIVER*>(hAdapter);
    if (!pBDD->IsDriverActive())
    {
        BDD_LOG_ASSERTION1("BDD (0x%I64x) is being called when not active!", pBDD);
        return STATUS_UNSUCCESSFUL;
    }
    return pBDD->EnumVidPnCofuncModality(pEnumCofuncModality);
}//函数使 VidPN 的源模式集和目标模式集与 VidPN 的拓扑和已固定的模式协同运行。

NTSTATUS
APIENTRY
BddDdiSetVidPnSourceVisibility(
    _In_ CONST HANDLE                            hAdapter,
    _In_ CONST DXGKARG_SETVIDPNSOURCEVISIBILITY* pSetVidPnSourceVisibility)
{
    PAGED_CODE();
    BDD_ASSERT_CHK(hAdapter != NULL);

    BASIC_DISPLAY_DRIVER* pBDD = reinterpret_cast<BASIC_DISPLAY_DRIVER*>(hAdapter);
    if (!pBDD->IsDriverActive())
    {
        BDD_LOG_ASSERTION1("BDD (0x%I64x) is being called when not active!", pBDD);
        return STATUS_UNSUCCESSFUL;
    }
    return pBDD->SetVidPnSourceVisibility(pSetVidPnSourceVisibility);
}//函数对与指定视频演示源关联的视频输出编解码器进行编程，以开始扫描或停止扫描源的主图面

NTSTATUS
APIENTRY
BddDdiCommitVidPn(
    _In_ CONST HANDLE                     hAdapter,
    _In_ CONST DXGKARG_COMMITVIDPN* CONST pCommitVidPn)
{
    PAGED_CODE();
    BDD_ASSERT_CHK(hAdapter != NULL);

    BASIC_DISPLAY_DRIVER* pBDD = reinterpret_cast<BASIC_DISPLAY_DRIVER*>(hAdapter);
    if (!pBDD->IsDriverActive())
    {
        BDD_LOG_ASSERTION1("BDD (0x%I64x) is being called when not active!", pBDD);
        return STATUS_UNSUCCESSFUL;
    }
    return pBDD->CommitVidPn(pCommitVidPn);
}//函数使指定的视频演示网络 （VidPN） 在显示适配器上处于活动状态。

NTSTATUS
APIENTRY
BddDdiUpdateActiveVidPnPresentPath(
    _In_ CONST HANDLE                                      hAdapter,
    _In_ CONST DXGKARG_UPDATEACTIVEVIDPNPRESENTPATH* CONST pUpdateActiveVidPnPresentPath)
{
    PAGED_CODE();
    BDD_ASSERT_CHK(hAdapter != NULL);

    BASIC_DISPLAY_DRIVER* pBDD = reinterpret_cast<BASIC_DISPLAY_DRIVER*>(hAdapter);
    if (!pBDD->IsDriverActive())
    {
        BDD_LOG_ASSERTION1("BDD (0x%I64x) is being called when not active!", pBDD);
        return STATUS_UNSUCCESSFUL;
    }
    return pBDD->UpdateActiveVidPnPresentPath(pUpdateActiveVidPnPresentPath);
}//函数更新显示适配器上当前处于活动状态的视频演示路径之一。

NTSTATUS
APIENTRY
BddDdiQueryVidPnHWCapability(
    _In_ CONST HANDLE                       hAdapter,
    _Inout_ DXGKARG_QUERYVIDPNHWCAPABILITY* pVidPnHWCaps)
{
    PAGED_CODE();
    BDD_ASSERT_CHK(hAdapter != NULL);

    BASIC_DISPLAY_DRIVER* pBDD = reinterpret_cast<BASIC_DISPLAY_DRIVER*>(hAdapter);
    if (!pBDD->IsDriverActive())
    {
        BDD_LOG_ASSERTION1("BDD (0x%I64x) is being called when not active!", pBDD);
        return STATUS_UNSUCCESSFUL;
    }
    return pBDD->QueryVidPnHWCapability(pVidPnHWCaps);
}//函数请求显示微型端口驱动程序报告功能正常的 VidPn 路径上的硬件功能。
//END: Paged Code
#pragma code_seg(pop)

#pragma code_seg(push)
#pragma code_seg()
// BEGIN: Non-Paged Code

VOID
BddDdiDpcRoutine(
    _In_  VOID* pDeviceContext)
{
    BDD_ASSERT_CHK(pDeviceContext != NULL);

    BASIC_DISPLAY_DRIVER* pBDD = reinterpret_cast<BASIC_DISPLAY_DRIVER*>(pDeviceContext);
    if (!pBDD->IsDriverActive())
    {
        BDD_LOG_ASSERTION1("BDD (0x%I64x) is being called when not active!", pBDD);
        return;
    }
    pBDD->DpcRoutine();
}

BOOLEAN
BddDdiInterruptRoutine(
    _In_  VOID* pDeviceContext,
    _In_  ULONG MessageNumber)
{
    BDD_ASSERT_CHK(pDeviceContext != NULL);

    BASIC_DISPLAY_DRIVER* pBDD = reinterpret_cast<BASIC_DISPLAY_DRIVER*>(pDeviceContext);
    return pBDD->InterruptRoutine(MessageNumber);
}//函数处理显示适配器生成的中断。

VOID
BddDdiResetDevice(
    _In_  VOID* pDeviceContext)
{
    BDD_ASSERT_CHK(pDeviceContext != NULL);

    BASIC_DISPLAY_DRIVER* pBDD = reinterpret_cast<BASIC_DISPLAY_DRIVER*>(pDeviceContext);
    pBDD->ResetDevice();
}//函数将显示适配器设置为 VGA 字符模式

NTSTATUS
APIENTRY
BddDdiSystemDisplayEnable(
    _In_  VOID* pDeviceContext,
    _In_  D3DDDI_VIDEO_PRESENT_TARGET_ID TargetId,
    _In_  PDXGKARG_SYSTEM_DISPLAY_ENABLE_FLAGS Flags,
    _Out_ UINT* Width,
    _Out_ UINT* Height,
    _Out_ D3DDDIFORMAT* ColorFormat)
{
    BDD_ASSERT_CHK(pDeviceContext != NULL);

    BASIC_DISPLAY_DRIVER* pBDD = reinterpret_cast<BASIC_DISPLAY_DRIVER*>(pDeviceContext);
    return pBDD->SystemDisplayEnable(TargetId, Flags, Width, Height, ColorFormat);
}//，以请求显示微型端口驱动程序将当前显示设备重置为指定状态。

VOID
APIENTRY
BddDdiSystemDisplayWrite(
    _In_  VOID* pDeviceContext,
    _In_  VOID* Source,
    _In_  UINT  SourceWidth,
    _In_  UINT  SourceHeight,
    _In_  UINT  SourceStride,
    _In_  UINT  PositionX,
    _In_  UINT  PositionY)
{
    BDD_ASSERT_CHK(pDeviceContext != NULL);

    BASIC_DISPLAY_DRIVER* pBDD = reinterpret_cast<BASIC_DISPLAY_DRIVER*>(pDeviceContext);
    pBDD->SystemDisplayWrite(Source, SourceWidth, SourceHeight, SourceStride, PositionX, PositionY);
}//由操作系统调用，以请求显示微型端口驱动程序将图像块写入显示设备。

// END: Non-Paged Code
#pragma code_seg(pop)
