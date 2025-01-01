/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                  Q W A D R O   E X E C U T I O N   E C O S Y S T E M
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

#define _AFX_CORE_C
#define _AFX_DEVICE_C
#define _AMX_ENGINE_C
#define _AMX_SIM_BRIDGE_C
#define _AMX_SIM_QUEUE_C
#define _AMX_CATALYST_C
#include "impl/amxImplementation.h"

_AMX amxCmd* _AmxCystPushCmd(afxCatalyst cyst, afxUnit id, afxUnit siz, afxCmdId* cmdId)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_CYST, 1, &cyst);

    amxCmd* cmd = AfxRequestArenaUnit(&cyst->cmdArena, siz);
    AFX_ASSERT(cmd);
    cmd->hdr.id = id;
    cmd->hdr.siz = siz;
    AFX_ASSERT(cmdId);
    *cmdId = AfxPushLink(&cmd->hdr.script, &cyst->commands);
    AFX_ASSERT(id != 268895528);
    return cmd;
}

////////////////////////////////////////////////////////////////////////////////

_AMX amxCatalystState _AmxGetCatalystStatus(afxCatalyst cyst)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_CYST, 1, &cyst);
    return cyst->state;
}

_AMX afxUnit AfxGetCatalystPort(afxCatalyst cyst)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_CYST, 1, &cyst);
    return cyst->portId;
}

_AMX afxUnit AfxGetCatalystPool(afxCatalyst cyst)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_CYST, 1, &cyst);
    return cyst->poolIdx;
}

_AMX afxError AfxCompileSimCommands(afxCatalyst cyst)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_CYST, 1, &cyst);

    /// If there was an error during recording, the application will be notified by an unsuccessful return 
    /// code returned by AfxEndCommandBuffer, and the command buffer will be moved to the invalid state.
    /// The command buffer must have been in the recording state, and, if successful, is moved to the executable state.

    if (cyst->state != amxCatalystState_RECORDING)
    {
        AfxThrowError();
        return err;
    }
    else
    {
        if (cyst->endCb(cyst))
        {
            AfxThrowError();
            return err;
        }
        else
        {
            cyst->state = amxCatalystState_EXECUTABLE;
        }
    }
    return err;
}

_AMX afxError AfxRecycleCatalyst(afxCatalyst cyst, afxBool freeRes)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_CYST, 1, &cyst);

    afxSimQueue mque = AfxGetProvider(cyst);
    AFX_ASSERT_OBJECTS(afxFcc_MQUE, 1, &mque);

    afxUnit poolIdx = cyst->poolIdx;

    if (AfxTryEnterSlockExclusive(&mque->cmdbReqLock))
    {
        if (AfxPushQueue(&mque->cmdbRecycQue, &cyst))
        {
            AfxDisposeObjects(1, &cyst);
        }
        else
        {
            cyst->state = amxCatalystState_INVALID;
        }
        AfxExitSlockExclusive(&mque->cmdbReqLock);
    }
    else
    {
        AfxDisposeObjects(1, &cyst);
    }
    return err;
}

_AMX afxError _AmxCystDtorCb(afxCatalyst cyst)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_CYST, 1, &cyst);

    while (AfxLoadAtom32(&cyst->submCnt))
    {
        AfxYield();
    }

    AFX_ASSERT(cyst->state != amxCatalystState_PENDING);

    //AfxRecycleCmdBuffers(TRUE, 1, &cyst);

    //if (cyst->base.vmt->dtor && cyst->base.vmt->dtor(cyst))
        //AfxThrowError();

    //AFX_ASSERT(!cyst->base.idd);

    AfxDismantleArena(&cyst->cmdArena);

    return err;
}

