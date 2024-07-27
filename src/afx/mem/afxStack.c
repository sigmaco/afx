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

#include "../src/afx/dev/afxDevCoreBase.h"

_AFX afxStackPage* _AfxStackAllocateBlock(afxStack *stak)
{
    afxNat v1 = stak->unitSiz * stak->unitsPerBlock;
    afxStackPage *block = AfxAllocate(1, sizeof(*block), 0, AfxHere());
    block->units = AfxAllocate(1, v1, 0, AfxHere());

    {
        block->usedUnitCnt = 0;
        block->firstIdx = 0;
        block->prev = 0;
    }

    return block;
}

_AFX void *AfxPushStackUnit(afxStack *stak, afxNat *idx)
{
    afxStackPage *v2 = stak->lastBlock;

    if (!v2 || v2->usedUnitCnt == stak->unitsPerBlock)
    {
        if (!stak->blockDir || stak->activeBlocks < stak->maxActiveBlocks)
            v2 = _AfxStackAllocateBlock(stak);

        if (v2)
        {
            v2->firstIdx = stak->totalUsedUnitCnt;
            v2->prev = stak->lastBlock;
            afxStackPage **v3 = stak->blockDir;
            stak->lastBlock = v2;

            if (v3)
                v3[stak->activeBlocks++] = v2;
        }
    }

    afxByte *area = 0;
    *idx = -1;

    if (v2)
    {
        area = &v2->units[v2->usedUnitCnt++ * stak->unitSiz];
        *idx = stak->totalUsedUnitCnt++;
    }
    return area;
}

_AFX char AfxPushStackUnits(afxStack *stak, afxNat cnt, afxNat *firstIdx, void const *initialVal)
{
    afxNat v4 = cnt;

    if (cnt <= 0)
        return 0;

    if (!AfxPushStackUnit(stak, &cnt))
        return 0;

    afxNat v6 = cnt;
    void const *v7 = initialVal;

    if (initialVal)
    {
        void *v8 = AfxGetStackUnit(stak, cnt);
        AfxCopy2(1, stak->unitSiz, v7, v8);
    }

    afxNat v9 = v4 - 1;

    *firstIdx = v6;

    if (!v9)
        return 1;

    while (1)
    {
        --v9;
        afxByte *v10 = AfxPushStackUnit(stak, &cnt);

        if (!v10)
            break;

        AfxCopy2(1, stak->unitSiz, v7, v10);

        if (!v9)
            return 1;
    }
    return 0;
}

_AFX void AfxDumpStack(afxStack *stak, void *dst)
{
    afxByte *v2 = (afxByte*)dst + stak->unitSiz * stak->unitsPerBlock;

    for (afxStackPage *i = stak->lastBlock; i; i = i->prev)
    {
        afxNat v4 = stak->unitSiz * i->usedUnitCnt;
        afxByte *v5 = i->units;
        v2 -= v4;
        afxByte *v6 = v2;

        if (v4)
        {
            afxNat v7 = stak->unitSiz * i->usedUnitCnt;
            do
            {
                *v6++ = *v5++;
                --v7;
            } while (v7);
        }
    }
}

_AFX void* AfxGetStackUnit(afxStack *stak, afxNat idx)
{
    afxStackPage** v2 = stak->blockDir;
    afxStackPage* v3;

    if (v2)
    {
        v3 = v2[idx / stak->unitsPerBlock];
        return &v3->units[stak->unitSiz * (idx - v3->firstIdx)];
    }

    v3 = stak->lastBlock;

    if (v3->firstIdx <= idx)
        return &v3->units[stak->unitSiz * (idx - v3->firstIdx)];

    do
        v3 = v3->prev;
    while (v3->firstIdx > idx);

    return &v3->units[stak->unitSiz * (idx - v3->firstIdx)];
}

_AFX void AfxDumpStackElement(afxStack *stak, afxNat idx, void *dst)
{
    afxStackPage** v2 = stak->blockDir;
    afxStackPage* v3;

    if (v2)
    {
        v3 = v2[idx / stak->unitsPerBlock];
        AfxCopy2(1, stak->unitSiz, &v3->units[stak->unitSiz * (idx - v3->firstIdx)], dst);
        return;
    }

    v3 = stak->lastBlock;

    if (v3->firstIdx <= idx)
    {
        AfxCopy2(1, stak->unitSiz, &v3->units[stak->unitSiz * (idx - v3->firstIdx)], dst);
        return;
    }

    do v3 = v3->prev;
    while (v3->firstIdx > idx);

    AfxCopy2(1, stak->unitSiz, &v3->units[stak->unitSiz * (idx - v3->firstIdx)], dst);
}

_AFX void AfxDumpStackElements(afxStack *stak, afxNat first, afxNat cnt, void *dst)
{
    afxStackPage** v2 = stak->blockDir;
    afxStackPage* v3;

    if (v2)
    {
        v3 = v2[first / stak->unitsPerBlock];
        AfxCopy2(cnt, stak->unitSiz, &v3->units[stak->unitSiz * (first - v3->firstIdx)], dst);
        return;
    }

    v3 = stak->lastBlock;

    if (v3->firstIdx <= first)
    {
        AfxCopy2(cnt, stak->unitSiz, &v3->units[stak->unitSiz * (first - v3->firstIdx)], dst);
        return;
    }

    do
        v3 = v3->prev;
    while (v3->firstIdx > first);

    AfxCopy2(cnt, stak->unitSiz, &v3->units[stak->unitSiz * (first - v3->firstIdx)], dst);
}

