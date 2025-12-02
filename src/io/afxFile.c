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

#define _CRT_SECURE_NO_WARNINGS 1
#define WIN32_LEAN_AND_MEAN 1
#include <Windows.h>
#include <sys/stat.h>
#include <stdio.h>

#define _AFX_CORE_C
#define _AFX_FILE_C
#define _AFX_STREAM_C
#define _AFX_STORAGE_C
#include "../exec/afxSystemDDK.h"
#if 0
AFX_OBJECT(afxFile)
{
    AFX_OBJ(afxStream)  iob;
    afxFileFlags        flags;
    afxUri2048          url; // ---- //./dev/dir/to/
    afxUri2048          resolvedUrl; // if in search, it is only the base URI.    
};
#endif

////////////////////////////////////////////////////////////////////////////////

_AFX afxBool _AfxIobEosFileCb(afxStream file)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &file);
    AFX_ASSERT(file->typeFcc == afxFcc_FILE);

    afxBool b = !!(feof(file->idd.f.fd));
    return b;
}

_AFX afxError _AfxIobSeekFileCb(afxStream file, afxSize offset, afxSeekOrigin origin)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &file);
    AFX_ASSERT(file->typeFcc == afxFcc_FILE);

    if (NIL != fseek(file->idd.f.fd, (long)offset, (int)origin))
        AfxThrowError();

    file->posn = ftell(file->idd.f.fd);

    if (origin == afxSeekOrigin_END)
        file->length = ftell(file->idd.f.fd) + offset;

    return err;
}

_AFX afxUnit _AfxIobTellFileCb(afxStream file)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &file);
    AFX_ASSERT(file->typeFcc == afxFcc_FILE);
    return ftell(file->idd.f.fd);
}

_AFX afxError _AfxIobWriteFileCb(afxStream file, void const* const src, afxUnit32 siz)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &file);
    AFX_ASSERT(file->typeFcc == afxFcc_FILE);
    AFX_ASSERT(siz);
    AFX_ASSERT(src);
    afxUnit clampedOffRange = siz;
    afxUnit written = (afxUnit)fwrite(src, sizeof(afxByte), siz, file->idd.f.fd);
    clampedOffRange -= written;
    return clampedOffRange;
}

_AFX afxError _AfxIobReadFileCb(afxStream file, void *dst, afxUnit32 siz)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &file);
    AFX_ASSERT(file->typeFcc == afxFcc_FILE);
    AFX_ASSERT(siz);
    AFX_ASSERT(dst);
    afxUnit clampedOffRange = siz;

    if (file->idd.f.shouldBeFlushed)
    {
        fflush(file->idd.f.fd);
        file->idd.f.shouldBeFlushed = FALSE;
    }

    afxUnit read = (afxUnit)fread(dst, sizeof(afxByte), siz, file->idd.f.fd);
    clampedOffRange -= read;
    return clampedOffRange;
}

_AFX afxError _AfxIobDtorFileCb(afxStream file)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &file);
    AFX_ASSERT(file->typeFcc == afxFcc_FILE);

    AfxRealizeFileSegments(file, 0, 0); // dealloc segmentation data

    if (file->idd.f.fd)
    {
        if (file->idd.f.shouldBeFlushed)
        {
            fflush(file->idd.f.fd);
            file->idd.f.shouldBeFlushed = FALSE;
        }

        //setvbuf(file->idd.f.fd, NIL, _IONBF, BUFSIZ);
        fclose(file->idd.f.fd);
        file->idd.f.fd = NIL;
    }

    AfxPopLink(&file->idd.f.disk);

    return err;
}

afxIobImpl const fileStreamImpl =
{
    .read = _AfxIobReadFileCb,
    .write = _AfxIobWriteFileCb,
    .tell = _AfxIobTellFileCb,
    .seek = _AfxIobSeekFileCb,
    .eos = _AfxIobEosFileCb,
    .dtor = _AfxIobDtorFileCb
};

