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

#ifndef AFX_MODEL_H
#define AFX_MODEL_H

#include "afxMesh.h"
#include "../dag/afxSkeleton.h"

// The granny_model structure describes a named collection of meshes that are all bound to the same skeleton.
// It is essentially any connected group of meshes that animate together.
// A character would be a model, so would a tank, or even an entire building if it was modeled as a single hierarchy.
// A complete granny_model is made up of a granny_skeleton and a set of granny_mesh objects, both of which canv be accessed directly from the granny_model structure.

AFX_DEFINE_HANDLE(afxModel);

// The information needed to construct a malleable model.

AFX afxString const*AfxModelGetName(afxModel mdl);
AFX afxSkeleton     AfxModelGetSkeleton(afxModel mdl);
    
AFX afxV4d*         AfxModelGetInitialPlacement(afxModel mdl);
AFX afxV4d*         AfxModelCopyInitialPlacement(afxModel mdl, afxM4d dst);
AFX void            AfxModelTransform(afxModel mdl, afxM4d m); // transform model (mesh data) and skeleton

AFX afxMesh         AfxModelGetMesh(afxModel mdl, afxNat mshIdx);
AFX afxNat          AfxModelGetMeshCount(afxModel mdl);
AFX afxNat          AfxModelGetMeshIndex(afxModel mdl, afxMesh msh);
AFX afxResult       AfxModelForEachMesh(afxModel mdl, afxResult(*f)(afxMesh msh, afxNat mshIdx, void *data), void *data);

#define _AFX_MDL_IMPL
#ifdef _AFX_MDL_IMPL

AFX_OBJECT(afxModel)
{
    afxObject       obj;
    afxString32     name;
    afxConnection   skl; // afxSkeleton
    afxM4d          init;
    afxAabb         aabb;
    afxNat          mshCnt;
    afxConnection   *meshes; // mesh bindings
};

#endif

#endif//AFX_MODEL_H