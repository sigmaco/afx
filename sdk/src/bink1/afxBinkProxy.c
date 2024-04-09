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

#define _AFX_BINK_VIDEO_C
#include "afxBinkVideo.h"
#include "afxBinkProxy.h"
#include "qwadro/core/afxSystem.h"
#include "qwadro/draw/dev/afxDrawStream.h"
#include "qwadro/draw/pipe/afxDrawOps.h"

static afxBool bootstrapped = FALSE;
static afxModule binkw32 = NIL;

static afxString const symNames[] =
{
    AFX_STRING("BinkBufferBlit"),
    AFX_STRING("BinkBufferCheckWinPos"),
    AFX_STRING("BinkBufferClear"),
    AFX_STRING("BinkBufferClose"),
    AFX_STRING("BinkBufferGetDescription"),
    AFX_STRING("BinkBufferGetError"),
    AFX_STRING("BinkBufferLock"),
    AFX_STRING("BinkBufferOpen"),
    AFX_STRING("BinkBufferSetDirectDraw"),
    AFX_STRING("BinkBufferSetHWND"),
    AFX_STRING("BinkBufferSetOffset"),
    AFX_STRING("BinkBufferSetResolution"),
    AFX_STRING("BinkBufferSetScale"),
    AFX_STRING("BinkBufferUnlock"),
    AFX_STRING("BinkCheckCursor"),
    AFX_STRING("BinkClose"),
    AFX_STRING("BinkCloseTrack"),
    AFX_STRING("BinkControlBackgroundIO"),
    AFX_STRING("BinkControlPlatformFeatures"),
    AFX_STRING("BinkCopyToBuffer"),
    AFX_STRING("BinkCopyToBufferRect"),
    AFX_STRING("BinkDDSurfaceType"),
    AFX_STRING("BinkDX8SurfaceType"),
    AFX_STRING("BinkDX9SurfaceType"),
    AFX_STRING("BinkDoFrame"),
    AFX_STRING("BinkDoFrameAsync"),
    AFX_STRING("BinkDoFrameAsyncWait"),
    AFX_STRING("BinkGetError"),
    AFX_STRING("BinkGetFrameBuffersInfo"),
    AFX_STRING("BinkGetKeyFrame"),
    AFX_STRING("BinkGetPalette"),
    AFX_STRING("BinkGetRealtime"),
    AFX_STRING("BinkGetRects"),
    AFX_STRING("BinkGetSummary"),
    AFX_STRING("BinkGetTrackData"),
    AFX_STRING("BinkGetTrackID"),
    AFX_STRING("BinkGetTrackMaxSize"),
    AFX_STRING("BinkGetTrackType"),
    AFX_STRING("BinkGoto"),
    AFX_STRING("BinkIsSoftwareCursor"),
    AFX_STRING("BinkLogoAddress"),
    AFX_STRING("BinkNextFrame"),
    AFX_STRING("BinkOpen"),
    AFX_STRING("BinkOpenDirectSound"),
    AFX_STRING("BinkOpenMiles"),
    AFX_STRING("BinkOpenTrack"),
    AFX_STRING("BinkOpenWaveOut"),
    AFX_STRING("BinkPause"),
    AFX_STRING("BinkRegisterFrameBuffers"),
    AFX_STRING("BinkRequestStopAsyncThread"),
    AFX_STRING("BinkRestoreCursor"),
    AFX_STRING("BinkService"),
    AFX_STRING("BinkSetError"),
    AFX_STRING("BinkSetFrameRate"),
    AFX_STRING("BinkSetIO"),
    AFX_STRING("BinkSetIOSize"),
    AFX_STRING("BinkSetMemory"),
    AFX_STRING("BinkSetMixBinVolumes"),
    AFX_STRING("BinkSetMixBins"),
    AFX_STRING("BinkSetPan"),
    AFX_STRING("BinkSetSimulate"),
    AFX_STRING("BinkSetSoundOnOff"),
    AFX_STRING("BinkSetSoundSystem"),
    AFX_STRING("BinkSetSoundTrack"),
    AFX_STRING("BinkSetVideoOnOff"),
    AFX_STRING("BinkSetVolume"),
    AFX_STRING("BinkShouldSkip"),
    AFX_STRING("BinkStartAsyncThread"),
    AFX_STRING("BinkWait"),
    AFX_STRING("BinkWaitStopAsyncThread"),
    AFX_STRING("RADTimerRead")
};

avxBinkVmt symVmt;

