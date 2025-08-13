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
#define _AVX_RASTER_C
#include "../ddi/avxImplementation.h"

 // 1d               =   1 x w11 >> lod
 // 1d array         = img x w11 >> lod
 // 2d               =   1 x wh1 >> lod
 // 2d array         = img x wh1
 // cube             =   1 x wh1 >> lod
 // cube array       = img x wh1 >> lod
 // 3d               =   1 x whd >> lod

_AVXINL avxFormat AvxGetRasterFormat(avxRaster ras)
{
#if AVX_VALIDATION_ENABLED
    afxError err = AFX_ERR_NONE;
    // @ras must be a valid avxRaster handle.
    AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &ras);
#endif
    return ras->fmt;
}

_AVXINL avxFormat AvxDescribeRasterFormat(avxRaster ras, afxMask exuMask, avxFormatDescription* pfd)
{
    afxError err = AFX_ERR_NONE;
#if AVX_VALIDATION_ENABLED
    // @ras must be a valid avxRaster handle.
    AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &ras);
    AFX_ASSERT_S(pfd, "must be a valid pointer to a avxFormatDescription structure.");
#endif//AVX_VALIDATION_ENABLED

    // This function is a utility to ease introdution of device-dependent formats.
    // Give the multi-device nature of Qwadro, it takes a index of a bridge established in the provider system.

    afxDrawSystem dsys = AfxGetProvider(ras);
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    afxDrawBridge dexu;
    AvxGetDrawBridges(dsys, exuMask, 1, &dexu);
    AFX_ASSERT_OBJECTS(afxFcc_DEXU, 1, &dexu);
    afxDrawDevice ddev = AvxGetBridgedDrawDevice(dexu, NIL);
    AFX_ASSERT_OBJECTS(afxFcc_DDEV, 1, &ddev);

    avxFormat fmt = ras->fmt;
    avxFormatDescription desc;
    if (AvxDescribeDeviceFormats(ddev, 1, &fmt, &desc))
    {
        AfxThrowError();
        *pfd = (avxFormatDescription) { 0 };
        return NIL;
    }
    else
    {
        *pfd = desc;
        return fmt;
    }
}

_AVXINL avxRasterUsage AvxTestRasterUsage(avxRaster ras, avxRasterUsage mask)
{
#if AVX_VALIDATION_ENABLED
    afxError err = AFX_ERR_NONE;
    // @ras must be a valid avxRaster handle.
    AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &ras);
#endif//AVX_VALIDATION_ENABLED
    return (ras->usage & mask);
}

_AVXINL avxRasterFlags AvxTestRasterFlags(avxRaster ras, avxRasterFlags mask)
{
#if AVX_VALIDATION_ENABLED
    afxError err = AFX_ERR_NONE;
    // @ras must be a valid avxRaster handle.
    AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &ras);
#endif//AVX_VALIDATION_ENABLED
    return (ras->flags & mask);
}

_AVXINL void AvxGetRasterSwizzling(avxRaster ras, avxSwizzling* csw)
{
#if AVX_VALIDATION_ENABLED
    afxError err = AFX_ERR_NONE;
    // @ras must be a valid avxRaster handle.
    AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &ras);
    AFX_ASSERT(csw);
#endif//AVX_VALIDATION_ENABLED
    *csw = ras->swizzling;
}

_AVXINL avxRange AvxGetRasterExtent(avxRaster ras, afxUnit lodIdx)
{
#if AVX_VALIDATION_ENABLED
    afxError err = AFX_ERR_NONE;
    // @ras must be a valid avxRaster handle.
    AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &ras);
    AFX_ASSERT_RANGE(ras->lodCnt, lodIdx, 1);
#endif//AVX_VALIDATION_ENABLED

    avxRange whd = ras->whd;
    if (AvxTestRasterFlags(ras, avxRasterFlag_MULTISAMP))
    {
        while (lodIdx--)
        {
            whd.w = whd.w * 2;
            whd.h = whd.h * 2;
            whd.d = whd.d * 2;
        }
    }
    else
    {
        while (lodIdx--)
        {
            whd.w = whd.w >> 1;
            whd.h = whd.h >> 1;
            whd.d = whd.d >> 1;
        }
        whd = AvxMaxRange(AVX_RANGE(1, 1, 1), whd);
    }
    return whd;
}