_AMX afxError _AmxCystCtorCb(afxCatalyst cyst, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_CYST, 1, &cyst);

    afxSimQueue mque = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_MQUE, 1, &mque);

    //afxSimulation dsys = AfxGetSimBridgeContext(mexu);

    cyst->submCnt = 0;
    cyst->submQueMask = NIL;
    cyst->portId = AfxGetSimQueuePort(mque);
    cyst->poolIdx = AfxGetObjectId(mque);

    cyst->disposable = TRUE;

    cyst->state = amxCatalystState_INITIAL;

    AfxDeployChain(&cyst->commands, cyst);
    AfxDeployArena(&cyst->cmdArena, NIL, AfxHere());

    cyst->endCb = _AmxCystEndCb;
    cyst->resetCb = _AmxCystResetCb;

    return err;
}

_AMX afxClassConfig const _AMX_CYST_CLASS_CONFIG =
{
    .fcc = afxFcc_CYST,
    .name = "Catalyst",
    .desc = "Simulation Catalyst",
    .fixedSiz = sizeof(AFX_OBJECT(afxCatalyst)),
    .ctor = (void*)_AmxCystCtorCb,
    .dtor = (void*)_AmxCystDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AMX afxError AfxAcquireCatalysts(afxSimulation dsys, afxUnit exuIdx, afxUnit queIdx, afxUnit cnt, afxCatalyst batches[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &dsys);
    AFX_ASSERT(batches);
    AFX_ASSERT(cnt);

    afxSimBridge mexu;
    if (!AfxGetSimBridges(dsys, exuIdx, 1, &mexu))
    {
        AfxThrowError();
        return err;
    }
    AFX_ASSERT_OBJECTS(afxFcc_MEXU, 1, &mexu);

    afxSimQueue mque;
    if (!AfxGetSimQueues(mexu, queIdx, 1, &mque))
    {
        AfxThrowError();
        return err;
    }
    AFX_ASSERT_OBJECTS(afxFcc_MQUE, 1, &mque);

    if (!AfxTryEnterSlockExclusive(&mque->cmdbReqLock))
    {
        AfxThrowError();
        return err;
    }

    afxUnit cnt2 = 0;
    afxCatalyst* unit;
    while ((unit = AfxPeekQueue(&mque->cmdbRecycQue)))
    {
        afxCatalyst cyst = *unit;
        AFX_ASSERT_OBJECTS(afxFcc_CYST, 1, &cyst);

        if (cyst->resetCb(cyst, TRUE, 0)) AfxThrowError();
        else
        {
            AFX_ASSERT(cyst->state == amxCatalystState_RECORDING);

            batches[cnt2] = cyst;
            ++cnt2;
        }

        if (err)
            AfxDisposeObjects(1, &cyst);

        AfxPopQueue(&mque->cmdbRecycQue);

        if (cnt2 == cnt)
            break;
    }

    AFX_ASSERT_OBJECTS(afxFcc_CYST, cnt2, batches);

    AfxExitSlockExclusive(&mque->cmdbReqLock);

    if (cnt2 < cnt)
    {
        afxUnit cnt3 = cnt - cnt2;

        afxClass* cls = (afxClass*)_AmxGetCatalystClass(mque);
        AFX_ASSERT_CLASS(cls, afxFcc_CYST);

        if (AfxAcquireObjects(cls, cnt3, (afxObject*)&batches[cnt2], (void const*[]) { mque }))
        {
            AfxThrowError();

            for (afxUnit j = cnt2; j-- > 0;)
                AfxRecycleCatalyst(batches[j], TRUE);

            cnt2 = 0;
        }
        else
        {
            for (afxUnit k = 0; k < cnt3; k++)
            {
                afxCatalyst cyst = batches[cnt2 + k];
                AFX_ASSERT_OBJECTS(afxFcc_CYST, 1, &cyst);

                if (cyst->resetCb(cyst, TRUE, 0)) AfxThrowError();
                else
                {
                    AFX_ASSERT(cyst->state == amxCatalystState_RECORDING);

                    batches[cnt2] = cyst;
                    ++cnt2;
                }
            }
            AFX_ASSERT_OBJECTS(afxFcc_CYST, cnt2, batches);
        }
    }
    return err;
}
