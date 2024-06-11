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
#include "qwadro/draw/afxDrawSystem.h"

_AVX void AfxDescribeSampler(afxSampler samp, afxSamplerConfig* spec)
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
    AfxCopyColor(spec->borderColor, samp->cfg.base.borderColor);
    spec->unnormalizedCoords = samp->cfg.base.unnormalizedCoords;
}

_AVX afxError _AvxSampStdDtor(afxSampler samp)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &samp, afxFcc_SAMP);

    return err;
}

_AVX afxError _AvxSampStdCtor(afxSampler samp, afxCookie const* cookie)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &samp, afxFcc_SAMP);

    afxYuvSamplerConfig const *cfg = ((afxYuvSamplerConfig const *)cookie->udd[1]) + cookie->no;
    afxBool yuv = *(afxBool const*)cookie->udd[2];

    AfxAssert(cfg);
    AfxAssert(cfg[0].base.magFilter < afxTexelFilter_TOTAL);
    AfxAssert(cfg[0].base.minFilter < afxTexelFilter_TOTAL);
    AfxAssert(cfg[0].base.mipFilter < afxTexelFilter_TOTAL);
    AfxAssert(cfg[0].base.uvw[0] < afxTexelAddress_TOTAL);
    AfxAssert(cfg[0].base.uvw[1] < afxTexelAddress_TOTAL);
    AfxAssert(cfg[0].base.uvw[2] < afxTexelAddress_TOTAL);

    samp->crc = 0;
    AfxAccumulateCrc32(&samp->crc, cfg, yuv ? sizeof(afxYuvSamplerConfig) : sizeof(afxSamplerConfig));

    samp->yuv = !!yuv;

    if (samp->yuv)
        samp->cfg = *cfg;
    else
        samp->cfg = (afxYuvSamplerConfig) { 0 }, samp->cfg.base = cfg->base;

    return err;
}

_AVX afxClassConfig const _AvxSampStdImplementation =
{
    .fcc = afxFcc_SAMP,
    .name = "Sampler",
    .desc = "Rasterization Sampler",
    .unitsPerPage = 2,
    .size = sizeof(AFX_OBJECT(afxSampler)),
    .ctor = (void*)_AvxSampStdCtor,
    .dtor = (void*)_AvxSampStdDtor
};

////////////////////////////////////////////////////////////////////////////////

_AVX afxError AfxAcquireSamplers(afxDrawContext dctx, afxNat cnt, afxSamplerConfig const cfg[], afxSampler samplers[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    AfxAssert(samplers);
    AfxAssert(cnt);
    afxBool rslt = 0;

    if (cnt != (afxNat)(rslt = AfxFindSamplers(dctx, cnt, cfg, samplers)))
    {
        afxManager* cls = AfxGetSamplerClass(dctx);
        AfxAssertClass(cls, afxFcc_SAMP);

        for (afxNat i = 0; i < cnt; i++)
        {
            if (AfxAcquireObjects(cls, 1, (afxObject*)&samplers[i], (void const*[]) { dctx, (void*)&cfg[i], (afxBool[]) { FALSE } }))
            {
                AfxThrowError();
                AfxReleaseObjects(rslt, samplers);
            }
        }
    }
    AfxTryAssertObjects(cnt, samplers, afxFcc_SAMP);
    return err;
}

_AVX afxError AfxAcquireYuvSamplers(afxDrawContext dctx, afxNat cnt, afxYuvSamplerConfig const cfg[], afxSampler samplers[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    AfxAssert(samplers);
    AfxAssert(cnt);
    afxBool rslt = 0;

    if (cnt != (afxNat)(rslt = AfxFindYuvSamplers(dctx, cnt, cfg, samplers)))
    {
        afxManager* cls = AfxGetSamplerClass(dctx);
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

struct findSampCb { afxNat crc; afxSampler sampler; };

_AVXINL afxBool _AvxFindSampCb(afxSampler samp, struct findSampCb* udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &samp, afxFcc_SAMP);

    if (samp->crc == udd->crc)
    {
        udd->sampler = samp;
        return 1; // break
    }
    return 0; // continue
};

_AVX afxBool AfxFindSamplers(afxDrawContext dctx, afxNat cnt, afxSamplerConfig const cfg[], afxSampler samplers[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    AfxAssert(samplers);
    AfxAssert(cfg);
    AfxAssert(cnt);
    
    afxBool rslt = 0;

    afxManager* cls = AfxGetSamplerClass(dctx);
    AfxAssertClass(cls, afxFcc_SAMP);

    for (afxNat i = 0; i < cnt; i++)
    {
        afxSamplerConfig cfg2 = cfg[i];
        afxNat32 crc = 0;
        AfxAccumulateCrc32(&crc, &cfg2, sizeof(cfg2));
        struct findSampCb udd;
        udd.crc = crc;
        udd.sampler = NIL;
        AfxInvokeObjects(cls, 0, AFX_N32_MAX, (void*)_AvxFindSampCb, &crc);
        AfxTryAssertObjects(1, &udd.sampler, afxFcc_SAMP);
        AfxAssert(!udd.sampler || crc == udd.sampler->crc);
        samplers[i] = udd.sampler;

        if (udd.sampler)
            ++rslt;
    }
    return rslt;
}

_AVX afxBool AfxFindYuvSamplers(afxDrawContext dctx, afxNat cnt, afxYuvSamplerConfig const cfg[], afxSampler samplers[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    AfxAssert(samplers);
    AfxAssert(cfg);
    AfxAssert(cnt);
    afxBool rslt = 0;

    afxManager* cls = AfxGetSamplerClass(dctx);
    AfxAssertClass(cls, afxFcc_SAMP);

    for (afxNat i = 0; i < cnt; i++)
    {
        afxYuvSamplerConfig cfg2 = cfg[i];
        afxNat32 crc = 0;
        AfxAccumulateCrc32(&crc, &cfg2, sizeof(cfg2));
        struct findSampCb udd;
        udd.crc = crc;
        udd.sampler = NIL;
        AfxInvokeObjects(cls, 0, AFX_N32_MAX, (void*)_AvxFindSampCb, &crc);
        AfxTryAssertObjects(1, &udd.sampler, afxFcc_SAMP);
        AfxAssert(!udd.sampler || crc == udd.sampler->crc);
        samplers[i] = udd.sampler;

        if (udd.sampler)
            ++rslt;
    }
    return rslt;
}

AVX void AfxDescribeDefaultSampler(afxSamplerConfig* config)
{
    afxError err = NIL;
    AfxAssert(config);
    static afxSamplerConfig const def =
    {
        .magFilter = afxTexelFilter_LINEAR,
        .minFilter = afxTexelFilter_POINT,
        .mipFilter = afxTexelFilter_LINEAR,
        .uvw[0] = afxTexelAddress_REPEAT,
        .uvw[1] = afxTexelAddress_REPEAT,
        .uvw[2] = afxTexelAddress_REPEAT,
        .anisotropyEnabled = FALSE,
        .anisotropyMaxDegree = 1,
        .compareEnabled = FALSE,
        .compareOp = afxCompareOp_LEQUAL,
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
