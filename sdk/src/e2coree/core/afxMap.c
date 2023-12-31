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
 *                   (c) 2017 SIGMA Technology Group — Federação SIGMA
 *                                    www.sigmaco.org
 */

#include "qwadro/core/afxMap.h"

_AFX afxBool AfxLocateMapUnit(afxMap* map, void* key, afxNat* idx)
{
    afxError err = NIL;
    AfxAssertType(map, afxFcc_MAP);
    AfxAssert(key);

    for (afxNat i = 0; i < map->pageCnt; i++)
    {
        afxByte *pag = map->pages[i];

        if (map->usage.pages[i].markedBitCnt)
        {
            for (afxNat j = 0; j < map->unitsPerPage; j++)
            {
                if (0 == map->f(key, &pag[j * map->keyUnitSiz]))
                {
                    if (idx)
                        *idx = j;

                    return TRUE;
                }
            }
        }
    }

    if (idx)
        *idx = AFX_INVALID_INDEX;

    return FALSE;
}

_AFX afxError AfxSetMapAt(afxMap* map, afxNat idx, void* key, void* val)
{
    afxError err = NIL;
    AfxAssertType(map, afxFcc_MAP);

    // 4 (x32) set  --- 128 bits per page
    // 2 // page

    afxByte *pag;
    afxNat pageIdx = idx / map->unitsPerPage;

    //2
    //3

    if (0 == map->pageCnt || (map->pageCnt - 1 < pageIdx))
    {
        afxNat qty = (pageIdx + 1) - map->pageCnt;
        void *pgs;

        if (!(pgs = AfxReallocate(map->mem, map->pages, (map->pageCnt + qty) * sizeof(map->pages[0]), 0, AfxHint()))) AfxThrowError();
        else
        {
            map->pages = pgs;

            for (afxNat i = 0; i < qty; i++)
                map->pages[map->pageCnt + i] = NIL;

            map->pageCnt += qty;
        }
    }

    if (!err)
    {
        AfxAssert(map->pages);
        pag = map->pages[pageIdx];

        if (!pag)
        {
            if (!(pag = AfxAllocate(map->mem, map->keyUnitSiz + map->valUnitSiz, map->unitsPerPage, 0, AfxHint()))) AfxThrowError();
            else
            {
                AfxZero(pag, (map->keyUnitSiz + map->valUnitSiz) * map->unitsPerPage);
                map->pages[pageIdx] = pag;
            }
        }

        if (!err)
        {
            if (AfxSetBit(&map->usage, idx)) AfxThrowError();
            else
            {
                afxNat locidx = idx % map->unitsPerPage;

                if (key)
                    AfxCopy(1, map->keyUnitSiz, key, &pag[map->valUnitSiz * locidx]);

                if (val)
                    AfxCopy(1, map->valUnitSiz, val, &pag[(map->keyUnitSiz * map->unitsPerPage) + (map->valUnitSiz * locidx)]);
            }
        }
    }
    return err;
}

_AFX afxError AfxInsertMapKey(afxMap* map, void* key, void* val, afxNat* idx)
{
    afxError err = NIL;
    AfxAssertType(map, afxFcc_MAP);

    if (map->usage.totalMarkedBitCnt == 0)
    {
        if (AfxSetMapAt(map, 0, key, val)) AfxThrowError();
        else
        {
            AfxAssert(map->usage.totalMarkedBitCnt == 1);
            AfxAssert(map->usage.pages[0].markedBitCnt == 1);

            if (idx)
                *idx = 0;
        }
    }
    else
    //if (bmp->pageCnt)
    {
        for (afxNat i = 0; i < map->pageCnt; i++)
        {
            afxBitmapPage *pag = &map->usage.pages[i];

            if (pag->markedBitCnt == 0)
            {
                afxNat idx2 = map->usage.totalMarkedBitCnt;

                if (AfxSetMapAt(map, idx2, key, val)) AfxThrowError();
                else
                {
                    AfxAssert(pag->markedBitCnt == 1);

                    if (idx)
                        *idx = idx2;
                }
            }
            else
            {
                if (map->unitsPerPage > pag->markedBitCnt)
                {
                    afxNat idx2 = pag->markedBitCnt;

                    if (AfxSetMapAt(map, idx2, key, val)) AfxThrowError();
                    else
                    {
                        AfxAssert(map->usage.totalMarkedBitCnt);
                        AfxAssert(pag->markedBitCnt);
                            
                        if (idx)
                            *idx = idx2;

                        return err;
                    }
                }
            }
        }
    }
    return err;
}

