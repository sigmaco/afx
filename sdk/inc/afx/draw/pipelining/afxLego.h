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

#ifndef AFX_LEGO_H
#define AFX_LEGO_H

#include "afx/draw/res/afxBuffer.h"
#include "afxShader.h"
#include "afx/draw/pipelining/afxSampler.h"

AFX_DEFINE_STRUCT(afxLegoBlueprintBinding)
{
    afxNat                  binding;
    afxFlags                visibility;
    afxShaderResourceType   type;
    afxNat                  cnt;
    afxString*              name; // 16
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

AFX_DEFINE_HANDLE(afxLego);

#ifndef AFX_DRAW_DRIVER_SRC

AFX_OBJECT(afxLego)
{
    afxObject           obj;
};

#endif


// A GPUBindGroup defines a set of resources to be bound together in a group and how the resources are used in shader stages.

AFX afxResult AfxLegoDescribeBinding(afxLego legt, afxNat first, afxNat cnt, afxLegoBindingDecl decl[]);
AFX afxNat32 AfxLegoGetCrc32(afxLego legt);

////////////////////////////////////////////////////////////////////////////////
// BLUEPRINT                                                                  //
////////////////////////////////////////////////////////////////////////////////

AFXINL void     AfxLegoBlueprintBegin(afxLegoBlueprint *blueprint, afxDrawContext dctx, afxNat estBindCnt);
AFXINL void     AfxLegoBlueprintErase(afxLegoBlueprint *blueprint);
AFXINL afxError AfxLegoBlueprintEnd(afxLegoBlueprint *blueprint, afxLego *legt);

AFXINL afxError AfxLegoBlueprintAddBinding(afxLegoBlueprint *blueprint, afxNat point, afxFlags visibility, afxShaderResourceType type, afxNat cnt, afxString const *name);
AFXINL afxError AfxLegoBlueprintAddShaderContributions(afxLegoBlueprint *blueprint, afxNat set, afxNat cnt, afxShader shd[]);

#endif//AFX_LEGO_H