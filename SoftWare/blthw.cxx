/******************************Module*Header*******************************\
* Module Name: blthw.cxx
*
* Sample display driver functions for a HW blt simulation. This file is 
* only provided to simulate how a real hardware-accelerated display-only 
* driver functions, and should not be used in a real driver.
*
* Copyright (c) 2011 Microsoft Corporation
\**************************************************************************/

#include "BDD.hxx"

typedef struct
{
    CONST DXGKRNL_INTERFACE*        DxgkInterface;
    DXGKARGCB_NOTIFY_INTERRUPT_DATA NotifyInterrupt;
} SYNC_NOTIFY_INTERRUPT;

KSYNCHRONIZE_ROUTINE SynchronizeVidSchNotifyInterrupt;

BOOLEAN SynchronizeVidSchNotifyInterrupt(_In_opt_ PVOID params)
{
    // This routine is non-paged code called at the device interrupt level (DIRQL)
    // to notify VidSch and schedule a DPC. It is meant as a demonstration of handling 
    // a real hardware interrupt, even though it is actually called from asynchronous 
    // present worker threads in this sample.
    SYNC_NOTIFY_INTERRUPT* pParam = reinterpret_cast<SYNC_NOTIFY_INTERRUPT*>(params);

    // The context is known to be non-NULL
    __analysis_assume(pParam != NULL);

    // Update driver information related to fences
    switch(pParam->NotifyInterrupt.InterruptType)
    {
    case DXGK_INTERRUPT_DISPLAYONLY_VSYNC: //垂直同步（VSync）中断，通常用于屏幕刷新信号。
    case DXGK_INTERRUPT_DISPLAYONLY_PRESENT_PROGRESS: //显示进度中断，可能用于帧渲染进度。
        break;
    default:
        NT_ASSERT(FALSE);
        return FALSE;
    }

    // Callback OS to report about the interrupt
    pParam->DxgkInterface->DxgkCbNotifyInterrupt(pParam->DxgkInterface->DeviceHandle,&pParam->NotifyInterrupt); //通知操作系统 "我收到了一个 GPU 相关的中断"。

    // Now queue a DPC for this interrupt (to callback schedule at DCP level and let it do more work there)
    // DxgkCbQueueDpc can return FALSE if there is already a DPC queued
    // this is an acceptable condition
    pParam->DxgkInterface->DxgkCbQueueDpc(pParam->DxgkInterface->DeviceHandle); //这里调用 DxgkCbQueueDpc 让操作系统安排一个 低优先级任务（DPC），在更合适的时机执行

    return TRUE;
}

#pragma code_seg("PAGE")

KSTART_ROUTINE HwContextWorkerThread;

struct DoPresentMemory
{
    PVOID                     DstAddr;
    UINT                      DstStride;
    ULONG                     DstBitPerPixel;
    UINT                      SrcWidth;
    UINT                      SrcHeight;
    BYTE*                     SrcAddr;
    LONG                      SrcPitch;
    ULONG                     NumMoves;             // in:  Number of screen to screen moves
    D3DKMT_MOVE_RECT*         Moves;               // in:  Point to the list of moves
    ULONG                     NumDirtyRects;        // in:  Number of direct rects
    RECT*                     DirtyRect;           // in:  Point to the list of dirty rects
    D3DKMDT_VIDPN_PRESENT_PATH_ROTATION Rotation;
    BOOLEAN                   SynchExecution;
    D3DDDI_VIDEO_PRESENT_SOURCE_ID  SourceID;
    HANDLE                    hAdapter;
    PMDL                      Mdl;
    BDD_HWBLT*                DisplaySource;
};

void
HwExecutePresentDisplayOnly(
    HANDLE Context);

NTSTATUS
StartHwBltPresentWorkerThread( //StartRoutine：线程的 入口函数（即该线程运行时会执行 StartRoutine）。
    _In_ PKSTART_ROUTINE StartRoutine,
    _In_ _When_(return==0, __drv_aliasesMem) PVOID StartContext) //StartContext：传递给 StartRoutine 的 上下文参数（一般是 DoPresentMemory*，包含 Present 的相关信息）
