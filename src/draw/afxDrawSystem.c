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


#define _AFX_CORE_C
//#define _AFX_DEVICE_C
#define _AFX_CONTEXT_C

#define _AVX_DRAW_C
#define _AVX_DRAW_SYSTEM_C
//#define _AVX_DRAW_DEVICE_C
//#define _AVX_DRAW_BRIDGE_C
//#define _AVX_DRAW_QUEUE_C
//#define _AVX_DRAW_SYSTEM_C
//#define _AVX_DRAW_OUTPUT_C
//#define _AVX_DRAW_INPUT_C
#define _AVX_BUFFER_C
#include "ddi/avxImplementation.h"

_AVX _avxDsysImpl const* _AvxDsysGetImpl(afxDrawSystem dsys)
{
    afxError err = AFX_ERR_NONE;
    // @dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    return dsys->pimpl;
}

_AVX afxMask _AvxDsysGetIoExuMask(afxDrawSystem dsys, afxMask* dedIoExuMask)
{
    afxError err = AFX_ERR_NONE;
    // @dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    if (dedIoExuMask) *dedIoExuMask = dsys->dedIoExuMask;
    return dsys->ioExuMask;
}

_AVX afxDrawFeatures const* _AvxDsysAccessReqFeatures(afxDrawSystem dsys)
{
    afxError err = AFX_ERR_NONE;
    // @dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    return &dsys->requirements;
}

_AVX afxDrawLimits const* _AvxDsysAccessLimits(afxDrawSystem dsys)
{
    afxError err = AFX_ERR_NONE;
    // @dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    return dsys->limits;
}

_AVX afxClass const* _AvxDsysGetDexuClassCb_SW(afxDrawSystem dsys)
{
    afxError err = AFX_ERR_NONE;
    // @dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    afxClass const* cls = &dsys->dexuCls;
    AFX_ASSERT_CLASS(cls, afxFcc_DEXU);
    return cls;
}

_AVX afxClass const* _AvxDsysGetFencClassCb_SW(afxDrawSystem dsys)
{
    afxError err = AFX_ERR_NONE;
    // @dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    afxClass *cls = &dsys->fencCls;
    AFX_ASSERT_CLASS(cls, afxFcc_FENC);
    return cls;
}

_AVX afxClass const* _AvxDsysGetDoutClassCb_SW(afxDrawSystem dsys)
{
    afxError err = AFX_ERR_NONE;
    // @dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    afxClass const* cls = &dsys->doutCls;
    AFX_ASSERT_CLASS(cls, afxFcc_DOUT);
    return cls;
}

_AVX afxClass const* _AvxDsysGetQrypClassCb_SW(afxDrawSystem dsys)
{
    afxError err = AFX_ERR_NONE;
    // @dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    afxClass *cls = &dsys->qrypCls;
    AFX_ASSERT_CLASS(cls, afxFcc_QRYP);
    return cls;
}

_AVX afxClass const* _AvxDsysGetVinClassCb_SW(afxDrawSystem dsys)
{
    afxError err = AFX_ERR_NONE;
    // @dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    afxClass *cls = &dsys->vinCls;
    AFX_ASSERT_CLASS(cls, afxFcc_VIN);
    return cls;
}

_AVX afxClass const* _AvxDsysGetRasClassCb_SW(afxDrawSystem dsys)
{
    afxError err = AFX_ERR_NONE;
    // @dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    afxClass *cls = &dsys->rasCls;
    AFX_ASSERT_CLASS(cls, afxFcc_RAS);
    return cls;
}

_AVX afxClass const* _AvxDsysGetBufClassCb_SW(afxDrawSystem dsys)
{
    afxError err = AFX_ERR_NONE;
    // @dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    afxClass *cls = &dsys->bufCls;
    AFX_ASSERT_CLASS(cls, afxFcc_BUF);
    return cls;
}

_AVX afxClass const* _AvxDsysGetSampClassCb_SW(afxDrawSystem dsys)
{
    afxError err = AFX_ERR_NONE;
    // @dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    afxClass *cls = &dsys->sampCls;
    AFX_ASSERT_CLASS(cls, afxFcc_SAMP);
    return cls;
}

_AVX afxClass const* _AvxDsysGetPipClassCb_SW(afxDrawSystem dsys)
{
    afxError err = AFX_ERR_NONE;
    // @dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    afxClass *cls = &dsys->pipCls;
    AFX_ASSERT_CLASS(cls, afxFcc_PIP);
    return cls;
}

_AVX afxClass const* _AvxDsysGetCanvClassCb_SW(afxDrawSystem dsys)
{
    afxError err = AFX_ERR_NONE;
    // @dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    afxClass *cls = &dsys->canvCls;
    AFX_ASSERT_CLASS(cls, afxFcc_CANV);
    return cls;
}

_AVX afxClass const* _AvxDsysGetShadClassCb_SW(afxDrawSystem dsys)
{
    afxError err = AFX_ERR_NONE;
    // @dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    afxClass *cls = &dsys->shadCls;
    AFX_ASSERT_CLASS(cls, afxFcc_SHD);
    return cls;
}

_AVX afxClass const* _AvxDsysGetLigaClassCb_SW(afxDrawSystem dsys)
{
    afxError err = AFX_ERR_NONE;
    // @dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    afxClass *cls = &dsys->ligaCls;
    AFX_ASSERT_CLASS(cls, afxFcc_LIGA);
    return cls;
}

