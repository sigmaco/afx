/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *       Q W A D R O   S O U N D   S Y N T H E S I S   I N F R A S T R U C T U R E
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

 // It is too hard to invent something when there is nothing to be copied. — Veryzon

#define _AFX_CORE_C
//#define _AFX_DEVICE_C
#define _AFX_CONTEXT_C

#define _AMX_MIX_C
//#define _AMX_AUDIENT_C
//#define _AMX_MIX_DEVICE_C
#define _AMX_MIX_SYSTEM_C
//#define _AMX_MIX_BRIDGE_C
//#define _AMX_MIX_QUEUE_C
//#define _AMX_MIX_INPUT_C
//#define _AMX_SINK_C
#include "impl/amxImplementation.h"

_AMX afxModule AfxGetMixSystemIcd(afxMixSystem msys)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    afxModule mdle = AfxGetProvider(msys);
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &mdle);
    return mdle;
}

_AMX afxClass const* _AmxGetAudioSinkClass(afxMixSystem msys)
{
    afxError err = AFX_ERR_NONE;
    // msys must be a valid afxMixSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    afxClass const* cls = &msys->asioCls;
    AFX_ASSERT_CLASS(cls, afxFcc_ASIO);
    return cls;
}

_AMX afxClass const* _AmxGetSessionClass(afxMixSystem msys)
{
    afxError err = AFX_ERR_NONE;
    // msys must be a valid afxMixSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    afxClass const* cls = &msys->msesCls;
    AFX_ASSERT_CLASS(cls, afxFcc_MSES);
    return cls;
}

_AMX afxClass const* _AmxGetMixBridgeClass(afxMixSystem msys)
{
    afxError err = AFX_ERR_NONE;
    // msys must be a valid afxMixDevice handle.
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    afxClass const* cls = &msys->mexuCls;
    AFX_ASSERT_CLASS(cls, afxFcc_MEXU);
    return cls;
}

_AMX afxClass const* _AmxGetMixContextClass(afxMixSystem msys)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    afxClass const* cls = &msys->mixCls;
    AFX_ASSERT_CLASS(cls, afxFcc_MIX);
    return cls;
}

_AMX afxClass const* _AmxGetBufferClass(afxMixSystem msys)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    afxClass const* cls = &msys->mbufCls;
    AFX_ASSERT_CLASS(cls, afxFcc_MBUF);
    return cls;
}

_AMX afxClass const* _AmxGetAudioClass(afxMixSystem msys)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    afxClass const* cls = &msys->wavCls;
    AFX_ASSERT_CLASS(cls, afxFcc_AUD);
    return cls;
}

_AMX afxClass const* _AmxGetSoundClass(afxMixSystem msys)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    afxClass const* cls = &msys->sndCls;
    AFX_ASSERT_CLASS(cls, afxFcc_SND);
    return cls;
}

_AMX afxClass const* _AmxGetAudientClass(afxMixSystem msys)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    afxClass const* cls = &msys->audiCls;
    AFX_ASSERT_CLASS(cls, afxFcc_SNDS);
    return cls;
}

_AMX afxUnit AmxGetMixBridges(afxMixSystem msys, afxUnit baseIdx, afxUnit cnt, afxMixBridge bridges[])
{
    afxError err = AFX_ERR_NONE;
    // dsys must be a valid afxMixSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    // bridges must be a valid pointer to afxMixBridge handles.
    AFX_ASSERT(bridges);
    afxUnit rslt = 0;

    afxUnit bridgeCnt = msys->bridgeCnt;
    AFX_ASSERT_RANGE(bridgeCnt, baseIdx, cnt);

    if (baseIdx < bridgeCnt)
    {
        for (afxUnit i = 0; i < cnt; i++)
        {
            afxMixBridge mexu = msys->bridges[baseIdx + i];
            AFX_ASSERT_OBJECTS(afxFcc_MEXU, 1, &mexu);
            bridges[rslt++] = mexu;
        }
    }
    return rslt;
}

