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

// This file is part of Acceleration for RenderWare on Qwadro.

#ifndef ASX_SAMPLE_CONTEXT_H
#define ASX_SAMPLE_CONTEXT_H

/*
    In computer simulation, a sample context refers to the specific scenario, environment, 
    or conditions under which a simulation is run or a particular model is applied. It provides 
    the background that helps define how the simulation operates and what data or parameters are being used. 
    The context can encompass factors such as initial conditions, system variables, assumptions, and constraints 
    that affect the results of the simulation.

    In summary, the sample context in a computer simulation refers to the conditions and assumptions that 
    frame the model or system being simulated.
*/

#include "qwadro/sim/arxPoseOps.h"
#include "qwadro/sim/arxMiscOps.h"

ASX afxUnit ArxGetCatalystPort(arxContext ctx);

ASX afxUnit ArxGetCatalystPool(arxContext ctx);

ASX afxError ArxCompileSimCommands(arxContext ctx);

ASX afxError ArxRecycleCatalyst(arxContext ctx, afxBool freeRes);

ASX afxError ArxAcquireSampleContexts(arxSimulation dsys, afxUnit exuIdx, afxUnit queIdx, afxUnit cnt, arxContext batches[]);

#endif//ASX_SAMPLE_CONTEXT_H
