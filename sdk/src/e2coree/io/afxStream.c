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

#define _AFX_CORE_C
#define _AFX_STREAM_C
#include "qwadro/core/afxClass.h"
#include "qwadro/io/afxStream.h"
#include "qwadro/core/afxSystem.h"
#include "qwadro/mem/afxMmu.h"
#include "qwadro/core/afxString.h"
#include "qwadro/async/afxThread.h"

// AUXILIAR FUNCTIONS /////////////////////////////////////////////////////////

_AFX afxBool _AfxStandardTestStreamEndConditionCallback(afxStream ios)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ios, afxFcc_IOS);
    afxBool b = (ios->posn == ios->range);
    return b;
}

_AFX afxError _AfxStandardStreamMoveCursorCallback(afxStream ios, afxInt offset, afxSeekMode origin)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ios, afxFcc_IOS);

    if (origin == afxSeekMode_ABSOLUTE)
    {
        AfxAssert(offset >= 0);
        AfxAssertRange(ios->range, (afxNat)offset, 1);

        if (ios->range <= (afxNat)offset)
            ios->posn = ios->range;
        else
            ios->posn = (afxNat)offset;
    }
    else if (origin == afxSeekMode_RELATIVE)
    {
        AfxAssertRange(ios->range, ios->posn + offset, 1);

        if (ios->posn + offset > ios->range)
            ios->posn = ios->range;
        else
            ios->posn += offset;
    }
    else if (origin == afxSeekMode_INVERSE)
    {
        AfxAssert(offset >= 0);
        AfxAssertRange(ios->range, (afxNat)offset, 1);

        if (ios->range <= (afxNat)offset)
            ios->posn = ios->range;
        else
            ios->posn = ios->range - offset;
    }
    else AfxThrowError();
    
    return err;
}

_AFX afxNat _AfxStandardStreamAskCursorCallback(afxStream ios)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ios, afxFcc_IOS);
    return ios->posn;
}

_AFX afxError _AfxStandardStreamWriteCallback(afxStream ios, void const* const src, afxNat siz)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ios, afxFcc_IOS);
    AfxAssert(siz);
    AfxAssert(src);
    afxNat clampedOffRange = siz;

    if ((siz > (ios->range - ios->posn)) && !ios->isUserBuf) // if it is not a forked or user-buffered stream, we will try to enlarge it.
    {
        afxNat bufCap = AfxGetStreamBufferCap(ios);

        if (siz > (bufCap - ios->posn))
        {
            afxNat scaledSiz = bufCap + (siz - (bufCap - ios->posn));

            if (AfxReadjustStreamBuffer(ios, scaledSiz))
                AfxThrowError();
        }
    }

    afxNat writeLen = siz;

    if (siz > (ios->range - ios->posn))
    {
        writeLen = ios->range - ios->posn;
        AfxThrowError();
    }

    AfxCopy(1, writeLen, src, (void*)AfxGetStreamData(ios, ios->posn));
    clampedOffRange -= writeLen;
    ios->posn += writeLen;

    return clampedOffRange;
}

_AFX afxError _AfxStandardStreamReadCallback(afxStream ios, void *dst, afxNat siz)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ios, afxFcc_IOS);
    AfxAssert(siz);
    AfxAssert(dst);
    afxNat clampedOffRange = siz;

    afxNat readLen = siz;

    if (siz > ios->range - ios->posn)
    {
        readLen = ios->range - ios->posn;
        AfxThrowError();
    }

    AfxCopy(1, readLen, AfxGetStreamData(ios, ios->posn), dst);
    clampedOffRange -= readLen;
    ios->posn += readLen;

    return clampedOffRange;
}

_AFXINL void Reverse32(afxNat cnt, void* data)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(data);
    AfxAssert(cnt);
    afxNat32* p = data;

    for (afxNat i = cnt / 4; i-- > 0;)
    {
        *p = (((*p << 16) | *p & 0xFF00) << 8) | (((*p >> 16) | *p & 0xFF0000u) >> 8);
        ++p;
    }
}

// READ ONLY METHODS //////////////////////////////////////////////////////////

_AFXINL void const* AfxGetStreamData(afxStream const ios, afxSize offset)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ios, afxFcc_IOS);
    AfxAssertRange(ios->range, offset, 1);

    if (!ios->src.start)
        if (AfxReadjustStreamBuffer((afxStream)ios, ios->src.cap))
            AfxThrowError();

    return &(ios->src.start[offset]);
}

_AFXINL afxSize AfxGetStreamLength(afxStream const ios)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ios, afxFcc_IOS);
    return ios->range;
}

_AFXINL afxNat AfxGetStreamBufferCap(afxStream const ios)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ios, afxFcc_IOS);
    return ios->src.cap;
}

