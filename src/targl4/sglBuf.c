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
#include "qwadro/draw/io/afxBuffer.h"
#include "qwadro/draw/afxDrawSystem.h"
#include "qwadro/draw/afxDrawSystem.h"

#if 0
_SGL void _AfxStdUbufImplUnmap(afxBuffer buf)
{
    if (!buf->base.mapped) AfxLogError("");
    else
    {
        afxBuffer base = AfxBuffer.GetBase(buf);

        buf->base.mapped = FALSE;

        if (base->gpuHandle)
        {
            glVmt const* gl = &(((afxDrawSystem)AfxGetObjectProvider(&buf->base.obj))->vmt);
            afxFlags usage = base->usage;
            GLenum target = NIL;

            if (usage & afxBufferUsage_VERTEX)
                target = GL_ARRAY_BUFFER;
            else if (usage & afxBufferUsage_INDEX)
                target = GL_ELEMENT_ARRAY_BUFFER;
            else if (usage & afxBufferUsage_UNIFORM)
                target = GL_UNIFORM_BUFFER;
            else if (usage & afxBufferUsage_STORAGE)
                target = GL_SHADER_STORAGE_BUFFER;
            else if (usage & afxBufferUsage_INDIRECT)
                target = GL_DRAW_INDIRECT_BUFFER;
            else if (usage & afxBufferUsage_UNIFORM_TEXELS)
                target = GL_PIXEL_UNPACK_BUFFER;
            else if (usage & afxBufferUsage_STORAGE_TEXELS)
                target = GL_PIXEL_PACK_BUFFER;
            else if (usage == afxBufferUsage_SRC)
                target = GL_COPY_READ_BUFFER;
            else if (usage == afxBufferUsage_DST)
                target = GL_COPY_WRITE_BUFFER;
            else AfxLogError("");

            GLint glaccess;

            if (usage & (afxBufferUsage_SRC | afxBufferUsage_DST))
            {
                // copy
                glaccess = GL_READ_WRITE;
            }
            else if (usage & afxBufferUsage_SRC)
            {
                glaccess = GL_READ_ONLY;
            }
            else if (usage & afxBufferUsage_DST)
            {
                glaccess = GL_WRITE_ONLY;
            }
            else
            {
                glaccess = NIL;
                AfxLogError("");
            }

            AfxAssert(gl->IsBuffer(base->gpuHandle));

            gl->BindBuffer(target, base->gpuHandle); _SglThrowErrorOccuried();
            gl->BufferSubData(target, buf->base.mappedOff, buf->base.mappedRange, &(((afxByte*)buf->base.buf)[buf->base.mappedOff])); _SglThrowErrorOccuried();
            gl->BindBuffer(target, 0); _SglThrowErrorOccuried();

            base->lastUpdTime = AfxGetTimer();
            buf->lastUpdTime = AfxGetTimer();
        }
    }
}

_SGL void* _AfxStdUbufImplMap(afxBuffer buf, afxSize off, afxSize siz)
{
    void *ptr = NIL;

    if (buf->base.mapped) AfxLogError("");
    else
    {
        buf->base.mappedOff = off;
        buf->base.mappedRange = siz ? siz : buf->base.cap - off;
        buf->base.mapped = TRUE;
    }
    return ptr;
}
#endif//0