_AFX afxError AfxDeleteMapKey(afxMap* map, void* key)
{
    afxError err = NIL;
    AfxAssertType(map, afxFcc_MAP);
    AfxAssert(key);

    afxNat idx = 0;

    if (AfxLocateMapUnit(map, key, &idx))
    {
        AfxClearBit(&map->usage, idx);
        afxNat pageIdx = idx / (map->unitsPerPage);

        if (0 == map->usage.pages[pageIdx].markedBitCnt)
        {
            AfxDeallocate(map->mem, map->pages[pageIdx]);
            map->pages[pageIdx] = NIL;

            if (map->pageCnt == map->pageCnt - 1) // if is the last page
            {
                if (pageIdx) // if exist other ones
                {
                    void *pgs;

                    if (!(pgs = AfxReallocate(map->mem, map->pages, (map->pageCnt - 1) * sizeof(map->pages[0]), 0, AfxHint()))) AfxThrowError();
                    else
                    {
                        map->pages = pgs;
                        --map->pageCnt;
                    }
                }
                else
                {
                    AfxDeallocate(map->mem, map->pages);
                    map->pages = NIL;
                    --map->pageCnt;
                }
            }
        }
    }
    return err;
}

_AFX afxResult _AfxMapCompareByteFn(void* a, void* b)
{
    return (afxNat8)a - (afxNat8)b;
}

_AFX afxResult _AfxMapCompareShortFn(void* a, void* b)
{
    return (afxNat16)a - (afxNat16)b;
}

_AFX afxResult _AfxMapCompareWordFn(void* a, void* b)
{
    return (afxNat32)a - (afxNat32)b;
}

_AFX afxResult _AfxMapCompareLongFn(void* a, void* b)
{
    return (afxNat64)a - (afxNat64)b;
}

_AFX void AfxAcquireMap(afxMap* map, afxNat keyUnitSiz, afxNat valUnitSiz, afxNat unitsPerPage, afxResult (*f)(void* a, void* b))
{
    afxError err = NIL;
    AfxAssert(map);
    AfxAssignTypeFcc(map, afxFcc_MAP);
    AfxAcquireBitmap(&map->usage, unitsPerPage * 32);
    map->unitsPerPage = AfxMax(unitsPerPage, 1);
    map->keyUnitSiz = keyUnitSiz;
    map->valUnitSiz = valUnitSiz;
    map->pageCnt = 0;
    map->pages = NIL;
    map->mem = NIL;

    if (keyUnitSiz == 8)
        map->f = _AfxMapCompareLongFn;
    else if (keyUnitSiz == 4)
        map->f = _AfxMapCompareWordFn;
    else if (keyUnitSiz == 2)
        map->f = _AfxMapCompareShortFn;
    else if (keyUnitSiz == 1)
        map->f = _AfxMapCompareByteFn;
    else
        map->f = f;
}

_AFX void AfxReleaseMap(afxMap* map)
{
    afxError err = NIL;
    AfxAssertType(map, afxFcc_MAP);

    for (afxNat i = 0; i < map->pageCnt; i++)
    {
        if (map->pages[i])
            AfxDeallocate(map->mem, map->pages[i]);
    }

    if (map->pages)
    {
        AfxDeallocate(map->mem, map->pages);
        map->pages = NIL;
    }

    AFX_ZERO(map);
}

_AFX afxError AfxTestMaps(void)
{
    afxError err = NIL;

    afxNat keys[] = { 1 , 2, 4, 8, 16};
    afxNat64 values[] = { 16, 8, 4, 2, 1 };

    afxMap map;
    AfxAcquireMap(&map, 4, 8, 10, NIL);

    afxNat idx;
    AfxInsertMapKey(&map, keys[0], values[0], idx);
    

    AfxReleaseMap(&map);
    return err;
}
