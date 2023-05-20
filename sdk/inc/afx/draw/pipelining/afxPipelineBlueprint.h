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

#include "afx/draw/pipelining/afxPipeline.h"
#include "afx/draw/pipelining/afxPipelineRig.h"
#include "afx/draw/pipelining/afxShader.h"

AFX_DEFINE_STRUCT(afxSource) // source for blueprints
{
    afxFcc              type; // raw, OBJ, URI or STR
    union
    {
        void*           raw;
        afxObject*      obj;
        afxUri128       uri; // map subentry uri to don't use in-place URI storage.
        afxString512    str; // map subentry str to don't use in-place string storage.
    };
    union
    {
        void*           udp;
        afxSize         udd;
    };
};

AFX_DEFINE_STRUCT(afxPipelineStageBlueprint)
{
    afxShaderStage                  stage;
    afxNat                          srcType; // 0 = shd, 1 = URI, 2 = code
    union
    {
        afxShader           shd;
        afxUri128                   uri;
        afxString4096               code;
    };
};

AFX_DEFINE_STRUCT(afxPipelineBlueprint)
{
    afxFcc                          fcc; // PIPB

    afxPipelineRig                  rig;
    afxNat32                        socketCnt; // if pipa isn't specified, the constructor will use it.
    afxLegoTemplate                   sockets[4]; // if pipa isn't specified, the pip constructor must create one of this data.

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
    afxPipelineRasterizerState      rasterization;

    afxBool                         hasDepthHandling;
    afxPipelineDepthState           depthHandling;

    afxBool                         hasMultisampling;
    afxPipelineMultisampleState     multisampling;

    afxBool                         hasColorBlending;
    afxPipelineColorBlendState      colorBlending;
};

AFX afxError    AfxPipelineBlueprintReset(afxPipelineBlueprint *pipb);

AFX afxError    AfxPipelineBlueprintAddShader(afxPipelineBlueprint *pipb, afxShaderStage stage, afxShader shd, void const *code, afxNat codeLen, afxUri const *uri);
AFX afxError    AfxPipelineBlueprintResetStage(afxPipelineBlueprint *pipb, afxNat idx, afxShaderStage stage, afxShader shd, void const *code, afxNat codeLen, afxUri const *uri);

AFX afxError    AfxPipelineBlueprintAddInputStream(afxPipelineBlueprint *pipb, afxNat location, afxNat binding, afxVertexFormat fmt, afxNat offset);
AFX afxError    AfxPipelineBlueprintResetInputStream(afxPipelineBlueprint *pipb, afxNat streamIdx, afxNat location, afxNat binding, afxVertexFormat fmt, afxNat offset);

AFX afxError    AfxPipelineBlueprintAddViewports(afxPipelineBlueprint *pipb, afxNat cnt, afxViewport const vp[]);
AFX afxError    AfxPipelineBlueprintResetViewport(afxPipelineBlueprint *pipb, afxNat idx, afxViewport const *vp);

AFX afxError    AfxPipelineBlueprintAddScissors(afxPipelineBlueprint *pipb, afxNat cnt, afxRect const rc[]);
AFX afxError    AfxPipelineBlueprintResetScissor(afxPipelineBlueprint *pipb, afxNat idx, afxRect const *rc);

AFX afxError    AfxPipelineBlueprintResetInputAssemblyState(afxPipelineBlueprint *pipb, afxPrimTopology topology, afxBool primRestartEnable);

AFX afxError    AfxPipelineBlueprintResetRasterizerState(afxPipelineBlueprint *pipb, afxPipelineRasterizerState const *state);
AFX afxError    AfxPipelineBlueprintResetDepthState(afxPipelineBlueprint *pipb, afxPipelineDepthState const *state);
AFX afxError    AfxPipelineBlueprintResetMultisampleState(afxPipelineBlueprint *pipb, afxPipelineMultisampleState const *state);
AFX afxError    AfxPipelineBlueprintResetColorBlendState(afxPipelineBlueprint *pipb, afxBool logicOpEnable, afxLogicOp logicOp, afxReal const blendConstants[4]);



AFX afxChar const *shdResTypeNames[AFX_SHD_RES_TYPE_TOTAL];
AFX afxChar const *fmtNames[AFX_FMT_TOTAL];

#endif//AFX_PIPELINE_BLUEPRINT_H