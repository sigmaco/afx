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

// This section is part of SIGMA GL/2.

#ifndef AFX_XML_BACKED_DRAWABLE_RESOURCES_H
#define AFX_XML_BACKED_DRAWABLE_RESOURCES_H

#include "afx/draw/afxDrawContext.h"

#if 0
AFX afxError    AfxUploadXmlBackedDrawOperations(afxNat cnt, afxUri const uri[], afxDrawContext dctx, afxDrawOperation dop[]);
#endif

AFX afxError    AfxParseXmlBackedDrawOperationBlueprint(afxXmlNode const *node, afxDrawOperationBlueprint *blueprint);
AFX afxError    AfxParseXmlBackedShaderBlueprint(afxXmlNode const *node, afxShaderBlueprint *blueprint);
AFX afxError    AfxParseXmlBackedPipelineBlueprint(afxXmlNode const *node, afxPipelineConfig *blueprint, afxDrawContext dctx);

//AFX afxError    AfxParseXmlBackedPipelineDepthState(afxXmlNode const *node, afxPipelineDepthState *state, afxNat *foundMask);
//AFX afxError    AfxParseXmlBackedPipelineRasterizerState(afxXmlNode const *node, afxPipelineRasterizerState *state, afxNat *foundMask);
//AFX afxError    AfxParseXmlBackedPipelinePrimitiveState(afxXmlNode const *node, afxPipelinePrimitiveState *state, afxNat *foundMask);

#endif//AFX_XML_BACKED_DRAWABLE_RESOURCES_H