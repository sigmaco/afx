#include "sgl.h"
#include "afx/afxQwadro.h"

typedef struct
{
    afxDrawInput din;
} _afxDscrCtorArgs;

// DRAW CODE API ///////////////////////////////////////////////////////////////

_SGL void _AfxDrawScriptCmdCommand(afxDrawScript dscr, afxNat id, _afxDscrCmd *cmd)
{
    afxError err = NIL;
    AfxAssertObject(dscr, AFX_FCC_DSCR);
    cmd->id = id;
    AfxLinkageDeploy(&cmd->script, &dscr->commands);
}

_SGL void _AfxDrawScriptCmdCopyTexture(afxDrawScript dscr, afxTexture dst, afxTexture src, afxNat rgnCnt, afxTextureRegion const rgn[])
{
    afxError err = NIL;
    AfxAssert(dscr->state == AFX_DSCR_STATE_RECORDING);

    _afxDscrCmdCopyTex *cmd = AfxAllocate(dscr->cachedCmdAll, sizeof(*cmd), AfxSpawnHint());
    AfxAssert(cmd);
    cmd->dst = dst;
    cmd->src = src;
    cmd->rgnCnt = rgnCnt;

    for (afxNat i = 0; i < rgnCnt; i++)
        cmd->rgn[i] = rgn[i];

    _AfxDrawScriptCmdCommand(dscr, AFX_DSC_COPY_TEXTURE, &cmd->cmd);
}

_SGL void _AfxDrawScriptCmdSetRasterizerState(afxDrawScript dscr, afxPipelineRasterizerState const *state)
{
    afxError err = NIL;
    AfxAssert(dscr->state == AFX_DSCR_STATE_RECORDING);

    _afxDscrCmdSetRasterizerState *cmd = AfxAllocate(dscr->cachedCmdAll, sizeof(*cmd), AfxSpawnHint());
    AfxAssert(cmd);
    cmd->state = *state;

    _AfxDrawScriptCmdCommand(dscr, AFX_DSC_SET_RASTERIZATION_STATE, &cmd->cmd);
}

_SGL void _AfxDrawScriptCmdSetDepthState(afxDrawScript dscr, afxPipelineDepthState const *state)
{
    afxError err = NIL;
    AfxAssert(dscr->state == AFX_DSCR_STATE_RECORDING);

    _afxDscrCmdSetDepthState *cmd = AfxAllocate(dscr->cachedCmdAll, sizeof(*cmd), AfxSpawnHint());
    AfxAssert(cmd);
    cmd->state = *state;

    _AfxDrawScriptCmdCommand(dscr, AFX_DSC_SET_DEPTH_STENCIL_STATE, &cmd->cmd);
}

_SGL void _AfxDrawScriptCmdSetInputAssemblyState(afxDrawScript dscr, afxPipelineInputAssemblyState const *state)
{
    afxError err = NIL;
    AfxAssert(dscr->state == AFX_DSCR_STATE_RECORDING);

    _afxDscrCmdSetInputAssemblyState *cmd = AfxAllocate(dscr->cachedCmdAll, sizeof(*cmd), AfxSpawnHint());
    AfxAssert(cmd);
    cmd->state = *state;

    _AfxDrawScriptCmdCommand(dscr, AFX_DSC_SET_INPUT_ASSEMBLY_STATE, &cmd->cmd);
}

_SGL void _AfxDrawScriptCmdBeginRendering(afxDrawScript dscr, afxRect const *area, afxNat layerCnt, afxNat rasterCnt, afxDrawTarget const rasters[], afxDrawTarget const *depth, afxDrawTarget const *stencil)
{
    afxError err = NIL;
    AfxAssert(dscr->state == AFX_DSCR_STATE_RECORDING);

    _afxDscrCmdBeginRendering *cmd = AfxAllocate(dscr->cachedCmdAll, sizeof(*cmd), AfxSpawnHint());
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

    _AfxDrawScriptCmdCommand(dscr, AFX_DSC_BEGIN_RENDERING, &cmd->cmd);
}

_SGL void _AfxDrawScriptCmdEndRendering(afxDrawScript dscr)
{
    afxError err = NIL;
    AfxAssert(dscr->state == AFX_DSCR_STATE_RECORDING);

    _afxDscrCmdEndRendering *cmd = AfxAllocate(dscr->cachedCmdAll, sizeof(*cmd), AfxSpawnHint());
    AfxAssert(cmd);
    _AfxDrawScriptCmdCommand(dscr, AFX_DSC_END_RENDERING, &cmd->cmd);
}

