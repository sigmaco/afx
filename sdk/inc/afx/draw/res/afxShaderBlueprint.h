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
 *                               (c) 2017 Federação SIGMA
 *                                    www.sigmaco.org
 */

#ifndef AFX_SHADER_BLUEPRINT_H
#define AFX_SHADER_BLUEPRINT_H

#include "afxShader.h"

AFX_DEFINE_STRUCT(afxShaderBlueprint)
{
    afxFcc                  fcc;
    afxDrawContext          dctx;
    afxUri128               uri; // 128
    afxShaderStage          stage;

    afxString8              entry; // 8
    afxArray                codes; // afxByte    
    afxArray                inOuts; // afxShaderBlueprintInOut
    afxArray                resources; // afxShaderBlueprintResource
    afxPrimTopology         topology;
    
    // push constants?
    // specialization?
};

AFX void                AfxShaderBlueprintBegin(afxShaderBlueprint* blueprint, afxShaderStage stage, afxUri const *uri, afxString const *entry, afxNat estCodeLen, afxNat estIoCnt, afxNat estResCnt);
AFX afxError            AfxShaderBlueprintEnd(afxShaderBlueprint *blueprint, afxNat cnt, afxShader shd[]);
AFX void                AfxShaderBlueprintErase(afxShaderBlueprint *blueprint);

AFX void                AfxShaderBlueprintRename(afxShaderBlueprint *blueprint, afxUri const *uri);
AFX void                AfxShaderBlueprintChooseEntryPoint(afxShaderBlueprint *blueprint, afxString const *entry);
AFX void                AfxShaderBlueprintSetStage(afxShaderBlueprint *blueprint, afxShaderStage stage);

AFX afxError            AfxShaderBlueprintAddCode(afxShaderBlueprint *blueprint, void const *start, afxNat range);
AFX afxError            AfxShaderBlueprintAddCodeFromStream(afxShaderBlueprint *blueprint, afxStream *ios, afxSize offset, afxNat range);
AFX afxError            AfxShaderBlueprintAddCodeFromResource(afxShaderBlueprint *blueprint, afxUri const *uri);

AFX afxError            AfxShaderBlueprintDeclareResource(afxShaderBlueprint *blueprint, afxNat set, afxNat binding, afxShaderResourceType type, afxNat cnt, afxString const *name);
AFX afxError            AfxShaderBlueprintDeclareInOut(afxShaderBlueprint *blueprint, afxNat location, afxVertexFormat fmt, afxString const *semantic);

#endif//AFX_SHADER_BLUEPRINT_H