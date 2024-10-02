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
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This code is part of SIGMA Future Storage <https://sigmaco.org/future-storage>

#define _AFX_CORE_C
#define _AFX_STREAM_C
#include "../dev/afxIoImplKit.h"

// AUXILIAR FUNCTIONS /////////////////////////////////////////////////////////

_AFX afxNat _AfxStdStreamTellCb(afxStream iob)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &iob, afxFcc_IOB);
    return iob->posn;
}

_AFX afxBool _AfxStdStreamEosCb(afxStream iob)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &iob, afxFcc_IOB);
    afxBool b = (iob->posn == iob->bufCap);
    return b;
}

_AFX afxError _AfxStdStreamSeekCb(afxStream iob, afxSize offset, afxSeekOrigin origin)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &iob, afxFcc_IOB);

    if (origin == afxSeekOrigin_BEGIN)
    {
        AfxAssert(offset >= 0);
        AfxAssertRange(iob->bufCap, (afxNat)offset, 1);

        if (iob->bufCap <= (afxNat)offset)
            iob->posn = iob->bufCap;
        else
            iob->posn = (afxNat)offset;
    }
    else if (origin == afxSeekOrigin_CURR)
    {
        AfxAssertRange(iob->bufCap, iob->posn + offset, 1);

        if (iob->posn + offset > iob->bufCap)
            iob->posn = iob->bufCap;
        else
            iob->posn += offset;
    }
    else if (origin == afxSeekOrigin_END)
    {
        AfxAssert(offset >= 0);
        AfxAssertRange(iob->bufCap, (afxNat)offset, 1);

        if (iob->bufCap <= (afxNat)offset)
            iob->posn = iob->bufCap;
        else
            iob->posn = iob->bufCap - offset;
    }
    else AfxThrowError();
    
    return err;
}

_AFX afxError _AfxStdStreamWriteCb(afxStream iob, void const* const src, afxNat32 siz)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &iob, afxFcc_IOB);
    AfxAssert(siz);
    AfxAssert(src);
    afxNat clampedOffRange = siz;

    if ((siz > (iob->bufCap - iob->posn)) && !iob->isUserBuf) // if it is not a forked or user-buffered stream, we will try to enlarge it.
    {
        afxNat bufCap = AfxGetStreamCapacity(iob);

        if (siz > (bufCap - iob->posn))
        {
            afxNat scaledSiz = bufCap + (siz - (bufCap - iob->posn));

            if (AfxAdjustStreamBuffer(iob, scaledSiz))
                AfxThrowError();
        }
    }

    afxNat writeLen = siz;

    if (siz > (iob->bufCap - iob->posn))
    {
        writeLen = iob->bufCap - iob->posn;
        AfxThrowError();
    }

    AfxCopy((void*)AfxGetStreamBuffer(iob, iob->posn), src, writeLen);
    clampedOffRange -= writeLen;
    iob->posn += writeLen;

    return clampedOffRange;
}

_AFX afxError _AfxStdStreamReadCb(afxStream iob, void *dst, afxNat32 siz)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &iob, afxFcc_IOB);
    AfxAssert(siz);
    AfxAssert(dst);
    afxNat clampedOffRange = siz;

    afxNat readLen = siz;

    if (siz > iob->bufCap - iob->posn)
    {
        readLen = iob->bufCap - iob->posn;
        AfxThrowError();
    }

    AfxCopy(dst, AfxGetStreamBuffer(iob, iob->posn), readLen);
    clampedOffRange -= readLen;
    iob->posn += readLen;

    return clampedOffRange;
}

_AFX afxIobImpl const stdStreamImpl = 
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
    AfxAssertRange(iob->bufCap, offset, 1);

    if (!iob->buf)
        if (AfxAdjustStreamBuffer((afxStream)iob, iob->bufCap))
            AfxThrowError();

    return err ? NIL : &(iob->buf[offset]);
}

_AFXINL afxSize AfxGetStreamRoom(afxStream const iob)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &iob, afxFcc_IOB);
    return iob->bufCap - iob->length;
}

