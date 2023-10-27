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

#ifndef AFX_MAP_H
#define AFX_MAP_H

#include "afx/core/afxBitmap.h"

AFX_DEFINE_STRUCT(afxMap)
{
    _AFX_DBG_FCC
    afxNat      unitsPerPage;
    afxNat      keyUnitSiz;
    afxNat      valUnitSiz;
    afxNat      pageCnt;
    afxByte**   pages;
    afxNat32    usage;
    afxResult   (*f)(void*, void*);
    afxContext   mem;
};

AFX void        AfxAcquireMap(afxMap* map, afxNat keyUnitSiz, afxNat valUnitSiz, afxNat unitsPerPage, afxResult(*diff)(void* a, void* b));
AFX void        AfxReleaseMap(afxMap* map);

AFX afxBool     AfxLocateMapUnit(afxMap* map, void* key, afxNat* idx);
AFX afxError    AfxInsertMapKey(afxMap* map, void* key, void* val, afxNat* idx);
AFX afxError    AfxDeleteMapKey(afxMap* map, void* key);

#endif//AFX_MAP_H