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

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>
// This software is part of Advanced Video Graphics Extensions & Experiments.

#define _AVX_DRAW_C
#define _AVX_RASTER_C
#include "avxIcd.h"

 // 1d               =   1 x w11 >> lod
 // 1d array         = img x w11 >> lod
 // 2d               =   1 x wh1 >> lod
 // 2d array         = img x wh1
 // cube             =   1 x wh1 >> lod
 // cube array       = img x wh1 >> lod
 // 3d               =   1 x whd >> lod

_AVXINL afxDrawSystem AvxGetRasterHost(avxRaster ras)
{
    afxError err = { 0 };
    // @ras must be a valid avxRaster handle.
    AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &ras);
    afxDrawSystem dsys = AfxGetHost(ras);
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    return dsys;
}

_AVXINL avxFormat AvxGetRasterFormat(avxRaster ras)
{
#if AVX_VALIDATION_ENABLED
    afxError err = { 0 };
    // @ras must be a valid avxRaster handle.
    AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &ras);
#endif
    return ras->fmt;
}

_AVXINL avxFormat AvxDescribeRasterFormat(avxRaster ras, avxFormatDescription* pfd)
{
    afxError err = { 0 };
#if AVX_VALIDATION_ENABLED
    // @ras must be a valid avxRaster handle.
    AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &ras);
    AFX_ASSERT_S(pfd, "must be a valid pointer to a avxFormatDescription structure.");
#endif//AVX_VALIDATION_ENABLED

    // This function is a utility to ease introdution of device-dependent formats.
    // Give the multi-device nature of Qwadro, it takes a index of a bridge established in the provider system.

    avxFormat fmt = ras->fmt;
    avxFormatDescription desc;
    AvxDescribeFormats(1, &fmt, &desc);
    *pfd = desc;
    return fmt;
}

_AVXINL avxRasterUsage AvxGetRasterUsage(avxRaster ras, avxRasterUsage mask)
{
#if AVX_VALIDATION_ENABLED
    afxError err = { 0 };
    // @ras must be a valid avxRaster handle.
    AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &ras);
#endif//AVX_VALIDATION_ENABLED
    return (!mask) ? ras->usage : (ras->usage & mask);
}

_AVXINL avxRasterFlags AvxGetRasterFlags(avxRaster ras, avxRasterFlags mask)
{
#if AVX_VALIDATION_ENABLED
    afxError err = { 0 };
    // @ras must be a valid avxRaster handle.
    AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &ras);
#endif//AVX_VALIDATION_ENABLED
    return (!mask) ? ras->flags : (ras->flags & mask);
}

_AVXINL void AvxGetRasterSwizzling(avxRaster ras, avxSwizzling* csw)
{
#if AVX_VALIDATION_ENABLED
    afxError err = { 0 };
    // @ras must be a valid avxRaster handle.
    AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &ras);
    AFX_ASSERT(csw);
#endif//AVX_VALIDATION_ENABLED
    *csw = ras->swizzling;
}

_AVXINL avxRange AvxGetRasterExtent(avxRaster ras, afxUnit lodIdx)
{
    afxError err = { 0 };
    // @ras must be a valid avxRaster handle.
    AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &ras);
    AFX_ASSERT_RANGE(ras->mipCnt, lodIdx, 1);

    afxBool is3d = ras->flags & avxRasterFlag_3D;

    avxRange whd = ras->whd;
    while (lodIdx--)
    {
        whd.w = whd.w >> 1;
        whd.h = whd.h >> 1;
        if (!is3d) whd.d = whd.d >> 1;
    }
    return whd;
}

_AVXINL void AvxDescribeRaster(avxRaster ras, avxRasterInfo* desc, avxSubrasterInfo* sub, avxExorasterInfo* exo)
{
#if AVX_VALIDATION_ENABLED
    afxError err = { 0 };
    // @ras must be a valid avxRaster handle.
    AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &ras);
    AFX_ASSERT(desc);
