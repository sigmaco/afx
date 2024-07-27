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

#ifndef AKX_MOTION_H
#define AKX_MOTION_H

#include "qwadro/sim/afxSimDefs.h"
#include "qwadro/space/afxCurve.h"
#include "qwadro/math/afxTransform.h"
#include "qwadro/base/afxFixedString.h"

AFX_DEFINE_STRUCT(akxMotionVector)
{
    //afxString           id;
    afxNat              seqKey;
    afxInt              dimension;
    afxCurve            value;
};

AFX_DEFINE_STRUCT(akxMotionTransform)
{
    //afxString           id;
    afxMask             flags;
    afxCurve            transmission;
    afxCurve            translation;
    afxCurve            transmutation;
};

AFX_DEFINE_STRUCT(akxMotionBlueprint)
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

AKX afxBool             AfxGetMotionId(akxMotion mot, afxString* id);

AKX void                AfxComputeMotionDisplacement(akxMotion mot, afxM4d m);

AKX afxBool             AfxFindMotionVector(akxMotion mot, afxString const* seqId, afxNat *seqIdx);
AKX afxBool             AfxFindMotionTransform(akxMotion mot, afxString const* seqId, afxNat *seqIdx);

AKX void                AfxUpdateMotionVectors(akxMotion mot, afxNat baseSeqIdx, afxNat seqCnt, akxMotionVector const vectors[], afxNat fetchRate);
AKX void                AfxUpdateMotionTransforms(akxMotion mot, afxNat baseSeqIdx, afxNat seqCnt, akxMotionTransform const transforms[], afxNat fetchRate);

////////////////////////////////////////////////////////////////////////////////

AKX afxError            AfxAssembleMotions(afxSimulation sim, afxNat cnt, akxMotionBlueprint const blueprints[], akxMotion motions[]);

#endif//AKX_MOTION_H
