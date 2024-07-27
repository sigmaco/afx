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

#include "qwadro/exec/afxDevice.h"
#include "qwadro/base/afxClass.h"
#include "qwadro/io/afxSource.h"
#include "qwadro/exec/afxDevice.h"
#include "qwadro/sound/dev/afxSoundDevice.h"
#include "qwadro/sound/dev/afxSoundContext.h"
#include "qwadro/sound/dev/afxSoundOutput.h"
#include "qwadro/sound/dev/afxSoundInput.h"

ASX afxClass const* AfxGetSoundDeviceClass(void);
ASX afxClass const* AfxGetSoundOutputClass(void);
ASX afxClass const* AfxGetSoundInputClass(void);

ASX afxNat          AfxEnumerateSoundDevices(afxNat first, afxNat cnt, afxSoundDevice devices[]);
ASX afxNat          AfxEvokeSoundDevices(afxBool(*flt)(afxSoundDevice, void*), void* fdd, afxNat first, afxNat cnt, afxSoundDevice devices[]);
ASX afxNat          AfxInvokeSoundDevices(afxNat first, afxNat cnt, afxBool(*f)(afxSoundDevice, void*), void* udd);
ASX afxNat          AfxInvokeSoundDevices2(afxNat first, afxNat cnt, afxBool(*flt)(afxSoundDevice, void*), void *fdd, afxBool(*f)(afxSoundDevice, void*), void *udd);

ASX afxBool         AfxGetSoundDevice(afxNat sdevIdx, afxSoundDevice* device);

#endif//ASX_SOUND_SYSTEM_H
