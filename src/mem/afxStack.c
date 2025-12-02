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

_AFX afxBool _AfxStackEnsureRoom(afxStack* stak, afxUnit requiredUnits)
{
    afxError err;
    AFX_ASSERT(stak);
#ifdef _AFX_STACK_VALIDATION_ENABLED
    AFX_ASSERT(stak->fcc == afxFcc_STAK);
#endif
    AFX_ASSERT(requiredUnits);
    afxStackPage* blk = stak->lastBlock;

    while (requiredUnits > 0)
    {
        afxBool needNewBlock = (!blk || blk->usedUnitCnt == stak->unitsPerBlock);

        if (needNewBlock)
        {
            if (stak->blockDir && stak->activeBlocks >= stak->maxActiveBlocks)
                return FALSE; // Hit limit

            afxStackPage* newBlk;
            if (!AfxAllocate(sizeof(*newBlk), 0, AfxHere(), (void**)&newBlk))
                return FALSE;

            afxUnit blockSize = stak->unitSiz * stak->unitsPerBlock;
            if (!AfxAllocate(blockSize, 0, AfxHere(), (void**)&newBlk->units))
                return FALSE;

            newBlk->usedUnitCnt = 0;
            newBlk->firstIdx = stak->totalUsedUnitCnt;
            newBlk->prev = stak->lastBlock;

            stak->lastBlock = newBlk;
            blk = newBlk;

            if (!stak->blockDir && stak->maxActiveBlocks != (afxUnit)-1)
            {
                afxStackPage** dir;
                AfxAllocate(stak->maxActiveBlocks * sizeof(afxStackPage*), 0, AfxHere(), (void**)&dir);
                stak->blockDir = dir;
                AfxZero(dir, stak->maxActiveBlocks * sizeof(afxStackPage*));
            }

            if (stak->blockDir)
                stak->blockDir[stak->activeBlocks++] = newBlk;
        }

        afxUnit unitsAvailable = stak->unitsPerBlock - blk->usedUnitCnt;
        afxUnit unitsToConsume = (requiredUnits < unitsAvailable) ? requiredUnits : unitsAvailable;

        blk->usedUnitCnt += unitsToConsume;
        stak->totalUsedUnitCnt += unitsToConsume;
        requiredUnits -= unitsToConsume;

#ifdef _AFX_STACK_METRICS_ENABLED
        stak->totalAllocatedBlocks += 1;
        stak->totalAllocatedBytes += (stak->unitsPerBlock * stak->unitSiz);
#endif
    }
    return TRUE;
}

_AFX void* AfxPushStack(afxStack* stak, afxUnit cnt, afxUnit* first, void const* src, afxUnit stride)
{
    afxError err;
    AFX_ASSERT(stak);
#ifdef _AFX_STACK_VALIDATION_ENABLED
    AFX_ASSERT(stak->fcc == afxFcc_STAK);
#endif
    AFX_ASSERT(cnt);

    if (!cnt || !_AfxStackEnsureRoom(stak, cnt))
    {
        if (first) *first = -1;
        return NULL;
    }

    afxStackPage* blk = stak->lastBlock;
    afxUnit startIdx = stak->totalUsedUnitCnt - cnt;

    if (first) *first = startIdx;

    afxUnit offset = (blk->usedUnitCnt - cnt) * stak->unitSiz;
    afxByte* dst = &blk->units[offset];

    if (src)
        AfxStream2(cnt, src, stride, &dst[offset], stak->unitSiz);

#ifdef _AFX_STACK_METRICS_ENABLED
    if (stak->totalUsedUnitCnt > stak->peakUsedUnitCnt)
        stak->peakUsedUnitCnt = stak->totalUsedUnitCnt;

    if (stak->activeBlocks > stak->peakActiveBlocks)
        stak->peakActiveBlocks = stak->activeBlocks;
#endif

    return dst;
}

_AFX void AfxPopStack(afxStack* stak, afxUnit cnt)
{
    afxError err;
    AFX_ASSERT(stak);
#ifdef _AFX_STACK_VALIDATION_ENABLED
    AFX_ASSERT(stak->fcc == afxFcc_STAK);
#endif
    AFX_ASSERT(cnt);

    while (cnt > 0 && stak->lastBlock)
    {
        afxStackPage* blk = stak->lastBlock;

        if (blk->usedUnitCnt > cnt)
        {
            blk->usedUnitCnt -= cnt;
            stak->totalUsedUnitCnt -= cnt;
            break;
        }
        else
        {
            cnt -= blk->usedUnitCnt;
            stak->totalUsedUnitCnt -= blk->usedUnitCnt;
            stak->lastBlock = blk->prev;

            if (stak->blockDir)
                stak->blockDir[--stak->activeBlocks] = NIL;

            AfxDeallocate((void**)&blk, AfxHere());
        }
    }
}

