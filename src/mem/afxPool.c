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

#define _AFX_ENFORCE_CLEAN_POOL_ALLOCATION (FALSE)
#define _AFX_POOL_ZERO_ON_REQUISITION (FALSE)
#define _AFX_POOL_ZERO_ON_RECLAMATION (FALSE)
#define _AFX_POOL_ZERO_ON_PAGINATION (FALSE)

_AFXINL afxBool AfxIsAnValidPoolUnit(afxPool* pool, afxSize idx)
{
    afxError err = { 0 };
    AFX_ASSERT(pool);
#ifdef _AFX_POOL_VALIDATION_ENABLED
    AFX_ASSERT(pool->fcc == afxFcc_POOL);
#endif

    // Avoid division by zero.
    if (!pool->unitsPerPage) return FALSE;

    afxSize pageIdx = idx / pool->unitsPerPage;

    if (0 == pool->pageCnt || (pageIdx >= pool->pageCnt))
        return err; // not paged/found

    AFX_ASSERT(pool->pages);
    afxPoolPage* pag = pool->pages[pageIdx];
#ifdef _AFX_POOL_VALIDATION_ENABLED
    AFX_ASSERT(pag->fcc == afxFcc_POOL);
#endif

    AFX_ASSERT(pag->base);

    afxSize localIdx = idx % /*AFX_MAX_UNITS_PER_POOL_PAGE*/pool->unitsPerPage;
    //afxUnit32 mask = AFX_BITMASK(localIdx);
    void* unit = &pag->base[pool->unitSiz * localIdx];
    afxBool booked = AFX_TEST_BIT_SET(pag->usage, localIdx); //pag->usage & mask;
    return booked;
}

_AFX afxUnit AfxEnumeratePoolItems(afxPool const* pool, afxUnit first, afxUnit cnt, void *items[])
{
    afxError err = { 0 };
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
                afxUnit32 mask = AFX_BITMASK(localIdx);
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
        afxPoolPage* pag = pool->pages[pageIdx];
        if (!pag) continue;

        if (!pag->usedCnt) continue;
        
        for (afxUnit unitIdx = 0; unitIdx < /*AFX_MAX_UNITS_PER_POOL_PAGE*/pool->unitsPerPage; unitIdx++)
        {
            //afxUnit32 mask = AFX_BITMASK(k);
            afxBool alloced = AFX_TEST_BIT_SET(pag->usage, unitIdx); //pag->usage & mask;

            //AfxReportWarn("pool %p, upp %u, pageIdx %u, usedCnt %u, %u, alloced %u", pool, pool->unitsPerPage, pageIdx, pag->usedCnt, unitIdx, alloced);

            if (!alloced) continue;
            
            if (posn >= first)
            {
                void* unit = &pag->base[pool->unitSiz * unitIdx];
                AFX_ASSERT(items);
                items[rslt] = unit;
                ++rslt;
            }
            ++posn;

            if (rslt == cnt)
                break;
        }

        if (rslt == cnt)
            break;
    }
    return rslt;
}

_AFX afxUnit AfxInvokePoolItems(afxPool const* pool, afxUnit first, afxUnit cnt, afxBool(*f)(void* item, void* udd), void *udd)
{
    afxError err = { 0 };
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
                afxUnit32 mask = AFX_BITMASK(localIdx);
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
        afxPoolPage *pag = pool->pages[pageIdx];
        if (!pag) continue;

        if (!pag->usedCnt) continue;

        for (afxUnit unitIdx = 0; unitIdx < /*AFX_MAX_UNITS_PER_POOL_PAGE*/pool->unitsPerPage; unitIdx++)
        {
            //afxUnit32 mask = AFX_BITMASK(k);
            afxBool alloced = AFX_TEST_BIT_SET(pag->usage, unitIdx); //pag->usage & mask;
            if (!alloced) continue;
            
            if (posn >= first)
            {
                void* unit = &pag->base[pool->unitSiz * unitIdx];

                ++rslt;

                if (f(unit, udd)) // return non-zero to abort curation
                    break;
            }
            ++posn;

            if (rslt == cnt)
                break;
        }

        if (rslt == cnt)
            break;
    }
    return rslt;
}