_AFXINL afxBool AfxStreamIsReadOnly(afxStream const ios)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ios, afxFcc_IOS);
    return AfxTestFlags(ios->flags, (afxIoFlag_R | ~afxIoFlag_W));
}

_AFXINL afxBool AfxStreamIsReadable(afxStream const ios)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ios, afxFcc_IOS);
    return AfxTestFlags(ios->flags, afxIoFlag_R);
}

_AFXINL afxBool AfxStreamIsWriteable(afxStream const ios)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ios, afxFcc_IOS);
    return AfxTestFlags(ios->flags, afxIoFlag_W);
}

_AFXINL afxBool AfxStreamIsExecutable(afxStream const ios)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ios, afxFcc_IOS);
    return AfxTestFlags(ios->flags, afxIoFlag_X);
}

_AFX afxBool AfxTestStreamEndCondition(afxStream ios)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ios, afxFcc_IOS);
    return ios->ioctl.eos(ios);
}

_AFX afxError AfxReadjustStreamBuffer(afxStream ios, afxNat siz)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ios, afxFcc_IOS);
    AfxAssert(siz);

    AfxAssert(!ios->isUserBuf);

    if (ios->isUserBuf) AfxThrowError();
    else
    {
        if ((siz > ios->src.cap) || (!ios->src.start))
        {
            afxArena* aren = AfxGetIoArena();
            AfxAssertType(aren, afxFcc_AREN);
            void const *oldData = ios->src.start;
            afxByte *start;
            afxNat alignedSiz;

            if (!siz)
            {
                alignedSiz = 0;
                start = NIL;
            }
            else
            {
                alignedSiz = AFX_ALIGN(siz, AfxGetIoBufferSize());                
                //void *start;

                if (!(start = AfxRequestArenaUnit(aren, alignedSiz))) AfxThrowError();
                else
                {
                    if (oldData)
                        AfxCopy(1, AfxGetStreamLength(ios), oldData, start);
                }
            }

            if (!err)
            {
                if (oldData)
                    AfxRecycleArenaUnit(aren, (void*)oldData, ios->src.cap);

                ios->src.cap = alignedSiz;
                ios->range = ios->src.cap;
                ios->src.start = start;

                if (ios->posn >= alignedSiz)
                    ios->posn = alignedSiz - 1;
            }
        }
    }
    return err;
}

_AFX afxError AfxSeekStream(afxStream ios, afxInt offset, afxSeekMode origin)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ios, afxFcc_IOS);
    AfxAssert(AfxTestFlags(ios->flags, afxIoFlag_X));
    AfxAssertRange(origin, afxSeekMode_ABSOLUTE, afxSeekMode_INVERSE);
    return ios->ioctl.seek(ios, offset, origin);
}

_AFX afxError AfxRecedeStream(afxStream ios, afxNat range)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ios, afxFcc_IOS);
    AfxAssert(AfxTestFlags(ios->flags, afxIoFlag_X));
    AfxAssert(range);
    return ios->ioctl.seek(ios, -(range), afxSeekMode_RELATIVE);
}

_AFX afxError AfxSkipStream(afxStream ios, afxInt range)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ios, afxFcc_IOS);
    AfxAssert(AfxTestFlags(ios->flags, afxIoFlag_X));
    AfxAssert(range);
    return ios->ioctl.seek(ios, range, afxSeekMode_RELATIVE);
}

_AFX afxError AfxGoToStreamEnd(afxStream ios, afxInt offset)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ios, afxFcc_IOS);
    AfxAssert(AfxTestFlags(ios->flags, afxIoFlag_X));
    return ios->ioctl.seek(ios, offset, afxSeekMode_INVERSE);
}

_AFX afxError AfxGoToStreamBegin(afxStream ios, afxInt offset)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ios, afxFcc_IOS);
    AfxAssert(AfxTestFlags(ios->flags, afxIoFlag_X));
    return ios->ioctl.seek(ios, offset, afxSeekMode_ABSOLUTE);
}

_AFX afxSize AfxMeasureStream(afxStream ios)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ios, afxFcc_IOS);
    AfxAssert(AfxTestFlags(ios->flags, afxIoFlag_X));
    afxSize curr = ios->ioctl.tell(ios);
    ios->ioctl.seek(ios, 0, afxSeekMode_INVERSE);
    afxSize range = ios->ioctl.tell(ios);
    ios->ioctl.seek(ios, curr, afxSeekMode_ABSOLUTE);
    return range;
}

_AFX afxSize AfxAskStreamPosn(afxStream ios)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ios, afxFcc_IOS);
    afxSize cur = ios->ioctl.tell(ios);
    AfxAssert(cur == ios->posn);
    return cur;
}

