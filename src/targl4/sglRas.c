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

#include "qwadro/draw/afxDrawSystem.h"
#include "qwadro/exec/afxSystem.h"

_SGL afxError _SglTexSubImage(glVmt const* gl, GLenum glTarget, afxRasterRegion const* rgn, GLenum glFmt, GLenum glType, afxAddress const src)
{
    afxError err = NIL;

    switch (glTarget)
    {
    case GL_TEXTURE_1D_ARRAY:
    {
        gl->TexSubImage2D(glTarget, rgn->lodIdx, rgn->origin[0], rgn->origin[2], rgn->whd[0], rgn->whd[2], glFmt, glType, (void const*)src); _SglThrowErrorOccuried();
        break;
    }
    case GL_TEXTURE_2D:
    case GL_TEXTURE_CUBE_MAP_POSITIVE_X:
    case GL_TEXTURE_CUBE_MAP_NEGATIVE_X:
    case GL_TEXTURE_CUBE_MAP_POSITIVE_Y:
    case GL_TEXTURE_CUBE_MAP_NEGATIVE_Y:
    case GL_TEXTURE_CUBE_MAP_POSITIVE_Z:
    case GL_TEXTURE_CUBE_MAP_NEGATIVE_Z:
    {
        gl->TexSubImage2D(glTarget, rgn->lodIdx, rgn->origin[0], rgn->origin[1], rgn->whd[0], rgn->whd[1], glFmt, glType, (void const*)src); _SglThrowErrorOccuried();
        break;
    }
    case GL_TEXTURE_CUBE_MAP:
    {
        gl->TexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + rgn->origin[2], rgn->lodIdx, rgn->origin[0], rgn->origin[1], rgn->whd[0], rgn->whd[1], glFmt, glType, (void const*)src); _SglThrowErrorOccuried();
        break;
    }
    case GL_TEXTURE_1D:
    {
        gl->TexSubImage1D(glTarget, rgn->lodIdx, rgn->origin[0], rgn->whd[0], glFmt, glType, (void const*)src); _SglThrowErrorOccuried();
        break;
    }
    case GL_TEXTURE_3D:
    {
        gl->TexSubImage3D(glTarget, rgn->lodIdx, rgn->origin[0], rgn->origin[1], rgn->origin[2], rgn->whd[0], rgn->whd[1], rgn->whd[2], glFmt, glType, (void const*)src); _SglThrowErrorOccuried();
        break;
    }
    case GL_TEXTURE_2D_ARRAY:
    {
        gl->TexSubImage3D(glTarget, rgn->lodIdx, rgn->origin[0], rgn->origin[1], rgn->origin[2], rgn->whd[0], rgn->whd[1], rgn->whd[2], glFmt, glType, (void const*)src); _SglThrowErrorOccuried();
        break;
    }
    default:
    {
        AfxThrowError();
        break;
    }
    }
    return err;
}

