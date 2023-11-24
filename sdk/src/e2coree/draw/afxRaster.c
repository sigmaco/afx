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
 *                   (c) 2017 SIGMA Technology Group — Federação SIGMA
 *                                    www.sigmaco.org
 */

#define _AFX_DRAW_C
#define _AFX_DRAW_CONTEXT_C
#define _AFX_RASTER_C
#include "afx/draw/afxDrawContext.h"

 // 1d               =   1 x w11 >> lod
 // 1d array         = img x w11 >> lod
 // 2d               =   1 x wh1 >> lod
 // 2d array         = img x wh1
 // cube             =   1 x wh1 >> lod
 // cube array       = img x wh1 >> lod
 // 3d               =   1 x whd >> lod

_AFXINL afxResult AfxTestRaster(afxRaster ras, afxRasterFlags bitmask)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);

    return ras->flags & bitmask;
}

_AFXINL void AfxUnflagRaster(afxRaster ras, afxRasterFlags bitmask)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);

    ras->flags &= ~bitmask;
}

_AFXINL void AfxFlagRaster(afxRaster ras, afxRasterFlags bitmask)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);

    ras->flags |= bitmask;
}

_AFX afxNat AfxCountRasterLods(afxRaster ras)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);

    return ras->lodCnt;
}

_AFX afxNat AfxCountRasterLayers(afxRaster ras)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);

    return ras->layerCnt;
}

_AFX afxBool AfxGetRasterSwizzling(afxRaster ras, afxColorSwizzling const** csw)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);

    if (csw)
        *csw = ras->swizzling;

    return !!ras->swizzling;
}

_AFX afxNat AfxGetRasterSampleCount(afxRaster ras)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);

    return ras->sampleCnt;
}

_AFX afxPixelFormat AfxGetRasterFormat(afxRaster ras)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);

    return ras->fmt;
}

_AFX void AfxGetRasterExtent(afxRaster ras, afxNat lodIdx, afxWhd extent)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);

    AfxAssert(ras->lodCnt > lodIdx);
    AfxAssert(extent);

    extent[0] = ras->whd[0];
    extent[1] = ras->whd[1];
    extent[2] = ras->whd[2];

    while (lodIdx)
    {
        extent[0] = extent[0] >> lodIdx;
        extent[1] = extent[1] >> lodIdx;
        extent[2] = extent[2] >> lodIdx;
        --lodIdx;
    }
}

_AFX afxNat AfxLocateRasterRegion(afxRaster ras, afxNat lodIdx, afxNat layerIdx, afxWhd const offset)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);

    AfxAssertRange(ras->lodCnt, lodIdx, 1);
    AfxAssertRange(ras->layerCnt, layerIdx, 1);
    //AfxAssertRangef(ras->whd[0], offset[0], 1); // is a evil trick to use it to measure ras
    //AfxAssertRangef(ras->whd[1], offset[1], 1);
    //AfxAssertRangef(ras->whd[2], offset[2], 1);
    
    afxPixelLayout pfd;
    AfxDescribePixelFormat(ras->fmt, &pfd);
    afxNat pixelSiz = AFX_ALIGN(pfd.bpp, 32) / AFX_BYTE_SIZE;

    afxWhd offset2 = { offset[0], offset[1], offset[2] };
    afxWhd whd = { ras->whd[0], ras->whd[1], ras->whd[2] };
    afxNat memOff = 0;

    while (lodIdx)
    {
        offset2[0] = offset2[0] >> lodIdx;
        offset2[1] = offset2[1] >> lodIdx;
        offset2[2] = offset2[2] >> lodIdx;
        whd[0] = whd[0] >> lodIdx;
        whd[1] = whd[1] >> lodIdx;
        whd[2] = whd[2] >> lodIdx;
        
        memOff += (ras->layerCnt * (whd[2] * (whd[1] * (whd[0] * pixelSiz))));
        --lodIdx;
    }

    memOff += (layerIdx * (whd[2] * (whd[1] * (whd[0] * pixelSiz))));
    memOff += (offset2[2] * (offset2[1] * (offset2[0] * pixelSiz)));
    memOff = AFX_ALIGN(memOff, AFX_SIMD_ALIGN);

    return memOff;
}

