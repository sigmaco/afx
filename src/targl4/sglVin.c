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

#include "qwadro/io/afxXml.h"
#include "sgl.h"

#include "qwadro/draw/pipe/afxPipeline.h"

#include "qwadro/draw/afxDrawSystem.h"
#include "qwadro/io/afxUri.h"
#include "qwadro/core/afxSystem.h"

_SGL afxError _DpuBindAndSyncVin(sglDpu* dpu, afxVertexInput vin, sglVertexInputState* nextVinBindings)
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
            vin->updFlags &= ~(SGL_UPD_FLAG_DEVICE);
        }
        else
        {
            gl->BindVertexArray(glHandle); _SglThrowErrorOccuried();
        }

        //sglVertexInputState const* nextVinBindings = &dpu->nextVinBindings;
        afxMask updMask = nextVinBindings->streamUpdMask;

        //if (updMask)
        {
            for (afxNat i = 0; i < vin->base.streamCnt; i++)
            {
                afxVertexInputStream const* stream = &vin->base.streams[i];
                afxNat streamIdx = stream->slotIdx;
                AfxAssertRange(SGL_MAX_VERTEX_ATTRIB_BINDINGS, streamIdx, 1);
                
                afxNat32 stride = nextVinBindings->sources[streamIdx].stride;
                afxNat32 range = nextVinBindings->sources[streamIdx].range;
                afxNat32 offset = nextVinBindings->sources[streamIdx].offset;
                afxBuffer buf = nextVinBindings->sources[streamIdx].buf;
                afxBool rebind = FALSE;

                if (updMask & AFX_BIT(streamIdx))
                {
                    

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
                    gl->BindVertexBuffer(streamIdx, 0, 0, 16); _SglThrowErrorOccuried();
                }
                else
                {
                    AfxAssert(range);
                    AfxAssert(stride);
                    DpuBindAndSyncBuf(dpu, GL_ARRAY_BUFFER, buf);
                    gl->BindVertexBuffer(streamIdx, buf->glHandle, offset, stride); _SglThrowErrorOccuried();
                }
            }
        }

        afxBool rebind = FALSE;
        afxBuffer buf = nextVinBindings->idxSrcBuf;

        if (!buf)
        {
            gl->BindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
            vin->bindings.idxSrcBuf = NIL;
            vin->bindings.idxSrcOff = 0;
            vin->bindings.idxSrcRange = 0;
            vin->bindings.idxSrcSiz = 0;
        }
        else
        {
            afxNat32 off = nextVinBindings->idxSrcOff;
            afxNat32 range = nextVinBindings->idxSrcRange;
            afxNat32 idxSrcSiz = nextVinBindings->idxSrcSiz;

            DpuBindAndSyncBuf(dpu, GL_ELEMENT_ARRAY_BUFFER, buf);
            gl->BindBuffer(GL_ELEMENT_ARRAY_BUFFER, buf->glHandle);
            
            vin->bindings.idxSrcBuf = buf;
            vin->bindings.idxSrcOff = off;
            vin->bindings.idxSrcRange = range;
            vin->bindings.idxSrcSiz = idxSrcSiz;
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

    if (_AvxVinStdImplementation.dtor(vin))
        AfxThrowError();

    return err;
}

_SGL afxError _SglVinCtor(afxVertexInput vin, afxCookie const* cookie)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vin, afxFcc_VIN);

    if (_AvxVinStdImplementation.ctor(vin, cookie)) AfxThrowError();
    else
    {
        vin->glHandle = 0;
        vin->updFlags = SGL_UPD_FLAG_DEVICE_INST;

        if (err && _AvxVinStdImplementation.dtor(vin))
            AfxThrowError();
    }
    return err;
}

_SGL afxClassConfig const _SglVinMgrCfg =
{
    .fcc = afxFcc_VIN,
    .name = "Vertex Input",
    .unitsPerPage = 2,
    .size = sizeof(AFX_OBJECT(afxVertexInput)),
    .ctor = (void*)_SglVinCtor,
    .dtor = (void*)_SglVinDtor
};
