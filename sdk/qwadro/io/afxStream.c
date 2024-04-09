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

// This code is part of SIGMA Future Storage <https://sigmaco.org/future-storage>

#define _AFX_CORE_C
#define _AFX_STREAM_C
#include "qwadro/core/afxSystem.h"

// AUXILIAR FUNCTIONS /////////////////////////////////////////////////////////

_AFX afxBool _AfxStdStreamEosCb(afxStream iob)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &iob, afxFcc_IOB);
    afxBool b = (iob->idd.m.posn == iob->idd.m.bufCap);
    return b;
}

_AFX afxError _AfxStdStreamSeekCb(afxStream iob, afxInt offset, afxSeekMode origin)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &iob, afxFcc_IOB);

    if (origin == afxSeekMode_ABSOLUTE)
    {
        AfxAssert(offset >= 0);
        AfxAssertRange(iob->idd.m.bufCap, (afxNat)offset, 1);

        if (iob->idd.m.bufCap <= (afxNat)offset)
            iob->idd.m.posn = iob->idd.m.bufCap;
        else
            iob->idd.m.posn = (afxNat)offset;
    }
    else if (origin == afxSeekMode_RELATIVE)
    {
        AfxAssertRange(iob->idd.m.bufCap, iob->idd.m.posn + offset, 1);

        if (iob->idd.m.posn + offset > iob->idd.m.bufCap)
            iob->idd.m.posn = iob->idd.m.bufCap;
        else
            iob->idd.m.posn += offset;
    }
    else if (origin == afxSeekMode_INVERSE)
    {
        AfxAssert(offset >= 0);
        AfxAssertRange(iob->idd.m.bufCap, (afxNat)offset, 1);

        if (iob->idd.m.bufCap <= (afxNat)offset)
            iob->idd.m.posn = iob->idd.m.bufCap;
        else
            iob->idd.m.posn = iob->idd.m.bufCap - offset;
    }
    else AfxThrowError();
    
    return err;
}

_AFX afxNat _AfxStdStreamTellCb(afxStream iob)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &iob, afxFcc_IOB);
    return iob->idd.m.posn;
}

_AFX afxError _AfxStdStreamWriteCb(afxStream iob, void const* const src, afxNat siz)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &iob, afxFcc_IOB);
    AfxAssert(siz);
    AfxAssert(src);
    afxNat clampedOffRange = siz;

    if ((siz > (iob->idd.m.bufCap - iob->idd.m.posn)) && !iob->idd.m.isUserBuf) // if it is not a forked or user-buffered stream, we will try to enlarge it.
    {
        afxNat bufCap = AfxGetStreamBufferCap(iob);

        if (siz > (bufCap - iob->idd.m.posn))
        {
            afxNat scaledSiz = bufCap + (siz - (bufCap - iob->idd.m.posn));

            if (AfxAdjustStreamBuffer(iob, scaledSiz))
                AfxThrowError();
        }
    }

    afxNat writeLen = siz;

    if (siz > (iob->idd.m.bufCap - iob->idd.m.posn))
    {
        writeLen = iob->idd.m.bufCap - iob->idd.m.posn;
        AfxThrowError();
    }

    AfxCopy2(1, writeLen, src, (void*)AfxGetStreamBuffer(iob, iob->idd.m.posn));
    clampedOffRange -= writeLen;
    iob->idd.m.posn += writeLen;

    return clampedOffRange;
}

_AFX afxError _AfxStdStreamReadCb(afxStream iob, void *dst, afxNat siz)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &iob, afxFcc_IOB);
    AfxAssert(siz);
    AfxAssert(dst);
    afxNat clampedOffRange = siz;

    afxNat readLen = siz;

    if (siz > iob->idd.m.bufCap - iob->idd.m.posn)
    {
        readLen = iob->idd.m.bufCap - iob->idd.m.posn;
        AfxThrowError();
    }

    AfxCopy2(1, readLen, AfxGetStreamBuffer(iob, iob->idd.m.posn), dst);
    clampedOffRange -= readLen;
    iob->idd.m.posn += readLen;

    return clampedOffRange;
}

