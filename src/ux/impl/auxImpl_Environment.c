/*
 *           ::::::::    :::::::::::    ::::::::    ::::     ::::       :::
 *          :+:    :+:       :+:       :+:    :+:   +:+:+: :+:+:+     :+: :+:
 *          +:+              +:+       +:+          +:+ +:+:+ +:+    +:+   +:+
 *          +#++:++#++       +#+       :#:          +#+  +:+  +#+   +#++:++#++:
 *                 +#+       +#+       +#+   +#+#   +#+       +#+   +#+     +#+
 *          #+#    #+#       #+#       #+#    #+#   #+#       #+#   #+#     #+#
 *           ########    ###########    ########    ###       ###   ###     ###
 *
 *                     S I G M A   T E C H N O L O G Y   G R O U P
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This software is part of Advanced User Experiences Extensions & Experiments.

#define _AUX_UX_C
#define _AUX_ENVIRONMENT_C
#define _AUX_WINDOW_C
#include "auxImplementation.h"

_AUX afxUnit64 _AfxEnvPollInputCb(afxEnvironment env, afxFlags flags, afxUnit64 timeout)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_ENV, 1, &env);
    return 0;
}

_AUX afxBool _AfxEnvHasClipboardContentCb(afxEnvironment env, afxUnit slot, afxFlags flags)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_ENV, 1, &env);
    return env->clipb.s.len;
}

_AUX afxUnit _AfxEnvGetClipboardContentCb(afxEnvironment env, afxUnit slot, afxFlags flags, afxString* buf)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_ENV, 1, &env);
    return AfxCopyString(buf, 0, &env->clipb.s, 0);
}

_AUX afxError _AfxEnvSetClipboardContentCb(afxEnvironment env, afxUnit slot, afxFlags flags, afxString const* buf)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_ENV, 1, &env);
    AfxCopyString(&env->clipb.s, 0, buf, 0);
    return err;
}

_AUX afxError _AfxEnvFocusWindowCb(afxEnvironment env, afxWindow wnd, afxFlags flags)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_ENV, 1, &env);

    afxWindow curr = env->focusedWnd;

    if (curr != wnd)
    {
        if (curr)
        {
            auxEvent ev = { 0 };
            ev.id = auxEventId_FOCUS_LOST;
            ev.wnd = curr;
            AfxEmitEvent(curr, &ev.ev);

            if (curr->fullscreen)
            {
                AfxTakeFullscreen(curr, FALSE);
            }
            curr->focused = FALSE;
            env->focusedWnd = NIL;
        }

        if (wnd)
        {
            if (gActiveEnv != env)
                gActiveEnv = env;

            wnd->focused = TRUE;
            env->focusedWnd = wnd;

            auxEvent ev = { 0 };
            ev.id = auxEventId_FOCUS;
            ev.wnd = wnd;
            AfxEmitEvent(wnd, &ev.ev);
        }
    }

    return err;
}

_AUX _auxEnvImpl const _AUX_ENV_IMPL =
{
    .pumpCb = _AfxEnvPollInputCb,
    .hasClipboardCb = _AfxEnvHasClipboardContentCb,
    .getClipboardCb = _AfxEnvGetClipboardContentCb,
    .setClipboardCb = _AfxEnvSetClipboardContentCb,
    .focusCb = _AfxEnvFocusWindowCb,
};



