/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                      S I G M A   T E C H N O L O G Y   G R O U P
 *
 *                                   Public Test Build
 *                               (c) 2017 Federação SIGMA
 *                                    www.sigmaco.org
 */

#define _AFX_BUFFER_C
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
            idd->lastUpdTime = AfxGetTimer();
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

_SGL afxError _SglDqueBindAndSyncBuf(afxDrawQueue dque, afxNat unit, afxBuffer buf, afxNat offset, afxNat rangeOrVtxStride, GLenum target, GLenum usage)
{
    //AfxEntry("buf=%p", buf);
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(buf, AFX_FCC_BUF);
    sglVmt const* gl = &dque->wglVmt;
    
    if (buf)
    {
        sglBufIdd *idd = AfxBufferGetIdd(buf);

        if ((idd->updFlags & SGL_UPD_FLAG_DEVICE))
        {
            if ((idd->updFlags & SGL_UPD_FLAG_DEVICE_INST))
            {
                if (idd->glHandle)
                {
                    gl->DeleteBuffers(1, &(idd->glHandle)); _SglThrowErrorOccuried();
                    idd->glHandle = NIL;
                }

                AfxAssert(NIL == idd->glHandle);

                idd->glTarget = target;
                idd->glUsage = usage;
                gl->GenBuffers(1, &(idd->glHandle)); _SglThrowErrorOccuried();
                gl->BindBuffer(idd->glTarget, idd->glHandle); _SglThrowErrorOccuried();
                AfxAssert(gl->IsBuffer(idd->glHandle));

                gl->BufferData(idd->glTarget, buf->siz, idd->bytemap, idd->glUsage); _SglThrowErrorOccuried();
                idd->updFlags &= ~(SGL_UPD_FLAG_DEVICE);
                AfxEcho("afxBuffer %p hardware-side data instanced.", buf);
            }
            else if ((idd->updFlags & SGL_UPD_FLAG_DEVICE_FLUSH))
            {
                AfxAssert(idd->glHandle);
                AfxAssert(idd->glTarget == target);
                AfxAssert(idd->glUsage == usage);

                gl->BindBuffer(idd->glTarget, idd->glHandle); _SglThrowErrorOccuried();
                gl->BufferSubData(idd->glTarget, idd->lastUpdOffset, idd->lastUpdRange, &(idd->bytemap[idd->lastUpdOffset])); _SglThrowErrorOccuried();
                idd->updFlags &= ~(SGL_UPD_FLAG_DEVICE_FLUSH);
            }

            gl->BindBuffer(idd->glTarget, 0); _SglThrowErrorOccuried();
            idd->lastUpdOffset = 0;
            idd->lastUpdRange = 0;
        }
        


        {
            AfxAssert(idd->glHandle);
            AfxAssert(idd->glTarget);

            switch (target)
            {
            case GL_ARRAY_BUFFER:
            {
                AfxAssert(gl->BindVertexBuffer);

                if (gl->BindVertexBuffer)
                {
                    gl->BindVertexBuffer(unit, idd->glHandle, offset, rangeOrVtxStride); _SglThrowErrorOccuried();
                }
#if 0
                else
                {
                    gl->BindBuffer(target, idd->glHandle); _SglThrowErrorOccuried();
                    AfxAssert(offset == 0);
                    AfxAssert(rangeOrVtxStride == 0);
                    AfxAssert(unit == 0);
                }
#endif
                break;
            }
            case GL_ELEMENT_ARRAY_BUFFER:
            {
                gl->BindBuffer(target, idd->glHandle); _SglThrowErrorOccuried();
                break;
            }
            default:
            {
                if (rangeOrVtxStride)
                {
                    gl->BindBufferRange(target, unit, idd->glHandle, offset, rangeOrVtxStride); _SglThrowErrorOccuried();
                }
                else
                {
                    gl->BindBufferBase(target, unit, idd->glHandle); _SglThrowErrorOccuried();
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
            AfxAssert(gl->BindVertexBuffer);

            if (gl->BindVertexBuffer)
            {
                gl->BindVertexBuffer(unit, 0, offset, rangeOrVtxStride); _SglThrowErrorOccuried();
            }
#if 0
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
    return err;
}

_SGL afxError _AfxBufferDump2(afxBuffer buf, afxSize offset, afxSize stride, afxSize cnt, void *dst, afxSize dstStride)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(buf, AFX_FCC_BUF);
    AfxAssert(buf->siz > offset);
    AfxAssert(stride);
    AfxAssert(buf->siz >= offset + (cnt * stride));
    AfxAssert(dst);

    sglBufIdd *idd = AfxBufferGetIdd(buf);

    if (idd->locked) AfxThrowError();
    else
    {
        idd->locked = TRUE;

        afxByte const *bytemap = &(idd->bytemap[offset]);

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

        idd->locked = FALSE;
    }
    return err;
}

_SGL afxError _AfxBufferDump(afxBuffer buf, afxSize base, afxSize range, void *dst)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(buf, AFX_FCC_BUF);
    AfxAssert(dst);
    AfxAssert(range);

    sglBufIdd *idd = AfxBufferGetIdd(buf);

    if (idd->locked) AfxThrowError();
    else
    {
        idd->locked = TRUE;

        AfxCopy(dst, &idd->bytemap[base], range);

        idd->locked = FALSE;
    }
    return err;
}

_SGL afxError _AfxBufferUpdate2(afxBuffer buf, afxSize offset, afxSize stride, afxNat cnt, void const *src, afxSize srcStride)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(buf, AFX_FCC_BUF);
    AfxAssert(buf->siz > offset);
    AfxAssert(stride);
    AfxAssert(buf->siz >= offset + (cnt * stride));
    AfxAssert(src);
    AfxAssert(srcStride);

    sglBufIdd *idd = AfxBufferGetIdd(buf);

    if (idd->locked) AfxThrowError();
    else
    {
        idd->locked = TRUE;

        afxByte *bytemap = &(idd->bytemap[offset]);

        if (srcStride == stride)
        {
            AfxCopy(bytemap, src, (cnt * stride));
        }
        else
        {
            afxByte const *src2 = src;

            for (afxNat i = 0; i < cnt; i++)
            {
                AfxAssert(stride != 1 || (stride == 1 && AFX_N8_MAX >= (afxNat8)(src2[i * srcStride])));
                AfxAssert(stride != 2 || (stride == 2 && AFX_N16_MAX >= (afxNat16)(src2[i * srcStride])));
                AfxAssert(stride != 4 || (stride == 4 && AFX_N32_MAX >= (afxNat32)(src2[i * srcStride])));

                AfxCopy(&(bytemap[i * stride]), &(src2[i * srcStride]), stride);
            }
        }

        if (idd->lastUpdOffset > offset)
            idd->lastUpdOffset = offset;

        if (idd->lastUpdRange < offset + (cnt * stride))
            idd->lastUpdRange = offset + (cnt * stride);

        idd->locked = FALSE;
        idd->updFlags |= SGL_UPD_FLAG_DEVICE_FLUSH;
    }
    return err;
}

_SGL afxError _AfxBufferUpdate(afxBuffer buf, afxSize base, afxSize range, void const *src)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(buf, AFX_FCC_BUF);
    AfxAssert(src);
    AfxAssert(range);

    sglBufIdd *idd = AfxBufferGetIdd(buf);

    if (idd->locked) AfxThrowError();
    else
    {
        idd->locked = TRUE;

        AfxCopy(&idd->bytemap[base], src, range);

        if (idd->lastUpdOffset > base)
            idd->lastUpdOffset = base;

        if (idd->lastUpdRange < base + range)
            idd->lastUpdRange = base + range;

        idd->locked = FALSE;
        idd->updFlags |= SGL_UPD_FLAG_DEVICE_FLUSH;
    }
    return err;
}

_SGL void const* _AfxBufferGetData(afxBuffer buf, afxSize offset)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(buf, AFX_FCC_BUF);
    AfxAssert(offset < buf->siz);
    sglBufIdd *idd = AfxBufferGetIdd(buf);
    return &(idd->bytemap[offset]);
}

