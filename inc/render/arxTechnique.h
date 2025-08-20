/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *             Q W A D R O   R E N D E R I N G   I N F R A S T R U C T U R E
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#ifndef ARX_TECHNIQUE_H
#define ARX_TECHNIQUE_H

/**
    A draw operation manages a set of state objects, resources, and shaders for implementing a rendering effect.
    An draw operation contains one or more techniques; each technique contains one or more passes; each pass contains state assignments.
    A draw technique is a collection of interrelated passes. Each technique contains one or more passes.
    A draw pass encapsulates state assignments within a technique.
    A pass is a block of code that sets render-state objects and shaders. A pass is declared within a technique.
*/

#include "qwadro/inc/render/arxRenderable.h"
#include "qwadro/inc/draw/op/avxPipeline.h"
#include "qwadro/inc/draw/op/avxRasterization.h"

AFX_DEFINE_STRUCT(arxDrawPass)
{
    avxPipeline         pip;
};

AFX_DEFINE_STRUCT(arxDrawSubpass)
{
    afxString           id; // 32
    avxPipeline         pip;
};

AFX_OBJECT(arxTechnique)
{
    afxUnit             shadCnt;
    avxShader*          shaders;
    afxUnit             pipCnt;
    afxString*          pipUrnMap;
    avxPipeline*        pipelines;
    afxUnit             razrCnt;
    afxString*          razrUrnMap;
    avxRasterizer*      rasterizers;

    afxUri128           uri;
    afxUnit             passCnt;
    arxDrawPass*        passes;
    afxString*          passUrnMap;

};

ARX afxString const*    ArxGetDrawTechniqueName(arxTechnique dtec);

ARX afxUnit             ArxCountDrawPasses(arxTechnique dtec);
ARX afxBool             ArxFindDrawPass(arxTechnique dtec, afxString const *name, afxUnit *idx);
ARX afxString const*    ArxGetDrawPassName(arxTechnique dtec, afxUnit passIdx);
ARX avxPipeline         ArxGetDrawPipeline(arxTechnique dtec, afxUnit passIdx);

ARX afxError            ArxUpdateRasterizationPass(arxTechnique dtec, afxUnit passIdx, avxPipelineBlueprint const* cfg);

ARX afxCmdId            AvxCmdUseDrawTechniqueSIGMA(afxDrawContext dctx, arxTechnique dtec, afxUnit passIdx, avxVertexInput vin, afxFlags dynamics);

////////////////////////////////////////////////////////////////////////////////

ARX afxError            ArxLoadDrawTechnique(arxRenderware din, afxUri const* uri, arxTechnique* technique);

#endif//ARX_TECHNIQUE_H
