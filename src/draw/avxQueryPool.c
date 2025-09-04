/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *        Q W A D R O   V I D E O   G R A P H I C S   I N F R A S T R U C T U R E
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#define _AVX_DRAW_C
#define _AVX_QUERY_POOL_C
#include "ddi/avxImplementation.h"

AVX afxResult AvxGetQueryResults(avxQueryPool qryp, afxUnit baseQuery, afxUnit queryCnt, void* dst, afxSize cap, afxSize stride, avxQueryResultFlags flags)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_QRYP, 1, &qryp);
    AFX_ASSERT_RANGE(qryp->cap, baseQuery, queryCnt);
    AFX_ASSERT(dst);
    AFX_ASSERT(cap);
    return 0;
}

AVX void AvxResetQueries(avxQueryPool qryp, afxUnit baseQuery, afxUnit queryCnt)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_QRYP, 1, &qryp);
    AFX_ASSERT_RANGE(qryp->cap, baseQuery, queryCnt);

}

_AVX afxDrawSystem AvxGetQueryPoolContext(avxQueryPool qryp)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_QRYP, 1, &qryp);
    afxDrawSystem dsys = AfxGetProvider(qryp);
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    return dsys;
}

_AVX afxError _AvxQrypDtorCb(avxQueryPool qryp)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_QRYP, 1, &qryp);
    return err;
}

_AVX afxError _AvxQrypCtorCb(avxQueryPool qryp, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_QRYP, 1, &qryp);

    afxDrawSystem dsys = args[0];
    avxQueryType type = *(avxQueryType const*)args[1];
    afxUnit cap = *(afxUnit const*)args[2];
    AFX_ASSERT(cap);

    if (type > avxQueryType_TOTAL)
    {
        AfxThrowError();
        return err;
    }

    qryp->type = type;
    qryp->cap = cap;
    qryp->tag = (afxString) { 0 };
    qryp->udd = NIL;

    return err;
}

_AVX afxClassConfig const _AVX_QRYP_CLASS_CONFIG =
{
    .fcc = afxFcc_QRYP,
    .name = "QueryPool",
    .desc = "Managed Query Pool",
    .fixedSiz = sizeof(AFX_OBJECT(avxQueryPool)),
    .ctor = (void*)_AvxQrypCtorCb,
    .dtor = (void*)_AvxQrypDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AVX afxError AfxAcquireQueryPools(afxDrawSystem dsys, avxQueryType type, afxUnit cap, afxUnit cnt, avxQueryPool pools[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);

    afxClass* cls = (afxClass*)_AvxDsysGetImpl(dsys)->qrypCls(dsys);
    AFX_ASSERT_CLASS(cls, afxFcc_QRYP);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)pools, (void const*[]) { dsys, &type, &cap }))
        AfxThrowError();

    return err;
}
