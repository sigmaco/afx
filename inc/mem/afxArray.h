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

/**
    The elements are stored contiguously, which means that elements can be accessed not only 
    through iterators, but also using offsets to regular pointers to elements.
    This means that a pointer to an element of a vector may be passed to any function that 
    expects a pointer to an element of an array.

    The storage of the vector is handled automatically, being expanded and contracted as needed.
    Vectors usually occupy more space than static arrays, because more memory is allocated to handle 
    future growth.
    This way a vector does not need to reallocate each time an element is inserted, but only when 
    the additional memory is exhausted.
    The total amount of allocated memory can be queried using capacity() function. Extra memory can 
    be returned to the system via a call to shrink_to_fit().

    Reallocations are usually costly operations in terms of performance. The reserve() function can 
    be used to eliminate reallocations if the number of elements is known beforehand.

    The complexity(efficiency) of common operations on vectors is as follows:
    - Random access - constant O(1)
    - Insertion or removal of elements at the end - amortized constant O(1)
    - Insertion or removal of elements - linear in the distance to the end of the vector O(n)

    std::vector(for T other than bool) meets the requirements of Container, MemoryAwareContainer, SequenceContainer, ContiguousContainer(since C++17) and ReversibleContainer.
*/

AFX_DEFINE_STRUCT(afxArray)
{
    afxUnit      unitSiz;
    afxUnit      cap;
    afxUnit      pop;
    union
    {
        afxByte*bytemap;
        void*   data;
    };
};

#define AfxArray(type_, cap_, initialVal_) AfxMakeArray((cap_), sizeof(type_), (initialVal_) ? ((type_ const[]){(initialVal_)}) : NIL)

AFXINL afxError     AfxWrapArray(afxArray* arr, afxUnit unitSiz, afxUnit cap, void* buf, afxUnit cnt);
AFXINL afxError     AfxMakeArray(afxArray* arr, afxUnit cap, afxUnit unitSiz, void const* const initial);
AFXINL void         AfxCleanUpArray(afxArray *arr);

AFXINL void*        AfxInsertArrayUnit(afxArray *arr, afxUnit *idx);
AFXINL void*        AfxInsertArrayUnits(afxArray *arr, afxUnit cnt, afxUnit *firstIdx, void const *initialVal);
AFXINL void         AfxPopArrayUnits(afxArray *arr, afxUnit cnt);

AFXINL afxError     AfxAddArrayUnits(afxArray *arr, afxUnit cnt, void const* src, afxUnit srcStride, afxUnit* firstIdx);
AFXINL afxError     AfxUpdateArrayUnits(afxArray *arr, afxUnit base, afxUnit cnt, void const* src, afxUnit srcStride);

//AFXINL void     AfxGetArrayElements(afxArray *arr, afxUnit first, afxUnit cnt, void *dst);
//AFXINL void     AfxGetArrayElement(afxArray *arr, afxUnit idx, void *dst);

AFXINL afxError     AfxReserveArraySpace(afxArray *arr, afxUnit cap);

#define             AfxForEachArrayUnit(arr_, iterIdx_, unitPtr_) for (afxUnit iterIdx_ = 0; (arr_)->cnt > iterIdx_, (unitPtr_ = AfxGetArrayUnit(arr_, iterIdx_)); iterIdx_++)
#define             AfxForEachArrayItem(arr_, iterIdx_, unitPtr_) for (afxUnit iterIdx_ = 0; (arr_)->cnt > iterIdx_, (unitPtr_ = AfxGetArrayUnit(arr_, iterIdx_)); iterIdx_++)

//

AFXINL afxError     AfxSwapArray(afxArray *arr, afxArray *other);
AFXINL afxError     AfxAppendArray(afxArray *arr, afxArray const *src);

// OCCUPATION

AFXINL afxBool      AfxIsArrayFull(afxArray const *arr);
AFXINL afxBool      AfxIsArrayEmpty(afxArray const *arr);

AFXINL void         AfxEmptyArray(afxArray *arr);

// UNIT

AFXINL void         AfxZeroArray(afxArray *arr, afxUnit firstUnit, afxUnit unitCnt);
AFXINL void         AfxFillArray(afxArray *arr, afxUnit firstUnit, afxUnit unitCnt, void const* val);

AFXINL void         AfxDumpArray(afxArray const *arr, afxUnit firstUnit, afxUnit unitCnt, void* dst);
AFXINL void         AfxUpdateArray(afxArray* arr, afxUnit firstUnit, afxUnit unitCnt, void const* src);

AFXINL void*        AfxGetArrayUnit(afxArray const *arr, afxUnit unitIdx);
AFXINL void*        AfxGetLastArrayUnit(afxArray const *arr);

// ELEMENT

#endif//AFX_ARRAY_H
