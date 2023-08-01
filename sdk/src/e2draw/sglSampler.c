/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                      S I G M A   T E C H N O L O G Y   G R O U P
 *
 *                                   Public Test Build
 *                               (c) 2017 Federação SIGMA
 *                                    www.sigmaco.org
 */

#define _AFX_SAMPLER_C
#define _AFX_DRAW_CONTEXT_C
#define _AFX_DRAW_QUEUE_C
#include "sgl.h"

#include "afx/draw/res/afxSampler.h"
#include "afx/draw/afxDrawSystem.h"

////////////////////////////////////////////////////////////////////////////////
// SAMPLER                                                                    //
////////////////////////////////////////////////////////////////////////////////

_SGL afxError _SglDqueBindAndSyncSmp(afxDrawQueue dque, afxNat unit, afxSampler smp, sglVmt const* gl)
{
    //AfxEntry("smp=%p", smp);
    afxError err = AFX_ERR_NONE;
    sglDqueIdd *dqueIdd=dque->idd;

    if (smp)
    {
        sglSampIdd *idd = smp->idd;
        AfxAssertObject(smp, AFX_FCC_SAMP);

        if ((idd->updFlags & SGL_UPD_FLAG_DEVICE))
        {
            GLenum magF = SglToGlTexelFilterMode(smp->magFilter);
            GLenum minF = SglToGlTexelFilterModeMipmapped(smp->minFilter, smp->mipmapFilter);
            GLint wrapS = SglToGlTexWrapMode(smp->uvw[0]);
            GLint wrapT = SglToGlTexWrapMode(smp->uvw[1]);
            GLint wrapR = SglToGlTexWrapMode(smp->uvw[2]);
            GLint cop = SglToGlCompareOp(smp->compareOp);

            if ((idd->updFlags & SGL_UPD_FLAG_DEVICE_INST))
            {
                if (idd->glHandle)
                {
                    gl->DeleteSamplers(1, &(idd->glHandle)); _SglThrowErrorOccuried();
                    idd->glHandle = NIL;
                }

                AfxAssert(NIL == idd->glHandle);
                gl->GenSamplers(1, &(idd->glHandle)); _SglThrowErrorOccuried();
                gl->BindSampler(unit, idd->glHandle); _SglThrowErrorOccuried();
                AfxAssert(gl->IsSampler(idd->glHandle));

                gl->SamplerParameteri(idd->glHandle, GL_TEXTURE_MAG_FILTER, magF); _SglThrowErrorOccuried();
                gl->SamplerParameteri(idd->glHandle, GL_TEXTURE_MIN_FILTER, minF); _SglThrowErrorOccuried();

                gl->SamplerParameteri(idd->glHandle, GL_TEXTURE_WRAP_S, wrapS); _SglThrowErrorOccuried();
                gl->SamplerParameteri(idd->glHandle, GL_TEXTURE_WRAP_T, wrapT); _SglThrowErrorOccuried();
                gl->SamplerParameteri(idd->glHandle, GL_TEXTURE_WRAP_R, wrapR); _SglThrowErrorOccuried();

                if (smp->anisotropyEnabled)
                {
                    gl->SamplerParameterf(idd->glHandle, GL_TEXTURE_MAX_ANISOTROPY, smp->anisotropyMaxDegree); _SglThrowErrorOccuried();
                }
                else
                {
                    //gl->SamplerParameterf(smp->glHandle, GL_TEXTURE_MAX_ANISOTROPY, 0); _SglThrowErrorOccuried();
                }

                gl->SamplerParameterf(idd->glHandle, GL_TEXTURE_LOD_BIAS, smp->lodBias); _SglThrowErrorOccuried();
                gl->SamplerParameterf(idd->glHandle, GL_TEXTURE_MIN_LOD, smp->minLod); _SglThrowErrorOccuried();
                gl->SamplerParameterf(idd->glHandle, GL_TEXTURE_MAX_LOD, smp->maxLod); _SglThrowErrorOccuried();

                if (smp->compareEnabled)
                {
                    // what about GL_TEXTURE_COMPARE_MODE?
                }
                gl->SamplerParameteri(idd->glHandle, GL_TEXTURE_COMPARE_MODE, GL_NONE); _SglThrowErrorOccuried();
                gl->SamplerParameteri(idd->glHandle, GL_TEXTURE_COMPARE_FUNC, cop); _SglThrowErrorOccuried();
                gl->SamplerParameterfv(idd->glHandle, GL_TEXTURE_BORDER_COLOR, (void*)smp->borderColor);

                idd->updFlags &= ~(SGL_UPD_FLAG_DEVICE);
                AfxEcho("afxSampler %p hardware-side data instanced.", smp);
            }
            else if ((idd->updFlags & SGL_UPD_FLAG_DEVICE_FLUSH))
            {
                AfxAssert(idd->glHandle);
                gl->BindSampler(unit, idd->glHandle); _SglThrowErrorOccuried();
                AfxThrowError(); // shoudn't be mutable
            }
        }
        else
        {
            AfxAssert(idd->glHandle);
            gl->BindSampler(unit, idd->glHandle); _SglThrowErrorOccuried();
        }
    }
    else
    {
        gl->BindSampler(unit, 0); _SglThrowErrorOccuried();
    }

    return err;
}

_SGL afxError _AfxSmpDtor(afxSampler smp)
{
    afxError err = AFX_ERR_NONE;
    AfxEntry("smp=%p", smp);
    AfxAssertObject(smp, AFX_FCC_SAMP);

    sglSampIdd *idd = smp->idd;

    if (idd && idd->glHandle)
    {
        afxDrawContext dctx = AfxGetSamplerContext(smp);
        _SglDeleteGlRes(dctx, 4, idd->glHandle);
        idd->glHandle = 0;

        AfxDeallocate(AfxGetDrawContextMemory(dctx), idd);
        smp->idd = NIL;
    }

    return err;
}

_SGL _afxSampVmt _SglSampVmt =
{
    _AfxSmpDtor
};

_SGL afxError _AfxSmpCtor(afxSampler smp)
{
    afxError err = AFX_ERR_NONE;
    AfxEntry("smp=%p", smp);
    AfxAssertObject(smp, AFX_FCC_SAMP);
    afxDrawContext dctx = AfxGetSamplerContext(smp);

    smp->vmt = &_SglSampVmt;
    sglSampIdd *idd = AfxAllocate(AfxGetDrawContextMemory(dctx), sizeof(*idd), 0, AfxSpawnHint());
    smp->idd = idd;
    idd->glHandle = 0;
    idd->updFlags = SGL_UPD_FLAG_DEVICE_INST;
    return err;
}
