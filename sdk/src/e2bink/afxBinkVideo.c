#define _CRT_SECURE_NO_WARNINGS 1
#define WIN32_LEAN_AND_MEAN 1
#include <Windows.h>
#include "../dep/bink/bink.h"

#define _AFX_BINK_VIDEO_C
#include "afxBinkVideo.h"
#include "qwadro/core/afxSystem.h"
#include "qwadro/draw/pipe/afxDrawScript.h"
#include "qwadro/draw/pipe/afxDrawCommands.h"

afxChar const *bink19cSymNamesCdecl[] =
{
    "BinkBufferBlit",
    "BinkBufferCheckWinPos",
    "BinkBufferClear",
    "BinkBufferClose",
    "BinkBufferGetDescription",
    "BinkBufferGetError",
    "BinkBufferLock",
    "BinkBufferOpen",
    "BinkBufferSetDirectDraw",
    "BinkBufferSetHWND",
    "BinkBufferSetOffset",
    "BinkBufferSetResolution",
    "BinkBufferSetScale",
    "BinkBufferUnlock",
    "BinkCheckCursor",
    "BinkClose",
    "BinkCloseTrack",
    "BinkControlBackgroundIO",
    "BinkControlPlatformFeatures",
    "BinkCopyToBuffer",
    "BinkCopyToBufferRect",
    "BinkDDSurfaceType",
    "BinkDX8SurfaceType",
    "BinkDX9SurfaceType",
    "BinkDoFrame",
    "BinkDoFrameAsync",
    "BinkDoFrameAsyncWait",
    "BinkGetError",
    "BinkGetFrameBuffersInfo",
    "BinkGetKeyFrame",
    "BinkGetPalette",
    "BinkGetRealtime",
    "BinkGetRects",
    "BinkGetSummary",
    "BinkGetTrackData",
    "BinkGetTrackID",
    "BinkGetTrackMaxSize",
    "BinkGetTrackType",
    "BinkGoto",
    "BinkIsSoftwareCursor",
    "BinkLogoAddress",
    "BinkNextFrame",
    "BinkOpen",
    "BinkOpenDirectSound",
    "BinkOpenMiles",
    "BinkOpenTrack",
    "BinkOpenWaveOut",
    "BinkPause",
    "BinkRegisterFrameBuffers",
    "BinkRequestStopAsyncThread",
    "BinkRestoreCursor",
    "BinkService",
    "BinkSetError",
    "BinkSetFrameRate",
    "BinkSetIO",
    "BinkSetIOSize",
    "BinkSetMemory",
    "BinkSetMixBinVolumes",
    "BinkSetMixBins",
    "BinkSetPan",
    "BinkSetSimulate",
    "BinkSetSoundOnOff",
    "BinkSetSoundSystem",
    "BinkSetSoundTrack",
    "BinkSetVideoOnOff",
    "BinkSetVolume",
    "BinkShouldSkip",
    "BinkStartAsyncThread",
    "BinkWait",
    "BinkWaitStopAsyncThread",
    "RADTimerRead"
};

