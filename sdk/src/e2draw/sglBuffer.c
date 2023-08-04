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
#define _AFX_DRAW_CONTEXT_C
#define _AFX_DRAW_QUEUE_C
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
            glVmt const* gl = &(((afxDrawSystem)AfxObjectGetProvider(&buf->obj))->vmt);
            afxFlags usage = base->usage;
            GLenum target = NIL;

            if (usage & AFX_BUF_USAGE_VERTEX)
                target = GL_ARRAY_BUFFER;
            else if (usage & AFX_BUF_USAGE_INDEX)
                target = GL_ELEMENT_ARRAY_BUFFER;
            else if (usage & AFX_BUF_USAGE_UNIFORM)
                target = GL_UNIFORM_BUFFER;
            else if (usage & AFX_BUF_USAGE_STORAGE)
                target = GL_SHADER_STORAGE_BUFFER;
            else if (usage & AFX_BUF_USAGE_INDIRECT)
                target = GL_DRAW_INDIRECT_BUFFER;
            else if (usage & AFX_BUF_USAGE_UNIFORM_TEXELS)
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

_SGL afxError _SglDqueBindAndSyncBuf(afxDrawQueue dque, afxNat unit, afxBuffer buf, afxNat offset, afxNat rangeOrVtxStride, GLenum target, GLenum usage, glVmt const* gl)
{
    //AfxEntry("buf=%p", buf);
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(buf, AFX_FCC_BUF);
    sglDqueIdd *dqueIdd = dque->idd;

    if (buf)
    {
        sglBufIdd *idd = AfxGetBufferIdd(buf);

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

                gl->BufferData(idd->glTarget, buf->siz, buf->bytemap, idd->glUsage); _SglThrowErrorOccuried();
                idd->updFlags &= ~(SGL_UPD_FLAG_DEVICE);
                AfxEcho("afxBuffer %p hardware-side data instanced.", buf);
            }
            else if ((idd->updFlags & SGL_UPD_FLAG_DEVICE_FLUSH))
            {
                AfxAssert(idd->glHandle);
                AfxAssert(idd->glTarget == target);
                AfxAssert(idd->glUsage == usage);

                gl->BindBuffer(idd->glTarget, idd->glHandle); _SglThrowErrorOccuried();
                gl->BufferSubData(idd->glTarget, idd->lastUpdOffset, idd->lastUpdRange, &(buf->bytemap[idd->lastUpdOffset])); _SglThrowErrorOccuried();
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

_SGL void* _SglMapBufferRange(afxBuffer buf, afxSize offset, afxNat range, afxFlags flags)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(buf, AFX_FCC_BUF);
    AfxAssertRange(buf->siz, offset, range);
    
    void *map = &(buf->bytemap[offset]);

    afxDrawContext dctx = AfxGetBufferContext(buf);
    AfxAssertObject(dctx, AFX_FCC_DCTX);

    sglBufIdd *idd = buf->idd;
    ++idd->locked;
    idd->lastUpdOffset = offset;
    idd->lastUpdRange = range;

    if (flags & AFX_BUF_MAP_W)
        idd->updFlags |= SGL_UPD_FLAG_DEVICE_FLUSH;

    if (flags & AFX_BUF_MAP_X)
    {
        idd->updFlags |= SGL_UPD_FLAG_DEVICE_INST;
    }

    return map;
}

_SGL afxError _SglUnmapBufferRange(afxBuffer buf)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(buf, AFX_FCC_BUF);
    sglBufIdd *idd = buf->idd;
    --idd->locked;
    return err;
}

_SGL afxError _AfxBufDtor(afxBuffer buf)
{
    afxError err = AFX_ERR_NONE;
    AfxEntry("buf=%p", buf);
    AfxAssertObject(buf, AFX_FCC_BUF);

    sglBufIdd *idd = AfxGetBufferIdd(buf);

    if (idd)
    {
        afxDrawContext dctx = AfxGetBufferContext(buf);
        AfxAssertObject(dctx, AFX_FCC_DCTX);
        afxMemory mem = AfxGetDrawContextMemory(dctx);
        AfxAssertObject(mem, AFX_FCC_MEM);

        if (idd->glHandle)
        {
            _SglDeleteGlRes(dctx, 0, idd->glHandle);
            idd->glHandle = 0;
        }

        AfxDeallocate(mem, idd);
    }

    return err;
}

_SGL _afxBufVmt const _SglBufVmt =
{
    _AfxBufDtor,
    _SglMapBufferRange,
    _SglUnmapBufferRange
};

_SGL afxError _SglBufCtor(afxBuffer buf)
{
    AfxEntry("buf=%p", buf);
    afxResult err = NIL;
    AfxAssertObject(buf, AFX_FCC_BUF);

    afxDrawContext dctx = AfxGetBufferContext(buf);
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    afxMemory mem = AfxGetDrawContextMemory(dctx);
    AfxAssertObject(mem, AFX_FCC_MEM);

    // idd
    buf->vmt = &_SglBufVmt;
    sglBufIdd *idd = AfxAllocate(mem, sizeof(*idd), 0, AfxSpawnHint());
    buf->idd = idd;
    idd->glHandle = NIL;
    idd->updFlags = SGL_UPD_FLAG_DEVICE_INST;

    return err;
}
