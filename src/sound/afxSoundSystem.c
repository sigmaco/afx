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

#define _ASX_SOUND_C
//#define _ASX_AUDIENT_C
//#define _ASX_SOUND_DEVICE_C
#define _ASX_SOUND_SYSTEM_C
//#define _ASX_SOUND_BRIDGE_C
//#define _ASX_SOUND_QUEUE_C
//#define _ASX_SOUND_INPUT_C
//#define _ASX_SINK_C
#include "impl/asxImplementation.h"

_ASX afxSoundDevice AfxGetSoundSystemDevice(afxSoundSystem ssys)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SSYS, 1, &ssys);
    afxSoundDevice sdev = AfxGetProvider(ssys);
    AFX_ASSERT_OBJECTS(afxFcc_SDEV, 1, &sdev);
    return sdev;
}

_ASX afxClass const* AsxGetAudioSinkClass(afxSoundSystem ssys)
{
    afxError err = AFX_ERR_NONE;
    /// ssys must be a valid afxSoundSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_SSYS, 1, &ssys);
    afxClass const* cls = &ssys->asioCls;
    AFX_ASSERT_CLASS(cls, afxFcc_ASIO);
    return cls;
}

_ASX afxClass const* AsxGetMixerClass(afxSoundSystem ssys)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SSYS, 1, &ssys);
    afxClass const* cls = &ssys->mixCls;
    AFX_ASSERT_CLASS(cls, afxFcc_MIX);
    return cls;
}

_ASX afxClass const* AsxGetAudioClass(afxSoundSystem ssys)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SSYS, 1, &ssys);
    afxClass const* cls = &ssys->wavCls;
    AFX_ASSERT_CLASS(cls, afxFcc_WAV);
    return cls;
}

_ASX afxClass const* AsxGetSoundClass(afxSoundSystem ssys)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SSYS, 1, &ssys);
    afxClass const* cls = &ssys->sndCls;
    AFX_ASSERT_CLASS(cls, afxFcc_SND);
    return cls;
}

_ASX afxClass const* AsxGetAudientClass(afxSoundSystem ssys)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SSYS, 1, &ssys);
    afxClass const* cls = &ssys->audiCls;
    AFX_ASSERT_CLASS(cls, afxFcc_AUD);
    return cls;
}

_ASX afxUnit AfxGetSoundBridges(afxSoundSystem ssys, afxUnit baseIdx, afxUnit cnt, afxSoundBridge bridges[])
{
    afxError err = AFX_ERR_NONE;
    /// dsys must be a valid afxSoundSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_SSYS, 1, &ssys);
    /// bridges must be a valid pointer to afxSoundBridge handles.
    AFX_ASSERT(bridges);
    afxUnit rslt = 0;

    afxUnit bridgeCnt = ssys->bridgeCnt;
    AFX_ASSERT_RANGE(bridgeCnt, baseIdx, cnt);

    if (baseIdx < bridgeCnt)
    {
        for (afxUnit i = 0; i < cnt; i++)
        {
            afxSoundBridge sexu = ssys->bridges[baseIdx + i];
            AFX_ASSERT_OBJECTS(afxFcc_SEXU, 1, &sexu);
            bridges[rslt++] = sexu;
        }
    }
    return rslt;
}

