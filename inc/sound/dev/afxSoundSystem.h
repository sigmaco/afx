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

/*
    Copyright (c) 2017 SIGMA FEDERATION.

    SIGMA FEDERATION ("SIGMA") supplies this software to you in
    consideration of your agreement to the following terms, and your use,
    installation, modification or redistribution of this SIGMA software
    constitutes acceptance of these terms.  If you do not agree with these
    terms, please do not use, install, modify or redistribute this SIGMA
    software.

    In consideration of your agreement to abide by the following terms, and
    subject to these terms, SIGMA grants you a personal, non-exclusive
    license, under SIGMA's copyrights in this original SIGMA software (the
    SIGMA Software), to use, reproduce, modify and redistribute the
    SIGMA Software, with or without modifications, in source and/or binary
    forms; provided that if you redistribute the SIGMA Software, you must
    retain the copyright notice of SIGMA, this notice and the following
    text and disclaimers in all such redistributions of the SIGMA Software.
    Neither the name, trademarks, service marks nor logos of SIGMA
    FEDERATION may be used to endorse or promote products derived from the
    SIGMA Software without specific prior written permission from SIGMA.
    Except as expressly stated in this notice, no other rights or licenses
    express or implied, are granted by SIGMA herein, including but not
    limited to any patent rights that may be infringed by your derivative
    works or by other works in which the SIGMA Software may be
    incorporated. No hardware is licensed hereunder.

    THE SIGMA SOFTWARE IS BEING PROVIDED ON AN "AS IS" BASIS, WITHOUT
    WARRANTIES OR CONDITIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED,
    INCLUDING WITHOUT LIMITATION, WARRANTIES OR CONDITIONS OF TITLE,
    NON-INFRINGEMENT, MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, OR
    ITS USE AND OPERATION EITHER ALONE OR IN COMBINATION WITH OTHER
    PRODUCTS.

    IN NO EVENT SHALL SIGMA BE LIABLE FOR ANY SPECIAL, INDIRECT,
    INCIDENTAL, EXEMPLARY, CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
    TO, LOST PROFITS; PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
    USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) OR ARISING IN ANY WAY
    OUT OF THE USE, REPRODUCTION, MODIFICATION AND/OR DISTRIBUTION OF THE
    SIGMA SOFTWARE, HOWEVER CAUSED AND WHETHER UNDER THEORY OF CONTRACT,
    TORT (INCLUDING NEGLIGENCE), STRICT LIABILITY OR OTHERWISE, EVEN IF
    SIGMA HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
    afxUnit              queCnt;
};

AFX_DEFINE_STRUCT(afxSoundContextConfig)
{
    afxSoundBridgeConfig    mainBridge;
    afxUnit                  auxBridgeCnt;
    afxSoundBridgeConfig*   auxBridges;

    void*                   udd;
};


ASX afxClass const* AfxGetSoundDeviceClass(void);

// SOUND DEVICE DISCOVERY //////////////////////////////////////////////////////

ASX afxUnit          AfxInvokeSoundDevices(afxUnit first, afxUnit cnt, afxBool(*f)(afxSoundDevice, void*), void* udd);
ASX afxUnit          AfxEvokeSoundDevices(afxBool(*flt)(afxSoundDevice, void*), void* fdd, afxUnit first, afxUnit cnt, afxSoundDevice devices[]);
ASX afxUnit          AfxEnumerateSoundDevices(afxUnit first, afxUnit cnt, afxSoundDevice devices[]);

ASX afxBool         AfxGetSoundDevice(afxUnit sdevIdx, afxSoundDevice* device);

// SOUND DEVICE HANDLING ///////////////////////////////////////////////////////

ASX afxUnit          AfxInvokeSoundContexts(afxSoundDevice sdev, afxUnit first, afxUnit cnt, afxBool(*f)(afxSoundContext, void*), void *udd);
ASX afxUnit          AfxInvokeSoundOutputs(afxSoundDevice sdev, afxUnit first, afxUnit cnt, afxBool(*f)(afxSoundOutput, void*), void *udd);
ASX afxUnit          AfxInvokeSoundInputs(afxSoundDevice sdev, afxUnit first, afxUnit cnt, afxBool(*f)(afxSoundInput, void*), void *udd);

ASX afxUnit          AfxEvokeSoundContexts(afxSoundDevice sdev, afxBool(*flt)(afxSoundContext, void*), void* fdd, afxUnit first, afxUnit cnt, afxSoundContext contexts[]);
ASX afxUnit          AfxEvokeSoundOutputs(afxSoundDevice sdev, afxBool(*flt)(afxSoundOutput, void*), void* fdd, afxUnit first, afxUnit cnt, afxSoundOutput outputs[]);
ASX afxUnit          AfxEvokeSoundInputs(afxSoundDevice sdev, afxBool(*flt)(afxSoundInput, void*), void* fdd, afxUnit first, afxUnit cnt, afxSoundInput inputs[]);

ASX afxUnit          AfxEnumerateSoundContexts(afxSoundDevice sdev, afxUnit first, afxUnit cnt, afxSoundContext contexts[]);
ASX afxUnit          AfxEnumerateSoundOutputs(afxSoundDevice sdev, afxUnit first, afxUnit cnt, afxSoundOutput outputs[]);
ASX afxUnit          AfxEnumerateSoundInputs(afxSoundDevice sdev, afxUnit first, afxUnit cnt, afxSoundInput inputs[]);

ASX afxClass const* AfxGetSoundContextClass(afxSoundDevice sdev);
ASX afxClass const* AfxGetSoundOutputClass(afxSoundDevice sdev);
ASX afxClass const* AfxGetSoundInputClass(afxSoundDevice sdev);

ASX afxUnit          AfxCountSoundPorts(afxSoundDevice sdev);

ASX afxBool         AfxIsSoundDevicePrompt(afxSoundDevice sdev);

// SOUND DEVICE CONTEXTUALIZATION //////////////////////////////////////////////

ASX afxError        AfxConfigureSoundDevice(afxUnit sdevId, afxSoundContextConfig* cfg);
ASX afxError        AfxConfigureSoundOutput(afxUnit sdevId, afxSoundOutputConfig* cfg);
ASX afxError        AfxConfigureSoundInput(afxUnit sdevId, afxSoundInputConfig* cfg);

ASX afxError        AfxOpenSoundDevice(afxUnit sdevId, afxSoundContextConfig const* cfg, afxSoundContext* context);
ASX afxError        AfxOpenSoundOutput(afxUnit sdevId, afxSoundOutputConfig const* cfg, afxSoundOutput* output);
ASX afxError        AfxOpenSoundInput(afxUnit sdevId, afxSoundInputConfig const* cfg, afxSoundInput* input);

#endif//ASX_SOUND_SYSTEM_H