_AFX afxError AfxCopyStreamRange(afxStream in, afxSize base, afxNat range, afxNat rate, afxStream out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &out, afxFcc_IOS);
    AfxAssert(AfxStreamIsWriteable(out));
    AfxAssertObjects(1, &in, afxFcc_IOS);
    AfxAssert(AfxStreamIsReadable(in));
    afxArena *aren = AfxGetIoArena();
    AfxAssertType(aren, afxFcc_AREN);
    void *space = AfxRequestArenaUnit(aren, range);
    afxNat clampedOffRange = range;

    if (!space) AfxThrowError();
    else
    {
        if (AfxGoToStreamBegin(in, base)) AfxThrowError();
        else
        {
            afxNat batchSiz = rate ? rate : AfxGetIoBufferSize();
            afxNat batches = range / batchSiz;
            afxNat remain = range % batchSiz;
            afxNat nextSizR = 0, nextSizW = 0;
            afxNat errSiz = 0;

            for (afxNat i = 0; i < batches; i++)
            {
                nextSizR = batchSiz;

                if ((errSiz = AfxReadStream(in, space, nextSizR, rate)))
                {
                    AfxThrowError();
                    nextSizW = nextSizR - errSiz;
                }
                else
                {
                    nextSizW = nextSizR;
                }
                
                if ((errSiz = AfxWriteStream(out, space, nextSizW, rate)))
                {
                    AfxThrowError();
                    nextSizW -= errSiz;
                }

                clampedOffRange -= nextSizW;

                if (err)
                    break;
            }

            if (!err)
            {
                nextSizR = remain;

                if ((errSiz = AfxReadStream(in, space, nextSizR, rate)))
                {
                    AfxThrowError();
                    nextSizW -= errSiz;
                }
                else
                {
                    nextSizW = nextSizR;
                }

                if ((errSiz = AfxWriteStream(out, space, nextSizW, rate)))
                {
                    AfxThrowError();
                    nextSizW -= errSiz;
                }
                clampedOffRange -= nextSizW;
            }
        }
        AfxRecycleArenaUnit(aren, space, range);
    }
    return clampedOffRange;
}

_AFX afxError AfxCopyStream(afxStream in, afxNat rate, afxStream out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &out, afxFcc_IOS);
    AfxAssert(AfxStreamIsWriteable(out));
    AfxAssertObjects(1, &in, afxFcc_IOS);
    AfxAssert(AfxStreamIsReadable(in));
    afxNat clampedOffRange = AfxMeasureStream(in);

    if ((clampedOffRange = AfxCopyStreamRange(in, 0, clampedOffRange, rate, out)))
        AfxThrowError();

    return clampedOffRange;
}

_AFX afxError AfxWriteStream(afxStream ios, void const* src, afxNat range, afxNat rate)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ios, afxFcc_IOS);
    AfxAssert(AfxStreamIsWriteable(ios));
    AfxAssert(range);
    AfxAssert(src);
    afxNat clampedOffRange = range;

    if (rate)
    {
        afxByte const*bytemap = src;
        afxNat batchSiz = rate ? rate : AfxGetIoBufferSize();
        afxNat batches = range / batchSiz;
        afxNat remain = range % batchSiz;
        afxNat nextSiz = 0;
        afxNat nextOff = 0;
        afxNat errSiz = 0;

        for (afxNat i = 0; i < batches; i++)
        {
            nextSiz = batchSiz;

            if ((errSiz = ios->ioctl.write(ios, &bytemap[nextOff], nextSiz)))
            {
                AfxThrowError();
                nextSiz -= errSiz;
            }

            nextOff += nextSiz;
            clampedOffRange -= nextSiz;

            if (ios->ioctl.writeFeedback)
                ios->ioctl.writeFeedback(ios, nextSiz, ios->idd);

            if (err)
                break;
        }

        if (!err && remain)
        {
            nextSiz = remain;

            if (!(errSiz = ios->ioctl.write(ios, &bytemap[nextOff], nextSiz)))
            {
                AfxThrowError();
                nextSiz -= errSiz;
            }

            clampedOffRange -= nextSiz;

            if (ios->ioctl.writeFeedback)
                ios->ioctl.writeFeedback(ios, nextSiz, ios->idd);
        }
    }
    else
    {
        if ((clampedOffRange = ios->ioctl.write(ios, src, range)))
            AfxThrowError();
    }
    return clampedOffRange;
}

_AFX afxError AfxWriteStreamAt(afxStream ios, afxSize offset, void const *src, afxNat range, afxNat rate)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ios, afxFcc_IOS);
    AfxAssert(AfxStreamIsWriteable(ios));
    AfxAssert(range);
    AfxAssert(src);
    afxNat clampedOffRange = range;

    if (AfxSeekStream(ios, offset, afxSeekMode_ABSOLUTE)) AfxThrowError();
    else if ((clampedOffRange = AfxWriteStream(ios, src, range, rate)))
        AfxThrowError();

    return clampedOffRange;
}