_ASX afxUnit AfxQuerySoundBridges(afxSoundSystem ssys, afxUnit sdevId, afxUnit portId, afxUnit first, afxUnit cnt, afxSoundBridge bridges[])
{
    afxError err = AFX_ERR_NONE;
    /// ssys must be a valid afxSoundSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_SSYS, 1, &ssys);
    afxUnit found = 0;
    afxUnit rslt = 0;

    afxUnit bridgeCnt = ssys->bridgeCnt;
    for (afxUnit i = 0; i < bridgeCnt; i++)
    {
        afxSoundBridge sexu = ssys->bridges[i];
        AFX_ASSERT_OBJECTS(afxFcc_SEXU, 1, &sexu);
        afxSoundDevice sdev;
        afxUnit portId2 = AfxQuerySoundBridgePort(sexu, &sdev);
        AFX_ASSERT_OBJECTS(afxFcc_SDEV, 1, &sdev);

        if ((sdevId != AFX_INVALID_INDEX) && (sdevId != AfxGetObjectId(sdev)))
            continue;

        if ((portId != AFX_INVALID_INDEX) && (portId != portId2))
            continue;

        if (cnt && (found >= first))
        {
            if (bridges)
            {
                bridges[rslt] = sexu;
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

_ASX afxError AfxWaitForSoundQueue(afxSoundSystem ssys, afxUnit exuIdx, afxUnit queIdx, afxTime timeout)
{
    afxError err = AFX_ERR_NONE;
    /// ssys must be a valid afxSoundSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_SSYS, 1, &ssys);
    /// exuIdx must be a valid index to a bridge.
    AFX_ASSERT_RANGE(ssys->bridgeCnt, exuIdx, 1);

    afxSoundBridge sexu;
    while (!AfxGetSoundBridges(ssys, exuIdx, 1, &sexu))
    {
        AfxThrowError();
        return err;
    }
    AFX_ASSERT_OBJECTS(afxFcc_SEXU, 1, &sexu);

    if (AfxWaitForEmptySoundQueue(sexu, queIdx, timeout))
        AfxThrowError();

    return err;
}

_ASX afxError AfxWaitForSoundBridge(afxSoundSystem ssys, afxUnit exuIdx, afxTime timeout)
{
    afxError err = AFX_ERR_NONE;
    /// ssys must be a valid afxSoundSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_SSYS, 1, &ssys);
    /// exuIdx must be a valid index to a bridge.
    AFX_ASSERT_RANGE((afxUnit)ssys->bridgeCnt, exuIdx, 1);

    afxSoundBridge sexu;
    while (!AfxGetSoundBridges(ssys, exuIdx, 1, &sexu))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT_OBJECTS(afxFcc_SEXU, 1, &sexu);
    AfxWaitForIdleSoundBridge(sexu, timeout);

    return err;
}

_ASX afxError AfxWaitForSoundSystem(afxSoundSystem ssys, afxTime timeout)
{
    afxError err = AFX_ERR_NONE;
    /// ssys must be a valid afxSoundSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_SSYS, 1, &ssys);

    if (!ssys->waitCb)
    {
        afxUnit bridgeCnt = ssys->bridgeCnt;

        for (afxUnit i = 0; i < bridgeCnt; i++)
            AfxWaitForSoundBridge(ssys, i, timeout);
    }
    else if (ssys->waitCb(ssys))
        AfxThrowError();

    return err;
}

_ASX afxError AfxRollMixers(afxSoundSystem ssys, afxReal clock, afxReal dur, afxUnit iterCnt, afxUnit cnt, afxMixer mixers[])
{
    afxError err = AFX_ERR_NONE;
    /// ssys must be a valid afxSoundSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_SSYS, 1, &ssys);

    afxSoundBridge sexu;
    AFX_ASSERT_RANGE(ssys->bridgeCnt, /*ctrl->exuIdx*/0, 1);
    if (!AfxGetSoundBridges(ssys, /*ctrl->exuIdx*/0, 1, &sexu))
    {
        AfxThrowError();
        return err;
    }
    AFX_ASSERT_OBJECTS(afxFcc_SEXU, 1, &sexu);

    if (_AsxRollMixers(sexu, clock, dur, iterCnt, cnt, mixers))
        AfxThrowError();

    return err;
}

_ASX afxError AfxSinkAudioSignal(afxSoundSystem ssys, void* ctrl, afxSink sink, afxAudio aud, afxWaveInterval const* seg)
{
    afxError err = AFX_ERR_NONE;
    /// ssys must be a valid afxSoundSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_SSYS, 1, &ssys);
    //AFX_ASSERT(ctrl);

    afxSoundBridge sexu;
    AFX_ASSERT_RANGE(ssys->bridgeCnt, /*ctrl->exuIdx*/0, 1);
    if (!AfxGetSoundBridges(ssys, /*ctrl->exuIdx*/0, 1, &sexu))
    {
        AfxThrowError();
        return err;
    }
    AFX_ASSERT_OBJECTS(afxFcc_SEXU, 1, &sexu);

    afxSink sout = sink;
    AFX_ASSERT_OBJECTS(afxFcc_ASIO, 1, &sout);
    afxSoundSystem con = AfxGetAudioSinkContext(sout);
    
    //avxPresentation ctrl2 = *ctrl;
    
    if (_AsxSubmitSink(sexu, ctrl, sink, aud, seg))
        AfxThrowError();

    return err;
}

_ASX afxError _AsxSsysStdDtorCb(afxSoundSystem ssys)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SSYS, 1, &ssys);
    afxSoundDevice sdev = AfxGetSoundSystemDevice(ssys);
    AFX_ASSERT_OBJECTS(afxFcc_SDEV, 1, &sdev);

    AfxWaitForSoundSystem(ssys, AFX_TIME_INFINITE);
    AfxWaitForSoundSystem(ssys, AFX_TIME_INFINITE);

    afxUnit bridgeCnt = ssys->bridgeCnt;

    AfxDeregisterChainedClasses(&ssys->ctx.classes);
    AFX_ASSERT(AfxIsChainEmpty(&ssys->ctx.classes));

    for (afxUnit i = bridgeCnt; i-- > 0;)
    {
        AFX_ASSERT_OBJECTS(afxFcc_SEXU, 1, &ssys->bridges[i]);
        AfxDisposeObjects(1, &ssys->bridges[i]);
    }

    afxObjectStash const stashes[] =
    {
        {
            .cnt = bridgeCnt,
            .siz = sizeof(ssys->bridges[0]),
            .var = (void**)&ssys->bridges
        }
    };

    if (AfxDeallocateInstanceData(ssys, ARRAY_SIZE(stashes), stashes))
        AfxThrowError();

    return err;
}

_ASX afxError _AsxSsysStdCtorCb(afxSoundSystem ssys, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SSYS, 1, &ssys);
    //AFX_ASSERT(spec);

    afxModule icd = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &icd);
    _asxSoundSystemAcquisition const* cfg = AFX_CAST(_asxSoundSystemAcquisition const*, args[1]) + invokeNo;
    _asxSoundBridgeAcquisition* bridgeCfgs = AFX_CAST(_asxSoundBridgeAcquisition*, args[2]) + invokeNo;

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

    ssys->running = FALSE;

    ssys->requirements = cfg->requirements;

    {
        afxChain* classes = &ssys->ctx.classes;
        AfxDeployChain(classes, (void*)ssys);

        afxClassConfig clsCfg;

        clsCfg = cfg->audiClsCfg ? *cfg->audiClsCfg : _ASX_AUDI_CLASS_CONFIG;
        AFX_ASSERT(clsCfg.fcc == afxFcc_AUD);
        AfxMountClass(&ssys->audiCls, NIL, classes, &clsCfg);

        clsCfg = cfg->wavClsCfg ? *cfg->wavClsCfg : _ASX_WAV_CLASS_CONFIG;
        AFX_ASSERT(clsCfg.fcc == afxFcc_WAV);
        AfxMountClass(&ssys->wavCls, NIL, classes, &clsCfg);

        clsCfg = cfg->sndClsCfg ? *cfg->sndClsCfg : _ASX_SND_CLASS_CONFIG;
        AFX_ASSERT(clsCfg.fcc == afxFcc_SND);
        AfxMountClass(&ssys->sndCls, NIL, classes, &clsCfg);

        clsCfg = cfg->mixClsCfg ? *cfg->mixClsCfg : _ASX_MIX_CLASS_CONFIG;
        AFX_ASSERT(clsCfg.fcc == afxFcc_MIX);
        AfxMountClass(&ssys->mixCls, NIL, classes, &clsCfg);

        clsCfg = cfg->sinkClsCfg ? *cfg->sinkClsCfg : _ASX_ASIO_CLASS_CONFIG;
        AFX_ASSERT(clsCfg.fcc == afxFcc_ASIO);
        AfxMountClass(&ssys->asioCls, NIL, classes, &clsCfg); // require sdev, sout

        clsCfg = cfg->sexuClsCfg ? *cfg->sexuClsCfg : _ASX_SEXU_CLASS_CONFIG;
        AFX_ASSERT(clsCfg.fcc == afxFcc_SEXU);
        AfxMountClass(&ssys->sexuCls, NIL, classes, &clsCfg);
    }

    AfxZero(ssys->heads, sizeof(ssys->heads));

    afxUnit totalDqueCnt = 0;
    afxUnit bridgeCnt = cfg->bridgeCnt;

    for (afxUnit i = 0; i < bridgeCnt; i++)
    {
        AFX_ASSERT(bridgeCfgs);
        totalDqueCnt += bridgeCfgs[i].minQueCnt;
    }

    ssys->bridgeCnt = bridgeCnt;
    afxUnit baseQueIdx = 0;

    afxObjectStash const stashes[] =
    {
        {
            .cnt = bridgeCnt,
            .siz = sizeof(ssys->bridges[0]),
            .var = (void**)&ssys->bridges
        }
    };

    if (AfxAllocateInstanceData(ssys, ARRAY_SIZE(stashes), stashes))
    {
        AfxThrowError();

        // Dismout the classes and return the error.
        AfxDeregisterChainedClasses(&ssys->ctx.classes);
        AFX_ASSERT(AfxIsChainEmpty(&ssys->ctx.classes));
        return err;
    }

    afxClass* cls = (afxClass*)_AsxGetSoundBridgeClass(ssys);
    AFX_ASSERT_CLASS(cls, afxFcc_SEXU);

    if (AfxAcquireObjects(cls, ssys->bridgeCnt, (afxObject*)ssys->bridges, (void const*[]) { ssys, bridgeCfgs }))
    {
        AfxThrowError();
    }
    else
    {
        AFX_ASSERT_OBJECTS(afxFcc_SEXU, ssys->bridgeCnt, ssys->bridges);

        afxSoundDevice sdev = AfxGetSoundBridgeDevice(ssys->bridges[0]);

        AfxCallDevice((afxDevice)sdev, 3, ssys);
        AfxCallDevice((afxDevice)sdev, 5, ssys);

        if (AfxDoDeviceService((afxDevice)sdev)) AfxThrowError(); // let the device build its SPUs.
        else
        {
            ssys->running = TRUE;
        }
    }

    if (err)
    {
        // Dismout the classes and return the error.
        AfxDeregisterChainedClasses(&ssys->ctx.classes);
        AFX_ASSERT(AfxIsChainEmpty(&ssys->ctx.classes));

        AfxDeallocateInstanceData(ssys, ARRAY_SIZE(stashes), stashes);
    }
    return err;
}

