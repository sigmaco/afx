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
 *                       (c) 2017 SIGMA, Engineering In Technology
 *                             <https://sigmaco.org/qwadro/>
 */

#include "qwadro/core/afxSystem.h"

_AFX afxNat AfxEnumeratePoolItems(afxPool const* pool, afxNat first, afxNat cnt, void *items[])
{
    afxError err = NIL;
    AfxAssertType(pool, afxFcc_POOL);
    AfxAssert(cnt);
    afxNat rslt = 0;
#if 0
    for (afxNat i = 0; i < cnt; i++)
    {
        afxNat pageIdx = (first + i) / pool->unitsPerPage;

        if (pageIdx >= pool->pageCnt) break;
        else
        {
            AfxAssert(pool->pages);
            afxPoolPage *pag = &pool->pages[pageIdx];

            if (pag->usage)
            {
                afxNat localIdx = i % /*32*/pool->unitsPerPage;
                afxNat32 mask = AFX_BIT_OFFSET(localIdx);
                void* unit = &pag->bytes[pool->unitSiz * localIdx];
                afxBool booked = pag->usage & mask;

                if (booked && items)
                    items[i] = unit;

                ++rslt;
            }
        }
    }
#endif
    afxNat posn = 0;

    for (afxNat pageIdx = 0; pageIdx < pool->pageCnt; pageIdx++)
    {
        afxPoolPage *pag = &pool->pages[pageIdx];

        if (pag->usedCnt)
        {
            for (afxNat unitIdx = 0; unitIdx < /*32*/pool->unitsPerPage; unitIdx++)
            {
                //afxNat32 mask = AFX_BIT_OFFSET(k);
                afxBool alloced = AfxTestBitEnabled(pag->usage, unitIdx); //pag->usage & mask;

                //AfxLogAdvertence("pool %p, upp %u, pageIdx %u, usedCnt %u, %u, alloced %u", pool, pool->unitsPerPage, pageIdx, pag->usedCnt, unitIdx, alloced);
                
                if (alloced)
                {
                    if (posn >= first)
                    {
                        void* unit = &pag->data[pool->unitSiz * unitIdx];
                        AfxAssert(items);
                        items[rslt] = unit;
                        ++rslt;
                    }
                    ++posn;
                }

                if (rslt == cnt)
                    break;
            }
        }

        if (rslt == cnt)
            break;
    }
    return rslt;
}

_AFX afxNat AfxEnumeratePoolUnits(afxPool const* pool, afxNat first, afxNat cnt, afxBool freeOnly, void *items[])
{
    afxError err = NIL;
    AfxAssertType(pool, afxFcc_POOL);
    AfxAssert(cnt);
    afxNat rslt = 0;

    for (afxNat i = 0; i < cnt; i++)
    {
        afxNat pageIdx = (first + i) / pool->unitsPerPage;

        if (pageIdx >= pool->pageCnt) break;
        else
        {
            AfxAssert(pool->pages);
            afxPoolPage *pag = &pool->pages[pageIdx];

            if (pag->data)
            {
                afxNat localIdx = (first + i) % /*32*/pool->unitsPerPage;
                //afxNat32 mask = AFX_BIT_OFFSET(localIdx);
                void* unit = &pag->data[pool->unitSiz * localIdx];

                if (items)
                    items[i] = unit;

                ++rslt;
            }
        }
    }
    return rslt;
}

