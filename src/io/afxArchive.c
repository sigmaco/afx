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
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define _AFX_CORE_C
#define _AFX_FILE_C
#define _AFX_ARCHIVE_C
#include "../impl/afxIoImplKit.h"


typedef struct zip zip;

#pragma pack(push, 1)

typedef struct
{
    afxUnit32 signature; // 0x04034B50
    afxUnit16 verNeededToExtract; // unsupported
    afxUnit16 genrlPurposeBitFlag; // unsupported
    afxUnit16 codec;
    afxUnit16 lastModFileTime;
    afxUnit16 lastModFileDate;
    afxUnit32 crc32;
    afxUnit32 encSiz;
    afxUnit32 decSiz;
    afxUnit16 fileNameLen;
    afxUnit16 extraFieldLen; // unsupported
} _afxZipSerializedLocalEntryHdr; // local file record

typedef struct
{
    afxUnit32 signature; // 0x02014B50
    afxUnit16 verMadeBy; // unsupported
    afxUnit16 verNeededToExtract; // unsupported
    afxUnit16 genrlPurposeBitFlag; // unsupported
    afxUnit16 codec; // 0-store,8-deflate
    afxUnit16 lastModFileTime;
    afxUnit16 lastModFileDate;
    afxUnit32 crc32;
    afxUnit32 encSiz;
    afxUnit32 decSiz;
    afxUnit16 fileNameLen;
    afxUnit16 extraFieldLen; // unsupported
    afxUnit16 fileCommentLen; // unsupported
    afxUnit16 diskNumStart; // unsupported
    afxUnit16 internalFileAttrs; // unsupported
    afxUnit32 externalFileAttrs; // unsupported
    afxUnit32 relativeOffsetOfLocalHdr;
} _afxZipSerializedCdEntryHdr; // global file record

typedef struct
{
    afxUnit32 signature; // 0x06054b50
    afxUnit16 diskNum; // unsupported
    afxUnit16 centralDirDiskNum; // unsupported
    afxUnit16 numEntriesThisDisk; // unsupported
    afxUnit16 numEntries;
    afxUnit32 centralDirSiz;
    afxUnit32 centralDirOffset;
    afxUnit16 zipCommentLen;
    // Followed by .ZIP file comment (variable size)
} _afxZipSerializedCdHdr;

#pragma pack(pop)

typedef struct _afxZipEntry
{
    //_afxZipSerializedCdEntryHdr hdr;
    afxUri128   path;
    afxSize     offset;
    afxUnit32    compressedSize;
    afxUnit32    uncompressedSize;
    afxUnit32    crc32;
    afxUnit32    codec;
} _afxZipEntry;

struct zip
{
    FILE *in, *out;
    _afxZipEntry *entries;
    unsigned count;
};

AFX_STATIC_ASSERT(sizeof(_afxZipSerializedLocalEntryHdr) == 30, "");
AFX_STATIC_ASSERT(sizeof(_afxZipSerializedCdEntryHdr) == 46, "");
AFX_STATIC_ASSERT(sizeof(_afxZipSerializedCdHdr) == 22, "");

#define _AFX_Z_BUFSIZ 65536

typedef afxError (*_afxZipRecordCallback)(afxArchive arc, afxUnit idx, _afxZipSerializedCdEntryHdr *hdr, afxUri const *path, void *extra, char *comment, void *user_data);
#if 0
_AFX afxBool _AfxGetArcD(afxArchive arc, struct _afxArcD **arc, struct _afxSysD* sysD)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_ARC, 1, &arc);
    AFX_ASSERT(arc);
    AfxAssertType(sysD, afxFcc_SYS);
    return AfxExposeResidentObjects(1, &arc, (void**)arc, &theSys->archives);
}
#endif

