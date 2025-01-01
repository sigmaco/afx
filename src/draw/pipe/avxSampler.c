/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *        Q W A D R O   V I D E O   G R A P H I C S   I N F R A S T R U C T U R E
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#define _AVX_DRAW_C
#define _AVX_SAMPLER_C
#include "../impl/avxImplementation.h"

_AVX void AfxDescribeSampler(avxSampler samp, avxSamplerInfo* spec)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(spec);
    AFX_ASSERT_OBJECTS(afxFcc_SAMP, 1, &samp);

    spec->magnify = samp->cfg.base.magnify;
    spec->minify = samp->cfg.base.minify;
    spec->mipify = samp->cfg.base.mipify;
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
    AFX_ASSERT_OBJECTS(afxFcc_SAMP, 1, &samp);

    return err;
}

_AVX afxError _AvxSampStdCtor(avxSampler samp, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SAMP, 1, &samp);

    avxYuvSamplerInfo const *cfg = ((avxYuvSamplerInfo const *)args[1]) + invokeNo;
    afxBool yuv = args[2] ? *(afxBool const*)args[2] : FALSE;

    AFX_ASSERT(cfg);
    AFX_ASSERT(cfg[0].base.magnify < avxTexelFilter_TOTAL);
    AFX_ASSERT(cfg[0].base.minify < avxTexelFilter_TOTAL);
    AFX_ASSERT(cfg[0].base.mipify < avxTexelFilter_TOTAL);
    AFX_ASSERT(cfg[0].base.uvw[0] < avxTexelAddress_TOTAL);
    AFX_ASSERT(cfg[0].base.uvw[1] < avxTexelAddress_TOTAL);
    AFX_ASSERT(cfg[0].base.uvw[2] < avxTexelAddress_TOTAL);

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

_AVX afxClassConfig const _AVX_SAMP_CLASS_CONFIG =
{
    .fcc = afxFcc_SAMP,
    .name = "Sampler",
    .desc = "Rasterization Sampler",
    .fixedSiz = sizeof(AFX_OBJECT(avxSampler)),
    .ctor = (void*)_AvxSampStdCtor,
    .dtor = (void*)_AvxSampStdDtor
};

////////////////////////////////////////////////////////////////////////////////

_AVX afxError AfxDeclareSamplers(afxDrawSystem dsys, afxUnit cnt, avxSamplerInfo const cfg[], avxSampler samplers[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    AFX_ASSERT(samplers);
    AFX_ASSERT(cnt);
    afxBool rslt = 0;

    if (cnt != (afxUnit)(rslt = AfxFindSamplers(dsys, cnt, cfg, samplers)))
    {
        afxClass* cls = AvxGetSamplerClass(dsys);
        AFX_ASSERT_CLASS(cls, afxFcc_SAMP);

        for (afxUnit i = 0; i < cnt; i++)
        {
            if (AfxAcquireObjects(cls, 1, (afxObject*)&samplers[i], (void const*[]) { dsys, (void*)&cfg[i], NIL }))
            {
                AfxThrowError();
                AfxDisposeObjects(rslt, samplers);
            }
        }
    }
    AFX_TRY_ASSERT_OBJECTS(afxFcc_SAMP, cnt, samplers);
    return err;
}

_AVX afxError AfxDeclareYuvSamplers(afxDrawSystem dsys, afxUnit cnt, avxYuvSamplerInfo const cfg[], avxSampler samplers[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    AFX_ASSERT(samplers);
    AFX_ASSERT(cnt);
    afxBool rslt = 0;

    if (cnt != (afxUnit)(rslt = AfxFindYuvSamplers(dsys, cnt, cfg, samplers)))
    {
        afxClass* cls = AvxGetSamplerClass(dsys);
        AFX_ASSERT_CLASS(cls, afxFcc_SAMP);
        
        for (afxUnit i = 0; i < cnt; i++)
        {
            if (AfxAcquireObjects(cls, 1, (afxObject*)&samplers[i], (void const*[]) { dsys, (void*)&cfg[i], (afxBool[]) { TRUE } }))
            {
                AfxThrowError();
                AfxDisposeObjects(rslt, samplers);
            }
        }
    }
    AFX_TRY_ASSERT_OBJECTS(afxFcc_SAMP, cnt, samplers);
    return err;
}

struct findSampCb { afxUnit crc; avxSampler sampler; };

_AVXINL afxBool _AvxFindSampCompareCb(avxSampler samp, struct findSampCb* udd)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SAMP, 1, &samp);

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
    AFX_ASSERT_OBJECTS(afxFcc_SAMP, 1, &samp);

    if (samp->crc == udd->crc)
    {
        udd->sampler = samp;
        return FALSE; // break
    }
    return TRUE; // continue
};

_AVX afxBool AfxFindSamplers(afxDrawSystem dsys, afxUnit cnt, avxSamplerInfo const cfg[], avxSampler samplers[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    AFX_ASSERT(samplers);
    AFX_ASSERT(cfg);
    AFX_ASSERT(cnt);
    afxBool rslt = 0;
    return rslt;
}

_AVX afxBool AfxFindYuvSamplers(afxDrawSystem dsys, afxUnit cnt, avxYuvSamplerInfo const cfg[], avxSampler samplers[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    AFX_ASSERT(samplers);
    AFX_ASSERT(cfg);
    AFX_ASSERT(cnt);
    afxBool rslt = 0;
    return rslt;
}

AVX void AfxDescribeDefaultSampler(avxSamplerInfo* config)
{
    afxError err = NIL;
    AFX_ASSERT(config);
    static avxSamplerInfo const def =
    {
        .magnify = avxTexelFilter_LINEAR,
        .minify = avxTexelFilter_POINT,
        .mipify = avxTexelFilter_LINEAR,
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
