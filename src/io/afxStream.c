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

// This code is part of SIGMA Future Storage <https://sigmaco.org/future-storage>

#define _AFX_CORE_C
#define _AFX_STREAM_C
#include "afxIoDDK.h"
#include "../exec/afxSystemDDK.h"

// AUXILIAR FUNCTIONS /////////////////////////////////////////////////////////

_AFX afxError _AfxStdStreamWriteCb(afxStream iob, void const* const src, afxUnit32 siz)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &iob);
    AFX_ASSERT(siz);
    AFX_ASSERT(src);
    afxUnit clampedOffRange = siz;

    if ((siz > (iob->bufCap - iob->posn)) && !iob->isUserBuf) // if it is not a forked or user-buffered stream, we will try to enlarge it.
    {
        afxUnit bufCap = AfxGetStreamCapacity(iob);

        if (siz > (bufCap - iob->posn))
        {
            afxUnit scaledSiz = bufCap + (siz - (bufCap - iob->posn));

            if (AfxBufferizeStream(iob, scaledSiz, NIL))
                AfxThrowError();
        }
    }

    afxUnit writeLen = siz;

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

_AFX afxError _AfxStdStreamReadCb(afxStream iob, void *dst, afxUnit32 siz)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &iob);
    AFX_ASSERT(siz);
    AFX_ASSERT(dst);
    afxUnit clampedOffRange = siz;

    afxUnit readLen = siz;

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
};

_AFXINL void Reverse32(afxUnit cnt, void* data)
{
    afxError err = { 0 };
    AFX_ASSERT(data);
    AFX_ASSERT(cnt);
    afxUnit32* p = data;

    for (afxUnit i = cnt / 4; i-- > 0;)
    {
        *p = (((*p << 16) | *p & 0xFF00) << 8) | (((*p >> 16) | *p & 0xFF0000u) >> 8);
        ++p;
    }
}

// READ ONLY METHODS //////////////////////////////////////////////////////////

_AFXINL void const* AfxGetStreamBuffer(afxStream const iob, afxSize offset)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &iob);
    AFX_ASSERT_RANGE(iob->bufCap, offset, 1);

    if (!iob->buf && !iob->isUserBuf && iob->usage == afxIoUsage_MEM)
    {
        if (AfxBufferizeStream((afxStream)iob, iob->bufCap, NIL))
            AfxThrowError();
    }

    if (!iob->buf)
        AfxThrowError();
    
    return err ? NIL : &(iob->buf[offset]);
}

_AFXINL afxSize AfxGetStreamRoom(afxStream const iob)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &iob);
    return iob->bufCap - iob->length;
}

_AFXINL afxSize AfxGetStreamLength(afxStream const iob)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &iob);
    return iob->length;
}

_AFXINL afxUnit AfxGetStreamCapacity(afxStream const iob)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &iob);
    return iob->bufCap;
}

_AFXINL afxBool AfxIsStreamReadOnly(afxStream const iob)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &iob);
    return AfxTestFlags(iob->flags, (afxIoFlag_R | ~afxIoFlag_W));
}

_AFXINL afxBool AfxIsStreamReadable(afxStream const iob)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &iob);
    return AfxTestFlags(iob->flags, afxIoFlag_R);
}

_AFXINL afxBool AfxIsStreamWriteable(afxStream const iob)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &iob);
    return AfxTestFlags(iob->flags, afxIoFlag_W);
}

_AFXINL afxBool AfxStreamIsSeekable(afxStream const iob)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &iob);
    return AfxTestFlags(iob->flags, afxIoFlag_X);
}

_AFXINL afxBool AfxHasStreamReachedEnd(afxStream iob)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &iob);
    
    if (iob->pimpl->eos)
        return iob->pimpl->eos(iob);

    // case it is not overloaded, then...

    return (iob->posn == iob->bufCap);
}

