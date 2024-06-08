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

#ifndef AFX_BINK_PROXY_H
#define AFX_BINK_PROXY_H

#include "qwadro/ux/afxUxDefs.h"
#include "qwadro/draw/io/afxRaster.h"
#include "qwadro/draw/io/afxBuffer.h"
#include "qwadro/core/afxSystem.h"

#ifdef _AFX_BINK_VIDEO_C
#define _AVXBINK DLLEXPORT
#else
#define _AVXBINK DLLIMPORT
#endif

#define AVXBINK _AVXBINK

AFX_DEFINE_UNION(avxBinkVmt)
{
    struct
    {
        void(__stdcall* BinkBufferBlit)(void*/*HBINKBUFFER*/buf, void*/*BINKRECT**/rects, afxNat numrects);
        void(__stdcall* BinkBufferCheckWinPos)(void*/*HBINKBUFFER*/buf, afxInt * NewWindowX, afxInt * NewWindowY);
        afxInt(__stdcall* BinkBufferClear)(void*/*HBINKBUFFER*/buf, afxNat RGB);
        void(__stdcall* BinkBufferClose)(void*/*HBINKBUFFER*/buf);
        char * (__stdcall* BinkBufferGetDescription)(void*/*HBINKBUFFER*/buf);
        char * (__stdcall* BinkBufferGetError)(void);
        afxInt(__stdcall* BinkBufferLock)(void*/*HBINKBUFFER*/buf);
        void*/*HBINKBUFFER*/(__stdcall* BinkBufferOpen)(void* /*HWND*/ wnd, afxNat width, afxNat height, afxNat bufferflags);
        afxInt(__stdcall* BinkBufferSetDirectDraw)(void * lpDirectDraw, void * lpPrimary);
        afxInt(__stdcall* BinkBufferSetHWND)(void*/*HBINKBUFFER*/buf, void* /*HWND*/ newwnd);
        afxInt(__stdcall* BinkBufferSetOffset)(void*/*HBINKBUFFER*/buf, afxInt destx, afxInt desty);
        void(__stdcall* BinkBufferSetResolution)(afxInt w, afxInt h, afxInt bits);
        afxInt(__stdcall* BinkBufferSetScale)(void*/*HBINKBUFFER*/buf, afxNat w, afxNat h);
        afxInt(__stdcall* BinkBufferUnlock)(void*/*HBINKBUFFER*/buf);
        afxInt(__stdcall* BinkCheckCursor)(void* /*HWND*/ wnd, afxInt x, afxInt y, afxInt w, afxInt h);
        void(__stdcall* BinkClose)(void*/*HBINK*/bnk);
        void(__stdcall* BinkCloseTrack)(void*/*HBINKTRACK*/bnkt);
        afxInt(__stdcall* BinkControlBackgroundIO)(void*/*HBINK*/bink, afxNat control);
        afxInt(__stdcall* BinkControlPlatformFeatures)(afxInt use, afxInt dont_use);
        afxInt(__stdcall* BinkCopyToBuffer)(void*/*HBINK*/bnk, void* dest, afxInt destpitch, afxNat destheight, afxNat destx, afxNat desty, afxNat flags);
        afxInt(__stdcall* BinkCopyToBufferRect)(void*/*HBINK*/bnk, void* dest, afxInt destpitch, afxNat destheight, afxNat destx, afxNat desty, afxNat srcx, afxNat srcy, afxNat srcw, afxNat srch, afxNat flags);
        afxInt(__stdcall* BinkDDSurfaceType)(void * lpDDS);
        afxInt(__stdcall* BinkDX8SurfaceType)(void* lpD3Ds);
        afxInt(__stdcall* BinkDX9SurfaceType)(void* lpD3Ds);
        afxInt(__stdcall* BinkDoFrame)(void*/*HBINK*/bnk);
        afxInt(__stdcall* BinkDoFrameAsync)(void*/*HBINK*/bink, afxNat yplane_thread_num, afxNat other_work_thread_num);
        afxInt(__stdcall* BinkDoFrameAsyncWait)(void*/*HBINK*/bink, afxInt us);
        char * (__stdcall* BinkGetError)(void);
        void(__stdcall* BinkGetFrameBuffersInfo)(void*/*HBINK*/bink, void*/*BINKFRAMEBUFFERS**/flego);
        afxNat(__stdcall* BinkGetKeyFrame)(void*/*HBINK*/bnk, afxNat frame, afxInt flags);
        void(__stdcall* BinkGetPalette)(void * out_pal);
        void(__stdcall* BinkGetRealtime)(void*/*HBINK*/bink, void*/*BINKREALTIME**/run, afxNat frames);
        afxInt(__stdcall* BinkGetRects)(void*/*HBINK*/bnk, afxNat flags);
        void(__stdcall* BinkGetSummary)(void*/*HBINK*/bnk, void*/*BINKSUMMARY**/sum);
        afxNat(__stdcall* BinkGetTrackData)(void*/*HBINKTRACK*/bnkt, void * dest);
        afxNat(__stdcall* BinkGetTrackID)(void*/*HBINK*/bnk, afxNat trackindex);
        afxNat(__stdcall* BinkGetTrackMaxSize)(void*/*HBINK*/bnk, afxNat trackindex);
        afxNat(__stdcall* BinkGetTrackType)(void*/*HBINK*/bnk, afxNat trackindex);
        void(__stdcall* BinkGoto)(void*/*HBINK*/bnk, afxNat frame, afxInt flags);  // use 1 for the first frame
        afxInt(__stdcall* BinkIsSoftwareCursor)(void * lpDDSP, void* /*HCURSOR*/ cur);
        void * (__stdcall* BinkLogoAddress)(void);
        void(__stdcall* BinkNextFrame)(void*/*HBINK*/bnk);
        void*/*HBINK*/(__stdcall* BinkOpen)(const char * name, afxNat flags);
        void*/*BINKSNDOPEN*/(__stdcall* BinkOpenDirectSound)(afxSize param); // don't call directly
        void*/*BINKSNDOPEN*/(__stdcall* BinkOpenMiles)(afxSize param); // don't call directly
        void*/*HBINKTRACK*/(__stdcall* BinkOpenTrack)(void*/*HBINK*/bnk, afxNat trackindex);
        void*/*BINKSNDOPEN*/(__stdcall* BinkOpenWaveOut)(afxSize param); // don't call directly
        afxInt(__stdcall* BinkPause)(void*/*HBINK*/bnk, afxInt pause);
        void(__stdcall* BinkRegisterFrameBuffers)(void*/*HBINK*/bink, void*/*BINKFRAMEBUFFERS**/flego);
        afxInt(__stdcall* BinkRequestStopAsyncThread)(afxInt thread_num);
        void(__stdcall* BinkRestoreCursor)(afxInt checkcount);
        void(__stdcall* BinkService)(void*/*HBINK*/bink);
        void(__stdcall* BinkSetError)(const char * err);
        void(__stdcall* BinkSetFrameRate)(afxNat forcerate, afxNat forceratediv);
        void(__stdcall* BinkSetIO)(void*/*BINKIOOPEN*/io);
        void(__stdcall* BinkSetIOSize)(afxNat iosize);
        void(__stdcall* BinkSetMemory)(void*/*BINKMEMALLOC*/a, void*/*BINKMEMFREE*/f);
        void(__stdcall* BinkSetMixBinVolumes)(void*/*HBINK*/bnk, afxNat trackid, afxNat * vol_mix_bins, afxInt * volumes, afxNat total);
        void(__stdcall* BinkSetMixBins)(void*/*HBINK*/bnk, afxNat trackid, afxNat * mix_bins, afxNat total);
        void(__stdcall* BinkSetPan)(void*/*HBINK*/bnk, afxNat trackid, afxInt pan);
        void(__stdcall* BinkSetSimulate)(afxNat sim);
        afxInt(__stdcall* BinkSetSoundOnOff)(void*/*HBINK*/bnk, afxInt onoff);
        afxInt(__stdcall* BinkSetSoundSystem)(void*/*BINKSNDSYSOPEN*/open, afxSize param);
        void(__stdcall* BinkSetSoundTrack)(afxNat total_tracks, afxNat * tracks);
        afxInt(__stdcall* BinkSetVideoOnOff)(void*/*HBINK*/bnk, afxInt onoff);
        void(__stdcall* BinkSetVolume)(void*/*HBINK*/bnk, afxNat trackid, afxInt volume);
        afxInt(__stdcall* BinkShouldSkip)(void*/*HBINK*/bink);
        afxInt(__stdcall* BinkStartAsyncThread)(afxInt thread_num, void const * param);
        afxInt(__stdcall* BinkWait)(void*/*HBINK*/bnk);
        afxInt(__stdcall* BinkWaitStopAsyncThread)(afxInt thread_num);
    };
    void *v[1];
};

