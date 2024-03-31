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

_SGL void _SglDpuBufCpy(sglDpuIdd* dpu, _sglCmdBufCpy const* cmd)
{
    afxError err = AFX_ERR_NONE;
    glVmt const* gl = &dpu->gl;
    
    _SglBindAndSyncBuf(dpu, sglBindFlag_BIND | sglBindFlag_KEEP | sglBindFlag_SYNC, GL_COPY_READ_BUFFER, cmd->src, 0, AfxGetBufferCapacity(cmd->src), 0, GL_INVALID_ENUM);
    _SglBindAndSyncBuf(dpu, sglBindFlag_BIND | sglBindFlag_KEEP | sglBindFlag_SYNC, GL_COPY_WRITE_BUFFER, cmd->dst, 0, AfxGetBufferCapacity(cmd->dst), 0, GL_INVALID_ENUM);

    for (afxNat i = 0; i < cmd->opCnt; i++)
    {
        gl->CopyBufferSubData(GL_COPY_READ_BUFFER, cmd->ops[i].srcOffset, GL_COPY_WRITE_BUFFER, cmd->ops[i].dstOffset, cmd->ops[i].range); _SglThrowErrorOccuried();
    }
    AfxThrowError();
}

_SGL afxCmdId _SglEncodeCmdBufCpy(afxDrawStream diob, afxBuffer src, afxBuffer dst, afxNat opCnt, afxBufferCopyOp const ops[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &diob, afxFcc_DIOB);
    AfxAssertObjects(1, &src, afxFcc_BUF);
    AfxAssertObjects(1, &dst, afxFcc_BUF);
    AfxAssert(opCnt);
    AfxAssert(ops);

    _sglCmdBufCpy *cmd = AfxRequestArenaUnit(&diob->base.cmdArena, sizeof(*cmd) + (opCnt * sizeof(cmd->ops[0])));
    AfxAssert(cmd);
    cmd->src = src;
    cmd->dst = dst;
    cmd->opCnt = opCnt;

    AfxAssert(8 > opCnt);

    for (afxNat i = 0; i < opCnt; i++)
        cmd->ops[i] = ops[i];

    return _SglEncodeCmdCommand(diob, (offsetof(afxCmd, buf.cpy) / sizeof(void*)), sizeof(cmd), &cmd->cmd);
}

_SGL void _SglDpuBufSet(sglDpuIdd* dpu, _sglCmdBufSet const* cmd)
{
    afxError err = AFX_ERR_NONE;
    glVmt const* gl = &dpu->gl;

    GLenum glTarget = GL_COPY_WRITE_BUFFER;
    _SglBindAndSyncBuf(dpu, sglBindFlag_BIND | sglBindFlag_KEEP | sglBindFlag_SYNC, glTarget, cmd->buf, cmd->offset, cmd->range, 0, GL_INVALID_ENUM);

    AfxAssert(cmd->buf->glAccess & GL_MAP_WRITE_BIT);
    void* dst = gl->MapBufferRange(glTarget, cmd->offset, cmd->range, GL_MAP_WRITE_BIT); _SglThrowErrorOccuried();
    AfxAssert(dst);
    AfxStream(cmd->range, 0, sizeof(cmd->data), &cmd->data, dst);
    gl->UnmapBuffer(glTarget); _SglThrowErrorOccuried();
    gl->FlushMappedBufferRange(glTarget, cmd->offset, cmd->range); _SglThrowErrorOccuried();
}

_SGL afxCmdId _SglEncodeCmdBufSet(afxDrawStream diob, afxBuffer buf, afxNat offset, afxNat range, afxNat data)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &diob, afxFcc_DIOB);
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    AfxAssert(range);

    _sglCmdBufSet *cmd = AfxRequestArenaUnit(&diob->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->buf = buf;
    cmd->offset = offset;
    cmd->range = range;
    cmd->data = data;

    return _SglEncodeCmdCommand(diob, (offsetof(afxCmd, buf.set) / sizeof(void*)), sizeof(cmd), &cmd->cmd);
}

_SGL void _SglDpuBufRw(sglDpuIdd* dpu, _sglCmdBufRw const* cmd)
{
    afxError err = AFX_ERR_NONE;
    glVmt const* gl = &dpu->gl;

    GLenum glTarget = cmd->toHost ? GL_COPY_READ_BUFFER : GL_COPY_WRITE_BUFFER;
    GLenum glAccess = cmd->toHost ? GL_MAP_READ_BIT : GL_MAP_WRITE_BIT;
    AfxAssert(cmd->buf->glAccess & glAccess);
    
    glTarget = cmd->buf->glTarget;

    
    if (cmd->toHost)
    {
        _SglBindAndSyncBuf(dpu, sglBindFlag_BIND | sglBindFlag_KEEP | sglBindFlag_SYNC, glTarget, cmd->buf, 0, AfxGetBufferCapacity(cmd->buf), 0, GL_INVALID_ENUM);
        
        gl->GetBufferSubData(glTarget, cmd->offset, cmd->range, cmd->dst); _SglThrowErrorOccuried();
    }
    else
    {
        glTarget = cmd->buf->glTarget;
        _SglBindAndSyncBuf(dpu, sglBindFlag_BIND | sglBindFlag_KEEP | sglBindFlag_SYNC, glTarget, cmd->buf, 0, AfxGetBufferCapacity(cmd->buf), 0, GL_INVALID_ENUM);

        gl->BufferSubData(glTarget, cmd->offset, cmd->range, cmd->src); _SglThrowErrorOccuried();
    }
}

