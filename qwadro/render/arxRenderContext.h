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

#ifndef ARX_RENDER_CONTEXT_H
#define ARX_RENDER_CONTEXT_H

// arxRenderContext devices operates like device submission queues grouping sets of draw streams and present their result to the connected draw output devices.

// No QWADRO, há um conceito de uma fila de submissão de trabalho para a GPU. Porém, diferentemente do Vulkan, no QWADRO esta "fila" é separada em dois.
// A primeira parte contém a demanda e a segunda parte o estado da fila junto a ponte.
// Isto porque assim podemos usar a mesma demanda para dois contextos diferentes, cada qual contendo a outra parte, e logo, seu estado particular.

/*
    Draw input refers to the data or instructions provided to a system or application to create or render graphical content. 
    It involves the input that specifies what should be drawn, how it should be drawn, and where it should appear.

    In graphics programming, draw input consists of the commands or data used to generate visual content on the screen.
    In 3D graphics, vertex data includes coordinates, colors, and texture coordinates that define the geometry of objects.
    In game development, draw input refers to the data and instructions used to render game graphics and scenes.
    In vector graphics, draw input consists of commands and parameters that define shapes and lines in a vector-based drawing application.

*/

#include "qwadro/draw/afxDrawSystem.h"

#include "qwadro/render/arxRenderable.h"
#include "qwadro/render/arxRenderModel.h"
#include "qwadro/render/arxRenderBody.h"
#include "qwadro/render/arxRenderTerrain.h"
#include "qwadro/cad/arxTerrain.h"
#include "qwadro/cad/arxModel.h"
#include "qwadro/cad/arxMesh.h"
#include "qwadro/render/arxPlacement.h"
#include "qwadro/cad/arxPose.h"
#include "qwadro/render/arxCamera.h"
#include "qwadro/render/arxSky.h"
#include "qwadro/render/arxBufferizer.h"
#include "qwadro/render/arxTechnique.h"
#include "qwadro/render/arxDrawInput.h"
#include "qwadro/render/arxIllumination.h"
#include "qwadro/render/arxRenderer.h"
#include "qwadro/sim/arxAnimation.h"


typedef enum arxEventId
{
    arxEventId_
} arxEventId;

AFX_DEFINE_STRUCT(arxEvent)
{
    arxEventId  id;
    afxBool     posted, accepted;
    void*       udd[1];
};

typedef afxBool(*afxDrawInputProc)(arxRenderContext rctx, arxEvent const* ev);

AFX_DEFINE_STRUCT(arxRenderwareConfig)
{
    afxDrawSystem       dsys;
    afxDrawInputProc    proc;
    afxUnit             dragTxuBase;
    afxUnit             dragTxuCnt;

    afxUnit             cmdPoolMemStock;
    afxUnit             estimatedSubmissionCnt;
    afxUnit             minVtxBufCap;
    afxUnit             maxVtxBufCap;
    afxUnit             minIdxBufCap;
    afxUnit             maxIdxBufCap;
    void*               udd;
};

ARX afxDrawSystem   ArxGetRenderwareDrawSystem(arxRenderContext rctx);

ARX void*           ArxGetRenderwareUdd(arxRenderContext rctx);

// OPTICAL MATRIX UTILITY

ARX void            ArxGetClipSpaceInfo(arxRenderContext rctx, avxClipSpaceDepth* depth, afxBool* leftHanded);

ARX void            ArxComputeLookToMatrices(arxRenderContext rctx, afxV3d const eye, afxV3d const dir, afxM4d v, afxM4d iv);
ARX void            ArxComputeLookAtMatrices(arxRenderContext rctx, afxV3d const eye, afxV3d const target, afxM4d v, afxM4d iv);

ARX void            ArxComputeBasicOrthographicMatrices(arxRenderContext rctx, afxReal aspectRatio, afxReal scale, afxReal range, afxM4d p, afxM4d ip);
ARX void            ArxComputeOrthographicMatrices(arxRenderContext rctx, afxV2d const extent, afxReal near, afxReal far, afxM4d p, afxM4d ip);
ARX void            ArxComputeOffcenterOrthographicMatrices(arxRenderContext rctx, afxReal left, afxReal right, afxReal bottom, afxReal top, afxReal near, afxReal far, afxM4d p, afxM4d ip);
ARX void            ArxComputeBoundingOrthographicMatrices(arxRenderContext rctx, afxBox const aabb, afxM4d p, afxM4d ip);

ARX void            ArxComputeFovMatrices(arxRenderContext rctx, afxReal fovY, afxReal aspectRatio, afxReal near, afxReal far, afxM4d p, afxM4d ip);
ARX void            ArxComputeFrustrumMatrices(arxRenderContext rctx, afxReal left, afxReal right, afxReal bottom, afxReal top, afxReal near, afxReal far, afxM4d p, afxM4d ip);
ARX void            ArxComputeBasicPerspectiveMatrices(arxRenderContext rctx, afxReal aspectRatio, afxReal range, afxM4d p, afxM4d ip);
ARX void            ArxComputePerspectiveMatrices(arxRenderContext rctx, afxV2d const extent, afxReal near, afxReal far, afxM4d p, afxM4d ip);

ARX afxError        ArxUplinkTxds(arxRenderContext rctx, afxUnit baseSlot, afxUnit slotCnt, afxUri const uris[]);

////////////////////////////////////////////////////////////////////////////////

ARX afxError ArxConfigureRenderContext(afxUnit icd, arxRenderwareConfig* cfg);

ARX afxError ArxAcquireRenderContext
(
    afxUnit icd,
    arxRenderwareConfig const* cfg, 
    arxRenderContext* input
);

#endif//ARX_RENDER_CONTEXT_H
