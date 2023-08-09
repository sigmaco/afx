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

#define _AFX_DRAW_INPUT_C
#define _AFX_DRAW_SCRIPT_C
#define _AFX_DRAW_CONTEXT_C
#include "sgl.h"
#include "afx/afxQwadro.h"

// DRAW CODE API ///////////////////////////////////////////////////////////////

_SGL void _SglDcmdCommand(afxDrawScript dscr, afxNat id, afxNat siz, _afxDscrCmd *cmd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dscr, AFX_FCC_DSCR);
    sglDscrIdd *idd = dscr->idd;
    cmd->id = id;
    cmd->siz = siz;
    AfxPushLinkage(&cmd->script, &idd->commands);
}

_SGL void _SglDcmdNextPass(afxDrawScript dscr, afxBool useAuxScripts)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->state == AFX_DSCR_STATE_RECORDING);

    _afxDscrCmdNextPass *cmd = AfxRequestArenaUnit(&dscr->cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->useAuxScripts = !!useAuxScripts;
    _SglDcmdCommand(dscr, AFX_DCMD_NEXT_PASS, sizeof(cmd), &cmd->cmd);
}

_SGL void _SglDcmdSetRasterizerState(afxDrawScript dscr, afxPipelineRasterizerState const *state)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->state == AFX_DSCR_STATE_RECORDING);

    _afxDscrCmdSetRasterizerState *cmd = AfxRequestArenaUnit(&dscr->cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->state = *state;

    _SglDcmdCommand(dscr, AFX_DCMD_SET_RASTERIZATION_STATE, sizeof(cmd), &cmd->cmd);
}

_SGL void _SglDcmdSetDepthState(afxDrawScript dscr, afxPipelineDepthState const *state)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->state == AFX_DSCR_STATE_RECORDING);

    _afxDscrCmdSetDepthState *cmd = AfxRequestArenaUnit(&dscr->cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->state = *state;

    _SglDcmdCommand(dscr, AFX_DCMD_SET_DEPTH_STATE, sizeof(cmd), &cmd->cmd);
}

_SGL void _SglDcmdSetInputAssemblyState(afxDrawScript dscr, afxPipelineInputAssemblyState const *state)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->state == AFX_DSCR_STATE_RECORDING);

    _afxDscrCmdSetInputAssemblyState *cmd = AfxRequestArenaUnit(&dscr->cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->state = *state;

    _SglDcmdCommand(dscr, AFX_DCMD_SET_INPUT_ASSEMBLY_STATE, sizeof(cmd), &cmd->cmd);
}

_SGL void _SglDcmdBeginDrawPass(afxDrawScript dscr, afxDrawPassState const *state)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->state == AFX_DSCR_STATE_RECORDING);

    _afxDscrCmdBeginDrawPass *cmd = AfxRequestArenaUnit(&dscr->cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    AfxRectCopy(&cmd->area, &state->area);
    cmd->layerCnt = state->layerCnt;
    cmd->rasterCnt = state->rasterCnt;
    cmd->canv = state->canv;

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

    _SglDcmdCommand(dscr, AFX_DCMD_BEGIN_DRAW_PASS, sizeof(cmd), &cmd->cmd);
}

_SGL void _SglDcmdEndDrawPass(afxDrawScript dscr)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->state == AFX_DSCR_STATE_RECORDING);

    _afxDscrCmd *cmd = AfxRequestArenaUnit(&dscr->cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    _SglDcmdCommand(dscr, AFX_DCMD_END_DRAW_PASS, sizeof(cmd), cmd);
}

_SGL void _SglDcmdSetViewports(afxDrawScript dscr, afxNat32 first, afxNat32 cnt, afxViewport const vp[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->state == AFX_DSCR_STATE_RECORDING);

    _afxDscrCmdSetVp *cmd = AfxRequestArenaUnit(&dscr->cmdArena, sizeof(*cmd));
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
    AfxAssert(dscr->state == AFX_DSCR_STATE_RECORDING);

    _afxDscrCmdSetScissor *cmd = AfxRequestArenaUnit(&dscr->cmdArena, sizeof(*cmd));
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
    AfxAssert(dscr->state == AFX_DSCR_STATE_RECORDING);

    _afxDscrCmdBindPip *cmd = AfxRequestArenaUnit(&dscr->cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->pip = pip;

    _SglDcmdCommand(dscr, AFX_DCMD_BIND_PIPELINE, sizeof(cmd), &cmd->cmd);
}

_SGL void _SglDcmdBindIndexBuffer(afxDrawScript dscr, afxBuffer buf, afxNat32 offset, afxNat32 idxSiz)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->state == AFX_DSCR_STATE_RECORDING);

    _afxDscrCmdBindIbuf *cmd = AfxRequestArenaUnit(&dscr->cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->buf = buf;
    cmd->offset = offset;
    cmd->idxSiz = idxSiz;

    _SglDcmdCommand(dscr, AFX_DCMD_BIND_INDEX_BUFFER, sizeof(cmd), &cmd->cmd);
}

