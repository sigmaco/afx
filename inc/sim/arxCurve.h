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

// Accelerated curve cosputation

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

#ifndef ARX_CURVE_H
#define ARX_CURVE_H

#include "qwadro/inc/math/afxSphere.h"
#include "qwadro/inc/render/arxRenderable.h"

typedef enum arxCurveFlag
{
    arxCurveFlag_IDENTITY   = AFX_BITMASK(0),
    arxCurveFlag_CONSTANT   = AFX_BITMASK(1),
    arxCurveFlag_KEYFRAMED  = AFX_BITMASK(2),
    arxCurveFlag_NORMALIZED = AFX_BITMASK(3),
} arxCurveFlags;

typedef enum arxCurveFormat
{
    arxCurveFormat_DaKeyframes32f, // Da, Keyframes32f
    arxCurveFormat_DaK32fC32f, // Da, K32f, C32f
    arxCurveFormat_DaIdentity, // Da, Identity
    arxCurveFormat_DaConstant32f, // Da, Constant32f
    arxCurveFormat_D3Constant32f, // D3, Constant32f
    arxCurveFormat_D4Constant32f, // D4, Constant32f
    // still unsupported
    arxCurveFormat_DaK16uC16u, // Da, K16u, C16u
    arxCurveFormat_DaK8uC8u, // Da, K8u, C8u
    arxCurveFormat_D4nK16uC15u, // D4n, K16u, C15u
    arxCurveFormat_D4nK8uC7u, // D4n, K8u, C7u
    arxCurveFormat_D3K16uC16u, // D3, K16u, C16u
    arxCurveFormat_D3K8uC8u, // D3, K8u, C8u
    arxCurveFormat_D9i1K16uC16u, // D9i1, K16u, C16u    
    arxCurveFormat_D9i3K16uC16u, // D9i3, K16u, C16u
    arxCurveFormat_D9i1K8uC8u, // D9i1, K8u, C8u
    arxCurveFormat_D9i3K8uC8u, // D9i3, K8u, C8u
    arxCurveFormat_D3i1K32fC32f, // D3i1, K32f, C32f
    arxCurveFormat_D3i1K16uC16u, // D3i1, K16u, C16u
    arxCurveFormat_D3i1K8uC8u // D3i1, K8u, C8u    
} arxCurveFormat;

AFX_DEFINE_STRUCT(arxCurveBlueprint)
{
    afxUnit         fmt;
    afxUnit         degree;
    afxUnit         dimens;
    afxUnit         knotCnt;
    afxReal const*  knotArray;
    afxReal const*  ctrlArray;
    arxCurve        srcCurve;
    afxUnit         sampleCnt;
    afxUnit         sampleDimension;
    afxReal const*  transformedSamples;
    afxReal const*  originalSamples;
    afxBool         curveBuilderNeedsFreeing;
};

AFX_DEFINE_STRUCT(arxCurveInfo2)
{
    arxCurveFormat  fmt;
    arxCurveFlags   flags;
    afxUnit         degree;
    afxUnit         dimens;
    afxUnit         knotCnt;
    afxUnit         ctrlCnt;
};

AFX_DEFINE_STRUCT(arxCurveInfo)
{
    arxCurveFormat  fmt;
    afxUnit         degree;
    afxUnit         dimens;
    afxUnit         knotCnt;
    afxReal const*  knots;
    afxReal const*  ctrls;

    arxCurve        src;
    afxUnit         sampleCnt;
    afxUnit         sampleDim;
    afxReal const*  xformedSamples;
    afxReal const*  origSamples;
};

AFX_DEFINE_STRUCT(arxSampleContext)
{
    afxReal localClock;
    afxReal localDur;
    afxBool underflowLoop;
    afxBool overflowLoop;
    afxInt frameIdx;
};

ARX void            ArxDescribeCurve(arxCurve cur, arxCurveInfo2* desc);

ARX afxInt          ArxGetCurveDegree(arxCurve c);
ARX afxUnit         ArxGetCurveDimensionality(arxCurve c);
ARX afxUnit         ArxGetCurveDimensionalityUnchecked(arxCurve c);

ARX arxCurveFlags   ArxGetCurveFlags(arxCurve c);
ARX arxCurveFlags   ArxTestCurveFlags(arxCurve c, arxCurveFlags mask);

ARX afxBool         ArxIsCurveIdentity(arxCurve c);
ARX afxBool         ArxIsCurveKeyframed(arxCurve c);
ARX afxBool         ArxIsCurveConstantOrIdentity(arxCurve c);
ARX afxBool         ArxIsCurveConstantNotIdentity(arxCurve c);

ARX afxReal*        ArxGetCurveKnots(arxCurve c);
ARX afxReal*        ArxGetCurveControls(arxCurve c);

ARX afxUnit         ArxFindCurveKnot(arxCurve c, afxReal t);

ARX afxUnit         ArxCountCurveKnots(arxCurve c);

ARX void            ArxUpdateCurveKnots(arxCurve c, afxUnit knotCnt, afxUnit dimension, afxReal const* knotSrc, afxReal const* ctrlSrc);
ARX void            ArxExtractCurveKnotValues(arxCurve c, afxUnit knotIdx, afxUnit knotCnt, afxReal* knotResults, afxReal* ctrlResults, afxReal const* identivec);
ARX afxReal         ArxExtractCurveKnotValue(arxCurve c, afxUnit knotIdx, afxReal* ctrlRslt, afxReal const* identity);

ARX void            ArxEvaluateCurveAtKnot(arxCurve c, afxUnit dimens, afxBool normalize, afxBool bwdsLoop, afxBool fwdsLoop, afxReal curveDur, afxUnit knotIdx, afxReal t, afxReal* rslt, afxReal const* identityVec);
ARX void            ArxEvaluateCurveAt(arxCurve c, afxUnit dimens, afxBool normalize, afxBool bwdsLoop, afxBool fwdsLoop, afxReal curveDur, afxReal t, afxReal* rslt, afxReal const* identityVec);

ARX void            ArxCopyCurve(arxCurve c, arxCurve src);

ARX void            ArxSetUpCurve(arxCurve c, arxCurveFormat fmt, afxUnit degree, afxUnit dim, afxUnit knotCnt);
ARX void            ArxCleanUpCurve(arxCurve c);

ARX void            ArxMakeCurveDaKC32f(arxCurve c, afxUnit degree, afxUnit dim, afxUnit knotCnt, afxReal const knots[], afxReal const ctrls[]);


ARX void            ArxBeginCurveCopy(arxCurveBlueprint* cb, arxCurve src);
ARX void            ArxBeginCurve(arxCurveBlueprint* cb, arxCurveFormat fmt, afxUnit degree, afxUnit dimension, afxUnit knotCnt);
ARX arxCurve        ArxEndCurve(arxCurveBlueprint* cb, afxSimulation sim);

ARXINL void         ArxResetCurveInfo(arxCurveInfo* cb, arxCurveFormat fmt, afxUnit degree, afxUnit dimens, afxUnit knotCnt);

////////////////////////////////////////////////////////////////////////////////

ARX afxError        ArxAcquireCurves(afxSimulation sim, afxUnit cnt, arxCurveInfo const info[], arxCurve curves[]);
ARX afxError        ArxAssembleCurves(afxSimulation sim, afxUnit cnt, arxCurveBlueprint const blueprints[], arxCurve curves[]);

#endif//ARX_CURVE_H
