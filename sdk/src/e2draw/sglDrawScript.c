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

#include "sgl.h"
#include "afx/afxQwadro.h"

// DRAW CODE API ///////////////////////////////////////////////////////////////

_SGL void _AfxDrawScriptCmdCommand(afxDrawScript dscr, afxNat id, afxNat siz, _afxDscrCmd *cmd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dscr, AFX_FCC_DSCR);
    cmd->id = id;
    cmd->siz = siz;
    AfxLinkageDeploy(&cmd->script, &dscr->commands);
}

_SGL void _AfxDrawScriptCmdEmployTechnique(afxDrawScript dscr, afxNat tecIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->state == AFX_DSCR_STATE_RECORDING);

    _afxDscrCmdEmployTec *cmd = AfxAllocate((void*)dscr->cmdAll, sizeof(*cmd), AfxSpawnHint());
    AfxAssert(cmd);
    cmd->tecIdx = tecIdx;
    _AfxDrawScriptCmdCommand(dscr, AFX_DSC_EMPLOY_TECHNIQUE, sizeof(cmd), &cmd->cmd);
}

_SGL void _AfxDrawScriptCmdNextPass(afxDrawScript dscr, afxBool useAuxScripts)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->state == AFX_DSCR_STATE_RECORDING);

    _afxDscrCmdNextPass *cmd = AfxAllocate((void*)dscr->cmdAll, sizeof(*cmd), AfxSpawnHint());
    AfxAssert(cmd);
    cmd->useAuxScripts = !!useAuxScripts;
    _AfxDrawScriptCmdCommand(dscr, AFX_DSC_NEXT_PASS, sizeof(cmd), &cmd->cmd);
}

_SGL void _AfxDrawScriptCmdSetRasterizerState(afxDrawScript dscr, afxPipelineRasterizerState const *state)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->state == AFX_DSCR_STATE_RECORDING);

    _afxDscrCmdSetRasterizerState *cmd = AfxAllocate((void*)dscr->cmdAll, sizeof(*cmd), AfxSpawnHint());
    AfxAssert(cmd);
    cmd->state = *state;

    _AfxDrawScriptCmdCommand(dscr, AFX_DSC_SET_RASTERIZATION_STATE, sizeof(cmd), &cmd->cmd);
}

_SGL void _AfxDrawScriptCmdSetDepthState(afxDrawScript dscr, afxPipelineDepthState const *state)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->state == AFX_DSCR_STATE_RECORDING);

    _afxDscrCmdSetDepthState *cmd = AfxAllocate((void*)dscr->cmdAll, sizeof(*cmd), AfxSpawnHint());
    AfxAssert(cmd);
    cmd->state = *state;

    _AfxDrawScriptCmdCommand(dscr, AFX_DSC_SET_DEPTH_STATE, sizeof(cmd), &cmd->cmd);
}

_SGL void _AfxDrawScriptCmdSetInputAssemblyState(afxDrawScript dscr, afxPipelineInputAssemblyState const *state)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->state == AFX_DSCR_STATE_RECORDING);

    _afxDscrCmdSetInputAssemblyState *cmd = AfxAllocate((void*)dscr->cmdAll, sizeof(*cmd), AfxSpawnHint());
    AfxAssert(cmd);
    cmd->state = *state;

    _AfxDrawScriptCmdCommand(dscr, AFX_DSC_SET_INPUT_ASSEMBLY_STATE, sizeof(cmd), &cmd->cmd);
}

_SGL void _AfxDrawScriptCmdBeginCombination(afxDrawScript dscr, afxRect const *area, afxNat layerCnt, afxNat rasterCnt, afxDrawTarget const rasters[], afxDrawTarget const *depth, afxDrawTarget const *stencil)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->state == AFX_DSCR_STATE_RECORDING);

    _afxDscrCmdBeginComb *cmd = AfxAllocate((void*)dscr->cmdAll, sizeof(*cmd), AfxSpawnHint());
    AfxAssert(cmd);
    AfxRectCopy(&cmd->area, area);
    cmd->layerCnt = layerCnt;
    cmd->rasterCnt = rasterCnt;

    for (afxNat i = 0; i < rasterCnt; i++)
        cmd->rasters[i] = rasters[i];

    if (depth)
        cmd->depth = *depth;
    else
        cmd->depth.surf = NIL;

    if (stencil)
        cmd->stencil = *stencil;
    else
        cmd->stencil.surf = NIL;

    _AfxDrawScriptCmdCommand(dscr, AFX_DSC_BEGIN_COMBINATION, sizeof(cmd), &cmd->cmd);
}

