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

#include "sgl.h"
#include "afx/afxQwadro.h"

// DRAW CODE API ///////////////////////////////////////////////////////////////

_SGL void _SglDcmdCommand(afxDrawScript dscr, afxNat id, afxNat siz, _afxDscrCmd *cmd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, AFX_FCC_DSCR);
    cmd->id = id;
    cmd->siz = siz;
    AfxPushLinkage(&cmd->script, &dscr->commands);
}

_SGL void _SglDcmdExecuteCommands(afxDrawScript dscr, afxNat cnt, afxDrawScript subsets[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == AFX_DSCR_STATE_RECORDING);

    _afxDscrCmdExecCmds *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->cnt = cnt;

    for (afxNat i = 0; i < cnt; i++)
        cmd->subsets[i] = subsets[i];

    _SglDcmdCommand(dscr, AFX_DCMD_EXECUTE_COMMANDS, sizeof(cmd), &cmd->cmd);
}

_SGL void _SglDcmdNextPass(afxDrawScript dscr, afxBool useAuxScripts)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == AFX_DSCR_STATE_RECORDING);

    _afxDscrCmdNextPass *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->useAuxScripts = !!useAuxScripts;
    _SglDcmdCommand(dscr, AFX_DCMD_NEXT_PASS, sizeof(cmd), &cmd->cmd);
}

_SGL void _SglDcmdSetRasterizerState(afxDrawScript dscr, afxPipelineRasterizerState const *state)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == AFX_DSCR_STATE_RECORDING);

    _afxDscrCmdSetRasterizerState *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->state = *state;

    _SglDcmdCommand(dscr, AFX_DCMD_SET_RASTERIZATION_STATE, sizeof(cmd), &cmd->cmd);
}

_SGL void _SglDcmdSetDepthState(afxDrawScript dscr, afxPipelineDepthState const *state)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == AFX_DSCR_STATE_RECORDING);

    _afxDscrCmdSetDepthState *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->state = *state;

    _SglDcmdCommand(dscr, AFX_DCMD_SET_DEPTH_STATE, sizeof(cmd), &cmd->cmd);
}

_SGL void _SglDcmdSetInputAssemblyState(afxDrawScript dscr, afxPipelineInputAssemblyState const *state)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == AFX_DSCR_STATE_RECORDING);

    _afxDscrCmdSetInputAssemblyState *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->state = *state;

    _SglDcmdCommand(dscr, AFX_DCMD_SET_INPUT_ASSEMBLY_STATE, sizeof(cmd), &cmd->cmd);
}

_SGL void _SglDcmdBeginCanvas(afxDrawScript dscr, afxCanvasConfig const *state)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == AFX_DSCR_STATE_RECORDING);

    _afxDscrCmdBeginCanvas *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    AfxRectCopy(&cmd->area, &state->area);
    cmd->layerCnt = state->layerCnt;
    cmd->rasterCnt = state->rasterCnt;
    //cmd->canv = state->canv;

    for (afxNat i = 0; i < state->rasterCnt; i++)
        cmd->rasters[i] = state->rasters[i];

    if (state->depth)
        cmd->depth = *state->depth;
    else
        cmd->depth = (afxDrawTarget) { 0 };

    if (state->stencil)
        cmd->stencil = *state->stencil;
    else
        cmd->stencil = (afxDrawTarget) { 0 };

    cmd->fboHandle = 0;

    _SglDcmdCommand(dscr, AFX_DCMD_BEGIN_DRAW_PASS, sizeof(cmd), &cmd->cmd);
}

_SGL void _SglDcmdEndCanvas(afxDrawScript dscr)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == AFX_DSCR_STATE_RECORDING);

    _afxDscrCmd *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    _SglDcmdCommand(dscr, AFX_DCMD_END_DRAW_PASS, sizeof(cmd), cmd);
}

_SGL void _SglDcmdSetViewports(afxDrawScript dscr, afxNat32 first, afxNat32 cnt, afxViewport const vp[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == AFX_DSCR_STATE_RECORDING);

    _afxDscrCmdSetVp *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->first = first;
    cmd->cnt = cnt;

    for (afxNat i = 0; i < cnt; i++)
        cmd->vp[i] = vp[i];

    _SglDcmdCommand(dscr, AFX_DCMD_SET_VIEWPORTS, sizeof(cmd), &cmd->cmd);
}

_SGL void _SglDcmdSetScissors(afxDrawScript dscr, afxNat32 first, afxNat32 cnt, afxRect const rect[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == AFX_DSCR_STATE_RECORDING);

    _afxDscrCmdSetScissor *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->first = first;
    cmd->cnt = cnt;

    for (afxNat i = 0; i < cnt; i++)
        cmd->rect[i] = rect[i];

    _SglDcmdCommand(dscr, AFX_DCMD_SET_SCISSORS, sizeof(cmd), &cmd->cmd);
}