_AFX afxError AfxWriteStream2(afxStream out, afxNat range, afxNat stride, void const* src, afxNat srcStride)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &out, afxFcc_IOS);
    AfxAssert(AfxStreamIsWriteable(out));
    AfxAssert(range);
    AfxAssert(src);
    afxNat clampedOffRange = range;

    afxNat unitSiz = AfxMinorNonZero(stride, srcStride); // minor non-zero

    afxByte const*src2 = src;
    afxNat batchSiz = (stride == srcStride) ? AfxGetIoBufferSize() : unitSiz;
    afxNat batchCnt = range / batchSiz;
    afxNat remain = range % batchSiz;
    afxNat nextSiz = batchSiz;
    afxNat errSiz = 0;

    for (afxNat i = 0; i < batchCnt; i++)
    {
        if ((errSiz = out->ioctl.write(out, src2, nextSiz)))
        {
            AfxThrowError();
            nextSiz -= errSiz;
        }

        clampedOffRange -= nextSiz;
        src2 += nextSiz;

        if (out->ioctl.writeFeedback)
            out->ioctl.writeFeedback(out, nextSiz, out->idd);

        if (err)
            break;
    }
    AfxThrowError(); // incomplete. see ReadStream2
    if (!err && remain)
    {
        nextSiz = remain;

        if ((errSiz = out->ioctl.write(out, src2, nextSiz)))
        {
            AfxThrowError();
            nextSiz -= errSiz;
        }

        clampedOffRange -= nextSiz;

        if (out->ioctl.writeFeedback)
            out->ioctl.writeFeedback(out, nextSiz, out->idd);
    }
    return clampedOffRange;
}

_AFX afxError AfxWriteStreamAt2(afxStream out, afxSize offset, afxNat range, afxNat stride, void const* src, afxNat srcStride)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &out, afxFcc_IOS);
    AfxAssert(AfxStreamIsWriteable(out));
    AfxAssert(range);
    AfxAssert(src);
    afxNat clampedOffRange = range;

    if (AfxSeekStream(out, offset, afxSeekMode_ABSOLUTE)) AfxThrowError();
    else if ((clampedOffRange = AfxWriteStream2(out, range, stride, src, srcStride)))
        AfxThrowError();

    return clampedOffRange;
}

_AFX afxError AfxWrite(afxStream out, afxNat cnt, afxNat siz, void const* src, afxNat srcStride)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &out, afxFcc_IOS);
    AfxAssert(AfxStreamIsWriteable(out));
    AfxAssert(srcStride);
    AfxAssert(cnt);
    AfxAssert(siz);
    AfxAssert(src);
    afxNat totalLen = cnt * siz;
    afxNat clampedOffRange = totalLen;
    afxByte*src2 = src;
    afxNat errSiz = 0;

    if (0 == srcStride || siz == srcStride)
    {
        afxNat batchSiz = 4096;
        afxNat batchCnt = totalLen / batchSiz;
        afxNat remainSiz = totalLen % batchSiz;

        for (afxNat i = 0; i < batchCnt; i++)
        {
            if ((errSiz = out->ioctl.write(out, src2, batchSiz)))
            {
                AfxThrowError();
                batchSiz -= errSiz;
            }

            clampedOffRange -= batchSiz;
            src2 += batchSiz;

            if (out->ioctl.writeFeedback)
                out->ioctl.writeFeedback(out, batchSiz, out->idd);

            if (err)
                break;
        }

        if (!err && remainSiz)
        {
            if ((errSiz = out->ioctl.write(out, src2, remainSiz)))
            {
                AfxThrowError();
                remainSiz -= errSiz;
            }

            clampedOffRange -= remainSiz;

            if (out->ioctl.writeFeedback)
                out->ioctl.writeFeedback(out, remainSiz, out->idd);
        }
    }
    else
    {
        afxNat unitSiz = siz;
        afxNat batchCnt = totalLen / srcStride;
        afxNat remainSiz = totalLen % srcStride;

        for (afxNat i = 0; i < batchCnt; i++)
        {
            if ((errSiz = out->ioctl.write(out, src2, unitSiz)))
            {
                AfxThrowError();
                unitSiz -= errSiz;
            }

            clampedOffRange -= unitSiz;
            src2 += unitSiz;

            if (out->ioctl.writeFeedback)
                out->ioctl.writeFeedback(out, unitSiz, out->idd);

            if (err)
                break;
        }

        if (!err && remainSiz)
        {
            if ((errSiz = out->ioctl.write(out, src2, remainSiz)))
            {
                AfxThrowError();
                remainSiz -= errSiz;
            }

            clampedOffRange -= remainSiz;

            if (out->ioctl.writeFeedback)
                out->ioctl.writeFeedback(out, remainSiz, out->idd);
        }
    }
    return clampedOffRange;
}

