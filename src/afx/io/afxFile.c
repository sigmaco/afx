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

#define _CRT_SECURE_NO_WARNINGS 1
#include <stdio.h>

#define _AFX_CORE_C
#define _AFX_FILE_C
#define _AFX_STREAM_C
#include "../src/afx/dev/afxDevIoBase.h"

////////////////////////////////////////////////////////////////////////////////

_AFX afxBool _AfxFileStreamEosCb(afxStream file)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &file, afxFcc_IOB);
    afxBool b = !!(feof(file->idd.f.fd));
    return b;
}

_AFX afxError _AfxFileStreamSeekCb(afxStream file, afxSize offset, afxSeekMode origin)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &file, afxFcc_IOB);

    if (NIL != fseek(file->idd.f.fd, (long)offset, (int)origin))
        AfxThrowError();

    return err;
}

_AFX afxNat _AfxFileStreamTellCb(afxStream file)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &file, afxFcc_IOB);
    return ftell(file->idd.f.fd);
}

_AFX afxError _AfxFileStreamWriteCb(afxStream file, void const* const src, afxNat32 siz)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &file, afxFcc_IOB);
    AfxAssert(siz);
    AfxAssert(src);
    afxNat clampedOffRange = siz;
    afxNat written = (afxNat)fwrite(src, sizeof(afxByte), siz, file->idd.f.fd);
    clampedOffRange -= written;
    return clampedOffRange;
}

_AFX afxError _AfxFileStreamReadCb(afxStream file, void *dst, afxNat32 siz)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &file, afxFcc_IOB);
    AfxAssert(siz);
    AfxAssert(dst);
    afxNat clampedOffRange = siz;

    if (file->idd.f.shouldBeFlushed)
    {
        fflush(file->idd.f.fd);
        file->idd.f.shouldBeFlushed = FALSE;
    }

    afxNat read = (afxNat)fread(dst, sizeof(afxByte), siz, file->idd.f.fd);
    clampedOffRange -= read;
    return clampedOffRange;
}

_AFX afxResult _AfxFileStreamDtorCb(afxStream file)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &file, afxFcc_IOB);

    AfxDeallocateUri(&file->idd.f.path);

    if (file->idd.f.fd)
    {
        if (file->idd.f.shouldBeFlushed)
        {
            fflush(file->idd.f.fd);
            file->idd.f.shouldBeFlushed = FALSE;
        }

        //setvbuf(file->idd.f.fd, NIL, _IONBF, BUFSIZ);
        fclose(file->idd.f.fd);
    }

    return err;
}

_afxIobImpl const fileStreamImpl =
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
    return file->idd.f.shouldBeFlushed;
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

    if (fflush(file->idd.f.fd))
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
    
    err = !fgets(str->str.buf, str->cap, file->idd.f.fd);

    str->str.len = AfxStrlen(str->str.start);

    return err;
}

_AFX afxUri const* AfxGetFilePath(afxStream file)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &file, afxFcc_IOB);
    AfxAssert(file->pimpl == &fileStreamImpl);
    return &(file->idd.f.path);
}

_AFX void* AfxGetFileDescriptor(afxStream file)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &file, afxFcc_IOB);
    AfxAssert(file->pimpl == &fileStreamImpl);
    return file->idd.f.fd;
}

_AFX afxResult AfxCopyFilePath(afxStream file, afxUri* uri)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &file, afxFcc_IOB);
    AfxAssert(file->pimpl == &fileStreamImpl);
    AfxAssert(uri);
    return AfxCopyUri(uri, &file->idd.f.path);
}

_AFX afxString const* AfxGetFilePathString(afxStream file)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &file, afxFcc_IOB);
    AfxAssert(file->pimpl == &fileStreamImpl);
    return AfxGetUriString(&file->idd.f.path);
}

