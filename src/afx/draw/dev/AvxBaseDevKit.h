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

#ifndef AVX_HOST_DEV_KIT_H
#define AVX_HOST_DEV_KIT_H

#include "../src/afx/dev/afxDevCoreBase.h"
#include "qwadro/draw/afxDrawSystem.h"

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
    afxDeviceInfo       dev;
    afxNat              portCnt;
    //afxDrawPortCaps const*portCaps;
    afxDrawDeviceCaps   caps;
    afxDrawDeviceLimits limits;
    avxClipSpace        clipSpace;
    afxClassConfig const*dctxClsCfg;
    afxClassConfig const*ddgeClsCfg;
    afxClassConfig const*dqueClsCfg;
};

#ifdef _AVX_DRAW_SYSTEM_C
AFX_OBJECT(afxDrawSystem)
{
    afxBool             ready;
    afxClass            ddevCls;
    afxClass            doutEndCls;
    afxClass            doutCls;
    afxClass            dinEndCls;
    afxClass            dinCls;
};
#endif

#ifdef _AVX_DRAW_DEVICE_C
AFX_DEFINE_STRUCT(afxDrawInputEndpoint)
{
    afxLinkage          ddev;
    afxChain            instances;
    afxMutex            mtx;
    afxCondition        cnd;
    afxString           name;
};

AFX_OBJECT(afxDrawDevice)
{
    AFX_OBJECT(afxDevice) dev;

    afxClass ddgeCls;
    afxClass dctxCls;
    afxChain openedDoutChain;
    afxChain openedDinChain;
    afxChain ineps;

    afxDrawDeviceCaps const* caps;
    afxDrawDeviceLimits const* limits;
    afxNat portCnt;
    afxDrawPortCaps const* portCaps;
    
    avxClipSpace clipSpace;

    afxBool relinking;
    afxCondition relinkedCnd;
    afxMutex relinkedCndMtx;

    afxError(*stopCb)(afxDrawDevice);
    afxError(*startCb)(afxDrawDevice);
    afxError(*openCb)(afxDrawDevice,afxDrawContext,afxCookie const*); // unused if a custom dctx ctor is installed.
    afxError(*closeCb)(afxDrawDevice,afxDrawContext); // unused if a custom dctx ctor is installed.
    afxError(*closeDinCb)(afxDrawDevice,afxDrawInput);
    afxError(*openDinCb)(afxDrawDevice,afxDrawInput,afxDrawInputConfig const*,afxUri const*);
    afxError(*relinkDinCb)(afxDrawDevice,afxDrawContext,afxNat,afxDrawInput[]);
    afxError(*closeDoutCb)(afxDrawDevice,afxDrawOutput);
    afxError(*openDoutCb)(afxDrawDevice,afxDrawOutput,afxDrawOutputConfig const*,afxUri const*);
    afxError(*relinkDoutCb)(afxDrawDevice,afxDrawContext,afxNat,afxDrawOutput[]);
    
    struct _afxDdevIdd* idd;
};
#endif//_AVX_DRAW_DEVICE_C

#ifdef _AVX_DRAW_OUTPUT_C

AFX_DEFINE_STRUCT(afxDrawOutputEndpoint)
{
    afxLinkage          ddev;
    afxChain            instances;
    afxMutex            mtx;
    afxCondition        cnd;
    afxString           name;
    afxDrawOutputCaps   caps;
};

AFX_DEFINE_STRUCT(afxDrawOutputLink)
{
    afxChain a;
};

