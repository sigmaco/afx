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

#ifndef AKX_CURVE_H
#define AKX_CURVE_H

#include "qwadro/math/afxSphere.h"
#include "qwadro/sim/afxSimDefs.h"

/*
    Knot vector (uniform or not) is part of the definition of a NURBS curve. So, you can actually define your own non-uniform knot vector as long as the knot vector follows the basic rules:

    1) # of knot values = # of control points + order

    2) All knot values must be non-decreasing. Namely, k[i] <= k[i+1].

    For example, with 9 control points, you can have non-uniform knot vector like [0, 0, 0, 0, a, b, c, d, e, 1, 1, 1, 1], where 0.0 < a <= b <= c <=d <=e < 1.0 for a degree 3 B-spline curve.
    Of course, choosing different values for a, b, c, d and e will result in curves with different shape.
*/

/// Knots are the things that make the curve continuous, like in Bezier curves, the line segment from the mid point control point have to have the same tangent and length and if they are then the Bezier curves are c-continuous.

typedef enum afxCurveFormat
{
    afxCurveFormat_DaKeyframes32f, /// Da, Keyframes32f
    afxCurveFormat_DaK32fC32f, /// Da, K32f, C32f
    afxCurveFormat_DaIdentity, /// Da, Identity
    afxCurveFormat_DaConstant32f, /// Da, Constant32f
    afxCurveFormat_D3Constant32f, /// D3, Constant32f
    afxCurveFormat_D4Constant32f, /// D4, Constant32f
    afxCurveFormat_DaK16uC16u, /// Da, K16u, C16u
    afxCurveFormat_DaK8uC8u, /// Da, K8u, C8u
    afxCurveFormat_D4nK16uC15u, /// D4n, K16u, C15u
    afxCurveFormat_D4nK8uC7u, /// D4n, K8u, C7u
    afxCurveFormat_D3K16uC16u, /// D3, K16u, C16u
    afxCurveFormat_D3K8uC8u, /// D3, K8u, C8u
    afxCurveFormat_D9i1K16uC16u, /// D9i1, K16u, C16u    
    afxCurveFormat_D9i3K16uC16u, /// D9i3, K16u, C16u
    afxCurveFormat_D9i1K8uC8u, /// D9i1, K8u, C8u
    afxCurveFormat_D9i3K8uC8u, /// D9i3, K8u, C8u
    afxCurveFormat_D3i1K32fC32f, /// D3i1, K32f, C32f
    afxCurveFormat_D3i1K16uC16u, /// D3i1, K16u, C16u
    afxCurveFormat_D3i1K8uC8u /// D3i1, K8u, C8u    
} afxCurveFormat;

AFX_DEFINE_HANDLE(afxCurve);

AFX_OBJECT(afxCurve)
{
    afxCurveFormat  fmt;
    afxNat          degree;
    afxNat          dimensionality;
    afxNat          knotCnt;
    afxReal*        knots; // 32f
    afxNat          ctrlCnt;
    union
    {
        afxV4d      ctrls4; // D4, Constant32f
        afxV3d      ctrls3;
        afxReal*    ctrls; // 32f
    };
};

AFX_DEFINE_STRUCT(afxCurveBlueprint)
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

AFX_DEFINE_STRUCT(afxCurveInfo)
{
    afxCurveFormat  fmt;
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

AKX afxError        AfxAcquireCurves(afxSimulation sim, afxNat cnt, afxCurveInfo const info[], afxCurve curves[]);
AKX afxError        AfxAssembleCurves(afxSimulation sim, afxNat cnt,afxCurveBlueprint const blueprints[], afxCurve curves[]);


AKX afxInt          AfxGetCurveDegree(afxCurve c);
AKX afxNat          AfxGetCurveDimensionality(afxCurve c);
AKX afxNat          AfxGetCurveDimensionalityUnchecked(afxCurve c);

AKX afxBool         AfxCurveIsIdentity(afxCurve c);
AKX afxBool         AfxCurveIsKeyframed(afxCurve c);
AKX afxBool         AfxCurveIsConstantOrIdentity(afxCurve c);
AKX afxBool         AfxCurveIsConstantNotIdentity(afxCurve c);

AKX afxReal*        AfxGetCurveKnots(afxCurve c);
AKX afxReal*        AfxGetCurveControls(afxCurve c);

AKX afxNat          AfxFindCurveKnot(afxCurve c, afxReal t);

AKX afxNat          AfxCountCurveKnots(afxCurve c);
AKX afxReal         AfxExtractCurveKnotValue(afxCurve c, afxNat knotIdx, afxReal* ctrlRslt, afxReal const* identity);

AKX void            AfxSetCurveAllKnotValues(afxCurve c, afxNat knotCnt, afxNat dimension, afxReal const* knotSrc, afxReal const* ctrlSrc);
AKX void            AfxExtractCurveKnotValues(afxCurve c, afxNat knotIdx, afxNat knotCnt, afxReal* knotResults, afxReal* ctrlResults, afxReal const* identivec);

AKX void            AfxCopyCurve(afxCurve c, afxCurve src);

AKX void            AfxSetUpCurve(afxCurve c, afxCurveFormat fmt, afxNat degree, afxNat dim, afxNat knotCnt);
AKX void            AfxCleanUpCurve(afxCurve c);

AKX void            AfxMakeCurveDaKC32f(afxCurve c, afxNat degree, afxNat dim, afxNat knotCnt, afxReal const knots[], afxReal const ctrls[]);


AKX void            AfxBeginCurveCopy(afxCurveBlueprint* cb, afxCurve src);
AKX void            AfxBeginCurve(afxCurveBlueprint* cb, afxCurveFormat fmt, afxNat degree, afxNat dimension, afxNat knotCnt);
AKX afxCurve        AfxEndCurve(afxCurveBlueprint* cb, afxSimulation sim);

AKXINL void         AfxResetCurveInfo(afxCurveInfo* cb, afxCurveFormat fmt, afxNat degree, afxNat dimensionality, afxNat knotCnt);

////////////////////////////////////////////////////////////////////////////////

typedef struct sample_context
{
    afxReal LocalClock;
    afxReal LocalDuration;
    afxBool UnderflowLoop;
    afxBool OverflowLoop;
    afxInt FrameIndex;
} sample_context;

typedef struct smt2
{
    void(*UncheckedSampleBSplineN)(int Degree, int Dimension, const afxReal *ti, const afxReal *pi, afxReal t, afxReal *Result);
    void(*UncheckedSampleBSpline)(int Degree, int Dimension, const afxReal *ti, const afxReal *pi, afxReal t, afxReal *Result);
    int(*CurveKnotCharacter)(afxCurve Curve);
    void(*CubicCoefficients)(afxReal const a[3], afxReal const b[3], afxReal t, afxReal* ci_3, afxReal* ci_2, afxReal* ci_1, afxReal* ci);
    void(*SampleBSpline)(afxNat Degree, afxNat Dimension, afxBool Normalize, const afxReal *ti, const afxReal *pi, afxReal t, afxReal *Result);
    char(*ConstructBSplineBuffers)(afxInt dim, afxCurve prev, afxCurve c, afxCurve next, afxReal prevCurDur, afxReal curDur, afxReal nextCurDur, afxInt knotIdx, afxReal* ti, afxReal* pi, afxReal** tiPtr, afxReal** piPtr, afxReal const* identityVec);
    void(*EvaluateCurve)(const sample_context *Context, afxCurve Curve, const afxReal *IdentityVector, int Dimension, afxReal *Result, afxBool CurveIsNormalized);
} smt2;

AKX smt2 Smt2;

#endif//AFX_CURVE_H
