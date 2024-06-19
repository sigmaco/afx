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

#include "qwadro/exec/afxSystem.h"

#if !0

_AFXINL afxNat AfxGetArrayCap(afxArray const *arr)
{
    afxError err = AFX_ERR_NONE;
    //AfxAssertType(arr, afxFcc_ARR);
    return arr->cap;
}

_AFXINL afxNat AfxCountArrayElements(afxArray const *arr)
{
    afxError err = AFX_ERR_NONE;
    //AfxAssertType(arr, afxFcc_ARR);
    return arr->cnt;
}

_AFXINL void AfxArraySetPop_(afxArray *arr, afxNat pop)
{
    afxError err = AFX_ERR_NONE;
    //AfxAssertType(arr, afxFcc_ARR);
    AfxAssert(arr->cap >= pop);
    arr->cnt = pop;
}

_AFXINL afxBool AfxArrayIsFull(afxArray const *arr)
{
    afxError err = AFX_ERR_NONE;
    //AfxAssertType(arr, afxFcc_ARR);
    return arr->cnt == arr->cap;
}

// AfxArrayIsEmpty() - Checks if the container has no elements, i.e. whether begin() == end().

_AFXINL afxBool AfxArrayIsEmpty(afxArray const *arr)
{
    afxError err = AFX_ERR_NONE;
    //AfxAssertType(arr, afxFcc_ARR);
    return !arr->cnt;
}

// AfxEmptyArray() - Erases mem elements from the container. After this call, size() returns zero.

// Invalidates any references, pointers, or iterators referring to contained elements.Any past - the - end iterators are also invalidated.

// Leaves the capacity() of the vector unchanged(note: the standard's restriction on the changes to capacity is in the specification of vector::reserve, see [1])

_AFXINL void AfxEmptyArray(afxArray *arr)
{
    afxError err = AFX_ERR_NONE;
    //AfxAssertType(arr, afxFcc_ARR);
    arr->cnt = 0;
}

_AFXINL void AfxFillArrayRange(afxArray *arr, afxNat first, afxNat cnt, void const* val)
{
    afxError err = AFX_ERR_NONE;
    //AfxAssertType(arr, afxFcc_ARR);
    AfxAssertRange(arr->cnt, first, cnt);
    AfxReserveArraySpace(arr, first + cnt);
    AfxFill2(cnt, arr->unitSiz, val, &arr->bytemap[first * arr->unitSiz]);
}

_AFXINL void AfxFillArray(afxArray *arr, void const* val)
{
    afxError err = AFX_ERR_NONE;
    //AfxAssertType(arr, afxFcc_ARR);
    arr->cnt = arr->cap;
    AfxFillArrayRange(arr, 0, arr->cap, val);
}

_AFXINL void AfxZeroArrayRange(afxArray *arr, afxNat firstUnit, afxNat unitCnt)
{
    afxError err = AFX_ERR_NONE;
    //AfxAssertType(arr, afxFcc_ARR);
    AfxAssertRange(arr->cnt, firstUnit, unitCnt);
    AfxReserveArraySpace(arr, firstUnit + unitCnt);
    AfxZero2(unitCnt, arr->unitSiz, &arr->bytemap[firstUnit * arr->unitSiz]);
}

_AFXINL void AfxZeroArray(afxArray *arr)
{
    afxError err = AFX_ERR_NONE;
    //AfxAssertType(arr, afxFcc_ARR);    
    arr->cnt = arr->cap;
    AfxZeroArrayRange(arr, 0, arr->cnt);
}

_AFXINL void AfxDeallocateArray(afxArray *arr)
{
    afxError err = AFX_ERR_NONE;
    //AfxAssertType(arr, afxFcc_ARR);
    AfxAssignTypeFcc(arr, 0);

    if (arr->bytemap)
        AfxDeallocate(arr->bytemap);

    arr->bytemap = NIL;
    arr->mem = NIL;
}