_ASX afxClassConfig const _ASX_SSYS_CLASS_CONFIG =
{
    .fcc = afxFcc_SSYS,
    .name = "SoundSystem",
    .desc = "Sound I/O System",
    .fixedSiz = sizeof(AFX_OBJECT(afxSoundSystem)),
    .ctor = (void*)_AsxSsysStdCtorCb,
    .dtor = (void*)_AsxSsysStdDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_ASX afxError AfxConfigureSoundSystem(afxUnit icd, afxSoundSystemConfig* cfg)
{
    afxError err = AFX_ERR_NONE;

    if (!cfg)
    {
        AFX_ASSERT(cfg);
        AfxThrowError();
        return err;
    }

    *cfg = (afxSoundSystemConfig const) { 0 };

    afxModule driver;
    if (!AsxGetIcd(icd, &driver))
    {
        AfxThrowError();
        return err;
    }
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &driver);
    AFX_ASSERT(AfxTestModule(driver, afxModuleFlag_ICD | afxModuleFlag_ASX));

    afxSoundDevice sdev;
    AFX_ASSERT(icd != AFX_INVALID_INDEX);
    if (!AfxEnumerateSoundDevices(icd, 0, 1, &sdev))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT_OBJECTS(afxFcc_SDEV, 1, &sdev);

    cfg->prime.minQueCnt = 3;

    return err;
}

