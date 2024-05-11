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
 *                       (c) 2017 SIGMA, Engitech, Scitech, Serpro
 *                             <https://sigmaco.org/qwadro/>
 */

#include "qwadro/core/afxSystem.h"

_AFX afxAllocationBlock* _AfxStackAllocateBlock(afxStack *stck)
{
    afxNat v1 = stck->unitSiz * stck->unitsPerBlock;
    afxAllocationBlock *block = AfxAllocate(1, sizeof(*block), 0, AfxHere());
    block->base = AfxAllocate(1, v1, 0, AfxHere());

    {
        block->usedUnitCnt = 0;
        block->firstIdx = 0;
        block->prev = 0;
    }

    return block;
}

_AFX void *AfxPushStackUnit(afxStack *stck, afxNat *idx)
{
    afxAllocationBlock *v2 = stck->lastBlock;

    if (!v2 || v2->usedUnitCnt == stck->unitsPerBlock)
    {
        if (!stck->blockDir || stck->activeBlocks < stck->maxActiveBlocks)
            v2 = _AfxStackAllocateBlock(stck);

        if (v2)
        {
            v2->firstIdx = stck->totalUsedUnitCnt;
            v2->prev = stck->lastBlock;
            afxAllocationBlock **v3 = stck->blockDir;
            stck->lastBlock = v2;

            if (v3)
                v3[stck->activeBlocks++] = v2;
        }
    }

    afxByte *area = 0;
    *idx = -1;

    if (v2)
    {
        area = &v2->base[v2->usedUnitCnt++ * stck->unitSiz];
        *idx = stck->totalUsedUnitCnt++;
    }
    return area;
}

_AFX char AfxPushStackUnits(afxStack *stck, afxNat cnt, afxNat *firstIdx, void const *initialVal)
{
    afxNat v4 = cnt;

    if (cnt <= 0)
        return 0;

    if (!AfxPushStackUnit(stck, &cnt))
        return 0;

    afxNat v6 = cnt;
    void const *v7 = initialVal;

    if (initialVal)
    {
        void *v8 = AfxGetStackUnit(stck, cnt);
        AfxCopy2(1, stck->unitSiz, v7, v8);
    }

    afxNat v9 = v4 - 1;

    *firstIdx = v6;

    if (!v9)
        return 1;

    while (1)
    {
        --v9;
        afxByte *v10 = AfxPushStackUnit(stck, &cnt);

        if (!v10)
            break;

        AfxCopy2(1, stck->unitSiz, v7, v10);

        if (!v9)
            return 1;
    }
    return 0;
}

_AFX void AfxSerializeStack(afxStack *stck, void *dst)
{
    afxByte *v2 = (afxByte*)dst + stck->unitSiz * stck->unitsPerBlock;

    for (afxAllocationBlock *i = stck->lastBlock; i; i = i->prev)
    {
        afxNat v4 = stck->unitSiz * i->usedUnitCnt;
        afxByte *v5 = i->base;
        v2 -= v4;
        afxByte *v6 = v2;

        if (v4)
        {
            afxNat v7 = stck->unitSiz * i->usedUnitCnt;
            do
            {
                *v6++ = *v5++;
                --v7;
            } while (v7);
        }
    }
}

_AFX void* AfxGetStackUnit(afxStack *stck, afxNat idx)
{
    afxAllocationBlock** v2 = stck->blockDir;
    afxAllocationBlock* v3;

    if (v2)
    {
        v3 = v2[idx / stck->unitsPerBlock];
        return &v3->base[stck->unitSiz * (idx - v3->firstIdx)];
    }

    v3 = stck->lastBlock;

    if (v3->firstIdx <= idx)
        return &v3->base[stck->unitSiz * (idx - v3->firstIdx)];

    do
        v3 = v3->prev;
    while (v3->firstIdx > idx);

    return &v3->base[stck->unitSiz * (idx - v3->firstIdx)];
}

_AFX void AfxDumpStackElement(afxStack *stck, afxNat idx, void *dst)
{
    afxAllocationBlock** v2 = stck->blockDir;
    afxAllocationBlock* v3;

    if (v2)
    {
        v3 = v2[idx / stck->unitsPerBlock];
        AfxCopy2(1, stck->unitSiz, &v3->base[stck->unitSiz * (idx - v3->firstIdx)], dst);
        return;
    }

    v3 = stck->lastBlock;

    if (v3->firstIdx <= idx)
    {
        AfxCopy2(1, stck->unitSiz, &v3->base[stck->unitSiz * (idx - v3->firstIdx)], dst);
        return;
    }

    do v3 = v3->prev;
    while (v3->firstIdx > idx);

    AfxCopy2(1, stck->unitSiz, &v3->base[stck->unitSiz * (idx - v3->firstIdx)], dst);
}

_AFX void AfxDumpStackElements(afxStack *stck, afxNat first, afxNat cnt, void *dst)
{
    afxAllocationBlock** v2 = stck->blockDir;
    afxAllocationBlock* v3;

    if (v2)
    {
        v3 = v2[first / stck->unitsPerBlock];
        AfxCopy2(cnt, stck->unitSiz, &v3->base[stck->unitSiz * (first - v3->firstIdx)], dst);
        return;
    }

    v3 = stck->lastBlock;

    if (v3->firstIdx <= first)
    {
        AfxCopy2(cnt, stck->unitSiz, &v3->base[stck->unitSiz * (first - v3->firstIdx)], dst);
        return;
    }

    do
        v3 = v3->prev;
    while (v3->firstIdx > first);

    AfxCopy2(cnt, stck->unitSiz, &v3->base[stck->unitSiz * (first - v3->firstIdx)], dst);
}