#endif//AVX_VALIDATION_ENABLED
    desc->lodCnt = ((ras->spp > 1) ? (ras->spp >> 1) : ras->mipCnt);
    desc->whd = AvxGetRasterExtent(ras, 0);
    desc->fmt = ras->fmt;
    desc->flags = ras->flags;
    desc->usage = ras->usage;

    desc->exuMask = 0;
    desc->udd = ras->udd;
    desc->tag = ras->tag;

    if (sub)
    {
        *sub = (avxSubrasterInfo)
        {
            .ras = ras->base,
            .baseLayer = ras->baseLayer,
            .baseLod = ras->baseMip,
            .swizzle = ras->swizzling
        };
    }

    if (exo)
    {
        *exo = (avxExorasterInfo)
        {
            0
        };
    }
}

_AVXINL void _AvxSanitizeRasterRegion(avxRaster ras, afxUnit cnt, avxRasterRegion const raw[], avxRasterRegion san[])
{
    afxError err = { 0 };
#if AVX_VALIDATION_ENABLED
    // @ras must be a valid avxRaster handle.
    AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &ras);
    AFX_ASSERT(raw);
    AFX_ASSERT(san);
    AFX_ASSERT(cnt);
#endif//AVX_VALIDATION_ENABLED

    /*
        This code sanitizes a avxRasterRegion struct to be within the bounds of a raster object.
    */
    
    for (afxUnit i = 0; i < cnt; i++)
    {
        avxRasterRegion* s = &san[i];
        avxRasterRegion const* r = &raw[i];
        s->lodIdx = AFX_MIN(r->lodIdx, ras->mipCnt - 1);
        avxRange whd = AvxGetRasterExtent(ras, s->lodIdx);
        s->origin.x = AFX_MIN(r->origin.x, (afxInt)(whd.w - 1));
        s->origin.y = AFX_MIN(r->origin.y, (afxInt)(whd.h - 1));
        s->origin.z = AFX_MIN(r->origin.z, (afxInt)(whd.d - 1));
        s->whd.w = AFX_CLAMP(r->whd.w, 1, whd.w - s->origin.x);
        s->whd.h = AFX_CLAMP(r->whd.h, 1, whd.h - s->origin.y);
        s->whd.d = AFX_CLAMP(r->whd.d, 1, whd.d - s->origin.z);

        // whd.w MUST not be 0.
        // whd.h MUST not be 0.
        // whd.d MUST not be 0.
        AFX_ASSERT(s->whd.w);
        AFX_ASSERT(s->whd.h);
        AFX_ASSERT(s->whd.d);
    }
}

_AVXINL void _AvxSanitizeRasterCopy(avxRaster ras, avxRaster src, afxUnit cnt, avxRasterCopy const raw[], avxRasterCopy san[])
{
    afxError err = { 0 };
#if AVX_VALIDATION_ENABLED
    // src must be a valid avxRaster handle.
    AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &src);
    // ras must be a valid avxRaster handle.
    AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &ras);
    AFX_ASSERT(raw);
    AFX_ASSERT(san);
    AFX_ASSERT(cnt);
