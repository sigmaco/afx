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

#define _CRT_SECURE_NO_WARNINGS 1
#include <stdio.h>

#define _AFX_CORE_C
#define _AFX_FILE_C
#define _AFX_STREAM_C
#include "qwadro/core/afxManager.h"
#include "qwadro/core/afxSystem.h"

////////////////////////////////////////////////////////////////////////////////

_AFX afxBool _AfxFileStreamEosCb(afxStream file)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &file, afxFcc_IOB);
    afxBool b = !!(feof(file->idd.fd));
    return b;
}

_AFX afxError _AfxFileStreamSeekCb(afxStream file, afxInt offset, afxSeekMode origin)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &file, afxFcc_IOB);

    if (NIL != fseek(file->idd.fd, (long)offset, (int)origin))
        AfxThrowError();

    file->posn = ftell(file->idd.fd);

    return err;
}

_AFX afxNat _AfxFileStreamTellCb(afxStream file)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &file, afxFcc_IOB);

    afxNat rslt = ftell(file->idd.fd);
    AfxAssert(rslt == (afxNat)file->posn);
    file->posn = rslt;

    return file->posn;
}

_AFX afxError _AfxFileStreamWriteCb(afxStream file, void const* const src, afxSize siz)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &file, afxFcc_IOB);

    AfxAssert(siz);
    AfxAssert(src);
    afxNat clampedOffRange = siz;

    afxNat written = (afxNat)fwrite(src, sizeof(afxByte), siz, file->idd.fd);

    clampedOffRange -= written;
    file->posn += written;
    AfxAssert(file->posn == (afxNat)ftell(file->idd.fd));
    return clampedOffRange;
}

_AFX afxError _AfxFileStreamReadCb(afxStream file, void *dst, afxSize siz)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &file, afxFcc_IOB);

    AfxAssert(siz);
    AfxAssert(dst);
    afxNat clampedOffRange = siz;

    if (file->idd.shouldBeFlushed)
    {
        fflush(file->idd.fd);
        file->idd.shouldBeFlushed = FALSE;
    }

    afxNat read = (afxNat)fread(dst, sizeof(afxByte), siz, file->idd.fd);

    clampedOffRange -= read;
    file->posn += read;
    AfxAssert(file->posn == (afxNat)ftell(file->idd.fd));
    return clampedOffRange;
}

_AFX afxResult _AfxFileStreamDtorCb(afxStream file)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &file, afxFcc_IOB);

    AfxDeallocateUri(&file->idd.path);

    if (file->idd.fd)
    {
        if (file->idd.shouldBeFlushed)
        {
            fflush(file->idd.fd);
            file->idd.shouldBeFlushed = FALSE;
        }

        //setvbuf(file->idd.fd, NIL, _IONBF, BUFSIZ);
        fclose(file->idd.fd);
    }

    return err;
}

afxStreamImpl const fileStreamImpl =
{
    .dtor = _AfxFileStreamDtorCb,
    .read = _AfxFileStreamReadCb,
    .write = _AfxFileStreamWriteCb,
    .tell = _AfxFileStreamTellCb,
    .seek = _AfxFileStreamSeekCb,
    .eos = _AfxFileStreamEosCb
};

////////////////////////////////////////////////////////////////////////////////

#if 0
_AFX afxBool AfxFileIsVirtual(afxFile file)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &file, afxFcc_FILE);
    return AfxTestFlags(file->flags, afxFileFlag_V);
}

_AFX afxBool AfxFileIsArchive(afxFile file)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &file, afxFcc_FILE);
    return AfxTestFlags(file->flags, afxFileFlag_A);
}

_AFX afxBool AfxFileIsTemporary(afxFile file)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &file, afxFcc_FILE);
    return AfxTestFlags(file->flags, afxFileFlag_T);
}

_AFX afxBool AfxFileIsSparse(afxFile file)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &file, afxFcc_FILE);
    return AfxTestFlags(file->flags, afxFileFlag_S);
}