_AFX afxBool AfxGetLinearPoolUnits(afxPool const* pool, afxUnit first, afxUnit cnt, void *ptr[])
{
    afxError err = { 0 };
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
    afxError err = { 0 };
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
    afxError err = { 0 };
    AFX_ASSERT(pool);
#ifdef _AFX_POOL_VALIDATION_ENABLED
    AFX_ASSERT(pool->fcc == afxFcc_POOL);
#endif
    afxBool alloced = 0;

    // Avoid division by zero.
    if (!pool->unitsPerPage) return FALSE;

    afxSize pageIdx = idx / pool->unitsPerPage;

    if (pool->pageCnt > pageIdx)
    {
        AFX_ASSERT(pool->pages);
        afxPoolPage* pag = pool->pages[pageIdx];
        
        if (!pag)
        {
            return FALSE;
        }

        afxSize localIdx = idx % /*AFX_MAX_UNITS_PER_POOL_PAGE*/pool->unitsPerPage;
        //afxUnit32 mask = AFX_BITMASK(localIdx);
        alloced = AFX_TEST_BIT_SET(pag->usage, localIdx); //pag->usage & mask;

        if (pag->base)
        {
            if (/*alloced && */ptr)
                *ptr = &pag->base[pool->unitSiz * localIdx];

        }
    }
    return alloced;
}

#if 0
_AFX afxBool AfxGetPoolItem(afxPool const* pool, afxSize idx, void **ptr)
{
    afxError err = { 0 };
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
            afxUnit32 mask = AFX_BITMASK(localIdx);
            alloced = pag->usage & mask;

            if (alloced && ptr)
                *ptr = &pag->bytes[pool->unitSiz * localIdx];
        }
    }
    return alloced;
}
#endif

_AFX afxError _AfxInflatePoolDir(afxPool* pool, afxUnit pagCnt)
{
    afxError err = { 0 };
    AFX_ASSERT(pool);
#ifdef _AFX_POOL_VALIDATION_ENABLED
    AFX_ASSERT(pool->fcc == afxFcc_POOL);
#endif

    if (0 == pool->pageCnt || (pagCnt >= pool->pageCnt))
    {
        afxSize diff = pagCnt - pool->pageCnt;
        AFX_ASSERT(diff);

        if (AfxReallocate(sizeof(pool->pages[0]) * (pool->pageCnt + diff), pool->memAlign, AfxHere(), (void**)&pool->pages))
        {
            AfxThrowError();
            return err;
        }
        else
        {
            for (afxSize i = 0; i < diff; i++)
            {
                pool->pages[pool->pageCnt + i] = NIL;
            }
            pool->pageCnt += diff;
        }
    }
    return err;
}

