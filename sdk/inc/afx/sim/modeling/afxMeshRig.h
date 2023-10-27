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

#ifndef AFX_MESH_RIG_H
#define AFX_MESH_RIG_H

#include "afx/sim/afxSkeleton.h"
#include "afx/sim/modeling/afxMesh.h"

AFX_OBJECT(afxMeshRig)
#ifdef _AFX_MESH_RIG_C
{
    afxMesh         msh;
    afxNat          vertebraCnt;
    afxSkeleton     srcSkl;
    afxNat*         srcBoneIndices;
    afxSkeleton     dstSkl;
    afxNat*         dstBoneIndices;
}
#endif
;

AFX afxMeshRig      AfxRigMesh(afxMesh msh, afxSkeleton src, afxSkeleton dst);

AFX afxBool         AfxMeshRigIsTransferred(afxMeshRig mshr);

AFX void            AfxBuildMeshRigMatrixArray(afxMeshRig mshr, afxWorldPose const* WorldPose, afxNat firstBoneIdx, afxNat boneCnt, afxM4d* xformBuffer);

#endif//AFX_MESH_RIG_H