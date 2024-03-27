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
#include "qwadro/core/afxManager.h"
#include "qwadro/draw/afxDrawContext.h"
#include "qwadro/draw/pipe/afxQueryOps.h"

AVX afxCmdId AfxCmdBeginQuery(afxDrawStream dscr, afxQueryPool pool, afxNat queryIdx, afxBool precise)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssertObjects(1, &pool, afxFcc_QRYP);
    AfxAssertRange(pool->cap, queryIdx, 1);
    return 0;
}

AVX afxCmdId AfxCmdEndQuery(afxDrawStream dscr, afxQueryPool pool, afxNat queryIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssertObjects(1, &pool, afxFcc_QRYP);
    AfxAssertRange(pool->cap, queryIdx, 1);
    return 0;
}

AVX afxCmdId AfxCmdCopyQueryResults(afxDrawStream dscr, afxQueryPool pool, afxNat baseQuery, afxNat queryCnt, afxBuffer buf, afxSize offset, afxSize stride, afxQueryResultFlags flags)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssertObjects(1, &pool, afxFcc_QRYP);
    AfxAssertRange(pool->cap, baseQuery, queryCnt);
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    AfxAssertRange(AfxGetBufferCapacity(buf), offset, stride);
    return 0;
}

AVX afxCmdId AfxCmdResetQueries(afxDrawStream dscr, afxQueryPool pool, afxNat baseQuery, afxNat queryCnt)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssertObjects(1, &pool, afxFcc_QRYP);
    AfxAssertRange(pool->cap, baseQuery, queryCnt);
    return 0;
}

AVX afxCmdId AfxCmdWriteTimestamp(afxDrawStream dscr, afxQueryPool pool, afxNat queryIdx, afxPipelineStage stage)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssertObjects(1, &pool, afxFcc_QRYP);
    AfxAssertRange(pool->cap, queryIdx, 1);
    return 0;
}