#endif//AVX_VALIDATION_ENABLED

    /*
        This code sanitizes a avxRasterCopy struct to be within the bounds of two raster objects.
    */

    afxUnit srcLodCnt = src->mipCnt;
    afxUnit dstLodCnt = ras->mipCnt;

    for (afxUnit i = 0; i < cnt; i++)
    {
        avxRasterCopy* s = &san[i];
        avxRasterCopy const* r = &raw[i];
        s->src.lodIdx = AFX_MIN(r->src.lodIdx, srcLodCnt - 1);
        avxRange srcExtent = AvxGetRasterExtent(src, s->src.lodIdx);
        s->src.origin.x = AFX_MIN(r->src.origin.x, (afxInt)(srcExtent.w - 1));
        s->src.origin.y = AFX_MIN(r->src.origin.y, (afxInt)(srcExtent.h - 1));
        s->src.origin.z = AFX_MIN(r->src.origin.z, (afxInt)(srcExtent.d - 1));

        s->dstLodIdx = AFX_MIN(r->dstLodIdx, dstLodCnt - 1);
        avxRange dstExtent = AvxGetRasterExtent(ras, s->dstLodIdx);
        s->dstOrigin.x = AFX_MIN(r->dstOrigin.x, (afxInt)(dstExtent.w - 1));
        s->dstOrigin.y = AFX_MIN(r->dstOrigin.y, (afxInt)(dstExtent.h - 1));
        s->dstOrigin.z = AFX_MIN(r->dstOrigin.z, (afxInt)(dstExtent.d - 1));

        s->src.whd.w = AFX_CLAMP(r->src.whd.w, 1, AFX_MIN((srcExtent.w - s->src.origin.x), (dstExtent.w - s->dstOrigin.x)));
        s->src.whd.h = AFX_CLAMP(r->src.whd.h, 1, AFX_MIN((srcExtent.h - s->src.origin.y), (dstExtent.h - s->dstOrigin.y)));
        s->src.whd.d = AFX_CLAMP(r->src.whd.d, 1, AFX_MIN((srcExtent.d - s->src.origin.z), (dstExtent.d - s->dstOrigin.z)));
    }
}

