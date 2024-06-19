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

#ifndef AFX_XML_BACKED_DRAWABLE_RESOURCES_H
#define AFX_XML_BACKED_DRAWABLE_RESOURCES_H

#include "qwadro/draw/afxDrawContext.h"

#if 0
AVX afxError    AfxUploadXmlBackedDrawOperations(afxNat cnt, afxUri const uri[], afxDrawContext dctx, afxDrawOperation dop[]);
#endif


AVX afxError    AfxParseXmlBackedShaderBlueprint(avxShaderBlueprint *blueprint, afxNat specIdx, afxXml const* xml, afxNat elemIdx);
AVX afxError    AfxParsePipelineFromXsh(avxPipelineBlueprint* pipb, afxUri const* uri);

AVX afxError    AfxLoadPipelineConfigFromXml(avxPipelineBlueprint* config, afxNat specIdx, afxXml const* xml, afxNat elemIdx);
AVX afxError    AfxLoadRasterizationConfigFromXml(avxRasterizationConfig* config, avxRasterizationConfig const* identity, afxNat specIdx, afxXml const* xml, afxNat elemIdx);

AVX afxError AfxLoadGlScript(avxShaderBlueprint* bp, afxUri const* path);

#endif//AFX_XML_BACKED_DRAWABLE_RESOURCES_H