_AFX afxError AfxTakePoolUnit(afxPool* pool, afxSize idx, void *val)
{
    afxError err = { 0 };
    AFX_ASSERT(pool);
#ifdef _AFX_POOL_VALIDATION_ENABLED
    AFX_ASSERT(pool->fcc == afxFcc_POOL);
#endif
    
    // Avoid division by zero.
    if (!pool->unitsPerPage) return FALSE;

    afxPoolPage *pag;
    afxSize pageIdx = idx / pool->unitsPerPage;

    if (0 == pool->pageCnt || (pageIdx >= pool->pageCnt))
    {
        if (_AfxInflatePoolDir(pool, (pageIdx + 1)))
        {
            AfxThrowError();
            return err;
        }
    }

    AFX_ASSERT(pool->pages);
    pag = pool->pages[pageIdx];

    if (!pag)
    {
        afxUnit pagSiz = pool->unitsPerPage * pool->unitSiz;
        pagSiz = AFX_ALIGN_SIZE(pagSiz, pool->memAlign);

        afxByte* pagHeap;
        if (AfxAllocate(pagSiz + sizeof(*pag), pool->memAlign, AfxHere(), (void**)&pagHeap))
        {
            AfxThrowError();
            return err;
        }

        pag = AFX_CAST(afxPoolPage*, pagHeap + pagSiz);
        AFX_ASSERT_ALIGNMENT(pag, AFX_PTR_ALIGNMENT);
#ifdef _AFX_POOL_VALIDATION_ENABLED
        AFX_ASSERT(pag->fcc = afxFcc_POOL);
#endif//_AFX_POOL_VALIDATION_ENABLED
        AfxPushLink(&pag->link, &pool->freePags);
        pag->usedCnt = 0;
        pag->usage = NIL;
        pag->base = pagHeap;
#ifdef _AFX_ENFORCE_CLEAN_POOL_ALLOCATION
        // TODO: Do not zero memory.
        AfxZero(pag->base, pool->unitSiz * pool->unitsPerPage);
#endif
        pool->pages[pageIdx] = pag;
    }

#ifdef _AFX_POOL_VALIDATION_ENABLED
    AFX_ASSERT(pag->fcc == afxFcc_POOL);
#endif

    afxSize localIdx = idx % /*AFX_MAX_UNITS_PER_POOL_PAGE*/pool->unitsPerPage;
    //afxUnit32 mask = AFX_BITMASK(localIdx);
    void* unit = &pag->base[pool->unitSiz * localIdx];
    afxBool alloced = AFX_TEST_BIT_SET(pag->usage, localIdx); //pag->usage & mask;

    if (alloced)
        AfxThrowError();

    if (val)
        AfxCopy(unit, val, pool->unitSiz);

    //if (!alloced)
    {
        pag->usage |= AFX_BITMASK(localIdx);
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
    afxError err = { 0 };
    AFX_ASSERT(pool);
#ifdef _AFX_POOL_VALIDATION_ENABLED
    AFX_ASSERT(pool->fcc == afxFcc_POOL);
#endif

    afxPoolPage *pag;
    afxSize pageIdx = idx / pool->unitsPerPage;

    if (0 == pool->pageCnt || (pageIdx >= pool->pageCnt))
        return err; // not paged/found

    AFX_ASSERT(pool->pages);
    pag = pool->pages[pageIdx];

    if (!pag)
        return err;

    afxSize localIdx = idx % /*AFX_MAX_UNITS_PER_POOL_PAGE*/pool->unitsPerPage;
    //afxUnit32 mask = AFX_BITMASK(localIdx);
    void* unit = &pag->base[pool->unitSiz * localIdx];
    afxBool alloced = AFX_TEST_BIT_SET(pag->usage, localIdx); //pag->usage & mask;

    if (alloced)
    {
        if (pag->usedCnt == pool->unitsPerPage)
        {
            // insert in free pages if not full again.
            AfxPushLink(&pag->link, &pool->freePags);
        }
        pag->usage &= ~AFX_BITMASK(localIdx);
        --pag->usedCnt;
        --pool->totalUsedCnt;
    }

    AFX_ASSERT(!AfxGetPoolUnit(pool, idx, NIL));
    return err;
}

_AFX afxBool AfxFindPoolUnit(afxPool* pool, void* unit2, afxUnit* idx, afxUnit* localIdx)
{
    afxError err = { 0 };
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
        if (!pag) continue;

        afxSize base = (afxSize)pag->base;

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
    afxError err = { 0 };
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

    if (!pag)
    {
        AfxThrowError();
        return err;
    }

#ifdef _AFX_POOL_VALIDATION_ENABLED
    AFX_ASSERT(pag->fcc == afxFcc_POOL);
#endif

    if (0 == pag->usedCnt)
    {
        AFX_ASSERT(pag->usage == 0);
        //AfxReportWarn("Deallocate page %p, data %p, usage %p", pag, pag->data, pag->usage);
        pag->usage = NIL;
        //pag->base = NIL;
        AfxPopLink(&pag->link);
        void*pagHeap = pag->base;
        AfxDeallocate((void**)&pagHeap, AfxHere());
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
    else
    {
        AfxThrowError();
    }
    return err;
}

_AFX afxError AfxReclaimPoolUnits(afxPool* pool, afxHere const dbg, afxUnit cnt, void* units[])
{
    afxError err = { 0 };
    AFX_ASSERT(pool);
#ifdef _AFX_POOL_VALIDATION_ENABLED
    AFX_ASSERT(pool->fcc == afxFcc_POOL);
#endif
    AFX_ASSERT(units);

    // 4 (x32) set  --- 128 bits per page
    // 2 // page

    afxUnit unitSiz = pool->unitSiz;
    afxUnit unitsPerPage = pool->unitsPerPage;

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxByte* unit = units[i];
        //afxSize pagSiz = unitsPerPage * unitSiz;
        afxUnit localIdx, idx;
        afxBool found = AfxFindPoolUnit(pool, unit, &idx, &localIdx);

        if (!found)
        {
            AfxThrowError();
            continue;
        }

        afxSize pageIdx = idx / unitsPerPage;
        AFX_ASSERT(pageIdx < pool->pageCnt);

        AFX_ASSERT(pool->pages);
        afxPoolPage* pag = pool->pages[pageIdx];
        if (!pag)
        {
            AfxThrowError();
            return err;
        }

#ifdef _AFX_POOL_VALIDATION_ENABLED
        AFX_ASSERT(pag->fcc == afxFcc_POOL);
#endif

        if (pag->usedCnt == 0)
        {
            AfxThrowError();
            continue;
        }

        AFX_ASSERT(pag->base);
        AFX_ASSERT(pag->usage);
        //afxSize localIdx = idx % /*AFX_MAX_UNITS_PER_POOL_PAGE*/pool->unitsPerPage;
        //afxUnit32 mask = AFX_BITMASK(localIdx);
        //void* unit = &pag->data[pool->unitSiz * localIdx];
        afxBool alloced = AFX_TEST_BIT_SET(pag->usage, localIdx);// pag->usage & mask;

        if (!alloced)
        {
            AfxThrowError();
            continue;
        }

        if (pag->usedCnt == pool->unitsPerPage)
        {
            // insert in free pages if not full again.
            AfxPushLink(&pag->link, &pool->freePags);
        }
#ifdef _AFX_ENFORCE_CLEAN_POOL_ALLOCATION
        AfxZero(unit, unitSiz);
#endif
        pag->usage &= ~AFX_BITMASK(localIdx);
        --pag->usedCnt;
        --pool->totalUsedCnt;

        AFX_ASSERT(!AfxGetPoolUnit(pool, idx, NIL));

        if (0 == pag->usedCnt)
        {
            DeletePoolPage(pool, pageIdx);
        }
        AFX_ASSERT(!AfxGetPoolUnit(pool, idx, NIL));
    }
    return err;
}

_AFX afxError AfxRequestPoolUnitsAt(afxPool* pool, afxHere const dbg, afxUnit cnt, afxUnit const ids[], void* units[])
{
    afxError err = { 0 };
    AFX_ASSERT(pool);
#ifdef _AFX_POOL_VALIDATION_ENABLED
    AFX_ASSERT(pool->fcc == afxFcc_POOL);
#endif
    AFX_ASSERT(units);
    AFX_ASSERT(ids);

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxUnit idx = ids[i];
        AfxTakePoolUnit(pool, idx, NIL);
        AfxGetPoolUnit(pool, idx, &units[i]);
    }
    return err;
}