_AFX afxError _AfxZipFindAndReadCdHdr(afxStream file, _afxZipSerializedCdHdr *endRecord)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &file);

    if (AfxSeekStream(file, 0, afxSeekOrigin_END)) AfxThrowError();
    else
    {
        afxUnit fileSize;
        _afxZipSerializedCdHdr *er = NIL;

        if ((fileSize = AfxAskStreamPosn(file)) <= sizeof(*er)) AfxThrowError();
        else
        {
            unsigned char buffer[_AFX_Z_BUFSIZ]; // maximum zip descriptor size
            afxUnit readBytes = (fileSize < sizeof(buffer)) ? fileSize : sizeof(buffer);

            if (AfxSeekStream(file, fileSize - readBytes, afxSeekOrigin_BEGIN)) AfxThrowError();
            else
            {
                if (AfxReadStream(file, readBytes, 0, buffer)) AfxThrowError();
                else
                {
                    // Naively assume signature can only be found in one place...
                    afxUnit i;

                    for (i = readBytes - sizeof(*er); i >= 0; i--)
                    {
                        er = (_afxZipSerializedCdHdr *)(buffer + i);

                        if (er->signature == 0x06054B50)
                            break;
                    }

                    if (i < 0) AfxThrowError();
                    else
                    {

                        AfxCopy(endRecord, er, sizeof(*er));

                        _afxZipSerializedCdHdr *e = endRecord;
                        AfxReportComment("signature: 0x%X", e->signature); // 0x06054b50
                        AfxReportComment("diskNum: %d", e->diskNum); // unsupported
                        AfxReportComment("centralDirDiskNum: %d", e->centralDirDiskNum); // unsupported
                        AfxReportComment("numEntriesThisDisk: %d", e->numEntriesThisDisk); // unsupported
                        AfxReportComment("numEntries: %d", e->numEntries);
                        AfxReportComment("centralDirSiz: %u %#x", e->centralDirSiz, e->centralDirSiz);
                        AfxReportComment("centralDirOffset: %u %#x", e->centralDirOffset, e->centralDirOffset);
                        AfxReportComment("zipCommentLength: %d", e->zipCommentLen);

                        if (endRecord->diskNum || endRecord->centralDirDiskNum || endRecord->numEntries != endRecord->numEntriesThisDisk)
                        {
                            AfxThrowError();
                            AfxReportError("Qwadro doesn't support splitted Zips.");
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
    AFX_ASSERT_OBJECTS(afxFcc_ARC, 1, &arc);

    afxStream ios = arc->file;

    if (AfxSeekStream(ios, endRecord->centralDirOffset, afxSeekOrigin_BEGIN)) AfxThrowError();
    else
    {
        for (afxInt i = 0; i < endRecord->numEntries; i++)
        {
            long offset = AfxAskStreamPosn(ios); // store current position
            AfxReportComment("Archived item %u -> %lu %#lx", i, (unsigned long)offset, (unsigned long)offset);
            
            if (AfxReadStream(ios, sizeof(fileHdr), 0, &fileHdr)) AfxThrowError();
            else
            {
                _afxZipSerializedCdEntryHdr *g = &fileHdr;
                AfxReportComment("signature: %u %#x", g->signature, g->signature); // 0x02014B50
                AfxReportComment("verMadeBy: %u %#x", g->verMadeBy, g->verMadeBy); // unsupported
                AfxReportComment("verNeededToExtract: %u %#x", g->verNeededToExtract, g->verNeededToExtract); // unsupported
                AfxReportComment("genrlPurposeBitFlag: %u %#x", g->genrlPurposeBitFlag, g->genrlPurposeBitFlag); // unsupported
                AfxReportComment("codec: %u %#x", g->codec, g->codec); // 0-store,8-deflate
                AfxReportComment("lastModFileTime: %u %#x", g->lastModFileTime, g->lastModFileTime);
                AfxReportComment("lastModFileDate: %u %#x", g->lastModFileDate, g->lastModFileDate);
                AfxReportComment("crc32: %#x", g->crc32);
                AfxReportComment("encSiz: %u", g->encSiz);
                AfxReportComment("decSiz: %u", g->decSiz);
                AfxReportComment("fileNameLen: %u", g->fileNameLen);
                AfxReportComment("textraFieldLen: %u", g->extraFieldLen); // unsupported
                AfxReportComment("fileCommentLen: %u", g->fileCommentLen); // unsupported
                AfxReportComment("diskNumStart: %u", g->diskNumStart); // unsupported
                AfxReportComment("internalFileAttributes: %#x", g->internalFileAttrs); // unsupported
                AfxReportComment("externalFileAttributes: %#x", g->externalFileAttrs); // unsupported
                AfxReportComment("relativeOffsetOfLocalHdr: %u %#x", g->relativeOffsetOfLocalHdr, g->relativeOffsetOfLocalHdr);

                if (fileHdr.signature != 0x02014B50) AfxThrowError();
                else
                {
                    AFX_ASSERT(fileHdr.fileNameLen);

                    if (fileHdr.fileNameLen + 1 >= _AFX_Z_BUFSIZ) AfxThrowError();
                    else
                    {
                        // filename
                        char filename[_AFX_Z_BUFSIZ / 3];

                        if (AfxReadStream(ios, fileHdr.fileNameLen, 0, filename)) AfxThrowError();
                        else
                        {
                            filename[fileHdr.fileNameLen] = '\0'; // NULL terminate
                            
                            afxUri path;
                            AfxMakeUri(&path, 0, filename, fileHdr.fileNameLen);

                            // extra block
                            unsigned char extra[_AFX_Z_BUFSIZ / 3] = { '\0' };

                            if (fileHdr.extraFieldLen)
                            {
                                if (AfxReadStream(ios, fileHdr.extraFieldLen, 0, extra))
                                    AfxThrowError();

                                extra[fileHdr.extraFieldLen] = '\0';
                            }
                            
                            if (!err)
                            {
                                // comment block
                                char comment[_AFX_Z_BUFSIZ / 3] = { '\0' };

                                if (fileHdr.fileCommentLen)
                                {
                                    if (AfxReadStream(ios, fileHdr.fileCommentLen, 0, comment))
                                        AfxThrowError();

                                    comment[fileHdr.fileCommentLen] = '\0'; // NULL terminate
                                }

                                if (!err)
                                {
                                    // seek to local file header, then skip file header + filename + extra field length
                                    afxUnit16 localFileNameLength, localExtraFieldLength;

                                    if (AfxSeekStream(ios, fileHdr.relativeOffsetOfLocalHdr + sizeof(_afxZipSerializedLocalEntryHdr) - sizeof(localFileNameLength) - sizeof(localExtraFieldLength), afxSeekOrigin_BEGIN)) AfxThrowError();
                                    else
                                    {
                                        if (AfxReadStream(ios, sizeof(localFileNameLength), 0, &localFileNameLength)) AfxThrowError();
                                        else
                                        {
                                            if (AfxReadStream(ios, sizeof(localExtraFieldLength), 0, &localExtraFieldLength)) AfxThrowError();
                                            else
                                            {
                                                if (AfxSeekStream(ios, fileHdr.relativeOffsetOfLocalHdr + sizeof(_afxZipSerializedLocalEntryHdr) + localFileNameLength + localExtraFieldLength, afxSeekOrigin_BEGIN)) AfxThrowError();
                                                else
                                                {
                                                    AfxReportComment("Archived item %u, %lu %#lx", i, (unsigned long)AfxAskStreamPosn(ios), (unsigned long)AfxAskStreamPosn(ios));

                                                    if (callback(arc, i, &fileHdr, &path, extra, comment, user_data))
                                                        break; // keep going while callback returns ok

                                                    AfxSeekStream(ios, offset, afxSeekOrigin_BEGIN); // return to position
                                                    AfxAdvanceStream(ios, sizeof(_afxZipSerializedCdEntryHdr) + g->fileNameLen); // skip entry
                                                    AfxAdvanceStream(ios, g->extraFieldLen + g->fileCommentLen); // skip entry
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
    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &file);

    if (header->codec == 0) // Store
    {
        if (AfxReadStream(file, header->decSiz, 0, &out))
            AfxThrowError();
    }
    else
    {
        AfxThrowError();
        AfxReportError("Qwadro doesn't support compression.");
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

_AFX afxError _AfxZipReadCdEntryCallback(afxArchive arc, afxUnit idx, _afxZipSerializedCdEntryHdr *header, afxUri const *path, void *extra, char *comment, void *user_data)
{
    afxError err = AFX_ERR_NONE;
    (void)extra;
    (void)comment;
    (void)user_data;

    AFX_ASSERT_OBJECTS(afxFcc_ARC, 1, &arc);

    _afxZipEntry *e = AfxGetArrayUnit(&arc->entries, idx);
    AfxMakeUri128(&e->path, NIL);
    AfxCopyUri(&e->path.uri, path);

    afxStream ios = arc->file;

    e->offset = AfxAskStreamPosn(ios);
    e->uncompressedSize = header->decSiz;
    e->compressedSize = header->encSiz;
    e->codec = header->codec;
    e->crc32 = header->crc32;
    AfxReportMessage("%04d/%02d/%02d %02d:%02d:%02d #%04u %.*s", ZYEAR(header->lastModFileDate), ZMONTH(header->lastModFileDate), ZDAY(header->lastModFileDate), ZHOUR(header->lastModFileTime), ZMINUTE(header->lastModFileTime), ZSECOND(header->lastModFileTime), idx, AfxPushString(&path->s));
    return err;
}

_AFX afxUnit AfxCountArchivedFiles(afxArchive arc)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_ARC, 1, &arc);

    //AFX_ASSERT(&arc->entries.fcc afxFcc_ARR);
    return arc->entries.pop;
}

_AFX afxBool AfxFindArchivedFile(afxArchive arc, afxUri const *name, afxUnit *idx)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_ARC, 1, &arc);

    AFX_ASSERT(name);
    AFX_ASSERT(!AfxIsUriBlank(name));

    //_afxZipEntry const *e;
    //AfxForEachArrayUnit(&arc->entries, i, e)

    for (afxUnit i = 0; i < arc->entries.pop; ++i)
    {
        _afxZipEntry const *e = AfxGetArrayUnit(&arc->entries, i);
        // in case of several copies, grab most recent file (last coincidence)
        if (0 == AfxCompareUri(name, &e->path.uri))
        {
            AFX_ASSERT(idx);
            *idx = i;
            return TRUE;
        }
    }
    return FALSE;
}

_AFX afxSize AfxGetArchivedFileOffset(afxArchive arc, afxUnit idx)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_ARC, 1, &arc);

    AFX_ASSERT(idx < arc->entries.pop);
    _afxZipEntry const *e = AfxGetArrayUnit(&arc->entries, idx);
    return e->offset;
}

_AFX afxBool AfxArchivedFileIsDirectory(afxArchive arc, afxUnit idx)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_ARC, 1, &arc);

    AFX_ASSERT(idx < arc->entries.pop);
    _afxZipEntry const *e = AfxGetArrayUnit(&arc->entries, idx);
    afxString const *nameStr = AfxGetUriString(&e->path.uri);
    afxUnit nameLen = AfxGetStringLength(nameStr);
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

_AFX afxError AfxDumpArchivedFile(afxArchive arc, afxUnit idx, afxUnit bufSiz, void *buf)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_ARC, 1, &arc);

    AFX_ASSERT(idx < arc->entries.pop);
    AFX_ASSERT(bufSiz);
    AFX_ASSERT(buf);

    _afxZipEntry const *e = AfxGetArrayUnit(&arc->entries, idx);
    AFX_ASSERT(e);
    afxUnit size = e->codec == 0 ? e->uncompressedSize : e->compressedSize;
    AFX_ASSERT(bufSiz >= size);

    afxStream ios = arc->file;
    
    if (AfxSeekStream(ios, e->offset, afxSeekOrigin_BEGIN)) AfxThrowError();
    else
    {
        if (AfxReadStream(ios, size > bufSiz ? bufSiz : size, 0, buf))
            AfxThrowError();
    }
    return err;
}

_AFX afxError AfxForkArchivedFile(afxArchive arc, afxUnit idx, afxStream *ios)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_ARC, 1, &arc);

    AFX_ASSERT(idx < arc->entries.pop);
    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &ios);

    _afxZipEntry const *e = AfxGetArrayUnit(&arc->entries, idx);
    AFX_ASSERT(e);
    afxUnit size = e->codec == 0 ? e->uncompressedSize : e->compressedSize;
    afxStream ios2;
    afxStreamInfo iobi = { 0 };
    iobi.bufCap = size;
    iobi.usage = afxIoUsage_FILE;
    iobi.flags = afxIoFlag_READABLE;

    if (AfxAcquireStream(1, &iobi, &ios2)) AfxThrowError();
    else
    {
        *ios = ios2;
        AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &ios2);

        //if (AfxCopyStreamRange(arc->file, e->offset, size, 0, ios2))
            AfxThrowError();

        if (err)
            AfxDisposeObjects(1, (void*[]){ ios2 });
    }
    return err;
}

_AFX afxError AfxDescribeArchivedFile(afxArchive arc, afxUnit idx, afxArchiveItemDescriptor *desc)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_ARC, 1, &arc);

    AFX_ASSERT(idx < arc->entries.pop);
    AFX_ASSERT(desc);
    _afxZipEntry const *e = AfxGetArrayUnit(&arc->entries, idx);
    AFX_ASSERT(e);
    desc->offset = e->offset;
    desc->size = e->uncompressedSize;
    desc->compressedSize = e->compressedSize;
    desc->codec = e->codec;
    desc->crc32 = e->crc32;
    return err;
}

_AFX afxError AfxOpenArchivedFile(afxArchive arc, afxUnit idx, afxStream *in)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_ARC, 1, &arc);

    AFX_ASSERT(idx < arc->entries.pop);

    _afxZipEntry const *e = AfxGetArrayUnit(&arc->entries, idx);
    AFX_ASSERT(e);
    afxUnit size = e->codec == 0 ? e->uncompressedSize : e->compressedSize;
    afxStream in2;
    afxStreamInfo iobi = { 0 };
    iobi.bufCap = size;
    iobi.usage = afxIoUsage_FILE;
    iobi.flags = afxIoFlag_READABLE;

    if (AfxAcquireStream(1, &iobi, &in2)) AfxThrowError();
    else
    {
        *in = in2;
        AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &in2);

        //if (AfxCopyStreamRange(arc->file, e->offset, size, 0, in2))
            AfxThrowError();

        if (err)
            AfxDisposeObjects(1, (void*[]) { in2 });
    }
    return err;
}