_AFXINL afxError AfxSeekStream(afxStream iob, afxSize offset, afxSeekOrigin origin)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &iob);
    //AFX_ASSERT(AfxTestFlags(iob->flags, afxIoFlag_X));
    AFX_ASSERT_RANGE(3, afxSeekOrigin_BEGIN, afxSeekOrigin_END);
    
    if (iob->pimpl->seek)
    {
        afxUnit bkp = (origin == afxSeekOrigin_CURR) ? AfxAskStreamPosn(iob) : 0;

        err = iob->pimpl->seek(iob, offset, origin);

        if (err)
            AfxThrowError();
        else
        {
            if (origin == afxSeekOrigin_BEGIN)
            {
                AFX_ASSERT(offset == AfxAskStreamPosn(iob));
            }
            else if (origin == afxSeekOrigin_CURR)
            {
                AFX_ASSERT((bkp + offset) == AfxAskStreamPosn(iob));
            }
            else if (origin == afxSeekOrigin_END)
            {
                AFX_ASSERT((iob->length - offset) == AfxAskStreamPosn(iob));
            }
        }
        return err;
    }

    // case it is not overloaded, then...

    if (origin == afxSeekOrigin_BEGIN)
    {
        AFX_ASSERT(offset >= 0);
        AFX_ASSERT_RANGE(iob->bufCap, (afxUnit)offset, 1);

        if (iob->bufCap <= (afxUnit)offset)
            iob->posn = iob->bufCap;
        else
            iob->posn = (afxUnit)offset;
    }
    else if (origin == afxSeekOrigin_CURR)
    {
        AFX_ASSERT_RANGE(iob->bufCap, iob->posn + offset, 1);

        if (iob->posn + offset > iob->bufCap)
            iob->posn = iob->bufCap;
        else
            iob->posn += offset;
    }
    else if (origin == afxSeekOrigin_END)
    {
        AFX_ASSERT(offset >= 0);
        AFX_ASSERT_RANGE(iob->bufCap, (afxUnit)offset, 1);

        if (iob->bufCap <= (afxUnit)offset)
            iob->posn = iob->bufCap;
        else
            iob->posn = iob->bufCap - offset;
    }
    else AfxThrowError();

    return err;
}

_AFXINL afxError AfxRewindStream(afxStream iob)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &iob);

    if (AfxSeekStream(iob, 0, afxSeekOrigin_BEGIN))
    {
        AfxThrowError();
    }
    else
    {
        AFX_ASSERT(iob->posn == 0);
    }
    return err;
}

_AFXINL afxError AfxRecedeStream(afxStream iob, afxUnit range)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &iob);
    //AFX_ASSERT(AfxTestFlags(iob->flags, afxIoFlag_X));
    
    if (range)
    {
        afxUnit bkp = AfxAskStreamPosn(iob);

        if (AfxSeekStream(iob, /*-*/range, afxSeekOrigin_CURR))
            AfxThrowError();
        else
        {
            AFX_ASSERT((bkp + range) == AfxAskStreamPosn(iob));
        }
    }
    return err;
}

_AFXINL afxError AfxAdvanceStream(afxStream iob, afxUnit range)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &iob);
    //AFX_ASSERT(AfxTestFlags(iob->flags, afxIoFlag_X));
    
    if (range)
    {
        afxUnit bkp = AfxAskStreamPosn(iob);

        if (AfxSeekStream(iob, range, afxSeekOrigin_CURR))
            AfxThrowError();
        else
        {
            AFX_ASSERT((bkp - range) == AfxAskStreamPosn(iob));
        }
    }
    return err;
}

_AFXINL afxSize AfxMeasureStream(afxStream iob)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &iob);
    //AFX_ASSERT(AfxTestFlags(iob->flags, afxIoFlag_X));
    
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
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &iob);

    if (iob->pimpl->tell)
        return iob->pimpl->tell(iob);

    // case it is not overloaded, then...

    return iob->posn;
}