_SGL void _AfxDrawScriptCmdBeginRenderPass(afxDrawScript dscr, afxCanvas canv, afxRect const *area, afxNat annexCnt, afxRenderPassAnnex const annexes[])
{
    afxError err = NIL;
    AfxAssert(dscr->state == AFX_DSCR_STATE_RECORDING);

    _afxDscrCmdBeginRenderPass *cmd = AfxAllocate(dscr->cachedCmdAll, sizeof(*cmd), AfxSpawnHint());
    AfxAssert(cmd);
    cmd->canv = canv;

    if (area)
        AfxRectCopy(&cmd->area, area);
    else
        AfxRectZero(&cmd->area);

    cmd->annexCnt = annexCnt;

    for (afxNat i = 0; i < annexCnt; i++)
        cmd->annexes[i] = annexes[i];

    _AfxDrawScriptCmdCommand(dscr, AFX_DSC_BEGIN_RENDER_PASS, &cmd->cmd);
}

_SGL void _AfxDrawScriptCmdEndRenderPass(afxDrawScript dscr)
{
    afxError err = NIL;
    AfxAssert(dscr->state == AFX_DSCR_STATE_RECORDING);

    _afxDscrCmdEndRenderPass *cmd = AfxAllocate(dscr->cachedCmdAll, sizeof(*cmd), AfxSpawnHint());
    AfxAssert(cmd);
    _AfxDrawScriptCmdCommand(dscr, AFX_DSC_END_RENDER_PASS, &cmd->cmd);
}

_SGL void _AfxDrawScriptCmdSetViewport(afxDrawScript dscr, afxNat32 first, afxNat32 cnt, afxViewport const vp[])
{
    afxError err = NIL;
    AfxAssert(dscr->state == AFX_DSCR_STATE_RECORDING);

    _afxDscrCmdSetVp *cmd = AfxAllocate(dscr->cachedCmdAll, sizeof(*cmd), AfxSpawnHint());
    AfxAssert(cmd);
    cmd->first = first;
    cmd->cnt = cnt;

    for (afxNat i = 0; i < cnt; i++)
        cmd->vp[i] = vp[i];

    _AfxDrawScriptCmdCommand(dscr, AFX_DSC_SET_VIEWPORT, &cmd->cmd);
}

_SGL void _AfxDrawScriptCmdSetScissor(afxDrawScript dscr, afxNat32 first, afxNat32 cnt, afxRect const rect[])
{
    afxError err = NIL;
    AfxAssert(dscr->state == AFX_DSCR_STATE_RECORDING);

    _afxDscrCmdSetScissor *cmd = AfxAllocate(dscr->cachedCmdAll, sizeof(*cmd), AfxSpawnHint());
    AfxAssert(cmd);
    cmd->first = first;
    cmd->cnt = cnt;

    for (afxNat i = 0; i < cnt; i++)
        cmd->rect[i] = rect[i];

    _AfxDrawScriptCmdCommand(dscr, AFX_DSC_SET_SCISSOR, &cmd->cmd);
}

_SGL void _AfxDrawScriptCmdBindPipeline(afxDrawScript dscr, afxPipeline pip)
{
    afxError err = NIL;
    AfxAssert(dscr->state == AFX_DSCR_STATE_RECORDING);

    _afxDscrCmdBindPip *cmd = AfxAllocate(dscr->cachedCmdAll, sizeof(*cmd), AfxSpawnHint());
    AfxAssert(cmd);
    cmd->pip = pip;

    _AfxDrawScriptCmdCommand(dscr, AFX_DSC_BIND_PIPELINE, &cmd->cmd);
}

_SGL void _AfxDrawScriptCmdBindIndexBuffer(afxDrawScript dscr, afxIndexBuffer ibuf, afxNat32 baseIdx)
{
    afxError err = NIL;
    AfxAssert(dscr->state == AFX_DSCR_STATE_RECORDING);

    _afxDscrCmdBindIbuf *cmd = AfxAllocate(dscr->cachedCmdAll, sizeof(*cmd), AfxSpawnHint());
    AfxAssert(cmd);
    cmd->ibuf = ibuf;
    cmd->baseIdx = baseIdx;

    _AfxDrawScriptCmdCommand(dscr, AFX_DSC_BIND_INDEX_BUFFER, &cmd->cmd);
}

_SGL void _AfxDrawScriptCmdBindLegos(afxDrawScript dscr, afxNat32 first, afxNat32 cnt, afxLego lego[])
{
    afxError err = NIL;
    AfxAssert(dscr->state == AFX_DSCR_STATE_RECORDING);

    _afxDscrCmdBindLego *cmd = AfxAllocate(dscr->cachedCmdAll, sizeof(*cmd), AfxSpawnHint());
    AfxAssert(cmd);
    cmd->first = first;
    cmd->cnt = cnt;

    for (afxNat i = 0; i < cnt; i++)
        cmd->lego[i] = lego[i];

    _AfxDrawScriptCmdCommand(dscr, AFX_DSC_BIND_PIPELINE_PLUGS, &cmd->cmd);
}