/*++

  Routine Description:

    This routine creates the worker thread to execute a single present 
    command. Creating a new thread on every asynchronous present is not 
    efficient, but this file is only meant as a simulation, not an example 
    of implementation.

  Arguments:

    StartRoutine - start routine
    StartContext - start context

  Return Value:

    Status


1.  解析 StartContext，获取 DoPresentMemory 和 DisplaySource
2.  初始化 OBJECT_ATTRIBUTES
3.  创建内核线程 (PsCreateSystemThread)
    - 线程执行 StartRoutine(StartContext)
4.  等待 Present 线程启动 (KeWaitForSingleObject)
5.  记录线程信息 (SetPresentWorkerThreadInfo)
6.  解除挂起，让 Present 线程执行 (KeSetEvent)
7.  返回 STATUS_PENDING




--*/
{
    PAGED_CODE();

    OBJECT_ATTRIBUTES ObjectAttributes; //OBJECT_ATTRIBUTES 用于描述 线程对象
    InitializeObjectAttributes(&ObjectAttributes, NULL, OBJ_KERNEL_HANDLE, NULL, NULL); //OBJ_KERNEL_HANDLE：指定创建的句柄只能在内核模式下访问（防止用户态访问）
    HANDLE hWorkerThread = NULL;

    // copy data from the context which is need here, as it will be deleted in separate thread
    DoPresentMemory* ctx = reinterpret_cast<DoPresentMemory*>(StartContext); //ctx（DoPresentMemory*）：包含 Present 相关数据（显存地址、旋转信息等）。
    BDD_HWBLT* displaySource = ctx->DisplaySource; //displaySource（BDD_HWBLT*）：表示 显示设备的 Present 线程管理信息。

    NTSTATUS Status = PsCreateSystemThread( //创建内核线程
        &hWorkerThread,
        THREAD_ALL_ACCESS,
        &ObjectAttributes,
        NULL,
        NULL,
        StartRoutine,
        StartContext);

    if (!NT_SUCCESS(Status))
    {
        return Status;
    }


    // wait for thread to start - infinite wait -
    // need to make sure the tread is running before OS stars submitting the work items to it
    KeWaitForSingleObject(&displaySource->m_hThreadStartupEvent, Executive, KernelMode, FALSE, NULL); //阻塞当前线程，直到 Present 线程启动，m_hThreadStartupEvent 是 事件对象，当 Present 线程启动后，会 发出信号（SetEvent） 解除阻塞。

    // Handle is passed to the parent object which must close it
    displaySource->SetPresentWorkerThreadInfo(hWorkerThread); //保存 Present 线程的句柄，方便后续 管理和终止线程

    // Resume context thread, this is done by setting the event the thread is waiting on
    KeSetEvent(&displaySource->m_hThreadSuspendEvent, 0, FALSE); //KeSetEvent 发出信号，让 Present 线程继续执行。

    return STATUS_PENDING;
}

BDD_HWBLT::BDD_HWBLT():m_DevExt (NULL), //m_DevExt：设为 NULL，表示设备扩展指针未初始化。
                m_SynchExecution(TRUE), //m_SynchExecution：设为 TRUE，表示 默认同步执行 Present 任务。
                m_hPresentWorkerThread(NULL), //m_hPresentWorkerThread 和 m_pPresentWorkerThread：初始化为 NULL，表示 尚未创建 Present 线程。
                m_pPresentWorkerThread(NULL)
{
    PAGED_CODE();

    KeInitializeEvent(&m_hThreadStartupEvent, NotificationEvent, FALSE); //m_hThreadStartupEvent 通知线程已启动。 
    KeInitializeEvent(&m_hThreadSuspendEvent, SynchronizationEvent, FALSE); //挂起 Present 线程，直到主线程允许它继续执行。


}


BDD_HWBLT::~BDD_HWBLT()
/*++

  Routine Description:

    This routine waits on present worker thread to exit before
    destroying the object

  Arguments:

    None

  Return Value:

    None

--*/
{
    PAGED_CODE();

    // make sure the worker thread has exited
    SetPresentWorkerThreadInfo(NULL);//清理 Present 线程信息.如果Present 线程仍在运行，则等待其退出，并释放相关资源。
}

