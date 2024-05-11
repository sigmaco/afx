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
 *                       (c) 2017 SIGMA, Engitech, Scitech, Serpro
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

    spec->magFilter = samp->magFilter;
    spec->minFilter = samp->minFilter;
    spec->mipmapFilter = samp->mipmapFilter;
    spec->uvw[0] = samp->uvw[0];
    spec->uvw[1] = samp->uvw[1];
    spec->uvw[2] = samp->uvw[2];
    spec->anisotropyEnabled = samp->anisotropyEnabled;
    spec->anisotropyMaxDegree = samp->anisotropyMaxDegree;
    spec->compareEnabled = samp->compareEnabled;
    spec->compareOp = samp->compareOp;
    AfxCopyColor(spec->borderColor, samp->borderColor);
    spec->unnormalizedCoords = samp->unnormalizedCoords;
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

    afxSamplerConfig const *config = ((afxSamplerConfig const *)cookie->udd[0]) + cookie->no;

    AfxAssert(config);
    AfxAssert(config[0].magFilter < afxTexelFilter_TOTAL);
    AfxAssert(config[0].minFilter < afxTexelFilter_TOTAL);
    AfxAssert(config[0].mipmapFilter < afxTexelFilter_TOTAL);
    AfxAssert(config[0].uvw[0] < afxTexelAddress_TOTAL);
    AfxAssert(config[0].uvw[1] < afxTexelAddress_TOTAL);
    AfxAssert(config[0].uvw[2] < afxTexelAddress_TOTAL);

    samp->crc32 = 0;
    AfxAccumulateCrc32(&samp->crc32, config, sizeof(*config));

    samp->magFilter = config->magFilter;
    samp->minFilter = config->minFilter;
    samp->mipmapFilter = config->mipmapFilter;
    samp->uvw[0] = config->uvw[0];
    samp->uvw[1] = config->uvw[1];
    samp->uvw[2] = config->uvw[2];
    samp->anisotropyEnabled = config->anisotropyEnabled;
    samp->anisotropyMaxDegree = config->anisotropyMaxDegree;
    samp->compareEnabled = config->compareEnabled;
    samp->compareOp = config->compareOp;
    AfxCopyColor(samp->borderColor, config->borderColor);
    samp->unnormalizedCoords = config->unnormalizedCoords;
    samp->lodBias = config->lodBias;
    samp->minLod = config->minLod;
    samp->maxLod = config->maxLod;

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

_AVX afxError AfxAcquireSamplers(afxDrawContext dctx, afxNat cnt, afxSamplerConfig const config[], afxSampler samplers[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    AfxAssert(samplers);
    AfxAssert(cnt);

    afxManager* cls = AfxGetSamplerClass(dctx);
    AfxAssertClass(cls, afxFcc_SAMP);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)samplers, (void const*[]) { (void*)config }))
        AfxThrowError();

    return err;
}

AVX void AfxDescribeDefaultSampler(afxSamplerConfig* config)
{
    afxError err = NIL;
    AfxAssert(config);
    static afxSamplerConfig const def =
    {
        .magFilter = afxTexelFilter_LINEAR,
        .minFilter = afxTexelFilter_POINT,
        .mipmapFilter = afxTexelFilter_LINEAR,
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

AVX afxSampler AfxAcquireBilinearSampler(afxDrawContext dctx)
{
    afxError err = NIL;
    afxSamplerConfig config;
    AfxDescribeDefaultSampler(&config);
    afxSampler smp;

    if (AfxAcquireSamplers(dctx, 1, &config, &smp))
        AfxThrowError();

    return smp;
}

AVX afxSampler AfxAcquireTrilinearSampler(afxDrawContext dctx)
{
    afxError err = NIL;
    afxSamplerConfig config;
    AfxDescribeDefaultSampler(&config);
    config.minFilter = afxTexelFilter_LINEAR;
    afxSampler smp;

    if (AfxAcquireSamplers(dctx, 1, &config, &smp))
        AfxThrowError();

    return smp;
}