_ASX afxError AfxEstablishSoundSystem(afxUnit icd, afxSoundSystemConfig const* cfg, afxSoundSystem* system)
{
    afxError err = AFX_ERR_NONE;

    if (!cfg)
    {
        AFX_ASSERT(cfg);
        AfxThrowError();
        return err;
    }

    afxModule driver;
    if (!AsxGetIcd(icd, &driver))
    {
        AfxThrowError();
        return err;
    }
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &driver);
    AFX_ASSERT(AfxTestModule(driver, afxModuleFlag_ICD | afxModuleFlag_ASX));

    afxSoundDevice sdev;
    AFX_ASSERT(cfg->prime.sdevId != AFX_INVALID_INDEX);
    if (!AfxEnumerateSoundDevices(icd, cfg->prime.sdevId, 1, &sdev))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT_OBJECTS(afxFcc_SDEV, 1, &sdev);

    if (AfxCallDevice((afxDevice)sdev, 1, NIL))
    {
        AfxThrowError(); // let the device build its DPUs.
        return err;
    }

    // Acquire bridges and queues
    afxUnit portId = AFX_INVALID_INDEX;
    afxUnit totalSqueCnt = 0;
    afxUnit baseQueIdx[AFX_MAX_SOUND_BRIDGE_PER_CONTEXT] = { 0 };
    _asxSoundBridgeAcquisition bridgeCfg[AFX_MAX_SOUND_BRIDGE_PER_CONTEXT] = { 0 };
    afxUnit bridgeCnt = 0;

    if (!AfxEnumerateSoundDevices(icd, cfg->prime.sdevId, 1, &sdev))
    {
        AfxThrowError();
        return err;
    }
    AFX_ASSERT_OBJECTS(afxFcc_SDEV, 1, &sdev);
    bridgeCfg[bridgeCnt].sdev = sdev;
    if (!AfxChooseSoundPorts(sdev, cfg->prime.capabilities, cfg->prime.acceleration, 1, &portId))
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
                if (!AfxEnumerateSoundDevices(icd, cfg->auxs[i].sdevId, 1, &sdev))
                {
                    AfxThrowError();
                    return err;
                }
                AFX_ASSERT_OBJECTS(afxFcc_SDEV, 1, &sdev);
                bridgeCfg[bridgeCnt].sdev = sdev;
                if (!AfxChooseSoundPorts(sdev, cfg->auxs[i].capabilities, cfg->auxs[i].acceleration, 1, &portId))
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

    _asxSoundSystemAcquisition cfg2 = { 0 };
    cfg2.bridgeCnt = bridgeCnt;
    cfg2.extensionCnt = cfg->extensionCnt;
    cfg2.extensions = cfg->extensions;
    cfg2.requirements = cfg->requirements;
    cfg2.udd = cfg->udd;

    afxClass* cls = (afxClass*)_AsxGetSoundSystemClass(driver);
    AFX_ASSERT_CLASS(cls, afxFcc_SSYS);

    afxSoundSystem ssys;
    if (AfxAcquireObjects(cls, 1, (afxObject*)&ssys, (void const*[]) { driver, &cfg2, &bridgeCfg[0], }))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT_OBJECTS(afxFcc_SSYS, 1, &ssys);
    AFX_ASSERT(system);
    *system = ssys;
    return err;
}