_AFX afxResult AfxFlushFileCache(afxStream file)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &file);
    AFX_ASSERT(file->typeFcc == afxFcc_FILE);
    AFX_ASSERT(file->pimpl == &fileStreamImpl);

    if (fflush(file->idd.f.fd))
        AfxThrowError();

    return err;
}

_AFX afxError AfxReadFileLine(afxStream file, afxString* str)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &file);
    AFX_ASSERT(file->typeFcc == afxFcc_FILE);
    AFX_ASSERT(file->pimpl == &fileStreamImpl);
    AFX_ASSERT(str);
    AFX_ASSERT(AfxGetStringCapacity(str, 0));

    err = !fgets(str->buf, str->cap, file->idd.f.fd);

    str->len = AfxStrlen(str->start);

    return err;
}

////////////////////////////////////////////////////////////////////////////////

_AFX afxBool AfxTestFileFlags(afxStream file, afxFileFlags flags)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &file);
    AFX_ASSERT(file->typeFcc == afxFcc_FILE);
    return file->flags & flags;
}

_AFX afxBool AfxGetFileUri(afxStream file, afxUri* location)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &file);
    AFX_ASSERT(file->typeFcc == afxFcc_FILE);
    AFX_ASSERT(location);
    AfxCopyUri(location, &file->idd.f.url.uri);
    return !err;
}

_AFX afxBool AfxGetResolvedFileUri(afxStream file, afxUri* location)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &file);
    AFX_ASSERT(file->typeFcc == afxFcc_FILE);
    AFX_ASSERT(location);
    AfxCopyUri(location, &file->idd.f.resolvedUrl.uri);
    AfxCanonicalizePath(location, AFX_ON_WINDOWS);
    return !err;
}

_AFX afxBool AfxTrackFileSegment(afxStream file, void const* at, afxUnit* segIdx)
{
    afxError err = { 0 };
    afxBool rslt = FALSE;

    for (afxUnit i = 0; i < file->idd.f.segCnt; i++)
    {
        afxByte* map = file->idd.f.segBufs->data;
        afxSize decodedSiz = file->idd.f.segHdrs[i].decodedSiz;

        if ((at >= (void const*)map) && (at <= (void const*)(map + decodedSiz)))
        {
            AFX_ASSERT(segIdx);
            *segIdx = i;
            rslt = TRUE;
            break;
        }
    }
    return rslt;
}

_AFX void* AfxResolveFileReference(afxStream file, afxFileBlock const* ref)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &file);
    AFX_ASSERT(ref);
    AFX_ASSERT_RANGE(file->idd.f.segCnt, ref->segIdx, 1);
    AFX_ASSERT_RANGE(file->idd.f.segHdrs[ref->segIdx].decodedSiz, ref->offset, 1);
    return (ref->segIdx < file->idd.f.segCnt) && (ref->offset < file->idd.f.segHdrs[ref->segIdx].decodedSiz) ? file->idd.f.segBufs[ref->segIdx].data + ref->offset : NIL;
}