# if 0
_SGL afxError _SglTexFlushDevice(glVmt const* gl, GLenum glTarget, afxRaster ras) // ras must be bound
{
    afxError err = AFX_ERR_NONE;

    //AfxAssert((ras->updFlags & SGL_UPD_FLAG_DEVICE_FLUSH) == SGL_UPD_FLAG_DEVICE_FLUSH);

    //afxWhd const xyz = { 0, 0, 0 };
    //afxWhd extent;
    //AfxImage.GetExtent(&ras->img, whd);

    //afxBool const isSurface = AfxTestRasterFlags(ras, afxRasterUsage_DRAW);
    afxBool const isCubemap = AfxTestRasterFlags(ras, afxRasterFlag_CUBEMAP);
    
    afxNat const lvlCnt = AfxCountRasterLods(ras);
    AfxAssert(lvlCnt);

    afxPixelLayout pfd;
    AfxDescribePixelFormat(ras->m.fmt, &pfd);
    GLint gpuUnpackAlign = 1;// (8 >= pfd.bpp ? 1 : ((16 >= pfd.bpp ? 2 : (pfd.bpp <= 32 ? 4 : (pfd.bpp <= 32))));

    afxRasterRegion rgn;
    afxNat rgnSize, rowSiz;
    void const* data;

    switch (glTarget)
    {
    case GL_TEXTURE_CUBE_MAP:
    {
        AfxAssert(isCubemap);
        AfxAssert(ras->m.layerCnt == 6);
        
        for (afxNat j = 0; j < ras->m.layerCnt; j++)
        {
            for (afxNat i = 0; i < lvlCnt; i++)
            {
                rgn.lodIdx = i;
                rgn.baseLayer = j;
                rgn.layerCnt = 1;
                rgn.offset[0] = ras->lastUpdOffset[0];
                rgn.offset[1] = ras->lastUpdOffset[1];
                rgn.offset[2] = ras->lastUpdOffset[2];
                rgn.whd[0] = ras->lastUpdRange[0];
                rgn.whd[1] = ras->lastUpdRange[1];
                rgn.whd[2] = ras->lastUpdRange[2];

                data = NIL;

                //if (!isSurface)
                    data = AfxMapRaster(ras, &rgn, afxRasterAccess_R, &rgnSize, &rowSiz);

                AfxAssert(data);
                gl->PixelStorei(GL_UNPACK_ALIGNMENT, gpuUnpackAlign);
                _SglTexSubImage(gl, GL_TEXTURE_CUBE_MAP, rgn.lodIdx, rgn.baseLayer, rgn.layerCnt, rgn.offset, rgn.whd, ras->glFmt, ras->glType, (afxAddress const)data);

                //if (!isSurface)
                    AfxUnmapRaster(ras, &rgn);
            }
        }
        break;
    }
    default:
    {
        for (afxNat i = ras->lastUpdLodBase; i < ras->lastUpdLodRange; i++)
        {
            for (afxNat j = ras->lastUpdImgBase; j < ras->lastUpdImgRange; j++)
            {
                rgn.lodIdx = i;
                rgn.baseLayer = j;
                rgn.layerCnt = 1;
                rgn.offset[0] = ras->lastUpdOffset[0];
                rgn.offset[1] = ras->lastUpdOffset[1];
                rgn.offset[2] = ras->lastUpdOffset[2];
                rgn.whd[0] = ras->lastUpdRange[0];
                rgn.whd[1] = ras->lastUpdRange[1];
                rgn.whd[2] = ras->lastUpdRange[2];

                data = NIL;

                //if (!isSurface)
                data = AfxMapRaster(ras, &rgn, afxRasterAccess_R, &rgnSize, &rowSiz);

                AfxAssert(data);
                gl->PixelStorei(GL_UNPACK_ALIGNMENT, gpuUnpackAlign);
                _SglTexSubImage(gl, glTarget, rgn.lodIdx, rgn.baseLayer, rgn.layerCnt, rgn.offset, rgn.whd, ras->glFmt, ras->glType, (afxAddress const)data);

                //if (!isSurface)
                AfxUnmapRaster(ras, &rgn);
            }
        }
        break;
    }
    }
    return err;
}
#endif

