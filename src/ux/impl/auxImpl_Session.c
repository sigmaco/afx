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
#define _AUX_SESSION_C
#define _AUX_WINDOW_C
#include "auxImplementation.h"

_AUX afxUnit64 _AfxSesPollInputCb(afxSession ses, afxFlags flags, afxUnit64 timeout)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SES, 1, &ses);
    return 0;
}

_AUX afxBool _AfxSesHasClipboardContentCb(afxSession ses, afxFlags flags)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SES, 1, &ses);
    return ses->clipb.s.len;
}

_AUX afxUnit _AfxSesGetClipboardContentCb(afxSession ses, afxString* buf)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SES, 1, &ses);
    return AfxCopyString(buf, 0, &ses->clipb.s, 0);
}

_AUX afxError _AfxSesSetClipboardContentCb(afxSession ses, afxString const* buf)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SES, 1, &ses);
    AfxCopyString(&ses->clipb.s, 0, buf, 0);
    return err;
}

_AUX afxError _AfxSesFocusWindowCb(afxSession ses, afxWindow wnd, afxFlags flags)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SES, 1, &ses);

    afxWindow curr = ses->focusedWnd;

    if (curr != wnd)
    {
        if (curr)
        {
            auxEvent ev = { 0 };
            ev.id = auxEventId_FOCUS_LOST;
            ev.wnd = curr;
            AfxEmitEvent(curr, &ev);

            if (curr->fullscreen)
            {
                AfxMakeWindowExclusive(curr, FALSE);
            }
            curr->focused = FALSE;
            ses->focusedWnd = NIL;
        }

        if (wnd)
        {
            wnd->focused = TRUE;
            ses->focusedWnd = wnd;

            auxEvent ev = { 0 };
            ev.id = auxEventId_FOCUS;
            ev.wnd = wnd;
            AfxEmitEvent(wnd, &ev);
        }
    }

    return err;
}

_AUX _auxSesImpl const _AUX_SES_IMPL =
{
    .pumpCb = _AfxSesPollInputCb,
    .hasClipboardCb = _AfxSesHasClipboardContentCb,
    .getClipboardCb = _AfxSesGetClipboardContentCb,
    .setClipboardCb = _AfxSesSetClipboardContentCb,
    .focusCb = _AfxSesFocusWindowCb,
};



