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

/// No QWADRO, uma fila é dividida em duas partes, afxDrawInput na vanguarda e afxDrawBridge na retaguarda.
/// Ao criar um afxDrawContext, necessariamente ao menos uma afxDrawBridge foi criada consequentemente e associado ao mesmo.
/// Isto porque, no QWADRO, você não executa as operações de desenho. Ao invés, você submete elas a serem processadas através de um afxDrawInput.
/// Uma afxDrawBridge livre, por sua vez, e que pode realizar determinada operação submetida, então puxará a submissão.

/// Uma afxDrawBridge é um objeto que mapeia afxDrawQueue para uma porta de execução de cargas de trabalho.
/// A afxDrawQueue é uma unidade de gestão de transporte, onde se armazena a carga de trabalho até sua consumação.

#ifndef AVX_DRAW_BRIDGE_H
#define AVX_DRAW_BRIDGE_H

#include "qwadro/draw/pipe/avxPipeline.h"
#include "qwadro/draw/io/afxRaster.h"

typedef enum afxDrawBridgeFlag
{
    afxDrawBridgeFlag_DRAW      = AFX_BIT(0), // supports draw ops
  //afxDrawBridgeFlag_DRAW_AUX  = AFX_BIT(1), // supports auxiliary draw ops (but can't perform a entire draw pipeline execution)
    afxDrawBridgeFlag_COMPUTE   = AFX_BIT(2), // supports compute ops
    afxDrawBridgeFlag_TRANSFER  = AFX_BIT(3), // supports transfer ops
    afxDrawBridgeFlag_VHS       = AFX_BIT(4), // supports VHS enc/dec
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
    avxCmdb        cmdb;
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
            afxRasterIo   op;
            union
            {
                void*       dst;
                void const* src;
                afxBuffer   buf;
                afxStream   iob;
            };
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

AVX afxDrawDevice       AfxGetDrawBridgeDevice(afxDrawBridge ddge);
AVX afxDrawContext      AfxGetDrawBridgeContext(afxDrawBridge ddge);

AVX afxNat              AfxGetDrawBridgePort(afxDrawBridge ddge);

AVX afxNat              AfxEnqueueExecutionRequest(afxDrawBridge ddge, afxFence fenc, afxNat cnt, afxExecutionRequest const req[]);
AVX afxNat              AfxEnqueueTransferRequest(afxDrawBridge ddge, afxFence fenc, afxNat cnt, afxTransferRequest const req[]);
AVX afxNat              AfxEnqueuePresentRequest(afxDrawBridge ddge, afxNat cnt, afxPresentationRequest const req[]);
AVX afxNat              AfxEnqueueStampRequest(afxDrawBridge ddge, afxNat cnt, afxPresentationRequest const req[], afxV2d const origin, afxString const* caption);

#endif//AVX_DRAW_BRIDGE_H