_SGL afxError DpuBindAndSyncBuf(sglDpu* dpu, GLenum glTarget, afxBuffer buf)
{
    //AfxEntry("buf=%p", buf);
    afxError err = AFX_ERR_NONE;
    glVmt const* gl = &dpu->gl;

    if (!buf)
    {
        gl->BindBuffer(glTarget, 0); _SglThrowErrorOccuried();
    }
    else
    {
        AfxAssertObjects(1, &buf, afxFcc_BUF);
        sglUpdateFlags devUpdReq = (buf->updFlags & SGL_UPD_FLAG_DEVICE);
        GLuint glHandle = buf->glHandle;
            
        if ((!glHandle) || (devUpdReq & SGL_UPD_FLAG_DEVICE_INST))
        {
            if (glHandle)
            {
                gl->DeleteBuffers(1, &(glHandle)); _SglThrowErrorOccuried();
                buf->glHandle = NIL;
                glHandle = NIL;
            }

            _SglThrowErrorOccuried();
            gl->GenBuffers(1, &(glHandle)); _SglThrowErrorOccuried();
            gl->BindBuffer(buf->glTarget, glHandle); _SglThrowErrorOccuried();
            AfxAssert(gl->IsBuffer(glHandle));
            buf->glHandle = glHandle;

            AfxAssert(gl->BufferStorage);
            gl->BufferStorage(buf->glTarget, buf->base.cap, NIL, buf->glAccess | GL_DYNAMIC_STORAGE_BIT); _SglThrowErrorOccuried();
            AfxLogEcho("Hardware-side buffer %p ready. %u, %u, %x", buf, buf->glTarget, glHandle, buf->base.usage);
            buf->updFlags &= ~(SGL_UPD_FLAG_DEVICE);

            if (glTarget != buf->glTarget)
            {
                gl->BindBuffer(buf->glTarget, 0); _SglThrowErrorOccuried();
                gl->BindBuffer(glTarget, glHandle); _SglThrowErrorOccuried();
            }
        }
        else
        {
            gl->BindBuffer(glTarget, glHandle); _SglThrowErrorOccuried();
        }
    }
    return err;
}

_SGL afxError _DpuLoadBuf(sglDpu* dpu, afxBuffer buf, afxSize offset, afxNat range, afxByte const* src)
{
    afxError err = AFX_ERR_NONE;
    glVmt const* gl = &dpu->gl;
    GLenum glTarget = GL_COPY_WRITE_BUFFER;
    DpuBindAndSyncBuf(dpu, glTarget, buf);
    gl->BufferSubData(glTarget, offset, range, src); _SglThrowErrorOccuried();
    return err;
}

_SGL afxError _DpuStoreBuf(sglDpu* dpu, afxBuffer buf, afxSize offset, afxNat range, afxByte* dst)
{
    afxError err = AFX_ERR_NONE;
    glVmt const* gl = &dpu->gl;
    GLenum glTarget = GL_COPY_READ_BUFFER;
    DpuBindAndSyncBuf(dpu, glTarget, buf);
    gl->GetBufferSubData(glTarget, offset, range, dst); _SglThrowErrorOccuried();
    return err;
}

_SGL afxError _DpuOutputBuf(sglDpu* dpu, afxBuffer buf, afxSize offset, afxNat range, afxStream out, afxSize at)
{
    afxError err = AFX_ERR_NONE;
    glVmt const* gl = &dpu->gl;
    GLenum glTarget = GL_COPY_READ_BUFFER;
    GLenum glAccess = GL_MAP_READ_BIT;
    DpuBindAndSyncBuf(dpu, glTarget, buf);

    void* src = gl->MapBufferRange(glTarget, offset, range, glAccess); _SglThrowErrorOccuried();

    if (!src) AfxThrowError();
    else
    {
        AfxWriteStreamAt(out, at, range, 0, src);

        gl->UnmapBuffer(glTarget); _SglThrowErrorOccuried();
    }
    return err;
}

_SGL afxError _DpuInputBuf(sglDpu* dpu, afxBuffer buf, afxSize offset, afxNat range, afxStream in, afxSize at)
{
    afxError err = AFX_ERR_NONE;
    glVmt const* gl = &dpu->gl;
    GLenum glTarget = GL_COPY_WRITE_BUFFER;
    GLenum glAccess = GL_MAP_WRITE_BIT;
    DpuBindAndSyncBuf(dpu, glTarget, buf);

    afxByte* dst = gl->MapBufferRange(glTarget, offset, range, glAccess); _SglThrowErrorOccuried();

    if (!dst) AfxThrowError();
    else
    {
        AfxReadStreamAt(in, at, range, 0, dst);
        gl->UnmapBuffer(glTarget); _SglThrowErrorOccuried();
        //gl->FlushMappedBufferRange(glTarget, op->dstOffset, op->range); _SglThrowErrorOccuried();
    }
    return err;
}