_AFX afxError AfxWriteAt(afxStream out, afxSize offset, afxNat cnt, afxNat siz, void const* src, afxNat srcStride)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &out, afxFcc_IOS);
    AfxAssert(AfxStreamIsWriteable(out));
    AfxAssert(cnt);
    AfxAssert(siz);
    AfxAssert(src);
    afxNat clampedOffRange = cnt * siz;

    if (AfxSeekStream(out, offset, afxSeekMode_ABSOLUTE)) AfxThrowError();
    else if ((clampedOffRange = AfxWrite(out, cnt, siz, src, srcStride)))
        AfxThrowError();

    return clampedOffRange;
}

_AFX afxError AfxReadStream(afxStream ios, void *dst, afxNat range, afxNat rate)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ios, afxFcc_IOS);
    AfxAssert(AfxStreamIsReadable(ios));
    AfxAssert(range);
    AfxAssert(dst);
    afxNat clampedOffRange = range;

    if (rate)
    {
        afxByte*bytemap = dst;
        afxNat batchSiz = rate ? rate : AfxGetIoBufferSize();
        afxNat batches = range / batchSiz;
        afxNat remain = range % batchSiz;
        afxNat nextSiz = 0;
        afxNat nextOff = 0;
        afxNat errSiz = 0;

        for (afxNat i = 0; i < batches; i++)
        {
            nextSiz = batchSiz;

            if ((errSiz = ios->ioctl.read(ios, &bytemap[nextOff], nextSiz)))
            {
                AfxThrowError();
                nextSiz -= errSiz;
            }

            nextOff += nextSiz;
            clampedOffRange -= nextSiz;

            if (ios->ioctl.readFeedback)
                ios->ioctl.readFeedback(ios, nextSiz, ios->idd);

            if (err)
                break;
        }

        if (!err && remain)
        {
            nextSiz = remain;

            if (!(errSiz = ios->ioctl.read(ios, &bytemap[nextOff], nextSiz)))
            {
                AfxThrowError();
                nextSiz -= errSiz;
            }

            clampedOffRange -= nextSiz;

            if (ios->ioctl.readFeedback)
                ios->ioctl.readFeedback(ios, nextSiz, ios->idd);
        }
    }
    else
    {
        if ((clampedOffRange = ios->ioctl.read(ios, dst, range)))
            AfxThrowError();
    }
    return clampedOffRange;
}

_AFX afxError AfxReadStreamAt(afxStream ios, afxSize offset, void *dst, afxNat range, afxNat rate)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ios, afxFcc_IOS);
    AfxAssert(AfxStreamIsReadable(ios));
    AfxAssert(range);
    AfxAssert(dst);
    afxNat clampedOffRange = range;

    if (AfxSeekStream(ios, offset, afxSeekMode_ABSOLUTE)) AfxThrowError();
    else if ((clampedOffRange = AfxReadStream(ios, dst, range, rate)))
        AfxThrowError();

    return clampedOffRange;
}

