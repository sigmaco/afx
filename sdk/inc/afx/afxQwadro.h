/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *              T H E   Q W A D R O   E X E C U T I O N   E C O S Y S T E M
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


#include "afx/core/async/afxCondition.h"
#include "afx/core/async/afxMutex.h"
#include "afx/core/async/afxThread.h"

#include "afx/core/afxCoreDefs.h"
#include "afx/core/afxFcc.h"
#include "afx/core/afxChain.h"
#include "afx/core/afxChain.h"
#include "afx/core/afxInstance.h"
#include "afx/core/afxSimd.h"
#include "afx/core/afxString.h"

#include "afx/core/diag/afxDebug.h"

#include "afx/core/afxSystem.h"
#include "afx/core/io/afxStream.h"
#include "afx/core/io/afxUri.h"

#include "afx/core/mem/afxContext.h"
#include "afx/core/mem/afxArray.h"
#include "afx/core/mem/afxContext.h"

#include "afx/core/time/afxTime.h"

#include "afx/core/afxApplication.h"
#include "afx/core/afxTerminal.h"

#include "afx/draw/afxDrawContext.h"
#include "afx/draw/afxDrawInput.h"
#include "afx/draw/afxDrawOutput.h"
#include "afx/draw/afxDrawSystem.h"

#include "afx/math/afxMathDefs.h"
#include "afx/math/volume/afxAabb.h"
#include "afx/math/volume/afxFrustum.h"
#include "afx/math/volume/afxPlane.h"
#include "afx/math/volume/afxSphere.h"
#include "afx/math/volume/afxVolume.h"

//#include "afx/sound/afxSoundSystem.h"

#include "afx/mmux/afxWidget.h"

#endif//AFX_QWADRO_H