_AFX void AfxUpdateStackElement(afxStack *stck, afxNat idx, void const* src)
{
    afxAllocationBlock** v2 = stck->blockDir;
    afxAllocationBlock* v3;

    if (v2)
    {
        v3 = v2[idx / stck->unitsPerBlock];
        AfxCopy2(1, stck->unitSiz, src, &v3->base[stck->unitSiz * (idx - v3->firstIdx)]);
        return;
    }

    v3 = stck->lastBlock;

    if (v3->firstIdx <= idx)
    {
        AfxCopy2(1, stck->unitSiz, src, &v3->base[stck->unitSiz * (idx - v3->firstIdx)]);
        return;
    }

    do
        v3 = v3->prev;
    while (v3->firstIdx > idx);

    AfxCopy2(1, stck->unitSiz, src, &v3->base[stck->unitSiz * (idx - v3->firstIdx)]);
}

_AFX void AfxUpdateStackElements(afxStack *stck, afxNat first, afxNat cnt, void const* src)
{
    afxAllocationBlock** v2 = stck->blockDir;
    afxAllocationBlock* v3;

    if (v2)
    {
        v3 = v2[first / stck->unitsPerBlock];
        AfxCopy2(cnt, stck->unitSiz, src, &v3->base[stck->unitSiz * (first - v3->firstIdx)]);
        return;
    }

    v3 = stck->lastBlock;

    if (v3->firstIdx <= first)
    {
        AfxCopy2(cnt, stck->unitSiz, src, &v3->base[stck->unitSiz * (first - v3->firstIdx)]);
        return;
    }

    do
        v3 = v3->prev;
    while (v3->firstIdx > first);

    AfxCopy2(cnt, stck->unitSiz, src, &v3->base[stck->unitSiz * (first - v3->firstIdx)]);
}

_AFX void AfxPopStackUnits(afxStack *stck, afxNat cnt)
{
    afxNat v2 = cnt;

    while (stck->lastBlock)
    {
        afxAllocationBlock *v3 = stck->lastBlock;

        if (v3->usedUnitCnt > v2)
            break;

        afxAllocationBlock *v4 = stck->lastBlock;
        stck->totalUsedUnitCnt -= v3->usedUnitCnt;
        v2 -= v3->usedUnitCnt;
        stck->lastBlock = v3->prev;
        AfxDeallocate(v4);
        afxAllocationBlock **v5 = stck->blockDir;

        if (v5)
            v5[stck->activeBlocks-- - 1] = NIL;
    }

    afxAllocationBlock *v6 = stck->lastBlock;

    if (v6)
    {
        stck->totalUsedUnitCnt -= v2;
        v6->usedUnitCnt -= v2;
    }
}

_AFX void AfxEmptyStack(afxStack *stck)
{
    AfxPopStackUnits(stck, stck->totalUsedUnitCnt);

    if (stck->blockDir)
    {
        AfxDeallocate(stck->blockDir);
        stck->blockDir = NIL;
    }
}

_AFX void AfxDeallocateStack(afxStack *stck)
{
    AfxPopStackUnits(stck, stck->totalUsedUnitCnt);

    if (stck->blockDir)
    {
        AfxDeallocate(stck->blockDir);
        stck->blockDir = NIL;
    }
}

_AFX void AfxAllocateStack(afxStack *stck, afxNat unitSiz, afxNat unitsPerBlock)
{
    stck->ctx = NIL;

    stck->unitSiz = unitSiz;
    stck->unitsPerBlock = unitsPerBlock ? unitsPerBlock : AfxGetMemoryPageSize() / unitSiz;
    stck->totalUsedUnitCnt = 0;
    stck->lastBlock = NIL;
    stck->maxUnits = (afxNat)-1;
    stck->activeBlocks = (afxNat)-1;
    stck->maxActiveBlocks = (afxNat)-1;
    stck->blockDir = NIL;
}

_AFX void AfxAllocatePagedStack(afxStack *stck, afxNat unitSiz, afxNat unitsPerBlock, afxNat maxUnits)
{
    stck->ctx = NIL;

    stck->unitSiz = unitSiz;
    stck->maxUnits = maxUnits;
    afxNat v4 = unitsPerBlock ? (unitsPerBlock + maxUnits - 1) / unitsPerBlock : AfxGetMemoryPageSize() / unitSiz;
    stck->unitsPerBlock = unitsPerBlock;
    stck->totalUsedUnitCnt = 0;
    stck->lastBlock = NIL;
    stck->activeBlocks = 0;
    stck->maxActiveBlocks = v4;
    afxAllocationBlock**v5 = AfxAllocate(v4, sizeof(afxAllocationBlock*), 0, AfxHere());
    afxNat v6 = sizeof(afxAllocationBlock*) * stck->maxActiveBlocks;
    stck->blockDir = v5;
    AfxZero2(1, v6, v5);
}