_AVX afxClass const* _AvxDsysGetTxdClassCb_SW(afxDrawSystem dsys)
{
    afxError err = AFX_ERR_NONE;
    // @dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    afxClass const* cls = &dsys->txdCls;
    AFX_ASSERT_CLASS(cls, afxFcc_TXD);
    return cls;
}

_AVX _avxDsysImpl const _AVX_DSYS_IMPL =
{
    .fencCls = _AvxDsysGetFencClassCb_SW,
    .dexuCls = _AvxDsysGetDexuClassCb_SW,
    .doutCls = _AvxDsysGetDoutClassCb_SW,
    .qrypCls = _AvxDsysGetQrypClassCb_SW,
    .vtxdCls = _AvxDsysGetVinClassCb_SW,
    .rasCls = _AvxDsysGetRasClassCb_SW,
    .bufCls = _AvxDsysGetBufClassCb_SW,
    .sampCls = _AvxDsysGetSampClassCb_SW,
    .pipCls = _AvxDsysGetPipClassCb_SW,
    .canvCls = _AvxDsysGetCanvClassCb_SW,
    .shadCls = _AvxDsysGetShadClassCb_SW,
    .ligaCls = _AvxDsysGetLigaClassCb_SW,

    .txdCls = _AvxDsysGetTxdClassCb_SW,

    .transferCb = _AvxDsysTransferCb_SW,
    .cohereCb = _AvxDsysCohereMappedBuffersCb_SW,
    .remapCb = _AvxDsysRemapBuffersCb_SW,

    .allocRasCb = _AvxDsysAllocateRastersCb_SW,
    .deallocRasCb = _AvxDsysDeallocateRastersCb_SW,
    .allocBufCb = _AvxDsysAllocateBuffersCb_SW,
    .deallocBufCb = _AvxDsysDeallocateBuffersCb_SW,
};

_AVX afxModule AvxGetDrawSystemIcd(afxDrawSystem dsys)
{
    afxError err = AFX_ERR_NONE;
    // @dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    afxModule icd = AfxGetProvider(dsys);
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &icd);
    AFX_ASSERT(AfxTestModule(icd, afxModuleFlag_ICD | afxModuleFlag_AVX));
    return icd;
}

_AVX void AvxGetEnabledDrawFeatures(afxDrawSystem dsys, afxDrawFeatures* features)
{
    afxError err = AFX_ERR_NONE;
    // @dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);

    AFX_ASSERT(features);
    *features = dsys->requirements;

    return;
}

_AVX afxUnit AvxTestForDrawExtensionsEnabled(afxDrawSystem dsys, afxUnit cnt, afxString const* exts, afxBool enabled[])
{
    afxError err = AFX_ERR_NONE;
    // @dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    // @names must be an array of Qwadro strings specifying the wanted symbol's names.
    AFX_ASSERT(exts);
    afxUnit rslt = 0;

    if (enabled)
    {
        afxString const* reqExts;

        afxUnit matchedIdx;
        for (afxUnit i = 0; i < cnt; i++)
        {
            if (AfxCompareStrings(reqExts, 0, FALSE, 1, &exts[i], &matchedIdx))
                rslt++, enabled[i] = TRUE;
            else
                enabled[i] = FALSE;
        }
    }
    else
    {
        afxString const* reqExts;

        afxUnit matchedIdx;
        for (afxUnit i = 0; i < cnt; i++)
        {
            if (AfxCompareStrings(reqExts, 0, FALSE, 1, &exts[i], &matchedIdx))
                rslt++;
        }
    }

    return rslt;
}

_AVX afxUnit AvxResolveDrawSymbols(afxDrawSystem dsys, afxUnit cnt, afxString const names[], void* addresses[])
{
    afxError err = AFX_ERR_NONE;
    // @dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    // @names must be an array of Qwadro strings specifying the wanted symbol's names.
    AFX_ASSERT(names);
    // @addresses must be a recipient array for returned function pointers.
    AFX_ASSERT(addresses);
    afxUnit rslt = 0;

    AFX_ASSERT(dsys->pimpl->getProcCb);
    rslt = dsys->pimpl->getProcCb(dsys, cnt, names, addresses);

    return rslt;
}

_AVX afxBool AvxGetShaderStringBase(afxDrawSystem dsys, afxStringBase* base)
{
    afxError err = AFX_ERR_NONE;
    // @dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    AFX_ASSERT(base);
    afxStringBase strb = dsys->shdStrb;
    AFX_TRY_ASSERT_OBJECTS(afxFcc_STRB, 1, &strb);
    *base = dsys->shdStrb;
    return !!strb;
}

_AVX afxUnit AvxGetDrawBridges(afxDrawSystem dsys, afxUnit baseIdx, afxUnit cnt, afxDrawBridge bridges[])
{
    afxError err = AFX_ERR_NONE;
    // @dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);

    afxUnit bridgeCnt = dsys->bridgeCnt;
    cnt = AFX_MIN(cnt, bridgeCnt - baseIdx);

    // bridges can be null or must be a valid pointer to afxDrawBridge handles.
    if (!bridges) return cnt;

    //AFX_ASSERT_RANGE(bridgeCnt, baseExuIdx, cnt);
    baseIdx = AFX_MIN(baseIdx, bridgeCnt - 1);

    afxUnit rslt = 0;
    for (afxUnit i = 0; i < cnt; i++)
    {
        afxDrawBridge dexu = dsys->bridges[baseIdx + i];
        AFX_ASSERT_OBJECTS(afxFcc_DEXU, 1, &dexu);
        bridges[rslt++] = dexu;
    }
    return rslt;
}

