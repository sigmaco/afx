/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *            Q W A D R O   M U L T I M E D I A   I N F R A S T R U C T U R E
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This software is part of Advanced Multimedia Extensions & Experiments.

#ifndef AMX_BUFFER_DDK_H
#define AMX_BUFFER_DDK_H

#include "amxStorageDDK.h"

#ifdef _AMX_BUFFER_C
#ifdef _AMX_BUFFER_IMPL
AFX_OBJECT(_amxBuffer)
#else
AFX_OBJECT(amxBuffer)
#endif
{
    afxString       tag;
    void*           udd;
    amxBuffer       base; // handle used by subacquision of another buffer
    afxSize         from; // offset used by subacquision of another buffer
    //afxUnit         size; // capacity
    amxBufferUsage  usage;
    amxBufferFlags  flags;
    amxFormat       fmt; // for tensor buffer
    afxUnit         fmtBps;
    afxUnit         fmtStride;

    afxMask         sharingMask;

    // STORAGE
    // required size of this storage block.
    afxSize         reqSiz;
    afxUnit         reqAlign;
    // required memory conditions for this storage block.
    afxFlags        reqMemType;
    _amxBufStorage  storage[1]; // non-sparse
};
#endif

AMX afxClassConfig const _AMX_MBUF_CLASS_CONFIG;

#endif//AMX_BUFFER_DDK_H