_AFX afxError AfxRequestPoolUnits(afxPool* pool, afxHere const dbg, afxUnit cnt, afxUnit ids[], void* units[])
{
    afxError err = { 0 };
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
        afxUnit localIdx, idx2;
        afxBool pageFound = FALSE;
        afxBool unitFound = FALSE;

        afxSize nextPageIdx = 0;
        afxSize pageIdx = 0;
        afxPoolPage *pag;

#if 0
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
                        //afxUnit32 mask = AFX_BITMASK(k);
                        afxBool alloced = AFX_TEST_BIT_SET(pag->usage, unitIdx); // pag->usage & mask;

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
#else
        if (!unitFound)
        {
            for (pageIdx = 0; pageIdx < pool->pageCnt; pageIdx++)
            {
                pag = pool->pages[pageIdx];
                if (!pag)
                {
                    if (!pageFound)
                    {
                        pageFound = TRUE;
                        nextPageIdx = pageIdx;
                    }
                    continue;
                }
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
                        unitFound = TRUE;
                        break;
                    }
                }

                if (unitFound)
                    break;
                else
                    AfxThrowError();
            }
        }
#endif

        if (!unitFound)
        {
            // if no room, generate a new page.

            if (!pageFound)
            {
                pageIdx = pool->pageCnt;
                pageFound = TRUE;
            }
            else
                pageIdx = nextPageIdx;

            localIdx = 0;
            unitFound = TRUE;
        }

        idx2 = (pageIdx * unitsPerPage) + localIdx;

        if (unitFound)
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
        AFX_ASSERT(AFX_TEST_ALIGNMENT(ptr, AFX_POOL_ALIGNMENT));

        if (!ptr)
        {
            AfxThrowError();
            AfxReclaimPoolUnits(pool, AfxHere(), reqIdx, units);
            return err;
        }
        units[reqIdx] = ptr;
    }
    return err;
}