static void LoadLibNow(void)
{
    afxError err = NIL;

    if (!binkw32)
    {
        afxSystem system;

        if (!AfxGetSystem(&system))
        {
            if (AfxDoSystemBootUp(NIL))
                AfxThrowError();
            else
            {
                bootstrapped = TRUE;
            }
        }

        afxUri uri;
#ifdef AFX_PLATFORM_WIN
#ifdef AFX_PLATFORM_W64
        AfxMakeUri(&uri, "system/binkw64.dl_", 0);
#else
        AfxMakeUri(&uri, "system/binkw32.dl_", 0);
#endif
#endif

        if (AfxLoadModule(&uri, NIL, &binkw32)) AfxThrowError();
        else
        {
            afxNat foundCnt = AfxFindSymbolAddresses(binkw32, AFX_COUNTOF(symNames), symNames, symVmt.v);
        }
    }
}

_AVXBINK void* BinkLogoAddress(void)
{
    LoadLibNow();
    return symVmt.BinkLogoAddress();
}

_AVXBINK void BinkSetError(const char* err)
{
    symVmt.BinkSetError(err);
}

_AVXBINK char* BinkGetError(void)
{
    return symVmt.BinkGetError();
}

_AVXBINK void*/*HBINK*/ BinkOpen(const char* name, afxNat flags)
{
    LoadLibNow();
    return symVmt.BinkOpen(name, flags);
}

_AVXBINK void BinkGetFrameBuffersInfo(void*/*HBINK*/bink, void*/*BINKFRAMEBUFFERS**/fbset)
{
    symVmt.BinkGetFrameBuffersInfo(bink, fbset);
}

_AVXBINK void BinkRegisterFrameBuffers(void*/*HBINK*/bink, void*/*BINKFRAMEBUFFERS**/fbset)
{
    symVmt.BinkRegisterFrameBuffers(bink, fbset);
}

_AVXBINK afxInt  BinkDoFrame(void*/*HBINK*/bnk)
{
    return symVmt.BinkDoFrame(bnk);
}

_AVXBINK void BinkNextFrame(void*/*HBINK*/bnk)
{
    symVmt.BinkNextFrame(bnk);
}

_AVXBINK afxInt  BinkWait(void*/*HBINK*/bnk)
{
    return symVmt.BinkWait(bnk);
}

_AVXBINK void BinkClose(void*/*HBINK*/bnk)
{
    symVmt.BinkClose(bnk);
}

_AVXBINK afxInt  BinkPause(void*/*HBINK*/bnk, afxInt pause)
{
    return symVmt.BinkPause(bnk, pause);
}

_AVXBINK afxInt  BinkCopyToBuffer(void*/*HBINK*/bnk, void* dest, afxInt destpitch, afxNat destheight, afxNat destx, afxNat desty, afxNat flags)
{
    return symVmt.BinkCopyToBuffer(bnk, dest, destpitch, destheight, destx, desty, flags);
}

_AVXBINK afxInt  BinkCopyToBufferRect(void*/*HBINK*/bnk, void* dest, afxInt destpitch, afxNat destheight, afxNat destx, afxNat desty, afxNat srcx, afxNat srcy, afxNat srcw, afxNat srch, afxNat flags)
{
    return symVmt.BinkCopyToBufferRect(bnk, dest, destpitch, destheight, destx, desty, srcx, srcy, srcw, srch, flags);
}

_AVXBINK afxInt BinkGetRects(void*/*HBINK*/bnk, afxNat flags)
{
    return symVmt.BinkGetRects(bnk, flags);
}

_AVXBINK void BinkGoto(void*/*HBINK*/bnk, afxNat frame, afxInt flags)
{
    symVmt.BinkGoto(bnk, frame, flags);
}

_AVXBINK afxNat  BinkGetKeyFrame(void*/*HBINK*/bnk, afxNat frame, afxInt flags)
{
    return symVmt.BinkGetKeyFrame(bnk, frame, flags);
}

_AVXBINK afxInt  BinkSetVideoOnOff(void*/*HBINK*/bnk, afxInt onoff)
{
    return symVmt.BinkSetVideoOnOff(bnk, onoff);
}

_AVXBINK afxInt  BinkSetSoundOnOff(void*/*HBINK*/bnk, afxInt onoff)
{
    return symVmt.BinkSetSoundOnOff(bnk, onoff);
}

_AVXBINK void BinkSetVolume(void*/*HBINK*/bnk, afxNat trackid, afxInt volume)
{
    symVmt.BinkSetVolume(bnk, trackid, volume);
}

_AVXBINK void BinkSetPan(void*/*HBINK*/bnk, afxNat trackid, afxInt pan)
{
    symVmt.BinkSetPan(bnk, trackid, pan);
}

_AVXBINK void BinkSetMixBins(void*/*HBINK*/bnk, afxNat trackid, afxNat* mix_bins, afxNat total)
{
    symVmt.BinkSetMixBins(bnk, trackid, mix_bins, total);
}

