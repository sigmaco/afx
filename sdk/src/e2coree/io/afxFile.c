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

#define _CRT_SECURE_NO_WARNINGS 1
#include <stdio.h>

#define _AFX_CORE_C
#define _AFX_FILE_C
#define _AFX_STREAM_C
#include "qwadro/core/afxClass.h"
#include "qwadro/core/afxSystem.h"

_AFX afxBool _AfxFileStreamHasReachedEndCallback(afxStream ios)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ios, afxFcc_IOS);
    afxFile file = (afxFile)ios->idd;
    AfxAssertObjects(1, &file, afxFcc_FILE);
    afxBool b = !!(feof(file->fd));
    return b;
}

_AFX afxError _AfxFileStreamMoveCursorCallback(afxStream ios, afxInt offset, afxSeekMode origin)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ios, afxFcc_IOS);
    afxFile file = (afxFile)ios->idd;
    AfxAssertObjects(1, &file, afxFcc_FILE);

    if (NIL != fseek(file->fd, (long)offset, (int)origin))
        AfxThrowError();

    file->ios->posn = ftell(file->fd);

    return err;
}

_AFX afxNat _AfxFileStreamAskCursorCallback(afxStream ios)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ios, afxFcc_IOS);
    afxFile file = (afxFile)ios->idd;
    AfxAssertObjects(1, &file, afxFcc_FILE);

    afxNat rslt = ftell(file->fd);
    AfxAssert(rslt == (afxNat)file->ios->posn);
    file->ios->posn = rslt;

    return file->ios->posn;
}

_AFX afxError _AfxFileStreamWriteCallback(afxStream ios, void const* const src, afxSize siz)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ios, afxFcc_IOS);
    afxFile file = (afxFile)ios->idd;
    AfxAssertObjects(1, &file, afxFcc_FILE);

    AfxAssert(siz);
    AfxAssert(src);
    afxNat clampedOffRange = siz;

    afxNat written = (afxNat)fwrite(src, sizeof(afxByte), siz, file->fd);

    clampedOffRange -= written;
    file->ios->posn += written;
    AfxAssert(file->ios->posn == (afxNat)ftell(file->fd));
    return clampedOffRange;
}

_AFX afxError _AfxFileStreamReadCallback(afxStream ios, void *dst, afxSize siz)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ios, afxFcc_IOS);
    afxFile file = (afxFile)ios->idd;
    AfxAssertObjects(1, &file, afxFcc_FILE);

    AfxAssert(siz);
    AfxAssert(dst);
    afxNat clampedOffRange = siz;

    if (file->shouldBeFlushed)
    {
        fflush(file->fd);
        file->shouldBeFlushed = FALSE;
    }

    afxNat read = (afxNat)fread(dst, sizeof(afxByte), siz, file->fd);
    
    clampedOffRange -= read;
    file->ios->posn += read;
    AfxAssert(file->ios->posn == (afxNat)ftell(file->fd));
    return clampedOffRange;
}

_AFX afxBool AfxFileIsVirtual(afxFile file)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &file, afxFcc_FILE);
    return AfxFlagsTest(file->flags, AFX_FILE_FLAG_V);
}

_AFX afxBool AfxFileIsArchive(afxFile file)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &file, afxFcc_FILE);
    return AfxFlagsTest(file->flags, AFX_FILE_FLAG_A);
}

_AFX afxBool AfxFileIsTemporary(afxFile file)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &file, afxFcc_FILE);
    return AfxFlagsTest(file->flags, AFX_FILE_FLAG_T);
}

_AFX afxBool AfxFileIsSparse(afxFile file)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &file, afxFcc_FILE);
    return AfxFlagsTest(file->flags, AFX_FILE_FLAG_S);
}

_AFX afxBool AfxFileIsCompressed(afxFile file)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &file, afxFcc_FILE);
    return AfxFlagsTest(file->flags, AFX_FILE_FLAG_Z);
}

_AFX afxBool AfxFileIsEncrypted(afxFile file)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &file, afxFcc_FILE);
    return AfxFlagsTest(file->flags, AFX_FILE_FLAG_E);
}