_SGL void _AfxDrawScriptCmdEndCombination(afxDrawScript dscr)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->state == AFX_DSCR_STATE_RECORDING);

    _afxDscrCmd *cmd = AfxAllocate((void*)dscr->cmdAll, sizeof(*cmd), AfxSpawnHint());
    AfxAssert(cmd);
    _AfxDrawScriptCmdCommand(dscr, AFX_DSC_END_COMBINATION, sizeof(cmd), cmd);
}

_SGL void _AfxDrawScriptCmdBeginOperation(afxDrawScript dscr, afxDrawOperation dop, afxNat tecIdx, afxCanvas canv, afxRect const *area, afxNat annexCnt, afxRenderPassAnnex const annexes[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->state == AFX_DSCR_STATE_RECORDING);

    _afxDscrCmdBeginOp *cmd = AfxAllocate((void*)dscr->cmdAll, sizeof(*cmd), AfxSpawnHint());
    AfxAssert(cmd);
    cmd->dop = dop;
    cmd->tecIdx = tecIdx;
    cmd->canv = canv;

    if (area)
        AfxRectCopy(&cmd->area, area);
    else
        AfxRectZero(&cmd->area);

    cmd->annexCnt = annexCnt;

    for (afxNat i = 0; i < annexCnt; i++)
        cmd->annexes[i] = annexes[i];

    _AfxDrawScriptCmdCommand(dscr, AFX_DSC_BEGIN_OPERATION, sizeof(cmd), &cmd->cmd);
}

_SGL void _AfxDrawScriptCmdEndOperation(afxDrawScript dscr)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->state == AFX_DSCR_STATE_RECORDING);

    _afxDscrCmd *cmd = AfxAllocate((void*)dscr->cmdAll, sizeof(*cmd), AfxSpawnHint());
    AfxAssert(cmd);
    _AfxDrawScriptCmdCommand(dscr, AFX_DSC_END_OPERATION, sizeof(cmd), cmd);
}

_SGL void _AfxDrawScriptCmdSetViewports(afxDrawScript dscr, afxNat32 first, afxNat32 cnt, afxViewport const vp[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->state == AFX_DSCR_STATE_RECORDING);

    _afxDscrCmdSetVp *cmd = AfxAllocate((void*)dscr->cmdAll, sizeof(*cmd), AfxSpawnHint());
    AfxAssert(cmd);
    cmd->first = first;
    cmd->cnt = cnt;

    for (afxNat i = 0; i < cnt; i++)
        cmd->vp[i] = vp[i];

    _AfxDrawScriptCmdCommand(dscr, AFX_DSC_SET_VIEWPORTS, sizeof(cmd), &cmd->cmd);
}

_SGL void _AfxDrawScriptCmdSetScissors(afxDrawScript dscr, afxNat32 first, afxNat32 cnt, afxRect const rect[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->state == AFX_DSCR_STATE_RECORDING);

    _afxDscrCmdSetScissor *cmd = AfxAllocate((void*)dscr->cmdAll, sizeof(*cmd), AfxSpawnHint());
    AfxAssert(cmd);
    cmd->first = first;
    cmd->cnt = cnt;

    for (afxNat i = 0; i < cnt; i++)
        cmd->rect[i] = rect[i];

    _AfxDrawScriptCmdCommand(dscr, AFX_DSC_SET_SCISSORS, sizeof(cmd), &cmd->cmd);
}

_SGL void _AfxDrawScriptCmdBindPipeline(afxDrawScript dscr, afxPipeline pip)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->state == AFX_DSCR_STATE_RECORDING);

    _afxDscrCmdBindPip *cmd = AfxAllocate((void*)dscr->cmdAll, sizeof(*cmd), AfxSpawnHint());
    AfxAssert(cmd);
    cmd->pip = pip;

    _AfxDrawScriptCmdCommand(dscr, AFX_DSC_BIND_PIPELINE, sizeof(cmd), &cmd->cmd);
}

