/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *              T H E   Q W A D R O   E X E C U T I O N   E C O S Y S T E M
 *
 *                                   Public Test Build
 *                   (c) 2017 SIGMA Technology Group — Federação SIGMA
 *                                    www.sigmaco.org
 */

#ifndef AFX_SKELETON_H
#define AFX_SKELETON_H

#include "../afxPose.h"
#include "../afxWorldPose.h"
#include "afx/core/io/afxUri.h"

/// O objeto afxSkeleton é uma coleção hierárquica de articulações que descrevem a estrutura articular interna de um afxModel, e auxilia no manejo e na animação do mesmo. 

/// Cada articulação no arranjo do afxSkeleton aloja o transforme para aquela articulação em dois meios diferentes. 
/// Primeiro, afxTransform "local" é o transforme decomposto da articulação relativa a seu parente imediato (dado pelo índice do parente).
/// Se esta não houver parente (isto é, o índex do parente é igual a AFX_INVALID_INDEX), então esta é relativa a origem do afxSkeleton.

/// Segundo, "iw" é uma afxM4d que é o transforme invertido no world-space para a articulação na postura padrão do afxSkeleton (isto é, a postura na qual o afxSkeleton foi originalmente modelado).

/// O afxTransform "local" é usado primariamente na composição de animações, e a afxM4d "iw" é usada primariamente para deformação de malha, razão pela qual esta informação é alojada em dois diferentes formatos.

/// Cada articulação também opcionalmente contém dados definidos pelo usuário "UDD".
/// Este campo aponta para algum dado, estranho à lógica do Qwadro, associado com a articulação, se houver qualquer.

AFX_DEFINE_HANDLE(afxSkeleton);

AFX_DEFINE_STRUCT(afxSkeletonBone)
{
    afxString*          name; // 32
    afxNat              parentIdx;
    afxTransform        local;
    afxM4d              iw;
    afxReal             lodError;
    void*               extendedData;
};

AFX_OBJECT(afxSkeleton)
{
    afxInstance           obj;
    afxUri*             uri; // 128
    afxNat              boneCnt;
    afxSkeletonBone     *bones; // afxNode
    afxNat              lodType;
};

AFX void*               AfxSkeletonGetSimulation(afxSkeleton skl);

AFX afxUri const*       AfxSkeletonGetUri(afxSkeleton skl);
AFX afxSkeleton         AfxSkeletonClone(afxSkeleton skl);

AFX afxNat              AfxSkeletonGetBoneCount(afxSkeleton skl);
AFX afxSkeletonBone*    AfxSkeletonGetBone(afxSkeleton skl, afxNat boneIdx);
AFX afxBool             AfxSkeletonFindBone(afxSkeleton skl, afxString const *name, afxNat *boneIdx);

AFX void                AfxSkeletonTransform(afxSkeleton skl, afxV3d const affine, afxM3d const linear, afxM3d const invLinear);

AFX void                AfxSkeletonComputeRestPosture(afxSkeleton skl, afxNat firstArt, afxNat artCnt, afxPose *lp);
AFX void                AfxSkeletonComputeRestPose(afxSkeleton skl, afxNat firstArt, afxNat artCnt, afxM4d const offset, afxWorldPose *wp);

AFX void                AfxSkeletonComputePose(afxSkeleton skl, afxNat firstArt, afxNat artCnt, afxPose const *lp, afxM4d const offset, afxWorldPose *wp);

#endif//AFX_SKELETON_H