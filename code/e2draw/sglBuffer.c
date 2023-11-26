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
#include "afx/draw/afxBuffer.h"
#include "afx/draw/afxDrawSystem.h"
#include "afx/draw/afxDrawSystem.h"

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

_SGL afxError _SglDpuBindAndSyncBuf(sglDpuIdd* dpu, afxNat unit, afxBuffer buf, afxNat offset, afxNat range, afxNat stride, GLenum target, GLenum usage, glVmt const* gl)
{
    //AfxEntry("buf=%p", buf);
    afxError err = AFX_ERR_NONE;
    (void)dpu;
    (void)unit;
    (void)offset;
    (void)range;
    (void)stride;

    if (buf)
    {
        if ((buf->updFlags & SGL_UPD_FLAG_DEVICE))
        {
            if ((buf->updFlags & SGL_UPD_FLAG_DEVICE_INST))
            {
                if (buf->glHandle)
                {
                    gl->DeleteBuffers(1, &(buf->glHandle)); _SglThrowErrorOccuried();
                    buf->glHandle = NIL;
                }

                AfxAssert(NIL == buf->glHandle);

                buf->glTarget = target;
                buf->glUsage = usage;
                gl->GenBuffers(1, &(buf->glHandle)); _SglThrowErrorOccuried();
                gl->BindBuffer(buf->glTarget, buf->glHandle); _SglThrowErrorOccuried();
                AfxAssert(gl->IsBuffer(buf->glHandle));

                gl->BufferData(buf->glTarget, buf->base.siz, buf->base.bytemap, buf->glUsage); _SglThrowErrorOccuried();
                buf->updFlags &= ~(SGL_UPD_FLAG_DEVICE);
                AfxEcho("afxBuffer %p hardware-side data instanced.", buf);
            }
            else if ((buf->updFlags & SGL_UPD_FLAG_DEVICE_FLUSH))
            {
                AfxAssert(buf->glHandle);
                AfxAssert(buf->glTarget == target);
                AfxAssert(buf->glUsage == usage);

                gl->BindBuffer(buf->glTarget, buf->glHandle); _SglThrowErrorOccuried();
                AfxAssertRange(buf->base.siz, buf->lastUpdOffset, buf->lastUpdRange);
                gl->BufferSubData(buf->glTarget, buf->lastUpdOffset, buf->lastUpdRange, &(buf->base.bytemap[buf->lastUpdOffset])); _SglThrowErrorOccuried();
                buf->updFlags &= ~(SGL_UPD_FLAG_DEVICE_FLUSH);
            }

            gl->BindBuffer(buf->glTarget, 0); _SglThrowErrorOccuried();
            buf->lastUpdOffset = 0;
            buf->lastUpdRange = 0;
        }
        
#if 0
        {
            AfxAssert(buf->glHandle);
            AfxAssert(buf->glTarget);

            switch (target)
            {
            case GL_ARRAY_BUFFER:
            {
                AfxAssertObjects(1, &buf, afxFcc_BUF);
                //AfxAssert(gl->BindVertexBuffer);

                if (gl->BindVertexBuffer)
                {
                    gl->BindVertexBuffer(unit, buf->glHandle, offset, rangeOrVtxStride); _SglThrowErrorOccuried();
                }
#if !0
                else
                {
                    gl->BindBuffer(target, buf->glHandle); _SglThrowErrorOccuried();
                    AfxAssert(offset == 0);
                    AfxAssert(rangeOrVtxStride == 0);
                    AfxAssert(unit == 0);
                }
#endif
                break;
            }
            case GL_ELEMENT_ARRAY_BUFFER:
            {
                AfxAssertObjects(1, &buf, afxFcc_BUF);

                gl->BindBuffer(target, buf->glHandle); _SglThrowErrorOccuried();
                break;
            }
            default:
            {
                AfxAssertObjects(1, &buf, afxFcc_BUF);

                if (rangeOrVtxStride)
                {
                    gl->BindBufferRange(target, unit, buf->glHandle, offset, rangeOrVtxStride); _SglThrowErrorOccuried();
                }
                else
                {
                    gl->BindBufferBase(target, unit, buf->glHandle); _SglThrowErrorOccuried();
                }
                break;
            }
            }
        }
#endif
    }
#if 0
    else
    {
        switch (target)
        {
        case GL_ARRAY_BUFFER:
        {
            //AfxAssert(gl->BindVertexBuffer);

            if (gl->BindVertexBuffer)
            {
                gl->BindVertexBuffer(unit, 0, offset, rangeOrVtxStride); _SglThrowErrorOccuried();
            }
#if !0
            else
            {
                gl->BindBuffer(target, 0); _SglThrowErrorOccuried();
                AfxAssert(offset == 0);
                AfxAssert(rangeOrVtxStride == 0);
                AfxAssert(unit == 0);
            }
#endif
            break;
        }
        case GL_ELEMENT_ARRAY_BUFFER:
        {
            gl->BindBuffer(target, 0); _SglThrowErrorOccuried();
            AfxAssert(offset == 0);
            AfxAssert(rangeOrVtxStride == 0);
            AfxAssert(unit == 0);
            break;
        }
        default:
        {
            if (rangeOrVtxStride)
            {
                gl->BindBufferRange(target, unit, 0, offset, rangeOrVtxStride); _SglThrowErrorOccuried();
            }
            else
            {
                gl->BindBufferBase(target, unit, 0); _SglThrowErrorOccuried();
            }
            break;
        }
        }
    }
#endif
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

    if (flags & AFX_BUF_MAP_W)
        buf->updFlags |= SGL_UPD_FLAG_DEVICE_FLUSH;

    if (flags & AFX_BUF_MAP_X)
    {
        buf->updFlags |= SGL_UPD_FLAG_DEVICE_INST;
    }

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
    afxContext mem = AfxGetDrawContextMemory(dctx);
    AfxAssertObjects(1, &mem, afxFcc_CTX);

    if (buf->glHandle)
    {
        _SglDctxDeleteGlRes(dctx, 0, buf->glHandle);
        buf->glHandle = 0;
    }

    if (buf->base.bytemap)
    {
        AfxDeallocate(mem, buf->base.bytemap);
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

    afxDrawContext dctx = AfxGetObjectProvider(buf);
    afxContext mem = AfxGetDrawContextMemory(dctx);
    AfxAssertObjects(1, &mem, afxFcc_CTX);

    if (!(buf->base.bytemap = AfxAllocate(mem, sizeof(afxByte), buf->base.siz, 0, AfxHint()))) AfxThrowError();
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
            AfxDeallocate(mem, buf->base.bytemap);
    }
    return err;
}

_SGL afxClassConfig _SglBufClsConfig =
{
    .fcc = afxFcc_BUF,
    .name = "Buffer",
    .unitsPerPage = 1,
    .size = sizeof(AFX_OBJECT(afxBuffer)),
    .ctx = NIL,
    .ctor = (void*)_SglBufCtor,
    .dtor = (void*)_SglBufDtor
};
