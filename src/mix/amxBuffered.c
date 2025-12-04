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

// This software is part of Advanced Multimedia Extensions & Experiments.

#define _AMX_MIX_C
#define _AMX_BUFFER_C
#include "amxIcd.h"

_AMX void AmxMakeBufferedRing(amxBufferedRing* rng, afxUnit rounds, afxUnit blockSiz, afxUnit blockAlign, amxBuffer buf, afxSize bufCap, void* mapped)
{
    // Small buffered ring utility.
    // Reuse memory across frames (no new buffer creation).
    // Efficient for dynamic scenes and many objects.
    // Prevents CPU-GPU sync stalls and fragmentation.

    afxError err;
    AFX_ASSERT(rng);
    *rng = (amxBufferedRing) { 0 };

    // Use triple buffering (rounds = 3) to rotate through buffer regions.
    rng->rounds = AFX_MAX(1, rounds);
    rng->blockAlign = AFX_ALIGN_SIZE(AFX_MIN(AMX_BUFFER_ALIGNMENT, blockAlign), AMX_BUFFER_ALIGNMENT);
    rng->blockSiz = AFX_MAX(rng->blockAlign, AMX_ALIGN_BUFFERED(blockSiz));

    AFX_ASSERT_OBJECTS(afxFcc_MBUF, 1, &buf);
    rng->buf = buf;
    rng->maxSiz = bufCap;
    rng->blockCnt = rng->maxSiz / rng->blockSiz;
    rng->currOffset = 0;

    rng->basePtr = NIL;
    if (mapped) rng->basePtr = mapped;
    else if (AmxMapBuffer(buf, 0, bufCap, NIL, (void**)&rng->basePtr))
        AfxThrowError();
}

_AMX afxSize AmxCycleBufferedRing(amxBufferedRing* rng)
{
    afxError err;
    AFX_ASSERT(rng);
    rng->currOffset = (rng->currOffset + rng->blockSiz * rng->blockCnt / rng->rounds) % rng->maxSiz;
    return rng->currOffset;
}

_AMX void* AmxAdvanceBufferedRing(amxBufferedRing* rng, afxUnit reqSiz, afxSize* pOffset, afxUnit* pRange)
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

