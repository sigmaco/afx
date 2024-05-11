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

AVX afxCmdId AfxCmdBeginQuery(afxDrawStream diob, afxQueryPool pool, afxNat queryIdx, afxBool precise)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &diob, afxFcc_DIOB);
    AfxAssertObjects(1, &pool, afxFcc_QRYP);
    AfxAssertRange(pool->cap, queryIdx, 1);
    return 0;
}

AVX afxCmdId AfxCmdEndQuery(afxDrawStream diob, afxQueryPool pool, afxNat queryIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &diob, afxFcc_DIOB);
    AfxAssertObjects(1, &pool, afxFcc_QRYP);
    AfxAssertRange(pool->cap, queryIdx, 1);
    return 0;
}

AVX afxCmdId AfxCmdCopyQueryResults(afxDrawStream diob, afxQueryPool pool, afxNat baseQuery, afxNat queryCnt, afxBuffer buf, afxSize offset, afxSize stride, afxQueryResultFlags flags)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &diob, afxFcc_DIOB);
    AfxAssertObjects(1, &pool, afxFcc_QRYP);
    AfxAssertRange(pool->cap, baseQuery, queryCnt);
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    AfxAssertRange(AfxGetBufferCapacity(buf), offset, stride);
    return 0;
}

AVX afxCmdId AfxCmdResetQueries(afxDrawStream diob, afxQueryPool pool, afxNat baseQuery, afxNat queryCnt)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &diob, afxFcc_DIOB);
    AfxAssertObjects(1, &pool, afxFcc_QRYP);
    AfxAssertRange(pool->cap, baseQuery, queryCnt);
    return 0;
}

AVX afxCmdId AfxCmdWriteTimestamp(afxDrawStream diob, afxQueryPool pool, afxNat queryIdx, afxPipelineStage stage)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &diob, afxFcc_DIOB);
    AfxAssertObjects(1, &pool, afxFcc_QRYP);
    AfxAssertRange(pool->cap, queryIdx, 1);
    return 0;
}
