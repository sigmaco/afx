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

#ifndef AFX_BINK_VIDEO_H
#define AFX_BINK_VIDEO_H

#include "qwadro/ux/afxUxDefs.h"
#include "qwadro/draw/io/afxRaster.h"
#include "qwadro/draw/io/afxBuffer.h"
#include "qwadro/core/afxSystem.h"

AFX_DEFINE_STRUCT(avxBinkSummary)
{
    afxNat Width;                  // Width of frames
    afxNat Height;                 // Height of frames
    afxNat TotalTime;              // total time (ms)
    afxNat FileFrameRate;          // frame rate
    afxNat FileFrameRateDiv;       // frame rate divisor
    afxNat FrameRate;              // frame rate
    afxNat FrameRateDiv;           // frame rate divisor
    afxNat TotalOpenTime;          // Time to open and prepare for decompression
    afxNat TotalFrames;            // Total Frames
    afxNat TotalPlayedFrames;      // Total Frames played
    afxNat SkippedFrames;          // Total number of skipped frames
    afxNat SkippedBlits;           // Total number of skipped blits
    afxNat SoundSkips;             // Total number of sound skips
    afxNat TotalBlitTime;          // Total time spent blitting
    afxNat TotalReadTime;          // Total time spent reading
    afxNat TotalVideoDecompTime;   // Total time spent decompressing video
    afxNat TotalAudioDecompTime;   // Total time spent decompressing audio
    afxNat TotalIdleReadTime;      // Total time spent reading while idle
    afxNat TotalBackReadTime;      // Total time spent reading in background
    afxNat TotalReadSpeed;         // Total io speed (bytes/second)
    afxNat SlowestFrameTime;       // Slowest single frame time (ms)
    afxNat Slowest2FrameTime;      // Second slowest single frame time (ms)
    afxNat SlowestFrameNum;        // Slowest single frame number
    afxNat Slowest2FrameNum;       // Second slowest single frame number
    afxNat AverageDataRate;        // Average data rate of the movie
    afxNat AverageFrameSize;       // Average size of the frame
    afxNat HighestMemAmount;       // Highest amount of memory allocated
    afxNat TotalIOMemory;          // Total extra memory allocated
    afxNat HighestIOUsed;          // Highest extra memory actually used
    afxNat Highest1SecRate;        // Highest 1 second rate
    afxNat Highest1SecFrame;       // Highest 1 second start frame
};

AFX_DEFINE_STRUCT(avxBinkPlane)
{
    afxInt Allocate;
    void* Buffer;
    afxNat BufferPitch;
};

AFX_DEFINE_STRUCT(avxBinkFramePlaneSet)
{
    avxBinkPlane YPlane;
    avxBinkPlane cRPlane;
    avxBinkPlane cBPlane;
    avxBinkPlane APlane;
};

#define BINKMAXFRAMEBUFFERS 2

AFX_DEFINE_STRUCT(avxBinkFramebuffers)
{
    afxInt TotalFrames;
    afxNat YABufferWidth;
    afxNat YABufferHeight;
    afxNat cRcBBufferWidth;
    afxNat cRcBBufferHeight;

    afxNat FrameNum;
    avxBinkPlane Frames[BINKMAXFRAMEBUFFERS][4]; // Y, cR, cB, A
};

AFX_DEFINE_STRUCT(afxBinkVideo)
{
    afxBool             running;
    afxDrawContext      dctx;
    afxModule           binkw32;
    void                *bik;
    afxNat              whd[3];

    avxBinkSummary      summary;
    
    // this is the Bink info on the textures
    avxBinkFramebuffers buffers;
    //avxCanvas               canv[BINKMAXFRAMEBUFFERS];
    // this is the GPU info for the textures
    afxNat              rasUnpakOff[4];
    afxNat              rasUnpakSiz[4];
    afxNat              stageBufSiz;
    afxBuffer           stageBuffers[BINKMAXFRAMEBUFFERS];
    afxRaster           rasters[BINKMAXFRAMEBUFFERS][4]; // Y, cR, cB, A.
    afxBool             hasAlphaPlane;

    afxRasterizer       yv12ToRgbaRazr;
    afxSampler          samplers[4];

    afxNat64            Last_timer;
    afxNat32            Frame_count;
    afxNat32            Bink_microseconds;
    afxNat32            Render_microseconds;
};

#ifdef _AFX_BINK_VIDEO_C
#define AFXBINK DLLEXPORT
#else
#define AFXBINK DLLIMPORT
#endif

AFXBINK afxError AfxSetUpBinkPlayer(afxBinkVideo *bnk, afxDrawContext dctx);
AFXBINK afxError AfxDropVideoBink(afxBinkVideo *bnk);

AFXBINK afxError AfxOpenVideoBink(afxBinkVideo *bnk, afxUri const *uri);
AFXBINK afxError AfxBinkClose(afxBinkVideo *bnk);

AFXBINK afxError AfxBinkDoFrame(afxBinkVideo *bnk, afxBool copyAll, afxBool neverSkip);
AFXBINK afxError AfxBinkBlitFrame(afxBinkVideo *bnk, avxCmdb cmdb);

#endif//AFX_BINK_VIDEO_H