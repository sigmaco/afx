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

#ifndef AFX_WORLD_POSE_H
#define AFX_WORLD_POSE_H

#include "qwadro/math/afxMathDefs.h"

AFX_DEFINE_HANDLE(akxPoseBuffer);

AFX_OBJECT(akxPoseBuffer)
{
    afxNat      matCnt;
    afxM4d*     world;
    afxM4d*     composite;
};

/// O objeto akxPoseBuffer é um buffer usado para manter o estado de um afxSkeleton de afxModel como expressado no "world space". 
/// Você pode criar uma akxPoseBuffer assim: 

AKX afxError    AfxAcquirePoseBuffers(afxSimulation sim, afxNat cnt, afxNat const artCnt[], afxBool const excludeComposite[], akxPoseBuffer wp[]);

/// Você pode encontrar o número de articulações representadas na akxPoseBuffer assim: 

AKX afxNat      AfxGetPoseBufferCapacity(akxPoseBuffer const wp);

/// Em qualquer tempo, você pode inspecionar ou modificar o estado alojado de uma articulação na akxPoseBuffer. 
/// Você acessa o estado da articulação como um afxTransform assim:

AKX afxM4d*     AfxGetWorldMatrices(akxPoseBuffer const wp, afxNat baseArtIdx);

/// O transforme para a articulação é uma afxM4d com componentes translacionais em afxReal[3][0], afxReal[3][1] e afxReal[3][2]. 
/// Desde que nunca há quaisquer componentes projetivos, os componentes afxReal[0][3], afxReal[1][3] e afxReal[2][3] são garantidos a estarem zerados, e o componente afxReal[3][3] é garantido de ser 1.

/// Similarmente, você pode inspecionar ou modificar a afxM4d composta para uma articulação: 

AKX afxM4d*     AfxGetCompositeMatrices(akxPoseBuffer const wp, afxNat baseArtIdx);

/// A afxM4d composta está no mesmo layout como aquela retornada de GetPoseBuffer4x4. 
/// É igual ao "world space" --- afxM4d espacial para a articulação (como dada por GetPoseBuffer4x4) multiplicada pelo transforme inverso de repouso no world-space --- para a articulação (como alojada no afxSkeleton). 
/// Portanto, esta representa a diferença entre a atual posição no world-space da articulação e sua posição de repouso no world-space.

/// As afxM4d de transformação no world-space e compostas são garantida a estarem contíguas, assim sendo, você pode também obter o ponteiro para o início da array e usá-lo para manualmente acessar articulações individuais. 

AKX afxM4d*     AfxPoseBufferGetWorldMatrixArray(akxPoseBuffer const wp);
AKX afxM4d*     AfxPoseBufferGetCompositeMatrixArray(akxPoseBuffer const wp);

#endif//AFX_WORLD_POSE_H