_AFX afxBool AfxFileIsCompressed(afxFile file)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &file, afxFcc_FILE);
    return AfxTestFlags(file->flags, afxFileFlag_Z);
}

_AFX afxBool AfxFileIsEncrypted(afxFile file)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &file, afxFcc_FILE);
    return AfxTestFlags(file->flags, afxFileFlag_E);
}

_AFX afxBool AfxFileIsHidden(afxFile file)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &file, afxFcc_FILE);
    return AfxTestFlags(file->flags, afxFileFlag_H);
}

_AFX afxBool AfxFileIsReserved(afxFile file)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &file, afxFcc_FILE);
    return AfxTestFlags(file->flags, afxFileFlag_Q);
}
#endif

_AFX afxBool AfxFileShouldBeFlushed(afxStream file)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &file, afxFcc_IOB);
    AfxAssert(file->pimpl == &fileStreamImpl);
    return file->idd.shouldBeFlushed;
}

#if 0
_AFX afxFileSection* AfxGetFileSection(afxFile file)
{
    return (afxFileSection*)(((afxByte*)file->hdr) + file->hdr->secOffset);
}
#endif

_AFX afxResult AfxFlushFile(afxStream file)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &file, afxFcc_IOB);
    AfxAssert(file->pimpl == &fileStreamImpl);

    if (fflush(file->idd.fd))
        AfxThrowError();

    return err;
}

_AFX afxError AfxReadFileString(afxStream file, afxRestring* str)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &file, afxFcc_IOB);
    AfxAssert(file->pimpl == &fileStreamImpl);
    AfxAssert(str);
    AfxAssert(AfxStringIsWriteable(str));
    
    err = !fgets(str->str.buf, str->cap, file->idd.fd);

    str->str.len = AfxStrlen(str->str.start);

    return err;
}

_AFX afxUri const* AfxGetFilePath(afxStream file)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &file, afxFcc_IOB);
    AfxAssert(file->pimpl == &fileStreamImpl);
    return &(file->idd.path);
}

_AFX void* AfxGetFileDescriptor(afxStream file)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &file, afxFcc_IOB);
    AfxAssert(file->pimpl == &fileStreamImpl);
    return file->idd.fd;
}

_AFX afxResult AfxCopyFilePath(afxStream file, afxUri* uri)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &file, afxFcc_IOB);
    AfxAssert(file->pimpl == &fileStreamImpl);
    AfxAssert(uri);
    return AfxCopyUri(uri, &file->idd.path);
}

_AFX afxString const* AfxGetFilePathString(afxStream file)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &file, afxFcc_IOB);
    AfxAssert(file->pimpl == &fileStreamImpl);
    return AfxGetUriString(&file->idd.path);
}

_AFX afxResult AfxCopyFilePathString(afxStream file, afxRestring* str)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &file, afxFcc_IOB);
    AfxAssert(file->pimpl == &fileStreamImpl);
    return AfxCopyString(str, AfxGetUriString(&file->idd.path));
}

#if 0
_AFX void* LoadFileSection(afxUrdSection const* sec, void *DestinationMemory, void *Reader, afxBool fileIsByteReversed)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(sec);
    AfxAssert(DestinationMemory);
    AfxAssert(Reader);

    void *result = 0;
    void *Result = 0;

    if (sec->expandedDataSiz)
    {
        afxNat alignedSiz = (sec->expandedDataSiz + 3) & 0xFFFFFFFC;

        if (DestinationMemory)
        {
            result = DestinationMemory;
            Result = DestinationMemory;
        }
        else
        {
            //use Section->InternalAlignment
            Result = AfxAllocate(1, alignedSiz, sec->internalAlignment, AfxHint());
            result = Result;
        }

        if (result)
        {
            if (sec->fmt)
            {
                afxNat v8 = AfxGetCompressionPaddingSize(sec->fmt);
                void *v9 = AfxAllocate(1, sec->dataSiz + v8, 0, AfxHint());

                if (v9)
                {
                    AfxReadStreamAt(Reader, sec->dataOffset, sec->dataSiz, 0, v9);

                    if (!AfxDecompressData(sec->fmt, fileIsByteReversed, sec->dataSiz, v9, /*Section->first16Bit*/0, /*Section->first8Bit*/0, sec->expandedDataSiz, Result))
                    {
                        AfxDeallocate(Result);
                        Result = 0;
                    }
                }

                AfxDeallocate(v9);
                result = Result;
            }
            else
            {
                AfxReadStreamAt(Reader, sec->dataOffset, sec->dataSiz, 0, result);
                result = Result;
            }
        }
    }
    return result;
}

