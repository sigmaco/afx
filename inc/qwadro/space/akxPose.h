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

/// O objeto akxPose é um buffer usado para manter o estado de um afxSkeleton como expressado em sua "pose local". 
/// Aqui, cada articulação no afxSkeleton é representada por um afxTransform que é relativo a sua articulação-parente imediata. 

#ifndef AKX_POSE_H
#define AKX_POSE_H

#include "qwadro/sim/afxSimDefs.h"
#include "qwadro/math/afxTransform.h"

AKX afxError        AfxAcquirePoses(afxSimulation sim, afxNat cnt, afxNat const artCnt[], akxPose lp[]);

AKX afxNat          AfxGetPoseCapacity(akxPose const lp);

AKX afxTransform*   AfxGetPoseTransform(akxPose const pose, afxNat artIdx);

AKX void            AfxCopyPose(akxPose pose, akxPose const from);

AKX void            AfxApplyRootMotionVectorsToPose(akxPose pose, afxV3d const translation, afxV3d const rotation);

AKX void            AfxAccumulateLocalTransform(akxPose LocalPose, int LocalPoseBoneIndex, int SkeletonBoneIndex, float Weight, const afxSkeleton ReferenceSkeleton, akxQuatBlend Mode, const afxTransform *Transform);

AKX afxNat          AfxRunManipulatedPose(akxPose pose, afxReal startTime, afxReal duration, afxNat iterCnt, akxTrackMask* modelMask, afxNat cnt, akxBody bodies[]);

#endif//AKX_POSE_H
