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

#ifndef AFX_MATERIAL_H
#define AFX_MATERIAL_H

#include "afxCadDefs.h"
#include "afx/draw/res/afxTexture.h"

// Graphics hardware is still undergoing major changes every year, and as such, there is still no standard way to represent material data for real-time rendering.
// Granny tries to avoid restricting your material rendering by keeping the standardized structure of materials to a minimum, and packing basically every material parameter provided by each art tool into the ExtendedData section of the granny_material structure.

// All professional art tools construct materials as "shader graphs", where materials are connected to each other to form a tree of shaders.
// Granny reproduces this data in its material data.
// Each material can have an arbitrary number of sub - materials that it uses as maps, which you can iterate over.
// Additionally, each material may be textured, and will have a reference to a granny_texture that provides the image data for the material.

AFX_DEFINE_HANDLE(afxMaterial);

AFX_DEFINE_STRUCT(afxMaterialMap)
{
    afxString16         usage; // usage
    afxConnection       subMtl; // afxMaterial
};

AFX_OBJECT(afxMaterial)
{
    afxObject           obj;
    afxString32         name;

    afxNat              mapCnt;
    afxMaterialMap      *maps;
    afxConnection       tex;
    void                *extData;

    // non-Granny stuff
    afxInt              s[3];
    afxV4d              color; // albedo
    afxReal             shininess;
};

//static_assert(offsetof(afxMaterial, color) % AFX_MTL_ALIGN == 0, "");

AFX afxString const*    AfxMaterialGetName(afxMaterial mtl);
AFX afxNat              AfxMaterialGetMapCount(afxMaterial mtl);
AFX afxResult           AfxMaterialResetMap(afxMaterial mtl, afxNat mapIdx, afxString const *name, afxMaterial subMtl);

AFX afxMaterial         AfxMaterialGetSubMaterial(afxMaterial mtl, afxNat mapIdx);
AFX afxResult           AfxMaterialSetSubMaterial(afxMaterial mtl, afxNat mapIdx, afxMaterial subMtl);
AFX afxMaterial         AfxMaterialFindSubMaterial(afxMaterial mtl, afxString const *usage);

AFX afxTexture          AfxMaterialGetTexture(afxMaterial mtl);
AFX afxResult           AfxMaterialSetTexture(afxMaterial mtl, afxTexture tex);

// non-Granny
AFX void                AfxMaterialColorize(afxMaterial mtl, afxV4d const color);
AFX void                AfxMaterialShine(afxMaterial mtl, afxReal shininess);

#endif//AFX_MATERIAL_H