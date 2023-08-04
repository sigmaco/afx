/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                      S I G M A   T E C H N O L O G Y   G R O U P
 *
 *                                   Public Test Build
 *                               (c) 2017 Federação SIGMA
 *                                    www.sigmaco.org
 */

#define _AFX_DRAW_SCRIPT_C
#define _AFX_DRAW_INPUT_C
#include "_classified/afxDrawClassified.h"

struct _dscrParadigm
{
    afxNat portIdx;
    afxDrawInput owner;
};

_AFX void AfxDrawCmdNextPass(afxDrawScript dscr, afxBool useAuxScripts)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dscr, AFX_FCC_DSCR);
    dscr->cmd->nextPass(dscr, useAuxScripts);
}

_AFX void AfxDrawCmdSetRasterizerState(afxDrawScript dscr, afxPipelineRasterizerState const *state)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dscr, AFX_FCC_DSCR);
	AfxAssert(state);
	AfxAssert(state->fillMode < AFX_FILL_MODE_TOTAL);
	AfxAssert(state->cullMode < AFX_CULL_MODE_TOTAL);
	AfxAssert(state->frontFace < AFX_FRONT_FACE_TOTAL);
	AfxAssert(state->lineWidth);
    dscr->cmd->setRasterizerState(dscr, state);
}

_AFX void AfxDrawCmdSetDepthState(afxDrawScript dscr, afxPipelineDepthState const *state)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dscr, AFX_FCC_DSCR);
	AfxAssert(state);
	AfxAssert(state->depthCompareOp < AFX_COMPARE_OP_TOTAL);
    dscr->cmd->setDepthState(dscr, state);
}

_AFX void AfxDrawCmdSetInputAssemblyState(afxDrawScript dscr, afxPipelineInputAssemblyState const *state)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dscr, AFX_FCC_DSCR);
	AfxAssert(state);
	AfxAssert(state->topology < AFX_PRIM_TOPOLOGY_TOTAL);
    dscr->cmd->setInputAssemblyState(dscr, state);
}

_AFX void AfxDrawCmdBeginDrawPass(afxDrawScript dscr, afxDrawPassState const *state)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dscr, AFX_FCC_DSCR);
    AfxAssert(state);

	AfxAssert(state->layerCnt);
	//AfxAssert(state->rasterCnt);
	AfxAssert(state->rasters);
	
	for (afxNat i = 0; i < state->rasterCnt; i++)
	{
		afxDrawTarget const *rt = &state->rasters[i];
		AfxTryAssertObject(rt->surf, AFX_FCC_SURF);
	}

	if (state->depth)
	{
		AfxTryAssertObject(state->depth->surf, AFX_FCC_SURF);
	}

	if (state->stencil)
	{
		AfxTryAssertObject(state->stencil->surf, AFX_FCC_SURF);
	}

    dscr->cmd->beginPass(dscr, state);
}

_AFX void AfxDrawCmdEndDrawPass(afxDrawScript dscr)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dscr, AFX_FCC_DSCR);
    dscr->cmd->endPass(dscr);
}

_AFX void AfxDrawCmdSetViewports(afxDrawScript dscr, afxNat first, afxNat cnt, afxViewport const vp[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dscr, AFX_FCC_DSCR);
	AfxAssert(8 > first);
	AfxAssert(8 >= cnt);
	AfxAssert(vp);
    dscr->cmd->setViewport(dscr, first, cnt, vp);
}

_AFX void AfxDrawCmdSetScissors(afxDrawScript dscr, afxNat first, afxNat cnt, afxRect const rect[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dscr, AFX_FCC_DSCR);
	AfxAssert(8 > first);
	AfxAssert(8 >= cnt);
	AfxAssert(rect);
    dscr->cmd->setScissor(dscr, first, cnt, rect);
}

_AFX void AfxDrawCmdBindPipeline(afxDrawScript dscr, afxPipeline pip)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dscr, AFX_FCC_DSCR);
	AfxAssertObject(pip, AFX_FCC_PIP);
    dscr->cmd->bindPipeline(dscr, pip);
}

