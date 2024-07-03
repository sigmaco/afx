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

  //////////////////////////////////////////////////////////////////////////////
 // Qwadro on Windows                                                        //
//////////////////////////////////////////////////////////////////////////////

#ifndef QOW_DEFS_H
#define QOW_DEFS_H

#define _CRT_SECURE_NO_WARNINGS 1
#define WIN32_LEAN_AND_MEAN 1
#include <Windows.h>
#include <dwmapi.h>
#include <shellapi.h>
#include <hidusage.h>

#define _AUX_UX_C
#define _AUX_SHELL_C
#define _AUX_SHELL_IMPL
#define _AUX_WINDOW_C
#define _AUX_WINDOW_IMPL
#include "AuxDevKit.h"

#define QOW_PLUGIN_ID AFX_MAKE_FCC('q', 'o', 'w', '\0')

AFX_OBJECT(afxShell)
{
    AFX_OBJECT(_auxShell) m;
    WNDCLASSEX  wndClss;
};

AFX_OBJECT(afxWindow)
{
    AFX_OBJECT(_auxWindow) m;
    HWND        hWnd;
    HDC         hFrameDc;
    HDC         hSurfaceDc;
    HICON       hIcon;
    afxAtomic   lastBufIdx;
    afxBool     swap;
};

AUX afxKey const _win32VkToQwadro[256];

#endif//QOW_DEFS_H
