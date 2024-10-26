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

/// Initialization file handling API.
/// In Qwadro, .ini files are open in a differently way of other files.
/// Here, it will always look for a file firstly in the process working directory, and, if not found, in Qwadro system directory.

#ifndef AFX_MANIFEST_H
#define AFX_MANIFEST_H

#include "qwadro/inc/base/afxDebug.h"
#include "qwadro/inc/base/afxFcc.h"
#include "qwadro/inc/mem/afxMemory.h"
#include "qwadro/inc/mem/afxMappedString.h"
#include "qwadro/inc/io/afxUri.h"

AFX_DEFINE_STRUCT(afxManifest)
{
    afxUnit              pageCnt;
    struct _afxIniPage* pages;
};

AFX afxError    AfxDeployManifest(afxManifest* ini);
AFX void        AfxDismantleManifest(afxManifest* ini);

AFX afxError    AfxLoadInitializationFile(afxManifest* ini, afxUri const* uri);
AFX afxError    AfxStoreInitializationFile(afxManifest const* ini, afxUri const* uri);

AFX afxBool     AfxGetInitializationNat(afxManifest const* ini, afxString const* sec, afxString const* key, afxUnit* value);
AFX afxBool     AfxGetInitializationInt(afxManifest const* ini, afxString const* sec, afxString const* key, afxInt* value);
AFX afxBool     AfxGetInitializationReal(afxManifest const* ini, afxString const* sec, afxString const* key, afxReal* value);
AFX afxBool     AfxGetInitializationReal64(afxManifest const* ini, afxString const* sec, afxString const* key, afxReal64* value);
AFX afxBool     AfxGetInitializationBool(afxManifest const* ini, afxString const* sec, afxString const* key, afxBool* value);
AFX afxBool     AfxGetInitializationString(afxManifest const* ini, afxString const* sec, afxString const* key, afxString* value);
AFX afxBool     AfxGetInitializationUri(afxManifest const* ini, afxString const* sec, afxString const* key, afxUri* value);

AFX afxUnit     AfxCountInitializationSections(afxManifest const* ini);
AFX afxUnit     AfxCountInitializationRecords(afxManifest const* ini, afxUnit secIdx);

AFX afxBool     AfxFindInitializationSection(afxManifest const* ini, afxString const* name, afxUnit* secIdx);
AFX afxBool     AfxFindInitializationRecord(afxManifest const* ini, afxUnit secIdx, afxString const* name, afxUnit* recIdx);

AFX afxBool     AfxGetManifestString(afxManifest const* ini, afxUnit secIdx, afxUnit recIdx, afxString* value);
AFX afxBool     AfxGetManifestUri(afxManifest const* ini, afxUnit secIdx, afxUnit recIdx, afxUri* uri);
AFX afxBool     AfxGetManifestNat(afxManifest const* ini, afxUnit secIdx, afxUnit recIdx, afxUnit* value);

AFX afxBool     AfxGetInitializationStringAt(afxManifest const* ini, afxUnit secIdx, afxString const* key, afxString* value);
AFX afxBool     AfxGetInitializationUriAt(afxManifest const* ini, afxUnit secIdx, afxString const* key, afxUri* value);
AFX afxBool     AfxGetInitializationNatAt(afxManifest const* ini, afxUnit secIdx, afxString const* key, afxUnit* value);
AFX afxBool     AfxGetInitializationIntAt(afxManifest const* ini, afxUnit secIdx, afxString const* key, afxInt* value);
AFX afxBool     AfxGetInitializationRealAt(afxManifest const* ini, afxUnit secIdx, afxString const* key, afxReal* value);
AFX afxBool     AfxGetInitializationReal64At(afxManifest const* ini, afxUnit secIdx, afxString const* key, afxReal64* value);
AFX afxBool     AfxGetInitializationBoolAt(afxManifest const* ini, afxUnit secIdx, afxString const* key, afxBool* value);

AFX afxError    AfxSetInitializationString(afxManifest* ini, afxString const* page, afxString const* key, afxString* value);

#endif//AFX_MANIFEST_H
