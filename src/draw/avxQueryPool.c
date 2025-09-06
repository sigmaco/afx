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
// This software is part of Advanced Video Graphics Extensions & Experiments.

#define _AVX_DRAW_C
#define _AVX_QUERY_POOL_C
#include "ddi/avxImplementation.h"

_AVX afxDrawSystem AvxGetQueryPoolHost(avxQueryPool qryp)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_QRYP, 1, &qryp);
    afxDrawSystem dsys = AfxGetHost(qryp);
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    return dsys;
}

AVX afxResult AvxGetQueryResults(avxQueryPool qryp, avxQueryResultFlags flags, afxUnit baseSlot, afxUnit slotCnt, afxSize dstCap, void* dst, afxSize stride)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_QRYP, 1, &qryp);
    AFX_ASSERT_RANGE(qryp->slotCnt, baseSlot, slotCnt);
    AFX_ASSERT(dst);
    AFX_ASSERT(dstCap);
    afxDrawSystem dsys = AvxGetQueryPoolHost(qryp);
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    AFX_ASSERT(_AvxDsysGetImpl(dsys)->getQrypRslt);
    return _AvxDsysGetImpl(dsys)->getQrypRslt(dsys, qryp, baseSlot, slotCnt, dst, dstCap, stride, flags);
}

AVX void AvxResetQueries(avxQueryPool qryp, afxUnit baseSlot, afxUnit slotCnt)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_QRYP, 1, &qryp);
    AFX_ASSERT_RANGE(qryp->slotCnt, baseSlot, slotCnt);

    afxDrawSystem dsys = AvxGetQueryPoolHost(qryp);
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    AFX_ASSERT(_AvxDsysGetImpl(dsys)->resetQryp);
    _AvxDsysGetImpl(dsys)->resetQryp(dsys, qryp, baseSlot, slotCnt);
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
    avxQueryPoolConfig const* cfg = (avxQueryPoolConfig const*)args[1];
    AFX_ASSERT(cfg->slotCnt);

    if (cfg->type > avxQueryType_TOTAL)
    {
        AfxThrowError();
        return err;
    }

    qryp->type = cfg->type;
    qryp->slotCnt = cfg->slotCnt;
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

_AVX afxError AfxAcquireQueryPools(afxDrawSystem dsys, afxUnit cnt, avxQueryPoolConfig const cfg[], avxQueryPool pools[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);

    afxClass* cls = (afxClass*)_AvxDsysGetImpl(dsys)->qrypCls(dsys);
    AFX_ASSERT_CLASS(cls, afxFcc_QRYP);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)pools, (void const*[]) { dsys, cfg }))
        AfxThrowError();

    return err;
}
