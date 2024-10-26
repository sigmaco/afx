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
 // High Performance Computing Infrastructure                                //
//////////////////////////////////////////////////////////////////////////////

// This section is part of SIGMA High Performance Computing Infrastructure.

// A computer simulation (or "sim") is an attempt to model a real-life or hypothetical situation on a computer so that it can be studied to see how the system works. 
// By changing variables in the simulation, predictions may be made about the behaviour of the system. 
// It is a tool to virtually investigate the behaviour of the system under study.

// A space is a purely mathematical representation of a world within space and its interactions.

/*
    Mathematics is a field of study that discovers and organizes abstract objects, methods, theories and theorems that are developed and proved for the needs of empirical sciences and mathematics itself. 
    There are many areas of mathematics, which include number theory (the study of numbers), algebra (the study of formulas and related structures), geometry (the study of shapes and spaces that contain them), 
    analysis (the study of continuous changes), and set theory (presently used as a foundation for all mathematics).
*/

#ifndef AMX_COMBO_SYSTEM_H
#define AMX_COMBO_SYSTEM_H

#include "qwadro/inc/draw/afxDrawSystem.h"

#include "qwadro/inc/sim/dev/afxComboBridge.h"

#include "qwadro/inc/sim/afxAnimation.h"
#include "qwadro/inc/sim/afxMotor.h"
#include "qwadro/inc/sim/afxMotion.h"

#include "qwadro/inc/sim/rendering/akxLighting.h"
#include "qwadro/inc/sim/rendering/akxRenderer.h"

AFX_DEFINE_STRUCT(afxComboDeviceCaps)
{
    afxBool8    work;
    afxBool8    curves;
    afxBool8    bsplines;
};

AFX_DEFINE_STRUCT(afxComboDeviceLimits)
{
    afxUnit nat;
};

AFX_DEFINE_STRUCT(afxPragmaPortCaps)
{
    afxComboBridgeFlags  queFlags;
    afxUnit              queCnt; // the count of queues in this port. Each port must support at least one queue.
};

AFX_DEFINE_STRUCT(akxSimulationConfig)
{
    afxBox              extent;

    afxV3d              right;
    afxV3d              up;
    afxV3d              back;
    afxV3d              origin;
    afxReal             unitsPerMeter;

    afxDrawContext      dctx;
    afxDrawInput        din;
};




AMX afxClass const* AfxGetComboDeviceClass(void);

// MATH DEVICE DISCOVERY ///////////////////////////////////////////////////////

AMX afxUnit          AfxInvokeComboDevices(afxUnit first, afxUnit cnt, afxBool(*f)(afxComboDevice, void*), void* udd);
AMX afxUnit          AfxEvokeComboDevices(afxBool(*flt)(afxComboDevice, void*), void* fdd, afxUnit first, afxUnit cnt, afxComboDevice devices[]);
AMX afxUnit          AfxEnumerateComboDevices(afxUnit first, afxUnit cnt, afxComboDevice devices[]);

AMX afxBool         AfxGetComboDevice(afxUnit mdevId, afxComboDevice* device);

// MATH DEVICE HANDLING ////////////////////////////////////////////////////////

AMX afxUnit          AfxInvokeSimulations(afxComboDevice mdev, afxUnit first, afxUnit cnt, afxBool(*f)(afxSimulation, void*), void *udd);
AMX afxUnit          AfxEvokeSimulations(afxComboDevice mdev, afxBool(*flt)(afxSimulation, void*), void* fdd, afxUnit first, afxUnit cnt, afxSimulation simulations[]);
AMX afxUnit          AfxEnumerateSimulations(afxComboDevice mdev, afxUnit first, afxUnit cnt, afxSimulation simulations[]);

AMX afxClass const* AfxGetSimulationClass(afxComboDevice mdev);

AMX afxUnit          AfxCountPragmaPorts(afxComboDevice mdev);

AMX afxBool         AfxIsComboDevicePrompt(afxComboDevice mdev);

// MATH DEVICE CONTEXTUALIZATION ///////////////////////////////////////////////

AMX afxError        AfxConfigureSimulation(afxUnit mdevId, akxSimulationConfig* cfg);

AMX afxError        AfxAcquireSimulations(afxUnit mdevId, akxSimulationConfig const* cfg, afxSimulation* simulation);

#endif//AMX_COMBO_SYSTEM_H
