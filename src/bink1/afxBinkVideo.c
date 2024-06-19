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

#define _CRT_SECURE_NO_WARNINGS 1
#define WIN32_LEAN_AND_MEAN 1
#include <Windows.h>
//#include "../dep/bink/bink.h"
#define BINKNOFRAMEBUFFERS    0x00000400L // Don't allocate internal frame buffers - application must call BinkRegisterFrameBuffers

#define _AFX_BINK_VIDEO_C
#include "afxBinkProxy.h"
#include "afxBinkVideo.h"
#include "qwadro/afxQwadro.h"

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

DLLEXPORT afxError FreeBinkTextures(afxBinkVideo *bnk)
{
    afxError err = AFX_ERR_NONE;

    for (int i = 0; i < bnk->buffers.TotalFrames; ++i)
    {
        for (afxNat j = 0; j < 4; j++)
        {
            if (bnk->rasters[i][j])
                AfxReleaseObjects(1, (void*[]) { bnk->rasters[i][j] });

        }

        if (bnk->stageBuffers[i])
            AfxReleaseObjects(1, (void*[]) { bnk->stageBuffers[i] });
    }
    return err;
}

DLLEXPORT afxError CreateBinkTextures(afxBinkVideo *bnk)
{
    afxError err = AFX_ERR_NONE;

    afxPixelLayout pfd;
    AfxDescribePixelFormat(afxPixelFormat_R8, &pfd);
    afxRasterInfo texi[4] = { 0 };
    texi[0].whd[0] = (texi[3].whd[0] = bnk->buffers.YABufferWidth);
    texi[0].whd[1] = (texi[3].whd[1] = bnk->buffers.YABufferHeight);
    texi[1].whd[0] = (texi[2].whd[0] = bnk->buffers.cRcBBufferWidth);
    texi[1].whd[1] = (texi[2].whd[1] = bnk->buffers.cRcBBufferHeight);
    texi[0].whd[2] = (texi[1].whd[2] = (texi[2].whd[2] = (texi[3].whd[2] = 1)));
    texi[0].fmt = (texi[1].fmt = (texi[2].fmt = (texi[3].fmt = afxPixelFormat_R8)));
    texi[0].usage = (texi[1].usage = (texi[2].usage = (texi[3].usage = afxRasterUsage_SAMPLING)));
    
    afxNat rasCnt = (bnk->hasAlphaPlane || (bnk->hasAlphaPlane = bnk->buffers.Frames[0][3].Allocate)) ? 4 : 3;

    bnk->rasUnpakOff[0] = 0;
    bnk->rasUnpakSiz[0] = AFX_ALIGN(bnk->buffers.YABufferWidth * bnk->buffers.YABufferHeight, 64);
    bnk->rasUnpakOff[1] = bnk->rasUnpakSiz[0];
    bnk->rasUnpakSiz[1] = AFX_ALIGN(bnk->buffers.cRcBBufferWidth * bnk->buffers.cRcBBufferHeight, 64);
    bnk->rasUnpakOff[2] = bnk->rasUnpakSiz[1] + bnk->rasUnpakSiz[0];
    bnk->rasUnpakSiz[2] = AFX_ALIGN(bnk->buffers.cRcBBufferWidth * bnk->buffers.cRcBBufferHeight, 64);
    bnk->rasUnpakOff[3] = bnk->rasUnpakSiz[2] + bnk->rasUnpakSiz[1] + bnk->rasUnpakSiz[0];
    bnk->rasUnpakSiz[3] = bnk->hasAlphaPlane ? AFX_ALIGN(bnk->buffers.YABufferWidth * bnk->buffers.YABufferHeight, 64) : 0;

    afxBufferSpecification bufi = { 0 };
    bufi.siz = bnk->rasUnpakSiz[0] + bnk->rasUnpakSiz[1] + bnk->rasUnpakSiz[2] + bnk->rasUnpakSiz[3];
    bufi.access = afxBufferAccess_W;
    bufi.usage = afxBufferUsage_SRC;

    bnk->stageBufSiz = bufi.siz;

    for (afxNat i = 0; i < BINKMAXFRAMEBUFFERS; ++i)
    {
        if (AfxAcquireBuffers(bnk->dctx, 1, &bufi, &bnk->stageBuffers[i]))
            AfxThrowError();

        if (AfxAcquireRasters(bnk->dctx, rasCnt, texi, bnk->rasters[i])) AfxThrowError();
        else
        {
            for (afxNat j = 0; j < rasCnt; j++)
            {
                bnk->buffers.Frames[i][j].BufferPitch = AFX_ALIGN(((j == 1 || 2 == j) ? bnk->buffers.cRcBBufferWidth : bnk->buffers.YABufferWidth), 16);
            }
        }
    }

    if (err)
        FreeBinkTextures(bnk);

    return err;
}

