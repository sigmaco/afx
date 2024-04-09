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
 *                       (c) 2017 SIGMA, Engineering In Technology
 *                             <https://sigmaco.org/qwadro/>
 */

#include "sgl.h"
#include "qwadro/afxQwadro.h"
#include "qwadro/draw/pipe/afxDrawOps.h"

_SGL afxCmdId _SglEncodeCmdCommand(afxDrawStream diob, afxNat id, afxNat siz, _sglCmd *cmd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &diob, afxFcc_DIOB);
    cmd->id = id;
    AfxAssertRange(SGL_CMD_TOTAL, cmd->id, 1);

    if (err)
    {
        int a = 0;
    }

    cmd->siz = siz;
    return AfxPushLinkage(&cmd->script, &diob->commands);
}

///////////////////////////////////////////

_SGL afxCmdId FlushNextCachedPipelineBinding(afxDrawStream diob, afxNat segment)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(diob->base.state == afxDrawStreamState_RECORDING);
    afxPipeline curr = diob->levelCaches[segment].pip.curr;
    afxPipeline next = diob->levelCaches[segment].pip.next;

    if ((curr != next) && next)
    {
        diob->levelCaches[segment].pip.curr = next;
        diob->levelCaches[segment].pip.next = NIL;

        _sglCmdPipeline *cmd = AfxRequestArenaUnit(&diob->base.cmdArena, sizeof(*cmd));
        AfxAssert(cmd);
        cmd->segment = segment;
        cmd->pip = next;
        return _SglEncodeCmdCommand(diob, (offsetof(afxCmd, BindPipeline) / sizeof(void*)), sizeof(cmd), &cmd->cmd);
    }
    return 0;
}

_SGL afxCmdId FlushCachedDrawStuff(afxDrawStream diob, afxNat segment)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(diob->base.state == afxDrawStreamState_RECORDING);

    return FlushNextCachedPipelineBinding(diob, segment);
}

_SGL afxCmdId FlushCachedDrawIndexedStuff(afxDrawStream diob, afxNat segment)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(diob->base.state == afxDrawStreamState_RECORDING);
    return FlushCachedDrawStuff(diob, segment);
}

_SGL afxCmdId FlushCachedStuff(afxDrawStream diob, afxNat segment)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(diob->base.state == afxDrawStreamState_RECORDING);
    return FlushCachedDrawIndexedStuff(diob, segment);
}

_SGL afxCmdId _SglEncodeCmdBindPipeline(afxDrawStream diob, afxNat segment, afxPipeline pip)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(diob->base.state == afxDrawStreamState_RECORDING);
#if 0
    afxPipeline next = diob->levelCaches[line].pip.next;

    if (next != pip)
    {
        afxCmdId cmdId = FlushNextCachedPipelineBinding(diob, line);
        diob->levelCaches[line].pip.next = pip;
        return cmdId;
    }
#else

    _sglCmdPipeline *cmd = AfxRequestArenaUnit(&diob->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->segment = segment;
    cmd->pip = pip;
    return _SglEncodeCmdCommand(diob, (offsetof(afxCmd, BindPipeline) / sizeof(void*)), sizeof(cmd), &cmd->cmd);
#endif
    return 0;
}

