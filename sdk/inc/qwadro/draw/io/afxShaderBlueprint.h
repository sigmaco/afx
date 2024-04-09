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

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#ifndef AFX_SHADER_BLUEPRINT_H
#define AFX_SHADER_BLUEPRINT_H

#include "qwadro/draw/io/afxShader.h"

AVX void                AfxShaderBlueprintBegin(afxShaderBlueprint* blueprint, afxShaderStage stage, afxUri const *uri, afxString const *entry, afxNat estCodeLen, afxNat estIoCnt, afxNat estResCnt);
AVX afxError            AfxShaderBlueprintEnd(afxShaderBlueprint *blueprint, afxNat cnt, afxShader shd[]);
AVX void                AfxShaderBlueprintErase(afxShaderBlueprint *blueprint);

AVX void                AfxShaderBlueprintRename(afxShaderBlueprint *blueprint, afxUri const *uri);
AVX void                AfxShaderBlueprintChooseEntryPoint(afxShaderBlueprint *blueprint, afxString const *entry);
AVX void                AfxShaderBlueprintSetStage(afxShaderBlueprint *blueprint, afxShaderStage stage);

AVX afxError            AfxShaderBlueprintAddCode(afxShaderBlueprint *blueprint, void const *start, afxNat range);
AVX afxError            AfxShaderBlueprintAddCodeFromStream(afxShaderBlueprint *blueprint, afxStream ios, afxSize offset, afxNat range);
AVX afxError            AfxShaderBlueprintAddCodeFromResource(afxShaderBlueprint *blueprint, afxUri const *uri);

AVX afxError            AfxShaderBlueprintDeclareResource(afxShaderBlueprint *blueprint, afxNat set, afxNat binding, afxShaderResourceType type, afxNat cnt, afxString const *name);
AVX afxError            AfxShaderBlueprintDeclareInOut(afxShaderBlueprint *blueprint, afxNat location, afxVertexFormat fmt, afxNat stream, afxString const *semantic);

#endif//AFX_SHADER_BLUEPRINT_H