_AFX afxNat AfxMeasureRasterRegion(afxRaster ras, afxRasterRegion const *rgn) // can't be multiple because regions couldn't be continous.
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);

    AfxAssert(rgn);
    AfxAssert(ras->lodCnt > rgn->lodIdx);
    AfxAssertRange(ras->layerCnt, rgn->baseLayer, rgn->layerCnt);
    AfxAssertRange(ras->whd[0], rgn->offset[0], rgn->extent[0]);
    AfxAssertRange(ras->whd[1], rgn->offset[1], rgn->extent[1]);
    AfxAssertRange(ras->whd[2], rgn->offset[2], rgn->extent[2]);

    afxPixelLayout pfd;
    AfxDescribePixelFormat(ras->fmt, &pfd);
    afxNat pixelSiz = AFX_ALIGN(pfd.bpp, 32) / AFX_BYTE_SIZE;

    afxWhd offset2 = { rgn->offset[0], rgn->offset[1], rgn->offset[2] };
    afxWhd extent2 = { rgn->extent[0], rgn->extent[1], rgn->extent[2] };
    afxWhd whd = { ras->whd[0], ras->whd[1], ras->whd[2] };
    afxNat size = 0;

    afxNat lodIdx = 0;
    do
    {
        offset2[0] = offset2[0] >> lodIdx;
        offset2[1] = offset2[1] >> lodIdx;
        offset2[2] = offset2[2] >> lodIdx;
        extent2[0] = extent2[0] >> lodIdx;
        extent2[1] = extent2[1] >> lodIdx;
        extent2[2] = extent2[2] >> lodIdx;
        whd[0] = whd[0] >> lodIdx;
        whd[1] = whd[1] >> lodIdx;
        whd[2] = whd[2] >> lodIdx;

        //memOff += (ras->layerCnt * (whd[2] * (whd[1] * (whd[0] * pixelSiz))));
        ++lodIdx;
    } while (lodIdx < rgn->lodIdx);
    
    extent2[0] = AfxMaxi(extent2[0], 1);
    extent2[1] = AfxMaxi(extent2[1], 1);
    extent2[2] = AfxMaxi(extent2[2], 1);

    for (afxNat i = 1; i < rgn->layerCnt; i++)
        size += ((whd[2] * (whd[1] * (whd[0] * pixelSiz))));

    size += (extent2[2] * (extent2[1] * (extent2[0] * pixelSiz)));

    size -= (offset2[2] * (offset2[1] * (offset2[0] * pixelSiz)));

    size = AFX_ALIGN(size, AFX_SIMD_ALIGN);

    return size;
}

////////////////////////////////////////////////////////////////////////////////
// SERIALIZATION AND STORAGE                                                  //
////////////////////////////////////////////////////////////////////////////////

_AFX afxError AfxInputRasterRegions(afxRaster ras, afxNat cnt, afxRasterRegion const rgn[], afxStream *in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    AfxAssert(cnt);
    AfxAssert(rgn);
    AfxAssertType(in, afxFcc_IOS);

    for (afxNat i = 0; i < cnt; i++)
    {
        afxNat siz = 0;
        void *ptr;

        if ((ptr = AfxOpenRasterRegion(ras, &rgn[i], AFX_TEX_OPEN_W, &siz))) AfxThrowError();
        else
        {
            if (AfxReadStream(in, ptr, siz, 0))
                AfxThrowError();

            AfxCloseRasterRegion(ras, &rgn[i]);
        }
    }
    return err;
}

