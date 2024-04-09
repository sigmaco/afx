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
 *                       (c) 2017 SIGMA, Engineering In Technology
 *                             <https://sigmaco.org/qwadro/>
 */

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#define _AFX_DRAW_C
#define _AFX_SAMPLER_C
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

////////////////////////////////////////////////////////////////////////////////

_AVX afxNat AfxEnumerateSamplers(afxDrawContext dctx, afxNat first, afxNat cnt, afxSampler samplers[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    AfxAssert(samplers);
    AfxAssert(cnt);
    afxManager* cls = AfxGetSamplerClass(dctx);
    AfxAssertClass(cls, afxFcc_SAMP);
    return AfxEnumerateObjects(cls, first, cnt, (afxObject*)samplers);
}

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
