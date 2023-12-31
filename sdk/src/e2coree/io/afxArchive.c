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
#define WIN32_LEAN_AND_MEAN 1
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define _AFX_CORE_C
#define _AFX_FILE_C
#define _AFX_ARCHIVE_C
#include "qwadro/core/afxSystem.h"
#include "qwadro/mem/afxMmu.h"
#include "qwadro/core/afxString.h"
#include "qwadro/async/afxThread.h"
#include "qwadro/io/afxArchive.h"
#include "qwadro/core/afxClass.h"


typedef struct zip zip;

#pragma pack(push, 1)

typedef struct
{
    afxNat32 signature; // 0x04034B50
    afxNat16 versionNeededToExtract; // unsupported
    afxNat16 generalPurposeBitFlag; // unsupported
    afxNat16 compressionMethod;
    afxNat16 lastModFileTime;
    afxNat16 lastModFileDate;
    afxNat32 crc32;
    afxNat32 compressedSize;
    afxNat32 uncompressedSize;
    afxNat16 fileNameLength;
    afxNat16 extraFieldLength; // unsupported
} _afxZipSerializedLocalEntryHdr; // local file record

typedef struct
{
    afxNat32 signature; // 0x02014B50
    afxNat16 versionMadeBy; // unsupported
    afxNat16 versionNeededToExtract; // unsupported
    afxNat16 generalPurposeBitFlag; // unsupported
    afxNat16 compressionMethod; // 0-store,8-deflate
    afxNat16 lastModFileTime;
    afxNat16 lastModFileDate;
    afxNat32 crc32;
    afxNat32 compressedSize;
    afxNat32 uncompressedSize;
    afxNat16 fileNameLength;
    afxNat16 extraFieldLength; // unsupported
    afxNat16 fileCommentLength; // unsupported
    afxNat16 diskNumberStart; // unsupported
    afxNat16 internalFileAttributes; // unsupported
    afxNat32 externalFileAttributes; // unsupported
    afxNat32 relativeOffsetOflocalHeader;
} _afxZipSerializedCdEntryHdr; // global file record

typedef struct
{
    afxNat32 signature; // 0x06054b50
    afxNat16 diskNumber; // unsupported
    afxNat16 centralDirectoryDiskNumber; // unsupported
    afxNat16 numEntriesThisDisk; // unsupported
    afxNat16 numEntries;
    afxNat32 centralDirectorySize;
    afxNat32 centralDirectoryOffset;
    afxNat16 zipCommentLength;
    // Followed by .ZIP file comment (variable size)
} _afxZipSerializedCdHdr;

#pragma pack(pop)

typedef struct _afxZipEntry
{
    //_afxZipSerializedCdEntryHdr hdr;
    afxFixedUri128   path;
    afxSize     offset;
    afxNat32    compressedSize;
    afxNat32    uncompressedSize;
    afxNat32    crc32;
    afxNat32    codec;
} _afxZipEntry;

struct zip
{
    FILE *in, *out;
    _afxZipEntry *entries;
    unsigned count;
};

static_assert(sizeof(_afxZipSerializedLocalEntryHdr) == 30, "");
static_assert(sizeof(_afxZipSerializedCdEntryHdr) == 46, "");
static_assert(sizeof(_afxZipSerializedCdHdr) == 22, "");

#define _AFX_Z_BUFSIZ 65536

typedef afxError (*_afxZipRecordCallback)(afxArchive arc, afxNat idx, _afxZipSerializedCdEntryHdr *hdr, afxUri const *path, void *extra, char *comment, void *user_data);
#if 0
_AFX afxBool _AfxGetArcD(afxArchive arc, struct _afxArcD **arc, struct _afxSysD* sysD)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &arc, afxFcc_ARC);
    AfxAssert(arc);
    AfxAssertType(sysD, afxFcc_SYS);
    return AfxExposeResidentObjects(1, &arc, (void**)arc, &theSys->archives);
}
#endif