#if !0
void LockBinkTextures(afxBinkVideo *bnk, afxNat i)
{
    afxError err = AFX_ERR_NONE;
    afxNat rasCnt = bnk->hasAlphaPlane ? 4 : 3;

#if !0
    for (afxInt i = 0; i < bnk->buffers.TotalFrames; ++i)
    {
        afxByte* start;

        if (!(start = AfxMapBuffer(bnk->stageBuffers[i], 0, bnk->stageBufSiz, NIL)))
        {
            AfxThrowError();
            break;
        }
        else
        {
            for (afxNat j = 0; j < rasCnt; j++)
            {
                bnk->buffers.Frames[i][j].Buffer = &start[bnk->rasUnpakOff[j]];
            }
        }
    }
#else
    afxByte* start;

    if (!(start = AfxMapBuffer(bnk->stageBuffers[i], 0, bnk->stageBufSiz[i], NIL)))
    {
        AfxThrowError();
    }
    else
    {
        for (afxNat j = 0; j < rasCnt; j++)
        {
            bnk->buffers.Frames[i][j].Buffer = &start[bnk->rasUnpakOff[j]];
        }
    }
#endif
    BinkRegisterFrameBuffers(bnk->bik, &bnk->buffers);
}

void UnlockBinkTextures(afxBinkVideo *bnk, afxNat i)
{
    afxError err = AFX_ERR_NONE;
#if !0
    for (afxInt i = 0; i < bnk->buffers.TotalFrames; ++i)
    {
        AfxUnmapBuffer(bnk->stageBuffers[i], TRUE);
    }
#else
    AfxUnmapBuffer(bnk->stageBuffers[i]);
#endif
}
#endif

DLLEXPORT afxError AfxBinkPrepareFrameBlit(afxBinkVideo *bnk, avxCmdb cmdb)
{
    afxError err = AFX_ERR_NONE;
    
    afxRasterIo op = { 0 };
    AfxGetRasterExtent(bnk->rasters[bnk->buffers.FrameNum][0], 0, op.rgn.whd);
    op.offset = bnk->rasUnpakOff[0];
    op.rowStride = bnk->buffers.Frames[bnk->buffers.FrameNum][0].BufferPitch;
    op.rowCnt = bnk->buffers.YABufferHeight;
    AvxCmdUnpackRaster(cmdb, bnk->rasters[bnk->buffers.FrameNum][0], &op, bnk->stageBuffers[bnk->buffers.FrameNum]);
    
    if (bnk->hasAlphaPlane)
    {
        op.offset = bnk->rasUnpakOff[3];
        op.rowStride = bnk->buffers.Frames[bnk->buffers.FrameNum][3].BufferPitch;
        op.rowCnt = bnk->buffers.YABufferHeight;
        AvxCmdUnpackRaster(cmdb, bnk->rasters[bnk->buffers.FrameNum][3], &op, bnk->stageBuffers[bnk->buffers.FrameNum]);
    }

    AfxGetRasterExtent(bnk->rasters[bnk->buffers.FrameNum][1], 0, op.rgn.whd);
    op.offset = bnk->rasUnpakOff[1];
    op.rowStride = bnk->buffers.Frames[bnk->buffers.FrameNum][1].BufferPitch;
    op.rowCnt = bnk->buffers.cRcBBufferHeight;
    AvxCmdUnpackRaster(cmdb, bnk->rasters[bnk->buffers.FrameNum][1], &op, bnk->stageBuffers[bnk->buffers.FrameNum]);
    op.offset = bnk->rasUnpakOff[2];
    op.rowStride = bnk->buffers.Frames[bnk->buffers.FrameNum][2].BufferPitch;
    op.rowCnt = bnk->buffers.cRcBBufferHeight;
    AvxCmdUnpackRaster(cmdb, bnk->rasters[bnk->buffers.FrameNum][2], &op, bnk->stageBuffers[bnk->buffers.FrameNum]);

    return err;
}

