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

#ifndef AFX_DRIVER_H
#define AFX_DRIVER_H

#include "qwadro/inc/base/afxString.h"
#include "qwadro/inc/base/afxFixedString.h"
#include "qwadro/inc/mem/afxArray.h"
#include "qwadro/inc/io/afxUri.h"
#include "qwadro/inc/base/afxManifest.h"
#include "qwadro/inc/base/afxClass.h"
#include "qwadro/inc/exec/afxFence.h"
#include "qwadro/inc/exec/afxSemaphore.h"

AFX afxManifest const*  AfxGetDriverManifest(afxDriver icd);
AFX void                AfxGetDriverInfo(afxDriver icd, afxString* name, afxString* vendor, afxString* website);
AFX void                AfxGetDriverVersion(afxDriver icd, afxNat* verMajor, afxNat* verMinor, afxNat* verPatch);

////////////////////////////////////////////////////////////////////////////////

AFX afxError            _AfxLoadDriver(afxUri const* uri, afxFlags flags, afxDriver* driver);

#endif//AFX_DRIVER_H