_AFX afxError AfxInputRasterRegion(afxRaster ras, afxRasterRegion const *rgn, afxStream *in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    AfxAssert(rgn);
    AfxAssertType(in, afxFcc_IOS);

    if (AfxInputRasterRegions(ras, 1, rgn, in))
        AfxThrowError();

    return err;
}

_AFX afxError AfxInputRaster(afxRaster ras, afxNat lodIdx, afxNat baseLayer, afxNat layerCnt, afxStream *in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);

    AfxAssertType(in, afxFcc_IOS);
    AfxAssertRange(ras->lodCnt, lodIdx, 1);
    AfxAssertRange(ras->layerCnt, baseLayer, layerCnt);

    afxRasterRegion rgn;
    rgn.lodIdx = lodIdx;
    rgn.baseLayer = baseLayer;
    rgn.layerCnt = layerCnt;
    rgn.offset[0] = 0;
    rgn.offset[1] = 0;
    rgn.offset[2] = 0;
    rgn.extent[0] = ras->whd[0];
    rgn.extent[1] = ras->whd[1];
    rgn.extent[2] = ras->whd[2];

    if (AfxInputRasterRegion(ras, &rgn, in))
        AfxThrowError();

    return err;
}

_AFX afxError AfxFetchRasterRegions(afxRaster ras, afxNat cnt, afxRasterRegion const rgn[], afxUri const uri[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    AfxAssert(cnt);
    AfxAssert(rgn);
    AfxAssert(uri);

    if (AfxFetchRasterRegionsFromTarga(ras, cnt, rgn, uri))
        AfxThrowError();

    return err;
}

_AFX afxError AfxFetchRasterRegion(afxRaster ras, afxRasterRegion const *rgn, afxUri const *uri)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);

    AfxAssert(rgn);
    AfxAssert(ras->lodCnt > rgn->lodIdx);
    AfxAssertRange(ras->layerCnt, rgn->baseLayer, rgn->layerCnt);
    AfxAssertRange(ras->whd[0], rgn->offset[0], rgn->extent[0]);
    AfxAssertRange(ras->whd[1], rgn->offset[1], rgn->extent[1]);
    AfxAssertRange(ras->whd[2], rgn->offset[2], rgn->extent[2]);
    AfxAssertType(uri, afxFcc_URI);
    AfxAssert(!AfxUriIsBlank(uri));

    if (AfxFetchRasterRegions(ras, 1, rgn, uri))
        AfxThrowError();

    return err;
}

_AFX afxError AfxFetchRaster(afxRaster ras, afxNat lodIdx, afxNat baseLayer, afxNat layerCnt, afxUri const *uri)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);

    AfxAssertType(uri, afxFcc_URI);
    AfxAssert(!AfxUriIsBlank(uri));

    afxRasterRegion rgn;
    rgn.lodIdx = lodIdx;
    rgn.baseLayer = baseLayer;
    rgn.layerCnt = layerCnt;
    rgn.offset[0] = 0;
    rgn.offset[1] = 0;
    rgn.offset[2] = 0;
    rgn.extent[0] = ras->whd[0];
    rgn.extent[1] = ras->whd[1];
    rgn.extent[2] = ras->whd[2];

    if (AfxFetchRasterRegion(ras, &rgn, uri))
        AfxThrowError();

    return err;
}

_AFX afxError AfxOutputRasterRegions(afxRaster ras, afxNat cnt, afxRasterRegion const rgn[], afxStream *out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);

    for (afxNat i = 0; i < cnt; i++)
    {
        afxNat siz = 0;
        void *ptr;

        if ((ptr = AfxOpenRasterRegion(ras, &rgn[i], AFX_TEX_OPEN_R, &siz))) AfxThrowError();
        else
        {
            if (AfxWriteStream(out, ptr, siz, 0))
                AfxThrowError();

            AfxCloseRasterRegion(ras, &rgn[i]);
        }
    }
    return err;
}

