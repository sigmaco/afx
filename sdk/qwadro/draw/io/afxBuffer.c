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

#define _AFX_DRAW_C
#define _AFX_BUFFER_C
#include "qwadro/core/afxClass.h"
#include "qwadro/draw/afxDrawContext.h"

_AVX afxDrawContext AfxGetBufferContext(afxBuffer buf)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    afxDrawContext dctx = AfxGetObjectProvider(buf);
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    return dctx;
}

_AVX afxNat AfxGetBufferCapacity(afxBuffer buf)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    return buf->siz;
}

_AVX afxBufferUsage AfxGetBufferUsage(afxBuffer buf)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    return buf->usage;
}

_AVX afxBufferUsage AfxTestBufferUsage(afxBuffer buf, afxBufferUsage usage)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    return buf->usage & usage;
}

_AVX afxBufferAccess AfxGetBufferAccess(afxBuffer buf)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    return buf->access;
}

_AVX afxBufferAccess AfxTestBufferAccess(afxBuffer buf, afxBufferAccess access)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    return buf->access & access;
}

_AVX void AfxUnmapBufferRange(afxBuffer buf)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &buf, afxFcc_BUF);

    buf->unmap(buf);
}

_AVX void* AfxMapBufferRange(afxBuffer buf, afxNat offset, afxNat range, afxFlags flags)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    //AfxAssertAlign(offset, 64);
    AfxAssertRange(buf->siz, offset, range);

    return buf->map(buf, offset, range, flags);
}

_AVX afxError AfxDumpBuffer2(afxBuffer buf, afxNat offset, afxNat stride, afxNat cnt, void *dst, afxNat dstStride)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    AfxAssertAlign(offset, 64);

    afxNat siz = AfxGetBufferCapacity(buf);
    AfxAssertRange(siz, offset, cnt * stride);
    AfxAssert(dst);
    AfxAssert(dstStride);

    afxByte *map = AfxMapBufferRange(buf, offset, cnt * stride, afxBufferAccess_R);

    if (!map) AfxThrowError();
    else
    {
        if (!dstStride || dstStride == stride)
        {
            AfxCopy(cnt, stride, map, dst);
        }
        else
        {
            afxByte *dst2 = dst;

            for (afxNat i = 0; i < cnt; i++)
            {
                AfxCopy(1, stride, &(map[i * stride]), &(dst2[i * dstStride]));
            }
        }

        AfxUnmapBufferRange(buf);
    }
    return err;
}

_AVX afxError AfxDumpBuffer(afxBuffer buf, afxNat offset, afxNat range, void *dst)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    AfxAssertAlign(offset, 64);

    afxNat siz = AfxGetBufferCapacity(buf);
    AfxAssert(siz > offset);
    AfxAssert(range);
    AfxAssert(siz >= offset + range);
    AfxAssert(dst);

    afxByte *map = AfxMapBufferRange(buf, offset, range, afxBufferAccess_R);

    if (!map) AfxThrowError();
    else
    {
        AfxCopy(1, range, map, dst);

        AfxUnmapBufferRange(buf);
    }
    return err;
}

_AVX afxError AfxUpdateBufferRegion(afxBuffer buf, afxBufferRegion const* rgn, void const* src, afxNat srcStride)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    AfxAssert(rgn);
    AfxAssertAlign(rgn->base, 64);
    

    afxNat bufSiz = AfxGetBufferCapacity(buf);
    AfxAssertRange(bufSiz, rgn->base, rgn->range);
    AfxAssert(src);
    AfxAssert(srcStride);

    afxByte *map = AfxMapBufferRange(buf, rgn->base, rgn->range, afxBufferAccess_W);

    if (!map) AfxThrowError();
    else
    {
        if ((srcStride == rgn->stride) && rgn->stride)
        {
            AfxCopy(rgn->range, 1, src, map);
        }
        else
        {
            afxByte const *src2 = src;
            afxNat unitSiz = AfxMinorNonZero(rgn->stride, srcStride); // minor non-zero
            AfxAssert(unitSiz == rgn->unitSiz);
            afxNat cnt = rgn->range / rgn->stride;
            afxNat cnt2 = rgn->range % rgn->stride;
            //AfxAssert(!cnt2);

            for (afxNat i = 0; i < cnt; i++)
            {
                AfxAssertRange(bufSiz, (i * rgn->stride) + rgn->offset, rgn->unitSiz);
                AfxCopy(1, rgn->unitSiz, &(src2[i * srcStride]), &(map[(i * rgn->stride) + rgn->offset]));
            }
        }

        AfxUnmapBufferRange(buf);
    }

    return err;
}

