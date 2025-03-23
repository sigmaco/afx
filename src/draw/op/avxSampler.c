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

_AVX avxClearValue const AVX_CLEAR_VALUE_DEFAULT =
{
    .rgba[0] = 0.f,
    .rgba[1] = 0.f,
    .rgba[2] = 0.f,
    .rgba[3] = 0.f,
};

_AVX avxSamplerInfo const AVX_SAMPLER_DEFAULT =
{
    .flags = NIL,

    .uvw[0] = avxTexelWrap_REPEAT,
    .uvw[1] = avxTexelWrap_REPEAT,
    .uvw[2] = avxTexelWrap_REPEAT,

    .magnify = avxTexelFilter_LINEAR,
    .minify = avxTexelFilter_NEAREST,
    .mipFlt = avxTexelFilter_LINEAR,
    .reductionMode = avxTexelReduction_AVG,
    .anisotropyEnabled = FALSE,
    .anisotropyMaxDegree = 1.f,

    .compareEnabled = FALSE,
    .compareOp = avxCompareOp_LEQUAL,

    .lodBias = 0,
    .minLod = -1000,
    .maxLod = 1000,

    .borderColor = AVX_CLEAR_VALUE_DEFAULT,

    .unnormalizedCoords = FALSE,

    .isYuv = FALSE,
    .yuvFmt = avxFormat_UNDEFINED,
    .yuvMtx = avxColorMatrix_RGB,
    .useNarrowRange = FALSE,
    .yuvSizzle =
    { 
        avxColorSwizzle_R,
        avxColorSwizzle_G,
        avxColorSwizzle_B,
        avxColorSwizzle_A
    },
    .xChromaOffset = FALSE,
    .yChromaOffset = FALSE,
    .chromaFilter = avxTexelFilter_LINEAR,
    .forceExplicitReconstr = FALSE
};

_AVX afxBool AvxIsSamplerYuvCapable(avxSampler samp)
{
    afxError err = AFX_ERR_NONE;
    // samp must be a valid avxSampler handle.
    AFX_ASSERT_OBJECTS(afxFcc_SAMP, 1, &samp);
    return samp->cfg.isYuv;
}

_AVX void AvxGetSamplerBorderColor(avxSampler samp, avxClearValue* val)
{
    afxError err = AFX_ERR_NONE;
    // samp must be a valid avxSampler handle.
    AFX_ASSERT_OBJECTS(afxFcc_SAMP, 1, &samp);
    // val must be a valid pointer to a valid avxClearValue structure.
    AFX_ASSERT(val);
    *val = samp->cfg.borderColor;
}

_AVX void AvxDescribeSampler(avxSampler samp, avxSamplerInfo* desc)
{
    afxError err = AFX_ERR_NONE;
    // samp must be a valid avxSampler handle.
    AFX_ASSERT_OBJECTS(afxFcc_SAMP, 1, &samp);
    // desc must be a valid pointer to a valid avxSamplerInfo structure.
    AFX_ASSERT(desc);
    *desc = samp->cfg;
}

_AVX afxError _AvxSampDtorCb(avxSampler samp)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SAMP, 1, &samp);

    return err;
}

