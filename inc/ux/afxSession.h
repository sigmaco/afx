/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *         Q W A D R O   M U L T I M E D I A   U X   I N F R A S T R U C T U R E
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
#include "qwadro/inc/math/avxViewport.h"
#include "qwadro/inc/ux/hid/afxKeyboard.h"
#include "qwadro/inc/ux/hid/afxMouse.h"

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

typedef enum afxSeatCaps
{
    afxSeatCap_KEYBOARD,
    afxSeatCap_MOUSE,
    afxSeatCap_GAMEPAD,
} afxSeatCaps;

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

    afxUnit             ddevId;
    afxUnit             vduIdx;
    afxDrawSystem       dsys;
    afxUnit             sdevId;
    afxUnit             soutIdx;
    afxMixSystem        msys;
};

AUX afxUnit         AfxEnumerateWidgets(afxWindow wnd, afxUnit first, afxUnit cnt, afxWidget widgets[]);

AUX afxError        AfxCloseSession(void);
AUX afxError        AfxOpenSession(afxSession ses, afxUri const* host, afxAuthMethod method, afxString const* credential);

AUX afxError        AfxStepSession(afxSession ses, void const*, void*);

AUX afxBool         AfxGetSessionVideo(afxDrawSystem* system);

AUX afxBool         AfxGetSessionAudio(afxMixSystem* system, afxSink* sink);

AUX afxError        AfxBeginFrame(afxSession ses);
AUX afxError        AfxWaitFrame(afxSession ses);
AUX afxError        AfxEndFrame(afxSession ses);

AUX afxError        AfxReconnectKeyboard(afxSession ses, afxUnit portIdx, afxUnit hidNo);

AUX afxTime         AfxPollInput(afxFlags flags, afxUnit64 timeout);

AUX afxError        AfxMakeWindowExclusive(afxWindow wnd, afxBool fullscreen);

AUX afxBool         AfxGetCursorPlacement(afxRect* rc, afxWindow wnd, afxRect* onFrame, afxRect* onSurface);

AUX afxBool         AfxHasClipboardContent(afxFlags flags);
AUX afxUnit         AfxGetClipboardContent(afxString* buf);
AUX afxError        AfxSetClipboardContent(afxString const* text);

AUX afxError    AfxFocusWindow(afxWindow wnd, afxFlags flags);
AUX afxBool     AfxGetFocusedWindow(afxWindow* window);

////////////////////////////////////////////////////////////////////////////////

AUX afxUnit         AfxGetSid(afxSession ses); // can be NIL

AUX afxBool         AfxGetSession(afxSession* session);

AUX afxError        AfxAcquireSession(afxUnit sshId, afxSessionConfig const* cfg, afxSession* session);

#endif//AUX_SESSION_H
