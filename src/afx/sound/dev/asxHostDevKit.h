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

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#ifndef ASX_HOST_DEV_KIT_H
#define ASX_HOST_DEV_KIT_H

#include "../src/afx/dev/afxDevCoreBase.h"
#include "qwadro/sound/afxUnisound.h"

typedef enum _sdevReqCode
{
    _sdevReqCode_0,
    _sdevReqCode_1,
    _sdevReqCode_SIN_DTOR,
    _sdevReqCode_SIN_CTOR,
    _sdevReqCode_SIN_RLNK,
    _sdevReqCode_SOUT_RLNK,
    _sdevReqCode_SOUT_DTOR,
    _sdevReqCode_SOUT_CTOR,
    _sdevReqCode_SCTX_DTOR,
    _sdevReqCode_SCTX_CTOR,
} _sdevReqCode;

AFX_DEFINE_STRUCT(afxSoundDeviceInfo)
{
    afxDeviceInfo       dev;
    afxNat              portCnt;
    //afxSoundPortCaps const*portCaps;
    //afxSoundDeviceCaps  caps;
    //afxSoundDeviceLimits limits;
    void*               idd;
    afxError            (*iddCtorCb)(afxSoundDevice);
    afxError            (*iddDtorCb)(afxSoundDevice);
    afxError            (*procCb)(afxSoundDevice,afxThread); // call their draw threads.    
    afxError            (*sinIddDtorCb)(afxSoundDevice, afxSoundInput);
    afxError            (*sinIddCtorCb)(afxSoundDevice,afxSoundInput, afxSoundInputConfig const*,afxUri const*);
    afxError            (*sinRelinkCb)(afxSoundDevice, afxSoundContext,afxNat,afxSoundInput[]);
    afxError            (*soutIddDtorCb)(afxSoundDevice,afxSoundOutput);
    afxError            (*soutIddCtorCb)(afxSoundDevice,afxSoundOutput,afxSoundOutputConfig const*,afxUri const*);
    afxError            (*soutRelinkCb)(afxSoundDevice,afxSoundContext,afxNat,afxSoundOutput[]);
    afxClassConfig const*sctxClsCfg;
    afxClassConfig const*sdgeClsCfg;
};

#ifdef _ASX_SOUND_SYSTEM_C
AFX_OBJECT(afxSoundSystem)
{
    afxBool             ready;
    afxClass            sdevCls;
    afxClass            soutCls;
    afxClass            oends;
    afxClass            sinCls;
    afxClass            iends;
};
#endif

#ifdef _ASX_SOUND_DEVICE_C
AFX_DEFINE_STRUCT(afxSoundOutputEndpoint)
{
    afxLinkage          sdev;
    afxChain            instances;
    afxMutex            mtx;
    afxCondition        cnd;
    afxString           name;
};

AFX_OBJECT(afxSoundDevice)
{
    AFX_OBJECT(afxDevice) dev;

    afxClass sdgeCls;
    afxClass sctxCls;
    afxChain openedSoutChain;
    afxChain openedSinChain;
    afxChain            ineps;

    //afxSoundDeviceCaps const* caps;
    //afxSoundDeviceLimits const* limits;
    afxNat              portCnt;
    afxSoundPortCaps const* portCaps;

    afxBool             relinking;
    afxCondition        relinkedCnd;
    afxMutex            relinkedCndMtx;

    afxError(*stopCb)(afxSoundDevice);
    afxError(*startCb)(afxSoundDevice);
    afxError(*openCb)(afxSoundDevice, afxSoundContext, afxCookie const*); // unused if a custom dctx ctor is installed.
    afxError(*closeCb)(afxSoundDevice, afxSoundContext); // unused if a custom dctx ctor is installed.
    afxError(*sinCloseCb)(afxSoundDevice, afxSoundInput);
    afxError(*sinOpenCb)(afxSoundDevice, afxSoundInput, afxSoundInputConfig const*, afxUri const*);
    afxError(*sinRelinkCb)(afxSoundDevice, afxSoundContext, afxNat, afxSoundInput[]);
    afxError(*soutCloseCb)(afxSoundDevice, afxSoundOutput);
    afxError(*soutOpenCb)(afxSoundDevice, afxSoundOutput, afxSoundOutputConfig const*, afxUri const*);
    afxError(*soutRelinkCb)(afxSoundDevice, afxSoundContext, afxNat, afxSoundOutput[]);

    struct _afxSdevIdd* idd;
};
#endif//_ASX_SOUND_DEVICE_C

#ifdef _ASX_SOUND_OUTPUT_C

AFX_OBJECT(afxSoundOutput)
{
    afxLinkage          sdev;
    afxLinkage          sctx; // bound context
    void*               udd; // user-defined data
    afxBool             (*getIddCb)(afxSoundOutput,afxNat,void*);
    afxAtom32           reconnecting;

    afxClock            startClock;
    afxClock            lastClock;
    afxClock            outCntResetClock;

    struct _afxDoutIdd* idd;

#ifdef _ASX_SOUND_OUTPUT_IMPL
#ifdef AFX_OS_WIN
    struct
    {
        HINSTANCE       hInst;
        HWND            hWnd;
    }                   w32;
#endif//AFX_OS_WIN
#endif//_ASX_SOUND_OUTPUT_IMPL
};
#endif//_ASX_SOUND_OUTPUT_C

#ifdef _ASX_SOUND_INPUT_C
AFX_OBJECT(afxSoundInput)
{
    afxLinkage          sdev;
    afxLinkage          sctx; // bound context

    afxChain            classes;
    afxSoundInputProc   procCb;

    afxBool             reconnecting;

    struct _afxDinIdd*  idd;
    void*               udd; // user-defined data
};
#endif//_ASX_SOUND_INPUT_C

ASX afxClassConfig const _AsxSqueStdImplementation;
ASX afxClassConfig const _AsxSdgeStdImplementation;
ASX afxClassConfig const _AsxSctxStdImplementation;

#endif//ASX_HOST_DEV_KIT_H