_AFX void* AfxPeekStack(afxStack* stak, afxUnit idx)
{
    afxError err;
    AFX_ASSERT(stak);
#ifdef _AFX_STACK_VALIDATION_ENABLED
    AFX_ASSERT(stak->fcc == afxFcc_STAK);
#endif

    if (idx >= stak->totalUsedUnitCnt)
        return NULL;

    afxStackPage* page;

    if (stak->blockDir)
    {
        page = stak->blockDir[idx / stak->unitsPerBlock];
    }
    else
    {
        page = stak->lastBlock;
        while (page && page->firstIdx > idx)
            page = page->prev;
    }

    AFX_ASSERT(page);
    return &page->units[stak->unitSiz * (idx - page->firstIdx)];
}

_AFX void AfxDumpStack(afxStack* stak, afxUnit first, afxUnit cnt, void* dst, afxUnit stride)
{
    afxError err;
    AFX_ASSERT(stak);
#ifdef _AFX_STACK_VALIDATION_ENABLED
    AFX_ASSERT(stak->fcc == afxFcc_STAK);
#endif
    AFX_ASSERT(cnt);

    afxUnit remaining = cnt;
    afxUnit offset = 0;

    while (remaining > 0)
    {
        afxStackPage* page = stak->blockDir
            ? stak->blockDir[first / stak->unitsPerBlock]
            : stak->lastBlock;

        while (!stak->blockDir && page && page->firstIdx > first)
            page = page->prev;

        afxUnit localIdx = first - page->firstIdx;
        afxUnit available = page->usedUnitCnt - localIdx;
        afxUnit toCopy = (available > remaining) ? remaining : available;

        void* src = &page->units[localIdx * stak->unitSiz];
        AfxStream2(toCopy, src, stak->unitSiz, (afxByte*)dst + offset * stride, stride);

        first += toCopy;
        remaining -= toCopy;
        offset += toCopy;
    }
}

_AFX void AfxUpdateStack(afxStack *stak, afxUnit first, afxUnit cnt, void const* src, afxUnit srcStride)
{
    afxError err;
    AFX_ASSERT(stak);
#ifdef _AFX_STACK_VALIDATION_ENABLED
    AFX_ASSERT(stak->fcc == afxFcc_STAK);
#endif
    AFX_ASSERT(cnt);

    afxUnit remaining = cnt;
    afxUnit offset = 0;

    while (remaining > 0)
    {
        afxStackPage* page = stak->blockDir
            ? stak->blockDir[first / stak->unitsPerBlock]
            : stak->lastBlock;

        while (!stak->blockDir && page && page->firstIdx > first)
            page = page->prev;

        afxUnit localIdx = first - page->firstIdx;
        afxUnit available = page->usedUnitCnt - localIdx;
        afxUnit toCopy = (available > remaining) ? remaining : available;

        void* dst = &page->units[localIdx * stak->unitSiz];
        AfxStream2(toCopy, src, srcStride, (afxByte*)dst + offset, stak->unitSiz);

        first += toCopy;
        remaining -= toCopy;
        offset += toCopy;
    }
}

_AFX void AfxQueryStackStatistics(afxStack const* stak, afxStackStats* stats)
{
    afxError err;
    AFX_ASSERT(stak);
#ifdef _AFX_STACK_VALIDATION_ENABLED
    AFX_ASSERT(stak->fcc == afxFcc_STAK);
#endif
    AFX_ASSERT(stats);

    stats->unitSiz = stak->unitSiz;
    stats->unitsPerBlock = stak->unitsPerBlock;

    stats->totalUsedUnitCnt = stak->totalUsedUnitCnt;
    stats->activeBlocks = stak->activeBlocks;
    stats->maxActiveBlocks = stak->maxActiveBlocks;

    stats->totalAllocatedUnitCnt = stak->unitsPerBlock * stak->activeBlocks;
    stats->usedBytes = stats->totalUsedUnitCnt * stak->unitSiz;
    stats->allocatedBytes = stats->totalAllocatedUnitCnt * stak->unitSiz;
    stats->slackBytes = stats->allocatedBytes - stats->usedBytes;

#ifdef _AFX_STACK_METRICS_ENABLED
    stats->peakUsedUnitCnt = stak->peakUsedUnitCnt;
    stats->peakActiveBlocks = stak->peakActiveBlocks;
    stats->totalAllocatedBlocks = stak->totalAllocatedBlocks;
    stats->totalAllocatedBytes = stak->totalAllocatedBytes;
#endif
}

