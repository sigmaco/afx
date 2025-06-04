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

// Hardware-accelerated curve cosputation

// A curve is a continuous and smooth mathematical object representing a set of points that form a shape or path in a given space.

/*
    Knot vector (uniform or not) is part of the definition of a NURBS curve. 
    So, you can actually define your own non-uniform knot vector as long as the knot vector follows the basic rules:

    1) # of knot values = # of control points + order

    2) All knot values must be non-decreasing. Namely, k[i] <= k[i+1].

    For example, with 9 control points, you can have non-uniform knot vector like [0, 0, 0, 0, a, b, c, d, e, 1, 1, 1, 1], where 0.0 < a <= b <= c <=d <=e < 1.0 for a degree 3 B-spline curve.
    Of course, choosing different values for a, b, c, d and e will result in curves with different shape.
*/

// Knots are the things that make the curve continuous, like in Bezier curves, 
// the line segment from the mid point control point have to have the same tangent 
// and length and if they are then the Bezier curves are c-continuous.

#ifndef ASX_CURVE_H
#define ASX_CURVE_H

#include "qwadro/inc/math/bound/afxSphere.h"
#include "qwadro/inc/sim/afxSimDefs.h"

typedef enum afxCurveFlag
{
    afxCurveFlag_IDENTITY   = AFX_BITMASK(0),
    afxCurveFlag_CONSTANT   = AFX_BITMASK(1),
    afxCurveFlag_KEYFRAMED  = AFX_BITMASK(2),
    afxCurveFlag_NORMALIZED = AFX_BITMASK(3),
} afxCurveFlags;

typedef enum afxCurveFormat
{
    afxCurveFormat_DaKeyframes32f, // Da, Keyframes32f
    afxCurveFormat_DaK32fC32f, // Da, K32f, C32f
    afxCurveFormat_DaIdentity, // Da, Identity
    afxCurveFormat_DaConstant32f, // Da, Constant32f
    afxCurveFormat_D3Constant32f, // D3, Constant32f
    afxCurveFormat_D4Constant32f, // D4, Constant32f
    // still unsupported
    afxCurveFormat_DaK16uC16u, // Da, K16u, C16u
    afxCurveFormat_DaK8uC8u, // Da, K8u, C8u
    afxCurveFormat_D4nK16uC15u, // D4n, K16u, C15u
    afxCurveFormat_D4nK8uC7u, // D4n, K8u, C7u
    afxCurveFormat_D3K16uC16u, // D3, K16u, C16u
    afxCurveFormat_D3K8uC8u, // D3, K8u, C8u
    afxCurveFormat_D9i1K16uC16u, // D9i1, K16u, C16u    
    afxCurveFormat_D9i3K16uC16u, // D9i3, K16u, C16u
    afxCurveFormat_D9i1K8uC8u, // D9i1, K8u, C8u
    afxCurveFormat_D9i3K8uC8u, // D9i3, K8u, C8u
    afxCurveFormat_D3i1K32fC32f, // D3i1, K32f, C32f
    afxCurveFormat_D3i1K16uC16u, // D3i1, K16u, C16u
    afxCurveFormat_D3i1K8uC8u // D3i1, K8u, C8u    
} afxCurveFormat;

AFX_DEFINE_STRUCT(afxCurveBlueprint)
{
    afxUnit         fmt;
    afxUnit         degree;
    afxUnit         dimens;
    afxUnit         knotCnt;
    afxReal const*  knotArray;
    afxReal const*  ctrlArray;
    afxCurve        srcCurve;
    afxUnit         sampleCnt;
    afxUnit         sampleDimension;
    afxReal const*  transformedSamples;
    afxReal const*  originalSamples;
    afxBool         curveBuilderNeedsFreeing;
};

AFX_DEFINE_STRUCT(afxCurveInfo2)
{
    afxCurveFormat  fmt;
    afxCurveFlags   flags;
    afxUnit         degree;
    afxUnit         dimens;
    afxUnit         knotCnt;
    afxUnit         ctrlCnt;
};

