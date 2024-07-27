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

// Hardware-accelerated curve computation

/*
    Knot vector (uniform or not) is part of the definition of a NURBS curve. So, you can actually define your own non-uniform knot vector as long as the knot vector follows the basic rules:

    1) # of knot values = # of control points + order

    2) All knot values must be non-decreasing. Namely, k[i] <= k[i+1].

    For example, with 9 control points, you can have non-uniform knot vector like [0, 0, 0, 0, a, b, c, d, e, 1, 1, 1, 1], where 0.0 < a <= b <= c <=d <=e < 1.0 for a degree 3 B-spline curve.
    Of course, choosing different values for a, b, c, d and e will result in curves with different shape.
*/

/// Knots are the things that make the curve continuous, like in Bezier curves, the line segment from the mid point control point have to have the same tangent and length and if they are then the Bezier curves are c-continuous.

#ifndef AKX_CURVE_H
#define AKX_CURVE_H

#include "qwadro/math/afxSphere.h"
#include "qwadro/sim/afxSimDefs.h"

typedef enum akxCurveFlag
{
    akxCurveFlag_IDENTITY   = AFX_BIT(0),
    akxCurveFlag_CONSTANT   = AFX_BIT(1),
    akxCurveFlag_KEYFRAMED  = AFX_BIT(2),
    akxCurveFlag_NORMALIZED = AFX_BIT(3),
} akxCurveFlags;

typedef enum akxCurveFormat
{
    akxCurveFormat_DaKeyframes32f, /// Da, Keyframes32f
    akxCurveFormat_DaK32fC32f, /// Da, K32f, C32f
    akxCurveFormat_DaIdentity, /// Da, Identity
    akxCurveFormat_DaConstant32f, /// Da, Constant32f
    akxCurveFormat_D3Constant32f, /// D3, Constant32f
    akxCurveFormat_D4Constant32f, /// D4, Constant32f
    // still unsupported
    akxCurveFormat_DaK16uC16u, /// Da, K16u, C16u
    akxCurveFormat_DaK8uC8u, /// Da, K8u, C8u
    akxCurveFormat_D4nK16uC15u, /// D4n, K16u, C15u
    akxCurveFormat_D4nK8uC7u, /// D4n, K8u, C7u
    akxCurveFormat_D3K16uC16u, /// D3, K16u, C16u
    akxCurveFormat_D3K8uC8u, /// D3, K8u, C8u
    akxCurveFormat_D9i1K16uC16u, /// D9i1, K16u, C16u    
    akxCurveFormat_D9i3K16uC16u, /// D9i3, K16u, C16u
    akxCurveFormat_D9i1K8uC8u, /// D9i1, K8u, C8u
    akxCurveFormat_D9i3K8uC8u, /// D9i3, K8u, C8u
    akxCurveFormat_D3i1K32fC32f, /// D3i1, K32f, C32f
    akxCurveFormat_D3i1K16uC16u, /// D3i1, K16u, C16u
    akxCurveFormat_D3i1K8uC8u /// D3i1, K8u, C8u    
} akxCurveFormat;

AFX_DEFINE_STRUCT(akxCurveBlueprint)
{
    afxNat          fmt;
    afxNat          degree;
    afxNat          dimensionality;
    afxNat          knotCnt;
    afxReal const*  knotArray;
    afxReal const*  ctrlArray;
    afxCurve        srcCurve;
    afxNat          sampleCnt;
    afxNat          sampleDimension;
    afxReal const*  transformedSamples;
    afxReal const*  originalSamples;
    afxBool         curveBuilderNeedsFreeing;
};

AFX_DEFINE_STRUCT(akxCurveInfo)
{
    akxCurveFormat  fmt;
    afxNat          degree;
    afxNat          dimensionality;
    afxNat          knotCnt;
    afxReal const*  knots;
    afxReal const*  ctrls;

    afxCurve        src;
    afxNat          sampleCnt;
    afxNat          sampleDim;
    afxReal const*  xformedSamples;
    afxReal const*  origSamples;
};

AKX afxInt          AfxGetCurveDegree(afxCurve c);
AKX afxNat          AfxGetCurveDimensionality(afxCurve c);
AKX afxNat          AfxGetCurveDimensionalityUnchecked(afxCurve c);

AKX akxCurveFlags   AfxGetCurveFlags(afxCurve c);
AKX akxCurveFlags   AfxTestCurveFlags(afxCurve c, akxCurveFlags mask);

AKX afxBool         AfxCurveIsIdentity(afxCurve c);
AKX afxBool         AfxCurveIsKeyframed(afxCurve c);
AKX afxBool         AfxCurveIsConstantOrIdentity(afxCurve c);
AKX afxBool         AfxCurveIsConstantNotIdentity(afxCurve c);

AKX afxReal*        AfxGetCurveKnots(afxCurve c);
AKX afxReal*        AfxGetCurveControls(afxCurve c);

AKX afxNat          AfxFindCurveKnot(afxCurve c, afxReal t);

AKX afxNat          AfxCountCurveKnots(afxCurve c);

AKX void            AfxSetCurveAllKnotValues(afxCurve c, afxNat knotCnt, afxNat dimension, afxReal const* knotSrc, afxReal const* ctrlSrc);
AKX void            AfxExtractCurveKnotValues(afxCurve c, afxNat knotIdx, afxNat knotCnt, afxReal* knotResults, afxReal* ctrlResults, afxReal const* identivec);
AKX afxReal         AfxExtractCurveKnotValue(afxCurve c, afxNat knotIdx, afxReal* ctrlRslt, afxReal const* identity);

AKX void            AfxEvaluateCurveAtKnot(afxCurve c, afxNat dimensionality, afxBool normalize, afxBool bwdsLoop, afxBool fwdsLoop, afxReal curveDur, afxNat knotIdx, afxReal t, afxReal* rslt, afxReal const* identityVec);
AKX void            AfxEvaluateCurveAt(afxCurve c, afxNat dimensionality, afxBool normalize, afxBool bwdsLoop, afxBool fwdsLoop, afxReal curveDur, afxReal t, afxReal* rslt, afxReal const* identityVec);

AKX void            AfxCopyCurve(afxCurve c, afxCurve src);

AKX void            AfxSetUpCurve(afxCurve c, akxCurveFormat fmt, afxNat degree, afxNat dim, afxNat knotCnt);
AKX void            AfxCleanUpCurve(afxCurve c);

AKX void            AfxMakeCurveDaKC32f(afxCurve c, afxNat degree, afxNat dim, afxNat knotCnt, afxReal const knots[], afxReal const ctrls[]);


AKX void            AfxBeginCurveCopy(akxCurveBlueprint* cb, afxCurve src);
AKX void            AfxBeginCurve(akxCurveBlueprint* cb, akxCurveFormat fmt, afxNat degree, afxNat dimension, afxNat knotCnt);
AKX afxCurve        AfxEndCurve(akxCurveBlueprint* cb, afxSimulation sim);

AKXINL void         AfxResetCurveInfo(akxCurveInfo* cb, akxCurveFormat fmt, afxNat degree, afxNat dimensionality, afxNat knotCnt);

////////////////////////////////////////////////////////////////////////////////

AKX afxError        AfxAcquireCurves(afxSimulation sim, afxNat cnt, akxCurveInfo const info[], afxCurve curves[]);
AKX afxError        AfxAssembleCurves(afxSimulation sim, afxNat cnt, akxCurveBlueprint const blueprints[], afxCurve curves[]);

#endif//AKX_CURVE_H
