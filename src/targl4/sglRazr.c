/*
 *             :::::::::::     :::     :::::::::   ::::::::      :::
 *                 :+:       :+: :+:   :+:    :+: :+:    :+:   :+: :+:
 *                 +:+      +:+   +:+  +:+    +:+ +:+         +:+   +:+
 *                 +#+     +#++:++#++: +#++:++#:  :#:        +#++:++#++:
 *                 +#+     +#+     +#+ +#+    +#+ +#+   +#+# +#+     +#+
 *                 #+#     #+#     #+# #+#    #+# #+#    #+# #+#     #+#
 *                 ###     ###     ### ###    ###  ########  ###     ###
 *
 *                  Q W A D R O   E X E C U T I O N   E C O S Y S T E M
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

#include "qwadro/io/afxXml.h"
#include "sgl.h"

#include "qwadro/draw/pipe/avxPipeline.h"

#include "qwadro/draw/afxDrawSystem.h"
#include "qwadro/io/afxUri.h"
#include "qwadro/exec/afxSystem.h"
#include "qwadro/draw/io/afxXsh.h"
// OpenGL/Vulkan Continuous Integration

_SGL afxError _DpuBindAndSyncRazr(sglDpu* dpu, avxRasterizer razr)
{
    afxError err = AFX_ERR_NONE;
    glVmt const* gl = &dpu->gl;
    AfxAssert(razr);

    if (!razr)
    {

    }
    else
    {
        AfxAssertObjects(1, &razr, afxFcc_RAZR);
        sglUpdateFlags devUpdReq = (razr->updFlags & SGL_UPD_FLAG_DEVICE);

        if ((devUpdReq & SGL_UPD_FLAG_DEVICE_INST))
        {

#if 0
            avxShader fsh;
            AfxGetPipelineStage(pip, avxShaderStage_FRAGMENT, &fsh);
            afxNat outCnt = AfxCountColorOutputChannels(pip->base.razr);
            //vtxShd->base.ioDecls.
            for (afxNat i = 0; i < outCnt; i++)
            {
                avxColorOutputChannel out;
                AfxGetColorOutputChannels(pip->base.razr, i, 1, &out);
            }
#endif

            razr->updFlags &= ~(SGL_UPD_FLAG_DEVICE);
        }
    }
    return err;
}

_SGL afxError _SglRazrDtor(avxRasterizer razr)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &razr, afxFcc_RAZR);

    if (_AvxRazrStdImplementation.dtor(razr))
        AfxThrowError();

    return err;
}

_SGL afxError _SglRazrCtor(avxRasterizer razr, afxCookie const* cookie)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &razr, afxFcc_RAZR);

    if (_AvxRazrStdImplementation.ctor(razr, cookie)) AfxThrowError();
    else
    {
        razr->updFlags = SGL_UPD_FLAG_DEVICE_INST;

        if (err && _AvxRazrStdImplementation.dtor(razr))
            AfxThrowError();
    }
    AfxAssertObjects(1, &razr, afxFcc_RAZR);
    return err;
}
