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

// A draw operation manages a set of state objects, resources, and shaders for implementing a rendering effect.
// An draw operation contains one or more techniques; each technique contains one or more passes; each pass contains state assignments.
// A draw technique is a collection of interrelated passes. Each technique contains one or more passes.
// A draw pass encapsulates state assignments within a technique.
// A pass is a block of code that sets render-state objects and shaders. A pass is declared within a technique.

#ifndef AFX_DRAW_OPERATION_H
#define AFX_DRAW_OPERATION_H

#include "qwadro/draw/pipe/afxPipeline.h"

AFX_DEFINE_STRUCT(afxDrawOperationBlueprintTechniquePass)
{
    afxString*                      name; // 32
    afxBool                         hasIa;
    afxBool                         hasRs;
    afxBool                         hasDs;
    //afxPipelinePrimitiveState   inputAssembly;
    //afxPipelineRasterizerState      rasterization;
    //afxPipelineDepthState           depthHandling;
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

AFX_OBJECT(afxDrawOperation)
{
    afxHandle                       obj; // afxFcc_DOP
    afxUri*                         uri; // 128
    afxNat                          techCnt;
    afxDrawTechnique2*               techniques;
};

AVX void*                           AfxDrawOperationGetContext(afxDrawOperation dop);

AVX afxNat                          AfxDrawOperationGetTechniqueCount(afxDrawOperation dop);
AVX afxBool                         AfxDrawOperationFindTechnique(afxDrawOperation dop, afxString const *name, afxNat *idx);
AVX afxString const*                AfxDrawOperationGetTechniqueName(afxDrawOperation dop, afxNat tecIdx);

AVX afxNat                          AfxDrawOperationGetPassCount(afxDrawOperation dop, afxNat tecIdx);
AVX afxBool                         AfxDrawOperationFindPass(afxDrawOperation dop, afxNat tecIdx, afxString const *name, afxNat *idx);
AVX afxString const*                AfxDrawOperationGetPassName(afxDrawOperation dop, afxNat tecIdx, afxNat passIdx);
AVX afxPipeline                     AfxDrawOperationGetPipeline(afxDrawOperation dop, afxNat tecIdx, afxNat passIdx);

////////////////////////////////////////////////////////////////////////////////
// DRAW OPERATION BLUEPRINT                                                   //
////////////////////////////////////////////////////////////////////////////////

AVX void                            AfxDrawOperationBlueprintBegin(afxDrawOperationBlueprint* blueprint, afxDrawContext dctx, afxUri const *uri, afxNat estShaderCnt, afxNat estTechCnt);
AVX afxError                        AfxDrawOperationBlueprintEnd(afxDrawOperationBlueprint *blueprint, afxNat cnt, afxDrawOperation dop[]);
AVX void                            AfxDrawOperationBlueprintErase(afxDrawOperationBlueprint *blueprint);

AVX void                            AfxDrawOperationBlueprintRename(afxDrawOperationBlueprint *blueprint, afxUri const *name);

AVX afxError                        AfxDrawOperationBlueprintAddTechnique(afxDrawOperationBlueprint *blueprint, afxNat *tecIdx, afxString const *name);
AVX void                            AfxDrawOperationBlueprintRenameTechnique(afxDrawOperationBlueprint *blueprint, afxNat tecIdx, afxString const *name);

AVX afxError                        AfxDrawOperationBlueprintAddPass(afxDrawOperationBlueprint *blueprint, afxNat tecIdx, afxNat *passIdx, afxString const *name);
AVX void                            AfxDrawOperationBlueprintRenamePass(afxDrawOperationBlueprint *blueprint, afxNat tecIdx, afxNat passIdx, afxString const *name);
AVX afxError                        AfxDrawOperationBlueprintAddShaders(afxDrawOperationBlueprint *blueprint, afxNat tecIdx, afxNat passIdx, afxNat cnt, afxUri const uri[]);
//AVX void                            AfxDrawOperationBlueprintConfigRasterizerState(afxDrawOperationBlueprint *blueprint, afxNat tecIdx, afxNat passIdx, afxPipelineRasterizerState const *state);
//AVX void                            AfxDrawOperationBlueprintConfigDepthState(afxDrawOperationBlueprint *blueprint, afxNat tecIdx, afxNat passIdx, afxPipelineDepthState const *state);
//AVX void                            AfxDrawOperationBlueprintConfigInputAssemblyState(afxDrawOperationBlueprint *blueprint, afxNat tecIdx, afxNat passIdx, afxPipelinePrimitiveState const *state);

#endif//AFX_DRAW_OPERATION_H
