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
 // MIX MEMORY BUFFERIZATION                                                 //
//////////////////////////////////////////////////////////////////////////////

// This software is part of Advanced Multimedia Extensions & Experiments.

#ifndef AMX_BUFFERED_H
#define AMX_BUFFERED_H

#include "qwadro/mix/amxBuffer.h"

AFX_DEFINE_STRUCT(amxBufferedRing)
{
    afxUnit     rounds;
    afxSize     blockSiz;
    afxUnit     blockAlign;
    amxBuffer   buf;
    afxSize     maxSiz;
    afxUnit     blockCnt;
    afxByte*    basePtr;
    afxSize     currOffset;
};

AMX void    AmxMakeBufferedRing(amxBufferedRing* rng, afxUnit rounds, afxUnit blockSiz, afxUnit blockAlign, amxBuffer buf, afxSize bufCap, void* mapped);
AMX afxSize AmxCycleBufferedRing(amxBufferedRing* rng);
AMX void*   AmxAdvanceBufferedRing(amxBufferedRing* rng, afxUnit reqSiz, afxSize* pOffset, afxUnit* pRange);

#endif//AMX_BUFFERED_H
