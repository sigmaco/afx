/*
 *           ::::::::    :::::::::::    ::::::::    ::::     ::::       :::
 *          :+:    :+:       :+:       :+:    :+:   +:+:+: :+:+:+     :+: :+:
 *          +:+              +:+       +:+          +:+ +:+:+ +:+    +:+   +:+
 *          +#++:++#++       +#+       :#:          +#+  +:+  +#+   +#++:++#++:
 *                 +#+       +#+       +#+   +#+#   +#+       +#+   +#+     +#+
 *          #+#    #+#       #+#       #+#    #+#   #+#       #+#   #+#     #+#
 *           ########    ###########    ########    ###       ###   ###     ###
 *
 *                     S I G M A   T E C H N O L O G Y   G R O U P
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This software is part of Advanced Video Graphics Extensions & Experiments.

#define _AVX_DRAW_C
#define _AVX_SAMPLER_C
#include "avxIcd.h"

void sample_bilinear(avxColor texture[], int width, int height, afxReal u, afxReal v, avxColor rslt)
{
    /*
        Bilinear Filtering
        Interpolates between the four nearest texels.
    */

    afxReal x = u * width - 0.5f;
    afxReal y = v * height - 0.5f;

    int x0 = (int)floorf(x);
    int y0 = (int)floorf(y);
    int x1 = x0 + 1;
    int y1 = y0 + 1;

    afxReal tx = x - x0;
    afxReal ty = y - y0;

    x0 = x0 < 0 ? 0 : (x0 >= width ? width - 1 : x0);
    y0 = y0 < 0 ? 0 : (y0 >= height ? height - 1 : y0);
    x1 = x1 < 0 ? 0 : (x1 >= width ? width - 1 : x1);
    y1 = y1 < 0 ? 0 : (y1 >= height ? height - 1 : y1);

    avxColor c00;
    AfxV4dCopy(c00, &texture[y0][x0]);
    avxColor c10;
    AfxV4dCopy(c10, &texture[y0][x1]);
    avxColor c01;
    AfxV4dCopy(c01, &texture[y1][x0]);
    avxColor c11;
    AfxV4dCopy(c11, &texture[y1][x1]);

    // Linear interpolation (lerp)
    avxColor c0;
    AfxV4dMix(c0, c00, c10, tx);
    avxColor c1;
    AfxV4dMix(c1, c01, c11, tx);
    // result.r = a.r * (1 - t) + b.r * t;
    AfxV4dMix(rslt, c0, c1, ty);
}

void sample_trilinear(avxColor texture0[], int w0, int h0, avxColor texture1[], int w1, int h1, afxReal u, afxReal v, afxReal lambda, avxColor rslt)
{
    /*
        Trilinear Filtering
        Bilinear filtering across two mipmap levels, then interpolate between those.

        We need:
            Two mip levels (texture0, texture1)
            A mipmap level interpolation factor lambda
    */

    avxColor c0;
    sample_bilinear(texture0, w0, h0, u, v, c0);
    avxColor c1;
    sample_bilinear(texture1, w1, h1, u, v, c1);
    AfxV4dMix(rslt, c0, c1, lambda);
}

