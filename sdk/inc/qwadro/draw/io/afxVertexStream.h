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

// This section is part of SIGMA GL/2.

#ifndef AFX_DRAW_STREAM_H
#define AFX_DRAW_STREAM_H

#include "qwadro/draw/io/afxBuffer.h"
#include "qwadro/draw/pipe/afxVertexInput.h"

AFX_DECLARE_STRUCT(afxVertexBufferSegment);

#ifdef _AFX_DRAW_C
#ifdef _AFX_VERTEX_BUFFER_C

AFX_DEFINE_STRUCT(afxVertexBufferSegment)
{
    afxVertexBuffer     vbuf; // afxVertexBuffer
    afxNat32            base;
    afxNat32            range;
};

AFX_DEFINE_STRUCT(afxVertexBufferRoom)
{
    afxLinkage          vbuf;
    afxNat32            base;
    afxNat32            range;
};

AFX_OBJECT(afxVertexBuffer)
{
    afxChain            rooms; /// spaces that can be occupied.
    afxSize             freeSiz;
    afxBufferAccess     access;
    afxSize             bufSiz;
    afxBuffer           buf;    
};

#endif//_AFX_VERTEX_BUFFER_C
#endif//_AFX_DRAW_C

AFX_DEFINE_STRUCT(afxVertexBufferSpecification)
{
    afxNat              bufCap;
    afxBufferAccess     access;
};

AVX afxError        AfxAcquireVertexBuffers(afxDrawInput din, afxNat cnt, afxVertexBufferSpecification const spec[], afxVertexBuffer vbuf[]);

AVX afxBuffer       AfxGetVertexBufferStorage(afxVertexBuffer vbuf);
AVX afxNat          AfxGetVertexBufferUsage(afxVertexBuffer vbuf);
AVX afxVertexInput  AfxGetVertexBufferLayout(afxVertexBuffer vbuf);
AVX afxNat          AfxGetVertexBufferCapacity(afxVertexBuffer vbuf);

AFX_DECLARE_STRUCT(awxVertexDataCache);

#ifdef _AFX_DRAW_C
#ifdef _AFX_INDEX_BUFFER_C

AFX_DEFINE_STRUCT(afxIndexBufferSegment)
{
    afxIndexBuffer      ibuf; // afxIndexBuffer
    afxNat              baseIdx;
    afxNat              idxCnt;
};

AFX_DEFINE_STRUCT(afxIndexBufferRoom)
{
    afxLinkage          ibuf;
    afxNat              baseIdx;
    afxNat              idxCnt;
};

AFX_OBJECT(afxIndexBuffer)
{
    afxChain            rooms; /// spaces that can be occupied.
    afxNat              bookedIdxCnt;
    afxBufferAccess     access;
    afxNat              idxCap;
    afxBuffer           buf;
    afxNat32            idxSiz;
};

#endif//_AFX_INDEX_BUFFER_C
#endif//_AFX_DRAW_C


//

AFX_DEFINE_STRUCT(afxIndexBufferSpecification)
{
    afxNat              idxCap;
    afxBufferAccess     access;
    afxNat32            idxSiz;
};

AVX afxError        AfxAcquireIndexBuffers(afxDrawContext dctx, afxNat cnt, afxIndexBufferSpecification const spec[], afxIndexBuffer ibuf[]);

AVX afxBuffer       AfxGetIndexBuffer(afxIndexBuffer ibuf);
AVX afxNat          AfxGetIndexBufferUsage(afxIndexBuffer ibuf);
AVX afxNat32        AfxGetIndexBufferStride(afxIndexBuffer ibuf);
AVX afxNat          AfxGetIndexBufferCapacity(afxIndexBuffer ibuf);

AVX afxNat          AfxReserveIndexBufferSegment(afxIndexBuffer ibuf, afxNat idxCnt, afxNat* baseIdx);
AVX void            AfxDisposeIndexBufferSegment(afxIndexBuffer ibuf, afxNat baseIdx, afxNat idxCnt);

#endif//AFX_DRAW_STREAM_H