_SGL afxError _DpuRemapBuf(sglDpu* dpu, afxBuffer buf, afxSize offset, afxNat range, afxFlags flags)
{
    afxError err = AFX_ERR_NONE;
    //AfxAssertObject(ddge, afxFcc_DDGE);
    glVmt const* gl = &dpu->gl;

    GLenum glTarget = NIL;

    if (buf->base.access & afxBufferAccess_W)
        glTarget = GL_COPY_WRITE_BUFFER;
    else
        glTarget = GL_COPY_READ_BUFFER;

    DpuBindAndSyncBuf(dpu, glTarget, buf);

    if (range)
    {
        GLenum glAccess = NIL;
        AfxAssert(!buf->base.bytemap);

#if !0
        if (range == AfxGetBufferCapacity(buf))
        {
            if (!(buf->base.access & afxBufferAccess_R))
            {
                if (buf->base.access & afxBufferAccess_W)
                    glAccess = GL_WRITE_ONLY;
            }
            else
            {
                if (buf->base.access & afxBufferAccess_W)
                    glAccess = GL_READ_WRITE;
                else
                    glAccess = GL_READ_ONLY;
            }

            buf->base.bytemap = gl->MapBuffer(glTarget, glAccess); _SglThrowErrorOccuried();
        }
        else
#endif
        {
            buf->base.bytemap = gl->MapBufferRange(glTarget, offset, range, buf->glAccess); _SglThrowErrorOccuried();
        }

        buf->base.mappedOffset = offset;
        buf->base.mappedRange = range;
        buf->base.mappedFlags = flags;
        AfxAssert(buf->base.bytemap);
    }
    else
    {
        AfxAssert(buf->base.bytemap);
        gl->UnmapBuffer(glTarget); _SglThrowErrorOccuried();
        buf->base.bytemap = NIL;

        //gl->FlushMappedBufferRange(glTarget, subm->buf->glMappedOff, subm->buf->glMappedSiz); _SglThrowErrorOccuried();
        buf->base.mappedOffset = 0;
        buf->base.mappedRange = 0;
        buf->base.mappedFlags = NIL;
    }
    //AfxAssert(!AfxLoadAtom32(&buf->base.pendingRemap));
    return err;
}

_SGL afxError _BufRemapCb(afxBuffer buf, afxSize offset, afxNat range, afxFlags flags, afxNat* portIdx, afxNat* dqueIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    AfxAssertRange(buf->base.cap, offset, range);
    
    if (buf->base.mappedRange) AfxThrowError();
    else
    {
        afxDrawContext dctx = AfxGetBufferContext(buf);
        AfxAssertObjects(1, &dctx, afxFcc_DCTX);
        afxNat bridgeIdx = 0;
        afxDrawBridge ddge = AfxGetDrawBridge(dctx, bridgeIdx);
        AfxAssertObjects(1, &ddge, afxFcc_DDGE);
        afxNat queIdx = AFX_INVALID_INDEX;

        if (AFX_INVALID_INDEX == (queIdx = _SglDdgeEnqueueMmapCb(ddge, buf, offset, range, flags)))
        {
            AfxThrowError();
        }
        else
        {
            *portIdx = bridgeIdx;
            *dqueIdx = queIdx;
        }
    }
    return err;
}

_SGL afxError _BufUnmapCb(afxBuffer buf, afxNat* portIdx, afxNat* dqueIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &buf, afxFcc_BUF);

    if (!buf->base.mappedRange) AfxThrowError();
    else
    {
        afxDrawContext dctx = AfxGetBufferContext(buf);
        AfxAssertObjects(1, &dctx, afxFcc_DCTX);
        afxNat bridgeIdx = 0;
        afxDrawBridge ddge = AfxGetDrawBridge(dctx, bridgeIdx);
        AfxAssertObjects(1, &ddge, afxFcc_DDGE);
        afxNat queIdx = AFX_INVALID_INDEX;

        if (AFX_INVALID_INDEX == (queIdx = _SglDdgeEnqueueMmapCb(ddge, buf, 0, 0, 0)))
        {
            AfxThrowError();
        }
        else
        {
            *portIdx = bridgeIdx;
            *dqueIdx = queIdx;
        }
    }
    return err;
}