_AVX afxError _AvxSampCtorCb(avxSampler samp, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SAMP, 1, &samp);

    afxDrawSystem dsys = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    avxSamplerInfo const *cfg = ((avxSamplerInfo const *)args[1]) + invokeNo;
    
    AFX_ASSERT(cfg);
    samp->tag = cfg->tag;
    samp->cfg = *cfg;
    samp->crc = 0;
    //AfxAccumulateCrc32(&samp->crc, cfg, yuv ? sizeof(avxYuvSamplerInfo) : sizeof(avxSamplerInfo));

    afxDrawLimits const* limits = _AvxDsysAccessLimits(dsys);
    afxDrawFeatures const* features = _AvxDsysAccessReqFeatures(dsys);

    // magFlt, minFlt and mipFlt must be a valid avxTexelFilter value.
    AFX_ASSERT(samp->cfg.magnify < avxTexelFilter_TOTAL);
    AFX_ASSERT(samp->cfg.minify < avxTexelFilter_TOTAL);
    AFX_ASSERT(samp->cfg.mipFlt < avxTexelFilter_TOTAL);

    // uvw must be a valid avxTexelWrap value.
    AFX_ASSERT(samp->cfg.uvw[0] < avxTexelWrap_TOTAL);
    AFX_ASSERT(samp->cfg.uvw[1] < avxTexelWrap_TOTAL);
    AFX_ASSERT(samp->cfg.uvw[2] < avxTexelWrap_TOTAL);

    if (features->samplerFilterMinMax == FALSE)
    {
        // When samplerFilterMinmax feature is not enabled, reduction mode must be WEIGHTED_AVERAGE.
        AFX_ASSERT(samp->cfg.reductionMode == avxTexelReduction_AVG);
    }

    if (features->samplerMirrorClampToEdge == FALSE)
    {
        // When samplerMirrorClampToEdge feature is not enabled, UVW addressing modes must not be REFLECT.
        AFX_ASSERT(samp->cfg.uvw[0] != avxTexelWrap_REFLECT);
        AFX_ASSERT(samp->cfg.uvw[0] != avxTexelWrap_REFLECT);
        AFX_ASSERT(samp->cfg.uvw[0] != avxTexelWrap_REFLECT);
    }

    if (samp->cfg.compareEnabled)
    {
        // When comparision is enabled, compareOp must be a valid avxCompareOp value.
        AFX_ASSERT(samp->cfg.compareOp < avxCompareOp_TOTAL);

        // When comparision is enabled, the reduction mode must be WEIGHTED_AVERAGE.
        AFX_ASSERT(samp->cfg.reductionMode == avxTexelReduction_AVG);

    }

    if (samp->cfg.anisotropyEnabled)
    {
        // When anisotropy is enabled, the samplerAnisotropy feature is required to be enabled.
        AFX_ASSERT(features->samplerAnisotropy);
        // When anisotropy is enabled, maxAnisotropy must be between 1.0 and maxSamplerAnisotropy, inclusive.
        AFX_ASSERT(limits->maxSamplerAnisotropy >= samp->cfg.anisotropyMaxDegree);
        samp->cfg.anisotropyMaxDegree = AfxClamp(samp->cfg.anisotropyMaxDegree, 1.0, limits->maxSamplerAnisotropy);
    }

    // The absolute value of lodBias must be less than or equal to maxSamplerLodBias.
    AFX_ASSERT(limits->maxSamplerLodBias >= samp->cfg.lodBias);
    samp->cfg.lodBias = AfxMin(samp->cfg.lodBias, limits->maxSamplerLodBias);

    // maxLod must be greater than or equal to minLod.
    AFX_ASSERT(samp->cfg.maxLod >= samp->cfg.minLod);
    
    if (samp->cfg.unnormalizedCoords)
    {
        // When unnormalizedCoordinates is enabled, minification and magnification filters must be equal.
        AFX_ASSERT(samp->cfg.minify == samp->cfg.magnify);
        // When unnormalizedCoordinates is enabled, mipmap filter must be NEAREST.
        AFX_ASSERT(samp->cfg.mipFlt == avxTexelFilter_NEAREST);
        // When unnormalizedCoordinates is enabled, minLod and maxLod must be zero.
        AFX_ASSERT(samp->cfg.minLod == 0);
        AFX_ASSERT(samp->cfg.maxLod == 0);
        // When unnormalizedCoordinates is enabled, UVW addressing modes must each be either EDGE or BORDER.
        AFX_ASSERT((samp->cfg.uvw[0] == avxTexelWrap_EDGE) || (samp->cfg.uvw[0] == avxTexelWrap_BORDER));
        AFX_ASSERT((samp->cfg.uvw[1] == avxTexelWrap_EDGE) || (samp->cfg.uvw[0] == avxTexelWrap_BORDER));
        AFX_ASSERT((samp->cfg.uvw[2] == avxTexelWrap_EDGE) || (samp->cfg.uvw[0] == avxTexelWrap_BORDER));
        // When unnormalizedCoordinates is enabled, anisotropy must be disabled.
        AFX_ASSERT(samp->cfg.anisotropyEnabled == FALSE);
        // When unnormalizedCoordinates is enabled, comparison must be disabled.
        AFX_ASSERT(samp->cfg.compareEnabled == FALSE);
    }

    if (samp->cfg.isYuv)
    {
        // When sampler YUV conversion is enabled, UVW addressing mode must be EDGE.
        AFX_ASSERT(samp->cfg.uvw[0] == avxTexelWrap_EDGE);
        AFX_ASSERT(samp->cfg.uvw[1] == avxTexelWrap_EDGE);
        AFX_ASSERT(samp->cfg.uvw[2] == avxTexelWrap_EDGE);
        samp->cfg.uvw[0] = avxTexelWrap_EDGE;
        samp->cfg.uvw[1] = avxTexelWrap_EDGE;
        samp->cfg.uvw[2] = avxTexelWrap_EDGE;

        // When sampler YUV conversion is enabled, anisotropy must NOT be enabled.
        AFX_ASSERT(samp->cfg.anisotropyEnabled == FALSE);
        samp->cfg.anisotropyEnabled = FALSE;

        // When sampler YUV conversion is enabled, unnormalizedCoordinates must NOT be enabled.
        AFX_ASSERT(samp->cfg.unnormalizedCoords == FALSE);
        samp->cfg.unnormalizedCoords = FALSE;

        // When sampler YUV conversion is enabled, reduction mode must be WEIGHTED_AVERAGE.
        AFX_ASSERT(samp->cfg.reductionMode == avxTexelReduction_AVG);
        samp->cfg.reductionMode = avxTexelReduction_AVG;

        // chromaFilter must be a valid avxTexelFilter value.
        AFX_ASSERT(samp->cfg.chromaFilter < avxTexelFilter_TOTAL);

        // components must be a valid avxSwizzling structure.
        AFX_ASSERT(samp->cfg.yuvSizzle.r < avxColorSwizzle_TOTAL);
        AFX_ASSERT(samp->cfg.yuvSizzle.g < avxColorSwizzle_TOTAL);
        AFX_ASSERT(samp->cfg.yuvSizzle.b < avxColorSwizzle_TOTAL);
        AFX_ASSERT(samp->cfg.yuvSizzle.a < avxColorSwizzle_TOTAL);

        // convMtx must be a valid avxColorMatrix value.
        AFX_ASSERT(samp->cfg.yuvMtx < avxColorMatrix_TOTAL);

        // format must be a valid avxFormat value.
        AFX_ASSERT(samp->cfg.yuvFmt < avxFormat_TOTAL);
    }
    return err;
}

