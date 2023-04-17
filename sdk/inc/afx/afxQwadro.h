/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                      S I G M A   T E C H N O L O G Y   G R O U P
 *
 *                                   Public Test Build
 *                      (c) 2017 SIGMA Co. & SIGMA Technology Group
 *                                    www.sigmaco.org
 */

#ifndef AFX_QWADRO_H
#define AFX_QWADRO_H

// QWADRO: A Modern Library for 4D Data Processing

// QWADRO is an open-source library that supports rapid development of software that deals with 4D data.
// The QWADRO frontend exposes a set of carefully selected data structures and algorithms in C.
// The backend is highly optimized and is set up for parallelization.
// We welcome contributions from the open-source community.


#include "core/async/afxCondition.h"
#include "core/async/afxMutex.h"
#include "core/async/afxThread.h"

#include "core/base/afxCoreDefs.h"
#include "core/base/afxFcc.h"
#include "core/base/afxLinkedList.h"
#include "core/base/afxLinkedList.h"
#include "core/base/afxObject.h"
#include "core/base/afxSimd.h"
#include "core/base/afxString.h"

#include "core/diag/afxDebug.h"

#include "core/afxSystem.h"
#include "core/io/afxStream.h"
#include "core/io/afxUri.h"

#include "core/mem/afxAllocator.h"
#include "core/mem/afxArray.h"
#include "core/mem/afxMemory.h"

#include "core/time/afxTime.h"

#include "afxApplication.h"
#include "core/afxTerminal.h"

#include "draw/afxDrawContext.h"
#include "draw/afxDrawInput.h"
#include "draw/afxDrawOutput.h"
#include "draw/afxDrawSystem.h"

#include "math/afxMatrix.h"
#include "math/afxQuat.h"
#include "math/afxVector.h"
#include "math/volume/afxAabb.h"
#include "math/volume/afxFrustum.h"
#include "math/volume/afxPlane.h"
#include "math/volume/afxSphere.h"
#include "math/volume/afxVolume.h"

//#include "sound/afxSoundSystem.h"

#include "mmux/afxWidget.h"

#endif//AFX_QWADRO_H