_AFX afxBool AfxFileIsHidden(afxFile file)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &file, afxFcc_FILE);
    return AfxFlagsTest(file->flags, AFX_FILE_FLAG_H);
}

_AFX afxBool AfxFileIsReserved(afxFile file)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &file, afxFcc_FILE);
    return AfxFlagsTest(file->flags, AFX_FILE_FLAG_Q);
}

_AFX afxBool AfxFileShouldBeFlushed(afxFile file)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &file, afxFcc_FILE);
    return file->shouldBeFlushed;
}

_AFX afxResult AfxFlushFile(afxFile file)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &file, afxFcc_FILE);

    if (fflush(file->fd))
        AfxThrowError();

    return err;
}

_AFX afxStream AfxGetFileStream(afxFile file)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &file, afxFcc_FILE);
    return file->ios;
}

_AFX afxError AfxReadFileString(afxFile file, afxBufferedString* str)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &file, afxFcc_FILE);
    AfxAssertType(str, afxFcc_STR);
    AfxAssert(AfxStringIsWriteable(str));
    
    if (!fgets(str->str.buf, str->cap, file->fd))
        AfxThrowError();

    return err;
}

_AFX afxResult AfxExtractFilePath(afxFile file, afxBufferedString *str)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &file, afxFcc_FILE);
    return AfxCopyString(str, AfxGetUriString(&file->path));
}

_AFX afxString const* AfxGetFilePathString(afxFile file)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &file, afxFcc_FILE);
    return AfxGetUriString(&file->path);
}

_AFX afxUri const* AfxGetFilePath(afxFile file)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &file, afxFcc_FILE);
    return &(file->path);
}

_AFX void* AfxGetFileHostDescriptor(afxFile file)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &file, afxFcc_FILE);
    return file->fd;
}

_AFX afxResult _AfxFileDtor(afxFile file)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &file, afxFcc_FILE);

    AfxDeallocateUri(&file->path);

    AfxReleaseObjects(1, (void*[]) { file->ios });

    if (file->fd)
    {
        if (file->shouldBeFlushed)
        {
            fflush(file->fd);
            file->shouldBeFlushed = FALSE;
        }

        //setvbuf(file->fd, NIL, _IONBF, BUFSIZ);
        fclose(file->fd);
    }
    
    return err;
}

_AFX afxError _AfxFileCtor(afxFile file, afxCookie const* cookie)
{
    afxError err = AFX_ERR_NONE;
    
    afxFileFlags flags = *(afxFileFlags const*)(cookie->udd[0]);
    afxUri const* uri = ((afxUri const*)cookie->udd[1]) + cookie->no;
    
    file->shouldBeFlushed = FALSE;

    afxChar mode[9] = { NIL }, *modePtr = mode;
    afxIoFlags ioFlags = NIL;

    if (AfxFlagsTest(flags, AFX_FILE_FLAG_W))
    {
        ioFlags |= afxIoFlag_W;
        *modePtr++ = 'w';
    }
    else if (AfxFlagsTest(flags, AFX_FILE_FLAG_R))
    {
        ioFlags |= afxIoFlag_R;
        *modePtr++ = 'r';
    }

    if (AfxFlagsTest(flags, AFX_FILE_FLAG_X))
    {
        ioFlags |= afxIoFlag_X;
        //*modePtr++ = 'x';
    }

    *modePtr++ = 'b';
    *modePtr++ = '+';

    afxUri path;
    AfxAssertType(uri, afxFcc_URI);
    

    if (!AfxGetUriPath(&path, uri)) AfxThrowError();
    else
    {
        afxFixedUri2048 uri2;
        AfxMakeFixedUri2048(&uri2, NIL);

        AfxResolveUri(flags, &path, &uri2.uri);
        
        afxChar const *rawName = AfxGetBufferedUriData(&uri2.uri, 0);

        if (!(file->fd = fopen(rawName, mode))) AfxThrowError();
        else
        {
            if (!(file->ios = AfxAcquireStream(0, ioFlags, NIL, 0))) AfxThrowError();
            else
            {
                AfxAssertObjects(1, &file->ios, afxFcc_IOS);

                //void const *ptr = AfxGetStreamData(file->ios, 0); // force buffer existence
                //afxNat strBufCap = AfxGetStreamBufferCap(file->ios);

                //if (0 != setvbuf(file->fd, (void*)ptr, _IOFBF, strBufCap)) AfxThrowError();
                //else
                {
                    file->ios->ioctl.read = _AfxFileStreamReadCallback;
                    file->ios->ioctl.readFeedback = NIL;
                    file->ios->ioctl.write = _AfxFileStreamWriteCallback;
                    file->ios->ioctl.writeFeedback = NIL;
                    file->ios->ioctl.eos = _AfxFileStreamHasReachedEndCallback;
                    file->ios->ioctl.seek = _AfxFileStreamMoveCursorCallback;
                    file->ios->ioctl.tell = _AfxFileStreamAskCursorCallback;
                    
                     AfxCloneUri(&file->path, &uri2.uri);
                    file->ios->idd = (void*)file;
                }

                if (err)
                    AfxReleaseObjects(1, (void*[]) { file->ios });
            }

            if (err)
                fclose(file->fd);
        }
    }

    return err;
}

