/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *            Q W A D R O   M U L T I M E D I A   I N F R A S T R U C T U R E
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This software is part of Advanced Multimedia Extensions & Experiments.

// It is too hard to invent something when there is nothing to be copied. — Veryzon

#define _AFX_CORE_C
//#define _AFX_DEVICE_C
#define _AFX_CONTEXT_C

#define _AMX_MIX_C
//#define _AMX_SOUNDSCAPE_C
//#define _AMX_MIX_DEVICE_C
#define _AMX_MIX_SYSTEM_C
//#define _AMX_MIX_BRIDGE_C
//#define _AMX_MIX_QUEUE_C
//#define _AMX_MIX_INPUT_C
//#define _AMX_SINK_C
#include "ddi/amxImplementation.h"
#define _AUX_UX_C
#include "src/ux/impl/auxImplementation.h"

_AMX _amxMsysImpl const* _AmxMsysGetImpl(afxMixSystem msys)
{
    afxError err = AFX_ERR_NONE;
    // @msys must be a valid afxMixSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    return msys->ddi;
}

_AMX afxMask _AmxMsysGetIoExuMask(afxMixSystem msys, afxMask* dedIoExuMask)
{
    afxError err = AFX_ERR_NONE;
    // @msys must be a valid afxMixSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    if (dedIoExuMask) *dedIoExuMask = msys->dedIoExuMask;
    return msys->ioExuMask;
}

_AMX afxModule AmxGetMixSystemIcd(afxMixSystem msys)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    afxModule mdle = AfxGetHost(msys);
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &mdle);
    return mdle;
}

_AMX void AmxGetEnabledDrawFeatures(afxMixSystem msys, afxMixFeatures* features)
{
    afxError err = AFX_ERR_NONE;
    // @msys must be a valid afxMixSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);

    AFX_ASSERT(features);
    *features = msys->requirements;

    return;
}

_AMX afxClass const* _AmxMsysGetSinkClass(afxMixSystem msys)
{
    afxError err = AFX_ERR_NONE;
    // msys must be a valid afxMixSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    afxClass const* cls = &msys->asioCls;
    AFX_ASSERT_CLASS(cls, afxFcc_ASIO);
    return cls;
}

_AMX afxClass const* _AmxMsysGetTraxClass(afxMixSystem msys)
{
    afxError err = AFX_ERR_NONE;
    // msys must be a valid afxMixSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    afxClass const* cls = &msys->traxCls;
    AFX_ASSERT_CLASS(cls, afxFcc_TRAX);
    return cls;
}

_AMX afxClass const* _AmxMsysGetMexuClass(afxMixSystem msys)
{
    afxError err = AFX_ERR_NONE;
    // msys must be a valid afxMixDevice handle.
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    afxClass const* cls = &msys->mexuCls;
    AFX_ASSERT_CLASS(cls, afxFcc_MEXU);
    return cls;
}

_AMX afxClass const* _AmxMsysGetMixClass(afxMixSystem msys)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    afxClass const* cls = &msys->mixCls;
    AFX_ASSERT_CLASS(cls, afxFcc_MIX);
    return cls;
}

_AMX afxClass const* _AmxMsysGetBufClass(afxMixSystem msys)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    afxClass const* cls = &msys->mbufCls;
    AFX_ASSERT_CLASS(cls, afxFcc_MBUF);
    return cls;
}

_AMX afxClass const* _AmxMsysGetVidClass(afxMixSystem msys)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    afxClass const* cls = &msys->vidCls;
    AFX_ASSERT_CLASS(cls, afxFcc_VID);
    return cls;
}

_AMX afxClass const* _AmxMsysGetViddClass(afxMixSystem msys)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    afxClass const* cls = &msys->viddCls;
    AFX_ASSERT_CLASS(cls, afxFcc_VIDD);
    return cls;
}

_AMX afxClass const* _AmxMsysGetAudClass(afxMixSystem msys)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    afxClass const* cls = &msys->wavCls;
    AFX_ASSERT_CLASS(cls, afxFcc_AUD);
    return cls;
}

_AMX afxClass const* _AmxMsysGetSndsClass(afxMixSystem msys)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    afxClass const* cls = &msys->sndsCls;
    AFX_ASSERT_CLASS(cls, afxFcc_SNDS);
    return cls;
}