_SGL afxError DpuBindAndSyncRas(sglDpu* dpu, afxNat glUnit, afxRaster ras)
{
    //AfxEntry("img=%p", img);
    afxError err = AFX_ERR_NONE;
    glVmt const* gl = &dpu->gl;
    
    if (!ras)
    {
        gl->ActiveTexture(GL_TEXTURE0 + glUnit); _SglThrowErrorOccuried();
        gl->BindTexture(GL_TEXTURE_2D, 0); _SglThrowErrorOccuried();
    }
    else
    {
        AfxAssertObjects(1, &ras, afxFcc_RAS);
        sglUpdateFlags devUpdReq = (ras->updFlags & SGL_UPD_FLAG_DEVICE);
        GLuint glHandle = ras->glHandle;
        GLenum glTarget = ras->glTarget;            
        afxBool bound = FALSE;

        _SglThrowErrorOccuried();
        gl->ActiveTexture(GL_TEXTURE0 + glUnit); _SglThrowErrorOccuried();

        if ((!glHandle) || (devUpdReq & SGL_UPD_FLAG_DEVICE_INST))
        {
            if (glHandle)
            {
                AfxAssert(gl->IsTexture(glHandle));
                gl->DeleteTextures(1, &glHandle); _SglThrowErrorOccuried();
                ras->glHandle = NIL;
                glHandle = NIL;
            }

            gl->GenTextures(1, &(glHandle)); _SglThrowErrorOccuried();
            gl->BindTexture(glTarget, glHandle); _SglThrowErrorOccuried();
            AfxAssert(gl->IsTexture(glHandle));
            bound = TRUE;
            ras->glHandle = glHandle;

            GLenum glIntFmt = ras->glIntFmt;

            switch (glTarget)
            {
            case GL_TEXTURE_1D:
            case GL_PROXY_TEXTURE_1D:
            {
                AfxAssert(gl->TexStorage1D);
                gl->TexStorage1D(glTarget, AfxCountRasterLods(ras), glIntFmt, ras->m.whd[0]); _SglThrowErrorOccuried();
                break;
            }
            case GL_TEXTURE_2D:
            case GL_TEXTURE_RECTANGLE:
            case GL_TEXTURE_CUBE_MAP:
            case GL_PROXY_TEXTURE_2D:
            case GL_PROXY_TEXTURE_RECTANGLE:
            case GL_PROXY_TEXTURE_CUBE_MAP:
            {
                AfxAssert(gl->TexStorage2D);
                gl->TexStorage2D(glTarget, AfxCountRasterLods(ras), glIntFmt, ras->m.whd[0], ras->m.whd[1]); _SglThrowErrorOccuried();
                break;
            }
            case GL_TEXTURE_1D_ARRAY:
            case GL_PROXY_TEXTURE_1D_ARRAY:
            {
                AfxAssert(gl->TexStorage2D);
                gl->TexStorage2D(glTarget, AfxCountRasterLods(ras), glIntFmt, ras->m.whd[0], ras->m.whd[2]); _SglThrowErrorOccuried();
                break;
            }
            case GL_TEXTURE_3D:
            {
                AfxAssert(gl->TexStorage3D);
                gl->TexStorage3D(glTarget, AfxCountRasterLods(ras), glIntFmt, ras->m.whd[0], ras->m.whd[1], ras->m.whd[2]); _SglThrowErrorOccuried();
                break;
            }
            case GL_TEXTURE_2D_ARRAY:
            {
                AfxAssert(gl->TexStorage3D);
                gl->TexStorage3D(glTarget, AfxCountRasterLods(ras), glIntFmt, ras->m.whd[0], ras->m.whd[1], ras->m.whd[2]); _SglThrowErrorOccuried();
                break;
            }
            case GL_TEXTURE_2D_MULTISAMPLE:
            case GL_PROXY_TEXTURE_2D_MULTISAMPLE:
            {
                AfxAssert(gl->TexStorage2DMultisample);
                gl->TexStorage2DMultisample(glTarget, AfxCountRasterSamples(ras), glIntFmt, ras->m.whd[0], ras->m.whd[1], GL_FALSE); _SglThrowErrorOccuried();
                break;
            }
            case GL_TEXTURE_2D_MULTISAMPLE_ARRAY:
            case GL_PROXY_TEXTURE_2D_MULTISAMPLE_ARRAY:
            {
                AfxAssert(gl->TexStorage3DMultisample);
                gl->TexStorage3DMultisample(glTarget, AfxCountRasterSamples(ras), glIntFmt, ras->m.whd[0], ras->m.whd[1], ras->m.whd[2], GL_FALSE); _SglThrowErrorOccuried();
                break;
            }
            default: AfxThrowError(); break;
            }
            AfxLogEcho("Hardware-side raster %p ready. %x, %x, [%u,%u,%u]", ras, glTarget, glIntFmt, ras->m.whd[0], ras->m.whd[1], ras->m.whd[2]);

            if (!err)
                ras->updFlags &= ~(SGL_UPD_FLAG_DEVICE);
        }
        else
        {
            gl->BindTexture(glTarget, glHandle); _SglThrowErrorOccuried();
        }
    }
    return err;
}

_SGL afxError _DpuLoadRas(sglDpu* dpu, afxRaster ras, afxRasterIo const* op, afxByte const* src)
{
    afxError err = AFX_ERR_NONE;
    glVmt const* gl = &dpu->gl;
    DpuBindAndSyncBuf(dpu, GL_PIXEL_UNPACK_BUFFER, NIL);
    DpuBindAndSyncRas(dpu, SGL_LAST_COMBINED_TEXTURE_IMAGE_UNIT, ras);

    afxNat pixelStride = op->rowStride / op->rgn.whd[0];
    afxNat rowLen = op->rowStride / pixelStride;

    gl->PixelStorei(GL_UNPACK_ALIGNMENT, 1); _SglThrowErrorOccuried();
    gl->PixelStorei(GL_UNPACK_ROW_LENGTH, rowLen); _SglThrowErrorOccuried();
    gl->PixelStorei(GL_UNPACK_IMAGE_HEIGHT, op->rowCnt); _SglThrowErrorOccuried();
    _SglTexSubImage(gl, ras->glTarget, &op->rgn, ras->glFmt, ras->glType, (afxAddress const)&(src[op->offset]));
    return err;
}