_AFX afxError _AfxZipFindAndReadCdHdr(afxArchive arc, _afxZipSerializedCdHdr *endRecord)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &arc, afxFcc_ARC);

    afxStream ios = AfxGetFileStream(&arc->file);

    if (AfxGoToStreamEnd(ios, 0)) AfxThrowError();
    else
    {
        afxNat fileSize;
        _afxZipSerializedCdHdr *er;

        if ((fileSize = AfxAskStreamPosn(ios)) <= sizeof(*er)) AfxThrowError();
        else
        {
            unsigned char buffer[_AFX_Z_BUFSIZ]; // maximum zip descriptor size
            afxNat readBytes = (fileSize < sizeof(buffer)) ? fileSize : sizeof(buffer);

            if (AfxGoToStreamBegin(ios, fileSize - readBytes)) AfxThrowError();
            else
            {
                if (AfxReadStream(ios, buffer, readBytes, 0)) AfxThrowError();
                else
                {
                    // Naively assume signature can only be found in one place...
                    afxNat i;

                    for (i = readBytes - sizeof(*er); i >= 0; i--)
                    {
                        er = (_afxZipSerializedCdHdr *)(buffer + i);

                        if (er->signature == 0x06054B50)
                            break;
                    }

                    if (i < 0) AfxThrowError();
                    else
                    {

                        AfxCopy(1, sizeof(*er), er, endRecord);

                        _afxZipSerializedCdHdr *e = endRecord;
                        AfxComment("signature: 0x%X", e->signature); // 0x06054b50
                        AfxComment("diskNumber: %d", e->diskNumber); // unsupported
                        AfxComment("centralDirectoryDiskNumber: %d", e->centralDirectoryDiskNumber); // unsupported
                        AfxComment("numEntriesThisDisk: %d", e->numEntriesThisDisk); // unsupported
                        AfxComment("numEntries: %d", e->numEntries);
                        AfxComment("centralDirectorySize: %u %#x", e->centralDirectorySize, e->centralDirectorySize);
                        AfxComment("centralDirectoryOffset: %u %#x", e->centralDirectoryOffset, e->centralDirectoryOffset);
                        AfxComment("zipCommentLength: %d", e->zipCommentLength);

                        if (endRecord->diskNumber || endRecord->centralDirectoryDiskNumber || endRecord->numEntries != endRecord->numEntriesThisDisk)
                        {
                            AfxThrowError();
                            AfxError("Qwadro doesn't support splitted Zips.");
                        }
                    }
                }
            }
        }
    }
    return err;
}

