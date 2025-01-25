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

// This code is part of SIGMA Advanced Math Extensions for Qwadro

#ifndef AFX_FRUSTUM_H
#define AFX_FRUSTUM_H

#include "qwadro/inc/math/bound/afxBox.h"
#include "qwadro/inc/math/bound/afxSphere.h"
#include "qwadro/inc/math/afxMatrix.h"
#include "qwadro/inc/math/bound/afxPlane.h"

typedef enum afxCubeFace
{
    afxCubeFace_L, // -0
    afxCubeFace_R, // +0
    afxCubeFace_B, // -0
    afxCubeFace_T, // +0
    afxCubeFace_N, // -0
    afxCubeFace_F, // +0

    afxCubeFace_TOTAL
} afxCubeFace;

typedef enum afxCubeCorner
{
    // near
    afxCubeCorner_LBN,
    afxCubeCorner_RBN,
    afxCubeCorner_RTN,
    afxCubeCorner_LTN,
    // far
    afxCubeCorner_LBF,
    afxCubeCorner_RBF,
    afxCubeCorner_RTF,
    afxCubeCorner_LTF,

    afxCubeCorner_TOTAL,
} afxCubeCorner;

AFX afxV4d const AFX_V4D_LBN;
AFX afxV4d const AFX_V4D_RBN;
AFX afxV4d const AFX_V4D_RTN;
AFX afxV4d const AFX_V4D_LTN;

AFX afxV4d const AFX_V4D_LBF;
AFX afxV4d const AFX_V4D_RBF;
AFX afxV4d const AFX_V4D_RTF;
AFX afxV4d const AFX_V4D_LTF;

AFX_DEFINE_STRUCT_ALIGNED(AFX_SIMD_ALIGNMENT, afxFrustum)
{
    afxPlane    planes[afxCubeFace_TOTAL];
    afxV4d      corners[afxCubeCorner_TOTAL];
    afxV4d      origin;
};

AFXINL void     AfxFrustumCopy(afxFrustum* f, afxFrustum const* in);

AFXINL afxBool  AfxFrustumDoesCullSphere(afxFrustum const* f, afxSphere const* sph);
AFXINL afxBool  AfxFrustumTestAabbs(afxFrustum const* f, afxUnit cnt, afxBox const aabb[]);
AFXINL afxBool  AfxFrustumDoesCullFrustum(afxFrustum const* f, afxFrustum const* other);

// frustrum and matrix must be in same space
AFXINL void     AfxRecomputeFrustum3(afxFrustum* f, afxM4d const pv);
AFXINL void     AfxRecomputeFrustum(afxFrustum* f, afxM4d const v, afxM4d const p);

AFXINL void     AfxAabbAbsorbFrustum(afxBox* aabb, afxFrustum const* f);

#endif//AFX_FRUSTUM_H
