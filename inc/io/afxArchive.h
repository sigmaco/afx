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

// This code is part of SIGMA Future Storage.

// afxArchive allows you to save a complex network of objects in a permanent
// binary form (usually disk storage) that persists after those objects are deleted.

#ifndef AFX_ARCHIVE_H
#define AFX_ARCHIVE_H

#include "qwadro/inc/io/afxFile.h"
#include "qwadro/inc/mem/afxArray.h"

/* URI for open zipped streams.
 * art.zip#idx
 * art.zip#name
 * art.zip?offset=&size= // unsafe, not more allowed.
 */

// Archives stores metafiles. Metafiles are files used to create files.

AFX_DEFINE_STRUCT(afxArchiveItemDescriptor)
{
    afxSize     offset;
    afxNat32    size;
    afxNat32    compressedSize;
    afxNat32    codec;
    afxNat32    crc32;
};

AFX afxNat      AfxCountArchivedFiles(afxArchive arc);

AFX afxBool     AfxArchivedFileIsDirectory(afxArchive arc, afxNat idx);
AFX afxError    AfxDumpArchivedFile(afxArchive arc, afxNat idx, afxNat bufSiz, void *buf);
AFX afxError    AfxForkArchivedFile(afxArchive arc, afxNat idx, afxStream *ios);
AFX afxError    AfxExtractArchivedFile(afxArchive arc, afxNat idx, afxUri const *uri); // extract to file directory.
AFX afxBool     AfxFindArchivedFile(afxArchive arc, afxUri const *name, afxNat *idx);
AFX afxError    AfxOpenArchivedFile(afxArchive arc, afxNat idx, afxStream *in);

AFX afxError    AfxDescribeArchivedFile(afxArchive arc, afxNat idx, afxArchiveItemDescriptor *desc);
AFX afxSize     AfxGetArchivedFileOffset(afxArchive arc, afxNat idx);
AFX afxNat      AfxGetArchivedFileCodec(afxArchive arc, afxNat idx);
AFX afxNat      AfxGetArchivedFileUncompressedSize(afxArchive arc, afxNat idx);
AFX afxString*  AfxGetArchivedFileName(afxArchive arc, afxNat idx, afxUri *name);
AFX afxNat32    AfxGetArchivedFileCrc(afxArchive arc, afxNat idx);

////////////////////////////////////////////////////////////////////////////////

AFX afxArchive  AfxOpenArchive(afxUri const* uri, afxFileFlags const flags, afxError* err);

#endif//AFX_ARCHIVE_H
