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

// This section is part of SIGMA GL/2.

#ifndef AFX_PIPELINE_RIG_H
#define AFX_PIPELINE_RIG_H

// A afxPipelineRig defines the interface between a set of resources bound in a GPUBindGroup and their accessibility in shader stages.

// sgl_frame
// sgl_pass
// sgl_mtl
// sgl_obj

// The descriptor set number 0 will be used for engine-global resources, and bound once per frame.
// The descriptor set number 1 will be used for per-pass resources, and bound once per pass.
// The descriptor set number 2 will be used for material resources, and the number 3 will be used for per-object resources.
// This way, the inner render loops will only be binding descriptor sets 2 and 3, and performance will be high.


#include "afx/draw/afxBuffer.h"
#include "afx/draw/afxShader.h"
#include "afx/draw/afxSampler.h"

AFX_DEFINE_STRUCT(afxPipelineRigBlueprintBinding)
{
    afxNat                  binding;
    afxFlags                visibility;
    afxShaderResourceType   type;
    afxNat                  cnt;
    afxString               name; // 16
};

AFX_DEFINE_STRUCT(afxPipelineRigBlueprint)
{
    afxFcc                  fcc;
    afxDrawContext          dctx;
    afxArray                bindings; // afxPipelineRigBlueprintBinding
};

AFX_DEFINE_STRUCT(afxPipelineRigBindingDecl)
{
    afxNat                  binding; // A unique identifier for a resource binding within the afxPipelineRigScheme, corresponding to a afxPipelineRigDataScheme.binding and a @binding attribute in the afxShader.
    afxFlags                visibility; // A bitset of the members of afxShaderStage. Each set bit indicates that a afxPipelineRigDataScheme's resource will be accessible from the associated shader stage.
    afxShaderResourceType   type;
    afxNat                  cnt;
    afxString const*        name;
};

AFX_DEFINE_STRUCT(afxPipelineRigEntry) // A GPUBindGroupLayoutEntry describes a single shader resource binding to be included in a GPUBindGroupLayout.
{
    // A afxPipelineRigDataScheme describes a single shader resource binding to be included in a afxPipelineRigScheme.
    afxNat32                binding; // A unique identifier for a resource binding within the afxPipelineRigScheme, corresponding to a afxPipelineRigDataScheme.binding and a @binding attribute in the afxShader.
    afxFlags                visibility; // A bitset of the members of afxShaderStage. Each set bit indicates that a afxPipelineRigDataScheme's resource will be accessible from the associated shader stage.
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

#ifdef _AFX_DRAW_C
#ifdef _AFX_PIPELINE_RIG_C
AFX_OBJECT(afxPipelineRig)
#else
struct afxBasePipelineRig
#endif
{
    afxNat32            crc32;
    afxNat              entryCnt;
    afxPipelineRigEntry *entries; // The map of binding indices pointing to the GPUBindGroupLayoutEntrys, which this GPUBindGroupLayout describes.
};
#endif

// A GPUBindGroup defines a set of resources to be bound together in a group and how the resources are used in shader stages.

AFX afxError    AfxBuildPipelineRigs(afxDrawContext dctx, afxNat cnt, afxPipelineRig lego[], afxPipelineRigBlueprint const blueprint[]);

AFX afxResult   AfxGetPipelineRigEntry(afxPipelineRig legt, afxNat first, afxNat cnt, afxPipelineRigBindingDecl decl[]);
AFX afxNat32    AfxGetPipelineRigHash(afxPipelineRig legt);

////////////////////////////////////////////////////////////////////////////////
// BLUEPRINT                                                                  //
////////////////////////////////////////////////////////////////////////////////

AFXINL void     AfxLegoBlueprintBegin(afxPipelineRigBlueprint *blueprint, afxNat estBindCnt);
AFXINL void     AfxLegoBlueprintErase(afxPipelineRigBlueprint *blueprint);
AFXINL afxError AfxLegoBlueprintEnd(afxPipelineRigBlueprint *blueprint, afxPipelineRig *legt);

AFXINL afxError AfxLegoBlueprintAddBinding(afxPipelineRigBlueprint *blueprint, afxNat point, afxFlags visibility, afxShaderResourceType type, afxNat cnt, afxString const *name);
AFXINL afxError AfxLegoBlueprintAddShaderContributions(afxPipelineRigBlueprint *blueprint, afxNat set, afxNat cnt, afxShader shd[]);

#endif//AFX_PIPELINE_RIG_H