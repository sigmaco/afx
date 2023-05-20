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

#ifndef AFX_UNIFORM_PIPELINE_DESCRIPTOR
#define AFX_UNIFORM_PIPELINE_DESCRIPTOR

#include "afx/draw/afxDrawContext.h"

AFX afxPipeline _AfxDrawContextUploadPipelineUrd(afxDrawContext dctx, afxUri const *uri);

AFX afxError AfxReflectSerializedPipelineRasterizerState(afxPipelineRasterizerState *state, afxUrdNode const *node);
AFX afxError AfxReflectSerializedPipelineDepthState(afxPipelineDepthState *state, afxUrdNode const *node);
AFX afxError AfxReflectSerializedPipelineInputAssemblyState(afxPipelineInputAssemblyState *state, afxUrdNode const *node);

#endif//AFX_UNIFORM_PIPELINE_DESCRIPTOR