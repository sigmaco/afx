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

#ifndef ARX_MOTION_H
#define ARX_MOTION_H

#include "qwadro/inc/render/anim/arxCurve.h"
#include "qwadro/inc/math/afxTransform.h"
#include "qwadro/inc/base/afxFixedString.h"

typedef enum arxMotionFlag
{
    arxMotionFlag_NONE
} arxMotionFlags;

AFX_DEFINE_STRUCT(arxVectorialMotion)
{
    afxUnit             seqKey;
    afxInt              dimension;
    arxCurve            value;
};

AFX_DEFINE_STRUCT(arxPivotalMotion)
{
    afxMask             flags;
    arxCurve            transmission;
    arxCurve            translation;
    arxCurve            transmutation;
};

AFX_DEFINE_STRUCT(arxMotionBlueprint)
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

ARX afxMask     ArxGetMotionFlags(arxMotion mot);

ARX afxBool     ArxGetMotionId(arxMotion mot, afxString* id);

ARX void        ArxComputeMotionDisplacement(arxMotion mot, afxM4d m);

ARX afxBool     ArxFindMotionVector(arxMotion mot, afxString const* seqId, afxUnit *seqIdx);
ARX afxBool     ArxFindMotionTransform(arxMotion mot, afxString const* seqId, afxUnit *seqIdx);

ARX void        ArxUpdateMotionVectors(arxMotion mot, afxUnit baseSeqIdx, afxUnit seqCnt, arxVectorialMotion const vectors[], afxUnit fetchRate);
ARX void        ArxUpdateMotionTransforms(arxMotion mot, afxUnit baseSeqIdx, afxUnit seqCnt, arxPivotalMotion const transforms[], afxUnit fetchRate);

////////////////////////////////////////////////////////////////////////////////

ARX afxError    ArxAssembleMotions(afxSimulation sim, afxUnit cnt, arxMotionBlueprint const blueprints[], arxMotion motions[]);

#endif//ARX_MOTION_H
