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

// TTY --- Our teletypewriter

#ifndef AFX_TERMINAL_H
#define AFX_TERMINAL_H

#include "qwadro/core/afxString.h"
#include "qwadro/core/afxRestring.h"

AFX afxBool     AfxAcquireTerminal(void);
AFX afxBool     AfxReleaseTerminal(void);

AFX afxResult   AfxPrompt(afxRestring* str, afxString const* ret);

AFX afxResult   AfxPrint(afxNat32 color, afxChar const* msg);
AFX afxResult   AfxPrintf(afxNat32 color, afxChar const* msg, ...);

#endif//AFX_TERMINAL_H
