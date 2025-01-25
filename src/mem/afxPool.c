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

#include "../impl/afxExecImplKit.h"

_AFX afxUnit AfxEnumeratePoolItems(afxPool const* pool, afxUnit first, afxUnit cnt, void *items[])
{
    afxError err = NIL;
    //AfxAssertType(pool, afxFcc_POOL);
    AFX_ASSERT(cnt);
    afxUnit rslt = 0;
#if 0
    for (afxUnit i = 0; i < cnt; i++)
    {
        afxUnit pageIdx = (first + i) / pool->unitsPerPage;

        if (pageIdx >= pool->pageCnt) break;
        else
        {
            AFX_ASSERT(pool->pages);
            afxPoolPage *pag = &pool->pages[pageIdx];

            if (pag->usage)
            {
                afxUnit localIdx = i % /*32*/pool->unitsPerPage;
                afxUnit32 mask = AFX_BIT(localIdx);
                void* unit = &pag->bytes[pool->unitSiz * localIdx];
                afxBool booked = pag->usage & mask;

                if (booked && items)
                    items[i] = unit;

                ++rslt;
            }
        }
    }
#endif
    afxUnit posn = 0;

    for (afxUnit pageIdx = 0; pageIdx < pool->pageCnt; pageIdx++)
    {
        afxPoolPage *pag = &pool->pages[pageIdx];

        if (pag->usedCnt)
        {
            for (afxUnit unitIdx = 0; unitIdx < /*32*/pool->unitsPerPage; unitIdx++)
            {
                //afxUnit32 mask = AFX_BIT(k);
                afxBool alloced = AfxTestBitEnabled(pag->usage, unitIdx); //pag->usage & mask;

                //AfxLogAdvertence("pool %p, upp %u, pageIdx %u, usedCnt %u, %u, alloced %u", pool, pool->unitsPerPage, pageIdx, pag->usedCnt, unitIdx, alloced);
                
                if (alloced)
                {
                    if (posn >= first)
                    {
                        void* unit = &pag->data[pool->unitSiz * unitIdx];
                        AFX_ASSERT(items);
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

_AFX afxUnit AfxEnumeratePoolUnits(afxPool const* pool, afxUnit first, afxUnit cnt, afxBool freeOnly, void *items[])
{
    afxError err = NIL;
    //AfxAssertType(pool, afxFcc_POOL);
    AFX_ASSERT(cnt);
    afxUnit rslt = 0;

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxUnit pageIdx = (first + i) / pool->unitsPerPage;

        if (pageIdx >= pool->pageCnt) break;
        else
        {
            AFX_ASSERT(pool->pages);
            afxPoolPage *pag = &pool->pages[pageIdx];

            if (pag->data)
            {
                afxUnit localIdx = (first + i) % /*32*/pool->unitsPerPage;
                //afxUnit32 mask = AFX_BIT(localIdx);
                void* unit = &pag->data[pool->unitSiz * localIdx];

                if (items)
                    items[i] = unit;

                ++rslt;
            }
        }
    }
    return rslt;
}

_AFX afxUnit AfxInvokePoolItems(afxPool const* pool, afxUnit first, afxUnit cnt, afxBool(*f)(void* item, void* udd), void *udd)
{
    afxError err = NIL;
    //AfxAssertType(pool, afxFcc_POOL);
    AFX_ASSERT(cnt);
    afxUnit rslt = 0;
#if 0
    for (afxUnit i = 0; i < cnt; i++)
    {
        afxUnit pageIdx = (first + i) / pool->unitsPerPage;

        if (pageIdx >= pool->pageCnt) break;
        else
        {
            AFX_ASSERT(pool->pages);
            afxPoolPage *pag = &pool->pages[pageIdx];

            if (pag->bytes)
            {
                afxUnit localIdx = i % /*32*/pool->unitsPerPage;
                afxUnit32 mask = AFX_BIT(localIdx);
                void* unit = &pag->bytes[pool->unitSiz * localIdx];
                afxBool booked = pag->usage & mask;

                ++rslt;

                if (booked && f(unit, udd)) // return non-zero to abort curation
                    break;
            }
        }
    }
#endif
    afxUnit posn = 0;

    for (afxUnit pageIdx = 0; pageIdx < pool->pageCnt; pageIdx++)
    {
        afxPoolPage *pag = &pool->pages[pageIdx];

        if (pag->usedCnt)
        {
            for (afxUnit unitIdx = 0; unitIdx < /*32*/pool->unitsPerPage; unitIdx++)
            {
                //afxUnit32 mask = AFX_BIT(k);
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

_AFX afxUnit AfxInvokePoolUnits(afxPool const* pool, afxUnit first, afxUnit cnt, afxBool freeOnly, afxBool(*f)(void* item, void* udd), void *udd)
{
    afxError err = NIL;
    //AfxAssertType(pool, afxFcc_POOL);
    AFX_ASSERT(cnt);
    afxUnit rslt = 0;

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxUnit pageIdx = (first + i) / pool->unitsPerPage;

        if (pageIdx >= pool->pageCnt) break;
        else
        {
            AFX_ASSERT(pool->pages);
            afxPoolPage *pag = &pool->pages[pageIdx];

            if (pag->data)
            {
                afxUnit localIdx = i % /*32*/pool->unitsPerPage;
                //afxUnit32 mask = AFX_BIT(localIdx);
                void* unit = &pag->data[pool->unitSiz * localIdx];

                ++rslt;

                if (f(unit, udd)) // return non-zero to abort curation
                    break;
            }
        }
    }
    return rslt;
}

_AFX afxBool AfxGetLinearPoolUnits(afxPool const* pool, afxUnit first, afxUnit cnt, void *ptr[])
{
    afxError err = NIL;
    //AfxAssertType(pool, afxFcc_POOL);
    afxBool rslt = !!cnt;

    for (afxUnit i = 0; i < cnt; i++)
    {
        if (!(AfxGetPoolUnit(pool, first + i, &ptr[i])))
        {
            rslt = FALSE;
            ptr[i] = NIL;
        }
    }
    return rslt;
}

_AFX afxBool AfxGetPoolUnits(afxPool const* pool, afxUnit cnt, afxSize const idx[], void *ptr[])
{
    afxError err = NIL;
    //AfxAssertType(pool, afxFcc_POOL);
    afxBool rslt = !!cnt;

    for (afxUnit i = 0; i < cnt; i++)
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
    //AfxAssertType(pool, afxFcc_POOL);
    afxBool alloced = 0;

    afxSize pageIdx = idx / pool->unitsPerPage;

    if (pool->pageCnt > pageIdx)
    {
        AFX_ASSERT(pool->pages);
        afxPoolPage *pag = &pool->pages[pageIdx];

        if (pag->data)
        {
            afxSize localIdx = idx % /*32*/pool->unitsPerPage;
            //afxUnit32 mask = AFX_BIT(localIdx);
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
        AFX_ASSERT(pool->pages);
        afxPoolPage *pag = &pool->pages[pageIdx];

        if (0 != pag->usage)
        {
            afxSize localIdx = idx % /*32*/pool->unitsPerPage;
            afxUnit32 mask = AFX_BIT(localIdx);
            alloced = pag->usage & mask;

            if (alloced && ptr)
                *ptr = &pag->bytes[pool->unitSiz * localIdx];
        }
    }
    return alloced;
}
#endif

_AFX afxError AfxTakePoolUnit(afxPool* pool, afxSize idx, void *val)
{
    afxError err = NIL;
    //AfxAssertType(pool, afxFcc_POOL);

    afxPoolPage *pag;
    afxSize pageIdx = idx / pool->unitsPerPage;

    if (0 == pool->pageCnt || (pageIdx >= pool->pageCnt))
    {
        afxSize diff = (pageIdx + 1) - pool->pageCnt;

        if (AfxReallocate(sizeof(pool->pages[0]) * (pool->pageCnt + diff), pool->memAlign, AfxHere(), (void**)&pool->pages)) AfxThrowError();
        else
        {
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
        AFX_ASSERT(pool->pages);
        pag = &pool->pages[pageIdx];

        if (!pag->data)
        {
            if (AfxAllocate(pool->unitsPerPage * pool->unitSiz, pool->memAlign, AfxHere(), (void**)&pag->data)) AfxThrowError();
            else
                AfxZero2(pag->data, pool->unitSiz, pool->unitsPerPage);

            pag->usage = NIL;
        }

        if (!err)
        {
            afxSize localIdx = idx % /*32*/pool->unitsPerPage;
            //afxUnit32 mask = AFX_BIT(localIdx);
            void* unit = &pag->data[pool->unitSiz * localIdx];
            afxBool alloced = AfxTestBitEnabled(pag->usage, localIdx); //pag->usage & mask;

            if (alloced)
                AfxThrowError();

            if (val)
                AfxCopy(unit, val, pool->unitSiz);

            //if (!alloced)
            {
                pag->usage |= AFX_BIT(localIdx);
                ++pag->usedCnt;
                ++pool->totalUsedCnt;
            }

            AFX_ASSERT(AfxGetPoolUnit(pool, idx, NIL));
        }
    }

    AFX_ASSERT(AfxGetPoolUnit(pool, idx, NIL));
    return err;
}

_AFX afxError AfxFreePoolUnit(afxPool* pool, afxSize idx)
{
    afxError err = NIL;
    //AfxAssertType(pool, afxFcc_POOL);

    afxPoolPage *pag;
    afxSize pageIdx = idx / pool->unitsPerPage;

    if (0 == pool->pageCnt || (pageIdx >= pool->pageCnt))
        return err; // not paged/found

    AFX_ASSERT(pool->pages);
    pag = &pool->pages[pageIdx];

    if (!pag->data)
        return err; // not allocated

    afxSize localIdx = idx % /*32*/pool->unitsPerPage;
    //afxUnit32 mask = AFX_BIT(localIdx);
    void* unit = &pag->data[pool->unitSiz * localIdx];
    afxBool alloced = AfxTestBitEnabled(pag->usage, localIdx); //pag->usage & mask;

    if (alloced)
    {
        pag->usage |= ~AFX_BIT(localIdx);
        --pag->usedCnt;
        --pool->totalUsedCnt;
    }

    AFX_ASSERT(!AfxGetPoolUnit(pool, idx, NIL));
    return err;
}

_AFX void AfxPopPoolUnits(afxPool* pool, afxUnit cnt, void* units[])
{
    afxError err = NIL;
    //AfxAssertType(pool, afxFcc_POOL);
    AFX_ASSERT(units);

    for (afxUnit i = cnt; i--> 0;)
    {
        AfxPopPoolUnit(pool, units[i]);
        units[i] = NIL;
    }
    //return err;
}

_AFX afxBool AfxFindPoolUnit(afxPool* pool, void* unit2, afxUnit* idx, afxUnit* localIdx)
{
    afxError err = NIL;
    //AfxAssertType(pool, afxFcc_POOL);

    // 4 (x32) set  --- 128 bits per page
    // 2 // page

    afxByte* unit = unit2;
    afxUnit unitSiz = pool->unitSiz;
    afxUnit localIdx2;
    afxUnit unitsPerPage = pool->unitsPerPage;
    afxSize pagSiz = unitsPerPage * unitSiz;
    afxUnit idx2;
    afxBool found = FALSE;

    for (afxUnit j = 0; j < pool->pageCnt; j++)
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

_AFX void AfxPopPoolUnit(afxPool* pool, void* unit2)
{
    afxError err = NIL;
    //AfxAssertType(pool, afxFcc_POOL);

    // 4 (x32) set  --- 128 bits per page
    // 2 // page

    afxByte* unit = unit2;
    afxUnit unitSiz = pool->unitSiz;
    afxUnit unitsPerPage = pool->unitsPerPage;
    //afxSize pagSiz = unitsPerPage * unitSiz;
    afxUnit localIdx, idx;
    afxBool found = AfxFindPoolUnit(pool, unit, &idx, &localIdx);

    if (!found) AfxThrowError();
    else
    {
        afxSize pageIdx = idx / unitsPerPage;
        AFX_ASSERT(pageIdx < pool->pageCnt);

        AFX_ASSERT(pool->pages);
        afxPoolPage *pag = &pool->pages[pageIdx];

        if (pag->usedCnt)
        {
            AFX_ASSERT(pag->data);
            AFX_ASSERT(pag->usage);
            //afxSize localIdx = idx % /*32*/pool->unitsPerPage;
            //afxUnit32 mask = AFX_BIT(localIdx);
            //void* unit = &pag->data[pool->unitSiz * localIdx];
            afxBool alloced = AfxTestBitEnabled(pag->usage, localIdx);// pag->usage & mask;

            if (alloced)
            {
                pag->usage &= ~AFX_BIT(localIdx);
                AfxZero(unit, unitSiz);
                --pag->usedCnt;
                --pool->totalUsedCnt;
            }

            AFX_ASSERT(!AfxGetPoolUnit(pool, idx, NIL));

            if (0 == pag->usedCnt)
            {
                AFX_ASSERT(pag->usedCnt == 0);
                //AfxLogAdvertence("Deallocate page %p, data %p, usage %p", pag, pag->data, pag->usage);
                AfxDeallocate((void**)&pag->data, AfxHere());
                pag->usage = NIL;
                pag->data = NIL;

                if (pageIdx == pool->pageCnt - 1) // if is the last page
                {
                    if (pageIdx) // if exist other ones
                    {
                        if (AfxReallocate(sizeof(pool->pages[0]) * (pool->pageCnt - 1), pool->memAlign, AfxHere(), (void**)&pool->pages)) AfxThrowError();
                        else
                        {
                            --pool->pageCnt;
                        }
                    }
                    else
                    {
                        AfxDeallocate((void**)&pool->pages, AfxHere());
                        --pool->pageCnt;
                    }
                }
            }
        }
        AFX_ASSERT(!AfxGetPoolUnit(pool, idx, NIL));
    }
}

_AFX afxError AfxPushPoolUnits(afxPool* pool, afxUnit cnt, void* units[])
{
    afxError err = NIL;
    //AfxAssertType(pool, afxFcc_POOL);
    AFX_ASSERT(units);

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxSize idx;

        if (!(units[i] = AfxPushPoolUnit(pool, &idx)))
        {
            AfxThrowError();

            AfxPopPoolUnits(pool, i, units);
            break;
        }
    }
    return err;
}

_AFX void* AfxPushPoolUnit(afxPool* pool, afxSize* idx)
{
    afxError err = NIL;
    //AfxAssertType(pool, afxFcc_POOL);
    //AFX_ASSERT(unit);
    

    afxUnit unitSiz = pool->unitSiz;
    afxUnit unitsPerPage = pool->unitsPerPage;
    afxUnit pagSiz = unitsPerPage * unitSiz;
    afxUnit localIdx, idx2;
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
                for (afxUnit unitIdx = 0; unitIdx < unitsPerPage; unitIdx++)
                {
                    //afxUnit32 mask = AFX_BIT(k);
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
        AFX_ASSERT(!AfxGetPoolUnit(pool, idx2, NIL));

        if (AfxTakePoolUnit(pool, idx2, NIL)) AfxThrowError();
        else
        {
            AFX_ASSERT(AfxGetPoolUnit(pool, idx2, NIL));
            *idx = idx2;
            AFX_ASSERT(AfxGetPoolUnit(pool, idx2, NIL));
        }
    }
    AFX_ASSERT(AfxGetPoolUnit(pool, idx2, NIL));
    void*ptr;
    AfxGetPoolUnit(pool, idx2, &ptr);
    AFX_ASSERT(AFX_IS_ALIGNED(ptr, AFX_POOL_ALIGNMENT));
    return ptr;
}

_AFX void AfxDeployPool(afxPool* pool, afxUnit unitSiz, afxUnit unitsPerPage, afxUnit memAlign)
{
    afxError err = NIL;
    AFX_ASSERT(pool);
    //AfxAssignTypeFcc(pool, afxFcc_POOL);
    pool->unitsPerPage = AfxMin(AfxMax(unitsPerPage,1), 32);
    pool->memAlign = AfxMax(AFX_POOL_ALIGNMENT, AFX_ALIGNED_SIZE(memAlign, AFX_PTR_ALIGNMENT));
    AFX_ASSERT(AFX_IS_ALIGNED(pool->memAlign, AFX_POOL_ALIGNMENT));
    pool->unitSiz = unitSiz;
    pool->totalUsedCnt = 0;
    //pool->dataOffset = 0;
    //pool->usageOffset = pool->unitsPerPage * pool->unitSiz;
    pool->pageCnt = 0;
    pool->pages = NIL;
    pool->mem = NIL;
}

_AFX void AfxDismantlePool(afxPool* pool)
{
    afxError err = NIL;
    //AfxAssertType(pool, afxFcc_POOL);
    
    for (afxUnit i = 0; i < pool->pageCnt; i++)
    {
        if (pool->pages[i].data)
            AfxDeallocate((void**)&pool->pages[i].data, AfxHere());
    }
    
    if (pool->pages)
    {
        AfxDeallocate((void**)&pool->pages, AfxHere());
        pool->pages = NIL;
    }

    AfxZero(pool, sizeof(*pool));
}

_AFX afxError AfxTestDictionarys(void)
{
    afxError err = NIL;


    afxPool pool;
    AfxDeployPool(&pool, 4, 33, 0);
    afxSize idx;

    for (afxUnit i = 0; i < 3000; i++)
        AfxTakePoolUnit(&pool, i, &idx);

    //afxUnit j = 3000;
    //for (afxUnit i = 0; i < 3000; i++)
        //AfxPopPoolUnit(&pool, --j);

    for (afxUnit i = 0; i < 3000; i++)
        AfxPushPoolUnit(&pool, &idx);


    AfxDismantlePool(&pool);

    return err;
}