afxChar const *bink19cSymNamesStdcall[] =
{
    "_BinkBufferBlit@12",
    "_BinkBufferCheckWinPos@12",
    "_BinkBufferClear@8",
    "_BinkBufferClose@4",
    "_BinkBufferGetDescription@4",
    "_BinkBufferGetError@0",
    "_BinkBufferLock@4",
    "_BinkBufferOpen@16",
    "_BinkBufferSetDirectDraw@8",
    "_BinkBufferSetHWND@8",
    "_BinkBufferSetOffset@12",
    "_BinkBufferSetResolution@12",
    "_BinkBufferSetScale@12",
    "_BinkBufferUnlock@4",
    "_BinkCheckCursor@20",
    "_BinkClose@4",
    "_BinkCloseTrack@4",
    "_BinkControlBackgroundIO@8",
    "_BinkControlPlatformFeatures@8",
    "_BinkCopyToBuffer@28",
    "_BinkCopyToBufferRect@44",
    "_BinkDDSurfaceType@4",
    "_BinkDX8SurfaceType@4",
    "_BinkDX9SurfaceType@4",
    "_BinkDoFrame@4",
    "_BinkDoFrameAsync@12",
    "_BinkDoFrameAsyncWait@8",
    "_BinkGetError@0",
    "_BinkGetFrameBuffersInfo@8",
    "_BinkGetKeyFrame@12",
    "_BinkGetPalette@4",
    "_BinkGetRealtime@12",
    "_BinkGetRects@8",
    "_BinkGetSummary@8",
    "_BinkGetTrackData@8",
    "_BinkGetTrackID@8",
    "_BinkGetTrackMaxSize@8",
    "_BinkGetTrackType@8",
    "_BinkGoto@12",
    "_BinkIsSoftwareCursor@8",
    "_BinkLogoAddress@0",
    "_BinkNextFrame@4",
    "_BinkOpen@8",
    "_BinkOpenDirectSound@4",
    "_BinkOpenMiles@4",
    "_BinkOpenTrack@8",
    "_BinkOpenWaveOut@4",
    "_BinkPause@8",
    "_BinkRegisterFrameBuffers@8",
    "_BinkRequestStopAsyncThread@4",
    "_BinkRestoreCursor@4",
    "_BinkService@4",
    "_BinkSetError@4",
    "_BinkSetFrameRate@8",
    "_BinkSetIO@4",
    "_BinkSetIOSize@4",
    "_BinkSetMemory@8",
    "_BinkSetMixBinVolumes@20",
    "_BinkSetMixBins@16",
    "_BinkSetPan@12",
    "_BinkSetSimulate@4",
    "_BinkSetSoundOnOff@8",
    "_BinkSetSoundSystem@8",
    "_BinkSetSoundTrack@8",
    "_BinkSetVideoOnOff@8",
    "_BinkSetVolume@12",
    "_BinkShouldSkip@4",
    "_BinkStartAsyncThread@8",
    "_BinkWait@4",
    "_BinkWaitStopAsyncThread@4",
    "_RADTimerRead@0"
};

