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
#include "qwadro/math/afxCurve.h"
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
    afxString           id;
    afxNat              seqKey;
    afxInt              dimension;
    afxCurve            value;
};

AFX_DEFINE_STRUCT(awxMotionTransform)
{
    //afxString           id;
    afxMask             flags;
    afxCurve            orientation;
    afxCurve            translation;
    afxCurve            deformation;
};

#ifdef _AFX_MOTION_C
AFX_OBJECT(awxMotion)
{
    afxString           id;
    afxStringCatalog    strc;
    afxMask             flags;
    afxNat              xformSeqCnt; // circuit count
    awxMotionTransform* xformSeqs; // uma arranjo de registros da moção de junta de afxSkeleton do correspondente afxModel.
    afxString*          xformId; // circuit id compatible with skeleton joints.
    afxNat              xformLodErrorCnt;
    afxReal*            xformLodErrors;
    afxNat              vecSeqCnt;
    awxMotionVector*    vecSeqs;
    afxTransform        init;
    awxMotionTransform* root;
    afxV3d              loopTranslation;
    awxPeriodicLoop*    periodicLoop;    

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
};
#endif

AFX_DEFINE_STRUCT(awxMotionBlueprint)
{
    afxString32         id;
    afxStringCatalog    strc;
    afxNat              vecCnt;
    afxNat              xformCnt;
    afxString const*    xformId;
    afxReal const*      xformLodError;
};

AKX afxBool             AfxGetMotionId(awxMotion mot, afxString* id);

AKX void                AfxGetMotionInitialPlacement(awxMotion mot, afxReal m[4][4]);

AKX afxBool             AfxFindMotionVector(awxMotion mot, afxString const* seqId, afxNat *seqIdx);
AKX afxBool             AfxFindMotionTransform(awxMotion mot, afxString const* seqId, afxNat *seqIdx);

AKX void                AfxUpdateMotionVectors(awxMotion mot, afxNat baseSeqIdx, afxNat seqCnt, afxString const id[], afxNat const keys[], afxInt const dimensions[], afxCurve const values[], afxNat fetchRate);
AKX void                AfxUpdateMotionTransforms(awxMotion mot, afxNat baseSeqIdx, afxNat seqCnt, afxNat const flags[], afxCurve const orient[], afxCurve const pos[], afxCurve const scale[], afxNat fetchRate);

AKX afxError            AfxAssembleMotions(afxSimulation sim, afxNat cnt, awxMotionBlueprint const blueprints[], awxMotion motions[]);

#endif//AFX_MOTION_H
