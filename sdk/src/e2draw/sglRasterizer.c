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

_SGL afxError _SglDpuBindAndSyncRasterizer(sglDpuIdd* dpu, afxRasterizer ras)
{
    afxError err = AFX_ERR_NONE;
    glVmt const* gl = &dpu->gl;
    afxBool rasterizationDisabled = !ras;

    AfxAssert(ras);

    afxRasterizationConfig config;
    AfxDescribeRasterizerConfiguration(ras, &config);

    return err;
}

_SGL afxError _SglRasDtor(afxRasterizer ras)
{
    AfxEntry("ras=%p", ras);
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);

    afxDrawContext dctx = AfxGetObjectProvider(ras);
    afxContext mem = AfxGetDrawContextMemory(dctx);
    AfxAssertObjects(1, &mem, afxFcc_CTX);

    if (ras->base.sampleMasks)
        AfxDeallocate(mem, ras->base.sampleMasks);

    if (ras->base.outs)
        AfxDeallocate(mem, ras->base.outs);

    return err;
}

_SGL afxError _SglRasCtor(afxRasterizer ras, afxCookie const* cookie)
{
    AfxEntry("ras=%p", ras);
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);

    afxDrawContext dctx = cookie->udd[0];
    afxRasterizationConfig const *rasc = ((afxRasterizationConfig const*)cookie->udd[1]) + cookie->no;
    //AfxAssertType(rasc, afxFcc_RASB);

    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    afxContext mem = AfxGetDrawContextMemory(dctx);
    AfxAssertObjects(1, &mem, afxFcc_CTX);

    afxRasterizationFlags rasFlags = rasc->rasFlags;
    ras->base.rasFlags = NIL;

    ras->base.fillMode = rasc->fillMode;
    ras->base.lineWidth = rasc->lineWidth;

    ras->base.depthBiasEnabled = !!rasc->depthBiasEnabled;
    ras->base.depthBiasSlopeScale = rasc->depthBiasSlopeScale;
    ras->base.depthBiasConstFactor = rasc->depthBiasConstFactor;
    ras->base.depthBiasClamp = rasc->depthBiasClamp;

    afxMultisamplingFlags msFlags = rasc->msFlags;
    ras->base.msFlags = NIL;

    ras->base.msEnabled = !!rasc->msEnabled;
    ras->base.minSampleShadingValue = rasc->minSampleShadingValue;

    ras->base.sampleCnt = rasc->sampleCnt;
    ras->base.sampleMasks = NIL;

    if (ras->base.sampleCnt && !(ras->base.sampleMasks = AfxAllocate(mem, ras->base.sampleCnt * sizeof(ras->base.sampleMasks[0]), 0, AfxSpawnHint()))) AfxThrowError();
    else
    {
        for (afxNat i = 0; i < ras->base.sampleCnt; i++)
            ras->base.sampleMasks[i] = rasc->sampleMasks[i];
    }

    if (!err)
    {
        afxDepthStencilFlags dsFlags = rasc->dsFlags;
        ras->base.dsFlags = NIL;

        ras->base.depthTestEnabled = !!rasc->depthTestEnabled;
        ras->base.depthCompareOp = rasc->depthCompareOp;
        ras->base.depthWriteEnabled = !!rasc->depthWriteEnabled;

        ras->base.stencilTestEnabled = !!rasc->stencilTestEnabled;
        ras->base.stencilFront = rasc->stencilFront;
        ras->base.stencilBack = rasc->stencilBack;
        
        ras->base.depthBoundsTestEnabled = !!rasc->depthBoundsTestEnabled;
        AfxCopyV2d(ras->base.depthBounds, rasc->depthBounds);

        ras->base.dsFmt = rasc->dsFmt; // ?

        afxColorOutputFlags pixelFlags = rasc->pixelFlags;
        ras->base.pixelFlags = NIL;

        ras->base.outCnt = rasc->colorOutCnt;
        ras->base.outs = NIL;

        if (ras->base.outCnt && !(ras->base.outs = AfxAllocate(mem, ras->base.outCnt * sizeof(ras->base.outs[0]), 0, AfxSpawnHint()))) AfxThrowError();
        else
        {
            for (afxNat i = 0; i < ras->base.outCnt; i++)
                ras->base.outs[i] = rasc->colorOuts[i];

            // deveria ser só o blend/write, já que só podemos determinar as saídas quando assembleado com fragment shaders enquanto pipeline completo.
        }

        if (!err)
        {
            AfxCopyV4d(ras->base.blendConstants, rasc->blendConstants);

            ras->base.logicOpEnabled = !!rasc->pixelLogicOpEnabled;
            ras->base.logicOp = rasc->pixelLogicOp;

            ras->updFlags = SGL_UPD_FLAG_DEVICE_INST;

            if (err)
            {
                if (ras->base.outs)
                    AfxDeallocate(mem, ras->base.outs);
            }
        }

        if (err)
        {
            if (ras->base.sampleMasks)
                AfxDeallocate(mem, ras->base.sampleMasks);
        }
    }
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    return err;
}

_SGL afxClassConfig _SglRasClsConfig =
{
    .fcc = afxFcc_RAS,
    .name = "Rasterizer",
    .unitsPerPage = 4,
    .size = sizeof(AFX_OBJECT(afxRasterizer)),
    .ctx = NIL,
    .ctor = (void*)_SglRasCtor,
    .dtor = (void*)_SglRasDtor
};
