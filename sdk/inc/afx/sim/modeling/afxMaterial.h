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

#ifndef AFX_MATERIAL_H
#define AFX_MATERIAL_H

#include "afx/sim/afxSimDefs.h"
#include "afx/draw/res/afxTexture.h"

// Graphics hardware is still undergoing major changes every year, and as such, there is still no standard way to represent material data for real-time rendering.
// Granny tries to avoid restricting your material rendering by keeping the standardized structure of materials to a minimum, and packing basically every material parameter provided by each art tool into the ExtendedData section of the granny_material structure.

// All professional art tools construct materials as "shader graphs", where materials are connected to each other to form a tree of shaders.
// Granny reproduces this data in its material data.
// Each material can have an arbitrary number of sub - materials that it uses as maps, which you can iterate over.
// Additionally, each material may be textured, and will have a reference to a granny_texture that provides the image data for the material.

AFX_DEFINE_HANDLE(afxMaterial);

// Um blueprint foi idealizado para ser um meta-objeto quase-completo para geração de um afxMaterial.

AFX_DECLARE_STRUCT(afxMaterialBlueprint);

AFX_DEFINE_STRUCT(afxMaterialBlueprintMap)
{
    afxString16                 semantic; // usually usage 16
    afxUri128                   uri; // 128
};

AFX_DEFINE_STRUCT(afxMaterialBlueprint)
{
    afxFcc                      fcc;
    void*                       sim;
    afxUri128                   uri; // 128
    afxArray                    maps; // afxMaterialBlueprintMap    
    afxUri128                   tex; // 128
};

AFX_DEFINE_STRUCT(afxMaterialMap)
{
    afxString*          usage; // usage 16
    afxMaterial         sub; // afxMaterial
};

AFX_OBJECT(afxMaterial)
{
    afxInstance           obj;
    afxUri*             uri; // 128

    afxNat              mapCnt;
    afxMaterialMap      *maps;
    afxTexture          tex;
    void                *extData;

    // non-Granny stuff
    afxInt              s[3];
    afxV4d              color; // albedo
    afxReal             shininess;
};

//static_assert(offsetof(afxMaterial, color) % AFX_MTL_ALIGN == 0, "");

AFX afxUri const*       AfxMaterialGetUri(afxMaterial mtl);
AFX afxNat              AfxMaterialGetMapCount(afxMaterial mtl);

AFX afxMaterial         AfxMaterialFindSub(afxMaterial mtl, afxString const *usage);
AFX afxMaterial         AfxMaterialGetSub(afxMaterial mtl, afxNat mapIdx);
AFX void                AfxMaterialSetSub(afxMaterial mtl, afxNat mapIdx, afxMaterial subMtl);

AFX afxTexture          AfxMaterialGetTexture(afxMaterial mtl);
AFX void                AfxMaterialSetTexture(afxMaterial mtl, afxTexture tex);

// non-Granny
AFX void                AfxMaterialColorize(afxMaterial mtl, afxV4d const color);
AFX void                AfxMaterialShine(afxMaterial mtl, afxReal shininess);

////////////////////////////////////////////////////////////////////////////////
// BLUEPRINT                                                                  //
////////////////////////////////////////////////////////////////////////////////

// Inicializa uma blueprint, e reserva recursos.
AFX void                        AfxMaterialBlueprintDeploy(afxMaterialBlueprint *mtlb, void* sim, afxUri const *name, afxNat estMtlCnt, afxUri const *texUri);

// Retira de uso uma blueprint, e libera recursos.
AFX void                        AfxMaterialBlueprintDiscard(afxMaterialBlueprint *mtlb);

AFX void                        AfxMaterialBlueprintErase(afxMaterialBlueprint *mtlb);

AFX void                        AfxMaterialBlueprintRename(afxMaterialBlueprint *mtlb, afxUri const *uri);

AFX void                        AfxMaterialBlueprintSetTexture(afxMaterialBlueprint *mtlb, afxUri const *uri);

AFX void                        AfxMaterialBlueprintAddMap(afxMaterialBlueprint *mtlb, afxUri const *tex, afxString const *semantic);

#endif//AFX_MATERIAL_H