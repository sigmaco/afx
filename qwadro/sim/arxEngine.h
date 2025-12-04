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
 // High Performance Cosputing Infrastructure                                //
//////////////////////////////////////////////////////////////////////////////

// This section is part of SIGMA High Performance Cosputing Infrastructure.
// This file is part of Acceleration for RenderWare on Qwadro.

#ifndef ASX_ENGINE_H
#define ASX_ENGINE_H

// A computer simulation (or "sim") is an attempt to model a real-life or hypothetical situation on a computer so that it can be studied to see how the system works. 
// By changing variables in the simulation, predictions may be made about the behaviour of the system. 
// It is a tool to virtually investigate the behaviour of the system under study.

// A space is a purely mathematical representation of a world within space and its interactions.

/*
    Mathematics is a field of study that discovers and organizes abstract objects, methods, theories and theorems that are developed and proved for the needs of empirical sciences and mathematics itself. 
    There are many areas of mathematics, which include number theory (the study of numbers), algebra (the study of formulas and related structures), geometry (the study of shapes and spaces that contain them), 
    analysis (the study of continuous changes), and set theory (presently used as a foundation for all mathematics).
*/

#include "qwadro/exec/afxDevice.h"
#include "qwadro/sim/arxSimBridge.h"

AFX_DEFINE_STRUCT(arxEngineFeatures)
{
    afxBool8    work;
    afxBool8    curves;
    afxBool8    bsplines;
    afxBool8    kinematics;
    afxBool8    rigidBody;
    afxBool8    terrain;
};

AFX_DEFINE_STRUCT(arxEngineLimits)
{
    afxUnit nat;
    afxUnit maxCurvDims; // 9?
};

ASX afxUnit     ArxCountEnginePorts(arxEngine seng);

ASX afxBool     ArxIsEnginePrompt(arxEngine seng);

ASX afxUnit     ArxChooseSimPorts(arxEngine seng, afxSimPortFlags caps, afxAcceleration accel, afxUnit maxCnt, afxUnit portId[]);

// IMPLEMENTATION DISCOVERY ////////////////////////////////////////////////////

ASX afxUnit     ArxInvokeEngines(afxUnit icd, afxUnit first, void* udd, afxBool(*f)(void*,arxEngine), afxUnit cnt);
ASX afxUnit     ArxEvokeEngines(afxUnit icd, afxUnit first, void* udd, afxBool(*flt)(void*,arxEngine), afxUnit cnt, arxEngine devices[]);
ASX afxUnit     ArxEnumerateEngines(afxUnit icd, afxUnit first, afxUnit cnt, arxEngine devices[]);

ASX afxUnit     ArxInvokeSimulations(afxUnit icd, afxUnit first, void *udd, afxBool(*f)(void*,arxSimulation), afxUnit cnt);
ASX afxUnit     ArxEvokeSimulations(afxUnit icd, afxUnit first, void* udd, afxBool(*flt)(void*, arxSimulation), afxUnit cnt, arxSimulation simulations[]);
ASX afxUnit     ArxEnumerateSimulations(afxUnit icd, afxUnit first, afxUnit cnt, arxSimulation simulations[]);

#endif//ASX_ENGINE_H