_AFX _afxIobImpl const stdStreamImpl = 
{
    .read = _AfxStdStreamReadCb,
    .write = _AfxStdStreamWriteCb,
    .tell = _AfxStdStreamTellCb,
    .seek = _AfxStdStreamSeekCb,
    .eos = _AfxStdStreamEosCb
};

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

_AFXINL void const* AfxGetStreamBuffer(afxStream const iob, afxSize offset)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &iob, afxFcc_IOB);
    AfxAssertRange(iob->idd.m.bufCap, offset, 1);

    if (!iob->idd.m.buf)
        if (AfxAdjustStreamBuffer((afxStream)iob, iob->idd.m.bufCap))
            AfxThrowError();

    return &(iob->idd.m.buf[offset]);
}

_AFXINL afxSize AfxGetStreamLength(afxStream const iob)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &iob, afxFcc_IOB);
    return iob->idd.m.bufCap;
}

_AFXINL afxNat AfxGetStreamBufferCap(afxStream const iob)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &iob, afxFcc_IOB);
    return iob->idd.m.bufCap;
}

_AFXINL afxBool AfxStreamIsReadOnly(afxStream const iob)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &iob, afxFcc_IOB);
    return AfxTestFlags(iob->flags, (afxIoFlag_R | ~afxIoFlag_W));
}

_AFXINL afxBool AfxStreamIsReadable(afxStream const iob)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &iob, afxFcc_IOB);
    return AfxTestFlags(iob->flags, afxIoFlag_R);
}

_AFXINL afxBool AfxStreamIsWriteable(afxStream const iob)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &iob, afxFcc_IOB);
    return AfxTestFlags(iob->flags, afxIoFlag_W);
}

_AFXINL afxBool AfxStreamIsExecutable(afxStream const iob)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &iob, afxFcc_IOB);
    return AfxTestFlags(iob->flags, afxIoFlag_X);
}

_AFX afxBool AfxStreamReachedEnd(afxStream iob)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &iob, afxFcc_IOB);
    return iob->pimpl->eos(iob);
}

_AFX afxError AfxAdjustStreamBuffer(afxStream iob, afxNat siz)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &iob, afxFcc_IOB);

    AfxAssert(!iob->idd.m.isUserBuf);

    if (iob->idd.m.isUserBuf) AfxThrowError();
    else
    {
        if ((siz > iob->idd.m.bufCap) || (!iob->idd.m.buf))
        {
            void const *oldData = iob->idd.m.buf;
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

                if (!(start = AfxAllocate(1, alignedSiz, 0, AfxHere()))) AfxThrowError();
                else
                {
                    if (oldData)
                        AfxCopy2(1, AfxGetStreamLength(iob), oldData, start);
                }
            }

            if (!err)
            {
                if (oldData)
                    AfxDeallocate((void*)oldData);

                iob->idd.m.bufCap = alignedSiz;
                iob->idd.m.bufCap = iob->idd.m.bufCap;
                iob->idd.m.buf = start;

                if (iob->idd.m.posn >= alignedSiz)
                    iob->idd.m.posn = alignedSiz - 1;
            }
        }
    }
    return err;
}

_AFXINL afxError AfxSeekStream(afxStream iob, afxInt offset, afxSeekMode origin)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &iob, afxFcc_IOB);
    //AfxAssert(AfxTestFlags(iob->flags, afxIoFlag_X));
    AfxAssertRange(3, afxSeekMode_ABSOLUTE, afxSeekMode_INVERSE);
    return iob->pimpl->seek(iob, offset, origin);
}

_AFXINL afxError AfxRewindStream(afxStream iob)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &iob, afxFcc_IOB);

    if (AfxSeekStreamFromBegin(iob, 0))
        AfxThrowError();

    return err;
}

_AFXINL afxError AfxRecedeStream(afxStream iob, afxNat range)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &iob, afxFcc_IOB);
    //AfxAssert(AfxTestFlags(iob->flags, afxIoFlag_X));
    AfxAssert(range);
    return iob->pimpl->seek(iob, -(range), afxSeekMode_RELATIVE);
}

_AFXINL afxError AfxAdvanceStream(afxStream iob, afxNat range)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &iob, afxFcc_IOB);
    //AfxAssert(AfxTestFlags(iob->flags, afxIoFlag_X));
    AfxAssert(range);
    return iob->pimpl->seek(iob, range, afxSeekMode_RELATIVE);
}