_AFX afxError AfxBufferizeStream(afxStream iob, afxUnit bufCap, void* buf)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &iob);
    AFX_ASSERT(!buf || bufCap); // if a buf is specified, it must have a capacity specified too.

    if (iob->pimpl->buf)
    {
        if (iob->pimpl->buf(iob, bufCap, buf))
            AfxThrowError();

        return err;
    }

    // case it is not overloaded, then...

    afxBool realloc = FALSE;

    if (!bufCap)
    {
        AFX_ASSERT(!buf);

        if (iob->buf)
        {
            if (iob->isUserBuf)
            {
                iob->buf = NIL;
                iob->isUserBuf = FALSE;
            }
            realloc = TRUE;
        }
    }
    else
    {
        if (!buf) realloc = TRUE;
        else
        {
            iob->buf = buf;
            iob->bufCap = bufCap;
            iob->isUserBuf = TRUE;
        }
    }

    if (!realloc)
    {
        return err;
    }

    AFX_ASSERT(!iob->isUserBuf);

    if ((bufCap > iob->bufCap) || (!iob->buf) || !bufCap)
    {
        void const *oldData = iob->buf;
        afxByte *start;
        afxUnit alignedSiz;

        if (!bufCap)
        {
            alignedSiz = 0;
            start = NIL;
        }
        else
        {
            alignedSiz = AFX_ALIGN_SIZE(bufCap, AfxGetIoBufferSize());
            //void *start;

            if (AfxAllocate(alignedSiz, 0, AfxHere(), (void**)&start)) AfxThrowError();
            else
            {
                if (oldData)
                    AfxCopy(start, oldData, AfxGetStreamLength(iob));
            }
        }

        if (!err)
        {
            if (oldData)
                AfxDeallocate((void**)&oldData, AfxHere());

            iob->bufCap = alignedSiz;
            iob->bufCap = iob->bufCap;
            iob->buf = start;

            if (iob->posn >= alignedSiz)
                iob->posn = alignedSiz ? alignedSiz - 1 : alignedSiz;
        }
    }
    return err;
}

_AFX afxError AfxWriteStream(afxStream iob, afxUnit range, afxUnit rate, void const* src)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &iob);
    AFX_ASSERT(AfxIsStreamWriteable(iob));
    AFX_ASSERT(range);
    AFX_ASSERT(src);
    afxUnit clampedOffRange = range;

    if (rate)
    {
        afxByte const*bytemap = src;
        afxUnit batchSiz = rate ? rate : AfxGetIoBufferSize();
        afxUnit batches = range / batchSiz;
        afxUnit remain = range % batchSiz;
        afxUnit nextSiz = 0;
        afxUnit nextOff = 0;
        afxUnit errSiz = 0;

        for (afxUnit i = 0; i < batches; i++)
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

_AFX afxError AfxWriteStreamAt(afxStream iob, afxSize offset, afxUnit range, afxUnit rate, void const* src)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &iob);
    AFX_ASSERT(AfxIsStreamWriteable(iob));
    AFX_ASSERT(range);
    AFX_ASSERT(src);
    afxUnit clampedOffRange = range;

    if (AfxSeekStream(iob, offset, afxSeekOrigin_BEGIN)) AfxThrowError();
    else if ((clampedOffRange = AfxWriteStream(iob, range, rate, src)))
        AfxThrowError();

    return clampedOffRange;
}

_AFX afxError AfxWriteStream2(afxStream out, afxUnit range, afxUnit stride, void const* src, afxUnit srcStride)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &out);
    AFX_ASSERT(AfxIsStreamWriteable(out));
    AFX_ASSERT(range);
    AFX_ASSERT(src);
    afxUnit clampedOffRange = range;

    afxUnit unitSiz = AFX_MIN_OR(stride, srcStride); // minor non-zero

    afxByte const*src2 = src;
    afxUnit batchSiz = (stride == srcStride) ? AfxGetIoBufferSize() : unitSiz;
    afxUnit batchCnt = range / batchSiz;
    afxUnit remain = range % batchSiz;
    afxUnit nextSiz = batchSiz;
    afxUnit errSiz = 0;

    for (afxUnit i = 0; i < batchCnt; i++)
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

_AFX afxError AfxWriteStreamAt2(afxStream out, afxSize offset, afxUnit range, afxUnit stride, void const* src, afxUnit srcStride)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &out);
    AFX_ASSERT(AfxIsStreamWriteable(out));
    AFX_ASSERT(range);
    AFX_ASSERT(src);
    afxUnit clampedOffRange = range;

    if (AfxSeekStream(out, offset, afxSeekOrigin_BEGIN)) AfxThrowError();
    else if ((clampedOffRange = AfxWriteStream2(out, range, stride, src, srcStride)))
        AfxThrowError();

    return clampedOffRange;
}

