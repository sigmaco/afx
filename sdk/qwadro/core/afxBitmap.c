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

#include "qwadro/core/afxBitmap.h"

_AFX afxError AfxSetBit(afxBitmap* bmp, afxNat bit)
{
    afxError err = NIL;
    AfxAssertType(bmp, afxFcc_BMP);

    // 4 (x32) set  --- 128 bits per page
    // 2 // page

    afxBitmapPage *pag;
    afxNat pageIdx = bit / bmp->setsPerPage;

    //2
    //3

    if (0 == bmp->pageCnt || (pageIdx >= bmp->pageCnt))
    {
        afxNat diff = (pageIdx + 1) - bmp->pageCnt;
        void *pgs;

        if (!(pgs = AfxReallocate(bmp->mem, bmp->pages, sizeof(*pag), (bmp->pageCnt + diff), 0, AfxHint()))) AfxThrowError();
        else
        {
            bmp->pages = pgs;

            for (afxNat i = 0; i < diff; i++)
            {
                pag = &bmp->pages[bmp->pageCnt + i];
                pag->sets = NIL;
                pag->markedBitCnt = 0;
            }
            bmp->pageCnt += diff;
        }
    }

    if (!err)
    {
        AfxAssert(bmp->pages);
        pag = &bmp->pages[pageIdx];

        if (!pag->sets)
        {
            if (!(pag->sets = AfxAllocate(bmp->mem, bmp->setsPerPage, sizeof(afxNat32), 0, AfxHint()))) AfxThrowError();
            else
                AfxZero(bmp->setsPerPage, sizeof(afxNat32), pag->sets);
        }

        if (!err)
        {
            afxNat32 mask = AfxGetBitOffset(bit % 32);
            afxNat32* set = &pag->sets[bit % bmp->setsPerPage];
            afxBool inc = !(AfxTestBitPosition(*set, bit % 32));
            *set |= inc;

            if (inc)
            {
                ++pag->markedBitCnt;
                ++bmp->totalMarkedBitCnt;
            }
        }
    }

    AfxAssert(AfxTestBit(bmp, bit));
    return err;
}

_AFX void AfxClearBit(afxBitmap* bmp, afxNat bit)
{
    afxError err = NIL;
    AfxAssertType(bmp, afxFcc_BMP);

    // 4 (x32) set  --- 128 bits per page
    // 2 // page

    afxNat pageIdx = bit / bmp->setsPerPage;

    if (bmp->pageCnt > pageIdx)
    {
        AfxAssert(bmp->pages);
        afxBitmapPage *pag = &bmp->pages[pageIdx];

        if (pag->sets)
        {
            afxNat32 mask = AfxGetBitOffset(bit % 32);
            afxNat32* set = &pag->sets[bit % bmp->setsPerPage];
            afxBool dec = AfxTestBitPosition(*set, bit % 32);
            *set &= ~mask;

            if (dec)
            {
                --pag->markedBitCnt;
                --bmp->totalMarkedBitCnt;
            }
        }

        if (0 == pag->markedBitCnt)
        {
            AfxDeallocate(bmp->mem, pag->sets);
            pag->sets = NIL;

            if (pageIdx == bmp->pageCnt - 1) // if is the last page
            {
                if (pageIdx) // if exist other ones
                {
                    void *pgs;

                    if (!(pgs = AfxReallocate(bmp->mem, bmp->pages, sizeof(*pag), (bmp->pageCnt - 1), 0, AfxHint()))) AfxThrowError();
                    else
                    {
                        bmp->pages = pgs;
                        --bmp->pageCnt;
                    }
                }
                else
                {
                    AfxDeallocate(bmp->mem, bmp->pages);
                    bmp->pages = NIL;
                    --bmp->pageCnt;
                }
            }
        }
    }

    AfxAssert(!AfxTestBit(bmp, bit));
}