_AFXINL afxError AfxSeekStreamFromEnd(afxStream iob, afxNat offset)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &iob, afxFcc_IOB);
    //AfxAssert(AfxTestFlags(iob->flags, afxIoFlag_X));
    return iob->pimpl->seek(iob, -(offset), afxSeekMode_INVERSE);
}

_AFXINL afxError AfxSeekStreamFromBegin(afxStream iob, afxNat offset)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &iob, afxFcc_IOB);
    //AfxAssert(AfxTestFlags(iob->flags, afxIoFlag_X));
    return iob->pimpl->seek(iob, offset, afxSeekMode_ABSOLUTE);
}

_AFXINL afxSize AfxMeasureStream(afxStream iob)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &iob, afxFcc_IOB);
    //AfxAssert(AfxTestFlags(iob->flags, afxIoFlag_X));
    afxSize curr = iob->pimpl->tell(iob);
    iob->pimpl->seek(iob, 0, afxSeekMode_INVERSE);
    afxSize range = iob->pimpl->tell(iob);
    iob->pimpl->seek(iob, curr, afxSeekMode_ABSOLUTE);
    return range;
}

_AFXINL afxSize AfxGetStreamPosn(afxStream iob)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &iob, afxFcc_IOB);
    afxSize cur = iob->pimpl->tell(iob);
    AfxAssert(cur == iob->idd.m.posn);
    return cur;
}

_AFX afxError AfxCopyStreamRange(afxStream in, afxSize base, afxNat range, afxNat rate, afxStream out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &out, afxFcc_IOB);
    AfxAssert(AfxStreamIsWriteable(out));
    AfxAssertObjects(1, &in, afxFcc_IOB);
    AfxAssert(AfxStreamIsReadable(in));

    void *space = AfxAllocate(1, range, 0, AfxHere());
    afxNat clampedOffRange = range;

    if (!space) AfxThrowError();
    else
    {
        if (AfxSeekStreamFromBegin(in, base)) AfxThrowError();
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

                if ((errSiz = AfxReadStream(in, nextSizR, rate, space)))
                {
                    AfxThrowError();
                    nextSizW = nextSizR - errSiz;
                }
                else
                {
                    nextSizW = nextSizR;
                }
                
                if ((errSiz = AfxWriteStream(out, nextSizW, rate, space)))
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

                if ((errSiz = AfxReadStream(in, nextSizR, rate, space)))
                {
                    AfxThrowError();
                    nextSizW -= errSiz;
                }
                else
                {
                    nextSizW = nextSizR;
                }

                if ((errSiz = AfxWriteStream(out, nextSizW, rate, space)))
                {
                    AfxThrowError();
                    nextSizW -= errSiz;
                }
                clampedOffRange -= nextSizW;
            }
        }
        AfxDeallocate(space);
    }
    return clampedOffRange;
}

_AFX afxError AfxCopyStream(afxStream in, afxNat rate, afxStream out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &out, afxFcc_IOB);
    AfxAssert(AfxStreamIsWriteable(out));
    AfxAssertObjects(1, &in, afxFcc_IOB);
    AfxAssert(AfxStreamIsReadable(in));
    afxNat clampedOffRange = AfxMeasureStream(in);

    if ((clampedOffRange = AfxCopyStreamRange(in, 0, clampedOffRange, rate, out)))
        AfxThrowError();

    return clampedOffRange;
}

_AFX afxError AfxWriteStream(afxStream iob, afxNat range, afxNat rate, void const* src)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &iob, afxFcc_IOB);
    AfxAssert(AfxStreamIsWriteable(iob));
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

            if ((errSiz = iob->pimpl->write(iob, &bytemap[nextOff], nextSiz)))
            {
                AfxThrowError();
                nextSiz -= errSiz;
            }

            nextOff += nextSiz;
            clampedOffRange -= nextSiz;

            if (iob->pimpl->writeFeedback)
                iob->pimpl->writeFeedback(iob, nextSiz, &iob->idd);

            if (err)
                break;
        }

        if (!err && remain)
        {
            nextSiz = remain;

            if (!(errSiz = iob->pimpl->write(iob, &bytemap[nextOff], nextSiz)))
            {
                AfxThrowError();
                nextSiz -= errSiz;
            }

            clampedOffRange -= nextSiz;

            if (iob->pimpl->writeFeedback)
                iob->pimpl->writeFeedback(iob, nextSiz, &iob->idd);
        }
    }
    else
    {
        if ((clampedOffRange = iob->pimpl->write(iob, src, range)))
            AfxThrowError();
    }
    return clampedOffRange;
}