_AFX afxError AfxExtractArchivedFile(afxArchive arc, afxUnit idx, afxUri const *uri) // extract to file directory.
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_ARC, 1, &arc);

    AFX_ASSERT(idx < arc->entries.pop);
    AFX_ASSERT(uri);
    AFX_ASSERT(!AfxIsUriBlank(uri));

    _afxZipEntry const *e = AfxGetArrayUnit(&arc->entries, idx);
    AFX_ASSERT(e);
    afxUnit size = e->codec == 0 ? e->uncompressedSize : e->compressedSize;
    
    afxStream ios = arc->file;
    //AfxSeekStreamFromBegin(ios, e->offset);
    afxStream file;
    afxStreamInfo iobi = { 0 };
    iobi.usage = afxIoUsage_FILE;
    iobi.flags = afxIoFlag_READABLE;
    AfxAcquireStream(1, &iobi, &file);

    if (AfxReopenFile(file, uri, afxFileFlag_W)) AfxThrowError();
    else
    {
        AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &file);

        //if (AfxCopyStreamRange(ios, e->offset, size, 0, file))
            AfxThrowError();

        AfxDisposeObjects(1, (void*[]) { file });
    }
    return err;
}

_AFX afxUnit AfxGetArchivedFileCodec(afxArchive arc, afxUnit idx)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_ARC, 1, &arc);

    AFX_ASSERT(idx < arc->entries.pop);
    _afxZipEntry const *e = AfxGetArrayUnit(&arc->entries, idx);
    afxUnit method = e->codec;
    return method < AFX_U8_MAX ? method : method >> AFX_BYTE_SIZE;
}