_AVX afxError AfxUpdateBuffer2(afxBuffer buf, afxNat offset, afxNat range, afxNat stride, void const *src, afxNat srcStride)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    AfxAssertAlign(offset, 64);

    afxNat bufSiz = AfxGetBufferCapacity(buf);
    AfxAssertRange(bufSiz, offset, range);
    AfxAssert(src);
    AfxAssert(srcStride);

    afxByte *map = AfxMapBufferRange(buf, offset, range, afxBufferAccess_W);

    if (!map) AfxThrowError();
    else
    {
        if ((srcStride == stride) && stride)
        {
            AfxCopy(1, range, src, map);
        }
        else
        {
            afxByte const *src2 = src;
            afxNat unitSiz = AfxMinorNonZero(stride, srcStride); // minor non-zero
            afxNat cnt = range / unitSiz;
            afxNat cnt2 = range % unitSiz;
            AfxAssert(!cnt2);

            for (afxNat i = 0; i < cnt; i++)
            {
                AfxAssert(stride != 1 || (stride == 1 && AFX_N8_MAX >= (afxNat8)(src2[i * srcStride])));
                AfxAssert(stride != 2 || (stride == 2 && AFX_N16_MAX >= (afxNat16)(src2[i * srcStride])));
                AfxAssert(stride != 4 || (stride == 4 && AFX_N32_MAX >= (afxNat32)(src2[i * srcStride])));

                AfxCopy(1, unitSiz, &(src2[i * srcStride]), &(map[i * stride]));
            }
        }

        AfxUnmapBufferRange(buf);
    }
    return err;
}

_AVX afxError AfxUpdateBuffer(afxBuffer buf, afxNat offset, afxNat range, void const *src)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    //AfxAssertAlign(offset, 64);

    afxNat siz = AfxGetBufferCapacity(buf);
    AfxAssertRange(siz, offset, range);
    AfxAssert(src);

    afxByte *map = AfxMapBufferRange(buf, offset, range, afxBufferAccess_W);

    if (!map) AfxThrowError();
    else
    {
        AfxCopy(1, range, src, map);

        AfxUnmapBufferRange(buf);
    }
    return err;
}

////////////////////////////////////////////////////////////////////////////////

_AVX afxNat AfxEnumerateBuffers(afxDrawContext dctx, afxNat first, afxNat cnt, afxBuffer buffers[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    AfxAssert(buffers);
    AfxAssert(cnt);
    afxClass* cls = AfxGetBufferClass(dctx);
    AfxAssertClass(cls, afxFcc_BUF);
    return AfxEnumerateInstances(cls, first, cnt, (afxObject*)buffers);
}

_AVX afxError AfxAcquireBuffers(afxDrawContext dctx, afxNat cnt, afxBufferSpecification const spec[], afxBuffer buffers[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    AfxAssert(buffers);
    AfxAssert(spec);
    AfxAssert(cnt);

    afxClass* cls = AfxGetBufferClass(dctx);
    AfxAssertClass(cls, afxFcc_BUF);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)buffers, (void const*[]) { (void*)spec }))
        AfxThrowError();

    return err;
}
