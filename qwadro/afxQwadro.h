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

 // QWADRO is an open-source coalition of libraries that supports rapid development of software that deals with 4D data.
 // The QWADRO frontend exposes a set of carefully selected data structures and algorithms in C.
 // The backend is highly optimized and is set up for parallelization.
 // We welcome contributions from the open-source community.

// Qwadro; experience what is inside.

/*
    CODING APPROACHES

    That's a clear and performance-oriented design philosophy. Under Qwadro’s guidelines, 
    we don't do defensive programming for required parameters at runtime; we assume correct usage in release builds, 
    and assert in debug to catch contract violations early.
    This approach keeps the API fast and clean, trusting the caller, while still making bugs obvious and traceable during development. 
    Errors will be catched during integration or testing rather than letting them silently break memory or logic.

    Behave as a low-level API should behave. Explicit when needed, implicit when allowed.

    This system is designed for high-performance situations.

    This system must provide manual, batch-based control over memory and data.

    This system must avoid hidden reallocations or copying - everything is explicit.

    Several things in this framework seems to be batch- and range-aware, so acting in a specific subrange of units is totally in line with its philosophy.

    Qwadro does exhaustive use of the "commit only on full success" behavior, similar to a transactional allocation model.
    It implies that batch operations, usually memory allocations, must undo previous successfull operations if any of them has failed.

*/

#ifndef AFX_QWADRO_H
#define AFX_QWADRO_H

// AFX
#include "qwadro/exec/afxSystem.h"
// AVX
#include "qwadro/draw/afxDrawSystem.h"
// AMX
#include "qwadro/mix/afxMixSystem.h"
// AUX
#include "qwadro/ux/afxShell.h"
#if !0
// ARX
#include "qwadro/scene/arxScenario.h"
// ASX
#include "qwadro/sim/arxSimulation.h"
#endif

#endif//AFX_QWADRO_H