_SGL void _SglDcmdBindPipeline(afxDrawScript dscr, afxPipeline pip)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == AFX_DSCR_STATE_RECORDING);

    _afxDscrCmdBindPip *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->pip = pip;

    _SglDcmdCommand(dscr, AFX_DCMD_BIND_PIPELINE, sizeof(cmd), &cmd->cmd);
}

_SGL void _SglDcmdBindIndexBuffer(afxDrawScript dscr, afxBuffer buf, afxNat32 offset, afxNat32 idxSiz)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == AFX_DSCR_STATE_RECORDING);

    _afxDscrCmdBindIbuf *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->buf = buf;
    cmd->offset = offset;
    cmd->idxSiz = idxSiz;

    _SglDcmdCommand(dscr, AFX_DCMD_BIND_INDEX_BUFFER, sizeof(cmd), &cmd->cmd);
}

_SGL void _SglDcmdBindManagedIndexBuffer(afxDrawScript dscr, afxIndexBuffer ibuf, afxNat rgnIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == AFX_DSCR_STATE_RECORDING);

    _afxDscrCmdBindIbuf *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->buf = ibuf->base.buf;

    afxNat idxCnt, idxSiz;
    AfxIndexBufferDescribeRegion(ibuf, rgnIdx, &cmd->offset, &idxCnt, &idxSiz);
    cmd->idxSiz = idxSiz;

    _SglDcmdCommand(dscr, AFX_DCMD_BIND_INDEX_BUFFER, sizeof(cmd), &cmd->cmd);
}

_SGL void _SglDcmdBindBuffers(afxDrawScript dscr, afxNat set, afxNat first, afxNat cnt, afxBuffer buf[], afxNat offset[], afxNat range[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == AFX_DSCR_STATE_RECORDING);

    _afxDscrCmdBindBuffers *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->set = set;
    cmd->first = first;
    cmd->cnt = cnt;

    for (afxNat i = 0; i < cnt; i++)
    {
        cmd->buf[i] = buf[i];
        cmd->offset[i] = offset[i];
        cmd->range[i] = range[i];
    }
    _SglDcmdCommand(dscr, AFX_DCMD_BIND_BUFFERS, sizeof(cmd), &cmd->cmd);
}

_SGL void _SglDcmdBindTextures(afxDrawScript dscr, afxNat set, afxNat first, afxNat cnt, afxSampler smp[], afxTexture tex[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == AFX_DSCR_STATE_RECORDING);

    _afxDscrCmdBindTextures *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->set = set;
    cmd->first = first;
    cmd->cnt = cnt;

    for (afxNat i = 0; i < cnt; i++)
    {
        cmd->smp[i] = smp[i];
        cmd->tex[i] = tex[i];
    }
    _SglDcmdCommand(dscr, AFX_DCMD_BIND_TEXTURES, sizeof(cmd), &cmd->cmd);
}

_SGL void _SglDcmdBindVertexBuffers(afxDrawScript dscr, afxNat first, afxNat cnt, afxBuffer buf[], afxSize const offset[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == AFX_DSCR_STATE_RECORDING);

    _afxDscrCmdBindVbuf *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->first = first;
    cmd->cnt = cnt;

    for (afxNat i = 0; i < cnt; i++)
    {
        cmd->buf[i] = buf ? buf[i] : 0;
        cmd->offset[i] = offset ? offset[i] : 0;
        //cmd->vtxArrSiz[i] = 0;
        cmd->vtxStride[i] = 0;
    }
    _SglDcmdCommand(dscr, AFX_DCMD_BIND_VERTEX_BUFFERS, sizeof(cmd), &cmd->cmd);
}

_SGL void _SglDcmdBindVertexBuffers2(afxDrawScript dscr, afxNat first, afxNat cnt, afxBuffer buf[], afxSize const offset[], afxSize const stride[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == AFX_DSCR_STATE_RECORDING);

    _afxDscrCmdBindVbuf *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->first = first;
    cmd->cnt = cnt;

    for (afxNat i = 0; i < cnt; i++)
    {
        cmd->buf[i] = buf ? buf[i] : 0;
        cmd->offset[i] = offset ? offset[i] : 0;
        //cmd->vtxArrSiz[i] = size ? size[i] : 0;
        cmd->vtxStride[i] = stride ? stride[i] : 0;
    }
    _SglDcmdCommand(dscr, AFX_DCMD_BIND_VERTEX_BUFFERS, sizeof(cmd), &cmd->cmd);
}

