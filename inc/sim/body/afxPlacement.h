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

// O objeto afxPlacement é um buffer usado para manter o estado de um afxSkeleton de afxModel como expressado no "world space". 

#ifndef AMX_PLACEMENT_H
#define AMX_PLACEMENT_H

#include "qwadro/inc/sim/afxSimDefs.h"

AMX afxError    AfxAcquirePlacements(afxSimulation sim, afxUnit cnt, afxUnit const artCnt[], afxBool const excludeComposite[], afxPlacement placements[]);

AMX afxUnit     AfxGetPlacementCapacity(afxPlacement const plce);

AMX afxM4d*     AfxGetPlacementMatrices(afxPlacement const plce, afxUnit baseArtIdx);

AMX afxM4d*     AfxGetPlacementDeltas(afxPlacement const plce, afxUnit baseArtIdx);

/// A afxM4d composta está no mesmo layout como aquela retornada de GetPosture4x4. 
/// É igual ao "world space" --- afxM4d espacial para a articulação (como dada por GetPosture4x4) multiplicada pelo transforme inverso de repouso no world-space --- para a articulação (como alojada no afxSkeleton). 
/// Portanto, esta representa a diferença entre a atual posição no world-space da articulação e sua posição de repouso no world-space.

/// As afxM4d de transformação no world-space e compostas são garantida a estarem contíguas, assim sendo, você pode também obter o ponteiro para o início da array e usá-lo para manualmente acessar articulações individuais. 

AMX afxM4d*     AfxPostureGetWorldMatrixArray(afxPlacement const plce);
AMX afxM4d*     AfxPostureGetCompositeMatrixArray(afxPlacement const plce);

#endif//AMX_PLACEMENT_H
