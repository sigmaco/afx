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
 *                   (c) 2017 SIGMA Technology Group — Federação SIGMA
 *                                    www.sigmaco.org
 */

#ifndef AFX_QWADRO_H
#define AFX_QWADRO_H

// QWADRO: A Modern Library for 4D Data Processing

// QWADRO is an open-source library that supports rapid development of software that deals with 4D data.
// The QWADRO frontend exposes a set of carefully selected data structures and algorithms in C.
// The backend is highly optimized and is set up for parallelization.
// We welcome contributions from the open-source community.


#include "qwadro/async/afxCondition.h"
#include "qwadro/async/afxMutex.h"
#include "qwadro/async/afxThread.h"

#include "qwadro/core/afxCoreDefs.h"
#include "qwadro/core/afxFcc.h"
#include "qwadro/core/afxClass.h"
#include "qwadro/core/afxChain.h"
#include "qwadro/core/afxObject.h"
#include "qwadro/core/afxSimd.h"
#include "qwadro/core/afxString.h"

#include "qwadro/core/afxDebug.h"

#include "qwadro/io/afxStream.h"
#include "qwadro/io/afxUri.h"

#include "qwadro/mem/afxMmu.h"
#include "qwadro/mem/afxArray.h"

#include "qwadro/async/afxTime.h"

#include "qwadro/ux/afxApplication.h"
#include "qwadro/core/afxTerminal.h"
#include "qwadro/core/afxSystem.h"


#include "qwadro/draw/afxDrawContext.h"
#include "qwadro/draw/afxDrawInput.h"
#include "qwadro/draw/afxDrawOutput.h"
#include "qwadro/draw/afxDrawSystem.h"

#include "qwadro/math/afxMathDefs.h"
#include "qwadro/math/afxAabb.h"
#include "qwadro/math/afxFrustum.h"
#include "qwadro/math/afxPlane.h"
#include "qwadro/math/afxSphere.h"
#include "qwadro/math/afxVolume.h"

//#include "qwadro/sound/aaxSoundSystem.h"

#include "qwadro/ux/afxWidget.h"

#endif//AFX_QWADRO_H
