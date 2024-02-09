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

#ifndef AFX_INDEXED_STRING_H
#define AFX_INDEXED_STRING_H

#include "qwadro/core/afxString.h"
#include "qwadro/mem/afxArray.h"

AFX_OBJECT(afxStringCatalog)
{
    afxNat              cnt;
    afxString*          strings;
    afxArray            buf;
};

AFX afxError    AfxCatalogStrings(afxStringCatalog strc, afxNat cnt, afxString const src[], afxNat strIdx[], afxNat strIdxStride);

AFX afxError    AfxResolveStrings(afxStringCatalog strc, afxNat cnt, afxNat strIdx[], afxString dst[]);

AFX afxError    AfxAcquireStringCatalogs(afxNat cnt, afxStringCatalog catalogs[]);

#endif//AFX_INDEXED_STRING_H
