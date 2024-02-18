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

#ifndef AFX_XML_BACKED_DRAWABLE_RESOURCES_H
#define AFX_XML_BACKED_DRAWABLE_RESOURCES_H

#include "qwadro/draw/afxDrawContext.h"

#if 0
AFX afxError    AfxUploadXmlBackedDrawOperations(afxNat cnt, afxUri const uri[], afxDrawContext dctx, afxDrawOperation dop[]);
#endif


AFX afxError    AfxParseXmlBackedShaderBlueprint(afxShaderBlueprint *blueprint, afxNat specIdx, afxXml const* xml, afxNat elemIdx);

AFX afxError    AfxLoadPipelineConfigFromXml(afxPipelineConfig* config, afxPipelineConfig const* identity, afxNat specIdx, afxXml const* xml, afxNat elemIdx);
AFX afxError    AfxLoadRasterizationConfigFromXml(afxRasterizationConfig* config, afxRasterizationConfig const* identity, afxNat specIdx, afxXml const* xml, afxNat elemIdx);

AFX afxError AfxLoadGlScript(afxShaderBlueprint* bp, afxUri const* path);

#endif//AFX_XML_BACKED_DRAWABLE_RESOURCES_H