_AFX void AfxDrawCmdBindIndexBuffer(afxDrawScript dscr, afxBuffer buf, afxNat offset, afxNat idxSiz)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dscr, AFX_FCC_DSCR);
    AfxAssertObject(buf, AFX_FCC_BUF);
    AfxAssert(offset < AfxGetBufferSize(buf));
    dscr->cmd->bindIndexBuffer(dscr, buf, offset, idxSiz);
}

_AFX void AfxDrawCmdBindManagedIndexBuffer(afxDrawScript dscr, afxIndexBuffer ibuf, afxNat rgnIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dscr, AFX_FCC_DSCR);
    AfxAssertObject(ibuf, AFX_FCC_BUF);
    dscr->cmd->bindManagedIndexBuffer(dscr, ibuf, rgnIdx);
}

_AFX void AfxDrawCmdBindBuffers(afxDrawScript dscr, afxNat set, afxNat first, afxNat cnt, afxBuffer buf[], afxNat offset[], afxNat range[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dscr, AFX_FCC_DSCR);
    AfxAssert(8 > first);
    AfxAssert(8 >= cnt);    
    dscr->cmd->bindBuffers(dscr, set, first, cnt, buf, offset, range);
}

_AFX void AfxDrawCmdBindTextures(afxDrawScript dscr, afxNat set, afxNat first, afxNat cnt, afxSampler smp[], afxTexture tex[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dscr, AFX_FCC_DSCR);
    AfxAssert(8 > first);
    AfxAssert(8 >= cnt);
    dscr->cmd->bindTextures(dscr, set, first, cnt, smp, tex);
}

_AFX void AfxDrawCmdBindVertexBuffers(afxDrawScript dscr, afxNat first, afxNat cnt, afxBuffer buf[], afxSize const offset[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dscr, AFX_FCC_DSCR);
    AfxAssert(8 > first);
    AfxAssert(8 >= cnt);
    dscr->cmd->bindVertexBuffers(dscr, first, cnt, buf, offset);
}

_AFX void AfxDrawCmdBindVertexBuffers2(afxDrawScript dscr, afxNat first, afxNat cnt, afxBuffer buf[], afxSize const offset[], afxSize const stride[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dscr, AFX_FCC_DSCR);
    AfxAssert(8 > first);
    AfxAssert(8 >= cnt);
    dscr->cmd->bindVertexBuffers2(dscr, first, cnt, buf, offset, stride);
}

_AFX void AfxDrawCmdBindManagedVertexBuffers(afxDrawScript dscr, afxNat first, afxNat cnt, afxVertexBuffer vbuf[], afxNat const baseVtx[], afxNat const vtxArr[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dscr, AFX_FCC_DSCR);
	AfxAssert(8 > first);
	AfxAssert(8 >= cnt);

	for (afxNat i = 0; i < cnt; i++)
	{
		afxVertexBuffer vbo = vbuf[i];

		AfxAssertObject(vbo, AFX_FCC_VBUF);

		if (baseVtx)
		{
			AfxAssert(baseVtx[i] < AfxVertexBufferGetCap(vbo));
		}

		if (vtxArr)
		{
			AfxAssert(vtxArr[i] < AfxVertexBufferGetRowCount(vbo));
		}
	}
    dscr->cmd->bindManagedVertexBuffers(dscr, first, cnt, vbuf, baseVtx, vtxArr);
}

_AFX void AfxDrawCmdDrawIndexed(afxDrawScript dscr, afxNat idxCnt, afxNat instCnt, afxNat firstIdx, afxNat vtxOff, afxNat firstInst)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dscr, AFX_FCC_DSCR);
	AfxAssert(idxCnt);
	AfxAssert(instCnt);
    dscr->cmd->drawIndexed(dscr, idxCnt, instCnt, firstIdx, vtxOff, firstInst);
}

_AFX void AfxDrawCmdDraw(afxDrawScript dscr, afxNat vtxCnt, afxNat instCnt, afxNat firstVtx, afxNat firstInst)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dscr, AFX_FCC_DSCR);
	AfxAssert(vtxCnt);
	AfxAssert(instCnt);
    dscr->cmd->draw(dscr, vtxCnt, instCnt, firstVtx, firstInst);
}