_AFX afxError AfxWriteStreamAt(afxStream iob, afxSize offset, afxNat range, afxNat rate, void const* src)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &iob, afxFcc_IOB);
    AfxAssert(AfxStreamIsWriteable(iob));
    AfxAssert(range);
    AfxAssert(src);
    afxNat clampedOffRange = range;

    if (AfxSeekStreamFromBegin(iob, offset)) AfxThrowError();
    else if ((clampedOffRange = AfxWriteStream(iob, range, rate, src)))
        AfxThrowError();

    return clampedOffRange;
}

_AFX afxError AfxWriteStream2(afxStream out, afxNat range, afxNat stride, void const* src, afxNat srcStride)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &out, afxFcc_IOB);
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
        if ((errSiz = out->pimpl->write(out, src2, nextSiz)))
        {
            AfxThrowError();
            nextSiz -= errSiz;
        }

        clampedOffRange -= nextSiz;
        src2 += nextSiz;

        if (out->pimpl->writeFeedback)
            out->pimpl->writeFeedback(out, nextSiz, &out->idd);

        if (err)
            break;
    }
    AfxThrowError(); // incomplete. see ReadStream2
    if (!err && remain)
    {
        nextSiz = remain;

        if ((errSiz = out->pimpl->write(out, src2, nextSiz)))
        {
            AfxThrowError();
            nextSiz -= errSiz;
        }

        clampedOffRange -= nextSiz;

        if (out->pimpl->writeFeedback)
            out->pimpl->writeFeedback(out, nextSiz, &out->idd);
    }
    return clampedOffRange;
}

_AFX afxError AfxWriteStreamAt2(afxStream out, afxSize offset, afxNat range, afxNat stride, void const* src, afxNat srcStride)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &out, afxFcc_IOB);
    AfxAssert(AfxStreamIsWriteable(out));
    AfxAssert(range);
    AfxAssert(src);
    afxNat clampedOffRange = range;

    if (AfxSeekStreamFromBegin(out, offset)) AfxThrowError();
    else if ((clampedOffRange = AfxWriteStream2(out, range, stride, src, srcStride)))
        AfxThrowError();

    return clampedOffRange;
}

_AFX afxError AfxWrite(afxStream out, afxNat cnt, afxNat siz, void const* src, afxNat srcStride)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &out, afxFcc_IOB);
    AfxAssert(AfxStreamIsWriteable(out));
    AfxAssert(srcStride);
    AfxAssert(cnt);
    AfxAssert(siz);
    AfxAssert(src);
    afxNat totalLen = cnt * siz;
    afxNat clampedOffRange = totalLen;
    afxByte const*src2 = src;
    afxNat errSiz = 0;

    if (0 == srcStride || siz == srcStride)
    {
        afxNat batchSiz = 4096;
        afxNat batchCnt = totalLen / batchSiz;
        afxNat remainSiz = totalLen % batchSiz;

        for (afxNat i = 0; i < batchCnt; i++)
        {
            if ((errSiz = out->pimpl->write(out, src2, batchSiz)))
            {
                AfxThrowError();
                batchSiz -= errSiz;
            }

            clampedOffRange -= batchSiz;
            src2 += batchSiz;

            if (out->pimpl->writeFeedback)
                out->pimpl->writeFeedback(out, batchSiz, &out->idd);

            if (err)
                break;
        }

        if (!err && remainSiz)
        {
            if ((errSiz = out->pimpl->write(out, src2, remainSiz)))
            {
                AfxThrowError();
                remainSiz -= errSiz;
            }

            clampedOffRange -= remainSiz;

            if (out->pimpl->writeFeedback)
                out->pimpl->writeFeedback(out, remainSiz, &out->idd);
        }
    }
    else
    {
        afxNat unitSiz = siz;
        afxNat batchCnt = totalLen / srcStride;
        afxNat remainSiz = totalLen % srcStride;

        for (afxNat i = 0; i < batchCnt; i++)
        {
            if ((errSiz = out->pimpl->write(out, src2, unitSiz)))
            {
                AfxThrowError();
                unitSiz -= errSiz;
            }

            clampedOffRange -= unitSiz;
            src2 += unitSiz;

            if (out->pimpl->writeFeedback)
                out->pimpl->writeFeedback(out, unitSiz, &out->idd);

            if (err)
                break;
        }

        if (!err && remainSiz)
        {
            if ((errSiz = out->pimpl->write(out, src2, remainSiz)))
            {
                AfxThrowError();
                remainSiz -= errSiz;
            }

            clampedOffRange -= remainSiz;

            if (out->pimpl->writeFeedback)
                out->pimpl->writeFeedback(out, remainSiz, &out->idd);
        }
    }
    return clampedOffRange;
}