_AFXINL afxSize AfxGetStreamLength(afxStream const iob)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &iob, afxFcc_IOB);
    return iob->length;
}

_AFXINL afxNat AfxGetStreamCapacity(afxStream const iob)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &iob, afxFcc_IOB);
    return iob->bufCap;
}

_AFXINL afxBool AfxIsStreamReadOnly(afxStream const iob)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &iob, afxFcc_IOB);
    return AfxTestFlags(iob->flags, (afxIoFlag_R | ~afxIoFlag_W));
}

_AFXINL afxBool AfxIsStreamReadable(afxStream const iob)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &iob, afxFcc_IOB);
    return AfxTestFlags(iob->flags, afxIoFlag_R);
}

_AFXINL afxBool AfxIsStreamWriteable(afxStream const iob)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &iob, afxFcc_IOB);
    return AfxTestFlags(iob->flags, afxIoFlag_W);
}

_AFXINL afxBool AfxStreamIsSeekable(afxStream const iob)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &iob, afxFcc_IOB);
    return AfxTestFlags(iob->flags, afxIoFlag_X);
}

_AFXINL afxBool AfxHasStreamReachedEnd(afxStream iob)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &iob, afxFcc_IOB);
    return iob->pimpl->eos(iob);
}

_AFXINL afxError AfxSeekStream(afxStream iob, afxSize offset, afxSeekOrigin origin)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &iob, afxFcc_IOB);
    //AfxAssert(AfxTestFlags(iob->flags, afxIoFlag_X));
    AfxAssertRange(3, afxSeekOrigin_BEGIN, afxSeekOrigin_END);
    return iob->pimpl->seek(iob, offset, origin);
}

_AFXINL afxError AfxRewindStream(afxStream iob)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &iob, afxFcc_IOB);

    if (AfxSeekStream(iob, 0, afxSeekOrigin_BEGIN))
        AfxThrowError();

    return err;
}

_AFXINL afxError AfxRecedeStream(afxStream iob, afxNat range)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &iob, afxFcc_IOB);
    //AfxAssert(AfxTestFlags(iob->flags, afxIoFlag_X));
    AfxAssert(range);

    if (!range) AfxThrowError();
    else if (AfxSeekStream(iob, /*-*/range, afxSeekOrigin_CURR))
        AfxThrowError();

    return err;
}

_AFXINL afxError AfxAdvanceStream(afxStream iob, afxNat range)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &iob, afxFcc_IOB);
    //AfxAssert(AfxTestFlags(iob->flags, afxIoFlag_X));
    AfxAssert(range);

    if (!range) AfxThrowError();
    else if (AfxSeekStream(iob, range, afxSeekOrigin_CURR))
        AfxThrowError();

    return err;
}

_AFXINL afxSize AfxMeasureStream(afxStream iob)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &iob, afxFcc_IOB);
    //AfxAssert(AfxTestFlags(iob->flags, afxIoFlag_X));
    
    // back up the stream
    afxSize bkpPosn = AfxAskStreamPosn(iob);
    
    AfxSeekStream(iob, 0, afxSeekOrigin_END);
    afxSize len = AfxAskStreamPosn(iob);
    
    // restore stream
    AfxSeekStream(iob, bkpPosn, afxSeekOrigin_BEGIN);

    return len;
}

_AFXINL afxSize AfxAskStreamPosn(afxStream iob)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &iob, afxFcc_IOB);
    afxSize posn = iob->pimpl->tell(iob);
    //AfxAssert(cur == iob->idd.m.posn);
    return posn;
}