_AFX afxError _AfxZipReadWholeCd(afxArchive arc, _afxZipSerializedCdHdr *endRecord, _afxZipRecordCallback callback, void *user_data)
{
    _afxZipSerializedCdEntryHdr fileHdr;
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &arc, afxFcc_ARC);

    afxStream ios = AfxGetFileStream(&arc->file);

    if (AfxGoToStreamBegin(ios, endRecord->centralDirectoryOffset)) AfxThrowError();
    else
    {
        for (afxInt i = 0; i < endRecord->numEntries; i++)
        {
            long offset = AfxAskStreamPosn(ios); // store current position
            AfxComment("Archived item %u -> %lu %#lx", i, (unsigned long)offset, (unsigned long)offset);
            
            if (AfxReadStream(ios, &fileHdr, sizeof(fileHdr), 0)) AfxThrowError();
            else
            {
                _afxZipSerializedCdEntryHdr *g = &fileHdr;
                AfxComment("signature: %u %#x", g->signature, g->signature); // 0x02014B50
                AfxComment("versionMadeBy: %u %#x", g->versionMadeBy, g->versionMadeBy); // unsupported
                AfxComment("versionNeededToExtract: %u %#x", g->versionNeededToExtract, g->versionNeededToExtract); // unsupported
                AfxComment("generalPurposeBitFlag: %u %#x", g->generalPurposeBitFlag, g->generalPurposeBitFlag); // unsupported
                AfxComment("compressionMethod: %u %#x", g->compressionMethod, g->compressionMethod); // 0-store,8-deflate
                AfxComment("lastModFileTime: %u %#x", g->lastModFileTime, g->lastModFileTime);
                AfxComment("lastModFileDate: %u %#x", g->lastModFileDate, g->lastModFileDate);
                AfxComment("crc32: %#x", g->crc32);
                AfxComment("compressedSize: %u", g->compressedSize);
                AfxComment("uncompressedSize: %u", g->uncompressedSize);
                AfxComment("fileNameLength: %u", g->fileNameLength);
                AfxComment("textraFieldLength: %u", g->extraFieldLength); // unsupported
                AfxComment("fileCommentLength: %u", g->fileCommentLength); // unsupported
                AfxComment("diskNumberStart: %u", g->diskNumberStart); // unsupported
                AfxComment("internalFileAttributes: %#x", g->internalFileAttributes); // unsupported
                AfxComment("externalFileAttributes: %#x", g->externalFileAttributes); // unsupported
                AfxComment("relativeOffsetOflocalHeader: %u %#x", g->relativeOffsetOflocalHeader, g->relativeOffsetOflocalHeader);

                if (fileHdr.signature != 0x02014B50) AfxThrowError();
                else
                {
                    AfxAssert(fileHdr.fileNameLength);

                    if (fileHdr.fileNameLength + 1 >= _AFX_Z_BUFSIZ) AfxThrowError();
                    else
                    {
                        // filename
                        char filename[_AFX_Z_BUFSIZ / 3];

                        if (AfxReadStream(ios, filename, fileHdr.fileNameLength, 0)) AfxThrowError();
                        else
                        {
                            filename[fileHdr.fileNameLength] = '\0'; // NULL terminate
                            
                            afxUri path;
                            AfxMakeUri(&path, filename, fileHdr.fileNameLength);

                            // extra block
                            unsigned char extra[_AFX_Z_BUFSIZ / 3] = { '\0' };

                            if (fileHdr.extraFieldLength)
                            {
                                if (AfxReadStream(ios, extra, fileHdr.extraFieldLength, 0))
                                    AfxThrowError();

                                extra[fileHdr.extraFieldLength] = '\0';
                            }
                            
                            if (!err)
                            {
                                // comment block
                                char comment[_AFX_Z_BUFSIZ / 3] = { '\0' };

                                if (fileHdr.fileCommentLength)
                                {
                                    if (AfxReadStream(ios, comment, fileHdr.fileCommentLength, 0))
                                        AfxThrowError();

                                    comment[fileHdr.fileCommentLength] = '\0'; // NULL terminate
                                }

                                if (!err)
                                {
                                    // seek to local file header, then skip file header + filename + extra field length
                                    afxNat16 localFileNameLength, localExtraFieldLength;

                                    if (AfxGoToStreamBegin(ios, fileHdr.relativeOffsetOflocalHeader + sizeof(_afxZipSerializedLocalEntryHdr) - sizeof(localFileNameLength) - sizeof(localExtraFieldLength))) AfxThrowError();
                                    else
                                    {
                                        if (AfxReadStream(ios, &localFileNameLength, sizeof(localFileNameLength), 0)) AfxThrowError();
                                        else
                                        {
                                            if (AfxReadStream(ios, &localExtraFieldLength, sizeof(localExtraFieldLength), 0)) AfxThrowError();
                                            else
                                            {
                                                if (AfxGoToStreamBegin(ios, fileHdr.relativeOffsetOflocalHeader + sizeof(_afxZipSerializedLocalEntryHdr) + localFileNameLength + localExtraFieldLength)) AfxThrowError();
                                                else
                                                {
                                                    AfxComment("Archived item %u, %lu %#lx", i, (unsigned long)AfxAskStreamPosn(ios), (unsigned long)AfxAskStreamPosn(ios));

                                                    if (callback(arc, i, &fileHdr, &path, extra, comment, user_data))
                                                        break; // keep going while callback returns ok

                                                    AfxGoToStreamBegin(ios, offset); // return to position
                                                    AfxSkipStream(ios, sizeof(_afxZipSerializedCdEntryHdr) + g->fileNameLength); // skip entry
                                                    AfxSkipStream(ios, g->extraFieldLength + g->fileCommentLength); // skip entry
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return err;
}

// Read data from file stream, described by header, to preallocated buffer.

_AFX afxError _AfxZipReadEntryData(afxArchive arc, _afxZipSerializedCdEntryHdr *header, void *out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &arc, afxFcc_ARC);

    afxStream ios = AfxGetFileStream(&arc->file);

    if (header->compressionMethod == 0) // Store
    {
        if (AfxReadStream(ios, &out, header->uncompressedSize, 0))
            AfxThrowError();
    }
    else
    {
        AfxThrowError();
        AfxError("Qwadro doesn't support compression.");
    }
    return err;
}

#define ZHOUR(t) ((t)>>11)
#define ZMINUTE(t) (((t)>>5) & 63)
#define ZSECOND(t) (((t) & 31) * 2)
#define ZTIME(h,m,s) (((h)<<11) + ((m)<<5) + (s)/2)

#define ZYEAR(t) (((t)>>9) + 1980)
#define ZMONTH(t) (((t)>>5) & 15)
#define ZDAY(t) ((t) & 31)
#define ZDATE(y,m,d) ((((y)-1980)<<9) + ((m)<<5) + (d))

_AFX afxError _AfxZipReadCdEntryCallback(afxArchive arc, afxNat idx, _afxZipSerializedCdEntryHdr *header, afxUri const *path, void *extra, char *comment, void *user_data)
{
    afxError err = AFX_ERR_NONE;
    (void)extra;
    (void)comment;
    (void)user_data;

    AfxAssertObjects(1, &arc, afxFcc_ARC);

    _afxZipEntry *e = AfxGetArrayUnit(&arc->entries, idx);
    AfxMakeFixedUri128(&e->path, NIL);
    AfxCopyUri(&e->path.uri, path);

    afxStream ios = AfxGetFileStream(&arc->file);

    e->offset = AfxAskStreamPosn(ios);
    e->uncompressedSize = header->uncompressedSize;
    e->compressedSize = header->compressedSize;
    e->codec = header->compressionMethod;
    e->crc32 = header->crc32;
    AfxEcho("%04d/%02d/%02d %02d:%02d:%02d #%04u %.*s", ZYEAR(header->lastModFileDate), ZMONTH(header->lastModFileDate), ZDAY(header->lastModFileDate), ZHOUR(header->lastModFileTime), ZMINUTE(header->lastModFileTime), ZSECOND(header->lastModFileTime), idx, AfxPushString(&path->str.str));
    return err;
}

_AFX afxNat AfxCountArchivedFiles(afxArchive arc)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &arc, afxFcc_ARC);

    AfxAssertType(&arc->entries, afxFcc_ARR);
    return AfxCountArrayElements(&arc->entries);
}

_AFX afxBool AfxFindArchivedFile(afxArchive arc, afxUri const *name, afxNat *idx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &arc, afxFcc_ARC);

    AfxAssertType(name, afxFcc_URI);
    AfxAssert(!AfxUriIsBlank(name));

    //_afxZipEntry const *e;
    //AfxForEachArrayUnit(&arc->entries, i, e)

    for (afxNat i = 0; i < AfxCountArrayElements(&arc->entries); ++i)
    {
        _afxZipEntry const *e = AfxGetArrayUnit(&arc->entries, i);
        // in case of several copies, grab most recent file (last coincidence)
        if (0 == AfxCompareUri(name, &e->path.uri))
        {
            AfxAssert(idx);
            *idx = i;
            return TRUE;
        }
    }
    return FALSE;
}

_AFX afxSize AfxGetArchivedFileOffset(afxArchive arc, afxNat idx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &arc, afxFcc_ARC);

    AfxAssert(idx < AfxCountArrayElements(&arc->entries));
    _afxZipEntry const *e = AfxGetArrayUnit(&arc->entries, idx);
    return e->offset;
}

_AFX afxBool AfxArchivedFileIsDirectory(afxArchive arc, afxNat idx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &arc, afxFcc_ARC);

    AfxAssert(idx < AfxCountArrayElements(&arc->entries));
    _afxZipEntry const *e = AfxGetArrayUnit(&arc->entries, idx);
    afxString const *nameStr = AfxGetUriString(&e->path.uri);
    afxNat nameLen = AfxGetStringLength(nameStr);
    afxChar const *rawName = AfxGetStringData(nameStr, 0);
    afxBool valid = FALSE;

    if (rawName[nameLen - 1] == '/')
        valid = TRUE;
#if 0
#ifdef _WIN32
    if (e->hdr.externalFileAttributes & /*FILE_ATTRIBUTE_DIRECTORY*/0x00000010)
        valid = FALSE;
#endif
#endif
    return valid;
}

_AFX afxError AfxDumpArchivedFile(afxArchive arc, afxNat idx, afxNat bufSiz, void *buf)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &arc, afxFcc_ARC);

    AfxAssert(idx < AfxCountArrayElements(&arc->entries));
    AfxAssert(bufSiz);
    AfxAssert(buf);

    _afxZipEntry const *e = AfxGetArrayUnit(&arc->entries, idx);
    AfxAssert(e);
    afxNat size = e->codec == 0 ? e->uncompressedSize : e->compressedSize;
    AfxAssert(bufSiz >= size);

    afxStream ios = AfxGetFileStream(&arc->file);
    
    if (AfxGoToStreamBegin(ios, e->offset)) AfxThrowError();
    else
    {
        if (AfxReadStream(ios, buf, size > bufSiz ? bufSiz : size, 0))
            AfxThrowError();
    }
    return err;
}

_AFX afxError AfxForkArchivedFile(afxArchive arc, afxNat idx, afxStream *ios)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &arc, afxFcc_ARC);

    AfxAssert(idx < AfxCountArrayElements(&arc->entries));
    AfxAssertObjects(1, &ios, afxFcc_IOS);

    _afxZipEntry const *e = AfxGetArrayUnit(&arc->entries, idx);
    AfxAssert(e);
    afxNat size = e->codec == 0 ? e->uncompressedSize : e->compressedSize;
    afxStream ios2;

    if (!(ios2 = AfxAcquireStream(0, afxIoFlag_RWX, NIL, size))) AfxThrowError();
    else
    {
        *ios = ios2;
        AfxAssertObjects(1, &ios2, afxFcc_IOS);

        if (AfxCopyStreamRange(AfxGetFileStream(&arc->file), ios2, e->offset, size, 0))
            AfxThrowError();

        if (err)
            AfxReleaseObjects(1, (void*[]){ ios2 });
    }
    return err;
}

_AFX afxError AfxDescribeArchivedFile(afxArchive arc, afxNat idx, afxArchiveItemDescriptor *desc)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &arc, afxFcc_ARC);

    AfxAssert(idx < AfxCountArrayElements(&arc->entries));
    AfxAssert(desc);
    _afxZipEntry const *e = AfxGetArrayUnit(&arc->entries, idx);
    AfxAssert(e);
    desc->offset = e->offset;
    desc->size = e->uncompressedSize;
    desc->compressedSize = e->compressedSize;
    desc->codec = e->codec;
    desc->crc32 = e->crc32;
    return err;
}

