/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                      S I G M A   T E C H N O L O G Y   G R O U P
 *
 *                                   Public Test Build
 *                      (c) 2017 SIGMA Co. & SIGMA Technology Group
 *                                    www.sigmaco.org
 */

// This content is part of SIGMA Future Storage <https://sigmaco.org/future-storage>

// afxArchive allows you to save a complex network of objects in a permanent
// binary form (usually disk storage) that persists after those objects are deleted.

#ifndef AFX_ARCHIVE_H
#define AFX_ARCHIVE_H

#include "afx/core/io/afxFile.h"

AFX_DEFINE_HANDLE(afxArchive);

AFX_OBJECT(afxArchive)
{
    AFX_OBJECT(afxFile) file;
#ifdef _AFX_ARCHIVE_C
    afxArray            entries;
#endif
};


/* URI for open zipped streams.
 * art.zip#idx
 * art.zip#name
 * art.zip?offset=&size= // unsafe, not more allowed.
 */

AFX_DEFINE_STRUCT(afxArchiveItemDescriptor)
{
    afxSize     offset;
    afxNat32    size;
    afxNat32    compressedSize;
    afxNat32    codec;
    afxNat32    crc32;
};

AFX void*       AfxArchiveGetSystem(afxArchive arc);
AFX void*       AfxArchiveGetFileSystem(afxArchive arc);

AFX afxNat      AfxArchiveGetItemCount(afxArchive arc);

AFX afxBool     AfxArchiveItemIsFile(afxArchive arc, afxNat idx);
AFX afxError    AfxArchiveExtractItem(afxArchive arc, afxNat idx, afxNat bufSiz, void *buf);
AFX afxError    AfxArchiveExtractItemToStream(afxArchive arc, afxNat idx, afxStream ios);
AFX afxError    AfxArchiveDownloadItem(afxArchive arc, afxNat idx, afxUri const *uri); // extract to file directory.
AFX afxNat      AfxArchiveFindItem(afxArchive arc, afxUri const *name);
AFX afxStream   AfxArchiveOpenItem(afxArchive arc, afxNat idx);

AFX afxError    AfxArchiveDescribeItem(afxArchive arc, afxNat idx, afxArchiveItemDescriptor *desc);
AFX afxSize     AfxArchiveGetItemOffset(afxArchive arc, afxNat idx);
AFX afxNat      AfxArchiveGetItemCodec(afxArchive arc, afxNat idx);
AFX afxNat      AfxArchiveGetItemSize(afxArchive arc, afxNat idx);
AFX afxString*  AfxArchiveGetItemName(afxArchive arc, afxNat idx, afxUri *name);
AFX afxNat32    AfxArchiveGetItemHash(afxArchive arc, afxNat idx);

#endif//AFX_ARCHIVE_H