_AVXBINK void BinkSetMixBinVolumes(void*/*HBINK*/bnk, afxNat trackid, afxNat* vol_mix_bins, afxInt* volumes, afxNat total)
{
    symVmt.BinkSetMixBinVolumes(bnk, trackid, vol_mix_bins, volumes, total);
}

_AVXBINK void BinkService(void*/*HBINK*/bink)
{
    symVmt.BinkService(bink);
}

_AVXBINK afxInt  BinkShouldSkip(void*/*HBINK*/bink)
{
    return symVmt.BinkShouldSkip(bink);
}

_AVXBINK void BinkGetPalette(void * out_pal)
{
    symVmt.BinkGetPalette(out_pal);
}

_AVXBINK afxInt  BinkControlBackgroundIO(void*/*HBINK*/bink, afxNat control)
{
    return symVmt.BinkControlBackgroundIO(bink, control);
}

_AVXBINK afxInt BinkStartAsyncThread(afxInt thread_num, void const * param)
{
    return symVmt.BinkStartAsyncThread(thread_num, param);
}

_AVXBINK afxInt  BinkDoFrameAsync(void*/*HBINK*/bink, afxNat yplane_thread_num, afxNat other_work_thread_num)
{
    return symVmt.BinkDoFrameAsync(bink, yplane_thread_num, other_work_thread_num);
}

_AVXBINK afxInt  BinkDoFrameAsyncWait(void*/*HBINK*/bink, afxInt us)
{
    return symVmt.BinkDoFrameAsyncWait(bink, us);
}

_AVXBINK afxInt BinkRequestStopAsyncThread(afxInt thread_num)
{
    return symVmt.BinkRequestStopAsyncThread(thread_num);
}

_AVXBINK afxInt BinkWaitStopAsyncThread(afxInt thread_num)
{
    return symVmt.BinkWaitStopAsyncThread(thread_num);
}

_AVXBINK void*/*HBINKTRACK*/ BinkOpenTrack(void*/*HBINK*/bnk, afxNat trackindex)
{
    return symVmt.BinkOpenTrack(bnk, trackindex);
}

_AVXBINK void BinkCloseTrack(void*/*HBINKTRACK*/bnkt)
{
    symVmt.BinkCloseTrack(bnkt);
}

_AVXBINK afxNat  BinkGetTrackData(void*/*HBINKTRACK*/bnkt, void* dest)
{
    return symVmt.BinkGetTrackData(bnkt, dest);
}

_AVXBINK afxNat  BinkGetTrackType(void*/*HBINK*/bnk, afxNat trackindex)
{
    return symVmt.BinkGetTrackType(bnk, trackindex);
}

_AVXBINK afxNat  BinkGetTrackMaxSize(void*/*HBINK*/bnk, afxNat trackindex)
{
    return symVmt.BinkGetTrackMaxSize(bnk, trackindex);
}

_AVXBINK afxNat  BinkGetTrackID(void*/*HBINK*/bnk, afxNat trackindex)
{
    return symVmt.BinkGetTrackID(bnk, trackindex);
}

_AVXBINK void BinkGetSummary(void*/*HBINK*/bnk, void*/*BINKSUMMARY**/sum)
{
    symVmt.BinkGetSummary(bnk, sum);
}

_AVXBINK void BinkGetRealtime(void*/*HBINK*/bink, void*/*BINKREALTIME**/run, afxNat frames)
{
    symVmt.BinkGetRealtime(bink, run, frames);
}

_AVXBINK void BinkSetSoundTrack(afxNat total_tracks, afxNat* tracks)
{
    symVmt.BinkSetSoundTrack(total_tracks, tracks);
}

_AVXBINK void BinkSetIO(void*/*BINKIOOPEN*/io)
{
    LoadLibNow();
    symVmt.BinkSetIO(io);
}

_AVXBINK void BinkSetFrameRate(afxNat forcerate, afxNat forceratediv)
{
    symVmt.BinkSetFrameRate(forcerate, forceratediv);
}

_AVXBINK void BinkSetSimulate(afxNat sim)
{
    LoadLibNow();
    symVmt.BinkSetSimulate(sim);
}

_AVXBINK void BinkSetIOSize(afxNat iosize)
{
    LoadLibNow();
    symVmt.BinkSetIOSize(iosize);
}

_AVXBINK afxInt  BinkSetSoundSystem(void*/*BINKSNDSYSOPEN*/open, afxSize param)
{
    LoadLibNow();
    return symVmt.BinkSetSoundSystem(open, param);
}

_AVXBINK void*/*HBINKBUFFER*/ BinkBufferOpen(void* /*HWND*/ wnd, afxNat width, afxNat height, afxNat bufferflags)
{
    LoadLibNow();
    return symVmt.BinkBufferOpen(wnd, width, height, bufferflags);
}

