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

AFX afxError    AfxAllocatePoolUnit(afxPool* pool, afxSize* idx);
AFX afxError    AfxAllocatePoolUnits(afxPool* pool, afxNat cnt, afxSize idx[]);

AFX void        AfxDeallocatePoolUnit(afxPool* pool, afxSize idx);
AFX void        AfxDeallocatePoolUnits(afxPool* pool, afxNat cnt, afxSize const idx[]);

AFX afxBool     AfxGetPoolItem(afxPool const* pool, afxSize idx, void **ptr);
AFX afxBool     AfxGetPoolUnit(afxPool const* pool, afxSize idx, void **ptr);
AFX afxBool     AfxGetPoolUnits(afxPool const* pool, afxNat cnt, afxSize const idx[], void *ptr[]);
AFX afxBool     AfxGetLinearPoolUnits(afxPool const* pool, afxNat first, afxNat cnt, void *ptr[]);

AFX afxError    AfxOccupyPoolUnit(afxPool* pool, afxSize idx, void *val);

AFX afxNat      AfxEnumeratePoolItems(afxPool const* pool, afxNat first, afxNat cnt, void *items[]);
AFX afxNat      AfxEnumeratePoolUnits(afxPool const* pool, afxNat first, afxNat cnt, afxBool freeOnly, void *items[]);

AFX afxNat      AfxCuratePoolItems(afxPool const* pool, afxNat first, afxNat cnt, afxBool(*f)(void* item, void* udd), void *udd);
AFX afxNat      AfxCuratePoolUnits(afxPool const* pool, afxNat first, afxNat cnt, afxBool freeOnly, afxBool (*f)(void* item, void* udd), void *udd);

#endif//AFX_POOL_H