_AVX afxBool AvxQueryRasterArrangement(avxRaster ras, avxRasterRegion const* rgn, avxRasterArrangement* layout)
{
    afxError err = { 0 };
    // @ras must be a valid avxRaster handle.
    AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &ras);

    afxUnit spp = ras->spp;
    afxUnit mipCnt = ras->mipCnt;
    afxUnit fmtStride = ras->fmtStride;
    afxUnit align = ras->reqAlign;
    if (!align) align = AVX_RASTER_ALIGNMENT;

    if (!rgn)
    {
        afxUnit size = ras->reqSiz;

        if (!size)
        {
            afxWarp whd = { ras->whd.w, ras->whd.h, ras->whd.d };
            afxBool is3d = !!AvxGetRasterFlags(ras, avxRasterFlag_3D);

            for (afxUnit i = 0; i < mipCnt; i++)
            {
                afxUnit rowStride = AFX_ALIGN_SIZE(whd.w * fmtStride * spp, AFX_SIMD_ALIGNMENT);
                afxUnit imgStride = AFX_ALIGN_SIZE(whd.h * rowStride, AFX_SIMD_ALIGNMENT);
                size += whd.d * imgStride;

                whd.w = AFX_MAX(1, whd.w >> 1);
                whd.h = AFX_MAX(1, whd.h >> 1);
                if (is3d) whd.d = AFX_MAX(1, whd.d >> 1);
            }

            size = AFX_ALIGN_SIZE(size, align);
        }

        AFX_ASSERT(layout);
        layout->offset = ras->storageOffset;
        layout->size = size;
        layout->align = align;
        afxUnit rowStride = AFX_ALIGN_SIZE(ras->whd.w * spp, align);
        layout->rowStride = rowStride;
        layout->fmtStride = fmtStride;

        if ((ras->flags & avxRasterFlag_LAYER))
            layout->imgStride = AFX_ALIGN_SIZE(ras->whd.h * rowStride, align);
        else
            layout->imgStride = ras->whd.h * rowStride;

        return !err;
    }

    AFX_ASSERT_RANGE(ras->mipCnt, rgn->lodIdx, 1);
    AFX_ASSERT_RANGE(ras->whd.d, rgn->origin.z, rgn->whd.d);

    avxFormatDescription pfd;
    AvxDescribeFormats(1, &ras->fmt, &pfd);

    afxBool is3d = AvxGetRasterFlags(ras, avxRasterFlag_3D);
    AFX_ASSERT(!rgn->origin.z || !is3d);

    afxUnit targetMipLevel = rgn->lodIdx;
    afxUnit baseArrayLayer = (is3d) ? 0 : rgn->origin.z;
    afxUnit layerCount = (is3d) ? 1 : rgn->whd.d;

    afxUnit imageWidth = ras->whd.w;
    afxUnit imageHeight = ras->whd.h;
    afxUnit imageDepth = (is3d) ? ras->whd.d : 1;
    afxSize offset = 0;

    // Accumulate all previous mip levels (for all layers).
    for (afxUnit mip = 0; mip < targetMipLevel; ++mip)
    {
        afxUnit mipW = AFX_MAX(1, imageWidth >> mip);
        afxUnit mipH = AFX_MAX(1, imageHeight >> mip);
        afxUnit mipD = ((is3d) ? AFX_MAX(1, imageDepth >> mip) : 1);

        // TODO: texel blocks

        afxUnit rowPitch = AFX_ALIGN_SIZE(mipW * fmtStride * spp, ras->reqAlign);
        afxUnit depthPitch = rowPitch * mipH;
        afxUnit mipSize = depthPitch * mipD;

        offset += mipSize * ((is3d) ? 1 : imageDepth); // uses total number of layers in image
    }

    // Now compute dimensions for target mip level.
    afxUnit mipW = AFX_MAX(1, imageWidth >> targetMipLevel);
    afxUnit mipH = AFX_MAX(1, imageHeight >> targetMipLevel);
    afxUnit mipD = ((is3d) ? AFX_MAX(1, imageDepth >> targetMipLevel) : 1);

    // TODO: texel blocks

    afxUnit rowPitch = AFX_ALIGN_SIZE(mipW * fmtStride * spp, ras->reqAlign);
    afxUnit depthPitch = rowPitch * mipH;
    afxUnit arrayPitch = depthPitch * ((is3d) ? 1 : mipD);

    // Accumulate offset from previous array layers (up to baseArrayLayer).
    offset += baseArrayLayer * arrayPitch;

    // Add offset inside selected region.
    offset += rgn->origin.z * ((is3d) ? depthPitch : 1);
    offset += rgn->origin.y * rowPitch;
    offset += rgn->origin.x * pfd.stride;

    // Compute size across all layers selected.
    // For each layer: depthPitch * (depth - 1) + rowPitch * (height - 1) + width * texelSize.
    afxSize singleLayerSize =
        (rgn->whd.d - 1) * depthPitch +
        (rgn->whd.h - 1) * rowPitch +
        rgn->whd.w * pfd.stride;

    afxSize totalSize = singleLayerSize * layerCount;

    AFX_ASSERT(layout);
    layout->imgStride = (is3d) ? depthPitch : arrayPitch;
    layout->rowStride = rowPitch;
    layout->offset = offset + ras->storageOffset;
    layout->size = totalSize;
    return !err;
}

_AVX afxError _AvxRasDtorCb(avxRaster ras)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &ras);

    if (ras->base)
    {
        AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &ras->base);
        AfxDisposeObjects(1, &ras->base);
    }

    afxDrawSystem dsys = AvxGetRasterHost(ras);
    if (_AvxDsysGetDdi(dsys)->deallocRasCb(dsys, 1, &ras))
    {
        AfxThrowError();
    }

    return err;
}