_AFX afxNat AfxInvokePoolItems(afxPool const* pool, afxNat first, afxNat cnt, afxBool(*f)(void* item, void* udd), void *udd)
{
    afxError err = NIL;
    AfxAssertType(pool, afxFcc_POOL);
    AfxAssert(cnt);
    afxNat rslt = 0;
#if 0
    for (afxNat i = 0; i < cnt; i++)
    {
        afxNat pageIdx = (first + i) / pool->unitsPerPage;

        if (pageIdx >= pool->pageCnt) break;
        else
        {
            AfxAssert(pool->pages);
            afxPoolPage *pag = &pool->pages[pageIdx];

            if (pag->bytes)
            {
                afxNat localIdx = i % /*32*/pool->unitsPerPage;
                afxNat32 mask = AFX_BIT_OFFSET(localIdx);
                void* unit = &pag->bytes[pool->unitSiz * localIdx];
                afxBool booked = pag->usage & mask;

                ++rslt;

                if (booked && f(unit, udd)) // return non-zero to abort curation
                    break;
            }
        }
    }
#endif
    afxNat posn = 0;

    for (afxNat pageIdx = 0; pageIdx < pool->pageCnt; pageIdx++)
    {
        afxPoolPage *pag = &pool->pages[pageIdx];

        if (pag->usedCnt)
        {
            for (afxNat unitIdx = 0; unitIdx < /*32*/pool->unitsPerPage; unitIdx++)
            {
                //afxNat32 mask = AFX_BIT_OFFSET(k);
                afxBool alloced = AfxTestBitEnabled(pag->usage, unitIdx); //pag->usage & mask;

                if (alloced)
                {
                    if (posn >= first)
                    {
                        void* unit = &pag->data[pool->unitSiz * unitIdx];

                        ++rslt;

                        if (f(unit, udd)) // return non-zero to abort curation
                            break;
                    }
                    ++posn;
                }

                if (rslt == cnt)
                    break;
            }
        }

        if (rslt == cnt)
            break;
    }
    return rslt;
}

_AFX afxNat AfxInvokePoolUnits(afxPool const* pool, afxNat first, afxNat cnt, afxBool freeOnly, afxBool(*f)(void* item, void* udd), void *udd)
{
    afxError err = NIL;
    AfxAssertType(pool, afxFcc_POOL);
    AfxAssert(cnt);
    afxNat rslt = 0;

    for (afxNat i = 0; i < cnt; i++)
    {
        afxNat pageIdx = (first + i) / pool->unitsPerPage;

        if (pageIdx >= pool->pageCnt) break;
        else
        {
            AfxAssert(pool->pages);
            afxPoolPage *pag = &pool->pages[pageIdx];

            if (pag->data)
            {
                afxNat localIdx = i % /*32*/pool->unitsPerPage;
                //afxNat32 mask = AFX_BIT_OFFSET(localIdx);
                void* unit = &pag->data[pool->unitSiz * localIdx];

                ++rslt;

                if (f(unit, udd)) // return non-zero to abort curation
                    break;
            }
        }
    }
    return rslt;
}

_AFX afxBool AfxGetLinearPoolUnits(afxPool const* pool, afxNat first, afxNat cnt, void *ptr[])
{
    afxError err = NIL;
    AfxAssertType(pool, afxFcc_POOL);
    afxBool rslt = !!cnt;

    for (afxNat i = 0; i < cnt; i++)
    {
        if (!(AfxGetPoolUnit(pool, first + i, &ptr[i])))
        {
            rslt = FALSE;
            ptr[i] = NIL;
        }
    }
    return rslt;
}

_AFX afxBool AfxGetPoolUnits(afxPool const* pool, afxNat cnt, afxSize const idx[], void *ptr[])
{
    afxError err = NIL;
    AfxAssertType(pool, afxFcc_POOL);
    afxBool rslt = !!cnt;

    for (afxNat i = 0; i < cnt; i++)
    {
        if (!(AfxGetPoolUnit(pool, idx[i], &ptr[i])))
        {
            rslt = FALSE;
            ptr[i] = NIL;
        }
    }
    return rslt;
}

_AFX afxBool AfxGetPoolUnit(afxPool const* pool, afxSize idx, void **ptr)
{
    afxError err = NIL;
    AfxAssertType(pool, afxFcc_POOL);
    afxBool alloced = 0;

    afxSize pageIdx = idx / pool->unitsPerPage;

    if (pool->pageCnt > pageIdx)
    {
        AfxAssert(pool->pages);
        afxPoolPage *pag = &pool->pages[pageIdx];

        if (pag->data)
        {
            afxSize localIdx = idx % /*32*/pool->unitsPerPage;
            //afxNat32 mask = AFX_BIT_OFFSET(localIdx);
            alloced = AfxTestBitEnabled(pag->usage, localIdx); //pag->usage & mask;

            if (/*alloced && */ptr)
                *ptr = &pag->data[pool->unitSiz * localIdx];
            
        }
    }
    return alloced;
}