_AFX void AfxUpdateStackElement(afxStack *stak, afxNat idx, void const* src)
{
    afxStackPage** v2 = stak->blockDir;
    afxStackPage* v3;

    if (v2)
    {
        v3 = v2[idx / stak->unitsPerBlock];
        AfxCopy2(1, stak->unitSiz, src, &v3->units[stak->unitSiz * (idx - v3->firstIdx)]);
        return;
    }

    v3 = stak->lastBlock;

    if (v3->firstIdx <= idx)
    {
        AfxCopy2(1, stak->unitSiz, src, &v3->units[stak->unitSiz * (idx - v3->firstIdx)]);
        return;
    }

    do
        v3 = v3->prev;
    while (v3->firstIdx > idx);

    AfxCopy2(1, stak->unitSiz, src, &v3->units[stak->unitSiz * (idx - v3->firstIdx)]);
}

_AFX void AfxUpdateStackElements(afxStack *stak, afxNat first, afxNat cnt, void const* src)
{
    afxStackPage** v2 = stak->blockDir;
    afxStackPage* v3;

    if (v2)
    {
        v3 = v2[first / stak->unitsPerBlock];
        AfxCopy2(cnt, stak->unitSiz, src, &v3->units[stak->unitSiz * (first - v3->firstIdx)]);
        return;
    }

    v3 = stak->lastBlock;

    if (v3->firstIdx <= first)
    {
        AfxCopy2(cnt, stak->unitSiz, src, &v3->units[stak->unitSiz * (first - v3->firstIdx)]);
        return;
    }

    do
        v3 = v3->prev;
    while (v3->firstIdx > first);

    AfxCopy2(cnt, stak->unitSiz, src, &v3->units[stak->unitSiz * (first - v3->firstIdx)]);
}

_AFX void AfxPopStackUnits(afxStack *stak, afxNat cnt)
{
    afxNat v2 = cnt;

    while (stak->lastBlock)
    {
        afxStackPage *v3 = stak->lastBlock;

        if (v3->usedUnitCnt > v2)
            break;

        afxStackPage *v4 = stak->lastBlock;
        stak->totalUsedUnitCnt -= v3->usedUnitCnt;
        v2 -= v3->usedUnitCnt;
        stak->lastBlock = v3->prev;
        AfxDeallocate(v4);
        afxStackPage **v5 = stak->blockDir;

        if (v5)
            v5[stak->activeBlocks-- - 1] = NIL;
    }

    afxStackPage *v6 = stak->lastBlock;

    if (v6)
    {
        stak->totalUsedUnitCnt -= v2;
        v6->usedUnitCnt -= v2;
    }
}

_AFX void AfxEmptyStack(afxStack *stak)
{
    AfxPopStackUnits(stak, stak->totalUsedUnitCnt);

    if (stak->blockDir)
    {
        AfxDeallocate(stak->blockDir);
        stak->blockDir = NIL;
    }
}

_AFX void AfxDeallocateStack(afxStack *stak)
{
    AfxPopStackUnits(stak, stak->totalUsedUnitCnt);

    if (stak->blockDir)
    {
        AfxDeallocate(stak->blockDir);
        stak->blockDir = NIL;
    }
}

_AFX void AfxAllocateStack(afxStack *stak, afxNat unitSiz, afxNat unitsPerBlock)
{
    stak->ctx = NIL;

    stak->unitSiz = unitSiz;
    stak->unitsPerBlock = unitsPerBlock ? unitsPerBlock : AfxGetMemoryPageSize() / unitSiz;
    stak->totalUsedUnitCnt = 0;
    stak->lastBlock = NIL;
    stak->maxUnits = (afxNat)-1;
    stak->activeBlocks = (afxNat)-1;
    stak->maxActiveBlocks = (afxNat)-1;
    stak->blockDir = NIL;
}

_AFX void AfxAllocatePagedStack(afxStack *stak, afxNat unitSiz, afxNat unitsPerBlock, afxNat maxUnits)
{
    stak->ctx = NIL;

    stak->unitSiz = unitSiz;
    stak->maxUnits = maxUnits;
    afxNat v4 = unitsPerBlock ? (unitsPerBlock + maxUnits - 1) / unitsPerBlock : AfxGetMemoryPageSize() / unitSiz;
    stak->unitsPerBlock = unitsPerBlock;
    stak->totalUsedUnitCnt = 0;
    stak->lastBlock = NIL;
    stak->activeBlocks = 0;
    stak->maxActiveBlocks = v4;
    afxStackPage**v5 = AfxAllocate(v4, sizeof(afxStackPage*), 0, AfxHere());
    afxNat v6 = sizeof(afxStackPage*) * stak->maxActiveBlocks;
    stak->blockDir = v5;
    AfxZero2(1, v6, v5);
}