_AVX afxError _AvxRasCtorCb(avxRaster ras, void** args, afxUnit invokeNo)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &ras);

    afxDrawSystem dsys = AvxGetRasterHost(ras);
    avxRasterInfo const* rasi = args[1] ? ((avxRasterInfo const*)args[1]) + invokeNo : NIL;
    avxSubrasterInfo const* subi = args[2] ? ((avxSubrasterInfo const*)args[2]) + invokeNo : NIL;
    avxExorasterInfo const* exorasi = args[3] ? ((avxExorasterInfo const*)args[3]) + invokeNo : NIL;
    
    if (subi)
    {
        ras->udd = subi->udd;
        ras->tag = subi->tag;

        avxRaster base = subi->ras;

        if (!base)
        {
            AFX_ASSERT(subi->ras);
            AfxThrowError();
            return err;
        }
        else if (subi->ras->base && (subi->ras->base != subi->ras))
        {
            // Base raster must be a storage raster, not a raster view.
            AFX_ASSERT(!subi->ras->base);
            AfxThrowError();
            return err;
        }

        ras->base = base;
        AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &base);

        if (!subi->fmt) ras->fmt = base->fmt;
        else
        {
            if (!AvxTestCompatibleFormats(base->fmt, subi->fmt))
            {
                // must have a specified format.
                AfxThrowError();
                err = afxError_INVALID;
                return err;
            }
            ras->fmt = subi->fmt;
        }
        // Stride should be always equal in compatible formats.
        ras->fmtStride = base->fmtStride;

        ras->baseMip = AFX_MIN(subi->baseLod, base->mipCnt - 1);
        ras->mipCnt = AFX_MAX(1, AFX_MIN(subi->lodCnt, base->mipCnt - ras->baseMip));
        ras->baseLayer = AFX_MIN(subi->baseLayer, base->whd.d - 1);
        ras->whd = AVX_RANGE(base->whd.w, base->whd.h, AFX_MAX(1, AFX_MIN(subi->layerCnt, base->whd.d - ras->baseLayer)));

        ras->swizzling.r = subi->swizzle.r;
        ras->swizzling.g = subi->swizzle.g;
        ras->swizzling.b = subi->swizzle.b;
        ras->swizzling.a = subi->swizzle.a;

        ras->flags = base->flags | subi->flags;
        ras->usage = base->usage;

        ras->storage[0] = base->storage[0];
        ras->storageOffset = base->storageOffset;

        ras->sub = TRUE;

        AfxReacquireObjects(1, &base);

        return err;
    }

    ras->udd = rasi->udd;
    ras->tag = rasi->tag;

    // FLAGS
    avxRasterFlags const mipAndMs = (avxRasterFlag_MIP | avxRasterFlag_MULTISAMP);
    avxRasterFlags flags = (rasi->flags & mipAndMs);
    ras->flags = NIL;

    if (flags == mipAndMs)
    {
        // can not be mipmapped and multisampled at same time.
        AfxThrowError();
        return err;
    }
    else if ((flags == NIL) && (ras->mipCnt > 1))
    {
        // If not specified, default to mipmap
        ras->flags |= avxRasterFlag_MIP;
    }
    else
    {
        ras->flags |= (flags & mipAndMs);
    }

    flags = (rasi->flags & (avxRasterFlag_3D | avxRasterFlag_CUBEMAP | avxRasterFlag_LAYER));

    if ((flags == (avxRasterFlag_3D | avxRasterFlag_LAYER)) &&
        (flags == (avxRasterFlag_3D | avxRasterFlag_CUBEMAP)))
    {
        // can not be volumetric (3D) and layered or cubemap at same time.
        AfxThrowError();
        return err;
    }
    else if ((flags == NIL) && (ras->whd.d > 1))
    {
        // If not specified, default to layered.
        ras->flags |= avxRasterFlag_LAYER;
    }
    else
    {
        ras->flags |= (flags & (avxRasterFlag_3D | avxRasterFlag_CUBEMAP | avxRasterFlag_LAYER));

        if (flags & avxRasterFlag_CUBEMAP)
        {
            // if cubemap, layers must be a multiple of 6.

            if (ras->whd.d % 6)
            {
                AfxThrowError();
                return err;
            }
        }
    }

    flags = (rasi->flags & (avxRasterFlag_1D | avxRasterFlag_2D | avxRasterFlag_3D));

    if ((flags == (avxRasterFlag_2D | avxRasterFlag_1D)) ||
        (flags == (avxRasterFlag_2D | avxRasterFlag_3D)) ||
        (flags == (avxRasterFlag_3D | avxRasterFlag_1D)))
    {
        // Can not be both at same time.
        AfxThrowError();
        return err;
    }
    else if (flags == NIL)
    {
        // default to 2D if nothing is specified.
        ras->flags |= avxRasterFlag_2D;
        // default to layered if depth is present. See code above.
    }
    else
    {
        ras->flags |= flags;
    }

    ras->base = NIL;
    ras->baseMip = 0;
    ras->mipCnt = (ras->flags & avxRasterFlag_MIP) ? AFX_MAX(rasi->lodCnt, 1) : 1;
    ras->baseLayer = 0;
    ras->swizzling.r = avxColorSwizzle_R;
    ras->swizzling.g = avxColorSwizzle_G;
    ras->swizzling.b = avxColorSwizzle_B;
    ras->swizzling.a = avxColorSwizzle_A;

    if (!rasi->fmt)
    {
        // must have a specified format.
        AfxThrowError();
        return err;
    }
    avxFormat fmt = rasi->fmt;
    avxFormatDescription pfd;
    AvxDescribeFormats(1, &fmt, &pfd);
    ras->fmt = fmt;
    ras->fmtStride = pfd.stride;

    ras->usage = rasi->usage;

    ras->whd = AvxMaxRange(AVX_RANGE(AFX_MAX(1, pfd.bcWh[0]), AFX_MAX(1, pfd.bcWh[1]), 1), rasi->whd);


    // STORAGE

    {
        avxRasterArrangement lay;
        AvxQueryRasterArrangement(ras, NIL, &lay);

        ras->reqSiz = lay.size;
        ras->reqAlign = lay.align;
        ras->reqMemType = NIL;

        ras->storageOffset = 0;
        ras->storage[0].mmu = AFX_INVALID_INDEX;
        ras->storage[0].host.addr = NIL;
    }
    return err;
}

