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
 *                   (c) 2017 SIGMA Technology Group — Federação SIGMA
 *                                    www.sigmaco.org
 */


#include "sgl.h"
#include "qwadro/draw/io/afxBuffer.h"
#include "qwadro/draw/afxDrawSystem.h"
#include "qwadro/draw/afxDrawSystem.h"

////////////////////////////////////////////////////////////////////////////////
// DRAW BUFFER                                                                //
////////////////////////////////////////////////////////////////////////////////

#if 0
_SGL void _AfxStdUbufImplUnmap(afxBuffer buf)
{
    if (!buf->base.mapped) AfxError("");
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
            else AfxError("");

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
                AfxError("");
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

    if (buf->base.mapped) AfxError("");
    else
    {
        buf->base.mappedOff = off;
        buf->base.mappedRange = siz ? siz : buf->base.siz - off;
        buf->base.mapped = TRUE;
    }
    return ptr;
}
#endif//0

_SGL afxError _SglBindAndSyncBuf(glVmt const* gl, afxBool syncOnly, GLenum glTarget, afxNat glUnit, afxBuffer buf, afxNat offset, afxNat range, afxNat stride, GLenum usage)
{
    //AfxEntry("buf=%p", buf);
    afxError err = AFX_ERR_NONE;
    GLuint glHandle;
    afxSize bufSiz;

    if (!buf)
    {
        glHandle = 0;
        bufSiz = 0;
        AfxAssert(offset == 0);
        AfxAssert(range == 0);
        //AfxAssert(stride == 0);
    }
    else
    {
        AfxAssertObjects(1, &buf, afxFcc_BUF);

        glHandle = buf->glHandle;
        bufSiz = AfxGetBufferSize(buf);
        sglUpdateFlags devUpdReq = (buf->updFlags & SGL_UPD_FLAG_DEVICE);

        if ((!glHandle) && (devUpdReq & SGL_UPD_FLAG_DEVICE_INST))
        {
            if (glHandle)
            {
                gl->DeleteBuffers(1, &(glHandle)); _SglThrowErrorOccuried();
                buf->glHandle = NIL;
                glHandle = NIL;
            }

            buf->glTarget = glTarget;
            buf->glUsage = usage;

            gl->GenBuffers(1, &(glHandle)); _SglThrowErrorOccuried();
            gl->BindBuffer(glTarget, glHandle); _SglThrowErrorOccuried();
            AfxAssert(gl->IsBuffer(glHandle));
            buf->glHandle = glHandle;

            afxBufferAccess access = buf->base.access;
            GLbitfield glAccess = NIL;
            
            if (access & afxBufferAccess_R)
                glAccess |= GL_DYNAMIC_STORAGE_BIT | GL_MAP_READ_BIT;
            
            if (access & afxBufferAccess_W)
                glAccess |= GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT;

            if (access & afxBufferAccess_X)
                glAccess |= GL_MAP_PERSISTENT_BIT;

            if (access & afxBufferAccess_COHERENT)
                glAccess |= GL_MAP_COHERENT_BIT | GL_MAP_PERSISTENT_BIT;

            buf->glAccess = glAccess;

            AfxAssert(gl->BufferStorage);
            gl->BufferStorage(glTarget, buf->base.siz, buf->base.bytemap, glAccess); _SglThrowErrorOccuried();
            //gl->BufferData(glTarget, buf->base.siz, buf->base.bytemap, buf->glUsage); _SglThrowErrorOccuried();
            AfxEcho("afxBuffer %p hardware-side data instanced. glTarget %u, glUnit %u, glHandle %u, offset %u, range %u, stride %u, usage %x", buf, glTarget, glUnit, glHandle, offset, range, stride, usage);
        }
        else if ((devUpdReq & SGL_UPD_FLAG_DEVICE_FLUSH))
        {
            AfxAssert(buf->glHandle == glHandle);
            AfxAssert(buf->glTarget == glTarget);
            AfxAssert(buf->glUsage == usage);

            AfxAssert(gl->IsBuffer(glHandle));
            gl->BindBuffer(glTarget, glHandle); _SglThrowErrorOccuried();
            AfxAssertRange(buf->base.siz, buf->lastUpdOffset, buf->lastUpdRange);
            gl->BufferSubData(glTarget, buf->lastUpdOffset, buf->lastUpdRange, &(buf->base.bytemap[buf->lastUpdOffset])); _SglThrowErrorOccuried();
        }

        buf->updFlags &= ~(SGL_UPD_FLAG_DEVICE);
        buf->lastUpdOffset = 0;
        buf->lastUpdRange = 0;

        if (syncOnly)
        {
            gl->BindBuffer(glTarget, 0); _SglThrowErrorOccuried();
        }
    }

    if (!syncOnly || !buf)
    {
        switch (glTarget)
        {
        case GL_ARRAY_BUFFER:
        {
            if (gl->BindVertexBuffer)
            {
                gl->BindVertexBuffer(glUnit, glHandle, offset, stride); _SglThrowErrorOccuried();
            }
            else
            {
                gl->BindBuffer(glTarget, glHandle); _SglThrowErrorOccuried();
                AfxAssert(offset == 0);
                AfxAssert(stride == 0 || !glHandle);
                AfxAssert(glUnit == 0);
            }
            break;
        }
        case GL_ATOMIC_COUNTER_BUFFER:
        case GL_TRANSFORM_FEEDBACK_BUFFER:
        case GL_UNIFORM_BUFFER:
        case GL_SHADER_STORAGE_BUFFER:
        {
            if (offset || range)
            {
                AfxAssert(range);
                AfxAssertRange(bufSiz, offset, range);
                gl->BindBufferRange(glTarget, glUnit, glHandle, offset, range); _SglThrowErrorOccuried();
            }
            else
            {
                gl->BindBufferBase(glTarget, glUnit, glHandle); _SglThrowErrorOccuried();
            }
            break;
        }
        case GL_ELEMENT_ARRAY_BUFFER:
        default:
        {
            gl->BindBuffer(glTarget, glHandle); _SglThrowErrorOccuried();
            break;
        }
        }
    }
    return err;
}

