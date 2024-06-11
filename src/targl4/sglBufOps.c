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

_SGL void DpuBufCpy(sglDpu* dpu, afxBuffer src, afxBuffer dst, afxNat opCnt, afxBufferCopyOp const ops[])
{
    afxError err = AFX_ERR_NONE;
    glVmt const* gl = &dpu->gl;

    DpuBindAndSyncBuf(dpu, GL_COPY_READ_BUFFER, src);
    DpuBindAndSyncBuf(dpu, GL_COPY_WRITE_BUFFER, dst);

    for (afxNat i = 0; i < opCnt; i++)
    {
        gl->CopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, ops[i].srcOffset, ops[i].dstOffset, ops[i].range); _SglThrowErrorOccuried();
    }
}

_SGL void DpuBufSet(sglDpu* dpu, afxBuffer buf, afxNat offset, afxNat range, afxNat data)
{
    afxError err = AFX_ERR_NONE;
    glVmt const* gl = &dpu->gl;

    GLenum glTarget = GL_COPY_WRITE_BUFFER;
    DpuBindAndSyncBuf(dpu, glTarget, buf);

    AfxAssert(buf->glAccess & GL_MAP_WRITE_BIT);
    void* dst = gl->MapBufferRange(glTarget, offset, range, GL_MAP_WRITE_BIT); _SglThrowErrorOccuried();
    AfxAssert(dst);
    AfxStream(range, 0, sizeof(data), &data, dst);
    gl->UnmapBuffer(glTarget); _SglThrowErrorOccuried();
    gl->FlushMappedBufferRange(glTarget, offset, range); _SglThrowErrorOccuried();
}

_SGL void DpuBufRw(sglDpu* dpu, afxBuffer buf, afxNat offset, afxNat range, afxBool toHost, void* srcOrDst)
{
    afxError err = AFX_ERR_NONE;
    glVmt const* gl = &dpu->gl;

    GLenum glTarget = toHost ? GL_COPY_READ_BUFFER : GL_COPY_WRITE_BUFFER;
    GLenum glAccess = toHost ? GL_MAP_READ_BIT : GL_MAP_WRITE_BIT;
    AfxAssert(buf->glAccess & glAccess);

    glTarget = buf->glTarget;
    DpuBindAndSyncBuf(dpu, glTarget, buf);

    if (toHost)
    {
        gl->GetBufferSubData(glTarget, offset, range, srcOrDst); _SglThrowErrorOccuried();
    }
    else
    {
        gl->BufferSubData(glTarget, offset, range, srcOrDst); _SglThrowErrorOccuried();
    }
}

_SGL void _DecodeCmdBufCpy(sglDpu* dpu, _sglCmdBufCpy const* cmd)
{
    afxError err = AFX_ERR_NONE;    
    DpuBufCpy(dpu, cmd->src, cmd->dst, cmd->opCnt, cmd->ops);
}

_SGL void _DecodeCmdBufSet(sglDpu* dpu, _sglCmdBufSet const* cmd)
{
    afxError err = AFX_ERR_NONE;
    DpuBufSet(dpu, cmd->buf, cmd->offset, cmd->range, cmd->data);
}

_SGL void _DecodeCmdBufRw(sglDpu* dpu, _sglCmdBufRw const* cmd)
{
    afxError err = AFX_ERR_NONE;
    DpuBufRw(dpu, cmd->buf, cmd->offset, cmd->range, cmd->toHost, cmd->toHost ? cmd->dst : (void*)cmd->src);
}

_SGL afxCmdId _SglEncodeCmdBufCpy(avxCmdb cmdb, afxBuffer src, afxBuffer dst, afxNat opCnt, afxBufferCopyOp const ops[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cmdb, afxFcc_CMDB);
    AfxAssertObjects(1, &src, afxFcc_BUF);
    AfxAssertObjects(1, &dst, afxFcc_BUF);
    AfxAssert(opCnt);
    AfxAssert(ops);

    _sglCmdBufCpy *cmd = AfxRequestArenaUnit(&cmdb->base.cmdArena, sizeof(*cmd) + (opCnt * sizeof(cmd->ops[0])));
    AfxAssert(cmd);
    cmd->src = src;
    cmd->dst = dst;
    cmd->opCnt = opCnt;

    AfxAssert(8 > opCnt);

    for (afxNat i = 0; i < opCnt; i++)
        cmd->ops[i] = ops[i];

    return _SglEncodeCmdCommand(cmdb, (offsetof(afxCmd, buf.cpy) / sizeof(void*)), sizeof(cmd), &cmd->cmd);
}

_SGL afxCmdId _SglEncodeCmdBufSet(avxCmdb cmdb, afxBuffer buf, afxNat offset, afxNat range, afxNat data)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cmdb, afxFcc_CMDB);
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    AfxAssert(range);

    _sglCmdBufSet *cmd = AfxRequestArenaUnit(&cmdb->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->buf = buf;
    cmd->offset = offset;
    cmd->range = range;
    cmd->data = data;

    return _SglEncodeCmdCommand(cmdb, (offsetof(afxCmd, buf.set) / sizeof(void*)), sizeof(cmd), &cmd->cmd);
}

_SGL afxCmdId _SglEncodeCmdBufRw(avxCmdb cmdb, afxBuffer buf, afxNat offset, afxNat range, afxBool toHost, void* data)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cmdb, afxFcc_CMDB);
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    AfxAssert(range);
    AfxAssert(data);
    afxCmdId rslt;
    _sglCmdBufRw *cmd;

    if (!toHost)
    {
        cmd = AfxRequestArenaUnit(&cmdb->base.cmdArena, sizeof(*cmd) + range);
        AfxAssert(cmd);
        cmd->toHost = !!toHost;
        cmd->buf = buf;
        cmd->offset = offset;
        cmd->range = range;

        AfxCopy2(range, 1, data, cmd->src);

        rslt = _SglEncodeCmdCommand(cmdb, (offsetof(afxCmd, buf.rw) / sizeof(void*)), sizeof(cmd), &cmd->cmd);
    }
    else
    {
        cmd = AfxRequestArenaUnit(&cmdb->base.cmdArena, sizeof(*cmd));
        AfxAssert(cmd);
        cmd->toHost = !!toHost;
        cmd->buf = buf;
        cmd->offset = offset;
        cmd->range = range;
        cmd->dst = data;

        rslt = _SglEncodeCmdCommand(cmdb, (offsetof(afxCmd, buf.rw) / sizeof(void*)), sizeof(cmd), &cmd->cmd);
    }
    return rslt;
}

_SGL afxCmdBuf const _SglEncodeCmdBufferVmt =
{
    .rw = _SglEncodeCmdBufRw,
    .set = _SglEncodeCmdBufSet,
    .cpy = _SglEncodeCmdBufCpy
};

