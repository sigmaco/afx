/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                  Q W A D R O   E X E C U T I O N   E C O S Y S T E M
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// Because MSVC doesn't implement C11 atomic library, 
// we have to roll down the whole thing in here; at least for 32-bit and 64-bit words.
// Nothing new and/or untrivial to a C software engineer. We are hardcore!

#ifndef AFX_ATOMIC_H
#define AFX_ATOMIC_H

#include "qwadro/exec/afxDebug.h"

#ifndef AFX_ATOMIC_ALIGNMENT
#   define AFX_ATOMIC_ALIGNMENT sizeof(afxUnit64) 
#endif

#ifdef AFX_ON_WINDOWS
typedef AFX_ALIGNED(AFX_ATOMIC_ALIGNMENT) volatile long       afxAtom32;
typedef AFX_ALIGNED(AFX_ATOMIC_ALIGNMENT) volatile long long  afxAtom64;
typedef AFX_ALIGNED(AFX_ATOMIC_ALIGNMENT) volatile void*      afxAtomPtr;
#else // LINUX
typedef AFX_ALIGNED(AFX_ATOMIC_ALIGNMENT) volatile    int32_t	afxAtom32;
typedef AFX_ALIGNED(AFX_ATOMIC_ALIGNMENT) volatile    int64_t	afxAtom64;
typedef AFX_ALIGNED(AFX_ATOMIC_ALIGNMENT) volatile    void*   afxAtomPtr;
#endif

AFX_STATIC_ASSERT(sizeof(afxAtom32) == sizeof(afxByte[4]), "");
AFX_STATIC_ASSERT(sizeof(afxAtom64) == sizeof(afxByte[8]), "");

typedef enum afxMemoryOrder
{
    // The strongest ordering, ensuring sequential consistency across all threads.
    // The default one. We moved it from last to be the first (0) item.
    afxMemoryOrder_SEQ_CST,

    // No synchronization or ordering of operations.
    afxMemoryOrder_RELAXED,

    afxMemoryOrder_CONSUME,

    // Ensures that all previous memory operations are completed before the load.
    afxMemoryOrder_ACQUIRE,

    // Ensures that all previous memory operations are completed after the store (not directly relevant for atomic_load_explicit()).
    afxMemoryOrder_RELEASE,

    afxMemoryOrder_ACQ_REL
} afxMemoryOrder;

// reads a value from an atomic object.
AFXINL afxInt32     AfxLoadAtom32(afxAtom32* src);
AFXINL afxInt64     AfxLoadAtom64(afxAtom64* src);
AFXINL void*        AfxLoadAtomPtr(afxAtomPtr* src);

// stores a value in an atomic object.
AFXINL void         AfxStoreAtom32(afxAtom32* dst, afxInt32 val);
AFXINL void         AfxStoreAtom64(afxAtom64* dsr, afxInt64 val);
AFXINL void         AfxStoreAtomPtr(afxAtomPtr* dst, void* val);

AFXINL afxInt32     AfxIncAtom32(afxAtom32* val);
AFXINL afxInt64     AfxIncAtom64(afxAtom64* val);

// atomic addition.
AFXINL afxInt32     AfxAddAtom32(afxAtom32* dst, afxInt32 val);
AFXINL afxInt64     AfxAddAtom64(afxAtom64* dst, afxInt64 val);

AFXINL afxInt32     AfxDecAtom32(afxAtom32* val);
AFXINL afxInt64     AfxDecAtom64(afxAtom64* val);

// atomic subtraction.
AFXINL afxInt32     AfxSubAtom32(afxAtom32* dst, afxInt32 val);
AFXINL afxInt64     AfxSubAtom64(afxAtom64* dst, afxInt64 val);

// swaps a value with the value of an atomic object.
AFXINL afxInt32     AfxExchangeAtom32(afxAtom32* dst, afxInt32 val);
AFXINL afxInt64     AfxExchangeAtom64(afxAtom64* dst, afxInt64 val);
AFXINL void*        AfxExchangeAtomPtr(afxAtomPtr* dst, void* val);

// atomic bitwise AND.
AFXINL afxInt32     AfxAndAtom32(afxAtom32* dst, afxInt32 val);
AFXINL afxInt64     AfxAndAtom64(afxAtom64* dst, afxInt64 val);

// atomic bitwise OR.
AFXINL afxInt32     AfxOrAtom32(afxAtom32* dst, afxInt32 val);
AFXINL afxInt64     AfxOrAtom64(afxAtom64* dst, afxInt64 val);