_AFX afxError AfxAdjustStreamBuffer(afxStream iob, afxNat siz)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &iob, afxFcc_IOB);

    AfxAssert(!iob->isUserBuf);

    if (iob->isUserBuf) AfxThrowError();
    else
    {
        if ((siz > iob->bufCap) || (!iob->buf) || !siz)
        {
            void const *oldData = iob->buf;
            afxByte *start;
            afxNat alignedSiz;

            if (!siz)
            {
                alignedSiz = 0;
                start = NIL;
            }
            else
            {
                alignedSiz = AFX_ALIGNED_SIZEOF(siz, AfxGetIoBufferSize());                
                //void *start;

                if (!(start = AfxAllocate(1, alignedSiz, 0, AfxHere()))) AfxThrowError();
                else
                {
                    if (oldData)
                        AfxCopy(start, oldData, AfxGetStreamLength(iob));
                }
            }

            if (!err)
            {
                if (oldData)
                    AfxDeallocate((void*)oldData);

                iob->bufCap = alignedSiz;
                iob->bufCap = iob->bufCap;
                iob->buf = start;

                if (iob->posn >= alignedSiz)
                    iob->posn = alignedSiz - 1;
            }
        }
    }
    return err;
}

_AFX afxError AfxWriteStream(afxStream iob, afxNat range, afxNat rate, void const* src)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &iob, afxFcc_IOB);
    AfxAssert(AfxIsStreamWriteable(iob));
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
    AfxAssert(AfxIsStreamWriteable(iob));
    AfxAssert(range);
    AfxAssert(src);
    afxNat clampedOffRange = range;

    if (AfxSeekStream(iob, offset, afxSeekOrigin_BEGIN)) AfxThrowError();
    else if ((clampedOffRange = AfxWriteStream(iob, range, rate, src)))
        AfxThrowError();

    return clampedOffRange;
}

_AFX afxError AfxWriteStream2(afxStream out, afxNat range, afxNat stride, void const* src, afxNat srcStride)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &out, afxFcc_IOB);
    AfxAssert(AfxIsStreamWriteable(out));
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
    AfxAssert(AfxIsStreamWriteable(out));
    AfxAssert(range);
    AfxAssert(src);
    afxNat clampedOffRange = range;

    if (AfxSeekStream(out, offset, afxSeekOrigin_BEGIN)) AfxThrowError();
    else if ((clampedOffRange = AfxWriteStream2(out, range, stride, src, srcStride)))
        AfxThrowError();

    return clampedOffRange;
}

_AFX afxError AfxWrite(afxStream out, afxNat cnt, afxNat siz, void const* src, afxNat srcStride)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &out, afxFcc_IOB);
    AfxAssert(AfxIsStreamWriteable(out));
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
    AfxAssert(AfxIsStreamWriteable(out));
    AfxAssert(cnt);
    AfxAssert(siz);
    AfxAssert(src);
    afxNat clampedOffRange = cnt * siz;

    if (AfxSeekStream(out, offset, afxSeekOrigin_BEGIN)) AfxThrowError();
    else if ((clampedOffRange = AfxWrite(out, cnt, siz, src, srcStride)))
        AfxThrowError();

    return clampedOffRange;
}

_AFX afxError AfxReadStream(afxStream iob, afxNat range, afxNat rate, void *dst)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &iob, afxFcc_IOB);
    AfxAssert(AfxIsStreamReadable(iob));
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
    AfxAssert(AfxIsStreamReadable(iob));
    AfxAssert(range);
    AfxAssert(dst);
    afxNat clampedOffRange = range;

    if (AfxSeekStream(iob, offset, afxSeekOrigin_BEGIN)) AfxThrowError();
    else if ((clampedOffRange = AfxReadStream(iob, range, rate, dst)))
        AfxThrowError();

    return clampedOffRange;
}

_AFX afxError AfxReadStream2(afxStream in, afxNat range, afxNat stride, void* dst, afxNat dstStride)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &in, afxFcc_IOB);
    AfxAssert(AfxIsStreamReadable(in));
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
                AfxCopy(dst2, tmpBuf, AfxMin(unitSiz, stride));
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
                AfxCopy(dst2, tmpBuf, AfxMin(unitSiz, stride));

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
    AfxAssert(AfxIsStreamReadable(in));
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
    AfxAssert(AfxIsStreamReadable(in));
    AfxAssert(cnt);
    AfxAssert(siz);
    AfxAssert(dst);
    afxNat clampedOffRange = cnt * siz;

    if (AfxSeekStream(in, offset, afxSeekOrigin_BEGIN)) AfxThrowError();
    else if ((clampedOffRange = AfxRead(in, cnt, siz, dst, dstStride)))
        AfxThrowError();

    return clampedOffRange;
}

