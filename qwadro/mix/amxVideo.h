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

#ifndef AMX_VIDEO_H
#define AMX_VIDEO_H

#include "qwadro/mix/afxMixDefs.h"

typedef enum amxVideoUsage
{
    amxVideoUsage_SAMPLED, // used as a texture by DSYS
    amxVideoUsage_MIX,
    amxVideoUsage_SINK,
} amxVideoUsage;

AFX_DEFINE_STRUCT(amxVideoInfo)
{
    afxUnit         latency; // number of buffers (e.g. 2)
    amxFormat       fmt; // type and bits per sample
    avxCanvasConfig ccfg;

    // video profile
    afxString       codec;
    afxUnit         subsampling[3]; // 4:2:0, 4:2:2, 4:4:4 --- nothing means monochrome.
    afxUnit         lumaBitDepth; // 8, 10, 12
    afxUnit         chromaBitDepth; // 8, 10, 12
};

AMX afxError AmxBindVideoSamples(amxVideo vid, afxUnit cnt, avxBufferedMap const maps[]);
AMX afxError AmxBindVideoBuffer(amxVideo vid, avxBuffer buf, afxSize offset, afxSize range);
AMX afxError AmxBindVideoPicture(amxVideo vid, afxUnit bufIdx, avxRaster ras);

// Decompresses the next frame of video on a background thread.
AMX afxError AmxDoVideoFrame(amxVideo vid, afxMask exuMask, afxFlags flags);
// Checks on the status of a background video frame decompression.
AMX afxError AmxWaitVideoFrame(amxVideo vid, afxUnit64 timeout, afxFlags flags);
// Moves to the next video frame in the video file.
AMX afxError AmxAdvanceVideo(amxVideo vid, afxFlags flags);

////////////////////////////////////////////////////////////////////////////////

AMX afxError AmxAcquireVideos(afxMixSystem msys, afxUnit cnt, amxVideoInfo const info[], amxVideo videos[]);

#endif//AMX_VIDEO_H
