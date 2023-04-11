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
 *                      (c) 2017 SIGMA Co. & SIGMA Technology Group
 *                                    www.sigmaco.org
 */

#ifndef AFX_MATH_DEFS_H
#define AFX_MATH_DEFS_H

#include "afx/core/diag/afxDebug.h"
#include "afx/core/base/afxSimd.h"
#include <math.h>
//#include <stdalign.h>

//#define MFX_USE_RWMATH

// AFX always targets a another object rather than 'self' to avoid using intermediate objects inside functions. It optimizes code when operations on a object type are grouped continously.
// That means "AfxNormalizeObject(self, to)" always should export to "to" instead of doing action in place.

// The "Right vector" is one of the vectors in a RenderWare Graphics matrix. It corresponds to the 'x' axis in a right-handed, xyz coordinate system. 
// The "Up vector" is one of the vectors in a RenderWare Graphics matrix. The up vector corresponds to the 'y' axis in a right-handed, xyz coordinate system. 
// The "at vector" is one of the vectors in a RenderWare Graphics matrix. It corresponds to the 'z' axis in a right-handed, xyz coordinate system. 
// RenderWare Graphics uses a right-handed co-ordinate system. Therefore with z increasing into the display screen, and y increasing upwards, the x co-ordinate system increases towards the left.

// This object defines a RenderWare Matrix object.
// The Matrix is heavily used throughout the API and a full range of functions are provided, including: rotation, multiplication, concatenation, scaling, translation, creation, destruction, stream read / write functions and a number of access functions to access particular vector data within the matrix.
// RenderWare uses 4x3, row - major affine matrices.

// Qwadro uses radians as default angle measurement unit. They are natural and faster.

// The linear algebra library produces OpenGL-oriented calculus. If you use Direct3D, kill yourself.

//#define MFX_USE_COLUMN_MAJOR_MATRIX
//#define MFX_USE_LEFT_HANDED_MATRIX
#define MFX_USE_EXPERIMENTAL
#define MFX_USE_RWMATH

#define AfxScalar(x_) ((afxReal)x_)

#define AFX_PI_         AfxScalar(14159265359)
#define AFX_PI          AfxScalar(3.14159265358979323846)
#define AFX_PI_OVER2    (AFX_PI / AfxScalar(2))
#define AFX_EPSILON     AfxScalar(FLT_EPSILON)

typedef afxReal         afxSimd(afxV2d[2]);
typedef afxReal         afxSimd(afxV3d[3]);
typedef afxReal         afxSimd(afxV4d[4]);

typedef afxV4d          afxSimd(afxQuat); // 0,1,2 = imaginary, 3 = real

typedef afxV2d          afxSimd(afxM2d[2]);
typedef afxV3d          afxSimd(afxM3d[3]);
typedef afxV4d          afxSimd(afxM4d[4]);

#if 0
static_assert(__alignof(afxV2d) == AFX_SIMD_ALIGN, "");
static_assert(__alignof(afxV3d) == AFX_SIMD_ALIGN, "");
static_assert(__alignof(afxV4d) == AFX_SIMD_ALIGN, "");
static_assert(__alignof(afxQuat) == AFX_SIMD_ALIGN, "");
static_assert(__alignof(afxM3d) == AFX_SIMD_ALIGN, "");
static_assert(__alignof(afxM4d) == AFX_SIMD_ALIGN, "");
#endif

AFX_DEFINE_STRUCT(afxRect)
{
    union { afxInt offset[2]; struct { afxInt x, y; }; };
    union { afxNat extent[2]; struct { afxNat w, h; }; };
};

AFXINL void         AfxRectZero(afxRect *rect) { rect->x = 0; rect->y = 0; rect->w = 1; rect->h = 1; }
AFXINL void         AfxRectCopy(afxRect *rect, afxRect const *src) { rect->x = src->x; rect->y = src->y; rect->w = src->w; rect->h = src->h; }

AFXINL afxReal      AfxRealMin(afxReal a, afxReal b) { return (a < b) ? a : b; }
AFXINL afxReal      AfxRealMax(afxReal a, afxReal b) { return (a > b) ? a : b; }

AFXINL afxBool      AfxRealIsAlmostEqual(afxReal a, afxReal b) { return ((AfxAbs(a - b)) <= (AFX_EPSILON * AfxRealMax(AfxScalar(1), AfxRealMax(a, b)))); }

AFXINL afxReal      AfxToNdc(afxReal x, afxReal total) { return x * (AfxScalar(1) / total); }
AFXINL afxReal      AfxFromNdc(afxReal x, afxReal total) { afxError err = NIL; AfxAssert(AfxScalar(0) <= x); AfxAssert(AfxScalar(1) >= x); return x * total; }


AFXINL void AfxExtractV2dInNdc(afxV2d v, afxV2d const b, afxV2d const total)
{
    afxError err = NIL;
    AfxAssert(v);
    AfxAssert(b);
    AfxAssert(total);
    v[0] = AfxToNdc(b[0], total[0]);
    v[1] = AfxToNdc(b[1], total[1]);
}

AFXINL void AfxExtractV2dOutNdc(afxV2d v, afxV2d const b, afxV2d const total)
{
    afxError err = NIL;
    AfxAssert(v);
    AfxAssert(b);
    AfxAssert(total);
    v[0] = AfxFromNdc(b[0], total[0]);
    v[1] = AfxFromNdc(b[1], total[1]);
}


AFXINL void AfxMakeV2dNdc(afxV2d v, afxV2d const total)
{
    afxError err = NIL;
    AfxAssert(v);
    AfxAssert(total);
    AfxAssert(v != total);
    v[0] = AfxToNdc(v[0], total[0]);
    v[1] = AfxToNdc(v[1], total[1]);
}

AFXINL void AfxUnmakeV2dNdc(afxV2d v, afxV2d const total)
{
    afxError err = NIL;
    AfxAssert(v);
    AfxAssert(total);
    AfxAssert(v != total);
    v[0] = AfxFromNdc(v[0], total[0]);
    v[1] = AfxFromNdc(v[1], total[1]);
}

AFXINL afxV4d* AfxCorrectRwMatrix(afxM4d m, afxM4d const in);

#endif//AFX_MATH_DEFS_H
