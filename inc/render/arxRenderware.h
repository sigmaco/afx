/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *             Q W A D R O   R E N D E R I N G   I N F R A S T R U C T U R E
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#ifndef ARX_DRAW_INPUT_H
#define ARX_DRAW_INPUT_H

// arxRenderware devices operates like device submission queues grouping sets of draw streams and present their result to the connected draw output devices.

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

#include "qwadro/inc/draw/afxDrawSystem.h"

#include "qwadro/inc/render/arxRenderable.h"
#include "qwadro/inc/render/arxRenderableModel.h"
#include "qwadro/inc/render/arxRenderableBody.h"
#include "qwadro/inc/render/arxRenderableTerrain.h"
#include "qwadro/inc/render/arxTerrain.h"
#include "qwadro/inc/render/arxModel.h"
#include "qwadro/inc/render/arxMesh.h"
#include "qwadro/inc/render/arxPlacement.h"
#include "qwadro/inc/render/arxPose.h"
#include "qwadro/inc/render/arxGeome.h"
#include "qwadro/inc/render/arxCamera.h"
#include "qwadro/inc/render/arxScene.h"
#include "qwadro/inc/render/afxVertexStream.h"
#include "qwadro/inc/render/arxTechnique.h"
#include "qwadro/inc/render/arxRenderContext.h"
#include "qwadro/inc/render/akxLighting.h"
#include "qwadro/inc/render/akxRenderer.h"
#include "qwadro/inc/render/anim/arxAnimation.h"


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

typedef afxBool(*afxDrawInputProc)(arxRenderware din, arxEvent const* ev);

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

ARX afxDrawSystem   ArxGetDrawInputContext(arxRenderware din);

ARX void*           ArxGetDrawInputUdd(arxRenderware din);

// OPTICAL MATRIX UTILITY

ARX void            ArxGetClipSpaceInfo(arxRenderware din, avxClipSpaceDepth* depth, afxBool* leftHanded);

ARX void            ArxComputeLookToMatrices(arxRenderware din, afxV3d const eye, afxV3d const dir, afxM4d v, afxM4d iv);
ARX void            ArxComputeLookAtMatrices(arxRenderware din, afxV3d const eye, afxV3d const target, afxM4d v, afxM4d iv);

ARX void            ArxComputeBasicOrthographicMatrices(arxRenderware din, afxReal aspectRatio, afxReal scale, afxReal range, afxM4d p, afxM4d ip);
ARX void            ArxComputeOrthographicMatrices(arxRenderware din, afxV2d const extent, afxReal near, afxReal far, afxM4d p, afxM4d ip);
ARX void            ArxComputeOffcenterOrthographicMatrices(arxRenderware din, afxReal left, afxReal right, afxReal bottom, afxReal top, afxReal near, afxReal far, afxM4d p, afxM4d ip);
ARX void            ArxComputeBoundingOrthographicMatrices(arxRenderware din, afxBox const aabb, afxM4d p, afxM4d ip);

ARX void            ArxComputeFovMatrices(arxRenderware din, afxReal fovY, afxReal aspectRatio, afxReal near, afxReal far, afxM4d p, afxM4d ip);
ARX void            ArxComputeFrustrumMatrices(arxRenderware din, afxReal left, afxReal right, afxReal bottom, afxReal top, afxReal near, afxReal far, afxM4d p, afxM4d ip);
ARX void            ArxComputeBasicPerspectiveMatrices(arxRenderware din, afxReal aspectRatio, afxReal range, afxM4d p, afxM4d ip);
ARX void            ArxComputePerspectiveMatrices(arxRenderware din, afxV2d const extent, afxReal near, afxReal far, afxM4d p, afxM4d ip);

ARX afxError        ArxUplinkTxds(arxRenderware din, afxUnit baseSlot, afxUnit slotCnt, afxUri const uris[]);

////////////////////////////////////////////////////////////////////////////////

ARX afxUnit ArxEnumerateDrawInputs
(
    afxUnit icd, 
    afxUnit first, 
    afxUnit cnt, 
    arxRenderware inputs[]
);

ARX afxUnit ArxInvokeDrawInputs
(
    afxUnit icd,
    afxUnit first, 
    afxUnit cnt, 
    afxBool(*f)(arxRenderware, void*), 
    void *udd
);

ARX afxUnit ArxEvokeDrawInputs
(
    afxUnit icd,
    afxBool(*f)(arxRenderware, void*), 
    void* udd, 
    afxUnit first, 
    afxUnit cnt, 
    arxRenderware inputs[]
);


ARX afxError ArxConfigureRenderware(afxUnit icd, arxRenderwareConfig* cfg);

ARX afxError ArxOpenRenderware
(
    afxUnit icd,
    arxRenderwareConfig const* cfg, 
    arxRenderware* input
);

#endif//ARX_DRAW_INPUT_H