_SGL void _AfxDrawScriptCmdBindIndexBuffer(afxDrawScript dscr, afxBuffer buf, afxNat32 offset, afxNat32 idxSiz)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->state == AFX_DSCR_STATE_RECORDING);

    _afxDscrCmdBindIbuf *cmd = AfxAllocate((void*)dscr->cmdAll, sizeof(*cmd), AfxSpawnHint());
    AfxAssert(cmd);
    cmd->buf = buf;
    cmd->offset = offset;
    cmd->idxSiz = idxSiz;

    _AfxDrawScriptCmdCommand(dscr, AFX_DSC_BIND_INDEX_BUFFER, sizeof(cmd), &cmd->cmd);
}

_SGL void _AfxDrawScriptCmdBindManagedIndexBuffer(afxDrawScript dscr, afxIndexBuffer ibuf, afxNat rgnIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->state == AFX_DSCR_STATE_RECORDING);

    _afxDscrCmdBindIbuf *cmd = AfxAllocate((void*)dscr->cmdAll, sizeof(*cmd), AfxSpawnHint());
    AfxAssert(cmd);
    cmd->buf = &ibuf->buf;

    afxNat idxCnt, idxSiz;
    AfxIndexBufferDescribeRegion(ibuf, rgnIdx, &cmd->offset, &idxCnt, &idxSiz);
    cmd->idxSiz = idxSiz;

    _AfxDrawScriptCmdCommand(dscr, AFX_DSC_BIND_INDEX_BUFFER, sizeof(cmd), &cmd->cmd);
}

_SGL void _AfxDrawScriptCmdBindBuffers(afxDrawScript dscr, afxNat set, afxNat first, afxNat cnt, afxBuffer buf[], afxNat offset[], afxNat range[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->state == AFX_DSCR_STATE_RECORDING);

    _afxDscrCmdBindBuffers *cmd = AfxAllocate((void*)dscr->cmdAll, sizeof(*cmd), AfxSpawnHint());
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
    _AfxDrawScriptCmdCommand(dscr, AFX_DSC_BIND_BUFFERS, sizeof(cmd), &cmd->cmd);
}

_SGL void _AfxDrawScriptCmdBindTextures(afxDrawScript dscr, afxNat set, afxNat first, afxNat cnt, afxSampler smp[], afxTexture tex[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->state == AFX_DSCR_STATE_RECORDING);

    _afxDscrCmdBindTextures *cmd = AfxAllocate((void*)dscr->cmdAll, sizeof(*cmd), AfxSpawnHint());
    AfxAssert(cmd);
    cmd->set = set;
    cmd->first = first;
    cmd->cnt = cnt;

    for (afxNat i = 0; i < cnt; i++)
    {
        cmd->smp[i] = smp[i];
        cmd->tex[i] = tex[i];
    }
    _AfxDrawScriptCmdCommand(dscr, AFX_DSC_BIND_TEXTURES, sizeof(cmd), &cmd->cmd);
}

_SGL void _AfxDrawScriptCmdBindVertexBuffers(afxDrawScript dscr, afxNat first, afxNat cnt, afxBuffer buf[], afxSize const offset[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->state == AFX_DSCR_STATE_RECORDING);

    _afxDscrCmdBindVbuf *cmd = AfxAllocate((void*)dscr->cmdAll, sizeof(*cmd), AfxSpawnHint());
    AfxAssert(cmd);
    cmd->first = first;
    cmd->cnt = cnt;

    for (afxNat i = 0; i < cnt; i++)
    {
        cmd->buf[i] = buf ? buf[i] : 0;
        cmd->offset[i] = offset ? offset[i] : 0;
        cmd->vtxArrSiz[i] = 0;
        cmd->vtxStride[i] = 0;
    }
    _AfxDrawScriptCmdCommand(dscr, AFX_DSC_BIND_VERTEX_BUFFERS, sizeof(cmd), &cmd->cmd);
}

_SGL void _AfxDrawScriptCmdBindVertexBuffers2(afxDrawScript dscr, afxNat first, afxNat cnt, afxBuffer buf[], afxSize const offset[], afxSize const size[], afxSize const stride[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->state == AFX_DSCR_STATE_RECORDING);

    _afxDscrCmdBindVbuf *cmd = AfxAllocate((void*)dscr->cmdAll, sizeof(*cmd), AfxSpawnHint());
    AfxAssert(cmd);
    cmd->first = first;
    cmd->cnt = cnt;

    for (afxNat i = 0; i < cnt; i++)
    {
        cmd->buf[i] = buf ? buf[i] : 0;
        cmd->offset[i] = offset ? offset[i] : 0;
        cmd->vtxArrSiz[i] = size ? size[i] : 0;
        cmd->vtxStride[i] = stride ? stride[i] : 0;
    }
    _AfxDrawScriptCmdCommand(dscr, AFX_DSC_BIND_VERTEX_BUFFERS, sizeof(cmd), &cmd->cmd);
}

