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

#include "sgl.h"

#include "afx/draw/afxSampler.h"
#include "afx/draw/afxDrawSystem.h"

////////////////////////////////////////////////////////////////////////////////
// SAMPLER                                                                    //
////////////////////////////////////////////////////////////////////////////////

_SGL afxError _SglDpuBindAndSyncSamp(sglDpuIdd* dpu, afxNat unit, afxSampler samp, glVmt const* gl)
{
    //AfxEntry("smp=%p", smp);
    afxError err = AFX_ERR_NONE;
    (void)dpu;

    if (samp)
    {
        AfxAssertObjects(1, &samp, afxFcc_SAMP);

        if ((samp->updFlags & SGL_UPD_FLAG_DEVICE))
        {
            GLenum magF = SglToGlTexelFilterMode(samp->base.magFilter);
            GLenum minF = SglToGlTexelFilterModeMipmapped(samp->base.minFilter, samp->base.mipmapFilter);
            GLint wrapS = SglToGlTexWrapMode(samp->base.uvw[0]);
            GLint wrapT = SglToGlTexWrapMode(samp->base.uvw[1]);
            GLint wrapR = SglToGlTexWrapMode(samp->base.uvw[2]);
            GLint cop = SglToGlCompareOp(samp->base.compareOp);

            if ((samp->updFlags & SGL_UPD_FLAG_DEVICE_INST))
            {
                if (samp->glHandle)
                {
                    gl->DeleteSamplers(1, &(samp->glHandle)); _SglThrowErrorOccuried();
                    samp->glHandle = NIL;
                }

                AfxAssert(NIL == samp->glHandle);
                gl->GenSamplers(1, &(samp->glHandle)); _SglThrowErrorOccuried();
                gl->BindSampler(unit, samp->glHandle); _SglThrowErrorOccuried();
                AfxAssert(gl->IsSampler(samp->glHandle));

                gl->SamplerParameteri(samp->glHandle, GL_TEXTURE_MAG_FILTER, magF); _SglThrowErrorOccuried();
                gl->SamplerParameteri(samp->glHandle, GL_TEXTURE_MIN_FILTER, minF); _SglThrowErrorOccuried();

                gl->SamplerParameteri(samp->glHandle, GL_TEXTURE_WRAP_S, wrapS); _SglThrowErrorOccuried();
                gl->SamplerParameteri(samp->glHandle, GL_TEXTURE_WRAP_T, wrapT); _SglThrowErrorOccuried();
                gl->SamplerParameteri(samp->glHandle, GL_TEXTURE_WRAP_R, wrapR); _SglThrowErrorOccuried();

                if (samp->base.anisotropyEnabled)
                {
                    gl->SamplerParameterf(samp->glHandle, GL_TEXTURE_MAX_ANISOTROPY, samp->base.anisotropyMaxDegree); _SglThrowErrorOccuried();
                }
                else
                {
                    //gl->SamplerParameterf(smp->glHandle, GL_TEXTURE_MAX_ANISOTROPY, 0); _SglThrowErrorOccuried();
                }

                gl->SamplerParameterf(samp->glHandle, GL_TEXTURE_LOD_BIAS, samp->base.lodBias); _SglThrowErrorOccuried();
                gl->SamplerParameterf(samp->glHandle, GL_TEXTURE_MIN_LOD, samp->base.minLod); _SglThrowErrorOccuried();
                gl->SamplerParameterf(samp->glHandle, GL_TEXTURE_MAX_LOD, samp->base.maxLod); _SglThrowErrorOccuried();

                if (samp->base.compareEnabled)
                {
                    // what about GL_TEXTURE_COMPARE_MODE?
                }
                gl->SamplerParameteri(samp->glHandle, GL_TEXTURE_COMPARE_MODE, GL_NONE); _SglThrowErrorOccuried();
                gl->SamplerParameteri(samp->glHandle, GL_TEXTURE_COMPARE_FUNC, cop); _SglThrowErrorOccuried();
                gl->SamplerParameterfv(samp->glHandle, GL_TEXTURE_BORDER_COLOR, (void*)samp->base.borderColor);

                samp->updFlags &= ~(SGL_UPD_FLAG_DEVICE);
                AfxEcho("afxSampler %p hardware-side data instanced.", samp);
            }
            else if ((samp->updFlags & SGL_UPD_FLAG_DEVICE_FLUSH))
            {
                AfxAssert(samp->glHandle);
                gl->BindSampler(unit, samp->glHandle); _SglThrowErrorOccuried();
                AfxThrowError(); // shoudn't be mutable
            }
        }
        else
        {
            AfxAssert(samp->glHandle);
            gl->BindSampler(unit, samp->glHandle); _SglThrowErrorOccuried();
        }
    }
    else
    {
        gl->BindSampler(unit, 0); _SglThrowErrorOccuried();
    }

    return err;
}

