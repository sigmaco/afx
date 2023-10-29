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

#include "afx/math/afxTransform.h"
#include "afx/sim/afxSimulation.h"
#include "afx/math/afxMatrix.h"
#include "afx/sim/afxAsset.h"

_AFXINL void AfxComputeBasisConversion(afxCadToolInfo const* info, afxReal desiredUnitsPerMeter, afxReal const desiredOrigin[3], afxReal const desiredAxes[3][3], afxReal atv[3], afxReal ltm[3][3], afxReal iltm[3][3])
{
    // char ComputeBasisConversion(const file_info *FileInfo, float DesiredUnitsPerMeter, const float *DesiredOrigin3, const float *DesiredRight3, const float *DesiredUp3, const float *DesiredBack3, float *ResultAffine3, float *ResultLinear3x3, float *ResultInverseLinear3x3)

    afxError err = NIL;
    AfxAssert(info);

    afxM3d desiredAxisSys, srcAxisSys;
    AfxCopyM3d(desiredAxisSys, desiredAxes);
    AfxCopyM3d(srcAxisSys, info->xyzAxes);

    AfxMultiplyM3dTransposed(ltm, desiredAxisSys, srcAxisSys);
        
    afxReal s = desiredUnitsPerMeter / info->unitsPerMeter;        
    AfxScaleV3d(ltm[0], ltm[0], s);
    AfxScaleV3d(ltm[1], ltm[1], s);
    AfxScaleV3d(ltm[2], ltm[2], s);

    AfxInvertM3d(iltm, ltm);

    AfxSubV3d(atv, desiredOrigin, info->origin);
}
