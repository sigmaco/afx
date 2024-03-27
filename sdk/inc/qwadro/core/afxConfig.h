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

/// Initialization file handling API.
/// In Qwadro, .ini files are open in a differently way of other files.
/// Here, it will always look for a file firstly in the process working directory, and, if not found, in Qwadro system directory.

#ifndef AFX_INI_H
#define AFX_INI_H

#include "qwadro/core/afxDebug.h"
#include "qwadro/core/afxFcc.h"
#include "qwadro/mem/afxMmu.h"
#include "qwadro/io/afxUri.h"

AFX_DEFINE_STRUCT(afxIni)
{
    afxNat              pageCnt;
    struct _afxIniPage* pages;
};

AFX afxError    AfxSetUpIni(afxIni* ini);
AFX void        AfxCleanUpIni(afxIni* ini);

AFX afxError    AfxIniLoadFromFile(afxIni* ini, afxUri const* uri);
AFX afxError    AfxIniSaveToFile(afxIni const* ini, afxUri const* uri);

AFX afxBool     AfxIniGetNat(afxIni const* ini, afxString const* page, afxString const* key, afxNat* value);
AFX afxBool     AfxIniGetInt(afxIni const* ini, afxString const* page, afxString const* key, afxInt* value);
AFX afxBool     AfxIniGetReal(afxIni const* ini, afxString const* page, afxString const* key, afxReal* value);
AFX afxBool     AfxIniGetReal64(afxIni const* ini, afxString const* page, afxString const* key, afxReal64* value);
AFX afxBool     AfxIniGetBool(afxIni const* ini, afxString const* page, afxString const* key, afxBool* value);
AFX afxBool     AfxIniGetString(afxIni const* ini, afxString const* page, afxString const* key, afxString* value);
AFX afxBool     AfxIniGetUri(afxIni const* ini, afxString const* page, afxString const* key, afxUri* value);

AFX afxNat      AfxIniCountPages(afxIni const* ini);
AFX afxNat      AfxIniCountKeys(afxIni const* ini, afxNat pagIdx);

AFX afxBool     AfxIniGetStringIndexed(afxIni const* ini, afxNat pagIdx, afxString const* key, afxString* value);
AFX afxBool     AfxIniGetUriIndexed(afxIni const* ini, afxNat pagIdx, afxString const* key, afxUri* value);
AFX afxBool     AfxIniGetNatIndexed(afxIni const* ini, afxNat pagIdx, afxString const* key, afxNat* value);
AFX afxBool     AfxIniGetIntIndexed(afxIni const* ini, afxNat pagIdx, afxString const* key, afxInt* value);
AFX afxBool     AfxIniGetRealIndexed(afxIni const* ini, afxNat pagIdx, afxString const* key, afxReal* value);
AFX afxBool     AfxIniGetReal64Indexed(afxIni const* ini, afxNat pagIdx, afxString const* key, afxReal64* value);
AFX afxBool     AfxIniGetBoolIndexed(afxIni const* ini, afxNat pagIdx, afxString const* key, afxBool* value);

AFX afxBool     AfxIniHasKey(afxIni const* ini, afxString const* page, afxString const* key);

AFX afxError    AfxIniSetString(afxIni* ini, afxString const* page, afxString const* key, afxString* value);

#endif//AFX_INI_H
