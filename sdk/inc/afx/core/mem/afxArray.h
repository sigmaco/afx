/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *              T H E   Q W A D R O   E X E C U T I O N   E C O S Y S T E M
 *
 *                                   Public Test Build
 *                   (c) 2017 SIGMA Technology Group — Federação SIGMA
 *                                    www.sigmaco.org
 */

#ifndef AFX_ARRAY_H
#define AFX_ARRAY_H

#include "afx/core/diag/afxDebug.h"
#include "afx/core/afxFcc.h"
#include "afx/core/mem/afxContext.h"

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
    _AFX_DBG_FCC;
    afxNat              unitSiz;
    afxNat              cap;
    afxNat              cnt;
    union
    {
        afxByte*        bytemap;
        void*           data;
    };
    afxContext           mem;
};

AFX void        AfxAcquireArray(afxArray *arr, afxNat unitSiz, afxNat cap, afxHint const hint);
AFX void        AfxReleaseArray(afxArray *arr);

AFX void*       AfxGetArrayUnit(afxArray const *arr, afxNat idx);
AFX void*       AfxInsertArrayUnit(afxArray *arr, afxNat *idx);
AFX afxError    AfxInsertArrayUnits(afxArray *arr, afxNat cnt, afxNat *firstIdx, void const *initialVal);
AFX void        AfxPopArrayUnits(afxArray *arr, afxNat cnt);

AFX void        AfxEmptyArray(afxArray *arr);
AFX void        AfxZeroArray(afxArray *arr);

AFX void        AfxSerializeArray(afxArray const *arr, void *dst);

AFXINL afxNat   AfxGetArrayPop(afxArray const *arr);

AFXINL afxError AfxReserveArraySpace(afxArray *arr, afxNat cap);

#define         AfxForEachArrayUnit(arr_, iterIdx_, unitPtr_) for (afxNat iterIdx_ = 0; iterIdx_ < AfxGetArrayPop(arr_), (unitPtr_ = AfxGetArrayUnit(arr_, iterIdx_)); iterIdx_++)

#endif//AFX_ARRAY_H