union
{
    void *v[sizeof(bink19cSymNamesStdcall) / sizeof(bink19cSymNamesStdcall[0])];
    struct
    {
        void(__stdcall* BinkBufferBlit)(HBINKBUFFER buf, BINKRECT PTR4* rects, U32 numrects);
        void(__stdcall* BinkBufferCheckWinPos)(HBINKBUFFER buf, S32 PTR4* NewWindowX, S32 PTR4* NewWindowY);
        S32(__stdcall* BinkBufferClear)(HBINKBUFFER buf, U32 RGB);
        void(__stdcall* BinkBufferClose)(HBINKBUFFER buf);
        char PTR4* (__stdcall* BinkBufferGetDescription)(HBINKBUFFER buf);
        char PTR4* (__stdcall* BinkBufferGetError)(void);
        S32(__stdcall* BinkBufferLock)(HBINKBUFFER buf);
        HBINKBUFFER(__stdcall* BinkBufferOpen)(void* /*HWND*/ wnd, U32 width, U32 height, U32 bufferflags);
        S32(__stdcall* BinkBufferSetDirectDraw)(void PTR4* lpDirectDraw, void PTR4* lpPrimary);
        S32(__stdcall* BinkBufferSetHWND)(HBINKBUFFER buf, void* /*HWND*/ newwnd);
        S32(__stdcall* BinkBufferSetOffset)(HBINKBUFFER buf, S32 destx, S32 desty);
        void(__stdcall* BinkBufferSetResolution)(S32 w, S32 h, S32 bits);
        S32(__stdcall* BinkBufferSetScale)(HBINKBUFFER buf, U32 w, U32 h);
        S32(__stdcall* BinkBufferUnlock)(HBINKBUFFER buf);
        S32(__stdcall* BinkCheckCursor)(void* /*HWND*/ wnd, S32 x, S32 y, S32 w, S32 h);
        void(__stdcall* BinkClose)(HBINK bnk);
        void(__stdcall* BinkCloseTrack)(HBINKTRACK bnkt);
        S32(__stdcall* BinkControlBackgroundIO)(HBINK bink, U32 control);
        S32(__stdcall* BinkControlPlatformFeatures)(S32 use, S32 dont_use);
        S32(__stdcall* BinkCopyToBuffer)(HBINK bnk, void* dest, S32 destpitch, U32 destheight, U32 destx, U32 desty, U32 flags);
        S32(__stdcall* BinkCopyToBufferRect)(HBINK bnk, void* dest, S32 destpitch, U32 destheight, U32 destx, U32 desty, U32 srcx, U32 srcy, U32 srcw, U32 srch, U32 flags);
        S32(__stdcall* BinkDDSurfaceType)(void PTR4* lpDDS);
        S32(__stdcall* BinkDX8SurfaceType)(void* lpD3Ds);
        S32(__stdcall* BinkDX9SurfaceType)(void* lpD3Ds);
        S32(__stdcall* BinkDoFrame)(HBINK bnk);
        S32(__stdcall* BinkDoFrameAsync)(HBINK bink, U32 yplane_thread_num, U32 other_work_thread_num);
        S32(__stdcall* BinkDoFrameAsyncWait)(HBINK bink, S32 us);
        char PTR4* (__stdcall* BinkGetError)(void);
        void(__stdcall* BinkGetFrameBuffersInfo)(HBINK bink, BINKFRAMEBUFFERS * flego);
        U32(__stdcall* BinkGetKeyFrame)(HBINK bnk, U32 frame, S32 flags);
        void(__stdcall* BinkGetPalette)(void * out_pal);
        void(__stdcall* BinkGetRealtime)(HBINK bink, BINKREALTIME PTR4* run, U32 frames);
        S32(__stdcall* BinkGetRects)(HBINK bnk, U32 flags);
        void(__stdcall* BinkGetSummary)(HBINK bnk, BINKSUMMARY PTR4* sum);
        U32(__stdcall* BinkGetTrackData)(HBINKTRACK bnkt, void PTR4* dest);
        U32(__stdcall* BinkGetTrackID)(HBINK bnk, U32 trackindex);
        U32(__stdcall* BinkGetTrackMaxSize)(HBINK bnk, U32 trackindex);
        U32(__stdcall* BinkGetTrackType)(HBINK bnk, U32 trackindex);
        void(__stdcall* BinkGoto)(HBINK bnk, U32 frame, S32 flags);  // use 1 for the first frame
        S32(__stdcall* BinkIsSoftwareCursor)(void PTR4* lpDDSP, void* /*HCURSOR*/ cur);
        void PTR4* (__stdcall* BinkLogoAddress)(void);
        void(__stdcall* BinkNextFrame)(HBINK bnk);
        HBINK(__stdcall* BinkOpen)(const char PTR4* name, U32 flags);
        BINKSNDOPEN(__stdcall* BinkOpenDirectSound)(UINTa param); // don't call directly
        BINKSNDOPEN(__stdcall* BinkOpenMiles)(UINTa param); // don't call directly
        HBINKTRACK(__stdcall* BinkOpenTrack)(HBINK bnk, U32 trackindex);
        BINKSNDOPEN(__stdcall* BinkOpenWaveOut)(UINTa param); // don't call directly
        S32(__stdcall* BinkPause)(HBINK bnk, S32 pause);
        void(__stdcall* BinkRegisterFrameBuffers)(HBINK bink, BINKFRAMEBUFFERS * flego);
        S32(__stdcall* BinkRequestStopAsyncThread)(S32 thread_num);
        void(__stdcall* BinkRestoreCursor)(S32 checkcount);
        void(__stdcall* BinkService)(HBINK bink);
        void(__stdcall* BinkSetError)(const char PTR4* err);
        void(__stdcall* BinkSetFrameRate)(U32 forcerate, U32 forceratediv);
        void(__stdcall* BinkSetIO)(BINKIOOPEN io);
        void(__stdcall* BinkSetIOSize)(U32 iosize);
        void(__stdcall* BinkSetMemory)(BINKMEMALLOC a, BINKMEMFREE f);
        void(__stdcall* BinkSetMixBinVolumes)(HBINK bnk, U32 trackid, U32 PTR4* vol_mix_bins, S32 PTR4* volumes, U32 total);
        void(__stdcall* BinkSetMixBins)(HBINK bnk, U32 trackid, U32 PTR4* mix_bins, U32 total);
        void(__stdcall* BinkSetPan)(HBINK bnk, U32 trackid, S32 pan);
        void(__stdcall* BinkSetSimulate)(U32 sim);
        S32(__stdcall* BinkSetSoundOnOff)(HBINK bnk, S32 onoff);
        S32(__stdcall* BinkSetSoundSystem)(BINKSNDSYSOPEN open, UINTa param);
        void(__stdcall* BinkSetSoundTrack)(U32 total_tracks, U32 PTR4* tracks);
        S32(__stdcall* BinkSetVideoOnOff)(HBINK bnk, S32 onoff);
        void(__stdcall* BinkSetVolume)(HBINK bnk, U32 trackid, S32 volume);
        S32(__stdcall* BinkShouldSkip)(HBINK bink);
        S32(__stdcall* BinkStartAsyncThread)(S32 thread_num, void const * param);
        S32(__stdcall* BinkWait)(HBINK bnk);
        S32(__stdcall* BinkWaitStopAsyncThread)(S32 thread_num);
    };
}
bink19c;

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