_AVXINL void AvxDescribeRaster(avxRaster ras, avxRasterInfo* desc)
{
#if AVX_VALIDATION_ENABLED
    afxError err = AFX_ERR_NONE;
    // @ras must be a valid avxRaster handle.
    AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &ras);
    AFX_ASSERT(desc);
#endif//AVX_VALIDATION_ENABLED
    desc->lodCnt = ras->lodCnt;
    desc->whd = AvxGetRasterExtent(ras, 0);
    desc->fmt = ras->fmt;
    desc->flags = ras->flags;
    desc->usage = ras->usage;
    desc->base = ras->base;
    desc->baseLayer = ras->baseLayer;
    desc->baseLod = ras->baseLod;
    desc->exuMask = 0;
    desc->swizzle = ras->swizzling;
    desc->udd = ras->udd;
    desc->tag = ras->tag;
}

_AVXINL void _AvxSanitizeRasterRegion(avxRaster ras, afxUnit cnt, avxRasterRegion const raw[], avxRasterRegion san[])
{
#if AVX_VALIDATION_ENABLED
    afxError err = AFX_ERR_NONE;
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
        s->lodIdx = AFX_MIN(r->lodIdx, ras->lodCnt - 1);
        avxRange whd = AvxGetRasterExtent(ras, s->lodIdx);
        s->origin.x = AFX_MIN(r->origin.x, (afxInt)(whd.w - 1));
        s->origin.y = AFX_MIN(r->origin.y, (afxInt)(whd.h - 1));
        s->origin.z = AFX_MIN(r->origin.z, (afxInt)(whd.d - 1));
        s->whd.w = AFX_CLAMP(r->whd.w, 1, whd.w - s->origin.x);
        s->whd.h = AFX_CLAMP(r->whd.h, 1, whd.h - s->origin.y);
        s->whd.d = AFX_CLAMP(r->whd.d, 1, whd.d - s->origin.z);
    }
}

_AVXINL void _AvxSanitizeRasterCopy(avxRaster ras, avxRaster src, afxUnit cnt, avxRasterCopy const raw[], avxRasterCopy san[])
{
#if AVX_VALIDATION_ENABLED
    afxError err = AFX_ERR_NONE;
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

    afxUnit srcLodCnt = src->lodCnt;
    afxUnit dstLodCnt = ras->lodCnt;

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

_AVX afxBool AvxQueryRasterLayout(avxRaster ras, afxUnit lodIdx, afxUnit layerIdx, avxRasterLayout* layout)
{
    afxError err = AFX_ERR_NONE;
#if AVX_VALIDATION_ENABLED
    // @ras must be a valid avxRaster handle.
    AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &ras);
    AFX_ASSERT_RANGE(ras->lodCnt, lodIdx, 1);
    AFX_ASSERT_RANGE(ras->whd.d, layerIdx, 1);
#endif//AVX_VALIDATION_ENABLED

    afxBool is3d = AvxTestRasterFlags(ras, avxRasterFlag_3D);
    AFX_ASSERT(!layerIdx || !is3d);

    avxRange whd = AvxGetRasterExtent(ras, lodIdx);

    avxFormatDescription pfd;
    AvxDescribeFormats(1, &ras->fmt, &pfd);
    
    afxUnit rowStride = AFX_ALIGN_SIZE(pfd.stride * whd.w, AFX_SIMD_ALIGNMENT);
    afxUnit imgStride = whd.h * rowStride;
    AFX_ASSERT(layout);
    layout->imgStride = imgStride;
    layout->rowStride = rowStride;
    layout->offset = 0;
    layout->size = is3d ? whd.d * imgStride : whd.d * imgStride;
    return !err;
}

_AVX afxError _AvxRasDtorCb(avxRaster ras)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &ras);

    if (ras->base)
    {
        AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &ras->base);
        AfxDisposeObjects(1, &ras->base);
    }

    return err;
}