_AFX afxError AfxOutputRasterRegion(afxRaster ras, afxRasterRegion const *rgn, afxStream *out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    AfxAssert(rgn);
    AfxAssertType(out, afxFcc_IOS);

    if (AfxOutputRasterRegions(ras, 1, rgn, out))
        AfxThrowError();

    return err;
}

_AFX afxError AfxOutputRaster(afxRaster ras, afxNat lodIdx, afxNat baseLayer, afxNat layerCnt, afxStream *out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);

    AfxAssertType(out, afxFcc_IOS);
    AfxAssertRange(ras->lodCnt, lodIdx, 1);
    AfxAssertRange(ras->layerCnt, baseLayer, layerCnt);

    afxRasterRegion rgn;
    rgn.lodIdx = lodIdx;
    rgn.baseLayer = baseLayer;
    rgn.layerCnt = layerCnt;
    rgn.offset[0] = 0;
    rgn.offset[1] = 0;
    rgn.offset[2] = 0;
    rgn.extent[0] = ras->whd[0];
    rgn.extent[1] = ras->whd[1];
    rgn.extent[2] = ras->whd[2];

    if (AfxOutputRasterRegion(ras, &rgn, out))
        AfxThrowError();

    return err;
}

_AFX afxError AfxPrintRasterRegions(afxRaster ras, afxNat cnt, afxRasterRegion const rgn[], afxUri const uri[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    AfxAssert(cnt);
    AfxAssert(rgn);
    AfxAssert(uri);

    if (AfxPrintRasterRegionsToTarga(ras, cnt, rgn, uri))
        AfxThrowError();

    return err;
}

_AFX afxError AfxPrintRasterRegion(afxRaster ras, afxRasterRegion const *rgn, afxUri const *uri)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);

    AfxAssert(rgn);
    AfxAssert(ras->lodCnt > rgn->lodIdx);
    AfxAssertRange(ras->layerCnt, rgn->baseLayer, rgn->layerCnt);
    AfxAssertRange(ras->whd[0], rgn->offset[0], rgn->extent[0]);
    AfxAssertRange(ras->whd[1], rgn->offset[1], rgn->extent[1]);
    AfxAssertRange(ras->whd[2], rgn->offset[2], rgn->extent[2]);

    AfxAssertType(uri, afxFcc_URI);
    AfxAssert(!AfxUriIsBlank(uri));

    if (AfxPrintRasterRegions(ras, 1, rgn, uri))
        AfxThrowError();

    return err;
}

_AFX afxError AfxPrintRaster(afxRaster ras, afxNat lodIdx, afxNat baseLayer, afxNat layerCnt, afxUri const *uri)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);

    AfxAssertType(uri, afxFcc_URI);
    AfxAssert(!AfxUriIsBlank(uri));

    afxRasterRegion rgn;
    rgn.lodIdx = lodIdx;
    rgn.baseLayer = baseLayer;
    rgn.layerCnt = layerCnt;
    rgn.offset[0] = 0;
    rgn.offset[1] = 0;
    rgn.offset[2] = 0;
    rgn.extent[0] = ras->whd[0];
    rgn.extent[1] = ras->whd[1];
    rgn.extent[2] = ras->whd[2];

    if (AfxPrintRasterRegion(ras, &rgn, uri))
        AfxThrowError();

    return err;
}

////////////////////////////////////////////////////////////////////////////////
// TRANSFERENCE AND COPY                                                      //
////////////////////////////////////////////////////////////////////////////////