extern avxBinkVmt symVmt;


AVXBINK void* __stdcall BinkLogoAddress(void);

AVXBINK void __stdcall BinkSetError(const char* err);
AVXBINK char* __stdcall BinkGetError(void);

AVXBINK void*/*HBINK*/ __stdcall BinkOpen(const char* name, afxNat flags);
AVXBINK void __stdcall BinkClose(void*/*HBINK*/bnk);

AVXBINK void __stdcall BinkGetFrameBuffersInfo(void*/*HBINK*/bink, void*/*BINKFRAMEBUFFERS**/fbset);
AVXBINK void __stdcall BinkRegisterFrameBuffers(void*/*HBINK*/bink, void*/*BINKFRAMEBUFFERS**/fbset);

AVXBINK afxInt  __stdcall BinkDoFrame(void*/*HBINK*/bnk);
AVXBINK void __stdcall BinkNextFrame(void*/*HBINK*/bnk);
AVXBINK afxInt  __stdcall BinkWait(void*/*HBINK*/bnk);
AVXBINK afxInt  __stdcall BinkPause(void*/*HBINK*/bnk, afxInt pause);
AVXBINK void __stdcall BinkGoto(void*/*HBINK*/bnk, afxNat frame, afxInt flags);
AVXBINK afxNat  __stdcall BinkGetKeyFrame(void*/*HBINK*/bnk, afxNat frame, afxInt flags);
AVXBINK afxInt  __stdcall BinkShouldSkip(void*/*HBINK*/bink);

