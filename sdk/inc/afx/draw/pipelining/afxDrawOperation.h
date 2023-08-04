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

// A draw operation manages a set of state objects, resources, and shaders for implementing a rendering effect.
// An draw operation contains one or more techniques; each technique contains one or more passes; each pass contains state assignments.
// A draw technique is a collection of interrelated passes. Each technique contains one or more passes.
// A draw pass encapsulates state assignments within a technique.
// A pass is a block of code that sets render-state objects and shaders. A pass is declared within a technique.

#ifndef AFX_DRAW_OPERATION_H
#define AFX_DRAW_OPERATION_H

#include "afx/draw/pipelining/afxPipeline.h"

AFX_DEFINE_STRUCT(afxDrawOperationBlueprintTechniquePass)
{
    afxString*                      name; // 32
    afxBool                         hasIa;
    afxBool                         hasRs;
    afxBool                         hasDs;
    afxPipelineInputAssemblyState   inputAssembly;
    afxPipelineRasterizerState      rasterization;
    afxPipelineDepthState           depthHandling;
    afxArray                        shaders;
};

AFX_DEFINE_STRUCT(afxDrawOperationBlueprintTechnique)
{
    afxString*                      name; // 32
    afxArray                        passes;
};

AFX_DEFINE_STRUCT(afxDrawOperationBlueprint)
{
    afxFcc                          fcc;
    afxDrawContext                  dctx;
    afxUri128                       uri; // 128
    afxArray                        techniques;
};

AFX_DEFINE_STRUCT(afxDrawPass)
{
    afxFcc                          fcc; // AFX_FCC_DPAS
    afxString*                      name; // 32
    afxPipeline                     pip;
};

AFX_DEFINE_STRUCT(afxDrawTechnique)
{
    afxFcc                          fcc; // AFX_FCC_DTEC
    afxString*                      name; // 32
    afxNat                          passCnt;
    afxDrawPass*                    passes;
};

AFX_OBJECT(afxDrawOperation)
{
    afxObject                       obj; // AFX_FCC_DOP
    afxUri*                         uri; // 128
    afxNat                          techCnt;
    afxDrawTechnique*               techniques;
};

AFX void*                           AfxDrawOperationGetContext(afxDrawOperation dop);

AFX afxNat                          AfxDrawOperationGetTechniqueCount(afxDrawOperation dop);
AFX afxBool                         AfxDrawOperationFindTechnique(afxDrawOperation dop, afxString const *name, afxNat *idx);
AFX afxString const*                AfxDrawOperationGetTechniqueName(afxDrawOperation dop, afxNat tecIdx);

AFX afxNat                          AfxDrawOperationGetPassCount(afxDrawOperation dop, afxNat tecIdx);
AFX afxBool                         AfxDrawOperationFindPass(afxDrawOperation dop, afxNat tecIdx, afxString const *name, afxNat *idx);
AFX afxString const*                AfxDrawOperationGetPassName(afxDrawOperation dop, afxNat tecIdx, afxNat passIdx);
AFX afxPipeline                     AfxDrawOperationGetPipeline(afxDrawOperation dop, afxNat tecIdx, afxNat passIdx);

////////////////////////////////////////////////////////////////////////////////
// DRAW OPERATION BLUEPRINT                                                   //
////////////////////////////////////////////////////////////////////////////////

AFX void                            AfxDrawOperationBlueprintBegin(afxDrawOperationBlueprint* blueprint, afxDrawContext dctx, afxUri const *uri, afxNat estShaderCnt, afxNat estTechCnt);
AFX afxError                        AfxDrawOperationBlueprintEnd(afxDrawOperationBlueprint *blueprint, afxNat cnt, afxDrawOperation dop[]);
AFX void                            AfxDrawOperationBlueprintErase(afxDrawOperationBlueprint *blueprint);

AFX void                            AfxDrawOperationBlueprintRename(afxDrawOperationBlueprint *blueprint, afxUri const *name);

AFX afxError                        AfxDrawOperationBlueprintAddTechnique(afxDrawOperationBlueprint *blueprint, afxNat *tecIdx, afxString const *name);
AFX void                            AfxDrawOperationBlueprintRenameTechnique(afxDrawOperationBlueprint *blueprint, afxNat tecIdx, afxString const *name);

AFX afxError                        AfxDrawOperationBlueprintAddPass(afxDrawOperationBlueprint *blueprint, afxNat tecIdx, afxNat *passIdx, afxString const *name);
AFX void                            AfxDrawOperationBlueprintRenamePass(afxDrawOperationBlueprint *blueprint, afxNat tecIdx, afxNat passIdx, afxString const *name);
AFX afxError                        AfxDrawOperationBlueprintAddShaders(afxDrawOperationBlueprint *blueprint, afxNat tecIdx, afxNat passIdx, afxNat cnt, afxUri const uri[]);
AFX void                            AfxDrawOperationBlueprintConfigRasterizerState(afxDrawOperationBlueprint *blueprint, afxNat tecIdx, afxNat passIdx, afxPipelineRasterizerState const *state);
AFX void                            AfxDrawOperationBlueprintConfigDepthState(afxDrawOperationBlueprint *blueprint, afxNat tecIdx, afxNat passIdx, afxPipelineDepthState const *state);
AFX void                            AfxDrawOperationBlueprintConfigInputAssemblyState(afxDrawOperationBlueprint *blueprint, afxNat tecIdx, afxNat passIdx, afxPipelineInputAssemblyState const *state);

#endif//AFX_DRAW_OPERATION_H