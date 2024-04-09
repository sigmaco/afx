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
    afxUri128   path;
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

_AFX afxError _AfxZipFindAndReadCdHdr(afxStream file, _afxZipSerializedCdHdr *endRecord)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &file, afxFcc_FILE);

    if (AfxSeekStreamFromEnd(file, 0)) AfxThrowError();
    else
    {
        afxNat fileSize;
        _afxZipSerializedCdHdr *er;

        if ((fileSize = AfxGetStreamPosn(file)) <= sizeof(*er)) AfxThrowError();
        else
        {
            unsigned char buffer[_AFX_Z_BUFSIZ]; // maximum zip descriptor size
            afxNat readBytes = (fileSize < sizeof(buffer)) ? fileSize : sizeof(buffer);

            if (AfxSeekStreamFromBegin(file, fileSize - readBytes)) AfxThrowError();
            else
            {
                if (AfxReadStream(file, readBytes, 0, buffer)) AfxThrowError();
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

                        AfxCopy2(1, sizeof(*er), er, endRecord);

                        _afxZipSerializedCdHdr *e = endRecord;
                        AfxLogComment("signature: 0x%X", e->signature); // 0x06054b50
                        AfxLogComment("diskNumber: %d", e->diskNumber); // unsupported
                        AfxLogComment("centralDirectoryDiskNumber: %d", e->centralDirectoryDiskNumber); // unsupported
                        AfxLogComment("numEntriesThisDisk: %d", e->numEntriesThisDisk); // unsupported
                        AfxLogComment("numEntries: %d", e->numEntries);
                        AfxLogComment("centralDirectorySize: %u %#x", e->centralDirectorySize, e->centralDirectorySize);
                        AfxLogComment("centralDirectoryOffset: %u %#x", e->centralDirectoryOffset, e->centralDirectoryOffset);
                        AfxLogComment("zipCommentLength: %d", e->zipCommentLength);

                        if (endRecord->diskNumber || endRecord->centralDirectoryDiskNumber || endRecord->numEntries != endRecord->numEntriesThisDisk)
                        {
                            AfxThrowError();
                            AfxLogError("Qwadro doesn't support splitted Zips.");
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

    afxStream ios = arc->file;

    if (AfxSeekStreamFromBegin(ios, endRecord->centralDirectoryOffset)) AfxThrowError();
    else
    {
        for (afxInt i = 0; i < endRecord->numEntries; i++)
        {
            long offset = AfxGetStreamPosn(ios); // store current position
            AfxLogComment("Archived item %u -> %lu %#lx", i, (unsigned long)offset, (unsigned long)offset);
            
            if (AfxReadStream(ios, sizeof(fileHdr), 0, &fileHdr)) AfxThrowError();
            else
            {
                _afxZipSerializedCdEntryHdr *g = &fileHdr;
                AfxLogComment("signature: %u %#x", g->signature, g->signature); // 0x02014B50
                AfxLogComment("versionMadeBy: %u %#x", g->versionMadeBy, g->versionMadeBy); // unsupported
                AfxLogComment("versionNeededToExtract: %u %#x", g->versionNeededToExtract, g->versionNeededToExtract); // unsupported
                AfxLogComment("generalPurposeBitFlag: %u %#x", g->generalPurposeBitFlag, g->generalPurposeBitFlag); // unsupported
                AfxLogComment("compressionMethod: %u %#x", g->compressionMethod, g->compressionMethod); // 0-store,8-deflate
                AfxLogComment("lastModFileTime: %u %#x", g->lastModFileTime, g->lastModFileTime);
                AfxLogComment("lastModFileDate: %u %#x", g->lastModFileDate, g->lastModFileDate);
                AfxLogComment("crc32: %#x", g->crc32);
                AfxLogComment("compressedSize: %u", g->compressedSize);
                AfxLogComment("uncompressedSize: %u", g->uncompressedSize);
                AfxLogComment("fileNameLength: %u", g->fileNameLength);
                AfxLogComment("textraFieldLength: %u", g->extraFieldLength); // unsupported
                AfxLogComment("fileCommentLength: %u", g->fileCommentLength); // unsupported
                AfxLogComment("diskNumberStart: %u", g->diskNumberStart); // unsupported
                AfxLogComment("internalFileAttributes: %#x", g->internalFileAttributes); // unsupported
                AfxLogComment("externalFileAttributes: %#x", g->externalFileAttributes); // unsupported
                AfxLogComment("relativeOffsetOflocalHeader: %u %#x", g->relativeOffsetOflocalHeader, g->relativeOffsetOflocalHeader);

                if (fileHdr.signature != 0x02014B50) AfxThrowError();
                else
                {
                    AfxAssert(fileHdr.fileNameLength);

                    if (fileHdr.fileNameLength + 1 >= _AFX_Z_BUFSIZ) AfxThrowError();
                    else
                    {
                        // filename
                        char filename[_AFX_Z_BUFSIZ / 3];

                        if (AfxReadStream(ios, fileHdr.fileNameLength, 0, filename)) AfxThrowError();
                        else
                        {
                            filename[fileHdr.fileNameLength] = '\0'; // NULL terminate
                            
                            afxUri path;
                            AfxMakeUri(&path, filename, fileHdr.fileNameLength);

                            // extra block
                            unsigned char extra[_AFX_Z_BUFSIZ / 3] = { '\0' };

                            if (fileHdr.extraFieldLength)
                            {
                                if (AfxReadStream(ios, fileHdr.extraFieldLength, 0, extra))
                                    AfxThrowError();

                                extra[fileHdr.extraFieldLength] = '\0';
                            }
                            
                            if (!err)
                            {
                                // comment block
                                char comment[_AFX_Z_BUFSIZ / 3] = { '\0' };

                                if (fileHdr.fileCommentLength)
                                {
                                    if (AfxReadStream(ios, fileHdr.fileCommentLength, 0, comment))
                                        AfxThrowError();

                                    comment[fileHdr.fileCommentLength] = '\0'; // NULL terminate
                                }

                                if (!err)
                                {
                                    // seek to local file header, then skip file header + filename + extra field length
                                    afxNat16 localFileNameLength, localExtraFieldLength;

                                    if (AfxSeekStreamFromBegin(ios, fileHdr.relativeOffsetOflocalHeader + sizeof(_afxZipSerializedLocalEntryHdr) - sizeof(localFileNameLength) - sizeof(localExtraFieldLength))) AfxThrowError();
                                    else
                                    {
                                        if (AfxReadStream(ios, sizeof(localFileNameLength), 0, &localFileNameLength)) AfxThrowError();
                                        else
                                        {
                                            if (AfxReadStream(ios, sizeof(localExtraFieldLength), 0, &localExtraFieldLength)) AfxThrowError();
                                            else
                                            {
                                                if (AfxSeekStreamFromBegin(ios, fileHdr.relativeOffsetOflocalHeader + sizeof(_afxZipSerializedLocalEntryHdr) + localFileNameLength + localExtraFieldLength)) AfxThrowError();
                                                else
                                                {
                                                    AfxLogComment("Archived item %u, %lu %#lx", i, (unsigned long)AfxGetStreamPosn(ios), (unsigned long)AfxGetStreamPosn(ios));

                                                    if (callback(arc, i, &fileHdr, &path, extra, comment, user_data))
                                                        break; // keep going while callback returns ok

                                                    AfxSeekStreamFromBegin(ios, offset); // return to position
                                                    AfxAdvanceStream(ios, sizeof(_afxZipSerializedCdEntryHdr) + g->fileNameLength); // skip entry
                                                    AfxAdvanceStream(ios, g->extraFieldLength + g->fileCommentLength); // skip entry
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

_AFX afxError _AfxZipReadEntryData(afxStream file, _afxZipSerializedCdEntryHdr *header, void *out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &file, afxFcc_FILE);

    if (header->compressionMethod == 0) // Store
    {
        if (AfxReadStream(file, header->uncompressedSize, 0, &out))
            AfxThrowError();
    }
    else
    {
        AfxThrowError();
        AfxLogError("Qwadro doesn't support compression.");
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
    AfxMakeUri128(&e->path, NIL);
    AfxCopyUri(&e->path.uri, path);

    afxStream ios = arc->file;

    e->offset = AfxGetStreamPosn(ios);
    e->uncompressedSize = header->uncompressedSize;
    e->compressedSize = header->compressedSize;
    e->codec = header->compressionMethod;
    e->crc32 = header->crc32;
    AfxLogEcho("%04d/%02d/%02d %02d:%02d:%02d #%04u %.*s", ZYEAR(header->lastModFileDate), ZMONTH(header->lastModFileDate), ZDAY(header->lastModFileDate), ZHOUR(header->lastModFileTime), ZMINUTE(header->lastModFileTime), ZSECOND(header->lastModFileTime), idx, AfxPushString(&path->str.str));
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

    AfxAssert(name);
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

    afxStream ios = arc->file;
    
    if (AfxSeekStreamFromBegin(ios, e->offset)) AfxThrowError();
    else
    {
        if (AfxReadStream(ios, size > bufSiz ? bufSiz : size, 0, buf))
            AfxThrowError();
    }
    return err;
}

_AFX afxError AfxForkArchivedFile(afxArchive arc, afxNat idx, afxStream *ios)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &arc, afxFcc_ARC);

    AfxAssert(idx < AfxCountArrayElements(&arc->entries));
    AfxAssertObjects(1, &ios, afxFcc_IOB);

    _afxZipEntry const *e = AfxGetArrayUnit(&arc->entries, idx);
    AfxAssert(e);
    afxNat size = e->codec == 0 ? e->uncompressedSize : e->compressedSize;
    afxStream ios2;

    if (!(ios2 = AfxAcquireStream(afxIoFlag_RWX, size))) AfxThrowError();
    else
    {
        *ios = ios2;
        AfxAssertObjects(1, &ios2, afxFcc_IOB);

        if (AfxCopyStreamRange(arc->file, e->offset, size, 0, ios2))
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

    if (!(in2 = AfxAcquireStream(afxIoFlag_RWX, size))) AfxThrowError();
    else
    {
        *in = in2;
        AfxAssertType(in2, afxFcc_IOB);

        if (AfxCopyStreamRange(arc->file, e->offset, size, 0, in2))
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
    AfxAssert(uri);
    AfxAssert(!AfxUriIsBlank(uri));

    _afxZipEntry const *e = AfxGetArrayUnit(&arc->entries, idx);
    AfxAssert(e);
    afxNat size = e->codec == 0 ? e->uncompressedSize : e->compressedSize;
    
    afxStream ios = arc->file;
    //AfxSeekStreamFromBegin(ios, e->offset);
    afxStream file;

    if (!(file = AfxOpenFile(uri, afxIoFlag_W))) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &file, afxFcc_FILE);

        if (AfxCopyStreamRange(ios, e->offset, size, 0, file))
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
    AfxAssert(name);
    _afxZipEntry const *e = AfxGetArrayUnit(&arc->entries, idx);
    AfxCopyUri(name, &e->path.uri);
    return (afxString*)AfxGetUriString(name);
}

_AFX afxError _AfxArcCtor(afxArchive arc, afxCookie const *cookie)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &arc, afxFcc_ARC);

    arc->file = cookie->udd[0];

    AfxAllocateArray(&arc->entries, 1, sizeof(_afxZipEntry), NIL);

    return err;
}

_AFX afxError _AfxArcDtor(afxArchive arc)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &arc, afxFcc_ARC);

    AfxDeallocateArray(&arc->entries);

    return err;
}

_AFX afxClassConfig const _AfxArcMgrCfg =
{
    .fcc = afxFcc_ARC,
    .name = "Archive",
    .desc = "Archiving Utility",
    .unitsPerPage = 2,
    .size = sizeof(AFX_OBJECT(afxArchive)),
    .mmu = NIL,
    .ctor = (void*)_AfxArcCtor,
    .dtor = (void*)_AfxArcDtor
};

////////////////////////////////////////////////////////////////////////////////

_AFX afxArchive AfxOpenArchive(afxUri const* uri, afxFileFlags const flags, afxError* error)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(uri);
    afxArchive arc = NIL;

    afxManager* cls = AfxGetArchiveClass();
    AfxAssertClass(cls, afxFcc_ARC);
    
    afxStream file = AfxOpenFile(uri, (flags & afxIoFlag_RWX));

    if (!file) AfxThrowError();
    else
{
        _afxZipSerializedCdHdr cdHdr = { 0 };

        if (_AfxZipFindAndReadCdHdr(file, &cdHdr)) AfxThrowError();
        else
        {
            if (AfxAcquireObjects(cls, 1, (afxObject*)&arc, (void const*[]) { file, &flags })) AfxThrowError();
            else
            {
                AfxReserveArraySpace(&arc->entries, cdHdr.numEntries);
                arc->entries.cnt = cdHdr.numEntries;

                if (_AfxZipReadWholeCd(arc, &cdHdr, _AfxZipReadCdEntryCallback, NIL)) AfxThrowError();
                else
                {

                }
            }
        }
    }

    if (error)
        *error = err;

    return arc;
}

_AFX afxNat AfxInvokeArchives(afxNat first, afxNat cnt, afxBool(*f)(afxArchive, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(f);
    afxManager* cls = AfxGetArchiveClass();
    AfxAssertClass(cls, afxFcc_ARC);
    return AfxInvokeObjects(cls, first, cnt, (void*)f, udd);
}

_AFX afxNat AfxEnumerateArchives(afxNat first, afxNat cnt, afxArchive archives[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(archives);
    afxManager* cls = AfxGetArchiveClass();
    AfxAssertClass(cls, afxFcc_ARC);
    return AfxEnumerateObjects(cls, first, cnt, (afxObject*)archives);
}

_AFX afxNat AfxCountArchives(void)
{
    afxError err = AFX_ERR_NONE;
    afxManager* cls = AfxGetArchiveClass();
    AfxAssertClass(cls, afxFcc_ARC);
    return AfxCountObjects(cls);
}