// swaps a value with an atomic object if the old value is what is expected, otherwise reads the old value.
AFXINL afxBool      AfxCasAtom32(afxAtom32* dst, afxInt32* expected, afxInt32 proposed);
AFXINL afxBool      AfxCasAtom64(afxAtom64* dst, afxInt64* expected, afxInt64 proposed);
AFXINL afxBool      AfxCasAtomPtr(afxAtomPtr* dst, void** expected, void* proposed);

////////////////////////////////////////////////////////////////////////////////

AFX void AfxAtomicInit32(volatile afxInt32 *obj, afxInt32 desired);
AFX void AfxAtomicInit64(volatile afxInt64 *obj, afxInt64 desired);

#define AfxAtomicInit(pobj_, desired_) \
switch(sizeof(*(pobj_))) \
{ \
case sizeof(afxInt64): AfxAtomicInit64((pobj_),(desired_)); break;\
case sizeof(afxInt32): AfxAtomicInit32((pobj_),(desired_)); break;\
default: AfxThrowError(); break; \
}

AFX afxInt32 AfxAtomicLoad32(const volatile afxInt32* obj, afxMemoryOrder order);
AFX afxInt64 AfxAtomicLoad64(const volatile afxInt64* obj, afxMemoryOrder order);

#define AfxAtomicLoad(pobj_, order_) \
switch(sizeof(*(pobj_))) \
{ \
case sizeof(afxInt64): AfxAtomicLoad64((pobj_),(order_)); break;\
case sizeof(afxInt32): AfxAtomicLoad32((pobj_),(order_)); break;\
default: AfxThrowError(); break; \
}

/*
    atomic_store_explicit()

    Atomically replaces the value of the atomic variable pointed to by obj with desired.
    The operation is atomic write operation.

    The first version orders memory accesses according to memory_order_seq_cst,
    the second version orders memory accesses according to order.
    Order must be one of memory_order_relaxed, memory_order_release or memory_order_seq_cst.
    Otherwise the behavior is undefined.
*/

AFX void AfxAtomicStore32(volatile afxInt32* obj, afxInt32 value, afxMemoryOrder order);
AFX void AfxAtomicStore64(volatile afxInt64* obj, afxInt64 value, afxMemoryOrder order);

#define AfxAtomicStore(pobj_, value_, order_) \
switch(sizeof(*(pobj_))) \
{ \
case sizeof(afxInt64): AfxAtomicStore64((pobj_),(value_),(order_)); break;\
case sizeof(afxInt32): AfxAtomicStore32((pobj_),(value_),(order_)); break;\
default: AfxThrowError(); break; \
}

AFX afxInt32 AfxAtomicExchange32(volatile afxInt32* obj, afxInt32 value, afxMemoryOrder order);
AFX afxInt64 AfxAtomicExchange64(volatile afxInt64* obj, afxInt64 value, afxMemoryOrder order);

#define AfxAtomicExchange(pobj_, value_, order_) \
switch(sizeof(*(pobj_))) \
{ \
case sizeof(afxInt64): AfxAtomicExchange64((pobj_),(value_),(order_)); break;\
case sizeof(afxInt32): AfxAtomicExchange32((pobj_),(value_),(order_)); break;\
default: AfxThrowError(); break; \
}

AFX afxBool AfxAtomicCompareExchangeStrong32(volatile afxInt32* obj, afxInt32* expected, afxInt32 desired, afxMemoryOrder succ, afxMemoryOrder fail);
AFX afxBool AfxAtomicCompareExchangeStrong64(volatile afxInt64* obj, afxInt64* expected, afxInt64 desired, afxMemoryOrder succ, afxMemoryOrder fail);

#define AfxAtomicCompareExchangeStrong(pobj_, expected_, desired_, succ_, fail_) \
switch(sizeof(*(pobj_))) \
{ \
case sizeof(afxInt64): AfxAtomicCompareExchangeStrong64((pobj_),(expected_),(desired_),(succ_),(fail_)); break;\
case sizeof(afxInt32): AfxAtomicCompareExchangeStrong32((pobj_),(expected_),(desired_),(succ_),(fail_)); break;\
default: AfxThrowError(); break; \
}

AFX afxBool AfxAtomicCompareExchangeWeak32(volatile afxInt32* obj, afxInt32* expected, afxInt32 desired, afxMemoryOrder succ, afxMemoryOrder fail);
AFX afxBool AfxAtomicCompareExchangeWeak64(volatile afxInt64* obj, afxInt64* expected, afxInt64 desired, afxMemoryOrder succ, afxMemoryOrder fail);