#pragma warning(push)
#pragma warning(disable:26135) // The function doesn't lock anything

void
BDD_HWBLT::SetPresentWorkerThreadInfo(
    HANDLE hWorkerThread)
/*++

  Routine Description:

    The method is updating present worker information
    It is called in following cases:
     - In ExecutePresent to update worker thread information
     - In Dtor to wait on worker thread to exit

  Arguments:

    hWorkerThread - handle of the present worker thread

  Return Value:

    None

--*/
{

    PAGED_CODE();

    if (m_pPresentWorkerThread)  // 存在旧的 Present 线程
    {
        // Wait for thread to exit
        KeWaitForSingleObject(m_pPresentWorkerThread, Executive, KernelMode,  // 等待旧线程退出
                              FALSE, NULL);
        // Dereference thread object
        ObDereferenceObject(m_pPresentWorkerThread); // 释放线程对象引用计数
        m_pPresentWorkerThread = NULL;

        NT_ASSERT(m_hPresentWorkerThread); // 断言确保句柄非空
        ZwClose(m_hPresentWorkerThread);    // 关闭旧的线程句柄
        m_hPresentWorkerThread = NULL;
    }

    if (hWorkerThread)  // 传入新的 Present 线程
    {
        // Make sure that thread's handle would be valid even if the thread exited
        ObReferenceObjectByHandle(hWorkerThread, THREAD_ALL_ACCESS, NULL, // 增加线程对象引用，确保句柄有效，即使线程退出
                                  KernelMode, &m_pPresentWorkerThread, NULL);
        NT_ASSERT(m_pPresentWorkerThread);  // 断言确保线程对象有效
        m_hPresentWorkerThread = hWorkerThread; // 存储新线程句柄
    }

}
#pragma warning(pop)

NTSTATUS
BDD_HWBLT::ExecutePresentDisplayOnly(
    _In_ BYTE*             DstAddr,
    _In_ UINT              DstBitPerPixel,
    _In_ BYTE*             SrcAddr,
    _In_ UINT              SrcBytesPerPixel,
    _In_ LONG              SrcPitch,
    _In_ ULONG             NumMoves,
    _In_ D3DKMT_MOVE_RECT* Moves,
    _In_ ULONG             NumDirtyRects,
    _In_ RECT*             DirtyRect,
    _In_ D3DKMDT_VIDPN_PRESENT_PATH_ROTATION Rotation)