_AFX afxError AfxUpdateRasterRegions(afxRaster ras, afxNat cnt, afxRasterRegion const rgn[], void const *src[], afxNat const siz[], afxPixelFormat const fmt[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);

    AfxAssert(cnt);
    AfxAssert(rgn);
    AfxAssert(src);
    AfxAssert(fmt);

    for (afxNat i = 0; i < cnt; i++)
    {
        AfxAssert(ras->lodCnt > rgn[i].lodIdx);
        AfxAssertRange(ras->layerCnt, rgn[i].baseLayer, rgn[i].layerCnt);
        AfxAssertRange(ras->whd[0], rgn[i].offset[0], rgn[i].extent[0]);
        AfxAssertRange(ras->whd[1], rgn[i].offset[1], rgn[i].extent[1]);
        AfxAssertRange(ras->whd[2], rgn[i].offset[2], rgn[i].extent[2]);

        afxNat size;
        void *data = AfxOpenRasterRegion(ras, &rgn[i], AFX_TEX_OPEN_W, &size);

        if (!data) AfxThrowError();
        else
        {
            AfxAssert(fmt[i] == ras->fmt);
            AfxAssert(size >= siz[i]);
            AfxCopy(1, AfxMini((siz[i]), size), src[i], data);

            AfxCloseRasterRegion(ras, &rgn[i]);
        }
    }
    return err;
}

_AFX afxError AfxUpdateRasterRegion(afxRaster ras, afxRasterRegion const *rgn, void const *src, afxNat siz, afxPixelFormat fmt)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);

    AfxAssert(rgn);
    AfxAssert(ras->lodCnt > rgn->lodIdx);
    AfxAssertRange(ras->layerCnt, rgn->baseLayer, rgn->layerCnt);
    AfxAssertRange(ras->whd[0], rgn->offset[0], rgn->extent[0]);
    AfxAssertRange(ras->whd[1], rgn->offset[1], rgn->extent[1]);
    AfxAssertRange(ras->whd[2], rgn->offset[2], rgn->extent[2]);
    AfxAssert(src);
    AfxAssert(fmt);
    
    if (AfxUpdateRasterRegions(ras, 1, rgn, &src, &siz, &fmt))
        AfxThrowError();

    return err;
}

_AFX afxError AfxUpdateRaster(afxRaster ras, afxNat lodIdx, afxNat baseLayer, afxNat layerCnt, void const *src, afxNat siz, afxPixelFormat fmt)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);

    AfxAssert(ras->lodCnt > lodIdx);
    AfxAssertRange(ras->layerCnt, baseLayer, layerCnt);
    AfxAssert(src);
    AfxAssert(fmt);

    afxRasterRegion rgn;
    rgn.lodIdx = lodIdx;
    rgn.baseLayer = baseLayer;
    rgn.layerCnt = layerCnt;
    rgn.offset[0] = 0;
    rgn.offset[1] = 0;
    rgn.offset[2] = 0;
    rgn.extent[0] = ras->whd[0];
    rgn.extent[1] = ras->whd[1];
    rgn.extent[2] = ras->whd[2];

    if (AfxUpdateRasterRegion(ras, &rgn, &src, siz, fmt))
        AfxThrowError();

    return err;
}

_AFX void AfxCloseRasterRegion(afxRaster ras, afxRasterRegion const *rgn)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);

    AfxAssert(rgn);
    AfxAssert(ras->lodCnt > rgn->lodIdx);
    AfxAssertRange(ras->layerCnt, rgn->baseLayer, rgn->layerCnt);
    AfxAssertRange(ras->whd[0], rgn->offset[0], rgn->extent[0]);
    AfxAssertRange(ras->whd[1], rgn->offset[1], rgn->extent[1]);
    AfxAssertRange(ras->whd[2], rgn->offset[2], rgn->extent[2]);

    if (ras->unmap(ras, rgn))
        AfxThrowError();
};

_AFX void* AfxOpenRasterRegion(afxRaster ras, afxRasterRegion const *rgn, afxRasterOpenFlags flags, afxNat *size)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);

    AfxAssert(rgn);
    AfxAssert(ras->lodCnt > rgn->lodIdx);
    AfxAssertRange(ras->layerCnt, rgn->baseLayer, rgn->layerCnt);
    AfxAssertRange(ras->whd[0], rgn->offset[0], rgn->extent[0]);
    AfxAssertRange(ras->whd[1], rgn->offset[1], rgn->extent[1]);
    AfxAssertRange(ras->whd[2], rgn->offset[2], rgn->extent[2]);
    
    void *ptr = NIL;

    if (ras->map(ras, rgn, flags, size, &ptr))
        AfxThrowError();

    return ptr;
}