_AFX afxUnit AfxGetArchivedFileUncompressedSize(afxArchive arc, afxUnit idx)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_ARC, 1, &arc);

    AFX_ASSERT(idx < arc->entries.pop);
    _afxZipEntry const *e = AfxGetArrayUnit(&arc->entries, idx);
    return e->uncompressedSize;
}

_AFX afxUnit32 AfxGetArchivedFileCrc(afxArchive arc, afxUnit idx)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_ARC, 1, &arc);

    AFX_ASSERT(idx < arc->entries.pop);
    _afxZipEntry const *e = AfxGetArrayUnit(&arc->entries, idx);
    return e->crc32;
}

_AFX afxString* AfxGetArchivedFileName(afxArchive arc, afxUnit idx, afxUri *name)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_ARC, 1, &arc);

    AFX_ASSERT(idx < arc->entries.pop);
    AFX_ASSERT(name);
    _afxZipEntry const *e = AfxGetArrayUnit(&arc->entries, idx);
    AfxCopyUri(name, &e->path.uri);
    return (afxString*)AfxGetUriString(name);
}

_AFX afxError _AfxArcCtor(afxArchive arc, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_ARC, 1, &arc);

    afxStorage fsys = args[0];
    arc->file = args[1];

    AfxMakeArray(&arc->entries, sizeof(_afxZipEntry), 1, NIL, 0);

    return err;
}