#define AfxAtomicCompareExchangeWeak(pobj_, expected_, desired_, succ_, fail_) \
switch(sizeof(*(pobj_))) \
{ \
case sizeof(afxInt64): AfxAtomicCompareExchangeWeak64((pobj_),(expected_),(desired_),(succ_),(fail_)); break;\
case sizeof(afxInt32): AfxAtomicCompareExchangeWeak32((pobj_),(expected_),(desired_),(succ_),(fail_)); break;\
default: AfxThrowError(); break; \
}

AFX afxInt32 AfxAtomicFetchAdd32(volatile afxInt32* obj, afxInt32 value, afxMemoryOrder order);
AFX afxInt64 AfxAtomicFetchAdd64(volatile afxInt64* obj, afxInt64 value, afxMemoryOrder order);

#define AfxAtomicFetchAdd(pobj_, value_, order_) \
switch(sizeof(*(pobj_))) \
{ \
case sizeof(afxInt64): AfxAtomicFetchAdd64((pobj_),(value_),(order_)); break;\
case sizeof(afxInt32): AfxAtomicFetchAdd32((pobj_),(value_),(order_)); break;\
default: AfxThrowError(); break; \
}

AFX afxInt32 AfxAtomicFetchSub32(volatile afxInt32* obj, afxInt32 value, afxMemoryOrder order);
AFX afxInt64 AfxAtomicFetchSub64(volatile afxInt64* obj, afxInt64 value, afxMemoryOrder order);

#define AfxAtomicFetchSub(pobj_, value_, order_) \
switch(sizeof(*(pobj_))) \
{ \
case sizeof(afxInt64): AfxAtomicFetchSub64((pobj_),(value_),(order_)); break;\
case sizeof(afxInt32): AfxAtomicFetchSub32((pobj_),(value_),(order_)); break;\
default: AfxThrowError(); break; \
}

AFX afxInt32 AfxAtomicFetchOr32(volatile afxInt32* obj, afxInt32 value, afxMemoryOrder order);
AFX afxInt64 AfxAtomicFetchOr64(volatile afxInt64* obj, afxInt64 value, afxMemoryOrder order);

#define AfxAtomicFetchOr(pobj_, value_, order_) \
switch(sizeof(*(pobj_))) \
{ \
case sizeof(afxInt64): AfxAtomicFetchOr64((pobj_),(value_),(order_)); break;\
case sizeof(afxInt32): AfxAtomicFetchOr32((pobj_),(value_),(order_)); break;\
default: AfxThrowError(); break; \
}

AFX afxInt32 AfxAtomicFetchXor32(volatile afxInt32* obj, afxInt32 value, afxMemoryOrder order);
AFX afxInt64 AfxAtomicFetchXor64(volatile afxInt64* obj, afxInt64 value, afxMemoryOrder order);

#define AfxAtomicFetchXor(pobj_, value_, order_) \
switch(sizeof(*(pobj_))) \
{ \
case sizeof(afxInt64): AfxAtomicFetchXor64((pobj_),(value_),(order_)); break;\
case sizeof(afxInt32): AfxAtomicFetchXor32((pobj_),(value_),(order_)); break;\
default: AfxThrowError(); break; \
}

AFX afxInt32 AfxAtomicFetchAnd32(volatile afxInt32* obj, afxInt32 value, afxMemoryOrder order);
AFX afxInt64 AfxAtomicFetchAnd64(volatile afxInt64* obj, afxInt64 value, afxMemoryOrder order);

#define AfxAtomicFetchAnd(pobj_, value_, order_) \
switch(sizeof(*(pobj_))) \
{ \
case sizeof(afxInt64): AfxAtomicFetchAnd64((pobj_),(value_),(order_)); break;\
case sizeof(afxInt32): AfxAtomicFetchAnd32((pobj_),(value_),(order_)); break;\
default: AfxThrowError(); break; \
}

AFX void AfxAtomicFlagClear(volatile afxBool* obj, afxMemoryOrder order);
AFX afxBool AfxAtomicFlagTestAndSet(volatile afxBool* obj, afxMemoryOrder order);

AFX void AfxAtomicSignalFence(afxMemoryOrder order);
AFX void AfxAtomicThreadFence(afxMemoryOrder order);

#endif//AFX_ATOMIC_H