_AFX afxError AfxBufferizeRaster(afxRaster ras)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);

    afxDrawContext dctx = AfxGetObjectProvider(ras);
    afxContext mem = AfxGetDrawContextMemory(dctx);
    AfxAssertObjects(1, &mem, afxFcc_CTX);

    afxRasterRegion rgn;
    rgn.baseLayer = 0;
    rgn.layerCnt = ras->layerCnt;
    rgn.offset[0] = 0;
    rgn.offset[1] = 0;
    rgn.offset[2] = 0;
    rgn.extent[0] = ras->whd[0];
    rgn.extent[1] = ras->whd[1];
    rgn.extent[2] = ras->whd[2];

    afxNat siz = 0;

    for (afxNat i = 0; i < ras->lodCnt; i++)
    {
        rgn.lodIdx = i;
        siz += AfxMeasureRasterRegion(ras, &rgn);
    }

    void *maps = NIL;

    if (siz && !(maps = AfxReallocate(mem, ras->maps, AFX_ALIGN(siz, AFX_SIMD_ALIGN), 1, AFX_SIMD_ALIGN, AfxHint()))) AfxThrowError();
    else
        ras->maps = maps;

    return err;
}

#if 0
_AFX afxError AfxReadjustTexture(afxRaster ras, afxWhd const extent)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    AfxAssert(extent);
    AfxAssert(extent[0]);
    AfxAssert(extent[1]);
    AfxAssert(extent[2]);
    AfxAssert(extent[0] != ras->whd[0] || extent[1] != ras->whd[1] || extent[2] != ras->whd[2]);

    afxWhd whd = { ras->whd[0], ras->whd[1], ras->whd[2] };
    ras->whd[0] = extent[0];
    ras->whd[1] = extent[1];
    ras->whd[2] = extent[2];

    if (AfxBufferizeRaster(ras))
    {
        AfxThrowError();
        ras->whd[0] = whd[0]; // try to restore it
        ras->whd[1] = whd[1];
        ras->whd[2] = whd[2];
        AfxBufferizeRaster(ras);
    }
    else
    {
        // Agora, forçemos a atualização

        afxNat rgnSize;
        afxRasterRegion rgn;
        rgn.baseLayer = 0;
        rgn.layerCnt = ras->layerCnt;
        rgn.offset[0] = 0;
        rgn.offset[1] = 0;
        rgn.offset[2] = 0;
        rgn.extent[0] = ras->whd[0];
        rgn.extent[1] = ras->whd[1];
        rgn.extent[2] = ras->whd[2];

        for (afxNat i = 0; i < ras->lodCnt; i++)
        {
            rgn.lodIdx = i;
            AfxOpenRasterRegion(ras, &rgn, AFX_TEX_OPEN_X, &rgnSize);
            AfxCloseRasterRegion(ras, &rgn);
        }

        afxEvent ev;
        AfxEventDeploy(&ev, AFX_EVENT_TEX_RESIZ, &ras->obj, ras->whd);
        AfxObjectEmitEvent(&ras->obj, &ev);
    }
    return err;
}
#endif

#if 0
_AFX void const* AfxTextureGetData(afxRaster ras, afxNat lod, afxNat layer, afxWhd const offset)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    static afxWhd const whdZeroed = { 0, 0, 0 };
    afxNat const *xyz = offset ? offset : whdZeroed;

    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    AfxAssert(ras->lodCnt > lod);
    AfxAssert(ras->layerCnt > layer);
    AfxAssert(ras->whd[0] > xyz[0]);
    AfxAssert(ras->whd[1] > xyz[1]);
    AfxAssert(ras->whd[2] > xyz[2]);

    afxSize off = AfxLocateRasterRegion(ras, lod, layer, offset);
    afxByte *bytemap = &(ras->bytemap[off]);
    return bytemap;
}
#endif