_AFXEXPORT afxError FreeBinkTextures(BINKTEXTURESET *set)
{
    afxError err = AFX_ERR_NONE;

    // Free the texture memory and then the textures directly
    for (int i = 0; i < BINKMAXFRAMEBUFFERS; ++i)
    {
#if 0
        if (set->canv[i])
            AfxReleaseObjects(1, (void*[]) { set->canv[i] });
#endif

        BINKFRAMETEXTURES *rsrc = &set->rsrc[i];
        
        for (afxNat j = 0; j < sizeof(rsrc->surfaces) / sizeof(rsrc->surfaces[0]); j++)
        {
            if (rsrc->surfaces[j])
                AfxReleaseObjects(1, (void*[]) { rsrc->surfaces[j] });
        }
    }

    return err;
}

_AFXEXPORT afxError CreateBinkTextures(afxBinkVideo *bnk)
{
    afxError err = AFX_ERR_NONE;

    afxRasterInfo texi[4] = { 0 };
    texi[0].whd[0] = (texi[3].whd[0] = bnk->set.bink_buffers.YABufferWidth);
    texi[0].whd[1] = (texi[3].whd[1] = bnk->set.bink_buffers.YABufferHeight);
    texi[1].whd[0] = (texi[2].whd[0] = bnk->set.bink_buffers.cRcBBufferWidth);
    texi[1].whd[1] = (texi[2].whd[1] = bnk->set.bink_buffers.cRcBBufferHeight);
    texi[0].whd[2] = (texi[1].whd[2] = (texi[2].whd[2] = (texi[3].whd[2] = 1)));
    texi[0].fmt = (texi[1].fmt = (texi[2].fmt = (texi[3].fmt = afxPixelFormat_R8)));
    texi[0].usage = (texi[1].usage = (texi[2].usage = (texi[3].usage = afxRasterUsage_SAMPLING)));

    afxRaster surf;
    
    for (afxNat i = 0; i < BINKMAXFRAMEBUFFERS; ++i)
    {
        BINKFRAMETEXTURES *rsrc = &bnk->set.rsrc[i];
        afxNat rowLen;
        afxNat rgnSiz;
        afxRasterRegion rgn;
        rgn.lodIdx = 0;
        rgn.baseLayer = 0;
        rgn.layerCnt = 1;
        rgn.offset[0] = 0;
        rgn.offset[1] = 0;
        rgn.offset[2] = 0;

        for (afxNat j = 0; j < sizeof(rsrc->surfaces) / sizeof(rsrc->surfaces[0]); j++)
        {
            if (j == 0)
            {
                // Create Y plane

                AfxAssert(bnk->set.bink_buffers.Frames[i].YPlane.Allocate);

                rgn.whd[0] = texi[j].whd[0];
                rgn.whd[1] = texi[j].whd[1];
                rgn.whd[2] = texi[j].whd[2];

                if (AfxAcquireRasters(bnk->dctx, 1, &texi[j], &surf)) AfxThrowError();
                else
                {
                    bnk->set.rsrc[i].surfaces[j] = surf;

                    if (!(bnk->set.bink_buffers.Frames[i].YPlane.Buffer = AfxOpenRasterRegion(surf, &rgn, afxRasterAccess_W, &rgnSiz, &rowLen)))
                        AfxThrowError();
                    else
                        bnk->set.bink_buffers.Frames[i].YPlane.BufferPitch = rowLen;
                }
            }
            else if (j == 1)
            {
                // Create cR plane

                AfxAssert(bnk->set.bink_buffers.Frames[i].cRPlane.Allocate);

                rgn.whd[0] = texi[j].whd[0];
                rgn.whd[1] = texi[j].whd[1];

                if (AfxAcquireRasters(bnk->dctx, 1, &texi[j], &surf)) AfxThrowError();
                else
                {
                    bnk->set.rsrc[i].surfaces[j] = surf;

                    if (!(bnk->set.bink_buffers.Frames[i].cRPlane.Buffer = AfxOpenRasterRegion(surf, &rgn, afxRasterAccess_W, &rgnSiz, &rowLen)))
                        AfxThrowError();
                    else
                        bnk->set.bink_buffers.Frames[i].cRPlane.BufferPitch = rowLen;
                }
            }
            else if (j == 2)
            {
                AfxAssert(bnk->set.bink_buffers.Frames[i].cBPlane.Allocate);

                rgn.whd[0] = texi[j].whd[0];
                rgn.whd[1] = texi[j].whd[1];

                if (AfxAcquireRasters(bnk->dctx, 1, &texi[j], &surf)) AfxThrowError();
                else
                {
                    bnk->set.rsrc[i].surfaces[j] = surf;

                    if (!(bnk->set.bink_buffers.Frames[i].cBPlane.Buffer = AfxOpenRasterRegion(surf, &rgn, afxRasterAccess_W, &rgnSiz, &rowLen)))
                        AfxThrowError();
                    else
                        bnk->set.bink_buffers.Frames[i].cBPlane.BufferPitch = rowLen;
                }
            }
            else if (j == 3)
            {
                if (bnk->set.bink_buffers.Frames[i].APlane.Allocate)
                {
                    rgn.whd[0] = texi[j].whd[0];
                    rgn.whd[1] = texi[j].whd[1];

                    if (AfxAcquireRasters(bnk->dctx, 1, &texi[j], &surf)) AfxThrowError();
                    else
                    {
                        bnk->set.rsrc[i].surfaces[j] = surf;

                        if (!(bnk->set.bink_buffers.Frames[i].APlane.Buffer = AfxOpenRasterRegion(surf, &rgn, afxRasterAccess_W, &rgnSiz, &rowLen)))
                            AfxThrowError();
                        else
                            bnk->set.bink_buffers.Frames[i].APlane.BufferPitch = rowLen;
                    }
                }
            }

            if (err) break;
            else
            {
                bnk->set.rsrc[i].sizes[j] = rgnSiz;
                AfxCloseRasterRegion(surf, &rgn);
            }
        }
#if 0
        if (!err)
        {
            afxWhd extent;
            AvxGetRasterExtent(bnk->set.rsrc[i].surfaces[0], 0, extent);

            avxCanvasBlueprint canb;
            AfxBeginCanvasBlueprint(&canb, extent);
            AvxCanvasBlueprintAddRaster(&canb, avxPixelFormat_RGBA8, avxRasterFlag_SAMPLING | avxRasterFlag_DRAW);

            if (AfxBuildCanvases(bnk->dctx, 1, &bnk->set.canv[i], &canb))
                AfxThrowError();
        }
#endif
    }

    if (err)
        FreeBinkTextures(&bnk->set);

    return err;
}

