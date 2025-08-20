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

#ifndef AVX_BUFFER_UTIL_H
#define AVX_BUFFER_UTIL_H

#include "qwadro/inc/draw/avxBuffer.h"

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

AVX void    AvxMakeBufferedRing(avxBufferedRing* rng, afxUnit rounds, afxUnit blockSiz, afxUnit blockAlign, avxBuffer buf, afxSize cap, void* mapped);
AVX afxSize AvxCycleBufferedRing(avxBufferedRing* rng);
AVX void*   AvxAdvanceBufferedRing(avxBufferedRing* rng, afxUnit reqSiz, afxSize* pOffset, afxUnit* pRange);

#endif//AVX_BUFFER_UTIL_H