_AMX afxUnit AmxGetMixBridges(afxMixSystem msys, afxUnit baseIdx, afxUnit cnt, afxMixBridge bridges[])
{
    afxError err = AFX_ERR_NONE;
    // msys must be a valid afxMixSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);

    afxUnit bridgeCnt = msys->bridgeCnt;
    cnt = AFX_MIN(cnt, bridgeCnt - baseIdx);

    // bridges can be null or must be a valid pointer to afxMixBridge handles.
    if (!bridges) return cnt;

    //AFX_ASSERT_RANGE(bridgeCnt, baseExuIdx, cnt);
    baseIdx = AFX_MIN(baseIdx, bridgeCnt - 1);

    afxUnit rslt = 0;
    for (afxUnit i = 0; i < cnt; i++)
    {
        afxMixBridge mexu = msys->bridges[baseIdx + i];
        AFX_ASSERT_OBJECTS(afxFcc_MEXU, 1, &mexu);
        bridges[rslt++] = mexu;
    }
    return rslt;
}

_AMX afxUnit AmxChooseMixBridges(afxMixSystem msys, afxUnit mdevId, afxMixCaps caps, afxMask exuMask, afxUnit first, afxUnit maxCnt, afxMixBridge bridges[])
{
    afxError err = AFX_ERR_NONE;
    // msys must be a valid afxMixSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    afxUnit found = 0;
    afxUnit rslt = 0;

    if ((mdevId == AFX_INVALID_INDEX) && (!caps) && (!bridges))
        return maxCnt ? AFX_MIN(maxCnt, msys->bridgeCnt - first) : AFX_MIN(msys->bridgeCnt, msys->bridgeCnt - first);

    afxUnit bridgeCnt = msys->bridgeCnt;
    for (afxUnit exuIdx = 0; exuIdx < bridgeCnt; exuIdx++)
    {
        // skip if a mask is specified and this EXU is not bitmapped in it.
        if (exuMask && !(exuMask & AFX_BITMASK(exuIdx)))
            continue;

        afxMixBridge mexu = msys->bridges[exuIdx];
        AFX_ASSERT_OBJECTS(afxFcc_MEXU, 1, &mexu);

        afxMixDevice mdev = AmxGetBridgedMixDevice(mexu, NIL);
        AFX_ASSERT_OBJECTS(afxFcc_MDEV, 1, &mdev);

        if ((mdevId != AFX_INVALID_INDEX) && (mdevId != AfxGetObjectId(mdev)))
            continue;

        if (caps)
        {
            afxMixPortInfo capsi;
            AmxQueryMixCapabilities(mdev, &capsi);

            if ((capsi.capabilities & caps) != caps)
                continue;
        }

        if (maxCnt && (found >= first))
        {
            if (bridges)
            {
                bridges[rslt] = mexu;
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

_AMX afxError AmxWaitForMixQueue(afxMixSystem msys, afxTime timeout, afxUnit exuIdx, afxUnit queIdx)
{
    afxError err = AFX_ERR_NONE;
    // msys must be a valid afxMixSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    // exuIdx must be a valid index to a bridge.
    AFX_ASSERT_RANGE(msys->bridgeCnt, exuIdx, 1);

    afxMixBridge mexu;
    while (!AmxGetMixBridges(msys, exuIdx, 1, &mexu))
    {
        AfxThrowError();
        return err;
    }
    AFX_ASSERT_OBJECTS(afxFcc_MEXU, 1, &mexu);

    if (AmxWaitForEmptyMixQueue(mexu, queIdx, timeout))
        AfxThrowError();

    return err;
}

_AMX afxError AmxWaitForMixBridge(afxMixSystem msys, afxTime timeout, afxUnit exuIdx)
{
    afxError err = AFX_ERR_NONE;
    // msys must be a valid afxMixSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    // exuIdx must be a valid index to a bridge.
    AFX_ASSERT_RANGE((afxUnit)msys->bridgeCnt, exuIdx, 1);

    afxMixBridge mexu;
    while (!AmxGetMixBridges(msys, exuIdx, 1, &mexu))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT_OBJECTS(afxFcc_MEXU, 1, &mexu);
    AmxWaitForIdleMixBridge(mexu, timeout);

    return err;
}

_AMX afxError AmxWaitForMixSystem(afxMixSystem msys, afxTime timeout)
{
    afxError err = AFX_ERR_NONE;
    // msys must be a valid afxMixSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);

    if (!msys->waitCb)
    {
        afxUnit bridgeCnt = msys->bridgeCnt;

        for (afxUnit i = 0; i < bridgeCnt; i++)
            AmxWaitForMixBridge(msys, i, timeout);
    }
    else if (msys->waitCb(msys))
        AfxThrowError();

    return err;
}

_AMX afxError _AmxMsysTransferCb_SW(afxMixSystem msys, amxTransference* ctrl, afxUnit opCnt, void const* ops)
{
    afxError err = AFX_ERR_NONE;
    // @msys must be a valid afxMixSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    AFX_ASSERT(opCnt);
    AFX_ASSERT(ctrl);
    AFX_ASSERT(ops);

    afxMask msysIoExuMask = msys->ioExuMask;
    afxMask exuMask = ctrl->exuMask;
    afxUnit exuCnt = msys->bridgeCnt;
    afxUnit firstExuIdx = AfxRandom2(0, exuCnt - 1);

    AFX_ASSERT(!exuMask || (msysIoExuMask & exuMask));

    afxBool queued = FALSE;

    while (1)
    {
        for (afxUnit exuIdx = firstExuIdx; exuIdx < exuCnt; exuIdx++)
        {
            firstExuIdx = 0;

            // Skip non-transfer-capable EXUs.
            if (!(msysIoExuMask & AFX_BITMASK(exuIdx)))
                continue; // for

            if (exuMask && !(exuMask & AFX_BITMASK(exuIdx)))
                continue; // for

            // Try to pick one of the dedicated ones when EXUs are not specified.
            if ((!exuMask) && msys->dedIoExuMask && (!(msys->dedIoExuMask & AFX_BITMASK(exuIdx))))
                continue; // for
#if 0
            // if a mask is specified and it is not one of the existing dedicated EXUs in mask.
            if (msys->dedIoExuMask)
            {
                if (exuMask)
                {
                    if ((exuMask & msys->dedIoExuMask))
                    {
                        if (!(msys->dedIoExuMask & AFX_BITMASK(exuIdx)))
                            continue;
                    }
                }
                else
                {

                }

                if (exuMask && (msys->dedIoExuMask & AFX_BITMASK(exuIdx)))
                    continue;
            }
#endif

            afxMixBridge mexu;
            if (!AmxGetMixBridges(msys, exuIdx, 1, &mexu))
            {
                AfxThrowError();
                continue;
            }

            afxError err2 = _AmxMexuTransferMixMemory(mexu, ctrl, opCnt, ops);

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

_AMX afxError _AmxMsysRemapBuffersCb_SW(afxMixSystem msys, afxBool unmap, afxUnit cnt, _amxBufferRemapping const maps[])
{
    afxError err = AFX_ERR_NONE;
    // @msys must be a valid afxMixSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    AFX_ASSERT(cnt);
    AFX_ASSERT(maps);

    afxError queErr;
    afxBool queued = FALSE;

    afxMask dedIoExuMask;
    afxMask ioExuMask = _AmxMsysGetIoExuMask(msys, &dedIoExuMask);
    afxUnit exuIdx = 0;
    afxMixBridge mexu;
    afxUnit exuCnt;

    // Firstly, try to put them in a dedicated queue.
    if (dedIoExuMask)
    {
        exuCnt = AmxChooseMixBridges(msys, AFX_INVALID_INDEX, afxMixCaps_TRANSFER, dedIoExuMask, 0, 0, NIL);
        AFX_ASSERT(exuCnt);
        exuIdx = 0;
        while (AmxChooseMixBridges(msys, AFX_INVALID_INDEX, afxMixCaps_TRANSFER, dedIoExuMask, exuIdx++, 1, &mexu))
        {
            queErr = _AmxMexuRemapBuffers(mexu, unmap, cnt, maps);
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
        exuCnt = AmxChooseMixBridges(msys, AFX_INVALID_INDEX, afxMixCaps_TRANSFER, ioExuMask, 0, 0, NIL);
        AFX_ASSERT(exuCnt);
        exuIdx = 0;
        while (AmxChooseMixBridges(msys, AFX_INVALID_INDEX, afxMixCaps_TRANSFER, ioExuMask, exuIdx++, 1, &mexu))
        {
            queErr = _AmxMexuRemapBuffers(mexu, unmap, cnt, maps);
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

_AMX afxError _AmxMsysCohereMappedBuffersCb_SW(afxMixSystem msys, afxBool discard, afxUnit cnt, amxBufferedMap const maps[])
{
    afxError err = AFX_ERR_NONE;
    // @msys must be a valid afxMixSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    AFX_ASSERT(cnt);
    AFX_ASSERT(maps);

    afxError queErr;
    afxBool queued = FALSE;

    afxMask dedIoExuMask;
    afxMask ioExuMask = _AmxMsysGetIoExuMask(msys, &dedIoExuMask);
    afxUnit exuIdx = 0;
    afxMixBridge mexu;
    afxUnit exuCnt;

    // Firstly, try to put them in a dedicated queue.
    if (dedIoExuMask)
    {
        exuCnt = AmxChooseMixBridges(msys, AFX_INVALID_INDEX, afxMixCaps_TRANSFER, dedIoExuMask, 0, 0, NIL);
        AFX_ASSERT(exuCnt);
        exuIdx = 0;
        while (AmxChooseMixBridges(msys, AFX_INVALID_INDEX, afxMixCaps_TRANSFER, dedIoExuMask, exuIdx++, 1, &mexu))
        {
            queErr = _AmxMexuCohereMappedBuffers(mexu, discard, cnt, maps);
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
        exuCnt = AmxChooseMixBridges(msys, AFX_INVALID_INDEX, afxMixCaps_TRANSFER, ioExuMask, 0, 0, NIL);
        AFX_ASSERT(exuCnt);
        exuIdx = 0;
        while (AmxChooseMixBridges(msys, AFX_INVALID_INDEX, afxMixCaps_TRANSFER, ioExuMask, exuIdx++, 1, &mexu))
        {
            queErr = _AmxMexuCohereMappedBuffers(mexu, discard, cnt, maps);
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

_AMX _amxMsysImpl const _AMX_MSYS_IMPL =
{
#if 0
    .fencCls = _AmxMsysGetFencClassCb_SW,
    .mexuCls = _AmxMsysGetDexuClassCb_SW,
    .sinkCls = _AmxMsysGetDoutClassCb_SW,
    .bufCls = _AmxMsysGetBufClassCb_SW,
#endif
    .transferCb = _AmxMsysTransferCb_SW,
    .cohereCb = _AmxMsysCohereMappedBuffersCb_SW,
    .remapCb = _AmxMsysRemapBuffersCb_SW,
#if 0
    .allocBufCb = _AmxMsysAllocateBuffersCb_SW,
    .deallocBufCb = _AmxMsysDeallocateBuffersCb_SW,
#endif
};

_AMX afxError _AmxMsysDtorCb(afxMixSystem msys)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);

    afxModule icd = AmxGetMixSystemIcd(msys);
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &icd);

    AmxWaitForMixSystem(msys, AFX_TIMEOUT_INFINITE);
    AmxWaitForMixSystem(msys, AFX_TIMEOUT_INFINITE);

    afxUnit bridgeCnt = msys->bridgeCnt;

    AfxDeregisterChainedClasses(&msys->ctx.classes);
    AFX_ASSERT(AfxIsChainEmpty(&msys->ctx.classes));

    for (afxUnit i = bridgeCnt; i-- > 0;)
    {
        AFX_ASSERT_OBJECTS(afxFcc_MEXU, 1, &msys->bridges[i]);
        AfxDisposeObjects(1, &msys->bridges[i]);
    }

    afxObjectStash const stashes[] =
    {
        {
            .cnt = bridgeCnt,
            .siz = sizeof(msys->bridges[0]),
            .var = (void**)&msys->bridges
        }
    };

    if (AfxDeallocateInstanceData(msys, ARRAY_SIZE(stashes), stashes))
        AfxThrowError();

    if (msys->dsys)
    {
        AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &msys->dsys);
        AfxDisposeObjects(1, &msys->dsys);
    }

    return err;
}

_AMX afxError _AmxMsysCtorCb(afxMixSystem msys, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    //AFX_ASSERT(spec);

    afxModule icd = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &icd);
    _amxMsysAcquisition const* cfg = AFX_CAST(_amxMsysAcquisition const*, args[1]) + invokeNo;
    _amxMexuAcquisition* bridgeCfgs = AFX_CAST(_amxMexuAcquisition*, args[2]) + invokeNo;

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

    msys->ddi = &_AMX_MSYS_IMPL;

    msys->running = FALSE;

    msys->requirements = cfg->reqFeatures;

    {
        afxChain* classes = &msys->ctx.classes;
        AfxDeployChain(classes, (void*)msys);

        afxClassConfig clsCfg;

        clsCfg = cfg->sndsClsCfg ? *cfg->sndsClsCfg : _AMX_SNDS_CLASS_CONFIG;
        AFX_ASSERT(clsCfg.fcc == afxFcc_SNDS);
        AfxMountClass(&msys->sndsCls, NIL, classes, &clsCfg);

        clsCfg = cfg->mbufClsCfg ? *cfg->mbufClsCfg : _AMX_MBUF_CLASS_CONFIG;
        AFX_ASSERT(clsCfg.fcc == afxFcc_MBUF);
        AfxMountClass(&msys->mbufCls, NIL, classes, &clsCfg);

        clsCfg = cfg->wavClsCfg ? *cfg->wavClsCfg : _AMX_AUD_CLASS_CONFIG;
        AFX_ASSERT(clsCfg.fcc == afxFcc_AUD);
        AfxMountClass(&msys->wavCls, NIL, classes, &clsCfg);

        clsCfg = cfg->vidClsCfg ? *cfg->vidClsCfg : _AMX_VID_CLASS_CONFIG;
        AFX_ASSERT(clsCfg.fcc == afxFcc_VID);
        AfxMountClass(&msys->vidCls, NIL, classes, &clsCfg);

        clsCfg = cfg->viddClsCfg ? *cfg->viddClsCfg : _AMX_VIDD_CLASS_CONFIG;
        AFX_ASSERT(clsCfg.fcc == afxFcc_VIDD);
        AfxMountClass(&msys->viddCls, NIL, classes, &clsCfg);

        clsCfg = cfg->mixClsCfg ? *cfg->mixClsCfg : _AMX_MIX_CLASS_CONFIG;
        AFX_ASSERT(clsCfg.fcc == afxFcc_MIX);
        AfxMountClass(&msys->mixCls, NIL, classes, &clsCfg);

        clsCfg = cfg->traxClsCfg ? *cfg->traxClsCfg : _AMX_TRAX_CLASS_CONFIG;
        AFX_ASSERT(clsCfg.fcc == afxFcc_TRAX);
        AfxMountClass(&msys->traxCls, NIL, classes, &clsCfg);

        afxClassConfig asioClsCfg;
        if (cfg->sinkClsCfg) asioClsCfg = *cfg->sinkClsCfg;
        else
        {
            asioClsCfg = _AMX_ASIO_CLASS_CONFIG;
            if (_AuxGetInteropSinkClass(msys, &AFX_STRING(""), &asioClsCfg))
            {
                asioClsCfg = _AMX_ASIO_CLASS_CONFIG;
            }
        }
        AFX_ASSERT(asioClsCfg.fcc == afxFcc_ASIO);
        AfxMountClass(&msys->asioCls, NIL, classes, &asioClsCfg); // require mdev, sout

        clsCfg = cfg->mexuClsCfg ? *cfg->mexuClsCfg : _AMX_MEXU_CLASS_CONFIG;
        AFX_ASSERT(clsCfg.fcc == afxFcc_MEXU);
        AfxMountClass(&msys->mexuCls, NIL, classes, &clsCfg);
    }

    AfxZero(msys->heads, sizeof(msys->heads));

    afxUnit totalDqueCnt = 0;
    afxUnit bridgeCnt = cfg->bridgeCnt;

    for (afxUnit i = 0; i < bridgeCnt; i++)
    {
        AFX_ASSERT(bridgeCfgs);
        totalDqueCnt += bridgeCfgs[i].minQueCnt;
    }

    msys->bridgeCnt = bridgeCnt;
    afxUnit baseQueIdx = 0;

    afxObjectStash const stashes[] =
    {
        {
            .cnt = bridgeCnt,
            .siz = sizeof(msys->bridges[0]),
            .var = (void**)&msys->bridges
        }
    };

    if (AfxAllocateInstanceData(msys, ARRAY_SIZE(stashes), stashes))
    {
        AfxThrowError();

        // Dismout the classes and return the error.
        AfxDeregisterChainedClasses(&msys->ctx.classes);
        AFX_ASSERT(AfxIsChainEmpty(&msys->ctx.classes));
        return err;
    }

    if ((msys->dsys = cfg->dsys))
    {
        msys->dsys = cfg->dsys;
        AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &msys->dsys);
        AfxReacquireObjects(1, &msys->dsys);
    }

    if (_AmxMsysBridgeDevices(msys, msys->bridgeCnt, bridgeCfgs, msys->bridges))
    {
        AfxThrowError();

        AfxDisposeObjects(1, &msys->dsys);

        AfxDeallocateInstanceData(msys, ARRAY_SIZE(stashes), stashes);

        // Dismout the classes and return the error.
        AfxDeregisterChainedClasses(&msys->ctx.classes);
        AFX_ASSERT(AfxIsChainEmpty(&msys->ctx.classes));
        return err;
    }
    AFX_ASSERT_OBJECTS(afxFcc_MEXU, msys->bridgeCnt, msys->bridges);

    msys->ioExuMask = NIL;
    msys->dedIoExuMask = NIL;
    msys->gfxExuMask = NIL;
    msys->cfxExuMask = NIL;
    msys->dedCfxExuMask = NIL;
    msys->videoExuMask = NIL;

    for (afxUnit i = 0; i < msys->bridgeCnt; i++)
    {
        afxMixBridge mexu;
        AmxGetMixBridges(msys, i, 1, &mexu);
        AFX_ASSERT_OBJECTS(afxFcc_MEXU, 1, &mexu);

        afxMixDevice mdev = AmxGetBridgedMixDevice(mexu, NIL);
        AFX_ASSERT_OBJECTS(afxFcc_MDEV, 1, &mdev);

        afxMixPortInfo capsi;
        AmxQueryMixCapabilities(mdev, &capsi);

        if ((capsi.capabilities & afxMixCaps_TRANSFER) == afxMixCaps_TRANSFER)
            msys->ioExuMask |= AFX_BITMASK(i);
        if ((capsi.capabilities & (afxMixCaps_TRANSFER | afxMixCaps_COMPUTE | afxMixCaps_MIX)) == afxMixCaps_TRANSFER)
            msys->dedIoExuMask |= AFX_BITMASK(i);

        if ((capsi.capabilities & afxMixCaps_COMPUTE) == afxMixCaps_COMPUTE)
            msys->cfxExuMask |= AFX_BITMASK(i);
        if ((capsi.capabilities & (afxMixCaps_COMPUTE | afxMixCaps_MIX)) == afxMixCaps_COMPUTE)
            msys->dedCfxExuMask |= AFX_BITMASK(i);

        if ((capsi.capabilities & afxMixCaps_MIX) == afxMixCaps_MIX)
            msys->gfxExuMask |= AFX_BITMASK(i);

        if ((capsi.capabilities & afxDrawFn_PRESENT) == afxDrawFn_PRESENT)
            msys->videoExuMask |= AFX_BITMASK(i);
    }

    AfxDeployChain(&msys->activeTrackers, msys);

    afxMixDevice mdev = AmxGetBridgedMixDevice(msys->bridges[0], NIL);

    AfxCallDevice((afxDevice)mdev, 3, msys);
    AfxCallDevice((afxDevice)mdev, 5, msys);

    if (AfxDoDeviceService((afxDevice)mdev)) AfxThrowError(); // let the device build its SPUs.
    else
    {
        msys->running = TRUE;
    }

    if (err)
    {
        // Dismout the classes and return the error.
        AfxDeregisterChainedClasses(&msys->ctx.classes);
        AFX_ASSERT(AfxIsChainEmpty(&msys->ctx.classes));

        AfxDeallocateInstanceData(msys, ARRAY_SIZE(stashes), stashes);
    }
    return err;
}

_AMX afxClassConfig const _AMX_MSYS_CLASS_CONFIG =
{
    .fcc = afxFcc_MSYS,
    .name = "MixSystem",
    .desc = "Mix I/O System",
    .fixedSiz = sizeof(AFX_OBJECT(afxMixSystem)),
    .ctor = (void*)_AmxMsysCtorCb,
    .dtor = (void*)_AmxMsysDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AMX afxUnit AmxEnumerateMixSystems(afxUnit icd, afxUnit first, afxUnit cnt, afxMixSystem systems[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(systems);
    AFX_ASSERT(cnt);
    afxUnit rslt = 0;

    afxModule mdle;
    if (_AmxGetIcd(icd, &mdle))
    {
        AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &mdle);
        AFX_ASSERT(AfxTestModule(mdle, afxModuleFlag_ICD | afxModuleFlag_AMX) == (afxModuleFlag_ICD | afxModuleFlag_AMX));
        afxClass const* cls = _AmxIcdGetMsysClass(mdle);
        AFX_ASSERT_CLASS(cls, afxFcc_MSYS);
        rslt = AfxEnumerateObjects(cls, first, cnt, (afxObject*)systems);
    }
}

_AMX afxUnit AmxEvokeMixSystems(afxUnit icd, afxUnit first, void* udd, afxBool(*f)(void*, afxMixSystem), afxUnit cnt, afxMixSystem systems[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(systems);
    AFX_ASSERT(cnt);
    AFX_ASSERT(f);
    afxUnit rslt = 0;

    afxModule mdle;
    if (_AmxGetIcd(icd, &mdle))
    {
        AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &mdle);
        AFX_ASSERT(AfxTestModule(mdle, afxModuleFlag_ICD | afxModuleFlag_AMX) == (afxModuleFlag_ICD | afxModuleFlag_AMX));
        afxClass const* cls = _AmxIcdGetMsysClass(mdle);
        AFX_ASSERT_CLASS(cls, afxFcc_MSYS);
        rslt = AfxEvokeObjects(cls, (void*)f, udd, first, cnt, (afxObject*)systems);
    }
    return rslt;
}

_AMX afxUnit AmxInvokeMixSystems(afxUnit icd, afxUnit first, void *udd, afxBool(*f)(void*, afxMixSystem), afxUnit cnt)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(cnt);
    AFX_ASSERT(f);
    afxUnit rslt = 0;

    afxModule mdle;
    if (_AmxGetIcd(icd, &mdle))
    {
        AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &mdle);
        AFX_ASSERT(AfxTestModule(mdle, afxModuleFlag_ICD | afxModuleFlag_AMX) == (afxModuleFlag_ICD | afxModuleFlag_AMX));
        afxClass const* cls = _AmxIcdGetMsysClass(mdle);
        AFX_ASSERT_CLASS(cls, afxFcc_MSYS);
        rslt = AfxInvokeObjects(cls, first, cnt, (void*)f, udd);
    }
    return rslt;
}

_AMX afxError AmxConfigureMixSystem(afxUnit icd, afxMixSystemConfig* cfg)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(icd != AFX_INVALID_INDEX);
    AFX_ASSERT(cfg);

    if (!cfg)
    {
        AfxThrowError();
        return err;
    }

    afxMixCaps caps = cfg->caps;
    afxAcceleration accel = cfg->accel;

    afxModule drv;
    if (!_AmxGetIcd(icd, &drv))
    {
        AfxThrowError();
        return err;
    }
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &drv);
    AFX_ASSERT(AfxTestModule(drv, afxModuleFlag_ICD | afxModuleFlag_AMX));

    if (0 == cfg->exuCnt)
    {
        cfg->exuCnt = 0;

        for (afxUnit i = 0; i < AVX_MAX_BRIDGES_PER_SYSTEM; i++)
        {
            afxUnit mdevId = i;

            afxMixDevice mdev;
            if (AmxEnumerateMixDevices(icd, mdevId, 1, &mdev))
            {
                AFX_ASSERT_OBJECTS(afxFcc_MDEV, 1, &mdev);

                afxMixPortInfo capsi;
                AmxQueryMixCapabilities(mdev, &capsi);

                if (caps && !(caps & capsi.capabilities))
                    continue;

                if (accel && !(accel & capsi.acceleration))
                    continue;

                cfg->exus[cfg->exuCnt].capabilities = capsi.capabilities;
                cfg->exus[cfg->exuCnt].acceleration = capsi.acceleration;
                cfg->exus[cfg->exuCnt].mdevId = mdevId;
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
            afxMixPortInfo capsi = { 0 };
            capsi.acceleration = cfg->exus[i].acceleration ? cfg->exus[i].acceleration : accel;
            capsi.capabilities = cfg->exus[i].capabilities ? cfg->exus[i].capabilities : caps;
            capsi.minQueCnt = cfg->exus[i].minQueCnt;

            afxUnit mdevId;
            if (AmxChooseMixDevices(icd, NIL, NIL, &capsi, 1, &mdevId))
            {
                afxMixDevice mdev;
                AmxEnumerateMixDevices(icd, mdevId, 1, &mdev);
                AFX_ASSERT_OBJECTS(afxFcc_MDEV, 1, &mdev);

                AmxQueryMixCapabilities(mdev, &capsi);

                cfg->exus[cfg->exuCnt].capabilities = capsi.capabilities;
                cfg->exus[cfg->exuCnt].acceleration = capsi.acceleration;
                cfg->exus[cfg->exuCnt].mdevId = mdevId;
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

_AMX afxError AmxEstablishMixSystem(afxUnit icd, afxMixSystemConfig const* cfg, afxMixSystem* system)
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
    if (!_AmxGetIcd(icd, &drv))
    {
        AfxThrowError();
        return err;
    }
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &drv);
    AFX_ASSERT(AfxTestModule(drv, afxModuleFlag_ICD | afxModuleFlag_AMX));

    // Acquire bridges and queues
    afxUnit totalSqueCnt = 0;
    afxUnit baseQueIdx[AMX_MAX_BRIDGES_PER_SYSTEM] = { 0 };
    _amxMexuAcquisition bridgeCfg[AMX_MAX_BRIDGES_PER_SYSTEM] = { 0 };
    afxUnit bridgeCnt = 0;

    AFX_ASSERT_RANGE(AMX_MAX_BRIDGES_PER_SYSTEM, 0, cfg->exuCnt);

    for (afxUnit i = 0; i < cfg->exuCnt; i++)
    {
        afxMixBridgeConfig const* exuCfg = &cfg->exus[i];

        afxUnit bridgeIdx = AFX_INVALID_INDEX;

#if 0 // disable to allowed multibridge to same port
        for (afxUnit j = 0; j < bridgeCnt; j++)
        {
            if (cfg->auxs[j].portId == bridgeCfg[j].portId)
            {
                bridgeCfg[bridgeIdx].minQueCnt += cfg->auxs[i].minQueCnt;
                bridgeCfg[bridgeIdx].queuePriority = NIL;
                //bridgeCfg[bridgeIdx].flags |= cfg->auxBridges[i].flags;

                baseQueIdx[bridgeIdx] = totalSqueCnt;
                totalSqueCnt += bridgeCfg[bridgeIdx].minQueCnt;
                break;
            }
        }
#endif

        // If not found, enlist this bridge it.
        if (bridgeIdx != AFX_INVALID_INDEX)
            continue;

        afxMixDevice mdev;
        if (!AmxEnumerateMixDevices(icd, exuCfg->mdevId, 1, &mdev))
        {
            AfxThrowError();
            break;
        }
        else
        {
            AFX_ASSERT_OBJECTS(afxFcc_MDEV, 1, &mdev);

            if (AfxCallDevice((afxDevice)mdev, 1, NIL))
            {
                AfxThrowError(); // let the device build its DPUs.
                break;
            }
        }

        bridgeCfg[bridgeCnt].mdev = mdev;

        afxUnit minQueCnt = AFX_CLAMP(exuCfg->minQueCnt, 1, AMX_MAX_QUEUES_PER_BRIDGE);;
        afxMixPortInfo capsi2 = { 0 };
        capsi2.capabilities = exuCfg->capabilities;
        capsi2.acceleration = exuCfg->acceleration;
        capsi2.minQueCnt = minQueCnt;

        bridgeCfg[bridgeCnt].exuIdx = bridgeCnt;
        bridgeCfg[bridgeCnt].minQueCnt = minQueCnt;
        bridgeCfg[bridgeCnt].mqueClsCfg = &_AMX_MQUE_CLASS_CONFIG;
        bridgeCfg[bridgeCnt].mixClsCfg = &_AMX_MIX_CLASS_CONFIG;
        ++bridgeCnt;
    }

    if (err)
    {
        return err;
    }

    _amxMsysAcquisition cfg2 = { 0 };
    cfg2.bridgeCnt = bridgeCnt;
    cfg2.reqExtCnt = cfg->extCnt;
    cfg2.reqExts = cfg->exts;
    cfg2.reqFeatures = cfg->features;
    cfg2.udd = cfg->udd;
    cfg2.dsys = cfg->dsys;

    afxClass* cls = (afxClass*)_AmxIcdGetMsysClass(drv);
    AFX_ASSERT_CLASS(cls, afxFcc_MSYS);

    afxMixSystem msys;
    if (AfxAcquireObjects(cls, 1, (afxObject*)&msys, (void const*[]) { drv, &cfg2, &bridgeCfg[0], }))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    *system = msys;
    return err;
}
