/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *              T H E   Q W A D R O   E X E C U T I O N   E C O S Y S T E M
 *
 *                                   Public Test Build
 *                   (c) 2017 SIGMA Technology Group — Federação SIGMA
 *                                    www.sigmaco.org
 */

#define _AFX_DRAW_SYSTEM_C
#define _AFX_DRAW_SCRIPT_C
#define _AFX_DRAW_INPUT_C
#define _AFX_DRAW_SYSTEM_C
#define _AFX_DRAW_CONTEXT_C
#include "_classified/afxDrawClassified.h"

_AFX void AfxCmdExecuteCommands(afxDrawScript dscr, afxNat cnt, afxDrawScript subsets[])
{
    afxError err = AFX_ERR_NONE;
    dscr->cmd->execCmd(dscr, cnt, subsets);
}

_AFX void AfxCmdNextPass(afxDrawScript dscr, afxBool useAuxScripts)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, AFX_FCC_DSCR);
    dscr->cmd->nextPass(dscr, useAuxScripts);
}

_AFX void AfxCmdSetRasterizerState(afxDrawScript dscr, afxPipelineRasterizerState const *state)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, AFX_FCC_DSCR);
	AfxAssert(state);
	AfxAssert(state->fillMode < AFX_FILL_MODE_TOTAL);
	AfxAssert(state->cullMode < AFX_CULL_MODE_TOTAL);
	AfxAssert(state->frontFace < AFX_FRONT_FACE_TOTAL);
	AfxAssert(state->lineWidth);
    dscr->cmd->setRasterizerState(dscr, state);
}

_AFX void AfxCmdSetDepthState(afxDrawScript dscr, afxPipelineDepthState const *state)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, AFX_FCC_DSCR);
	AfxAssert(state);
	AfxAssert(state->depthCompareOp < AFX_COMPARE_OP_TOTAL);
    dscr->cmd->setDepthState(dscr, state);
}

_AFX void AfxCmdSetInputAssemblyState(afxDrawScript dscr, afxPipelineInputAssemblyState const *state)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, AFX_FCC_DSCR);
	AfxAssert(state);
	AfxAssert(state->topology < AFX_PRIM_TOPOLOGY_TOTAL);
    dscr->cmd->setInputAssemblyState(dscr, state);
}

_AFX void AfxCmdBeginCanvas(afxDrawScript dscr, afxCanvasConfig const *state)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, AFX_FCC_DSCR);
    AfxAssert(state);
	AfxAssert(state->layerCnt);
	//AfxAssert(state->rasterCnt);
	AfxAssert(state->rasters);
	
	//for (afxNat i = 0; i < state->rasterCnt; i++)
	{
		//afxDrawTarget const *rt = &state->rasters[i];
		//AfxTryAssertObject(rt->tex, AFX_FCC_SURF);
	}

	if (state->depth)
	{
		//AfxTryAssertObject(state->depth->tex, AFX_FCC_SURF);
	}

	if (state->stencil)
	{
		//AfxTryAssertObject(state->stencil->tex, AFX_FCC_SURF);
	}

    dscr->cmd->beginCanv(dscr, state);
}

_AFX void AfxCmdEndCanvas(afxDrawScript dscr)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, AFX_FCC_DSCR);
    dscr->cmd->endCanv(dscr);
}

_AFX void AfxCmdSetViewports(afxDrawScript dscr, afxNat first, afxNat cnt, afxViewport const vp[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, AFX_FCC_DSCR);
	AfxAssert(8 > first);
	AfxAssert(8 >= cnt);
	AfxAssert(vp);
    dscr->cmd->setViewport(dscr, first, cnt, vp);
}

_AFX void AfxCmdSetScissors(afxDrawScript dscr, afxNat first, afxNat cnt, afxRect const rect[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, AFX_FCC_DSCR);
	AfxAssert(8 > first);
	AfxAssert(8 >= cnt);
	AfxAssert(rect);
    dscr->cmd->setScissor(dscr, first, cnt, rect);
}

_AFX void AfxCmdBindPipeline(afxDrawScript dscr, afxPipeline pip)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, AFX_FCC_DSCR);
	AfxAssertObjects(1, &pip, AFX_FCC_PIP);
    dscr->cmd->bindPipeline(dscr, pip);
}

_AFX void AfxCmdBindIndexBuffer(afxDrawScript dscr, afxBuffer buf, afxNat offset, afxNat idxSiz)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, AFX_FCC_DSCR);
    AfxAssertObjects(1, &buf, AFX_FCC_BUF);
    AfxAssert(offset < AfxGetBufferSize(buf));
    dscr->cmd->bindIndexBuffer(dscr, buf, offset, idxSiz);
}

_AFX void AfxCmdBindManagedIndexBuffer(afxDrawScript dscr, afxIndexBuffer ibuf, afxNat rgnIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, AFX_FCC_DSCR);
    AfxAssertObjects(1, &ibuf, AFX_FCC_BUF);
    dscr->cmd->bindManagedIndexBuffer(dscr, ibuf, rgnIdx);
}

