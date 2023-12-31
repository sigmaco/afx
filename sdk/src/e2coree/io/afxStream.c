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
    afxBool b = (ios->posn == ios->len);
    return b;
}

_AFX afxError _AfxStandardStreamMoveCursorCallback(afxStream ios, afxInt offset, afxSeekMode origin)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ios, afxFcc_IOS);

    if (origin == afxSeekMode_ABSOLUTE)
    {
        AfxAssert(offset >= 0);
        AfxAssertRange(ios->len, (afxNat)offset, 1);

        if (ios->len <= (afxNat)offset)
            ios->posn = ios->len;
        else
            ios->posn = (afxNat)offset;
    }
    else if (origin == afxSeekMode_RELATIVE)
    {
        AfxAssertRange(ios->len, ios->posn + offset, 1);

        if (ios->posn + offset > ios->len)
            ios->posn = ios->len;
        else
            ios->posn += offset;
    }
    else if (origin == afxSeekMode_INVERSE)
    {
        AfxAssert(offset >= 0);
        AfxAssertRange(ios->len, (afxNat)offset, 1);

        if (ios->len <= (afxNat)offset)
            ios->posn = ios->len;
        else
            ios->posn = ios->len - offset;
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

_AFX afxError _AfxStandardStreamWriteCallback(afxStream ios, void const* const src, afxSize siz)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ios, afxFcc_IOS);
    AfxAssert(siz);
    AfxAssert(src);
    afxSize clampedOffRange = siz;

    if ((siz > (ios->len - ios->posn)) && !ios->isUserBuf) // if it is not a forked or user-buffered stream, we will try to enlarge it.
    {
        afxSize bufCap = AfxGetStreamBufferCap(ios);

        if (siz > (bufCap - ios->posn))
        {
            afxSize scaledSiz = bufCap + (siz - (bufCap - ios->posn));

            if (AfxReadjustStreamBuffer(ios, scaledSiz))
                AfxThrowError();
        }
    }

    afxSize writeLen = siz;

    if (siz > (ios->len - ios->posn))
    {
        writeLen = ios->len - ios->posn;
        AfxThrowError();
    }

    AfxCopy(1, writeLen, src, (void*)AfxGetStreamData(ios, ios->posn));
    clampedOffRange -= writeLen;
    ios->posn += writeLen;

    return clampedOffRange;
}

_AFX afxError _AfxStandardStreamReadCallback(afxStream ios, void *dst, afxSize siz)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ios, afxFcc_IOS);
    AfxAssert(siz);
    AfxAssert(dst);
    afxSize clampedOffRange = siz;

    afxSize readLen = siz;

    if (siz > ios->len - ios->posn)
    {
        readLen = ios->len - ios->posn;
        AfxThrowError();
    }

    AfxCopy(1, readLen, AfxGetStreamData(ios, ios->posn), dst);
    clampedOffRange -= readLen;
    ios->posn += readLen;

    return clampedOffRange;
}

// READ ONLY METHODS //////////////////////////////////////////////////////////

_AFXINL void const* AfxGetStreamData(afxStream const ios, afxSize offset)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ios, afxFcc_IOS);
    AfxAssertRange(ios->len, offset, 1);

    if (!ios->src.start)
        if (AfxReadjustStreamBuffer((afxStream)ios, ios->src.cap))
            AfxThrowError();

    return &(ios->src.start[offset]);
}

_AFXINL afxSize AfxGetStreamLength(afxStream const ios)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ios, afxFcc_IOS);
    return ios->len;
}

_AFXINL afxSize AfxGetStreamBufferCap(afxStream const ios)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ios, afxFcc_IOS);
    return ios->src.cap;
}

_AFXINL afxBool AfxStreamIsReadOnly(afxStream const ios)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ios, afxFcc_IOS);
    return AfxFlagsTest(ios->flags, (afxIoFlag_R | ~afxIoFlag_W));
}

_AFXINL afxBool AfxStreamIsReadable(afxStream const ios)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ios, afxFcc_IOS);
    return AfxFlagsTest(ios->flags, afxIoFlag_R);
}

_AFXINL afxBool AfxStreamIsWriteable(afxStream const ios)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ios, afxFcc_IOS);
    return AfxFlagsTest(ios->flags, afxIoFlag_W);
}

_AFXINL afxBool AfxStreamIsExecutable(afxStream const ios)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ios, afxFcc_IOS);
    return AfxFlagsTest(ios->flags, afxIoFlag_X);
}

_AFX afxBool AfxTestStreamEndCondition(afxStream ios)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ios, afxFcc_IOS);
    return ios->ioctl.eos(ios);
}

