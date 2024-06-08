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

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

// A afxLigature defines the interface between a set of resources bound in a afxBindSet and their accessibility in shader stages.

// A afxBindSet defines a set of resources to be bound together in a group and how the resources are used in shader stages.

// sgl_frame
// sgl_pass
// sgl_mtl
// sgl_obj

// The descriptor set number 0 will be used for engine-global resources, and bound once per frame.
// The descriptor set number 1 will be used for per-pass resources, and bound once per pass.
// The descriptor set number 2 will be used for material resources, and the number 3 will be used for per-object resources.
// This way, the inner render loops will only be binding descriptor sets 2 and 3, and performance will be high.

#ifndef AVX_BIND_SCHEMA_H
#define AVX_BIND_SCHEMA_H

#include "qwadro/draw/io/afxShader.h"

AFX_DEFINE_HANDLE(afxLigature);

AFX_DEFINE_STRUCT(afxPipelineRigBlueprintBinding)
{
    afxNat                  binding;
    afxMask                 visibility;
    afxShaderResourceType   type;
    afxNat                  cnt;
    afxRestring             name; // 16
};

AFX_DEFINE_STRUCT(afxPipelineRigBlueprint)
{
    afxFcc                  fcc;
    afxDrawContext          dctx;
    afxArray                bindings; // afxPipelineRigBlueprintBinding
};

AFX_DEFINE_STRUCT(afxPipelineRigBindingDecl)
{
    afxNat                  binding; // A unique identifier for a resource binding within the afxLigature, corresponding to a afxLigatureEntry.binding and a @binding attribute in the afxShader.
    afxMask                 visibility; // A bitset of the members of afxShaderStage. Each set bit indicates that a afxLigatureEntry's resource will be accessible from the associated shader stage.
    afxShaderResourceType   type;
    afxNat                  cnt;
    afxString const*        name;
};

AFX_DEFINE_STRUCT(afxLigatureEntry) // A afxLigatureEntry describes a single shader resource binding to be included in a afxLigature.
{
    // A afxLigatureEntry describes a single shader resource binding to be included in a afxLigature.
    afxNat                  set;
    afxNat32                binding; // A unique identifier for a resource binding within the afxLigature, corresponding to a afxLigatureEntry.binding and a @binding attribute in the afxShader.
    afxMask                 visibility; // A bitset of the members of afxShaderStage. Each set bit indicates that a afxLigatureEntry's resource will be accessible from the associated shader stage.
    afxShaderResourceType   type;
    afxNat                  cnt;
    afxString8              name;

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

#ifdef _AVX_DRAW_C
#ifdef _AVX_BIND_SCHEMA_C
AFX_DEFINE_STRUCT(afxLigatureSet)
{
    afxNat          set;
    afxNat          baseEntry;
    afxNat          entryCnt;
    afxNat32        crc32;
};
#ifndef _AVX_BIND_SCHEMA_IMPL
AFX_OBJECT(afxLigature)
#else
struct afxBaseLigature
#endif
{
    afxNat              totalEntryCnt;
    afxLigatureEntry*   totalEntries; // The map of binding indices pointing to the afxLigatureEntry, which this afxLigature describes.

    afxNat              setCnt;
    afxLigatureSet*     sets;
};
#endif//_AVX_BIND_SCHEMA_C
#endif//_AVX_DRAW_C

AVX afxNat32        AfxGetLigatureHash(afxLigature liga, afxNat set);

AVX afxResult       AfxGetLigatureEntry(afxLigature liga, afxNat set, afxIndex first, afxNat cnt, afxPipelineRigBindingDecl decl[]);

////////////////////////////////////////////////////////////////////////////////

AFX_DEFINE_STRUCT(afxLigatureConfig)
{
    afxNat      shaderCnt;
    afxShader*  shaders;
};

AVX afxError        AfxBuildLigatures(afxDrawContext dctx, afxNat cnt, afxLigatureConfig const cfg[], afxLigature ligatures[]);

AVX afxLigature     AfxFindLigatures(afxDrawContext dctx, afxNat bindCnt, afxPipelineRigBindingDecl const bindings[]);

////////////////////////////////////////////////////////////////////////////////
// BLUEPRINT                                                                  //
////////////////////////////////////////////////////////////////////////////////

AVXINL void     AfxLegoBlueprintBegin(afxPipelineRigBlueprint *blueprint, afxNat estBindCnt);
AVXINL void     AfxLegoBlueprintErase(afxPipelineRigBlueprint *blueprint);
AVXINL afxError AfxLegoBlueprintEnd(afxPipelineRigBlueprint *blueprint, afxLigature *liga);

AVXINL afxError AfxLegoBlueprintAddBinding(afxPipelineRigBlueprint *blueprint, afxNat point, afxFlags visibility, afxShaderResourceType type, afxNat cnt, afxString const *name);
AVXINL afxError AfxLegoBlueprintAddShaderContributions(afxPipelineRigBlueprint *blueprint, afxNat set, afxNat cnt, afxShaderBlueprint shdb[]);

#endif//AVX_BIND_SCHEMA_H