_AFX afxError AfxReadStream2(afxStream in, afxNat range, afxNat stride, void* dst, afxNat dstStride)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &in, afxFcc_IOS);
    AfxAssert(AfxStreamIsReadable(in));
    AfxAssert(dstStride);
    AfxAssert(stride);
    AfxAssert(range);
    AfxAssert(dst);
    afxNat clampedOffRange = range;
    afxByte*dst2 = dst;
    afxNat errSiz = 0;

    if (stride == dstStride)
    {
        afxNat batchSiz = 4096;
        afxNat batchCnt = range / batchSiz;
        afxNat remainSiz = range % batchSiz;
        
        for (afxNat i = 0; i < batchCnt; i++)
        {
            if ((errSiz = in->ioctl.read(in, dst2, batchSiz)))
            {
                AfxThrowError();
                batchSiz -= errSiz;
            }
            
            clampedOffRange -= batchSiz;
            dst2 += batchSiz;

            if (in->ioctl.readFeedback)
                in->ioctl.readFeedback(in, batchSiz, in->idd);

            if (err)
                break;
        }

        if (!err && remainSiz)
        {
            if ((errSiz = in->ioctl.read(in, dst2, remainSiz)))
            {
                AfxThrowError();
                remainSiz -= errSiz;
            }

            clampedOffRange -= remainSiz;

            if (in->ioctl.readFeedback)
                in->ioctl.readFeedback(in, remainSiz, in->idd);
        }
    }
    else
    {
        afxByte tmpBuf[4096];
        afxNat tmpBufSiz = AFX_COUNTOF(tmpBuf);
        afxNat unitSiz = AfxMinorNonZero(stride, dstStride); // minor non-zero
        afxNat unitCnt = range / unitSiz;
        afxNat remainSiz = range % unitSiz;

        if (unitSiz == stride)
        {
            afxNat unitsPerBatch = tmpBufSiz / unitCnt;
            afxNat batchCnt = unitCnt / unitsPerBatch;
            afxNat unitsRemain = unitCnt % unitsPerBatch;
            afxNat batchSiz = unitSiz * unitsPerBatch;
            afxNat remainSiz = unitsRemain * unitSiz;

            for (afxNat i = 0; i < batchCnt; i++)
            {
                if ((errSiz = in->ioctl.read(in, tmpBuf, batchSiz)))
                {
                    AfxThrowError();
                    batchSiz -= errSiz;
                }

                clampedOffRange -= batchSiz;
                AfxStream2(batchSiz / unitsPerBatch, tmpBuf, unitSiz, dst2, dstStride);
                dst2 += dstStride * unitsPerBatch;

                if (in->ioctl.readFeedback)
                    in->ioctl.readFeedback(in, batchSiz, in->idd);

                if (err)
                    break;
            }

            if (!err && remainSiz)
            {
                if ((errSiz = in->ioctl.read(in, dst2, remainSiz)))
                {
                    AfxThrowError();
                    remainSiz -= errSiz;
                }

                clampedOffRange -= remainSiz;
                AfxStream2(remainSiz / unitsPerBatch, tmpBuf, unitSiz, dst2, dstStride);

                if (in->ioctl.readFeedback)
                    in->ioctl.readFeedback(in, remainSiz, in->idd);
            }
        }
        else
        {
            for (afxNat i = 0; i < unitCnt; i++)
            {
                if ((errSiz = in->ioctl.read(in, tmpBuf, stride)))
                {
                    AfxThrowError();
                    stride -= errSiz;
                }

                clampedOffRange -= stride;
                AfxCopy(1, AfxMin(unitSiz, stride), tmpBuf, dst2);
                dst2 += dstStride;

                if (in->ioctl.readFeedback)
                    in->ioctl.readFeedback(in, stride, in->idd);

                if (err)
                    break;
            }

            if (!err && remainSiz)
            {
                if ((errSiz = in->ioctl.read(in, tmpBuf, remainSiz)))
                {
                    AfxThrowError();
                    stride -= errSiz;
                }

                clampedOffRange -= stride;
                AfxCopy(1, AfxMin(unitSiz, stride), tmpBuf, dst2);

                if (in->ioctl.readFeedback)
                    in->ioctl.readFeedback(in, stride, in->idd);
            }
        }
    }
    return clampedOffRange;
}

_AFX afxError AfxRead(afxStream in, afxNat cnt, afxNat siz, void* dst, afxNat dstStride)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &in, afxFcc_IOS);
    AfxAssert(AfxStreamIsReadable(in));
    AfxAssert(dstStride);
    AfxAssert(cnt);
    AfxAssert(siz);
    AfxAssert(dst);
    afxNat totalLen = cnt * siz;
    afxNat clampedOffRange = totalLen;
    afxByte*dst2 = dst;
    afxNat errSiz = 0;

    if (0 == dstStride || siz == dstStride)
    {
        afxNat batchSiz = 4096;
        afxNat batchCnt = totalLen / batchSiz;
        afxNat remainSiz = totalLen % batchSiz;

        for (afxNat i = 0; i < batchCnt; i++)
        {
            if ((errSiz = in->ioctl.read(in, dst2, batchSiz)))
            {
                AfxThrowError();
                batchSiz -= errSiz;
            }

            clampedOffRange -= batchSiz;
            dst2 += batchSiz;

            if (in->ioctl.readFeedback)
                in->ioctl.readFeedback(in, batchSiz, in->idd);

            if (err)
                break;
        }

        if (!err && remainSiz)
        {
            if ((errSiz = in->ioctl.read(in, dst2, remainSiz)))
            {
                AfxThrowError();
                remainSiz -= errSiz;
            }

            clampedOffRange -= remainSiz;

            if (in->ioctl.readFeedback)
                in->ioctl.readFeedback(in, remainSiz, in->idd);
        }
    }
    else
    {
        afxNat unitSiz = siz;
        afxNat batchCnt = totalLen / unitSiz;
        afxNat remainSiz = totalLen % unitSiz;

        for (afxNat i = 0; i < batchCnt; i++)
        {
            if ((errSiz = in->ioctl.read(in, dst2, unitSiz)))
            {
                AfxThrowError();
                unitSiz -= errSiz;
            }

            clampedOffRange -= unitSiz;
            dst2 += unitSiz;

            if (in->ioctl.readFeedback)
                in->ioctl.readFeedback(in, unitSiz, in->idd);

            if (err)
                break;
        }

        if (!err && remainSiz)
        {
            if ((errSiz = in->ioctl.read(in, dst2, remainSiz)))
            {
                AfxThrowError();
                remainSiz -= errSiz;
            }

            clampedOffRange -= remainSiz;

            if (in->ioctl.readFeedback)
                in->ioctl.readFeedback(in, remainSiz, in->idd);
        }
    }
    return clampedOffRange;
}

