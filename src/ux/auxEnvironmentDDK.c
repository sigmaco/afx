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
#include "auxIcd.h"

_AUX afxUnit64 _AfxEnvPollInputCb(afxEnvironment env, afxFlags flags, afxUnit64 timeout)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_ENV, 1, &env);
    return 0;
}

_AUX afxBool _AfxEnvHasClipboardContentCb(afxEnvironment env, afxUnit seat, afxUnit slot, afxFlags flags)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_ENV, 1, &env);
    AFX_ASSERT_RANGE(env->seatCnt, seat, 1);
    return env->seats[seat].clipb.s.len;
}

_AUX afxUnit _AfxEnvGetClipboardContentCb(afxEnvironment env, afxUnit seat, afxUnit slot, afxFlags flags, afxString* buf)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_ENV, 1, &env);
    AFX_ASSERT_RANGE(env->seatCnt, seat, 1);
    return AfxCopyString(buf, 0, &env->seats[seat].clipb.s, 0);
}

_AUX afxError _AfxEnvSetClipboardContentCb(afxEnvironment env, afxUnit seat, afxUnit slot, afxFlags flags, afxString const* buf)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_ENV, 1, &env);
    AFX_ASSERT_RANGE(env->seatCnt, seat, 1);
    AfxCopyString(&env->seats[seat].clipb.s, 0, buf, 0);
    return err;
}

_AUX afxError _AfxEnvFocusWindowCb(afxEnvironment env, afxUnit seat, afxWindow wnd, afxFlags flags)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_ENV, 1, &env);
    AFX_ASSERT_RANGE(env->seatCnt, seat, 1);

    afxWindow curr = env->focusedWnd;

    if (curr != wnd)
    {
        if (curr)
        {
            AFX_ASSERT_OBJECTS(afxFcc_WND, 1, &curr);

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
            AFX_ASSERT_OBJECTS(afxFcc_WND, 1, &wnd);

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

_AUX _auxDdiEnv const _AUX_DDI_ENV =
{
    .pumpCb = _AfxEnvPollInputCb,
    .hasClipboardCb = _AfxEnvHasClipboardContentCb,
    .getClipboardCb = _AfxEnvGetClipboardContentCb,
    .setClipboardCb = _AfxEnvSetClipboardContentCb,
    .focusCb = _AfxEnvFocusWindowCb,
};



