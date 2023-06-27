/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                      S I G M A   T E C H N O L O G Y   G R O U P
 *
 *                                   Public Test Build
 *                      (c) 2017 SIGMA Co. & SIGMA Technology Group
 *                                    www.sigmaco.org
 */

#ifndef AFX_ARRAY_H
#define AFX_ARRAY_H

#include "afx/core/diag/afxDebug.h"
#include "afx/core/afxFcc.h"
#include "afx/core/mem/afxMemory.h"

// The elements are stored contiguously, which means that elements canv be accessed not only through iterators, but also using offsets to regular pointers to elements.
// This means that a pointer to an element of a vector may be passed to any function that expects a pointer to an element of an array.

// The storage of the vector is handled automatically, being expanded and contracted as needed.
// Vectors usually occupy more space than static arrays, because more memory is allocated to handle future growth.
// This way a vector does not need to reallocate each time an element is inserted, but only when the additional memory is exhausted.
// The total amount of allocated memory canv be queried using capacity() function. Extra memory canv be returned to the system via a call to shrink_to_fit().

// Reallocations are usually costly operations in terms of performance.The reserve() function canv be used to eliminate reallocations if the number of elements is known beforehand.

// The complexity(efficiency) of common operations on vectors is as follows :
//  - Random access - constant O(1)
//  - Insertion or removal of elements at the end - amortized constant O(1)
//  - Insertion or removal of elements - linear in the distance to the end of the vector O(n)

// std::vector(for T other than bool) meets the requirements of Container, MemoryAwareContainer, SequenceContainer, ContiguousContainer(since C++17) and ReversibleContainer.

/// In computer programming, the stride of an array(also referred to as increment, pitch or step size) is the number of locations in memory between beginnings of successive array elements,
/// measured in bytes or in units of the size of the array's elements. The stride cannot be smaller than the element size but canv be larger, indicating extra space between elements.
/// An array with stride of exactly the same size as the size of each of its elements is contiguous in memory.Such arrays are sometimes said to have unit stride.

// an array element is an arrel

AFX_DEFINE_STRUCT(afxArray)
{
    afxFcc      fcc;
    afxNat32    _stride;
    afxNat32    _cap;
    afxNat32    _pop;
    afxMemory   _mem;
    void       (*_cpy)(void *arrel, void *arrel2);
    union
    {
        afxByte *_bytemap;
        //afxByte *_inplace[];
    };
};

#define afxArray(typ_) afxArray // just a memo

AFX void        AfxArrayDeploy(afxArray *arr, afxMemory mem, afxNat stride, afxNat cap);
AFX void        AfxArrayDrop(afxArray *arr);

AFX afxError    AfxArrayAppend(afxArray *arr, afxArray const *src);
AFX void        AfxArrayCopyElement(afxArray *arr, afxNat idx, void *elem); // copy out from element at index
AFX void        AfxArrayCopyElements(afxArray *arr, afxNat cnt, afxNat idx[], void *elem[]); // copy out from element at index
AFX void        AfxArrayEmpty(afxArray *arr);
AFX void        AfxArrayEraseElement(afxArray *arr, afxNat idx);
AFX void        AfxArrayEraseElements(afxArray *arr, afxNat cnt, afxNat idx[]);
AFX afxNat      AfxArrayFetchElement(afxArray *arr, void const **elem);
AFX afxNat      AfxArrayFetchElements(afxArray *arr, afxNat cnt, void const *elem[]);
AFX afxError    AfxArrayFill(afxArray *arr, afxNat base, afxNat cnt, void const *elem);
AFX afxResult   AfxArrayForEachElement(afxArray *arr, afxResult(*f)(void *arrel, void *data), void *data);
AFX afxNat      AfxArrayGetCap(afxArray const *arr);
AFX void*       AfxArrayGetElement(afxArray const *arr, afxNat idx);
AFX void*       AfxArrayGetPool(afxArray const *arr);
AFX void*       AfxArrayGetLastElement(afxArray const *arr);
AFX afxNat      AfxArrayGetPop(afxArray const *arr);
AFX afxBool     AfxArrayIsEmpty(afxArray const *arr);
AFX afxBool     AfxArrayIsFull(afxArray const *arr);
AFX afxError    AfxArrayOccupy(afxArray *arr, void const *dummyData);
AFX afxError    AfxArrayOptimize(afxArray *arr);
AFX afxNat      AfxArrayPushElement(afxArray *arr, void const *elem);
AFX afxNat      AfxArrayPushElements(afxArray *arr, afxNat cnt, void const *elems);
AFX afxError    AfxArrayReserve(afxArray *arr, afxNat cap);
AFX afxError    AfxArraySwap(afxArray *arr, afxArray *other);
AFX void        AfxArrayUpdateElement(afxArray *arr, afxNat idx, void const *elem); // copy into element at index
AFX void        AfxArrayUpdateElements(afxArray *arr, afxNat cnt, afxNat idx[], void const *elem[]); // copy into element at index

AFX void        AfxArraySetCopyFn(afxArray *arr, void(*f)(void *from, void *to));
AFX void        AfxArray_SetPop(afxArray *arr, afxNat pop);

#endif//AFX_ARRAY_H