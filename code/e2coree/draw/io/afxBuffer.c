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
#define _AFX_BUFFER_C
#include "qwadro/draw/afxDrawContext.h"


_AFX afxError AfxDumpBuffer2(afxBuffer buf, afxSize offset, afxSize stride, afxSize cnt, void *dst, afxSize dstStride)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &buf, afxFcc_BUF);

    afxSize siz = AfxGetBufferSize(buf);
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

_AFX afxError AfxDumpBuffer(afxBuffer buf, afxSize offset, afxSize range, void *dst)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &buf, afxFcc_BUF);

    afxSize siz = AfxGetBufferSize(buf);
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

_AFX afxError AfxUpdateBufferRegion(afxBuffer buf, afxBufferRegion const* rgn, void const* src, afxSize srcStride)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    AfxAssert(rgn);

    afxSize bufSiz = AfxGetBufferSize(buf);
    AfxAssertRange(bufSiz, rgn->base, rgn->range);
    AfxAssert(src);
    AfxAssert(srcStride);

    afxByte *map = AfxMapBufferRange(buf, rgn->base, rgn->range, afxBufferAccess_W);

    if (!map) AfxThrowError();
    else
    {
        if ((srcStride == rgn->stride) && rgn->stride)
        {
            AfxCopy(1, rgn->range, src, map);
        }
        else
        {
            afxByte const *src2 = src;
            afxNat unitSiz = AfxMinorNonZero(rgn->stride, srcStride); // minor non-zero
            AfxAssert(unitSiz == rgn->unitSiz);
            afxNat cnt = rgn->range / rgn->stride;
            afxNat cnt2 = rgn->range % rgn->stride;
            AfxAssert(!cnt2);

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

_AFX afxError AfxUpdateBuffer2(afxBuffer buf, afxSize offset, afxSize range, afxSize stride, void const *src, afxSize srcStride)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &buf, afxFcc_BUF);

    afxSize bufSiz = AfxGetBufferSize(buf);
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

_AFX afxError AfxUpdateBuffer(afxBuffer buf, afxSize offset, afxSize range, void const *src)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &buf, afxFcc_BUF);

    afxSize siz = AfxGetBufferSize(buf);
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

    return buf->map(buf, offset, range, flags);
}

_AFX void AfxUnmapBufferRange(afxBuffer buf)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &buf, afxFcc_BUF);

    buf->unmap(buf);
}

_AFX afxError AfxAcquireBuffers(afxDrawContext dctx, afxNat cnt, afxBufferSpecification const spec[], afxBuffer buf[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(spec);
    AfxAssert(buf);
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);

    afxClass* cls = AfxGetBufferClass(dctx);
    AfxAssertClass(cls, afxFcc_BUF);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)buf, (void const*[]) { (void*)spec }))
        AfxThrowError();

    return err;
}