_AFX afxError AfxWrite(afxStream out, afxUnit cnt, afxUnit siz, void const* src, afxUnit srcStride)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &out);
    AFX_ASSERT(AfxIsStreamWriteable(out));
    AFX_ASSERT(srcStride);
    AFX_ASSERT(cnt);
    AFX_ASSERT(siz);
    AFX_ASSERT(src);
    afxUnit totalLen = cnt * siz;
    afxUnit clampedOffRange = totalLen;
    afxByte const*src2 = src;
    afxUnit errSiz = 0;

    if (0 == srcStride || siz == srcStride)
    {
        afxUnit batchSiz = 4096;
        afxUnit batchCnt = totalLen / batchSiz;
        afxUnit remainSiz = totalLen % batchSiz;

        for (afxUnit i = 0; i < batchCnt; i++)
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
        afxUnit unitSiz = siz;
        afxUnit batchCnt = totalLen / srcStride;
        afxUnit remainSiz = totalLen % srcStride;

        for (afxUnit i = 0; i < batchCnt; i++)
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

_AFX afxError AfxWriteAt(afxStream out, afxSize offset, afxUnit cnt, afxUnit siz, void const* src, afxUnit srcStride)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &out);
    AFX_ASSERT(AfxIsStreamWriteable(out));
    AFX_ASSERT(cnt);
    AFX_ASSERT(siz);
    AFX_ASSERT(src);
    afxUnit clampedOffRange = cnt * siz;

    if (AfxSeekStream(out, offset, afxSeekOrigin_BEGIN)) AfxThrowError();
    else if ((clampedOffRange = AfxWrite(out, cnt, siz, src, srcStride)))
        AfxThrowError();

    return clampedOffRange;
}

_AFX afxError AfxReadStream(afxStream iob, afxUnit range, afxUnit rate, void *dst)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &iob);
    AFX_ASSERT(AfxIsStreamReadable(iob));
    afxUnit clampedOffRange = range;
    
    if (range)
    {
        AFX_ASSERT(dst);

        if (rate)
        {
            afxByte*bytemap = dst;
            afxUnit batchSiz = rate ? rate : AfxGetIoBufferSize();
            afxUnit batches = range / batchSiz;
            afxUnit remain = range % batchSiz;
            afxUnit nextSiz = 0;
            afxUnit nextOff = 0;
            afxUnit errSiz = 0;

            for (afxUnit i = 0; i < batches; i++)
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
    }
    return clampedOffRange;
}

_AFX afxError AfxReadStreamAt(afxStream iob, afxSize offset, afxUnit range, afxUnit rate, void *dst)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &iob);
    AFX_ASSERT(AfxIsStreamReadable(iob));
    AFX_ASSERT(range);
    AFX_ASSERT(dst);
    afxUnit clampedOffRange = range;

    if (AfxSeekStream(iob, offset, afxSeekOrigin_BEGIN)) AfxThrowError();
    else if ((clampedOffRange = AfxReadStream(iob, range, rate, dst)))
        AfxThrowError();

    return clampedOffRange;
}

_AFX afxError AfxReadStream2(afxStream in, afxUnit range, afxUnit stride, void* dst, afxUnit dstStride)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &in);
    AFX_ASSERT(AfxIsStreamReadable(in));
    AFX_ASSERT(dstStride);
    AFX_ASSERT(stride);
    AFX_ASSERT(range);
    AFX_ASSERT(dst);
    afxUnit clampedOffRange = range;
    afxByte*dst2 = dst;
    afxUnit errSiz = 0;

    if (stride == dstStride)
    {
        afxUnit batchSiz = 4096;
        afxUnit batchCnt = range / batchSiz;
        afxUnit remainSiz = range % batchSiz;
        
        for (afxUnit i = 0; i < batchCnt; i++)
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
        afxUnit tmpBufSiz = ARRAY_SIZE(tmpBuf);
        afxUnit unitSiz = AFX_MIN_OR(stride, dstStride); // minor non-zero
        afxUnit unitCnt = range / unitSiz;
        afxUnit remainSiz = range % unitSiz;

        if (unitSiz == stride)
        {
            afxUnit unitsPerBatch = tmpBufSiz / unitCnt;
            afxUnit batchCnt = unitCnt / unitsPerBatch;
            afxUnit unitsRemain = unitCnt % unitsPerBatch;
            afxUnit batchSiz = unitSiz * unitsPerBatch;
            afxUnit remainSiz = unitsRemain * unitSiz;

            for (afxUnit i = 0; i < batchCnt; i++)
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
            for (afxUnit i = 0; i < unitCnt; i++)
            {
                if ((errSiz = in->pimpl->read(in, tmpBuf, stride)))
                {
                    AfxThrowError();
                    stride -= errSiz;
                }

                clampedOffRange -= stride;
                AfxCopy(dst2, tmpBuf, AFX_MIN(unitSiz, stride));
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
                AfxCopy(dst2, tmpBuf, AFX_MIN(unitSiz, stride));

                if (in->pimpl->readFeedback)
                    in->pimpl->readFeedback(in, stride, &in->idd);
            }
        }
    }
    return clampedOffRange;
}

