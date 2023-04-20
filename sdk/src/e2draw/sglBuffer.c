#include "sgl.h"
#include "afx/draw/res/afxBuffer.h"
#include "afx/draw/afxDrawSystem.h"
#include "../e2coree/draw/afxDrawParadigms.h"

////////////////////////////////////////////////////////////////////////////////
// DRAW BUFFER                                                                //
////////////////////////////////////////////////////////////////////////////////

#if 0
_SGL void _AfxStdUbufImplUnmap(afxBuffer buf)
{
    if (!buf->mapped) AfxError("");
    else
    {
        afxBuffer base = AfxBuffer.GetBase(buf);

        buf->mapped = FALSE;

        if (base->gpuHandle)
        {
            sglVmt const* gl = &(((afxDrawSystem)AfxObjectGetProvider(&buf->obj))->vmt);
            afxFlags usage = base->usage;
            GLenum target = NIL;

            if (usage & AFX_BUF_USAGE_VERTICES)
                target = GL_ARRAY_BUFFER;
            else if (usage & AFX_BUF_USAGE_INDICES)
                target = GL_ELEMENT_ARRAY_BUFFER;
            else if (usage & AFX_BUF_USAGE_CONSTANT)
                target = GL_UNIFORM_BUFFER;
            else if (usage & AFX_BUF_USAGE_STORAGE)
                target = GL_SHADER_STORAGE_BUFFER;
            else if (usage & AFX_BUF_USAGE_INDIRECT)
                target = GL_DRAW_INDIRECT_BUFFER;
            else if (usage & AFX_BUF_USAGE_CONSTANT_TEXELS)
                target = GL_PIXEL_UNPACK_BUFFER;
            else if (usage & AFX_BUF_USAGE_STORAGE_TEXELS)
                target = GL_PIXEL_PACK_BUFFER;
            else if (usage == AFX_BUF_USAGE_SRC)
                target = GL_COPY_READ_BUFFER;
            else if (usage == AFX_BUF_USAGE_DST)
                target = GL_COPY_WRITE_BUFFER;
            else AfxError("");

            GLint glaccess;

            if (usage & (AFX_BUF_USAGE_SRC | AFX_BUF_USAGE_DST))
            {
                // copy
                glaccess = GL_READ_WRITE;
            }
            else if (usage & AFX_BUF_USAGE_SRC)
            {
                glaccess = GL_READ_ONLY;
            }
            else if (usage & AFX_BUF_USAGE_DST)
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
            gl->BufferSubData(target, buf->mappedOff, buf->mappedRange, &(((afxByte*)buf->buf)[buf->mappedOff])); _SglThrowErrorOccuried();
            gl->BindBuffer(target, 0); _SglThrowErrorOccuried();

            base->lastUpdTime = AfxGetTimer();
            buf->lastUpdTime = AfxGetTimer();
        }
    }
}

_SGL void* _AfxStdUbufImplMap(afxBuffer buf, afxSize off, afxSize siz)
{
    void *ptr = NIL;

    if (buf->mapped) AfxError("");
    else
    {
        buf->mappedOff = off;
        buf->mappedRange = siz ? siz : buf->siz - off;
        buf->mapped = TRUE;
    }
    return ptr;
}
#endif//0

