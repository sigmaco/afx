#pragma once
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

#ifndef AMX_MOTION_H
#define AMX_MOTION_H

#include "qwadro/inc/sim/afxSimDefs.h"
#include "qwadro/inc/math/afxCurve.h"
#include "qwadro/inc/math/afxTransform.h"
#include "qwadro/inc/base/afxFixedString.h"

AFX_DEFINE_STRUCT(afxMotionVector)
{
    //afxString           id;
    afxNat              seqKey;
    afxInt              dimension;
    afxCurve            value;
};

AFX_DEFINE_STRUCT(afxMotionTransform)
{
    //afxString           id;
    afxMask             flags;
    afxCurve            transmission;
    afxCurve            translation;
    afxCurve            transmutation;
};

AFX_DEFINE_STRUCT(afxMotionBlueprint)
{
    afxString32         id;
    afxNat              vecCnt;
    afxString const*    vectors;
    afxNat              pivotCnt;
    afxString const*    pivots;
    afxReal const*      pivotLodError;
    afxBool             incPivotLodError;
    afxTransform        displacement;
};

AMX afxBool             AfxGetMotionId(afxMotion mot, afxString* id);

AMX void                AfxComputeMotionDisplacement(afxMotion mot, afxM4d m);

AMX afxBool             AfxFindMotionVector(afxMotion mot, afxString const* seqId, afxNat *seqIdx);
AMX afxBool             AfxFindMotionTransform(afxMotion mot, afxString const* seqId, afxNat *seqIdx);

AMX void                AfxUpdateMotionVectors(afxMotion mot, afxNat baseSeqIdx, afxNat seqCnt, afxMotionVector const vectors[], afxNat fetchRate);
AMX void                AfxUpdateMotionTransforms(afxMotion mot, afxNat baseSeqIdx, afxNat seqCnt, afxMotionTransform const transforms[], afxNat fetchRate);

////////////////////////////////////////////////////////////////////////////////

AMX afxError            AfxAssembleMotions(afxSimulation sim, afxNat cnt, afxMotionBlueprint const blueprints[], afxMotion motions[]);

#endif//AMX_MOTION_H
