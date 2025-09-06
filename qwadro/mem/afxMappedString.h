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

#include "qwadro/base/afxString.h"
#include "qwadro/mem/afxArray.h"

AFX_DEFINE_STRUCT(afxReferencedString)
{
    afxLink          strb;
    afxString           str; // stores just a offset into string arena
    afxUnit              refCnt;
    afxChar             data[];
};

AFX_OBJECT(afxStringBase)
{
    afxChain            strings;
    afxReferencedString*first;
};

AFX afxUnit      AfxCatalogStrings2(afxStringBase strc, afxUnit cnt, afxString const in[], afxString out[]);

AFX afxUnit      AfxResolveStrings2(afxStringBase strc, afxUnit cnt, afxString const in[], afxString out[]);

AFX afxError    AfxAcquireStringCatalogs(afxUnit cnt, afxStringBase catalogs[]);

AFX afxUnit      AfxDecatalogStrings(afxStringBase strc, afxUnit cnt, afxString const strings[]);
AFX afxUnit      AfxCatalogStrings(afxStringBase strc, afxUnit cnt, afxString const in[], afxString out[]);

#endif//AFX_INDEXED_STRING_H