_SGL void _SglDcmdBindManagedVertexBuffers(afxDrawScript dscr, afxNat first, afxNat cnt, afxVertexBuffer vbuf[], afxNat const baseVtx[], afxNat const vtxArr[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == AFX_DSCR_STATE_RECORDING);

    _afxDscrCmdBindVbuf *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->first = first;
    cmd->cnt = cnt;

    for (afxNat i = 0; i < cnt; i++)
    {
        if (vbuf[i])
        {
            afxNat arr = vtxArr ? vtxArr[i] : 0;
            cmd->buf[i] = vbuf[i]->base.buf;
            cmd->offset[i] = AfxVertexBufferGetOffset(vbuf[i], baseVtx ? baseVtx[i] : 0, arr);
            cmd->vtxStride[i] = AfxVertexBufferGetStride(vbuf[i], arr);
            //cmd->vtxArrSiz[i] = 0;
        }
        else
        {
            cmd->buf[i] = 0;
            cmd->offset[i] = 0;
            cmd->vtxStride[i] = 0;
            //cmd->vtxArrSiz[i] = 0;
        }
    }
    _SglDcmdCommand(dscr, AFX_DCMD_BIND_VERTEX_BUFFERS, sizeof(cmd), &cmd->cmd);
}

_SGL void _SglDcmdDrawIndexed(afxDrawScript dscr, afxNat32 idxCnt, afxNat32 instCnt, afxNat32 firstIdx, afxNat32 vtxOff, afxNat32 firstInst)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == AFX_DSCR_STATE_RECORDING);

    _afxDscrCmdDrawIndexed *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->idxCnt = idxCnt;
    cmd->instCnt = instCnt;
    cmd->firstIdx = firstIdx;
    cmd->vtxOff = vtxOff;
    cmd->firstInst = firstInst;

    _SglDcmdCommand(dscr, AFX_DCMD_DRAW_INDEXED, sizeof(cmd), &cmd->cmd);
}

_SGL void _SglDcmdDraw(afxDrawScript dscr, afxNat32 vtxCnt, afxNat32 instCnt, afxNat32 firstVtx, afxNat32 firstInst)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == AFX_DSCR_STATE_RECORDING);

    _afxDscrCmdDraw *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->vtxCnt = vtxCnt;
    cmd->instCnt = instCnt;
    cmd->firstVtx = firstVtx;
    cmd->firstInst = firstInst;

    _SglDcmdCommand(dscr, AFX_DCMD_DRAW, sizeof(cmd), &cmd->cmd);
}

_SGL afxError _SglDscrVmtResetCb(afxDrawScript dscr)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state != AFX_DSCR_STATE_PENDING);

    _afxDscrCmd const* cmdHdr;
    AfxChainForEveryLinkageB2F(&dscr->commands, _afxDscrCmd, script, cmdHdr)
    {
        if (cmdHdr->id == AFX_DCMD_END)
            break;

        if (cmdHdr->id == AFX_DCMD_BEGIN_DRAW_PASS)
        {
            _afxDscrCmdBeginCanvas const *cmd = AFX_REBASE(cmdHdr, _afxDscrCmdBeginCanvas, cmd);

            if (cmd->fboHandle)
            {
                afxDrawContext dctx = AfxGetObjectProvider(dscr);
                _SglDeleteGlRes(dctx, 3, cmd->fboHandle);
                //cmd->fboHandle = 0;
            }
        }
    }

#if 0
    while (1)
    {
        afxChain *commands = &idd->commands;
        afxLinkage *first = AfxGetLastLinkage(commands), *dummy = AfxGetChainAnchor(commands);

        if (first == dummy) break;
        else
        {
            _afxDscrCmd *cmd = AFX_REBASE(first, _afxDscrCmd, script);
            AfxPopLinkage(&cmd->script);

            //if (cmd != &dscr->base.cmdEnd)
            {
                //AfxDeallocate(all, cmd);
                //AfxAssertType(idd->cmdArena, AFX_FCC_AREN);
                AfxRecycleArenaUnit(&idd->cmdArena, cmd, cmd->siz);
            }
        }
    }
#else
    AfxExhaustArena(&dscr->base.cmdArena);
#endif

    return err;
}

_SGL afxError _SglDscrVmtEndCb(afxDrawScript dscr)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, AFX_FCC_DSCR);
    return err;
}

_SGL afxError _SglDscrVmtBeginCb(afxDrawScript dscr, afxBool permanent)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, AFX_FCC_DSCR);
    dscr->base.disposable = !permanent;
    return err;
}