void LockBinkTextures(afxBinkVideo *bnk)
{
    //int frame_cur = bnk->set.bink_buffers.FrameNum;

    for (afxInt i = 0; i < bnk->set.bink_buffers.TotalFrames; ++i)
    {
        //i = (bnk->set.bink_buffers.FrameNum + 1) % bnk->set.bink_buffers.TotalFrames; // hack
        BINKFRAMETEXTURES *rsrc = &bnk->set.rsrc[i];
        //U32 lock_flags = (i == frame_cur) ? 0 : D3DLOCK_READONLY;

        afxNat rowLen;
        afxRasterRegion rgn;
        rgn.lodIdx = 0;
        rgn.baseLayer = 0;
        rgn.layerCnt = 1;
        rgn.offset[0] = 0;
        rgn.offset[1] = 0;
        rgn.offset[2] = 0;

        for (afxNat j = 0; j < sizeof(rsrc->surfaces) / sizeof(rsrc->surfaces[0]); j++)
        {
            if (bnk->set.rsrc[i].surfaces[j])
                AfxGetRasterExtent(bnk->set.rsrc[i].surfaces[j], 0, rgn.whd);

            if (j == 0)
            {
                if ((bnk->set.bink_buffers.Frames[i].YPlane.Buffer = AfxOpenRasterRegion(bnk->set.rsrc[i].surfaces[j], &rgn, afxRasterAccess_W, NIL, &rowLen))) // = lr.pBits;
                    bnk->set.bink_buffers.Frames[i].YPlane.BufferPitch = rowLen;// = lr.Pitch;
            }
            else if (j == 1)
            {
                if ((bnk->set.bink_buffers.Frames[i].cRPlane.Buffer = AfxOpenRasterRegion(bnk->set.rsrc[i].surfaces[j], &rgn, afxRasterAccess_W, NIL, &rowLen))) // = lr.pBits;
                    bnk->set.bink_buffers.Frames[i].cRPlane.BufferPitch = rowLen;// = lr.Pitch;
            }
            else if (j == 2)
            {
                if ((bnk->set.bink_buffers.Frames[i].cBPlane.Buffer = AfxOpenRasterRegion(bnk->set.rsrc[i].surfaces[j], &rgn, afxRasterAccess_W, NIL, &rowLen))) // = lr.pBits;
                    bnk->set.bink_buffers.Frames[i].cBPlane.BufferPitch = rowLen;// = lr.Pitch;
            }
            else if (j == 3 && bnk->set.rsrc[i].surfaces[j])
            {
                if ((bnk->set.bink_buffers.Frames[i].APlane.Buffer = AfxOpenRasterRegion(bnk->set.rsrc[i].surfaces[j], &rgn, afxRasterAccess_W, NIL, &rowLen))) // = lr.pBits;
                    bnk->set.bink_buffers.Frames[i].APlane.BufferPitch = rowLen; // = lr.Pitch;
            }

        }
        //break;//hack
    }
}

