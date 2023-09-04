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
    AfxAssert(dscr->base.state == afxDrawScriptState_RECORDING);

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
    AfxAssert(dscr->base.state == afxDrawScriptState_RECORDING);

    _afxDscrCmdNextPass *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->useAuxScripts = !!useAuxScripts;
    _SglDcmdCommand(dscr, AFX_DCMD_NEXT_PASS, sizeof(cmd), &cmd->cmd);
}

_SGL void _SglDcmdSetRasterizerState(afxDrawScript dscr, afxPipelineRasterizerState const *state)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == afxDrawScriptState_RECORDING);

    _afxDscrCmdSetRasterizerState *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->state = *state;

    _SglDcmdCommand(dscr, AFX_DCMD_SET_RASTERIZATION_STATE, sizeof(cmd), &cmd->cmd);
}

_SGL void _SglDcmdSetDepthState(afxDrawScript dscr, afxPipelineDepthState const *state)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == afxDrawScriptState_RECORDING);

    _afxDscrCmdSetDepthState *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->state = *state;

    _SglDcmdCommand(dscr, AFX_DCMD_SET_DEPTH_STATE, sizeof(cmd), &cmd->cmd);
}

_SGL void _SglDcmdSetInputAssemblyState(afxDrawScript dscr, afxPipelineInputAssemblyState const *state)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == afxDrawScriptState_RECORDING);

    _afxDscrCmdSetInputAssemblyState *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->state = *state;

    _SglDcmdCommand(dscr, AFX_DCMD_SET_INPUT_ASSEMBLY_STATE, sizeof(cmd), &cmd->cmd);
}

_SGL void _SglDcmdBeginCanvas(afxDrawScript dscr, afxCanvasConfig const *state)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == afxDrawScriptState_RECORDING);

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
    AfxAssert(dscr->base.state == afxDrawScriptState_RECORDING);

    _afxDscrCmd *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    _SglDcmdCommand(dscr, AFX_DCMD_END_DRAW_PASS, sizeof(cmd), cmd);
}

_SGL void _SglDcmdSetViewports(afxDrawScript dscr, afxNat32 first, afxNat32 cnt, afxViewport const vp[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == afxDrawScriptState_RECORDING);

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
    AfxAssert(dscr->base.state == afxDrawScriptState_RECORDING);

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
    AfxAssert(dscr->base.state == afxDrawScriptState_RECORDING);

    _afxDscrCmdBindPip *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->pip = pip;

    _SglDcmdCommand(dscr, AFX_DCMD_BIND_PIPELINE, sizeof(cmd), &cmd->cmd);
}

_SGL void _SglDcmdBindIndexStream(afxDrawScript dscr, afxBuffer buf, afxNat32 offset, afxNat32 idxSiz)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == afxDrawScriptState_RECORDING);

    _afxDscrCmdBindIbuf *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);

    if ((cmd->buf = buf))
    {
        AfxAssertObjects(1, &cmd->buf, AFX_FCC_BUF);
    }

    cmd->offset = offset;
    cmd->idxSiz = idxSiz;

    _SglDcmdCommand(dscr, AFX_DCMD_BIND_INDEX_BUFFER, sizeof(cmd), &cmd->cmd);
}

_SGL void _SglDcmdBindManagedIndexStream(afxDrawScript dscr, afxIndexBuffer ibuf, afxNat rgnIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == afxDrawScriptState_RECORDING);

    afxNat32 offset, idxSiz;
    AfxIndexBufferDescribeRegion(ibuf, rgnIdx, &offset, NIL, &idxSiz);
    _SglDcmdBindIndexStream(dscr, ibuf->base.buf, offset, idxSiz);
}