_AFX afxError AfxOpenArchivedFile(afxArchive arc, afxNat idx, afxStream *in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &arc, afxFcc_ARC);

    AfxAssert(idx < AfxCountArrayElements(&arc->entries));

    _afxZipEntry const *e = AfxGetArrayUnit(&arc->entries, idx);
    AfxAssert(e);
    afxNat size = e->codec == 0 ? e->uncompressedSize : e->compressedSize;
    afxStream in2;

    if (!(in2 = AfxAcquireStream(0, afxIoFlag_RWX, NIL, size))) AfxThrowError();
    else
    {
        *in = in2;
        AfxAssertType(in2, afxFcc_IOS);

        if (AfxCopyStreamRange(AfxGetFileStream(&arc->file), in2, e->offset, size, 0))
            AfxThrowError();

        if (err)
            AfxReleaseObjects(1, (void*[]) { in2 });
    }
    return err;
}

_AFX afxError AfxExtractArchivedFile(afxArchive arc, afxNat idx, afxUri const *uri) // extract to file directory.
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &arc, afxFcc_ARC);

    AfxAssert(idx < AfxCountArrayElements(&arc->entries));
    AfxAssertType(uri, afxFcc_URI);
    AfxAssert(!AfxUriIsBlank(uri));

    afxMmu mmu = AfxGetIoContext();
    AfxAssertObjects(1, &mmu, afxFcc_MMU);

    _afxZipEntry const *e = AfxGetArrayUnit(&arc->entries, idx);
    AfxAssert(e);
    afxNat size = e->codec == 0 ? e->uncompressedSize : e->compressedSize;
    
    afxStream ios = AfxGetFileStream(&arc->file);
    //AfxGoToStreamBegin(ios, e->offset);
    afxFile file;

    if (AfxOpenFiles(AFX_FILE_FLAG_W, 1, uri, &file)) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &file, afxFcc_FILE);

        if (AfxCopyStreamRange(AfxGetFileStream(file), ios, e->offset, size, 0))
            AfxThrowError();

        AfxReleaseObjects(1, (void*[]) { file });
    }
    return err;
}