void UnlockBinkTextures(afxBinkVideo *bnk)
{
    for (afxInt i = 0; i < bnk->set.bink_buffers.TotalFrames; ++i)
    {
        //i = (bnk->set.bink_buffers.FrameNum + 1) % bnk->set.bink_buffers.TotalFrames; // hack
        BINKFRAMETEXTURES *rsrc = &bnk->set.rsrc[i];

        for (afxNat j = 0; j < sizeof(rsrc->surfaces) / sizeof(rsrc->surfaces[0]); j++)
        {
            if (bnk->set.rsrc[i].surfaces[j])
            {
                afxRasterRegion rgn;
                rgn.lodIdx = 0;
                rgn.baseLayer = 0;
                rgn.layerCnt = 1;
                rgn.offset[0] = 0;
                rgn.offset[1] = 0;
                rgn.offset[2] = 0;
                AfxGetRasterExtent(bnk->set.rsrc[i].surfaces[j], 0, rgn.whd);
                AfxCloseRasterRegion(bnk->set.rsrc[i].surfaces[j], &rgn);
            }

            if (j == 0)
                bnk->set.bink_buffers.Frames[i].YPlane.Buffer = NIL;
            else if (j == 1)
                bnk->set.bink_buffers.Frames[i].cRPlane.Buffer = NIL;
            else if (j == 2)
                bnk->set.bink_buffers.Frames[i].cBPlane.Buffer = NIL;
            else if (j == 3)
                bnk->set.bink_buffers.Frames[i].APlane.Buffer = NIL;
        }
        //break; // hack
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
    //AvxCmdBindLegos(dscr, 0, 1, &(bnk->set.rsrc[bnk->set.bink_buffers.FrameNum].lego));
    afxSampler samplers[] = { bnk->smp, bnk->smp, bnk->smp, bnk->smp };
    AfxCmdBindRasters(dscr, 0, 0, 3, samplers, bnk->set.rsrc[bnk->set.bink_buffers.FrameNum].surfaces);

    AfxCmdDraw(dscr, 4, 1, 0, 0); // tristripped quad in shader

    End_timer(bnk->Render_microseconds);
    return err;
}

static void DecompressFrame(afxBinkVideo *bnk)
{
    Start_timer();

    //bnk->set.bink_buffers.FrameNum = outBufIdx;

    // Lock the textures.
    LockBinkTextures(bnk);

    End_and_start_next_timer(bnk->Render_microseconds);

    // Decompress a frame
    bink19c.BinkDoFrame(bnk->bik);

    // if we are falling behind, decompress an extra frame to catch up
    while (bink19c.BinkShouldSkip(bnk->bik))
    {
        bink19c.BinkNextFrame(bnk->bik);
        bink19c.BinkDoFrame(bnk->bik);
    }

    End_and_start_next_timer(bnk->Bink_microseconds);

    // Unlock the textures.
    UnlockBinkTextures(bnk);

    End_and_start_next_timer(bnk->Render_microseconds);

    // Keep playing the movie.
    bink19c.BinkNextFrame(bnk->bik);

    End_timer(bnk->Bink_microseconds);
}

_AFXEXPORT afxError AfxBinkDoFrame(afxBinkVideo *bnk, afxBool copyAll, afxBool neverSkip)
{
    (void)copyAll;
    (void)neverSkip;
    afxError err = AFX_ERR_NONE;

    // Is it time for a new Bink frame (we play at the speed of the background vid in this example)

    if (!bink19c.BinkWait(bnk->bik))
    {
        DecompressFrame(bnk);
    }
    return err;
}

_AFXEXPORT afxError AfxBinkClose(afxBinkVideo *bnk)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(bink19c.BinkClose);

    if (bnk->bik)
        bink19c.BinkClose(bnk->bik);

    bnk->bik = NIL;
    FreeBinkTextures(&bnk->set);

    return err;
}

