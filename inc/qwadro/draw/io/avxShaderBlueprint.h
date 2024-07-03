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

#ifndef AVX_SHADER_BLUEPRINT_H
#define AVX_SHADER_BLUEPRINT_H

#include "qwadro/draw/io/avxShader.h"

AVX void                AfxShaderBlueprintBegin(avxShaderBlueprint* blueprint, avxShaderStage stage, afxUri const *uri, afxString const *entry, afxNat estCodeLen, afxNat estIoCnt, afxNat estResCnt);
AVX afxError            AfxShaderBlueprintEnd(avxShaderBlueprint *blueprint, afxNat cnt, avxShader shd[]);
AVX void                AfxShaderBlueprintErase(avxShaderBlueprint *blueprint);

AVX void                AfxShaderBlueprintRename(avxShaderBlueprint *blueprint, afxUri const *uri);
AVX void                AfxShaderBlueprintChooseEntryPoint(avxShaderBlueprint *blueprint, afxString const *entry);
AVX void                AfxShaderBlueprintSetStage(avxShaderBlueprint *blueprint, avxShaderStage stage);

AVX afxError            AfxShaderBlueprintAddCode(avxShaderBlueprint *blueprint, void const *start, afxNat range);
AVX afxError            AfxShaderBlueprintAddCodeFromStream(avxShaderBlueprint *blueprint, afxStream ios, afxSize offset, afxNat range);
AVX afxError            AfxShaderBlueprintAddCodeFromResource(avxShaderBlueprint *blueprint, afxUri const *uri);

AVX afxError            AfxShaderBlueprintDeclareResource(avxShaderBlueprint *blueprint, afxNat set, afxNat binding, avxShaderResourceType type, afxNat cnt, afxString const *name);
AVX afxError            AfxShaderBlueprintDeclareInOut(avxShaderBlueprint *blueprint, afxNat location, afxVertexFormat fmt, afxNat stream, afxString const *semantic);

#endif//AVX_SHADER_BLUEPRINT_H
