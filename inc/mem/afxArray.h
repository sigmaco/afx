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

#ifndef AFX_ARRAY_H
#define AFX_ARRAY_H

#include "qwadro/inc/base/afxDebug.h"
#include "qwadro/inc/base/afxFcc.h"
#include "qwadro/inc/mem/afxMemory.h"

// The elements are stored contiguously, which means that elements can be accessed not only through iterators, but also using offsets to regular pointers to elements.
// This means that a pointer to an element of a vector may be passed to any function that expects a pointer to an element of an array.

// The storage of the vector is handled automatically, being expanded and contracted as needed.
// Vectors usually occupy more space than static arrays, because more memory is allocated to handle future growth.
// This way a vector does not need to reallocate each time an element is inserted, but only when the additional memory is exhausted.
// The total amount of allocated memory can be queried using capacity() function. Extra memory can be returned to the system via a call to shrink_to_fit().

// Reallocations are usually costly operations in terms of performance. The reserve() function can be used to eliminate reallocations if the number of elements is known beforehand.

// The complexity(efficiency) of common operations on vectors is as follows :
//  - Random access - constant O(1)
//  - Insertion or removal of elements at the end - amortized constant O(1)
//  - Insertion or removal of elements - linear in the distance to the end of the vector O(n)

// std::vector(for T other than bool) meets the requirements of Container, MemoryAwareContainer, SequenceContainer, ContiguousContainer(since C++17) and ReversibleContainer.

AFX_DEFINE_STRUCT(afxArray)
{
    afxNat              unitSiz;
    afxNat              cap;
    afxNat              cnt;
    union
    {
        afxByte*        bytemap;
        void*           data;
    };
    afxMmu           mem;
};

AFX_DEFINE_STRUCT(afxArrayInfo)
{
    afxNat          unitSiz; // size of storage unit
    afxNat          unitAlign;
    afxNat          estCap; // estimated cap
    afxMmu      ctx;
};

#define AfxArray(type_, cap_, initialVal_) AfxAllocateArray((cap_), sizeof(type_), (initialVal_) ? ((type_ const[]){(initialVal_)}) : NIL)

AFXINL afxError     AfxWrapArray(afxArray* arr, afxNat unitSiz, afxNat cap, void* buf, afxNat cnt);
AFXINL afxError     AfxAllocateArray(afxArray* arr, afxNat cap, afxNat unitSiz, void const* const initial);
AFXINL void         AfxDeallocateArray(afxArray *arr);

AFXINL void*        AfxInsertArrayUnit(afxArray *arr, afxNat *idx);
AFXINL void*        AfxInsertArrayUnits(afxArray *arr, afxNat cnt, afxNat *firstIdx, void const *initialVal);
AFXINL void         AfxPopArrayUnits(afxArray *arr, afxNat cnt);

AFXINL afxError     AfxAddArrayUnits(afxArray *arr, afxNat cnt, void const* src, afxNat srcStride, afxNat* firstIdx);
AFXINL afxError     AfxUpdateArrayUnits(afxArray *arr, afxNat base, afxNat cnt, void const* src, afxNat srcStride);

//AFXINL void     AfxGetArrayElements(afxArray *arr, afxNat first, afxNat cnt, void *dst);
//AFXINL void     AfxGetArrayElement(afxArray *arr, afxNat idx, void *dst);

AFXINL afxNat       AfxCountArrayElements(afxArray const *arr);

AFXINL afxError     AfxReserveArraySpace(afxArray *arr, afxNat cap);

#define             AfxForEachArrayUnit(arr_, iterIdx_, unitPtr_) for (afxNat iterIdx_ = 0; (arr_)->cnt > iterIdx_, (unitPtr_ = AfxGetArrayUnit(arr_, iterIdx_)); iterIdx_++)
#define             AfxForEachArrayItem(arr_, iterIdx_, unitPtr_) for (afxNat iterIdx_ = 0; (arr_)->cnt > iterIdx_, (unitPtr_ = AfxGetArrayUnit(arr_, iterIdx_)); iterIdx_++)

//

AFXINL afxError     AfxSwapArray(afxArray *arr, afxArray *other);
AFXINL afxError     AfxAppendArray(afxArray *arr, afxArray const *src);

// OCCUPATION

AFXINL afxNat       AfxGetArrayCap(afxArray const *arr);
AFXINL afxNat       AfxCountArrayElements(afxArray const *arr);

AFXINL afxBool      AfxArrayIsFull(afxArray const *arr);
AFXINL afxBool      AfxArrayIsEmpty(afxArray const *arr);

AFXINL void         AfxEmptyArray(afxArray *arr);

// UNIT

AFXINL void         AfxZeroArray(afxArray *arr);
AFXINL void         AfxZeroArrayRange(afxArray *arr, afxNat firstUnit, afxNat unitCnt);

AFXINL void         AfxFillArray(afxArray *arr, void const* val);
AFXINL void         AfxFillArrayRange(afxArray *arr, afxNat firstUnit, afxNat unitCnt, void const* val);

AFXINL void*        AfxGetArrayUnit(afxArray const *arr, afxNat unitIdx);
AFXINL void*        AfxGetLastArrayUnit(afxArray const *arr);

AFXINL void         AfxDumpArray(afxArray const *arr, void* dst);
AFXINL void         AfxDumpArrayRange(afxArray const *arr, afxNat firstUnit, afxNat unitCnt, void* dst);

AFXINL void         AfxUpdateArray(afxArray* arr, void const* src);
AFXINL void         AfxUpdateArrayRange(afxArray* arr, afxNat firstUnit, afxNat unitCnt, void const* src);

// ELEMENT

#endif//AFX_ARRAY_H