_AFXINL afxError AfxAllocateArray(afxArray* arr, afxNat cap, afxNat unitSiz, void const* const initial)
{
    afxError err = AFX_ERR_NONE;
    //afxArray arr;
    AfxAssert(arr);
    AfxAssert(unitSiz);
    //AfxAssignTypeFcc(arr, afxFcc_ARR);
    arr->cap = cap;
    arr->cnt = 0;
    arr->unitSiz = unitSiz;
    arr->mem = NIL;
    arr->bytemap = NIL;

    if (initial && arr->cap)
    {
        AfxReserveArraySpace(arr, cap);
        AfxFill2(cap, unitSiz, initial, arr->bytemap);
    }
    return err;
}

// AfxOptimizeArray() - Requests the removal of unused capacity.

// It is a non - binding request to reduce capacity() to size().It depends on the implementation whether the request is fulfilled.

// If reallocation occurs, mem iterators, including the past the end iterator, and mem references to the elements are invalidated.
// If no reallocation takes place, no iterators or references are invalidated.

_AFXINL afxError AfxOptimizeArray(afxArray *arr)
{
    afxError err = AFX_ERR_NONE;
    //AfxAssertType(arr, afxFcc_ARR);
    AfxLogError("To be implemented. Should release unused reserved memory.");
    AfxThrowError();
    return err;
}

// AfxReserveArraySpace() - Increase the capacity of the vector (the total number of elements that the vector canv hold without requiring reallocation) to a value that's greater or equal to newcap.
// If newcap is greater than the current capacity(), new storage is allocated, otherwise the function does nothing.

// reserve() does not change the size of the vector.

// If newcap is greater than capacity(), mem iterators, including the past -the- end iterator, and mem references to the elements are invalidated. Otherwise, no iterators or references are invalidated.

_AFXINL afxError AfxReserveArraySpace(afxArray *arr, afxNat cap)
{
    afxError err = AFX_ERR_NONE;
    //AfxAssertType(arr, afxFcc_ARR);
    AfxAssert(cap);

    if ((cap > arr->cap) || !arr->bytemap)
    {
        afxByte *p;

        if (!(p = AfxReallocate(arr->bytemap, arr->unitSiz, cap, 0, AfxHere()) /*AfxReallocate(&((afxSize *)(*arr))[-2], (siz), AfxHere())*/)) AfxThrowError();
        else
        {
            arr->cap = cap;
            arr->bytemap = (afxByte*)p;
        }
    }
    return err;
}

_AFXINL void* AfxGetArrayUnit(afxArray const *arr, afxNat unitIdx)
{
    afxError err = AFX_ERR_NONE;
    //AfxAssertType(arr, afxFcc_ARR);
    AfxAssertRange(arr->cnt, unitIdx, 1);
    //AfxReserveArraySpace((void*)arr, unitIdx + 1);
    AfxAssert(arr->bytemap);
    return &(arr->bytemap[unitIdx * arr->unitSiz]);
}

_AFXINL void* AfxGetLastArrayUnit(afxArray const *arr)
{
    afxError err = AFX_ERR_NONE;
    //AfxAssertType(arr, afxFcc_ARR);
    AfxAssert(arr->bytemap);
    return arr->cnt ? &(arr->bytemap[(arr->cnt - 1) * arr->unitSiz]) : NIL;
}

_AFXINL void AfxUpdateArrayRange(afxArray *arr, afxNat firstUnit, afxNat unitCnt, void const* src)
{
    afxError err = AFX_ERR_NONE;
    //AfxAssertType(arr, afxFcc_ARR);
    AfxAssertRange(arr->cnt, firstUnit, unitCnt);
    AfxAssert(arr->bytemap);
    AfxAssert(src);
    AfxCopy2(unitCnt, arr->unitSiz, src, &(arr->bytemap[firstUnit * arr->unitSiz]));
}

_AFXINL void AfxUpdateArray(afxArray *arr, void const* src)
{
    afxError err = AFX_ERR_NONE;
    //AfxAssertType(arr, afxFcc_ARR);
    AfxAssert(src);
    AfxUpdateArrayRange(arr, 0, arr->cnt, src);
}

