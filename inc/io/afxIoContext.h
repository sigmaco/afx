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

#ifndef AFX_IO_CONTEXT_H
#define AFX_IO_CONTEXT_H

#include "qwadro/inc/io/afxIoBridge.h"

AFX afxError        AfxExhaustIoContext
(
    afxIoContext    ioc,
    afxBool         freeMem
);

AFX afxError        AfxRecordIoCommands
(
    afxIoContext    ioc, 
    afxBool         once, 
    afxBool         deferred, 
    afxUnit*        batchId
);

AFX afxError        AfxDiscardIoCommands
(
    afxIoContext    ioc,
    afxBool         freeRes
);

AFX afxError        AfxCompileIoCommands
(
    afxIoContext    ioc,
    afxUnit         batchId
);

AFX afxError        AfxRecycleIoCommands
(
    afxIoContext    ioc,
    afxUnit         batchId, 
    afxBool         freeRes
);

AFX afxBool         AfxDoesIoCommandsExist(afxIoContext ioc, afxUnit batchId);

////////////////////////////////////////////////////////////////////////////////

AFX afxError        AfxAcquireIoContexts
(
    afxIoCaps       caps,
    afxMask         exuMask,
    afxUnit         cnt, 
    afxIoContext    contexts[]
);

#endif//AFX_IO_CONTEXT_H