_AFX afxBool AfxOpenFileSections(afxFile file, afxNat baseSec, afxNat secCnt, void *buf[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &file, afxFcc_FILE);

    AfxAssert(file->hdr);
    AfxAssertRange(file->openSecCnt, baseSec, secCnt);

    afxBool ret = TRUE;

    afxUrdSection *sections = ((afxUrdSection*)((afxByte*)file->hdr + file->hdr->secOffset));

    for (afxNat i = 0; i < secCnt; i++)
    {
        afxNat secIdx = baseSec + i;
        afxUrdSection *sec = &sections[secIdx];

        if (!file->openSections[secIdx]) AfxThrowError();
        else
        {
            file->openSections[secIdx] = LoadFileSection(&sections[secIdx], buf, file, FALSE);
            file->isUserMem[secIdx] = !!buf;

            if (!(!sec->expandedDataSiz || file->openSections[secIdx]))
                ret = FALSE;
        }
    }
    return ret;
}

_AFX afxBool AfxOpenFileSection(afxFile file, afxNat secIdx, void *buf)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &file, afxFcc_FILE);
    return AfxOpenFileSections(file, secIdx, 1, buf ? &buf : NIL);
}

_AFX void AfxCloseFileSections(afxFile file, afxNat baseSec, afxNat secCnt)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &file, afxFcc_FILE);

    afxMmu mmu = AfxGetIoContext();
    AfxAssertObjects(1, &mmu, afxFcc_MMU);

    AfxAssertRange(file->openSecCnt, baseSec, secCnt);

    for (afxNat i = 0; i < secCnt; i++)
    {
        afxNat secIdx = baseSec + i;
        AfxAssertRange(file->openSecCnt, secIdx, 1);

        if (file->openSections[secIdx])
        {
            if (!file->isUserMem[secIdx])
                AfxDeallocate(mmu, file->openSections[secIdx]);

            file->isUserMem[secIdx] = FALSE;
            file->marshalled[secIdx] = FALSE;
            file->openSections[secIdx] = NIL;
        }
    }
}

_AFX void AfxCloseFileSection(afxFile file, afxNat secIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &file, afxFcc_FILE);
    AfxCloseFileSections(file, secIdx, 1);
}

_AFX void AfxCloseAllFileSections(afxFile file)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &file, afxFcc_FILE);

    if (file->openSecCnt)
        AfxCloseFileSections(file, 0, file->openSecCnt);
}
#endif

