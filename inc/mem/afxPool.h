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

#ifndef AFX_POOL_H
#define AFX_POOL_H

#include "qwadro/inc/base/afxDebug.h"
#include "qwadro/inc/base/afxFcc.h"
#include "qwadro/inc/mem/afxMemory.h"

AFX_DEFINE_STRUCT(afxPoolPage)
{
    afxNat          usedCnt;
    afxNat32        usage;
    afxByte*        data;
};

AFX_DEFINE_STRUCT(afxPool)
{
    afxNat          unitSiz;
    afxNat          unitsPerPage;
    afxNat          totalUsedCnt;
    afxNat          pageCnt;
    afxPoolPage*    pages;
    afxMmu          mem;
};

AFX void        AfxSetUpPool(afxPool* pool, afxNat unitSiz, afxNat unitsPerPage);
AFX void        AfxCleanUpPool(afxPool* pool);

AFX void*       AfxAllocatePoolUnit(afxPool* pool, afxSize* idx);
AFX afxError    AfxAllocatePoolUnits(afxPool* pool, afxNat cnt, void* units[]);

AFX void        AfxDeallocatePoolUnit(afxPool* pool, void* unit);
AFX void        AfxDeallocatePoolUnits(afxPool* pool, afxNat cnt, void* units[]);

AFX afxBool     AfxGetPoolItem(afxPool const* pool, afxSize idx, void **ptr);
AFX afxBool     AfxGetPoolUnit(afxPool const* pool, afxSize idx, void **ptr);
AFX afxBool     AfxGetPoolUnits(afxPool const* pool, afxNat cnt, afxSize const idx[], void *ptr[]);
AFX afxBool     AfxGetLinearPoolUnits(afxPool const* pool, afxNat first, afxNat cnt, void *ptr[]);

AFX afxBool     AfxFindPoolUnitIndex(afxPool* pool, void* unit, afxNat* idx, afxNat* localIdx);
AFX afxError    AfxOccupyPoolUnit(afxPool* pool, afxSize idx, void *val);

AFX afxNat      AfxEnumeratePoolItems(afxPool const* pool, afxNat first, afxNat cnt, void *items[]);
AFX afxNat      AfxEnumeratePoolUnits(afxPool const* pool, afxNat first, afxNat cnt, afxBool freeOnly, void *items[]);

AFX afxNat      AfxInvokePoolItems(afxPool const* pool, afxNat first, afxNat cnt, afxBool(*f)(void* item, void* udd), void *udd);
AFX afxNat      AfxInvokePoolUnits(afxPool const* pool, afxNat first, afxNat cnt, afxBool freeOnly, afxBool (*f)(void* item, void* udd), void *udd);

#endif//AFX_POOL_H
