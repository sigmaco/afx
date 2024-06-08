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
 // Advanced Sound Experience on Qwadro                                      //
//////////////////////////////////////////////////////////////////////////////
// This section is part of SIGMA A4D; Advanced Audio Extensions for Qwadro.

#ifndef ASX_SOUND_SYSTEM_H
#define ASX_SOUND_SYSTEM_H

#include "qwadro/core/afxDevice.h"
#include "qwadro/core/afxManager.h"
#include "qwadro/io/afxSource.h"
#include "qwadro/core/afxDevice.h"
#include "qwadro/sound/afxSoundContext.h"
#include "qwadro/sound/afxSoundOutput.h"
#include "qwadro/sound/afxSoundInput.h"
#include "qwadro/sound/afxListener.h"

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

AFX_DEFINE_STRUCT(afxSoundSystemConfig)
{
    afxSize const*      attrs[2];
};

#ifdef _ASX_SOUND_C
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

    afxManager          contexts;
    afxChain            outputs;
    afxChain            inputs;
    afxChain            ineps;

    //afxSoundDeviceCaps  caps;
    //afxSoundDeviceLimits limits;
    afxNat              portCnt;
    struct
    {
        //afxSoundPortCaps portCaps;
        afxManager      sdgeMgr;
    }*                  ports;
    
    afxBool             relinking;
    afxCondition        relinkedCnd;
    afxMutex            relinkedCndMtx;

    //afxError            (*procCb)(afxSoundDevice,afxThread); // call their draw threads.    
    afxError            (*iddDtorCb)(afxSoundDevice);
    afxError            (*sinIddDtorCb)(afxSoundDevice,afxSoundInput);
    afxError            (*sinIddCtorCb)(afxSoundDevice,afxSoundInput,afxSoundInputConfig const*,afxUri const*);
    afxError            (*sinRelinkCb)(afxSoundDevice,afxSoundContext,afxNat,afxSoundInput[]);
    afxError            (*soutIddDtorCb)(afxSoundDevice,afxSoundOutput);
    afxError            (*soutIddCtorCb)(afxSoundDevice, afxSoundOutput,afxSoundOutputConfig const*,afxUri const*);
    afxError            (*soutRelinkCb)(afxSoundDevice,afxSoundContext,afxNat,afxSoundOutput[]);

    struct _afxSdevIdd* idd;
};
#endif//_ASX_SOUND_DEVICE_C
#ifdef _ASX_SOUND_SYSTEM_C
AFX_OBJECT(afxSoundSystem)
{
    afxChain            managers;
    afxManager          sdevMgr;
    afxManager          soutMgr;
    afxManager          sinMgr;

    afxManager          oends;
    afxManager          iends;
};
#endif//_ASX_SOUND_SYSTEM_C
#endif//_ASX_SOUND_C

ASX afxManager*     AfxGetSoundDeviceManager(void);
ASX afxManager*     AfxGetSoundOutputClass(void);
ASX afxManager*     AfxGetSoundInputClass(void);

ASX afxNat          AfxCountSoundDevices(void);
ASX afxNat          AfxInvokeSoundDevices(afxNat first, afxNat cnt, afxBool(*f)(afxSoundDevice, void*), void *udd);
ASX afxNat          AfxEnumerateSoundDevices(afxNat first, afxNat cnt, afxSoundDevice sdev[]);

////////////////////////////////////////////////////////////////////////////////
// SOUND DEVICE                                                               //
////////////////////////////////////////////////////////////////////////////////

ASX afxBool         AfxGetSoundDevice(afxNat sdevIdx, afxSoundDevice* device);

ASX afxNat          AfxCountSoundPorts(afxSoundDevice sdev);

ASX afxManager*     AfxGetSoundContextManager(afxSoundDevice sdev);
ASX afxBool         AfxSoundDeviceIsRunning(afxSoundDevice sdev);

ASX afxNat          AfxCountSoundContexts(afxSoundDevice sdev);
ASX afxNat          AfxInvokeSoundContexts(afxSoundDevice sdev, afxNat first, afxNat cnt, afxBool(*f)(afxSoundContext, void*), void *udd);
ASX afxNat          AfxEnumerateSoundContexts(afxSoundDevice sdev, afxNat first, afxNat cnt, afxSoundContext contexts[]);

ASX afxNat          AfxCountSoundInputs(afxSoundDevice sdev);
ASX afxNat          AfxInvokeSoundInputs(afxSoundDevice sdev, afxNat first, afxNat cnt, afxBool(*f)(afxSoundInput, void*), void *udd);
ASX afxNat          AfxEnumerateSoundInputs(afxSoundDevice sdev, afxNat first, afxNat cnt, afxSoundInput inputs[]);

ASX afxNat          AfxCountSoundOutputs(afxSoundDevice sdev);
ASX afxNat          AfxInvokeSoundOutputs(afxSoundDevice sdev, afxNat first, afxNat cnt, afxBool(*f)(afxSoundOutput, void*), void *udd);
ASX afxNat          AfxEnumerateSoundOutputs(afxSoundDevice sdev, afxNat first, afxNat cnt, afxSoundOutput outputs[]);

////////////////////////////////////////////////////////////////////////////////

ASX afxClassConfig const _AsxSdgeStdImplementation;
ASX afxClassConfig const _AsxSctxStdImplementation;

#endif//ASX_SOUND_SYSTEM_H
