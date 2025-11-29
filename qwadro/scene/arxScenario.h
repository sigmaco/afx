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

#ifndef ARX_SCENARIO_H
#define ARX_SCENARIO_H

// arxScenario devices operates like device submission queues grouping sets of draw streams and present their result to the connected draw output devices.

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
#include "qwadro/scene/arxPlacement.h"
#include "qwadro/cad/arxPose.h"
#include "qwadro/scene/arxCamera.h"
#include "qwadro/scene/arxSky.h"
#include "qwadro/scene/arxBufferizer.h"
#include "qwadro/render/arxTechnique.h"
#include "qwadro/render/arxRenderContext.h"
#include "qwadro/scene/arxIllumination.h"
#include "qwadro/render/arxRenderer.h"
#include "qwadro/anim/arxAnimation.h"


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

typedef afxBool(*afxDrawInputProc)(arxScenario scio, arxEvent const* ev);

AFX_DEFINE_STRUCT(arxScenarioConfig)
{
    afxDrawSystem       dsys;
    afxDrawInputProc    proc;
    afxUnit             dragTxuBase;
    afxUnit             dragTxuCnt;

    afxV3d              right;
    afxV3d              up;
    afxV3d              back;
    afxV3d              origin;
    afxBox              extent;
    afxReal             unitsPerMeter;
    afxReal             allowedLodErrFadingFactor;

    afxUnit             cmdPoolMemStock;
    afxUnit             estimatedSubmissionCnt;
    afxUnit             minVtxBufCap;
    afxUnit             maxVtxBufCap;
    afxUnit             minIdxBufCap;
    afxUnit             maxIdxBufCap;
    void*               udd;
};

ARX afxDrawSystem   ArxGetScenarioDrawSystem(arxScenario scio);

ARX void*           ArxGetScenarioUdd(arxScenario scio);

// OPTICAL MATRIX UTILITY

ARX void            ArxGetClipSpaceInfo(arxScenario scio, avxClipSpaceDepth* depth, afxBool* leftHanded);

ARX void ArxComputeLookToMatrices
(
    arxScenario scio, 
    afxV3d const eye, 
    afxV3d const dir, 
    afxM4d v, 
    afxM4d iv
);

ARX void ArxComputeLookAtMatrices
(
    arxScenario scio, 
    afxV3d const eye, 
    afxV3d const target, 
    afxM4d v, 
    afxM4d iv
);

ARX void ArxComputeBasicOrthographicMatrices
(
    arxScenario scio, 
    afxReal aspectRatio, 
    afxReal scale, 
    afxReal range, 
    afxM4d p, 
    afxM4d ip
);

ARX void ArxComputeOrthographicMatrices
(
    arxScenario scio, 
    afxV2d const extent, 
    afxReal near, 
    afxReal far, 
    afxM4d p, 
    afxM4d ip
);

ARX void ArxComputeOffcenterOrthographicMatrices
(
    arxScenario scio, 
    afxReal left, 
    afxReal right, 
    afxReal bottom, 
    afxReal top, 
    afxReal near, 
    afxReal far, 
    afxM4d p, 
    afxM4d ip
);

ARX void ArxComputeBoundingOrthographicMatrices
(
    arxScenario scio, 
    afxBox const aabb, 
    afxM4d p, 
    afxM4d ip
);

ARX void ArxComputeFovMatrices
(
    arxScenario scio, 
    afxReal fovY, 
    afxReal aspectRatio, 
    afxReal near, 
    afxReal far, 
    afxM4d p, 
    afxM4d ip
);

ARX void ArxComputeFrustrumMatrices
(
    arxScenario scio, 
    afxReal left, 
    afxReal right, 
    afxReal bottom, 
    afxReal top, 
    afxReal near, 
    afxReal far, 
    afxM4d p, 
    afxM4d ip
);

ARX void ArxComputeBasicPerspectiveMatrices
(
    arxScenario scio, 
    afxReal aspectRatio, 
    afxReal range, 
    afxM4d p, 
    afxM4d ip
);

ARX void ArxComputePerspectiveMatrices
(
    arxScenario scio, 
    afxV2d const extent, 
    afxReal near, 
    afxReal far, 
    afxM4d p, 
    afxM4d ip
);

ARX afxError        ArxUplinkTxds(arxScenario scio, afxUnit baseSlot, afxUnit slotCnt, afxUri const uris[]);

ARX void            ArxQueryLodErrorTolerance(arxScenario scio, afxReal allowedErr, afxReal *allowedErrEnd, afxReal *allowedErrScaler);

ARX void ArxComputeSimilarity
(
    arxScenario scio, 
    afxReal unitsPerMeter, 
    afxV3d const right, 
    afxV3d const up, 
    afxV3d const back, 
    afxV3d const origin, 
    afxM3d ltm, 
    afxM3d iltm, 
    afxV3d atv
);

ARX afxError ArxCullBodies(arxScenario scio, afxFrustum const* f, afxArray* pvs);

ARX afxUnit ArxRecenterAllCapstanClocks(arxScenario scio, afxReal dCurrentClock);

////////////////////////////////////////////////////////////////////////////////

ARX afxError ArxConfigureScenario
(
    afxUnit icd, 
    arxScenarioConfig* cfg
);

ARX afxError ArxAcquireScenario
(
    afxUnit icd,
    arxScenarioConfig const* cfg, 
    arxScenario* input
);

#endif//ARX_SCENARIO_H
