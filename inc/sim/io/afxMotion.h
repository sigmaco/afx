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

#ifndef ASX_MOTION_H
#define ASX_MOTION_H

#include "qwadro/inc/sim/afxSimDefs.h"
#include "qwadro/inc/sim/io/afxCurve.h"
#include "qwadro/inc/math/afxTransform.h"
#include "qwadro/inc/base/afxFixedString.h"

typedef enum afxMotionFlag
{
    afxMotionFlag_NONE
} afxMotionFlags;

AFX_DEFINE_STRUCT(afxVectorialMotion)
{
    afxUnit             seqKey;
    afxInt              dimension;
    afxCurve            value;
};

AFX_DEFINE_STRUCT(afxPivotalMotion)
{
    afxMask             flags;
    afxCurve            transmission;
    afxCurve            translation;
    afxCurve            transmutation;
};

AFX_DEFINE_STRUCT(afxMotionBlueprint)
{
    afxString32         id;
    afxUnit             vecCnt;
    afxString const*    vectors;
    afxUnit             pivotCnt;
    afxString const*    pivots;
    afxReal const*      pivotLodError;
    afxBool             incPivotLodError;
    afxTransform        displacement;
};

ASX afxMask     AfxGetMotionFlags(afxMotion mot);

ASX afxBool     AfxGetMotionId(afxMotion mot, afxString* id);

ASX void        AfxComputeMotionDisplacement(afxMotion mot, afxM4d m);

ASX afxBool     AfxFindMotionVector(afxMotion mot, afxString const* seqId, afxUnit *seqIdx);
ASX afxBool     AfxFindMotionTransform(afxMotion mot, afxString const* seqId, afxUnit *seqIdx);

ASX void        AfxUpdateMotionVectors(afxMotion mot, afxUnit baseSeqIdx, afxUnit seqCnt, afxVectorialMotion const vectors[], afxUnit fetchRate);
ASX void        AfxUpdateMotionTransforms(afxMotion mot, afxUnit baseSeqIdx, afxUnit seqCnt, afxPivotalMotion const transforms[], afxUnit fetchRate);

////////////////////////////////////////////////////////////////////////////////

ASX afxError    AfxAssembleMotions(afxSimulation sim, afxUnit cnt, afxMotionBlueprint const blueprints[], afxMotion motions[]);

#endif//ASX_MOTION_H
