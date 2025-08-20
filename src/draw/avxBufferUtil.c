/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *        Q W A D R O   V I D E O   G R A P H I C S   I N F R A S T R U C T U R E
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#include "ddi/avxImplementation.h"

_AVX void AvxMakeBufferedRing(avxBufferedRing* rng, afxUnit rounds, afxUnit blockSiz, afxUnit blockAlign, avxBuffer buf, afxSize cap, void* mapped)
{
    // Small buffered ring utility.
    // Reuse memory across frames (no new buffer creation).
    // Efficient for dynamic scenes and many objects.
    // Prevents CPU-GPU sync stalls and fragmentation.

    afxError err;
    AFX_ASSERT(rng);
    *rng = (avxBufferedRing) { 0 };

    // Use triple buffering (rounds = 3) to rotate through buffer regions.
    rng->rounds = AFX_MAX(1, rounds);
    rng->blockAlign = AFX_ALIGN_SIZE(AFX_MIN(AVX_BUFFER_ALIGNMENT, blockAlign), AVX_BUFFER_ALIGNMENT);
    rng->blockSiz = AFX_MAX(rng->blockAlign, AVX_ALIGN_BUFFERED(blockSiz));

    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);
    rng->buf = buf;
    rng->maxSiz = cap;
    rng->blockCnt = rng->maxSiz / rng->blockSiz;
    rng->currOffset = 0;

    // With persistent and coherent mapping, you don't need glFenceSync() unless extremely fine-grained sync is required.
    rng->basePtr = NIL;
    if (mapped) rng->basePtr = mapped;
    else if (AvxMapBuffer(buf, 0, cap, NIL, (void**)&rng->basePtr))
        AfxThrowError();
}

_AVX afxSize AvxCycleBufferedRing(avxBufferedRing* rng)
{
    afxError err;
    AFX_ASSERT(rng);
    rng->currOffset = (rng->currOffset + rng->blockSiz * rng->blockCnt / rng->rounds) % rng->maxSiz;
    return rng->currOffset;
}

_AVX void* AvxAdvanceBufferedRing(avxBufferedRing* rng, afxUnit reqSiz, afxSize* pOffset, afxUnit* pRange)
{
    afxError err;
    AFX_ASSERT(rng);

    reqSiz = AFX_ALIGN_SIZE(reqSiz, rng->blockSiz);

    if (rng->currOffset + reqSiz > rng->maxSiz)
    {
        // Wrap around
        rng->currOffset = 0;
    }

    void* blockPtr = &rng->basePtr[rng->currOffset];

    // Bind for shader access
    // You now write directly into blockPtr and the shader sees the bound block via layout(std140, binding = %).
    if (pOffset) *pOffset = rng->currOffset;
    if (pRange) *pRange = reqSiz;

    rng->currOffset += reqSiz;

    return blockPtr;
}

