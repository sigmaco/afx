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

// SIGMA Future Storage

#ifndef AFX_FILE_H
#define AFX_FILE_H

#include "afxStream.h"

AFX_DEFINE_HANDLE(afxFile);

AFX_OBJECT(afxFile)
{
    AFX_OBJECT(afxStream)   ios;
    void                    *fd;
    afxUri4096              uri;
    afxBool                 shouldBeFlushed;
};

AFX void*       AfxFileGetSystem(afxFile file);
AFX void*       AfxFileGetFileSystem(afxFile file);

AFX afxResult   AfxFileFlush(afxFile file);

#endif//AFX_FILE_H