_AVXBINK afxInt BinkBufferSetHWND(void*/*HBINKBUFFER*/buf, void* /*HWND*/ newwnd)
{
    return symVmt.BinkBufferSetHWND(buf, newwnd);
}

_AVXBINK afxInt BinkDDSurfaceType(void* lpDDS)
{
    return symVmt.BinkDDSurfaceType(lpDDS);
}

_AVXBINK afxInt BinkIsSoftwareCursor(void* lpDDSP, void* /*HCURSOR*/ cur)
{
    return symVmt.BinkIsSoftwareCursor(lpDDSP, cur);
}

_AVXBINK afxInt BinkCheckCursor(void* /*HWND*/ wnd, afxInt x, afxInt y, afxInt w, afxInt h)
{
    return symVmt.BinkCheckCursor(wnd, x, y, w, h);
}

_AVXBINK afxInt BinkBufferSetDirectDraw(void* lpDirectDraw, void* lpPrimary)
{
    return symVmt.BinkBufferSetDirectDraw(lpDirectDraw, lpPrimary);
}

_AVXBINK void BinkBufferClose(void*/*HBINKBUFFER*/buf)
{
    symVmt.BinkBufferClose(buf);
}

_AVXBINK afxInt BinkBufferLock(void*/*HBINKBUFFER*/buf)
{
    return symVmt.BinkBufferLock(buf);
}

_AVXBINK afxInt BinkBufferUnlock(void*/*HBINKBUFFER*/buf)
{
    return symVmt.BinkBufferUnlock(buf);
}

_AVXBINK void BinkBufferSetResolution(afxInt w, afxInt h, afxInt bits)
{
    symVmt.BinkBufferSetResolution(w, h, bits);
}

_AVXBINK void BinkBufferCheckWinPos(void*/*HBINKBUFFER*/buf, afxInt* NewWindowX, afxInt* NewWindowY)
{
    symVmt.BinkBufferCheckWinPos(buf, NewWindowX, NewWindowY);
}

_AVXBINK afxInt BinkBufferSetOffset(void*/*HBINKBUFFER*/buf, afxInt destx, afxInt desty)
{
    return symVmt.BinkBufferSetOffset(buf, destx, desty);
}

_AVXBINK void BinkBufferBlit(void*/*HBINKBUFFER*/buf, void*/*BINKRECT**/rects, afxNat numrects)
{
    symVmt.BinkBufferBlit(buf, rects, numrects);
}

_AVXBINK afxInt BinkBufferSetScale(void*/*HBINKBUFFER*/buf, afxNat w, afxNat h)
{
    return symVmt.BinkBufferSetScale(buf, w, h);
}

_AVXBINK char* BinkBufferGetDescription(void*/*HBINKBUFFER*/buf)
{
    return symVmt.BinkBufferGetDescription(buf);
}

_AVXBINK char* BinkBufferGetError(void)
{
    return symVmt.BinkBufferGetError();
}

_AVXBINK void BinkRestoreCursor(afxInt checkcount)
{
    return symVmt.BinkRestoreCursor(checkcount);
}

_AVXBINK afxInt BinkBufferClear(void*/*HBINKBUFFER*/buf, afxNat RGB)
{
    return symVmt.BinkBufferClear(buf, RGB);
}

_AVXBINK afxInt BinkDX8SurfaceType(void* lpD3Ds)
{
    LoadLibNow();
    return symVmt.BinkDX8SurfaceType(lpD3Ds);
}

_AVXBINK afxInt BinkDX9SurfaceType(void* lpD3Ds)
{
    LoadLibNow();
    return symVmt.BinkDX9SurfaceType(lpD3Ds);
}

_AVXBINK void*/*BINKSNDOPEN*/ BinkOpenMiles(afxSize param)
{
    LoadLibNow();
    return symVmt.BinkOpenMiles(param);
}

_AVXBINK void*/*BINKSNDOPEN*/ BinkOpenWaveOut(afxSize param)
{
    LoadLibNow();
    return symVmt.BinkOpenWaveOut(param);
}

_AVXBINK void*/*BINKSNDOPEN*/ BinkOpenDirectSound(afxSize param)
{
    LoadLibNow();
    return symVmt.BinkOpenDirectSound(param);
}

_AVXBINK void BinkSetMemory(void*/*BINKMEMALLOC*/a, void*/*BINKMEMFREE*/f)
{
    LoadLibNow();
    symVmt.BinkSetMemory(a, f);
}

_AVXBINK afxInt BinkControlPlatformFeatures(afxInt use, afxInt dont_use)
{
    LoadLibNow();
    return symVmt.BinkControlPlatformFeatures(use, dont_use);
}
