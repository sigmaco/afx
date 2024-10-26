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

#include "../dev/afxExecImplKit.h"

_AFXINL afxBool AfxIsArrayFull(afxArray const *arr)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(arr);
    return (arr->pop == arr->cap);
}

_AFXINL afxBool AfxIsArrayEmpty(afxArray const *arr)
{
    // AfxIsArrayEmpty() - Checks if the container has no elements, i.e. whether begin() == end().

    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(arr);
    return (arr->pop == 0);
}

_AFXINL void AfxEmptyArray(afxArray *arr)
{
    // AfxEmptyArray() - Erases mem elements from the container. After this call, size() returns zero.
    // Invalidates any references, pointers, or iterators referring to contained elements.Any past - the - end iterators are also invalidated.
    // Leaves the capacity() of the vector unchanged(note: the standard's restriction on the changes to capacity is in the specification of vector::reserve, see [1])

    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(arr);
    arr->pop = 0;
}

_AFXINL void AfxFillArray(afxArray *arr, afxUnit firstUnit, afxUnit unitCnt, void const* val)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(arr);
    AFX_ASSERT_RANGE(arr->pop, firstUnit, unitCnt);
    AfxReserveArraySpace(arr, firstUnit + unitCnt);
    AfxFill2(&arr->bytemap[firstUnit * arr->unitSiz], val, arr->unitSiz, unitCnt);
}

_AFXINL void AfxZeroArray(afxArray *arr, afxUnit firstUnit, afxUnit unitCnt)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(arr);
    AFX_ASSERT_RANGE(arr->pop, firstUnit, unitCnt);
    AfxReserveArraySpace(arr, firstUnit + unitCnt);
    AfxZero2(&arr->bytemap[firstUnit * arr->unitSiz], arr->unitSiz, unitCnt);
}

_AFXINL void AfxCleanUpArray(afxArray *arr)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(arr);
    AfxEmptyArray(arr);

    if (arr->bytemap)
        AfxDeallocate(arr->bytemap), arr->bytemap = NIL;
}

_AFXINL afxError AfxMakeArray(afxArray* arr, afxUnit cap, afxUnit unitSiz, void const* const initial)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(arr);
    AFX_ASSERT(unitSiz);
    arr->unitSiz = unitSiz;
    arr->cap = cap;
    arr->pop = 0;
    arr->bytemap = NIL;

    if (initial && cap)
    {
        AfxReserveArraySpace(arr, cap);
        AfxFill2(arr->bytemap, initial, unitSiz, cap);
    }
    return err;
}

_AFXINL afxError AfxWrapArray(afxArray* arr, afxUnit unitSiz, afxUnit cap, void* buf, afxUnit cnt)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(arr);
    arr->unitSiz = unitSiz;
    arr->cap = cap;
    arr->pop = cnt;
    arr->bytemap = buf;
    return err;
}

_AFXINL afxError AfxOptimizeArray(afxArray *arr)
{
    // AfxOptimizeArray() - Requests the removal of unused capacity.
    // It is a non - binding request to reduce capacity() to size().It depends on the implementation whether the request is fulfilled.
    // If reallocation occurs, mem iterators, including the past the end iterator, and mem references to the elements are invalidated.
    // If no reallocation takes place, no iterators or references are invalidated.

    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(arr);
    AfxLogError("To be implemented. Should release unused reserved memory.");
    AfxThrowError();
    return err;
}

_AFXINL afxError AfxReserveArraySpace(afxArray *arr, afxUnit cap)
{
    // AfxReserveArraySpace() - Increase the capacity of the vector (the total number of elements that the vector canv hold without requiring reallocation) to a value that's greater or equal to newcap.
    // If newcap is greater than the current capacity(), new storage is allocated, otherwise the function does nothing.
    // reserve() does not change the size of the vector.
    // If newcap is greater than capacity(), mem iterators, including the past -the- end iterator, and mem references to the elements are invalidated. Otherwise, no iterators or references are invalidated.

    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(arr);
    AFX_ASSERT(cap);

    if ((cap > arr->cap) || !arr->bytemap)
    {
        if (!arr->bytemap)
            cap = AfxMax(cap, arr->cap);

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

_AFXINL void* AfxGetArrayUnit(afxArray const *arr, afxUnit unitIdx)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(arr);
    AFX_ASSERT_RANGE(arr->pop, unitIdx, 1);
    //AfxReserveArraySpace((void*)arr, unitIdx + 1);
    AFX_ASSERT(arr->bytemap);
    return &(arr->bytemap[unitIdx * arr->unitSiz]);
}

_AFXINL void* AfxGetLastArrayUnit(afxArray const *arr)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(arr);
    AFX_ASSERT(arr->bytemap);
    return arr->pop ? &(arr->bytemap[(arr->pop - 1) * arr->unitSiz]) : NIL;
}

_AFXINL void AfxUpdateArray(afxArray* arr, afxUnit firstUnit, afxUnit unitCnt, void const* src)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(arr);
    AFX_ASSERT_RANGE(arr->pop, firstUnit, unitCnt);
    AFX_ASSERT(arr->bytemap);
    AFX_ASSERT(src);
    AfxCopy2(&(arr->bytemap[firstUnit * arr->unitSiz]), src, arr->unitSiz, unitCnt);
}

