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
#include "impl/avxImplementation.h"

_AVX afxModule AfxGetDrawSystemIcd(afxDrawSystem dsys)
{
    afxError err = AFX_ERR_NONE;
    /// dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    afxModule icd = AfxGetProvider(dsys);
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &icd);
    AFX_ASSERT(AfxTestModule(icd, afxModuleFlag_ICD | afxModuleFlag_AVX));
    return icd;
}

_AVX afxClass const* _AvxGetDrawBridgeClass(afxDrawSystem dsys)
{
    afxError err = AFX_ERR_NONE;
    /// dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    afxClass const* cls = &dsys->dexuCls;
    AFX_ASSERT_CLASS(cls, afxFcc_DEXU);
    return cls;
}

_AVX afxClass const* _AvxGetDrawOutputClass(afxDrawSystem dsys)
{
    afxError err = AFX_ERR_NONE;
    /// dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    afxClass const* cls = &dsys->doutCls;
    AFX_ASSERT_CLASS(cls, afxFcc_DOUT);
    return cls;
}

_AVX afxClass const* _AvxGetDrawInputClass(afxDrawSystem dsys)
{
    afxError err = AFX_ERR_NONE;
    /// dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    afxClass const* cls = &dsys->dinCls;
    AFX_ASSERT_CLASS(cls, afxFcc_DIN);
    return cls;
}

_AVX afxClass* AvxGetQueryPoolClass(afxDrawSystem dsys)
{
    afxError err = AFX_ERR_NONE;
    /// dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    afxClass *cls = &dsys->qrypCls;
    AFX_ASSERT_CLASS(cls, afxFcc_QRYP);
    return cls;
}

_AVX afxClass* AvxGetVertexInputClass(afxDrawSystem dsys)
{
    afxError err = AFX_ERR_NONE;
    /// dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    afxClass *cls = &dsys->vinCls;
    AFX_ASSERT_CLASS(cls, afxFcc_VIN);
    return cls;
}

_AVX afxClass* AvxGetRasterClass(afxDrawSystem dsys)
{
    afxError err = AFX_ERR_NONE;
    /// dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    afxClass *cls = &dsys->rasCls;
    AFX_ASSERT_CLASS(cls, afxFcc_RAS);
    return cls;
}

_AVX afxClass const* AvxGetBufferClass(afxDrawSystem dsys)
{
    afxError err = AFX_ERR_NONE;
    /// dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    afxClass *cls = &dsys->bufCls;
    AFX_ASSERT_CLASS(cls, afxFcc_BUF);
    return cls;
}

_AVX afxClass* AvxGetSamplerClass(afxDrawSystem dsys)
{
    afxError err = AFX_ERR_NONE;
    /// dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    afxClass *cls = &dsys->sampCls;
    AFX_ASSERT_CLASS(cls, afxFcc_SAMP);
    return cls;
}

_AVX afxClass* AvxGetPipelineClass(afxDrawSystem dsys)
{
    afxError err = AFX_ERR_NONE;
    /// dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    afxClass *cls = &dsys->pipCls;
    AFX_ASSERT_CLASS(cls, afxFcc_PIP);
    return cls;
}

_AVX afxClass* AvxGetCanvasClass(afxDrawSystem dsys)
{
    afxError err = AFX_ERR_NONE;
    /// dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    afxClass *cls = &dsys->canvCls;
    AFX_ASSERT_CLASS(cls, afxFcc_CANV);
    return cls;
}

_AVX afxClass* AvxGetShaderClass(afxDrawSystem dsys)
{
    afxError err = AFX_ERR_NONE;
    /// dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    afxClass *cls = &dsys->shadCls;
    AFX_ASSERT_CLASS(cls, afxFcc_SHD);
    return cls;
}

