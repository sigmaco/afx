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

  //////////////////////////////////////////////////////////////////////////////
 // The Qwadro Video Graphics Infrastructure                                 //
//////////////////////////////////////////////////////////////////////////////

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#ifndef AVX_DEVICE_IMPL_KIT_H
#define AVX_DEVICE_IMPL_KIT_H

#include "../dev/afxExecImplKit.h"
#include "qwadro/inc/draw/afxDrawSystem.h"
#include "qwadro/inc/mem/afxInterlockedQueue.h"
#include "qwadro/inc/mem/afxSlabAllocator.h"

typedef enum _ddevReqCode
{
    avxDrawDeviceReqCode_INIT,
    avxDrawDeviceReqCode_TERM,
    avxDrawDeviceReqCode_START,
    avxDrawDeviceReqCode_STOP,
}
_ddevReqCode;

AFX_DEFINE_STRUCT(afxDrawDeviceInfo)
{
    afxDeviceInfo           dev;
    afxDrawLimits           limits;
    afxDrawFeatures         features;
    afxNat                  portCnt;
    afxDrawPortCaps const*  portCaps;
    avxClipSpace            clipSpace;
    afxClassConfig const*   dthrClsCfg;
    afxClassConfig const*   dqueClsCfg;
    afxClassConfig const*   ddgeClsCfg;
    afxClassConfig const*   dctxClsCfg;
    afxClassConfig const*   doutClsCfg;
    afxClassConfig const*   dinClsCfg;

    afxError(*relinkDoutCb)(afxDrawDevice, afxDrawContext, afxNat, afxDrawOutput[]);
    afxError(*relinkDinCb)(afxDrawDevice, afxDrawContext, afxNat, afxDrawInput[]);
};

#ifdef _AVX_DRAW_DEVICE_C
#ifndef _AFX_DEVICE_C
#   error "Require afxDevice implementation"
#endif
AFX_OBJECT(afxDrawDevice)
{
    AFX_OBJ(afxDevice)  dev;
    afxDrawLimits       limits;
    afxDrawFeatures     features;
    afxNat              portCnt;
    afxDrawPortCaps*    portCaps;
    avxClipSpace        clipSpace;

    afxClass            dthrCls;
    afxClass            dqueCls;
    afxClass            dexuCls;
    afxClass            dctxCls;
    afxClass            doutCls;
    afxClass            dinCls;

    afxChain            ineps;
    afxBool             relinking;
    afxCondition        relinkedCnd;
    afxMutex            relinkedCndMtx;

    afxError(*relinkDoutCb)(afxDrawDevice,afxDrawContext,afxNat,afxDrawOutput[]);    
    afxError(*relinkDinCb)(afxDrawDevice, afxDrawContext, afxNat, afxDrawInput[]);

    struct _afxDdevIdd* idd;
};
#endif//_AVX_DRAW_DEVICE_C

#ifdef _AVX_DRAW_QUEUE_C
#ifdef _AVX_DRAW_QUEUE_IMPL
AFX_OBJECT(_avxDrawQueue)
#else
AFX_OBJECT(afxDrawQueue)
#endif
{
    afxNat          portId;
    afxDrawBridge   dexu; // owner bridge
    afxDrawContext  dctx; // owner context
    afxBool         immediate; // 0 = deferred, 1 = immediate
    afxBool         closed; // can't enqueue

    afxSlock        workArenaSlock;
    afxArena        workArena; // used by submission of queue operations, not stream commands.        
    afxChain        workChn;
    afxMutex        workChnMtx;

    afxCondition    idleCnd;
    afxMutex        idleCndMtx;

    afxChain        classes;
    // one command pool per queue to avoid thread interation.
    afxClass        cmdbCls;
    // one stream manager per queue to avoid thread interaction
    afxQueue        cmdbRecycQue;
    afxSlock        cmdbReqLock;
    afxBool         cmdbLockedForReq;

    afxError        (*waitCb)(afxDrawQueue,afxTime);
    afxError        (*submitCb)(afxDrawQueue, avxSubmission const*, afxNat, avxCmdb[]);
    afxError        (*transferCb)(afxDrawQueue, avxTransference const*, afxNat, void const*);
    afxError        (*submCb)(afxDrawQueue, afxFence fenc, afxNat cnt, avxQueueOp const req[]);
};
#endif//_AVX_DRAW_QUEUE_C

#ifdef _AVX_DRAW_BRIDGE_C
#ifdef _AVX_DRAW_BRIDGE_IMPL
AFX_OBJECT(_avxDrawBridge)
#else
AFX_OBJECT(afxDrawBridge)
#endif
{
    afxDrawContext  dctx; // owner
    afxNat          exuIdx;
    afxNat          portId;
    
    afxNat          baseQueIdx; // indexed in draw context's queue array.
    afxNat          queCnt; // common queues? (non-private)
    
    afxError        (*waitCb)(afxDrawBridge, afxNat);
    afxError        (*pingCb)(afxDrawBridge, afxNat);
};
#endif//_AVX_DRAW_BRIDGE_C

