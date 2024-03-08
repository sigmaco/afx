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

#ifndef AFX_FRUSTUM_H
#define AFX_FRUSTUM_H

#include "afxAabb.h"
#include "afxPlane.h"
#include "afxSphere.h"
#include "qwadro/math/afxMathDefs.h"

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

AFX_DEFINE_STRUCT(afxFrustum)
{
    afxV4d      origin;
    afxPlane    planes[afxCubeFace_TOTAL];
    afxV4d      corners[afxCubeCorner_TOTAL];
};

AFXINL void     AfxCopyFrustum(afxFrustum* f, afxFrustum const* in);

AFXINL afxBool  AfxFrustumDoesCullSphere(afxFrustum const* f, afxSphere const* sph);
AFXINL afxBool  AfxFrustumDoesCullAabb(afxFrustum const* f, afxAabb const aabb);
AFXINL afxBool  AfxFrustumDoesCullFrustum(afxFrustum const* f, afxFrustum const* other);

// frustrum and matrix must be in same space
AFXINL void     AfxRecomputeFrustum3(afxFrustum* f, afxM4d const pv);
AFXINL void     AfxRecomputeFrustum(afxFrustum* f, afxM4d const v, afxM4d const p);

AFXINL void     AfxAabbEncapsulateFrustum(afxAabb aabb, afxFrustum const* f);

#endif//AFX_FRUSTUM_H
