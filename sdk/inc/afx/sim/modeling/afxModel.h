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

// Um blueprint foi idealizado para ser um meta-objeto quase-completo para geração de um modelo.

typedef enum afxModelBlueprintFlag
{
    AFX_MDLP_FLAG_DONT_SHARE_VBUF = AFX_FLAG(0),
    AFX_MDLP_FLAG_DONT_SHARE_IBUF = AFX_FLAG(1)
} afxModelBlueprintFlag;

AFX_DEFINE_STRUCT(afxModelBlueprint)
{
    afxFcc          fcc;
    void*           sim;
    afxUri128       uri;
    afxSkeleton     skl;
    afxTransform    init;
    afxArray        meshes;
};

// The information needed to construct a malleable model.

AFX_DEFINE_STRUCT(afxMeshSlot)
{
    afxMesh             msh;
};

AFX_OBJECT(afxModel)
{
    afxObject           obj;
    afxUri*             uri; // 128
    afxSkeleton         skl;
    afxTransform        init;
    afxAabb             aabb;
    afxNat              meshSlotCnt;
    afxMeshSlot         *meshSlots;
};

AFX afxUri const*       AfxModelGetUri(afxModel mdl);
AFX afxSkeleton         AfxModelGetSkeleton(afxModel mdl);

AFX void                AfxModelGetInitialPlacement(afxModel mdl, afxM4d m);
AFX void                AfxModelTransform(afxModel mdl, afxV3d const affine, afxM3d const linear, afxM3d const invLinear, afxReal affineTol, afxReal linearTol, afxFlags flags);

AFX afxMesh             AfxModelGetBoundMesh(afxModel mdl, afxNat mshIdx);
AFX afxNat              AfxModelGetMeshBindingCount(afxModel mdl);

////////////////////////////////////////////////////////////////////////////////
// BLUEPRINT                                                                  //
////////////////////////////////////////////////////////////////////////////////

// Inicializa uma blueprint, e reserva recursos.
AFX void                    AfxModelBlueprintDeploy(afxModelBlueprint *mdlb, void* sim, afxUri const *uri, afxSkeleton skl, afxTransform *initialPlacement, afxNat estMeshCnt);

// Retira de uso uma blueprint, e libera recursos.
AFX void                    AfxModelBlueprintDiscard(afxModelBlueprint *mdlb);

AFX void                    AfxModelBlueprintErase(afxModelBlueprint *mdlb);

// Obtém o blueprint do afxSkeleton.
AFX void                    AfxModelBlueprintSetSkeleton(afxModelBlueprint *mdlb, afxSkeleton skl);

AFX afxError                AfxModelBlueprintAddMeshes(afxModelBlueprint *mdlb, afxNat cnt, afxMesh msh[]);

#endif//AFX_MODEL_H