_AFX afxError AfxSetUpPool(afxPool* pool, afxUnit unitSiz, afxUnit unitsPerPage, afxUnit memAlign)
{
    afxError err = { 0 };
    AFX_ASSERT(pool);
    AFX_ASSERT(unitSiz);
    AFX_ASSERT(unitSiz > sizeof(void*));
#ifdef _AFX_POOL_VALIDATION_ENABLED
    pool->fcc = afxFcc_POOL;
#endif
    pool->unitsPerPage = AFX_CLAMP(unitsPerPage, 1, AFX_MAX_UNITS_PER_POOL_PAGE);
    pool->memAlign = AFX_MAX(AFX_POOL_ALIGNMENT, AFX_ALIGN_SIZE(memAlign, AFX_PTR_ALIGNMENT));
    AFX_ASSERT(AFX_TEST_ALIGNMENT(pool->memAlign, AFX_POOL_ALIGNMENT));
    pool->unitSiz = unitSiz;
    pool->totalUsedCnt = 0;
    pool->pageCnt = 0;
    pool->pages = NIL; // Will allocate on first use
    pool->mem = NIL;
    AfxMakeChain(&pool->freePags, pool);
    return err;
}

_AFX afxError AfxExhaustPool(afxPool* pool, afxBool keepPages)
{
    afxError err = { 0 };
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
            pag->usage = 0;
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
            pag->usage = 0;

            if (pag->base)
            {
                AfxDeallocate((void**)&pag->base, AfxHere());
                pag->base = NIL;
            }
            pool->pages[i] = NIL;
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
    afxError err = { 0 };


    afxPool pool;
    AfxSetUpPool(&pool, 4, 33, 0);
    afxSize idx;

    for (afxUnit i = 0; i < 3000; i++)
        AfxTakePoolUnit(&pool, i, &idx);

    //afxUnit j = 3000;
    //for (afxUnit i = 0; i < 3000; i++)
        //AfxPopPoolUnit(&pool, --j);

    void* unit;
    for (afxUnit i = 0; i < 3000; i++)
        AfxRequestPoolUnits(&pool, AfxHere(), 1, &idx, &unit);


    AfxExhaustPool(&pool, FALSE);

    return err;
}
