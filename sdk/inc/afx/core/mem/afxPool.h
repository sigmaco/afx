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

#ifndef AFX_POOL_H
#define AFX_POOL_H

#include "afx/core/diag/afxDebug.h"
#include "afx/core/afxFcc.h"
#include "afx/core/mem/afxContext.h"

AFX_DEFINE_STRUCT(afxPoolPage)
{
    afxNat32        usage;
    afxNat          usedCnt;
    afxByte*        bytes;
};

AFX_DEFINE_STRUCT(afxPool)
{
    _AFX_DBG_FCC
    afxNat          unitSiz;
    afxNat          unitsPerPage;
    afxNat          totalUsedCnt;
    afxNat          pageCnt;
    afxPoolPage*    pages;
    afxContext       mem;
};

AFX void        AfxAcquirePool(afxPool* pool, afxNat unitSiz, afxNat unitsPerPage);
AFX void        AfxReleasePool(afxPool* pool);

AFX afxError    AfxRequestPoolUnit(afxPool* pool, afxNat* idx);
AFX void        AfxRecyclePoolUnit(afxPool* pool, afxNat idx);
AFX afxError    AfxOccupyPoolUnit(afxPool* pool, afxNat idx, void *val);
AFX afxBool     AfxGetPoolUnit(afxPool const* pool, afxNat idx, void **ptr);

AFX afxNat      AfxEnumeratePoolItems(afxPool* pool, afxNat first, afxNat cnt, void *items[]);
AFX afxNat      AfxEnumeratePoolUnits(afxPool* pool, afxNat first, afxNat cnt, afxBool freeOnly, void *items[]);

AFX afxNat      AfxCuratePoolItems(afxPool* pool, afxNat first, afxNat cnt, afxBool(*f)(void* item, void* udd), void *udd);
AFX afxNat      AfxCuratePoolUnits(afxPool* pool, afxNat first, afxNat cnt, afxBool freeOnly, afxBool (*f)(void* item, void* udd), void *udd);

#endif//AFX_POOL_H