_AFX void AfxCmdBindBuffers(afxDrawScript dscr, afxNat set, afxNat first, afxNat cnt, afxBuffer buf[], afxNat offset[], afxNat range[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, AFX_FCC_DSCR);
    AfxAssert(8 > first);
    AfxAssert(8 >= cnt);    
    dscr->cmd->bindBuffers(dscr, set, first, cnt, buf, offset, range);
}

_AFX void AfxCmdBindTextures(afxDrawScript dscr, afxNat set, afxNat first, afxNat cnt, afxSampler smp[], afxTexture tex[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, AFX_FCC_DSCR);
    AfxAssert(8 > first);
    AfxAssert(8 >= cnt);
    dscr->cmd->bindTextures(dscr, set, first, cnt, smp, tex);
}

_AFX void AfxCmdBindVertexBuffers(afxDrawScript dscr, afxNat first, afxNat cnt, afxBuffer buf[], afxSize const offset[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, AFX_FCC_DSCR);
    AfxAssert(8 > first);
    AfxAssert(8 >= cnt);
    dscr->cmd->bindVertexBuffers(dscr, first, cnt, buf, offset);
}

_AFX void AfxCmdBindVertexBuffers2(afxDrawScript dscr, afxNat first, afxNat cnt, afxBuffer buf[], afxSize const offset[], afxSize const stride[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, AFX_FCC_DSCR);
    AfxAssert(8 > first);
    AfxAssert(8 >= cnt);
    dscr->cmd->bindVertexBuffers2(dscr, first, cnt, buf, offset, stride);
}

_AFX void AfxCmdBindManagedVertexBuffers(afxDrawScript dscr, afxNat first, afxNat cnt, afxVertexBuffer vbuf[], afxNat const baseVtx[], afxNat const vtxArr[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, AFX_FCC_DSCR);
	AfxAssert(8 > first);
	AfxAssert(8 >= cnt);

	for (afxNat i = 0; i < cnt; i++)
	{
		afxVertexBuffer vbo = vbuf[i];

		AfxAssertObjects(1, &vbo, AFX_FCC_VBUF);

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

_AFX void AfxCmdDrawIndexed(afxDrawScript dscr, afxNat idxCnt, afxNat instCnt, afxNat firstIdx, afxNat vtxOff, afxNat firstInst)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, AFX_FCC_DSCR);
	AfxAssert(idxCnt);
	AfxAssert(instCnt);
    dscr->cmd->drawIndexed(dscr, idxCnt, instCnt, firstIdx, vtxOff, firstInst);
}

_AFX void AfxCmdDraw(afxDrawScript dscr, afxNat vtxCnt, afxNat instCnt, afxNat firstVtx, afxNat firstInst)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, AFX_FCC_DSCR);
	AfxAssert(vtxCnt);
	AfxAssert(instCnt);
    dscr->cmd->draw(dscr, vtxCnt, instCnt, firstVtx, firstInst);
}

_AFX afxError AfxResetDrawScript(afxDrawScript dscr)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, AFX_FCC_DSCR);

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
    AfxAssertObjects(1, &dscr, AFX_FCC_DSCR);

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
    AfxAssertObjects(1, &dscr, AFX_FCC_DSCR);

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
    AfxAssertObjects(1, &dscr, AFX_FCC_DSCR);
    return dscr->state;
}

AFX afxNat AfxGetDrawScriptPort(afxDrawScript dscr)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, AFX_FCC_DSCR);
    return dscr->portIdx;
}

_AFX afxDrawInput AfxGetDrawScriptInput(afxDrawScript dscr)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, AFX_FCC_DSCR);
    afxDrawInput din = dscr->din;
    //AfxAssertType(din, AFX_FCC_DIN);
    return din;
}

_AFX afxError AfxAcquireDrawScripts(afxDrawInput din, afxNat portIdx, afxNat cnt, afxDrawScript dscr[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, AFX_FCC_DIN);
    afxDrawContext dctx;

    if (!(AfxGetConnectedDrawInputContext(din, &dctx))) AfxThrowError();
    else
    {
        //AfxAssertType(dctxD, AFX_FCC_DCTX);
        afxNat cnt2 = 0;

        for (afxNat i = 0; i < din->scripts.cnt; i++)
        {
            afxDrawScript dscr2 = *(afxDrawScript*)AfxGetArrayUnit(&din->scripts, i);
            AfxAssertObjects(1, &dscr2, AFX_FCC_DSCR);

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
            struct _dscrParadigm paradigm = { 0 };
            paradigm.portIdx = portIdx;
            paradigm.owner = din;

            if (AfxAcquireObjects(&dctx->openPorts[portIdx].scripts, cnt - cnt2, (afxHandle*)&dscr[cnt2], (void*[]) { &paradigm }))
                AfxThrowError();
        }
    }

    if (err)
        for (afxNat i = 0; i < cnt; i++)
            dscr[i] = NIL;

    return err;
}
