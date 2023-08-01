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

// Graphics pipelines consist of multiple shader stages, multiple fixed-function
// pipeline stages, and a pipeline layout.


#ifndef AFX_PIPELINE_BLUEPRINT_H
#define AFX_PIPELINE_BLUEPRINT_H

#include "afx/draw/pipelining/afxPipeline.h"

AFX_DEFINE_STRUCT(afxPipelineBlueprint)
{
    afxFcc                          fcc; // PIPB
    afxDrawContext                  dctx;

    afxArray                        shaders;
    afxArray                        viewports;
    afxArray                        scissors;

    afxBool                         hasAssembling;
    afxBool                         hasRasterization;
    afxBool                         hasDepthHandling;
    afxBool                         hasMultisampling;
    afxBool                         hasColorBlending;

    afxPipelineInputAssemblyState   inAssembling;
    afxPipelineRasterizerState      rasterization;
    afxPipelineDepthState           depthHandling;
    afxPipelineMultisampleState     multisampling;
    afxPipelineColorBlendState      colorBlending;
};

AFX void                AfxPipelineBlueprintFormulate(afxPipelineBlueprint* blueprint, afxDrawContext dctx);
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