_AFX afxError AfxReadAt(afxStream in, afxSize offset, afxNat cnt, afxNat siz, void *dst, afxNat dstStride)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &in, afxFcc_IOS);
    AfxAssert(AfxStreamIsReadable(in));
    AfxAssert(cnt);
    AfxAssert(siz);
    AfxAssert(dst);
    afxNat clampedOffRange = cnt * siz;

    if (AfxSeekStream(in, offset, afxSeekMode_ABSOLUTE)) AfxThrowError();
    else if ((clampedOffRange = AfxRead(in, cnt, siz, dst, dstStride)))
        AfxThrowError();

    return clampedOffRange;
}

_AFX afxError AfxReadReversed(afxStream in, afxNat cnt, afxNat siz, void* dst, afxNat dstStride)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &in, afxFcc_IOS);
    AfxAssert(AfxStreamIsReadable(in));
    AfxAssert(cnt);
    AfxAssert(siz);
    AfxAssert(dst);
    afxNat clampedOffRange;

    if ((clampedOffRange = AfxRead(in, cnt, siz, dst, dstStride)))
        AfxThrowError();

    Reverse32((((cnt * siz) - clampedOffRange) + 3) & 0xFFFFFFFC, dst);
    return clampedOffRange;
}

_AFX afxError AfxReadReversedAt(afxStream in, afxSize offset, afxNat cnt, afxNat siz, void *dst, afxNat dstStride)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &in, afxFcc_IOS);
    AfxAssert(AfxStreamIsReadable(in));
    AfxAssert(cnt);
    AfxAssert(siz);
    AfxAssert(dst);
    afxNat clampedOffRange;

    if ((clampedOffRange = AfxReadAt(in, offset, cnt, siz, dst, dstStride)))
        AfxThrowError();

    Reverse32((((cnt * siz) - clampedOffRange) + 3) & 0xFFFFFFFC, dst);
    return clampedOffRange;
}

_AFX afxError AfxReadStreamAt2(afxStream in, afxSize offset, afxNat range, afxNat stride, void* dst, afxNat dstStride)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &in, afxFcc_IOS);
    AfxAssert(AfxStreamIsReadable(in));
    AfxAssert(range);
    AfxAssert(dst);
    afxNat clampedOffRange = range;

    if (AfxSeekStream(in, offset, afxSeekMode_ABSOLUTE)) AfxThrowError();
    else if ((clampedOffRange = AfxReadStream2(in, range, stride, dst, dstStride)))
        AfxThrowError();

    return clampedOffRange;
}

_AFX afxError AfxReadStreamReversed(afxStream in, void* dst, afxNat range, afxNat rate)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &in, afxFcc_IOS);
    AfxAssert(AfxStreamIsReadable(in));
    AfxAssert(range);
    AfxAssert(dst);
    afxNat clampedOffRange;

    if ((clampedOffRange = AfxReadStream(in, dst, range, rate)))
        AfxThrowError();

    Reverse32(((range - clampedOffRange) + 3) & 0xFFFFFFFC, dst);
    return clampedOffRange;
}

_AFX afxError AfxReadStreamReversedAt(afxStream in, afxSize offset, void* dst, afxNat range, afxNat rate)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &in, afxFcc_IOS);
    AfxAssert(AfxStreamIsReadable(in));
    AfxAssert(range);
    AfxAssert(dst);
    afxNat clampedOffRange;

    if ((clampedOffRange = AfxReadStreamAt(in, offset, dst, range, rate)))
        AfxThrowError();

    Reverse32(((range - clampedOffRange) + 3) & 0xFFFFFFFC, dst);
    return clampedOffRange;
}

_AFX afxStream AfxForkStreamRange(afxStream parent, afxSize offset, afxNat range)
{
    afxError err = AFX_ERR_NONE;
    AfxEntry("parent=%p,offset=%x,range=%u", parent, offset, range);
    AfxAssertObjects(1, &parent, afxFcc_IOS);
    AfxAssertRange(parent->range, offset, range);

    afxSize maxRange = parent->range;
    afxSize effectiveRange = range > maxRange ? maxRange : range;
    AfxAssert(effectiveRange == range);

    afxStream ios = AfxAcquireStream(0, parent->flags, AfxGetStreamData(parent, offset), effectiveRange);

    ios->ioctl = parent->ioctl;
    ios->dtor = parent->dtor;
    ios->idd = parent->idd;

    return ios;// range - effectiveRange;
}