_AVX afxUnit AvxChooseDrawBridges(afxDrawSystem dsys, afxUnit ddevId, afxDrawCaps caps, afxMask exuMask, afxUnit first, afxUnit maxCnt, afxDrawBridge bridges[])
{
    afxError err = AFX_ERR_NONE;
    // @dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    afxUnit found = 0;
    afxUnit rslt = 0;

    if ((ddevId == AFX_INVALID_INDEX) && (!caps) && (!bridges))
        return maxCnt ? AFX_MIN(maxCnt, dsys->bridgeCnt - first) : AFX_MIN(dsys->bridgeCnt, dsys->bridgeCnt - first);

    afxUnit bridgeCnt = dsys->bridgeCnt;
    for (afxUnit exuIdx = 0; exuIdx < bridgeCnt; exuIdx++)
    {
        // skip if a mask is specified and this EXU is not bitmapped in it.
        if (exuMask && !(exuMask & AFX_BITMASK(exuIdx)))
            continue;

        afxDrawBridge dexu = dsys->bridges[exuIdx];
        AFX_ASSERT_OBJECTS(afxFcc_DEXU, 1, &dexu);
        
        afxDrawDevice ddev = AvxGetBridgedDrawDevice(dexu, NIL);
        AFX_ASSERT_OBJECTS(afxFcc_DDEV, 1, &ddev);

        if ((ddevId != AFX_INVALID_INDEX) && (ddevId != AfxGetObjectId(ddev)))
            continue;

        if (caps)
        {
            afxDrawPortInfo capsi;
            AvxQueryDrawCapabilities(ddev, &capsi);
            
            if ((capsi.capabilities & caps) != caps)
                continue;
        }

        if (maxCnt && (found >= first))
        {
            if (bridges)
            {
                bridges[rslt] = dexu;
            }

            if (maxCnt == rslt)
                break;
        }

        if (found >= first)
            rslt++;

        found++;
    }
    return rslt;
}

_AVX afxError AvxWaitForDrawQueue(afxDrawSystem dsys, afxUnit64 timeout, afxUnit exuIdx, afxUnit queId)
{
    afxError err = AFX_ERR_NONE;
    // @dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    
    afxDrawBridge dexu;
    // exuIdx must be a valid index to a bridge.
    AFX_ASSERT_RANGE(dsys->bridgeCnt, exuIdx, 1);
    if (!AvxGetDrawBridges(dsys, exuIdx, 1, &dexu))
    {
        AfxThrowError();
        return err;
    }
    AFX_ASSERT_OBJECTS(afxFcc_DEXU, 1, &dexu);

    afxDrawQueue dque;
    if (!AvxGetDrawQueues(dexu, queId, 1, &dque))
    {
        AfxThrowError();
        return err;
    }
    AFX_ASSERT_OBJECTS(afxFcc_DQUE, 1, &dque);

    if (AvxWaitForEmptyDrawQueue(dque, timeout))
        AfxThrowError();

    return err;
}

_AVX afxError AvxWaitForDrawBridges(afxDrawSystem dsys, afxUnit64 timeout, afxMask exuMask)
{
    afxError err = AFX_ERR_NONE;
    // @dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);

    afxUnit exuCnt = dsys->bridgeCnt;
    for (afxUnit exuIdx = 0; exuIdx < exuCnt; exuIdx++)
    {
        if (exuMask && !(AFX_TEST_BIT_SET(exuMask, exuIdx)))
            continue;

        afxDrawBridge dexu;
        if (!AvxGetDrawBridges(dsys, exuIdx, 1, &dexu))
        {
            AfxThrowError();
            return err;
        }
        AFX_ASSERT_OBJECTS(afxFcc_DEXU, 1, &dexu);
        AvxWaitForIdleDrawBridge(dexu, timeout);
    }
    return err;
}

_AVX afxError AvxWaitForDrawSystem(afxDrawSystem dsys, afxUnit64 timeout)
{
    afxError err = AFX_ERR_NONE;
    // @dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);

    if (!dsys->pimpl->waitCb)
    {
        afxUnit exuCnt = dsys->bridgeCnt;
        for (afxUnit exuIdx = 0; exuIdx < exuCnt; exuIdx++)
        {
            afxDrawBridge dexu;
            if (!AvxGetDrawBridges(dsys, exuIdx, 1, &dexu))
            {
                AfxThrowError();
                return err;
            }
            AFX_ASSERT_OBJECTS(afxFcc_DEXU, 1, &dexu);
            AvxWaitForIdleDrawBridge(dexu, timeout);
        }
    }
    else if (dsys->pimpl->waitCb(dsys, timeout))
        AfxThrowError();

    return err;
}

