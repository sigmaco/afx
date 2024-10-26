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
 // Advanced User Experience                                                 //
//////////////////////////////////////////////////////////////////////////////

#ifndef AUX_SESSION_H
#define AUX_SESSION_H

#include "qwadro/inc/ux/afxUxDefs.h"
#include "qwadro/inc/math/afxViewport.h"
#include "qwadro/inc/ux/afxKeyboard.h"
#include "qwadro/inc/ux/afxMouse.h"

typedef enum afxSessionState
{
    afxSessionState_NONE,
    afxSessionState_CONNECTING,
    afxSessionState_SOCKET_CONNECTED,
    afxSessionState_BANNER_RECEIVED,
    afxSessionState_INITIAL_KEX,
    afxSessionState_KEXINIT_RECEIVED,
    afxSessionState_DH,
    afxSessionState_AUTHENTICATING,
    afxSessionState_AUTHENTICATED,
    afxSessionState_ERROR,
    afxSessionState_DISCONNECTED
} afxSessionState;

typedef enum afxAuthMethod
{
    afxAuthMethod_NONE, // Try to authenticate through the "none" method.
    afxAuthMethod_PASSWORD, // Try to authenticate by password.
    afxAuthMethod_PRIVKEY, // Authenticate with public/private key or certificate.
    afxAuthMethod_PASSPHRASE, // Tries to automatically authenticate with public key and "none".
    afxAuthMethod_PUBKEY, // Try to authenticate with the given public key.

    afxAuthMethod_TOTAL
} afxAuthMethod;

AFX_DEFINE_STRUCT(afxSeatConfig)
{
    afxString           u;
    afxFlags            flags;
    afxUnit             portIdx;
    afxUnit             queueCnt;
    afxReal const*      queuePriority;
};

AFX_DEFINE_STRUCT(afxSessionConfig)
{
    afxUnit             seatCnt;
    afxSeatConfig const*seats;

    afxString           username;

    // Note from libssh
    // Most server implementations do not permit changing the username during authentication.
    // The username should only be set with ssh_options_set() only before you connect to the server.

    afxDrawDevice       ddevId;
    afxUnit             vduIdx;
    afxDrawContext      dctx;

    afxSoundContext     sctx;
};

AUX afxClass const* AfxGetWidgetClass(afxSession ses);
AUX afxClass const* AfxGetWindowClass(afxSession ses);
AUX afxClass const* AfxGetScriptClass(afxSession ses);

AUX afxUnit         AfxEnumerateWidgets(afxSession ses, afxUnit first, afxUnit cnt, afxWidget widgets[]);

AUX afxError        AfxCloseSession(afxSession ses);
AUX afxError        AfxOpenSession(afxSession ses, afxUri const* host, afxAuthMethod method, afxString const* credential);

AUX afxError        AfxStepSession(afxSession ses, void const*, void*);

AUX afxError        AfxBeginFrame(afxSession ses);
AUX afxError        AfxWaitFrame(afxSession ses);
AUX afxError        AfxEndFrame(afxSession ses);

AUX afxError        AfxReconnectKeyboard(afxSession ses, afxUnit portIdx, afxUnit hidNo);

////////////////////////////////////////////////////////////////////////////////

AUX afxUnit         AfxGetSid(afxSession ses); // can be NIL

AUX afxError        AfxAcquireSession(afxUnit sshId, afxSessionConfig const* cfg, afxSession* session);

#endif//AUX_SESSION_H