#if 0
_AFX afxError _AfxFileCtor(afxFile file, afxCookie const* cookie)
{
    afxError err = AFX_ERR_NONE;
    
    afxFileFlags flags = *(afxFileFlags const*)(cookie->udd[0]);
    afxUri const* uri = ((afxUri const*)cookie->udd[1]) + cookie->no;
    
    file->shouldBeFlushed = FALSE;

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

    *modePtr++ = 'b';
    *modePtr++ = '+';

    afxUri path;
    AfxAssert(uri);
    

    if (!AfxPickUriPath(uri, &path)) AfxThrowError();
    else
    {
        afxUri2048 uri2;
        AfxMakeUri2048(&uri2, NIL);

        AfxResolveUri(flags, &path, &uri2.uri);
        
        afxChar const *rawName = AfxGetUriStorage(&uri2.uri, 0);

        if (!(file->fd = fopen(rawName, mode))) AfxThrowError();
        else
        {
            if (!(file->iob = AfxAcquireStream(ioFlags, 0, NIL, 0))) AfxThrowError();
            else
            {
                AfxAssertObjects(1, &file->iob, afxFcc_IOB);

                //void const *ptr = AfxGetStreamBuffer(file->iob, 0); // force buffer existence
                //afxNat strBufCap = AfxGetStreamBufferCap(file->iob);

                //if (0 != setvbuf(file->fd, (void*)ptr, _IOFBF, strBufCap)) AfxThrowError();
                //else
                {
                    file->iob->ioctl.read = _AfxFileStreamReadCallback;
                    file->iob->ioctl.readFeedback = NIL;
                    file->iob->ioctl.write = _AfxFileStreamWriteCallback;
                    file->iob->ioctl.writeFeedback = NIL;
                    file->iob->ioctl.eos = _AfxFileStreamHasReachedEndCallback;
                    file->iob->ioctl.seek = _AfxFileStreamMoveCursorCallback;
                    file->iob->ioctl.tell = _AfxFileStreamAskCursorCallback;
                    
                    AfxDuplicateUri(&file->path, &uri2.uri);
                    file->iob->idd = (void*)file;
                }

                if (err)
                    AfxReleaseObjects(1, (void*[]) { file->iob });
            }

            if (err)
                fclose(file->fd);
        }
    }

    return err;
}
#endif

#if 0
_AFX afxClassConfig const _AfxFileClsConfig =
{
    .fcc = afxFcc_FILE,
    .name = "File",
    .unitsPerPage = 2,
    .size = sizeof(AFX_OBJECT(afxFile)),
    .mmu = NIL,
    .ctor = (void*)_AfxFileCtor,
    .dtor = (void*)NIL
};
#endif

////////////////////////////////////////////////////////////////////////////////

_AFX afxStream AfxOpenFile(afxUri const* uri, afxIoFlags flags)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(uri);
    afxStream file = NIL;

    afxStreamIdd idd = { 0 };

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
    
    //*modePtr++ = 'b';
    *modePtr++ = '+';

    afxUri path;
    AfxAssert(uri);
    AfxPickUriPath(uri, &path);
    AfxAssert(!AfxUriIsBlank(&path));
    afxUri2048 uri2;
    AfxMakeUri2048(&uri2, NIL);
    AfxResolveUri((afxFileFlags)flags, &path, &uri2.uri);

    AfxEcho("Opening file... <%.*s>('%.*s'),%x", AfxPushString(AfxGetUriString(uri)), AfxPushString(AfxGetUriString(&uri2.uri)), flags);

    afxChar const *rawName = AfxGetUriStorage(&uri2.uri, 0);

    if (!(idd.fd = fopen(rawName, mode))) AfxThrowError();
    else
    {
        long cachedPosn = ftell(idd.fd);

        if (0 != fseek(idd.fd, 0, SEEK_END))
            AfxThrowError();

        idd.src.len = ftell(idd.fd);
        fseek(idd.fd, cachedPosn, SEEK_SET);

        AfxDuplicateUri(&idd.path, &uri2.uri);

        if (!(file = AfxAcquireImplementedStream(ioFlags, &fileStreamImpl, &idd))) AfxThrowError();
        else
        {

        }

        if (err)
            fclose(idd.fd);
    }

    return file;
}