_SGL afxError _SglBufBindAndSync(afxBuffer buf, afxNat unit, afxNat offset, afxNat rangeOrVtxStride, GLenum target, GLenum usage, afxDrawEngine deng)
{
    //AfxEntry("buf=%p", buf);
    afxError err = NIL;
    AfxAssertObject(buf, AFX_FCC_BUF);
    sglVmt const* gl = &deng->wglVmt;
    
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

                gl->BufferData(buf->glTarget, buf->siz, buf->bytemap, buf->glUsage); _SglThrowErrorOccuried();
                buf->updFlags &= ~(SGL_UPD_FLAG_DEVICE);
                AfxEcho("buf %p, instanced.", buf);
            }
            else if ((buf->updFlags & SGL_UPD_FLAG_DEVICE_FLUSH))
            {
                AfxAssert(buf->glHandle);
                AfxAssert(buf->glTarget == target);
                AfxAssert(buf->glUsage == usage);

                gl->BindBuffer(buf->glTarget, buf->glHandle); _SglThrowErrorOccuried();
                gl->BufferSubData(buf->glTarget, buf->lastUpdOffset, buf->lastUpdRange, &(buf->bytemap[buf->lastUpdOffset])); _SglThrowErrorOccuried();
                buf->updFlags &= ~(SGL_UPD_FLAG_DEVICE_FLUSH);
            }

            gl->BindBuffer(buf->glTarget, 0); _SglThrowErrorOccuried();
            buf->lastUpdOffset = 0;
            buf->lastUpdRange = 0;
        }
        


        {
            AfxAssert(buf->glHandle);
            AfxAssert(buf->glTarget);

            switch (target)
            {
            case GL_ARRAY_BUFFER:
            {
                if (gl->BindVertexBuffer)
                {
                    gl->BindVertexBuffer(unit, buf->glHandle, offset, rangeOrVtxStride); _SglThrowErrorOccuried();
                }
                else
                {
                    gl->BindBuffer(target, buf->glHandle); _SglThrowErrorOccuried();
                    AfxAssert(offset == 0);
                    AfxAssert(rangeOrVtxStride == 0);
                    AfxAssert(unit == 0);
                }
                break;
            }
            case GL_ELEMENT_ARRAY_BUFFER:
            {
                gl->BindBuffer(target, buf->glHandle); _SglThrowErrorOccuried();
                break;
            }
            default:
            {
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
    }
    else
    {
        switch (target)
        {
        case GL_ARRAY_BUFFER:
        {
            if (gl->BindVertexBuffer)
            {
                gl->BindVertexBuffer(unit, 0, offset, rangeOrVtxStride); _SglThrowErrorOccuried();
            }
            else
            {
                gl->BindBuffer(target, 0); _SglThrowErrorOccuried();
                AfxAssert(offset == 0);
                AfxAssert(rangeOrVtxStride == 0);
                AfxAssert(unit == 0);
            }
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
    return err;
}

_SGL afxError _AfxBufferDump2(afxBuffer buf, afxSize offset, afxSize stride, afxSize cnt, void *dst, afxSize dstStride)
{
    afxError err = NIL;
    AfxAssertObject(buf, AFX_FCC_BUF);
    AfxAssert(buf->siz > offset);
    AfxAssert(stride);
    AfxAssert(buf->siz >= offset + (cnt * stride));
    AfxAssert(dst);

    if (buf->locked) AfxThrowError();
    else
    {
        buf->locked = TRUE;

        afxByte const *bytemap = &(buf->bytemap[offset]);

        if (!dstStride || dstStride == stride)
        {
            AfxCopy(dst, bytemap, (cnt * stride));
        }
        else
        {
            afxByte *dst2 = dst;

            for (afxNat i = 0; i < cnt; i++)
            {
                AfxCopy(&(dst2[i * dstStride]), &(bytemap[i * stride]), stride);
            }
        }

        buf->locked = FALSE;
    }
    return err;
}

_SGL afxError _AfxBufferDump(afxBuffer buf, afxSize base, afxSize range, void *dst)
{
    afxError err = NIL;
    AfxAssertObject(buf, AFX_FCC_BUF);
    AfxAssert(dst);
    AfxAssert(range);

    if (buf->locked) AfxThrowError();
    else
    {
        buf->locked = TRUE;

        AfxCopy(dst, &buf->bytemap[base], range);

        buf->locked = FALSE;
    }
    return err;
}

_SGL afxError _AfxBufferUpdate2(afxBuffer buf, afxSize offset, afxSize stride, afxNat cnt, void const *src, afxSize srcStride)
{
    afxError err = NIL;
    AfxAssertObject(buf, AFX_FCC_BUF);
    AfxAssert(buf->siz > offset);
    AfxAssert(stride);
    AfxAssert(buf->siz >= offset + (cnt * stride));
    AfxAssert(src);

    if (buf->locked) AfxThrowError();
    else
    {
        buf->locked = TRUE;

        afxByte *bytemap = &(buf->bytemap[offset]);

        if (!srcStride || srcStride == stride)
        {
            AfxCopy(bytemap, src, (cnt * stride));
        }
        else
        {
            afxByte const *src2 = src;

            for (afxNat i = 0; i < cnt; i++)
            {
                AfxCopy(&(bytemap[i * stride]), &(src2[i * srcStride]), stride);
            }
        }

        if (buf->lastUpdOffset > offset)
            buf->lastUpdOffset = offset;

        if (buf->lastUpdRange < buf->lastUpdOffset + (cnt * stride))
            buf->lastUpdRange = buf->lastUpdRange + (cnt * stride);

        buf->locked = FALSE;
        buf->updFlags |= SGL_UPD_FLAG_DEVICE_FLUSH;
    }
    return err;
}

_SGL afxError _AfxBufferUpdate(afxBuffer buf, afxSize base, afxSize range, void const *src)
{
    afxError err = NIL;
    AfxAssertObject(buf, AFX_FCC_BUF);
    AfxAssert(src);
    AfxAssert(range);

    if (buf->locked) AfxThrowError();
    else
    {
        buf->locked = TRUE;

        AfxCopy(&buf->bytemap[base], src, range);

        if (buf->lastUpdOffset > base)
            buf->lastUpdOffset = base;

        if (buf->lastUpdRange < buf->lastUpdOffset + range)
            buf->lastUpdRange = buf->lastUpdRange + range;

        buf->locked = FALSE;
        buf->updFlags |= SGL_UPD_FLAG_DEVICE_FLUSH;
    }
    return err;
}

_SGL void const* _AfxBufferGetData(afxBuffer buf, afxSize offset)
{
    afxError err = NIL;
    AfxAssertObject(buf, AFX_FCC_BUF);
    AfxAssert(offset < buf->siz);
    return &(buf->bytemap[offset]);
}

_SGL afxSize _AfxBufferGetSize(afxBuffer buf)
{
    afxError err = NIL;
    AfxAssertObject(buf, AFX_FCC_BUF);
    return buf->siz;
}

_SGL afxBuffer _AfxDrawContextAcquireBuffer(afxDrawContext dctx, afxBufferSpecification const *spec)
{
    afxError err = NIL;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    AfxAssert(spec);
    //AfxEntry("dsys=%p,siz=%u,usage=%x", dsys, siz, usage);
    afxBuffer buf = NIL;

    _afxUbufCtorArgs args =
    {
        dctx,
        spec
    };

    if (!(buf = AfxObjectAcquire(AfxDrawContextGetBufferClass(dctx), &args, AfxSpawnHint())))
        AfxThrowError();

    return buf;
}

_SGL afxError _AfxBufDtor(afxBuffer buf)
{
    afxError err = NIL;
    AfxEntry("buf=%p", buf);
    AfxAssertObject(buf, AFX_FCC_BUF);

    if (buf->glHandle)
    {
        afxDrawContext dctx = AfxBufferGetContext(buf);
        _SglEnqueueGlResourceDeletion(dctx, 0, 0, buf->glHandle);
        buf->glHandle = 0;
    }

    if (buf->bytemap)
        AfxDeallocate(NIL, buf->bytemap);

    return err;
}

_SGL afxBufImpl const _AfxStdBufImpl;
afxBufImpl const _AfxStdBufImpl =
{
    _AfxBufferGetData,
    _AfxBufferGetSize,
    _AfxBufferDump,
    _AfxBufferDump2,
    _AfxBufferUpdate,
    _AfxBufferUpdate2,
    NIL,
    NIL
};

_SGL afxError _AfxBufCtor(afxBuffer buf, _afxUbufCtorArgs *args)
{
    AfxEntry("buf=%p", buf);
    afxResult err = NIL;
    AfxAssertObject(buf, AFX_FCC_BUF);

    afxBufferSpecification const *spec = args->spec;
    AfxAssert(spec);

    buf->siz = spec->siz;
    buf->usage = spec->usage;

    if (!(buf->bytemap = AfxAllocate(NIL, buf->siz, AfxSpawnHint()))) AfxThrowError();
    else
    {
        if (spec->src)
            AfxCopy(buf->bytemap, spec->src, buf->siz);

        buf->lastUpdOffset = 0;
        buf->lastUpdRange = buf->siz;
        buf->locked = FALSE;

        // idd
        buf->glHandle = NIL;
        buf->updFlags = SGL_UPD_FLAG_DEVICE_INST;

        if (err)
            AfxDeallocate(NIL, buf->bytemap);
    }
    return err;
}

_SGL afxClassSpecification const _AfxBufClassSpec;

afxClassSpecification const _AfxBufClassSpec =
{
    AFX_FCC_BUF,
    NIL,
    0,
    sizeof(AFX_OBJECT(afxBuffer)),
    NIL,
    (void*)_AfxBufCtor,
    (void*)_AfxBufDtor,
    "Buffer",
    &_AfxStdBufImpl
};
