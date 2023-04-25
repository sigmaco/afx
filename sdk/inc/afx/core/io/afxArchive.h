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

// afxArchive allows you to save a complex network of objects in a permanent
// binary form (usually disk storage) that persists after those objects are deleted.

#ifndef AFX_ARCHIVE_H
#define AFX_ARCHIVE_H

#include "afx/core/io/afxFile.h"

AFX_DEFINE_HANDLE(afxArchive);

AFX_OBJECT(afxArchive)
{
    AFX_OBJECT(afxFile) file;
};

AFX void*       AfxArchiveGetSystem(afxArchive arch);
AFX void*       AfxArchiveGetFileSystem(afxArchive arch);


#endif//AFX_ARCHIVE_H