_AFX afxResult AfxTestBit(afxBitmap const* bmp, afxNat bit)
{
    afxError err = NIL;
    AfxAssertType(bmp, afxFcc_BMP);
    afxResult rslt = 0;

    // 4 (x32) set  --- 128 bits per page
    // 2 // page

    afxNat pageIdx = bit / bmp->setsPerPage;

    if (bmp->pageCnt > pageIdx)
    {
        AfxAssert(bmp->pages);
        afxBitmapPage *pag = &bmp->pages[pageIdx];

        if (pag->sets)
        {
            afxNat32 mask = AfxGetBitOffset(bit % 32);
            afxNat32 const* set = &pag->sets[bit % bmp->setsPerPage];
            rslt = AfxTestBitPosition(*set, bit % 32); // (((*set) >> (31 - (bit % 32))) & 0x1);// ((*set) & mask);
        }
    }
    return rslt;
}

afxNat const masks[] =
{
    AfxGetBitOffset(0),
    AfxGetBitOffset(1),
    AfxGetBitOffset(2),
    AfxGetBitOffset(3),
    AfxGetBitOffset(4),
    AfxGetBitOffset(5),
    AfxGetBitOffset(6),
    AfxGetBitOffset(7),
    AfxGetBitOffset(8),
    AfxGetBitOffset(9),
    AfxGetBitOffset(10),
    AfxGetBitOffset(11),
    AfxGetBitOffset(12),
    AfxGetBitOffset(13),
    AfxGetBitOffset(14),
    AfxGetBitOffset(15),
    AfxGetBitOffset(16),
    AfxGetBitOffset(17),
    AfxGetBitOffset(18),
    AfxGetBitOffset(19),
    AfxGetBitOffset(20),
    AfxGetBitOffset(21),
    AfxGetBitOffset(22),
    AfxGetBitOffset(23),
    AfxGetBitOffset(24),
    AfxGetBitOffset(25),
    AfxGetBitOffset(26),
    AfxGetBitOffset(27),
    AfxGetBitOffset(28),
    AfxGetBitOffset(29),
    AfxGetBitOffset(30),
    AfxGetBitOffset(31),
};