_AVX afxClassConfig const _AVX_CLASS_CONFIG_RAS =
{
    .fcc = afxFcc_RAS,
    .name = "Raster",
    .desc = "Formatted Video Buffer",
    .fixedSiz = sizeof(AFX_OBJECT(avxRaster)),
    .ctor = (void*)_AvxRasCtorCb,
    .dtor = (void*)_AvxRasDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AVX afxError AvxAcquireRasters(afxDrawSystem dsys, afxUnit cnt, avxRasterInfo const info[], avxRaster rasters[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    AFX_ASSERT(rasters);
    AFX_ASSERT(info);
    AFX_ASSERT(cnt);

#if AVX_VALIDATION_ENABLED
    for (afxUnit i = 0; i < cnt; i++)
    {
        avxRasterFlags flags = info[i].flags;

        if ((flags & avxRasterFlag_CUBEMAP) && (info[i].whd.d % 6))
        {
            AFX_ASSERT(info[i].whd.d % 6 == 0);
            AfxThrowError();

            if ((info[i].whd.d / 6) && !(flags & avxRasterFlag_LAYER))
            {
                AFX_ASSERT_AND((info[i].whd.d / 6), (flags & avxRasterFlag_LAYER));
                AfxThrowError();
            }
        }

        if ((flags & avxRasterFlag_1D) && (info[i].whd.h > 1))
        {
            AFX_ASSERT(1 >= info[i].whd.h);
            AfxThrowError();

            if ((info[i].whd.d > 1) && !(flags & avxRasterFlag_LAYER))
            {
                AFX_ASSERT_OR(1 >= info[i].whd.d, flags & avxRasterFlag_LAYER);
                AfxThrowError();
            }
        }

        if ((flags & avxRasterFlag_2D) && (info[i].whd.d > 1) && !(flags & avxRasterFlag_LAYER))
        {
            AfxThrowError();

            if ((info[i].whd.d > 1) && !(flags & avxRasterFlag_LAYER))
            {
                AFX_ASSERT_OR(1 >= info[i].whd.d, flags & avxRasterFlag_LAYER);
                AfxThrowError();
            }
        }
    }
#endif//AVX_VALIDATION_ENABLED

    afxClass* cls = (afxClass*)_AvxDsysGetDdi(dsys)->rasCls(dsys);
    AFX_ASSERT_CLASS(cls, afxFcc_RAS);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)rasters, (void const*[]) { dsys, info, NIL, NIL }))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT_OBJECTS(afxFcc_RAS, cnt, rasters);

    if (_AvxDsysGetDdi(dsys)->allocRasCb(dsys, cnt, info, rasters))
    {
        AfxDisposeObjects(cnt, rasters);
        AfxThrowError();
        return err;
    }