_AMX afxUnit AfxQueryMixBridges(afxMixSystem msys, afxUnit sdevId, afxUnit portId, afxUnit first, afxUnit cnt, afxMixBridge bridges[])
{
    afxError err = AFX_ERR_NONE;
    // msys must be a valid afxMixSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    afxUnit found = 0;
    afxUnit rslt = 0;

    afxUnit bridgeCnt = msys->bridgeCnt;
    for (afxUnit i = 0; i < bridgeCnt; i++)
    {
        afxMixBridge mexu = msys->bridges[i];
        AFX_ASSERT_OBJECTS(afxFcc_MEXU, 1, &mexu);
        afxUnit portId2;
        afxMixDevice mdev = AmxGetBridgedMixDevice(mexu, &portId2);
        AFX_ASSERT_OBJECTS(afxFcc_MDEV, 1, &mdev);

        if ((sdevId != AFX_INVALID_INDEX) && 
            (sdevId != AfxGetObjectId(mdev)))
            continue;

        if ((portId != AFX_INVALID_INDEX) && 
            (portId != portId2))
            continue;

        if (cnt && (found >= first))
        {
            if (bridges)
            {
                bridges[rslt] = mexu;
            }

            if (cnt == rslt)
                break;
        }

        if (found >= first)
            rslt++;

        found++;
    }
    return rslt;
}

_AMX afxError AfxWaitForMixQueue(afxMixSystem msys, afxUnit exuIdx, afxUnit queIdx, afxTime timeout)
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

_AMX afxError AmxWaitForMixBridge(afxMixSystem msys, afxUnit exuIdx, afxTime timeout)
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

_AMX afxError AfxWaitForMixSystem(afxMixSystem msys, afxTime timeout)
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

_AMX afxError AfxSinkAudioSignal(afxMixSystem msys, void* ctrl, afxSink sink, amxAudio aud, amxAudioPeriod const* seg)
{
    afxError err = AFX_ERR_NONE;
    // msys must be a valid afxMixSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    //AFX_ASSERT(ctrl);

    afxMixBridge mexu;
    AFX_ASSERT_RANGE(msys->bridgeCnt, /*ctrl->exuIdx*/0, 1);
    if (!AmxGetMixBridges(msys, /*ctrl->exuIdx*/0, 1, &mexu))
    {
        AfxThrowError();
        return err;
    }
    AFX_ASSERT_OBJECTS(afxFcc_MEXU, 1, &mexu);

    afxSink sout = sink;
    AFX_ASSERT_OBJECTS(afxFcc_ASIO, 1, &sout);
    afxMixSystem con = AfxGetAudioSinkContext(sout);
    
    //avxPresentation ctrl2 = *ctrl;
    
    if (_AmxMexuSubmitSink(mexu, ctrl, sink, aud, seg))
        AfxThrowError();

    return err;
}