_AFX afxClassConfig _AfxFileClsConfig =
{
    .fcc = afxFcc_FILE,
    .name = "File",
    .unitsPerPage = 2,
    .size = sizeof(AFX_OBJECT(afxFile)),
    .mmu = NIL,
    .ctor = (void*)_AfxFileCtor,
    .dtor = (void*)_AfxFileDtor
};

////////////////////////////////////////////////////////////////////////////////

_AFX afxError AfxOpenFiles(afxFileFlags flags, afxNat cnt, afxUri const uri[], afxFile file[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(uri, afxFcc_URI);
    AfxAssert(uri);

    afxClass* cls = AfxGetFileClass();
    AfxAssertClass(cls, afxFcc_FILE);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)file, (void const*[]) { &flags, uri }))
        AfxThrowError();

    return err;
}

_AFX afxError AfxLoadFiles(afxFileFlags flags, afxNat cnt, afxUri const uri[], afxFile file[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(uri, afxFcc_URI);
    AfxAssert(uri);

    afxClass* cls = AfxGetFileClass();
    AfxAssertClass(cls, afxFcc_FILE);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)file, (void const*[]) { &flags, uri })) AfxThrowError();
    else
    {
        for (afxNat i = 0; i < cnt; i++)
        {
            AfxReadjustStreamBuffer(file[i]->ios, AfxMeasureStream(AfxGetFileStream(file[i])));
            AfxGoToStreamBegin(file[i]->ios, 0);
            
            AfxThrowError();
            AfxGoToStreamBegin(file[i]->ios, 0);
        }
    }

    return err;
}

_AFX afxError AfxReloadFile(afxStream ios, afxFileFlags flags, afxUri const *uri)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(uri, afxFcc_URI);
    AfxEntry("uri=%p,flags=%x", uri, flags);
    AfxAssert(uri);
    afxFile file;

    if (AfxOpenFiles(flags, 1, uri, &file)) AfxThrowError();
    else
    {
        AfxReadjustStreamBuffer(ios, AfxMeasureStream(AfxGetFileStream(file)));
        AfxGoToStreamBegin(ios, 0);
        AfxCopyStream(ios, file->ios, 0);
        AfxGoToStreamBegin(ios, 0);
        AfxReleaseObjects(1, (void*[]) { file });
    }
    return err;
}

_AFX afxNat AfxCurateFiles(afxNat first, afxNat cnt, afxBool(*f)(afxFile, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(f);
    afxClass* cls = AfxGetFileClass();
    AfxAssertClass(cls, afxFcc_FILE);
    return AfxCurateInstances(cls, first, cnt, (void*)f, udd);
}

_AFX afxNat AfxEnumerateFiles(afxNat first, afxNat cnt, afxFile file[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(file);
    afxClass* cls = AfxGetFileClass();
    AfxAssertClass(cls, afxFcc_FILE);
    return AfxEnumerateInstances(cls, first, cnt, (afxObject*)file);
}

_AFX afxNat AfxCountFiles(void)
{
    afxError err = AFX_ERR_NONE;
    afxClass* cls = AfxGetFileClass();
    AfxAssertClass(cls, afxFcc_FILE);
    return AfxCountInstances(cls);
}
