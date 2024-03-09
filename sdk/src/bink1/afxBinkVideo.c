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
 *                       (c) 2017 SIGMA, Engineering In Technology
 *                             <https://sigmaco.org/qwadro/>
 */

#define _CRT_SECURE_NO_WARNINGS 1
#define WIN32_LEAN_AND_MEAN 1
#include <Windows.h>
//#include "../dep/bink/bink.h"
#define BINKNOFRAMEBUFFERS    0x00000400L // Don't allocate internal frame buffers - application must call BinkRegisterFrameBuffers

#define _AFX_BINK_VIDEO_C
#include "afxBinkProxy.h"
#include "afxBinkVideo.h"
#include "qwadro/core/afxSystem.h"
#include "qwadro/draw/pipe/afxDrawScript.h"
#include "qwadro/draw/pipe/afxDrawCommands.h"

static void Start_us_count(afxNat64* out_count)
{
    QueryPerformanceCounter((LARGE_INTEGER*)out_count);
}

static afxNat32 Delta_us_count(afxNat64* last_count)
{
    static afxNat64 frequency = 1000;
    static afxInt32 got_frequency = 0;
    afxNat64 start;

    if (!got_frequency)
    {
        got_frequency = 1;
        QueryPerformanceFrequency((LARGE_INTEGER*)&frequency);
    }

    start = *last_count;
    QueryPerformanceCounter((LARGE_INTEGER*)last_count);
    return((afxNat32)(((*last_count - start) * (afxNat64)1000000) / frequency));
}

#define Start_timer() { afxNat64 __timer; Start_us_count( &__timer );
#define End_and_start_next_timer( count ) count += Delta_us_count( &__timer );
#define End_timer( count ) End_and_start_next_timer( count ) }

_AFXEXPORT afxError FreeBinkTextures(afxBinkVideo *bnk)
{
    afxError err = AFX_ERR_NONE;

    for (int i = 0; i < bnk->buffers.TotalFrames; ++i)
        for (afxNat j = 0; j < 4; j++)
            if (bnk->rasters[i][j])
                AfxReleaseObjects(1, (void*[]) { bnk->rasters[i][j] });

    return err;
}

_AFXEXPORT afxError CreateBinkTextures(afxBinkVideo *bnk)
{
    afxError err = AFX_ERR_NONE;

    afxRasterInfo texi[4] = { 0 };
    texi[0].whd[0] = (texi[3].whd[0] = bnk->buffers.YABufferWidth);
    texi[0].whd[1] = (texi[3].whd[1] = bnk->buffers.YABufferHeight);
    texi[1].whd[0] = (texi[2].whd[0] = bnk->buffers.cRcBBufferWidth);
    texi[1].whd[1] = (texi[2].whd[1] = bnk->buffers.cRcBBufferHeight);
    texi[0].whd[2] = (texi[1].whd[2] = (texi[2].whd[2] = (texi[3].whd[2] = 1)));
    texi[0].fmt = (texi[1].fmt = (texi[2].fmt = (texi[3].fmt = afxPixelFormat_R8)));
    texi[0].usage = (texi[1].usage = (texi[2].usage = (texi[3].usage = afxRasterUsage_SAMPLING)));
    
    afxNat rasCnt = (bnk->hasAlphaPlane || (bnk->hasAlphaPlane = bnk->buffers.Frames[0][3].Allocate)) ? 4 : 3;

    afxRasterRegion rgn = { 0 };
    rgn.lodIdx = 0;
    rgn.baseLayer = 0;
    rgn.layerCnt = 1;
    rgn.offset[0] = 0;
    rgn.offset[1] = 0;
    rgn.offset[2] = 0;

    for (afxNat i = 0; i < BINKMAXFRAMEBUFFERS; ++i)
    {
        if (AfxAcquireRasters(bnk->dctx, rasCnt, texi, bnk->rasters[i])) AfxThrowError();
        else
        {
            for (afxNat j = 0; j < rasCnt; j++)
            {
                AfxGetRasterExtent(bnk->rasters[i][j], 0, rgn.whd);

                afxNat rowLen;
                afxNat rgnSiz;

                void* start;

                if (!(start = AfxOpenRasterRegion(bnk->rasters[i][j], &rgn, afxRasterAccess_W, &rgnSiz, &rowLen)))
                {
                    AfxThrowError();
                    break;
                }

                bnk->buffers.Frames[i][j].Buffer = start;
                bnk->buffers.Frames[i][j].BufferPitch = rowLen;

                AfxCloseRasterRegion(bnk->rasters[i][j], &rgn);
            }
        }

    }

    if (err)
        FreeBinkTextures(bnk);

    return err;
}