_AFX afxError AfxReadReversed(afxStream in, afxNat cnt, afxNat siz, void* dst, afxNat dstStride)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &in, afxFcc_IOB);
    AfxAssert(AfxIsStreamReadable(in));
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
    AfxAssert(AfxIsStreamReadable(in));
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
    AfxAssert(AfxIsStreamReadable(in));
    AfxAssert(range);
    AfxAssert(dst);
    afxNat clampedOffRange = range;

    if (AfxSeekStream(in, offset, afxSeekOrigin_BEGIN)) AfxThrowError();
    else if ((clampedOffRange = AfxReadStream2(in, range, stride, dst, dstStride)))
        AfxThrowError();

    return clampedOffRange;
}

_AFX afxError AfxReadStreamReversed(afxStream in, afxNat range, afxNat rate, void* dst)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &in, afxFcc_IOB);
    AfxAssert(AfxIsStreamReadable(in));
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
    AfxAssert(AfxIsStreamReadable(in));
    AfxAssert(range);
    AfxAssert(dst);
    afxNat clampedOffRange;

    if ((clampedOffRange = AfxReadStreamAt(in, offset, range, rate, dst)))
        AfxThrowError();

    Reverse32(((range - clampedOffRange) + 3) & 0xFFFFFFFC, dst);
    return clampedOffRange;
}

_AFX afxError AfxMergeStream(afxStream out, afxStream in, afxSize base, afxNat range, afxNat rate)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &out, afxFcc_IOB);
    AfxAssert(AfxIsStreamWriteable(out));
    AfxAssertObjects(1, &in, afxFcc_IOB);
    AfxAssert(AfxIsStreamReadable(in));

    void *space = AfxAllocate(1, range, 0, AfxHere());
    afxNat clampedOffRange = range;

    if (!space) AfxThrowError();
    else
    {
        if (AfxSeekStream(in, base, afxSeekOrigin_BEGIN)) AfxThrowError();
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

#if 0 // MERGE/FORK

_AFX afxError AfxCopyStreamRange(afxStream in, afxSize base, afxNat range, afxNat rate, afxStream out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &out, afxFcc_IOB);
    AfxAssert(AfxIsStreamWriteable(out));
    AfxAssertObjects(1, &in, afxFcc_IOB);
    AfxAssert(AfxIsStreamReadable(in));

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
    AfxAssert(AfxIsStreamWriteable(out));
    AfxAssertObjects(1, &in, afxFcc_IOB);
    AfxAssert(AfxIsStreamReadable(in));
    afxNat clampedOffRange = AfxMeasureStream(in);

    if ((clampedOffRange = AfxCopyStreamRange(in, 0, clampedOffRange, rate, out)))
        AfxThrowError();

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

    afxStream iob = NIL;
    AfxOpenStream(parent->flags, AfxGetStreamBuffer(parent, offset), effectiveRange, &iob);

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
#endif

_AFX afxBool AfxResetStream(afxStream iob)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &iob, afxFcc_IOB);

    if (iob->pimpl->dtor)
        iob->pimpl->dtor(iob);

    if (!iob->isUserBuf && iob->buf)
    {
        AfxAdjustStreamBuffer(iob, 0);
        AfxAssert(!iob->buf);
    }

    return TRUE;
}

_AFX afxResult _AfxIosDtorCb(afxStream iob)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &iob, afxFcc_IOB);

    AfxResetStream(iob);

    if (iob->pimpl->dtor)
        iob->pimpl->dtor(iob);

    if (!iob->isUserBuf && iob->buf)
    {
        AfxAdjustStreamBuffer(iob, 0);
        AfxAssert(!iob->buf);
    }

    return err;
}

