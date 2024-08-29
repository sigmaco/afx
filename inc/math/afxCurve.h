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

#ifndef AMX_CURVE_H
#define AMX_CURVE_H

#include "qwadro/inc/math/afxSphere.h"
#include "qwadro/inc/sim/afxSimDefs.h"

typedef enum afxCurveFlag
{
    afxCurveFlag_IDENTITY   = AFX_BIT(0),
    afxCurveFlag_CONSTANT   = AFX_BIT(1),
    afxCurveFlag_KEYFRAMED  = AFX_BIT(2),
    afxCurveFlag_NORMALIZED = AFX_BIT(3),
} afxCurveFlags;

typedef enum afxCurveFormat
{
    afxCurveFormat_DaKeyframes32f, /// Da, Keyframes32f
    afxCurveFormat_DaK32fC32f, /// Da, K32f, C32f
    afxCurveFormat_DaIdentity, /// Da, Identity
    afxCurveFormat_DaConstant32f, /// Da, Constant32f
    afxCurveFormat_D3Constant32f, /// D3, Constant32f
    afxCurveFormat_D4Constant32f, /// D4, Constant32f
    // still unsupported
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

AFX_DEFINE_STRUCT(afxCurveBlueprint)
{
    afxNat          fmt;
    afxNat          degree;
    afxNat          dimens;
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
    afxNat          dimens;
    afxNat          knotCnt;
    afxReal const*  knots;
    afxReal const*  ctrls;

    afxCurve        src;
    afxNat          sampleCnt;
    afxNat          sampleDim;
    afxReal const*  xformedSamples;
    afxReal const*  origSamples;
};

typedef struct sample_context
{
    afxReal LocalClock;
    afxReal LocalDuration;
    afxBool UnderflowLoop;
    afxBool OverflowLoop;
    afxInt FrameIndex;
} sample_context;

AMX afxInt          AfxGetCurveDegree(afxCurve c);
AMX afxNat          AfxGetCurveDimensionality(afxCurve c);
AMX afxNat          AfxGetCurveDimensionalityUnchecked(afxCurve c);

AMX afxCurveFlags   AfxGetCurveFlags(afxCurve c);
AMX afxCurveFlags   AfxTestCurveFlags(afxCurve c, afxCurveFlags mask);

AMX afxBool         AfxIsCurveIdentity(afxCurve c);
AMX afxBool         AfxIsCurveKeyframed(afxCurve c);
AMX afxBool         AfxIsCurveConstantOrIdentity(afxCurve c);
AMX afxBool         AfxIsCurveConstantNotIdentity(afxCurve c);

AMX afxReal*        AfxGetCurveKnots(afxCurve c);
AMX afxReal*        AfxGetCurveControls(afxCurve c);

AMX afxNat          AfxFindCurveKnot(afxCurve c, afxReal t);

AMX afxNat          AfxCountCurveKnots(afxCurve c);

AMX void            AfxSetCurveAllKnotValues(afxCurve c, afxNat knotCnt, afxNat dimension, afxReal const* knotSrc, afxReal const* ctrlSrc);
AMX void            AfxExtractCurveKnotValues(afxCurve c, afxNat knotIdx, afxNat knotCnt, afxReal* knotResults, afxReal* ctrlResults, afxReal const* identivec);
AMX afxReal         AfxExtractCurveKnotValue(afxCurve c, afxNat knotIdx, afxReal* ctrlRslt, afxReal const* identity);

AMX void            AfxEvaluateCurveAtKnot(afxCurve c, afxNat dimens, afxBool normalize, afxBool bwdsLoop, afxBool fwdsLoop, afxReal curveDur, afxNat knotIdx, afxReal t, afxReal* rslt, afxReal const* identityVec);
AMX void            AfxEvaluateCurveAt(afxCurve c, afxNat dimens, afxBool normalize, afxBool bwdsLoop, afxBool fwdsLoop, afxReal curveDur, afxReal t, afxReal* rslt, afxReal const* identityVec);

AMX void            AfxCopyCurve(afxCurve c, afxCurve src);

AMX void            AfxSetUpCurve(afxCurve c, afxCurveFormat fmt, afxNat degree, afxNat dim, afxNat knotCnt);
AMX void            AfxCleanUpCurve(afxCurve c);

AMX void            AfxMakeCurveDaKC32f(afxCurve c, afxNat degree, afxNat dim, afxNat knotCnt, afxReal const knots[], afxReal const ctrls[]);


AMX void            AfxBeginCurveCopy(afxCurveBlueprint* cb, afxCurve src);
AMX void            AfxBeginCurve(afxCurveBlueprint* cb, afxCurveFormat fmt, afxNat degree, afxNat dimension, afxNat knotCnt);
AMX afxCurve        AfxEndCurve(afxCurveBlueprint* cb, afxSimulation sim);

AMXINL void         AfxResetCurveInfo(afxCurveInfo* cb, afxCurveFormat fmt, afxNat degree, afxNat dimens, afxNat knotCnt);

////////////////////////////////////////////////////////////////////////////////

AMX afxError        AfxAcquireCurves(afxSimulation sim, afxNat cnt, afxCurveInfo const info[], afxCurve curves[]);
AMX afxError        AfxAssembleCurves(afxSimulation sim, afxNat cnt, afxCurveBlueprint const blueprints[], afxCurve curves[]);

#endif//AMX_CURVE_H