_AFX afxResult AfxCopyFilePathString(afxStream file, afxRestring* str)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &file, afxFcc_IOB);
    AfxAssert(file->pimpl == &fileStreamImpl);
    return AfxCopyString(str, AfxGetUriString(&file->idd.f.path));
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
            Result = AfxAllocate(1, alignedSiz, sec->internalAlignment, AfxHere());
            result = Result;
        }

        if (result)
        {
            if (sec->fmt)
            {
                afxNat v8 = AfxGetCompressionPaddingSize(sec->fmt);
                void *v9 = AfxAllocate(1, sec->dataSiz + v8, 0, AfxHere());

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
    

    if (!AfxClipUriPath(&path, uri)) AfxThrowError();
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
_AFX afxClassConfig const _AfxFileMgrCfg =
{
    .fcc = afxFcc_FILE,
    .name = "File",
    .fixedSiz = sizeof(AFX_OBJECT(afxFile)),
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

    _afxIobIdd idd = { 0 };

    afxChar mode[9] = { NIL }, *modePtr = mode;
    afxIoFlags ioFlags = NIL;

    if (AfxTestFlags(flags, afxFileFlag_W))
    {
        ioFlags |= afxIoFlag_W;
        *modePtr++ = 'w';
    } else if (AfxTestFlags(flags, afxFileFlag_R))
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

    afxUri path;
    AfxAssert(uri);
    AfxClipUriPath(&path, uri);
    AfxAssert(!AfxUriIsBlank(&path));
    afxUri2048 uri2;
    AfxMakeUri2048(&uri2, NIL);
    AfxResolveUri((afxFileFlags)flags, &path, &uri2.uri);

    AfxLogEcho("Opening file... <%.*s>('%.*s'),%x", AfxPushString(AfxGetUriString(uri)), AfxPushString(AfxGetUriString(&uri2.uri)), flags);

    afxChar const *rawName = AfxGetUriStorage(&uri2.uri, 0);

    if (!(idd.f.fd = fopen(rawName, mode))) AfxThrowError();
    else
    {
        long cachedPosn = ftell(idd.f.fd);

        if (0 != fseek(idd.f.fd, 0, SEEK_END))
            AfxThrowError();

        fseek(idd.f.fd, cachedPosn, SEEK_SET);

        AfxDuplicateUri(&idd.f.path, &uri2.uri);

        if (!(file = AfxAcquireImplementedStream(ioFlags, &fileStreamImpl, &idd))) AfxThrowError();
        else
        {

        }

        if (err)
            fclose(idd.f.fd);
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

    AfxLogEcho("Loading file... <%.*s>('%.*s')", AfxPushString(AfxGetUriString(uri)), AfxPushString(AfxGetUriString(&path.uri)));

    afxChar const *rawName = AfxGetUriStorage(&path.uri, 0);
    FILE* fd;

    if (!(fd = fopen(rawName, "rb"))) AfxThrowError();
    else
    {
        if (0 != fseek(fd, 0, SEEK_END))
            AfxThrowError();

        afxNat len = ftell(fd);
        fseek(fd, 0, SEEK_SET);

        if (!(iob = AfxAcquireStream(afxIoFlag_R, len))) AfxThrowError();
        else
        {
            afxByte* buf = (afxByte*)AfxGetStreamBuffer(iob, 0);
            AfxRewindStream(iob);
            fread(buf, sizeof(afxByte), len, fd);
            buf[len] = '\0';
        }
        fclose(fd);
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

    AfxLogEcho("Reloading file %u... <%.*s>('%.*s')", AfxGetObjectId(iob), AfxPushString(AfxGetUriString(uri)), AfxPushString(AfxGetUriString(&path.uri)));

    afxChar const *rawName = AfxGetUriStorage(&path.uri, 0);
    FILE* fd;

    if (!(fd = fopen(rawName, "rb"))) AfxThrowError();
    else
    {
        if (0 != fseek(fd, 0, SEEK_END))
            AfxThrowError();

        afxNat len = ftell(fd);
        fseek(fd, 0, SEEK_SET);

        AfxAdjustStreamBuffer(iob, (afxNat)len);
        AfxRewindStream(iob);
        afxByte* buf = (afxByte*)AfxGetStreamBuffer(iob, 0);
        fread(buf, sizeof(afxByte), len, fd);
        buf[len] = '\0';
        
        fclose(fd);
    }
    return err;
}

_AFX afxStream AfxWrapFile(void* fd, afxIoFlags flags)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(fd);
    afxStream file = NIL;

    AfxLogEcho("Wrapping file... <%p>,%x", fd, flags);

    _afxIobIdd idd = { 0 };
    idd.f.fd = fd;
    idd.f.isUserFd = TRUE;

    if (!(file = AfxAcquireImplementedStream(flags, &fileStreamImpl, &idd)))
        AfxThrowError();

    return file;
}
