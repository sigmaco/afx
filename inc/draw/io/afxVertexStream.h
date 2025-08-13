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

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#ifndef AVX_BUFFERIZER_H
#define AVX_BUFFERIZER_H

#include "qwadro/inc/draw/io/avxBuffer.h"
#include "qwadro/inc/draw/op/avxVertexInput.h"

AFX_DEFINE_HANDLE(avxBufferizer);

#ifdef _AVX_DRAW_C
#ifdef _AVX_VERTEX_BUFFER_C

#endif//_AVX_VERTEX_BUFFER_C
#endif//_AVX_DRAW_C

AFX_DEFINE_STRUCT(avxBufferizerInfo)
{
    afxUnit         bufCap;
    avxBufferFlags  access;
    avxBufferUsage  usage;
};

AVX afxError        AfxAcquireBufferizer(afxDrawInput din, avxBufferizerInfo const* info, avxBufferizer* bufferizer);

AVX avxBuffer       AfxGetVertexBufferStorage(afxVertexBuffer vbuf);
AVX afxUnit         AfxGetVertexBufferUsage(afxVertexBuffer vbuf);
AVX avxVertexInput  AfxGetVertexBufferLayout(afxVertexBuffer vbuf);
AVX afxUnit         AfxGetVertexBufferCapacity(afxVertexBuffer vbuf);

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

typedef struct
{
    avxBuffer buffer;
    void* mapped_ptr;
    afxSize capacity;
    afxSize used;
    int frame_in_use; // When it was last used
} _avxFrameBufferizerChunk;

typedef struct {
    _avxFrameBufferizerChunk* last;
    _avxFrameBufferizerChunk* chunks;
    afxSize num_chunks;
    afxSize capacity_chunks;
    int current_frame;
    
    afxUnit rounds;
    afxUnit blockAlign;
    afxUnit minChunkSiz;
    avxBufferFlags bufFlags;
    avxBufferUsage bufUsage;
    afxDrawSystem dsys;
} avxBufferedPump;

AVX void AvxDeployBufferedPump(avxBufferedPump* alloc);
AVX void* AvxRequestBufferedPump(avxBufferedPump* alloc, afxSize size, avxBuffer* out_buffer, afxSize* out_offset);
AVX void AvxAdvanceBufferedPump(avxBufferedPump* alloc);
AVX void AvxDismantleBufferedPump(avxBufferedPump* alloc);

#endif//AVX_BUFFERIZER_H