_SGL afxError _DpuStoreRas(sglDpu* dpu, afxRaster ras, afxRasterIo const* op, afxByte* dst)
{
    afxError err = AFX_ERR_NONE;
    glVmt const* gl = &dpu->gl;
    //DpuBindAndSyncBuf(dpu, GL_PIXEL_PACK_BUFFER, NIL);
    DpuBindAndSyncRas(dpu, SGL_LAST_COMBINED_TEXTURE_IMAGE_UNIT, ras);

    afxNat bufSiz = op->rowStride  * op->rowCnt * op->rgn.whd[2];
    afxNat pixelStride = op->rowStride / op->rgn.whd[0];
    afxNat rowLen = op->rowStride / pixelStride;

    GLuint pbo;
    GLenum pboTarget = GL_PIXEL_PACK_BUFFER;
    gl->GenBuffers(1, &pbo); _SglThrowErrorOccuried();
    gl->BindBuffer(pboTarget, pbo); _SglThrowErrorOccuried();
    gl->BufferStorage(pboTarget, bufSiz, NIL, GL_DYNAMIC_STORAGE_BIT | GL_MAP_READ_BIT); _SglThrowErrorOccuried();

    gl->PixelStorei(GL_PACK_ALIGNMENT, 1); _SglThrowErrorOccuried();
    gl->PixelStorei(GL_PACK_ROW_LENGTH, rowLen); _SglThrowErrorOccuried();
    gl->PixelStorei(GL_PACK_IMAGE_HEIGHT, op->rowCnt); _SglThrowErrorOccuried();

    afxBool is3d = AfxRasterIs3d(ras);

    gl->BindFramebuffer(GL_READ_FRAMEBUFFER, dpu->fboOpSrc); _SglThrowErrorOccuried();
    _SglBindFboAttachment(&dpu->gl, GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, ras->glTarget, ras->glHandle, op->rgn.lodIdx, is3d ? 0 : op->rgn.origin[2], is3d ? op->rgn.origin[2] : 0);
    gl->ReadBuffer(GL_COLOR_ATTACHMENT0); _SglThrowErrorOccuried();
    gl->ReadPixels(op->rgn.origin[0], op->rgn.origin[1], op->rgn.whd[0], op->rgn.whd[1], ras->glFmt, ras->glType, NIL); _SglThrowErrorOccuried();

    gl->GetBufferSubData(pboTarget, 0, bufSiz, &(dst[op->offset])); _SglThrowErrorOccuried();
    gl->BindBuffer(pboTarget, 0); _SglThrowErrorOccuried();
    gl->DeleteBuffers(1, &pbo); _SglThrowErrorOccuried();
    return err;
}

_SGL afxError _DpuUnpackRas(sglDpu* dpu, afxRaster ras, afxRasterIo const* op, afxBuffer buf)
{
    afxError err = AFX_ERR_NONE;
    glVmt const* gl = &dpu->gl;
    DpuBindAndSyncBuf(dpu, GL_PIXEL_UNPACK_BUFFER, buf);
    DpuBindAndSyncRas(dpu, SGL_LAST_COMBINED_TEXTURE_IMAGE_UNIT, ras);

    afxNat pixelStride = op->rowStride / op->rgn.whd[0];
    afxNat rowLen = op->rowStride / pixelStride;

    gl->PixelStorei(GL_UNPACK_ALIGNMENT, 1); _SglThrowErrorOccuried();
    gl->PixelStorei(GL_UNPACK_ROW_LENGTH, rowLen); _SglThrowErrorOccuried();
    gl->PixelStorei(GL_UNPACK_IMAGE_HEIGHT, op->rowCnt); _SglThrowErrorOccuried();
    _SglTexSubImage(gl, ras->glTarget, &op->rgn, ras->glFmt, ras->glType, (afxAddress const)op->offset);
    return err;
}