_AFX afxStream AfxLoadFile(afxUri const* uri)
{
    afxError err = AFX_ERR_NONE;
    afxStream iob = NIL;
    AfxAssert(uri);
    AfxAssert(!AfxUriIsBlank(uri));

    afxUri2048 path;
    AfxMakeUri2048(&path, NIL);
    AfxResolveUri(afxFileFlag_R, uri, &path.uri);

    AfxEcho("Loading file... <%.*s>('%.*s')", AfxPushString(AfxGetUriString(uri)), AfxPushString(AfxGetUriString(&path.uri)));

    afxStreamIdd idd = { 0 };

    afxChar const *rawName = AfxGetUriStorage(&path.uri, 0);

    if (!(idd.fd = fopen(rawName, "r"))) AfxThrowError();
    else
    {
        if (0 != fseek(idd.fd, 0, SEEK_END))
            AfxThrowError();

        idd.src.len = ftell(idd.fd);
        fseek(idd.fd, 0, SEEK_SET);

        if (!(iob = AfxAcquireStream(afxIoFlag_R, idd.src.len))) AfxThrowError();
        else
        {
            AfxAdjustStreamBuffer(iob, (afxNat)idd.src.len + 1);
            AfxRewindStream(iob);
            fread((void*)AfxGetStreamBuffer(iob, 0), sizeof(afxByte), idd.src.len, idd.fd);
            ((afxByte*)AfxGetStreamBuffer(iob, idd.src.len))[0] = '\0';
            AfxRewindStream(iob);
        }
        fclose(idd.fd);
    }
    return iob;
}

_AFX afxError AfxReloadFile(afxStream iob, afxUri const *uri)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(iob->pimpl == &stdStreamImpl);
    AfxAssert(uri);
    AfxAssert(!AfxUriIsBlank(uri));

    afxUri2048 path;
    AfxMakeUri2048(&path, NIL);
    AfxResolveUri(afxFileFlag_R, uri, &path.uri);

    AfxEcho("Reloading file %u... <%.*s>('%.*s')", AfxIdentifyObject(iob), AfxPushString(AfxGetUriString(uri)), AfxPushString(AfxGetUriString(&path.uri)));

    afxStreamIdd idd = { 0 };

    afxChar const *rawName = AfxGetUriStorage(&path.uri, 0);

    if (!(idd.fd = fopen(rawName, "r"))) AfxThrowError();
    else
    {
        if (0 != fseek(idd.fd, 0, SEEK_END))
            AfxThrowError();

        idd.src.len = ftell(idd.fd);
        fseek(idd.fd, 0, SEEK_SET);

        AfxAdjustStreamBuffer(iob, (afxNat)idd.src.len + 1);
        AfxRewindStream(iob);
        fread((void*)AfxGetStreamBuffer(iob, 0), sizeof(afxByte), idd.src.len, idd.fd);
        ((afxByte*)AfxGetStreamBuffer(iob, idd.src.len))[0] = '\0';
        AfxRewindStream(iob);
        
        fclose(idd.fd);
    }
    return err;
}

_AFX afxStream AfxWrapFile(void* fd, afxIoFlags flags)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(fd);
    afxStream file = NIL;

    AfxEcho("Wrapping file... <%p>,%x", fd, flags);

    afxStreamIdd idd = { 0 };
    idd.fd = fd;
    idd.isUserFd = TRUE;

    if (!(file = AfxAcquireImplementedStream(flags, &fileStreamImpl, &idd)))
        AfxThrowError();

    return file;
}

#if 0
_AFX afxNat AfxInvokeFiles(afxNat first, afxNat cnt, afxBool(*f)(afxFile, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(f);
    afxManager* cls = AfxGetFileClass();
    AfxAssertClass(cls, afxFcc_FILE);
    return AfxInvokeObjects(cls, first, cnt, (void*)f, udd);
}

_AFX afxNat AfxEnumerateFiles(afxNat first, afxNat cnt, afxFile files[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(files);
    afxManager* cls = AfxGetFileClass();
    AfxAssertClass(cls, afxFcc_FILE);
    return AfxEnumerateObjects(cls, first, cnt, (afxObject*)files);
}

_AFX afxNat AfxCountFiles(void)
{
    afxError err = AFX_ERR_NONE;
    afxManager* cls = AfxGetFileClass();
    AfxAssertClass(cls, afxFcc_FILE);
    return AfxCountObjects(cls);
}
#endif