_AFX afxBool AfxOpenFileSegments(afxStream file, afxUnit baseSegIdx, afxUnit segCnt, afxStream in)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &file);
    AFX_ASSERT_RANGE(file->idd.f.segCnt, baseSegIdx, segCnt);
    afxBool ret = TRUE;

    for (afxUnit i = 0; i < file->idd.f.segCnt; i++)
    {
        afxUnit segIdx = baseSegIdx + i;
        afxFileSegment* seg = &file->idd.f.segHdrs[segIdx];

        if (!(!seg->decodedSiz || file->idd.f.segBufs[segIdx].data))
            ret = FALSE;

        if (file->idd.f.segBufs[segIdx].refCnt)
        {
            AFX_ASSERT(file->idd.f.segBufs[segIdx].data);
            ++file->idd.f.segBufs[segIdx].refCnt;
        }
        else if (seg->decodedSiz)
        {
            afxUnit decSizAligned = (seg->decodedSiz + 3) & 0xFFFFFFFC;

            if (AfxAllocate(decSizAligned, seg->decodedAlign, AfxHere(), (void**)&file->idd.f.segBufs[segIdx])) AfxThrowError();
            else
            {
                if (!seg->codec)
                {
                    AfxReadStreamAt(in, seg->start, seg->encodedSiz, 0, file->idd.f.segBufs[segIdx].data);
                }
                else
                {
                    void* buf;

                    if (AfxAllocate(seg->encodedSiz, seg->encodedAlign, AfxHere(), (void**)&buf)) AfxThrowError();
                    else
                    {
                        if (AfxReadStreamAt(in, seg->start, seg->encodedSiz, 0, buf)) AfxThrowError();
                        else
                        {
                            //AfxDecodeStream(in, seg->start, seg->decAlign, seg->encSiz / seg->encAlign);
                        }
                    }
                }

                file->idd.f.segBufs[segIdx].refCnt = 1;

                if (err && file->idd.f.segBufs[segIdx].data)
                {
                    AfxDeallocate((void**)&file->idd.f.segBufs[segIdx].data, AfxHere());
                    file->idd.f.segBufs[segIdx].data = NIL;
                    file->idd.f.segBufs[segIdx].refCnt = 0;
                }
            }
        }
    }
    return ret;
}

_AFX void AfxCloseFileSegments(afxStream file, afxUnit baseSegIdx, afxUnit segCnt)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &file);
    AFX_ASSERT_RANGE(file->idd.f.segCnt, baseSegIdx, segCnt);

    for (afxUnit i = 0; i < segCnt; i++)
    {
        afxUnit segIdx = baseSegIdx + i;

        if (file->idd.f.segBufs[segIdx].refCnt)
        {
            if (--file->idd.f.segBufs[segIdx].refCnt == 0)
            {
                AfxDeallocate((void**)&file->idd.f.segBufs[segIdx].data, AfxHere());
                file->idd.f.segBufs[segIdx].data = NIL;
                file->idd.f.segBufs[segIdx].refCnt = 0;
            }
        }
    }
}

_AFX afxError AfxRealizeFileSegments(afxStream file, afxUnit32 baseSegOffset, afxUnit segCnt)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &file);

    if (!segCnt)
    {
        if (file->idd.f.segCnt)
        {
            AfxCloseFileSegments(file, 0, file->idd.f.segCnt);

            AfxDeallocate((void**)&file->idd.f.segHdrs, AfxHere());
            AfxDeallocate((void**)&file->idd.f.segBufs, AfxHere());
        }
        return err;
    }

    if (AfxAllocate(segCnt * sizeof(file->idd.f.segHdrs[0]), 0, AfxHere(), (void**)&file->idd.f.segHdrs)) AfxThrowError();
    else if (AfxAllocate(segCnt * sizeof(file->idd.f.segBufs[0]), 0, AfxHere(), (void**)&file->idd.f.segBufs))
        AfxThrowError();

    if (!err && AfxReadStreamAt(file, baseSegOffset, segCnt * sizeof(file->idd.f.segHdrs[0]), 0, file->idd.f.segHdrs))
    {
        AfxThrowError();
    }

    if (err)
    {
        AfxDeallocate((void**)&file->idd.f.segHdrs, AfxHere());
        AfxDeallocate((void**)&file->idd.f.segBufs, AfxHere());
    }
    else
    {
        file->idd.f.segCnt = segCnt;

        for (afxUnit i = 0; i < file->idd.f.segCnt; i++)
        {
            file->idd.f.segBufs[i].data = NIL;
            file->idd.f.segBufs[i].refCnt = 0;
        }
    }
    return err;
}

_AFX afxBool _AfxOpenFileCb(void* udd, afxUnit diskId, afxUnit endpointIdx, afxUri const* path, afxUri const* osPath)
{
    afxError err = { 0 };
    afxBool next = TRUE;

    afxUnit* diskIdVar = ((void**)udd)[0];
    afxUnit* endpointIdxVar = ((void**)udd)[1];
    afxUri* url = ((void**)udd)[2];
    afxUri* resolvedUrl = ((void**)udd)[3];
    
    *diskIdVar = diskId;
    *endpointIdxVar = endpointIdx;
    AfxCopyUri(resolvedUrl, osPath);
    AfxCopyUri(url, path);

    if (!err)
        next = FALSE;

    return next;
}

