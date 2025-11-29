/*
 *           ::::::::    :::::::::::    ::::::::    ::::     ::::       :::
 *          :+:    :+:       :+:       :+:    :+:   +:+:+: :+:+:+     :+: :+:
 *          +:+              +:+       +:+          +:+ +:+:+ +:+    +:+   +:+
 *          +#++:++#++       +#+       :#:          +#+  +:+  +#+   +#++:++#++:
 *                 +#+       +#+       +#+   +#+#   +#+       +#+   +#+     +#+
 *          #+#    #+#       #+#       #+#    #+#   #+#       #+#   #+#     #+#
 *           ########    ###########    ########    ###       ###   ###     ###
 *
 *                     S I G M A   T E C H N O L O G Y   G R O U P
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

#include "../exec/afxSystemDDK.h"

_AFXINL afxError AfxMakeArray(afxArray* arr, afxUnit unitSiz, afxUnit cap, void* buf, afxUnit pop)
{
    afxError err = { 0 };
    AFX_ASSERT(arr);
    AFX_ASSERT(unitSiz);
    AFX_ASSERT(cap >= pop);
#ifdef _AFX_ARRAY_VALIDATION_ENABLED
    arr->fcc = afxFcc_ARR;
#endif
    arr->unitSiz = unitSiz;
    arr->align = 0;
    arr->cap = AFX_MAX(1, cap);
    arr->pop = buf ? pop : 0;
    arr->bytemap = buf;
    arr->alloced = !buf;
    return err;
}

_AFXINL afxError AfxEmptyArray(afxArray* arr, afxBool dontFree, afxBool zeroOut)
{
    // AfxEmptyArray() - Erases mem elements from the container. After this call, size() returns zero.
    // Invalidates any references, pointers, or iterators referring to contained elements. Any past - the - end iterators are also invalidated.
    // Leaves the capacity() of the vector unchanged(note: the standard's restriction on the changes to capacity is in the specification of vector::reserve, see [1])

    afxError err = { 0 };
    AFX_ASSERT(arr);

    if (!dontFree)
    {
        arr->cap = 0;
        if (arr->bytemap && arr->alloced)
        {
            AfxDeallocate((void**)&arr->bytemap, AfxHere());
            arr->alloced = FALSE;
        }
    }
    else if (zeroOut && arr->bytemap)
    {
#if 0 // zero the population or the capacity?
        AfxZero(arr->bytemap, arr->pop * arr->unitSiz);
#else
        AfxZero(arr->bytemap, arr->cap * arr->unitSiz);
#endif
    }
    arr->pop = 0;
    return err;
}

_AFXINL afxBool AfxIsArrayFull(afxArray const* arr)
{
    afxError err = { 0 };
    AFX_ASSERT(arr);
    return (arr->pop == arr->cap);
}

_AFXINL afxBool AfxIsArrayEmpty(afxArray const* arr)
{
    // AfxIsArrayEmpty() - Checks if the container has no elements, i.e. whether begin() == end().

    afxError err = { 0 };
    AFX_ASSERT(arr);
    return (arr->pop == 0);
}

_AFXINL afxError AfxOptimizeArray(afxArray* arr)
{
    // AfxOptimizeArray() - Requests the removal of unused capacity.
    // It is a non - binding request to reduce capacity() to size().It depends on the implementation whether the request is fulfilled.
    // If reallocation occurs, mem iterators, including the past the end iterator, and mem references to the elements are invalidated.
    // If no reallocation takes place, no iterators or references are invalidated.

    afxError err = { 0 };
    AFX_ASSERT(arr);
    AfxReportError("To be implemented. Should release unused reserved memory.");
    AfxThrowError();
    return err;
}

_AFXINL afxError AfxReserveArraySpace(afxArray* arr, afxUnit cap)
{
    // AfxReserveArraySpace() - Increase the capacity of the vector (the total number of elements that the vector canv hold without requiring reallocation) to a value that's greater or equal to newcap.
    // If newcap is greater than the current capacity(), new storage is allocated, otherwise the function does nothing.
    // reserve() does not change the size of the vector.
    // If newcap is greater than capacity(), mem iterators, including the past -the- end iterator, and mem references to the elements are invalidated. Otherwise, no iterators or references are invalidated.

    afxError err = { 0 };
    AFX_ASSERT(arr);
    AFX_ASSERT(cap);

    // If has room and buffer, quits.
    if ((arr->cap > cap) && arr->bytemap)
        return err;

    if (!arr->bytemap)
        cap = AFX_MAX(cap, arr->cap);

    AFX_ASSERT(arr->alloced);

    if (AfxReallocate(arr->unitSiz * cap, arr->align, AfxHere(), (void**)&arr->bytemap))
    {
        AfxThrowError();
    }
    else
    {
        arr->cap = cap;
    }
    return err;
}

_AFXINL void* AfxGetArrayUnit(afxArray const* arr, afxUnit unitIdx)
{
    afxError err = { 0 };
    AFX_ASSERT(arr);
    AFX_ASSERT_RANGE(arr->pop, unitIdx, 1);
    //if (unitIdx >= arr->pop) return NIL; // bounds check
    AFX_ASSERT(arr->bytemap);
    return &(arr->bytemap[unitIdx * arr->unitSiz]);
}

_AFXINL void AfxUpdateArray(afxArray* arr, afxUnit firstUnit, afxUnit unitCnt, afxUnit offset, void const* src, afxUnit stride)
{
    afxError err = { 0 };
    AFX_ASSERT(arr);
    AFX_ASSERT_RANGE(arr->pop, firstUnit, unitCnt);
    AFX_ASSERT(arr->bytemap);
    AFX_ASSERT(src);
    //AfxCopy2(&(arr->bytemap[firstUnit * arr->unitSiz]), src, arr->unitSiz, unitCnt);
    AfxStream2(unitCnt, src, stride, &arr->bytemap[firstUnit * arr->unitSiz + offset], arr->unitSiz);
}

_AFXINL void AfxDumpArray(afxArray const* arr, afxUnit firstUnit, afxUnit unitCnt, afxUnit offset, void *dst, afxUnit stride)
{
    afxError err = { 0 };
    AFX_ASSERT(arr);
    AFX_ASSERT_RANGE(arr->pop, firstUnit, unitCnt);
    AFX_ASSERT(arr->bytemap);
    AFX_ASSERT(dst);
    //AfxCopy2(dst, &arr->bytemap[firstUnit * arr->unitSiz], arr->unitSiz, unitCnt);
    AfxStream2(unitCnt, &arr->bytemap[firstUnit * arr->unitSiz + offset], arr->unitSiz, dst, stride);
}

_AFXINL void* AfxPushArrayUnits(afxArray* arr, afxUnit cnt, afxUnit* baseUnitIdx, void const* src, afxUnit srcStride)
{
    afxError err = { 0 };
    AFX_ASSERT(arr);
    AFX_ASSERT(cnt);

    if (AfxReserveArraySpace(arr, arr->pop + cnt))
    {
        AfxThrowError();

        if (baseUnitIdx)
            *baseUnitIdx = AFX_INVALID_INDEX;
        
        return NIL;
    }
    
    AFX_ASSERT(arr->cap >= arr->pop + cnt);

    afxUnit baseIdx = arr->pop;
    arr->pop += cnt;

    if (baseUnitIdx)
        *baseUnitIdx = baseIdx;

    afxByte* first = AfxGetArrayUnit(arr, baseIdx);

    if (src)
        AfxUpdateArray(arr, baseIdx, cnt, 0, src, srcStride);

    return first;
}

_AFXINL afxError AfxInsertArrayUnits(afxArray* arr, afxUnit baseIdx, afxUnit cnt, void const* src, afxUnit srcStride)
{
    afxError err = { 0 };
    AFX_ASSERT(arr);
    AFX_ASSERT_RANGE(arr->pop, baseIdx, 1);
    AFX_ASSERT(cnt);

    if (baseIdx > arr->pop || cnt)
    {
        AfxThrowError();
        return err;
    }

    afxUnit needed = arr->pop + cnt;

    if (needed > arr->cap)
    {
        afxUnit newCap = arr->cap;
        while (newCap < needed) newCap = (newCap == 0) ? 1 : newCap * 2;

        afxError err = AfxReserveArraySpace(arr, newCap);
        if (err) return err;
    }

    afxByte* base = (afxByte*)arr->items;
    
    // Shift existing data to make room.
    if (baseIdx < arr->pop)
    {
        AfxMemmove(&base[(baseIdx + cnt) * arr->unitSiz], &base[baseIdx * arr->unitSiz], (arr->pop - baseIdx) * arr->unitSiz);
    }

    // Copy or initialize inserted units.
    afxByte* dst = base + baseIdx * arr->unitSiz;

    if (src)
    {
        afxByte const* src2 = (afxByte const*)src;

        for (afxUnit i = 0; i < cnt; ++i)
        {
            AfxMemcpy(&dst[i * arr->unitSiz], &src2[i * srcStride], arr->unitSiz);
        }
    }
#if 0
    else
    {
        AfxMemset(dest, 0, cnt * arr->unitSiz);
    }
#endif
    arr->pop += cnt;
    return err;
}

_AFXINL afxError AfxRemoveArrayUnits(afxArray* arr, afxUnit idx, afxUnit cnt)
{
    afxError err = { 0 };
    AFX_ASSERT(arr);
    AFX_ASSERT_RANGE(arr->pop, idx, cnt);

    if (idx >= arr->pop || cnt == 0 || (idx + cnt) > arr->pop)
        return err;

    afxByte* base = (afxByte*)arr->items;
    afxUnit remaining = arr->pop - (idx + cnt);
    if (remaining > 0)
    {
        AfxMemmove(&base[idx * arr->unitSiz], &base[(idx + cnt) * arr->unitSiz], remaining * arr->unitSiz);
    }

    arr->pop -= cnt;
    return err;
}

_AFXINL afxError AfxAppendArray(afxArray* arr, afxArray const* src)
{
    afxError err = { 0 };
    AFX_ASSERT(arr);
    AFX_ASSERT(src);
    AFX_ASSERT(arr != src);
    afxUnit idx;

    if (AfxReserveArraySpace(arr, arr->pop + src->pop)) AfxThrowError();
    else
    {
        afxUnit firstIdx = arr->pop;
        AFX_ASSERT(arr->cap >= arr->pop + src->pop);

        afxByte* map = &arr->bytemap[firstIdx * arr->unitSiz];

        if (arr->unitSiz == src->unitSiz)
        {
            AfxCopy(map, src->items, src->unitSiz * src->pop);
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

_AFXINL afxError AfxSwapArray(afxArray* arr, afxArray* other)
{
    // AfxSwapArray() - Exchanges the contents of the container with those of other.Does not invoke any move, copy, or swap operations on individual elements.
    // All iterators and references remain valid. The past-the-end iterator is invalidated.

    afxError err = { 0 };
    AFX_ASSERT(arr);
    AFX_ASSERT(other);
    AFX_ASSERT(arr != other);

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

_AFXINL afxError AfxCloneArray(afxArray* arr, afxArray const* src, afxUnit baseIdx, afxUnit cnt)
{
    afxError err = { 0 };
    AFX_ASSERT(arr);
    AFX_ASSERT(src);
    AFX_ASSERT(arr !=  src);
    AFX_ASSERT_RANGE(src->pop, baseIdx, cnt);

    if (baseIdx >= src->pop || (baseIdx + cnt) > src->pop)
        return err;

    arr->unitSiz = src->unitSiz;

    if (AfxReserveArraySpace(arr, cnt))
    {
        AfxThrowError();
        return err;
    }

    if (cnt > 0)
    {
        void* srcStart = (afxByte*)src->items + baseIdx * src->unitSiz;
        AfxMemcpy(arr->items, srcStart, cnt * src->unitSiz);
    }

    arr->pop = cnt;
    return err;
}

_AFXINL afxError AfxCopyArray(afxUnit unitCnt, afxUnit size, afxArray const* src, afxUnit srcUnitIdx, afxUnit srcOffset, afxArray* dst, afxUnit dstUnitIdx, afxUnit dstOffset)
{
    afxError err = { 0 };
    AFX_ASSERT(dst);
    AFX_ASSERT(src);
    AFX_ASSERT(dst != src);
    AFX_ASSERT_RANGE(dst->pop, dstUnitIdx, unitCnt);
    AFX_ASSERT_RANGE(src->pop, srcUnitIdx, unitCnt);
    AFX_ASSERT(unitCnt);
    AFX_ASSERT(size);

    if (!dst->bytemap || !src->bytemap || size == 0 || unitCnt == 0)
    {
        AfxThrowError();
        return err;
    }
    // Bounds checks
    if ((srcUnitIdx + unitCnt) > src->pop || (dstUnitIdx + unitCnt) > dst->pop)
    {
        AfxThrowError();
        return err;
    }
    // Safety: make sure copy won't exceed unit boundaries
    if ((srcOffset + size) > src->unitSiz || (dstOffset + size) > dst->unitSiz)
    {
        AfxThrowError();
        return err;
    }
    afxByte* dstPtr = (afxByte*)src->bytemap + dstUnitIdx * dst->unitSiz + dstOffset;
    afxByte const* srcPtr = (afxByte const*)src->bytemap + srcUnitIdx * src->unitSiz + srcOffset;

    for (afxUnit i = 0; i < unitCnt; ++i)
    {
        AfxMemcpy(dstPtr, srcPtr, size);
        dstPtr += dst->unitSiz;
        srcPtr += src->unitSiz;
    }
    return err;
}

_AFX afxBool AfxLookUpArray(afxArray* arr, void* data, afxUnit* elemIdx)
{
    afxError err = { 0 };
    AFX_ASSERT(arr);
    AFX_ASSERT(data);
    AFX_ASSERT(elemIdx);

    for (afxUnit i = 0; i < arr->pop; i++)
    {
        void* el = AfxGetArrayUnit(arr, i);

        if (0 == AfxMemcmp(el, data, arr->unitSiz))
        {
            AFX_ASSERT(elemIdx);
            *elemIdx = i;
            return TRUE;
        }
    }
    return FALSE;
}

_AFX afxError AfxPushArrayUnique(afxArray* arr, void* data, afxUnit* elemIdx)
{
    afxError err = { 0 };
    AFX_ASSERT(arr);
    AFX_ASSERT(data);
    AFX_ASSERT(elemIdx);

    if (!AfxLookUpArray(arr, data, elemIdx))
    {
        void* room = AfxPushArrayUnits(arr, 1, elemIdx, NIL, 0);

        if (!room) AfxThrowError();
        else
        {
            AfxCopy(room, data, arr->unitSiz);
        }
    }
    return err;
}
