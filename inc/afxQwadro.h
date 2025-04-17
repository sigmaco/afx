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
#include "qwadro/inc/exec/afxSystem.h"
// AVX
#include "qwadro/inc/draw/afxDrawSystem.h"
// ASX
#include "qwadro/inc/mix/afxMixSystem.h"
// ASX
#include "qwadro/inc/sim/afxSimulation.h"
// AUX
#include "qwadro/inc/ux/afxShell.h"
// ASX
#include "qwadro/inc/sim/afxSimulation.h"

#endif//AFX_QWADRO_H
