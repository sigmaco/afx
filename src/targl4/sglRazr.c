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
 *                       (c) 2017 SIGMA, Engitech, Scitech, Serpro
 *                             <https://sigmaco.org/qwadro/>
 */

#include "qwadro/io/afxXml.h"
#include "sgl.h"

#include "qwadro/draw/pipe/afxPipeline.h"

#include "qwadro/draw/afxDrawSystem.h"
#include "qwadro/io/afxUri.h"
#include "qwadro/core/afxSystem.h"
#include "qwadro/draw/io/afxXsh.h"
// OpenGL/Vulkan Continuous Integration

_SGL afxError _DpuBindAndSyncRazr(sglDpu* dpu, afxRasterizer razr)
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
            afxShader fsh;
            AfxFindLinkedShader(pip, afxShaderStage_FRAGMENT, &fsh);
            afxNat outCnt = AfxCountColorOutputChannels(pip->base.razr);
            //vtxShd->base.ioDecls.
            for (afxNat i = 0; i < outCnt; i++)
            {
                afxColorOutputChannel out;
                AfxGetColorOutputChannels(pip->base.razr, i, 1, &out);
            }
#endif

            razr->updFlags &= ~(SGL_UPD_FLAG_DEVICE);
        }
    }
    return err;
}

_SGL afxError _SglRazrDtor(afxRasterizer razr)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &razr, afxFcc_RAZR);

    if (_AvxRazrStdImplementation.dtor(razr))
        AfxThrowError();

    return err;
}

_SGL afxError _SglRazrCtor(afxRasterizer razr, afxCookie const* cookie)
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

_SGL afxClassConfig const _SglRazrMgrCfg =
{
    .fcc = afxFcc_RAZR,
    .name = "Rasterizer",
    .unitsPerPage = 2,
    .size = sizeof(AFX_OBJECT(afxRasterizer)),
    .mmu = NIL,
    .ctor = (void*)_SglRazrCtor,
    .dtor = (void*)_SglRazrDtor
};
