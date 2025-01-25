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

#define _AMX_SOUND_C
//#define _AMX_AUDIENT_C
//#define _AMX_MIX_DEVICE_C
#define _AMX_MIX_SYSTEM_C
//#define _AMX_MIX_BRIDGE_C
//#define _AMX_MIX_QUEUE_C
//#define _AMX_MIX_INPUT_C
//#define _AMX_SINK_C
#include "impl/amxImplementation.h"

_AMX afxMixDevice AfxGetSoundSystemDevice(afxMixSystem msys)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    afxMixDevice mdev = AfxGetProvider(msys);
    AFX_ASSERT_OBJECTS(afxFcc_MDEV, 1, &mdev);
    return mdev;
}

_AMX afxClass const* _AmxGetAudioSinkClass(afxMixSystem msys)
{
    afxError err = AFX_ERR_NONE;
    /// msys must be a valid afxMixSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    afxClass const* cls = &msys->asioCls;
    AFX_ASSERT_CLASS(cls, afxFcc_ASIO);
    return cls;
}

_AMX afxClass const* _AmxGetSoundBridgeClass(afxMixSystem msys)
{
    afxError err = AFX_ERR_NONE;
    /// msys must be a valid afxMixDevice handle.
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

_AMX afxClass const* _AmxGetAudioClass(afxMixSystem msys)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    afxClass const* cls = &msys->wavCls;
    AFX_ASSERT_CLASS(cls, afxFcc_WAV);
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
    AFX_ASSERT_CLASS(cls, afxFcc_AUD);
    return cls;
}

