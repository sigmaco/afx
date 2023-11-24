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
 *                   (c) 2017 SIGMA Technology Group — Federação SIGMA
 *                                    www.sigmaco.org
 */

// Projective transformation matrix.

#ifndef AFX_PROJECTIVE_MATRIX_H
#define AFX_PROJECTIVE_MATRIX_H

#include "afxMatrix.h"

AFXINL void     AfxComputeShadowM4d(afxM4d m, afxPlane const p, afxReal const lightPos[3]);

AFXINL void     AfxCubemapM4d_OpenGL(afxM4d m, afxNat face);
AFXINL void     AfxCubemapM4d_Direct3D(afxM4d m, afxNat face);

AFXINL void     AfxComputeOrthographicM4d_LH(afxReal p[4][4], afxReal const extent[2], afxReal near, afxReal far);
AFXINL void     AfxComputeOrthographicM4d_RH(afxReal p[4][4], afxReal const extent[2], afxReal near, afxReal far);

AFXINL void     AfxComputeOffcenterOrthographicM4d_LH(afxReal p[4][4], afxReal left, afxReal right, afxReal bottom, afxReal top, afxReal near, afxReal far);
AFXINL void     AfxComputeOffcenterOrthographicM4d_RH(afxReal p[4][4], afxReal left, afxReal right, afxReal bottom, afxReal top, afxReal near, afxReal far);

AFXINL void     AfxComputePerspectiveM4d_LH(afxReal p[4][4], afxReal const extent[2], afxReal near, afxReal far);
AFXINL void     AfxComputePerspectiveM4d_RH(afxReal p[4][4], afxReal const extent[2], afxReal near, afxReal far);

AFXINL void     AfxComputeFovPerspectiveM4dLH(afxReal p[4][4], afxReal fovY, afxReal aspectRatio, afxReal near, afxReal far);
AFXINL void     AfxComputeFovPerspectiveM4dRH(afxReal p[4][4], afxReal fovY, afxReal aspectRatio, afxReal near, afxReal far);

AFXINL void     AfxComputeOffcenterPerspectiveM4d_LH(afxReal p[4][4], afxReal left, afxReal right, afxReal bottom, afxReal top, afxReal near, afxReal far);
AFXINL void     AfxComputeOffcenterPerspectiveM4d_RH(afxReal p[4][4], afxReal left, afxReal right, afxReal bottom, afxReal top, afxReal near, afxReal far);

AFXINL void     AfxComputeLookToM4d_LH(afxReal v[4][4], afxReal const eye[3], afxReal const dir[3], afxReal const up[3]);
AFXINL void     AfxComputeLookToM4d_RH(afxReal v[4][4], afxReal const eye[3], afxReal const dir[3], afxReal const up[3]);

AFXINL void     AfxComputeLookAtM4d_LH(afxReal v[4][4], afxReal const eye[3], afxReal const target[3], afxReal const up[3]);
AFXINL void     AfxComputeLookAtM4d_RH(afxReal v[4][4], afxReal const eye[3], afxReal const target[3], afxReal const up[3]);

AFXINL void     AfxComputeRenderWareViewM4d(afxReal v[4][4], afxReal const cam[4][4]);
AFXINL void     AfxComputeRenderWareProjectionM4d(afxReal p[4][4], afxViewport const* vp, afxBool perspective);

#endif//AFX_PROJECTIVE_MATRIX_H
