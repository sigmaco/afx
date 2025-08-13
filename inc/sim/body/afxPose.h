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

/// O objeto afxPose é um buffer usado para manter o estado de um afxSkeleton como expressado em sua "pose local". 
/// Aqui, cada articulação no afxSkeleton é representada por um afxTransform que é relativo a sua articulação-parente imediata. 

#ifndef ASX_POSE_H
#define ASX_POSE_H

#include "qwadro/inc/sim/afxSimDefs.h"
#include "qwadro/inc/math/afxTransform.h"

ASX afxUnit         AsxGetPoseCapacity(afxPose pose);

ASX afxTransform*   AsxGetPoseTransform(afxPose pose, afxUnit artIdx);

ASX void            AsxCopyPose(afxPose pose, afxUnit toBaseArtIdx, afxPose from, afxUnit fromBaseArtIdx, afxUnit cnt, afxUnit const artMap[]);
ASX void            AsxModulatePose(afxPose pose, afxUnit toBaseArtIdx, afxPose composite, afxUnit fromBaseArtIdx, afxUnit artCnt, afxReal weightNone, afxReal weightAll, akxTrackMask* mask, afxUnit const jntRemap[]);

ASX void            AfxComputeAttachmentWorldMatrix(afxPose pose, afxModel skl, afxUnit jntIdx, afxUnit const sparseJntMap[], afxUnit const sparseJntMapRev[], afxM4d const displace, afxM4d m);
ASX void            AfxComputeAttachmentOffset(afxPose pose, afxModel skl, afxUnit jntIdx, afxUnit const sparseJntMap[], afxUnit const sparseJntMapRev[], afxM4d const displace, afxM4d m);

ASX void            AfxRebuildRestPose(afxPose pose, afxModel skl, afxUnit baseJntIdx, afxUnit jntCnt);

ASX void            AsxApplyPoseRootMotionVectors(afxPose pose, afxV3d const translation, afxV3d const rotation);

ASX void            AsxCommencePoseAccumulation(afxPose pose, afxUnit baseArtIdx, afxUnit artCnt, afxUnit const jntMap[]);
ASX void            AsxConcludePoseAccumulation(afxPose pose, afxUnit baseArtIdx, afxUnit artCnt, afxModel skl, afxReal allowedErr, afxUnit const jntMap[]);
ASX void            AsxAccumulateLocalTransform(afxPose pose, afxUnit artIdx, afxUnit sklJntIdx, afxReal weight, afxModel skl, afxQuatBlend blendOp, afxTransform const* t);

////////////////////////////////////////////////////////////////////////////////

ASX afxError        AfxAcquirePoses(afxSimulation sim, afxUnit cnt, afxUnit const artCnt[], afxPose lp[]);

#endif//ASX_POSE_H