_SGL void _AfxDrawScriptCmdBindWildVertexBuffers(afxDrawScript dscr, afxNat first, afxNat cnt, afxBuffer buf[], afxSize const offset[], afxSize const size[], afxSize const stride[])
{
    afxError err = NIL;
    AfxAssert(dscr->state == AFX_DSCR_STATE_RECORDING);

    _afxDscrCmdBindVbuf *cmd = AfxAllocate(dscr->cachedCmdAll, sizeof(*cmd), AfxSpawnHint());
    AfxAssert(cmd);
    cmd->first = first;
    cmd->cnt = cnt;

    for (afxNat i = 0; i < cnt; i++)
    {
        cmd->buf[i] = buf ? buf[i] : 0;
        cmd->offset[i] = offset ? offset[i] : 0;
        cmd->vtxArrSiz[i] = size ? size[i] : 0;
        cmd->vtxStride[i] = stride ? stride[i] : 0;
        cmd->wild = TRUE;
    }
    _AfxDrawScriptCmdCommand(dscr, AFX_DSC_BIND_WILD_VERTEX_BUFFERS, &cmd->cmd);
}

_SGL void _AfxDrawScriptCmdBindVertexBuffers(afxDrawScript dscr, afxNat first, afxNat cnt, afxVertexBuffer vbuf[], afxNat const baseVtx[], afxNat const vtxArr[])
{
    afxError err = NIL;
    AfxAssert(dscr->state == AFX_DSCR_STATE_RECORDING);

    _afxDscrCmdBindVbuf *cmd = AfxAllocate(dscr->cachedCmdAll, sizeof(*cmd), AfxSpawnHint());
    AfxAssert(cmd);
    cmd->first = first;
    cmd->cnt = cnt;

    for (afxNat i = 0; i < cnt; i++)
    {
        cmd->vbuf[i] = vbuf ? vbuf[i] : 0;
        cmd->baseVtx[i] = baseVtx ? baseVtx[i] : 0;
        cmd->vtxArr[i] = vtxArr ? vtxArr[i] : 0;
        cmd->wild = FALSE;
    }
    _AfxDrawScriptCmdCommand(dscr, AFX_DSC_BIND_VERTEX_BUFFERS, &cmd->cmd);
}

_SGL void _AfxDrawScriptCmdDrawIndexed(afxDrawScript dscr, afxNat32 idxCnt, afxNat32 instCnt, afxNat32 firstIdx, afxNat32 vtxOff, afxNat32 firstInst)
{
    afxError err = NIL;
    AfxAssert(dscr->state == AFX_DSCR_STATE_RECORDING);

    _afxDscrCmdDrawIndexed *cmd = AfxAllocate(dscr->cachedCmdAll, sizeof(*cmd), AfxSpawnHint());
    AfxAssert(cmd);
    cmd->idxCnt = idxCnt;
    cmd->instCnt = instCnt;
    cmd->firstIdx = firstIdx;
    cmd->vtxOff = vtxOff;
    cmd->firstInst = firstInst;

    _AfxDrawScriptCmdCommand(dscr, AFX_DSC_DRAW_INDEXED, &cmd->cmd);
}

_SGL void _AfxDrawScriptCmdDraw(afxDrawScript dscr, afxNat32 vtxCnt, afxNat32 instCnt, afxNat32 firstVtx, afxNat32 firstInst)
{
    afxError err = NIL;
    AfxAssert(dscr->state == AFX_DSCR_STATE_RECORDING);

    _afxDscrCmdDraw *cmd = AfxAllocate(dscr->cachedCmdAll, sizeof(*cmd), AfxSpawnHint());
    AfxAssert(cmd);
    cmd->vtxCnt = vtxCnt;
    cmd->instCnt = instCnt;
    cmd->firstVtx = firstVtx;
    cmd->firstInst = firstInst;

    _AfxDrawScriptCmdCommand(dscr, AFX_DSC_DRAW, &cmd->cmd);
}