_AFX afxError AfxWriteAt(afxStream out, afxSize offset, afxNat cnt, afxNat siz, void const* src, afxNat srcStride)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &out, afxFcc_IOB);
    AfxAssert(AfxStreamIsWriteable(out));
    AfxAssert(cnt);
    AfxAssert(siz);
    AfxAssert(src);
    afxNat clampedOffRange = cnt * siz;

    if (AfxSeekStreamFromBegin(out, offset)) AfxThrowError();
    else if ((clampedOffRange = AfxWrite(out, cnt, siz, src, srcStride)))
        AfxThrowError();

    return clampedOffRange;
}

_AFX afxError AfxReadStream(afxStream iob, afxNat range, afxNat rate, void *dst)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &iob, afxFcc_IOB);
    AfxAssert(AfxStreamIsReadable(iob));
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

            if ((errSiz = iob->pimpl->read(iob, &bytemap[nextOff], nextSiz)))
            {
                AfxThrowError();
                nextSiz -= errSiz;
            }

            nextOff += nextSiz;
            clampedOffRange -= nextSiz;

            if (iob->pimpl->readFeedback)
                iob->pimpl->readFeedback(iob, nextSiz, &iob->idd);

            if (err)
                break;
        }

        if (!err && remain)
        {
            nextSiz = remain;

            if (!(errSiz = iob->pimpl->read(iob, &bytemap[nextOff], nextSiz)))
            {
                AfxThrowError();
                nextSiz -= errSiz;
            }

            clampedOffRange -= nextSiz;

            if (iob->pimpl->readFeedback)
                iob->pimpl->readFeedback(iob, nextSiz, &iob->idd);
        }
    }
    else
    {
        if ((clampedOffRange = iob->pimpl->read(iob, dst, range)))
            AfxThrowError();
    }
    return clampedOffRange;
}

_AFX afxError AfxReadStreamAt(afxStream iob, afxSize offset, afxNat range, afxNat rate, void *dst)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &iob, afxFcc_IOB);
    AfxAssert(AfxStreamIsReadable(iob));
    AfxAssert(range);
    AfxAssert(dst);
    afxNat clampedOffRange = range;

    if (AfxSeekStreamFromBegin(iob, offset)) AfxThrowError();
    else if ((clampedOffRange = AfxReadStream(iob, range, rate, dst)))
        AfxThrowError();

    return clampedOffRange;
}

