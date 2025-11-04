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

// This software is part of Advanced User Experiences Extensions & Experiments.

  //////////////////////////////////////////////////////////////////////////////
 // Advanced User Experience                                                 //
//////////////////////////////////////////////////////////////////////////////

#ifndef AUX_ENVIRONMENT_H
#define AUX_ENVIRONMENT_H

#include "qwadro/ux/afxUxDefs.h"
#include "qwadro/draw/avxViewport.h"
#include "qwadro/hid/afxKeyboard.h"

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

AFX_DEFINE_STRUCT(afxEnvironmentConfig)
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

AUX afxUnit AfxGetEnvironmentId
(
    // can be NIL
    afxEnvironment env
);

AUX afxBool AfxGetEnvironment
(
    afxEnvironment* environment
);

AUX afxError AfxConfigureEnvironment
(
    afxUnit icd, 
    afxEnvironmentConfig const* cfg
);

AUX afxError AfxAcquireEnvironment
(
    afxUnit icd,
    afxEnvironmentConfig const* cfg, 
    afxEnvironment* environment
);

////////////////////////////////////////////////////////////////////////////////

AUX afxError        AfxCloseEnvironment(void);

AUX afxError        AfxOpenEnvironment(afxEnvironment env, afxUri const* host, afxAuthMethod method, afxString const* credential);

AUX afxError        AfxStepEnvironment(afxEnvironment env, void const*, void*);

AUX afxBool         AfxGetEnvironmentVideo(afxDrawSystem* system);

AUX afxBool         AfxGetEnvironmentAudio(afxMixSystem* system, afxSink* sink);

AUX afxError        AfxBeginFrame(afxEnvironment env);
AUX afxError        AfxWaitFrame(afxEnvironment env);
AUX afxError        AfxEndFrame(afxEnvironment env);

AUX afxError        AfxReconnectKeyboard(afxEnvironment env, afxUnit portIdx, afxUnit hidNo);

AUX afxTime         AfxDoUx(afxFlags flags, afxUnit64 timeout);

AUX afxError        AfxTakeFullscreen(afxWindow wnd, afxBool fullscreen);

AUX afxBool         AfxGetCursorPlacement(afxRect* rc, afxWindow wnd, afxRect* onFrame, afxRect* onSurface);

AUX afxBool         AfxHasClipboardContent(afxUnit slot, afxFlags flags);
AUX afxUnit         AfxGetClipboardContent(afxUnit slot, afxFlags flags, afxString* buf);
AUX afxError        AfxSetClipboardContent(afxUnit slot, afxFlags flags, afxString const* text);

AUX afxError        AfxFocusWindow(afxWindow wnd, afxFlags flags);
AUX afxBool         AfxGetFocusedWindow(afxWindow* window);

AUX afxError        AfxDrawBackgroundEXT(afxDrawContext dctx, afxFlags flags);

#endif//AUX_ENVIRONMENT_H
