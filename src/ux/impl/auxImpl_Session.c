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

#define _AUX_UX_C
#define _AUX_SESSION_C
#include "auxImplementation.h"

_AUX afxUnit64 _AfxSesPollInput(afxSession ses, afxFlags flags, afxUnit64 timeout)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SES, 1, &ses);
    return 0;
}

_AUX afxBool _AfxSesHasClipboardContent(afxSession ses, afxFlags flags)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SES, 1, &ses);
    return ses->clipb.s.len;
}

_AUX afxUnit _AfxSesGetClipboardContent(afxSession ses, afxString* buf)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SES, 1, &ses);
    return AfxCopyString(buf, 0, &ses->clipb.s, 0);
}

_AUX afxError _AfxSesSetClipboardContent(afxSession ses, afxString const* buf)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SES, 1, &ses);
    AfxCopyString(&ses->clipb.s, 0, buf, 0);
    return err;
}

_AUX afxError _AfxSesFocusWindow(afxSession ses, afxWindow wnd, afxFlags flags)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SES, 1, &ses);
    return err;
}

_AUX _auxSesImpl const _AUX_SES_IMPL =
{
    .pumpCb = _AfxSesPollInput,
    .hasClipboardCb = _AfxSesHasClipboardContent,
    .getClipboardCb = _AfxSesGetClipboardContent,
    .setClipboardCb = _AfxSesSetClipboardContent,
    .focusCb = _AfxSesFocusWindow,
};