#ifdef _AVX_DRAW_CONTEXT_C
#ifndef _AFX_CONTEXT_C
#   error "Require afxContext implementation"
#endif
#ifdef _AVX_DRAW_CONTEXT_IMPL
AFX_OBJECT(_avxDrawContext)
#else
AFX_OBJECT(afxDrawContext)
#endif
{
    AFX_OBJ(afxContext) ctx;
    afxBool             running;
    afxNat              exuCnt;
    afxDrawBridge*      exus;
    afxNat              queCnt;
    afxDrawQueue*       queues;
    afxChain            inputs;
    afxChain            outputs;

    afxArena aren;

    //afxChain classes;
    afxClass bufCls;
    afxClass rasCls;
    afxClass canvCls;

    afxClass vinCls;
    afxClass ligaCls;
    afxClass shadCls;
    afxClass pipCls;
    afxClass razrCls;

    afxClass sampCls;
    afxClass qrypCls;

    afxStringBase   shdStrb;

    afxCmd const* stdCmds;

    avxClipSpace clipSpace;

    afxError(*waitCb)(afxDrawContext,afxTime);

    struct _afxDctxIdd* idd;
    void* udd; // user-defined data
};
#ifdef _AVX_DRAW_CONTEXT_IMPL
//AFX_STATIC_ASSERT(offsetof(AFX_OBJECT(afxDrawContext), m) == 0x00, "");
#else
//AFX_STATIC_ASSERT(offsetof(AFX_OBJECT(afxDrawContext), m) == 0x00, "");
#endif
#endif//_AVX_DRAW_CONTEXT_C

/*
    Blit: Transfers or copies pixel data from one area to another within or between buffers. It’s used for operations like rendering images or updating parts of the screen.
    Flip: Swaps the front and back buffers in double buffering to update the display with a new frame, reducing flickering and improving performance.
    Swap: Refers to the exchange of buffers in multiple buffering strategies (e.g., double or triple buffering) to update the screen, often used to manage buffer visibility and performance.
    Present: Displays the contents of the current frame buffer on the screen, making the rendered image visible to the user.
*/

#ifdef _AVX_DRAW_OUTPUT_C
AFX_DEFINE_STRUCT(afxVideoEndpoint)
{
    afxUri32            urn;
    afxDrawOutputCaps   caps;    
    afxReal64           wpOverHp; // physical w/h
    afxReal64           wrOverHr; // (usually screen) resolution w/h
    afxBool             resizable;
    afxReal             refreshRate;
    afxWhd              res; // Screen resolution. Absolute extent available.
    union
    {
        afxObject       obj;
        afxWindow       wnd;
    };
    afxObject           endpointNotifyObj; // must ensure life of draw output
    avxPresentNotifier  endpointNotifyFn;
    avxPresentAlpha     presentAlpha; // consider transparency for external composing (usually on windowing system).
    avxPresentTransform presentTransform; // NIL leaves it as it is.
    avxPresentMode      presentMode; // FIFO
    afxBool             doNotClip; // usually false to don't do off-screen draw on compositor-based endpoints (aka window).
};

