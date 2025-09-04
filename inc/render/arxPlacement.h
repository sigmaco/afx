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

// O objeto arxPlacement � um buffer usado para manter o estado de um arxSkeleton de arxModel como expressado no "world space". 

#ifndef ARX_PLACEMENT_H
#define ARX_PLACEMENT_H

#include "qwadro/inc/render/arxRenderable.h"

ARX afxUnit     ArxGetPlacementCapacity(arxPlacement plce);

ARX afxM4d*     ArxGetPlacementMatrices(arxPlacement plce, afxUnit baseArtIdx);

ARX afxM4d*     ArxGetPlacementDeltas(arxPlacement plce, afxUnit baseArtIdx);

/// A afxM4d composta est� no mesmo layout como aquela retornada de GetPosture4x4. 
/// � igual ao "world space" --- afxM4d espacial para a articula��o (como dada por GetPosture4x4) multiplicada pelo transforme inverso de repouso no world-space --- para a articula��o (como alojada no arxSkeleton). 
/// Portanto, esta representa a diferen�a entre a atual posi��o no world-space da articula��o e sua posi��o de repouso no world-space.

/// As afxM4d de transforma��o no world-space e compostas s�o garantida a estarem cont�guas, assim sendo, voc� pode tamb�m obter o ponteiro para o in�cio da array e us�-lo para manualmente acessar articula��es individuais. 

// Pass NIL as @pose to ArxBuildPlacement() to compute the rest placement.
// Pass TRUE as composite to ArxBuildPlacement() to compute the placement's composite buffer.
ARX void        ArxBuildPlacement(arxPlacement plce, arxPose pose, arxModel skl, afxUnit baseJntIdx, afxUnit jntCnt, afxUnit baseReqJnt, afxUnit reqJntCnt, afxM4d const displace, afxBool skipDeltas);

// extract the pose
ARX void        ArxRebuildPose(arxPlacement plce, arxModel skl, afxUnit baseJntIdx, afxUnit jntCnt, afxM4d const displace, afxBool rigid, arxPose pose);

ARX void        ArxBuildCompositeMatrices(arxPlacement plce, arxModel skl, afxUnit baseJnt, afxUnit cnt, afxBool /*3x4*/transposed, afxM4d matrices[]);
ARX void        ArxBuildIndexedCompositeMatrices(arxPlacement plce, arxModel skl, afxUnit cnt, afxUnit const jntMap[], afxBool /*3x4*/transposed, afxM4d matrices[]);

////////////////////////////////////////////////////////////////////////////////

ARX afxError    ArxAcquirePlacements(arxRenderware din, afxUnit cnt, afxUnit const artCnt[], afxBool const excludeDeltas[], arxPlacement placements[]);

#endif//ARX_PLACEMENT_H