_AFXINL void AfxDumpArray(afxArray const* arr, afxUnit firstUnit, afxUnit unitCnt, void *dst)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(arr);
    AFX_ASSERT_RANGE(arr->pop, firstUnit, unitCnt);
    AFX_ASSERT(arr->bytemap);
    AFX_ASSERT(dst);
    AfxCopy2(dst, &arr->bytemap[firstUnit * arr->unitSiz], arr->unitSiz, unitCnt);
}

_AFXINL void* AfxInsertArrayUnits(afxArray* arr, afxUnit cnt, afxUnit* firstIdx, void const *initialVal)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(arr);
    AFX_ASSERT(cnt);
    afxByte *first;

    if (AfxIsArrayFull(arr) || !arr->bytemap)
        if (AfxReserveArraySpace(arr, arr->pop + cnt))
            AfxThrowError();

    if (err)
    {
        first = NIL;

        if (firstIdx)
            *firstIdx = AFX_INVALID_INDEX;
    }
    else
    {
        AFX_ASSERT(arr->cap >= arr->pop + cnt);

        afxUnit baseIdx = arr->pop;
        arr->pop += cnt;

        if (firstIdx)
            *firstIdx = baseIdx;

        first = AfxGetArrayUnit(arr, baseIdx);

        if (!initialVal)
            AfxZero2(first, arr->unitSiz, cnt);
        else
            AfxFill2(first, initialVal, arr->unitSiz, cnt);
    }
    return first;
}

_AFXINL void* AfxInsertArrayUnit(afxArray *arr, afxUnit *idx)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(arr);
    void *ptr;

    if (!(ptr = AfxInsertArrayUnits(arr, 1, idx, NIL)))
        AfxThrowError();

    return ptr;
}

_AFXINL afxError AfxAddArrayUnits(afxArray *arr, afxUnit cnt, void const* src, afxUnit srcStride, afxUnit *firstIdx)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(arr);
    AFX_ASSERT(cnt);
    *firstIdx = AFX_INVALID_INDEX;

    if (AfxIsArrayFull(arr) || !arr->bytemap)
        if (AfxReserveArraySpace(arr, arr->pop + cnt))
            AfxThrowError();

    if (!err)
    {
        *firstIdx = arr->pop;

        AFX_ASSERT(arr->cap >= arr->pop + cnt);

        afxByte* map = &arr->bytemap[arr->pop * arr->unitSiz];

        if (!src)
        {
            AfxZero2(map, arr->unitSiz, cnt);
        }
        else
        {
            AFX_ASSERT(srcStride);

            if (srcStride == arr->unitSiz)
            {
                AfxCopy2(map, src, arr->unitSiz, cnt);
            }
            else
            {
                afxByte const *src2 = src;

                for (afxUnit i = 0; i < cnt; i++)
                {
                    AfxCopy(&(map[i * arr->unitSiz]), &(src2[i * srcStride]), arr->unitSiz);
                }
            }
        }
        arr->pop += cnt;
    }
    return err;
}

_AFXINL afxError AfxUpdateArrayUnits(afxArray *arr, afxUnit base, afxUnit cnt, void const* src, afxUnit srcStride)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(arr);
    AFX_ASSERT_RANGE(arr->pop, base, cnt);
    AFX_ASSERT(srcStride);

    afxByte* map = &arr->bytemap[base * arr->unitSiz];

    if (srcStride == arr->unitSiz)
    {
        AfxCopy2(map, src, arr->unitSiz, cnt);
    }
    else
    {
        afxByte const *src2 = src;

        for (afxUnit i = 0; i < cnt; i++)
        {
            AfxCopy(&(map[i * arr->unitSiz]), &(src2[i * srcStride]), arr->unitSiz);
        }
    }

    return err;
}

_AFXINL afxError AfxAppendArray(afxArray* arr, afxArray const *src)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(arr);
    AFX_ASSERT(src);
    afxUnit idx;

    if (AfxReserveArraySpace(arr, arr->pop + src->pop)) AfxThrowError();
    else
    {
        afxUnit firstIdx = arr->pop;
        AFX_ASSERT(arr->cap >= arr->pop + src->pop);

        afxByte* map = &arr->bytemap[firstIdx * arr->unitSiz];

        if (arr->unitSiz == src->unitSiz)
        {
            AfxCopy2(map, src->data, src->unitSiz, src->pop);
        }
        else
        {
            afxByte const *src2 = src->bytemap;

            for (afxUnit i = 0; i < src->pop; i++)
            {
                AfxCopy(&(map[i * arr->unitSiz]), &(src2[i * src->unitSiz]), arr->unitSiz);
            }
        }
        arr->pop += src->pop;
    }
    return err;
}

_AFXINL afxError AfxSwapArray(afxArray *arr, afxArray *other)
{
    // AfxSwapArray() - Exchanges the contents of the container with those of other.Does not invoke any move, copy, or swap operations on individual elements.
    // All iterators and references remain valid. The past-the-end iterator is invalidated.

    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(arr);
    AFX_ASSERT(other);

    afxUnit32 stride = other->unitSiz, cap = other->cap, pop = other->pop;
    afxByte *bytemap = other->bytemap;
    
    other->unitSiz = arr->unitSiz;
    other->cap = arr->cap;
    other->pop = arr->pop;
    other->bytemap = arr->bytemap;

    arr->unitSiz = stride;
    arr->cap = cap;
    arr->pop = pop;
    arr->bytemap = bytemap;

    return err;
}