_AFX afxError AfxRead(afxStream in, afxUnit cnt, afxUnit siz, void* dst, afxUnit dstStride)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &in);
    AFX_ASSERT(AfxIsStreamReadable(in));
    AFX_ASSERT(dstStride);
    AFX_ASSERT(cnt);
    AFX_ASSERT(siz);
    AFX_ASSERT(dst);
    afxUnit totalLen = cnt * siz;
    afxUnit clampedOffRange = totalLen;
    afxByte*dst2 = dst;
    afxUnit errSiz = 0;

    if (0 == dstStride || siz == dstStride)
    {
        afxUnit batchSiz = 4096;
        afxUnit batchCnt = totalLen / batchSiz;
        afxUnit remainSiz = totalLen % batchSiz;

        for (afxUnit i = 0; i < batchCnt; i++)
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
        afxUnit unitSiz = siz;
        afxUnit batchCnt = totalLen / unitSiz;
        afxUnit remainSiz = totalLen % unitSiz;

        for (afxUnit i = 0; i < batchCnt; i++)
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

_AFX afxError AfxReadAt(afxStream in, afxSize offset, afxUnit cnt, afxUnit siz, void *dst, afxUnit dstStride)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &in);
    AFX_ASSERT(AfxIsStreamReadable(in));
    AFX_ASSERT(cnt);
    AFX_ASSERT(siz);
    AFX_ASSERT(dst);
    afxUnit clampedOffRange = cnt * siz;

    if (AfxSeekStream(in, offset, afxSeekOrigin_BEGIN)) AfxThrowError();
    else if ((clampedOffRange = AfxRead(in, cnt, siz, dst, dstStride)))
        AfxThrowError();

    return clampedOffRange;
}

_AFX afxError AfxReadReversed(afxStream in, afxUnit cnt, afxUnit siz, void* dst, afxUnit dstStride)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &in);
    AFX_ASSERT(AfxIsStreamReadable(in));
    AFX_ASSERT(cnt);
    AFX_ASSERT(siz);
    AFX_ASSERT(dst);
    afxUnit clampedOffRange;

    if ((clampedOffRange = AfxRead(in, cnt, siz, dst, dstStride)))
        AfxThrowError();

    Reverse32((((cnt * siz) - clampedOffRange) + 3) & 0xFFFFFFFC, dst);
    return clampedOffRange;
}

_AFX afxError AfxReadReversedAt(afxStream in, afxSize offset, afxUnit cnt, afxUnit siz, void *dst, afxUnit dstStride)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &in);
    AFX_ASSERT(AfxIsStreamReadable(in));
    AFX_ASSERT(cnt);
    AFX_ASSERT(siz);
    AFX_ASSERT(dst);
    afxUnit clampedOffRange;

    if ((clampedOffRange = AfxReadAt(in, offset, cnt, siz, dst, dstStride)))
        AfxThrowError();

    Reverse32((((cnt * siz) - clampedOffRange) + 3) & 0xFFFFFFFC, dst);
    return clampedOffRange;
}

_AFX afxError AfxReadStreamAt2(afxStream in, afxSize offset, afxUnit range, afxUnit stride, void* dst, afxUnit dstStride)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &in);
    AFX_ASSERT(AfxIsStreamReadable(in));
    AFX_ASSERT(range);
    AFX_ASSERT(dst);
    afxUnit clampedOffRange = range;

    if (AfxSeekStream(in, offset, afxSeekOrigin_BEGIN)) AfxThrowError();
    else if ((clampedOffRange = AfxReadStream2(in, range, stride, dst, dstStride)))
        AfxThrowError();

    return clampedOffRange;
}

_AFX afxError AfxReadStreamReversed(afxStream in, afxUnit range, afxUnit rate, void* dst)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &in);
    AFX_ASSERT(AfxIsStreamReadable(in));
    AFX_ASSERT(range);
    AFX_ASSERT(dst);
    afxUnit clampedOffRange;

    if ((clampedOffRange = AfxReadStream(in, range, rate, dst)))
        AfxThrowError();

    Reverse32(((range - clampedOffRange) + 3) & 0xFFFFFFFC, dst);
    return clampedOffRange;
}

