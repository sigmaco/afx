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

// afxDrawInput devices operates like device submission queues grouping sets of draw streams and present their result to the connected draw output devices.

// No QWADRO, h� um conceito de uma fila de submiss�o de trabalho para a GPU. Por�m, diferentemente do Vulkan, no QWADRO esta "fila" � separada em dois.
// A primeira parte cont�m a demanda e a segunda parte o estado da fila junto a ponte.
// Isto porque assim podemos usar a mesma demanda para dois contextos diferentes, cada qual contendo a outra parte, e logo, seu estado particular.

#ifndef AVX_DRAW_INPUT_H
#define AVX_DRAW_INPUT_H

#include "qwadro/base/afxClass.h"
#include "qwadro/io/afxUri.h"
#include "qwadro/draw/dev/afxDrawBridge.h"
#include "qwadro/draw/io/afxVertexStream.h"
#include "qwadro/draw/io/afxDrawTechnique.h"
#include "qwadro/draw/math/avxMatrix.h"

typedef enum avxEventId
{
    avxEventId_FENCE,
    avxEventId_EXECUTE,
    avxEventId_PRESENT,
    avxEventId_UPLOAD,
    avxEventId_DOWNLOAD,
    avxEventId_PREFETCH,
    avxEventId_REFRESH,
    avxEventId_RECONNECT,
    avxEventId_EXTENT,    
} avxEventId;

AFX_DEFINE_STRUCT(avxEvent)
{
    avxEventId  id;
    afxBool         posted, accepted;
    void*           udd[1];
};

typedef afxBool(*afxDrawInputProc)(afxDrawInput din, avxEvent const* ev);

AFX_DEFINE_STRUCT(afxDrawInputConfig)
{
    afxDrawInputProc    proc;
    afxNat              dragTxuBase;
    afxNat              dragTxuCnt;

    afxNat              cmdPoolMemStock;
    afxNat              estimatedSubmissionCnt;
    afxNat              minVtxBufCap;
    afxNat              maxVtxBufCap;
    afxNat              minIdxBufCap;
    afxNat              maxIdxBufCap;
    void*               udd;
};

AVX afxDrawDevice   AfxGetDrawInputDevice(afxDrawInput din);

AVX void*           AfxGetDrawInputUdd(afxDrawInput din);

AVX afxClass const* AfxGetCameraClass(afxDrawInput din);
AVX afxClass const* AfxGetDrawTechniqueClass(afxDrawInput din);
AVX afxClass*       AfxGetIndexBufferClass(afxDrawInput din);
AVX afxClass*       AfxGetVertexBufferClass(afxDrawInput din);
AVX afxClass const* AfxGetTxdClass(afxDrawInput din);

// Connection

AVX afxError        AfxDisconnectDrawInput(afxDrawInput din);
AVX afxBool         AfxReconnectDrawInput(afxDrawInput din, afxDrawContext dctx);
AVX afxBool         AfxGetDrawInputContext(afxDrawInput din, afxDrawContext* context);

// Matrices

AVX void            AfxDescribeClipSpace(afxDrawInput din, avxClipSpace* clip);

AVX void            AfxComputeLookToMatrices(afxDrawInput din, afxV3d const eye, afxV3d const dir, afxM4d v, afxM4d iv);
AVX void            AfxComputeLookAtMatrices(afxDrawInput din, afxV3d const eye, afxV3d const target, afxM4d v, afxM4d iv);

AVX void            AfxComputeBasicOrthographicMatrices(afxDrawInput din, afxReal aspectRatio, afxReal scale, afxReal range, afxM4d p, afxM4d ip);
AVX void            AfxComputeOrthographicMatrices(afxDrawInput din, afxV2d const extent, afxReal near, afxReal far, afxM4d p, afxM4d ip);
AVX void            AfxComputeOffcenterOrthographicMatrices(afxDrawInput din, afxReal left, afxReal right, afxReal bottom, afxReal top, afxReal near, afxReal far, afxM4d p, afxM4d ip);
AVX void            AfxComputeBoundingOrthographicMatrices(afxDrawInput din, afxBox const aabb, afxM4d p, afxM4d ip);

AVX void            AfxComputeFovMatrices(afxDrawInput din, afxReal fovY, afxReal aspectRatio, afxReal near, afxReal far, afxM4d p, afxM4d ip);
AVX void            AfxComputeFrustrumMatrices(afxDrawInput din, afxReal left, afxReal right, afxReal bottom, afxReal top, afxReal near, afxReal far, afxM4d p, afxM4d ip);
AVX void            AfxComputeBasicPerspectiveMatrices(afxDrawInput din, afxReal aspectRatio, afxReal range, afxM4d p, afxM4d ip);
AVX void            AfxComputePerspectiveMatrices(afxDrawInput din, afxV2d const extent, afxReal near, afxReal far, afxM4d p, afxM4d ip);

AVX afxError        AfxUplinkTxds(afxDrawInput din, afxNat baseSlot, afxNat slotCnt, afxUri const uris[]);


AVX afxError        AfxOpenTarga(afxDrawInput din, afxUri const* uri, afxRaster* raster);
AVX afxError        AfxLoadTarga(afxDrawInput din, afxUri const* uri, afxRaster* raster);
AVX afxError        AfxReloadTarga(afxRaster ras, afxUri const* uri);


AVX afxNat          AfxExecuteCmdBuffers(afxDrawInput din, afxNat cnt, afxExecutionRequest const req[], afxFence fenc);

AVX afxNat          AfxEvokeCameras(afxDrawInput din, afxBool(*flt)(avxCamera, void*), void* fdd, afxNat first, afxNat cnt, avxCamera cameras[]);
AVX afxNat          AfxInvokeCameras(afxDrawInput din, afxNat first, afxNat cnt, afxBool(*f)(avxCamera, void*), void* udd);
AVX afxNat          AfxEnumerateCameras(afxDrawInput din, afxNat first, afxNat cnt, avxCamera cameras[]);

////////////////////////////////////////////////////////////////////////////////

AVX afxError        AfxConfigureDrawInput(afxNat ddevId, afxDrawInputConfig* cfg);

AVX afxError        AfxAcquireDrawInput(afxNat ddevId, afxDrawInputConfig const* cfg, afxDrawInput* input);

AVX afxError        AfxOpenDrawInput(afxNat ddevId, afxUri const* endpoint, afxDrawInputConfig const* cfg, afxDrawInput* input);

#endif//AVX_DRAW_INPUT_H