void sample_anisotropic(avxColor texture[], int width, int height, afxReal u, afxReal v, afxReal du_dx, afxReal dv_dx, afxReal du_dy, afxReal dv_dy, avxColor rslt)
{
    /*
        Anisotropic filtering is possible to simulate in C, but it's complex because it typically involves sampling along an elliptical 
        footprint in texture space, which mimics how GPU hardware handles anisotropy. Implementing full anisotropic filtering in software 
        isn't trivial, but we can achieve a simplified C implementation that captures the core idea: more samples along the direction of texture stretch (anisotropy).

        Basic idea of anisotropic filtering.
        Detect stretching: Compute the derivatives of texture coordinates (how fast they change across screen space).
        Determine dominant axis: Decide if the texture is more stretched in the u or v direction.
        Sample multiple texels along that axis.
        Average (or weighted average) those samples

        Following there is a simplified version assuming you can compute or estimate the derivatives du/dx, dv/dx, du/dy, dv/dy. 
        In real applications, these come from screen-space gradients (usually available in shaders). For CPU-side simulations, 
        we can approximate.

        du_dx, dv_dx, etc. represent texture coordinate derivatives across screen space.
        This version samples along one axis. Full anisotropic filtering would ideally sample along an ellipse in u,v space; e.g., Elliptical Weighted Average (EWA).
        We could expand this to sample along a 2D footprint if needed (much slower though).

        Real GPU Anisotropic Filtering (for context)
        Modern GPUs sample along elliptical footprints using:
            EWA Filtering
            Mipmapping with anisotropic weighting
        These use hardware-accelerated derivatives and mipmaps to fetch samples efficiently.
    */

    // Compute scale factors (approximate)
    afxReal dudx2 = du_dx * du_dx + dv_dx * dv_dx;
    afxReal dudy2 = du_dy * du_dy + dv_dy * dv_dy;

    afxUnit const MAX_SAMPLES = 8;

    // Maximum stretch direction
    afxReal major_len = sqrtf(fmaxf(dudx2, dudy2));
    int num_samples = (int)(major_len * MAX_SAMPLES);
    if (num_samples < 1) num_samples = 1;
    if (num_samples > MAX_SAMPLES) num_samples = MAX_SAMPLES;

    // Sample along the dominant direction
    for (afxUnit i = 0; i < num_samples; ++i)
    {
        afxReal offset = ((afxReal)i + 0.5f) / num_samples - 0.5f;

        // Sample offset along anisotropy direction (simplified as horizontal here)
        afxReal sample_u = u + offset * du_dx;
        afxReal sample_v = v + offset * dv_dx;

        avxColor c;
        sample_bilinear(texture, width, height, sample_u, sample_v, c);
        rslt[0] += c[0];
        rslt[1] += c[1];
        rslt[2] += c[2];
        rslt[3] += c[3];
    }

    // Average
    rslt[0] /= num_samples;
    rslt[1] /= num_samples;
    rslt[2] /= num_samples;
    rslt[3] /= num_samples;
}

_AVX void AvxGetColorMatrix(avxColorMatrix cm, afxReal yOffset, afxReal uvOffset, afxBool fullRange, afxM4d m)
{
    /*
        This function is designed to return a 4×4 color conversion matrix for a given color matrix type (like avxColorMatrix_YUV709, 
        avxColorMatrix_YUV601, etc.). It takes an avxColorMatrix enum value (e.g., avxColorMatrix_YUV601) as input, 
        then fills the provided float m[4][4] matrix with the appropriate color space conversion coefficients.
        These coefficients are likely used to convert from YUV (or Y'CbCr) to RGB using a 4×4 matrix, which may also include offsets (as part of the last column).

        Using a 4×4 matrix allows for affine transformations, including bias offsets (like subtracting 128 from Cb/Cr) in a single matrix operation.
        This is why it's common to embed the bias offset (e.g., -128) into m03, m13, m23.
        This way, if our image data is stored as (Y, Cb, Cr, 1) per pixel (as a vector), multiplying by this matrix gives us (R, G, B, 1).

        In full-range, we can typically skip rescaling Y, we you still need to subtract 128 from Cb/Cr (or U/V) because they’re still centered at 128.
    */

    afxError err = { 0 };
    AFX_ASSERT(m);

    // yOffset = 0.0f;
    // uvOffset = 128.0f;

    // For homogeneous coordinates.
    AfxM4dReset(m);

    switch (cm)
    {
    // Identity matrix (no conversion)
    case avxColorMatrix_RGB: break;
    case avxColorMatrix_YUV:
        // No model conversion, but you might scale or shift here
        m[0][0] = 1.0f;
        m[1][1] = 1.0f;
        m[2][2] = 1.0f;
        break;

    case avxColorMatrix_YUV601:
    {
        if (fullRange)
        {
            m[0][0] = 1.0f;
            m[0][2] = 1.402f;
            m[0][3] = -1.402f * uvOffset;

            m[1][0] = 1.0f;
            m[1][1] = -0.344136f;
            m[1][2] = -0.714136f;
            m[1][3] = (0.344136f + 0.714136f) * uvOffset;

            m[2][0] = 1.0f;
            m[2][1] = 1.772f;
            m[2][3] = -1.772f * uvOffset;
        }
        else
        {
            m[0][0] = 1.0f;
            m[0][2] = 1.402f;
            m[0][3] = -uvOffset * 1.402f;

            m[1][0] = 1.0f;
            m[1][1] = -0.344136f;
            m[1][2] = -0.714136f;
            m[1][3] = uvOffset * (0.344136f + 0.714136f);

            m[2][0] = 1.0f;
            m[2][1] = 1.772f;
            m[2][3] = -uvOffset * 1.772f;
        }
        break;
    }
    case avxColorMatrix_YUV709:
    {
        if (fullRange)
        {
            m[0][0] = 1.0f;
            m[0][2] = 1.5748f;
            m[0][3] = -1.5748f * uvOffset;

            m[1][0] = 1.0f;
            m[1][1] = -0.187324f;
            m[1][2] = -0.468124f;
            m[1][3] = (0.187324f + 0.468124f) * uvOffset;

            m[2][0] = 1.0f;
            m[2][1] = 1.8556f;
            m[2][3] = -1.8556f * uvOffset;
        }
        else
        {
            m[0][0] = 1.0f;
            m[0][2] = 1.5748f;
            m[0][3] = -uvOffset * 1.5748f;

            m[1][0] = 1.0f;
            m[1][1] = -0.187324f;
            m[1][2] = -0.468124f;
            m[1][3] = uvOffset * (0.187324f + 0.468124f);

            m[2][0] = 1.0f;
            m[2][1] = 1.8556f;
            m[2][3] = -uvOffset * 1.8556f;
        }
        break;
    }
    case avxColorMatrix_YUV2020:
    {
        if (fullRange)
        {
            m[0][0] = 1.0f;
            m[0][2] = 1.4746f;
            m[0][3] = -1.4746f * uvOffset;

            m[1][0] = 1.0f;
            m[1][1] = -0.16455f;
            m[1][2] = -0.57135f;
            m[1][3] = (0.16455f + 0.57135f) * uvOffset;

            m[2][0] = 1.0f;
            m[2][1] = 1.8814f;
            m[2][3] = -1.8814f * uvOffset;
        }
        else
        {
            m[0][0] = 1.0f;
            m[0][2] = 1.4746f;
            m[0][3] = -uvOffset * 1.4746f;

            m[1][0] = 1.0f;
            m[1][1] = -0.16455f;
            m[1][2] = -0.57135f;
            m[1][3] = uvOffset * (0.16455f + 0.57135f);

            m[2][0] = 1.0f;
            m[2][1] = 1.8814f;
            m[2][3] = -uvOffset * 1.8814f;
        }
        break;
    }
    default:
    {
        // Default to identity if unknown
        break;
    }
    }
}

