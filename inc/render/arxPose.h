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

/// O objeto arxPose � um buffer usado para manter o estado de um arxSkeleton como expressado em sua "pose local". 
/// Aqui, cada articula��o no arxSkeleton � representada por um afxTransform que � relativo a sua articula��o-parente imediata. 

#ifndef ARX_POSE_H
#define ARX_POSE_H

#include "qwadro/inc/render/arxRenderable.h"
#include "qwadro/inc/math/afxTransform.h"

ARX afxUnit         ArxGetPoseCapacity(arxPose pose);

ARX afxTransform*   ArxGetPoseTransform(arxPose pose, afxUnit artIdx);

ARX void            ArxCopyPose(arxPose pose, afxUnit toBaseArtIdx, arxPose from, afxUnit fromBaseArtIdx, afxUnit cnt, afxUnit const artMap[]);
ARX void            ArxModulatePose(arxPose pose, afxUnit toBaseArtIdx, arxPose composite, afxUnit fromBaseArtIdx, afxUnit artCnt, afxReal weightNone, afxReal weightAll, akxTrackMask* mask, afxUnit const jntRemap[]);

ARX void            ArxComputeAttachmentWorldMatrix(arxPose pose, arxModel skl, afxUnit jntIdx, afxUnit const sparseJntMap[], afxUnit const sparseJntMapRev[], afxM4d const displace, afxM4d m);
ARX void            ArxComputeAttachmentOffset(arxPose pose, arxModel skl, afxUnit jntIdx, afxUnit const sparseJntMap[], afxUnit const sparseJntMapRev[], afxM4d const displace, afxM4d m);

ARX void            ArxRebuildRestPose(arxPose pose, arxModel skl, afxUnit baseJntIdx, afxUnit jntCnt);

ARX void            ArxApplyPoseRootMotionVectors(arxPose pose, afxV3d const translation, afxV3d const rotation);

ARX void            ArxCommencePoseAccumulation(arxPose pose, afxUnit baseArtIdx, afxUnit artCnt, afxUnit const jntMap[]);
ARX void            ArxConcludePoseAccumulation(arxPose pose, afxUnit baseArtIdx, afxUnit artCnt, arxModel skl, afxReal allowedErr, afxUnit const jntMap[]);
ARX void            ArxAccumulateLocalTransform(arxPose pose, afxUnit artIdx, afxUnit sklJntIdx, afxReal weight, arxModel skl, afxQuatBlend blendOp, afxTransform const* t);

////////////////////////////////////////////////////////////////////////////////

AFX_DEFINE_STRUCT(arxPoseInfo)
{
    afxUnit     artCnt;
    afxReal     fillThreshold;
    afxString   tag;
    void*       udd;
};

ARX afxError    ArxAcquirePoses(arxRenderware din, afxUnit cnt, arxPoseInfo const info[], arxPose lp[]);

#endif//ARX_POSE_H