_AFXINL void AfxDumpArrayRange(afxArray const* arr, afxNat firstUnit, afxNat unitCnt, void *dst)
{
    afxError err = AFX_ERR_NONE;
    //AfxAssertType(arr, afxFcc_ARR);
    AfxAssertRange(arr->cnt, firstUnit, unitCnt);
    AfxAssert(arr->bytemap);
    AfxAssert(dst);
    AfxCopy2(unitCnt, arr->unitSiz, &arr->bytemap[firstUnit * arr->unitSiz], dst);
}

_AFXINL void AfxDumpArray(afxArray const* arr, void *dst)
{
    afxError err = AFX_ERR_NONE;
    //AfxAssertType(arr, afxFcc_ARR);
    AfxAssert(dst);
    AfxDumpArrayRange(arr, 0, arr->cnt, dst);
}

_AFXINL void* AfxInsertArrayUnits(afxArray *arr, afxNat cnt, afxNat *firstIdx, void const *initialVal)
{
    afxError err = AFX_ERR_NONE;
    //AfxAssertType(arr, afxFcc_ARR);
    AfxAssert(cnt);
    afxByte *first;

    if (AfxArrayIsFull(arr) || !arr->bytemap)
        if (AfxReserveArraySpace(arr, arr->cnt + cnt))
            AfxThrowError();

    if (err)
    {
        first = NIL;

        if (firstIdx)
            *firstIdx = AFX_INVALID_INDEX;
    }
    else
    {
        AfxAssert(arr->cap >= arr->cnt + cnt);

        afxNat baseIdx = arr->cnt;
        arr->cnt += cnt;

        if (firstIdx)
            *firstIdx = baseIdx;

        first = AfxGetArrayUnit(arr, baseIdx);

        if (!initialVal)
            AfxZero2(cnt, arr->unitSiz, first);
        else
            AfxFill2(cnt, arr->unitSiz, initialVal, first);
    }
    return first;
}

_AFXINL void* AfxInsertArrayUnit(afxArray *arr, afxNat *idx)
{
    afxError err = AFX_ERR_NONE;
    //AfxAssertType(arr, afxFcc_ARR);
    void *ptr;

    if (!(ptr = AfxInsertArrayUnits(arr, 1, idx, NIL)))
        AfxThrowError();

    return ptr;
}

_AFXINL afxError AfxAddArrayUnits(afxArray *arr, afxNat cnt, void const* src, afxNat srcStride, afxNat *firstIdx)
{
    afxError err = AFX_ERR_NONE;
    //AfxAssertType(arr, afxFcc_ARR);
    AfxAssert(cnt);
    *firstIdx = AFX_INVALID_INDEX;

    if (AfxArrayIsFull(arr) || !arr->bytemap)
        if (AfxReserveArraySpace(arr, arr->cnt + cnt))
            AfxThrowError();

    if (!err)
    {
        *firstIdx = arr->cnt;

        AfxAssert(arr->cap >= arr->cnt + cnt);

        afxByte* map = &arr->bytemap[arr->cnt * arr->unitSiz];

        if (!src)
        {
            AfxZero2(cnt, arr->unitSiz, map);
        }
        else
        {
            AfxAssert(srcStride);

            if (srcStride == arr->unitSiz)
            {
                AfxCopy2(cnt, arr->unitSiz, src, map);
            }
            else
            {
                afxByte const *src2 = src;

                for (afxNat i = 0; i < cnt; i++)
                {
                    AfxCopy2(1, arr->unitSiz, &(src2[i * srcStride]), &(map[i * arr->unitSiz]));
                }
            }
        }
        arr->cnt += cnt;
    }
    return err;
}

_AFXINL afxError AfxUpdateArrayUnits(afxArray *arr, afxNat base, afxNat cnt, void const* src, afxNat srcStride)
{
    afxError err = AFX_ERR_NONE;
    //AfxAssertType(arr, afxFcc_ARR);
    AfxAssertRange(arr->cnt, base, cnt);
    AfxAssert(srcStride);

    afxByte* map = &arr->bytemap[base * arr->unitSiz];

    if (srcStride == arr->unitSiz)
    {
        AfxCopy2(cnt, arr->unitSiz, src, map);
    }
    else
    {
        afxByte const *src2 = src;

        for (afxNat i = 0; i < cnt; i++)
        {
            AfxCopy2(1, arr->unitSiz, &(src2[i * srcStride]), &(map[i * arr->unitSiz]));
        }
    }

    return err;
}