_AVX void AvxComputeColorConversionMatrix(avxColorMatrix from, avxColorMatrix to, afxBool fullRange, afxM4d m)
{
    /*
        AvxComputeColorConversionMatrix(from, to, m) is an abstraction over individual model matrices, 
        letting you convert between any two color models, including RGB <> YUV, and YUV709 <> YUV2020, etc.

        It is a more general-purpose function than AvxGetColorMatrix(). Instead of just returning a matrix to convert from YUV to RGB, 
        it returns a 4×4 matrix that converts from any color matrix model to another.
    */

    afxM4d m_from;
    afxM4d m_to;
    afxM4d m_to_inv;

    AvxGetColorMatrix(from, 0.0, 128.0, fullRange, m_from); // from -> RGB
    AvxGetColorMatrix(to, 0.0, 128.0, fullRange, m_to); // to -> RGB
    AfxM4dInvert(m_to_inv, m_to); // RGB -> to
    AfxM4dMultiply(m, m_to_inv, m_from); // from -> RGB -> to
}

_AVX afxDrawSystem AvxGetSamplerHost(avxSampler samp)
{
    afxError err = { 0 };
    // @samp must be a valid avxSampler handle.
    AFX_ASSERT_OBJECTS(afxFcc_SAMP, 1, &samp);
    afxDrawSystem dsys = AfxGetHost(samp);
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    return dsys;
}

_AVX afxFlags AvxGetSamplerFlags(avxSampler samp, afxFlags mask)
{
    afxError err = { 0 };
    // @samp must be a valid avxSampler handle.
    AFX_ASSERT_OBJECTS(afxFcc_SAMP, 1, &samp);
    return (!mask) ? samp->flags : (samp->flags & mask);
}

_AVX afxBool AvxIsSamplerYuvCapable(avxSampler samp)
{
    afxError err = { 0 };
    // samp must be a valid avxSampler handle.
    AFX_ASSERT_OBJECTS(afxFcc_SAMP, 1, &samp);
    return samp->cfg.isYuv;
}

_AVX void AvxGetSamplerBorderColor(avxSampler samp, avxClearValue* val)
{
    afxError err = { 0 };
    // samp must be a valid avxSampler handle.
    AFX_ASSERT_OBJECTS(afxFcc_SAMP, 1, &samp);
    // val must be a valid pointer to a valid avxClearValue structure.
    AFX_ASSERT(val);
    *val = samp->cfg.borderColor;
}