_AFXEXPORT afxError AfxBinkOpen(afxBinkVideo *bnk, afxUri const *uri)
{
    afxError err = AFX_ERR_NONE;
    afxFixedUri2048 uri2;
    AfxMakeFixedUri2048(&uri2, NIL);

    AfxAssert(!bnk->bik);

    AfxResolveUri(AFX_FILE_FLAG_R, uri, &uri2.uri);

    bink19c.BinkSetSoundSystem(bink19c.BinkOpenDirectSound, (UINTa)0);

    HBINK bik;
    AfxAssert(bink19c.BinkOpen);
    bnk->bik = (bik = bink19c.BinkOpen((void*)uri2.buf, BINKNOFRAMEBUFFERS));
    AfxAssert(bik);

    bnk->whd[0] = bik->Width;
    bnk->whd[1] = bik->Height;
    bnk->whd[2] = 1;

    AfxZero(1, sizeof(bnk->set), &bnk->set);
    bink19c.BinkGetFrameBuffersInfo(bnk->bik, &bnk->set.bink_buffers);
    CreateBinkTextures(bnk);
    // Register our locked texture pointers with Bink
    bink19c.BinkRegisterFrameBuffers(bnk->bik, &bnk->set.bink_buffers);
    
    bnk->Last_timer = 0;

    return err;
}

_AFXEXPORT afxError AfxBinkDrop(afxBinkVideo *bnk)
{
    afxError err = AFX_ERR_NONE;
    AfxBinkClose(bnk);
    AfxReleaseObjects(1, (void*[]) { bnk->binkw32 });

    if (bnk->smp)
        AfxReleaseObjects(1, (void*[]) { bnk->smp });

    if (bnk->yv12ToRgbaPip)
        AfxReleaseObjects(1, (void*[]) { bnk->yv12ToRgbaPip });

    //AfxReleaseObject(&bnk);
    return err;
}

_AFXEXPORT afxError AfxBinkDeploy(afxBinkVideo *bnk, afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    bnk->running = FALSE;

    afxUri uri;
#ifdef _WIN64
    AfxMakeUri(&uri, "binkw64.dll", 0);
#else
    AfxMakeUri(&uri, "binkw32.dll", 0);
#endif

    if (AfxAcquireModules(1, &uri, &bnk->binkw32)) AfxThrowError();
    else
    {
#ifdef _WIN64
        AfxFindModuleSymbols(bnk->binkw32, (sizeof(bink19cSymNamesCdecl) / sizeof(bink19cSymNamesCdecl[0])), bink19cSymNamesCdecl, bink19c.v);
#else
        AfxFindModuleSymbols(bnk->binkw32, (sizeof(bink19cSymNamesStdcall) / sizeof(bink19cSymNamesStdcall[0])), bink19cSymNamesStdcall, bink19c.v);
#endif
        bink19c.BinkSetSoundSystem(bink19c.BinkOpenMiles, (UINTa)0);
    }
    
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

    AfxAcquireSamplers(bnk->dctx, 1, &smpSpec, &bnk->smp);
    AfxAssertObjects(1, &bnk->smp, afxFcc_SAMP);

    AfxMakeUri(&uri, "data/pipeline/yv12ToRgba.xsh.xml?yFlipped", 0);
    bnk->yv12ToRgbaPip = AfxLoadPipelineFromXsh(dctx, &uri);
    AfxAssert(bnk->yv12ToRgbaPip);

    return err;
}

