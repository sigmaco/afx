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

#ifndef AFX_VOLUME_H
#define AFX_VOLUME_H

#include "qwadro/inc/math/afxVector.h"

typedef enum
{
    AFX_VOL_AABB,
    AFX_VOL_SPHERE,
    AFX_VOL_FRUSTUM
} afxVolumeType;

AFX_DEFINE_STRUCT_ALIGNED(AFX_SIMD_ALIGNMENT, afxPlane)
{
    afxV4d uvwd; // U, V, W, D
};

#pragma pack(push, 1)
AFX_DEFINE_STRUCT_ALIGNED(AFX_SIMD_ALIGNMENT, afxBox)
// A set of six 32-bit floats consisting of the minimum x, y, and z values followed by the maximum x, y, and z values.
{
    // A vector that stores the minimum coordinates (the lower corner of a bounding box in 3D or 4D space).
    afxV4d  min;
    // A vector storing the maximum coordinates (the upper corner of a bounding box).
    afxV4d  max; // [ sup/max, inf/min ]
};

typedef afxBox afxAabb; // [ inf/min, sup/max ]
typedef afxBox afxObb;
#pragma pack(pop)

#pragma pack(push, 1)
AFX_DEFINE_STRUCT_ALIGNED(AFX_SIMD_ALIGNMENT, afxSphere)
{
    afxV4d  xyzr;
};
#pragma pack(pop)

AFX_STATIC_ASSERT(sizeof(afxSphere) == 16, "");
//AFX_STATIC_ASSERT(offsetof(afxSphere, radius) == 12, "");

AFX_DEFINE_STRUCT(afxVolume)
{
    afxVolumeType   type;
};

#define AFX_ASSERT_TRIANGLE_BOUNDS(idxA_, idxB_, idxC_, maxTriCnt_) \
    ((!!((((maxTriCnt_)*3) > (idxA_)) || \
        (((maxTriCnt_)*3) > (idxB_)) || \
        (((maxTriCnt_)*3) > (idxC_)))) || \
        (AfxThrowError(),AfxReportAssert("<%s, %s, %s>(%u, %u %u) is out of <%s>(%u) triangles' range.",AFX_STRINGIFY(idxA_),AFX_STRINGIFY(idxB_),AFX_STRINGIFY(idxC_),(idxA_),(idxB_),(idxC_),AFX_STRINGIFY(maxTriCnt_),(maxTriCnt_)),0))

#endif//AFX_VOLUME_H