_AFX afxError AfxReopenFile(afxStream iob, afxUri const* uri, afxFileFlags flags)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &iob);
    AFX_ASSERT(uri);
#ifdef _AFX_DBG_FILE
    AfxReportMessage("Reopening... <%.*s>", AfxPushString(AfxGetUriString(uri)));
#endif

    if (iob->usage != afxIoUsage_FILE)
    {
        AfxThrowError();
        return err;
    }

    AfxRealizeFileSegments(iob, 0, 0); // dealloc segmentation data
    AfxResetStream(iob); // anyway, reset it.

    iob->typeFcc = afxFcc_FILE;
    iob->pimpl = &fileStreamImpl;
    iob->idd.f.fflags = flags;
    iob->idd.f.fd = NIL;
    AfxMakeUri2048(&iob->idd.f.resolvedUrl, NIL);
    AfxMakeUri2048(&iob->idd.f.url, NIL);
    AfxPushLink(&iob->idd.f.disk, NIL);

    afxUnit diskId = AFX_INVALID_INDEX;
    afxUnit endpointIdx = AFX_INVALID_INDEX;

    if (!AfxFindFiles(uri, flags, _AfxOpenFileCb, (void*[]) { &diskId, &endpointIdx, &iob->idd.f.url.uri, &iob->idd.f.resolvedUrl.uri }))
    {
        AfxThrowError();
        AfxReportError("Couldn't resolve URI <%.*s>", AfxPushString(uri ? &uri->s : &AFX_STRING_EMPTY));
        return err;
    }
    
    afxChar mode[9] = { NIL }, *modePtr = mode;
    afxIoFlags ioFlags = NIL;

    if (AfxTestFlags(flags, afxFileFlag_W))
    {
        ioFlags |= afxIoFlag_W;
        *modePtr++ = 'w';
    }
    else if (AfxTestFlags(flags, afxFileFlag_R))
    {
        ioFlags |= afxIoFlag_R;
        *modePtr++ = 'r';
    }

    if (AfxTestFlags(flags, afxFileFlag_X))
    {
        ioFlags |= afxIoFlag_X;
        //*modePtr++ = 'x';
    }

    *modePtr++ = '+';
    *modePtr++ = 'b';

    if (iob->idd.f.fd)
        fclose(iob->idd.f.fd), iob->idd.f.fd = NIL;

    if (!(iob->idd.f.fd = fopen((char const*)(iob->idd.f.resolvedUrl.buf), mode)))
    {
        AfxThrowError();
        perror("fopen failed");
        AfxReportError("Couldn't open resolved URI <%.*s>('%.*s')", AfxPushString(uri ? &uri->s : &AFX_STRING_EMPTY), AfxPushString(&iob->idd.f.resolvedUrl.uri.s));
        return err;
    }

    iob->pimpl = &fileStreamImpl; // reload can have replaced it.
    iob->length = AfxMeasureStream(iob);

    if (err)
        fclose(iob->idd.f.fd), iob->idd.f.fd = NIL;

    if (!err && diskId != AFX_INVALID_INDEX)
    {
        afxStorage disk;
        afxUri dev, path;
        AfxSplitPath(uri, &dev, &path);

        if (AfxIsUriBlank(&dev)) AfxEnumerateStorages(0, 1, &disk);
        else if (!AfxFindStorage(uri, &disk))
            disk = NIL;

        if (disk)
        {
            AFX_ASSERT_OBJECTS(afxFcc_FSYS, 1, &disk);
            AfxPushLink(&iob->idd.f.disk, &disk->fileChain);
        }
    }
    return err;
}