_SGL void _AfxDrawScriptCmdBindManagedVertexBuffers(afxDrawScript dscr, afxNat first, afxNat cnt, afxVertexBuffer vbuf[], afxNat const baseVtx[], afxNat const vtxArr[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->state == AFX_DSCR_STATE_RECORDING);

    _afxDscrCmdBindVbuf *cmd = AfxAllocate((void*)dscr->cmdAll, sizeof(*cmd), AfxSpawnHint());
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
            cmd->vtxArrSiz[i] = 0;
        }
        else
        {
            cmd->buf[i] = 0;
            cmd->offset[i] = 0;
            cmd->vtxStride[i] = 0;
            cmd->vtxArrSiz[i] = 0;
        }
    }
    _AfxDrawScriptCmdCommand(dscr, AFX_DSC_BIND_VERTEX_BUFFERS, sizeof(cmd), &cmd->cmd);
}

_SGL void _AfxDrawScriptCmdDrawIndexed(afxDrawScript dscr, afxNat32 idxCnt, afxNat32 instCnt, afxNat32 firstIdx, afxNat32 vtxOff, afxNat32 firstInst)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->state == AFX_DSCR_STATE_RECORDING);

    _afxDscrCmdDrawIndexed *cmd = AfxAllocate((void*)dscr->cmdAll, sizeof(*cmd), AfxSpawnHint());
    AfxAssert(cmd);
    cmd->idxCnt = idxCnt;
    cmd->instCnt = instCnt;
    cmd->firstIdx = firstIdx;
    cmd->vtxOff = vtxOff;
    cmd->firstInst = firstInst;

    _AfxDrawScriptCmdCommand(dscr, AFX_DSC_DRAW_INDEXED, sizeof(cmd), &cmd->cmd);
}

_SGL void _AfxDrawScriptCmdDraw(afxDrawScript dscr, afxNat32 vtxCnt, afxNat32 instCnt, afxNat32 firstVtx, afxNat32 firstInst)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->state == AFX_DSCR_STATE_RECORDING);

    _afxDscrCmdDraw *cmd = AfxAllocate((void*)dscr->cmdAll, sizeof(*cmd), AfxSpawnHint());
    AfxAssert(cmd);
    cmd->vtxCnt = vtxCnt;
    cmd->instCnt = instCnt;
    cmd->firstVtx = firstVtx;
    cmd->firstInst = firstInst;

    _AfxDrawScriptCmdCommand(dscr, AFX_DSC_DRAW, sizeof(cmd), &cmd->cmd);
}

_SGL afxError _AfxDrawScriptReset(afxDrawScript dscr)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->state != AFX_DSCR_STATE_PENDING);

    while (1)
    {
        afxChain *commands = &dscr->commands;
        afxLinkage *first = AfxChainGetBegin(commands), *dummy = AfxChainGetAnchor(commands);

        if (first == dummy) break;
        else
        {
            _afxDscrCmd *cmd = AfxContainerOf(first, _afxDscrCmd, script);
            AfxLinkageDrop(&cmd->script);

            if (cmd != &dscr->cmdEnd)
            {
                //AfxDeallocate(all, cmd);
                AfxAssertObject(dscr->cmdAll, AFX_FCC_AREN);
                AfxArenaRecycle(dscr->cmdAll, cmd, cmd->siz);
            }
        }
    }

    dscr->lastUpdTime = AfxGetTimer();
    dscr->state = AFX_DSCR_STATE_INITIAL;
    return err;
}

_SGL afxError _AfxDrawScriptEnd(afxDrawScript dscr)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->state == AFX_DSCR_STATE_RECORDING);

    _AfxDrawScriptCmdCommand(dscr, AFX_DSC_END, sizeof(dscr->cmdEnd), &dscr->cmdEnd);

    dscr->state = AFX_DSCR_STATE_EXECUTABLE;
    dscr->lastUpdTime = AfxGetTimer();
    return err;
}

_SGL afxError _AfxDrawScriptBegin(afxDrawScript dscr, afxNat flags)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->state == AFX_DSCR_STATE_INITIAL || dscr->state == AFX_DSCR_STATE_EXECUTABLE);
    (void)flags;

    dscr->state = AFX_DSCR_STATE_RECORDING;
    return err;
}

