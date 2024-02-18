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
 *                       (c) 2017 SIGMA, Engineering In Technology
 *                             <https://sigmaco.org/qwadro/>
 */

// This section is part of SIGMA GL/2.

#ifndef AFX_BIND_SCHEMA_H
#define AFX_BIND_SCHEMA_H

// A afxBindSchema defines the interface between a set of resources bound in a afxBindSet and their accessibility in shader stages.

// sgl_frame
// sgl_pass
// sgl_mtl
// sgl_obj

// The descriptor set number 0 will be used for engine-global resources, and bound once per frame.
// The descriptor set number 1 will be used for per-pass resources, and bound once per pass.
// The descriptor set number 2 will be used for material resources, and the number 3 will be used for per-object resources.
// This way, the inner render loops will only be binding descriptor sets 2 and 3, and performance will be high.

#include "qwadro/draw/pipe/afxShader.h"

AFX_DEFINE_STRUCT(afxPipelineRigBlueprintBinding)
{
    afxNat                  binding;
    afxMask                 visibility;
    afxShaderResourceType   type;
    afxNat                  cnt;
    afxRestring       name; // 16
};

AFX_DEFINE_STRUCT(afxPipelineRigBlueprint)
{
    afxFcc                  fcc;
    afxDrawContext          dctx;
    afxArray                bindings; // afxPipelineRigBlueprintBinding
};

AFX_DEFINE_STRUCT(afxPipelineRigBindingDecl)
{
    afxNat                  binding; // A unique identifier for a resource binding within the afxBindSchema, corresponding to a afxBindSchemaEntry.binding and a @binding attribute in the afxShader.
    afxMask                 visibility; // A bitset of the members of afxShaderStage. Each set bit indicates that a afxBindSchemaEntry's resource will be accessible from the associated shader stage.
    afxShaderResourceType   type;
    afxNat                  cnt;
    afxString const*        name;
};

AFX_DEFINE_STRUCT(afxBindSchemaEntry) // A afxBindSchemaEntry describes a single shader resource binding to be included in a afxBindSchema.
{
    // A afxBindSchemaEntry describes a single shader resource binding to be included in a afxBindSchema.
    afxNat32                binding; // A unique identifier for a resource binding within the afxBindSchema, corresponding to a afxBindSchemaEntry.binding and a @binding attribute in the afxShader.
    afxMask                 visibility; // A bitset of the members of afxShaderStage. Each set bit indicates that a afxBindSchemaEntry's resource will be accessible from the associated shader stage.
    afxShaderResourceType   type;
    afxNat                  cnt;
    afxRestring       name; // by QWADRO

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
#ifdef _AFX_BIND_SCHEMA_C
#ifndef _AFX_BIND_SCHEMA_IMPL
AFX_OBJECT(afxBindSchema)
#else
struct afxBaseBindSchema
#endif
{
    afxString           id;
    afxNat32            crc32;
    afxNat              entryCnt;
    afxBindSchemaEntry *entries; // The map of binding indices pointing to the afxBindSchemaEntry, which this afxBindSchema describes.
};
#endif
#endif

// A afxBindSet defines a set of resources to be bound together in a group and how the resources are used in shader stages.

AFX afxError    AfxAcquireBindSchemas(afxDrawContext dctx, afxNat cnt, afxBindSchema schemas[], afxPipelineRigBlueprint const blueprint[]);

AFX afxResult   AfxGetBindSchemaEntry(afxBindSchema legt, afxNat first, afxNat cnt, afxPipelineRigBindingDecl decl[]);
AFX afxNat32    AfxGetBindSchemaHash(afxBindSchema legt);

////////////////////////////////////////////////////////////////////////////////
// BLUEPRINT                                                                  //
////////////////////////////////////////////////////////////////////////////////

AFXINL void     AfxLegoBlueprintBegin(afxPipelineRigBlueprint *blueprint, afxNat estBindCnt);
AFXINL void     AfxLegoBlueprintErase(afxPipelineRigBlueprint *blueprint);
AFXINL afxError AfxLegoBlueprintEnd(afxPipelineRigBlueprint *blueprint, afxBindSchema *legt);

AFXINL afxError AfxLegoBlueprintAddBinding(afxPipelineRigBlueprint *blueprint, afxNat point, afxFlags visibility, afxShaderResourceType type, afxNat cnt, afxString const *name);
AFXINL afxError AfxLegoBlueprintAddShaderContributions(afxPipelineRigBlueprint *blueprint, afxNat set, afxNat cnt, afxShaderBlueprint shdb[]);

#endif//AFX_BIND_SCHEMA_H