_AVX afxClassConfig const _AVX_SAMP_CLASS_CONFIG =
{
    .fcc = afxFcc_SAMP,
    .name = "Sampler",
    .desc = "Rasterization Sampler",
    .fixedSiz = sizeof(AFX_OBJECT(avxSampler)),
    .ctor = (void*)_AvxSampCtorCb,
    .dtor = (void*)_AvxSampDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AVX afxError AvxDeclareSamplers(afxDrawSystem dsys, afxUnit cnt, avxSamplerInfo const cfg[], avxSampler samplers[])
{
    afxError err = AFX_ERR_NONE;
    // dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    AFX_ASSERT(cnt);
    // cfg must be a valid pointer to a valid avxSamplerInfo structure.
    AFX_ASSERT(cfg);
    // samplers must be a valid pointer to a avxSampler handle.
    AFX_ASSERT(samplers);

    // dsys must support at least one bridge with one of the GRAPHICS or COMPUTE capabilities.    
    afxDrawBridge dexu;
    afxBool bridgedFound = AvxChooseDrawBridges(dsys, AFX_INVALID_INDEX, AFX_INVALID_INDEX, afxDrawPortFlag_DRAW | afxDrawPortFlag_COMPUTE, 0, 1, &dexu);
    AFX_ASSERT(bridgedFound);

    afxClass* cls = (afxClass*)_AvxDsysGetImpl(dsys)->sampCls(dsys);
    AFX_ASSERT_CLASS(cls, afxFcc_SAMP);

    // There must have less than maxSamplerAllocCnt avxSampler objects currently created on the system.
    afxDrawLimits const* limits = _AvxDsysAccessLimits(dsys);
    AFX_ASSERT(limits->maxSamplerAllocCnt > cls->instCnt);

    afxBool rslt = 0;

    if (cnt != (afxUnit)(rslt = AvxFindSamplers(dsys, cnt, cfg, samplers)))
    {
        for (afxUnit i = 0; i < cnt; i++)
        {
            if (AfxAcquireObjects(cls, 1, (afxObject*)&samplers[i], (void const*[]) { dsys, (void*)&cfg[i], NIL }))
            {
                AfxThrowError();
                AfxDisposeObjects(rslt, samplers);
            }
        }
    }

    if (err)
        return err;

    AFX_ASSERT_OBJECTS(afxFcc_SAMP, cnt, samplers);

#if AVX_VALIDATION_ENABLED
    for (afxUnit i = 0; i < cnt; i++)
    {
        avxSampler smp = samplers[i];
    }
#endif

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

_AVX afxBool AvxFindSamplers(afxDrawSystem dsys, afxUnit cnt, avxSamplerInfo const cfg[], avxSampler samplers[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    AFX_ASSERT(samplers);
    AFX_ASSERT(cfg);
    AFX_ASSERT(cnt);
    afxBool rslt = 0;
    return rslt;
}