_AFX afxError AfxReloadFile(afxStream iob, afxUri const* uri)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &iob);
    AFX_ASSERT2(uri, !AfxIsUriBlank(uri));

    if (iob->usage != afxIoUsage_FILE)
    {
        AfxThrowError();
        return err;
    }

    if (AfxReopenFile(iob, uri, afxFileFlag_R))
    {
        AfxThrowError();
        return err;
    }

    afxUnit bufSiz = AfxMeasureStream(iob);

    if (AfxBufferizeStream(iob, bufSiz, NIL))
    {
        AfxThrowError();
        return err;
    }

    afxByte* buf = (afxByte*)AfxGetStreamBuffer(iob, 0);
    AfxRewindStream(iob);
    fread(buf, sizeof(afxByte), bufSiz, iob->idd.f.fd);
    buf[bufSiz] = '\0';
    iob->length = bufSiz;

    fclose(iob->idd.f.fd);
    iob->idd.f.fd = NIL;

    iob->pimpl = &stdStreamImpl; // reload can have replaced it.

    return err;
}

#if 0
_AFX afxError AfxWrapFile(afxStream iob, void* fd, afxIoFlags flags)
{
    afxError err = { 0 };
    AFX_ASSERT(fd);
    afxStream file = NIL;

    AfxReportMessage("Wrapping file... <%p>,%x", fd, flags);

    _afxIobIdd idd = { 0 };
    idd.f.fd = fd;
    idd.f.isUserFd = TRUE;

    if (AfxAcquireImplementedStream(0, flags, &fileStreamImpl, &idd, &file))
        AfxThrowError();

    AFX_ASSERT(stream);
    *stream = file;

    return err;
}
#endif

////////////////////////////////////////////////////////////////////////////////

_AFX afxError AfxOpenFile(afxUri const* uri, afxFileFlags flags, afxStream* file)
{
    afxError err = { 0 };
#ifdef _AFX_DBG_FILE
    AfxReportMessage("Opening... <%.*s>", AfxPushString(AfxGetUriString(uri)));
#endif
    AFX_ASSERT(uri);

    afxStream iob;
    afxStreamInfo info = { 0 };
    info.usage = afxIoUsage_FILE;
    info.flags = flags ? flags & AFX_FILE_PERM_MASK : afxIoFlag_RWX;

    if (AfxAcquireImplementedStream(&fileStreamImpl, 1, &info, &iob))
    {
        AfxThrowError();
        return err;
    }

    AfxPushLink(&iob->idd.f.disk, NIL);

    iob->typeFcc = afxFcc_FILE;
    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &iob);

    if (AfxReopenFile(iob, uri, flags))
    {
        AfxThrowError();
        AfxReportError("Couldn't open <%.*s>", AfxPushString(uri ? &uri->s : &AFX_STRING_EMPTY));
    }
    else if (!iob->idd.f.fd)
        AfxThrowError();

    if (err)
    {
        AfxDisposeObjects(1, &iob);
    }
    *file = iob;
    return err;
}

_AFX afxError AfxLoadFile(afxUri const* uri, afxStream* file)
// will upload the entire file data into RAM and close the file.
{
    afxError err = { 0 };
#ifdef _AFX_DBG_FILE
    AfxReportMessage("Loading... <%.*s>", AfxPushString(AfxGetUriString(uri)));
#endif
    AFX_ASSERT(uri);

    afxStream iob;
    afxStreamInfo info = { 0 };
    info.usage = afxIoUsage_FILE;
    info.flags = afxIoFlag_R;

    if (AfxAcquireImplementedStream(&fileStreamImpl, 1, &info, &iob))
    {
        AfxThrowError();
        return err;
    }

    AfxPushLink(&iob->idd.f.disk, NIL);
    iob->typeFcc = afxFcc_FILE;

    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &iob);

    if (AfxReloadFile(iob, uri))
    {
        AfxThrowError();
        AfxReportError("Couldn't load <%.*s>", AfxPushString(uri ? &uri->s : &AFX_STRING_EMPTY));
    }

    if (err)
    {
        AfxDisposeObjects(1, &iob);
    }
    *file = iob;
    return err;
}