_AVX afxClass* AvxGetLigatureClass(afxDrawSystem dsys)
{
    afxError err = AFX_ERR_NONE;
    /// dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    afxClass *cls = &dsys->ligaCls;
    AFX_ASSERT_CLASS(cls, afxFcc_LIGA);
    return cls;
}

_AVX afxBool AvxGetShaderStringBase(afxDrawSystem dsys, afxStringBase* base)
{
    afxError err = AFX_ERR_NONE;
    /// dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    AFX_ASSERT(base);
    afxStringBase strb = dsys->shdStrb;
    AFX_TRY_ASSERT_OBJECTS(afxFcc_STRB, 1, &strb);
    *base = dsys->shdStrb;
    return !!strb;
}

_AVX afxUnit AfxGetDrawBridges(afxDrawSystem dsys, afxUnit baseExuIdx, afxUnit cnt, afxDrawBridge bridges[])
{
    afxError err = AFX_ERR_NONE;
    /// dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    /// bridges must be a valid pointer to afxDrawBridge handles.
    AFX_ASSERT(bridges);
    afxUnit rslt = 0;

    afxUnit bridgeCnt = dsys->bridgeCnt;
    AFX_ASSERT_RANGE(bridgeCnt, baseExuIdx, cnt);

    if (baseExuIdx < bridgeCnt)
    {
        for (afxUnit i = 0; i < cnt; i++)
        {
            afxDrawBridge dexu = dsys->bridges[baseExuIdx + i];
            AFX_ASSERT_OBJECTS(afxFcc_DEXU, 1, &dexu);
            bridges[rslt++] = dexu;
        }
    }
    return rslt;
}

