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

#ifndef AFX_FRUSTUM_H
#define AFX_FRUSTUM_H

#include "afxAabb.h"
#include "afxPlane.h"
#include "afxSphere.h"
#include "afx/math/afxMathDefs.h"

typedef enum afxCubeFace
{
    afxCubeFace_L,
    afxCubeFace_R,
    afxCubeFace_B,
    afxCubeFace_T,
    afxCubeFace_N,
    afxCubeFace_F,

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

AFX_DEFINE_STRUCT(afxFrustum)
{
    afxV4d      origin;
    afxPlane    planes[afxCubeFace_TOTAL];
    afxV4d      corners[afxCubeCorner_TOTAL];  // 0~3 near, 4~7 far
};

AFXINL void     AfxCopyFrustum(afxFrustum* f, afxFrustum const* in);

AFXINL afxBool  AfxFrustumDoesCullSphere(afxFrustum const* f, afxSphere const* sph);

AFXINL afxBool  AfxFrustumDoesCullAabb(afxFrustum const* f, afxAabb const* aabb);

AFXINL afxBool  AfxFrustumDoesCullFrustum(afxFrustum const* f, afxFrustum const* other);

// frustrum and matrix must be in same space
AFXINL void     AfxRecomputeFrustum(afxFrustum* f, afxReal const pv[4][4]);
AFXINL void     AfxRecomputeFrustum2(afxFrustum* f, afxReal const v[4][4], afxReal const p[4][4]);

AFXINL void     AfxAabbEncapsulateFrustum(afxAabb* aabb, afxFrustum const* f);

#endif//AFX_FRUSTUM_H