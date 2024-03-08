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
 *                       (c) 2017 SIGMA, Engineering In Technology
 *                             <https://sigmaco.org/qwadro/>
 */

/// Hardware-accelerated curve computation

#ifndef AFX_CURVE_H
#define AFX_CURVE_H

#include "qwadro/math/afxMathDefs.h"
#include "afxSphere.h"
#include "qwadro/sim/awxSimDefs.h"


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
    afxNat          dimension;
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
    afxBool         curveBuilderNeedsFreeing;
    afxNat          fmt;
    afxNat          degree;
    afxNat          dimension;
    afxNat          knotCnt;
    afxReal const*  knotArray;
    afxReal const*  ctrlArray;
    afxCurve const* srcCurve;
    afxNat          sampleCnt;
    afxNat          sampleDimension;
    afxReal const*  transformedSamples;
    afxReal const*  originalSamples;
};

AKX afxInt      AfxGetCurveDegree(afxCurve const* c);
AKX afxNat      AfxGetCurveDimension(afxCurve const* c);
AKX afxNat      AfxGetCurveDimensionUnchecked(afxCurve const* c);

AKX afxBool     AfxCurveIsIdentity(afxCurve const* c);
AKX afxBool     AfxCurveIsKeyframed(afxCurve const* c);
AKX afxBool     AfxCurveIsConstantOrIdentity(afxCurve const* c);
AKX afxBool     AfxCurveIsConstantNotIdentity(afxCurve const* c);

AKX afxReal*    AfxGetCurveKnots(afxCurve const* c);
AKX afxReal*    AfxGetCurveControls(afxCurve const* c);

AKX afxNat      AfxFindCurveKnot(afxCurve const* c, afxReal t);

AKX afxNat      AfxCountCurveKnots(afxCurve const* c);
AKX afxReal     AfxExtractCurveKnotValue(afxCurve const* c, afxNat knotIdx, afxReal* ctrlRslt, afxReal const* identity);

AKX void        AfxSetCurveAllKnotValues(afxCurve* c, afxNat knotCnt, afxNat dimension, afxReal const* knotSrc, afxReal const* ctrlSrc);
AKX void        AfxExtractCurveKnotValues(afxCurve const* c, afxNat knotIdx, afxNat knotCnt, afxReal* knotResults, afxReal* ctrlResults, afxReal const* identivec);

AKX void        AfxCopyCurve(afxCurve* c, afxCurve const* src);

AKX void        AfxSetUpCurve(afxCurve* c, afxCurveFormat fmt, afxNat degree, afxNat dim, afxNat knotCnt);
AKX void        AfxCleanUpCurve(afxCurve* c);

AKX void        AfxMakeCurveDaKC32f(afxCurve* c, afxNat degree, afxNat dim, afxNat knotCnt, afxReal const knots[], afxReal const ctrls[]);


AKX void        AfxBeginCurveCopy(afxCurveBlueprint* cb, afxCurve const* src);
AKX void        AfxBeginCurve(afxCurveBlueprint* cb, afxCurveFormat fmt, afxNat degree, afxNat dimension, afxNat knotCnt);
AKX afxCurve*   AfxEndCurve(afxCurveBlueprint* cb, afxCurve* c);

#endif//AFX_CURVE_H