AFX_OBJECT(afxDrawOutput)
{
    afxLinkage          ddev;
    afxLinkage          dctx; // bound context

    // canvas
    afxWhd              whd;
    avxColorSpace       colorSpc; // raster color space. sRGB is the default.    
    afxPixelFormat      pixelFmt; // pixel format of raster surfaces.
    afxPixelFormat      pixelFmtDs[2]; // pixel format for depth/stencil. D24/S8/D24S8
    afxRasterUsage      bufUsage; // raster usage
    afxRasterUsage      bufUsageDs[2]; // raster usage for depth/stencil
    afxRasterFlags      bufFlags; // raster flags. What evil things we will do with it?
    afxRasterFlags      bufFlagsDs[2]; // raster flags for depth/stencil

    // swapchain
    afxSlock            buffersLock;
    afxAtom32           lockedBufCnt;
    afxAtom32           submCnt;
    afxNat              bufCnt; // usually 2 or 3; double or triple buffered.
    afxAtom32           lastLockedBufIdx;
    afxAtom32           lastUnlockedBufIdx;
    afxAtom32           presentingBufIdx;
    afxChain            swapchain; // display order
    struct
    {
        //afxRaster       ras; // avxCanvas // should have 1 fb for each swapchain raster.
        avxCanvas       canv;
        afxSemaphore    readySem;
        afxAtom32       booked;
        afxSlock        lck;
    }*                  buffers;
    afxError            (*lockCb)(afxDrawOutput, afxTime timeout, afxNat*bufIdx);
    afxError            (*unlockCb)(afxDrawOutput, afxNat cnt, afxNat const bufIdx[]);

    // endpoint
    afxReal64           wpOverHp; // physical w/h
    afxReal64           wrOverHr; // (usually screen) resolution w/h
    afxReal64           wwOverHw; // window w/h
    afxBool             resizing;
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

    afxAtom32           reconnecting;

    afxClock            startClock;
    afxClock            lastClock;
    afxNat              outNo;
    afxNat              outRate; // outputs per second
    afxClock            outCntResetClock;

    afxBool (*iddCb)(afxDrawOutput,afxNat,void*);
    struct _afxDoutIdd* idd; // alloc'ed by the driver
};
#endif//_AVX_DRAW_OUTPUT_C

#ifdef _AVX_DRAW_INPUT_C
AFX_OBJECT(afxDrawInput)
{
    afxLinkage          ddev;
    afxLinkage          dctx; // bound context

    afxChain            classes;
    afxClass            camCls;
    afxClass            ibuffers;
    afxClass            vbuffers;

    struct
    {
        afxDrawBridgeFlags queFlags;
        afxNat          portIdx;
        // one command pool per queue to avoid thread interation.
        afxClass      streams;
        // one stream manager per queue to avoid thread interaction

        afxQueue        recycQue;
        afxSlock        reqLock;
        afxBool         lockedForReq;
    }*pools;
    afxNat              poolCnt;

    avxClipSpace        clipSpace;

    afxUri128           txdUris[8];
    afxFile             txdHandles[8];

    afxNat              minVtxBufSiz;
    afxNat              maxVtxBufSiz;
    afxNat              minIdxBufSiz;
    afxNat              maxIdxBufSiz; // 13500000

    afxDrawInputProc    procCb;

    afxBool             reconnecting;

    struct _afxDinIdd*  idd;
    void*               udd; // user-defined data
};
#endif//_AVX_DRAW_INPUT_C

#ifdef _AVX_CAMERA_C
AFX_OBJECT(avxCamera)
{
    afxReal             wpOverHp; // physical w/h
    afxReal             wrOverHr; // resolution w/h
    afxReal             wwOverHw; // window w/h
    afxReal             fovY;
    afxReal             nearClipPlane;
    afxReal             farClipPlane;
    afxReal             depthRangeEpsilon;
    avxClipBoundary     depthRange;
    afxBool             useQuatOrient;
    afxQuat             orient;
    afxM3d              orientM3d;
    afxV3d              pos;
    afxV3d              elevAzimRoll;
    afxV3d              offset;
    afxM4d              v;
    afxM4d              iv; // inverse view
    afxM4d              p;
    afxM4d              ip; // inverse projection
    afxBool             vChanged;
    afxBool             pChanged;



    afxBool     perspective; // is projection perspective or orthographic.
    afxFrustum  frustum;
};
#endif//_AVX_CAMERA_C

//AFX_STATIC_ASSERT(offsetof(avxCamera, focus % AFX_CAM_ALIGN == 0, "");

AVX afxClassConfig const _AvxDqueStdImplementation;
AVX afxClassConfig const _AvxDdgeStdImplementation;
AVX afxClassConfig const _AvxDctxStdImplementation;
AVX afxClassConfig const _AvxCmdbStdImplementation;

AVX afxClass const* _AvxGetDrawQueueClass(afxDrawBridge ddge);
AVX afxClass const* _AvxGetDrawBridgeClass(afxDrawDevice ddev);
AVX afxNat _AvxCountDrawQueues(afxDrawBridge ddge);
AVX afxBool _AvxGetDrawQueue(afxDrawBridge ddge, afxNat queIdx, afxDrawQueue* queue);
AVX afxError _AvxWaitForIdleDrawQueue(afxDrawBridge ddge, afxNat queIdx);
AVX afxError _AvxWaitForIdleDrawBridge(afxDrawBridge ddge);

#endif//AVX_HOST_DEV_KIT_H