_AVX afxError _AvxDsysTransferCb_SW(afxDrawSystem dsys, avxTransference* ctrl, afxUnit opCnt, void const* ops)
{
    afxError err = AFX_ERR_NONE;
    // @dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    AFX_ASSERT(opCnt);
    AFX_ASSERT(ctrl);
    AFX_ASSERT(ops);

    afxMask dsysIoExuMask = dsys->ioExuMask;
    afxMask exuMask = ctrl->exuMask;
    afxUnit exuCnt = dsys->bridgeCnt;
    afxUnit firstExuIdx = AfxRandom2(0, exuCnt - 1);

    AFX_ASSERT(!exuMask || (dsysIoExuMask & exuMask));

    afxBool queued = FALSE;

    while (1)
    {
        for (afxUnit exuIdx = firstExuIdx; exuIdx < exuCnt; exuIdx++)
        {
            firstExuIdx = 0;

            // Skip non-transfer-capable EXUs.
            if (!(dsysIoExuMask & AFX_BITMASK(exuIdx)))
                continue; // for

            if (exuMask && !(exuMask & AFX_BITMASK(exuIdx)))
                continue; // for

            // Try to pick one of the dedicated ones when EXUs are not specified.
            if ((!exuMask) && dsys->dedIoExuMask && (!(dsys->dedIoExuMask & AFX_BITMASK(exuIdx))))
                continue; // for
#if 0
            // if a mask is specified and it is not one of the existing dedicated EXUs in mask.
            if (dsys->dedIoExuMask)
            {
                if (exuMask)
                {
                    if ((exuMask & dsys->dedIoExuMask))
                    {
                        if (!(dsys->dedIoExuMask & AFX_BITMASK(exuIdx)))
                            continue;
                    }
                }
                else
                {

                }

                if (exuMask && (dsys->dedIoExuMask & AFX_BITMASK(exuIdx)))
                    continue;
            }
#endif

            afxDrawBridge dexu;
            if (!AvxGetDrawBridges(dsys, exuIdx, 1, &dexu))
            {
                AfxThrowError();
                continue;
            }

            afxError err2 = _AvxDexuTransferVideoMemory(dexu, ctrl, opCnt, ops);

            err = err2;

            if (!err2)
            {
                queued = TRUE;
                break; // for
            }

            if (err2 == afxError_TIMEOUT || err2 == afxError_BUSY)
                continue; // for

            AfxThrowError();
        }

        if (err || queued) break; // while --- find bridges
    }
    return err;
}

