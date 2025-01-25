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

// A computer simulation (or "sim") is an attempt to model a real-life or hypothetical situation on a computer so that it can be studied to see how the system works. 
// By changing variables in the simulation, predictions may be made about the behaviour of the system. 
// It is a tool to virtually investigate the behaviour of the system under study.

// A space is a purely mathematical representation of a world within space and its interactions.

/*
    Mathematics is a field of study that discovers and organizes abstract objects, methods, theories and theorems that are developed and proved for the needs of empirical sciences and mathematics itself. 
    There are many areas of mathematics, which include number theory (the study of numbers), algebra (the study of formulas and related structures), geometry (the study of shapes and spaces that contain them), 
    analysis (the study of continuous changes), and set theory (presently used as a foundation for all mathematics).
*/

#ifndef ASX_ENGINE_H
#define ASX_ENGINE_H

#include "qwadro/inc/exec/afxDevice.h"
#include "qwadro/inc/sim/afxSimBridge.h"

AFX_DEFINE_STRUCT(afxEngineFeatures)
{
    afxBool8    work;
    afxBool8    curves;
    afxBool8    bsplines;
    afxBool8    kinematics;
    afxBool8    rigidBody;
    afxBool8    terrain;
};

AFX_DEFINE_STRUCT(afxEngineLimits)
{
    afxUnit nat;
    afxUnit maxCurvDims; // 9?
};

ASX afxUnit     AfxCountEnginePorts(afxEngine seng);

ASX afxBool     AfxIsEnginePrompt(afxEngine seng);

ASX afxUnit     AfxChooseSimPorts(afxEngine seng, afxSimPortFlags caps, afxAcceleration accel, afxUnit maxCnt, afxUnit portId[]);

// IMPLEMENTATION DISCOVERY ////////////////////////////////////////////////////

ASX afxUnit     AfxInvokeEngines(afxUnit icd, afxUnit first, void* udd, afxBool(*f)(void*,afxEngine), afxUnit cnt);
ASX afxUnit     AfxEvokeEngines(afxUnit icd, afxUnit first, void* udd, afxBool(*flt)(void*,afxEngine), afxUnit cnt, afxEngine devices[]);
ASX afxUnit     AfxEnumerateEngines(afxUnit icd, afxUnit first, afxUnit cnt, afxEngine devices[]);

ASX afxUnit     AfxInvokeSimulations(afxUnit icd, afxUnit first, void *udd, afxBool(*f)(void*,afxSimulation), afxUnit cnt);
ASX afxUnit     AfxEvokeSimulations(afxUnit icd, afxUnit first, void* udd, afxBool(*flt)(void*, afxSimulation), afxUnit cnt, afxSimulation simulations[]);
ASX afxUnit     AfxEnumerateSimulations(afxUnit icd, afxUnit first, afxUnit cnt, afxSimulation simulations[]);

#endif//ASX_ENGINE_H
