/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                      Q W A D R O   U S E R   I / O   S Y S T E M
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This software is part of Advanced Multimedia UX Extensions & Experiments.

#define _AUX_UX_C
#define _AUX_WINDOW_C
#include "auxImplementation.h"

_AUX afxUnit _AfxWndFormatTitleCb(afxWindow wnd)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_WND, 1, &wnd);

    AFX_ASSERT(AfxGetTid() == AfxGetObjectTid(wnd));

    return 0;
}

_AUX afxError _AfxWndAdjustCb(afxWindow wnd, afxRect const* c)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_WND, 1, &wnd);
    return err;
}

_AUX _auxWndDdi const _AUX_WND_IMPL =
{
    .adjustCb = _AfxWndAdjustCb,
    .titleCb = _AfxWndFormatTitleCb
};

