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
 *                       (c) 2017 SIGMA, Engitech, Scitech, Serpro
 *                             <https://sigmaco.org/qwadro/>
 */

// Hardware-accelerated curve computation

#ifndef AFX_CURVE_H
#define AFX_CURVE_H

#include "qwadro/math/afxSphere.h"
#include "qwadro/sound/afxSoundDefs.h"

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

AFX_DEFINE_STRUCT(afxCurve)
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
    afxCurve const* srcCurve;
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
};

ASX afxError        AfxAcquireCurves(afxNat cnt, afxCurveInfo const info[], afxCurve curves[]);
ASX afxError        AfxAssembleCurves(afxNat cnt,afxCurveBlueprint const blueprints[], afxCurve curves[]);


ASX afxInt          AfxGetCurveDegree(afxCurve const* c);
ASX afxNat          AfxGetCurveDimensionality(afxCurve const* c);
ASX afxNat          AfxGetCurveDimensionalityUnchecked(afxCurve const* c);

ASX afxBool         AfxCurveIsIdentity(afxCurve const* c);
ASX afxBool         AfxCurveIsKeyframed(afxCurve const* c);
ASX afxBool         AfxCurveIsConstantOrIdentity(afxCurve const* c);
ASX afxBool         AfxCurveIsConstantNotIdentity(afxCurve const* c);

ASX afxReal*        AfxGetCurveKnots(afxCurve const* c);
ASX afxReal*        AfxGetCurveControls(afxCurve const* c);

ASX afxNat          AfxFindCurveKnot(afxCurve const* c, afxReal t);

ASX afxNat          AfxCountCurveKnots(afxCurve const* c);
ASX afxReal         AfxExtractCurveKnotValue(afxCurve const* c, afxNat knotIdx, afxReal* ctrlRslt, afxReal const* identity);

ASX void            AfxSetCurveAllKnotValues(afxCurve* c, afxNat knotCnt, afxNat dimension, afxReal const* knotSrc, afxReal const* ctrlSrc);
ASX void            AfxExtractCurveKnotValues(afxCurve const* c, afxNat knotIdx, afxNat knotCnt, afxReal* knotResults, afxReal* ctrlResults, afxReal const* identivec);

ASX void            AfxCopyCurve(afxCurve* c, afxCurve const* src);

ASX void            AfxSetUpCurve(afxCurve* c, afxCurveFormat fmt, afxNat degree, afxNat dim, afxNat knotCnt);
ASX void            AfxCleanUpCurve(afxCurve* c);

ASX void            AfxMakeCurveDaKC32f(afxCurve* c, afxNat degree, afxNat dim, afxNat knotCnt, afxReal const knots[], afxReal const ctrls[]);


ASX void            AfxBeginCurveCopy(afxCurveBlueprint* cb, afxCurve const* src);
ASX void            AfxBeginCurve(afxCurveBlueprint* cb, afxCurveFormat fmt, afxNat degree, afxNat dimension, afxNat knotCnt);
ASX afxCurve*       AfxEndCurve(afxCurveBlueprint* cb, afxCurve* c);


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
    int(*CurveKnotCharacter)(const afxCurve *Curve);
    void(*CubicCoefficients)(afxReal const a[3], afxReal const b[3], afxReal t, afxReal* ci_3, afxReal* ci_2, afxReal* ci_1, afxReal* ci);
    void(*SampleBSpline)(afxNat Degree, afxNat Dimension, afxBool Normalize, const afxReal *ti, const afxReal *pi, afxReal t, afxReal *Result);
    char(*ConstructBSplineBuffers)(afxInt dim, afxCurve const* prev, afxCurve const* c, afxCurve const* next, afxReal prevCurDur, afxReal curDur, afxReal nextCurDur, afxInt knotIdx, afxReal* ti, afxReal* pi, afxReal** tiPtr, afxReal** piPtr, afxReal const* identityVec);
    void(*EvaluateCurve)(const sample_context *Context, const afxCurve *Curve, const afxReal *IdentityVector, int Dimension, afxReal *Result, afxBool CurveIsNormalized);
} smt2;

ASX smt2 Smt2;

#endif//AFX_CURVE_H
