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

#include "qwadro/inc/mix/io/afxAudio.h"

AFX_DEFINE_STRUCT(amxSessionConfig)
{
    afxUnit exuIdx;
};

AMX afxError AmxAcquireMediaSession(afxMixSystem msys, amxSessionConfig const* cfg, amxSession* session);

// The transport control methods:
// Pauses the session.
// This method pauses the presentation clock.
// This method is asynchronous. When the operation completes, the session sends an MESessionPaused event.
// This method fails if the session is stopped.
afxError AmxPauseSession(amxSession mses);
// Starts the session.
// When this method is called, the session starts the presentation clock and begins to process media samples.
// This method is asynchronous. When the method completes, the session sends an MESessionStarted event.
afxError AmxStartSession(amxSession mses);
// Stops the session.
// This method is asynchronous. When the operation completes, the session sends an MESessionStopped event.
afxError AmxStopSession(amxSession mses);
// Retrieves the capabilities of the Media Session, based on the current presentation.

// Closes the Media Session and releases all of the resources it is using.
// This method is asynchronous. When the operation completes, the session sends an MESessionClosed event.
afxError AmxCloseSession(amxSession mses);
// Retrieves the session's presentation clock.
afxError AmxGetSessionClock(amxSession mses, afxClock* clock);
afxError AmxGetSessionCapabilies(amxSession mses, void* caps);
// Shuts down the session and releases all the resources used by the session.
// DisposeObjects(1, &mses);

AFX_DEFINE_STRUCT(amxSink)
// Media sinks. Stream sinks handle the actual processing of data on each stream.
{

};

// Sets the presentation clock on the media sink.
// During streaming, the media sink attempts to match rates with the presentation clock. Ideally, the media sink presents samples at the correct time according to the presentation clock and does not fall behind. Rateless media sinks are an exception to this rule, as they consume samples as quickly as possible and ignore the clock.
afxError AmxSetSinkClock(amxSink msnk, afxClock* clock);
// Shuts down the media sink and releases the resources it is using.
// AfxDisposeObjects(1, &msnk)

// Requests a sample from the media source.
afxError AmxRequestStreamSample(amxSink msnk);

#endif//AMX_SESSION_H
