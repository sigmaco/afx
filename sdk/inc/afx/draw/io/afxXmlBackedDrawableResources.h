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

#ifndef AFX_XML_BACKED_DRAWABLE_RESOURCES_H
#define AFX_XML_BACKED_DRAWABLE_RESOURCES_H

#include "afx/draw/afxDrawContext.h"

#if 0
AFX afxError    AfxUploadXmlBackedDrawOperations(afxNat cnt, afxUri const uri[], afxDrawContext dctx, afxDrawOperation dop[]);
#endif

AFX afxError    AfxParseXmlBackedDrawOperationBlueprint(afxXmlNode const *node, afxDrawOperationBlueprint *blueprint);
AFX afxError    AfxParseXmlBackedShaderBlueprint(afxXmlNode const *node, afxShaderBlueprint *blueprint);

AFX afxError    AfxParseXmlBackedPipelineDepthState(afxXmlNode const *node, afxPipelineDepthState *state, afxNat *foundMask);
AFX afxError    AfxParseXmlBackedPipelineRasterizerState(afxXmlNode const *node, afxPipelineRasterizerState *state, afxNat *foundMask);
AFX afxError    AfxParseXmlBackedPipelineInputAssemblyState(afxXmlNode const *node, afxPipelineInputAssemblyState *state, afxNat *foundMask);

#endif//AFX_XML_BACKED_DRAWABLE_RESOURCES_H