_SGL void _SglDcmdBindManagedIndexBuffer(afxDrawScript dscr, afxIndexBuffer ibuf, afxNat rgnIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->state == AFX_DSCR_STATE_RECORDING);

    _afxDscrCmdBindIbuf *cmd = AfxRequestArenaUnit(&dscr->cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->buf = &ibuf->buf;

    afxNat idxCnt, idxSiz;
    AfxIndexBufferDescribeRegion(ibuf, rgnIdx, &cmd->offset, &idxCnt, &idxSiz);
    cmd->idxSiz = idxSiz;

    _SglDcmdCommand(dscr, AFX_DCMD_BIND_INDEX_BUFFER, sizeof(cmd), &cmd->cmd);
}

_SGL void _SglDcmdBindBuffers(afxDrawScript dscr, afxNat set, afxNat first, afxNat cnt, afxBuffer buf[], afxNat offset[], afxNat range[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->state == AFX_DSCR_STATE_RECORDING);

    _afxDscrCmdBindBuffers *cmd = AfxRequestArenaUnit(&dscr->cmdArena, sizeof(*cmd));
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
    AfxAssert(dscr->state == AFX_DSCR_STATE_RECORDING);

    _afxDscrCmdBindTextures *cmd = AfxRequestArenaUnit(&dscr->cmdArena, sizeof(*cmd));
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
    AfxAssert(dscr->state == AFX_DSCR_STATE_RECORDING);

    _afxDscrCmdBindVbuf *cmd = AfxRequestArenaUnit(&dscr->cmdArena, sizeof(*cmd));
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
    AfxAssert(dscr->state == AFX_DSCR_STATE_RECORDING);

    _afxDscrCmdBindVbuf *cmd = AfxRequestArenaUnit(&dscr->cmdArena, sizeof(*cmd));
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
    AfxAssert(dscr->state == AFX_DSCR_STATE_RECORDING);

    _afxDscrCmdBindVbuf *cmd = AfxRequestArenaUnit(&dscr->cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->first = first;
    cmd->cnt = cnt;

    for (afxNat i = 0; i < cnt; i++)
    {
        if (vbuf[i])
        {
            afxNat arr = vtxArr ? vtxArr[i] : 0;
            cmd->buf[i] = &vbuf[i]->buf;
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
    AfxAssert(dscr->state == AFX_DSCR_STATE_RECORDING);

    _afxDscrCmdDrawIndexed *cmd = AfxRequestArenaUnit(&dscr->cmdArena, sizeof(*cmd));
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
    AfxAssert(dscr->state == AFX_DSCR_STATE_RECORDING);

    _afxDscrCmdDraw *cmd = AfxRequestArenaUnit(&dscr->cmdArena, sizeof(*cmd));
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
    AfxAssert(dscr->state != AFX_DSCR_STATE_PENDING);


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

            //if (cmd != &dscr->cmdEnd)
            {
                //AfxDeallocate(all, cmd);
                //AfxAssertType(idd->cmdArena, AFX_FCC_AREN);
                AfxRecycleArenaUnit(&idd->cmdArena, cmd, cmd->siz);
            }
        }
    }
#else
    AfxExhaustArena(&dscr->cmdArena);
#endif

    return err;
}

_SGL afxError _SglDscrVmtEndCb(afxDrawScript dscr)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dscr, AFX_FCC_DSCR);
    return err;
}

_SGL afxError _SglDscrVmtBeginCb(afxDrawScript dscr, afxBool permanent)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dscr, AFX_FCC_DSCR);
    dscr->disposable = !permanent;
    return err;
}

_SGL afxError _SglDscrVmtDtorCb(afxDrawScript dscr)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dscr, AFX_FCC_DSCR);
    sglDscrIdd *idd = dscr->idd;

    afxContext mem = AfxGetDrawMemory();

    AfxDeallocate(mem, idd);
    dscr->idd = NIL;

    return err;
}

_SGL _afxDcmdVmt _SglDcmdVmt =
{
    _SglDcmdBeginDrawPass,
    _SglDcmdNextPass,
    _SglDcmdEndDrawPass,

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
    _SglDscrVmtDtorCb,
    _SglDscrVmtBeginCb,
    _SglDscrVmtEndCb,
    _SglDscrVmtResetCb,
};

_SGL afxError _SglDctxVmtDscrCb(afxDrawScript dscr)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dscr, AFX_FCC_DSCR);

    afxContext mem = AfxGetDrawMemory();

    sglDscrIdd *idd = AfxAllocate(mem, sizeof(*idd), 0, AfxSpawnHint());
    dscr->idd = idd;
    
    AfxAcquireChain(&idd->commands, dscr);

    dscr->vmt = &_SglDscrVmt;
    dscr->cmd = &_SglDcmdVmt;

    return err;
}
