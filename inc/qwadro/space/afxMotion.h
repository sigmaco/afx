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

#ifndef AFX_MOTION_H
#define AFX_MOTION_H

#include "qwadro/sim/afxSimDefs.h"
#include "qwadro/space/afxCurve.h"
#include "qwadro/math/afxTransform.h"
#include "qwadro/base/afxFixedString.h"

AFX_DEFINE_STRUCT(akxPeriodicLoop)
{
    afxReal             radius;
    afxReal             dAngle;
    afxReal             dZ;
    afxV3d              basisX;
    afxV3d              basisY;
    afxV3d              axis;
};

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

#ifdef _AFX_MOTION_C
AFX_OBJECT(afxMotion)
{
    afxString           id;
    afxStringBase       strb;
    afxMask             flags;
    afxNat              pivotCnt; // circuit count
    akxMotionTransform* pivotCurve; // uma arranjo de registros da moção de junta de afxSkeleton do correspondente afxModel.
    afxString*          pivotId; // circuit id compatible with skeleton joints.
    //afxNat              pivotLodErrorCnt;
    afxReal*            pivotLodError;
    afxNat              vecCnt;
    akxMotionVector*    vecCurve;
    afxString*          vecId;
    afxTransform        displacement;
    akxMotionTransform* root;
    afxV3d              loopTranslation;
    akxPeriodicLoop*    periodicLoop;    

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

AFX_DEFINE_STRUCT(akxMotionBlueprint)
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

AKX afxBool             AfxGetMotionId(afxMotion mot, afxString* id);

AKX void                AfxComputeMotionDisplacement(afxMotion mot, afxM4d m);

AKX afxBool             AfxFindMotionVector(afxMotion mot, afxString const* seqId, afxNat *seqIdx);
AKX afxBool             AfxFindMotionTransform(afxMotion mot, afxString const* seqId, afxNat *seqIdx);

AKX void                AfxUpdateMotionVectors(afxMotion mot, afxNat baseSeqIdx, afxNat seqCnt, akxMotionVector const vectors[], afxNat fetchRate);
AKX void                AfxUpdateMotionTransforms(afxMotion mot, afxNat baseSeqIdx, afxNat seqCnt, akxMotionTransform const transforms[], afxNat fetchRate);

////////////////////////////////////////////////////////////////////////////////

AKX afxError            AfxAssembleMotions(afxSimulation sim, afxNat cnt, akxMotionBlueprint const blueprints[], afxMotion motions[]);

#endif//AFX_MOTION_H
