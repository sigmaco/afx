/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *            Q W A D R O   M U L T I M E D I A   I N F R A S T R U C T U R E
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This software is part of Advanced Multimedia Extensions & Experiments.

#ifndef AMX_TRACKER_H
#define AMX_TRACKER_H

#include "qwadro/mix/amxAudio.h"
#include "qwadro/mix/afxSink.h"

AFX_DEFINE_STRUCT(amxTrackerConfig)
{
    afxFlags    flags;
    afxMask     exuMask;
    afxString   tag;
    void*       udd;
    // audio
    afxUnit     freq;
    afxUnit     binCnt;
    afxUnit     srcCnt;
    afxUnit     trackCnt;
    afxUnit     voiceCnt;
    // video
    afxFlags    vidFlags;
    amxFormat   picFmt;
    amxFormat   refPicFmt;
    afxUnit     maxPicWidth;
    afxUnit     maxPicHeight;
    afxUnit     maxDpbSlots;
    afxUnit     maxActiveRefPics;
};

AMX afxError AmxAcquireTracker
(
    afxMixSystem            msys, 
    amxTrackerConfig const* cfg, 
    amxTracker*             tracker
);

////////////////////////////////////////////////////////////////////////////////

// Starts or resumes a suspended/paused the session.
AMX afxError AmxStartTracker
(
    amxTracker  trax, 
    afxBool     resume
);

// Stops or pauses/suspends the session.
AMX afxError AmxStopTracker
(
    amxTracker  trax, 
    afxBool     suspend
);

// Exhausts the session and disposes all of the resources it is using.
AMX afxError AmxExhaustTracker
(
    amxTracker  trax, 
    afxFlags    flags
);

// Retrieves the session's presentation clock.
AMX afxError AmxGetTrackerClock(amxTracker trax, afxClock* clock);
AMX afxError AmxGetTrackerCapabilies(amxTracker trax, void* caps);


AMX afxCmdId AmxBindIoStream
(
    amxTracker trax, 
    afxUnit pin, 
    afxStream iob, 
    afxSize offset, 
    afxSize range, 
    afxUnit stride
);

AMX afxCmdId AmxBindBufferedStream
(
    amxTracker trax, 
    afxUnit pin, 
    amxBuffer buf, 
    afxSize offset, 
    afxSize range, 
    afxUnit freq, 
    amxFormat fmt
);

AMX afxCmdId AmxBindAudioTrack
(
    amxTracker trax, 
    afxUnit track, 
    amxAudio aud, 
    afxUnit srcIdx, 
    amxFormat fmt
);

AMX afxCmdId AmxBindBuffer
(
    amxTracker trax,
    afxUnit bank,
    amxBuffer buf,
    afxUnit offset,
    afxUnit range,
    afxUnit freq
);

AMX afxCmdId AmxBindAudio
(
    amxTracker trax,
    afxUnit bank,
    amxAudio aud
);

AMX afxCmdId AmxBindSink
(
    amxTracker trax,
    afxSink sink
);

#endif//AMX_TRACKER_H
