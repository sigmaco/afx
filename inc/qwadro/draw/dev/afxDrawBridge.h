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
 *                       (c) 2017 SIGMA, Engitech, Scitech, Serpro
 *                             <https://sigmaco.org/qwadro/>
 */

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

/// No QWADRO, uma fila é dividida em duas partes, afxDrawInput na vanguarda e afxDrawBridge na retaguarda.
/// Ao criar um afxDrawContext, necessariamente ao menos uma afxDrawBridge foi criada consequentemente e associado ao mesmo.
/// Isto porque, no QWADRO, você não executa as operações de desenho. Ao invés, você submete elas a serem processadas através de um afxDrawInput.
/// Uma afxDrawBridge livre, por sua vez, e que pode realizar determinada operação submetida, então puxará a submissão.

/// A afxDrawBridge é uma unidade de gestão de transporte, onde se armazena a carga de trabalho até sua consumação.

#ifndef AVX_DRAW_BRIDGE_H
#define AVX_DRAW_BRIDGE_H

#include "qwadro/draw/pipe/afxPipeline.h"
#include "qwadro/draw/io/afxRaster.h"

typedef enum afxDrawBridgeFlags
{
    afxDrawBridgeFlag_DRAW      = AFX_BIT_OFFSET(0), // supports draw ops
  //afxDrawBridgeFlag_DRAW_AUX  = AFX_BIT_OFFSET(1), // supports auxiliary draw ops (but can't perform a entire draw pipeline execution)
    afxDrawBridgeFlag_COMPUTE   = AFX_BIT_OFFSET(2), // supports compute ops
    afxDrawBridgeFlag_TRANSFER  = AFX_BIT_OFFSET(3), // supports transfer ops
    afxDrawBridgeFlag_VHS       = AFX_BIT_OFFSET(4), // supports VHS enc/dec
} afxDrawBridgeFlags;

AFX_DEFINE_STRUCT(afxDrawCommand)
{
    afxNat              cmdNo;
    afxLinkage          imm;
    afxLinkage          drawCall;
    afxLinkage          stateCh;
    afxLinkage          pipBind;
    afxLinkage          vbufBind;
    afxLinkage          ibufBind;
    afxLinkage          bufBind;
};

AFX_DEFINE_STRUCT(afxExecutionRequest)
{
    afxSemaphore        wait;
    afxDrawStream       diob;
    afxSemaphore        signal;
};

AFX_DEFINE_STRUCT(afxPresentationRequest)
{
    afxSemaphore        wait;
    afxDrawOutput       dout;
    afxNat              bufIdx;
};

AFX_DEFINE_STRUCT(afxTransferRequest)
{
    union
    {
        struct
        {
            afxRaster       ras;
            afxRasterRegion rgn;
            union
            {
                void*       dst;
                void const* src;
                afxBuffer   buf;
                afxStream   iob;
            };
            afxSize         offset; // offset into buffer data;
            afxNat          rowStride; // bytes per row
            afxNat          rowCnt; // rows per layer
        }                   img;
        struct
        {
            afxBuffer       buf;
            afxSize         offset;
            afxNat          range;
            union
            {
                void*       dst;
                void const* src;
                afxStream   iob;
            };
            afxSize         dataOff;
        }                   data;
    };

    afxFcc              srcFcc;
    afxFcc              dstFcc;

    afxCodec            codec;
    afxNat              decompressedSiz;

    afxSemaphore        wait;
    afxSemaphore        signal;
};

AFX_DEFINE_STRUCT(afxDrawBridgeConfig)
{
    afxDrawBridgeFlags  flags;
    afxNat              portIdx;
    afxNat              queueCnt;
    afxReal const*      queuePriority;
};

#ifdef _AVX_DRAW_C
#ifdef _AVX_DRAW_BRIDGE_C
AFX_DEFINE_STRUCT(afxDrawQueue)
{
    afxDrawBridge       ddge;
    afxBool             immediate; // 0 = deferred, 1 = immediate
    afxBool             closed; // can't enqueue

    afxSlock            workArenaSlock;
    afxArena            workArena; // used by submission of queue operations, not stream commands.        
    afxChain            workChn;
    afxMutex            workChnMtx;
    
    afxCondition        idleCnd;
    afxMutex            idleCndMtx;

    // one command pool per queue to avoid thread interation.
    // one stream manager per queue to avoid thread interaction.

    afxManager          streams;
    afxQueue            recycQue;
    afxSlock            reqLock;
    afxBool             lockedForReq;
};
#ifdef _AVX_DRAW_BRIDGE_IMPL
struct _afxBaseDrawBridge
#else
AFX_OBJECT(afxDrawBridge)
#endif
{
    afxDrawContext      dctx; // owner
    afxNat              portIdx;
    afxNat              queCnt;
    afxDrawQueue*       queues;
    afxChain            managers;

    afxError            (*waitCb)(afxDrawBridge,afxNat);
    afxNat              (*executeCb)(afxDrawBridge,afxFence,afxNat,afxExecutionRequest const[]);
    afxNat              (*transferCb)(afxDrawBridge, afxFence, afxNat, afxTransferRequest const[]);
    afxNat              (*presentCb)(afxDrawBridge,afxNat,afxPresentationRequest const[]);
    afxNat              (*stampCb)(afxDrawBridge,afxNat,afxPresentationRequest const[],afxV2d const,afxString const*);
};
#endif//_AVX_DRAW_BRIDGE_C
#endif//_AVX_DRAW_C

AVX afxDrawDevice       AfxGetDrawBridgeDevice(afxDrawBridge ddge);
AVX afxDrawContext      AfxGetDrawBridgeContext(afxDrawBridge ddge);

AVX afxNat              AfxCountDrawQueues(afxDrawBridge ddge);

AVX afxError            AfxWaitForIdleDrawQueue
/// Wait for a queue to become idle. To wait on the host for the completion of outstanding queue operations for a given queue.
(
    afxDrawBridge       ddge, /// the bridge on which to wait.
    afxNat              queIdx /// the queue on which to wait.
);

AVX afxNat              AfxEnqueueExecutionRequest(afxDrawBridge ddge, afxFence fenc, afxNat cnt, afxExecutionRequest const req[]);
AVX afxNat              AfxEnqueueTransferRequest(afxDrawBridge ddge, afxFence fenc, afxNat cnt, afxTransferRequest const req[]);
AVX afxNat              AfxEnqueuePresentRequest(afxDrawBridge ddge, afxNat cnt, afxPresentationRequest const req[]);
AVX afxNat              AfxEnqueueStampRequest(afxDrawBridge ddge, afxNat cnt, afxPresentationRequest const req[], afxV2d const origin, afxString const* caption);

#endif//AVX_DRAW_BRIDGE_H
