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

#define _AFX_ENFORCE_CLEAN_POOL_ALLOCATION (FALSE)

_AFX afxUnit AfxEnumeratePoolItems(afxPool const* pool, afxUnit first, afxUnit cnt, void *items[])
{
    afxError err = NIL;
    AFX_ASSERT(pool);
#ifdef _AFX_POOL_VALIDATION_ENABLED
    AFX_ASSERT(pool->fcc == afxFcc_POOL);
#endif
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
                afxUnit localIdx = i % /*AFX_MAX_UNITS_PER_POOL_PAGE*/pool->unitsPerPage;
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
            for (afxUnit unitIdx = 0; unitIdx < /*AFX_MAX_UNITS_PER_POOL_PAGE*/pool->unitsPerPage; unitIdx++)
            {
                //afxUnit32 mask = AFX_BIT(k);
                afxBool alloced = AfxTestBitEnabled(pag->usage, unitIdx); //pag->usage & mask;

                //AfxReportWarn("pool %p, upp %u, pageIdx %u, usedCnt %u, %u, alloced %u", pool, pool->unitsPerPage, pageIdx, pag->usedCnt, unitIdx, alloced);
                
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

_AFX afxUnit AfxInvokePoolItems(afxPool const* pool, afxUnit first, afxUnit cnt, afxBool(*f)(void* item, void* udd), void *udd)
{
    afxError err = NIL;
    AFX_ASSERT(pool);
#ifdef _AFX_POOL_VALIDATION_ENABLED
    AFX_ASSERT(pool->fcc == afxFcc_POOL);
#endif
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
                afxUnit localIdx = i % /*AFX_MAX_UNITS_PER_POOL_PAGE*/pool->unitsPerPage;
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
            for (afxUnit unitIdx = 0; unitIdx < /*AFX_MAX_UNITS_PER_POOL_PAGE*/pool->unitsPerPage; unitIdx++)
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

_AFX afxBool AfxGetLinearPoolUnits(afxPool const* pool, afxUnit first, afxUnit cnt, void *ptr[])
{
    afxError err = NIL;
    AFX_ASSERT(pool);
#ifdef _AFX_POOL_VALIDATION_ENABLED
    AFX_ASSERT(pool->fcc == afxFcc_POOL);
#endif
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
    AFX_ASSERT(pool);
#ifdef _AFX_POOL_VALIDATION_ENABLED
    AFX_ASSERT(pool->fcc == afxFcc_POOL);
#endif
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
    AFX_ASSERT(pool);
#ifdef _AFX_POOL_VALIDATION_ENABLED
    AFX_ASSERT(pool->fcc == afxFcc_POOL);
#endif
    afxBool alloced = 0;

    afxSize pageIdx = idx / pool->unitsPerPage;

    if (pool->pageCnt > pageIdx)
    {
        AFX_ASSERT(pool->pages);
        afxPoolPage *pag = &pool->pages[pageIdx];

        if (pag->data)
        {
            afxSize localIdx = idx % /*AFX_MAX_UNITS_PER_POOL_PAGE*/pool->unitsPerPage;
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
            afxSize localIdx = idx % /*AFX_MAX_UNITS_PER_POOL_PAGE*/pool->unitsPerPage;
            afxUnit32 mask = AFX_BIT(localIdx);
            alloced = pag->usage & mask;

            if (alloced && ptr)
                *ptr = &pag->bytes[pool->unitSiz * localIdx];
        }
    }
    return alloced;
}
#endif

_AFX afxError AfxInflatePool(afxPool* pool, afxUnit pagCnt)
{
    afxError err = NIL;
    AFX_ASSERT(pool);
#ifdef _AFX_POOL_VALIDATION_ENABLED
    AFX_ASSERT(pool->fcc == afxFcc_POOL);
#endif

    if (0 == pool->pageCnt || (pagCnt >= pool->pageCnt))
    {
        afxSize diff = pagCnt - pool->pageCnt;

        if (AfxReallocate(sizeof(pool->pages[0]) * (pool->pageCnt + diff), pool->memAlign, AfxHere(), (void**)&pool->pages))
        {
            AfxThrowError();
            return err;
        }
        else
        {
            for (afxSize i = 0; i < diff; i++)
            {
                afxPoolPage *pag = &pool->pages[pool->pageCnt + i];

#ifdef _AFX_POOL_VALIDATION_ENABLED
                AFX_ASSERT(pag->fcc = afxFcc_POOL);
#endif
                AfxPushLink(&pag->link, &pool->freePags);
                pag->data = NIL;
                pag->usage = NIL;
                pag->usedCnt = 0;
            }
            pool->pageCnt += diff;
        }
    }
    return err;
}

_AFX afxError AfxTakePoolUnit(afxPool* pool, afxSize idx, void *val)
{
    afxError err = NIL;
    AFX_ASSERT(pool);
#ifdef _AFX_POOL_VALIDATION_ENABLED
    AFX_ASSERT(pool->fcc == afxFcc_POOL);
#endif

    afxPoolPage *pag;
    afxSize pageIdx = idx / pool->unitsPerPage;

    if (0 == pool->pageCnt || (pageIdx >= pool->pageCnt))
    {
        if (AfxInflatePool(pool, (pageIdx + 1)))
        {
            AfxThrowError();
        }
    }

    if (!err)
    {
        AFX_ASSERT(pool->pages);
        pag = &pool->pages[pageIdx];

#ifdef _AFX_POOL_VALIDATION_ENABLED
        AFX_ASSERT(pag->fcc == afxFcc_POOL);
#endif

        if (!pag->data)
        {
            AFX_ASSERT(pag->usage == 0);
            AFX_ASSERT(pag->usedCnt == 0);

            if (AfxAllocate(pool->unitsPerPage * pool->unitSiz, pool->memAlign, AfxHere(), (void**)&pag->data))
            {
                AfxThrowError();
            }
            else
            {
#ifdef _AFX_ENFORCE_CLEAN_POOL_ALLOCATION
                // TODO: Do not zero memory.
                AfxZero(pag->data, pool->unitSiz * pool->unitsPerPage);
#endif
            }
        }

        if (!err)
        {
            afxSize localIdx = idx % /*AFX_MAX_UNITS_PER_POOL_PAGE*/pool->unitsPerPage;
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

                if (pag->usedCnt == pool->unitsPerPage)
                {
                    // removed from free pages if full.
                    AfxPopLink(&pag->link);
                }
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
    AFX_ASSERT(pool);
#ifdef _AFX_POOL_VALIDATION_ENABLED
    AFX_ASSERT(pool->fcc == afxFcc_POOL);
#endif

    afxPoolPage *pag;
    afxSize pageIdx = idx / pool->unitsPerPage;

    if (0 == pool->pageCnt || (pageIdx >= pool->pageCnt))
        return err; // not paged/found

    AFX_ASSERT(pool->pages);
    pag = &pool->pages[pageIdx];

    if (!pag->data)
    {
        AFX_ASSERT(pag->usage == 0);
        AFX_ASSERT(pag->usedCnt == 0);
        return err; // not allocated
    }
    afxSize localIdx = idx % /*AFX_MAX_UNITS_PER_POOL_PAGE*/pool->unitsPerPage;
    //afxUnit32 mask = AFX_BIT(localIdx);
    void* unit = &pag->data[pool->unitSiz * localIdx];
    afxBool alloced = AfxTestBitEnabled(pag->usage, localIdx); //pag->usage & mask;

    if (alloced)
    {
        if (pag->usedCnt == pool->unitsPerPage)
        {
            // insert in free pages if not full again.
            AfxPushLink(&pag->link, &pool->freePags);
        }
        pag->usage |= ~AFX_BIT(localIdx);
        --pag->usedCnt;
        --pool->totalUsedCnt;
    }

    AFX_ASSERT(!AfxGetPoolUnit(pool, idx, NIL));
    return err;
}

_AFX afxBool AfxFindPoolUnit(afxPool* pool, void* unit2, afxUnit* idx, afxUnit* localIdx)
{
    afxError err = NIL;
    AFX_ASSERT(pool);
#ifdef _AFX_POOL_VALIDATION_ENABLED
    AFX_ASSERT(pool->fcc == afxFcc_POOL);
#endif

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

_AFX afxError DeletePoolPage(afxPool* pool, afxUnit pagIdx)
{
    afxError err = NIL;
    AFX_ASSERT(pool);
#ifdef _AFX_POOL_VALIDATION_ENABLED
    AFX_ASSERT(pool->fcc == afxFcc_POOL);
#endif

    if (pagIdx >= pool->pageCnt)
    {
        AFX_ASSERT_RANGE(pool->pageCnt, pagIdx, 1);
        AfxThrowError();
        return err;
    }

    afxPoolPage* pag = &pool->pages[pagIdx];

#ifdef _AFX_POOL_VALIDATION_ENABLED
    AFX_ASSERT(pag->fcc == afxFcc_POOL);
#endif

    if (0 == pag->usedCnt)
    {
        AFX_ASSERT(pag->usage == 0);
        //AfxReportWarn("Deallocate page %p, data %p, usage %p", pag, pag->data, pag->usage);
        AfxDeallocate((void**)&pag->data, AfxHere());
        pag->usage = NIL;
        pag->data = NIL;
        AfxPopLink(&pag->link);

        if (pagIdx == pool->pageCnt - 1) // if is the last page
        {
            if (pagIdx) // if exist other ones
            {
                if (AfxReallocate(sizeof(pool->pages[0]) * (pool->pageCnt - 1), pool->memAlign, AfxHere(), (void**)&pool->pages))
                {
                    AfxThrowError();
                }
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
    return err;
}

_AFX afxError AfxReclaimPoolUnits(afxPool* pool, afxUnit cnt, void* units[])
{
    afxError err = NIL;
    AFX_ASSERT(pool);
#ifdef _AFX_POOL_VALIDATION_ENABLED
    AFX_ASSERT(pool->fcc == afxFcc_POOL);
#endif
    AFX_ASSERT(units);

    // 4 (x32) set  --- 128 bits per page
    // 2 // page

    for (afxUnit objIdx = 0; objIdx < cnt; objIdx++)
    {
        afxByte* unit = units[objIdx];
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
#ifdef _AFX_POOL_VALIDATION_ENABLED
            AFX_ASSERT(pag->fcc == afxFcc_POOL);
#endif

            if (pag->usedCnt)
            {
                AFX_ASSERT(pag->data);
                AFX_ASSERT(pag->usage);
                //afxSize localIdx = idx % /*AFX_MAX_UNITS_PER_POOL_PAGE*/pool->unitsPerPage;
                //afxUnit32 mask = AFX_BIT(localIdx);
                //void* unit = &pag->data[pool->unitSiz * localIdx];
                afxBool alloced = AfxTestBitEnabled(pag->usage, localIdx);// pag->usage & mask;

                if (alloced)
                {
                    if (pag->usedCnt == pool->unitsPerPage)
                    {
                        // insert in free pages if not full again.
                        AfxPushLink(&pag->link, &pool->freePags);
                    }
                    pag->usage &= ~AFX_BIT(localIdx);
#ifdef _AFX_ENFORCE_CLEAN_POOL_ALLOCATION
                    AfxZero(unit, unitSiz);
#endif
                    --pag->usedCnt;
                    --pool->totalUsedCnt;
                }

                AFX_ASSERT(!AfxGetPoolUnit(pool, idx, NIL));

                if (0 == pag->usedCnt)
                {
                    DeletePoolPage(pool, pageIdx);
                }
            }
            AFX_ASSERT(!AfxGetPoolUnit(pool, idx, NIL));
        }
    }
    return err;
}

_AFX afxError AfxRequestPoolUnits(afxPool* pool, afxUnit cnt, afxUnit ids[], void* units[])
{
    afxError err = NIL;
    AFX_ASSERT(pool);
#ifdef _AFX_POOL_VALIDATION_ENABLED
    AFX_ASSERT(pool->fcc == afxFcc_POOL);
#endif
    AFX_ASSERT(units);

    for (afxUnit reqIdx = 0; reqIdx < cnt; reqIdx++)
    {
        afxUnit unitSiz = pool->unitSiz;
        afxUnit unitsPerPage = pool->unitsPerPage;
        afxUnit pagSiz = unitsPerPage * unitSiz;
        afxUnit localIdx, idx2;
        afxBool found = FALSE;

        afxSize pageIdx = 0;
        afxPoolPage *pag;

        AFX_ITERATE_CHAIN(afxPoolPage, pag, link, &pool->freePags)
        //for (pageIdx = 0; pageIdx < pool->pageCnt; pageIdx++)
        {
            //pag = &pool->pages[pageIdx];

#ifdef _AFX_POOL_VALIDATION_ENABLED
            AFX_ASSERT(pag->fcc == afxFcc_POOL);
#endif
            if (err)
            {
                int a = 1;
            }

            for (pageIdx = 0; pageIdx < pool->pageCnt; pageIdx++)
                if (&pool->pages[pageIdx] == pag)
                    break;

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

            if (AfxTakePoolUnit(pool, idx2, NIL))
                AfxThrowError();
            else
            {
                AFX_ASSERT(AfxGetPoolUnit(pool, idx2, NIL));

                if (ids)
                    ids[reqIdx] = idx2;

                AFX_ASSERT(AfxGetPoolUnit(pool, idx2, NIL));
            }
        }
        AFX_ASSERT(AfxGetPoolUnit(pool, idx2, NIL));
        void*ptr;
        AfxGetPoolUnit(pool, idx2, &ptr);
        AFX_ASSERT(AFX_IS_ALIGNED(ptr, AFX_POOL_ALIGNMENT));

        if (!ptr)
        {
            AfxThrowError();
            AfxReclaimPoolUnits(pool, reqIdx, units);
            return err;
        }
        units[reqIdx] = ptr;
    }
    return err;
}

_AFX afxError AfxDeployPool(afxPool* pool, afxUnit unitSiz, afxUnit unitsPerPage, afxUnit memAlign)
{
    afxError err = NIL;
    AFX_ASSERT(pool);
    AFX_ASSERT(unitSiz);
    AFX_ASSERT(unitSiz > sizeof(void*));
#ifdef _AFX_POOL_VALIDATION_ENABLED
    pool->fcc = afxFcc_POOL;
#endif
    pool->unitsPerPage = AFX_CLAMP(unitsPerPage, 1, AFX_MAX_UNITS_PER_POOL_PAGE);
    pool->memAlign = AFX_MAX(AFX_POOL_ALIGNMENT, AFX_ALIGNED_SIZE(memAlign, AFX_PTR_ALIGNMENT));
    AFX_ASSERT(AFX_IS_ALIGNED(pool->memAlign, AFX_POOL_ALIGNMENT));
    pool->unitSiz = unitSiz;
    pool->totalUsedCnt = 0;
    pool->pageCnt = 0;
    pool->pages = NIL; // Will allocate on first use
    pool->mem = NIL;
    AfxDeployChain(&pool->freePags, pool);
    return err;
}

_AFX afxError AfxExhaustPool(afxPool* pool, afxBool keepPages)
{
    afxError err = NIL;
    AFX_ASSERT(pool);
#ifdef _AFX_POOL_VALIDATION_ENABLED
    AFX_ASSERT(pool->fcc == afxFcc_POOL);
#endif
    
    if (keepPages)
    {
        // Keep the pages, just reset usage tracking.

        for (afxUnit i = 0; i < pool->pageCnt; i++)
        {
            afxPoolPage* pag = &pool->pages[i];
#ifdef _AFX_POOL_VALIDATION_ENABLED
            AFX_ASSERT(pag->fcc == afxFcc_POOL);
#endif
            pag->usedCnt = 0;
            pag->usage = 0;
        }
    }
    else
    {
        // Fully free all pages.

        for (afxUnit i = 0; i < pool->pageCnt; i++)
        {
            afxPoolPage* pag = &pool->pages[i];
#ifdef _AFX_POOL_VALIDATION_ENABLED
            AFX_ASSERT(pag->fcc == afxFcc_POOL);
#endif
            pag->usedCnt = 0;
            pag->usage = 0;

            if (pag->data)
            {
                AfxDeallocate((void**)&pag->data, AfxHere());
                pag->data = NIL;
            }
        }

        if (pool->pageCnt)
        {
            AFX_ASSERT(pool->pages);
            AfxDeallocate((void**)&pool->pages, AfxHere());
            pool->pages = NIL;
        }
        pool->pageCnt = 0;
    }

    pool->totalUsedCnt = 0;

    AFX_ASSERT((pool->pageCnt && pool->pages) || (!pool->pageCnt && !pool->pages));

    return err;
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

    void* unit;
    for (afxUnit i = 0; i < 3000; i++)
        AfxRequestPoolUnits(&pool, 1, &idx, &unit);


    AfxExhaustPool(&pool, FALSE);

    return err;
}