_AFX afxError AfxReadStreamReversedAt(afxStream in, afxSize offset, afxUnit range, afxUnit rate, void* dst)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &in);
    AFX_ASSERT(AfxIsStreamReadable(in));
    AFX_ASSERT(range);
    AFX_ASSERT(dst);
    afxUnit clampedOffRange;

    if ((clampedOffRange = AfxReadStreamAt(in, offset, range, rate, dst)))
        AfxThrowError();

    Reverse32(((range - clampedOffRange) + 3) & 0xFFFFFFFC, dst);
    return clampedOffRange;
}

_AFX afxError AfxDoStreamOutput(afxStream out, afxUnit stride, afxUnit cnt, void const* src, afxUnit srcStride)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &out);
    AFX_ASSERT(AfxIsStreamWriteable(out));
    //AFX_ASSERT(range);
    AFX_ASSERT(src);
    afxUnit clampedOffRange = stride * cnt;

    afxUnit unitSiz = AFX_MIN_OR(stride, srcStride); // minor non-zero

    afxByte const*src2 = src;
    afxUnit batchSiz = (stride == srcStride) ? AfxGetIoBufferSize() : unitSiz;
    afxUnit nextSiz = 0;
    afxUnit errSiz = 0;

    afxUnit outPadding = stride > srcStride ? stride - srcStride : 0;
    afxByte zero[AFX_U16_MAX] = { 0 };

    for (afxUnit i = 0; i < cnt; i++)
    {
        nextSiz = stride;

        if ((errSiz = out->pimpl->write(out, src2, unitSiz)))
        {
            AfxThrowError();
            nextSiz -= errSiz;
        }
        else if (outPadding)
        {
            if ((errSiz = out->pimpl->write(out, zero, outPadding)))
            {
                AfxThrowError();
                nextSiz -= errSiz;
            }
        }
        clampedOffRange -= nextSiz;
        src2 += srcStride;

        if (out->pimpl->writeFeedback)
            out->pimpl->writeFeedback(out, nextSiz, &out->idd);

        if (err)
            break;
    }
    return clampedOffRange;
}

_AFX afxError AfxDoStreamOutputAt(afxStream out, afxSize offset, afxUnit stride, afxUnit cnt, void const* src, afxUnit srcStride)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &out);
    AFX_ASSERT(AfxIsStreamWriteable(out));
    AFX_ASSERT(stride);
    AFX_ASSERT(cnt);
    AFX_ASSERT(src);
    afxUnit clampedOffRange = stride * cnt;

    if (AfxSeekStream(out, offset, afxSeekOrigin_BEGIN)) AfxThrowError();
    else if ((clampedOffRange = AfxDoStreamOutput(out, stride, cnt, src, srcStride)))
        AfxThrowError();

    return clampedOffRange;
}

_AFX afxError AfxDoStreamInput(afxStream in, afxUnit stride, afxUnit cnt, void* dst, afxUnit dstStride)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &in);
    AFX_ASSERT(AfxIsStreamReadable(in));
    //AFX_ASSERT(range);
    AFX_ASSERT(dst);
    afxUnit clampedOffRange = stride * cnt;

    afxUnit unitSiz = AFX_MIN_OR(stride, dstStride); // minor non-zero

    afxByte*dst2 = dst;
    afxUnit batchSiz = (stride == dstStride) ? AfxGetIoBufferSize() : unitSiz;
    afxUnit nextSiz = batchSiz;
    afxUnit errSiz = 0;

    afxUnit outPadding = stride > dstStride ? stride - dstStride : 0;
    afxByte zero[AFX_U16_MAX] = { 0 };

    for (afxUnit i = 0; i < cnt; i++)
    {
        if ((errSiz = in->pimpl->write(in, dst2, unitSiz)))
        {
            AfxThrowError();
            nextSiz -= errSiz;
        }
        else if (outPadding)
        {
            if ((errSiz = in->pimpl->write(in, zero, outPadding)))
            {
                AfxThrowError();
                nextSiz -= errSiz;
            }
        }
        clampedOffRange -= nextSiz;
        dst2 += dstStride;

        if (in->pimpl->writeFeedback)
            in->pimpl->writeFeedback(in, nextSiz, &in->idd);

        if (err)
            break;
    }
    return clampedOffRange;
}