_AFX afxError AfxReadStream2(afxStream in, afxNat range, afxNat stride, void* dst, afxNat dstStride)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &in, afxFcc_IOB);
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
            if ((errSiz = in->pimpl->read(in, dst2, batchSiz)))
            {
                AfxThrowError();
                batchSiz -= errSiz;
            }
            
            clampedOffRange -= batchSiz;
            dst2 += batchSiz;

            if (in->pimpl->readFeedback)
                in->pimpl->readFeedback(in, batchSiz, &in->idd);

            if (err)
                break;
        }

        if (!err && remainSiz)
        {
            if ((errSiz = in->pimpl->read(in, dst2, remainSiz)))
            {
                AfxThrowError();
                remainSiz -= errSiz;
            }

            clampedOffRange -= remainSiz;

            if (in->pimpl->readFeedback)
                in->pimpl->readFeedback(in, remainSiz, &in->idd);
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
                if ((errSiz = in->pimpl->read(in, tmpBuf, batchSiz)))
                {
                    AfxThrowError();
                    batchSiz -= errSiz;
                }

                clampedOffRange -= batchSiz;
                AfxStream2(batchSiz / unitsPerBatch, tmpBuf, unitSiz, dst2, dstStride);
                dst2 += dstStride * unitsPerBatch;

                if (in->pimpl->readFeedback)
                    in->pimpl->readFeedback(in, batchSiz, &in->idd);

                if (err)
                    break;
            }

            if (!err && remainSiz)
            {
                if ((errSiz = in->pimpl->read(in, dst2, remainSiz)))
                {
                    AfxThrowError();
                    remainSiz -= errSiz;
                }

                clampedOffRange -= remainSiz;
                AfxStream2(remainSiz / unitsPerBatch, tmpBuf, unitSiz, dst2, dstStride);

                if (in->pimpl->readFeedback)
                    in->pimpl->readFeedback(in, remainSiz, &in->idd);
            }
        }
        else
        {
            for (afxNat i = 0; i < unitCnt; i++)
            {
                if ((errSiz = in->pimpl->read(in, tmpBuf, stride)))
                {
                    AfxThrowError();
                    stride -= errSiz;
                }

                clampedOffRange -= stride;
                AfxCopy2(1, AfxMin(unitSiz, stride), tmpBuf, dst2);
                dst2 += dstStride;

                if (in->pimpl->readFeedback)
                    in->pimpl->readFeedback(in, stride, &in->idd);

                if (err)
                    break;
            }

            if (!err && remainSiz)
            {
                if ((errSiz = in->pimpl->read(in, tmpBuf, remainSiz)))
                {
                    AfxThrowError();
                    stride -= errSiz;
                }

                clampedOffRange -= stride;
                AfxCopy2(1, AfxMin(unitSiz, stride), tmpBuf, dst2);

                if (in->pimpl->readFeedback)
                    in->pimpl->readFeedback(in, stride, &in->idd);
            }
        }
    }
    return clampedOffRange;
}

_AFX afxError AfxRead(afxStream in, afxNat cnt, afxNat siz, void* dst, afxNat dstStride)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &in, afxFcc_IOB);
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
            if ((errSiz = in->pimpl->read(in, dst2, batchSiz)))
            {
                AfxThrowError();
                batchSiz -= errSiz;
            }

            clampedOffRange -= batchSiz;
            dst2 += batchSiz;

            if (in->pimpl->readFeedback)
                in->pimpl->readFeedback(in, batchSiz, &in->idd);

            if (err)
                break;
        }

        if (!err && remainSiz)
        {
            if ((errSiz = in->pimpl->read(in, dst2, remainSiz)))
            {
                AfxThrowError();
                remainSiz -= errSiz;
            }

            clampedOffRange -= remainSiz;

            if (in->pimpl->readFeedback)
                in->pimpl->readFeedback(in, remainSiz, &in->idd);
        }
    }
    else
    {
        afxNat unitSiz = siz;
        afxNat batchCnt = totalLen / unitSiz;
        afxNat remainSiz = totalLen % unitSiz;

        for (afxNat i = 0; i < batchCnt; i++)
        {
            if ((errSiz = in->pimpl->read(in, dst2, unitSiz)))
            {
                AfxThrowError();
                unitSiz -= errSiz;
            }

            clampedOffRange -= unitSiz;
            dst2 += unitSiz;

            if (in->pimpl->readFeedback)
                in->pimpl->readFeedback(in, unitSiz, &in->idd);

            if (err)
                break;
        }

        if (!err && remainSiz)
        {
            if ((errSiz = in->pimpl->read(in, dst2, remainSiz)))
            {
                AfxThrowError();
                remainSiz -= errSiz;
            }

            clampedOffRange -= remainSiz;

            if (in->pimpl->readFeedback)
                in->pimpl->readFeedback(in, remainSiz, &in->idd);
        }
    }
    return clampedOffRange;
}

_AFX afxError AfxReadAt(afxStream in, afxSize offset, afxNat cnt, afxNat siz, void *dst, afxNat dstStride)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &in, afxFcc_IOB);
    AfxAssert(AfxStreamIsReadable(in));
    AfxAssert(cnt);
    AfxAssert(siz);
    AfxAssert(dst);
    afxNat clampedOffRange = cnt * siz;

    if (AfxSeekStreamFromBegin(in, offset)) AfxThrowError();
    else if ((clampedOffRange = AfxRead(in, cnt, siz, dst, dstStride)))
        AfxThrowError();

    return clampedOffRange;
}