#if 0
_AFX afxBool AfxGetPoolItem(afxPool const* pool, afxSize idx, void **ptr)
{
    afxError err = NIL;
    AfxAssertType(pool, afxFcc_POOL);
    afxBool alloced = 0;

    afxSize pageIdx = idx / pool->unitsPerPage;

    if (pool->pageCnt > pageIdx)
    {
        AfxAssert(pool->pages);
        afxPoolPage *pag = &pool->pages[pageIdx];

        if (0 != pag->usage)
        {
            afxSize localIdx = idx % /*32*/pool->unitsPerPage;
            afxNat32 mask = AFX_BIT_OFFSET(localIdx);
            alloced = pag->usage & mask;

            if (alloced && ptr)
                *ptr = &pag->bytes[pool->unitSiz * localIdx];
        }
    }
    return alloced;
}
#endif

_AFX afxError AfxOccupyPoolUnit(afxPool* pool, afxSize idx, void *val)
{
    afxError err = NIL;
    AfxAssertType(pool, afxFcc_POOL);

    afxPoolPage *pag;
    afxSize pageIdx = idx / pool->unitsPerPage;

    if (0 == pool->pageCnt || (pageIdx >= pool->pageCnt))
    {
        afxSize diff = (pageIdx + 1) - pool->pageCnt;
        void *pgs;

        if (!(pgs = AfxReallocate(pool->pages, sizeof(pool->pages[0]), (pool->pageCnt + diff), 0, AfxHint()))) AfxThrowError();
        else
        {
            pool->pages = pgs;

            for (afxSize i = 0; i < diff; i++)
            {
                pag = &pool->pages[pool->pageCnt + i];
                pag->data = NIL;
                pag->usage = NIL;
                pag->usedCnt = 0;
            }
            pool->pageCnt += diff;
        }
    }

    if (!err)
    {
        AfxAssert(pool->pages);
        pag = &pool->pages[pageIdx];

        if (!pag->data)
        {
            if (!(pag->data = AfxAllocate(pool->unitsPerPage, pool->unitSiz, 0, AfxHint()))) AfxThrowError();
            else
                AfxZero2(pool->unitsPerPage, pool->unitSiz, pag->data);

            pag->usage = NIL;
        }

        if (!err)
        {
            afxSize localIdx = idx % /*32*/pool->unitsPerPage;
            //afxNat32 mask = AFX_BIT_OFFSET(localIdx);
            void* unit = &pag->data[pool->unitSiz * localIdx];
            afxBool alloced = AfxTestBitEnabled(pag->usage, localIdx); //pag->usage & mask;

            if (val)
                AfxCopy2(1, pool->unitSiz, val, unit);

            //if (!alloced)
            {
                pag->usage |= AFX_BIT_OFFSET(localIdx);
                ++pag->usedCnt;
                ++pool->totalUsedCnt;
            }

            AfxAssert(AfxGetPoolUnit(pool, idx, NIL));
        }
    }

    AfxAssert(AfxGetPoolUnit(pool, idx, NIL));
    return err;
}

_AFX void AfxDeallocatePoolUnits(afxPool* pool, afxNat cnt, void* units[])
{
    afxError err = NIL;
    AfxAssertType(pool, afxFcc_POOL);
    AfxAssert(units);

    for (afxNat i = cnt; i--> 0;)
    {
        AfxDeallocatePoolUnit(pool, units[i]);
        units[i] = NIL;
    }
    //return err;
}