_AFX afxNat AfxGetArchivedFileCodec(afxArchive arc, afxNat idx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &arc, afxFcc_ARC);

    AfxAssert(idx < AfxCountArrayElements(&arc->entries));
    _afxZipEntry const *e = AfxGetArrayUnit(&arc->entries, idx);
    afxNat method = e->codec;
    return method < AFX_N8_MAX ? method : method >> AFX_BYTE_SIZE;
}

_AFX afxNat AfxGetArchivedFileUncompressedSize(afxArchive arc, afxNat idx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &arc, afxFcc_ARC);

    AfxAssert(idx < AfxCountArrayElements(&arc->entries));
    _afxZipEntry const *e = AfxGetArrayUnit(&arc->entries, idx);
    return e->uncompressedSize;
}

_AFX afxNat32 AfxGetArchivedFileCrc(afxArchive arc, afxNat idx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &arc, afxFcc_ARC);

    AfxAssert(idx < AfxCountArrayElements(&arc->entries));
    _afxZipEntry const *e = AfxGetArrayUnit(&arc->entries, idx);
    return e->crc32;
}

_AFX afxString* AfxGetArchivedFileName(afxArchive arc, afxNat idx, afxUri *name)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &arc, afxFcc_ARC);

    AfxAssert(idx < AfxCountArrayElements(&arc->entries));
    AfxAssertType(name, afxFcc_URI);
    _afxZipEntry const *e = AfxGetArrayUnit(&arc->entries, idx);
    AfxCopyUri(name, &e->path.uri);
    return (afxString*)AfxGetUriString(name);
}

