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

#include "qwadro/io/afxXml.h"
#include "sgl.h"

#include "qwadro/draw/pipe/afxPipeline.h"

#include "qwadro/draw/afxDrawSystem.h"
#include "qwadro/io/afxUri.h"
#include "qwadro/core/afxSystem.h"

_SGL afxError _SglDpuBindAndSyncVin(sglDpuIdd* dpu, afxVertexInput vin, sglVertexInputState* nextVinBindings)
{
    //AfxEntry("pip=%p", pip);
    afxError err = AFX_ERR_NONE;
    glVmt const* gl = &dpu->gl;

    if (!vin)
    {
        gl->BindVertexArray(dpu->emptyVao); _SglThrowErrorOccuried();
    }
    else
    {
        AfxAssertObjects(1, &vin, afxFcc_VIN);

        GLuint glHandle = vin->glHandle;

        if ((!glHandle) || (vin->updFlags & SGL_UPD_FLAG_DEVICE))
        {
            if ((!glHandle) || (vin->updFlags & SGL_UPD_FLAG_DEVICE_INST))
            {
                if (glHandle)
                {
                    gl->DeleteVertexArrays(1, &glHandle); _SglThrowErrorOccuried();
                    glHandle = NIL;
                }
                gl->GenVertexArrays(1, &glHandle); _SglThrowErrorOccuried();
                gl->BindVertexArray(glHandle); _SglThrowErrorOccuried();
                vin->glHandle = glHandle;

                afxNat attrCnt = vin->base.attrCnt;

                for (afxNat i = 0; i < attrCnt; i++)
                {
                    afxVertexInputAttr const* attr = &vin->base.attrs[i];
                    AfxAssertRange(afxVertexFormat_TOTAL, attr->fmt, 1);
                    AfxAssertRange(SGL_MAX_VERTEX_ATTRIBS, attr->location, 1);
                    AfxAssertRange(SGL_MAX_VERTEX_ATTRIB_BINDINGS, attr->streamIdx, 1);
                    AfxAssertRange(SGL_MAX_VERTEX_ATTRIB_RELATIVE_OFFSET, attr->offset, 1);

                    afxNat location = attr->location;
                    afxNat srcIdx = attr->streamIdx;

                    GLint glsiz;
                    GLenum gltype;
                    GLuint glStride;
                    AfxToGlVertexFormat(attr->fmt, &glsiz, &gltype, &glStride);

                    //gl->BindAttribLocation(glHandle, location, AfxGetStringData(&(vsh->base.ioDecls[i].semantic), 0)); _SglThrowErrorOccuried();

                    AfxAssert(16 > location);  // max vertex attrib
                    gl->EnableVertexAttribArray(location); _SglThrowErrorOccuried();
                    AfxAssert(gl->BindVertexBuffer);
                    gl->VertexAttribFormat(location, glsiz, gltype, !!attr->normalized, attr->offset); _SglThrowErrorOccuried();
                    //afxNat srcIdx = streamIdx;// dpu->state.vertexInput.streams[streamIdx].srcIdx;
                    //AfxAssertRange(_SGL_MAX_VBO_PER_BIND, srcIdx, 1);
                    AfxAssertRange(SGL_MAX_VERTEX_ATTRIB_BINDINGS, srcIdx, 1);
                    gl->VertexAttribBinding(location, srcIdx); _SglThrowErrorOccuried();

                    afxNat instDivisor = vin->base.streams[srcIdx].instanceRate;

                    if (instDivisor)
                    {
                        gl->VertexAttribDivisor(location, instDivisor); _SglThrowErrorOccuried();
                    }
                }

                afxNat streamCnt = vin->base.streamCnt;

                for (afxNat i = 0; i < streamCnt; i++)
                {
                    afxVertexInputStream const* stream = &vin->base.streams[i];
                    AfxAssertRange(SGL_MAX_VERTEX_ATTRIB_BINDINGS, stream->slotIdx, 1);
                    //AfxAssertRange(SGL_MAX_VERTEX_ATTRIB_STRIDE, 0, stream->stride);
                    //gl->BindVertexBuffer(stream->slotIdx, 0, 0, stream->stride); _SglThrowErrorOccuried();
                }
            }
            else if ((vin->updFlags & SGL_UPD_FLAG_DEVICE_FLUSH))
            {
                AfxAssert(glHandle);
                AfxThrowError(); // can't be modified
            }
            vin->updFlags &= ~(SGL_UPD_FLAG_DEVICE);
        }

        gl->BindVertexArray(glHandle); _SglThrowErrorOccuried();

        //sglVertexInputState const* nextVinBindings = &dpu->nextVinBindings;
        afxMask updMask = nextVinBindings->streamUpdMask;

        for (afxNat i = 0; i < vin->base.streamCnt; i++)
        {
            afxVertexInputStream const* stream = &vin->base.streams[i];
            afxNat streamIdx = stream->slotIdx;

            afxNat32 stride = nextVinBindings->sources[streamIdx].stride;
            afxNat32 range = nextVinBindings->sources[streamIdx].range;
            afxNat32 offset = nextVinBindings->sources[streamIdx].offset;
            afxBuffer buf = nextVinBindings->sources[streamIdx].buf;
            afxBool rebind = FALSE;

            //if (updMask & AFX_BIT_OFFSET(streamIdx))
            {
                AfxAssertRange(SGL_MAX_VERTEX_ATTRIB_BINDINGS, streamIdx, 1);

                if ((vin->bindings.sources[streamIdx].buf != buf) ||
                    (vin->bindings.sources[streamIdx].offset != offset) ||
                    (vin->bindings.sources[streamIdx].range != range) ||
                    (vin->bindings.sources[streamIdx].stride != stride)
                    )
                {
                    vin->bindings.sources[streamIdx].buf = buf;
                    vin->bindings.sources[streamIdx].offset = offset;
                    vin->bindings.sources[streamIdx].range = range;
                    vin->bindings.sources[streamIdx].stride = stride;
                    rebind = TRUE;
                }
            }
            
            AfxAssert(gl->BindVertexBuffer);

            if (!buf)
            {
                gl->BindVertexBuffer(streamIdx, 0, 0, 0); _SglThrowErrorOccuried();
            }
            else
            {
                AfxAssert(stride);
                _SglBindAndSyncBuf(dpu, sglBindFlag_SYNC, GL_ARRAY_BUFFER, buf, offset, range, stride, /*buf->glUsage ? buf->glUsage : */GL_STATIC_DRAW);
                gl->BindVertexBuffer(streamIdx, buf->glHandle, offset, stride); _SglThrowErrorOccuried();
            }
        }

        //nextVinBindings->streamUpdMask = NIL;
        //nextVinBindings->streamUpdCnt = 0;
        
        afxBuffer buf = nextVinBindings->idxSrcBuf;
        afxNat32 off = nextVinBindings->idxSrcOff;
        afxNat32 range = nextVinBindings->idxSrcRange;
        afxNat32 idxSrcSiz = nextVinBindings->idxSrcSiz;
        afxBool rebind = FALSE;

        //if (nextVinBindings->iboUpdReq)
        {
            //nextVinBindings->iboUpdReq = FALSE;

            if (vin->bindings.idxSrcBuf != buf)
            {
                vin->bindings.idxSrcBuf = buf;
                rebind = TRUE;
            }                
            vin->bindings.idxSrcOff = off;
            vin->bindings.idxSrcRange = range;
            vin->bindings.idxSrcSiz = idxSrcSiz;
        }

        if (!buf)
        {
            gl->BindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        }
        else
        {
            _SglBindAndSyncBuf(dpu, sglBindFlag_SYNC | sglBindFlag_BIND | sglBindFlag_KEEP, GL_ELEMENT_ARRAY_BUFFER, buf, off, range, idxSrcSiz, /*buf->glUsage ? buf->glUsage : */GL_STATIC_DRAW);
            gl->BindBuffer(GL_ELEMENT_ARRAY_BUFFER, buf->glHandle);
        }
    }
    return err;
}

