/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                      Q W A D R O   D R A W   I / O   S Y S T E M
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

  //////////////////////////////////////////////////////////////////////////////
 // The Qwadro Video Graphics Infrastructure                                 //
//////////////////////////////////////////////////////////////////////////////

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>
// This software is part of Advanced Video Graphics Extensions & Experiments.

#ifndef AVX_BUFFER_IMPL_H
#define AVX_BUFFER_IMPL_H

#include "avxStorageImpl.h"

AFX_DECLARE_STRUCT(_avxDdiBuf);
AFX_DECLARE_STRUCT(_avxIddBuf);

#ifdef _AVX_BUFFER_C
#ifdef _AVX_BUFFER_IMPL
AFX_OBJECT(_avxBuffer)
#else
AFX_OBJECT(avxBuffer)
#endif
{
    _avxIddBuf*         idd;
    _avxDdiBuf const*   ddi;
    // User-defined data.
    void*               udd;
    // Debugging tag.
    afxString           tag;

    avxBuffer           base; // handle used by subacquision of another buffer
    afxSize             from; // offset used by subacquision of another buffer
    //afxUnit         size; // capacity
    avxBufferUsage      usage;
    avxBufferFlags      flags;
    avxFormat           fmt; // for tensor buffer

    afxMask             sharingMask;

    // STORAGE
    // required size of this storage block.
    afxSize             reqSiz;
    afxUnit             reqAlign;
    // required memory conditions for this storage block.
    afxFlags            reqMemType;
    _avxBufStorage      storage[1]; // non-sparse
    afxSize             storageOffset;
};
#endif//_AVX_BUFFER_C

AVX afxClassConfig const _AVX_BUF_CLASS_CONFIG;

AVXINL void _AvxSanitizeBufferCopy(avxBuffer buf, avxBuffer src, afxUnit cnt, avxBufferCopy const raw[], avxBufferCopy san[]);
AVXINL void _AvxSanitizeBufferIo(avxBuffer buf, avxBuffer src, afxUnit cnt, avxBufferIo const raw[], avxBufferIo san[]);

#endif//AVX_BUFFER_IMPL_H
