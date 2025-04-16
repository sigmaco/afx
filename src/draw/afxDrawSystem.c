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
#include "impl/avxImplementation.h"

_AVX _avxDsysImpl const* _AvxDsysGetImpl(afxDrawSystem dsys)
{
    afxError err = AFX_ERR_NONE;
    // @dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    return dsys->pimpl;
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

_AVX afxClass const* _AvxDsysGetDexuClass(afxDrawSystem dsys)
{
    afxError err = AFX_ERR_NONE;
    // @dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    afxClass const* cls = &dsys->dexuCls;
    AFX_ASSERT_CLASS(cls, afxFcc_DEXU);
    return cls;
}

_AVX afxClass const* _AvxDsysGetFencClass(afxDrawSystem dsys)
{
    afxError err = AFX_ERR_NONE;
    // @dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    afxClass *cls = &dsys->fencCls;
    AFX_ASSERT_CLASS(cls, afxFcc_FENC);
    return cls;
}

_AVX afxClass const* _AvxDsysGetDoutClass(afxDrawSystem dsys)
{
    afxError err = AFX_ERR_NONE;
    // @dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    afxClass const* cls = &dsys->doutCls;
    AFX_ASSERT_CLASS(cls, afxFcc_DOUT);
    return cls;
}

_AVX afxClass const* _AvxDsysGetDinClass(afxDrawSystem dsys)
{
    afxError err = AFX_ERR_NONE;
    // @dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    afxClass const* cls = &dsys->dinCls;
    AFX_ASSERT_CLASS(cls, afxFcc_DIN);
    return cls;
}

_AVX afxClass const* _AvxDsysGetQrypClass(afxDrawSystem dsys)
{
    afxError err = AFX_ERR_NONE;
    // @dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    afxClass *cls = &dsys->qrypCls;
    AFX_ASSERT_CLASS(cls, afxFcc_QRYP);
    return cls;
}

_AVX afxClass const* _AvxDsysGetVtxdClass(afxDrawSystem dsys)
{
    afxError err = AFX_ERR_NONE;
    // @dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    afxClass *cls = &dsys->vinCls;
    AFX_ASSERT_CLASS(cls, afxFcc_VIN);
    return cls;
}

_AVX afxClass const* _AvxDsysGetRasClass(afxDrawSystem dsys)
{
    afxError err = AFX_ERR_NONE;
    // @dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    afxClass *cls = &dsys->rasCls;
    AFX_ASSERT_CLASS(cls, afxFcc_RAS);
    return cls;
}

_AVX afxClass const* _AvxDsysGetBufClass(afxDrawSystem dsys)
{
    afxError err = AFX_ERR_NONE;
    // @dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    afxClass *cls = &dsys->bufCls;
    AFX_ASSERT_CLASS(cls, afxFcc_BUF);
    return cls;
}

_AVX afxClass const* _AvxDsysGetSampClass(afxDrawSystem dsys)
{
    afxError err = AFX_ERR_NONE;
    // @dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    afxClass *cls = &dsys->sampCls;
    AFX_ASSERT_CLASS(cls, afxFcc_SAMP);
    return cls;
}

_AVX afxClass const* _AvxDsysGetPipClass(afxDrawSystem dsys)
{
    afxError err = AFX_ERR_NONE;
    // @dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    afxClass *cls = &dsys->pipCls;
    AFX_ASSERT_CLASS(cls, afxFcc_PIP);
    return cls;
}

_AVX afxClass const* _AvxDsysGetCanvClass(afxDrawSystem dsys)
{
    afxError err = AFX_ERR_NONE;
    // @dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    afxClass *cls = &dsys->canvCls;
    AFX_ASSERT_CLASS(cls, afxFcc_CANV);
    return cls;
}

_AVX afxClass const* _AvxDsysGetShadClass(afxDrawSystem dsys)
{
    afxError err = AFX_ERR_NONE;
    // @dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    afxClass *cls = &dsys->shadCls;
    AFX_ASSERT_CLASS(cls, afxFcc_SHD);
    return cls;
}

_AVX afxClass const* _AvxDsysGetLigaClass(afxDrawSystem dsys)
{
    afxError err = AFX_ERR_NONE;
    // @dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    afxClass *cls = &dsys->ligaCls;
    AFX_ASSERT_CLASS(cls, afxFcc_LIGA);
    return cls;
}

_AVX afxClass const* _AvxDsysGetTxdClass(afxDrawSystem dsys)
{
    afxError err = AFX_ERR_NONE;
    // @dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    afxClass const* cls = &dsys->txdCls;
    AFX_ASSERT_CLASS(cls, afxFcc_TXD);
    return cls;
}

_AVX afxClass const* _AvxDsysGetCamClass(afxDrawSystem dsys)
{
    afxError err = AFX_ERR_NONE;
    // @dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    afxClass const* cls = &dsys->camCls;
    AFX_ASSERT_CLASS(cls, afxFcc_CAM);
    return cls;
}

_AVX _avxDsysImpl const _AVX_DSYS_IMPL =
{
    .fencCls = _AvxDsysGetFencClass,
    .dexuCls = _AvxDsysGetDexuClass,
    .doutCls = _AvxDsysGetDoutClass,
    .dinCls = _AvxDsysGetDinClass,
    .qrypCls = _AvxDsysGetQrypClass,
    .vtxdCls = _AvxDsysGetVtxdClass,
    .rasCls = _AvxDsysGetRasClass,
    .bufCls = _AvxDsysGetBufClass,
    .sampCls = _AvxDsysGetSampClass,
    .pipCls = _AvxDsysGetPipClass,
    .canvCls = _AvxDsysGetCanvClass,
    .shadCls = _AvxDsysGetShadClass,
    .ligaCls = _AvxDsysGetLigaClass,

    .camCls = _AvxDsysGetCamClass,
    .txdCls = _AvxDsysGetTxdClass,
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

_AVX afxUnit AvxGetDrawSystemProcedures(afxDrawSystem dsys, afxUnit cnt, afxString const names[], void* addresses[])
{
    afxError err = AFX_ERR_NONE;
    // @dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    // @names must be an array of Qwadro strings specifying the wanted symbol's names.
    AFX_ASSERT(names);
    // @addresses must be a recipient array for returned function pointers.
    AFX_ASSERT(addresses);
    afxUnit rslt = 0;

    AFX_ASSERT(dsys->pimpl->getProcs);
    rslt = dsys->pimpl->getProcs(dsys, cnt, names, addresses);

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
    // bridges must be a valid pointer to afxDrawBridge handles.
    AFX_ASSERT(bridges);
    afxUnit rslt = 0;

    afxUnit bridgeCnt = dsys->bridgeCnt;
    //AFX_ASSERT_RANGE(bridgeCnt, baseExuIdx, cnt);

    if (baseIdx < bridgeCnt)
    {
        for (afxUnit i = 0; i < cnt; i++)
        {
            afxDrawBridge dexu = dsys->bridges[baseIdx + i];
            AFX_ASSERT_OBJECTS(afxFcc_DEXU, 1, &dexu);
            bridges[rslt++] = dexu;
        }
    }
    return rslt;
}

_AVX afxUnit AvxChooseDrawBridges(afxDrawSystem dsys, afxUnit ddevId, afxUnit portId, afxDrawPortFlags portFlags, afxUnit first, afxUnit maxCnt, afxDrawBridge bridges[])
{
    afxError err = AFX_ERR_NONE;
    // @dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    afxUnit found = 0;
    afxUnit rslt = 0;

    if ((ddevId == AFX_INVALID_INDEX) && (portId == AFX_INVALID_INDEX) &&
        (!portFlags) && (!bridges))
        return maxCnt ? AFX_MIN(maxCnt, dsys->bridgeCnt - first) : AFX_MIN(dsys->bridgeCnt, dsys->bridgeCnt - first);

    afxUnit bridgeCnt = dsys->bridgeCnt;
    for (afxUnit i = 0; i < bridgeCnt; i++)
    {
        afxDrawBridge dexu = dsys->bridges[i];
        AFX_ASSERT_OBJECTS(afxFcc_DEXU, 1, &dexu);
        
        afxUnit portId2;
        afxDrawDevice ddev = AvxGetBridgedDrawDevice(dexu, &portId2);
        AFX_ASSERT_OBJECTS(afxFcc_DDEV, 1, &ddev);

        if ((ddevId != AFX_INVALID_INDEX) && 
            (ddevId != AfxGetObjectId(ddev)))
            continue;

        if ((portId != AFX_INVALID_INDEX) && 
            (portId != portId2))
            continue;

        if (portFlags)
        {
            afxDrawCapabilities caps;
            AvxQueryDrawCapabilities(ddev, portId2, 1, &caps);
            
            if ((caps.capabilities & portFlags) != portFlags)
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

_AVX afxError AvxWaitForDrawQueue(afxDrawSystem dsys, afxUnit exuIdx, afxUnit queId, afxUnit64 timeout)
{
    afxError err = AFX_ERR_NONE;
    // @dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    // portIdx must be a valid index to a bridge.
    AFX_ASSERT_RANGE((afxUnit)dsys->bridgeCnt, exuIdx, 1);
    
    afxDrawBridge dexu;
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

_AVX afxError AvxWaitForDrawBridges(afxDrawSystem dsys, afxMask exuMask, afxUnit64 timeout)
{
    afxError err = AFX_ERR_NONE;
    // @dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);

    afxUnit exuCnt = dsys->bridgeCnt;
    for (afxUnit exuIdx = 0; exuIdx < exuCnt; exuIdx++)
    {
        if (exuMask && !(exuMask & AFX_BIT(exuIdx)))
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
        afxUnit bridgeCnt = dsys->bridgeCnt;

        for (afxUnit i = 0; i < bridgeCnt; i++)
            AvxWaitForDrawBridges(dsys, i, timeout);
    }
    else if (dsys->pimpl->waitCb(dsys, timeout))
        AfxThrowError();

    return err;
}

_AVX afxError _AvxTransferVideoMemory(afxDrawSystem dsys, avxTransference* ctrl, afxUnit opCnt, void const* ops)
{
    afxError err = AFX_ERR_NONE;
    // @dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    AFX_ASSERT(opCnt);
    AFX_ASSERT(ctrl);
    AFX_ASSERT(ops);

    afxMask exuMask = ctrl->exuMask;
    afxUnit exuCnt = dsys->bridgeCnt;
    for (afxUnit exuIdx = 0; exuIdx < exuCnt; exuIdx++)
    {
        if (exuMask && !(exuMask & AFX_BIT(exuIdx)))
            continue;

        afxDrawBridge dexu;
        if (!AvxGetDrawBridges(dsys, exuIdx, 1, &dexu))
        {
            AfxThrowError();
            return err;
        }

        afxClass const* dqueCls = _AvxGetDrawQueueClass(dexu);

        // sanitize arguments
        afxUnit totalQueCnt = dqueCls->instCnt;
        afxUnit baseQueIdx = AFX_MIN(ctrl->baseQueIdx, totalQueCnt - 1);
        afxUnit queCnt = AFX_CLAMP(ctrl->queCnt, 1, totalQueCnt - baseQueIdx);
        AFX_ASSERT(queCnt);
        afxBool queued = FALSE;

        do
        {
            for (afxUnit i = 0; i < queCnt; i++)
            {
                afxUnit queIdx = baseQueIdx + i;

                afxDrawQueue dque;
                if (!AvxGetDrawQueues(dexu, queIdx, 1, &dque))
                {
                    AfxThrowError();
                    break;
                }

                afxError err2;
                if ((err2 = _AvxDqueTransferResources(dque, ctrl, opCnt, ops)))
                {
                    if (err2 == afxError_TIMEOUT)
                        continue;

                    AfxThrowError();
                    break;
                }
                queued = TRUE;
                break;
            }

            if (err || queued)
                break; // reiterate if not queue for timeout?

        } while (0);
        break;
    }
    return err;
}

_AVX afxError _AvxDsysDtorCb(afxDrawSystem dsys)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);

    //AvxWaitForDrawSystem(dsys, AFX_TIME_INFINITE);
    //AvxWaitForDrawSystem(dsys, AFX_TIME_INFINITE);

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

        afxClassConfig clsCfg;

        clsCfg = cfg->bufClsCfg ? *cfg->bufClsCfg : _AVX_BUF_CLASS_CONFIG;
        AFX_ASSERT(clsCfg.fcc == afxFcc_BUF);
        AfxMountClass(&dsys->bufCls, NIL, classes, &clsCfg);

        clsCfg = cfg->rasClsCfg ? *cfg->rasClsCfg : _AVX_RAS_CLASS_CONFIG;
        AFX_ASSERT(clsCfg.fcc == afxFcc_RAS);
        AfxMountClass(&dsys->rasCls, NIL, classes, &clsCfg);

        clsCfg = cfg->canvClsCfg ? *cfg->canvClsCfg : _AVX_CANV_CLASS_CONFIG;
        AFX_ASSERT(clsCfg.fcc == afxFcc_CANV);
        AfxMountClass(&dsys->canvCls, NIL, classes, &clsCfg); // req RAS

        clsCfg = cfg->sampClsCfg ? *cfg->sampClsCfg : _AVX_SAMP_CLASS_CONFIG;
        AFX_ASSERT(clsCfg.fcc == afxFcc_SAMP);
        AfxMountClass(&dsys->sampCls, NIL, classes, &clsCfg);

        clsCfg = cfg->ligaClsCfg ? *cfg->ligaClsCfg : _AVX_LIGA_CLASS_CONFIG;
        AFX_ASSERT(clsCfg.fcc == afxFcc_LIGA);
        AfxMountClass(&dsys->ligaCls, NIL, classes, &clsCfg); // req BUF, RAS, SAMP

        clsCfg = cfg->vtxdClsCfg ? *cfg->vtxdClsCfg : _AVX_VIN_CLASS_CONFIG;
        AFX_ASSERT(clsCfg.fcc == afxFcc_VIN);
        AfxMountClass(&dsys->vinCls, NIL, classes, &clsCfg);

        clsCfg = cfg->shdClsCfg ? *cfg->shdClsCfg : _AVX_SHD_CLASS_CONFIG;
        AFX_ASSERT(clsCfg.fcc == afxFcc_SHD);
        AfxMountClass(&dsys->shadCls, NIL, classes, &clsCfg);

        clsCfg = cfg->pipClsCfg ? *cfg->pipClsCfg : _AVX_PIP_CLASS_CONFIG;
        AFX_ASSERT(clsCfg.fcc == afxFcc_PIP);
        AfxMountClass(&dsys->pipCls, NIL, classes, &clsCfg); // req SHD, LIGA, VTXD

        clsCfg = cfg->qrypClsCfg ? *cfg->qrypClsCfg : _AVX_QRYP_CLASS_CONFIG;
        AFX_ASSERT(clsCfg.fcc == afxFcc_QRYP);
        AfxMountClass(&dsys->qrypCls, NIL, classes, &clsCfg);

        clsCfg = cfg->doutClsCfg ? *cfg->doutClsCfg : _AVX_DOUT_CLASS_CONFIG;
        AFX_ASSERT(clsCfg.fcc == afxFcc_DOUT);
        AfxMountClass(&dsys->doutCls, NIL, classes, &clsCfg); // req RAS, CANV

        clsCfg = cfg->dinClsCfg ? *cfg->dinClsCfg : _AVX_DIN_CLASS_CONFIG;
        AFX_ASSERT(clsCfg.fcc == afxFcc_DIN);
        AfxMountClass(&dsys->dinCls, NIL, classes, &clsCfg); // req BUF

        clsCfg = cfg->txdClsCfg ? *cfg->txdClsCfg : _AVX_TXD_CLASS_CONFIG;
        AFX_ASSERT(clsCfg.fcc == afxFcc_TXD);
        AfxMountClass(&dsys->txdCls, NIL, classes, &clsCfg); // req RAS

        clsCfg = cfg->camClsCfg ? *cfg->camClsCfg : _AVX_CAM_CLASS_CONFIG;
        AFX_ASSERT(clsCfg.fcc == afxFcc_CAM);
        AfxMountClass(&dsys->camCls, NIL, classes, &clsCfg);

        clsCfg = cfg->dexuClsCfg ? *cfg->dexuClsCfg : _AVX_DEXU_CLASS_CONFIG;
        AFX_ASSERT(clsCfg.fcc == afxFcc_DEXU);
        AfxMountClass(&dsys->dexuCls, NIL, classes, &clsCfg);
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

    afxClass* cls = (afxClass*)_AvxDsysGetDexuClass(dsys);
    AFX_ASSERT_CLASS(cls, afxFcc_DEXU);

    if (AfxAcquireObjects(cls, dsys->bridgeCnt, (afxObject*)dsys->bridges, (void const*[]) { dsys, bridgeCfgs }))
    {
        AfxThrowError();
    }
    else
    {
        AFX_ASSERT_OBJECTS(afxFcc_DEXU, dsys->bridgeCnt, dsys->bridges);
        
        afxDrawDevice ddev = AvxGetBridgedDrawDevice(dsys->bridges[0], NIL);

        AfxCallDevice((afxDevice)ddev, 3, dsys);
        AfxCallDevice((afxDevice)ddev, 5, dsys);

        if (AfxDoDeviceService((afxDevice)ddev))
        {
            AfxThrowError(); // let the device build its DPUs.
        }
        else
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
        afxClass const* cls = _AvxGetDrawSystemClass(mdle);
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
        afxClass const* cls = _AvxGetDrawSystemClass(mdle);
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
        afxClass const* cls = _AvxGetDrawSystemClass(mdle);
        AFX_ASSERT_CLASS(cls, afxFcc_DSYS);
        rslt = AfxEnumerateObjects(cls, first, cnt, (afxObject*)systems);
    }
}

_AVX afxError AvxConfigureDrawSystem(afxUnit icd, afxDrawSystemConfig* cfg)
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

    afxModule driver;
    if (!_AvxGetIcd(icd, &driver))
    {
        AfxThrowError();
        return err;
    }
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &driver);
    AFX_ASSERT(AfxTestModule(driver, afxModuleFlag_ICD | afxModuleFlag_AVX));

    afxUnit ddevId = 0;
    afxDrawDevice ddev;
    if (!AvxEnumerateDrawDevices(icd, ddevId, 1, &ddev))
    {
        AfxThrowError();
        return err;
    }
    
    AFX_ASSERT_OBJECTS(afxFcc_DDEV, 1, &ddev);
    
    cfg->ver = 0;
    cfg->exuCnt = 0;
    cfg->reqExtCnt = 0;
    cfg->reqExts = NIL;
    cfg->reqFeatures = (afxDrawFeatures){ 0 };
    cfg->udd = NIL;

    for (afxUnit i = 0; i < AVX_MAX_BRIDGES_PER_SYSTEM; i++)
    {
        afxDrawCapabilities caps;
        if (!AvxQueryDrawCapabilities(ddev, i, 1, &caps))
            break;

        cfg->exus[i].capabilities = caps.capabilities;
        cfg->exus[i].acceleration = caps.acceleration;
        cfg->exus[i].ddevId = ddevId;
        cfg->exus[i].minQueCnt = caps.minQueCnt;
        cfg->exus[i].queuePriority = NIL;
        ++cfg->exuCnt;
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

    afxModule driver;
    if (!_AvxGetIcd(icd, &driver))
    {
        AfxThrowError();
        return err;
    }
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &driver);
    AFX_ASSERT(AfxTestModule(driver, afxModuleFlag_ICD | afxModuleFlag_AVX));

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
        afxDrawCapabilities caps2 = { 0 };
        caps2.capabilities = exuCfg->capabilities;
        caps2.acceleration = exuCfg->acceleration;
        caps2.minQueCnt = minQueCnt;

        afxUnit portId = AFX_INVALID_INDEX;
        if (!AfxChooseDrawPorts(ddev, &caps2, 1, &portId))
        {
            AfxThrowError();
            break;
        }

        AFX_ASSERT(portId != AFX_INVALID_INDEX);
        bridgeCfg[bridgeCnt].portId = portId;
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

    afxClass* cls = (afxClass*)_AvxGetDrawSystemClass(driver);
    AFX_ASSERT_CLASS(cls, afxFcc_DSYS);

    afxDrawSystem dsys;
    if (AfxAcquireObjects(cls, 1, (afxObject*)&dsys, (void const*[]) { driver, &cfg2, &bridgeCfg[0] }))
    {
        AfxThrowError();
        return err;
    }
    
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    *system = dsys;

    return err;
}
