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

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

// afxDrawInput devices operates like device submission queues grouping sets of draw streams and present their result to the connected draw output devices.

// No QWADRO, há um conceito de uma fila de submissão de trabalho para a GPU. Porém, diferentemente do Vulkan, no QWADRO esta "fila" é separada em dois.
// A primeira parte contém a demanda e a segunda parte o estado da fila junto a ponte.
// Isto porque assim podemos usar a mesma demanda para dois contextos diferentes, cada qual contendo a outra parte, e logo, seu estado particular.

#ifndef AFX_DRAW_INPUT_H
#define AFX_DRAW_INPUT_H

#include "qwadro/core/afxManager.h"
#include "qwadro/io/afxUri.h"
#include "qwadro/draw/dev/afxDrawBridge.h"
#include "qwadro/draw/io/afxVertexStream.h"

typedef enum afxDrawEventId
{
    afxDrawEventId_FENCE,
    afxDrawEventId_EXECUTE,
    afxDrawEventId_PRESENT,
    afxDrawEventId_UPLOAD,
    afxDrawEventId_DOWNLOAD,
    afxDrawEventId_PREFETCH,
    afxDrawEventId_REFRESH,
} afxDrawEventId;

AFX_DEFINE_STRUCT(afxDrawEvent)
{
    afxDrawEventId  id;
    afxBool         posted, accepted;
    void*           udd[0];
};

typedef afxBool(*afxDrawInputProc)(afxDrawInput din, afxDrawEvent const* ev);

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

#ifdef _AFX_DRAW_C
#ifdef _AFX_DRAW_INPUT_C
AFX_OBJECT(afxDrawInput)
{
    afxLinkage          ddev;
    afxLinkage          dctx; // bound context
    
    afxChain            classes;
    afxManager          ibuffers;
    afxManager          vbuffers;

    afxClipSpace        cachedClipCfg;

    afxUri128           txdUris[8];
    afxFile             txdHandles[8];

    afxArray            scripts;
    afxNat              minScriptReserve;

    afxNat              minVtxBufSiz;
    afxNat              maxVtxBufSiz;
    afxNat              minIdxBufSiz;
    afxNat              maxIdxBufSiz; // 13500000

    afxDrawInputProc    procCb;

    struct _afxDinIdd*  idd;
    void*               udd; // user-defined data
};
#endif
#endif

AVX afxDrawDevice   AfxGetDrawInputDevice(afxDrawInput din);

AVX void*           AfxGetDrawInputUdd(afxDrawInput din);

AVX afxManager*     AfxGetVertexBufferClass(afxDrawInput din);
AVX afxManager*     AfxGetIndexBufferClass(afxDrawInput din);

// Connection

AVX afxBool         AfxReconnectDrawInput(afxDrawInput din, afxDrawContext dctx);
AVX afxError        AfxDisconnectDrawInput(afxDrawInput din);
AVX afxBool         AfxGetDrawInputContext(afxDrawInput din, afxDrawContext* context);

// Matrices

AVX void            AfxDescribeClipSpace(afxDrawInput din, afxClipSpace* clip);

AVX void            AfxComputeLookToMatrices(afxDrawInput din, afxV3d const eye, afxV3d const dir, afxM4d v, afxM4d iv);
AVX void            AfxComputeLookAtMatrices(afxDrawInput din, afxV3d const eye, afxV3d const target, afxM4d v, afxM4d iv);

AVX void            AfxComputeBasicOrthographicMatrices(afxDrawInput din, afxReal aspectRatio, afxReal scale, afxReal range, afxM4d p, afxM4d ip);
AVX void            AfxComputeOrthographicMatrices(afxDrawInput din, afxV2d const extent, afxReal near, afxReal far, afxM4d p, afxM4d ip);
AVX void            AfxComputeOffcenterOrthographicMatrices(afxDrawInput din, afxReal left, afxReal right, afxReal bottom, afxReal top, afxReal near, afxReal far, afxM4d p, afxM4d ip);
AVX void            AfxComputeBoundingOrthographicMatrices(afxDrawInput din, afxAabb const aabb, afxM4d p, afxM4d ip);

AVX void            AfxComputeFovMatrices(afxDrawInput din, afxReal fovY, afxReal aspectRatio, afxReal near, afxReal far, afxM4d p, afxM4d ip);
AVX void            AfxComputeFrustrumMatrices(afxDrawInput din, afxReal left, afxReal right, afxReal bottom, afxReal top, afxReal near, afxReal far, afxM4d p, afxM4d ip);
AVX void            AfxComputeBasicPerspectiveMatrices(afxDrawInput din, afxReal aspectRatio, afxReal range, afxM4d p, afxM4d ip);
AVX void            AfxComputePerspectiveMatrices(afxDrawInput din, afxV2d const extent, afxReal near, afxReal far, afxM4d p, afxM4d ip);

AVX afxError        AfxUplinkTxds(afxDrawInput din, afxNat baseSlot, afxNat slotCnt, afxUri const uris[]);


AVX afxError        AfxOpenTarga(afxDrawInput din, afxUri const* uri, afxRaster* raster);
AVX afxError        AfxLoadTarga(afxDrawInput din, afxUri const* uri, afxRaster* raster);
AVX afxError        AfxReloadTarga(afxRaster ras, afxUri const* uri);


AVX afxNat          AfxExecuteDrawStreams(afxDrawInput din, afxNat cnt, afxExecutionRequest const req[], afxFence fenc);

////////////////////////////////////////////////////////////////////////////////

AVX afxError        AfxAcquireDrawInput(afxNat ddevId, afxDrawInputConfig const* cfg, afxDrawInput* input);

AVX afxError        AfxOpenDrawInput(afxNat ddevId, afxDrawInputConfig const* cfg, afxUri const* endpoint, afxDrawInput* input);

#endif//AFX_DRAW_INPUT_H