_SGL afxError _DpuPackRas(sglDpu* dpu, afxRaster ras, afxRasterIo const* op, afxBuffer buf)
{
    afxError err = AFX_ERR_NONE;
    glVmt const* gl = &dpu->gl;
    DpuBindAndSyncBuf(dpu, GL_PIXEL_PACK_BUFFER, buf);
    DpuBindAndSyncRas(dpu, SGL_LAST_COMBINED_TEXTURE_IMAGE_UNIT, ras);

    afxNat bufSiz = op->rowStride  * op->rowCnt * op->rgn.whd[2];
    afxNat pixelStride = op->rowStride / op->rgn.whd[0];
    afxNat rowLen = op->rowStride / pixelStride;

    gl->PixelStorei(GL_PACK_ALIGNMENT, 1); _SglThrowErrorOccuried();
    gl->PixelStorei(GL_PACK_ROW_LENGTH, rowLen); _SglThrowErrorOccuried();
    gl->PixelStorei(GL_PACK_IMAGE_HEIGHT, op->rowCnt); _SglThrowErrorOccuried();

    afxBool is3d = AfxRasterIs3d(ras);

    gl->BindFramebuffer(GL_READ_FRAMEBUFFER, dpu->fboOpSrc); _SglThrowErrorOccuried();
    _SglBindFboAttachment(&dpu->gl, GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, ras->glTarget, ras->glHandle, op->rgn.lodIdx, is3d ? 0 : op->rgn.origin[2], is3d ? op->rgn.origin[2] : 0);
    gl->ReadBuffer(GL_COLOR_ATTACHMENT0); _SglThrowErrorOccuried();
    gl->ReadPixels(op->rgn.origin[0], op->rgn.origin[1], op->rgn.whd[0], op->rgn.whd[1], ras->glFmt, ras->glType, (void*)op->offset); _SglThrowErrorOccuried();

    //GLenum pboTarget = GL_PIXEL_PACK_BUFFER;
    //gl->GetBufferSubData(pboTarget, 0, bufSiz, (void*)op->offset); _SglThrowErrorOccuried();

    return err;
}

_SGL afxError _DpuOutputRas(sglDpu* dpu, afxRaster ras, afxRasterIo const* op, afxStream out)
{
    afxError err = AFX_ERR_NONE;
    glVmt const* gl = &dpu->gl;
    DpuBindAndSyncRas(dpu, SGL_LAST_COMBINED_TEXTURE_IMAGE_UNIT, ras);

    afxNat bufSiz = op->rowStride  * op->rowCnt * op->rgn.whd[2];
    afxNat pixelStride = op->rowStride / op->rgn.whd[0];
    afxNat rowLen = op->rowStride / pixelStride;

    GLuint pbo;
    GLenum pboTarget = GL_PIXEL_PACK_BUFFER;
    gl->GenBuffers(1, &pbo); _SglThrowErrorOccuried();
    gl->BindBuffer(pboTarget, pbo); _SglThrowErrorOccuried();
    gl->BufferStorage(pboTarget, bufSiz, NIL, GL_DYNAMIC_STORAGE_BIT | GL_MAP_READ_BIT); _SglThrowErrorOccuried();

    gl->PixelStorei(GL_PACK_ALIGNMENT, 1); _SglThrowErrorOccuried();
    gl->PixelStorei(GL_PACK_ROW_LENGTH, rowLen); _SglThrowErrorOccuried();
    gl->PixelStorei(GL_PACK_IMAGE_HEIGHT, op->rowCnt); _SglThrowErrorOccuried();

    afxBool is3d = AfxRasterIs3d(ras);

    gl->BindFramebuffer(GL_READ_FRAMEBUFFER, dpu->fboOpSrc); _SglThrowErrorOccuried();
    _SglBindFboAttachment(&dpu->gl, GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, ras->glTarget, ras->glHandle, op->rgn.lodIdx, is3d ? 0 : op->rgn.origin[2], is3d ? op->rgn.origin[2] : 0);
    gl->ReadBuffer(GL_COLOR_ATTACHMENT0); _SglThrowErrorOccuried();
    gl->ReadPixels(op->rgn.origin[0], op->rgn.origin[1], op->rgn.whd[0], op->rgn.whd[1], ras->glFmt, ras->glType, NIL); _SglThrowErrorOccuried();

    afxByte* bytemap = gl->MapBuffer(pboTarget, GL_READ_ONLY); _SglThrowErrorOccuried();
    AfxWriteStreamAt(out, op->offset, bufSiz, 0, bytemap);
    gl->UnmapBuffer(pboTarget); _SglThrowErrorOccuried();
    gl->BindBuffer(pboTarget, 0); _SglThrowErrorOccuried();
    gl->DeleteBuffers(1, &pbo); _SglThrowErrorOccuried();
    return err;
}

