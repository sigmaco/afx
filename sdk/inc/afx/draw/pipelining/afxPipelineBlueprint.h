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

// Graphics pipelines consist of multiple shader stages, multiple fixed-function
// pipeline stages, and a pipeline layout.


#ifndef AFX_PIPELINE_BLUEPRINT_H
#define AFX_PIPELINE_BLUEPRINT_H

#include "afx/draw/pipelining/afxPipeline.h"

AFX void                AfxFormulatePipelineBlueprint(afxPipelineBlueprint* blueprint);
AFX void                AfxPipelineBlueprintDiscard(afxPipelineBlueprint *blueprint);
AFX void                AfxPipelineBlueprintErase(afxPipelineBlueprint *blueprint);

AFX afxError            AfxPipelineBlueprintAddShaders(afxPipelineBlueprint *blueprint, afxNat cnt, afxUri const uri[]);
AFX afxError            AfxPipelineBlueprintAddViewports(afxPipelineBlueprint *blueprint, afxNat cnt, afxViewport const vp[]);
AFX afxError            AfxPipelineBlueprintAddScissors(afxPipelineBlueprint *blueprint, afxNat cnt, afxRect const rc[]);

AFX void                AfxPipelineBlueprintConfigInputAssemblyState(afxPipelineBlueprint *blueprint, afxPipelineInputAssemblyState const *state);
AFX void                AfxPipelineBlueprintConfigRasterizerState(afxPipelineBlueprint *blueprint, afxPipelineRasterizerState const *state);
AFX void                AfxPipelineBlueprintConfigDepthState(afxPipelineBlueprint *blueprint, afxPipelineDepthState const *state);
AFX void                AfxPipelineBlueprintConfigMultisampleState(afxPipelineBlueprint *blueprint, afxPipelineMultisampleState const *state);
AFX void                AfxPipelineBlueprintConfigColorBlendState(afxPipelineBlueprint *blueprint, afxPipelineColorBlendState const *state);

AFX afxChar const *shdResTypeNames[AFX_SHD_RES_TYPE_TOTAL];
AFX afxChar const *fmtNames[AFX_FMT_TOTAL];

#endif//AFX_PIPELINE_BLUEPRINT_H