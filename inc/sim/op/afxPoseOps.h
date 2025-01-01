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

#ifndef AMX_POSE_OPS_H
#define AMX_POSE_OPS_H

#include "qwadro/inc/sim/body/afxPose.h"

AMX afxCmdId AmxCmdCopyPose(afxCatalyst cyst, afxPose src, afxUnit from, afxPose dst, afxUnit base, afxUnit cnt);

AMX afxCmdId AmxCmdApplyRootMotionVectors(afxCatalyst cyst, afxPose pose, afxV3d const translation, afxV3d const rotation);

AMX afxCmdId AmxCmdSampleBodyAnimationsAcceleratedLOD(afxCatalyst cyst, afxBody bod, afxUnit pivotCnt, afxM4d const offset, afxPose scratch, afxPlacement plce, afxReal allowedErr);
AMX afxCmdId AmxCmdSampleSingleBodyAnimationLODSparse(afxCatalyst cyst, afxBody bod, afxCapstan moto, afxUnit basePivotIdx, afxUnit pivotCnt, afxPose pose, afxReal allowedErr, afxUnit const* sparseBoneArray);
AMX afxCmdId AmxCmdSampleBodyAnimationsLODSparse(afxCatalyst cyst, afxBody bod, afxUnit basePivotIdx, afxUnit pivotCnt, afxPose pose, afxReal allowedErr, afxUnit const* sparseBoneArray);

AMX afxCmdId AmxCmdBuildIndexedCompositeBuffer(afxCatalyst cyst, afxModel mdl, afxUnit rigIdx, afxPlacement const plce, afxUnit cnt, afxUnit bufIdx, afxUnit offset);

#endif//AMX_POSE_OPS_H