_SGL afxCmdId _SglEncodeCmdBindBuffers(afxDrawStream diob, afxNat set, afxNat first, afxNat cnt, afxBuffer buf[], afxNat offset[], afxNat range[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(diob->base.state == afxDrawStreamState_RECORDING);

    _sglCmdBindBuffers *cmd = AfxRequestArenaUnit(&diob->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->set = set;
    cmd->first = first;
    cmd->cnt = cnt;

    for (afxNat i = 0; i < cnt; i++)
    {
        cmd->buf[i] = buf ? buf[i] : 0;
        cmd->offset[i] = offset ? offset[i] : 0;
        cmd->range[i] = range ? range[i] : 0;
    }
    return _SglEncodeCmdCommand(diob, offsetof(afxCmd, BindBuffers) / sizeof(void*), sizeof(cmd), &cmd->cmd);
}

_SGL afxCmdId _SglEncodeCmdBindRasters(afxDrawStream diob, afxNat set, afxNat first, afxNat cnt, afxSampler smp[], afxRaster tex[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(diob->base.state == afxDrawStreamState_RECORDING);

    _sglCmdBindRasters *cmd = AfxRequestArenaUnit(&diob->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->set = set;
    cmd->first = first;
    cmd->cnt = cnt;

    for (afxNat i = 0; i < cnt; i++)
    {
        cmd->smp[i] = smp ? smp[i] : 0;
        cmd->tex[i] = tex ? tex[i] : 0;
    }

    return _SglEncodeCmdCommand(diob, offsetof(afxCmd, BindRasters) / sizeof(void*), sizeof(cmd), &cmd->cmd);
}

_SGL afxCmdId _SglEncodeCmdExecuteCommands(afxDrawStream diob, afxNat cnt, afxDrawStream subsets[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(diob->base.state == afxDrawStreamState_RECORDING);

    _sglCmdExecCmds *cmd = AfxRequestArenaUnit(&diob->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->cnt = cnt;

    for (afxNat i = 0; i < cnt; i++)
        cmd->subsets[i] = subsets[i];

    return _SglEncodeCmdCommand(diob, (offsetof(afxCmd, ExecuteCommands) / sizeof(void*)), sizeof(cmd), &cmd->cmd);
}

// Draw

_SGL afxCmdId _SglEncodeCmdDraw(afxDrawStream diob, afxNat32 vtxCnt, afxNat32 instCnt, afxNat32 firstVtx, afxNat32 firstInst)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(diob->base.state == afxDrawStreamState_RECORDING);

    FlushCachedDrawStuff(diob, 0);

    _sglCmdDraw *cmd = AfxRequestArenaUnit(&diob->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->vtxCnt = vtxCnt;
    cmd->instCnt = instCnt;
    cmd->firstVtx = firstVtx;
    cmd->firstInst = firstInst;

    return _SglEncodeCmdCommand(diob, (offsetof(afxCmd, Draw) / sizeof(void*)), sizeof(cmd), &cmd->cmd);
}

_SGL afxCmdId _SglEncodeCmdDrawIndexed(afxDrawStream diob, afxNat32 idxCnt, afxNat32 instCnt, afxNat32 firstIdx, afxNat32 vtxOff, afxNat32 firstInst)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(diob->base.state == afxDrawStreamState_RECORDING);

    FlushCachedDrawIndexedStuff(diob, 0);

    _sglCmdDrawIndexed *cmd = AfxRequestArenaUnit(&diob->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->idxCnt = idxCnt;
    cmd->instCnt = instCnt;
    cmd->firstIdx = firstIdx;
    cmd->vtxOff = vtxOff;
    cmd->firstInst = firstInst;

    return _SglEncodeCmdCommand(diob, (offsetof(afxCmd, DrawIndexed) / sizeof(void*)), sizeof(cmd), &cmd->cmd);
}

_SGL afxError _SglDiobEndCb(afxDrawStream diob)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &diob, afxFcc_DIOB);
    FlushCachedStuff(diob, 0);
    return err;
}

_SGL afxCmd _SglEncodeCmdVmt =
{
    .BindPipeline = _SglEncodeCmdBindPipeline,
    .Draw = _SglEncodeCmdDraw,
    .DrawIndexed = _SglEncodeCmdDrawIndexed,
    .ExecuteCommands = _SglEncodeCmdExecuteCommands,
    .BindBuffers = _SglEncodeCmdBindBuffers,    
    .BindRasters = _SglEncodeCmdBindRasters,
    //.Rasterization = _SglEncodeCmdRasterizationVmt
};