_AFX afxError AfxGenerateRasterLods(afxRaster ras, afxNat firstLod, afxNat lodCnt)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    (void)firstLod;
    AfxAssert(lodCnt);
    AfxThrowError();
    return err;
}

_AFX afxError AfxSwapRasterChannels(afxRaster ras, afxColorSwizzle a, afxColorSwizzle b, afxRasterRegion const *rgn)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);

    AfxAssert(rgn);
    AfxAssert(ras->lodCnt > rgn->lodIdx);
    AfxAssertRange(ras->layerCnt, rgn->baseLayer, rgn->layerCnt);
    AfxAssertRange(ras->whd[0], rgn->offset[0], rgn->extent[0]);
    AfxAssertRange(ras->whd[1], rgn->offset[1], rgn->extent[1]);
    AfxAssertRange(ras->whd[2], rgn->offset[2], rgn->extent[2]);

    afxPixelFormat fmt = AfxGetRasterFormat(ras);
    AfxAssert(fmt == AFX_PFD_BGR8 || fmt == AFX_PFD_BGRA8 || fmt == AFX_PFD_RGB8 || fmt == AFX_PFD_RGBA8);
    
    afxPixelLayout pfd;
    AfxDescribePixelFormat(fmt, &pfd);
    afxNat pixelSiz = pfd.bpp / AFX_BYTE_SIZE;

    AfxAssert(a < AFX_COLOR_SIZZLE_ZERO);

    afxNat rgnSize;
    afxByte *bytemap = AfxOpenRasterRegion(ras, rgn, AFX_TEX_OPEN_RW, &rgnSize);

    // swap the R and B values to get RGB since the bitmap color format is in BGR
    for (afxNat k = 0; k < rgn->extent[0] * rgn->extent[1] * pixelSiz; k += pixelSiz)
    {
        afxByte temp = (b != AFX_COLOR_SIZZLE_ZERO ? (b != AFX_COLOR_SIZZLE_ONE ? bytemap[k + b] : 1) : 0);
        bytemap[k + b] = bytemap[k + a];
        bytemap[k + a] = temp;
    }

    AfxCloseRasterRegion(ras, rgn);
    return err;
}

_AFX afxError AfxAcquireRasters(afxDrawContext dctx, afxNat cnt, afxRasterInfo const info[], afxRaster ras[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    AfxAssert(cnt);
    AfxAssert(info);
    AfxAssert(ras);

    if (AfxAcquireObjects(&dctx->rasters, cnt, (afxHandle*)ras, (void*[]) { (void*)info }))
        AfxThrowError();

    AfxAssertObjects(cnt, ras, afxFcc_RAS);

    return err;
}

_AFX afxRaster AfxAssembleRaster(afxDrawContext dctx, afxRasterFlags flags, afxNat cnt, afxUri const uri[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    AfxAssert(cnt);
    AfxAssert(uri);
    afxRaster ras;

    if (AfxAssembleRastersFromTarga(dctx, flags, cnt, uri, &ras))
        AfxThrowError();

    AfxTryAssertObjects(1, &ras, afxFcc_RAS);

    return ras;
}

_AFX afxRaster AfxAssembleCubemapRasters(afxDrawContext dctx, afxRasterFlags flags, afxUri const uri[6])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    AfxAssert(uri);
    afxRaster ras;

    if (!(ras = AfxAssembleRaster(dctx, flags | AFX_TEX_FLAG_CUBEMAP, 6, uri)))
        AfxThrowError();

    AfxTryAssertObjects(1, &ras, afxFcc_RAS);

    return ras;
}