_AMX afxError _AmxMsysDtorCb(afxMixSystem msys)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);

    afxModule icd = AfxGetMixSystemIcd(msys);
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &icd);

    AfxWaitForMixSystem(msys, AFX_TIME_INFINITE);
    AfxWaitForMixSystem(msys, AFX_TIME_INFINITE);

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

    msys->running = FALSE;

    msys->requirements = cfg->reqFeatures;

    {
        afxChain* classes = &msys->ctx.classes;
        AfxDeployChain(classes, (void*)msys);

        afxClassConfig clsCfg;

        clsCfg = cfg->audiClsCfg ? *cfg->audiClsCfg : _AMX_SNDS_CLASS_CONFIG;
        AFX_ASSERT(clsCfg.fcc == afxFcc_SNDS);
        AfxMountClass(&msys->audiCls, NIL, classes, &clsCfg);

        clsCfg = cfg->mbufClsCfg ? *cfg->mbufClsCfg : _AMX_MBUF_CLASS_CONFIG;
        AFX_ASSERT(clsCfg.fcc == afxFcc_MBUF);
        AfxMountClass(&msys->mbufCls, NIL, classes, &clsCfg);

        clsCfg = cfg->wavClsCfg ? *cfg->wavClsCfg : _AMX_AUD_CLASS_CONFIG;
        AFX_ASSERT(clsCfg.fcc == afxFcc_AUD);
        AfxMountClass(&msys->wavCls, NIL, classes, &clsCfg);

        clsCfg = cfg->sndClsCfg ? *cfg->sndClsCfg : _AMX_SND_CLASS_CONFIG;
        AFX_ASSERT(clsCfg.fcc == afxFcc_SND);
        AfxMountClass(&msys->sndCls, NIL, classes, &clsCfg);

        clsCfg = cfg->mixClsCfg ? *cfg->mixClsCfg : _AMX_MIX_CLASS_CONFIG;
        AFX_ASSERT(clsCfg.fcc == afxFcc_MIX);
        AfxMountClass(&msys->mixCls, NIL, classes, &clsCfg);

        clsCfg = cfg->msesClsCfg ? *cfg->msesClsCfg : _AMX_MSES_CLASS_CONFIG;
        AFX_ASSERT(clsCfg.fcc == afxFcc_MSES);
        AfxMountClass(&msys->msesCls, NIL, classes, &clsCfg);

        clsCfg = cfg->sinkClsCfg ? *cfg->sinkClsCfg : _AMX_ASIO_CLASS_CONFIG;
        AFX_ASSERT(clsCfg.fcc == afxFcc_ASIO);
        AfxMountClass(&msys->asioCls, NIL, classes, &clsCfg); // require mdev, sout

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

    afxClass* cls = (afxClass*)_AmxGetMixBridgeClass(msys);
    AFX_ASSERT_CLASS(cls, afxFcc_MEXU);

    if (AfxAcquireObjects(cls, msys->bridgeCnt, (afxObject*)msys->bridges, (void const*[]) { msys, bridgeCfgs }))
    {
        AfxThrowError();
    }
    else
    {
        AFX_ASSERT_OBJECTS(afxFcc_MEXU, msys->bridgeCnt, msys->bridges);

        if (cfg->dsys)
        {
            msys->dsys = cfg->dsys;
            AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &msys->dsys);
            AfxReacquireObjects(1, &msys->dsys);
        }

        afxMixDevice mdev = AmxGetBridgedMixDevice(msys->bridges[0], NIL);

        AfxCallDevice((afxDevice)mdev, 3, msys);
        AfxCallDevice((afxDevice)mdev, 5, msys);

        if (AfxDoDeviceService((afxDevice)mdev)) AfxThrowError(); // let the device build its SPUs.
        else
        {
            msys->running = TRUE;
        }
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

_AMX afxUnit AfxEnumerateMixSystems(afxUnit icd, afxUnit first, afxUnit cnt, afxMixSystem systems[])
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
        afxClass const* cls = _AmxGetMixSystemClass(mdle);
        AFX_ASSERT_CLASS(cls, afxFcc_MSYS);
        rslt = AfxEnumerateObjects(cls, first, cnt, (afxObject*)systems);
    }
}

_AMX afxUnit AfxEvokeMixSystems(afxUnit icd, afxUnit first, void* udd, afxBool(*f)(void*, afxMixSystem), afxUnit cnt, afxMixSystem systems[])
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
        afxClass const* cls = _AmxGetMixSystemClass(mdle);
        AFX_ASSERT_CLASS(cls, afxFcc_MSYS);
        rslt = AfxEvokeObjects(cls, (void*)f, udd, first, cnt, (afxObject*)systems);
    }
    return rslt;
}

