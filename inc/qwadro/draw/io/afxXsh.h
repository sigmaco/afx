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
 *                       (c) 2017 SIGMA, Engitech, Scitech, Serpro
 *                             <https://sigmaco.org/qwadro/>
 */

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#ifndef AFX_XML_BACKED_DRAWABLE_RESOURCES_H
#define AFX_XML_BACKED_DRAWABLE_RESOURCES_H

#include "qwadro/draw/dev/afxDrawContext.h"

#if 0
AVX afxError    AfxUploadXmlBackedDrawOperations(afxNat cnt, afxUri const uri[], afxDrawContext dctx, afxDrawOperation dop[]);
#endif


AVX afxError    AfxParseXmlBackedShaderBlueprint(afxShaderBlueprint *blueprint, afxNat specIdx, afxXml const* xml, afxNat elemIdx);

AVX afxError    AfxLoadPipelineConfigFromXml(afxPipelineConfig* config, afxPipelineConfig const* identity, afxNat specIdx, afxXml const* xml, afxNat elemIdx);
AVX afxError    AfxLoadRasterizationConfigFromXml(afxRasterizationConfig* config, afxRasterizationConfig const* identity, afxNat specIdx, afxXml const* xml, afxNat elemIdx);

AVX afxError AfxLoadGlScript(afxShaderBlueprint* bp, afxUri const* path);

#endif//AFX_XML_BACKED_DRAWABLE_RESOURCES_H