_AFX afxError AfxDoStreamInputAt(afxStream in, afxSize offset, afxUnit stride, afxUnit cnt, void* dst, afxUnit dstStride)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &in);
    AFX_ASSERT(AfxIsStreamReadable(in));
    AFX_ASSERT(stride);
    AFX_ASSERT(cnt);
    AFX_ASSERT(dst);
    afxUnit clampedOffRange = stride * cnt;

    if (AfxSeekStream(in, offset, afxSeekOrigin_BEGIN)) AfxThrowError();
    else if ((clampedOffRange = AfxDoStreamInput(in, stride, cnt, dst, dstStride)))
        AfxThrowError();

    return clampedOffRange;
}

_AFX afxError AfxMergeStream(afxStream out, afxStream in, afxSize base, afxUnit range, afxUnit rate)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &out);
    AFX_ASSERT(AfxIsStreamWriteable(out));
    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &in);
    AFX_ASSERT(AfxIsStreamReadable(in));

    void *space;
    AfxAllocate(range, 0, AfxHere(), (void**)&space);
    afxUnit clampedOffRange = range;

    if (!space) AfxThrowError();
    else
    {
        if (AfxSeekStream(in, base, afxSeekOrigin_BEGIN)) AfxThrowError();
        else
        {
            afxUnit batchSiz = rate ? rate : AfxGetIoBufferSize();
            afxUnit batches = range / batchSiz;
            afxUnit remain = range % batchSiz;
            afxUnit nextSizR = 0, nextSizW = 0;
            afxUnit errSiz = 0;

            for (afxUnit i = 0; i < batches; i++)
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
        AfxDeallocate((void**)&space, AfxHere());
    }
    return clampedOffRange;
}

#if 0 // MERGE/FORK

_AFX afxError AfxCopyStream(afxStream in, afxSize base, afxUnit range, afxUnit rate, afxStream out)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &out);
    AFX_ASSERT(AfxIsStreamWriteable(out));
    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &in);
    AFX_ASSERT(AfxIsStreamReadable(in));

    void *space = AfxAllocate(1, range, 0, AfxHere());
    afxUnit clampedOffRange = range;

    if (!space) AfxThrowError();
    else
    {
        if (AfxSeekStreamFromBegin(in, base)) AfxThrowError();
        else
        {
            afxUnit batchSiz = rate ? rate : AfxGetIoBufferSize();
            afxUnit batches = range / batchSiz;
            afxUnit remain = range % batchSiz;
            afxUnit nextSizR = 0, nextSizW = 0;
            afxUnit errSiz = 0;

            for (afxUnit i = 0; i < batches; i++)
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

_AFX afxStream AfxForkStream(afxStream parent, afxSize offset, afxUnit range)
{
    afxError err = { 0 };
    AfxAssertObjects(1, &parent, afxFcc_IOB);
    AFX_ASSERT_RANGE(parent->idd.m.bufCap, offset, range);

    afxSize maxRange = parent->idd.m.bufCap;
    afxSize effectiveRange = range > maxRange ? maxRange : range;
    AFX_ASSERT(effectiveRange == range);

    afxStream iob = NIL;
    AfxOpenStream(parent->flags, AfxGetStreamBuffer(parent, offset), effectiveRange, &iob);

    iob->pimpl = parent->pimpl;
    iob->idd = parent->idd;

    return iob;// range - effectiveRange;
}

#endif

_AFX afxBool AfxResetStream(afxStream iob)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &iob);

    if (iob->pimpl->dtor)
        iob->pimpl->dtor(iob);

    if (!iob->isUserBuf && iob->buf)
    {
        AfxBufferizeStream(iob, 0, NIL);
        AFX_ASSERT(!iob->buf);
    }

    return TRUE;
}

_AFX afxError AfxReopenStream(afxStream iob, afxUnit bufCap, void* start, afxSize len)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &iob);
    AFX_ASSERT(!start || (len || bufCap));
    AFX_ASSERT(!len || start); // if a length is specified, it must have a start.

    AfxRewindStream(iob);

    if (len)
    {
        if (!start)
        {
            AfxThrowError();
            return err;
        }
        else
        {
            if (AfxBufferizeStream(iob, bufCap, start))
            {
                AfxThrowError();
                return err;
            }
        }

        iob->length = len;
    }
    else
    {
        if (AfxBufferizeStream(iob, bufCap, start))
            AfxThrowError();
    }
    return err;
}

