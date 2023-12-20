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
 *                   (c) 2017 SIGMA Technology Group — Federação SIGMA
 *                                    www.sigmaco.org
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

AFX_DEFINE_STRUCT(afxVertexBufferRoom2)
{
    afxBool             free;
    afxNat32            base;
    afxNat32            range;
    afxVertexInput      layout;
    afxNat              streamIdx;
};

AFX_OBJECT(afxVertexBuffer)
{
    afxChain            rooms; /// spaces that can be occupied.
    afxSize             bookedSiz;
    afxBufferAccess     access;
    afxSize             bufSiz;
    afxNat              vtxSiz;
    afxNat              alignedVtxSiz;
    afxBuffer           buf;
    afxVertexInput      layout;
    afxNat              baseStream;
    afxNat              streamCnt;

    struct
    {
        // each page administrates a buffer
        afxBufferAccess bufAccess;
        afxSize         bufCap;
        afxBuffer       buf;
        afxPool         segments;
    }**outro;
    afxNat outroCnt;

    struct
    {
        afxVertexInput  layout;
        afxNat          baseStream;
        afxNat          streamCnt;
        afxNat32        stride;

        afxNat          vtxSiz;
        afxNat          vtxSizAligned;

        afxBufferAccess bufAccess;
        afxSize         bufSiz;
        afxBuffer       buf;

        afxSize         freeSiz;
        afxChain        rooms; /// spaces that can be occupied.
    }**                 pages;
    afxNat              pageCnt;

    //afxPool pool;
};

#endif//_AFX_VERTEX_BUFFER_C
#endif//_AFX_DRAW_C

AFX_DEFINE_STRUCT(afxVertexBufferSpecification)
{
    afxNat              vtxCap;
    afxBufferAccess     access;
    afxVertexInput      layout;
    afxNat              baseStream;
    afxNat              streamCnt;
};

AFX afxError        AfxAcquireVertexBuffers(afxDrawContext dctx, afxNat cnt, afxVertexBufferSpecification const spec[], afxVertexBuffer vbuf[]);

AFX afxBuffer       AfxGetVertexBufferStorage(afxVertexBuffer vbuf);
AFX afxNat          AfxGetVertexBufferUsage(afxVertexBuffer vbuf);
AFX afxVertexInput  AfxGetVertexBufferLayout(afxVertexBuffer vbuf);
AFX afxNat          AfxGetVertexBufferCapacity(afxVertexBuffer vbuf);

AFX afxNat          AfxRequestVertexBufferSegment(afxVertexBuffer vbuf, afxNat vtxCnt, afxNat* baseVtx);
AFX void            AfxDiscardVertexBufferSegment(afxVertexBuffer vbuf, afxNat baseVtx, afxNat vtxCnt);

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

AFX afxError        AfxAcquireIndexBuffers(afxDrawContext dctx, afxNat cnt, afxIndexBufferSpecification const spec[], afxIndexBuffer ibuf[]);

AFX afxBuffer       AfxGetIndexBuffer(afxIndexBuffer ibuf);
AFX afxNat          AfxGetIndexBufferUsage(afxIndexBuffer ibuf);
AFX afxNat32        AfxGetIndexBufferStride(afxIndexBuffer ibuf);
AFX afxNat          AfxGetIndexBufferCapacity(afxIndexBuffer ibuf);

AFX afxNat          AfxReserveIndexBufferSegment(afxIndexBuffer ibuf, afxNat idxCnt, afxNat* baseIdx);
AFX void            AfxDisposeIndexBufferSegment(afxIndexBuffer ibuf, afxNat baseIdx, afxNat idxCnt);

#endif//AFX_DRAW_STREAM_H