AFX_DEFINE_STRUCT(afxCurveInfo)
{
    afxCurveFormat  fmt;
    afxUnit         degree;
    afxUnit         dimens;
    afxUnit         knotCnt;
    afxReal const*  knots;
    afxReal const*  ctrls;

    afxCurve        src;
    afxUnit         sampleCnt;
    afxUnit         sampleDim;
    afxReal const*  xformedSamples;
    afxReal const*  origSamples;
};

AFX_DEFINE_STRUCT(afxSampleContext)
{
    afxReal localClock;
    afxReal localDur;
    afxBool underflowLoop;
    afxBool overflowLoop;
    afxInt frameIdx;
};

ASX void AfxDescribeCurve(afxCurve cur, afxCurveInfo2* desc);

ASX afxInt          AfxGetCurveDegree(afxCurve c);
ASX afxUnit         AfxGetCurveDimensionality(afxCurve c);
ASX afxUnit         AfxGetCurveDimensionalityUnchecked(afxCurve c);

ASX afxCurveFlags   AfxGetCurveFlags(afxCurve c);
ASX afxCurveFlags   AfxTestCurveFlags(afxCurve c, afxCurveFlags mask);

ASX afxBool         AfxIsCurveIdentity(afxCurve c);
ASX afxBool         AfxIsCurveKeyframed(afxCurve c);
ASX afxBool         AfxIsCurveConstantOrIdentity(afxCurve c);
ASX afxBool         AfxIsCurveConstantNotIdentity(afxCurve c);

ASX afxReal*        AfxGetCurveKnots(afxCurve c);
ASX afxReal*        AfxGetCurveControls(afxCurve c);

ASX afxUnit         AfxFindCurveKnot(afxCurve c, afxReal t);

ASX afxUnit         AfxCountCurveKnots(afxCurve c);

ASX void            AfxUpdateCurveKnots(afxCurve c, afxUnit knotCnt, afxUnit dimension, afxReal const* knotSrc, afxReal const* ctrlSrc);
ASX void            AfxExtractCurveKnotValues(afxCurve c, afxUnit knotIdx, afxUnit knotCnt, afxReal* knotResults, afxReal* ctrlResults, afxReal const* identivec);
ASX afxReal         AfxExtractCurveKnotValue(afxCurve c, afxUnit knotIdx, afxReal* ctrlRslt, afxReal const* identity);

ASX void            AfxCopyCurve(afxCurve c, afxCurve src);

ASX void            AfxSetUpCurve(afxCurve c, afxCurveFormat fmt, afxUnit degree, afxUnit dim, afxUnit knotCnt);
ASX void            AfxCleanUpCurve(afxCurve c);

ASX void            AfxMakeCurveDaKC32f(afxCurve c, afxUnit degree, afxUnit dim, afxUnit knotCnt, afxReal const knots[], afxReal const ctrls[]);


ASX void            AfxBeginCurveCopy(afxCurveBlueprint* cb, afxCurve src);
ASX void            AfxBeginCurve(afxCurveBlueprint* cb, afxCurveFormat fmt, afxUnit degree, afxUnit dimension, afxUnit knotCnt);
ASX afxCurve        AfxEndCurve(afxCurveBlueprint* cb, afxSimulation sim);

ASXINL void         AfxResetCurveInfo(afxCurveInfo* cb, afxCurveFormat fmt, afxUnit degree, afxUnit dimens, afxUnit knotCnt);

////////////////////////////////////////////////////////////////////////////////

ASX afxError        AfxAcquireCurves(afxSimulation sim, afxUnit cnt, afxCurveInfo const info[], afxCurve curves[]);
ASX afxError        AfxAssembleCurves(afxSimulation sim, afxUnit cnt, afxCurveBlueprint const blueprints[], afxCurve curves[]);

#endif//ASX_CURVE_H
