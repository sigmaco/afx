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

#define _AFX_DRAW_C
#define _AFX_QUERY_POOL_C
#include "qwadro/draw/afxDrawSystem.h"

AVX afxResult AfxGetQueryResults(afxQueryPool pool, afxNat baseQuery, afxNat queryCnt, void* dst, afxSize cap, afxSize stride, afxQueryResultFlags flags)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &pool, afxFcc_QRYP);
    AfxAssertRange(pool->cap, baseQuery, queryCnt);
    AfxAssert(dst);
    AfxAssert(cap);
    return 0;
}

AVX void AfxResetQueries(afxQueryPool pool, afxNat baseQuery, afxNat queryCnt)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &pool, afxFcc_QRYP);
    AfxAssertRange(pool->cap, baseQuery, queryCnt);

}

_AVX afxDrawContext AfxGetQueryPoolContext(afxQueryPool qryp)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &qryp, afxFcc_QRYP);
    afxDrawContext dctx = AfxGetLinker(&qryp->dctx);
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    return dctx;
}

////////////////////////////////////////////////////////////////////////////////

_AVX afxError AfxAcquireQueryPools(afxDrawContext dctx, afxQueryType type, afxNat cap, afxNat cnt, afxQueryPool quer[])
{
    afxError err = AFX_ERR_NONE;

    afxDrawDevice ddev = AfxGetDrawContextDevice(dctx);
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    afxManager* cls = AfxGetQueryPoolClass(ddev);
    AfxAssertClass(cls, afxFcc_QRYP);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)quer, (void const*[]) { ddev, dctx, &type, &cap }))
        AfxThrowError();

    return err;
}
