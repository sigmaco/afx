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

#ifndef AFX_INDEXED_STRING_H
#define AFX_INDEXED_STRING_H

#include "qwadro/inc/base/afxString.h"
#include "qwadro/inc/mem/afxArray.h"

AFX_DEFINE_STRUCT(afxReferencedString)
{
    afxLinkage          strb;
    afxString           str; // stores just a offset into string arena
    afxNat              refCnt;
    afxChar             data[];
};

AFX_OBJECT(afxStringBase)
{
    afxChain            strings;
    afxReferencedString*first;
};

AFX afxNat      AfxCatalogStrings2(afxStringBase strc, afxNat cnt, afxString const in[], afxString out[]);

AFX afxNat      AfxResolveStrings2(afxStringBase strc, afxNat cnt, afxString const in[], afxString out[]);

AFX afxError    AfxAcquireStringCatalogs(afxNat cnt, afxStringBase catalogs[]);

AFX afxNat      AfxDecatalogStrings(afxStringBase strc, afxNat cnt, afxString const strings[]);
AFX afxNat      AfxCatalogStrings(afxStringBase strc, afxNat cnt, afxString const in[], afxString out[]);

#endif//AFX_INDEXED_STRING_H
