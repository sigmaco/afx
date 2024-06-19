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
#include "qwadro/draw/pipe/avxDrawOps.h"

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
