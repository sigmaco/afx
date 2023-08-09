/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *              T H E   Q W A D R O   E X E C U T I O N   E C O S Y S T E M
 *
 *                                   Public Test Build
 *                   (c) 2017 SIGMA Technology Group — Federação SIGMA
 *                                    www.sigmaco.org
 */

#include "afx/math/afxMathDefs.h"
#include "afx/math/afxScalar.h"

////////////////////////////////////////////////////////////////////////////////
// SIMILARITY TRANSFORMATIONS                                                 //
////////////////////////////////////////////////////////////////////////////////

_AFXINL void AfxV3dAssimilate(afxV3d const v, afxV3d const affine, afxM3d const linear, afxV3d out)
{
    // From void __cdecl granny::InPlaceSimilarityTransformPosition(const float *Affine3, const float *Linear3x3, float *Position3)
    afxError err = NIL;
    AfxAssert(out);
    AfxAssert(v);
    AfxAssert(affine);
    AfxAssert(linear);
    AfxV3dMultiplyM3d(v, linear, out); // originalmente era transposto, pois Granny é colum-major.
    AfxV3dAdd(affine, out, out);
}

_AFXINL void AfxQuatAssimilate(afxQuat const q, afxM3d const linear, afxM3d const invLinear, afxQuat out)
{
    // From void __cdecl granny::InPlaceSimilarityTransformOrientation(const float *Linear3x3, const float *InverseLinear3x3, float *Orientation4)
    afxError err = NIL;
    AfxAssert(out);
    AfxAssert(q);
    AfxAssert(linear);
    AfxAssert(invLinear);

    afxReal len = AfxQuatMagnitude(q);
    AfxQuatScale(q, 1.f / len, out);

    afxM3d rotM, tmp;
    AfxQuatMakeRotationM3d(out, rotM);
    AfxM3dMultiply(linear, rotM, tmp); // as ordens de multiplicação deveria mudar devido a Granny3D ser colum-major?
    AfxM3dMultiply(tmp, invLinear, rotM);
    AfxM3dMakeRotationQuat(rotM, out);

    AfxQuatScale(out, len, out);
}

_AFXINL void AfxM3dAssimilate(afxM3d const m, afxM3d const linear, afxM3d const invLinear, afxM3d out)
{
    // From void __cdecl granny::InPlaceSimilarityTransformScaleShear(const float *Linear3x3, const float *InverseLinear3x3, float *ScaleShear3x3)
    afxError err = NIL;
    AfxAssert(out);
    AfxAssert(m);
    AfxAssert(linear);
    AfxAssert(invLinear);
    // Reversed from Granny
    afxM3d tmp;
    AfxM3dMultiply(linear, m, tmp);  // as ordens de multiplicação deveria mudar devido a Granny3D ser colum-major?
    AfxM3dMultiply(tmp, invLinear, out);
}

_AFXINL void AfxM4dAssimilate4x3(afxM4d const m, afxV3d const affine, afxM3d const linear, afxM3d const invLinear, afxM4d out)
{
    // From void __cdecl granny::InPlaceSimilarityTransform4x3(const float *Affine3, const float *Linear3x3, const float *InverseLinear3x3, float *ResultInit)
    afxError err = NIL;
    AfxAssert(out);
    AfxAssert(m);
    AfxAssert(affine);
    AfxAssert(linear);
    AfxAssert(invLinear);

    afxM3d Upper3x3;
    Upper3x3[0][0] = m[0][0]; // transpondo?
    Upper3x3[0][1] = m[1][0];
    Upper3x3[0][2] = m[2][0];
    Upper3x3[1][0] = m[0][1];
    Upper3x3[1][1] = m[1][1];
    Upper3x3[1][2] = m[2][1];
    Upper3x3[2][0] = m[0][2];
    Upper3x3[2][1] = m[1][2];
    Upper3x3[2][2] = m[2][2];

    afxV3d pos;
    pos[0] = m[3][0];
    pos[1] = m[3][1];
    pos[2] = m[3][2];
    AfxV3dAssimilate(pos, affine, linear, pos);
    AfxM3dAssimilate(Upper3x3, linear, invLinear, Upper3x3);

    out[0][0] = Upper3x3[0][0];
    out[1][0] = Upper3x3[0][1];
    out[2][0] = Upper3x3[0][2];
    out[0][1] = Upper3x3[1][0];
    out[1][1] = Upper3x3[1][1];
    out[2][1] = Upper3x3[1][2];
    out[0][2] = Upper3x3[2][0];
    out[1][2] = Upper3x3[2][1];
    out[2][2] = Upper3x3[2][2];
    out[3][0] = pos[0];
    out[3][1] = pos[1];
    out[3][2] = pos[2];
}

_AFXINL void AfxTransformAssimilate(afxTransform const *t, afxV3d const affine, afxM3d const linear, afxM3d const invLinear, afxTransform *out)
{
    AfxV3dAssimilate(t->origin, affine, linear, out->origin);
    AfxQuatAssimilate(t->orientation, linear, invLinear, out->orientation);
    AfxM3dAssimilate(t->scaleShear, linear, invLinear, out->scaleShear);
}
