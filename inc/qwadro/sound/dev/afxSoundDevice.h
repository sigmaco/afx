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

#ifndef ASX_SOUND_DEVICE_H
#define ASX_SOUND_DEVICE_H

#include "qwadro/exec/afxDevice.h"
#include "qwadro/base/afxClass.h"
#include "qwadro/io/afxSource.h"
#include "qwadro/exec/afxDevice.h"
//#include "qwadro/sound/dev/afxSoundContext.h"
#include "qwadro/sound/dev/afxSoundOutput.h"
#include "qwadro/sound/dev/afxSoundInput.h"
#include "qwadro/sound/asxSound.h"

typedef enum afxSoundBridgeFlag
{
    afxSoundBridgeFlag_NIL
} afxSoundBridgeFlag;

AFX_DEFINE_STRUCT(afxSoundPortCaps)
{
    afxSoundBridgeFlag  queFlags;
    afxNat              queCnt;
};

ASX afxBool         AfxSoundDeviceIsRunning(afxSoundDevice sdev);
ASX afxClass*       AfxGetSoundContextClass(afxSoundDevice sdev);
ASX afxNat          AfxCountSoundPorts(afxSoundDevice sdev);

ASX afxNat          AfxEnumerateSoundContexts(afxSoundDevice sdev, afxNat first, afxNat cnt, afxSoundContext contexts[]);
ASX afxNat          AfxEnumerateSoundOutputs(afxSoundDevice sdev, afxNat first, afxNat cnt, afxSoundOutput outputs[]);
ASX afxNat          AfxEnumerateSoundInputs(afxSoundDevice sdev, afxNat first, afxNat cnt, afxSoundInput inputs[]);

ASX afxNat          AfxEvokeSoundContexts(afxSoundDevice sdev, afxBool(*flt)(afxSoundContext, void*), void* fdd, afxNat first, afxNat cnt, afxSoundContext contexts[]);
ASX afxNat          AfxEvokeSoundOutputs(afxSoundDevice sdev, afxBool(*flt)(afxSoundOutput, void*), void* fdd, afxNat first, afxNat cnt, afxSoundOutput outputs[]);
ASX afxNat          AfxEvokeSoundInputs(afxSoundDevice sdev, afxBool(*flt)(afxSoundInput, void*), void* fdd, afxNat first, afxNat cnt, afxSoundInput inputs[]);

ASX afxNat          AfxInvokeSoundContexts(afxSoundDevice sdev, afxNat first, afxNat cnt, afxBool(*f)(afxSoundContext, void*), void *udd);
ASX afxNat          AfxInvokeSoundOutputs(afxSoundDevice sdev, afxNat first, afxNat cnt, afxBool(*f)(afxSoundOutput, void*), void *udd);
ASX afxNat          AfxInvokeSoundInputs(afxSoundDevice sdev, afxNat first, afxNat cnt, afxBool(*f)(afxSoundInput, void*), void *udd);

ASX afxNat          AfxInvokeSoundContexts2(afxSoundDevice sdev, afxNat first, afxNat cnt, afxBool(*flt)(afxSoundContext, void*), void* fdd, afxBool(*f)(afxSoundContext, void*), void *udd);
ASX afxNat          AfxInvokeSoundOutputs2(afxSoundDevice sdev, afxNat first, afxNat cnt, afxBool(*flt)(afxSoundOutput, void*), void *fdd, afxBool(*f)(afxSoundOutput, void*), void *udd);
ASX afxNat          AfxInvokeSoundInputs2(afxSoundDevice sdev, afxNat first, afxNat cnt, afxBool(*flt)(afxSoundInput, void*), void *fdd, afxBool(*f)(afxSoundInput, void*), void *udd);

#endif//ASX_SOUND_DEVICE_H
