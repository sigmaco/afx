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

#define _AFX_POOL_ZERO_ON_REQUISITION (FALSE)
#define _AFX_POOL_ZERO_ON_RECLAMATION (FALSE)
#define _AFX_POOL_ZERO_ON_PAGINATION (FALSE)
#define _AFX_POOL_PAGE_HEADER (TRUE)
//#define _AFX_POOL_USE_FREE_PAGES (TRUE)

_AFXINL afxBool AfxIsAnValidPoolUnit(afxPool* pool, afxSize idx)
{
    afxError err = NIL;
    AFX_ASSERT(pool);
#ifdef _AFX_POOL_VALIDATION_ENABLED
    AFX_ASSERT(pool->fcc == afxFcc_POOL);
#endif

    afxSize pageIdx = idx / pool->unitsPerPage;

    if (0 == pool->pageCnt || (pageIdx >= pool->pageCnt))
        return err; // not paged/found

    AFX_ASSERT(pool->pages);
    afxPoolPage* pag = pool->pages[pageIdx];
#ifdef _AFX_POOL_VALIDATION_ENABLED
    AFX_ASSERT(pag->fcc == afxFcc_POOL);
#endif

    AFX_ASSERT(pag->heap);

    afxSize localIdx = idx % /*AFX_MAX_UNITS_PER_POOL_PAGE*/pool->unitsPerPage;
    //afxUnit32 mask = AFX_BITMASK(localIdx);
    void* unit = &pag->heap[pool->unitSiz * localIdx];
    afxBool booked = AFX_TEST_BIT_SET(pag->usage, localIdx); //pag->usage & mask;
    return booked;
}

