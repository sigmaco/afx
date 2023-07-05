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
 *                      S I G M A   T E C H N O L O G Y   G R O U P
 *
 *                                   Public Test Build
 *                      (c 2017 SIGMA Co. & SIGMA Technology Group
 *                                    www.sigmaco.org
 */

#ifndef AFX_CURVE2_H
#define AFX_CURVE2_H

 // Terminology: BodyControl is a movable model of a person or animal that is used in entertainment and is typically moved either by strings controlled from above or by a hand inside it.

 // There are two primary objects involved in the run-time animation layer : the afxBody and the afxBodyControl.
 // Together, they represent the two ends of the animation process.
 // The afxBody represents the state of a single model that may have some number of animations affecting it, 
 // whereas the afxBodyControl represents the state of an animation that may be affecting some number of models.
 // When your engine creates a new animating entity in the game world, you create a afxBody to accompany it.
 // When you want to play an animation on such an instance, you create a afxBodyControl.

#include "afx/math/afxMathDefs.h"

AFX_DEFINE_STRUCT(afxCurveBlueprint)
{
    int a;
};

AFX_DEFINE_STRUCT(afxCurveDataHeader)
{
    afxNat8 Format;
    afxNat8 Degree;
};

AFX_DEFINE_STRUCT(afxCurveDataDaK32fC32f)
{
    afxCurveDataHeader CurveDataHeader;
    afxInt16 Padding;
    afxInt KnotCount;
    afxReal* Knots;
    afxInt ControlCount;
    afxReal* Controls;
};

AFX_DEFINE_STRUCT(afxCurve2)
{
    struct
    {
        afxNat Type;
        void * Object;
    } CurveData;
};

AFX afxCurveBlueprint* AfxBeginCurve(void const * TypeDefinition, afxInt Degree, afxInt Dimension, afxInt KnotCount);

AFX afxCurveBlueprint* AfxBeginCurveCopy(afxCurve2 const*SourceCurve);
AFX void        AfxPushCurveKnotArray(afxCurveBlueprint *Builder, afxReal const * KnotArray);
AFX void        AfxPushCurveControlArray(afxCurveBlueprint *Builder, afxReal const * ControlArray);
AFX void        AfxPushCurveSampleArrays(afxCurveBlueprint *Builder, afxInt SampleCount, afxInt Dimension, afxReal const * TransformedSamples, afxReal const * OriginalSamples);
AFX afxInt      AfxGetResultingCurveSize(afxCurveBlueprint const*Builder);
AFX afxCurve2*  AfxEndCurveInPlace(afxCurveBlueprint *Builder, void * Memory);
AFX afxCurve2*  AfxEndCurve(afxCurveBlueprint *Builder);
AFX void        AfxAbortCurveBuilder(afxCurveBlueprint *Builder);
AFX afxInt      AfxGetResultingCurveDataSize(afxCurveBlueprint const*Builder);
AFX afxCurve2*  AfxEndCurveDataInPlace(afxCurveBlueprint *Builder, afxCurve2 * Curve, void * CurveDataMemory);

AFX void        AfxCurveInitializeFormat(afxCurve2 *curv);
AFX afxBool     AfxCurveFormatIsInitializedCorrectly(afxCurve2 const *curv, afxBool CheckTypes);
AFX afxBool     AfxCurveIsKeyframed(afxCurve2 const *curv);
AFX afxBool     AfxCurveIsIdentity(afxCurve2 const *curv);
AFX afxBool     AfxCurveIsConstantOrIdentity(afxCurve2 const *curv);
AFX afxBool     AfxCurveIsConstantNotIdentity(afxCurve2 const *curv);
AFX afxInt      AfxCurveGetKnotCount(afxCurve2 const *curv);
AFX afxInt      AfxCurveGetDimension(afxCurve2 const *curv);
AFX afxInt      AfxCurveGetDegree(afxCurve2 const *curv);
AFX void        AfxFreeCurve(afxCurve2 *curv);
AFX afxBool     AfxCurveIsTypeDaK32fC32f(afxCurve2 const*SrcCurve);
AFX afxCurveDataDaK32fC32f* AfxCurveGetContentsOfDaK32fC32f(afxCurve2 const*SrcCurve);
AFX afxCurve2*  AfxCurveConvertToDaK32fC32f(afxCurve2 const*SrcCurve, afxReal const * IdentityVector);
AFX afxInt      AfxGetResultingDaK32fC32fCurveSize(afxCurve2 const*SrcCurve);
AFX afxCurve2*  AfxCurveConvertToDaK32fC32fInPlace(afxCurve2 const*SrcCurve, void * Memory, afxReal const * IdentityVector);
AFX void        AfxCurveMakeStaticDaK32fC32f(afxCurve2 *curv, afxCurveDataDaK32fC32f* CurveData, afxInt KnotCount, afxInt Degree, afxInt Dimension, afxReal const * Knots, afxReal const * Controls);
AFX afxReal     AfxCurveExtractKnotValue(afxCurve2 const *curv, afxInt KnotIndex, afxReal * ControlResult, afxReal const * IdentityVector);
AFX void        AfxCurveExtractKnotValues(afxCurve2 const *curv, afxInt KnotIndexStart, afxInt KnotCount, afxReal * KnotResults, afxReal * ControlResults, afxReal const * IdentityVector);
AFX void        AfxCurveSetAllKnotValues(afxCurve2 *curv, afxInt KnotCount, afxInt Dimension, afxReal const * KnotSrc, afxReal const * ControlSrc);
AFX void        AfxCurveScaleOffsetSwizzle(afxCurve2 *curv, afxInt Dimension, afxReal const * Scales, afxReal const * Offsets, afxInt const * Swizzles);
AFX afxInt      AfxCurveFindKnot(afxCurve2 const *curv, afxReal t);
AFX afxInt      AfxCurveFindCloseKnot(afxCurve2 const *curv, afxReal t, afxInt StartingKnotIndex);
AFX afxInt      AfxCurveGetSize(afxCurve2 const *curv);

#endif//AFX_CURVE2_H