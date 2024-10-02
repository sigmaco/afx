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

#define _AVX_DRAW_C
#define _AVX_SAMPLER_C
#include "../../dev/AvxImplKit.h"

_AVX void AfxDescribeSampler(avxSampler samp, avxSamplerInfo* spec)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(spec);
    AfxAssertObjects(1, &samp, afxFcc_SAMP);

    spec->magFilter = samp->cfg.base.magFilter;
    spec->minFilter = samp->cfg.base.minFilter;
    spec->mipFilter = samp->cfg.base.mipFilter;
    spec->uvw[0] = samp->cfg.base.uvw[0];
    spec->uvw[1] = samp->cfg.base.uvw[1];
    spec->uvw[2] = samp->cfg.base.uvw[2];
    spec->anisotropyEnabled = samp->cfg.base.anisotropyEnabled;
    spec->anisotropyMaxDegree = samp->cfg.base.anisotropyMaxDegree;
    spec->compareEnabled = samp->cfg.base.compareEnabled;
    spec->compareOp = samp->cfg.base.compareOp;
    AfxColorCopy(spec->borderColor, samp->cfg.base.borderColor);
    spec->unnormalizedCoords = samp->cfg.base.unnormalizedCoords;
}

_AVX afxError _AvxSampStdDtor(avxSampler samp)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &samp, afxFcc_SAMP);

    return err;
}

_AVX afxError _AvxSampStdCtor(avxSampler samp, void** args, afxNat invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &samp, afxFcc_SAMP);

    avxYuvSamplerInfo const *cfg = ((avxYuvSamplerInfo const *)args[1]) + invokeNo;
    afxBool yuv = args[2] ? *(afxBool const*)args[2] : FALSE;

    AfxAssert(cfg);
    AfxAssert(cfg[0].base.magFilter < avxTexelFilter_TOTAL);
    AfxAssert(cfg[0].base.minFilter < avxTexelFilter_TOTAL);
    AfxAssert(cfg[0].base.mipFilter < avxTexelFilter_TOTAL);
    AfxAssert(cfg[0].base.uvw[0] < avxTexelAddress_TOTAL);
    AfxAssert(cfg[0].base.uvw[1] < avxTexelAddress_TOTAL);
    AfxAssert(cfg[0].base.uvw[2] < avxTexelAddress_TOTAL);

    samp->crc = 0;
    //AfxAccumulateCrc32(&samp->crc, cfg, yuv ? sizeof(avxYuvSamplerInfo) : sizeof(avxSamplerInfo));

    samp->yuv = !!yuv;

    if (samp->yuv) samp->cfg = *cfg;
    else
    {
        AfxZero(&samp->cfg, sizeof(samp->cfg));
        AfxCopy(&samp->cfg.base, &cfg->base, sizeof(samp->cfg.base));
    }
    return err;
}

_AVX afxClassConfig const _AvxSampStdImplementation =
{
    .fcc = afxFcc_SAMP,
    .name = "Sampler",
    .desc = "Rasterization Sampler",
    .fixedSiz = sizeof(AFX_OBJECT(avxSampler)),
    .ctor = (void*)_AvxSampStdCtor,
    .dtor = (void*)_AvxSampStdDtor
};

////////////////////////////////////////////////////////////////////////////////

_AVX afxError AfxAcquireSamplers(afxDrawContext dctx, afxNat cnt, avxSamplerInfo const cfg[], avxSampler samplers[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    AfxAssert(samplers);
    AfxAssert(cnt);
    afxBool rslt = 0;

    if (cnt != (afxNat)(rslt = AfxFindSamplers(dctx, cnt, cfg, samplers)))
    {
        afxClass* cls = AfxGetSamplerClass(dctx);
        AfxAssertClass(cls, afxFcc_SAMP);

        for (afxNat i = 0; i < cnt; i++)
        {
            if (AfxAcquireObjects(cls, 1, (afxObject*)&samplers[i], (void const*[]) { dctx, (void*)&cfg[i], NIL }))
            {
                AfxThrowError();
                AfxReleaseObjects(rslt, samplers);
            }
        }
    }
    AfxTryAssertObjects(cnt, samplers, afxFcc_SAMP);
    return err;
}

_AVX afxError AfxAcquireYuvSamplers(afxDrawContext dctx, afxNat cnt, avxYuvSamplerInfo const cfg[], avxSampler samplers[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    AfxAssert(samplers);
    AfxAssert(cnt);
    afxBool rslt = 0;

    if (cnt != (afxNat)(rslt = AfxFindYuvSamplers(dctx, cnt, cfg, samplers)))
    {
        afxClass* cls = AfxGetSamplerClass(dctx);
        AfxAssertClass(cls, afxFcc_SAMP);
        
        for (afxNat i = 0; i < cnt; i++)
        {
            if (AfxAcquireObjects(cls, 1, (afxObject*)&samplers[i], (void const*[]) { dctx, (void*)&cfg[i], (afxBool[]) { TRUE } }))
            {
                AfxThrowError();
                AfxReleaseObjects(rslt, samplers);
            }
        }
    }
    AfxTryAssertObjects(cnt, samplers, afxFcc_SAMP);
    return err;
}

struct findSampCb { afxNat crc; avxSampler sampler; };

_AVXINL afxBool _AvxFindSampCompareCb(avxSampler samp, struct findSampCb* udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &samp, afxFcc_SAMP);

    if (samp->crc == udd->crc)
    {
        udd->sampler = samp;
        return FALSE; // break
    }
    return TRUE; // continue
};

_AVXINL afxBool _AvxFindSampCb(avxSampler samp, struct findSampCb* udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &samp, afxFcc_SAMP);

    if (samp->crc == udd->crc)
    {
        udd->sampler = samp;
        return FALSE; // break
    }
    return TRUE; // continue
};

_AVX afxBool AfxFindSamplers(afxDrawContext dctx, afxNat cnt, avxSamplerInfo const cfg[], avxSampler samplers[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    AfxAssert(samplers);
    AfxAssert(cfg);
    AfxAssert(cnt);
    afxBool rslt = 0;
    return rslt;
}

_AVX afxBool AfxFindYuvSamplers(afxDrawContext dctx, afxNat cnt, avxYuvSamplerInfo const cfg[], avxSampler samplers[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    AfxAssert(samplers);
    AfxAssert(cfg);
    AfxAssert(cnt);
    afxBool rslt = 0;
    return rslt;
}

AVX void AfxDescribeDefaultSampler(avxSamplerInfo* config)
{
    afxError err = NIL;
    AfxAssert(config);
    static avxSamplerInfo const def =
    {
        .magFilter = avxTexelFilter_LINEAR,
        .minFilter = avxTexelFilter_POINT,
        .mipFilter = avxTexelFilter_LINEAR,
        .uvw[0] = avxTexelAddress_REPEAT,
        .uvw[1] = avxTexelAddress_REPEAT,
        .uvw[2] = avxTexelAddress_REPEAT,
        .anisotropyEnabled = FALSE,
        .anisotropyMaxDegree = 1,
        .compareEnabled = FALSE,
        .compareOp = avxCompareOp_LEQUAL,
        .lodBias = 0,
        .minLod = -1000,
        .maxLod = 1000,
        .borderColor[0] = 0.f,
        .borderColor[1] = 0.f,
        .borderColor[2] = 0.f,
        .borderColor[3] = 0.f,
        .unnormalizedCoords = FALSE
    };
    *config = def;
}