_AFX afxError AfxReadReversed(afxStream in, afxNat cnt, afxNat siz, void* dst, afxNat dstStride)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &in, afxFcc_IOB);
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
    AfxAssertObjects(1, &in, afxFcc_IOB);
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
    AfxAssertObjects(1, &in, afxFcc_IOB);
    AfxAssert(AfxStreamIsReadable(in));
    AfxAssert(range);
    AfxAssert(dst);
    afxNat clampedOffRange = range;

    if (AfxSeekStreamFromBegin(in, offset)) AfxThrowError();
    else if ((clampedOffRange = AfxReadStream2(in, range, stride, dst, dstStride)))
        AfxThrowError();

    return clampedOffRange;
}

_AFX afxError AfxReadStreamReversed(afxStream in, afxNat range, afxNat rate, void* dst)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &in, afxFcc_IOB);
    AfxAssert(AfxStreamIsReadable(in));
    AfxAssert(range);
    AfxAssert(dst);
    afxNat clampedOffRange;

    if ((clampedOffRange = AfxReadStream(in, range, rate, dst)))
        AfxThrowError();

    Reverse32(((range - clampedOffRange) + 3) & 0xFFFFFFFC, dst);
    return clampedOffRange;
}

_AFX afxError AfxReadStreamReversedAt(afxStream in, afxSize offset, afxNat range, afxNat rate, void* dst)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &in, afxFcc_IOB);
    AfxAssert(AfxStreamIsReadable(in));
    AfxAssert(range);
    AfxAssert(dst);
    afxNat clampedOffRange;

    if ((clampedOffRange = AfxReadStreamAt(in, offset, range, rate, dst)))
        AfxThrowError();

    Reverse32(((range - clampedOffRange) + 3) & 0xFFFFFFFC, dst);
    return clampedOffRange;
}

_AFX afxStream AfxForkStreamRange(afxStream parent, afxSize offset, afxNat range)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &parent, afxFcc_IOB);
    AfxAssertRange(parent->idd.m.bufCap, offset, range);

    afxSize maxRange = parent->idd.m.bufCap;
    afxSize effectiveRange = range > maxRange ? maxRange : range;
    AfxAssert(effectiveRange == range);

    afxStream iob = AfxOpenStream(parent->flags, AfxGetStreamBuffer(parent, offset), effectiveRange);

    iob->pimpl = parent->pimpl;
    iob->idd = parent->idd;

    return iob;// range - effectiveRange;
}

_AFX afxStream AfxForkStream(afxStream parent)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &parent, afxFcc_IOB);
    //afxError clampedOffLen = 0;
    afxStream sub;

    if ((sub = AfxForkStreamRange(parent, 0, parent->idd.m.bufCap)))
        AfxThrowError();

    return sub;
}

_AFX afxBool AfxCloseStream(afxStream iob)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &iob, afxFcc_IOB);
    return AfxReleaseObjects(1, (void*[]) { iob });
}

_AFX afxResult _AfxIosDtor(afxStream iob)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &iob, afxFcc_IOB);

    if (iob->pimpl->dtor)
        iob->pimpl->dtor(iob);

    if (!iob->idd.m.isUserBuf && iob->idd.m.buf)
    {
        AfxAdjustStreamBuffer(iob, 0);
    }

    return err;
}

_AFX afxError _AfxIosCtor(afxStream iob, afxCookie const* cookie)
{
    afxError err = AFX_ERR_NONE;

    afxIoFlags flags = *((afxIoFlags const*)cookie->udd[0]);
    _afxIobImpl const* pimpl = cookie->udd[1];
    _afxIobIdd const* idd = cookie->udd[2];

    AfxAssignTypeFcc(iob, afxFcc_IOB);
    iob->flags = flags ? flags : afxIoFlag_RWX;
    iob->idd.m.posn = 0;
    
    iob->idd = *idd;
    iob->idd.m.isUserBuf = !!iob->idd.m.buf;

    if (&stdStreamImpl == (iob->pimpl = pimpl))
    {

        afxNat bufSiz;

        if ((flags & afxIoFlag_R) && !(bufSiz = iob->idd.m.bufCap)) AfxThrowError();
        else if ((flags & afxIoFlag_W) && !(bufSiz = iob->idd.m.bufCap))
            AfxThrowError();
        else
        {
            if (!iob->idd.m.isUserBuf)
                if (AfxAdjustStreamBuffer(iob, bufSiz))
                    AfxThrowError();
        }
    }
    return err;
}