void LockBinkTextures(afxBinkVideo *bnk)
{
    afxRasterRegion rgn;
    rgn.lodIdx = 0;
    rgn.baseLayer = 0;
    rgn.layerCnt = 1;
    rgn.offset[0] = 0;
    rgn.offset[1] = 0;
    rgn.offset[2] = 0;

    for (afxInt i = 0; i < bnk->buffers.TotalFrames; ++i)
    {
        afxNat rowLen;
        afxNat rasCnt = (bnk->hasAlphaPlane) ? 4 : 3;

        for (afxNat j = 0; j < rasCnt; j++)
        {
            AfxGetRasterExtent(bnk->rasters[i][j], 0, rgn.whd);

            if ((bnk->buffers.Frames[i][j].Buffer = AfxOpenRasterRegion(bnk->rasters[i][j], &rgn, afxRasterAccess_W, NIL, &rowLen))) // = lr.pBits;
                bnk->buffers.Frames[i][j].BufferPitch = rowLen;// = lr.Pitch;
        }
    }
}

void UnlockBinkTextures(afxBinkVideo *bnk)
{
    afxRasterRegion rgn;
    rgn.lodIdx = 0;
    rgn.baseLayer = 0;
    rgn.layerCnt = 1;
    rgn.offset[0] = 0;
    rgn.offset[1] = 0;
    rgn.offset[2] = 0;

    for (afxInt i = 0; i < bnk->buffers.TotalFrames; ++i)
    {
        afxNat rasCnt = (bnk->hasAlphaPlane) ? 4 : 3;

        for (afxNat j = 0; j < rasCnt; j++)
        {
            AfxGetRasterExtent(bnk->rasters[i][j], 0, rgn.whd);
            AfxCloseRasterRegion(bnk->rasters[i][j], &rgn);
            bnk->buffers.Frames[i][j].Buffer = NIL;
        }
    }
}

_AFXEXPORT afxError AfxBinkBlitFrame(afxBinkVideo *bnk, afxDrawScript dscr)
{
    afxError err = AFX_ERR_NONE;
    Start_timer();
    ++bnk->Frame_count;

    AfxAssertObjects(1, &bnk->yv12ToRgbaPip, afxFcc_PIP);
    AfxCmdBindPipeline(dscr, 0, bnk->yv12ToRgbaPip);

    // Set the textures.
    //AvxCmdBindLegos(dscr, 0, 1, &(bnk->rsrc[bnk->buffers.FrameNum].lego));
    AfxCmdBindRasters(dscr, 0, 0, 3, bnk->samplers, bnk->rasters[bnk->buffers.FrameNum]);

    AfxCmdDraw(dscr, 0, 0, 0, 4); // tristripped quad in shader

    End_timer(bnk->Render_microseconds);
    return err;
}

static void DecompressFrame(afxBinkVideo *bnk)
{
    Start_timer();

    //bnk->buffers.FrameNum = outBufIdx;

    // Lock the textures.
    LockBinkTextures(bnk);

    End_and_start_next_timer(bnk->Render_microseconds);

    // Decompress a frame
    BinkDoFrame(bnk->bik);

    // if we are falling behind, decompress an extra frame to catch up
    while (BinkShouldSkip(bnk->bik))
    {
        BinkNextFrame(bnk->bik);
        BinkDoFrame(bnk->bik);
    }

    End_and_start_next_timer(bnk->Bink_microseconds);

    // Unlock the textures.
    UnlockBinkTextures(bnk);

    End_and_start_next_timer(bnk->Render_microseconds);

    // Keep playing the movie.
    BinkNextFrame(bnk->bik);

    End_timer(bnk->Bink_microseconds);
}