_AMX afxUnit AfxInvokeMixSystems(afxUnit icd, afxUnit first, void *udd, afxBool(*f)(void*, afxMixSystem), afxUnit cnt)
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
        afxClass const* cls = _AmxGetMixSystemClass(mdle);
        AFX_ASSERT_CLASS(cls, afxFcc_MSYS);
        rslt = AfxInvokeObjects(cls, first, cnt, (void*)f, udd);
    }
    return rslt;
}

_AMX afxError AfxConfigureMixSystem(afxUnit icd, afxMixSystemConfig* cfg)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(icd != AFX_INVALID_INDEX);
    AFX_ASSERT(cfg);

    if (!cfg)
    {
        AfxThrowError();
        return err;
    }

    *cfg = (afxMixSystemConfig const) { 0 };

    afxModule driver;
    if (!_AmxGetIcd(icd, &driver))
    {
        AfxThrowError();
        return err;
    }
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &driver);
    AFX_ASSERT(AfxTestModule(driver, afxModuleFlag_ICD | afxModuleFlag_AMX));

    afxUnit mdevId = 0;
    afxMixDevice mdev;
    if (!AfxEnumerateMixDevices(icd, mdevId, 1, &mdev))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT_OBJECTS(afxFcc_MDEV, 1, &mdev);

    cfg->ver = 0;
    cfg->exuCnt = 0;
    cfg->reqExtCnt = 0;
    cfg->reqExts = NIL;
    cfg->reqFeatures = (afxMixFeatures) { 0 };
    cfg->udd = NIL;

    for (afxUnit i = 0; i < AMX_MAX_BRIDGES_PER_SYSTEM; i++)
    {
        afxMixCapabilities caps;
        if (!AfxQueryMixCapabilities(mdev, i, 1, &caps))
            break;

        cfg->exus[i].capabilities = caps.capabilities;
        cfg->exus[i].acceleration = caps.acceleration;
        cfg->exus[i].mdevId = mdevId;
        cfg->exus[i].minQueCnt = caps.minQueCnt;
        cfg->exus[i].queuePriority = NIL;
        ++cfg->exuCnt;
    }
    return err;
}

_AMX afxError AfxEstablishMixSystem(afxUnit icd, afxMixSystemConfig const* cfg, afxMixSystem* system)
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

    afxModule driver;
    if (!_AmxGetIcd(icd, &driver))
    {
        AfxThrowError();
        return err;
    }
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &driver);
    AFX_ASSERT(AfxTestModule(driver, afxModuleFlag_ICD | afxModuleFlag_AMX));

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
        if (!AfxEnumerateMixDevices(icd, exuCfg->mdevId, 1, &mdev))
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
        afxMixCapabilities caps2 = { 0 };
        caps2.capabilities = exuCfg->capabilities;
        caps2.acceleration = exuCfg->acceleration;

        afxUnit portId = AFX_INVALID_INDEX;
        if (!AfxChooseMixPorts(mdev, &caps2, 1, &portId))
        {
            AfxThrowError();
            break;
        }

        AFX_ASSERT(portId != AFX_INVALID_INDEX);
        bridgeCfg[bridgeCnt].portId = portId;
        bridgeCfg[bridgeCnt].exuIdx = bridgeCnt;
        bridgeCfg[bridgeCnt].minQueCnt = minQueCnt;
        ++bridgeCnt;
    }

    _amxMsysAcquisition cfg2 = { 0 };
    cfg2.bridgeCnt = bridgeCnt;
    cfg2.reqExtCnt = cfg->reqExtCnt;
    cfg2.reqExts = cfg->reqExts;
    cfg2.reqFeatures = cfg->reqFeatures;
    cfg2.udd = cfg->udd;
    cfg2.dsys = cfg->dsys;

    afxClass* cls = (afxClass*)_AmxGetMixSystemClass(driver);
    AFX_ASSERT_CLASS(cls, afxFcc_MSYS);

    afxMixSystem msys;
    if (AfxAcquireObjects(cls, 1, (afxObject*)&msys, (void const*[]) { driver, &cfg2, &bridgeCfg[0], }))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    *system = msys;
    return err;
}
