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

#include "sgl.h"

#include "qwadro/draw/pipe/afxSampler.h"
#include "qwadro/draw/afxDrawSystem.h"

////////////////////////////////////////////////////////////////////////////////
// SAMPLER                                                                    //
////////////////////////////////////////////////////////////////////////////////

_SGL afxError _DpuBindAndSyncSamp(sglDpu* dpu, sglBindFlags bindFlags, afxNat glUnit, afxSampler samp)
{
    //AfxEntry("smp=%p", smp);
    afxError err = AFX_ERR_NONE;
    glVmt const* gl = &dpu->gl;

    if (samp)
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

            GLenum magF = SglToGlTexelFilterMode(samp->base.magFilter);
            GLenum minF = SglToGlTexelFilterModeMipmapped(samp->base.minFilter, samp->base.mipmapFilter);
            GLint wrapS = SglToGlTexWrapMode(samp->base.uvw[0]);
            GLint wrapT = SglToGlTexWrapMode(samp->base.uvw[1]);
            GLint wrapR = SglToGlTexWrapMode(samp->base.uvw[2]);
            GLint cop = SglToGlCompareOp(samp->base.compareOp);

            gl->SamplerParameteri(glHandle, GL_TEXTURE_MAG_FILTER, magF); _SglThrowErrorOccuried();
            gl->SamplerParameteri(glHandle, GL_TEXTURE_MIN_FILTER, minF); _SglThrowErrorOccuried();

            gl->SamplerParameteri(glHandle, GL_TEXTURE_WRAP_S, wrapS); _SglThrowErrorOccuried();
            gl->SamplerParameteri(glHandle, GL_TEXTURE_WRAP_T, wrapT); _SglThrowErrorOccuried();
            gl->SamplerParameteri(glHandle, GL_TEXTURE_WRAP_R, wrapR); _SglThrowErrorOccuried();

            if (samp->base.anisotropyEnabled)
            {
                gl->SamplerParameterf(glHandle, GL_TEXTURE_MAX_ANISOTROPY, samp->base.anisotropyMaxDegree); _SglThrowErrorOccuried();
            }
            else
            {
                //gl->SamplerParameterf(smp->glHandle, GL_TEXTURE_MAX_ANISOTROPY, 0); _SglThrowErrorOccuried();
            }

            gl->SamplerParameterf(glHandle, GL_TEXTURE_LOD_BIAS, samp->base.lodBias); _SglThrowErrorOccuried();
            gl->SamplerParameterf(glHandle, GL_TEXTURE_MIN_LOD, samp->base.minLod); _SglThrowErrorOccuried();
            gl->SamplerParameterf(glHandle, GL_TEXTURE_MAX_LOD, samp->base.maxLod); _SglThrowErrorOccuried();

            if (samp->base.compareEnabled)
            {
                // what about GL_TEXTURE_COMPARE_MODE?
            }

            gl->SamplerParameteri(glHandle, GL_TEXTURE_COMPARE_MODE, GL_NONE); _SglThrowErrorOccuried();
            gl->SamplerParameteri(glHandle, GL_TEXTURE_COMPARE_FUNC, cop); _SglThrowErrorOccuried();
            gl->SamplerParameterfv(glHandle, GL_TEXTURE_BORDER_COLOR, (void*)samp->base.borderColor); _SglThrowErrorOccuried();

            AfxLogEcho("afxSampler %p hardware-side data instanced.", samp);

            if (bound && !(bindFlags & sglBindFlag_KEEP))
            {
                gl->BindSampler(glUnit, 0); _SglThrowErrorOccuried();
                //bound = FALSE;
            }
        }
        samp->updFlags &= ~(SGL_UPD_FLAG_DEVICE);
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
