/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *          Q W A D R O   4 D   R E N D E R I N G   I N F R A S T R U C T U R E
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>
// This file is part of Advanced Renderware Extensions & Experiments for Qwadro.

#ifndef ARX_BUFFERIZER_H
#define ARX_BUFFERIZER_H

#include "qwadro/draw/avxBuffer.h"
#include "qwadro/draw/avxVertexInput.h"
#include "qwadro/render/arxRenderable.h"

AFX_DEFINE_HANDLE(arxBufferizer);

#ifdef _ARX_DRAW_C
#ifdef _ARX_VERTEX_BUFFER_C

#endif//_ARX_VERTEX_BUFFER_C
#endif//_ARX_DRAW_C

AFX_DEFINE_STRUCT(arxBufferizerInfo)
{
    afxUnit         bufCap;
    avxBufferFlags  access;
    avxBufferUsage  usage;
};

ARX afxError        ArxAcquireBufferizer(arxScenario scio, arxBufferizerInfo const* info, arxBufferizer* bufferizer);

ARX avxBuffer       ArxGetVertexBufferStorage(afxVertexBuffer vbuf);
ARX afxUnit         ArxGetVertexBufferUsage(afxVertexBuffer vbuf);
ARX avxVertexInput  ArxGetVertexBufferLayout(afxVertexBuffer vbuf);
ARX afxUnit         ArxGetVertexBufferCapacity(afxVertexBuffer vbuf);

typedef struct
{
    avxBuffer buffer;
    void* mapped_ptr;
    afxSize capacity;
    afxSize used;
    int frame_in_use; // When it was last used
} _arxFrameBufferizerChunk;

typedef struct {
    _arxFrameBufferizerChunk* last;
    _arxFrameBufferizerChunk* chunks;
    afxSize num_chunks;
    afxSize capacity_chunks;
    int current_frame;
    
    afxUnit rounds;
    afxUnit blockAlign;
    afxUnit minChunkSiz;
    avxBufferFlags bufFlags;
    avxBufferUsage bufUsage;
    afxDrawSystem dsys;
} arxBufferedPump;

ARX void ArxDeployBufferedPump(arxBufferedPump* alloc);
ARX void* ArxRequestBufferedPump(arxBufferedPump* alloc, afxSize size, avxBuffer* out_buffer, afxSize* out_offset);
ARX void ArxAdvanceBufferedPump(arxBufferedPump* alloc);
ARX void ArxDismantleBufferedPump(arxBufferedPump* alloc);

#endif//ARX_BUFFERIZER_H