_SGL afxError _SglSampDtor(afxSampler samp)
{
    afxError err = AFX_ERR_NONE;
    AfxEntry("samp=%p", samp);
    AfxAssertObjects(1, &samp, afxFcc_SAMP);

    if (samp->glHandle)
    {
        afxDrawContext dctx = AfxGetObjectProvider(samp);
        _SglDctxDeleteGlRes(dctx, 4, samp->glHandle);
        samp->glHandle = 0;
    }

    return err;
}

_SGL afxError _SglSampCtor(afxSampler samp, afxCookie const* cookie)
{
    afxError err = AFX_ERR_NONE;
    AfxEntry("samp=%p", samp);
    AfxAssertObjects(1, &samp, afxFcc_SAMP);

    afxSamplerConfig const *config = ((afxSamplerConfig const *)cookie->udd[0]) + cookie->no;

    AfxAssert(config);
    AfxAssert(config[0].magFilter < afxTexelFilter_TOTAL);
    AfxAssert(config[0].minFilter < afxTexelFilter_TOTAL);
    AfxAssert(config[0].mipmapFilter < afxTexelFilter_TOTAL);
    AfxAssert(config[0].uvw[0] < afxTexelAddress_TOTAL);
    AfxAssert(config[0].uvw[1] < afxTexelAddress_TOTAL);
    AfxAssert(config[0].uvw[2] < afxTexelAddress_TOTAL);

    samp->base.crc32 = 0;
    AfxCrc32(&samp->base.crc32, config, sizeof(*config));

    samp->base.magFilter = config->magFilter;
    samp->base.minFilter = config->minFilter;
    samp->base.mipmapFilter = config->mipmapFilter;
    samp->base.uvw[0] = config->uvw[0];
    samp->base.uvw[1] = config->uvw[1];
    samp->base.uvw[2] = config->uvw[2];
    samp->base.anisotropyEnabled = config->anisotropyEnabled;
    samp->base.anisotropyMaxDegree = config->anisotropyMaxDegree;
    samp->base.compareEnabled = config->compareEnabled;
    samp->base.compareOp = config->compareOp;
    AfxCopyColor(samp->base.borderColor, config->borderColor);
    samp->base.unnormalizedCoords = config->unnormalizedCoords;
    samp->base.lodBias = config->lodBias;
    samp->base.minLod = config->minLod;
    samp->base.maxLod = config->maxLod;

    samp->glHandle = 0;
    samp->updFlags = SGL_UPD_FLAG_DEVICE_INST;

    return err;
}

_SGL afxClassConfig _SglSampClsConfig =
{
    .fcc = afxFcc_SAMP,
    .name = "Sampler",
    .unitsPerPage = 1,
    .size = sizeof(AFX_OBJECT(afxSampler)),
    .ctx = NIL,
    .ctor = (void*)_SglSampCtor,
    .dtor = (void*)_SglSampDtor
};