_AVX afxError _AvxRasCtorCb(avxRaster ras, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &ras);

    afxDrawSystem dsys = AfxGetProvider(ras);
    avxRasterInfo const* rasi = ((avxRasterInfo const*)args[1]) + invokeNo;

    if (rasi->base)
    {
        if (rasi->base->base && (rasi->base->base != rasi->base))
        {
            // Base raster must be a storage raster, not a raster view.
            AFX_ASSERT(!rasi->base->base);
            AfxThrowError();
            return err;
        }
    }

    ras->udd = rasi->udd;
    ras->tag = rasi->tag;
    ras->base = NIL;

    //ras->swizzling = NIL; //&AVX_COLOR_SWIZZLING_IDENTITY;

    ras->baseLod = 0;
    ras->lodCnt = AFX_MAX(rasi->lodCnt, 1);
    ras->baseLayer = 0;
    ras->whd = AvxMaxRange(AVX_RANGE(1, 1, 1), rasi->whd);

    ras->flags = NIL;
    ras->usage = rasi->usage;

    if (!rasi->fmt)
    {
        // must have a specified format.
        AfxThrowError();
        return err;
    }
    ras->fmt = rasi->fmt;

    ras->swizzling.r = rasi->swizzle.r;
    ras->swizzling.g = rasi->swizzle.g;
    ras->swizzling.b = rasi->swizzle.b;
    ras->swizzling.a = rasi->swizzle.a;


    // FLAGS

    avxRasterFlags const mipAndMs = (avxRasterFlag_MIPMAP | avxRasterFlag_MULTISAMP);
    avxRasterFlags flags = (rasi->flags & mipAndMs);

    if (flags == mipAndMs)
    {
        // can not be mipmapped and multisampled at same time.
        AfxThrowError();
        return err;
    }
    else if ((flags == NIL) && (ras->lodCnt > 1))
    {
        // If not specified, default to mipmap
        ras->flags |= avxRasterFlag_MIPMAP;
    }
    else
    {
        ras->flags |= (flags & mipAndMs);
    }
    
    flags = (rasi->flags & (avxRasterFlag_3D | avxRasterFlag_CUBEMAP | avxRasterFlag_LAYERED));

    if ((flags == (avxRasterFlag_3D | avxRasterFlag_LAYERED)) &&
        (flags == (avxRasterFlag_3D | avxRasterFlag_CUBEMAP)))
    {
        // can not be volumetric (3D) and layered or cubemap at same time.
        AfxThrowError();
        return err;
    }
    else if ((flags == NIL) && (ras->whd.d > 1))
    {
        // If not specified, default to layered.
        ras->flags |= avxRasterFlag_LAYERED;
    }
    else
    {
        ras->flags |= (flags & (avxRasterFlag_3D | avxRasterFlag_CUBEMAP | avxRasterFlag_LAYERED));

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

    // STORAGE

    {
        avxFormatDescription pfd;
        AvxDescribeFormats(1, &ras->fmt, &pfd);

        afxUnit align = AVX_RASTER_ALIGNMENT;
        afxUnit size = 0;

        afxWarp whd = { ras->whd.w, ras->whd.h, ras->whd.d };
        afxBool is3d = !!AvxTestRasterFlags(ras, avxRasterFlag_3D);

        if (AvxTestRasterFlags(ras, avxRasterFlag_MULTISAMP))
        {
            for (afxUnit i = 0; i < ras->lodCnt; i++)
            {
                afxUnit rowStride = AFX_ALIGN_SIZE(whd.w * pfd.stride, AFX_SIMD_ALIGNMENT);
                afxUnit imgStride = AFX_ALIGN_SIZE(whd.h * rowStride, AFX_SIMD_ALIGNMENT);
                size += whd.d * imgStride;

                whd.w = AFX_MAX(1, whd.w * 2);
                whd.h = AFX_MAX(1, whd.h * 2);
                if (is3d) whd.d = AFX_MAX(1, whd.d * 2);
            }
        }
        else
        {
            for (afxUnit i = 0; i < ras->lodCnt; i++)
            {
                afxUnit rowStride = AFX_ALIGN_SIZE(whd.w * pfd.stride, AFX_SIMD_ALIGNMENT);
                afxUnit imgStride = AFX_ALIGN_SIZE(whd.h * rowStride, AFX_SIMD_ALIGNMENT);
                size += whd.d * imgStride;

                whd.w = AFX_MAX(1, whd.w >> 1);
                whd.h = AFX_MAX(1, whd.h >> 1);
                if (is3d) whd.d = AFX_MAX(1, whd.d >> 1);
            }
        }

        size = AFX_ALIGN_SIZE(size, align);

        ras->reqSiz = size;
        ras->reqAlign = align;
        ras->reqMemType = NIL;

        ras->storage[0].mmu = AFX_INVALID_INDEX;
        ras->storage[0].offset = 0;
        ras->storage[0].hostedAlloc.addr = NIL;
    }
    return err;
}