/*++

  Routine Description:

    The method creates present worker thread and provides context
    for it filled with present commands

  Arguments:

    _In_ BYTE* DstAddr,                  // 目标显存地址（Destination）
    _In_ UINT DstBitPerPixel,            // 目标颜色深度
    _In_ BYTE* SrcAddr,                  // 源显存地址（Source）
    _In_ UINT SrcBytesPerPixel,          // 源像素格式（每像素字节数）
    _In_ LONG SrcPitch,                  // 源数据每行占用的字节数
    _In_ ULONG NumMoves,                 // 移动矩形个数
    _In_ D3DKMT_MOVE_RECT* Moves,        // 移动矩形数组
    _In_ ULONG NumDirtyRects,            // 更新区域个数
    _In_ RECT* DirtyRect,                // 更新区域数组
    _In_ D3DKMDT_VIDPN_PRESENT_PATH_ROTATION Rotation // 旋转参数

    DstAddr - address of destination surface
    DstBitPerPixel - color depth of destination surface
    SrcAddr - address of source surface
    SrcBytesPerPixel - bytes per pixel of source surface
    SrcPitch - source surface pitch (bytes in a row)
    NumMoves - number of moves to be copied
    Moves - moves' data
    NumDirtyRects - number of rectangles to be copied
    DirtyRect - rectangles' data
    Rotation - roatation to be performed when executing copy
    CallBack - callback for present worker thread to report execution status

  Return Value:

    Status

--*/
{

    PAGED_CODE();

    NTSTATUS Status = STATUS_SUCCESS;

    SIZE_T sizeMoves = NumMoves*sizeof(D3DKMT_MOVE_RECT);
    SIZE_T sizeRects = NumDirtyRects*sizeof(RECT);
    SIZE_T size = sizeof(DoPresentMemory) + sizeMoves + sizeRects; //计算 DoPresentMemory 结构体 + 需要存储的 Moves 和 DirtyRects 的总大小

    DoPresentMemory* ctx = reinterpret_cast<DoPresentMemory*>
                                (new (BDD_POOL_FLAGS(POOL_FLAG_PAGED)) BYTE[size]); //在分页池（Paged Pool）中分配 ctx 结构体，用于存储 Present 任务的上下文数据

    if (!ctx)
    {
        return STATUS_NO_MEMORY;
    }

    RtlZeroMemory(ctx,size);

    const CURRENT_BDD_MODE* pModeCur = m_DevExt->GetCurrentMode(m_SourceId); //获取 当前显示模式

    ctx->DstAddr          = DstAddr;
    ctx->DstBitPerPixel   = DstBitPerPixel;
    ctx->DstStride        = pModeCur->DispInfo.Pitch;
    ctx->SrcWidth         = pModeCur->SrcModeWidth;
    ctx->SrcHeight        = pModeCur->SrcModeHeight;
    ctx->SrcAddr          = NULL;
    ctx->SrcPitch         = SrcPitch;
    ctx->Rotation         = Rotation;
    ctx->NumMoves         = NumMoves;
    ctx->Moves            = Moves;
    ctx->NumDirtyRects    = NumDirtyRects;
    ctx->DirtyRect        = DirtyRect;
    ctx->SourceID         = m_SourceId;
    ctx->hAdapter         = m_DevExt;
    ctx->Mdl              = NULL;
    ctx->DisplaySource    = this;

    // Alternate between synch and asynch execution, for demonstrating 
    // that a real hardware implementation can do either
    m_SynchExecution = !m_SynchExecution; //执行模式

    ctx->SynchExecution   = m_SynchExecution;

    {
        // Map Source into kernel space, as Blt will be executed by system worker thread
        UINT sizeToMap = SrcBytesPerPixel*pModeCur->SrcModeWidth*pModeCur->SrcModeHeight;

        PMDL mdl = IoAllocateMdl((PVOID)SrcAddr, sizeToMap,  FALSE, FALSE, NULL); //计算 要映射的显存大小，并创建 MDL
        if(!mdl)
        {
            return STATUS_INSUFFICIENT_RESOURCES;
        }

        KPROCESSOR_MODE AccessMode = static_cast<KPROCESSOR_MODE>(( SrcAddr <=
                        (BYTE* const) MM_USER_PROBE_ADDRESS)?UserMode:KernelMode); //判断 SrcAddr 是用户模式还是内核模式地址。尝试锁定物理页，确保数据在内存中不会被交换出。
        __try
        {
            // Probe and lock the pages of this buffer in physical memory.
            // We need only IoReadAccess.
            MmProbeAndLockPages(mdl, AccessMode, IoReadAccess);
        }
        #pragma prefast(suppress: __WARNING_EXCEPTIONEXECUTEHANDLER, "try/except is only able to protect against user-mode errors and these are the only errors we try to catch here");
        __except(EXCEPTION_EXECUTE_HANDLER)
        {
            Status = GetExceptionCode();
            IoFreeMdl(mdl);
            return Status;
        }

        // Map the physical pages described by the MDL into system space.
        // Note: double mapping the buffer this way causes lot of system
        // overhead for large size buffers.
        ctx->SrcAddr = reinterpret_cast<BYTE*>
            (MmGetSystemAddressForMdlSafe(mdl, NormalPagePriority )); //获取 MDL 对应的内核地址，以便后续操作

        if(!ctx->SrcAddr) {
            Status = STATUS_INSUFFICIENT_RESOURCES;
            MmUnlockPages(mdl);
            IoFreeMdl(mdl);
            return Status;
        }

        // Save Mdl to unmap and unlock the pages in worker thread
        ctx->Mdl = mdl;
    }

    BYTE* rects = reinterpret_cast<BYTE*>(ctx+1); //将 Moves 和 DirtyRects 复制到 ctx 内存块，并更新指针。

    // copy moves and update pointer
    if (Moves)
    {
        memcpy(rects,Moves,sizeMoves);
        ctx->Moves = reinterpret_cast<D3DKMT_MOVE_RECT*>(rects);
        rects += sizeMoves;
    }

    // copy dirty rects and update pointer
    if (DirtyRect)
    {
        memcpy(rects,DirtyRect,sizeRects);
        ctx->DirtyRect = reinterpret_cast<RECT*>(rects);
    }


    if (m_SynchExecution) //如果是同步模式，立即调用 HwExecutePresentDisplayOnly 进行处理。
    {
        HwExecutePresentDisplayOnly((PVOID)ctx);
        return STATUS_SUCCESS;
    }
    else
    {
        // Create a worker thread to perform the present asynchronously
        // Ctx will be deleted in worker thread (on exit)
        return StartHwBltPresentWorkerThread(HwContextWorkerThread,(PVOID)ctx); //如果是异步模式，调用 StartHwBltPresentWorkerThread 创建一个线程 处理 ctx。
    }
}