_SGL void _SglDcmdBindBuffers(afxDrawScript dscr, afxNat set, afxNat first, afxNat cnt, afxBuffer buf[], afxNat offset[], afxNat range[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == afxDrawScriptState_RECORDING);

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
    AfxAssert(dscr->base.state == afxDrawScriptState_RECORDING);

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

_SGL void _SglDcmdSetVertexInputLayout(afxDrawScript dscr, afxNat cnt, afxVertexInputPoint const spec[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == afxDrawScriptState_RECORDING);

    _afxDscrCmdSetVtxInLayout *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->cnt = cnt;

    for (afxNat i = 0; i < cnt; i++)
    {
        cmd->spec[i] = spec[i];
    }
    _SglDcmdCommand(dscr, AFX_DCMD_SET_VERTEX_INPUT_LAYOUT, sizeof(cmd), &cmd->cmd);
}

_SGL void _SglDcmdBindVertexStreams(afxDrawScript dscr, afxNat first, afxNat cnt, afxVertexInputStream const spec[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == afxDrawScriptState_RECORDING);

    _afxDscrCmdBindVbuf *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->first = first;
    cmd->cnt = cnt;

    for (afxNat i = 0; i < cnt; i++)
    {
        afxVertexInputStream const* spec2 = &spec[i];

        cmd->spec[i].offset = spec2->offset;
        cmd->spec[i].range = spec2->range;

        if ((cmd->spec[i].buf = spec2->buf))
        {
            AfxAssertObjects(1, &cmd->spec[i].buf, AFX_FCC_BUF);
            AfxAssertRange(AfxGetBufferSize(cmd->spec[i].buf), cmd->spec[i].offset, cmd->spec[i].range);
        }
        
        cmd->spec[i].stride = spec2->stride;
        cmd->spec[i].instance = !!spec2->instance;
        cmd->spec[i].instDivisor = spec2->instDivisor;
    }
    _SglDcmdCommand(dscr, AFX_DCMD_BIND_VERTEX_BUFFERS, sizeof(cmd), &cmd->cmd);
}

_SGL void _SglDcmdBindManagedVertexStreams(afxDrawScript dscr, afxNat first, afxNat cnt, afxVertexBuffer vbuf[], afxNat const baseVtx[], afxNat const vtxArr[], afxBool inst, afxNat divisor)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == afxDrawScriptState_RECORDING);

    afxNat visCnt = 0;
    afxNat secIndices[_SGL_MAX_VBO_PER_BIND];
    afxVertexInputStream vis[_SGL_MAX_VBO_PER_BIND];
    afxNat vipCnt = 0;
    afxVertexInputPoint vip[_SGL_MAX_INSTREAM_PER_SET];

    for (afxNat i = 0; i < cnt; i++)
    {
        if (vbuf[i])
        {
            afxNat arr = vtxArr ? vtxArr[i] : 0;

            afxNat secIdx;
            vip[i].location = arr;
            AfxDescribeVertexAttribute(vbuf[i], arr, &secIdx, &vip[i].fmt, &vip[i].offset, &vip[i].usage);
            ++vipCnt;
            
            afxBool found = FALSE;

            for (afxNat j = 0; j < visCnt; j++)
            {
                if (secIndices[j] == secIdx)
                {
                    found = TRUE;
                    vip[i].stream = j;
                    break;
                }
            }

            if (!found)
            {
                vip[i].stream = visCnt;
                secIndices[visCnt] = secIdx;

                afxNat32 secBase, secRange, secStride;
                AfxDescribeVertexSection(vbuf[i], secIdx, &secBase, &secRange, &secStride);

                afxNat subOff = baseVtx ? (secStride * baseVtx[i]) : 0;

                vis[visCnt].buf = vbuf[i]->base.buf;
                vis[visCnt].offset = secBase + subOff;
                vis[visCnt].range = secRange - vis[visCnt].offset;
                vis[visCnt].stride = secStride;

                vis[visCnt].instance = !!inst;
                vis[visCnt].instDivisor = divisor;

                ++visCnt;
            }
        }
    }

    _SglDcmdBindVertexStreams(dscr, first, visCnt, vis);
    _SglDcmdSetVertexInputLayout(dscr, vipCnt, vip);
}

_SGL void _SglDcmdDrawIndexed(afxDrawScript dscr, afxNat32 idxCnt, afxNat32 instCnt, afxNat32 firstIdx, afxNat32 vtxOff, afxNat32 firstInst)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == afxDrawScriptState_RECORDING);

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
    AfxAssert(dscr->base.state == afxDrawScriptState_RECORDING);

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
    AfxAssert(dscr->base.state != afxDrawScriptState_PENDING);

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
    _SglDcmdBindVertexStreams,
    _SglDcmdSetVertexInputLayout,
    _SglDcmdBindManagedVertexStreams,
    _SglDcmdBindIndexStream,
    _SglDcmdBindManagedIndexStream,
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
        if (dscr->base.state != afxDrawScriptState_PENDING) break;
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

    dscr->base.state = afxDrawScriptState_INITIAL;
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