_AFX afxBool AfxFindPoolUnitIndex(afxPool* pool, void* unit2, afxNat* idx, afxNat* localIdx)
{
    afxError err = NIL;
    AfxAssertType(pool, afxFcc_POOL);

    // 4 (x32) set  --- 128 bits per page
    // 2 // page

    afxByte* unit = unit2;
    afxNat unitSiz = pool->unitSiz;
    afxNat localIdx2;
    afxNat unitsPerPage = pool->unitsPerPage;
    afxSize pagSiz = unitsPerPage * unitSiz;
    afxNat idx2;
    afxBool found = FALSE;

    for (afxNat j = 0; j < pool->pageCnt; j++)
    {
        afxSize base = (afxSize)pool->pages[j].data;

        if (base && (pagSiz > (afxSize)(unit - base)))
        {
            afxSize a = (afxSize)unit / unitSiz;
            afxSize b = base / unitSiz;
            localIdx2 = a - b;
            idx2 = (j * unitsPerPage) + localIdx2;

            found = TRUE;
            break;
        }
    }
    
    if (idx)
        *idx = found ? idx2 : AFX_INVALID_INDEX;

    if (localIdx)
        *localIdx = found ? localIdx2 : AFX_INVALID_INDEX;

    return found;
}

_AFX void AfxDeallocatePoolUnit(afxPool* pool, void* unit2)
{
    afxError err = NIL;
    AfxAssertType(pool, afxFcc_POOL);

    // 4 (x32) set  --- 128 bits per page
    // 2 // page

    afxByte* unit = unit2;
    afxNat unitSiz = pool->unitSiz;
    afxNat unitsPerPage = pool->unitsPerPage;
    //afxSize pagSiz = unitsPerPage * unitSiz;
    afxNat localIdx, idx;
    afxBool found = AfxFindPoolUnitIndex(pool, unit, &idx, &localIdx);

    if (!found) AfxThrowError();
    else
    {
        afxSize pageIdx = idx / unitsPerPage;
        AfxAssert(pageIdx < pool->pageCnt);

        AfxAssert(pool->pages);
        afxPoolPage *pag = &pool->pages[pageIdx];

        if (pag->usedCnt)
        {
            AfxAssert(pag->data);
            AfxAssert(pag->usage);
            //afxSize localIdx = idx % /*32*/pool->unitsPerPage;
            //afxNat32 mask = AFX_BIT_OFFSET(localIdx);
            //void* unit = &pag->data[pool->unitSiz * localIdx];
            afxBool alloced = AfxTestBitEnabled(pag->usage, localIdx);// pag->usage & mask;

            if (alloced)
            {
                pag->usage &= ~AFX_BIT_OFFSET(localIdx);
                AfxZero2(1, unitSiz, unit);
                --pag->usedCnt;
                --pool->totalUsedCnt;
            }

            AfxAssert(!AfxGetPoolUnit(pool, idx, NIL));

            if (0 == pag->usedCnt)
            {
                AfxAssert(pag->usedCnt == 0);
                //AfxLogAdvertence("Deallocate page %p, data %p, usage %p", pag, pag->data, pag->usage);
                AfxDeallocate(pag->data);
                pag->usage = NIL;
                pag->data = NIL;

                if (pageIdx == pool->pageCnt - 1) // if is the last page
                {
                    if (pageIdx) // if exist other ones
                    {
                        void *pgs;

                        if (!(pgs = AfxReallocate(pool->pages, sizeof(*pag), (pool->pageCnt - 1), 0, AfxHint()))) AfxThrowError();
                        else
                        {
                            pool->pages = pgs;
                            --pool->pageCnt;
                        }
                    }
                    else
                    {
                        AfxDeallocate(pool->pages);
                        pool->pages = NIL;
                        --pool->pageCnt;
                    }
                }
            }
        }
        AfxAssert(!AfxGetPoolUnit(pool, idx, NIL));
    }
}

_AFX afxError AfxAllocatePoolUnits(afxPool* pool, afxNat cnt, void* units[])
{
    afxError err = NIL;
    AfxAssertType(pool, afxFcc_POOL);
    AfxAssert(units);

    for (afxNat i = 0; i < cnt; i++)
    {
        afxSize idx;

        if (!(units[i] = AfxAllocatePoolUnit(pool, &idx)))
        {
            AfxThrowError();

            AfxDeallocatePoolUnits(pool, i, units);
            break;
        }
    }
    return err;
}

