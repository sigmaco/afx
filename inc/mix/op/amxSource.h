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

#ifndef AMX_SOURCE_H
#define AMX_SOURCE_H

#include "qwadro/inc/mix/io/afxAudio.h"

AFX_DEFINE_STRUCT(amxStream)
// Represents one stream in a media source.
{

};

AFX_OBJECT(amxSource)
// Media sources are objects that generate media data. For example, the data might come from a video file, 
// a network stream, or a hardware device, such as a camera. Each media source contains one or more streams, 
// and each stream delivers data of one type, such as audio or video.
{

};

// Stops all active streams in the media source.
// This method is asynchronous. When the operation completes, the media source sends and MESourceStopped event, and every active stream sends an MEStreamStopped event.
// When a media source is stopped, its current position reverts to zero. After that, if the Start method is called with VT_EMPTY for the starting position, playback starts from the beginning of the presentation.
// While the source is stopped, no streams produce data.
afxError AmxStopSource(amxSource msrc);

// Starts, seeks, or restarts the media source by specifying where to start playback.
// A call to Start results in a seek if the previous state was started or paused, and the new starting position is not VT_EMPTY. Not every media source can seek.
afxError AfxStartSource(amxSource msrc, afxSize at);

// Pauses all active streams in the media source.
// The media source must be in the started state. The method fails if the media source is paused or stopped.
afxError AfxPauseSource(amxSource msrc);

// Shuts down the media source and releases the resources it is using.
// AfxDisposeObjects(1, &msrc);



#endif//AMX_SOURCE_H