AVXBINK afxInt  __stdcall BinkCopyToBuffer(void*/*HBINK*/bnk, void* dest, afxInt destpitch, afxNat destheight, afxNat destx, afxNat desty, afxNat flags);
AVXBINK afxInt  __stdcall BinkCopyToBufferRect(void*/*HBINK*/bnk, void* dest, afxInt destpitch, afxNat destheight, afxNat destx, afxNat desty, afxNat srcx, afxNat srcy, afxNat srcw, afxNat srch, afxNat flags);
AVXBINK afxInt __stdcall BinkGetRects(void*/*HBINK*/bnk, afxNat flags);

AVXBINK afxInt  __stdcall BinkSetVideoOnOff(void*/*HBINK*/bnk, afxInt onoff);
AVXBINK afxInt  __stdcall BinkSetSoundOnOff(void*/*HBINK*/bnk, afxInt onoff);

AVXBINK void __stdcall BinkSetVolume(void*/*HBINK*/bnk, afxNat trackid, afxInt volume);

AVXBINK void __stdcall BinkSetPan(void*/*HBINK*/bnk, afxNat trackid, afxInt pan);

AVXBINK void __stdcall BinkSetMixBins(void*/*HBINK*/bnk, afxNat trackid, afxNat * mix_bins, afxNat total);
AVXBINK void __stdcall BinkSetMixBinVolumes(void*/*HBINK*/bnk, afxNat trackid, afxNat * vol_mix_bins, afxInt * volumes, afxNat total);

AVXBINK void __stdcall BinkService(void*/*HBINK*/bink);


AVXBINK void __stdcall BinkGetPalette(void * out_pal);

AVXBINK afxInt  __stdcall BinkControlBackgroundIO(void*/*HBINK*/bink, afxNat control);

AVXBINK afxInt __stdcall BinkStartAsyncThread(afxInt thread_num, void const * param);

AVXBINK afxInt  __stdcall BinkDoFrameAsync(void*/*HBINK*/bink, afxNat yplane_thread_num, afxNat other_work_thread_num);
AVXBINK afxInt  __stdcall BinkDoFrameAsyncWait(void*/*HBINK*/bink, afxInt us);

AVXBINK afxInt __stdcall BinkRequestStopAsyncThread(afxInt thread_num);
AVXBINK afxInt __stdcall BinkWaitStopAsyncThread(afxInt thread_num);

AVXBINK void*/*HBINKTRACK*/ __stdcall BinkOpenTrack(void*/*HBINK*/bnk, afxNat trackindex);
AVXBINK void __stdcall BinkCloseTrack(void*/*HBINKTRACK*/bnkt);

AVXBINK afxNat  __stdcall BinkGetTrackData(void*/*HBINKTRACK*/bnkt, void * dest);

