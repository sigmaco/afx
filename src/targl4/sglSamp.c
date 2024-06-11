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

#include "sgl.h"

#include "qwadro/draw/pipe/afxSampler.h"
#include "qwadro/draw/afxDrawSystem.h"

////////////////////////////////////////////////////////////////////////////////
// SAMPLER                                                                    //
////////////////////////////////////////////////////////////////////////////////

_SGL afxError _DpuBindAndSyncSamp(sglDpu* dpu, afxNat glUnit, afxSampler samp)
{
    //AfxEntry("smp=%p", smp);
    afxError err = AFX_ERR_NONE;
    glVmt const* gl = &dpu->gl;

    if (!samp)
    {
        gl->BindSampler(glUnit, 0); _SglThrowErrorOccuried();
    }
    else
    {
        AfxAssertObjects(1, &samp, afxFcc_SAMP);
        sglUpdateFlags devUpdReq = (samp->updFlags & SGL_UPD_FLAG_DEVICE);
        GLuint glHandle = samp->glHandle;
        afxBool bound = FALSE;

        if ((!glHandle) || (devUpdReq & SGL_UPD_FLAG_DEVICE_INST))
        {
            if (glHandle)
            {
                gl->DeleteBuffers(1, &(glHandle)); _SglThrowErrorOccuried();
                samp->glHandle = NIL;
                glHandle = NIL;
            }

            gl->GenSamplers(1, &(glHandle)); _SglThrowErrorOccuried();
            gl->BindSampler(glUnit, glHandle); _SglThrowErrorOccuried();
            AfxAssert(gl->IsSampler(glHandle));
            samp->glHandle = glHandle;
            bound = TRUE;

            GLenum magF = SglToGlTexelFilterMode(samp->base.cfg.base.magFilter);
            GLenum minF = SglToGlTexelFilterModeMipmapped(samp->base.cfg.base.minFilter, samp->base.cfg.base.mipFilter);
            GLint wrapS = SglToGlTexWrapMode(samp->base.cfg.base.uvw[0]);
            GLint wrapT = SglToGlTexWrapMode(samp->base.cfg.base.uvw[1]);
            GLint wrapR = SglToGlTexWrapMode(samp->base.cfg.base.uvw[2]);
            GLint cop = SglToGlCompareOp(samp->base.cfg.base.compareOp);

            gl->SamplerParameteri(glHandle, GL_TEXTURE_MAG_FILTER, magF); _SglThrowErrorOccuried();
            gl->SamplerParameteri(glHandle, GL_TEXTURE_MIN_FILTER, minF); _SglThrowErrorOccuried();

            gl->SamplerParameteri(glHandle, GL_TEXTURE_WRAP_S, wrapS); _SglThrowErrorOccuried();
            gl->SamplerParameteri(glHandle, GL_TEXTURE_WRAP_T, wrapT); _SglThrowErrorOccuried();
            gl->SamplerParameteri(glHandle, GL_TEXTURE_WRAP_R, wrapR); _SglThrowErrorOccuried();

            if (samp->base.cfg.base.anisotropyEnabled)
            {
                gl->SamplerParameterf(glHandle, GL_TEXTURE_MAX_ANISOTROPY, samp->base.cfg.base.anisotropyMaxDegree); _SglThrowErrorOccuried();
            }
            else
            {
                //gl->SamplerParameterf(smp->glHandle, GL_TEXTURE_MAX_ANISOTROPY, 0); _SglThrowErrorOccuried();
            }

            gl->SamplerParameterf(glHandle, GL_TEXTURE_LOD_BIAS, samp->base.cfg.base.lodBias); _SglThrowErrorOccuried();
            gl->SamplerParameterf(glHandle, GL_TEXTURE_MIN_LOD, samp->base.cfg.base.minLod); _SglThrowErrorOccuried();
            gl->SamplerParameterf(glHandle, GL_TEXTURE_MAX_LOD, samp->base.cfg.base.maxLod); _SglThrowErrorOccuried();

            if (samp->base.cfg.base.compareEnabled)
            {
                // what about GL_TEXTURE_COMPARE_MODE?
            }

            gl->SamplerParameteri(glHandle, GL_TEXTURE_COMPARE_MODE, GL_NONE); _SglThrowErrorOccuried();
            gl->SamplerParameteri(glHandle, GL_TEXTURE_COMPARE_FUNC, cop); _SglThrowErrorOccuried();
            gl->SamplerParameterfv(glHandle, GL_TEXTURE_BORDER_COLOR, (void*)samp->base.cfg.base.borderColor); _SglThrowErrorOccuried();

            AfxLogEcho("Hardware-side sampler %p ready.", samp);
            samp->updFlags &= ~(SGL_UPD_FLAG_DEVICE);
        }
        else
        {
            gl->BindSampler(glUnit, glHandle); _SglThrowErrorOccuried();
        }
    }
    return err;
}

_SGL afxError _SglSampDtor(afxSampler samp)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &samp, afxFcc_SAMP);

    if (samp->glHandle)
    {
        afxDrawContext dctx = AfxGetObjectProvider(samp);
        _SglDctxDeleteGlRes(dctx, 4, (void*)samp->glHandle);
        samp->glHandle = 0;
    }

    if (_AvxSampStdImplementation.dtor(samp))
        AfxThrowError();

    return err;
}

_SGL afxError _SglSampCtor(afxSampler samp, afxCookie const* cookie)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &samp, afxFcc_SAMP);

    if (_AvxSampStdImplementation.ctor(samp, cookie)) AfxThrowError();
    else
    {
        samp->glHandle = 0;
        samp->updFlags = SGL_UPD_FLAG_DEVICE_INST;

        if (err && _AvxSampStdImplementation.dtor(samp))
            AfxThrowError();
    }
    return err;
}

_SGL afxClassConfig const _SglSampMgrCfg =
{
    .fcc = afxFcc_SAMP,
    .name = "Sampler",
    .unitsPerPage = 2,
    .size = sizeof(AFX_OBJECT(afxSampler)),
    .mmu = NIL,
    .ctor = (void*)_SglSampCtor,
    .dtor = (void*)_SglSampDtor
};