_SGL afxError _SglVinDtor(afxVertexInput vin)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vin, afxFcc_VIN);
    afxDrawContext dctx = AfxGetObjectProvider(vin);

    if (vin->glHandle)
    {
        _SglDctxDeleteGlRes(dctx, 7, (void*)vin->glHandle);
        vin->glHandle = 0;
    }
    return err;
}

_SGL afxError _SglVinCtor(afxVertexInput vin, afxCookie const* cookie)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vin, afxFcc_VIN);

    afxDrawContext dctx = cookie->udd[0];
    afxNat streamCnt = *(afxNat const*)cookie->udd[1];
    afxVertexInputStream const* streams = cookie->udd[2];
    afxNat attrCnt = *(afxNat const*)cookie->udd[3];
    afxVertexInputAttr const* attrs = cookie->udd[4];

    afxMmu mmu = AfxGetDrawContextMmu(dctx);
    AfxAssertObjects(1, &mmu, afxFcc_MMU);

    if (attrCnt && !(vin->base.attrs = AfxAllocate(attrCnt, sizeof(vin->base.attrs[0]), 0, AfxHint()))) AfxThrowError();
    else
    {
        for (afxNat i = 0; i < attrCnt; i++)
        {
            afxVertexInputAttr* attr = &vin->base.attrs[i];
            attr->fmt = attrs[i].fmt;
            attr->location = attrs[i].location;
            attr->offset = attrs[i].offset;
            attr->streamIdx = attrs[i].streamIdx;
            attr->normalized = !!attrs[i].normalized;
        }
        vin->base.attrCnt = attrCnt;

        if (streamCnt && !(vin->base.streams = AfxAllocate(streamCnt, sizeof(vin->base.attrs[0]), 0, AfxHint()))) AfxThrowError();
        else
        {
            for (afxNat i = 0; i < streamCnt; i++)
            {
                afxVertexInputStream* stream = &vin->base.streams[i];
                stream->slotIdx = streams[i].slotIdx;
                //stream->stride = streams[i].stride;
                stream->instanceRate = streams[i].instanceRate;
            }
            vin->base.streamCnt = streamCnt;

            vin->glHandle = 0;
            vin->updFlags = SGL_UPD_FLAG_DEVICE_INST;
        }
    }
    return err;
}

_SGL afxClassConfig const _SglVinClsConfig =
{
    .fcc = afxFcc_VIN,
    .name = "Vertex Input",
    .unitsPerPage = 2,
    .size = sizeof(AFX_OBJECT(afxVertexInput)),
    .mmu = NIL,
    .ctor = (void*)_SglVinCtor,
    .dtor = (void*)_SglVinDtor
};