_AFX afxError AfxResetDrawScript(afxDrawScript dscr)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dscr, AFX_FCC_DSCR);

    if (dscr->state == AFX_DSCR_STATE_PENDING) AfxThrowError();
    else
    {
        if (dscr->vmt->reset(dscr)) AfxThrowError();
        else
        {
            dscr->state = AFX_DSCR_STATE_INITIAL;
        }
    }
    return err;
}

_AFX afxError AfxEndDrawScript(afxDrawScript dscr)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dscr, AFX_FCC_DSCR);

    if (dscr->state != AFX_DSCR_STATE_RECORDING) AfxThrowError();
    else
    {
        if (dscr->vmt->end(dscr)) AfxThrowError();
        else
        {
            dscr->state = AFX_DSCR_STATE_EXECUTABLE;
        }
    }
    return err;
}

_AFX afxError AfxBeginDrawScript(afxDrawScript dscr, afxBool permanent)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dscr, AFX_FCC_DSCR);

    if (!(dscr->state == AFX_DSCR_STATE_INITIAL || dscr->state == AFX_DSCR_STATE_EXECUTABLE)) AfxThrowError();
    else
    {
        if (dscr->vmt->begin(dscr, permanent)) AfxThrowError();
        else
        {
            dscr->state = AFX_DSCR_STATE_RECORDING;
        }
    }
    return err;
}

AFX afxDrawScriptState AfxGetDrawScriptState(afxDrawScript dscr)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dscr, AFX_FCC_DSCR);
    return dscr->state;
}

AFX afxNat AfxGetDrawScriptPort(afxDrawScript dscr)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dscr, AFX_FCC_DSCR);
    return dscr->portIdx;
}

_AFX afxDrawInput AfxGetDrawScriptInput(afxDrawScript dscr)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dscr, AFX_FCC_DSCR);
    afxDrawInput din = AfxGetLinker(&dscr->din);
    AfxAssertObject(din, AFX_FCC_DIN);
    return din;
}

_AFX afxDrawDriver AfxGetDrawScriptDriver(afxDrawScript dscr)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dscr, AFX_FCC_DSCR);
    afxDrawDriver ddrv = AfxObjectGetProvider(&dscr->obj);
    AfxAssertObject(ddrv, AFX_FCC_DDRV);
    return ddrv;
}

