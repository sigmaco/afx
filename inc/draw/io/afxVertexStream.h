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

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#ifndef AVX_VERTEX_BUFFER_H
#define AVX_VERTEX_BUFFER_H

#include "qwadro/inc/draw/io/afxBuffer.h"
#include "qwadro/inc/draw/pipe/avxVertexInput.h"

AFX_DEFINE_HANDLE(avxVertexBufferizer);

#ifdef _AVX_DRAW_C
#ifdef _AVX_VERTEX_BUFFER_C

#endif//_AVX_VERTEX_BUFFER_C
#endif//_AVX_DRAW_C

AFX_DEFINE_STRUCT(afxVertexBufferSpecification)
{
    afxNat              bufCap;
    afxBufferFlags     access;
};

AVX afxError        AfxAcquireVertexBuffers(afxDrawInput din, afxNat cnt, afxVertexBufferSpecification const spec[], afxVertexBuffer vbuf[]);

AVX afxBuffer       AfxGetVertexBufferStorage(afxVertexBuffer vbuf);
AVX afxNat          AfxGetVertexBufferUsage(afxVertexBuffer vbuf);
AVX avxVertexInput  AfxGetVertexBufferLayout(afxVertexBuffer vbuf);
AVX afxNat          AfxGetVertexBufferCapacity(afxVertexBuffer vbuf);

//AFX_DECLARE_STRUCT(akxVertexCache);

#endif//AVX_VERTEX_BUFFER_H