_AFX afxError _AfxArcCtor(afxArchive arc, afxCookie const *cookie)
{
    AfxEntry("arc=%p", arc);
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &arc, afxFcc_ARC);

    _afxZipSerializedCdHdr cdHdr = { 0 };

    if (_AfxZipFindAndReadCdHdr(arc, &cdHdr)) AfxThrowError();
    else
    {
        AfxAllocateArray(&arc->entries, cdHdr.numEntries, sizeof(_afxZipEntry), NIL);
        
        AfxReserveArraySpace(&arc->entries, cdHdr.numEntries);
        arc->entries.cnt = cdHdr.numEntries;

        if (_AfxZipReadWholeCd(arc, &cdHdr, _AfxZipReadCdEntryCallback, NIL)) AfxThrowError();
        else
        {

        }
    }
    return err;
}

_AFX afxError _AfxArcDtor(afxArchive arc)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &arc, afxFcc_ARC);

    AfxDeallocateArray(&arc->entries);

    return err;
}

_AFX afxClassConfig _AfxArcClsConfig =
{
    .fcc = afxFcc_ARC,
    .name = "File Archive",
    .unitsPerPage = 1,
    .size = sizeof(AFX_OBJECT(afxArchive)),
    .mmu = NIL,
    .ctor = (void*)_AfxArcCtor,
    .dtor = (void*)_AfxArcDtor
};

////////////////////////////////////////////////////////////////////////////////

_AFX afxError AfxAcquireArchives(afxNat cnt, afxArchive arc[], afxUri const uri[], afxFileFlags const flags[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(uri);

    afxClass* cls = AfxGetArchiveClass();
    AfxAssertClass(cls, afxFcc_ARC);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)arc, (void const*[]) { flags, uri }))
        AfxThrowError();

    return err;
}

_AFX afxNat AfxCurateArchives(afxNat first, afxNat cnt, afxBool(*f)(afxArchive, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(f);
    afxClass* cls = AfxGetArchiveClass();
    AfxAssertClass(cls, afxFcc_ARC);
    return AfxCurateInstances(cls, first, cnt, (void*)f, udd);
}

_AFX afxNat AfxEnumerateArchives(afxNat first, afxNat cnt, afxArchive arc[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(arc);
    afxClass* cls = AfxGetArchiveClass();
    AfxAssertClass(cls, afxFcc_ARC);
    return AfxEnumerateInstances(cls, first, cnt, (afxObject*)arc);
}

_AFX afxNat AfxCountArchives(void)
{
    afxError err = AFX_ERR_NONE;
    afxClass* cls = AfxGetArchiveClass();
    AfxAssertClass(cls, afxFcc_ARC);
    return AfxCountInstances(cls);
}