_AFX afxError _AfxIosCtorCb(afxStream iob, void** args, afxNat invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &iob, afxFcc_IOB);

    afxStreamInfo const* info = ((afxStreamInfo const*)args[1]) + invokeNo;
    afxIobImpl const* pimpl = args[2];
    AfxAssert(info);

    AfxZero(&iob->idd, sizeof(iob->idd));

    iob->flags = info->flags ? info->flags : afxStreamFlag_READABLE | afxStreamFlag_WRITEABLE | afxStreamFlag_SEEKABLE;
    iob->bufCap = AfxGetIoBufferSize();
    iob->isUserBuf = FALSE;
    iob->buf = NIL;
    iob->usage = info->usage ? info->usage : afxStreamUsage_MEM;
    iob->length = 0;
    iob->pimpl = pimpl;
    iob->posn = 0;

    if (&stdStreamImpl == iob->pimpl)
    {
        afxNat bufSiz;

        if ((iob->flags & afxIoFlag_R) && !(bufSiz = iob->bufCap)) AfxThrowError();
        else if ((iob->flags & afxIoFlag_W) && !(bufSiz = iob->bufCap)) AfxThrowError();
        else
        {
            if (!iob->isUserBuf)
                if (AfxAdjustStreamBuffer(iob, bufSiz))
                    AfxThrowError();
        }
    }
    return err;
}

_AFX afxClassConfig const _AfxIosClsCfg =
{
    .fcc = afxFcc_IOB,
    .name = "Stream",
    .desc = "I/O Serialization Buffer",
    .fixedSiz = sizeof(AFX_OBJECT(afxStream)),
    .ctor = (void*)_AfxIosCtorCb,
    .dtor = (void*)_AfxIosDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AFX afxError AfxAcquireImplementedStream(afxIobImpl const* pimpl, afxNat cnt, afxStreamInfo const infos[], afxStream streams[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(streams);
    AfxAssert(infos);
    AfxAssert(pimpl);
    AfxAssert(cnt);

    if (!cnt || !infos || !streams) AfxThrowError();
    else
    {
        afxClass* cls = (afxClass*)AfxGetStreamClass();
        AfxAssertClass(cls, afxFcc_IOB);

        if (AfxAcquireObjects(cls, cnt, (afxObject*)streams, (void const*[]) { NIL, infos, pimpl })) AfxThrowError();
        else
        {
            AfxAssertObjects(cnt, streams, afxFcc_IOB);
        }
    }
    return err;
}

_AFX afxError AfxAcquireStream(afxNat cnt, afxStreamInfo const infos[], afxStream streams[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(infos);
    AfxAssert(streams);
    AfxAssert(cnt);

    if (!cnt || !infos || !streams) AfxThrowError();
    else
    {
        if (AfxAcquireImplementedStream(&stdStreamImpl, cnt, infos, streams)) AfxThrowError();
        else
        {
            AfxAssertObjects(cnt, streams, afxFcc_IOB);
        }
    }
    return err;
}

_AFX afxError AfxReopenStream(afxStream iob, void* buf, afxSize len)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &iob, afxFcc_IOB);
    
    if (!len) AfxThrowError();
    else if (!buf) AfxThrowError();
    else if (AfxAdjustStreamBuffer(iob, 0)) AfxThrowError();
    {
        iob->buf = buf;
        iob->isUserBuf = TRUE;
        iob->bufCap = len;
        iob->length = len;
        AfxRewindStream(iob);
    }
    return err;
}

_AFX afxError AfxReopenInputStream(afxStream iob, void const* start, afxSize len)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &iob, afxFcc_IOB);

    if (!start) AfxThrowError();
    else if(!len) AfxThrowError();
    else if (AfxReopenStream(iob, (void*)start, len))
        AfxThrowError();

    return err;
}

_AFX afxError AfxReopenOutputStream(afxStream iob, void* buf, afxNat bufCap)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &iob, afxFcc_IOB);

    if (!bufCap) AfxThrowError();
    else if (!buf) AfxThrowError();
    else if (AfxReopenStream(iob, buf, bufCap))
        AfxThrowError();

    return err;
}