_SGL afxCmdId _SglEncodeCmdBufRw(afxDrawStream diob, afxBuffer buf, afxNat offset, afxNat range, afxBool toHost, void* data)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &diob, afxFcc_DIOB);
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    AfxAssert(range);
    AfxAssert(data);
    afxCmdId rslt;
    _sglCmdBufRw *cmd;

    if (!toHost)
    {
        cmd = AfxRequestArenaUnit(&diob->base.cmdArena, sizeof(*cmd) + range);
        AfxAssert(cmd);
        cmd->toHost = !!toHost;
        cmd->buf = buf;
        cmd->offset = offset;
        cmd->range = range;

        AfxCopy2(range, 1, data, cmd->src);

        rslt = _SglEncodeCmdCommand(diob, (offsetof(afxCmd, buf.rw) / sizeof(void*)), sizeof(cmd), &cmd->cmd);
    }
    else
    {
        cmd = AfxRequestArenaUnit(&diob->base.cmdArena, sizeof(*cmd));
        AfxAssert(cmd);
        cmd->toHost = !!toHost;
        cmd->buf = buf;
        cmd->offset = offset;
        cmd->range = range;
        cmd->dst = data;

        rslt = _SglEncodeCmdCommand(diob, (offsetof(afxCmd, buf.rw) / sizeof(void*)), sizeof(cmd), &cmd->cmd);
    }
    return rslt;
}

_SGL void _SglDpuBufIo(sglDpuIdd* dpu, _sglCmdBufIo const* cmd)
{
    afxError err = AFX_ERR_NONE;
    glVmt const* gl = &dpu->gl;

    GLenum glTarget = cmd->export ? GL_COPY_READ_BUFFER : GL_COPY_WRITE_BUFFER;
    GLenum glAccess = cmd->export ? GL_MAP_READ_BIT : GL_MAP_WRITE_BIT;
    AfxAssert(cmd->buf->glAccess & glAccess);

    if (cmd->export)
    {
        for (afxNat i = 0; i < cmd->opCnt; i++)
        {
            afxBufferIoOp const* op = &cmd->ops[i];
            _SglBindAndSyncBuf(dpu, sglBindFlag_BIND | sglBindFlag_KEEP | sglBindFlag_SYNC, GL_COPY_READ_BUFFER, cmd->buf, op->srcOffset, op->range, 0, GL_INVALID_ENUM);

            void* src = gl->MapBufferRange(glTarget, op->srcOffset, op->range, glAccess); _SglThrowErrorOccuried();
            AfxAssert(src);

            AfxWriteStreamAt2(cmd->io, op->dstOffset, op->range, op->dstStride, src, op->srcStride);

            gl->UnmapBuffer(glTarget); _SglThrowErrorOccuried();
        }
    }
    else
    {
        for (afxNat i = 0; i < cmd->opCnt; i++)
        {
            afxBufferIoOp const* op = &cmd->ops[i];
            _SglBindAndSyncBuf(dpu, sglBindFlag_BIND | sglBindFlag_KEEP | sglBindFlag_SYNC, GL_COPY_WRITE_BUFFER, cmd->buf, op->dstOffset, op->range, 0, GL_INVALID_ENUM);

            afxByte* dst = gl->MapBufferRange(glTarget, op->dstOffset, op->range, glAccess); _SglThrowErrorOccuried();
            AfxAssert(dst);

            AfxReadStreamAt2(cmd->io, op->srcOffset, op->range, op->srcStride, dst, op->dstStride);

            gl->UnmapBuffer(glTarget); _SglThrowErrorOccuried();
            gl->FlushMappedBufferRange(glTarget, op->dstOffset, op->range); _SglThrowErrorOccuried();
        }
    }
}

_SGL afxCmdId _SglEncodeCmdBufIo(afxDrawStream diob, afxBuffer buf, afxNat opCnt, afxBufferIoOp const ops[], afxBool export, afxStream io)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &diob, afxFcc_DIOB);
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    AfxAssertObjects(1, &io, afxFcc_IOB);
    AfxAssert(opCnt);
    AfxAssert(ops);

    _sglCmdBufIo *cmd = AfxRequestArenaUnit(&diob->base.cmdArena, sizeof(*cmd) + (opCnt * sizeof(cmd->ops[0])));
    AfxAssert(cmd);
    cmd->export = !!export;
    cmd->io = io;
    cmd->buf = buf;
    cmd->opCnt = opCnt;
    AfxAssert(8 > opCnt);

    for (afxNat i = 0; i < opCnt; i++)
        cmd->ops[i] = ops[i];

    return _SglEncodeCmdCommand(diob, (offsetof(afxCmd, buf.io) / sizeof(void*)), sizeof(cmd), &cmd->cmd);
}

_SGL afxCmdBuffer const _SglEncodeCmdBufferVmt =
{
    .io = _SglEncodeCmdBufIo,
    .rw = _SglEncodeCmdBufRw,
    .set = _SglEncodeCmdBufSet,
    .cpy = _SglEncodeCmdBufCpy
};