_AFX afxResult _AfxIosDtorCb(afxStream iob)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &iob);

    AfxResetStream(iob);

    if (iob->pimpl->dtor)
        iob->pimpl->dtor(iob);

    if (!iob->isUserBuf && iob->buf)
    {
        AfxBufferizeStream(iob, 0, NIL);
        AFX_ASSERT(!iob->buf);
    }

    return err;
}

_AFX afxError _AfxIosCtorCb(afxStream iob, void** args, afxUnit invokeNo)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &iob);

    afxStreamInfo const* info = ((afxStreamInfo const*)args[1]) + invokeNo;
    afxIobImpl const* pimpl = args[2];
    AFX_ASSERT(info);

    AfxZero(&iob->idd, sizeof(iob->idd));

    iob->flags = info->flags ? info->flags : afxIoFlag_READABLE | afxIoFlag_WRITEABLE | afxIoFlag_SEEKABLE;
    iob->bufCap = AfxGetIoBufferSize();
    iob->isUserBuf = FALSE;
    iob->buf = NIL;
    iob->usage = info->usage ? info->usage : afxIoUsage_MEM | afxIoUsage_FILE;
    iob->length = 0;
    iob->pimpl = pimpl;
    iob->posn = 0;

    if (&stdStreamImpl == iob->pimpl)
    {
        afxUnit bufSiz;

        if ((iob->flags & afxIoFlag_R) && !(bufSiz = iob->bufCap))
        {
            AfxThrowError();
            return err;
        }
        
        if ((iob->flags & afxIoFlag_W) && !(bufSiz = iob->bufCap))
        {
            AfxThrowError();
            return err;
        }
        
        if (!iob->isUserBuf)
            if (AfxBufferizeStream(iob, bufSiz, NIL))
                AfxThrowError();

    }
    return err;
}

_AFX afxClassConfig const _AFX_IOB_CLASS_CONFIG =
{
    .fcc = afxFcc_IOB,
    .name = "Stream",
    .desc = "I/O Buffer",
    .fixedSiz = sizeof(AFX_OBJECT(afxStream)),
    .ctor = (void*)_AfxIosCtorCb,
    .dtor = (void*)_AfxIosDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AFX afxError AfxAcquireImplementedStream(afxIobImpl const* pimpl, afxUnit cnt, afxStreamInfo const infos[], afxStream streams[])
{
    afxError err = { 0 };
    AFX_ASSERT(streams);
    AFX_ASSERT(infos);
    AFX_ASSERT(pimpl);
    AFX_ASSERT(cnt);

    if (!cnt || !infos || !streams)
    {
        AfxThrowError();
        return err;
    }

    afxSystem sys;
    AfxGetSystem(&sys);
    AFX_ASSERT_OBJECTS(afxFcc_SYS, 1, &sys);
    afxClass* cls = (afxClass*)_AfxSysGetIobClass(sys);
    AFX_ASSERT_CLASS(cls, afxFcc_IOB);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)streams, (void const*[]) { NIL, infos, pimpl }))
    {
        AfxThrowError();
        return err;
    }
    
    AFX_ASSERT_OBJECTS(afxFcc_IOB, cnt, streams);
    
    return err;
}

_AFX afxError AfxAcquireStream(afxUnit cnt, afxStreamInfo const infos[], afxStream streams[])
{
    afxError err = { 0 };
    AFX_ASSERT(infos);
    AFX_ASSERT(streams);
    AFX_ASSERT(cnt);

    if (!cnt || !infos || !streams)
    {
        AfxThrowError();
        return err;
    }

    if (AfxAcquireImplementedStream(&stdStreamImpl, cnt, infos, streams))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT_OBJECTS(afxFcc_IOB, cnt, streams);

    return err;
}

_AFX afxError AfxOpenStream(afxUnit bufCap, void* start, afxUnit len, afxIoFlags flags, afxStream* stream)
{
    afxError err = { 0 };
    AFX_ASSERT(!start || (bufCap || len));
    AFX_ASSERT(!len || start); // if a data length is specified, it must have a pointer specified too.
    
    afxStreamInfo info = { 0 };
    info.usage = afxIoUsage_MEM;
    info.bufCap = bufCap;
    info.flags = flags ? flags : afxIoFlag_RWX;

    if (AfxAcquireImplementedStream(&stdStreamImpl, 1, &info, stream))
    {
        AfxThrowError();
        return err;
    };
    
    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &stream);

    return err;
}