#ifdef _AVX_DRAW_OUTPUT_IMPL
AFX_OBJECT(_avxDrawOutput)
#else
AFX_OBJECT(afxDrawOutput)
#endif
{
    afxLinkage          dctx; // bound context
    afxAtom32           reconnecting;

    // canvas
    afxWhd              whd;
    afxReal64           wwOverHw; // window w/h
    afxBool             resizing;
    avxColorSpace       colorSpc; // raster color space. sRGB is the default.    
    avxFormat           pixelFmt; // pixel format of raster surfaces.
    afxRasterUsage      bufUsage; // raster usage
    afxRasterFlags      bufFlags; // raster flags. What evil things we will do with it?
    avxFormat           pixelFmtDs[2]; // pixel format for depth/stencil. D24/S8/D24S8
    afxRasterUsage      bufUsageDs[2]; // raster usage for depth/stencil
    afxRasterFlags      bufFlagsDs[2]; // raster flags for depth/stencil

    // swapchain
    afxNat              bufCnt; // usually 2 or 3; double or triple buffered.
    afxAtom32           presentingBufIdx;
    avxCanvas*          canvases;
    afxInterlockedQueue freeBuffers;

    afxError            (*lockCb)(afxDrawOutput, afxTime timeout, afxNat*bufIdx);
    afxError            (*unlockCb)(afxDrawOutput, afxNat cnt, afxNat const bufIdx[]);
    afxError            (*presentCb)(afxDrawOutput,afxNat bufIdx, afxNat queCnt, afxDrawQueue queues[]);

    // endpoint
    afxVideoEndpoint    endp;
    afxReal64           wpOverHp; // physical w/h
    afxReal64           wrOverHr; // (usually screen) resolution w/h
    afxBool             resizable;
    afxReal             refreshRate;
    afxWhd              res; // Screen resolution. Absolute extent available.
    afxObject           endpointNotifyObj; // must ensure life of draw output
    avxPresentNotifier  endpointNotifyFn;
    avxPresentAlpha     presentAlpha; // consider transparency for external composing (usually on windowing system).
    avxPresentTransform presentTransform; // NIL leaves it as it is.
    avxPresentMode      presentMode; // FIFO
    afxBool             doNotClip; // usually false to don't do off-screen draw on compositor-based endpoints (aka window).
    void*               udd[4]; // user-defined data

    afxNat              suspendCnt;
    afxSlock            suspendSlock;

    afxAtom32           submCnt;
    afxCondition        idleCnd;
    afxMutex            idleCndMtx;

    afxClock            startClock;
    afxClock            lastClock;
    afxNat              outNo;
    afxNat              outRate; // outputs per second
    afxClock            outCntResetClock;

    afxError            (*adjustCb)(afxDrawOutput,afxWhd const);
    afxBool             (*iddCb)(afxDrawOutput,afxNat,void*);
    struct _afxDoutIdd* idd; // alloc'ed by the driver
};
#endif//_AVX_DRAW_OUTPUT_C

#ifdef _AVX_DRAW_INPUT_C
AFX_DEFINE_STRUCT(afxDrawInputEndpoint)
{
    afxLinkage          ddev;
    afxChain            instances;
    afxMutex            mtx;
    afxCondition        cnd;
    afxString           name;
};

#ifdef _AVX_DRAW_INPUT_IMPL
AFX_OBJECT(_avxDrawInput)
#else
AFX_OBJECT(afxDrawInput)
#endif
{
    afxLinkage          dctx; // bound context
    afxBool             reconnecting;

    afxChain            classes;
    afxClass            camCls;
    afxClass            dtecCls;
    afxClass            txdCls;
    afxClass            texCls;
    afxClass            ibuffers;
    afxClass            vbuffers;
    
    afxClass            mshCls;
    afxClass            geomCls;

    void*               vbMgr;

    avxClipSpace        clipSpace;

    afxUri128           txdUris[8];
    afxFile             txdHandles[8];

    afxNat              minVtxBufSiz;
    afxNat              maxVtxBufSiz;
    afxNat              minIdxBufSiz;
    afxNat              maxIdxBufSiz; // 13500000

    afxDrawInputProc    procCb;


    afxStringBase   strbJointBiasesTags;
    afxStringBase   strbMorphTags;

    struct _afxDinIdd*  idd;
    void*               udd; // user-defined data
};
#endif//_AVX_DRAW_INPUT_C

AVX afxClassConfig const _AvxDqueStdImplementation;
AVX afxClassConfig const _AvxDexuStdImplementation;
AVX afxClassConfig const _AvxDctxStdImplementation;
AVX afxClassConfig const _AvxCmdbStdImplementation;

AVX afxClass const* _AvxGetDrawQueueClass(afxDrawDevice ddev);
AVX afxClass const* _AvxGetDrawBridgeClass(afxDrawDevice ddev);
AVX afxClass const* _AvxGetDrawThreadClass(afxDrawDevice ddev);
//AVX afxNat _AvxCountDrawQueues(afxDrawBridge dexu, afxNat baseQueIdx);
AVX afxError _AvxWaitForEmptyDrawQueue(afxDrawQueue dque, afxTime timeout);
AVX afxError _AvxWaitForIdleDrawBridge(afxDrawBridge dexu, afxTime timeout);

AVX afxClass const* _AvxGetDrawBatchClass(afxDrawQueue dque);

AVX afxError _AvxRegisterDrawDevices(afxModule icd, afxNat cnt, afxDrawDeviceInfo const infos[], afxDrawDevice devices[]);

AVX afxError _AfxAdjustDrawOutput(afxDrawOutput dout, afxWhd const whd);
AVX afxError _AvxPresentDrawOutput(afxDrawOutput dout, afxNat bufIdx, afxNat queCnt, afxDrawQueue queues[]);

AVX afxClassConfig const _AvxDqueStdImplementation;
AVX afxClassConfig const _AvxDexuStdImplementation;
AVX afxClassConfig const _AvxDctxStdImplementation;
AVX afxClassConfig const _AvxDoutStdImplementation;
AVX afxClassConfig const _AvxDinStdImplementation;

#endif//AVX_DEVICE_IMPL_KIT_H