_AVX afxUnit AfxQueryDrawBridges(afxDrawSystem dsys, afxUnit ddevId, afxUnit portId, afxUnit first, afxUnit cnt, afxDrawBridge bridges[])
{
    afxError err = AFX_ERR_NONE;
    /// dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    afxUnit found = 0;
    afxUnit rslt = 0;

    afxUnit bridgeCnt = dsys->bridgeCnt;
    for (afxUnit i = 0; i < bridgeCnt; i++)
    {
        afxDrawBridge dexu = dsys->bridges[i];
        AFX_ASSERT_OBJECTS(afxFcc_DEXU, 1, &dexu);
        afxDrawDevice ddev;
        afxUnit portId2 = AfxQueryDrawBridgePort(dexu, &ddev);
        AFX_ASSERT_OBJECTS(afxFcc_DDEV, 1, &ddev);

        if ((ddevId != AFX_INVALID_INDEX) && (ddevId != AfxGetObjectId(ddev)))
            continue;

        if ((portId != AFX_INVALID_INDEX) && (portId != portId2))
            continue;

        if (cnt && (found >= first))
        {
            if (bridges)
            {
                bridges[rslt] = dexu;
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

_AVX afxError AfxWaitForDrawQueue(afxDrawSystem dsys, afxUnit exuIdx, afxUnit queId, afxTime timeout)
{
    afxError err = AFX_ERR_NONE;
    /// dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    /// portIdx must be a valid index to a bridge.
    AFX_ASSERT_RANGE((afxUnit)dsys->bridgeCnt, exuIdx, 1);
    
    afxDrawBridge dexu;
    while (!AfxGetDrawBridges(dsys, exuIdx, 1, &dexu))
    {
        AfxThrowError();
        return err;
    }
    AFX_ASSERT_OBJECTS(afxFcc_DEXU, 1, &dexu);

    afxDrawQueue dque;
    while (!AfxGetDrawQueues(dexu, queId, 1, &dque))
    {
        AfxThrowError();
        return err;
    }
    AFX_ASSERT_OBJECTS(afxFcc_DQUE, 1, &dque);

    if (AfxWaitForEmptyDrawQueue(dque, timeout))
        AfxThrowError();

    return err;
}

_AVX afxError AfxWaitForDrawBridge(afxDrawSystem dsys, afxUnit exuIdx, afxTime timeout)
{
    afxError err = AFX_ERR_NONE;
    /// dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    /// portIdx must be a valid index to a bridge.
    AFX_ASSERT_RANGE((afxUnit)dsys->bridgeCnt, exuIdx, 1);
    
    afxDrawBridge dexu;    
    while (!AfxGetDrawBridges(dsys, exuIdx, 1, &dexu))
    {
        AfxThrowError();
        return err;
    }
    AFX_ASSERT_OBJECTS(afxFcc_DEXU, 1, &dexu);

    AfxWaitForIdleDrawBridge(dexu, timeout);

    return err;
}

_AVX afxError AfxWaitForDrawPort(afxDrawSystem dsys, afxUnit portId, afxTime timeout)
{
    afxError err = AFX_ERR_NONE;
    /// dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);

    afxUnit i = 0;
    afxDrawBridge dexu;
    while (AfxGetDrawBridges(dsys, i++, 1, &dexu))
    {
        AFX_ASSERT_OBJECTS(afxFcc_DEXU, 1, &dexu);        
        afxUnit portId2 = AfxQueryDrawBridgePort(dexu, NIL);

        if (portId == portId2)
            if (AfxWaitForIdleDrawBridge(dexu, timeout))
                AfxThrowError();
    }
    return err;
}

_AVX afxError AfxWaitForDrawDevice(afxDrawSystem dsys, afxUnit devId, afxTime timeout)
{
    afxError err = AFX_ERR_NONE;
    /// dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);

    afxUnit i = 0;
    afxDrawBridge dexu;
    while (AfxGetDrawBridges(dsys, i++, 1, &dexu))
    {
        AFX_ASSERT_OBJECTS(afxFcc_DEXU, 1, &dexu);
        afxDrawDevice ddev;
        AfxQueryDrawBridgePort(dexu, &ddev);
        AFX_ASSERT_OBJECTS(afxFcc_DDEV, 1, &ddev);
        afxUnit devId2 = AfxGetObjectId(ddev);

        if (devId == devId2)
            if (AfxWaitForIdleDrawBridge(dexu, timeout))
                AfxThrowError();
    }
    return err;
}

_AVX afxError AfxWaitForDrawSystem(afxDrawSystem dsys, afxTime timeout)
{
    afxError err = AFX_ERR_NONE;
    /// dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);

    if (!dsys->waitCb)
    {
        afxUnit bridgeCnt = dsys->bridgeCnt;

        for (afxUnit i = 0; i < bridgeCnt; i++)
            AfxWaitForDrawBridge(dsys, i, timeout);
    }
    else if (dsys->waitCb(dsys, timeout))
        AfxThrowError();

    return err;
}

// DRAW INPUT/OUTPUT DEVICE CONNECTIONS

_AVX afxUnit AfxEnumerateDrawOutputs(afxDrawSystem dsys, afxUnit first, afxUnit cnt, afxDrawOutput outputs[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    AFX_ASSERT(!cnt || outputs);
    afxClass const* cls = _AvxGetDrawOutputClass(dsys);
    AFX_ASSERT_CLASS(cls, afxFcc_DOUT);
    return AfxEnumerateObjects(cls, first, cnt, (afxObject*)outputs);
}

_AVX afxUnit AfxEnumerateDrawInputs(afxDrawSystem dsys, afxUnit first, afxUnit cnt, afxDrawInput inputs[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    AFX_ASSERT(!cnt || inputs);
    afxClass const* cls = _AvxGetDrawInputClass(dsys);
    AFX_ASSERT_CLASS(cls, afxFcc_DIN);
    return AfxEnumerateObjects(cls, first, cnt, (afxObject*)inputs);
}

_AVX afxUnit AfxEvokeDrawOutputs(afxDrawSystem dsys, afxBool(*f)(afxDrawOutput, void*), void* udd, afxUnit first, afxUnit cnt, afxDrawOutput outputs[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    AFX_ASSERT(outputs);
    AFX_ASSERT(f);
    afxClass const* cls = _AvxGetDrawOutputClass(dsys);
    AFX_ASSERT_CLASS(cls, afxFcc_DOUT);
    return AfxEvokeObjects(cls, (void*)f, udd, first, cnt, (afxObject*)outputs);
}

_AVX afxUnit AfxEvokeDrawInputs(afxDrawSystem dsys, afxBool(*f)(afxDrawInput, void*), void* udd, afxUnit first, afxUnit cnt, afxDrawInput inputs[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    AFX_ASSERT(inputs);
    AFX_ASSERT(f);
    afxClass const* cls = _AvxGetDrawInputClass(dsys);
    AFX_ASSERT_CLASS(cls, afxFcc_DIN);
    return AfxEvokeObjects(cls, (void*)f, udd, first, cnt, (afxObject*)inputs);
}

_AVX afxUnit AfxInvokeDrawOutputs(afxDrawSystem dsys, afxUnit first, afxUnit cnt, afxBool(*f)(afxDrawOutput, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    AFX_ASSERT(f);
    afxClass const* cls = _AvxGetDrawOutputClass(dsys);
    AFX_ASSERT_CLASS(cls, afxFcc_DOUT);
    return AfxInvokeObjects(cls, first, cnt, (void*)f, udd);
}

_AVX afxUnit AfxInvokeDrawInputs(afxDrawSystem dsys, afxUnit first, afxUnit cnt, afxBool(*f)(afxDrawInput, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    AFX_ASSERT(f);
    afxClass const* cls = _AvxGetDrawInputClass(dsys);
    AFX_ASSERT_CLASS(cls, afxFcc_DIN);
    return AfxInvokeObjects(cls, first, cnt, (void*)f, udd);
}

_AVX afxError AfxRollDrawCommands(afxDrawSystem dsys, avxSubmission* ctrl, afxUnit cnt, afxDrawContext contexts[])
{
    afxError err = AFX_ERR_NONE;
    /// dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    AFX_ASSERT(contexts);
    AFX_ASSERT(ctrl);
    AFX_ASSERT(cnt);

    /*
        If any command buffer submitted to this queue is in the executable state, it is moved to the pending state.
        Once execution of all submissions of a command buffer complete, it moves from the pending state, back to the executable state.
        If a command buffer was recorded with the VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT flag, it instead moves back to the invalid state.
    */

    afxDrawBridge dexu;
    if (!AfxGetDrawBridges(dsys, ctrl->exuIdx, 1, &dexu))
    {
        AfxThrowError();
        return err;
    }

    afxClass const* dqueCls = _AvxGetDrawQueueClass(dexu);

    // sanitize arguments
    afxUnit totalQueCnt = dqueCls->instCnt;
    afxUnit baseQueIdx = AfxMin(ctrl->baseQueIdx, totalQueCnt - 1);
    afxUnit queCnt = AfxClamp(ctrl->queCnt, 1, totalQueCnt - baseQueIdx);
    AFX_ASSERT(queCnt);
    afxBool queued = FALSE;

    do
    {
        for (afxUnit i = 0; i < queCnt; i++)
        {
            afxUnit queIdx = baseQueIdx + i;

            afxDrawQueue dque;
            if (!AfxGetDrawQueues(dexu, queIdx, 1, &dque))
            {
                AfxThrowError();
                break;
            }

            afxError err2;
            if ((err2 = _AvxSubmitDrawCommands(dque, ctrl, 1, contexts)))
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
    return err;
}

_AVX afxError _AvxTransferVideoMemory(afxDrawSystem dsys, avxTransference* ctrl, afxUnit opCnt, void const* ops)
{
    afxError err = AFX_ERR_NONE;
    /// dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    AFX_ASSERT(opCnt);
    AFX_ASSERT(ctrl);
    AFX_ASSERT(ops);

    afxDrawBridge dexu;
    if (!AfxGetDrawBridges(dsys, ctrl->exuIdx, 1, &dexu))
    {
        AfxThrowError();
        return err;
    }
    
    afxClass const* dqueCls = _AvxGetDrawQueueClass(dexu);

    // sanitize arguments
    afxUnit totalQueCnt = dqueCls->instCnt;
    afxUnit baseQueIdx = AfxMin(ctrl->baseQueIdx, totalQueCnt - 1);
    afxUnit queCnt = AfxClamp(ctrl->queCnt, 1, totalQueCnt - baseQueIdx);
    AFX_ASSERT(queCnt);
    afxBool queued = FALSE;

    do
    {
        for (afxUnit i = 0; i < queCnt; i++)
        {
            afxUnit queIdx = baseQueIdx + i;

            afxDrawQueue dque;
            if (!AfxGetDrawQueues(dexu, queIdx, 1, &dque))
            {
                AfxThrowError();
                break;
            }

            afxError err2;
            if ((err2 = _AvxSubmitTransferences(dque, ctrl, opCnt, ops)))
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
    return err;
}

_AVX afxError AfxPresentDrawOutputs(afxDrawSystem dsys, avxPresentation* ctrl, afxUnit cnt, afxDrawOutput outputs[], afxUnit const bufIdx[])
{
    afxError err = AFX_ERR_NONE;
    /// dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    AFX_ASSERT(ctrl);

    /*
        Any writes to memory backing the images referenced by the pImageIndices and pSwapchains members of pPresentInfo,
        that are available before vkQueuePresentKHR is executed, are automatically made visible to the read access performed by the presentation engine.
        This automatic visibility operation for an image happens-after the semaphore signal operation, and happens-before the presentation engine accesses the image.

        Queueing an image for presentation defines a set of queue operations, including waiting on the semaphores and submitting a presentation request to the presentation engine.
        However, the scope of this set of queue operations does not include the actual processing of the image by the presentation engine.
    */

    afxDrawBridge dexu;
    if (!AfxGetDrawBridges(dsys, ctrl->exuIdx, 1, &dexu))
    {
        AfxThrowError();
        return err;
    }

    afxClass const* dqueCls = _AvxGetDrawQueueClass(dexu);

    // sanitize arguments
    afxUnit totalQueCnt = dqueCls->instCnt;
    afxUnit baseQueIdx = AfxMin(ctrl->baseQueIdx, totalQueCnt - 1);
    afxUnit queCnt = AfxClamp(ctrl->queCnt, 1, totalQueCnt - baseQueIdx);
    AFX_ASSERT(queCnt);
    afxBool queued = FALSE;

    do
    {
        for (afxUnit i = 0; i < queCnt; i++)
        {
            afxUnit queIdx = baseQueIdx + i;

            afxDrawQueue dque;
            if (!AfxGetDrawQueues(dexu, queIdx, 1, &dque))
            {
                AfxThrowError();
                break;
            }

            for (afxUnit j = 0; j < cnt; j++)
            {
                afxDrawOutput dout = outputs[j];
                AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);
                
                afxError err2;
                if ((err2 = _AvxPresentDrawOutput(dque, ctrl, dout, bufIdx[i])))
                {
                    if (err2 == afxError_TIMEOUT)
                        continue;

                    AfxThrowError();
                    break;
                }
            }
            queued = TRUE;
            break;
        }

        if (err || queued)
            break; // reiterate if not queue for timeout?
    } while (0);
    return err;
}

_AVX afxError _AvxDsysStdDtorCb(afxDrawSystem dsys)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);

    AfxWaitForDrawSystem(dsys, AFX_TIME_INFINITE);
    AfxWaitForDrawSystem(dsys, AFX_TIME_INFINITE);

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
        AFX_TRY_ASSERT_OBJECTS(afxFcc_DEXU, bridgeCnt, dsys->bridges);

        for (afxUnit i = bridgeCnt; i-- > 0;)
        {
            AfxDisposeObjects(1, &dsys->bridges[i]);
        }
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

_AVX afxError _AvxDsysStdCtorCb(afxDrawSystem dsys, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    //AFX_ASSERT(spec);

    afxModule icd = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &icd);
    _avxDrawSystemAcquisition const* cfg = AFX_CAST(_avxDrawSystemAcquisition const*, args[1]) + invokeNo;
    _avxDrawBridgeAcquisition* bridgeCfgs = AFX_CAST(_avxDrawBridgeAcquisition*, args[2]) + invokeNo;
    
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

    dsys->running = FALSE;

    dsys->requirements = cfg->requirements;
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

        clsCfg = cfg->vtxdClsCfg ? *cfg->vtxdClsCfg : _AVX_DOUT_CLASS_CONFIG;
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

    afxClass* cls = (afxClass*)_AvxGetDrawBridgeClass(dsys);
    AFX_ASSERT_CLASS(cls, afxFcc_DEXU);

    if (AfxAcquireObjects(cls, dsys->bridgeCnt, (afxObject*)dsys->bridges, (void const*[]) { dsys, bridgeCfgs }))
    {
        AfxThrowError();
    }
    else
    {
        AFX_ASSERT_OBJECTS(afxFcc_DEXU, dsys->bridgeCnt, dsys->bridges);
        
        afxDrawDevice ddev = AfxGetDrawBridgeDevice(dsys->bridges[0]);

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
    .ctor = (void*)_AvxDsysStdCtorCb,
    .dtor = (void*)_AvxDsysStdDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AVX afxError AfxConfigureDrawSystem(afxUnit icd, afxDrawSystemConfig* cfg)
{
    afxError err = AFX_ERR_NONE;

    if (!cfg)
    {
        AFX_ASSERT(cfg);
        AfxThrowError();
        return err;
    }

    *cfg = (afxDrawSystemConfig const) { 0 };

    afxModule driver;
    if (!AvxGetIcd(icd, &driver))
    {
        AfxThrowError();
        return err;
    }
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &driver);
    AFX_ASSERT(AfxTestModule(driver, afxModuleFlag_ICD | afxModuleFlag_AVX));

    afxDrawDevice ddev;
    AFX_ASSERT(icd != AFX_INVALID_INDEX);
    if (!AfxEnumerateDrawDevices(icd, 0, 1, &ddev))
    {
        AfxThrowError();
        return err;
    }
    
    AFX_ASSERT_OBJECTS(afxFcc_DDEV, 1, &ddev);
    
    //cfg->mainBridge.flags = NIL;
    cfg->prime.capabilities = afxDrawPortFlag_DRAW | afxDrawPortFlag_COMPUTE | afxDrawPortFlag_TRANSFER;
    cfg->prime.minQueCnt = _AVX_DQUE_CLASS_CONFIG.unitsPerPage;

    return err;
}

_AVX afxError AfxEstablishDrawSystem(afxUnit icd, afxDrawSystemConfig const* cfg, afxDrawSystem* system)
{
    afxError err = AFX_ERR_NONE;

    if (!cfg)
    {
        AFX_ASSERT(cfg);
        AfxThrowError();
        return err;
    }

    afxModule driver;
    if (!AvxGetIcd(icd, &driver))
    {
        AfxThrowError();
        return err;
    }
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &driver);
    AFX_ASSERT(AfxTestModule(driver, afxModuleFlag_ICD | afxModuleFlag_AVX));

    afxDrawDevice ddev;
    AFX_ASSERT(cfg->prime.ddevId != AFX_INVALID_INDEX);
    if (!AfxEnumerateDrawDevices(icd, cfg->prime.ddevId, 1, &ddev))
    {
        AfxThrowError();
        return err;
    }
    
    AFX_ASSERT_OBJECTS(afxFcc_DDEV, 1, &ddev);

    if (AfxCallDevice((afxDevice)ddev, 1, NIL))
    {
        AfxThrowError(); // let the device build its DPUs.
        return err;
    }

    // Acquire bridges and queues
    afxUnit portId = AFX_INVALID_INDEX;
    afxUnit totalDqueCnt = 0;
    afxUnit baseQueIdx[AFX_MAX_DRAW_BRIDGE_PER_SYSTEM] = { 0 };
    _avxDrawBridgeAcquisition bridgeCfg[AFX_MAX_DRAW_BRIDGE_PER_SYSTEM] = { 0 };
    afxUnit bridgeCnt = 0;

    if (!AfxEnumerateDrawDevices(icd, cfg->prime.ddevId, 1, &ddev))
    {
        AfxThrowError();
        return err;
    }
    AFX_ASSERT_OBJECTS(afxFcc_DDEV, 1, &ddev);
    bridgeCfg[bridgeCnt].ddev = ddev;
    if (!AfxChooseDrawPorts(ddev, cfg->prime.capabilities, cfg->prime.acceleration, 1, &portId))
    {
        AfxThrowError();
        return err;
    }
    AFX_ASSERT(portId != AFX_INVALID_INDEX);
    bridgeCfg[bridgeCnt].portId = portId;
    bridgeCfg[bridgeCnt].exuIdx = 0;
    bridgeCfg[bridgeCnt].minQueCnt = AfxClamp(cfg->prime.minQueCnt, 1, AFX_MAX_DRAW_QUEUE_PER_BRIDGE);
    bridgeCfg[bridgeCnt].dqueClsCfg = &_AVX_DQUE_CLASS_CONFIG;
    bridgeCfg[bridgeCnt].dctxClsCfg = &_AVX_DCTX_CLASS_CONFIG;
    ++bridgeCnt;

    if (cfg->auxCnt && cfg->auxs)
    {
        AFX_ASSERT(15 >= cfg->auxCnt);

        for (afxUnit i = 0; i < cfg->auxCnt; i++)
        {
            afxUnit bridgeIdx = AFX_INVALID_INDEX;
#if 0 // allow multibridge to same device port
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
            if (bridgeIdx == AFX_INVALID_INDEX)
            {
                if (!AfxEnumerateDrawDevices(icd, cfg->auxs[i].ddevId, 1, &ddev))
                {
                    AfxThrowError();
                    return err;
                }
                AFX_ASSERT_OBJECTS(afxFcc_DDEV, 1, &ddev);
                bridgeCfg[bridgeCnt].ddev = ddev;
                if (!AfxChooseDrawPorts(ddev, cfg->auxs[i].capabilities, cfg->auxs[i].acceleration, 1, &portId))
                {
                    AfxThrowError();
                    return err;
                }
                AFX_ASSERT(portId != AFX_INVALID_INDEX);
                bridgeCfg[bridgeCnt].portId = portId;
                bridgeCfg[bridgeCnt].exuIdx = bridgeCnt;
                bridgeCfg[bridgeCnt].minQueCnt = AfxClamp(cfg->auxs[i].minQueCnt, 1, AFX_MAX_DRAW_QUEUE_PER_BRIDGE);
                bridgeCfg[bridgeCnt].dqueClsCfg = &_AVX_DQUE_CLASS_CONFIG;
                bridgeCfg[bridgeCnt].dctxClsCfg = &_AVX_DCTX_CLASS_CONFIG;
                ++bridgeCnt;
            }
        }
    }

    _avxDrawSystemAcquisition cfg2 = { 0 };
    cfg2.bridgeCnt = bridgeCnt;
    cfg2.extensionCnt = cfg->extensionCnt;
    cfg2.extensions = cfg->extensions;
    cfg2.requirements = cfg->requirements;
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
    AFX_ASSERT(system);
    *system = dsys;

    return err;
}
