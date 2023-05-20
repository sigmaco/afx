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

// A draw operation manages a set of state objects, resources, and shaders for implementing a rendering effect.
// An draw operation contains one or more techniques; each technique contains one or more passes; each pass contains state assignments.
// A draw technique is a collection of interrelated passes. Each technique contains one or more passes.
// A draw pass encapsulates state assignments within a technique.
// A pass is a block of code that sets render-state objects and shaders. A pass is declared within a technique.

#ifndef AFX_DRAW_OPERATION_H
#define AFX_DRAW_OPERATION_H

#include "afx/draw/pipelining/afxPipeline.h"

AFX_DEFINE_STRUCT(afxDrawPassBlueprint)
{
    afxString32                 name;
    afxBool                     hasRs;
    afxPipelineRasterizerState  rasterization;
    afxBool                     hasDs;
    afxPipelineDepthState       depthHandling;
    afxBool                     hasIa;
    afxPipelineInputAssemblyState inputAssembly;
    afxNat                      shaderCnt;
    afxNat                      shaderIdx[6];
};

AFX_DEFINE_STRUCT(afxDrawTechniqueBlueprint)
{
    afxString32                 name;
    afxNat                      passCnt;
    afxDrawPassBlueprint        passes[16];
};

AFX_DEFINE_STRUCT(afxDrawOperationBlueprint)
{
    afxFcc                      fcc;
    afxString32                 name;
    afxNat                      reqShaderCnt;
    afxUri                      reqShaders[32];
    afxNat                      techCnt;
    afxDrawTechniqueBlueprint   techs[16];
};

AFX_DEFINE_STRUCT(afxDrawPass)
{
    afxFcc                      fcc; // AFX_FCC_DPAS
    afxString32                 name;
    afxPipeline                 pip;
};

AFX_DEFINE_STRUCT(afxDrawTechnique)
{
    afxFcc                      fcc; // AFX_FCC_DTEC
    afxString32                 name;
    afxNat                      passCnt;
    afxDrawPass*                passes;
};

AFX_DEFINE_HANDLE(afxDrawOperation);

AFX_OBJECT(afxDrawOperation)
{
    afxObject                   obj; // AFX_FCC_DOP
    afxString32                 name;
    afxNat                      techCnt;
    afxDrawTechnique*           techniques;
};

AFX void*                       AfxDrawOperationGetContext(afxDrawOperation dop);
AFX void*                       AfxDrawOperationGetDriver(afxDrawOperation dop);
AFX void*                       AfxDrawOperationGetDrawSystem(afxDrawOperation dop);

AFX afxNat                      AfxDrawOperationGetTechniqueCount(afxDrawOperation dop);
AFX afxNat                      AfxDrawOperationFindTechnique(afxDrawOperation dop, afxString const *name);
AFX afxString const*            AfxDrawOperationGetTechniqueName(afxDrawOperation dop, afxNat tecIdx);

AFX afxNat                      AfxDrawOperationGetPassCount(afxDrawOperation dop, afxNat tecIdx);
AFX afxNat                      AfxDrawOperationFindPass(afxDrawOperation dop, afxNat tecIdx, afxString const *name);
AFX afxString const*            AfxDrawOperationGetPassName(afxDrawOperation dop, afxNat tecIdx, afxNat passIdx);
AFX afxPipeline                 AfxDrawOperationGetPipeline(afxDrawOperation dop, afxNat tecIdx, afxNat passIdx);

////////////////////////////////////////////////////////////////////////////////
// DRAW OPERATION BLUEPRINT                                                   //
////////////////////////////////////////////////////////////////////////////////

AFX afxError                    AfxDrawOperationBlueprintReset(afxDrawOperationBlueprint *blueprint, afxString const *name);
AFX afxError                    AfxDrawOperationBlueprintReflectUrd(afxDrawOperationBlueprint *blueprint, afxUrdNode const *node);
AFX afxError                    AfxDrawOperationBlueprintRename(afxDrawOperationBlueprint *blueprint, afxString const *name);

AFX afxNat                      AfxDrawOperationBlueprintAddTechnique(afxDrawOperationBlueprint *blueprint, afxString const *name);
AFX afxError                    AfxDrawOperationBlueprintRenameTechnique(afxDrawOperationBlueprint *blueprint, afxNat tecIdx, afxString const *name);

AFX afxNat                      AfxDrawOperationBlueprintAddPass(afxDrawOperationBlueprint *blueprint, afxNat tecIdx, afxString const *name, afxNat shaderCnt, afxUri const shaders[], afxPipelineRasterizerState const* rasterization, afxPipelineDepthState const* depthHandling);
AFX afxError                    AfxDrawOperationBlueprintRenamePass(afxDrawOperationBlueprint *blueprint, afxNat tecIdx, afxNat passIdx, afxString const *name);
AFX afxError                    AfxDrawOperationBlueprintAddShaders(afxDrawOperationBlueprint *blueprint, afxNat tecIdx, afxNat passIdx, afxNat cnt, afxUri const uri[]);
AFX afxError                    AfxDrawOperationBlueprintConfigRasterizerState(afxDrawOperationBlueprint *blueprint, afxNat tecIdx, afxNat passIdx, afxPipelineRasterizerState const *state);
AFX afxError                    AfxDrawOperationBlueprintConfigDepthState(afxDrawOperationBlueprint *blueprint, afxNat tecIdx, afxNat passIdx, afxPipelineDepthState const *state);
AFX afxError                    AfxDrawOperationBlueprintConfigInputAssemblyState(afxDrawOperationBlueprint *blueprint, afxNat tecIdx, afxNat passIdx, afxPipelineInputAssemblyState const *state);

#endif//AFX_DRAW_OPERATION_H