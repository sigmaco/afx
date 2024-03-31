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
 *                       (c) 2017 SIGMA, Engineering In Technology
 *                             <https://sigmaco.org/qwadro/>
 */

#ifndef AFX_QWADRO_H
#define AFX_QWADRO_H

// QWADRO: A Modern Library for 4D Data Processing

// QWADRO is an open-source library that supports rapid development of software that deals with 4D data.
// The QWADRO frontend exposes a set of carefully selected data structures and algorithms in C.
// The backend is highly optimized and is set up for parallelization.
// We welcome contributions from the open-source community.

#include "qwadro/core/afxCoreDefs.h"
#include "qwadro/core/afxFcc.h"
#include "qwadro/core/afxChain.h"
#include "qwadro/math/afxSimd.h"
#include "qwadro/core/afxString.h"

#include "qwadro/core/afxMutex.h"
#include "qwadro/core/afxThread.h"
#include "qwadro/core/afxTime.h"
#include "qwadro/core/afxTxu.h"
#include "qwadro/core/afxCondition.h"

#include "qwadro/core/afxManager.h"
#include "qwadro/core/afxObject.h"

#include "qwadro/io/afxStream.h"
#include "qwadro/io/afxUri.h"

#include "qwadro/mem/afxArray.h"
#include "qwadro/mem/afxMmu.h"

#include "qwadro/math/afxMathDefs.h"
#include "qwadro/math/afxAabb.h"
#include "qwadro/math/afxFrustum.h"
#include "qwadro/math/afxPlane.h"
#include "qwadro/math/afxSphere.h"
#include "qwadro/math/afxVolume.h"

#include "qwadro/core/afxDebug.h"
#include "qwadro/core/afxSystem.h"
#include "qwadro/core/afxTerminal.h"


#include "qwadro/draw/afxDrawSystem.h"
#include "qwadro/sound/afxSoundSystem.h"
#include "qwadro/ux/afxApplication.h"

#endif//AFX_QWADRO_H