_SGL afxDrawScriptState _AfxDrawScriptGetState(afxDrawScript dscr)
{
    return dscr->state;
}

_SGL afxDscrImpl _AfxDscrImpl =
{
    _AfxDrawScriptGetState,
    _AfxDrawScriptBegin,
    _AfxDrawScriptEnd,
    _AfxDrawScriptReset,

    _AfxDrawScriptCmdBeginCombination,
    _AfxDrawScriptCmdBeginOperation,
    _AfxDrawScriptCmdEmployTechnique,
    _AfxDrawScriptCmdNextPass,
    _AfxDrawScriptCmdEndOperation,
    _AfxDrawScriptCmdEndCombination,

    _AfxDrawScriptCmdBindPipeline,

    _AfxDrawScriptCmdSetInputAssemblyState,
    _AfxDrawScriptCmdSetViewports,
    _AfxDrawScriptCmdSetRasterizerState,
    _AfxDrawScriptCmdSetDepthState,
    _AfxDrawScriptCmdSetScissors,

    _AfxDrawScriptCmdBindBuffers,
    _AfxDrawScriptCmdBindVertexBuffers,
    _AfxDrawScriptCmdBindVertexBuffers2,
    _AfxDrawScriptCmdBindManagedVertexBuffers,
    _AfxDrawScriptCmdBindIndexBuffer,
    _AfxDrawScriptCmdBindManagedIndexBuffer,
    _AfxDrawScriptCmdBindTextures,

    _AfxDrawScriptCmdDraw,
    _AfxDrawScriptCmdDrawIndexed,
};

_SGL afxBool _SglDscrEventHandler(afxObject *obj, afxEvent *ev)
{
    afxError err = AFX_ERR_NONE;
    afxDrawScript dscr = (void*)obj;
    AfxAssertObject(dscr, AFX_FCC_DSCR);
    (void)ev;
    return FALSE;
}

_SGL afxBool _SglDscrEventFilter(afxObject *obj, afxObject *watched, afxEvent *ev)
{
    afxError err = AFX_ERR_NONE;
    afxDrawScript dscr = (void*)obj;
    AfxAssertObject(dscr, AFX_FCC_DSCR);
    (void)watched;
    (void)ev;
    return FALSE;
}

_SGL afxError _AfxDscrDtor(afxDrawScript dscr)
{
    AfxEntry("dscr=%p", dscr);
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dscr, AFX_FCC_DSCR);

    while (1)
    {
        if (dscr->state != AFX_DSCR_STATE_PENDING) break;
        else
        {
            // AfxLinkageDrop(&dscr->queue); // we can't do it here. We need wait for draw context to liberate it.

            afxDrawContext dctx = AfxObjectGetProvider(&dscr->obj);
            AfxAssertObject(dctx, AFX_FCC_DCTX);
            //AfxDrawContextProcess(dctx); // process until draw context ends its works and unlock this script.
            AfxYield();
        }
    }

    AfxDrawScriptReset(dscr);
    AfxObjectRelease((void*)dscr->cmdAll);

    return err;
}

_SGL afxError _AfxDscrCtor(void *cache, afxNat idx, afxDrawScript dscr, afxDrawInput *dins)
{
    AfxEntry("dscr=%p", dscr);
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dscr, AFX_FCC_DSCR);

    afxDrawInput din = dins[idx];

    dscr->cmdAll = din->cmdAll;

    if (!(AfxObjectReacquire((void*)dscr->cmdAll, &dscr->obj, NIL, NIL, NIL))) AfxThrowError();
    else
    {
        dscr->state = AFX_DSCR_STATE_INITIAL;
        AfxChainDeploy(&dscr->commands, dscr);

        dscr->lastUpdTime = AfxGetTimer();
    }
    return err;
}

_SGL afxClassSpecification const _AfxDscrClassSpec;

afxClassSpecification const _AfxDscrClassSpec =
{
    AFX_FCC_DSCR,
    NIL,
    0,
    sizeof(AFX_OBJECT(afxDrawScript)),
    NIL,
    (void*)_AfxDscrCtor,
    (void*)_AfxDscrDtor,
    .event = _SglDscrEventHandler,
    .eventFilter = _SglDscrEventFilter,
    "afxDrawScript",
    &_AfxDscrImpl
};
