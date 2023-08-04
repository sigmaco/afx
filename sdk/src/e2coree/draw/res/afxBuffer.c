/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                      S I G M A   T E C H N O L O G Y   G R O U P
 *
 *                                   Public Test Build
 *                               (c) 2017 Federação SIGMA
 *                                    www.sigmaco.org
 */

#define _AFX_BUFFER_C
#define _AFX_DRAW_CONTEXT_C
#include "../_classified/afxDrawClassified.h"

_AFX afxError AfxDumpBuffer2(afxBuffer buf, afxSize offset, afxSize stride, afxSize cnt, void *dst, afxSize dstStride)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(buf, AFX_FCC_BUF);
    afxSize siz = AfxGetBufferSize(buf);
    AfxAssertRange(siz, offset, cnt * stride);
    AfxAssert(dst);
    AfxAssert(dstStride);

    afxByte *map = AfxMapBufferRange(buf, offset, cnt * stride, AFX_BUF_MAP_R);

    if (!map) AfxThrowError();
    else
    {
        afxByte const *bytemap = &(map[offset]);

        if (!dstStride || dstStride == stride)
        {
            AfxCopy(dst, bytemap, (cnt * stride));
        }
        else
        {
            afxByte *dst2 = dst;

            for (afxNat i = 0; i < cnt; i++)
            {
                AfxCopy(&(dst2[i * dstStride]), &(bytemap[i * stride]), stride);
            }
        }

        AfxUnmapBufferRange(buf);
    }
    return err;
}

_AFX afxError AfxDumpBuffer(afxBuffer buf, afxSize base, afxSize range, void *dst)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(buf, AFX_FCC_BUF);
    afxSize siz = AfxGetBufferSize(buf);
    AfxAssert(siz > base);
    AfxAssert(range);
    AfxAssert(siz >= base + range);
    AfxAssert(dst);

    afxByte *map = AfxMapBufferRange(buf, base, range, AFX_BUF_MAP_R);

    if (!map) AfxThrowError();
    else
    {
        AfxCopy(dst, &map[base], range);

        AfxUnmapBufferRange(buf);
    }
    return err;
}

_AFX afxError AfxUpdateBuffer2(afxBuffer buf, afxSize offset, afxSize stride, afxNat cnt, void const *src, afxSize srcStride)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(buf, AFX_FCC_BUF);
    afxSize siz = AfxGetBufferSize(buf);
    AfxAssertRange(siz, offset, cnt * stride);
    AfxAssert(src);
    AfxAssert(srcStride);

    afxByte *map = AfxMapBufferRange(buf, offset, (cnt * stride), AFX_BUF_MAP_W);

    if (!map) AfxThrowError();
    else
    {
        afxByte *bytemap = &(map[offset]);

        if (srcStride == stride)
        {
            AfxCopy(bytemap, src, (cnt * stride));
        }
        else
        {
            afxByte const *src2 = src;

            for (afxNat i = 0; i < cnt; i++)
            {
                AfxAssert(stride != 1 || (stride == 1 && AFX_N8_MAX >= (afxNat8)(src2[i * srcStride])));
                AfxAssert(stride != 2 || (stride == 2 && AFX_N16_MAX >= (afxNat16)(src2[i * srcStride])));
                AfxAssert(stride != 4 || (stride == 4 && AFX_N32_MAX >= (afxNat32)(src2[i * srcStride])));

                AfxCopy(&(bytemap[i * stride]), &(src2[i * srcStride]), stride);
            }
        }

        if (buf->lastUpdOffset > offset)
            buf->lastUpdOffset = offset;

        if (buf->lastUpdRange < offset + (cnt * stride))
            buf->lastUpdRange = offset + (cnt * stride);

        AfxUnmapBufferRange(buf);
    }
    return err;
}

_AFX afxError AfxUpdateBuffer(afxBuffer buf, afxSize base, afxSize range, void const *src)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(buf, AFX_FCC_BUF);
    afxSize siz = AfxGetBufferSize(buf);
    AfxAssertRange(siz, base, range);
    AfxAssert(src);

    afxByte *map = AfxMapBufferRange(buf, base, range, AFX_BUF_MAP_W);

    if (!map) AfxThrowError();
    else
    {
        AfxCopy(&map[base], src, range);

        if (buf->lastUpdOffset > base)
            buf->lastUpdOffset = base;

        if (buf->lastUpdRange < base + range)
            buf->lastUpdRange = base + range;

        AfxUnmapBufferRange(buf);
    }
    return err;
}