_AFX afxError AfxPullBit(afxBitmap* bmp, afxNat* bit)
{
    afxError err = NIL;
    AfxAssertType(bmp, afxFcc_BMP);
    AfxAssert(bit);
    
    afxNat idx;
    afxNat pageIdx;
    afxBitmapPage *pag;
    
    for (pageIdx = 0; pageIdx < bmp->pageCnt; pageIdx++)
    {
        pag = &bmp->pages[pageIdx];
        AfxAssert(pag->markedBitCnt <= bmp->setsPerPage * 32);
        
        if (pag->markedBitCnt < bmp->setsPerPage * 32)
        {
            if (pag->markedBitCnt == 0) idx = (pageIdx * bmp->setsPerPage);
            else
            {
                for (afxNat j = 0; j < bmp->setsPerPage; j++)
                {
                    afxNat32 const set = pag->sets[j];

                    if (set == NIL) idx = (pageIdx * bmp->setsPerPage) + (j * 32);
                    else if (set != AFX_N32_MAX)
                    {
                        for (afxNat k = 0; k < 32; k++)
                        {
                            //if (!((set >> (31 - k)) & 0x1))
                            if (!AfxTestBitEnabled(set, 1<<k))
                            {
                                idx = (pageIdx * bmp->setsPerPage) + (j * 32) + k;
                                AfxAssert(!AfxTestBit(bmp, idx));

                                if (AfxSetBit(bmp, idx)) AfxThrowError();
                                else
                                {
                                    AfxAssert(AfxTestBit(bmp, idx));
                                    AfxAssert(bmp->totalMarkedBitCnt);
                                    AfxAssert(pag->markedBitCnt);
                                    *bit = idx;
                                    return err;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    if (bmp->pageCnt == 0)
    {
        // if no room, generate a new page.
        pageIdx = bmp->pageCnt;
        idx = (pageIdx * bmp->setsPerPage);
    }

    if (AfxSetBit(bmp, idx)) AfxThrowError();
    else
    {
        AfxAssert(AfxTestBit(bmp, idx));
        *bit = idx;
    }
    return err;
}

_AFX void AfxAcquireBitmap(afxBitmap* bmp, afxNat setsPerPage)
{
    afxError err = NIL;
    AfxAssert(bmp);
    AfxAssignTypeFcc(bmp, afxFcc_BMP);
    bmp->setsPerPage = AfxMax(setsPerPage, 1);
    bmp->totalMarkedBitCnt = 0;
    bmp->pageCnt = 0;
    bmp->pages = NIL;
    bmp->mem = NIL;
}

_AFX void AfxReleaseBitmap(afxBitmap* bmp)
{
    afxError err = NIL;
    AfxAssertType(bmp, afxFcc_BMP);
    
    for (afxNat i = 0; i < bmp->pageCnt; i++)
    {
        if (bmp->pages[i].sets)
            AfxDeallocate(bmp->mem, bmp->pages[i].sets);
    }
    
    if (bmp->pages)
    {
        AfxDeallocate(bmp->mem, bmp->pages);
        bmp->pages = NIL;
    }

    AfxZero(1, sizeof(*bmp), bmp);
}

_AFX afxError AfxTestBitmaps(void)
{
    afxError err = NIL;

    afxBitmap bmp;
    AfxAcquireBitmap(&bmp, 1024);
    AfxSetBit(&bmp, 1);
    AfxAssert(AfxTestBit(&bmp, 1));
    AfxSetBit(&bmp, 2);
    AfxAssert(AfxTestBit(&bmp, 2));
    AfxClearBit(&bmp, 2);
    AfxAssert(!AfxTestBit(&bmp, 2));
    AfxSetBit(&bmp, 3);
    AfxSetBit(&bmp, 30);
    AfxSetBit(&bmp, 33);
    AfxAssert(AfxTestBit(&bmp, 33));
    AfxSetBit(&bmp, 34);
    AfxClearBit(&bmp, 34);
    AfxAssert(!AfxTestBit(&bmp, 34));
    AfxAssert(!AfxTestBit(&bmp, 32));
    AfxSetBit(&bmp, 66);
    AfxAssert(AfxTestBit(&bmp, 66));
    AfxAssert(!AfxTestBit(&bmp, 67));
    AfxAssert(!AfxTestBit(&bmp, 65));
    afxResult testBit = AfxTestBit(&bmp, 30);
    afxNat bit = 0;
    AfxPullBit(&bmp, &bit);
    afxNat bit2 = 0;
    AfxPullBit(&bmp, &bit2);
    afxNat bit3 = 0;
    AfxPullBit(&bmp, &bit3);
    afxNat bit4 = 0;
    AfxPullBit(&bmp, &bit4);
    afxNat bit5 = 0;
    AfxPullBit(&bmp, &bit5);
    afxNat bit6 = 0;
    AfxPullBit(&bmp, &bit6);
    afxNat bit7 = 0;
    AfxPullBit(&bmp, &bit7);

    AfxReleaseBitmap(&bmp);


    //afxBitmap bmp;
    AfxAcquireBitmap(&bmp, 1024);
    AfxSetBit(&bmp, 1);
    //afxNat bit7 = 0;

    for (afxNat i = 0; i < 3000; i++)
        AfxSetBit(&bmp, i);

    afxNat j = 3000;
    for (afxNat i = 0; i < 3000; i++)
        AfxClearBit(&bmp, --j);

    for (afxNat i = 0; i < 3000; i++)
        AfxPullBit(&bmp, &bit7);

    for (afxNat i = 0; i < 3000; i++)
        AfxClearBit(&bmp, i);

    AfxReleaseBitmap(&bmp);

    return err;
}