_AVX afxError _AvxDsysRemapBuffersCb_SW(afxDrawSystem dsys, afxBool unmap, afxUnit cnt, _avxBufferRemapping const maps[])
{
    afxError err = AFX_ERR_NONE;
    // @dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    AFX_ASSERT(cnt);
    AFX_ASSERT(maps);

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
        exuCnt = AvxChooseDrawBridges(dsys, AFX_INVALID_INDEX, afxDrawCaps_TRANSFER, dedIoExuMask, 0, 0, NIL);
        AFX_ASSERT(exuCnt);
        exuIdx = 0;
        while (AvxChooseDrawBridges(dsys, AFX_INVALID_INDEX, afxDrawCaps_TRANSFER, dedIoExuMask, exuIdx++, 1, &dexu))
        {
            queErr = _AvxDexuRemapBuffers(dexu, unmap, cnt, maps);
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
        exuCnt = AvxChooseDrawBridges(dsys, AFX_INVALID_INDEX, afxDrawCaps_TRANSFER, ioExuMask, 0, 0, NIL);
        AFX_ASSERT(exuCnt);
        exuIdx = 0;
        while (AvxChooseDrawBridges(dsys, AFX_INVALID_INDEX, afxDrawCaps_TRANSFER, ioExuMask, exuIdx++, 1, &dexu))
        {
            queErr = _AvxDexuRemapBuffers(dexu, unmap, cnt, maps);
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

_AVX afxError _AvxDsysCohereMappedBuffersCb_SW(afxDrawSystem dsys, afxBool discard, afxUnit cnt, avxBufferedMap const maps[])
{
    afxError err = AFX_ERR_NONE;
    // @dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    AFX_ASSERT(cnt);
    AFX_ASSERT(maps);

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
        exuCnt = AvxChooseDrawBridges(dsys, AFX_INVALID_INDEX, afxDrawCaps_TRANSFER, dedIoExuMask, 0, 0, NIL);
        AFX_ASSERT(exuCnt);
        exuIdx = 0;
        while (AvxChooseDrawBridges(dsys, AFX_INVALID_INDEX, afxDrawCaps_TRANSFER, dedIoExuMask, exuIdx++, 1, &dexu))
        {
            queErr = _AvxDexuCohereMappedBuffers(dexu, discard, cnt, maps);
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
        exuCnt = AvxChooseDrawBridges(dsys, AFX_INVALID_INDEX, afxDrawCaps_TRANSFER, ioExuMask, 0, 0, NIL);
        AFX_ASSERT(exuCnt);
        exuIdx = 0;
        while (AvxChooseDrawBridges(dsys, AFX_INVALID_INDEX, afxDrawCaps_TRANSFER, ioExuMask, exuIdx++, 1, &dexu))
        {
            queErr = _AvxDexuCohereMappedBuffers(dexu, discard, cnt, maps);
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

_AVX afxError _AvxDsysDtorCb(afxDrawSystem dsys)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);

    //AvxWaitForDrawSystem(dsys, AFX_TIMEOUT_INFINITE);
    //AvxWaitForDrawSystem(dsys, AFX_TIMEOUT_INFINITE);

    afxUnit bridgeCnt = dsys->bridgeCnt;

    AfxDeregisterChainedClasses(&dsys->ctx.classes);
    AFX_ASSERT(AfxIsChainEmpty(&dsys->ctx.classes));
#if 0
    AfxAssertObjects(dsys->exuCnt, dsys->exus, afxFcc_DEXU);

    for (afxUnit j = dsys->exuCnt; j-- > 0;)
        while (!AfxDisposeObjects(1, &dsys->exus[j]));
#endif

    if (dsys->shdStrb)
        AfxDisposeObjects(1, &dsys->shdStrb);

    if (dsys->bridges)
    {
#if 0
        AFX_TRY_ASSERT_OBJECTS(afxFcc_DEXU, bridgeCnt, dsys->bridges);

        for (afxUnit i = bridgeCnt; i-- > 0;)
        {
            AfxDisposeObjects(1, &dsys->bridges[i]);
        }
#endif
    }

    afxObjectStash const stashes[] =
    {
        {
            .cnt = bridgeCnt,
            .siz = sizeof(dsys->bridges[0]),
            .var = (void**)&dsys->bridges
        }
    };

    if (AfxDeallocateInstanceData(dsys, ARRAY_SIZE(stashes), stashes))
        AfxThrowError();

    return err;
}

_AVX afxError _AvxDsysCtorCb(afxDrawSystem dsys, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    //AFX_ASSERT(spec);

    afxModule icd = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &icd);
    _avxDsysAcquisition const* cfg = AFX_CAST(_avxDsysAcquisition const*, args[1]) + invokeNo;
    _avxDexuAcquisition* bridgeCfgs = AFX_CAST(_avxDexuAcquisition*, args[2]) + invokeNo;
    
    if (!cfg)
    {
        AFX_ASSERT(cfg);
        AfxThrowError();
        return err;
    }

    if (!bridgeCfgs)
    {
        AFX_ASSERT(bridgeCfgs);
        AfxThrowError();
        return err;
    }

    dsys->pimpl = &_AVX_DSYS_IMPL;

    dsys->running = FALSE;

    dsys->limits = _AvxDdevGetLimits(bridgeCfgs[0].ddev);
    dsys->requirements = cfg->reqFeatures;
    dsys->leftHandedSpace = FALSE;//ddev->leftHandedSpace;
    dsys->clipSpaceDepth = avxClipSpaceDepth_ZERO_TO_ONE;//ddev->clipSpaceDepth;

    {
        afxChain* classes = &dsys->ctx.classes;
        AfxDeployChain(classes, (void*)dsys);

        afxClassConfig bufClsCfg = cfg->bufClsCfg ? *cfg->bufClsCfg : _AVX_BUF_CLASS_CONFIG;
        AFX_ASSERT(bufClsCfg.fcc == afxFcc_BUF);
        AfxMountClass(&dsys->bufCls, NIL, classes, &bufClsCfg);

        afxClassConfig rasClsCfg = cfg->rasClsCfg ? *cfg->rasClsCfg : _AVX_RAS_CLASS_CONFIG;
        AFX_ASSERT(rasClsCfg.fcc == afxFcc_RAS);
        AfxMountClass(&dsys->rasCls, NIL, classes, &rasClsCfg);

        afxClassConfig canvClsCfg = cfg->canvClsCfg ? *cfg->canvClsCfg : _AVX_CANV_CLASS_CONFIG;
        AFX_ASSERT(canvClsCfg.fcc == afxFcc_CANV);
        AfxMountClass(&dsys->canvCls, NIL, classes, &canvClsCfg); // req RAS

        afxClassConfig sampClsCfg = cfg->sampClsCfg ? *cfg->sampClsCfg : _AVX_SAMP_CLASS_CONFIG;
        AFX_ASSERT(sampClsCfg.fcc == afxFcc_SAMP);
        AfxMountClass(&dsys->sampCls, NIL, classes, &sampClsCfg);

        afxClassConfig ligaClsCfg = cfg->ligaClsCfg ? *cfg->ligaClsCfg : _AVX_LIGA_CLASS_CONFIG;
        AFX_ASSERT(ligaClsCfg.fcc == afxFcc_LIGA);
        AfxMountClass(&dsys->ligaCls, NIL, classes, &ligaClsCfg); // req BUF, RAS, SAMP

        afxClassConfig vtxdClsCfg = cfg->vtxdClsCfg ? *cfg->vtxdClsCfg : _AVX_VIN_CLASS_CONFIG;
        AFX_ASSERT(vtxdClsCfg.fcc == afxFcc_VIN);
        AfxMountClass(&dsys->vinCls, NIL, classes, &vtxdClsCfg);

        afxClassConfig shdClsCfg = cfg->shdClsCfg ? *cfg->shdClsCfg : _AVX_SHD_CLASS_CONFIG;
        AFX_ASSERT(shdClsCfg.fcc == afxFcc_SHD);
        AfxMountClass(&dsys->shadCls, NIL, classes, &shdClsCfg);

        afxClassConfig pipClsCfg = cfg->pipClsCfg ? *cfg->pipClsCfg : _AVX_PIP_CLASS_CONFIG;
        AFX_ASSERT(pipClsCfg.fcc == afxFcc_PIP);
        AfxMountClass(&dsys->pipCls, NIL, classes, &pipClsCfg); // req SHD, LIGA, VTXD

        afxClassConfig qrypClsCfg = cfg->qrypClsCfg ? *cfg->qrypClsCfg : _AVX_QRYP_CLASS_CONFIG;
        AFX_ASSERT(qrypClsCfg.fcc == afxFcc_QRYP);
        AfxMountClass(&dsys->qrypCls, NIL, classes, &qrypClsCfg);

        afxClassConfig doutClsCfg = cfg->doutClsCfg ? *cfg->doutClsCfg : _AVX_DOUT_CLASS_CONFIG;
        AFX_ASSERT(doutClsCfg.fcc == afxFcc_DOUT);
        AfxMountClass(&dsys->doutCls, NIL, classes, &doutClsCfg); // req RAS, CANV

        afxClassConfig txdClsCfg = cfg->txdClsCfg ? *cfg->txdClsCfg : _AVX_TXD_CLASS_CONFIG;
        AFX_ASSERT(txdClsCfg.fcc == afxFcc_TXD);
        AfxMountClass(&dsys->txdCls, NIL, classes, &txdClsCfg); // req RAS

        afxClassConfig dexuClsCfg = cfg->dexuClsCfg ? *cfg->dexuClsCfg : _AVX_DEXU_CLASS_CONFIG;
        AFX_ASSERT(dexuClsCfg.fcc == afxFcc_DEXU);
        AfxMountClass(&dsys->dexuCls, NIL, classes, &dexuClsCfg);
    }

    afxUnit totalDqueCnt = 0;
    afxUnit bridgeCnt = cfg->bridgeCnt;

    for (afxUnit i = 0; i < bridgeCnt; i++)
    {
        AFX_ASSERT(bridgeCfgs);
        totalDqueCnt += bridgeCfgs[i].minQueCnt;
    }

    dsys->bridgeCnt = bridgeCnt;
    afxUnit baseQueIdx = 0;

    afxObjectStash const stashes[] =
    {
        {
            .cnt = bridgeCnt,
            .siz = sizeof(dsys->bridges[0]),
            .var = (void**)&dsys->bridges
        }
    };

    if (AfxAllocateInstanceData(dsys, ARRAY_SIZE(stashes), stashes))
    {
        AfxThrowError();

        // Dismout the classes and return the error.
        AfxDeregisterChainedClasses(&dsys->ctx.classes);
        AFX_ASSERT(AfxIsChainEmpty(&dsys->ctx.classes));
        return err;
    }

    if (_AvxDsysBridgeDevices(dsys, dsys->bridgeCnt, bridgeCfgs, dsys->bridges))
    {
        AfxThrowError();

        AfxDeallocateInstanceData(dsys, ARRAY_SIZE(stashes), stashes);

        // Dismout the classes and return the error.
        AfxDeregisterChainedClasses(&dsys->ctx.classes);
        AFX_ASSERT(AfxIsChainEmpty(&dsys->ctx.classes));
        return err;
    }
    AFX_ASSERT_OBJECTS(afxFcc_DEXU, dsys->bridgeCnt, dsys->bridges);

    dsys->ioExuMask = NIL;
    dsys->dedIoExuMask = NIL;
    dsys->gfxExuMask = NIL;
    dsys->cfxExuMask = NIL;
    dsys->dedCfxExuMask = NIL;
    dsys->videoExuMask = NIL;

    for (afxUnit i = 0; i < dsys->bridgeCnt; i++)
    {
        afxDrawBridge dexu;
        AvxGetDrawBridges(dsys, i, 1, &dexu);
        AFX_ASSERT_OBJECTS(afxFcc_DEXU, 1, &dexu);

        afxDrawDevice ddev = AvxGetBridgedDrawDevice(dexu, NIL);
        AFX_ASSERT_OBJECTS(afxFcc_DDEV, 1, &ddev);

        afxDrawPortInfo capsi;
        AvxQueryDrawCapabilities(ddev, &capsi);

        if ((capsi.capabilities & afxDrawCaps_TRANSFER) == afxDrawCaps_TRANSFER)
            dsys->ioExuMask |= AFX_BITMASK(i);
        if (capsi.capabilities & (afxDrawCaps_TRANSFER | afxDrawCaps_COMPUTE | afxDrawCaps_DRAW) == afxDrawCaps_TRANSFER)
            dsys->dedIoExuMask |= AFX_BITMASK(i);

        if ((capsi.capabilities & afxDrawCaps_COMPUTE) == afxDrawCaps_COMPUTE)
            dsys->cfxExuMask |= AFX_BITMASK(i);
        if ((capsi.capabilities & (afxDrawCaps_COMPUTE | afxDrawCaps_DRAW)) == afxDrawCaps_COMPUTE)
            dsys->dedCfxExuMask |= AFX_BITMASK(i);

        if ((capsi.capabilities & afxDrawCaps_DRAW) == afxDrawCaps_DRAW)
            dsys->gfxExuMask |= AFX_BITMASK(i);

        if ((capsi.capabilities & afxDrawCaps_PRESENT) == afxDrawCaps_PRESENT)
            dsys->videoExuMask |= AFX_BITMASK(i);
    }

    if (!err)
    {
        afxDrawDevice ddev = AvxGetBridgedDrawDevice(dsys->bridges[0], NIL);

        AfxCallDevice((afxDevice)ddev, 3, dsys);
        AfxCallDevice((afxDevice)ddev, 5, dsys);

        if (AfxDoDeviceService((afxDevice)ddev))
            AfxThrowError(); // let the device build its DPUs.
            
        if (!err)
        {
            if (AfxAcquireStringCatalogs(1, &dsys->shdStrb))
                AfxThrowError();

            {
                dsys->running = TRUE;
            }

            if (err && dsys->shdStrb)
                AfxDisposeObjects(1, &dsys->shdStrb);
        }
    }

    if (err)
    {
        AfxDeallocateInstanceData(dsys, ARRAY_SIZE(stashes), stashes);

        // Dismout the classes and return the error.
        AfxDeregisterChainedClasses(&dsys->ctx.classes);
        AFX_ASSERT(AfxIsChainEmpty(&dsys->ctx.classes));
    }

    return err;
}

_AVX afxClassConfig const _AVX_DSYS_CLASS_CONFIG =
{
    .fcc = afxFcc_DSYS,
    .name = "DrawSystem",
    .desc = "Draw I/O System",
    .fixedSiz = sizeof(AFX_OBJECT(afxDrawSystem)),
    .ctor = (void*)_AvxDsysCtorCb,
    .dtor = (void*)_AvxDsysDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AVX afxUnit AvxInvokeDrawSystems(afxUnit icd, afxUnit first, void *udd, afxBool(*f)(void*, afxDrawSystem), afxUnit cnt)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(cnt);
    AFX_ASSERT(f);
    afxUnit rslt = 0;

    afxModule mdle;
    if (_AvxGetIcd(icd, &mdle))
    {
        AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &mdle);
        AFX_ASSERT(AfxTestModule(mdle, afxModuleFlag_ICD | afxModuleFlag_AVX) == (afxModuleFlag_ICD | afxModuleFlag_AVX));
        afxClass const* cls = _AvxIcdGetDsysClass(mdle);
        AFX_ASSERT_CLASS(cls, afxFcc_DSYS);
        rslt = AfxInvokeObjects(cls, first, cnt, (void*)f, udd);
    }
    return rslt;
}

_AVX afxUnit AvxEvokeDrawSystems(afxUnit icd, afxUnit first, void* udd, afxBool(*f)(void*, afxDrawSystem), afxUnit cnt, afxDrawSystem systems[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(systems);
    AFX_ASSERT(f);
    afxUnit rslt = 0;

    afxModule mdle;
    if (_AvxGetIcd(icd, &mdle))
    {
        AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &mdle);
        AFX_ASSERT(AfxTestModule(mdle, afxModuleFlag_ICD | afxModuleFlag_AVX) == (afxModuleFlag_ICD | afxModuleFlag_AVX));
        afxClass const* cls = _AvxIcdGetDsysClass(mdle);
        AFX_ASSERT_CLASS(cls, afxFcc_DSYS);
        rslt = AfxEvokeObjects(cls, (void*)f, udd, first, cnt, (afxObject*)systems);
    }
    return rslt;
}

_AVX afxUnit AvxEnumerateDrawSystems(afxUnit icd, afxUnit first, afxUnit cnt, afxDrawSystem systems[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(systems);
    AFX_ASSERT(cnt);
    afxUnit rslt = 0;

    afxModule mdle;
    if (_AvxGetIcd(icd, &mdle))
    {
        AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &mdle);
        AFX_ASSERT(AfxTestModule(mdle, afxModuleFlag_ICD | afxModuleFlag_AVX) == (afxModuleFlag_ICD | afxModuleFlag_AVX));
        afxClass const* cls = _AvxIcdGetDsysClass(mdle);
        AFX_ASSERT_CLASS(cls, afxFcc_DSYS);
        rslt = AfxEnumerateObjects(cls, first, cnt, (afxObject*)systems);
    }
}

_AVX afxError AvxConfigureDrawSystem(afxUnit icd, afxDrawCaps caps, afxAcceleration accel, afxDrawSystemConfig* cfg)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(icd != AFX_INVALID_INDEX);
    AFX_ASSERT(cfg);

    if (!cfg)
    {
        AfxThrowError();
        return err;
    }

    *cfg = (afxDrawSystemConfig const) { 0 };

    afxModule drv;
    if (!_AvxGetIcd(icd, &drv))
    {
        AfxThrowError();
        return err;
    }
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &drv);
    AFX_ASSERT(AfxTestModule(drv, afxModuleFlag_ICD | afxModuleFlag_AVX));

    if (0 == cfg->exuCnt)
    {
        cfg->exuCnt = 0;

        for (afxUnit i = 0; i < AVX_MAX_BRIDGES_PER_SYSTEM; i++)
        {
            afxUnit ddevId = i;

            afxDrawDevice ddev;
            if (AvxEnumerateDrawDevices(icd, ddevId, 1, &ddev))
            {
                AFX_ASSERT_OBJECTS(afxFcc_DDEV, 1, &ddev);

                afxDrawPortInfo capsi;
                AvxQueryDrawCapabilities(ddev, &capsi);

                if (caps && !(caps & capsi.capabilities))
                    continue;

                if (accel && !(accel & capsi.acceleration))
                    continue;

                cfg->exus[cfg->exuCnt].capabilities = capsi.capabilities;
                cfg->exus[cfg->exuCnt].acceleration = capsi.acceleration;
                cfg->exus[cfg->exuCnt].ddevId = ddevId;
                cfg->exus[cfg->exuCnt].minQueCnt = capsi.minQueCnt;
                cfg->exus[cfg->exuCnt].queuePriority = NIL;
                ++cfg->exuCnt;
            }
        }
    }
    else
    {
        afxUnit exuCnt = cfg->exuCnt;
        cfg->exuCnt = 0;

        for (afxUnit i = 0; i < exuCnt; i++)
        {
            afxDrawPortInfo capsi;
            capsi.acceleration = cfg->exus[i].acceleration ? cfg->exus[i].acceleration : accel;
            capsi.capabilities = cfg->exus[i].capabilities ? cfg->exus[i].capabilities : caps;
            capsi.minQueCnt = cfg->exus[i].minQueCnt;

            afxUnit ddevId;
            if (AvxChooseDrawDevices(icd, NIL, NIL, &capsi, 1, &ddevId))
            {
                afxDrawDevice ddev;
                AvxEnumerateDrawDevices(icd, ddevId, 1, &ddev);
                AFX_ASSERT_OBJECTS(afxFcc_DDEV, 1, &ddev);

                AvxQueryDrawCapabilities(ddev, &capsi);

                cfg->exus[cfg->exuCnt].capabilities = capsi.capabilities;
                cfg->exus[cfg->exuCnt].acceleration = capsi.acceleration;
                cfg->exus[cfg->exuCnt].ddevId = ddevId;
                cfg->exus[cfg->exuCnt].minQueCnt = capsi.minQueCnt;
                cfg->exus[cfg->exuCnt].queuePriority = NIL;
                ++cfg->exuCnt;
            }
#if 0
            else
            {
                cfg->exus[i].capabilities = caps.capabilities;
                cfg->exus[i].acceleration = caps.acceleration;
                cfg->exus[i].minQueCnt = caps.minQueCnt;
                cfg->exus[i].ddevId = NIL;
            }
#endif
        }
    }

    return err;
}