_AFX afxError _AfxArcDtor(afxArchive arc)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_ARC, 1, &arc);

    AfxEmptyArray(&arc->entries, FALSE, FALSE);

    return err;
}

_AFX afxClassConfig const _AfxArcClsCfg =
{
    .fcc = afxFcc_ARC,
    .name = "Archive",
    .desc = "Archiving Utility",
    .fixedSiz = sizeof(AFX_OBJECT(afxArchive)),
    .ctor = (void*)_AfxArcCtor,
    .dtor = (void*)_AfxArcDtor
};

////////////////////////////////////////////////////////////////////////////////

_AFX afxArchive AfxOpenArchive(afxUri const* uri, afxFileFlags const flags, afxError* error)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(uri);
    afxArchive arc = NIL;

    afxStream file;
    afxStreamInfo iobi = { 0 };
    iobi.usage = afxIoUsage_FILE;
    iobi.flags = afxIoFlag_READABLE;
    AfxAcquireStream(1, &iobi, &file);

    if (AfxReopenFile(file, uri, (flags & afxIoFlag_RWX))) AfxThrowError();
    else
{
        afxStorage fsys = AfxGetHost(file);
        afxUnit diskId = AfxGetObjectId(fsys);

        afxClass* cls = (afxClass*)AfxGetArchiveClass(fsys);
        AFX_ASSERT_CLASS(cls, afxFcc_ARC);
    
        _afxZipSerializedCdHdr cdHdr = { 0 };

        if (_AfxZipFindAndReadCdHdr(file, &cdHdr)) AfxThrowError();
        else
        {
            if (AfxAcquireObjects(cls, 1, (afxObject*)&arc, (void const*[]) { fsys, file, &flags })) AfxThrowError();
            else
            {
                AfxReserveArraySpace(&arc->entries, cdHdr.numEntries);
                arc->entries.pop = cdHdr.numEntries;

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
