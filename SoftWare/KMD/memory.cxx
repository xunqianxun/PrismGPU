/******************************Module*Header*******************************\
* Module Name: bdd.h
*
* Basic Display Driver memory allocation, deletion, and tracking 
*
*
* Copyright (c) 2010 Microsoft Corporation
\**************************************************************************/

#include "BDD.hxx"
//#include "HW.hxx"

#pragma code_seg("PAGE")

//
// New and delete operators
//
//_When_((PoolType & NonPagedPoolMustSucceed) != 0,
//    __drv_reportError("Must succeed pool allocations are forbidden. "
//            "Allocation failures cause a system crash"))
//void* __cdecl operator new(size_t Size, POOL_TYPE PoolType)
//{
//    PAGED_CODE();
//
//    Size = (Size != 0) ? Size : 1;
//    
//    // Note that ExAllocatePool2 replaces ExAllocatePool* APIs in OS's starting
//    // with Windows 10, version 2004. If your driver targets previous versions it
//    // should use ExAllocatePoolZero instead.
//    void* pObject = ExAllocatePool2(PoolType, Size, BDDTAG);
//
//#if DBG
//    if (pObject != NULL)
//    {
//        RtlFillMemory(pObject, Size, 0xCD);
//    }
//#endif // DBG
//
//    return pObject;
//}
//
//_When_((PoolType & NonPagedPoolMustSucceed) != 0,
//    __drv_reportError("Must succeed pool allocations are forbidden. "
//            "Allocation failures cause a system crash"))
//void* __cdecl operator new[](size_t Size, POOL_TYPE PoolType)
//{
//    PAGED_CODE();
//
//    Size = (Size != 0) ? Size : 1;
//    
//    void* pObject = ExAllocatePool2(PoolType, Size, BDDTAG);
//
//#if DBG
//    if (pObject != NULL)
//    {
//        RtlFillMemory(pObject, Size, 0xCD);
//    }
//#endif // DBG
//
//    return pObject;
//}
//
//void __cdecl operator delete(void* pObject)
//{
//    PAGED_CODE();
//
//    if (pObject != NULL)
//    {
//        ExFreePool(pObject);
//    }
//}
//
////
//// size_t version is needed for VS2015(C++ 14).  
//// 
//void __cdecl operator delete(void* pObject, size_t s)
//{
//    PAGED_CODE();
//
//    UNREFERENCED_PARAMETER( s );
//
//    ::operator delete( pObject );
//}
//
//void __cdecl operator delete[](void* pObject)
//{
//    PAGED_CODE();
//
//    if (pObject != NULL)
//    {
//        ExFreePool(pObject);
//    }
//}

void* __cdecl operator new(size_t Size, BDD_POOL_FLAGS PoolFlags)
{
    PAGED_CODE();

    Size = (Size != 0) ? Size : 1;

    void* pObject = ExAllocatePool2(static_cast<POOL_FLAGS>(PoolFlags), Size, BDDTAG);

#if DBG
    if (pObject != NULL)
    {
        RtlFillMemory(pObject, Size, 0xCD);
    }
#endif // DBG

    return pObject;
}

void* __cdecl operator new[](size_t Size, BDD_POOL_FLAGS PoolFlags)
{
    PAGED_CODE();

    Size = (Size != 0) ? Size : 1;

    void* pObject = ExAllocatePool2(static_cast<POOL_FLAGS>(PoolFlags), Size, BDDTAG);

#if DBG
    if (pObject != NULL)
    {
        RtlFillMemory(pObject, Size, 0xCD);
    }
#endif // DBG

    return pObject;
}

void __cdecl operator delete(void* pObject)
{
    PAGED_CODE();

    if (pObject != NULL)
    {
        ExFreePool(pObject);
    }
}

//
// size_t version is needed for VS2015(C++ 14).  
// 
void __cdecl operator delete(void* pObject, size_t s)
{
    PAGED_CODE();

    UNREFERENCED_PARAMETER(s);

    ::operator delete(pObject);
}

void __cdecl operator delete[](void* pObject)
{
    PAGED_CODE();

    if (pObject != NULL)
    {
        ExFreePool(pObject);
    }
}