void
ReportPresentProgress(
    _In_ HANDLE Adapter, //Adapter 是 显示适配器（GPU）句柄，实际是 BASIC_DISPLAY_DRIVER* 类型的指针。
    _In_ D3DDDI_VIDEO_PRESENT_SOURCE_ID VidPnSourceId,
    _In_ BOOLEAN CompletedOrFailed)
/*++

  Routine Description:

    This routine runs a fake interrupt routine in order to tell the OS about the present progress.

  Arguments:

    Adapter - Handle to the adapter (Device Extension)
    VidPnSourceId - Video present source id for the callback
    CompletedOrFailed - Present progress status for the source

  Return Value:

    None

--*/
{
    PAGED_CODE();

    BASIC_DISPLAY_DRIVER* pDevExt =
        reinterpret_cast<BASIC_DISPLAY_DRIVER*>(Adapter); //pDevExt 代表 GPU 设备的扩展结构，它包含 DirectX 内核模式接口（DXGK Interface）。

    SYNC_NOTIFY_INTERRUPT SyncNotifyInterrupt = {};
    SyncNotifyInterrupt.DxgkInterface = pDevExt->GetDxgkInterface();
    SyncNotifyInterrupt.NotifyInterrupt.InterruptType = DXGK_INTERRUPT_DISPLAYONLY_PRESENT_PROGRESS; //代表 Present 进度通知中断
    SyncNotifyInterrupt.NotifyInterrupt.DisplayOnlyPresentProgress.VidPnSourceId = VidPnSourceId;

    SyncNotifyInterrupt.NotifyInterrupt.DisplayOnlyPresentProgress.ProgressId =
                            (CompletedOrFailed)?DXGK_PRESENT_DISPLAYONLY_PROGRESS_ID_COMPLETE: //CompletedOrFailed 决定 Present 任务的状态 完成或者失败
                            DXGK_PRESENT_DISPLAYONLY_PROGRESS_ID_FAILED;

    // Execute the SynchronizeVidSchNotifyInterrupt function at the interrupt 
    // IRQL in order to fake a real present progress interrupt
    BOOLEAN bRet = FALSE;
    NT_VERIFY(NT_SUCCESS(pDevExt->GetDxgkInterface()->DxgkCbSynchronizeExecution(
                                    pDevExt->GetDxgkInterface()->DeviceHandle,
                                    (PKSYNCHRONIZE_ROUTINE)SynchronizeVidSchNotifyInterrupt,
                                    (PVOID)&SyncNotifyInterrupt,0,&bRet)));
    NT_ASSERT(bRet);
}


void
HwContextWorkerThread(
    HANDLE Context)
{
    PAGED_CODE();

    DoPresentMemory* ctx = reinterpret_cast<DoPresentMemory*>(Context);
    BDD_HWBLT* displaySource = ctx->DisplaySource; //用于 管理 Present 线程的状态和同步。

    // Signal event to indicate that the tread has started
    KeSetEvent(&displaySource->m_hThreadStartupEvent, 0, FALSE); //发送事件，通知系统线程已启动 通知 StartHwBltPresentWorkerThread 线程已成功创建，让 StartHwBltPresentWorkerThread 继续执行。

    // Suspend context thread, do this by waiting on the suspend event
    KeWaitForSingleObject(&displaySource->m_hThreadSuspendEvent, Executive, KernelMode, FALSE, NULL); //让线程 进入等待状态，直到 KeSetEvent(&displaySource->m_hThreadSuspendEvent, 0, FALSE); 解除挂起。

    HwExecutePresentDisplayOnly(Context); //调用 HwExecutePresentDisplayOnly 执行 Present 任务
}


