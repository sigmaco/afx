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

#ifndef AFX_LEGO_H
#define AFX_LEGO_H

// A afxLego defines the interface between a set of resources bound in a GPUBindGroup and their accessibility in shader stages.

#include "afx/draw/res/afxBuffer.h"
#include "afx/draw/res/afxShader.h"
#include "afx/draw/res/afxSampler.h"

AFX_DEFINE_STRUCT(afxLegoBlueprintBinding)
{
    afxNat                  binding;
    afxFlags                visibility;
    afxShaderResourceType   type;
    afxNat                  cnt;
    afxString               name; // 16
};

AFX_DEFINE_STRUCT(afxLegoBlueprint)
{
    afxFcc                  fcc;
    afxDrawContext          dctx;
    afxArray                bindings; // afxLegoBlueprintBinding
};

AFX_DEFINE_STRUCT(afxLegoBindingDecl)
{
    afxNat                  binding; // A unique identifier for a resource binding within the afxLegoScheme, corresponding to a afxLegoDataScheme.binding and a @binding attribute in the afxShader.
    afxFlags                visibility; // A bitset of the members of afxShaderStage. Each set bit indicates that a afxLegoDataScheme's resource will be accessible from the associated shader stage.
    afxShaderResourceType   type;
    afxNat                  cnt;
    afxString const*        name;
};

AFX_DEFINE_STRUCT(afxLegoEntry) // A GPUBindGroupLayoutEntry describes a single shader resource binding to be included in a GPUBindGroupLayout.
{
    // A afxLegoDataScheme describes a single shader resource binding to be included in a afxLegoScheme.
    afxNat32                binding; // A unique identifier for a resource binding within the afxLegoScheme, corresponding to a afxLegoDataScheme.binding and a @binding attribute in the afxShader.
    afxFlags                visibility; // A bitset of the members of afxShaderStage. Each set bit indicates that a afxLegoDataScheme's resource will be accessible from the associated shader stage.
    afxShaderResourceType   type;
    afxNat                  cnt;
    afxString               name; // by QWADRO

    union
    {
        struct
        {
            afxBool hasDynOffsets; // if exist it is a push constant block?
            afxNat  minSizeBound;
            afxBool writeable; // if true it is no uniform buffer?
        } buf;
        struct
        {
            int a;
        } img;
    };
};

#ifdef _AFX_LEGO_C
AFX_OBJECT(afxLego)
#else
struct afxBaseLego
#endif
{
    afxNat32        crc32;
    afxNat          entryCnt;
    afxLegoEntry    *entries; // The map of binding indices pointing to the GPUBindGroupLayoutEntrys, which this GPUBindGroupLayout describes.
};

// A GPUBindGroup defines a set of resources to be bound together in a group and how the resources are used in shader stages.

AFX afxNat      AfxEnumerateLegos(afxDrawContext dctx, afxNat first, afxNat cnt, afxLego lego[]);

AFX afxError    AfxBuildLegos(afxDrawContext dctx, afxNat cnt, afxLego lego[], afxLegoBlueprint const blueprint[]);

AFX afxResult   AfxLegoDescribeBinding(afxLego legt, afxNat first, afxNat cnt, afxLegoBindingDecl decl[]);
AFX afxNat32    AfxLegoGetCrc32(afxLego legt);

////////////////////////////////////////////////////////////////////////////////
// BLUEPRINT                                                                  //
////////////////////////////////////////////////////////////////////////////////

AFXINL void     AfxLegoBlueprintBegin(afxLegoBlueprint *blueprint, afxNat estBindCnt);
AFXINL void     AfxLegoBlueprintErase(afxLegoBlueprint *blueprint);
AFXINL afxError AfxLegoBlueprintEnd(afxLegoBlueprint *blueprint, afxLego *legt);

AFXINL afxError AfxLegoBlueprintAddBinding(afxLegoBlueprint *blueprint, afxNat point, afxFlags visibility, afxShaderResourceType type, afxNat cnt, afxString const *name);
AFXINL afxError AfxLegoBlueprintAddShaderContributions(afxLegoBlueprint *blueprint, afxNat set, afxNat cnt, afxShader shd[]);

#endif//AFX_LEGO_H