_AVX void AvxDescribeSampler(avxSampler samp, avxSamplerConfig* desc)
{
    afxError err = { 0 };
    // samp must be a valid avxSampler handle.
    AFX_ASSERT_OBJECTS(afxFcc_SAMP, 1, &samp);
    // desc must be a valid pointer to a valid avxSamplerConfig structure.
    AFX_ASSERT(desc);
    *desc = samp->cfg;
}

_AVX afxError _AvxSampDtorCb(avxSampler samp)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SAMP, 1, &samp);

    return err;
}

_AVX afxError _AvxSampCtorCb(avxSampler samp, void** args, afxUnit invokeNo)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SAMP, 1, &samp);

    afxDrawSystem dsys = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    avxSamplerConfig const *cfg = ((avxSamplerConfig const *)args[1]) + invokeNo;
    
    AFX_ASSERT(cfg);
    samp->tag = cfg->tag;
    samp->udd = cfg->udd;
    samp->cfg = *cfg;
    samp->crc = 0;
    //AfxAccumulateCrc32(&samp->crc, cfg, yuv ? sizeof(avxYuvSamplerInfo) : sizeof(avxSamplerConfig));

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
        samp->cfg.anisotropyMaxDegree = AFX_CLAMP(samp->cfg.anisotropyMaxDegree, 1.0, limits->maxSamplerAnisotropy);
    }
    else samp->cfg.anisotropyMaxDegree = 1.0f;

    // The absolute value of lodBias must be less than or equal to maxSamplerLodBias.
    AFX_ASSERT(limits->maxSamplerLodBias >= samp->cfg.lodBias);
    samp->cfg.lodBias = AFX_MIN(samp->cfg.lodBias, limits->maxSamplerLodBias);

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
    .desc = "Texture Sampler State Object",
    .fixedSiz = sizeof(AFX_OBJECT(avxSampler)),
    .ctor = (void*)_AvxSampCtorCb,
    .dtor = (void*)_AvxSampDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AVX afxError AvxConfigureSampler(afxDrawSystem dsys, avxSamplerConfig* cfg)
{
    afxError err = { 0 };
    // dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);

    afxDrawLimits const* limits = _AvxDsysAccessLimits(dsys);
    afxDrawFeatures const* features = _AvxDsysAccessReqFeatures(dsys);
    AFX_ASSERT(limits);

    if (features->samplerFilterMinMax == FALSE)
    {
        // When samplerFilterMinmax feature is not enabled, reduction mode must be WEIGHTED_AVERAGE.
        cfg->reductionMode = avxTexelReduction_AVG;
    }

    if (features->samplerMirrorClampToEdge == FALSE)
    {
        // When samplerMirrorClampToEdge feature is not enabled, UVW addressing modes must not be REFLECT.
        if (cfg->uvw[0] == avxTexelWrap_REFLECT) cfg->uvw[0] = avxTexelWrap_EDGE;
        if (cfg->uvw[1] == avxTexelWrap_REFLECT) cfg->uvw[1] = avxTexelWrap_EDGE;
        if (cfg->uvw[2] == avxTexelWrap_REFLECT) cfg->uvw[2] = avxTexelWrap_EDGE;
    }

    // maxLod must be greater than or equal to minLod.
    cfg->maxLod = AFX_MAX(cfg->maxLod, cfg->minLod);

    if (cfg->compareEnabled)
    {
        // When comparision is enabled, compareOp must be a valid avxCompareOp value.
        cfg->compareOp = AFX_CLAMP(cfg->compareOp, avxCompareOp_ALWAYS, avxCompareOp_TOTAL - 1);

        // When comparision is enabled, the reduction mode must be WEIGHTED_AVERAGE.
        cfg->reductionMode = avxTexelReduction_AVG;

    }

    if (cfg->unnormalizedCoords)
    {
        // When unnormalizedCoordinates is enabled, minification and magnification filters must be equal.
        cfg->minify = cfg->magnify;
        // When unnormalizedCoordinates is enabled, mipmap filter must be NEAREST.
        cfg->mipFlt = avxTexelFilter_NEAREST;
        // When unnormalizedCoordinates is enabled, minLod and maxLod must be zero.
        cfg->minLod = 0;
        cfg->maxLod = 0;
        // When unnormalizedCoordinates is enabled, UVW addressing modes must each be either EDGE or BORDER.
        cfg->uvw[0] = AFX_CLAMP(cfg->uvw[0], avxTexelWrap_EDGE, avxTexelWrap_BORDER);
        cfg->uvw[1] = AFX_CLAMP(cfg->uvw[1], avxTexelWrap_EDGE, avxTexelWrap_BORDER);
        cfg->uvw[2] = AFX_CLAMP(cfg->uvw[2], avxTexelWrap_EDGE, avxTexelWrap_BORDER);
        // When unnormalizedCoordinates is enabled, anisotropy must be disabled.
        cfg->anisotropyEnabled = FALSE;
        // When unnormalizedCoordinates is enabled, comparison must be disabled.
        cfg->compareEnabled = FALSE;
    }

    if (cfg->isYuv)
    {
        // When sampler YUV conversion is enabled, UVW addressing mode must be EDGE.
        cfg->uvw[0] = avxTexelWrap_EDGE;
        cfg->uvw[1] = avxTexelWrap_EDGE;
        cfg->uvw[2] = avxTexelWrap_EDGE;

        // When sampler YUV conversion is enabled, anisotropy must NOT be enabled.
        cfg->anisotropyEnabled = FALSE;

        // When sampler YUV conversion is enabled, unnormalizedCoordinates must NOT be enabled.
        cfg->unnormalizedCoords = FALSE;

        // When sampler YUV conversion is enabled, reduction mode must be WEIGHTED_AVERAGE.
        cfg->reductionMode = AFX_CLAMP(cfg->reductionMode, avxTexelReduction_AVG, avxTexelReduction_TOTAL - 1);

        // chromaFilter must be a valid avxTexelFilter value.
        cfg->chromaFilter = AFX_CLAMP(cfg->chromaFilter, avxTexelFilter_NEAREST, avxTexelFilter_TOTAL - 1);

        // components must be a valid avxSwizzling structure.
        cfg->yuvSizzle.r = AFX_CLAMP(cfg->yuvSizzle.r, avxColorSwizzle_R, avxColorSwizzle_TOTAL - 1);
        cfg->yuvSizzle.g = AFX_CLAMP(cfg->yuvSizzle.g, avxColorSwizzle_R, avxColorSwizzle_TOTAL - 1);
        cfg->yuvSizzle.b = AFX_CLAMP(cfg->yuvSizzle.b, avxColorSwizzle_R, avxColorSwizzle_TOTAL - 1);
        cfg->yuvSizzle.a = AFX_CLAMP(cfg->yuvSizzle.a, avxColorSwizzle_R, avxColorSwizzle_TOTAL - 1);

        // convMtx must be a valid avxColorMatrix value.
        cfg->yuvMtx = AFX_CLAMP(cfg->yuvMtx, avxColorMatrix_RGB, avxColorMatrix_TOTAL);

        // format must be a valid avxFormat value.
        cfg->yuvFmt = AFX_CLAMP(cfg->yuvFmt, avxFormat_UNDEFINED + 1, avxFormat_TOTAL - 1);
    }

    if (cfg->anisotropyEnabled)
        cfg->anisotropyMaxDegree = AFX_MAX(1.f, AFX_MIN(cfg->anisotropyMaxDegree, limits->maxSamplerAnisotropy));
    else
        cfg->anisotropyMaxDegree = 1.f;

    cfg->lodBias = AFX_MIN(cfg->lodBias, limits->maxSamplerLodBias);

    return err;
}

