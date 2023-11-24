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
 *                   (c) 2017 SIGMA Technology Group — Federação SIGMA
 *                                    www.sigmaco.org
 */

#include "afx/core/afxXml.h"
#include "sgl.h"

#include "afx/draw/afxPipeline.h"
#include "afx/draw/afxPipeline.h"

#include "afx/draw/afxDrawSystem.h"
#include "afx/core/afxUri.h"
#include "afx/core/afxSystem.h"
// OpenGL/Vulkan Continuous Integration

#define _AFX_DBG_IGNORE_BLEND
#define _AFX_DBG_IGNORE_COLOR_MASK
#define _AFX_DBG_IGNORE_LOGICAL_OP
#define _AFX_DBG_IGNORE_MULTISAMPLING
#define _AFX_DBG_IGNORE_STENCIL
#define _AFX_DBG_IGNORE_DEPTH_BIAS

_SGL afxError _SglDpuBindAndSyncRasterizer(sglDpuIdd* dpu, afxRasterizer rast)
{
    afxError err = AFX_ERR_NONE;
    glVmt const* gl = &dpu->gl;
    afxBool rasterizationDisabled = !rast;

    AfxAssert(rast);

    afxRasterizationConfig config;
    AfxDescribeRasterizerConfiguration(rast, &config);

    return err;
}

_SGL afxError _SglPiprDtor(afxRasterizer rast)
{
    AfxEntry("rast=%p", rast);
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &rast, afxFcc_PIPR);

    afxDrawContext dctx = AfxGetObjectProvider(rast);
    afxContext mem = AfxGetDrawContextMemory(dctx);
    AfxAssertObjects(1, &mem, afxFcc_CTX);

    if (rast->base.sampleMasks)
        AfxDeallocate(mem, rast->base.sampleMasks);

    if (rast->base.outs)
        AfxDeallocate(mem, rast->base.outs);

    return err;
}

_SGL afxError _SglPiprCtor(afxRasterizer rast, afxCookie const* cookie)
{
    AfxEntry("rast=%p", rast);
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &rast, afxFcc_PIPR);

    afxDrawContext dctx = cookie->udd[0];
    afxRasterizationConfig const *rasc = ((afxRasterizationConfig const*)cookie->udd[1]) + cookie->no;
    //AfxAssertType(rasc, afxFcc_RASB);

    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    afxContext mem = AfxGetDrawContextMemory(dctx);
    AfxAssertObjects(1, &mem, afxFcc_CTX);

    afxRasterizationFlags rasFlags = rasc->rasFlags;
    rast->base.rasFlags = NIL;

    rast->base.fillMode = rasc->fillMode;
    rast->base.lineWidth = rasc->lineWidth;

    rast->base.depthBiasEnabled = !!rasc->depthBiasEnabled;
    rast->base.depthBiasSlopeScale = rasc->depthBiasSlopeScale;
    rast->base.depthBiasConstFactor = rasc->depthBiasConstFactor;
    rast->base.depthBiasClamp = rasc->depthBiasClamp;

    afxMultisamplingFlags msFlags = rasc->msFlags;
    rast->base.msFlags = NIL;

    rast->base.msEnabled = !!rasc->msEnabled;
    rast->base.minSampleShadingValue = rasc->minSampleShadingValue;

    rast->base.sampleCnt = rasc->sampleCnt;
    rast->base.sampleMasks = NIL;

    if (rast->base.sampleCnt && !(rast->base.sampleMasks = AfxAllocate(mem, sizeof(rast->base.sampleMasks[0]), rast->base.sampleCnt, 0, AfxHint()))) AfxThrowError();
    else
    {
        for (afxNat i = 0; i < rast->base.sampleCnt; i++)
            rast->base.sampleMasks[i] = rasc->sampleMasks[i];
    }

    if (!err)
    {
        afxDepthStencilFlags dsFlags = rasc->dsFlags;
        rast->base.dsFlags = NIL;

        rast->base.depthTestEnabled = !!rasc->depthTestEnabled;
        rast->base.depthCompareOp = rasc->depthCompareOp;
        rast->base.depthWriteEnabled = !!rasc->depthWriteEnabled;

        rast->base.stencilTestEnabled = !!rasc->stencilTestEnabled;
        rast->base.stencilFront = rasc->stencilFront;
        rast->base.stencilBack = rasc->stencilBack;
        
        rast->base.depthBoundsTestEnabled = !!rasc->depthBoundsTestEnabled;
        AfxCopyV2d(rast->base.depthBounds, rasc->depthBounds);

        rast->base.dsFmt = rasc->dsFmt; // ?

        afxColorOutputFlags pixelFlags = rasc->pixelFlags;
        rast->base.pixelFlags = NIL;

        rast->base.outCnt = rasc->colorOutCnt;
        rast->base.outs = NIL;

        if (rast->base.outCnt && !(rast->base.outs = AfxAllocate(mem, sizeof(rast->base.outs[0]), rast->base.outCnt, 0, AfxHint()))) AfxThrowError();
        else
        {
            for (afxNat i = 0; i < rast->base.outCnt; i++)
                rast->base.outs[i] = rasc->colorOuts[i];

            // deveria ser só o blend/write, já que só podemos determinar as saídas quando assembleado com fragment shaders enquanto pipeline completo.
        }

        if (!err)
        {
            AfxCopyV4d(rast->base.blendConstants, rasc->blendConstants);

            rast->base.logicOpEnabled = !!rasc->pixelLogicOpEnabled;
            rast->base.logicOp = rasc->pixelLogicOp;

            rast->updFlags = SGL_UPD_FLAG_DEVICE_INST;

            if (err)
            {
                if (rast->base.outs)
                    AfxDeallocate(mem, rast->base.outs);
            }
        }

        if (err)
        {
            if (rast->base.sampleMasks)
                AfxDeallocate(mem, rast->base.sampleMasks);
        }
    }
    AfxAssertObjects(1, &rast, afxFcc_PIPR);
    return err;
}

_SGL afxClassConfig _SglPiprClsConfig =
{
    .fcc = afxFcc_PIPR,
    .name = "Rasterizer",
    .unitsPerPage = 4,
    .size = sizeof(AFX_OBJECT(afxRasterizer)),
    .ctx = NIL,
    .ctor = (void*)_SglPiprCtor,
    .dtor = (void*)_SglPiprDtor
};