_AFXEXPORT afxError AfxBinkDoFrame(afxBinkVideo *bnk, afxBool copyAll, afxBool neverSkip)
{
    (void)copyAll;
    (void)neverSkip;
    afxError err = AFX_ERR_NONE;

    // Is it time for a new Bink frame (we play at the speed of the background vid in this example)

    if (!BinkWait(bnk->bik))
    {
        DecompressFrame(bnk);
    }
    return err;
}

_AFXEXPORT afxError AfxBinkClose(afxBinkVideo *bnk)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(BinkClose);

    if (bnk->bik)
        BinkClose(bnk->bik);

    bnk->bik = NIL;
    FreeBinkTextures(bnk);

    return err;
}

_AFXEXPORT afxError AfxBinkOpen(afxBinkVideo *bnk, afxUri const *uri)
{
    afxError err = AFX_ERR_NONE;
    afxUri2048 uri2;
    AfxMakeUri2048(&uri2, NIL);

    AfxAssert(!bnk->bik);

    AfxResolveUri(afxFileFlag_R, uri, &uri2.uri);

    BinkSetSoundSystem(BinkOpenDirectSound, 0);

    void* bik;
    AfxAssert(BinkOpen);
    bnk->bik = (bik = BinkOpen((void*)uri2.buf, BINKNOFRAMEBUFFERS));
    AfxAssert(bik);

    BinkGetSummary(bik, &bnk->summary);

    bnk->whd[0] = bnk->summary.Width;
    bnk->whd[1] = bnk->summary.Height;
    bnk->whd[2] = 1;

    AfxZero(1, sizeof(bnk->buffers), &bnk->buffers);
    BinkGetFrameBuffersInfo(bnk->bik, &bnk->buffers);
    CreateBinkTextures(bnk);
    // Register our locked texture pointers with Bink
    BinkRegisterFrameBuffers(bnk->bik, &bnk->buffers);
    
    bnk->Last_timer = 0;

    return err;
}

_AFXEXPORT afxError AfxBinkDrop(afxBinkVideo *bnk)
{
    afxError err = AFX_ERR_NONE;
    AfxBinkClose(bnk);
    AfxReleaseObjects(1, (void*[]) { bnk->binkw32 });

    if (bnk->samplers[0])
        AfxReleaseObjects(1, (void*[]) { bnk->samplers[0] });

    if (bnk->yv12ToRgbaPip)
        AfxReleaseObjects(1, (void*[]) { bnk->yv12ToRgbaPip });

    //AfxReleaseObject(&bnk);
    return err;
}

_AFXEXPORT afxError AfxBinkDeploy(afxBinkVideo *bnk, afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    bnk->running = FALSE;

    BinkSetSoundSystem(BinkOpenMiles, 0);

    //AfxFindSymbolAddresses(bnk->binkw32, AFX_COUNTOF(test), test, bink19c.v);

    bnk->running = FALSE;
    bnk->dctx = dctx;
    bnk->bik = NIL;
    bnk->whd[0] = 1;
    bnk->whd[1] = 1;
    bnk->whd[2] = 1;
    
    bnk->yv12ToRgbaPip = NIL;

    afxSamplerConfig smpSpec = { 0 };
    smpSpec.magFilter = (smpSpec.minFilter = afxTexelFilter_LINEAR);
    smpSpec.mipmapFilter = afxTexelFilter_LINEAR;
    smpSpec.uvw[0] = (smpSpec.uvw[1] = (smpSpec.uvw[2] = afxTexelAddress_CLAMP));
    //smpSpec.uvw[1] = afxTexelAddress_REPEAT;
    //smpSpec.uvw[2] = afxTexelAddress_REPEAT;

    AfxAcquireSamplers(bnk->dctx, 1, &smpSpec, &bnk->samplers[0]);
    AfxAssertObjects(1, &bnk->samplers, afxFcc_SAMP);

    bnk->samplers[1] = bnk->samplers[0];
    bnk->samplers[2] = bnk->samplers[0];
    bnk->samplers[3] = bnk->samplers[0];

    afxUri uri;
    AfxMakeUri(&uri, "data/pipeline/video/rgbOutYuv.xsh.xml?yFlipped", 0);
    bnk->yv12ToRgbaPip = AfxAssemblyPipelineFromXsh(dctx, &uri);
    AfxAssert(bnk->yv12ToRgbaPip);

    return err;
}