_AFX afxStream AfxForkStream(afxStream parent)
{
    afxError err = AFX_ERR_NONE;
    AfxEntry("parent=%p", parent);
    AfxAssertObjects(1, &parent, afxFcc_IOS);
    //afxError clampedOffLen = 0;
    afxStream sub;

    if ((sub = AfxForkStreamRange(parent, 0, parent->range)))
        AfxThrowError();

    return sub;
}

_AFX afxResult _AfxIosDtor(afxStream ios)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ios, afxFcc_IOS);

    if (ios->dtor)
        ios->dtor(ios);

    if (ios->isUserBuf && ios->src.start)
    {
        AfxReadjustStreamBuffer(ios, 0);
    }

    return err;
}

_AFX afxError _AfxIosCtor(afxStream ios, afxCookie const* cookie)
{
    afxError err = AFX_ERR_NONE;

    afxSize cap = *((afxSize const*)cookie->udd[0]);
    afxIoFlags flags = *((afxIoFlags const*)cookie->udd[1]);
    void* start = ((void*)cookie->udd[2]);
    afxSize range = *((afxSize const*)cookie->udd[3]);

    afxArena* aren = AfxGetIoArena();
    AfxAssertType(aren, afxFcc_AREN);

    AfxAssignTypeFcc(ios, afxFcc_IOS);
    ios->flags = flags ? flags : afxIoFlag_RWX;

    ios->range = range;
    ios->posn = 0;
    ios->src.start = start;
    ios->isUserBuf = !!start;
    ios->src.cap = cap ? cap : (ios->isUserBuf ? 0 : AfxGetIoBufferSize());

    ios->ioctl.read = _AfxStandardStreamReadCallback;
    ios->ioctl.readFeedback = NIL;
    ios->ioctl.write = _AfxStandardStreamWriteCallback;
    ios->ioctl.writeFeedback = NIL;
    ios->ioctl.seek = _AfxStandardStreamMoveCursorCallback;
    ios->ioctl.eos = _AfxStandardTestStreamEndConditionCallback;
    ios->ioctl.tell = _AfxStandardStreamAskCursorCallback;
    ios->dtor = NIL;
    ios->idd = NIL;

    return err;
}

_AFX afxClassConfig const _AfxIosClsConfig =
{
    .fcc = afxFcc_IOS,
    .name = "I/O Stream",
    .unitsPerPage = 2,
    .size = sizeof(AFX_OBJECT(afxStream)),
    .mmu = NIL,
    .ctor = (void*)_AfxIosCtor,
    .dtor = (void*)_AfxIosDtor
};

////////////////////////////////////////////////////////////////////////////////

_AFX afxStream AfxAcquireStream(afxNat cap, afxIoFlags flags, void const* start, afxSize range)
{
    afxError err = AFX_ERR_NONE;
    AfxEntry("cap=%u,flags=%x,start=%p,range=%u", cap, flags, start, range);

    afxClass* cls = AfxGetStreamClass();
    AfxAssertClass(cls, afxFcc_IOS);
    afxStream ios;

    if (AfxAcquireObjects(cls, 1, (afxObject*)&ios, (void const*[]) { &cap, &flags, start, &range }))
        AfxThrowError();

    return ios;
}

_AFX afxStream AfxAcquireInputStream(afxNat cap, void const *start, afxSize range)
{
    afxError err = AFX_ERR_NONE;
    AfxEntry("cap=%u,start=%p,range=%u", cap, start, range);
    afxStream in;

    if ((in = AfxAcquireStream(cap, afxIoFlag_RX, start, range)))
        AfxThrowError();

    return in;
}

_AFX afxStream AfxAcquireOutputStream(afxNat cap, void *start, afxSize range)
{
    afxError err = AFX_ERR_NONE;
    AfxEntry("cap=%u,start=%p,range=%u", cap, start, range);
    afxStream out;

    if ((out = AfxAcquireStream(cap, afxIoFlag_WX, start, range)))
        AfxThrowError();

    return out;
}

_AFX afxNat AfxInvokeStreams(afxNat first, afxNat cnt, afxBool(*f)(afxStream, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(f);
    afxClass* cls = AfxGetStreamClass();
    AfxAssertClass(cls, afxFcc_IOS);
    return AfxInvokeInstances(cls, first, cnt, (void*)f, udd);
}

_AFX afxNat AfxEnumerateStreams(afxNat first, afxNat cnt, afxStream ios[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(ios);
    afxClass* cls = AfxGetStreamClass();
    AfxAssertClass(cls, afxFcc_IOS);
    return AfxEnumerateInstances(cls, first, cnt, (afxObject*)ios);
}

_AFX afxNat AfxCountStreams(void)
{
    afxError err = AFX_ERR_NONE;
    afxClass* cls = AfxGetStreamClass();
    AfxAssertClass(cls, afxFcc_IOS);
    return AfxCountInstances(cls);
}
