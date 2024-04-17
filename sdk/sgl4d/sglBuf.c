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
        buf->base.mappedRange = siz ? siz : buf->base.siz - off;
        buf->base.mapped = TRUE;
    }
    return ptr;
}
#endif//0

_SGL afxError DpuBindAndSyncBuf(sglDpu* dpu, sglBindFlags bindFlags, GLenum glTarget, afxBuffer buf, afxNat offset, afxNat range, afxNat stride, GLenum usage)
{
    //AfxEntry("buf=%p", buf);
    afxError err = AFX_ERR_NONE;
    glVmt const* gl = &dpu->gl;
    GLuint glHandle;
    afxSize bufSiz;
    afxBool bound = FALSE;

    if (!buf)
    {
        glHandle = 0;
        bufSiz = 0;
        AfxAssert(offset == 0);
        AfxAssert(range == 0);
        AfxAssert(stride == 0);

        if (bindFlags & sglBindFlag_BIND)
        {
            switch (glTarget)
            {
            case GL_PIXEL_UNPACK_BUFFER:
            {
                dpu->activeUnpakPbo = NIL;
                break;
            }
            case GL_PIXEL_PACK_BUFFER:
            {
                dpu->activePakPbo = NIL;
                break;
            }
            default:
                break;
            }
            gl->BindBuffer(glTarget, 0); _SglThrowErrorOccuried();
        }
    }
    else
    {
        AfxAssertObjects(1, &buf, afxFcc_BUF);
        sglUpdateFlags devUpdReq = (buf->updFlags & SGL_UPD_FLAG_DEVICE);
        glHandle = buf->glHandle;
        bufSiz = AfxGetBufferCapacity(buf);
        afxBool bound = FALSE;
            
        if ((!glHandle) || (devUpdReq & SGL_UPD_FLAG_DEVICE_INST))
        {
            if (glHandle)
            {
                gl->DeleteBuffers(1, &(glHandle)); _SglThrowErrorOccuried();
                buf->glHandle = NIL;
                glHandle = NIL;
            }

            //buf->glTarget = glTarget;
            //buf->glUsage = usage;
            _SglThrowErrorOccuried();
            gl->GenBuffers(1, &(glHandle)); _SglThrowErrorOccuried();
            gl->BindBuffer(buf->glTarget, glHandle); _SglThrowErrorOccuried();
            AfxAssert(gl->IsBuffer(glHandle));
            buf->glHandle = glHandle;

            if (glTarget == buf->glTarget)
                bound = TRUE;

            AfxAssert(gl->BufferStorage);
            gl->BufferStorage(buf->glTarget, buf->base.siz, buf->base.bytemap, buf->glAccess | GL_DYNAMIC_STORAGE_BIT); _SglThrowErrorOccuried();
            //gl->BufferData(glTarget, buf->base.siz, buf->base.bytemap, buf->glUsage); _SglThrowErrorOccuried();
            AfxLogEcho("afxBuffer %p hardware-side data instanced. glTarget %u, glHandle %u, offset %u, range %u, stride %u, usage %x", buf, buf->glTarget, glHandle, offset, range, stride, usage);

            if (glTarget != buf->glTarget)
            {
                gl->BindBuffer(buf->glTarget, 0); _SglThrowErrorOccuried();
                bound = FALSE;
            }
        }
        else if ((devUpdReq & SGL_UPD_FLAG_DEVICE_FLUSH))
        {
            AfxAssert(buf->glHandle == glHandle);
            //AfxAssert(buf->glTarget == glTarget);
            //AfxAssert(buf->glUsage == usage);
            AfxAssert(gl->IsBuffer(glHandle));

            if (!bound)
            {
                gl->BindBuffer(glTarget, glHandle); _SglThrowErrorOccuried();
                bound = TRUE;
            }
            //AfxAssertRange(buf->base.siz, buf->lastUpdOffset, buf->lastUpdRange);
            //gl->BufferSubData(glTarget, buf->lastUpdOffset, buf->lastUpdRange, &(buf->base.bytemap[buf->lastUpdOffset])); _SglThrowErrorOccuried();
        }

        buf->updFlags &= ~(SGL_UPD_FLAG_DEVICE);
        //buf->lastUpdOffset = 0;
        //buf->lastUpdRange = 0;

        if (bound)
        {
            if (!(bindFlags & sglBindFlag_KEEP))
            {
                gl->BindBuffer(glTarget, 0); _SglThrowErrorOccuried();
                bound = FALSE;
            }
        }
        else
        {
            if (bindFlags & sglBindFlag_BIND)
            {
                switch (glTarget)
                {
                case GL_PIXEL_UNPACK_BUFFER:
                {
                    dpu->activeUnpakPbo = buf;
                    break;
                }
                case GL_PIXEL_PACK_BUFFER:
                {
                    dpu->activePakPbo = buf;
                    break;
                }
                default:
                    break;
                }
                gl->BindBuffer(glTarget, glHandle); _SglThrowErrorOccuried();
                bound = TRUE;
            }
        }
    }
    return err;
}