#if AVX_VALIDATION_ENABLED
    for (afxUnit i = 0; i < cnt; i++)
    {
        avxRasterInfo rasi;
        AvxDescribeRaster(rasters[i], &rasi, NIL, NIL);

        AFX_ASSERT(rasi.lodCnt >= info[i].lodCnt);
        AFX_ASSERT(rasi.whd.w >= info[i].whd.w);
        AFX_ASSERT(rasi.whd.h >= info[i].whd.h);
        AFX_ASSERT(rasi.whd.d >= info[i].whd.d);
        AFX_ASSERT(rasi.exuMask == info[i].exuMask);
        AFX_ASSERT((rasi.flags & info[i].flags) == info[i].flags);
        AFX_ASSERT(rasi.fmt == info[i].fmt);
        AFX_ASSERT(rasi.tag.start == info[i].tag.start);
        AFX_ASSERT(rasi.udd == info[i].udd);
        AFX_ASSERT((rasi.usage & info[i].usage) == info[i].usage);
    }
#endif

    return err;
}

_AVX afxError AvxAcquireExternalRasters(afxDrawSystem dsys, afxUnit cnt, avxExorasterInfo const info[], avxRaster rasters[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    AFX_ASSERT(rasters);
    AFX_ASSERT(info);
    AFX_ASSERT(cnt);

#if AVX_VALIDATION_ENABLED
    for (afxUnit i = 0; i < cnt; i++)
    {
        avxRasterFlags flags = info[i].rasi.flags;

        if ((flags & avxRasterFlag_CUBEMAP) && (info[i].rasi.whd.d % 6))
        {
            AFX_ASSERT(info[i].rasi.whd.d % 6 == 0);
            AfxThrowError();

            if ((info[i].rasi.whd.d / 6) && !(flags & avxRasterFlag_LAYER))
            {
                AFX_ASSERT_AND((info[i].rasi.whd.d / 6), (flags & avxRasterFlag_LAYER));
                AfxThrowError();
            }
        }

        if ((flags & avxRasterFlag_1D) && (info[i].rasi.whd.h > 1))
        {
            AFX_ASSERT(1 >= info[i].rasi.whd.h);
            AfxThrowError();

            if ((info[i].rasi.whd.d > 1) && !(flags & avxRasterFlag_LAYER))
            {
                AFX_ASSERT_OR(1 >= info[i].rasi.whd.d, flags & avxRasterFlag_LAYER);
                AfxThrowError();
            }
        }

        if ((flags & avxRasterFlag_2D) && (info[i].rasi.whd.d > 1) && !(flags & avxRasterFlag_LAYER))
        {
            AfxThrowError();

            if ((info[i].rasi.whd.d > 1) && !(flags & avxRasterFlag_LAYER))
            {
                AFX_ASSERT_OR(1 >= info[i].rasi.whd.d, flags & avxRasterFlag_LAYER);
                AfxThrowError();
            }
        }
    }
#endif//AVX_VALIDATION_ENABLED

    afxClass* cls = (afxClass*)_AvxDsysGetDdi(dsys)->rasCls(dsys);
    AFX_ASSERT_CLASS(cls, afxFcc_RAS);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)rasters, (void const*[]) { dsys, info, NIL, info }))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT_OBJECTS(afxFcc_RAS, cnt, rasters);

    if (_AvxDsysGetDdi(dsys)->allocRasCb(dsys, cnt, info, rasters))
    {
        AfxDisposeObjects(cnt, rasters);
        AfxThrowError();
        return err;
    }

