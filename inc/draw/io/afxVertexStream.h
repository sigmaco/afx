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

#include "qwadro/inc/draw/afxBuffer.h"
#include "qwadro/inc/draw/pipe/avxVertexInput.h"

AFX_DECLARE_STRUCT(afxVertexBufferSegment);

#ifdef _AVX_DRAW_C
#ifdef _AVX_VERTEX_BUFFER_C

AFX_DEFINE_STRUCT(afxVertexBufferSegment)
{
    afxVertexBuffer     vbuf; // afxVertexBuffer
    afxNat32            base;
    afxNat32            range;
};

AFX_DEFINE_STRUCT(afxVertexStreamB)
{
    afxLinkage          din;
    afxBuffer           buf;
    afxBufferAccess     bufAcc;
    afxSize             bufSiz;
    struct
    {
        afxNat32        base;
        afxNat32        range;
        afxNat32        stride;
    }                   src[2];
    /*
    afxNat              attrCnt;
    struct
    {
        afxNat          srcIdx;
        afxNat32        offset;
        afxVertexFormat fmt;
    }                   attrs[];
    */
};

AFX_OBJECT(afxVertexBuffer)
{
    afxBuffer           buf;
    afxBufferAccess     access;
    afxSize             bufSiz;
    afxSize             freeSiz;
    afxChain            rooms; /// spaces that can be occupied.
};

#endif//_AVX_VERTEX_BUFFER_C
#endif//_AVX_DRAW_C

AFX_DEFINE_STRUCT(afxVertexBufferSpecification)
{
    afxNat              bufCap;
    afxBufferAccess     access;
};

AVX afxError        AfxAcquireVertexBuffers(afxDrawInput din, afxNat cnt, afxVertexBufferSpecification const spec[], afxVertexBuffer vbuf[]);

AVX afxBuffer       AfxGetVertexBufferStorage(afxVertexBuffer vbuf);
AVX afxNat          AfxGetVertexBufferUsage(afxVertexBuffer vbuf);
AVX avxVertexInput  AfxGetVertexBufferLayout(afxVertexBuffer vbuf);
AVX afxNat          AfxGetVertexBufferCapacity(afxVertexBuffer vbuf);

//AFX_DECLARE_STRUCT(akxVertexCache);

#ifdef _AVX_DRAW_C
#ifdef _AVX_INDEX_BUFFER_C

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

#endif//_AVX_INDEX_BUFFER_C
#endif//_AVX_DRAW_C


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

#endif//AVX_VERTEX_BUFFER_H
