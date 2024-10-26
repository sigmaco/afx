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

#ifndef AMX_POSE_H
#define AMX_POSE_H

#include "qwadro/inc/sim/afxSimDefs.h"
#include "qwadro/inc/math/afxTransform.h"

AMX afxError        AfxAcquirePoses(afxSimulation sim, afxUnit cnt, afxUnit const artCnt[], afxPose lp[]);

AMX afxUnit          AfxGetPoseCapacity(afxPose const lp);

AMX afxTransform*   AfxGetPoseTransform(afxPose const pose, afxUnit artIdx);

AMX void            AfxCopyPose(afxPose pose, afxPose const from);

AMX void            AfxApplyRootMotionVectorsToPose(afxPose pose, afxV3d const translation, afxV3d const rotation);

AMX void            AfxAccumulateLocalTransform(afxPose LocalPose, int LocalPoseBoneIndex, int SkeletonBoneIndex, float Weight, const afxSkeleton ReferenceSkeleton, afxQuatBlend Mode, const afxTransform *Transform);

#endif//AMX_POSE_H
