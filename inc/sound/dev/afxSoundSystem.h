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

#include "qwadro/inc/exec/afxDevice.h"
#include "qwadro/inc/base/afxClass.h"
#include "qwadro/inc/io/afxSource.h"
#include "qwadro/inc/exec/afxDevice.h"
#include "qwadro/inc/sound/dev/afxSoundContext.h"
#include "qwadro/inc/sound/dev/afxSoundOutput.h"
#include "qwadro/inc/sound/dev/afxSoundInput.h"

typedef enum afxSoundBridgeFlag
{
    afxSoundBridgeFlag_NIL
} afxSoundBridgeFlag;

AFX_DEFINE_STRUCT(afxSoundPortCaps)
{
    afxSoundBridgeFlag  queFlags;
    afxNat              queCnt;
};

AFX_DEFINE_STRUCT(afxSoundContextConfig)
{
    afxSoundBridgeConfig    mainBridge;
    afxNat                  auxBridgeCnt;
    afxSoundBridgeConfig*   auxBridges;

    void*                   udd;
};


ASX afxClass const* AfxGetSoundDeviceClass(void);

// SOUND DEVICE DISCOVERY //////////////////////////////////////////////////////

ASX afxNat          AfxInvokeSoundDevices(afxNat first, afxNat cnt, afxBool(*f)(afxSoundDevice, void*), void* udd);
ASX afxNat          AfxEvokeSoundDevices(afxBool(*flt)(afxSoundDevice, void*), void* fdd, afxNat first, afxNat cnt, afxSoundDevice devices[]);
ASX afxNat          AfxEnumerateSoundDevices(afxNat first, afxNat cnt, afxSoundDevice devices[]);

ASX afxBool         AfxGetSoundDevice(afxNat sdevIdx, afxSoundDevice* device);

// SOUND DEVICE HANDLING ///////////////////////////////////////////////////////

ASX afxNat          AfxInvokeSoundContexts(afxSoundDevice sdev, afxNat first, afxNat cnt, afxBool(*f)(afxSoundContext, void*), void *udd);
ASX afxNat          AfxInvokeSoundOutputs(afxSoundDevice sdev, afxNat first, afxNat cnt, afxBool(*f)(afxSoundOutput, void*), void *udd);
ASX afxNat          AfxInvokeSoundInputs(afxSoundDevice sdev, afxNat first, afxNat cnt, afxBool(*f)(afxSoundInput, void*), void *udd);

ASX afxNat          AfxEvokeSoundContexts(afxSoundDevice sdev, afxBool(*flt)(afxSoundContext, void*), void* fdd, afxNat first, afxNat cnt, afxSoundContext contexts[]);
ASX afxNat          AfxEvokeSoundOutputs(afxSoundDevice sdev, afxBool(*flt)(afxSoundOutput, void*), void* fdd, afxNat first, afxNat cnt, afxSoundOutput outputs[]);
ASX afxNat          AfxEvokeSoundInputs(afxSoundDevice sdev, afxBool(*flt)(afxSoundInput, void*), void* fdd, afxNat first, afxNat cnt, afxSoundInput inputs[]);

ASX afxNat          AfxEnumerateSoundContexts(afxSoundDevice sdev, afxNat first, afxNat cnt, afxSoundContext contexts[]);
ASX afxNat          AfxEnumerateSoundOutputs(afxSoundDevice sdev, afxNat first, afxNat cnt, afxSoundOutput outputs[]);
ASX afxNat          AfxEnumerateSoundInputs(afxSoundDevice sdev, afxNat first, afxNat cnt, afxSoundInput inputs[]);

ASX afxClass const* AfxGetSoundContextClass(afxSoundDevice sdev);
ASX afxClass const* AfxGetSoundOutputClass(afxSoundDevice sdev);
ASX afxClass const* AfxGetSoundInputClass(afxSoundDevice sdev);

ASX afxNat          AfxCountSoundPorts(afxSoundDevice sdev);

ASX afxBool         AfxIsSoundDevicePrompt(afxSoundDevice sdev);

// SOUND DEVICE CONTEXTUALIZATION //////////////////////////////////////////////

ASX afxError        AfxConfigureSoundDevice(afxNat sdevId, afxSoundContextConfig* cfg);
ASX afxError        AfxConfigureSoundOutput(afxNat sdevId, afxSoundOutputConfig* cfg);
ASX afxError        AfxConfigureSoundInput(afxNat sdevId, afxSoundInputConfig* cfg);

ASX afxError        AfxOpenSoundDevice(afxNat sdevId, afxSoundContextConfig const* cfg, afxSoundContext* context);
ASX afxError        AfxOpenSoundOutput(afxNat sdevId, afxSoundOutputConfig const* cfg, afxSoundOutput* output);
ASX afxError        AfxOpenSoundInput(afxNat sdevId, afxSoundInputConfig const* cfg, afxSoundInput* input);

#endif//ASX_SOUND_SYSTEM_H