_AFX afxError AfxReadjustStreamBuffer(afxStream ios, afxSize siz)
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
                ios->len = ios->src.cap;
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
    AfxAssert(AfxFlagsTest(ios->flags, afxIoFlag_X));
    AfxAssertRange(origin, afxSeekMode_ABSOLUTE, afxSeekMode_INVERSE);
    return ios->ioctl.seek(ios, offset, origin);
}

_AFX afxError AfxRecedeStream(afxStream ios, afxSize range)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ios, afxFcc_IOS);
    AfxAssert(AfxFlagsTest(ios->flags, afxIoFlag_X));
    AfxAssert(range);
    return ios->ioctl.seek(ios, -(range), afxSeekMode_RELATIVE);
}

_AFX afxError AfxSkipStream(afxStream ios, afxInt range)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ios, afxFcc_IOS);
    AfxAssert(AfxFlagsTest(ios->flags, afxIoFlag_X));
    AfxAssert(range);
    return ios->ioctl.seek(ios, range, afxSeekMode_RELATIVE);
}

_AFX afxError AfxGoToStreamEnd(afxStream ios, afxInt offset)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ios, afxFcc_IOS);
    AfxAssert(AfxFlagsTest(ios->flags, afxIoFlag_X));
    return ios->ioctl.seek(ios, offset, afxSeekMode_INVERSE);
}

_AFX afxError AfxGoToStreamBegin(afxStream ios, afxInt offset)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ios, afxFcc_IOS);
    AfxAssert(AfxFlagsTest(ios->flags, afxIoFlag_X));
    return ios->ioctl.seek(ios, offset, afxSeekMode_ABSOLUTE);
}

_AFX afxSize AfxMeasureStream(afxStream ios)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ios, afxFcc_IOS);
    AfxAssert(AfxFlagsTest(ios->flags, afxIoFlag_X));
    afxSize curr = ios->ioctl.tell(ios);
    ios->ioctl.seek(ios, 0, afxSeekMode_INVERSE);
    afxSize len = ios->ioctl.tell(ios);
    ios->ioctl.seek(ios, curr, afxSeekMode_ABSOLUTE);
    return len;
}

_AFX afxSize AfxAskStreamPosn(afxStream ios)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ios, afxFcc_IOS);
    afxSize cur = ios->ioctl.tell(ios);
    AfxAssert(cur == ios->posn);
    return cur;
}

_AFX afxError AfxCopyStreamRange(afxStream out, afxStream in, afxSize base, afxSize range, afxNat feedback)
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
            afxNat batchSiz = feedback ? feedback : AfxGetIoBufferSize();
            afxNat batches = range / batchSiz;
            afxNat remain = range % batchSiz;
            afxNat nextSizR = 0, nextSizW = 0;
            afxNat errSiz = 0;

            for (afxNat i = 0; i < batches; i++)
            {
                nextSizR = batchSiz;

                if ((errSiz = AfxReadStream(in, space, nextSizR, feedback)))
                {
                    AfxThrowError();
                    nextSizW = nextSizR - errSiz;
                }
                else
                {
                    nextSizW = nextSizR;
                }
                
                if ((errSiz = AfxWriteStream(out, space, nextSizW, feedback)))
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

                if ((errSiz = AfxReadStream(in, space, nextSizR, feedback)))
                {
                    AfxThrowError();
                    nextSizW -= errSiz;
                }
                else
                {
                    nextSizW = nextSizR;
                }

                if ((errSiz = AfxWriteStream(out, space, nextSizW, feedback)))
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

_AFX afxError AfxCopyStream(afxStream out, afxStream in, afxNat feedback)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &out, afxFcc_IOS);
    AfxAssert(AfxStreamIsWriteable(out));
    AfxAssertObjects(1, &in, afxFcc_IOS);
    AfxAssert(AfxStreamIsReadable(in));
    afxNat clampedOffRange = AfxMeasureStream(in);

    if ((clampedOffRange = AfxCopyStreamRange(out, in, 0, clampedOffRange, feedback)))
        AfxThrowError();

    return clampedOffRange;
}

_AFX afxError AfxWriteStream(afxStream ios, void const* src, afxSize len, afxNat feedback)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ios, afxFcc_IOS);
    AfxAssert(AfxStreamIsWriteable(ios));
    AfxAssert(len);
    AfxAssert(src);
    afxNat clampedOffRange = len;

    if (feedback)
    {
        afxByte const *bytemap = src;
        afxNat batchSiz = feedback ? feedback : AfxGetIoBufferSize();
        afxNat batches = len / batchSiz;
        afxNat remain = len % batchSiz;
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
        if ((clampedOffRange = ios->ioctl.write(ios, src, len)))
            AfxThrowError();
    }
    return clampedOffRange;
}

