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
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

/// A draw operation manages a set of state objects, resources, and shaders for implementing a rendering effect.
/// An draw operation contains one or more techniques; each technique contains one or more passes; each pass contains state assignments.
/// A draw technique is a collection of interrelated passes. Each technique contains one or more passes.
/// A draw pass encapsulates state assignments within a technique.
/// A pass is a block of code that sets render-state objects and shaders. A pass is declared within a technique.

#ifndef AVX_DRAW_TECHNIQUE_H
#define AVX_DRAW_TECHNIQUE_H

#include "qwadro/inc/draw/pipe/avxPipeline.h"
#include "qwadro/inc/draw/pipe/avxRasterizationOps.h"
#include "qwadro/inc/draw/pipe/avxRasterizer.h"

AFX_DEFINE_STRUCT(afxDrawPass)
{
    avxPipeline         pip;
    avxRasterizer       razr;
};

AFX_DEFINE_STRUCT(afxDrawSubpass)
{
    afxString           id; // 32
    avxPipeline         pip;
    avxRasterizer       razr;
};

AFX_OBJECT(afxDrawTechnique)
{
    afxNat              shadCnt;
    avxShader*          shaders;
    afxNat              pipCnt;
    afxString*          pipUrnMap;
    avxPipeline*        pipelines;
    afxNat              razrCnt;
    afxString*          razrUrnMap;
    avxRasterizer*      rasterizers;

    afxUri128           uri;
    afxNat              passCnt;
    afxDrawPass*        passes;
    afxString*          passUrnMap;

};

AVX afxString const*    AfxGetDrawTechniqueName(afxDrawTechnique dtec);

AVX afxNat              AfxCountDrawPasses(afxDrawTechnique dtec);
AVX afxBool             AfxFindDrawPass(afxDrawTechnique dtec, afxString const *name, afxNat *idx);
AVX afxString const*    AfxGetDrawPassName(afxDrawTechnique dtec, afxNat passIdx);
AVX avxPipeline         AfxGetDrawPipeline(afxDrawTechnique dtec, afxNat passIdx);

AVX afxError            AfxUpdateRasterizationPass(afxDrawTechnique dtec, afxNat passIdx, avxRasterizationConfig const* cfg);

AVX afxCmdId            AvxCmdApplyDrawTechnique(avxCmdb cmdb, afxDrawTechnique dtec, afxNat passIdx, avxVertexInput vin, afxFlags dynamics);

////////////////////////////////////////////////////////////////////////////////

AVX afxError            AfxAcquireDrawTechniques(afxDrawInput din, afxNat cnt, afxNat const passCnt[], afxDrawTechnique techniques[]);

AVX afxError            AfxLoadDrawTechniques(afxDrawInput din, afxNat cnt, afxUri const uris[], afxDrawTechnique techniques[]);

#endif//AVX_DRAW_TECHNIQUE_H
