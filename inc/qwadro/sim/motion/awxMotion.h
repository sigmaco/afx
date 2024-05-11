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
 *                      (c 2017 SIGMA Co. & SIGMA Technology Group
 *                             <https://sigmaco.org/qwadro/>
 */

#ifndef AFX_MOTION_H
#define AFX_MOTION_H

#include "qwadro/sim/awxSimDefs.h"
#include "afxCurve2.h"
#include "qwadro/math/afxTransform.h"
#include "qwadro/core/afxFixedString.h"

AFX_DEFINE_STRUCT(awxPeriodicLoop)
{
    afxReal             radius;
    afxReal             dAngle;
    afxReal             dZ;
    afxV3d              basisX;
    afxV3d              basisY;
    afxV3d              axis;
};

AFX_DEFINE_STRUCT(awxMotionVector)
{
    //afxString           id;
    afxNat              seqKey;
    afxInt              dimension;
    afxCurve            value;
};

AFX_DEFINE_STRUCT(awxMotionTransform)
{
    //afxString           id;
    afxMask             flags;
    afxCurve            transmission;
    afxCurve            translation;
    afxCurve            transmutation;
};

#ifdef _AFX_MOTION_C
AFX_OBJECT(awxMotion)
{
    afxString           id;
    afxStringBase       strb;
    afxMask             flags;
    afxNat              pivotCnt; // circuit count
    awxMotionTransform* pivotCurve; // uma arranjo de registros da moção de junta de afxSkeleton do correspondente afxModel.
    afxString*          pivotId; // circuit id compatible with skeleton joints.
    //afxNat              pivotLodErrorCnt;
    afxReal*            pivotLodError;
    afxNat              vecCnt;
    awxMotionVector*    vecCurve;
    afxString*          vecId;
    afxTransform        displacement;
    awxMotionTransform* root;
    afxV3d              loopTranslation;
    awxPeriodicLoop*    periodicLoop;    

#if 0
    afxNat              curveCnt;
    afxCurve*           curves;
    struct
    {
        afxNat          baseCurIdx;
        afxNat          curCnt;
    }*                  xforms;
    afxMask*            xformFlag;
    afxCurve*           deformation;
    afxCurve*           translation;
    afxCurve*           transmission;
    afxString*          articulation;
#endif
};
#endif

AFX_DEFINE_STRUCT(awxMotionBlueprint)
{
    afxString32         id;
    afxStringBase       strb;
    afxNat              vecCnt;
    afxString const*    vecId;
    afxNat              pivotCnt;
    afxString const*    pivotId;
    afxReal const*      pivotLodError;
    afxBool             incPivotLodError;
    afxTransform        displacement;
};

AKX afxBool             AfxGetMotionId(awxMotion mot, afxString* id);

AKX void                AfxComputeMotionDisplacement(awxMotion mot, afxM4d m);

AKX afxBool             AfxFindMotionVector(awxMotion mot, afxString const* seqId, afxNat *seqIdx);
AKX afxBool             AfxFindMotionTransform(awxMotion mot, afxString const* seqId, afxNat *seqIdx);

AKX void                AfxUpdateMotionVectors(awxMotion mot, afxNat baseSeqIdx, afxNat seqCnt, awxMotionVector const vectors[], afxNat fetchRate);
AKX void                AfxUpdateMotionTransforms(awxMotion mot, afxNat baseSeqIdx, afxNat seqCnt, awxMotionTransform const transforms[], afxNat fetchRate);

////////////////////////////////////////////////////////////////////////////////

AKX afxError            AfxAssembleMotions(afxSimulation sim, afxNat cnt, awxMotionBlueprint const blueprints[], awxMotion motions[]);

#endif//AFX_MOTION_H