_SGL afxError _DpuInputRas(sglDpu* dpu, afxRaster ras, afxRasterIo const* op, afxStream in)
{
    afxError err = AFX_ERR_NONE;
    glVmt const* gl = &dpu->gl;
    DpuBindAndSyncRas(dpu, SGL_LAST_COMBINED_TEXTURE_IMAGE_UNIT, ras);

    afxNat bufSiz = op->rowStride  * op->rowCnt * op->rgn.whd[2];
    afxNat pixelStride = op->rowStride / op->rgn.whd[0];
    afxNat rowLen = op->rowStride / pixelStride;

    GLuint pbo;
    GLenum pboTarget = GL_PIXEL_UNPACK_BUFFER;
    gl->GenBuffers(1, &pbo); _SglThrowErrorOccuried();
    gl->BindBuffer(pboTarget, pbo); _SglThrowErrorOccuried();
    gl->BufferStorage(pboTarget, bufSiz, NIL, GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT); _SglThrowErrorOccuried();
    afxByte* bytemap = gl->MapBuffer(pboTarget, GL_WRITE_ONLY); _SglThrowErrorOccuried();
    AfxReadStreamAt(in, op->offset, bufSiz, 0, bytemap);
    gl->UnmapBuffer(pboTarget); _SglThrowErrorOccuried();
    gl->PixelStorei(GL_UNPACK_ALIGNMENT, 1); _SglThrowErrorOccuried();
    gl->PixelStorei(GL_UNPACK_ROW_LENGTH, rowLen); _SglThrowErrorOccuried();
    gl->PixelStorei(GL_UNPACK_IMAGE_HEIGHT, op->rowCnt); _SglThrowErrorOccuried();
    _SglTexSubImage(gl, ras->glTarget, &op->rgn, ras->glFmt, ras->glType, NIL);
    gl->BindBuffer(pboTarget, 0); _SglThrowErrorOccuried();
    gl->DeleteBuffers(1, &pbo); _SglThrowErrorOccuried();
    return err;
}

_SGL afxError _DpuCopyRas(sglDpu* dpu, afxRaster src, afxRaster dst, afxNat opCnt, afxRasterCopy const ops[])
{
    afxError err = AFX_ERR_NONE;
    glVmt const* gl = &dpu->gl;
    AfxAssertObjects(1, &dst, afxFcc_RAS);
    AfxAssertObjects(1, &src, afxFcc_RAS);
    DpuBindAndSyncRas(dpu, SGL_LAST_COMBINED_TEXTURE_IMAGE_UNIT, src);
    DpuBindAndSyncRas(dpu, SGL_LAST_COMBINED_TEXTURE_IMAGE_UNIT - 1, dst);
    _SglCopyTexSubImage(dpu, dst->glTarget, src->glTarget, src->glHandle, opCnt, ops);
    return err;
}

_SGL afxError _SglRasDtor(afxRaster ras)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);

    afxDrawContext dctx = AfxGetObjectProvider(ras);

    if (ras->glHandle)
    {
        _SglDctxDeleteGlRes(dctx, 1, (void*)ras->glHandle);
        ras->glHandle = 0;
    }

    if (_AvxRasStdImplementation.dtor(ras))
        AfxThrowError();

    return err;
}

_SGL afxError _SglRasCtor(afxRaster ras, afxCookie const* cookie)
{
    afxError err = AFX_ERR_NONE;

    if (_AvxRasStdImplementation.ctor(ras, cookie)) AfxThrowError();
    else
    {
        ras->updFlags = SGL_UPD_FLAG_DEVICE_INST;

        GLint glIntFmt;
        SglDetermineGlTargetInternalFormatType(ras, &ras->glTarget, &ras->glIntFmt, &ras->glFmt, &ras->glType);
        
        if (err && _AvxRasStdImplementation.dtor(ras))
            AfxThrowError();
    }
    return err;
}
