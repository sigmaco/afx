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
    afxNat nat;
};

AFX_DEFINE_STRUCT(afxPragmaPortCaps)
{
    afxComboBridgeFlags  queFlags;
    afxNat              queCnt; // the count of queues in this port. Each port must support at least one queue.
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

AMX afxNat          AfxInvokeComboDevices(afxNat first, afxNat cnt, afxBool(*f)(afxComboDevice, void*), void* udd);
AMX afxNat          AfxEvokeComboDevices(afxBool(*flt)(afxComboDevice, void*), void* fdd, afxNat first, afxNat cnt, afxComboDevice devices[]);
AMX afxNat          AfxEnumerateComboDevices(afxNat first, afxNat cnt, afxComboDevice devices[]);

AMX afxBool         AfxGetComboDevice(afxNat mdevId, afxComboDevice* device);

// MATH DEVICE HANDLING ////////////////////////////////////////////////////////

AMX afxNat          AfxInvokeSimulations(afxComboDevice mdev, afxNat first, afxNat cnt, afxBool(*f)(afxSimulation, void*), void *udd);
AMX afxNat          AfxEvokeSimulations(afxComboDevice mdev, afxBool(*flt)(afxSimulation, void*), void* fdd, afxNat first, afxNat cnt, afxSimulation simulations[]);
AMX afxNat          AfxEnumerateSimulations(afxComboDevice mdev, afxNat first, afxNat cnt, afxSimulation simulations[]);

AMX afxClass const* AfxGetSimulationClass(afxComboDevice mdev);

AMX afxNat          AfxCountPragmaPorts(afxComboDevice mdev);

AMX afxBool         AfxIsComboDevicePrompt(afxComboDevice mdev);

// MATH DEVICE CONTEXTUALIZATION ///////////////////////////////////////////////

AMX afxError        AfxConfigureSimulation(afxNat mdevId, akxSimulationConfig* cfg);

AMX afxError        AfxAcquireSimulations(afxNat mdevId, akxSimulationConfig const* cfg, afxSimulation* simulation);

#endif//AMX_COMBO_SYSTEM_H
