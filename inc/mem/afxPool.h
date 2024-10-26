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

#define AFX_POOL_ALIGNMENT AFX_SIMD_ALIGNMENT

AFX_DEFINE_STRUCT(afxPoolPage)
{
    afxUnit          usedCnt;
    afxUnit32        usage;
    afxByte*        data;
};

AFX_DEFINE_STRUCT(afxPool)
{
    afxUnit          unitSiz;
    afxUnit          memAlign;
    afxUnit          unitsPerPage;
    afxUnit          totalUsedCnt;
    afxUnit          pageCnt;
    afxPoolPage*    pages;
    afxMmu          mem;
};

AFX void        AfxSetUpPool(afxPool* pool, afxUnit unitSiz, afxUnit unitsPerPage, afxUnit memAlign);
AFX void        AfxCleanUpPool(afxPool* pool);

AFX void*       AfxAllocatePoolUnit(afxPool* pool, afxSize* idx);
AFX afxError    AfxAllocatePoolUnits(afxPool* pool, afxUnit cnt, void* units[]);

AFX void        AfxDeallocatePoolUnit(afxPool* pool, void* unit);
AFX void        AfxDeallocatePoolUnits(afxPool* pool, afxUnit cnt, void* units[]);

AFX afxBool     AfxGetPoolItem(afxPool const* pool, afxSize idx, void **ptr);
AFX afxBool     AfxGetPoolUnit(afxPool const* pool, afxSize idx, void **ptr);
AFX afxBool     AfxGetPoolUnits(afxPool const* pool, afxUnit cnt, afxSize const idx[], void *ptr[]);
AFX afxBool     AfxGetLinearPoolUnits(afxPool const* pool, afxUnit first, afxUnit cnt, void *ptr[]);

AFX afxBool     AfxFindPoolUnitIndex(afxPool* pool, void* unit, afxUnit* idx, afxUnit* localIdx);
AFX afxError    AfxOccupyPoolUnit(afxPool* pool, afxSize idx, void *val);

AFX afxUnit      AfxEnumeratePoolItems(afxPool const* pool, afxUnit first, afxUnit cnt, void *items[]);
AFX afxUnit      AfxEnumeratePoolUnits(afxPool const* pool, afxUnit first, afxUnit cnt, afxBool freeOnly, void *items[]);

AFX afxUnit      AfxInvokePoolItems(afxPool const* pool, afxUnit first, afxUnit cnt, afxBool(*f)(void* item, void* udd), void *udd);
AFX afxUnit      AfxInvokePoolUnits(afxPool const* pool, afxUnit first, afxUnit cnt, afxBool freeOnly, afxBool (*f)(void* item, void* udd), void *udd);

#endif//AFX_POOL_H
