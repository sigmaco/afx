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

#ifndef AFX_PIPELINE_BLUEPRINT_H
#define AFX_PIPELINE_BLUEPRINT_H

#include "afxPipeline.h"
#include "afxPipelineRig.h"
#include "afxPipelineModule.h"

AFX_DEFINE_STRUCT(afxPipelineStageBlueprint)
{
    afxShaderStage                  stage;
    afxNat                          srcType; // 0 = pipm, 1 = URI, 2 = code
    union
    {
        afxPipelineModule           pipm;
        afxUri128                   uri;
        afxString4096               code;
    };
};

AFX_DEFINE_STRUCT(afxPipelineBlueprint)
{
    afxFcc                          fcc; // PIPB

    afxPipelineRig                  rig;
    afxNat32                        socketCnt; // if pipa isn't specified, the constructor will use it.
    afxLegoSchema               sockets[4]; // if pipa isn't specified, the pip constructor must create one of this data.

    afxNat                          stageCnt;
    afxPipelineStageBlueprint       stages[8];
    afxNat                          inStreamCnt;
    afxPipelineInputStream          inStreams[8];
    afxNat                          vpCnt;
    afxViewport                     vps[8];
    afxNat                          scissorCnt;
    afxRect                         scissors[8];

    afxBool                         hasAssembling;
    afxPipelineInputAssemblyState   inAssembling;

    afxBool                         hasRasterization;
    afxPipelineRasterizerState   rasterization;

    afxBool                         hasDepthHandling;
    afxPipelineDepthState           depthHandling;

    afxBool                         hasMultisampling;
    afxPipelineMultisampleState     multisampling;

    afxBool                         hasColorBlending;
    afxPipelineColorBlendState      colorBlending;
};

AFX afxError    AfxPipelineBlueprintDeploy(afxPipelineBlueprint *pipb);
AFX afxError    AfxPipelineBlueprintDrop(afxPipelineBlueprint *pipb);

AFX afxError    AfxPipelineBlueprintValidate(afxPipelineBlueprint *pipb);

AFX afxError    AfxPipelineBlueprintAddModule(afxPipelineBlueprint *pipb, afxShaderStage stage, afxPipelineModule pipm, void const *code, afxNat codeLen, afxUri const *uri);
AFX afxError    AfxPipelineBlueprintResetStage(afxPipelineBlueprint *pipb, afxNat idx, afxShaderStage stage, afxPipelineModule pipm, void const *code, afxNat codeLen, afxUri const *uri);

AFX afxError    AfxPipelineBlueprintAddInputStream(afxPipelineBlueprint *pipb, afxNat location, afxNat binding, afxVertexFormat fmt, afxNat offset);
AFX afxError    AfxPipelineBlueprintResetInputStream(afxPipelineBlueprint *pipb, afxNat streamIdx, afxNat location, afxNat binding, afxVertexFormat fmt, afxNat offset);

AFX afxError    AfxPipelineBlueprintAddViewports(afxPipelineBlueprint *pipb, afxNat cnt, afxViewport const vp[]);
AFX afxError    AfxPipelineBlueprintResetViewport(afxPipelineBlueprint *pipb, afxNat idx, afxViewport const *vp);

AFX afxError    AfxPipelineBlueprintAddScissors(afxPipelineBlueprint *pipb, afxNat cnt, afxRect const rc[]);
AFX afxError    AfxPipelineBlueprintResetScissor(afxPipelineBlueprint *pipb, afxNat idx, afxRect const *rc);

AFX afxError    AfxPipelineBlueprintDefineInputAssemblyState(afxPipelineBlueprint *pipb, afxPrimTopology topology, afxBool primRestartEnable);

AFX afxError    AfxPipelineBlueprintDefineRasterizerState(afxPipelineBlueprint *pipb, afxPipelineRasterizerState const *state);
AFX afxError    AfxPipelineBlueprintDefineDepthState(afxPipelineBlueprint *pipb, afxPipelineDepthState const *state);
AFX afxError    AfxPipelineBlueprintDefineMultisampleState(afxPipelineBlueprint *pipb, afxPipelineMultisampleState const *state);
AFX afxError    AfxPipelineBlueprintDefineColorBlendState(afxPipelineBlueprint *pipb, afxBool logicOpEnable, afxLogicOp logicOp, afxReal const blendConstants[4]);



AFX afxChar const *pipmResTypeNames[AFX_SUPPLY_TYPE_TOTAL];
AFX afxChar const *fmtNames[AFX_FMT_TOTAL];

#endif//AFX_PIPELINE_BLUEPRINT_H