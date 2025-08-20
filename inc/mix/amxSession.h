/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *       Q W A D R O   S O U N D   S Y N T H E S I S   I N F R A S T R U C T U R E
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

#ifndef AMX_SESSION_H
#define AMX_SESSION_H

#include "qwadro/inc/mix/amxAudio.h"
#include "qwadro/inc/mix/afxSink.h"

AFX_DEFINE_STRUCT(amxSessionConfig)
{
    afxUnit exuIdx;
    
};

// The transport control methods:
// Pauses the session.
// This method pauses the presentation clock.
// This method is asynchronous. When the operation completes, the session sends an MESessionPaused event.
// This method fails if the session is stopped.
AMX afxError AmxPauseSession(amxSession mses);
// Starts the session.
// When this method is called, the session starts the presentation clock and begins to process media samples.
// This method is asynchronous. When the method completes, the session sends an MESessionStarted event.
AMX afxError AmxStartSession(amxSession mses);
// Stops the session.
// This method is asynchronous. When the operation completes, the session sends an MESessionStopped event.
AMX afxError AmxStopSession(amxSession mses);
// Retrieves the capabilities of the Media Session, based on the current presentation.

// Closes the Media Session and releases all of the resources it is using.
// This method is asynchronous. When the operation completes, the session sends an MESessionClosed event.
AMX afxError AmxCloseSession(amxSession mses);
// Retrieves the session's presentation clock.
AMX afxError AmxGetSessionClock(amxSession mses, afxClock* clock);
AMX afxError AmxGetSessionCapabilies(amxSession mses, void* caps);
// Shuts down the session and releases all the resources used by the session.
// DisposeObjects(1, &mses);

////////////////////////////////////////////////////////////////////////////////

AMX afxError AmxAcquireMediaSession(afxMixSystem msys, amxSessionConfig const* cfg, amxSession* session);

#endif//AMX_SESSION_H