_AVX afxError AvxAcquireSamplers(afxDrawSystem dsys, afxUnit cnt, avxSamplerConfig const cfg[], avxSampler samplers[])
{
    afxError err = { 0 };
    // dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    AFX_ASSERT(cnt);
    // cfg must be a valid pointer to a valid avxSamplerConfig structure.
    AFX_ASSERT(cfg);
    // samplers must be a valid pointer to a avxSampler handle.
    AFX_ASSERT(samplers);

    // dsys must support at least one bridge with one of the GRAPHICS or COMPUTE capabilities.    
    afxDrawBridge dexu;
    afxBool bridgedFound = AvxChooseDrawBridges(dsys, AFX_INVALID_INDEX, avxAptitude_GFX | avxAptitude_PCX, NIL, 0, 1, &dexu);
    AFX_ASSERT(bridgedFound);

    afxClass* cls = (afxClass*)_AvxDsysGetDdi(dsys)->sampCls(dsys);
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
    afxError err = { 0 };
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
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SAMP, 1, &samp);

    if (samp->crc == udd->crc)
    {
        udd->sampler = samp;
        return FALSE; // break
    }
    return TRUE; // continue
};

_AVX afxBool AvxFindSamplers(afxDrawSystem dsys, afxUnit cnt, avxSamplerConfig const cfg[], avxSampler samplers[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    AFX_ASSERT(samplers);
    AFX_ASSERT(cfg);
    AFX_ASSERT(cnt);
    afxBool rslt = 0;
    return rslt;
}