_SGL void* _SglMapBufferRange(afxBuffer buf, afxSize offset, afxNat range, afxFlags flags)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    AfxAssertRange(buf->base.siz, offset, range);
    
    void *map = &(buf->base.bytemap[offset]);

    ++buf->locked;

    if (buf->lastUpdOffset > offset)
        buf->lastUpdOffset = offset;

    if (buf->lastUpdOffset + buf->lastUpdRange < offset + range)
        buf->lastUpdRange = offset + range;

    AfxAssertRange(buf->base.siz, buf->lastUpdOffset, buf->lastUpdRange);

    if (flags & afxBufferAccess_W)
        buf->updFlags |= SGL_UPD_FLAG_DEVICE_FLUSH;

#if 0
    if (flags & afxBufferAccess_X)
    {
        buf->updFlags |= SGL_UPD_FLAG_DEVICE_INST;
    }
#endif

    return map;
}

_SGL afxError _SglUnmapBufferRange(afxBuffer buf)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    --buf->locked;
    return err;
}

_SGL afxError _SglBufDtor(afxBuffer buf)
{
    afxError err = AFX_ERR_NONE;
    AfxEntry("buf=%p", buf);

    afxDrawContext dctx = AfxGetObjectProvider(buf);
    afxMmu mmu = AfxGetDrawContextMmu(dctx);
    AfxAssertObjects(1, &mmu, afxFcc_MMU);

    if (buf->glHandle)
    {
        _SglDctxDeleteGlRes(dctx, 0, buf->glHandle);
        buf->glHandle = 0;
    }

    if (buf->base.bytemap)
    {
        AfxDeallocate(mmu, buf->base.bytemap);
    }
    return err;
}

_SGL afxError _SglBufCtor(afxBuffer buf, afxCookie const* cookie)
{
    AfxEntry("buf=%p", buf);
    afxResult err = NIL;
    AfxAssertObjects(1, &buf, afxFcc_BUF);

    afxBufferSpecification const *spec = ((afxBufferSpecification const *)cookie->udd[0]) + cookie->no;

    AfxAssert(spec);

    buf->base.siz = spec->siz;
    buf->base.usage = spec->usage;
    buf->base.access = spec->access;

    afxDrawContext dctx = AfxGetObjectProvider(buf);
    afxMmu mmu = AfxGetDrawContextMmu(dctx);
    AfxAssertObjects(1, &mmu, afxFcc_MMU);

    if (!(buf->base.bytemap = AfxAllocate(mmu, sizeof(afxByte), buf->base.siz, 0, AfxHint()))) AfxThrowError();
    else
    {
        if (spec->src)
            AfxCopy(1, buf->base.siz, spec->src, buf->base.bytemap);

        buf->lastUpdOffset = 0;
        buf->lastUpdRange = buf->base.siz;
        buf->locked = FALSE;

        buf->glHandle = NIL;
        buf->updFlags = SGL_UPD_FLAG_DEVICE_INST;

        buf->base.map = _SglMapBufferRange;
        buf->base.unmap = _SglUnmapBufferRange;

        if (err)
            AfxDeallocate(mmu, buf->base.bytemap);
    }
    return err;
}

_SGL afxClassConfig _SglBufClsConfig =
{
    .fcc = afxFcc_BUF,
    .name = "Buffer",
    .unitsPerPage = 1,
    .size = sizeof(AFX_OBJECT(afxBuffer)),
    .mmu = NIL,
    .ctor = (void*)_SglBufCtor,
    .dtor = (void*)_SglBufDtor
};