_SGL afxError _AfxDrawScriptReset(afxDrawScript dscr)
{
    afxError err = NIL;
    AfxAssert(dscr->state != AFX_DSCR_STATE_PENDING);

    while (1)
    {
        afxChain *commands = &dscr->commands;
        afxLinkage *first = AfxChainGetFirstLinkage(commands), *dummy = AfxChainGetDummyLinkage(commands);

        if (first == dummy) break;
        else
        {
            _afxDscrCmd *cmd = AfxContainerOf(first, _afxDscrCmd, script);
            AfxLinkageDrop(&cmd->script);

            if (cmd != &dscr->cmdBegin && cmd != &dscr->cmdEnd)
            {
                AfxDeallocate(NIL, cmd);
            }
        }
    }

    dscr->lastUpdTime = AfxGetTimer();
    dscr->state = AFX_DSCR_STATE_INITIAL;
    return err;
}

_SGL afxError _AfxDrawScriptEnd(afxDrawScript dscr)
{
    afxError err = NIL;
    AfxAssert(dscr->state == AFX_DSCR_STATE_RECORDING);

    _AfxDrawScriptCmdCommand(dscr, AFX_DSC_END, &dscr->cmdEnd);

    dscr->state = AFX_DSCR_STATE_EXECUTABLE;
    dscr->lastUpdTime = AfxGetTimer();
    return err;
}

_SGL afxError _AfxDrawScriptBegin(afxDrawScript dscr, afxNat flags)
{
    afxError err = NIL;
    AfxAssert(dscr->state == AFX_DSCR_STATE_INITIAL || dscr->state == AFX_DSCR_STATE_EXECUTABLE);
    (void)flags;

    dscr->state = AFX_DSCR_STATE_RECORDING;
    _AfxDrawScriptCmdCommand(dscr, AFX_DSC_BEGIN, &dscr->cmdBegin);
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

    _AfxDrawScriptCmdDraw,
    _AfxDrawScriptCmdDrawIndexed,
    _AfxDrawScriptCmdBindLegos,
    _AfxDrawScriptCmdBindVertexBuffers,
    _AfxDrawScriptCmdBindWildVertexBuffers,
    _AfxDrawScriptCmdBindIndexBuffer,
    _AfxDrawScriptCmdBindPipeline,
    _AfxDrawScriptCmdCopyTexture,
    _AfxDrawScriptCmdSetScissor,
    _AfxDrawScriptCmdSetViewport,
    _AfxDrawScriptCmdBeginRenderPass,
    _AfxDrawScriptCmdEndRenderPass,
    _AfxDrawScriptCmdBeginRendering,
    _AfxDrawScriptCmdEndRendering,
    _AfxDrawScriptCmdSetRasterizerState,
    _AfxDrawScriptCmdSetDepthState,
    _AfxDrawScriptCmdSetInputAssemblyState
};

_SGL afxDrawScript _AfxDrawInputAcquireScript(afxDrawInput din, afxBool recycle)
{
    afxError err = NIL;
    AfxAssertObject(din, AFX_FCC_DIN);
    afxDrawScript dscr = NIL;

    afxDrawContext dctx = AfxObjectGetProvider(&din->obj);
    AfxAssertObject(dctx, AFX_FCC_DCTX);

    if (recycle)
    {
        AfxChainForEveryLinkage(&dctx->dscrClass.instances, AFX_OBJECT(afxDrawScript), obj.cls, dscr)
        {
            AfxAssertObject(dscr, AFX_FCC_DSCR);

            if (dscr->state != AFX_DSCR_STATE_PENDING && dscr->state != AFX_DSCR_STATE_RECORDING)
            {
                if (AfxDrawScriptReset(dscr)) AfxThrowError();
                else
                {
                    return dscr;
                }
            }
        }
    }

    AfxEntry("din=%p", din);

    if (err) dscr = NIL;
    else
    {
        _afxDscrCtorArgs args =
        {
            din
        };

        if (!(dscr = AfxObjectAcquire(AfxDrawContextGetScriptClass(dctx), &args, AfxSpawnHint())))
            AfxThrowError();
    }
    return dscr;
}

_SGL afxError _AfxDscrCtor(afxDrawScript dscr, _afxDscrCtorArgs *args)
{
    AfxEntry("dscr=%p", dscr);
    afxError err = NIL;
    AfxAssertObject(dscr, AFX_FCC_DSCR);

    dscr->cachedCmdAll = args->din->cmdAll;
    dscr->state = AFX_DSCR_STATE_INITIAL;
    AfxChainDeploy(&dscr->commands, dscr);

    dscr->lastUpdTime = AfxGetTimer();

    return err;
}

_SGL afxError _AfxDscrDtor(afxDrawScript dscr)
{
    AfxEntry("dscr=%p", dscr);
    afxError err = NIL;
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
    //AfxObjectRelease(&(dscr->cmds->obj));

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
    "Draw Script",
    &_AfxDscrImpl
};