_SGL afxSize _AfxBufferGetSize(afxBuffer buf)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(buf, AFX_FCC_BUF);
    return buf->siz;
}

_SGL afxBool _SglBufEventHandler(afxObject *obj, afxEvent *ev)
{
    afxError err = AFX_ERR_NONE;
    afxBuffer buf = (void*)obj;
    AfxAssertObject(buf, AFX_FCC_BUF);
    (void)ev;
    return FALSE;
}

_SGL afxBool _SglBufEventFilter(afxObject *obj, afxObject *watched, afxEvent *ev)
{
    afxError err = AFX_ERR_NONE;
    afxBuffer buf = (void*)obj;
    AfxAssertObject(buf, AFX_FCC_BUF);
    (void)watched;
    (void)ev;
    return FALSE;
}

_SGL afxError _AfxBufDtor(afxBuffer buf)
{
    afxError err = AFX_ERR_NONE;
    AfxEntry("buf=%p", buf);
    AfxAssertObject(buf, AFX_FCC_BUF);

    sglBufIdd *idd = AfxBufferGetIdd(buf);

    if (idd->glHandle)
    {
        afxDrawContext dctx = AfxBufferGetContext(buf);
        _SglEnqueueGlResourceDeletion(dctx, 0, 0, idd->glHandle);
        idd->glHandle = 0;
    }

    if (idd->bytemap)
    {
        afxDrawContext dctx = AfxBufferGetContext(buf);
        AfxAssertObject(dctx, AFX_FCC_DCTX);
        afxMemory mem = AfxDrawContextGetMemory(dctx);
        AfxAssertObject(mem, AFX_FCC_MEM);

        AfxDeallocate(mem, idd->bytemap);
    }
    return err;
}

_SGL afxBufImpl const _SglBufImpl;
afxBufImpl const _SglBufImpl =
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

_SGL afxError _AfxBufCtor(void *cache, afxNat idx, afxBuffer buf, afxBufferSpecification const *specs)
{
    AfxEntry("buf=%p", buf);
    afxResult err = NIL;
    AfxAssertObject(buf, AFX_FCC_BUF);

    afxBufferSpecification const *spec = &specs[idx];
    AfxAssert(spec);

    buf->siz = spec->siz;
    buf->usage = spec->usage;

    afxDrawContext dctx = AfxObjectGetProvider(&buf->obj);
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    afxMemory mem = AfxDrawContextGetMemory(dctx);
    AfxAssertObject(mem, AFX_FCC_MEM);

    sglBufIdd *idd = AfxBufferGetIdd(buf);

    if (!(idd->bytemap = AfxAllocate(mem, buf->siz, AfxSpawnHint()))) AfxThrowError();
    else
    {
        if (spec->src)
            AfxCopy(idd->bytemap, spec->src, buf->siz);

        idd->lastUpdOffset = 0;
        idd->lastUpdRange = buf->siz;
        idd->locked = FALSE;

        // idd
        idd->glHandle = NIL;
        idd->updFlags = SGL_UPD_FLAG_DEVICE_INST;

        if (err)
            AfxDeallocate(mem, idd->bytemap);
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
    .event = _SglBufEventHandler,
    .eventFilter = _SglBufEventFilter,
    "afxBuffer",
    &_SglBufImpl
};
