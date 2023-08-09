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

typedef enum
{
    AFX_FRUS_PLAN_L,
    AFX_FRUS_PLAN_R,
    AFX_FRUS_PLAN_B,
    AFX_FRUS_PLAN_T,
    AFX_FRUS_PLAN_N,
    AFX_FRUS_PLAN_F,

    AFX_FRUS_PLAN_TOTAL
} afxFrustumPlane;

typedef enum
{
    // near
    AFX_FRUS_CORN_LBN,
    AFX_FRUS_CORN_RBN,
    AFX_FRUS_CORN_RTN,
    AFX_FRUS_CORN_LTN,
    // far
    AFX_FRUS_CORN_LBF,
    AFX_FRUS_CORN_RBF,
    AFX_FRUS_CORN_RTF,
    AFX_FRUS_CORN_LTF,

    AFX_FRUS_CORN_TOTAL,
} afxFrustumCorners;

AFX_DEFINE_STRUCT(afxFrustum)
{
    afxVolume   vol;
    afxV4d      origin;
    afxPlane    planes[AFX_FRUS_PLAN_TOTAL];
    afxV4d      corners[AFX_FRUS_CORN_TOTAL];  // 0~3 near, 4~7 far
};

AFXINL afxFrustum*  AfxFrustumCopy(afxFrustum *f, afxFrustum const *in);

AFXINL afxBool      AfxFrustumDoesCullSphere(afxFrustum const *frus, afxSphere const *sph);

AFXINL afxBool      AfxFrustumDoesCullAabb(afxFrustum const *frus, afxAabb const *aabb);

AFXINL afxBool      AfxFrustumDoesCullFrustum(afxFrustum const *frus, afxFrustum const *other);

AFXINL afxFrustum*  AfxFrustumRecompute(afxFrustum *frus, afxM4d const v, const afxM4d p);

AFXINL afxAabb*     AfxAabbEncapsulateFrustum(afxAabb* aabb, afxFrustum const *frus);

#endif//AFX_FRUSTUM_H