_SGL _afxDcmdVmt _SglDcmdVmt[] =
{
    _SglDcmdExecuteCommands,
    _SglDcmdBeginCanvas,
    _SglDcmdNextPass,
    _SglDcmdEndCanvas,

    _SglDcmdBindPipeline,

    _SglDcmdSetInputAssemblyState,
    _SglDcmdSetViewports,
    _SglDcmdSetRasterizerState,
    _SglDcmdSetDepthState,
    _SglDcmdSetScissors,

    _SglDcmdBindBuffers,
    _SglDcmdBindVertexBuffers,
    _SglDcmdBindVertexBuffers2,
    _SglDcmdBindManagedVertexBuffers,
    _SglDcmdBindIndexBuffer,
    _SglDcmdBindManagedIndexBuffer,
    _SglDcmdBindTextures,

    _SglDcmdDraw,
    _SglDcmdDrawIndexed,
};

_SGL _afxDscrVmt _SglDscrVmt =
{
    _SglDscrVmtBeginCb,
    _SglDscrVmtEndCb,
    _SglDscrVmtResetCb,
};

_SGL afxError _SglDscrDtor(afxDrawScript dscr)
{
    AfxEntry("dscr=%p", dscr);
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, AFX_FCC_DSCR);

    while (1)
    {
        if (dscr->base.state != AFX_DSCR_STATE_PENDING) break;
        else
        {
            // AfxPopLinkage(&dscr->base.queue); // we can't do it here. We need wait for draw context to liberate it.

            //afxDrawContext dctx = AfxGetObjectProvider(&dscr->base.obj);
            //AfxAssertType(dctxD, AFX_FCC_DCTX);
            //AfxDrawContextProcess(dctx); // process until draw context ends its works and unlock this script.
            //AfxYieldThreading();
        }
        break;
    }

    afxDrawInput din = dscr->base.din;
    AfxAssertObjects(1, &din, AFX_FCC_DIN);

    for (afxNat i = 0; i < din->base.scripts.cnt; i++)
    {
        afxDrawScript dscr2 = *(afxDrawScript*)AfxGetArrayUnit(&din->base.scripts, i);

        if (dscr2 == dscr)
        {
            *((afxDrawScript*)&din->base.scripts.bytemap[din->base.scripts.unitSiz * i]) = NIL;
        }
    }

    AfxResetDrawScript(dscr);

    //if (dscr->base.vmt->dtor && dscr->base.vmt->dtor(dscr))
        //AfxThrowError();

    //AfxAssert(!dscr->base.idd);

    AfxReleaseArena(&dscr->base.cmdArena);

    return err;
}

_SGL afxError _SglDscrCtor(afxDrawScript dscr, afxCookie const* cookie)
{
    AfxEntry("dscr=%p", dscr);
    afxError err = AFX_ERR_NONE;
    //AfxAssertObjects(1, &dscr, AFX_FCC_DSCR);

    struct _dscrParadigm *paradigm = ((struct _dscrParadigm *)cookie->udd[0]) + cookie->no;

    afxDrawInput din = paradigm->owner;
    AfxAssertObjects(1, &din, AFX_FCC_DIN);

    afxDrawContext dctx;
    AfxGetConnectedDrawInputContext(din, &dctx);
    afxContext ctx = AfxGetDrawContextMemory(dctx);

    dscr->base.submRefCnt = 0;
    dscr->base.portIdx = paradigm->portIdx;
    AfxAcquireArena(ctx, &dscr->base.cmdArena, NIL, AfxSpawnHint());

    dscr->base.din = din;

    afxNat index = 0;
    AfxInsertArrayUnits(&din->base.scripts, 1, &index, &dscr);

    dscr->base.disposable = TRUE;

    dscr->base.state = AFX_DSCR_STATE_INITIAL;
    dscr->base.vmt = NIL;
    dscr->base.cmd = NIL;

    AfxAcquireChain(&dscr->commands, dscr);

    dscr->base.vmt = &_SglDscrVmt;
    dscr->base.cmd = _SglDcmdVmt;

    {

        AfxAssert(dscr->base.vmt);
        AfxAssert(dscr->base.cmd);
    }

    if (err)
        AfxReleaseArena(&dscr->base.cmdArena);

    return err;
}

_SGL afxClassConfig _SglDscrClsConfig =
{
    .fcc = AFX_FCC_DSCR,
    .name = "Draw Script",
    .unitsPerPage = 1,
    .size = sizeof(AFX_OBJECT(afxDrawScript)),
    .ctx = NIL,
    .ctor = (void*)_SglDscrCtor,
    .dtor = (void*)_SglDscrDtor
};