_AFX void AfxPrintStackStats(afxStack const* stak)
{
    afxError err;
    AFX_ASSERT(stak);
#ifdef _AFX_STACK_VALIDATION_ENABLED
    AFX_ASSERT(stak->fcc == afxFcc_STAK);
#endif
    afxStackStats stats;
    AfxQueryStackStatistics(stak, &stats);

#if _AFX_STACK_METRICS_ENABLED
    AfxReportf(0, AfxHere(), "Stack Stats <%p>:\n"
        " - Unit size:         %u bytes\n"
        " - Units per block:   %u\n"
        " - Total used units:  %u\n"
        " - Allocated units:   %u\n"
        " - Active blocks:     %u / %u\n"
        " - Used bytes:        %u\n"
        " - Allocated bytes:   %u\n"
        " - Slack bytes:       %u\n"
        " - Peak used units : %u\n"
        " - Peak active blocks: %u\n"
        " - Total allocated blocks (cumulative): %u\n"
        " - Total allocated bytes (cumulative): %u\n",
        stak,
        stats.unitSiz,
        stats.unitsPerBlock,
        stats.totalUsedUnitCnt,
        stats.totalAllocatedUnitCnt,
        stats.activeBlocks,
        stats.maxActiveBlocks,
        stats.usedBytes,
        stats.allocatedBytes,
        stats.slackBytes,
        stats.peakUsedUnitCnt,
        stats.peakActiveBlocks,
        stats.totalAllocatedBlocks,
        stats.totalAllocatedBytes);
#else
    AfxReportf(0, AfxHere(), "Stack Stats <%p>:\n"
        " - Unit size:         %u bytes\n"
        " - Units per block:   %u\n"
        " - Total used units:  %u\n"
        " - Allocated units:   %u\n"
        " - Active blocks:     %u / %u\n"
        " - Used bytes:        %u\n"
        " - Allocated bytes:   %u\n"
        " - Slack bytes:       %u",
        stak,
        stats.unitSiz,
        stats.unitsPerBlock,
        stats.totalUsedUnitCnt,
        stats.totalAllocatedUnitCnt,
        stats.activeBlocks,
        stats.maxActiveBlocks,
        stats.usedBytes,
        stats.allocatedBytes,
        stats.slackBytes);
#endif
}

_AFX void AfxExhaustStack(afxStack* stak, afxBool fully)
{
    afxError err;
    AFX_ASSERT(stak);
#ifdef _AFX_STACK_VALIDATION_ENABLED
    AFX_ASSERT(stak->fcc == afxFcc_STAK);
#endif

    // Pop all units (frees all stack pages)
    AfxPopStack(stak, stak->totalUsedUnitCnt);

    // Optionally clear block directory
    if (stak->blockDir)
    {
        if (fully)
        {
            AfxDeallocate((void**)&stak->blockDir, AfxHere());
            stak->blockDir = NIL;
            stak->maxActiveBlocks = (afxUnit)-1;
        }
        else // PRESERVE
        {
            // Zero out but preserve the allocated memory
            AfxZero(stak->blockDir, sizeof(afxStackPage*) * stak->maxActiveBlocks);
        }
    }

    // Reset internal tracking
    stak->totalUsedUnitCnt = 0;
    stak->lastBlock = NIL;
    stak->activeBlocks = 0;
}

_AFX void AfxInitStack(afxStack* stak, afxUnit unitSiz, afxUnit unitsPerBlock, afxUnit maxUnits)
{
    afxError err;
    AFX_ASSERT(stak);
    AFX_ASSERT(unitSiz);

#ifdef _AFX_STACK_VALIDATION_ENABLED
    stak->fcc = afxFcc_STAK;
#endif

    stak->ctx = NIL;
    stak->unitSiz = unitSiz;

    if (!unitsPerBlock)
        unitsPerBlock = AfxGetMemoryPageSize() / unitSiz;

    stak->unitsPerBlock = unitsPerBlock;
    stak->maxUnits = maxUnits ? maxUnits : (afxUnit)-1;

    stak->totalUsedUnitCnt = 0;
    stak->lastBlock = NIL;
    stak->activeBlocks = 0;

    if (stak->maxUnits != (afxUnit)-1)
        stak->maxActiveBlocks = (stak->maxUnits + unitsPerBlock - 1) / unitsPerBlock;
    else
        stak->maxActiveBlocks = (afxUnit)-1;

    stak->blockDir = NIL; // <- Deferred allocation

#ifdef _AFX_STACK_METRICS_ENABLED
    stak->peakUsedUnitCnt = 0;
    stak->peakActiveBlocks = 0;
    stak->totalAllocatedBlocks = 0;
    stak->totalAllocatedBytes = 0;
#endif
}
