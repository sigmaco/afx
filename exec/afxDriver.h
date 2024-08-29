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

#include "qwadro/base/afxString.h"
#include "qwadro/base/afxFixedString.h"
#include "qwadro/mem/afxArray.h"
#include "qwadro/io/afxUri.h"
#include "qwadro/base/afxManifest.h"
#include "qwadro/base/afxClass.h"
#include "qwadro/exec/afxFence.h"
#include "qwadro/exec/afxSemaphore.h"

AFX afxManifest const*  AfxGetDriverManifest(afxDriver icd);
AFX void                AfxGetDriverInfo(afxDriver icd, afxString* name, afxString* vendor, afxString* website);
AFX void                AfxGetDriverVersion(afxDriver icd, afxNat* verMajor, afxNat* verMinor, afxNat* verPatch);

////////////////////////////////////////////////////////////////////////////////

AFX afxError            _AfxLoadDriver(afxUri const* uri, afxFlags flags, afxDriver* driver);

#endif//AFX_DRIVER_H