_AFX afxSize AfxGetBufferSize(afxBuffer buf)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(buf, AFX_FCC_BUF);
    return buf->siz;
}

_AFX void* AfxGetBufferIdd(afxBuffer buf)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(buf, AFX_FCC_BUF);
    return buf->idd;
}

_AFX void* AfxMapBufferRange(afxBuffer buf, afxSize offset, afxNat range, afxFlags flags)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(buf, AFX_FCC_BUF);
    AfxAssertRange(buf->siz, offset, range);

    ++buf->locked;

    return buf->vmt->map(buf, offset, range, flags);
}

_AFX void AfxUnmapBufferRange(afxBuffer buf)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(buf, AFX_FCC_BUF);

    --buf->locked;

    buf->vmt->unmap(buf);
}

_AFX afxDrawContext AfxGetBufferContext(afxBuffer buf)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(buf, AFX_FCC_BUF);
    afxDrawContext dctx = AfxObjectGetProvider(&buf->obj);
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    return dctx;
}

_AFX afxError AfxAcquireBuffers(afxDrawContext dctx, afxNat cnt, afxBufferSpecification const spec[], afxBuffer buf[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(spec);
    AfxAssert(buf);
    //AfxEntry("dsys=%p,siz=%u,usage=%x", dsys, siz, usage);
    
    if (AfxClassAcquireObjects(AfxGetBufferClass(dctx), NIL, cnt, spec, (afxObject**)buf, AfxSpawnHint()))
        AfxThrowError();

    for (afxNat i = 0; i < cnt; i++)
        AfxTryAssertObject(buf[i], AFX_FCC_BUF);

    return err;
}

_AFX afxBool _AfxBufEventHandler(afxObject *obj, afxEvent *ev)
{
    afxError err = AFX_ERR_NONE;
    afxBuffer buf = (void*)obj;
    AfxAssertObject(buf, AFX_FCC_BUF);
    (void)ev;
    return FALSE;
}

_AFX afxBool _AfxBufEventFilter(afxObject *obj, afxObject *watched, afxEvent *ev)
{
    afxError err = AFX_ERR_NONE;
    afxBuffer buf = (void*)obj;
    AfxAssertObject(buf, AFX_FCC_BUF);
    (void)watched;
    (void)ev;
    return FALSE;
}

_AFX afxError _AfxBufDtor(afxBuffer buf)
{
    afxError err = AFX_ERR_NONE;
    AfxEntry("buf=%p", buf);
    AfxAssertObject(buf, AFX_FCC_BUF);

    if (buf->bytemap)
    {
        afxMemory mem = AfxGetDrawMemory();
        AfxAssertObject(mem, AFX_FCC_MEM);

        AfxDeallocate(mem, buf->bytemap);
    }
    return err;
}

_AFX afxError _AfxBufCtor(void *cache, afxNat idx, afxBuffer buf, afxBufferSpecification const *specs)
{
    AfxEntry("buf=%p", buf);
    (void)cache;
    afxResult err = NIL;
    AfxAssertObject(buf, AFX_FCC_BUF);

    afxBufferSpecification const *spec = &specs[idx];
    AfxAssert(spec);

    afxDrawContext dctx = AfxGetBufferContext(buf);

    buf->siz = spec->siz;
    buf->usage = spec->usage;

    afxMemory mem = AfxGetDrawMemory();
    AfxAssertObject(mem, AFX_FCC_MEM);

    if (!(buf->bytemap = AfxAllocate(mem, buf->siz, 0, AfxSpawnHint()))) AfxThrowError();
    else
    {
        if (spec->src)
            AfxCopy(buf->bytemap, spec->src, buf->siz);

        buf->lastUpdOffset = 0;
        buf->lastUpdRange = buf->siz;
        buf->locked = FALSE;

        buf->idd = NIL;

        if (dctx->vmt->buf && dctx->vmt->buf(buf))
            AfxThrowError();

        AfxAssert(buf->vmt);

        if (err)
            AfxDeallocate(mem, buf->bytemap);
    }
    return err;
}

_AFX afxClassSpecification const _AfxBufClassSpec;

afxClassSpecification const _AfxBufClassSpec =
{
    AFX_FCC_BUF,
    NIL,
    0,
    sizeof(AFX_OBJECT(afxBuffer)),
    NIL,
    (void*)_AfxBufCtor,
    (void*)_AfxBufDtor,
    .event = _AfxBufEventHandler,
    .eventFilter = _AfxBufEventFilter,
    "afxBuffer",
    NIL
};