_AFX afxError AfxAcquireDrawScripts(afxDrawInput din, afxNat portIdx, afxNat cnt, afxDrawScript dscr[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(din, AFX_FCC_DIN);
    
    afxDrawContext dctx;
    if (!(AfxGetConnectedDrawInputContext(din, &dctx))) AfxThrowError();
    else
    {
        AfxAssertObject(dctx, AFX_FCC_DCTX);
        afxNat cnt2 = 0;
        
        for (afxNat i = 0; i < din->scripts.cnt; i++)
        {
            afxDrawScript dscr2 = *(afxDrawScript*)AfxGetArrayUnit(&din->scripts, i);
            AfxAssertObject(dscr2, AFX_FCC_DSCR);

            if (AFX_DSCR_STATE_INVALID == AfxGetDrawScriptState(dscr2))
            {
                if (AfxResetDrawScript(dscr2)) AfxThrowError();
                else
                {
                    dscr[cnt2] = dscr2;
                    ++cnt2;
                }
            }

            if (cnt2 >= cnt)
                break;
        }

        if (cnt2 < cnt)
        {
            afxDrawDriver ddrv = AfxGetDrawInputDriver(din);
            AfxAssertObject(ddrv, AFX_FCC_DDRV);
            AfxEntry("ddrv=%p", ddrv);

            struct _dscrParadigm paradigm = { 0 };
            paradigm.portIdx = portIdx;
            paradigm.owner = din;

            if (AfxClassAcquireObjects(AfxGetDrawScriptClass(ddrv), NIL, cnt - cnt2, &paradigm, (afxObject**)&dscr[cnt2], AfxSpawnHint()))
                AfxThrowError();
        }
    }

    if (err)
        for (afxNat i = 0; i < cnt; i++)
            dscr[i] = NIL;

    return err;
}

_AFX afxBool _AfxDscrEventHandler(afxObject *obj, afxEvent *ev)
{
    afxError err = AFX_ERR_NONE;
    afxDrawScript dscr = (void*)obj;
    AfxAssertObject(dscr, AFX_FCC_DSCR);
    (void)ev;
    return FALSE;
}

_AFX afxBool _AfxDscrEventFilter(afxObject *obj, afxObject *watched, afxEvent *ev)
{
    afxError err = AFX_ERR_NONE;
    afxDrawScript dscr = (void*)obj;
    AfxAssertObject(dscr, AFX_FCC_DSCR);
    (void)watched;
    (void)ev;
    return FALSE;
}

_AFX afxError _AfxDscrDtor(afxDrawScript dscr)
{
    AfxEntry("dscr=%p", dscr);
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dscr, AFX_FCC_DSCR);

    while (1)
    {
        if (dscr->state != AFX_DSCR_STATE_PENDING) break;
        else
        {
            // AfxPopLinkage(&dscr->queue); // we can't do it here. We need wait for draw context to liberate it.

            //afxDrawContext dctx = AfxObjectGetProvider(&dscr->obj);
            //AfxAssertObject(dctx, AFX_FCC_DCTX);
            //AfxDrawContextProcess(dctx); // process until draw context ends its works and unlock this script.
            //AfxYieldThreading();
        }
        break;
    }

    afxDrawInput din = dscr->din;

    for (afxNat i = 0; i < din->scripts.cnt; i++)
    {
        afxDrawScript dscr2 = *(afxDrawScript*)AfxGetArrayUnit(&din->scripts, i);
        
        if (dscr2 == dscr)
        {
            *((afxDrawScript*)&din->scripts.bytemap[din->scripts.unitSiz * i]) = NIL;
        }
    }

    AfxResetDrawScript(dscr);

    if (dscr->vmt->dtor && dscr->vmt->dtor(dscr))
        AfxThrowError();

    AfxAssert(!dscr->idd);

    AfxReleaseArena(&dscr->cmdArena);

    return err;
}

_AFX afxError _AfxDscrCtor(void *cache, afxNat idx, afxDrawScript dscr, struct _dscrParadigm *paradigm)
{
    AfxEntry("dscr=%p", dscr);
    afxError err = AFX_ERR_NONE;
    (void)cache;
    (void)idx;
    AfxAssertObject(dscr, AFX_FCC_DSCR);

    afxDrawInput din = paradigm->owner;
    AfxAssertObject(din, AFX_FCC_DIN);
    afxDrawContext dctx;
    AfxGetConnectedDrawInputContext(din, &dctx);
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    afxMemory mem = AfxGetDrawContextMemory(dctx);

    dscr->submRefCnt = 0;
    dscr->portIdx = paradigm->portIdx;
    AfxAcquireArena(mem, &dscr->cmdArena, NIL, AfxSpawnHint());

    dscr->din = din;

    afxNat index = 0;
    AfxInsertArrayUnits(&din->scripts, 1, &index, &dscr);

    dscr->disposable = TRUE;

    dscr->state = AFX_DSCR_STATE_INITIAL;
    dscr->idd = NIL;
    dscr->vmt = NIL;
    dscr->cmd = NIL;

    if (dctx->vmt->dscr && dctx->vmt->dscr(dscr)) AfxThrowError();
    else
    {
        
        AfxAssert(dscr->vmt);
        AfxAssert(dscr->cmd);
    }

    if (err)
        AfxReleaseArena(&dscr->cmdArena);

    return err;
}

_AFX afxClassSpecification const _AfxDscrClassSpec;

afxClassSpecification const _AfxDscrClassSpec =
{
    AFX_FCC_DSCR,
    NIL,
    0,
    sizeof(AFX_OBJECT(afxDrawScript)),
    NIL,
    (void*)_AfxDscrCtor,
    (void*)_AfxDscrDtor,
    .event = _AfxDscrEventHandler,
    .eventFilter = _AfxDscrEventFilter,
    "afxDrawScript",
    NIL
};