_AFXINL afxNat AfxArrayFetchElement(afxArray *arr, void const **elem)
{
    afxError err = AFX_ERR_NONE;
    //AfxAssertType(arr, afxFcc_ARR);
    afxNat idx = AFX_INVALID_INDEX;

    if (AfxArrayIsFull(arr) || !arr->bytemap)
        if (AfxReserveArraySpace(arr, arr->cap + 1))
            AfxThrowError();

    if (!err)
    {
        if (elem)
        {
            *elem = &(arr->bytemap[arr->cnt * arr->unitSiz]);
        }

        if (!err)
        {
            idx = arr->cnt;
            arr->cnt++;
        }
    }
    return idx;
}

_AFXINL afxNat AfxArrayFetchElements(afxArray *arr, afxNat cnt, void const *elem[])
{
    afxError err = AFX_ERR_NONE;
    //AfxAssertType(arr, afxFcc_ARR);
    AfxAssert(cnt);
    afxNat idx = AFX_INVALID_INDEX;

    if (AfxArrayIsFull(arr) || !arr->bytemap)
        if (AfxReserveArraySpace(arr, arr->cap + cnt))
            AfxThrowError();

    if (!err)
    {
        if (elem)
        {
            *elem = &(arr->bytemap[arr->cnt * arr->unitSiz]);
        }

        if (!err)
        {
            idx = arr->cnt;
            arr->cnt++;
        }
    }
    return idx;
}

// AfxArrayEraseElement() - Erases the specified elements from the container.
//  - 1) Removes the element at pos.
// Invalidates iterators and references at or after the point of the erase, including the end() iterator.

// The iterator pos must be valid and dereferenceable.Thus the end() iterator(which is valid, but is not dereferenceable) cannot be used as a value for pos.

// The iterator first does not need to be dereferenceable if first == last: erasing an empty range is a no - op.

_AFXINL afxResult AfxArrayForEachElement(afxArray *arr, afxResult(*f)(void *arrel, void *data), void *data)
{
    afxError err = AFX_ERR_NONE;
    //AfxAssertType(arr, afxFcc_ARR);
    AfxAssert(f);
    afxResult cnt = 0;

    for (afxNat i = 0; i < arr->cnt; i++)
    {
        void *arrel = AfxGetArrayUnit(arr, i);

        cnt++;

        if (!(f(arrel, data)))
            break;
    }
    return cnt;
}

_AFXINL afxError AfxAppendArray(afxArray *arr, afxArray const *src)
{
    afxError err = AFX_ERR_NONE;
    //AfxAssertType(arr, afxFcc_ARR);
    //AfxAssertType(src, afxFcc_ARR);
    afxNat idx;

    if (AfxReserveArraySpace(arr, arr->cnt + src->cnt)) AfxThrowError();
    else if (AfxAddArrayUnits(arr, src->cnt, src->bytemap, arr->unitSiz, &idx))
        AfxThrowError();

    return err;
}

// AfxSwapArray() - Exchanges the contents of the container with those of other.Does not invoke any move, copy, or swap operations on individual elements.
// All iterators and references remain valid. The past-the-end iterator is invalidated.

_AFXINL afxError AfxSwapArray(afxArray *arr, afxArray *other)
{
    afxError err = AFX_ERR_NONE;
    //AfxAssertType(arr, afxFcc_ARR);
    //AfxAssertType(other, afxFcc_ARR);

    afxNat32 stride = other->unitSiz, cap = other->cap, pop = other->cnt;
    afxByte *bytemap = other->bytemap;
    
    other->unitSiz = arr->unitSiz;
    other->cap = arr->cap;
    other->cnt = arr->cnt;
    other->bytemap = arr->bytemap;

    arr->unitSiz = stride;
    arr->cap = cap;
    arr->cnt = pop;
    arr->bytemap = bytemap;

    return err;
}

#endif
