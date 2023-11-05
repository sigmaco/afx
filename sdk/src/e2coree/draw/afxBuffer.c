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
#define _AFX_BUFFER_C
#include "afxDrawClassified.h"
#include "afx/draw/afxDrawSystem.h"


_AFX afxError AfxDumpBuffer2(afxBuffer buf, afxSize offset, afxSize stride, afxSize cnt, void *dst, afxSize dstStride)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &buf, afxFcc_BUF);

    afxSize siz = AfxGetBufferSize(buf);
    AfxAssertRange(siz, offset, cnt * stride);
    AfxAssert(dst);
    AfxAssert(dstStride);

    afxByte *map = AfxMapBufferRange(buf, offset, cnt * stride, AFX_BUF_MAP_R);

    if (!map) AfxThrowError();
    else
    {
        if (!dstStride || dstStride == stride)
        {
            AfxCopy(dst, map, (cnt * stride));
        }
        else
        {
            afxByte *dst2 = dst;

            for (afxNat i = 0; i < cnt; i++)
            {
                AfxCopy(&(dst2[i * dstStride]), &(map[i * stride]), stride);
            }
        }

        AfxUnmapBufferRange(buf);
    }
    return err;
}

_AFX afxError AfxDumpBuffer(afxBuffer buf, afxSize offset, afxSize range, void *dst)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &buf, afxFcc_BUF);

    afxSize siz = AfxGetBufferSize(buf);
    AfxAssert(siz > offset);
    AfxAssert(range);
    AfxAssert(siz >= offset + range);
    AfxAssert(dst);

    afxByte *map = AfxMapBufferRange(buf, offset, range, AFX_BUF_MAP_R);

    if (!map) AfxThrowError();
    else
    {
        AfxCopy(dst, map, range);

        AfxUnmapBufferRange(buf);
    }
    return err;
}

_AFX afxError AfxUpdateBufferRegion(afxBuffer buf, afxBufferRegion const* rgn, void const* src, afxSize srcStride)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    AfxAssert(rgn);

    afxSize bufSiz = AfxGetBufferSize(buf);
    AfxAssertRange(bufSiz, rgn->base, rgn->range);
    AfxAssert(src);
    AfxAssert(srcStride);

    afxByte *map = AfxMapBufferRange(buf, rgn->base, rgn->range, AFX_BUF_MAP_W);

    if (!map) AfxThrowError();
    else
    {
        if ((srcStride == rgn->stride) && rgn->stride)
        {
            AfxCopy(map, src, rgn->range);
        }
        else
        {
            afxByte const *src2 = src;
            //afxNat unitSiz = AfxMinor(rgn->stride, srcStride); // minor non-zero
            afxNat cnt = rgn->range / rgn->stride;

            for (afxNat i = 0; i < cnt; i++)
            {
                AfxCopy(&(map[(i * rgn->stride) + rgn->offset]), &(src2[i * srcStride]), rgn->unitSiz);
            }
        }

        AfxUnmapBufferRange(buf);
    }

    return err;
}

_AFX afxError AfxUpdateBuffer2(afxBuffer buf, afxSize offset, afxSize range, afxSize stride, void const *src, afxSize srcStride)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &buf, afxFcc_BUF);

    afxSize bufSiz = AfxGetBufferSize(buf);
    AfxAssertRange(bufSiz, offset, range);
    AfxAssert(src);
    AfxAssert(srcStride);

    afxByte *map = AfxMapBufferRange(buf, offset, range, AFX_BUF_MAP_W);

    if (!map) AfxThrowError();
    else
    {
        if ((srcStride == stride) && stride)
        {
            AfxCopy(map, src, range);
        }
        else
        {
            afxByte const *src2 = src;
            afxNat unitSiz = AfxMinor(stride, srcStride); // minor non-zero
            afxNat cnt = range / unitSiz;

            for (afxNat i = 0; i < cnt; i++)
            {
                AfxAssert(stride != 1 || (stride == 1 && AFX_N8_MAX >= (afxNat8)(src2[i * srcStride])));
                AfxAssert(stride != 2 || (stride == 2 && AFX_N16_MAX >= (afxNat16)(src2[i * srcStride])));
                AfxAssert(stride != 4 || (stride == 4 && AFX_N32_MAX >= (afxNat32)(src2[i * srcStride])));

                AfxCopy(&(map[i * stride]), &(src2[i * srcStride]), unitSiz);
            }
        }

        AfxUnmapBufferRange(buf);
    }
    return err;
}

_AFX afxError AfxUpdateBuffer(afxBuffer buf, afxSize offset, afxSize range, void const *src)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &buf, afxFcc_BUF);

    afxSize siz = AfxGetBufferSize(buf);
    AfxAssertRange(siz, offset, range);
    AfxAssert(src);

    afxByte *map = AfxMapBufferRange(buf, offset, range, AFX_BUF_MAP_W);

    if (!map) AfxThrowError();
    else
    {
        AfxCopy(map, src, range);

        AfxUnmapBufferRange(buf);
    }
    return err;
}

_AFX afxSize AfxGetBufferSize(afxBuffer buf)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &buf, afxFcc_BUF);

    return buf->siz;
}

_AFX void* AfxMapBufferRange(afxBuffer buf, afxSize offset, afxNat range, afxFlags flags)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &buf, afxFcc_BUF);

    AfxAssertRange(buf->siz, offset, range);

    return buf->vmt->map(buf, offset, range, flags);
    return NIL;
}

_AFX void AfxUnmapBufferRange(afxBuffer buf)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &buf, afxFcc_BUF);

    buf->vmt->unmap(buf);
}

_AFX afxError AfxAcquireBuffers(afxDrawContext dctx, afxNat cnt, afxBuffer buf[], afxBufferSpecification const spec[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(spec);
    AfxAssert(buf);
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    
    if (AfxAcquireObjects(&dctx->buffers, cnt, (afxHandle*)buf, (void*[]) { (void*)spec }))
        AfxThrowError();

    return err;
}
