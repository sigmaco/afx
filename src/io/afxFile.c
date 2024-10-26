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
#define WIN32_LEAN_AND_MEAN 1
#include <Windows.h>
#include <sys/stat.h>
#include <stdio.h>

#define _AFX_CORE_C
#define _AFX_FILE_C
#define _AFX_STREAM_C
#define _AFX_STORAGE_C
#include "../dev/afxIoImplKit.h"
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
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &file, afxFcc_IOB);
    AFX_ASSERT(file->typeFcc == afxFcc_FILE);

    afxBool b = !!(feof(file->idd.f.fd));
    return b;
}

_AFX afxError _AfxIobSeekFileCb(afxStream file, afxSize offset, afxSeekOrigin origin)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &file, afxFcc_IOB);
    AFX_ASSERT(file->typeFcc == afxFcc_FILE);

    if (NIL != fseek(file->idd.f.fd, (long)offset, (int)origin))
        AfxThrowError();

    return err;
}

_AFX afxUnit _AfxIobTellFileCb(afxStream file)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &file, afxFcc_IOB);
    AFX_ASSERT(file->typeFcc == afxFcc_FILE);
    return ftell(file->idd.f.fd);
}

_AFX afxError _AfxIobWriteFileCb(afxStream file, void const* const src, afxUnit32 siz)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &file, afxFcc_IOB);
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
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &file, afxFcc_IOB);
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
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &file, afxFcc_IOB);
    AFX_ASSERT(file->typeFcc == afxFcc_FILE);

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

    AfxPopLinkage(&file->idd.f.disk);

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
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &file, afxFcc_IOB);
    AFX_ASSERT(file->typeFcc == afxFcc_FILE);
    AFX_ASSERT(file->pimpl == &fileStreamImpl);

    if (fflush(file->idd.f.fd))
        AfxThrowError();

    return err;
}

_AFX afxError AfxReadFileLine(afxStream file, afxString* str)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &file, afxFcc_IOB);
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
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &file, afxFcc_IOB);
    AFX_ASSERT(file->typeFcc == afxFcc_FILE);
    return file->flags & flags;
}

_AFX afxBool AfxGetFileUri(afxStream file, afxUri* location)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &file, afxFcc_IOB);
    AFX_ASSERT(file->typeFcc == afxFcc_FILE);
    AFX_ASSERT(location);
    AfxCopyUri(location, &file->idd.f.url.uri);
    return !err;
}

_AFX afxBool AfxGetResolvedFileUri(afxStream file, afxUri* location)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &file, afxFcc_IOB);
    AFX_ASSERT(file->typeFcc == afxFcc_FILE);
    AFX_ASSERT(location);
    AfxCopyUri(location, &file->idd.f.resolvedUrl.uri);
    AfxCanonicalizePath(location, AFX_OS_WIN);
    return !err;
}

_AFX afxBool _AfxOpenFileCb(void* udd, afxUri const* path, afxUri const* osPath)
{
    afxError err = AFX_ERR_NONE;
    afxBool next = TRUE;

    afxStream file = ((void**)udd)[0];
    AfxAssertObjects(1, &file, afxFcc_IOB);
    AFX_ASSERT(file->typeFcc == afxFcc_FILE);
    afxFileFlags flags = *(afxFileFlags*)((void**)udd)[1];

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

    afxChar const *rawName = AfxGetUriData(osPath, 0);

    if (file->idd.f.fd)
        fclose(file->idd.f.fd), file->idd.f.fd = NIL;

    if (!(file->idd.f.fd = fopen(rawName, mode)))
        AfxThrowError();
    else
    {
        long cachedPosn = ftell(file->idd.f.fd);

        if (0 != fseek(file->idd.f.fd, 0, SEEK_END))
            AfxThrowError();

        fseek(file->idd.f.fd, cachedPosn, SEEK_SET);

        file->pimpl = &fileStreamImpl; // reload can have replaced it.

        AfxCopyUri(&file->idd.f.resolvedUrl.uri, osPath);
        AfxCopyUri(&file->idd.f.url.uri, path);

        file->length = AfxMeasureStream(file);

        if (err)
            fclose(file->idd.f.fd), file->idd.f.fd = NIL;
    }

    if (!err)
        next = FALSE;

    return next;
}

_AFX afxError AfxReopenFile(afxStream iob, afxUri const* uri, afxFileFlags flags)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &iob, afxFcc_IOB);
    AfxLogEcho("Reopening... <%.*s>", AfxPushString(AfxGetUriString(uri)));

    AfxResetStream(iob); // anyway, reset it.

    afxStorage disk;
    afxUri dev, path;
    AfxSplitPath(uri, &dev, &path);

    if (AfxIsUriBlank(&dev)) AfxEnumerateStorages(0, 1, &disk);
    else if (!AfxFindStorage(uri, &disk))
        disk = NIL;

    if (!disk) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &disk, afxFcc_FSYS);

        iob->typeFcc = afxFcc_FILE;
        iob->pimpl = &fileStreamImpl;
        iob->idd.f.fflags = flags;
        iob->idd.f.fd = NIL;
        AfxMakeUri2048(&iob->idd.f.resolvedUrl, NIL);
        AfxMakeUri2048(&iob->idd.f.url, NIL);
        AfxPushLinkage(&iob->idd.f.disk, &disk->fileChain);

        if (!AfxFindFiles(uri, flags, _AfxOpenFileCb, (void*[]) { iob, &flags }))
        {
            AfxThrowError();
            AfxPopLinkage(&iob->idd.f.disk);
            AfxLogError("Couldn't open <%.*s>", AfxPushString(uri ? &uri->str : &AFX_STR_EMPTY));
        }
        else if (!iob->idd.f.fd)
            AfxThrowError();
    }
    return err;
}

_AFX afxError AfxReloadFile(afxStream iob, afxUri const* uri)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &iob, afxFcc_IOB);
    AfxAssert2(uri, !AfxIsUriBlank(uri));

    if (AfxReopenFile(iob, uri, afxFileFlag_R)) AfxThrowError();
    else
    {
        long cachedPosn = ftell(iob->idd.f.fd);

        if (0 != fseek(iob->idd.f.fd, 0, SEEK_END))
            AfxThrowError();

        long siz = ftell(iob->idd.f.fd);
        fseek(iob->idd.f.fd, cachedPosn, SEEK_SET);

        if (AfxAdjustStreamBuffer(iob, siz)) AfxThrowError();
        else
        {
            afxByte* buf = (afxByte*)AfxGetStreamBuffer(iob, 0);
            AfxRewindStream(iob);
            fread(buf, sizeof(afxByte), siz, iob->idd.f.fd);
            buf[siz] = '\0';
            iob->length = siz;

            iob->pimpl = &stdStreamImpl; // reload can have replaced it.

            fclose(iob->idd.f.fd), iob->idd.f.fd = NIL;
        }
    }
    return err;
}

#if 0
_AFX afxError AfxWrapFile(afxStream iob, void* fd, afxIoFlags flags)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(fd);
    afxStream file = NIL;

    AfxLogEcho("Wrapping file... <%p>,%x", fd, flags);

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
