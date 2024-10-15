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

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#define _AVX_DRAW_C
#define _AVX_QUERY_POOL_C
#include "../../dev/AvxImplKit.h"

AVX afxResult AfxGetQueryResults(avxQueryPool qryp, afxNat baseQuery, afxNat queryCnt, void* dst, afxSize cap, afxSize stride, afxQueryResultFlags flags)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &qryp, afxFcc_QRYP);
    AfxAssertRange(qryp->cap, baseQuery, queryCnt);
    AfxAssert(dst);
    AfxAssert(cap);
    return 0;
}

AVX void AfxResetQueries(avxQueryPool qryp, afxNat baseQuery, afxNat queryCnt)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &qryp, afxFcc_QRYP);
    AfxAssertRange(qryp->cap, baseQuery, queryCnt);

}

_AVX afxDrawContext AfxGetQueryPoolContext(avxQueryPool qryp)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &qryp, afxFcc_QRYP);
    afxDrawContext dctx = AfxGetProvider(qryp);
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    return dctx;
}

_AVX afxError _AvxQrypDtorCb(avxQueryPool qryp)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &qryp, afxFcc_QRYP);
    return err;
}

_AVX afxError _AvxQrypCtorCb(avxQueryPool qryp, void** args, afxNat invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &qryp, afxFcc_QRYP);

    afxDrawContext dctx = args[0];
    afxQueryType type = *(afxQueryType const*)args[1];
    afxNat cap = *(afxNat const*)args[2];
    AfxAssert(cap);

    if (type > afxQueryType_TOTAL) AfxThrowError();
    else
    {
        qryp->type = type;
        qryp->cap = cap;
    }
    return err;
}

////////////////////////////////////////////////////////////////////////////////

_AVX afxError AfxAcquireQueryPools(afxDrawContext dctx, afxQueryType type, afxNat cap, afxNat cnt, avxQueryPool pools[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);

    afxClass* cls = AfxGetQueryPoolClass(dctx);
    AfxAssertClass(cls, afxFcc_QRYP);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)pools, (void const*[]) { dctx, &type, &cap }))
        AfxThrowError();

    return err;
}

_AVX afxClassConfig const _AvxQrypStdImplementation =
{
    .fcc = afxFcc_QRYP,
    .name = "QueryPool",
    .desc = "Managed Query Pool",
    .fixedSiz = sizeof(AFX_OBJECT(avxQueryPool)),
    .ctor = (void*)_AvxQrypCtorCb,
    .dtor = (void*)_AvxQrypDtorCb
};