_AFX afxClassConfig const _AfxIosMgrCfg =
{
    .fcc = afxFcc_IOB,
    .name = "Stream",
    .desc = "I/O Bufferization",
    .unitsPerPage = 2,
    .size = sizeof(AFX_OBJECT(afxStream)),
    .mmu = NIL,
    .ctor = (void*)_AfxIosCtor,
    .dtor = (void*)_AfxIosDtor
};

////////////////////////////////////////////////////////////////////////////////

_AFX afxStream AfxAcquireImplementedStream(afxIoFlags flags, _afxIobImpl const* pimpl, _afxIobIdd const* idd)
{
    afxError err = AFX_ERR_NONE;

    afxManager* cls = AfxGetStreamClass();
    AfxAssertClass(cls, afxFcc_IOB);
    afxStream iob;

    if (AfxAcquireObjects(cls, 1, (afxObject*)&iob, (void const*[]) { &flags, pimpl, idd }))
        AfxThrowError();

    return iob;
}

_AFX afxStream AfxAcquireStream(afxIoFlags flags, afxNat bufCap)
{
    afxError err = AFX_ERR_NONE;
    afxStream iob;

    _afxIobIdd idd = { 0 };
    idd.m.bufCap = bufCap ? bufCap : 512;
    idd.m.buf = NIL;
    idd.m.isUserBuf = FALSE;

    if (!(iob = AfxAcquireImplementedStream(flags, &stdStreamImpl, &idd)))
        AfxThrowError();

    return iob;
}

_AFX afxStream AfxOpenStream(afxIoFlags flags, void const* start, afxSize siz)
{
    afxError err = AFX_ERR_NONE;
    afxStream iob = NIL;
    AfxAssert(flags);

    _afxIobIdd idd = { 0 };
    idd.m.bufCap = siz;
    idd.m.buf = (afxByte*)start;
    idd.m.isUserBuf = TRUE;

    if (!siz) AfxThrowError();
    else if (!start) AfxThrowError();
    else if (!(iob = AfxAcquireImplementedStream(flags, &stdStreamImpl, &idd)))
        AfxThrowError();

    return iob;
}

_AFX afxStream AfxOpenInputStream(void const *start, afxSize len)
{
    afxError err = AFX_ERR_NONE;
    afxStream in = NIL;

    if (!start) AfxThrowError();
    else if(!len) AfxThrowError();
    else if (!(in = AfxOpenStream(afxIoFlag_R, start, len)))
        AfxThrowError();

    return in;
}

_AFX afxStream AfxOpenOutputStream(void* buf, afxNat bufCap)
{
    afxError err = AFX_ERR_NONE;
    afxStream out = NIL;

    if (!bufCap) AfxThrowError();
    else if (!buf) AfxThrowError();
    else if (!(out = AfxOpenStream(afxIoFlag_W, buf, bufCap)))
        AfxThrowError();

    return out;
}

_AFX afxNat AfxInvokeStreams(afxNat first, afxNat cnt, afxBool(*f)(afxStream, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(f);
    afxManager* cls = AfxGetStreamClass();
    AfxAssertClass(cls, afxFcc_IOB);
    return AfxInvokeObjects(cls, first, cnt, (void*)f, udd);
}

_AFX afxNat AfxEnumerateStreams(afxNat first, afxNat cnt, afxStream streams[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(streams);
    afxManager* cls = AfxGetStreamClass();
    AfxAssertClass(cls, afxFcc_IOB);
    return AfxEnumerateObjects(cls, first, cnt, (afxObject*)streams);
}

_AFX afxNat AfxCountStreams(void)
{
    afxError err = AFX_ERR_NONE;
    afxManager* cls = AfxGetStreamClass();
    AfxAssertClass(cls, afxFcc_IOB);
    return AfxCountObjects(cls);
}