_AFX void* AfxAllocatePoolUnit(afxPool* pool, afxSize* idx)
{
    afxError err = NIL;
    AfxAssertType(pool, afxFcc_POOL);
    //AfxAssert(unit);
    

    afxNat unitSiz = pool->unitSiz;
    afxNat unitsPerPage = pool->unitsPerPage;
    afxNat pagSiz = unitsPerPage * unitSiz;
    afxNat localIdx, idx2;
    afxBool found = FALSE;

    afxSize pageIdx;
    afxPoolPage *pag;
    
    for (pageIdx = 0; pageIdx < pool->pageCnt; pageIdx++)
    {
        pag = &pool->pages[pageIdx];

        if (pag->usedCnt != unitsPerPage)
        {
            if (pag->usedCnt == 0)
            {
                idx2 = (pageIdx * unitsPerPage);
                found = TRUE;
                break;
            }
            else
            {
                for (afxNat unitIdx = 0; unitIdx < unitsPerPage; unitIdx++)
                {
                    //afxNat32 mask = AFX_BIT_OFFSET(k);
                    afxBool alloced = AfxTestBitEnabled(pag->usage, unitIdx); // pag->usage & mask;

                    if (!(alloced))
                    {
                        idx2 = (pageIdx * unitsPerPage) + (localIdx = unitIdx);
                        found = TRUE;
                        break;
                    }
                }
            }
        }

        if (found)
            break;
    }

    if (!found)
    {
        // if no room, generate a new page.
        pageIdx = pool->pageCnt;
        idx2 = (pageIdx * unitsPerPage);
        found = TRUE;
    }

    if (found)
    {
        AfxAssert(!AfxGetPoolUnit(pool, idx2, NIL));

        if (AfxOccupyPoolUnit(pool, idx2, NIL)) AfxThrowError();
        else
        {
            AfxAssert(AfxGetPoolUnit(pool, idx2, NIL));
            *idx = idx2;
            AfxAssert(AfxGetPoolUnit(pool, idx2, NIL));
        }
    }
    AfxAssert(AfxGetPoolUnit(pool, idx2, NIL));
    void*ptr;
    AfxGetPoolUnit(pool, idx2, &ptr);
    return ptr;
}

_AFX void AfxSetUpPool(afxPool* pool, afxNat unitSiz, afxNat unitsPerPage)
{
    afxError err = NIL;
    AfxAssert(pool);
    AfxAssignTypeFcc(pool, afxFcc_POOL);
    pool->unitsPerPage = AfxMin(AfxMax(unitsPerPage,1), 32);
    pool->unitSiz = unitSiz;
    pool->totalUsedCnt = 0;
    //pool->dataOffset = 0;
    //pool->usageOffset = pool->unitsPerPage * pool->unitSiz;
    pool->pageCnt = 0;
    pool->pages = NIL;
    pool->mem = NIL;
}

_AFX void AfxCleanUpPool(afxPool* pool)
{
    afxError err = NIL;
    AfxAssertType(pool, afxFcc_POOL);
    
    for (afxNat i = 0; i < pool->pageCnt; i++)
    {
        if (pool->pages[i].data)
            AfxDeallocate(pool->pages[i].data);
    }
    
    if (pool->pages)
    {
        AfxDeallocate(pool->pages);
        pool->pages = NIL;
    }

    AfxZero2(1, sizeof(*pool), pool);
}

_AFX afxError AfxTestDictionarys(void)
{
    afxError err = NIL;


    afxPool pool;
    AfxSetUpPool(&pool, 4, 33);
    afxSize idx;

    for (afxNat i = 0; i < 3000; i++)
        AfxOccupyPoolUnit(&pool, i, &idx);

    //afxNat j = 3000;
    //for (afxNat i = 0; i < 3000; i++)
        //AfxDeallocatePoolUnit(&pool, --j);

    for (afxNat i = 0; i < 3000; i++)
        AfxAllocatePoolUnit(&pool, &idx);


    AfxCleanUpPool(&pool);

    return err;
}