_SGL afxError _BufDtorCb(afxBuffer buf)
{
    afxError err = AFX_ERR_NONE;

    afxDrawContext dctx = AfxGetBufferContext(buf);

    if (buf->base.mappedRange)
    {
        AfxUnmapBuffer(buf, TRUE);
        AfxAssert(!buf->base.mappedRange);
    }

    if (buf->glHandle)
    {
        _SglDctxDeleteGlRes(dctx, 0, (void*)buf->glHandle);
        buf->glHandle = 0;
    }

    if (_AvxBufStdImplementation.dtor(buf))
        AfxThrowError();

    return err;
}

_SGL afxError _BufCtorCb(afxBuffer buf, afxCookie const* cookie)
{
    afxResult err = NIL;
    AfxAssertObjects(1, &buf, afxFcc_BUF);

    if (_AvxBufStdImplementation.ctor(buf, cookie)) AfxThrowError();
    else
    {
        afxBufferSpecification const *spec = ((afxBufferSpecification const *)cookie->udd[1]) + cookie->no;

        buf->glHandle = NIL;
        buf->glTarget = NIL;
        buf->updFlags = SGL_UPD_FLAG_DEVICE_INST;

        afxBufferUsage usage = buf->base.usage;
        afxBufferAccess access = buf->base.access;
        GLbitfield glAccess = NIL;

        if (usage & afxBufferUsage_VERTEX)
            buf->glTarget = GL_ARRAY_BUFFER;
        else if (usage & afxBufferUsage_INDEX)
            buf->glTarget = GL_ELEMENT_ARRAY_BUFFER;
        else if (usage & afxBufferUsage_UNIFORM)
            buf->glTarget = GL_UNIFORM_BUFFER;
        else if (usage & afxBufferUsage_STORAGE)
            buf->glTarget = GL_SHADER_STORAGE_BUFFER;
        else if (usage & afxBufferUsage_INDIRECT)
            buf->glTarget = GL_DRAW_INDIRECT_BUFFER;
        else if (usage & afxBufferUsage_QUERY)
            buf->glTarget = GL_QUERY_BUFFER;
        else if (usage & afxBufferUsage_DST)
            buf->glTarget = GL_PIXEL_PACK_BUFFER;
        else if (usage & afxBufferUsage_SRC)
            buf->glTarget = GL_PIXEL_UNPACK_BUFFER;
        else
            AfxThrowError();

        if (access & afxBufferAccess_R)
            glAccess |= GL_MAP_READ_BIT;

        if (access & afxBufferAccess_W)
            glAccess |= GL_MAP_WRITE_BIT;

        if (access & afxBufferAccess_X)
            glAccess |= GL_MAP_PERSISTENT_BIT;

        if (access & afxBufferAccess_COHERENT)
            glAccess |= GL_MAP_COHERENT_BIT | GL_MAP_PERSISTENT_BIT;

        buf->glAccess = glAccess;

        buf->base.remap = _BufRemapCb;
        buf->base.unmap = _BufUnmapCb;

        if (spec->src)
        {
            if (AfxUpdateBuffer(buf, 0, buf->base.cap, spec->src))
                AfxThrowError();
        }

        if (err && _AvxBufStdImplementation.dtor(buf))
            AfxThrowError();
    }
    return err;
}

_SGL afxClassConfig const _SglBufMgrCfg =
{
    .fcc = afxFcc_BUF,
    .name = "Buffer",
    .desc = "Device Memory Buffer",
    .unitsPerPage = 2,
    .size = sizeof(AFX_OBJECT(afxBuffer)),
    .ctor = (void*)_BufCtorCb,
    .dtor = (void*)_BufDtorCb
};