AVXBINK afxNat  __stdcall BinkGetTrackType(void*/*HBINK*/bnk, afxNat trackindex);

AVXBINK afxNat  __stdcall BinkGetTrackMaxSize(void*/*HBINK*/bnk, afxNat trackindex);

AVXBINK afxNat  __stdcall BinkGetTrackID(void*/*HBINK*/bnk, afxNat trackindex);

AVXBINK void __stdcall BinkGetSummary(void*/*HBINK*/bnk, void*/*BINKSUMMARY **/sum);

AVXBINK void __stdcall BinkGetRealtime(void*/*HBINK*/bink, void*/*BINKREALTIME **/run, afxNat frames);

AVXBINK void __stdcall BinkSetSoundTrack(afxNat total_tracks, afxNat * tracks);

AVXBINK void __stdcall BinkSetIO(void*/*BINKIOOPEN*/io);

AVXBINK void __stdcall BinkSetFrameRate(afxNat forcerate, afxNat forceratediv);

AVXBINK void __stdcall BinkSetSimulate(afxNat sim);

AVXBINK void __stdcall BinkSetIOSize(afxNat iosize);

AVXBINK afxInt  __stdcall BinkSetSoundSystem(void*/*BINKSNDSYSOPEN*/open, afxSize param);

AVXBINK void*/*HBINKBUFFER*/ __stdcall BinkBufferOpen(void*/*HWND*/wnd, afxNat width, afxNat height, afxNat bufferflags);

AVXBINK afxInt __stdcall BinkBufferSetHWND(void*/*HBINKBUFFER*/buf, void*/*HWND*/newwnd);

AVXBINK afxInt __stdcall BinkDDSurfaceType(void * lpDDS);

AVXBINK afxInt __stdcall BinkIsSoftwareCursor(void * lpDDSP, void*/*HCURSOR*/cur);

AVXBINK afxInt __stdcall BinkCheckCursor(void* /*HWND*/ wnd, afxInt x, afxInt y, afxInt w, afxInt h);

AVXBINK afxInt __stdcall BinkBufferSetDirectDraw(void * lpDirectDraw, void * lpPrimary);

AVXBINK void __stdcall BinkBufferClose(void*/*HBINKBUFFER*/buf);

AVXBINK afxInt __stdcall BinkBufferLock(void*/*HBINKBUFFER*/buf);
AVXBINK afxInt __stdcall BinkBufferUnlock(void*/*HBINKBUFFER*/buf);

AVXBINK void __stdcall BinkBufferSetResolution(afxInt w, afxInt h, afxInt bits);

AVXBINK void __stdcall BinkBufferCheckWinPos(void*/*HBINKBUFFER*/buf, afxInt * NewWindowX, afxInt * NewWindowY);

AVXBINK afxInt __stdcall BinkBufferSetOffset(void*/*HBINKBUFFER*/buf, afxInt destx, afxInt desty);

AVXBINK void __stdcall BinkBufferBlit(void*/*HBINKBUFFER*/buf, void*/*BINKRECT **/rects, afxNat numrects);

AVXBINK afxInt __stdcall BinkBufferSetScale(void*/*HBINKBUFFER*/buf, afxNat w, afxNat h);

AVXBINK char * __stdcall BinkBufferGetDescription(void*/*HBINKBUFFER*/buf);

AVXBINK char * __stdcall BinkBufferGetError(void);

AVXBINK void __stdcall BinkRestoreCursor(afxInt checkcount);

AVXBINK afxInt __stdcall BinkBufferClear(void*/*HBINKBUFFER*/buf, afxNat RGB);

AVXBINK afxInt __stdcall BinkDX8SurfaceType(void* lpD3Ds);

AVXBINK afxInt __stdcall BinkDX9SurfaceType(void* lpD3Ds);

AVXBINK void*/*BINKSNDOPEN*/ __stdcall BinkOpenMiles(afxSize param);

AVXBINK void*/*BINKSNDOPEN*/ __stdcall BinkOpenWaveOut(afxSize param);

AVXBINK void*/*BINKSNDOPEN*/ __stdcall BinkOpenDirectSound(afxSize param);

AVXBINK void __stdcall BinkSetMemory(void*/*BINKMEMALLOC*/a, void*/*BINKMEMFREE*/f);

AVXBINK afxInt __stdcall BinkControlPlatformFeatures(afxInt use, afxInt dont_use);


#endif//AFX_BINK_PROXY_H
