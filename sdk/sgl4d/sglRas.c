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

#include "qwadro/draw/afxDrawSystem.h"
#include "qwadro/core/afxSystem.h"

_SGL afxError _SglTexSubImage(glVmt const* gl, GLenum glTarget, GLint level, afxNat baseLayer, afxNat layerCnt, afxWhd const xyz, afxWhd const whd, GLenum glFmt, GLenum glType, afxAddress const src)
{
    afxError err = NIL;

    switch (glTarget)
    {
    case GL_TEXTURE_1D_ARRAY:
    {
        gl->TexSubImage2D(glTarget, level, xyz[0], baseLayer, whd[0], layerCnt, glFmt, glType, (void const*)src); _SglThrowErrorOccuried();
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
        gl->TexSubImage2D(glTarget, level, xyz[0], xyz[1], whd[0], whd[1], glFmt, glType, (void const*)src); _SglThrowErrorOccuried();
        break;
    }
    case GL_TEXTURE_CUBE_MAP:
    {
        gl->TexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + baseLayer, level, xyz[0], xyz[1], whd[0], whd[1], glFmt, glType, (void const*)src); _SglThrowErrorOccuried();
        break;
    }
    case GL_TEXTURE_1D:
    {
        gl->TexSubImage1D(glTarget, level, xyz[0], whd[0], glFmt, glType, (void const*)src); _SglThrowErrorOccuried();
        break;
    }
    case GL_TEXTURE_3D:
    {
        gl->TexSubImage3D(glTarget, level, xyz[0], xyz[1], xyz[2], whd[0], whd[1], whd[2], glFmt, glType, (void const*)src); _SglThrowErrorOccuried();
        break;
    }
    case GL_TEXTURE_2D_ARRAY:
    {
        gl->TexSubImage3D(glTarget, level, xyz[0], xyz[1], baseLayer, whd[0], whd[1], layerCnt, glFmt, glType, (void const*)src); _SglThrowErrorOccuried();
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
    AfxDescribePixelFormat(ras->base.fmt, &pfd);
    GLint gpuUnpackAlign = 1;// (8 >= pfd.bpp ? 1 : ((16 >= pfd.bpp ? 2 : (pfd.bpp <= 32 ? 4 : (pfd.bpp <= 32))));

    afxRasterRegion rgn;
    afxNat rgnSize, rowSiz;
    void const* data;

    switch (glTarget)
    {
    case GL_TEXTURE_CUBE_MAP:
    {
        AfxAssert(isCubemap);
        AfxAssert(ras->base.layerCnt == 6);
        
        for (afxNat j = 0; j < ras->base.layerCnt; j++)
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

_SGL afxError DpuBindAndSyncRas(sglDpu* dpu, sglBindFlags bindFlags, afxNat glUnit, afxRaster ras)
{
    //AfxEntry("img=%p", img);
    afxError err = AFX_ERR_NONE;
    glVmt const* gl = &dpu->gl;
    
    if (!ras)
    {
        if (bindFlags & sglBindFlag_BIND)
        {
            gl->ActiveTexture(GL_TEXTURE0 + glUnit); _SglThrowErrorOccuried();
            gl->BindTexture(GL_TEXTURE_2D, 0); _SglThrowErrorOccuried();
        }
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

            GLint glIntFmt;
            SglDetermineGlTargetInternalFormatType(ras, &glTarget, &glIntFmt, &ras->glFmt, &ras->glType);
            ras->glTarget = glTarget;
            ras->glIntFmt = glIntFmt;

            gl->GenTextures(1, &(glHandle)); _SglThrowErrorOccuried();
            gl->BindTexture(glTarget, glHandle); _SglThrowErrorOccuried();
            AfxAssert(gl->IsTexture(glHandle));
            bound = TRUE;
            ras->glHandle = glHandle;
                    
            switch (glTarget)
            {
            case GL_TEXTURE_1D:
            case GL_PROXY_TEXTURE_1D:
            {
                AfxAssert(gl->TexStorage1D);
                gl->TexStorage1D(glTarget, AfxCountRasterLods(ras), glIntFmt, ras->base.whd[0]); _SglThrowErrorOccuried();
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
                gl->TexStorage2D(glTarget, AfxCountRasterLods(ras), glIntFmt, ras->base.whd[0], ras->base.whd[1]); _SglThrowErrorOccuried();
                break;
            }
            case GL_TEXTURE_1D_ARRAY:
            case GL_PROXY_TEXTURE_1D_ARRAY:
            {
                AfxAssert(gl->TexStorage2D);
                gl->TexStorage2D(glTarget, AfxCountRasterLods(ras), glIntFmt, ras->base.whd[0], ras->base.layerCnt); _SglThrowErrorOccuried();
                break;
            }
            case GL_TEXTURE_3D:
            {
                AfxAssert(gl->TexStorage3D);
                gl->TexStorage3D(glTarget, AfxCountRasterLods(ras), glIntFmt, ras->base.whd[0], ras->base.whd[1], ras->base.whd[2]); _SglThrowErrorOccuried();
                break;
            }
            case GL_TEXTURE_2D_ARRAY:
            {
                AfxAssert(gl->TexStorage3D);
                gl->TexStorage3D(glTarget, AfxCountRasterLods(ras), glIntFmt, ras->base.whd[0], ras->base.whd[1], ras->base.layerCnt); _SglThrowErrorOccuried();
                break;
            }
            case GL_TEXTURE_2D_MULTISAMPLE:
            case GL_PROXY_TEXTURE_2D_MULTISAMPLE:
            {
                AfxAssert(gl->TexStorage2DMultisample);
                gl->TexStorage2DMultisample(glTarget, AfxCountRasterSamples(ras), glIntFmt, ras->base.whd[0], ras->base.whd[1], GL_FALSE); _SglThrowErrorOccuried();
                break;
            }
            case GL_TEXTURE_2D_MULTISAMPLE_ARRAY:
            case GL_PROXY_TEXTURE_2D_MULTISAMPLE_ARRAY:
            {
                AfxAssert(gl->TexStorage3DMultisample);
                gl->TexStorage3DMultisample(glTarget, AfxCountRasterSamples(ras), glIntFmt, ras->base.whd[0], ras->base.whd[1], ras->base.layerCnt, GL_FALSE); _SglThrowErrorOccuried();
                break;
            }
            default: AfxThrowError(); break;
            }
            AfxLogEcho("afxRaster %p hardware-side data instanced. Target %x, format %x, whd [%u,%u,%u]", ras, glTarget, glIntFmt, ras->base.whd[0], ras->base.whd[1], ras->base.whd[2]);
        }

        if ((devUpdReq & SGL_UPD_FLAG_DEVICE_FLUSH))
        {
            if (!bound)
            {
                AfxAssert(gl->IsTexture(glHandle));
                gl->BindTexture(glTarget, glHandle); _SglThrowErrorOccuried();
                AfxAssert(ras->glHandle == glHandle);
                bound = TRUE;
            }

#if 0
            _SglTexFlushDevice(gl, glTarget, ras);
            ras->lastUpdOffset[0] = 0;
            ras->lastUpdOffset[1] = 0;
            ras->lastUpdOffset[2] = 0;
            ras->lastUpdRange[0] = 0;
            ras->lastUpdRange[1] = 0;
            ras->lastUpdRange[2] = 0;
#endif
        }

        if (bound && !(bindFlags & sglBindFlag_KEEP))
        {
            gl->BindTexture(glTarget, 0); _SglThrowErrorOccuried();
        }

        if (!err)
            ras->updFlags &= ~(SGL_UPD_FLAG_DEVICE);
    }
    return err;
}

_SGL afxError _RasDtor(afxRaster ras)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);

    afxDrawContext dctx = AfxGetObjectProvider(ras);

    if (ras->glHandle)
    {
        _SglDctxDeleteGlRes(dctx, 1, (void*)ras->glHandle);
        ras->glHandle = 0;
    }

    return err;
}

_SGL afxError _RasCtor(afxRaster ras, afxCookie const* cookie)
{
    afxError err = AFX_ERR_NONE;

    afxRasterInfo const *texi = ((afxRasterInfo const *)cookie->udd[1]) + cookie->no;

    afxDrawContext dctx = AfxGetObjectProvider(ras);

    ras->base.swizzling = NIL; //&AFX_STD_COLOR_SWIZZLING;
    ras->base.sampleCnt = AfxMax(texi->sampleCnt, 1);
    //ras->base.linearlyTiled = FALSE;

    ras->base.layerCnt = AfxMax(texi->layerCnt, 1);
    ras->base.lodCnt = AfxMax(texi->lodCnt, 1);

    ras->base.flags = texi->flags;
    ras->base.usage = texi->usage;

    ras->base.fmt = texi->fmt;

    ras->base.whd[0] = AfxMax(texi->whd[0], 1);
    ras->base.whd[1] = AfxMax(texi->whd[1], 1);
    ras->base.whd[2] = AfxMax(texi->whd[2], 1);

    if (ras->base.whd[1] > 1)
    {
        if (ras->base.whd[2] > 1)
            ras->base.flags |= afxRasterFlag_3D;
        else
            ras->base.flags |= afxRasterFlag_2D;
    }
    else
    {
        if (ras->base.whd[2] > 1)
            ras->base.flags |= afxRasterFlag_3D;
        else
            ras->base.flags |= afxRasterFlag_1D;
    }

    if (ras->base.layerCnt > 1)
        ras->base.flags |= afxRasterFlag_LAYERED;

    if (ras->base.lodCnt > 1)
        ras->base.flags |= afxRasterFlag_SUBSAMPLED;

    //ras->idd = NIL;

    if (!err)
    {
        ras->updFlags = SGL_UPD_FLAG_DEVICE_INST;

        //ras->lastUpdImgRange = AfxCountRasterLayers(ras);
        //ras->lastUpdLodRange = AfxCountRasterLods(ras);
        //AfxGetRasterExtent(ras, 0, ras->lastUpdRange);
    }

    return err;
}

_SGL afxClassConfig const _SglRasMgrCfg =
{
    .fcc = afxFcc_RAS,
    .name = "Raster",
    .unitsPerPage = 2,
    .size = sizeof(AFX_OBJECT(afxRaster)),
    .mmu = NIL,
    .ctor = (void*)_RasCtor,
    .dtor = (void*)_RasDtor
};
