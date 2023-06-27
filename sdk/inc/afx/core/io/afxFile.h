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

#ifndef AFX_FILE_H
#define AFX_FILE_H

#include "afx/core/io/afxStream.h"
#include "afx/core/io/afxUri.h"

AFX_DEFINE_HANDLE(afxFile);

AFX_OBJECT(afxFile)
{
    AFX_OBJECT(afxStream)   ios;
#ifdef _AFX_FILE_C
    void                    *fd;
    afxUri*                 uri;
    afxBool                 shouldBeFlushed;
#endif
};

AFX void*                   AfxFileGetSystem(afxFile file);
AFX void*                   AfxFileGetFileSystem(afxFile file);
AFX void*                   AfxFileGetHostDescriptor(afxFile file);

AFX afxError                AfxFileReopen(afxFile file, afxRwx const rwx, afxUri const *uri);
AFX afxError                AfxFileReload(afxFile file, afxRwx const rwx, afxUri const *uri);

AFX afxResult               AfxFileCopyUri(afxFile file, afxString *str);
AFX afxString const*        AfxFileGetUriString(afxFile file);
AFX afxUri const*           AfxFileGetUri(afxFile file);

AFX afxResult               AfxFileFlush(afxFile file);
AFX afxBool                 AfxFileShouldBeFlushed(afxFile file);

#endif//AFX_FILE_H