/*
 *             :::::::::::     :::     :::::::::   ::::::::      :::
 *                 :+:       :+: :+:   :+:    :+: :+:    :+:   :+: :+:
 *                 +:+      +:+   +:+  +:+    +:+ +:+         +:+   +:+
 *                 +#+     +#++:++#++: +#++:++#:  :#:        +#++:++#++:
 *                 +#+     +#+     +#+ +#+    +#+ +#+   +#+# +#+     +#+
 *                 #+#     #+#     #+# #+#    #+# #+#    #+# #+#     #+#
 *                 ###     ###     ### ###    ###  ########  ###     ###
 *
 *                  Q W A D R O   E X E C U T I O N   E C O S Y S T E M
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

#include "sgl.h"
#include "qwadro/afxQwadro.h"
#include "qwadro/draw/pipe/afxDrawOps.h"

_SGL afxCmdId _SglEncodeCmdCommand(avxCmdb cmdb, afxNat id, afxNat siz, _sglCmd *cmd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cmdb, afxFcc_CMDB);
    cmd->id = id;
    AfxAssertRange(SGL_CMD_TOTAL, cmd->id, 1);

    if (err)
    {
        int a = 0;
    }

    cmd->siz = siz;
    return AfxPushLinkage(&cmd->script, &cmdb->commands);
}

///////////////////////////////////////////

_SGL afxCmdId FlushNextCachedPipelineBinding(avxCmdb cmdb, afxNat segment)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cmdb->base.state == avxCmdbState_RECORDING);
    afxPipeline curr = cmdb->levelCaches[segment].pip.curr;
    afxPipeline next = cmdb->levelCaches[segment].pip.next;

    if ((curr != next) && next)
    {
        cmdb->levelCaches[segment].pip.curr = next;
        cmdb->levelCaches[segment].pip.next = NIL;

        _sglCmdPipeline *cmd = AfxRequestArenaUnit(&cmdb->base.cmdArena, sizeof(*cmd));
        AfxAssert(cmd);
        cmd->segment = segment;
        cmd->pip = next;
        return _SglEncodeCmdCommand(cmdb, (offsetof(afxCmd, BindPipeline) / sizeof(void*)), sizeof(cmd), &cmd->cmd);
    }
    return 0;
}

_SGL afxCmdId FlushCachedDrawStuff(avxCmdb cmdb, afxNat segment)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cmdb->base.state == avxCmdbState_RECORDING);

    return FlushNextCachedPipelineBinding(cmdb, segment);
}

_SGL afxCmdId FlushCachedDrawIndexedStuff(avxCmdb cmdb, afxNat segment)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cmdb->base.state == avxCmdbState_RECORDING);
    return FlushCachedDrawStuff(cmdb, segment);
}

_SGL afxCmdId FlushCachedStuff(avxCmdb cmdb, afxNat segment)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cmdb->base.state == avxCmdbState_RECORDING);
    return FlushCachedDrawIndexedStuff(cmdb, segment);
}

//

_SGL void _DecodeCmdBindPipeline(sglDpu* dpu, _sglCmdPipeline *cmd)
{
    _DpuBindPipeline(dpu, cmd->pip, cmd->dynamics);
}

_SGL void _DecodeCmdBindRasterizer(sglDpu* dpu, _sglCmdRasterizer *cmd)
{
    _DpuBindRasterizer(dpu, cmd->razr, cmd->dynamics);
}

_SGL afxCmdId _EncodeCmdBindPipeline(avxCmdb cmdb, afxNat segment, afxPipeline pip, afxFlags dynamics)
{
    afxError err = AFX_ERR_NONE;
#if 0
    afxPipeline next = cmdb->levelCaches[line].pip.next;

    if (next != pip)
    {
        afxCmdId cmdId = FlushNextCachedPipelineBinding(cmdb, line);
        cmdb->levelCaches[line].pip.next = pip;
        return cmdId;
    }
#else

    _sglCmdPipeline *cmd = AfxRequestArenaUnit(&cmdb->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->segment = segment;
    cmd->pip = pip;
    cmd->dynamics = dynamics;
    return _SglEncodeCmdCommand(cmdb, (offsetof(afxCmd, BindPipeline) / sizeof(void*)), sizeof(cmd), &cmd->cmd);
#endif
    return 0;
}

_SGL afxCmdId _EncodeCmdBindRasterizer(avxCmdb cmdb, afxRasterizer razr, afxFlags dynamics)
{
    afxError err = AFX_ERR_NONE;
#if 0
    afxPipeline next = cmdb->levelCaches[line].pip.next;

    if (next != pip)
    {
        afxCmdId cmdId = FlushNextCachedPipelineBinding(cmdb, line);
        cmdb->levelCaches[line].pip.next = pip;
        return cmdId;
    }
#else

    _sglCmdRasterizer *cmd = AfxRequestArenaUnit(&cmdb->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->razr = razr;
    cmd->dynamics = dynamics;
    return _SglEncodeCmdCommand(cmdb, (offsetof(afxCmd, BindRasterizer) / sizeof(void*)), sizeof(cmd), &cmd->cmd);
#endif
    return 0;
}
//

_SGL void _DecodeCmdBindBuffers(sglDpu* dpu, _sglCmdBindBuffers const *cmd)
{
    _DpuBindBuffers(dpu, cmd->set, cmd->first, cmd->cnt, (afxBuffer*)cmd->buf, cmd->offset, cmd->range);
}

_SGL afxCmdId _EncodeCmdBindBuffers(avxCmdb cmdb, afxNat set, afxNat first, afxNat cnt, afxBuffer buf[], afxNat offset[], afxNat range[])
{
    afxError err = AFX_ERR_NONE;
    _sglCmdBindBuffers *cmd = AfxRequestArenaUnit(&cmdb->base.cmdArena, sizeof(*cmd));
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
    return _SglEncodeCmdCommand(cmdb, offsetof(afxCmd, BindBuffers) / sizeof(void*), sizeof(cmd), &cmd->cmd);
}

//

_SGL void _DecodeCmdBindRasters(sglDpu* dpu, _sglCmdBindRasters const *cmd)
{
    _DpuBindRasters(dpu, cmd->set, cmd->first, cmd->cnt, (afxSampler*)cmd->smp, (afxRaster*)cmd->tex);
}

_SGL afxCmdId _EncodeCmdBindRasters(avxCmdb cmdb, afxNat set, afxNat first, afxNat cnt, afxSampler smp[], afxRaster tex[])
{
    afxError err = AFX_ERR_NONE;
    _sglCmdBindRasters *cmd = AfxRequestArenaUnit(&cmdb->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->set = set;
    cmd->first = first;
    cmd->cnt = cnt;

    for (afxNat i = 0; i < cnt; i++)
    {
        cmd->smp[i] = smp ? smp[i] : 0;
        cmd->tex[i] = tex ? tex[i] : 0;
    }

    return _SglEncodeCmdCommand(cmdb, offsetof(afxCmd, BindRasters) / sizeof(void*), sizeof(cmd), &cmd->cmd);
}

//

_SGL afxCmdId _EncodeCmdExecuteCommands(avxCmdb cmdb, afxNat cnt, avxCmdb subsets[])
{
    afxError err = AFX_ERR_NONE;
    _sglCmdExecCmds *cmd = AfxRequestArenaUnit(&cmdb->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->cnt = cnt;

    for (afxNat i = 0; i < cnt; i++)
        cmd->subsets[i] = subsets[i];

    return _SglEncodeCmdCommand(cmdb, (offsetof(afxCmd, ExecuteCommands) / sizeof(void*)), sizeof(cmd), &cmd->cmd);
}

// Draw

_SGL void _DecodeCmdDraw(sglDpu* dpu, _sglCmdDraw const *cmd)
{
    _DpuDraw(dpu, cmd->vtxCnt, cmd->instCnt, cmd->firstVtx, cmd->firstInst);
}

_SGL afxCmdId _EncodeCmdDraw(avxCmdb cmdb, afxNat32 vtxCnt, afxNat32 instCnt, afxNat32 firstVtx, afxNat32 firstInst)
{
    afxError err = AFX_ERR_NONE;
    FlushCachedDrawStuff(cmdb, 0);

    _sglCmdDraw *cmd = AfxRequestArenaUnit(&cmdb->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->vtxCnt = vtxCnt;
    cmd->instCnt = instCnt;
    cmd->firstVtx = firstVtx;
    cmd->firstInst = firstInst;

    return _SglEncodeCmdCommand(cmdb, (offsetof(afxCmd, Draw) / sizeof(void*)), sizeof(cmd), &cmd->cmd);
}

_SGL void _DecodeCmdDrawIndexed(sglDpu* dpu, _sglCmdDrawIndexed const* cmd)
{
    _DpuDrawIndexed(dpu, cmd->idxCnt, cmd->instCnt, cmd->firstIdx, cmd->vtxOff, cmd->firstInst);
}

_SGL afxCmdId _EncodeCmdDrawIndexed(avxCmdb cmdb, afxNat32 idxCnt, afxNat32 instCnt, afxNat32 firstIdx, afxNat32 vtxOff, afxNat32 firstInst)
{
    afxError err = AFX_ERR_NONE;
    FlushCachedDrawIndexedStuff(cmdb, 0);

    _sglCmdDrawIndexed *cmd = AfxRequestArenaUnit(&cmdb->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->idxCnt = idxCnt;
    cmd->instCnt = instCnt;
    cmd->firstIdx = firstIdx;
    cmd->vtxOff = vtxOff;
    cmd->firstInst = firstInst;

    return _SglEncodeCmdCommand(cmdb, (offsetof(afxCmd, DrawIndexed) / sizeof(void*)), sizeof(cmd), &cmd->cmd);
}

_SGL afxError _SglCmdbEndCb(avxCmdb cmdb)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cmdb, afxFcc_CMDB);
    FlushCachedStuff(cmdb, 0);
    return err;
}

_SGL afxCmd _SglEncodeCmdVmt =
{
    .BindPipeline = _EncodeCmdBindPipeline,
    .BindRasterizer = _EncodeCmdBindRasterizer,
    .Draw = _EncodeCmdDraw,
    .DrawIndexed = _EncodeCmdDrawIndexed,
    .ExecuteCommands = _EncodeCmdExecuteCommands,
    .BindBuffers = _EncodeCmdBindBuffers,    
    .BindRasters = _EncodeCmdBindRasters,
    //.Rasterization = _EncodeCmdRasterizationVmt
};