void
HwExecutePresentDisplayOnly(
    HANDLE Context)
/*++

  Routine Description:

    The routine executes present's commands and report progress to the OS

  Arguments:

    Context - Context with present's command

  Return Value:

    None

--*/
{
    PAGED_CODE();

    DoPresentMemory* ctx = reinterpret_cast<DoPresentMemory*>(Context); //解析 ctx 结构体，获取 Present 任务的上下文信息

    // Set up destination blt info
    BLT_INFO DstBltInfo; //DstBltInfo 用于描述 目标显存
    DstBltInfo.pBits = ctx->DstAddr;
    DstBltInfo.Pitch = ctx->DstStride;
    DstBltInfo.BitsPerPel = ctx->DstBitPerPixel;
    DstBltInfo.Offset.x = 0;
    DstBltInfo.Offset.y = 0;
    DstBltInfo.Rotation = ctx->Rotation;
    DstBltInfo.Width = ctx->SrcWidth;
    DstBltInfo.Height = ctx->SrcHeight;

    // Set up source blt info
    BLT_INFO SrcBltInfo; //SrcBltInfo 用于描述 源显存（通常是 CPU 绘制的画面）
    SrcBltInfo.pBits = ctx->SrcAddr;
    SrcBltInfo.Pitch = ctx->SrcPitch;
    SrcBltInfo.BitsPerPel = 32;
    SrcBltInfo.Offset.x = 0;
    SrcBltInfo.Offset.y = 0;
    SrcBltInfo.Rotation = D3DKMDT_VPPR_IDENTITY;
    if (ctx->Rotation == D3DKMDT_VPPR_ROTATE90 || //如果需要 90° 或 270° 旋转，则交换 Width 和 Height。
        ctx->Rotation == D3DKMDT_VPPR_ROTATE270)
    {
        SrcBltInfo.Width = DstBltInfo.Height;
        SrcBltInfo.Height = DstBltInfo.Width;
    }
    else
    {
        SrcBltInfo.Width = DstBltInfo.Width;
        SrcBltInfo.Height = DstBltInfo.Height;
    }


    // Copy all the scroll rects from source image to video frame buffer.
    for (UINT i = 0; i < ctx->NumMoves; i++) // Moves 用于 窗口拖动、滚动等操作。
    {
        BltBits(&DstBltInfo, //BltBits：将 Moves 指定的区域从 SrcBltInfo 复制到 DstBltInfo
        &SrcBltInfo,
        1, // NumRects
        &ctx->Moves[i].DestRect);
    }

    // Copy all the dirty rects from source image to video frame buffer.
    for (UINT i = 0; i < ctx->NumDirtyRects; i++) //DirtyRects 用于 增量更新（如窗口重绘）
    {

        BltBits(&DstBltInfo,
        &SrcBltInfo,
        1, // NumRects
        &ctx->DirtyRect[i]);
    }

    // Unmap unmap and unlock the pages.
    if (ctx->Mdl) //解锁 内存页，释放 MDL 结构体，避免 内存泄漏。

    {
        MmUnlockPages(ctx->Mdl);
        IoFreeMdl(ctx->Mdl);
    }

    if(ctx->SynchExecution) //如果是同步模式，函数直接返回（Present 任务已经完成）。
    {
        // This code simulates Blt executed synchronously
        // nothing should be done here, just exit
        ;
    }
    else //异步则调用 ReportPresentProgress 向操作系统汇报 Present 完成（模拟硬件中断）。
    {
        // TRUE == completed
        // This code is emulates interrupt which HW should generate
        ReportPresentProgress(ctx->hAdapter,ctx->SourceID,TRUE);
    }

    delete [] reinterpret_cast<BYTE*>(ctx);
}