_AMX afxUnit AfxGetSoundBridges(afxMixSystem msys, afxUnit baseIdx, afxUnit cnt, afxMixBridge bridges[])
{
    afxError err = AFX_ERR_NONE;
    /// dsys must be a valid afxMixSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    /// bridges must be a valid pointer to afxMixBridge handles.
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

_AMX afxUnit AfxQuerySoundBridges(afxMixSystem msys, afxUnit sdevId, afxUnit portId, afxUnit first, afxUnit cnt, afxMixBridge bridges[])
{
    afxError err = AFX_ERR_NONE;
    /// msys must be a valid afxMixSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    afxUnit found = 0;
    afxUnit rslt = 0;

    afxUnit bridgeCnt = msys->bridgeCnt;
    for (afxUnit i = 0; i < bridgeCnt; i++)
    {
        afxMixBridge mexu = msys->bridges[i];
        AFX_ASSERT_OBJECTS(afxFcc_MEXU, 1, &mexu);
        afxMixDevice mdev;
        afxUnit portId2 = AfxQueryMixBridgePort(mexu, &mdev);
        AFX_ASSERT_OBJECTS(afxFcc_MDEV, 1, &mdev);

        if ((sdevId != AFX_INVALID_INDEX) && (sdevId != AfxGetObjectId(mdev)))
            continue;

        if ((portId != AFX_INVALID_INDEX) && (portId != portId2))
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

_AMX afxError AfxWaitForSoundQueue(afxMixSystem msys, afxUnit exuIdx, afxUnit queIdx, afxTime timeout)
{
    afxError err = AFX_ERR_NONE;
    /// msys must be a valid afxMixSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    /// exuIdx must be a valid index to a bridge.
    AFX_ASSERT_RANGE(msys->bridgeCnt, exuIdx, 1);

    afxMixBridge mexu;
    while (!AfxGetSoundBridges(msys, exuIdx, 1, &mexu))
    {
        AfxThrowError();
        return err;
    }
    AFX_ASSERT_OBJECTS(afxFcc_MEXU, 1, &mexu);

    if (AfxWaitForEmptyMixQueue(mexu, queIdx, timeout))
        AfxThrowError();

    return err;
}

_AMX afxError AfxWaitForSoundBridge(afxMixSystem msys, afxUnit exuIdx, afxTime timeout)
{
    afxError err = AFX_ERR_NONE;
    /// msys must be a valid afxMixSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    /// exuIdx must be a valid index to a bridge.
    AFX_ASSERT_RANGE((afxUnit)msys->bridgeCnt, exuIdx, 1);

    afxMixBridge mexu;
    while (!AfxGetSoundBridges(msys, exuIdx, 1, &mexu))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT_OBJECTS(afxFcc_MEXU, 1, &mexu);
    AfxWaitForIdleMixBridge(mexu, timeout);

    return err;
}

_AMX afxError AfxWaitForSoundSystem(afxMixSystem msys, afxTime timeout)
{
    afxError err = AFX_ERR_NONE;
    /// msys must be a valid afxMixSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);

    if (!msys->waitCb)
    {
        afxUnit bridgeCnt = msys->bridgeCnt;

        for (afxUnit i = 0; i < bridgeCnt; i++)
            AfxWaitForSoundBridge(msys, i, timeout);
    }
    else if (msys->waitCb(msys))
        AfxThrowError();

    return err;
}

_AMX afxError AfxRollMixers(afxMixSystem msys, afxReal clock, afxReal dur, afxUnit iterCnt, afxUnit cnt, afxMixContext mixers[])
{
    afxError err = AFX_ERR_NONE;
    /// msys must be a valid afxMixSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);

    afxMixBridge mexu;
    AFX_ASSERT_RANGE(msys->bridgeCnt, /*ctrl->exuIdx*/0, 1);
    if (!AfxGetSoundBridges(msys, /*ctrl->exuIdx*/0, 1, &mexu))
    {
        AfxThrowError();
        return err;
    }
    AFX_ASSERT_OBJECTS(afxFcc_MEXU, 1, &mexu);

    if (_AmxRollMixers(mexu, clock, dur, iterCnt, cnt, mixers))
        AfxThrowError();

    return err;
}

_AMX afxError AfxSinkAudioSignal(afxMixSystem msys, void* ctrl, afxSink sink, afxAudio aud, afxWaveInterval const* seg)
{
    afxError err = AFX_ERR_NONE;
    /// msys must be a valid afxMixSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    //AFX_ASSERT(ctrl);

    afxMixBridge mexu;
    AFX_ASSERT_RANGE(msys->bridgeCnt, /*ctrl->exuIdx*/0, 1);
    if (!AfxGetSoundBridges(msys, /*ctrl->exuIdx*/0, 1, &mexu))
    {
        AfxThrowError();
        return err;
    }
    AFX_ASSERT_OBJECTS(afxFcc_MEXU, 1, &mexu);

    afxSink sout = sink;
    AFX_ASSERT_OBJECTS(afxFcc_ASIO, 1, &sout);
    afxMixSystem con = AfxGetAudioSinkContext(sout);
    
    //avxPresentation ctrl2 = *ctrl;
    
    if (_AmxSubmitSink(mexu, ctrl, sink, aud, seg))
        AfxThrowError();

    return err;
}

_AMX afxError _AmxMsysStdDtorCb(afxMixSystem msys)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    afxMixDevice mdev = AfxGetSoundSystemDevice(msys);
    AFX_ASSERT_OBJECTS(afxFcc_MDEV, 1, &mdev);

    AfxWaitForSoundSystem(msys, AFX_TIME_INFINITE);
    AfxWaitForSoundSystem(msys, AFX_TIME_INFINITE);

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

    return err;
}

_AMX afxError _AmxMsysStdCtorCb(afxMixSystem msys, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    //AFX_ASSERT(spec);

    afxModule icd = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &icd);
    _amxMixSystemAcquisition const* cfg = AFX_CAST(_amxMixSystemAcquisition const*, args[1]) + invokeNo;
    _amxMixBridgeAcquisition* bridgeCfgs = AFX_CAST(_amxMixBridgeAcquisition*, args[2]) + invokeNo;

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

    msys->requirements = cfg->requirements;

    {
        afxChain* classes = &msys->ctx.classes;
        AfxDeployChain(classes, (void*)msys);

        afxClassConfig clsCfg;

        clsCfg = cfg->audiClsCfg ? *cfg->audiClsCfg : _AMX_AUDI_CLASS_CONFIG;
        AFX_ASSERT(clsCfg.fcc == afxFcc_AUD);
        AfxMountClass(&msys->audiCls, NIL, classes, &clsCfg);

        clsCfg = cfg->wavClsCfg ? *cfg->wavClsCfg : _AMX_WAV_CLASS_CONFIG;
        AFX_ASSERT(clsCfg.fcc == afxFcc_WAV);
        AfxMountClass(&msys->wavCls, NIL, classes, &clsCfg);

        clsCfg = cfg->sndClsCfg ? *cfg->sndClsCfg : _AMX_SND_CLASS_CONFIG;
        AFX_ASSERT(clsCfg.fcc == afxFcc_SND);
        AfxMountClass(&msys->sndCls, NIL, classes, &clsCfg);

        clsCfg = cfg->mixClsCfg ? *cfg->mixClsCfg : _AMX_MIX_CLASS_CONFIG;
        AFX_ASSERT(clsCfg.fcc == afxFcc_MIX);
        AfxMountClass(&msys->mixCls, NIL, classes, &clsCfg);

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

    afxClass* cls = (afxClass*)_AmxGetSoundBridgeClass(msys);
    AFX_ASSERT_CLASS(cls, afxFcc_MEXU);

    if (AfxAcquireObjects(cls, msys->bridgeCnt, (afxObject*)msys->bridges, (void const*[]) { msys, bridgeCfgs }))
    {
        AfxThrowError();
    }
    else
    {
        AFX_ASSERT_OBJECTS(afxFcc_MEXU, msys->bridgeCnt, msys->bridges);

        afxMixDevice mdev = AfxGetMixBridgeDevice(msys->bridges[0]);

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
    .ctor = (void*)_AmxMsysStdCtorCb,
    .dtor = (void*)_AmxMsysStdDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AMX afxError AfxConfigureMixSystem(afxUnit icd, afxMixSystemConfig* cfg)
{
    afxError err = AFX_ERR_NONE;

    if (!cfg)
    {
        AFX_ASSERT(cfg);
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

    afxMixDevice mdev;
    AFX_ASSERT(icd != AFX_INVALID_INDEX);
    if (!AfxEnumerateSoundDevices(icd, 0, 1, &mdev))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT_OBJECTS(afxFcc_MDEV, 1, &mdev);

    cfg->prime.minQueCnt = 3;

    return err;
}

_AMX afxError AfxEstablishMixSystem(afxUnit icd, afxMixSystemConfig const* cfg, afxMixSystem* system)
{
    afxError err = AFX_ERR_NONE;

    if (!cfg)
    {
        AFX_ASSERT(cfg);
        AfxThrowError();
        return err;
    }

    afxModule driver;
    if (!_AmxGetIcd(icd, &driver))
    {
        AfxThrowError();
        return err;
    }
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &driver);
    AFX_ASSERT(AfxTestModule(driver, afxModuleFlag_ICD | afxModuleFlag_AMX));

    afxMixDevice mdev;
    AFX_ASSERT(cfg->prime.sdevId != AFX_INVALID_INDEX);
    if (!AfxEnumerateSoundDevices(icd, cfg->prime.sdevId, 1, &mdev))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT_OBJECTS(afxFcc_MDEV, 1, &mdev);

    if (AfxCallDevice((afxDevice)mdev, 1, NIL))
    {
        AfxThrowError(); // let the device build its DPUs.
        return err;
    }

    // Acquire bridges and queues
    afxUnit portId = AFX_INVALID_INDEX;
    afxUnit totalSqueCnt = 0;
    afxUnit baseQueIdx[AFX_MAX_SOUND_BRIDGE_PER_CONTEXT] = { 0 };
    _amxMixBridgeAcquisition bridgeCfg[AFX_MAX_SOUND_BRIDGE_PER_CONTEXT] = { 0 };
    afxUnit bridgeCnt = 0;

    if (!AfxEnumerateSoundDevices(icd, cfg->prime.sdevId, 1, &mdev))
    {
        AfxThrowError();
        return err;
    }
    AFX_ASSERT_OBJECTS(afxFcc_MDEV, 1, &mdev);
    bridgeCfg[bridgeCnt].mdev = mdev;
    if (!AfxChooseMixPorts(mdev, cfg->prime.capabilities, cfg->prime.acceleration, 1, &portId))
    {
        AfxThrowError();
        return err;
    }
    AFX_ASSERT(portId != AFX_INVALID_INDEX);
    bridgeCfg[bridgeCnt].portId = portId;
    bridgeCfg[bridgeCnt].exuIdx = 0;
    bridgeCfg[bridgeCnt].minQueCnt = AfxClamp(cfg->prime.minQueCnt, 1, AFX_MAX_SOUND_QUEUE_PER_BRIDGE);
    ++bridgeCnt;

    if (cfg->auxCnt && cfg->auxs)
    {
        AFX_ASSERT(15 >= cfg->auxCnt);

        for (afxUnit i = 0; i < cfg->auxCnt; i++)
        {
            afxUnit bridgeIdx = AFX_INVALID_INDEX;
#if 0 // allowed multibridge to same port
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
            if (bridgeIdx == AFX_INVALID_INDEX)
            {
                if (!AfxEnumerateSoundDevices(icd, cfg->auxs[i].sdevId, 1, &mdev))
                {
                    AfxThrowError();
                    return err;
                }
                AFX_ASSERT_OBJECTS(afxFcc_MDEV, 1, &mdev);
                bridgeCfg[bridgeCnt].mdev = mdev;
                if (!AfxChooseMixPorts(mdev, cfg->auxs[i].capabilities, cfg->auxs[i].acceleration, 1, &portId))
                {
                    AfxThrowError();
                    return err;
                }
                AFX_ASSERT(portId != AFX_INVALID_INDEX);
                bridgeCfg[bridgeCnt].portId = portId;
                bridgeCfg[bridgeCnt].exuIdx = bridgeCnt;
                bridgeCfg[bridgeCnt].minQueCnt = AfxClamp(cfg->auxs[i].minQueCnt, 1, AFX_MAX_SOUND_QUEUE_PER_BRIDGE);
                ++bridgeCnt;
            }
        }
    }

    _amxMixSystemAcquisition cfg2 = { 0 };
    cfg2.bridgeCnt = bridgeCnt;
    cfg2.extensionCnt = cfg->extensionCnt;
    cfg2.extensions = cfg->extensions;
    cfg2.requirements = cfg->requirements;
    cfg2.udd = cfg->udd;

    afxClass* cls = (afxClass*)_AmxGetMixSystemClass(driver);
    AFX_ASSERT_CLASS(cls, afxFcc_MSYS);

    afxMixSystem msys;
    if (AfxAcquireObjects(cls, 1, (afxObject*)&msys, (void const*[]) { driver, &cfg2, &bridgeCfg[0], }))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    AFX_ASSERT(system);
    *system = msys;
    return err;
}