_AFX afxUnit AfxEnumeratePoolItems(afxPool const* pool, afxUnit first, afxUnit cnt, void *items[])
{
    afxError err = NIL;
    AFX_ASSERT(pool);
#ifdef _AFX_POOL_VALIDATION_ENABLED
    AFX_ASSERT(pool->fcc == afxFcc_POOL);
#endif
    AFX_ASSERT(cnt);
    afxUnit rslt = 0;

    afxUnit unitsPerPage = pool->unitsPerPage;
    afxSize pagIdx = first / unitsPerPage;
    afxSize pagCnt = AFX_MIN(1, cnt / unitsPerPage);

    afxUnit posn = first;

    for (; pagIdx < pagCnt; pagIdx++)
    {
        afxPoolPage* pag = pool->pages[pagIdx];
        if (!pag) continue;
#ifdef _AFX_POOL_VALIDATION_ENABLED
        AFX_ASSERT(pag->fcc == afxFcc_POOL);
#endif
        if (pag->usedCnt == 0)
            continue;
        
        for (afxUnit unitIdx = 0; unitIdx < unitsPerPage; unitIdx++)
        {
            //afxUnit32 mask = AFX_BITMASK(k);
            afxBool booked = AFX_TEST_BIT_SET(pag->usage, unitIdx); //pag->usage & mask;
            if (!booked) continue;

            //AfxReportWarn("pool %p, upp %u, pageIdx %u, usedCnt %u, %u, alloced %u", pool, pool->unitsPerPage, pageIdx, pag->usedCnt, unitIdx, alloced);

            if (posn >= first)
            {
                void* unit = &pag->heap[pool->unitSiz * unitIdx];
                AFX_ASSERT(items);
                items[rslt] = unit;
                ++rslt;
            }
            ++posn;

            if (rslt == cnt) break;
        }

        if (rslt == cnt) break;
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

    afxUnit unitsPerPage = pool->unitsPerPage;
    afxSize pagIdx = first / unitsPerPage;
    afxSize pagCnt = AFX_MIN(1, cnt / unitsPerPage);

    afxUnit posn = first;

    for (; pagIdx < pagCnt; pagIdx++)
    {
        afxPoolPage* pag = pool->pages[pagIdx];
        if (!pag) continue;
#ifdef _AFX_POOL_VALIDATION_ENABLED
        AFX_ASSERT(pag->fcc == afxFcc_POOL);
#endif
        if (pag->usedCnt == 0) continue;

        for (afxUnit unitIdx = 0; unitIdx < unitsPerPage; unitIdx++)
        {
            //afxUnit32 mask = AFX_BITMASK(k);
            afxBool booked = AFX_TEST_BIT_SET(pag->usage, unitIdx); //pag->usage & mask;
            if (!booked) continue;

            if (posn >= first)
            {
                void* unit = &pag->heap[pool->unitSiz * unitIdx];

                ++rslt;

                if (f(unit, udd)) // return non-zero to abort curation
                    break;
            }
            ++posn;

            if (rslt == cnt) break;
        }

        if (rslt == cnt) break;
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

    afxSize pagIdx = idx / pool->unitsPerPage;

    if (pagIdx >= pool->pageCnt)
        return FALSE;

    AFX_ASSERT(pool->pages);
    afxPoolPage *pag = pool->pages[pagIdx];
    if (!pag) return FALSE;
#ifdef _AFX_POOL_VALIDATION_ENABLED
    AFX_ASSERT(pag->fcc == afxFcc_POOL);
#endif
    AFX_ASSERT(pag->heap);

    afxSize localIdx = idx % /*AFX_MAX_UNITS_PER_POOL_PAGE*/pool->unitsPerPage;
    //afxUnit32 mask = AFX_BITMASK(localIdx);
    afxBool alloced = AFX_TEST_BIT_SET(pag->usage, localIdx); //pag->usage & mask;

    if (/*alloced && */ptr)
        *ptr = &pag->heap[pool->unitSiz * localIdx];

    return alloced;
}

#if !0
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
            afxSize pagHeapSiz = pool->unitsPerPage * pool->unitSiz;
            afxSize pagSiz = sizeof(pool->pages[0]) + pagHeapSiz;

            for (afxSize i = 0; i < diff; i++)
            {
                //AFX_ASSERT(!pag);
                //AFX_ASSERT(pag->usage == 0);
                //AFX_ASSERT(pag->usedCnt == 0);

                void* pagHeap = NIL;
                if (AfxAllocate(pagSiz, pool->memAlign, AfxHere(), (void**)&pagHeap))
                {
                    AfxThrowError();
                }
                else
                {
                    AFX_ASSERT_ALIGNMENT(pagHeap, pool->memAlign);
                    afxPoolPage* pag = AFX_CAST(afxByte*, pagHeap) + pagHeapSiz;
                    AFX_ASSERT_ALIGNMENT(pag, AFX_PTR_ALIGNMENT);

#ifdef _AFX_POOL_VALIDATION_ENABLED
                    pag->fcc = afxFcc_POOL;
#endif
#ifdef _AFX_POOL_USE_FREE_PAGES
                    AfxPushLink(&pag->link, &pool->freePags);
#endif
                    pag->heap = pagHeap;
                    pag->usage = NIL;
                    pag->usedCnt = 0;

#ifdef _AFX_POOL_ZERO_ON_PAGINATION
                    // TODO: Do not zero memory.
                    AfxZero(pag->heap, pagHeapSiz);
#endif
                    pool->pages[pool->pageCnt + i] = pag;
                }
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

    afxSize pageIdx = idx / pool->unitsPerPage;

    if (0 == pool->pageCnt || (pageIdx >= pool->pageCnt))
    {
        if (AfxInflatePool(pool, (pageIdx + 1)))
        {
            AfxThrowError();
            return err;
        }
    }

    AFX_ASSERT(pool->pages);
    afxPoolPage* pag = pool->pages[pageIdx];

#ifdef _AFX_POOL_VALIDATION_ENABLED
    AFX_ASSERT(pag->fcc == afxFcc_POOL);
#endif

    afxSize localIdx = idx % pool->unitsPerPage;
    //afxUnit32 mask = AFX_BITMASK(localIdx);
    void* unit = &pag->heap[pool->unitSiz * localIdx];
    afxBool booked = AFX_TEST_BIT_SET(pag->usage, localIdx); //pag->usage & mask;

    if (booked)
    {
        AfxThrowError();
        return err;
    }

    if (val)
        AfxCopy(unit, val, pool->unitSiz);

    //if (!alloced)
    {
        pag->usage |= AFX_BITMASK(localIdx);
        AFX_ASSERT(AFX_TEST_BIT_SET(pag->usage, localIdx));
        ++pag->usedCnt;
        ++pool->totalUsedCnt;

        if (pag->usedCnt == pool->unitsPerPage)
        {
            // removed from free pages if full.
            AfxPopLink(&pag->link);
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

    afxSize pageIdx = idx / pool->unitsPerPage;

    if (0 == pool->pageCnt || (pageIdx >= pool->pageCnt))
        return err; // not paged/found

    AFX_ASSERT(pool->pages);
    afxPoolPage* pag = pool->pages[pageIdx];
    if (!pag) return err; // not allocated
#ifdef _AFX_POOL_VALIDATION_ENABLED
    AFX_ASSERT(pag->fcc == afxFcc_POOL);
#endif

    AFX_ASSERT(pag->usage);
    AFX_ASSERT(pag->usedCnt != 0);

    afxSize localIdx = idx % pool->unitsPerPage;
    //afxUnit32 mask = AFX_BITMASK(localIdx);
    void* unit = &pag->heap[pool->unitSiz * localIdx];
    afxBool booked = AFX_TEST_BIT_SET(pag->usage, localIdx); //pag->usage & mask;

    if (!booked)
    {
        AfxThrowError();
    }
    else
    {
#ifdef _AFX_POOL_USE_FREE_PAGES
        if (pag->usedCnt == pool->unitsPerPage)
        {
            // insert in free pages if not full again.
            AfxPushLink(&pag->link, &pool->freePags);
        }
#endif
        pag->usage &= ~AFX_BITMASK(localIdx);
        AFX_ASSERT(!AFX_TEST_BIT_SET(pag->usage, localIdx));
        --pag->usedCnt;
        --pool->totalUsedCnt;
    }

    AFX_ASSERT(!AfxGetPoolUnit(pool, idx, NIL));
    return err;
}
#endif

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
        afxPoolPage* pag = pool->pages[j];

        if (!pag)
            continue;

        afxSize base = (afxSize)pag->heap;

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

    afxPoolPage* pag = pool->pages[pagIdx];
    pool->pages[pagIdx] = NIL;

#ifdef _AFX_POOL_VALIDATION_ENABLED
    AFX_ASSERT(pag->fcc == afxFcc_POOL);
#endif

    AFX_ASSERT(0 == pag->usedCnt);

    if (0 == pag->usedCnt)
    {
        AFX_ASSERT(!pag->usage);
        //AfxReportWarn("Deallocate page %p, data %p, usage %p", pag, pag->heap, pag->usage);
        //pag->freeMask = pool->freeMaskPattern;
        pag->heap = NIL;
        AfxPopLink(&pag->link);

        AfxDeallocate((void**)&pag, AfxHere());

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

    afxUnit unitSiz = pool->unitSiz;
    afxUnit unitsPerPage = pool->unitsPerPage;
    afxSize pagSiz = unitsPerPage * unitSiz;

    for (afxUnit objIdx = 0; objIdx < cnt; objIdx++)
    {
        afxByte* unit = units[objIdx];

        afxUnit pagIdx;
        afxPoolPage* pag;
        afxUnit localIdx, idx;
        afxBool found = FALSE;

        for (pagIdx = 0; pagIdx < pool->pageCnt; pagIdx++)
        {
            AFX_ASSERT(pool->pages);
            pag = pool->pages[pagIdx];
            if (!pag) continue;
#ifdef _AFX_POOL_VALIDATION_ENABLED
            AFX_ASSERT(pag->fcc == afxFcc_POOL);
#endif//_AFX_POOL_VALIDATION_ENABLED

            afxSize base = (afxSize)pag->heap;

            if (base && (pagSiz > (afxSize)(unit - base)))
            {
                afxSize a = (afxSize)unit / unitSiz;
                afxSize b = base / unitSiz;
                localIdx = a - b;
                idx = (pagIdx * unitsPerPage) + localIdx;
                found = TRUE;
                break;
            }
        }

        if (!found)
        {
            AfxThrowError();
            continue;
        }

        AFX_ASSERT(pagIdx < pool->pageCnt);
#ifdef _AFX_POOL_VALIDATION_ENABLED
        AFX_ASSERT(pag->fcc == afxFcc_POOL);
#endif
        AFX_ASSERT(pag->usedCnt);

        AFX_ASSERT(pag->heap);
        AFX_ASSERT(AFX_TEST_BIT_SET(pag->usage, localIdx));
        //afxSize localIdx = idx % /*AFX_MAX_UNITS_PER_POOL_PAGE*/pool->unitsPerPage;
        //afxUnit32 mask = AFX_BITMASK(localIdx);
        //void* unit = &pag->heap[pool->unitSiz * localIdx];
        afxBool booked = AFX_TEST_BIT_SET(pag->usage, localIdx);// pag->usage & mask;
        AFX_ASSERT(booked);

        if (!booked)
        {
            AfxThrowError();
            continue;
        }
        else
        {
#ifdef _AFX_POOL_USE_FREE_PAGES
            if (pag->usedCnt == pool->unitsPerPage)
            {
                // insert in free pages if not full again.
                AfxPushLink(&pag->link, &pool->freePags);
            }
#endif
            pag->usage &= ~AFX_BITMASK(localIdx);
            AFX_ASSERT(!AFX_TEST_BIT_SET(pag->usage, localIdx));
#ifdef _AFX_POOL_ZERO_ON_RECLAMATION
            AfxZero(unit, unitSiz);
#endif
            --pag->usedCnt;
            --pool->totalUsedCnt;
        }

        AFX_ASSERT(!AfxGetPoolUnit(pool, idx, NIL));

        if (!pag->usage)
        {
            AFX_ASSERT(0 == pag->usedCnt);
            //AfxReportWarn("Deallocate page %p, data %p, usage %p", pag, pag->heap, pag->usage);
            //pag->freeMask = pool->freeMaskPattern;
            void* start = pag->heap;
            pag->heap = NIL;
            AfxPopLink(&pag->link);

            AfxDeallocate((void**)&start, AfxHere());
            pool->pages[pagIdx] = NIL;

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
        AFX_ASSERT(!AfxGetPoolUnit(pool, idx, NIL));
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

    afxUnit unitSiz = pool->unitSiz;
    afxUnit unitsPerPage = pool->unitsPerPage;
    afxUnit pagSiz = unitsPerPage * unitSiz;

    for (afxUnit reqIdx = 0; reqIdx < cnt; reqIdx++)
    {
        afxBool found = FALSE;

        afxUnit localIdx, globalIdx;
        afxSize pagIdx = 0;
        afxPoolPage* pag;

        if (!found)
        {
            for (pagIdx = 0; pagIdx < pool->pageCnt; pagIdx++)
            {
                pag = pool->pages[pagIdx];
                if (!pag) continue;
#ifdef _AFX_POOL_VALIDATION_ENABLED
                AFX_ASSERT(pag->fcc == afxFcc_POOL);
#endif
                AFX_ASSERT(AFX_MAX_UNITS_PER_POOL_PAGE >= pag->usedCnt);
                if (pag->usedCnt == unitsPerPage) continue;

                // TODO: Instead of using usage mask per page, start using free mask to ease discard.

                for (localIdx = 0; localIdx < unitsPerPage; localIdx++)
                {
                    //afxUnit32 mask = AFX_BITMASK(k);
                    afxBool booked = AFX_TEST_BIT_SET(pag->usage, localIdx); // pag->usage & mask;

                    if (!booked)
                    {
                        found = TRUE;
                        break;
                    }
                }

                if (found)
                    break;
                else
                    AfxThrowError();
            }
        }

#ifdef _AFX_POOL_USE_FREE_PAGES
        if (!found)
        {
            AFX_ITERATE_CHAIN_B2F(afxPoolPage, pag, link, &pool->freePags)
            {
#ifdef _AFX_POOL_VALIDATION_ENABLED
                AFX_ASSERT(pag->fcc == afxFcc_POOL);
#endif//_AFX_POOL_VALIDATION_ENABLED
                if (err)
                {
                    int a = 1;
                }

                // TODO: Store id on page?
                for (pageIdx = 0; pageIdx < pool->pageCnt; pageIdx++)
                    if (pool->pages[pageIdx] == pag) break; // for

                // If this is a free page, it must have room.
                AFX_ASSERT(pag->usedCnt < unitsPerPage);

                if (pag->usedCnt == 0)
                {
                    localIdx = 0;
                    found = TRUE;
                    break;
                }
                else
                {
                    AFX_ASSERT(AFX_MAX_UNITS_PER_POOL_PAGE >= pag->usedCnt);

                    for (localIdx = 0; localIdx < unitsPerPage; localIdx++)
                    {
                        //afxUnit32 mask = AFX_BITMASK(k);
                        afxBool booked = AFX_TEST_BIT_SET(pag->usage, localIdx); // pag->usage & mask;

                        if (!booked)
                        {
                            found = TRUE;
                            break; // for
                        }
                    }
                }

                if (found) break;
            }
        }
#endif//_AFX_POOL_USE_FREE_PAGES

        if (!found)
        {
            // if no room, generate a new page.
            pagIdx = pool->pageCnt;
            localIdx = 0;
            found = TRUE;
        }

        if (!found)
        {
            AfxThrowError();
            continue;
        }

        globalIdx = (pagIdx * unitsPerPage) + localIdx;

        AFX_ASSERT(!AfxGetPoolUnit(pool, globalIdx, NIL));

        if (0 == pool->pageCnt || (pagIdx >= pool->pageCnt))
        {
            afxSize diff = (pagIdx + 1) - pool->pageCnt;

            if (diff > pool->maxPagCnt)
            {
                AFX_ASSERT_CAPACITY(pool->maxPagCnt, diff);
                AfxThrowError();
                return err;
            }

            if (AfxReallocate(sizeof(pool->pages[0]) * (pool->pageCnt + diff), 0, AfxHere(), (void**)&pool->pages))
            {
                AfxThrowError();
                return err;
            }
            else
            {
                afxSize pagHeapSiz = pool->unitsPerPage * pool->unitSiz;
                afxSize pagSiz = sizeof(pool->pages[0][0]) + pagHeapSiz;

                for (afxSize i = 0; i < diff; i++)
                {
                    //AFX_ASSERT(!pag);
                    //AFX_ASSERT(pag->usage == 0);
                    //AFX_ASSERT(pag->usedCnt == 0);

                    void* pagHeap = NIL;
                    if (AfxAllocate(pagSiz, pool->memAlign, AfxHere(), (void**)&pagHeap))
                    {
                        AfxThrowError();
                    }
                    else
                    {
                        AFX_ASSERT_ALIGNMENT(pagHeap, pool->memAlign);
                        afxPoolPage* pag = (afxPoolPage*)(AFX_CAST(afxByte*, pagHeap) + pagHeapSiz);
                        AFX_ASSERT_ALIGNMENT(pag, AFX_PTR_ALIGNMENT);

#ifdef _AFX_POOL_VALIDATION_ENABLED
                        pag->fcc = afxFcc_POOL;
#endif
                        AfxPushLink(&pag->link, &pool->freePags);
                        pag->heap = pagHeap;
                        pag->usage = NIL;
                        pag->usedCnt = 0;

#ifdef _AFX_POOL_ZERO_ON_REQUISITION
                        AfxZero(pag->heap, pagHeapSiz);
#endif
                        pool->pages[pool->pageCnt + i] = pag;
                    }
                }
                pool->pageCnt += diff;
            }
        }

        AFX_ASSERT(pool->pages);
        pag = pool->pages[pagIdx];

#ifdef _AFX_POOL_VALIDATION_ENABLED
        AFX_ASSERT(pag->fcc == afxFcc_POOL);
#endif

        //afxSize localIdx = globalIdx % /*AFX_MAX_UNITS_PER_POOL_PAGE*/pool->unitsPerPage;
        //afxUnit32 mask = AFX_BITMASK(localIdx);
        void* unit = &pag->heap[pool->unitSiz * localIdx];
        afxBool booked = AFX_TEST_BIT_SET(pag->usage, localIdx); //pag->usage & mask;

        if (booked)
            AfxThrowError();

        //if (!alloced)
        {
            pag->usage |= AFX_BITMASK(localIdx);
            ++pag->usedCnt;
            ++pool->totalUsedCnt;
#ifdef _AFX_POOL_USE_FREE_PAGES
            if (pag->usedCnt == pool->unitsPerPage)
            {
                // removed from free pages if full.
                AfxPopLink(&pag->link);
            }
#endif//_AFX_POOL_USE_FREE_PAGES
        }

        AFX_ASSERT(AFX_TEST_BIT_SET(pag->usage, localIdx));
        AFX_ASSERT(AfxGetPoolUnit(pool, globalIdx, NIL));

        if (ids)
            ids[reqIdx] = globalIdx;

        if (units)
            units[reqIdx] = unit;
        
    }
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
            afxPoolPage* pag = pool->pages[i];
            if (!pag) continue;
#ifdef _AFX_POOL_VALIDATION_ENABLED
            AFX_ASSERT(pag->fcc == afxFcc_POOL);
#endif
            pag->usedCnt = 0;
            pag->usage = NIL;
            AfxPopLink(&pag->link);
#ifdef _AFX_POOL_USE_FREE_PAGES
            AfxPushLink(&pag->link, &pool->freePags);
#endif//_AFX_POOL_USE_FREE_PAGES
        }
    }
    else
    {
        // Fully free all pages.

        for (afxUnit i = 0; i < pool->pageCnt; i++)
        {
            afxPoolPage* pag = pool->pages[i];
            if (!pag) continue;
#ifdef _AFX_POOL_VALIDATION_ENABLED
            AFX_ASSERT(pag->fcc == afxFcc_POOL);
#endif
            pag->usedCnt = 0;
            pag->usage = NIL;
            AfxPopLink(&pag->link);

            void* pagHeap = pag->heap;
            AfxDeallocate((void**)&pagHeap, AfxHere());
            pool->pages[i] = NIL;
        }

        // Deallocate the directory

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
    pool->memAlign = AFX_MAX(AFX_POOL_ALIGNMENT, AFX_ALIGN_SIZE(memAlign, AFX_SIMD_ALIGNMENT));
    AFX_ASSERT(AFX_IS_ALIGNED(pool->memAlign, AFX_POOL_ALIGNMENT));
    pool->unitSiz = unitSiz;
    pool->totalUsedCnt = 0;
    pool->maxPagCnt = AFX_U32_MAX;
    pool->pageCnt = 0;
    pool->pages = NIL; // Will allocate on first use
    pool->mem = NIL;
    AfxDeployChain(&pool->freePags, pool);

    pool->freeMaskPattern = NIL;
    for (afxUnit i = 0; i < pool->unitsPerPage; i++)
        pool->freeMaskPattern |= AFX_BITMASK(i);

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