_SGL void* _BufMap(afxBuffer buf, afxSize offset, afxNat range, afxFlags flags)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    AfxAssertRange(buf->base.siz, offset, range);
    
    if (buf->base.bytemap) AfxThrowError();
    else
    {
        afxDrawContext dctx = AfxGetBufferContext(buf);
        AfxAssertObjects(1, &dctx, afxFcc_DCTX);
        afxDrawBridge ddge = AfxGetDrawBridge(dctx, 0);
        AfxAssertObjects(1, &ddge, afxFcc_DDGE);
        afxNat queIdx = AFX_INVALID_INDEX;

        if (AFX_INVALID_INDEX == (queIdx = _SglDdgeEnqueueMmapCb(ddge, buf, offset, range, flags)))
        {
            AfxThrowError();
        }
        else
        {
            if (AfxWaitForIdleDrawQueue(ddge, queIdx))
            {
                AfxThrowError();
            }
            AfxAssert(buf->base.bytemap);
        }
    }
    return buf->base.bytemap;
}

_SGL afxError _BufUnmap(afxBuffer buf)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &buf, afxFcc_BUF);

    if (!buf->base.bytemap) AfxThrowError();
    else
    {
        afxDrawContext dctx = AfxGetBufferContext(buf);
        AfxAssertObjects(1, &dctx, afxFcc_DCTX);
        afxDrawBridge ddge = AfxGetDrawBridge(dctx, 0);
        AfxAssertObjects(1, &ddge, afxFcc_DDGE);
        afxNat queIdx = AFX_INVALID_INDEX;

        if (AFX_INVALID_INDEX == (queIdx = _SglDdgeEnqueueMmapCb(ddge, buf, 0, 0, 0)))
        {
            AfxThrowError();
        }
        else
        {
            if (AfxWaitForIdleDrawQueue(ddge, queIdx))
            {
                AfxThrowError();
            }
            AfxAssert(!buf->base.bytemap);
        }
    }
    return err;
}

_SGL afxError _BufDtor(afxBuffer buf)
{
    afxError err = AFX_ERR_NONE;

    afxDrawContext dctx = AfxGetBufferContext(buf);

    if (buf->base.bytemap)
    {
        AfxUnmapBuffer(buf);
        AfxAssert(!buf->base.bytemap);
    }

    if (buf->glHandle)
    {
        _SglDctxDeleteGlRes(dctx, 0, (void*)buf->glHandle);
        buf->glHandle = 0;
    }
    return err;
}

_SGL afxError _BufCtor(afxBuffer buf, afxCookie const* cookie)
{
    afxResult err = NIL;
    AfxAssertObjects(1, &buf, afxFcc_BUF);

    afxDrawContext dctx = AfxGetBufferContext(buf);

    afxBufferSpecification const *spec = ((afxBufferSpecification const *)cookie->udd[0]) + cookie->no;
    AfxAssert(spec);

    if (!spec->siz) AfxThrowError();
    else
    {
        if (!spec->usage) AfxThrowError();
        else
        {
            buf->base.siz = spec->siz;
            buf->base.usage = spec->usage;
            buf->base.access = spec->access ? spec->access : afxBufferAccess_W;

            buf->base.bytemap = NIL;

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

            buf->base.map = _BufMap;
            buf->base.unmap = _BufUnmap;

            if (spec->src)
            {
                if (AfxUpdateBuffer(buf, 0, buf->base.siz, spec->src))
                    AfxThrowError();
            }
        }
    }
    return err;
}

_SGL afxClassConfig const _SglBufMgrCfg =
{
    .fcc = afxFcc_BUF,
    .name = "Buffer",
    .unitsPerPage = 2,
    .size = sizeof(AFX_OBJECT(afxBuffer)),
    .ctor = (void*)_BufCtor,
    .dtor = (void*)_BufDtor
};
