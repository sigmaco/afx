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

_SGL void DpuBufCpy(sglDpu* dpu, afxBuffer src, afxBuffer dst, afxNat opCnt, afxBufferCopyOp const ops[])
{
    afxError err = AFX_ERR_NONE;
    glVmt const* gl = &dpu->gl;

    DpuBindAndSyncBuf(dpu, sglBindFlag_BIND | sglBindFlag_KEEP | sglBindFlag_SYNC, GL_COPY_READ_BUFFER, src, 0, AfxGetBufferCapacity(src), 0, GL_INVALID_ENUM);
    DpuBindAndSyncBuf(dpu, sglBindFlag_BIND | sglBindFlag_KEEP | sglBindFlag_SYNC, GL_COPY_WRITE_BUFFER, dst, 0, AfxGetBufferCapacity(dst), 0, GL_INVALID_ENUM);

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
    DpuBindAndSyncBuf(dpu, sglBindFlag_BIND | sglBindFlag_KEEP | sglBindFlag_SYNC, glTarget, buf, offset, range, 0, GL_INVALID_ENUM);

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
    DpuBindAndSyncBuf(dpu, sglBindFlag_BIND | sglBindFlag_KEEP | sglBindFlag_SYNC, glTarget, buf, 0, AfxGetBufferCapacity(buf), 0, GL_INVALID_ENUM);

    if (toHost)
    {
        gl->GetBufferSubData(glTarget, offset, range, srcOrDst); _SglThrowErrorOccuried();
    }
    else
    {
        gl->BufferSubData(glTarget, offset, range, srcOrDst); _SglThrowErrorOccuried();
    }
}

_SGL void _SglDecodeBufCpy(sglDpu* dpu, _sglCmdBufCpy const* cmd)
{
    afxError err = AFX_ERR_NONE;    
    DpuBufCpy(dpu, cmd->src, cmd->dst, cmd->opCnt, cmd->ops);
}

_SGL void _SglDecodeBufSet(sglDpu* dpu, _sglCmdBufSet const* cmd)
{
    afxError err = AFX_ERR_NONE;
    DpuBufSet(dpu, cmd->buf, cmd->offset, cmd->range, cmd->data);
}

_SGL void _SglDecodeBufRw(sglDpu* dpu, _sglCmdBufRw const* cmd)
{
    afxError err = AFX_ERR_NONE;
    DpuBufRw(dpu, cmd->buf, cmd->offset, cmd->range, cmd->toHost, cmd->toHost ? cmd->dst : cmd->src);
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

_SGL afxCmdBuffer const _SglEncodeCmdBufferVmt =
{
    .rw = _SglEncodeCmdBufRw,
    .set = _SglEncodeCmdBufSet,
    .cpy = _SglEncodeCmdBufCpy
};