DLLEXPORT afxError AfxBinkBlitFrame(afxBinkVideo *bnk, avxCmdb cmdb)
{
    afxError err = AFX_ERR_NONE;
    Start_timer();
    ++bnk->Frame_count;

    AfxAssertObjects(1, &bnk->yv12ToRgbaRazr, afxFcc_RAZR);
    AvxCmdBindRasterizer(cmdb, bnk->yv12ToRgbaRazr, NIL);

    // Set the textures.
    //AvxCmdBindLegos(cmdb, 0, 1, &(bnk->rsrc[bnk->buffers.FrameNum].lego));
    AvxCmdBindSamplers(cmdb, 0, 0, 3, bnk->samplers);
    AvxCmdBindRasters(cmdb, 0, 0, 3, bnk->rasters[bnk->buffers.FrameNum], NIL);

    AvxCmdDraw(cmdb, 0, 1, 0, 4); // tristripped quad in shader

    End_timer(bnk->Render_microseconds);
    return err;
}

static void DecompressFrame(afxBinkVideo *bnk)
{
    Start_timer();

    //bnk->buffers.FrameNum = outBufIdx;

    // Lock the textures.
    LockBinkTextures(bnk, bnk->buffers.FrameNum);

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
    UnlockBinkTextures(bnk, bnk->buffers.FrameNum);

    End_and_start_next_timer(bnk->Render_microseconds);

    // Keep playing the movie.
    BinkNextFrame(bnk->bik);

    End_timer(bnk->Bink_microseconds);
}

DLLEXPORT afxError AfxBinkDoFrame(afxBinkVideo *bnk, afxBool copyAll, afxBool neverSkip)
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

DLLEXPORT afxError AfxBinkClose(afxBinkVideo *bnk)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(BinkClose);

    if (bnk->bik)
        BinkClose(bnk->bik);

    bnk->bik = NIL;
    FreeBinkTextures(bnk);

    return err;
}

DLLEXPORT afxError AfxOpenVideoBink(afxBinkVideo *bnk, afxUri const *uri)
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

    AfxZero2(1, sizeof(bnk->buffers), &bnk->buffers);
    BinkGetFrameBuffersInfo(bnk->bik, &bnk->buffers);
    CreateBinkTextures(bnk);
    // Register our locked texture pointers with Bink
    BinkRegisterFrameBuffers(bnk->bik, &bnk->buffers);
    
    bnk->Last_timer = 0;

    return err;
}

DLLEXPORT afxError AfxDropVideoBink(afxBinkVideo *bnk)
{
    afxError err = AFX_ERR_NONE;
    AfxBinkClose(bnk);
    AfxReleaseObjects(1, &bnk->binkw32);

    if (bnk->samplers[0])
        AfxReleaseObjects(1, &bnk->samplers[0]);

    if (bnk->yv12ToRgbaRazr)
        AfxReleaseObjects(1, &bnk->yv12ToRgbaRazr);

    //AfxReleaseObject(&bnk);
    return err;
}

DLLEXPORT afxError AfxSetUpBinkPlayer(afxBinkVideo *bnk, afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    bnk->running = FALSE;

    //BinkSetSoundSystem(BinkOpenMiles, 0);

    //AfxFindSymbolAddresses(bnk->binkw32, AFX_COUNTOF(test), test, bink19c.v);

    bnk->running = FALSE;
    bnk->dctx = dctx;
    bnk->bik = NIL;
    bnk->whd[0] = 1;
    bnk->whd[1] = 1;
    bnk->whd[2] = 1;
    
    bnk->yv12ToRgbaRazr = NIL;

    avxSamplerConfig smpSpec = { 0 };
    smpSpec.magFilter = (smpSpec.minFilter = avxTexelFilter_LINEAR);
    smpSpec.mipFilter = avxTexelFilter_LINEAR;
    smpSpec.uvw[0] = (smpSpec.uvw[1] = (smpSpec.uvw[2] = avxTexelAddress_CLAMP));
    //smpSpec.uvw[1] = avxTexelAddress_REPEAT;
    //smpSpec.uvw[2] = avxTexelAddress_REPEAT;

    AfxAcquireSamplers(bnk->dctx, 1, &smpSpec, &bnk->samplers[0]);
    AfxAssertObjects(1, &bnk->samplers, afxFcc_SAMP);

    bnk->samplers[1] = bnk->samplers[0];
    bnk->samplers[2] = bnk->samplers[0];
    bnk->samplers[3] = bnk->samplers[0];

    afxUri uri;
    AfxMakeUri(&uri, "../system/video/rgbOutYuv.xsh.xml?yFlipped", 0);
    bnk->yv12ToRgbaRazr = AfxLoadRasterizerFromXsh(dctx, NIL, &uri);
    AfxAssert(bnk->yv12ToRgbaRazr);

    return err;
}

