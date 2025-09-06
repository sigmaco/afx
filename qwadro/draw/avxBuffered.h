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

  //////////////////////////////////////////////////////////////////////////////
 // QWADRO VIDEO MEMORY BUFFER                                               //
//////////////////////////////////////////////////////////////////////////////

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>
// This software is part of Advanced Video Graphics Extensions & Experiments.

#ifndef AVX_BUFFERED_H
#define AVX_BUFFERED_H

#include "qwadro/draw/avxBuffer.h"

AFX_DEFINE_STRUCT(avxBufferedRing)
{
    afxUnit     rounds;
    afxSize     blockSiz;
    afxUnit     blockAlign;
    avxBuffer   buf;
    afxSize     maxSiz;
    afxUnit     blockCnt;
    afxByte*    basePtr;
    afxSize     currOffset;
};

AVX void    AvxMakeBufferedRing(avxBufferedRing* rng, afxUnit rounds, afxUnit blockSiz, afxUnit blockAlign, avxBuffer buf, afxSize bufCap, void* mapped);
AVX afxSize AvxCycleBufferedRing(avxBufferedRing* rng);
AVX void*   AvxAdvanceBufferedRing(avxBufferedRing* rng, afxUnit reqSiz, afxSize* pOffset, afxUnit* pRange);

#endif//AVX_BUFFERED_H