#if AVX_VALIDATION_ENABLED
    for (afxUnit i = 0; i < cnt; i++)
    {
        avxExorasterInfo exo;
        avxRasterInfo rasi;
        AvxDescribeRaster(rasters[i], &rasi, NIL, &exo);

        AFX_ASSERT(rasi.lodCnt >= info[i].rasi.lodCnt);
        AFX_ASSERT(rasi.whd.w >= info[i].rasi.whd.w);
        AFX_ASSERT(rasi.whd.h >= info[i].rasi.whd.h);
        AFX_ASSERT(rasi.whd.d >= info[i].rasi.whd.d);
        AFX_ASSERT(rasi.exuMask == info[i].rasi.exuMask);
        AFX_ASSERT((rasi.flags & info[i].rasi.flags) == info[i].rasi.flags);
        AFX_ASSERT(rasi.fmt == info[i].rasi.fmt);
        AFX_ASSERT(rasi.tag.start == info[i].rasi.tag.start);
        AFX_ASSERT(rasi.udd == info[i].rasi.udd);
        AFX_ASSERT((rasi.usage & info[i].rasi.usage) == info[i].rasi.usage);
    }
#endif

    return err;
}

_AVX afxError AvxReacquireRasters(afxDrawSystem dsys, afxUnit cnt, avxSubrasterInfo const info[], avxRaster rasters[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    AFX_ASSERT(rasters);
    AFX_ASSERT(info);
    AFX_ASSERT(cnt);

#if AVX_VALIDATION_ENABLED
    for (afxUnit i = 0; i < cnt; i++)
    {
        avxRasterFlags flags = info[i].flags;

        if (!info[i].ras)
        {
            AFX_ASSERT(info[i].ras);
            AfxThrowError();
        }
    }

    if (err) return err;
#endif//AVX_VALIDATION_ENABLED

    afxClass* cls = (afxClass*)_AvxDsysGetDdi(dsys)->rasCls(dsys);
    AFX_ASSERT_CLASS(cls, afxFcc_RAS);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)rasters, (void const*[]) { dsys, NIL, info, NIL }))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT_OBJECTS(afxFcc_RAS, cnt, rasters);

#if AVX_VALIDATION_ENABLED
    for (afxUnit i = 0; i < cnt; i++)
    {
        avxSubrasterInfo sub;
        avxRasterInfo rasi;
        AvxDescribeRaster(rasters[i], &rasi, &sub, NIL);

        AFX_ASSERT(sub.ras == info[i].ras);
        AFX_ASSERT(sub.lodCnt >= info[i].lodCnt);
        AFX_ASSERT(sub.baseLayer == info[i].baseLayer);
        AFX_ASSERT(sub.baseLod == info[i].baseLod);
        AFX_ASSERT((rasi.flags & info[i].flags) == info[i].flags);
        AFX_ASSERT(rasi.fmt == info[i].fmt);
        AFX_ASSERT(rasi.tag.start == info[i].tag.start);
        AFX_ASSERT(sub.swizzle.r == info[i].swizzle.r);
        AFX_ASSERT(sub.swizzle.g == info[i].swizzle.g);
        AFX_ASSERT(sub.swizzle.b == info[i].swizzle.b);
        AFX_ASSERT(sub.swizzle.a == info[i].swizzle.a);
        AFX_ASSERT(rasi.udd == info[i].udd);
    }
#endif

    return err;
}
