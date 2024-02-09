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
#include "qwadro/core/afxClass.h"
#include "qwadro/draw/afxDrawContext.h"
#include "qwadro/draw/pipe/afxQueryPool.h"

AFX afxResult AfxGetQueryResults(afxQueryPool pool, afxNat baseQuery, afxNat queryCnt, void* dst, afxSize cap, afxSize stride, afxQueryResultFlags flags)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &pool, afxFcc_QRYP);
    AfxAssertRange(pool->cap, baseQuery, queryCnt);
    AfxAssert(dst);
    AfxAssert(cap);

}

AFX void AfxResetQueries(afxQueryPool pool, afxNat baseQuery, afxNat queryCnt)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &pool, afxFcc_QRYP);
    AfxAssertRange(pool->cap, baseQuery, queryCnt);

}

_AFX afxDrawContext AfxGetQueryPoolContext(afxQueryPool qryp)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &qryp, afxFcc_QRYP);
    afxDrawContext dctx = AfxGetObjectProvider(qryp);
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    return dctx;
}

////////////////////////////////////////////////////////////////////////////////

_AFX afxNat AfxEnumerateQueryPools(afxDrawContext dctx, afxNat first, afxNat cnt, afxQueryPool pools[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    AfxAssert(pools);
    AfxAssert(cnt);
    afxClass* cls = AfxGetQueryPoolClass(dctx);
    AfxAssertClass(cls, afxFcc_QRYP);
    return AfxEnumerateInstances(cls, first, cnt, (afxObject*)pools);
}

_AFX afxError AfxAcquireQueryPools(afxDrawContext dctx, afxQueryType type, afxNat cap, afxNat cnt, afxQueryPool quer[])
{
    afxError err = AFX_ERR_NONE;

    afxClass* cls = AfxGetQueryPoolClass(dctx);
    AfxAssertClass(cls, afxFcc_QRYP);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)quer, (void const*[]) { dctx, &type, &cap }))
        AfxThrowError();

    return err;
}

////////////////////////////////////////////////////////////////////////////////

AFX afxCmdId AfxCmdBeginQuery(afxDrawScript dscr, afxQueryPool pool, afxNat queryIdx, afxBool precise)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssertObjects(1, &pool, afxFcc_QRYP);
    AfxAssertRange(pool->cap, queryIdx, 1);

}

AFX afxCmdId AfxCmdEndQuery(afxDrawScript dscr, afxQueryPool pool, afxNat queryIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssertObjects(1, &pool, afxFcc_QRYP);
    AfxAssertRange(pool->cap, queryIdx, 1);

}

AFX afxCmdId AfxCmdCopyQueryResults(afxDrawScript dscr, afxQueryPool pool, afxNat baseQuery, afxNat queryCnt, afxBuffer buf, afxSize offset, afxSize stride, afxQueryResultFlags flags)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssertObjects(1, &pool, afxFcc_QRYP);
    AfxAssertRange(pool->cap, baseQuery, queryCnt);
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    AfxAssertRange(AfxGetBufferCapacity(buf), offset, stride);

}

AFX afxCmdId AfxCmdResetQueries(afxDrawScript dscr, afxQueryPool pool, afxNat baseQuery, afxNat queryCnt)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssertObjects(1, &pool, afxFcc_QRYP);
    AfxAssertRange(pool->cap, baseQuery, queryCnt);

}

AFX afxCmdId AfxCmdWriteTimestamp(afxDrawScript dscr, afxQueryPool pool, afxNat queryIdx, afxPipelineStage stage)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssertObjects(1, &pool, afxFcc_QRYP);
    AfxAssertRange(pool->cap, queryIdx, 1);

}
