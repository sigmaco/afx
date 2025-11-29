/*
 *           ::::::::    :::::::::::    ::::::::    ::::     ::::       :::
 *          :+:    :+:       :+:       :+:    :+:   +:+:+: :+:+:+     :+: :+:
 *          +:+              +:+       +:+          +:+ +:+:+ +:+    +:+   +:+
 *          +#++:++#++       +#+       :#:          +#+  +:+  +#+   +#++:++#++:
 *                 +#+       +#+       +#+   +#+#   +#+       +#+   +#+     +#+
 *          #+#    #+#       #+#       #+#    #+#   #+#       #+#   #+#     #+#
 *           ########    ###########    ########    ###       ###   ###     ###
 *
 *                     S I G M A   T E C H N O L O G Y   G R O U P
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>
// This software is part of Advanced Video Graphics Extensions & Experiments.

#define _AVX_DRAW_C
#define _AVX_QUERY_POOL_C
#include "avxIcd.h"

_AVX afxDrawSystem AvxGetQueryPoolHost(avxQueryPool qryp)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_QRYP, 1, &qryp);
    afxDrawSystem dsys = AfxGetHost(qryp);
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    return dsys;
}

AVX afxResult AvxGetQueryResults(avxQueryPool qryp, avxQueryResultFlags flags, afxUnit baseSlot, afxUnit slotCnt, afxSize dstCap, void* dst, afxSize stride)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_QRYP, 1, &qryp);
    AFX_ASSERT_RANGE(qryp->slotCnt, baseSlot, slotCnt);
    AFX_ASSERT(dst);
    AFX_ASSERT(dstCap);
    afxDrawSystem dsys = AvxGetQueryPoolHost(qryp);
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    AFX_ASSERT(_AvxDsysGetDdi(dsys)->getQrypRslt);
    return _AvxDsysGetDdi(dsys)->getQrypRslt(dsys, qryp, flags, baseSlot, slotCnt, dstCap, dst, stride);
}

AVX void AvxResetQueries(avxQueryPool qryp, afxUnit baseSlot, afxUnit slotCnt)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_QRYP, 1, &qryp);
    AFX_ASSERT_RANGE(qryp->slotCnt, baseSlot, slotCnt);

    afxDrawSystem dsys = AvxGetQueryPoolHost(qryp);
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    AFX_ASSERT(_AvxDsysGetDdi(dsys)->resetQryp);
    _AvxDsysGetDdi(dsys)->resetQryp(dsys, qryp, baseSlot, slotCnt);
}

_AVX afxError _AvxQrypDtorCb(avxQueryPool qryp)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_QRYP, 1, &qryp);
    return err;
}

_AVX afxError _AvxQrypCtorCb(avxQueryPool qryp, void** args, afxUnit invokeNo)
{
    afxError err = { 0 };
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

_AVX afxClassConfig const _AVX_CLASS_CONFIG_QRYP =
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
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);

    afxClass* cls = (afxClass*)_AvxDsysGetDdi(dsys)->qrypCls(dsys);
    AFX_ASSERT_CLASS(cls, afxFcc_QRYP);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)pools, (void const*[]) { dsys, cfg }))
    {
        AfxThrowError();
    }
    return err;
}

_AVX afxError _AvxDsysResetQrypCb_SW(afxDrawSystem dsys, avxQueryPool qryp, afxUnit base, afxUnit cnt)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);

    afxError queErr;
    afxBool queued = FALSE;

    afxMask dedIoExuMask;
    afxMask ioExuMask = _AvxDsysGetIoExuMask(dsys, &dedIoExuMask);
    afxUnit exuIdx = 0;
    afxDrawBridge dexu;
    afxUnit exuCnt;

    // Firstly, try to put them in a dedicated queue.
    if (dedIoExuMask)
    {
        exuCnt = AvxChooseDrawBridges(dsys, AFX_INVALID_INDEX, avxAptitude_DMA, dedIoExuMask, 0, 0, NIL);
        AFX_ASSERT(exuCnt);
        exuIdx = 0;
        while (AvxChooseDrawBridges(dsys, AFX_INVALID_INDEX, avxAptitude_DMA, dedIoExuMask, exuIdx++, 1, &dexu))
        {
            queErr = _AvxDexuResetQueries(dexu, qryp, base, cnt);
            err = queErr;

            if (!queErr)
            {
                queued = TRUE;
                break; // while
            }

            if (queErr == afxError_TIMEOUT || queErr == afxError_BUSY)
                continue; // while

            AfxThrowError();
        }
    }

    // If we can not put them in a dedicated queue, try to put them in a shared one.
    if (!queued)
    {
        exuCnt = AvxChooseDrawBridges(dsys, AFX_INVALID_INDEX, avxAptitude_DMA, ioExuMask, 0, 0, NIL);
        AFX_ASSERT(exuCnt);
        exuIdx = 0;
        while (AvxChooseDrawBridges(dsys, AFX_INVALID_INDEX, avxAptitude_DMA, ioExuMask, exuIdx++, 1, &dexu))
        {
            queErr = _AvxDexuResetQueries(dexu, qryp, base, cnt);
            err = queErr;

            if (!queErr)
            {
                queued = TRUE;
                break; // while
            }

            if (queErr == afxError_TIMEOUT || queErr == afxError_BUSY)
                continue; // while

            AfxThrowError();
        }
    }
    return err;
}

_AVX afxResult _AvxDsysGetQrypRsltCb_SW(afxDrawSystem dsys, avxQueryPool qryp, avxQueryResultFlags flags, afxUnit base, afxUnit cnt, afxSize dstCap, void* dst, afxSize stride)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);

    afxError queErr;
    afxBool queued = FALSE;

    afxMask dedIoExuMask;
    afxMask ioExuMask = _AvxDsysGetIoExuMask(dsys, &dedIoExuMask);
    afxUnit exuIdx = 0;
    afxDrawBridge dexu;
    afxUnit exuCnt;

    // Firstly, try to put them in a dedicated queue.
    if (dedIoExuMask)
    {
        exuCnt = AvxChooseDrawBridges(dsys, AFX_INVALID_INDEX, avxAptitude_DMA, dedIoExuMask, 0, 0, NIL);
        AFX_ASSERT(exuCnt);
        exuIdx = 0;
        while (AvxChooseDrawBridges(dsys, AFX_INVALID_INDEX, avxAptitude_DMA, dedIoExuMask, exuIdx++, 1, &dexu))
        {
            queErr = _AvxDexuQueryResults(dexu, qryp, flags, base, cnt, dstCap, dst, stride);
            err = queErr;

            if (!queErr)
            {
                queued = TRUE;
                break; // while
            }

            if (queErr == afxError_TIMEOUT || queErr == afxError_BUSY)
                continue; // while

            AfxThrowError();
        }
    }

    // If we can not put them in a dedicated queue, try to put them in a shared one.
    if (!queued)
    {
        exuCnt = AvxChooseDrawBridges(dsys, AFX_INVALID_INDEX, avxAptitude_DMA, ioExuMask, 0, 0, NIL);
        AFX_ASSERT(exuCnt);
        exuIdx = 0;
        while (AvxChooseDrawBridges(dsys, AFX_INVALID_INDEX, avxAptitude_DMA, ioExuMask, exuIdx++, 1, &dexu))
        {
            queErr = _AvxDexuQueryResults(dexu, qryp, flags, base, cnt, dstCap, dst, stride);
            err = queErr;

            if (!queErr)
            {
                queued = TRUE;
                break; // while
            }

            if (queErr == afxError_TIMEOUT || queErr == afxError_BUSY)
                continue; // while

            AfxThrowError();
        }
    }
    return err;
}