_AFX afxError AfxWriteStreamAt(afxStream ios, afxSize offset, void const *src, afxSize len, afxNat feedback)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ios, afxFcc_IOS);
    AfxAssert(AfxStreamIsWriteable(ios));
    AfxAssert(len);
    AfxAssert(src);
    afxSize clampedOffRange = len;

    if (AfxSeekStream(ios, offset, afxSeekMode_ABSOLUTE)) AfxThrowError();
    else if ((clampedOffRange = AfxWriteStream(ios, src, len, feedback)))
        AfxThrowError();

    return clampedOffRange;
}

_AFX afxError AfxReadStream(afxStream ios, void *dst, afxSize len, afxNat feedback)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ios, afxFcc_IOS);
    AfxAssert(AfxStreamIsReadable(ios));
    AfxAssert(len);
    AfxAssert(dst);
    afxSize clampedOffRange = len;

    if (feedback)
    {
        afxByte *bytemap = dst;
        afxSize batchSiz = feedback ? feedback : AfxGetIoBufferSize();
        afxSize batches = len / batchSiz;
        afxSize remain = len % batchSiz;
        afxSize nextSiz = 0;
        afxSize nextOff = 0;
        afxSize errSiz = 0;

        for (afxSize i = 0; i < batches; i++)
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
        if ((clampedOffRange = ios->ioctl.read(ios, dst, len)))
            AfxThrowError();
    }
    return clampedOffRange;
}

_AFX afxError AfxReadStreamAt(afxStream ios, afxSize offset, void *dst, afxSize len, afxNat feedback)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ios, afxFcc_IOS);
    AfxAssert(AfxStreamIsReadable(ios));
    AfxAssert(len);
    AfxAssert(dst);
    afxSize clampedOffRange = len;

    if (AfxSeekStream(ios, offset, afxSeekMode_ABSOLUTE)) AfxThrowError();
    else if ((clampedOffRange = AfxReadStream(ios, dst, len, feedback)))
        AfxThrowError();

    return clampedOffRange;
}

_AFX afxStream AfxForkStreamRange(afxStream parent, afxSize offset, afxSize range)
{
    afxError err = AFX_ERR_NONE;
    AfxEntry("parent=%p,offset=%x,range=%u", parent, offset, range);
    AfxAssertObjects(1, &parent, afxFcc_IOS);
    AfxAssertRange(parent->len, offset, range);

    afxSize maxRange = parent->len;
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

    if ((sub = AfxForkStreamRange(parent, 0, parent->len)))
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
    afxSize len = *((afxSize const*)cookie->udd[3]);

    afxArena* aren = AfxGetIoArena();
    AfxAssertType(aren, afxFcc_AREN);

    AfxAssignTypeFcc(ios, afxFcc_IOS);
    ios->flags = flags ? flags : afxIoFlag_RWX;

    ios->len = len;
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

_AFX afxClassConfig _AfxIosClsConfig =
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

_AFX afxStream AfxAcquireStream(afxNat cap, afxIoFlags flags, void const* start, afxSize len)
{
    afxError err = AFX_ERR_NONE;
    AfxEntry("cap=%u,flags=%x,start=%p,len=%u", cap, flags, start, len);

    afxClass* cls = AfxGetStreamClass();
    AfxAssertClass(cls, afxFcc_IOS);
    afxStream ios;

    if (AfxAcquireObjects(cls, 1, (afxObject*)&ios, (void const*[]) { &cap, &flags, start, &len }))
        AfxThrowError();

    return ios;
}

_AFX afxStream AfxAcquireInputStream(afxNat cap, void const *start, afxSize len)
{
    afxError err = AFX_ERR_NONE;
    AfxEntry("cap=%u,start=%p,len=%u", cap, start, len);
    afxStream in;

    if ((in = AfxAcquireStream(cap, afxIoFlag_RX, start, len)))
        AfxThrowError();

    return in;
}

_AFX afxStream AfxAcquireOutputStream(afxNat cap, void *start, afxSize len)
{
    afxError err = AFX_ERR_NONE;
    AfxEntry("cap=%u,start=%p,len=%u", cap, start, len);
    afxStream out;

    if ((out = AfxAcquireStream(cap, afxIoFlag_WX, start, len)))
        AfxThrowError();

    return out;
}

_AFX afxNat AfxCurateStreams(afxNat first, afxNat cnt, afxBool(*f)(afxStream, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(f);
    afxClass* cls = AfxGetStreamClass();
    AfxAssertClass(cls, afxFcc_IOS);
    return AfxCurateInstances(cls, first, cnt, (void*)f, udd);
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
