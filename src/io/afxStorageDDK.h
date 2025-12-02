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

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#ifndef AFX_STORAGE_DDK_H
#define AFX_STORAGE_DDK_H

#include "qwadro/exec/afxSystem.h"

#ifdef _AFX_STORAGE_C

AFX_DEFINE_STRUCT(afxStorageUnit)
{
    afxLink          fsys;
    afxFileFlags        flags;
    afxUri              rootPath;  // path of exchange point
    afxArchive          arc;
};

AFX_OBJECT(afxStorage)
{
    afxChar             diskId; // single letter identifying this file system.
    afxUri8             baseUrl; // qualified name of exchange point <//./z/>
    afxChain            storages;
    afxChain            classes;
    afxClass            fileCls;
    afxClass            archCls;
    afxClass            urdCls;
    afxChain            fileChain;
};

#endif//_AFX_STORAGE_C

AFX afxClass const*     AfxGetFileClass(afxStorage fsys);
AFX afxClass const*     AfxGetArchiveClass(afxStorage fsys);

#endif//AFX_STORAGE_DDK_H