_AVX afxClassConfig const _AVX_RAS_CLASS_CONFIG =
{
    .fcc = afxFcc_RAS,
    .name = "Raster",
    .desc = "Formatted Video Memory Buffer",
    .fixedSiz = sizeof(AFX_OBJECT(avxRaster)),
    .ctor = (void*)_AvxRasCtorCb,
    .dtor = (void*)_AvxRasDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AVX afxError AvxAcquireRasters(afxDrawSystem dsys, afxUnit cnt, avxRasterInfo const info[], avxRaster rasters[])
{
    afxError err = AFX_ERR_NONE;
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

            if ((info[i].whd.d / 6) && !(flags & avxRasterFlag_LAYERED))
            {
                AFX_ASSERT_AND((info[i].whd.d / 6), (flags & avxRasterFlag_LAYERED));
                AfxThrowError();
            }
        }

        if ((flags & avxRasterFlag_1D) && (info[i].whd.h > 1))
        {
            AFX_ASSERT(1 >= info[i].whd.h);
            AfxThrowError();

            if ((info[i].whd.d > 1) && !(flags & avxRasterFlag_LAYERED))
            {
                AFX_ASSERT_OR(1 >= info[i].whd.d, flags & avxRasterFlag_LAYERED);
                AfxThrowError();
            }
        }

        if ((flags & avxRasterFlag_2D) && (info[i].whd.d > 1) && !(flags & avxRasterFlag_LAYERED))
        {
            AfxThrowError();

            if ((info[i].whd.d > 1) && !(flags & avxRasterFlag_LAYERED))
            {
                AFX_ASSERT_OR(1 >= info[i].whd.d, flags & avxRasterFlag_LAYERED);
                AfxThrowError();
            }
        }
    }
#endif//AVX_VALIDATION_ENABLED

    afxClass* cls = (afxClass*)_AvxDsysGetImpl(dsys)->rasCls(dsys);
    AFX_ASSERT_CLASS(cls, afxFcc_RAS);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)rasters, (void const*[]) { dsys, info }))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT_OBJECTS(afxFcc_RAS, cnt, rasters);

#if AVX_VALIDATION_ENABLED
    for (afxUnit i = 0; i < cnt; i++)
    {
        avxRasterInfo rasi;
        AvxDescribeRaster(rasters[i], &rasi);

        AFX_ASSERT(rasi.base == info[i].base);
        AFX_ASSERT(rasi.lodCnt >= info[i].lodCnt);
        AFX_ASSERT(rasi.baseLayer == info[i].baseLayer);
        AFX_ASSERT(rasi.baseLod == info[i].baseLod);
        AFX_ASSERT(rasi.whd.w >= info[i].whd.w);
        AFX_ASSERT(rasi.whd.h >= info[i].whd.h);
        AFX_ASSERT(rasi.whd.d >= info[i].whd.d);
        AFX_ASSERT(rasi.exuMask == info[i].exuMask);
        AFX_ASSERT((rasi.flags & info[i].flags) == info[i].flags);
        AFX_ASSERT(rasi.fmt == info[i].fmt);
        AFX_ASSERT(rasi.tag.start == info[i].tag.start);
        AFX_ASSERT(rasi.swizzle.r == info[i].swizzle.r);
        AFX_ASSERT(rasi.swizzle.g == info[i].swizzle.g);
        AFX_ASSERT(rasi.swizzle.b == info[i].swizzle.b);
        AFX_ASSERT(rasi.swizzle.a == info[i].swizzle.a);
        AFX_ASSERT(rasi.udd == info[i].udd);
        AFX_ASSERT((rasi.usage & info[i].usage) == info[i].usage);
    }
#endif

    return err;
}
