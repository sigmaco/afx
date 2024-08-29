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

#include "qwadro/base/afxDebug.h"
#include "qwadro/base/afxFcc.h"
#include "qwadro/mem/afxMemory.h"
#include "qwadro/mem/afxMappedString.h"
#include "qwadro/io/afxUri.h"

AFX_DEFINE_STRUCT(afxManifest)
{
    afxNat              pageCnt;
    struct _afxIniPage* pages;
};

AFX afxError    AfxSetUpIni(afxManifest* ini);
AFX void        AfxCleanUpIni(afxManifest* ini);

AFX afxError    AfxIniLoadFromFile(afxManifest* ini, afxUri const* uri);
AFX afxError    AfxIniSaveToFile(afxManifest const* ini, afxUri const* uri);

AFX afxBool     AfxIniGetNat(afxManifest const* ini, afxString const* page, afxString const* key, afxNat* value);
AFX afxBool     AfxIniGetInt(afxManifest const* ini, afxString const* page, afxString const* key, afxInt* value);
AFX afxBool     AfxIniGetReal(afxManifest const* ini, afxString const* page, afxString const* key, afxReal* value);
AFX afxBool     AfxIniGetReal64(afxManifest const* ini, afxString const* page, afxString const* key, afxReal64* value);
AFX afxBool     AfxIniGetBool(afxManifest const* ini, afxString const* page, afxString const* key, afxBool* value);
AFX afxBool     AfxIniGetString(afxManifest const* ini, afxString const* page, afxString const* key, afxString* value);
AFX afxBool     AfxIniGetUri(afxManifest const* ini, afxString const* page, afxString const* key, afxUri* value);

AFX afxNat      AfxCountManifestPages(afxManifest const* ini);
AFX afxNat      AfxCountManifestRecords(afxManifest const* ini, afxNat pagIdx);

AFX afxBool     AfxFindManifestPage(afxManifest const* ini, afxString const* name, afxNat* pagIdx);
AFX afxBool     AfxFindManifestRecord(afxManifest const* ini, afxNat pagIdx, afxString const* name, afxNat* recIdx);

AFX afxBool     AfxGetManifestString(afxManifest const* ini, afxNat pagIdx, afxNat recIdx, afxString* value);
AFX afxBool     AfxGetManifestUri(afxManifest const* ini, afxNat pagIdx, afxNat recIdx, afxUri* uri);
AFX afxBool     AfxGetManifestNat(afxManifest const* ini, afxNat pagIdx, afxNat recIdx, afxNat* value);

AFX afxBool     AfxIniGetStringIndexed(afxManifest const* ini, afxNat pagIdx, afxString const* key, afxString* value);
AFX afxBool     AfxIniGetUriIndexed(afxManifest const* ini, afxNat pagIdx, afxString const* key, afxUri* value);
AFX afxBool     AfxIniGetNatIndexed(afxManifest const* ini, afxNat pagIdx, afxString const* key, afxNat* value);
AFX afxBool     AfxIniGetIntIndexed(afxManifest const* ini, afxNat pagIdx, afxString const* key, afxInt* value);
AFX afxBool     AfxIniGetRealIndexed(afxManifest const* ini, afxNat pagIdx, afxString const* key, afxReal* value);
AFX afxBool     AfxIniGetReal64Indexed(afxManifest const* ini, afxNat pagIdx, afxString const* key, afxReal64* value);
AFX afxBool     AfxIniGetBoolIndexed(afxManifest const* ini, afxNat pagIdx, afxString const* key, afxBool* value);

AFX afxBool     AfxIniHasKey(afxManifest const* ini, afxString const* page, afxString const* key);

AFX afxError    AfxIniSetString(afxManifest* ini, afxString const* page, afxString const* key, afxString* value);

AFX afxBool     AfxQueryProfileString(afxManifest* ini, afxString const* sec, afxString const* key, afxString const* def, afxString* value);
AFX afxBool     AfxQueryProfileUri(afxManifest* ini, afxString const* sec, afxString const* key, afxUri const* def, afxUri* value);

AFX afxBool     AfxQueryProfileReal(afxManifest* ini, afxString const* sec, afxString const* key, afxReal const* def, afxReal* value);
AFX afxBool     AfxQueryProfileBool(afxManifest* ini, afxString const* sec, afxString const* key, afxBool const* def, afxBool* value);
AFX afxBool     AfxQueryProfileNat(afxManifest* ini, afxString const* sec, afxString const* key, afxNat const* def, afxNat* value);
AFX afxBool     AfxQueryProfileInt(afxManifest* ini, afxString const* sec, afxString const* key, afxInt const* def, afxInt* value);

#endif//AFX_MANIFEST_H