_AVX afxError AvxEstablishDrawSystem(afxUnit icd, afxDrawSystemConfig const* cfg, afxDrawSystem* system)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(icd != AFX_INVALID_INDEX);
    AFX_ASSERT(system);
    AFX_ASSERT(cfg);

    if (!cfg)
    {
        AfxThrowError();
        return err;
    }
    else
    {
        AFX_ASSERT(cfg->exuCnt);

        if (!cfg->exuCnt)
        {
            AfxThrowError();
            return err;
        }
    }

    afxModule drv;
    if (!_AvxGetIcd(icd, &drv))
    {
        AfxThrowError();
        return err;
    }
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &drv);
    AFX_ASSERT(AfxTestModule(drv, afxModuleFlag_ICD | afxModuleFlag_AVX));

    // Acquire bridges and queues
    afxUnit totalDqueCnt = 0;
    afxUnit baseQueIdx[AVX_MAX_BRIDGES_PER_SYSTEM] = { 0 };
    _avxDexuAcquisition bridgeCfg[AVX_MAX_BRIDGES_PER_SYSTEM] = { 0 };
    afxUnit bridgeCnt = 0;

    AFX_ASSERT_RANGE(AVX_MAX_BRIDGES_PER_SYSTEM, 0, cfg->exuCnt);

    for (afxUnit i = 0; i < cfg->exuCnt; i++)
    {
        afxDrawBridgeConfig const* exuCfg = &cfg->exus[i];

        afxUnit bridgeIdx = AFX_INVALID_INDEX;

#if 0 // disable it to allow multibridge to same device port
        for (afxUnit j = 0; j < bridgeCnt; j++)
        {
            if (cfg->auxs[j].portId == bridgeCfg[j].portId)
            {
                bridgeCfg[bridgeIdx].minQueCnt += cfg->auxs[i].minQueCnt;
                bridgeCfg[bridgeIdx].queuePriority = NIL;
                //bridgeCfg[bridgeIdx].flags |= cfg->auxBridges[i].flags;

                baseQueIdx[bridgeIdx] = totalDqueCnt;
                totalDqueCnt += bridgeCfg[bridgeIdx].minQueCnt;
                break;
            }
        }
#endif

        // If not found, enlist this bridge it.
        if (bridgeIdx != AFX_INVALID_INDEX)
            continue;

        afxDrawDevice ddev;
        if (!AvxEnumerateDrawDevices(icd, exuCfg->ddevId, 1, &ddev))
        {
            AfxThrowError();
            break;
        }
        else
        {
            AFX_ASSERT_OBJECTS(afxFcc_DDEV, 1, &ddev);

            if (AfxCallDevice((afxDevice)ddev, 1, NIL))
            {
                AfxThrowError(); // let the device build its DPUs.
                break;
            }
        }

        bridgeCfg[bridgeCnt].ddev = ddev;

        afxUnit minQueCnt = AFX_CLAMP(exuCfg->minQueCnt, 1, AVX_MAX_QUEUES_PER_BRIDGE);
        afxDrawPortInfo capsi2 = { 0 };
        capsi2.capabilities = exuCfg->capabilities;
        capsi2.acceleration = exuCfg->acceleration;
        capsi2.minQueCnt = minQueCnt;

        bridgeCfg[bridgeCnt].exuIdx = bridgeCnt;
        bridgeCfg[bridgeCnt].minQueCnt = minQueCnt;
        bridgeCfg[bridgeCnt].dqueClsCfg = &_AVX_DQUE_CLASS_CONFIG;
        bridgeCfg[bridgeCnt].dctxClsCfg = &_AVX_DCTX_CLASS_CONFIG;
        ++bridgeCnt;
    }

    if (err)
    {
        return err;
    }

    _avxDsysAcquisition cfg2 = { 0 };
    cfg2.bridgeCnt = bridgeCnt;
    cfg2.reqExtCnt = cfg->reqExtCnt;
    cfg2.reqExts = cfg->reqExts;
    cfg2.reqFeatures = cfg->reqFeatures;
    cfg2.udd = cfg->udd;

    afxClass* cls = (afxClass*)_AvxIcdGetDsysClass(drv);
    AFX_ASSERT_CLASS(cls, afxFcc_DSYS);

    afxDrawSystem dsys;
    if (AfxAcquireObjects(cls, 1, (afxObject*)&dsys, (void const*[]) { drv, &cfg2, &bridgeCfg[0] }))
    {
        AfxThrowError();
        return err;
    }
    
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    *system = dsys;

    return err;
}
