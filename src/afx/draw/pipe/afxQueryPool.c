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
 *                       (c) 2017 SIGMA, Engitech, Scitech, Serpro
 *                             <https://sigmaco.org/qwadro/>
 */

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#define _AVX_DRAW_C
#define _AVX_QUERY_POOL_C
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
    afxDrawContext dctx = AfxGetObjectProvider(qryp);
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    return dctx;
}

////////////////////////////////////////////////////////////////////////////////

_AVX afxError AfxAcquireQueryPools(afxDrawContext dctx, afxQueryType type, afxNat cap, afxNat cnt, afxQueryPool quer[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    afxManager* cls = AfxGetQueryPoolClass(dctx);
    AfxAssertClass(cls, afxFcc_QRYP);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)quer, (void const*[]) { dctx, &type, &cap }))
        AfxThrowError();

    return err;
}
