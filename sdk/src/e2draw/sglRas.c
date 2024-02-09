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

////////////////////////////////////////////////////////////////////////////////
// IMAGE                                                                      //
////////////////////////////////////////////////////////////////////////////////
/*
Well-defined sized internal formats glTexStorage

GL_R8	GL_RED	8				
GL_R8_SNORM	GL_RED	s8				
GL_R16	GL_RED	16				
GL_R16_SNORM	GL_RED	s16				
GL_RG8	GL_RG	8	8			
GL_RG8_SNORM	GL_RG	s8	s8			
GL_RG16	GL_RG	16	16			
GL_RG16_SNORM	GL_RG	s16	s16			
GL_R3_G3_B2	GL_RGB	3	3	2		
GL_RGB4	GL_RGB	4	4	4		
GL_RGB5	GL_RGB	5	5	5		
GL_RGB8	GL_RGB	8	8	8		
GL_RGB8_SNORM	GL_RGB	s8	s8	s8		
GL_RGB10	GL_RGB	10	10	10		
GL_RGB12	GL_RGB	12	12	12		
GL_RGB16_SNORM	GL_RGB	16	16	16		
GL_RGBA2	GL_RGB	2	2	2	2	
GL_RGBA4	GL_RGB	4	4	4	4	
GL_RGB5_A1	GL_RGBA	5	5	5	1	
GL_RGBA8	GL_RGBA	8	8	8	8	
GL_RGBA8_SNORM	GL_RGBA	s8	s8	s8	s8	
GL_RGB10_A2	GL_RGBA	10	10	10	2	
GL_RGB10_A2UI	GL_RGBA	ui10	ui10	ui10	ui2	
GL_RGBA12	GL_RGBA	12	12	12	12	
GL_RGBA16	GL_RGBA	16	16	16	16	
GL_SRGB8	GL_RGB	8	8	8		
GL_SRGB8_ALPHA8	GL_RGBA	8	8	8	8	
GL_R16F	GL_RED	f16				
GL_RG16F	GL_RG	f16	f16			
GL_RGB16F	GL_RGB	f16	f16	f16		
GL_RGBA16F	GL_RGBA	f16	f16	f16	f16	
GL_R32F	GL_RED	f32				
GL_RG32F	GL_RG	f32	f32			
GL_RGB32F	GL_RGB	f32	f32	f32		
GL_RGBA32F	GL_RGBA	f32	f32	f32	f32	
GL_R11F_G11F_B10F	GL_RGB	f11	f11	f10		
GL_RGB9_E5	GL_RGB	9	9	9		5
GL_R8I	GL_RED	i8				
GL_R8UI	GL_RED	ui8				
GL_R16I	GL_RED	i16				
GL_R16UI	GL_RED	ui16				
GL_R32I	GL_RED	i32				
GL_R32UI	GL_RED	ui32				
GL_RG8I	GL_RG	i8	i8			
GL_RG8UI	GL_RG	ui8	ui8			
GL_RG16I	GL_RG	i16	i16			
GL_RG16UI	GL_RG	ui16	ui16			
GL_RG32I	GL_RG	i32	i32			
GL_RG32UI	GL_RG	ui32	ui32			
GL_RGB8I	GL_RGB	i8	i8	i8		
GL_RGB8UI	GL_RGB	ui8	ui8	ui8		
GL_RGB16I	GL_RGB	i16	i16	i16		
GL_RGB16UI	GL_RGB	ui16	ui16	ui16		
GL_RGB32I	GL_RGB	i32	i32	i32		
GL_RGB32UI	GL_RGB	ui32	ui32	ui32		
GL_RGBA8I	GL_RGBA	i8	i8	i8	i8	
GL_RGBA8UI	GL_RGBA	ui8	ui8	ui8	ui8	
GL_RGBA16I	GL_RGBA	i16	i16	i16	i16	
GL_RGBA16UI	GL_RGBA	ui16	ui16	ui16	ui16	
GL_RGBA32I	GL_RGBA	i32	i32	i32	i32	
GL_RGBA32UI	GL_RGBA	ui32	ui32	ui32	ui32	

*/

_SGL void SglToGlFormat(afxPixelFormat pf, GLint *fmt, GLenum *layout, GLenum *type)
{
    static struct { GLint intFmt; GLenum layout, type; } const v[] =
    {
        {   NIL,                    NIL,                NIL                     },

        {   GL_R8,                 GL_RED,             GL_UNSIGNED_BYTE        },
        {   GL_RG8,                  GL_RG,              GL_UNSIGNED_BYTE        },
        {   GL_RGB8,                 GL_RGB,             GL_UNSIGNED_BYTE        },
        {   GL_RGBA8,                GL_RGBA,            GL_UNSIGNED_BYTE        },

        {   GL_RG8,                  GL_RG,              GL_UNSIGNED_BYTE        },
        {   GL_RGB8,                 GL_BGR,             GL_UNSIGNED_BYTE        },
        {   GL_RGBA8,                GL_BGRA,            GL_UNSIGNED_BYTE        },

        {   GL_R32F,                GL_RED,             GL_FLOAT                },
        {   GL_RG32F,               GL_RG,              GL_FLOAT                },
        {   GL_RGB32F,              GL_RGB,             GL_FLOAT                },
        {   GL_RGBA32F,             GL_RGBA,            GL_FLOAT                },

        {   GL_SRGB8,               GL_RGB,             GL_UNSIGNED_BYTE            },
        {   GL_SRGB8_ALPHA8,        GL_RGBA,            GL_UNSIGNED_BYTE            },
        {   GL_RGB9_E5,             GL_RGB,             GL_UNSIGNED_INT_5_9_9_9_REV },

        {   GL_RGBA4,               GL_RGBA,            GL_UNSIGNED_SHORT_4_4_4_4   },
        {   GL_RGB5_A1,             GL_RGBA,            GL_UNSIGNED_SHORT_5_5_5_1   },
        {   GL_RGB565,              GL_RGB,             GL_UNSIGNED_SHORT_5_6_5     },
        {   GL_RGB10_A2,            GL_RGBA,            GL_UNSIGNED_INT_10_10_10_2  },

        {   GL_STENCIL_INDEX8,      GL_STENCIL_INDEX,   GL_UNSIGNED_BYTE        },
        {   GL_DEPTH_COMPONENT16,   GL_DEPTH_COMPONENT, GL_UNSIGNED_SHORT       },
        {   GL_DEPTH_COMPONENT24,   GL_DEPTH_COMPONENT, GL_UNSIGNED_INT         },
        {   GL_DEPTH_COMPONENT32,   GL_DEPTH_COMPONENT, GL_UNSIGNED_INT         },
        {   GL_DEPTH_COMPONENT32F,  GL_DEPTH_COMPONENT, GL_FLOAT                },
        {   GL_DEPTH24_STENCIL8,    GL_DEPTH_STENCIL,   GL_UNSIGNED_INT_24_8    },
        {   GL_DEPTH32F_STENCIL8,   GL_DEPTH_STENCIL,   GL_UNSIGNED_INT_24_8    },

        {  GL_COMPRESSED_RGB_S3TC_DXT1_EXT, GL_COMPRESSED_RGB_S3TC_DXT1_EXT, GL_COMPRESSED_RGB_S3TC_DXT1_EXT },
        {  GL_COMPRESSED_RGBA_S3TC_DXT1_EXT, GL_COMPRESSED_RGBA_S3TC_DXT1_EXT, GL_COMPRESSED_RGBA_S3TC_DXT1_EXT },
        {  GL_COMPRESSED_RGBA_S3TC_DXT3_EXT, GL_COMPRESSED_RGBA_S3TC_DXT3_EXT, GL_COMPRESSED_RGBA_S3TC_DXT3_EXT },
        {  GL_COMPRESSED_RGBA_S3TC_DXT5_EXT, GL_COMPRESSED_RGBA_S3TC_DXT5_EXT, GL_COMPRESSED_RGBA_S3TC_DXT5_EXT },
    };
    
    afxError err = NIL;
    AfxAssert(v[afxPixelFormat_D32F].intFmt == GL_DEPTH_COMPONENT32F);
    AfxAssert(v[afxPixelFormat_D32].intFmt == GL_DEPTH_COMPONENT32);
    AfxAssert(v[afxPixelFormat_D24].intFmt == GL_DEPTH_COMPONENT24);
    AfxAssert(v[afxPixelFormat_D24S8].intFmt == GL_DEPTH24_STENCIL8);
    AfxAssert(v[afxPixelFormat_D32FS8].intFmt == GL_DEPTH32F_STENCIL8);

    *fmt = v[pf].intFmt;
    *layout = v[pf].layout;
    *type = v[pf].type;
}

_SGL GLenum SglToGlImageDimensions(afxNat d)
{
    static GLenum const v[] =
    {
        NIL,
        GL_TEXTURE_1D,
        GL_TEXTURE_2D,
        GL_TEXTURE_3D,
        GL_TEXTURE_CUBE_MAP,

        GL_INVALID_ENUM
    };

    return v[d];
}

_SGL GLenum SglToGlTexelFilterMode(afxTexelFilter tfm)
{
    static GLenum const v[] =
    {
        GL_NEAREST,
        GL_LINEAR,

        GL_INVALID_ENUM
    };

    return v[tfm];
}

_SGL GLenum SglToGlTexelFilterModeMipmapped(afxTexelFilter min, afxTexelFilter mip)
{
    static GLenum const nearest[] =
    {
        GL_NEAREST_MIPMAP_NEAREST, // - no filtering, sharp switching between mipmaps
        GL_NEAREST_MIPMAP_LINEAR, // - no filtering, smooth transition between mipmaps

        GL_INVALID_ENUM
    };

    static GLenum const linear[] =
    {
        GL_LINEAR_MIPMAP_NEAREST, //- filtering, sharp switching between mipmaps
        GL_LINEAR_MIPMAP_LINEAR,

        GL_INVALID_ENUM
    };

    return min == afxTexelFilter_LINEAR ? linear[mip] : nearest[mip];
}

_SGL GLenum SglToGlTexWrapMode(afxTexelAddress twm)
{
    static GLenum const v[] =
    {
        GL_REPEAT,
        GL_MIRRORED_REPEAT,
        GL_CLAMP_TO_EDGE,
        GL_CLAMP_TO_BORDER,
        GL_MIRROR_CLAMP_TO_EDGE,

        GL_INVALID_ENUM
    };

    return v[twm];
}

_SGL GLenum SglToGlColorSwizzle(afxColorSwizzle swizzle)
{
    static GLenum const v[] =
    {
        GL_RED,
        GL_GREEN,
        GL_BLUE,
        GL_ALPHA,
        GL_ZERO,
        GL_ONE,

        GL_INVALID_ENUM
    };
    return v[swizzle];
}

_SGL void SglToGlColorSwizzling(afxColorSwizzling const *swizzling, GLenum *arrayedSwizzles)
{
    arrayedSwizzles[0] = SglToGlColorSwizzle(swizzling->r);
    arrayedSwizzles[1] = SglToGlColorSwizzle(swizzling->g);
    arrayedSwizzles[2] = SglToGlColorSwizzle(swizzling->b);
    arrayedSwizzles[3] = SglToGlColorSwizzle(swizzling->a);
}

_SGL void SglDetermineGlTargetInternalFormatType(afxRaster ras, GLenum *target, GLint *intFmt, GLenum *fmt, GLenum *type)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(target);
    AfxAssert(intFmt);
    AfxAssert(fmt);
    AfxAssert(type);
    afxResult cubemap = AfxTestRasterFlags(ras, afxRasterFlag_CUBEMAP);
    AfxAssert(ras->base.whd[0]); // always have at least one dimension.
    
    if (!(ras->base.whd[1] > 1)) // Y
    {
        if (ras->base.layerCnt > 1)
        {
            *target = GL_TEXTURE_1D_ARRAY;
        }
        else
        {
            *target = GL_TEXTURE_1D;
        }
    }
    else
    {
        // 2D or 2D array?
        
        if (!(ras->base.whd[2] > 1))
        {
            if (cubemap)
            {
                if (ras->base.layerCnt / 6 > 1)
                {
                    *target = GL_TEXTURE_CUBE_MAP_ARRAY;
                }
                else
                {
                    *target = GL_TEXTURE_CUBE_MAP;
                }
            }
            else
            {
                if (ras->base.layerCnt > 1)
                {
                    *target = GL_TEXTURE_2D_ARRAY;
                }
                else
                {
                    *target = GL_TEXTURE_2D;
                }
            }
        }
        else
        {
            *target = GL_TEXTURE_3D;
        }
    }

    SglToGlFormat(ras->base.fmt, intFmt, fmt, type);
}

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
    afxNat rgnSize;
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
                    data = AfxOpenRasterRegion(ras, &rgn, afxRasterAccess_R, &rgnSize, NIL);

                AfxAssert(data);
                gl->PixelStorei(GL_UNPACK_ALIGNMENT, gpuUnpackAlign);
                _SglTexSubImage(gl, GL_TEXTURE_CUBE_MAP_POSITIVE_X + rgn.baseLayer, rgn.lodIdx, rgn.baseLayer, rgn.layerCnt, rgn.offset, rgn.whd, ras->glFmt, ras->glType, data);

                //if (!isSurface)
                    AfxCloseRasterRegion(ras, &rgn);
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
                data = AfxOpenRasterRegion(ras, &rgn, afxRasterAccess_R, &rgnSize, NIL);

                AfxAssert(data);
                gl->PixelStorei(GL_UNPACK_ALIGNMENT, gpuUnpackAlign);
                _SglTexSubImage(gl, glTarget, rgn.lodIdx, rgn.baseLayer, rgn.layerCnt, rgn.offset, rgn.whd, ras->glFmt, ras->glType, data);

                //if (!isSurface)
                AfxCloseRasterRegion(ras, &rgn);
            }
        }
        break;
    }
    }
    return err;
}

_SGL afxError _SglBindAndSyncRas(sglDpuIdd* dpu, sglBindFlags bindFlags, afxNat glUnit, afxRaster ras)
{
    //AfxEntry("img=%p", img);
    afxError err = AFX_ERR_NONE;
    glVmt const* gl = &dpu->gl;
    
    if (!ras)
    {
        if (bindFlags & sglBindFlag_BIND)
        {
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
            AfxEcho("afxRaster %p hardware-side data instanced. Target %x, format %x, whd [%u,%u,%u]", ras, glTarget, glIntFmt, ras->base.whd[0], ras->base.whd[1], ras->base.whd[2]);
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

            _SglTexFlushDevice(gl, glTarget, ras);
            ras->lastUpdOffset[0] = 0;
            ras->lastUpdOffset[1] = 0;
            ras->lastUpdOffset[2] = 0;
            ras->lastUpdRange[0] = 0;
            ras->lastUpdRange[1] = 0;
            ras->lastUpdRange[2] = 0;
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

_SGL afxError _AfxCloseTextureRegion(afxRaster ras, afxRasterRegion const *rgn)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    (void)rgn;
    
    --ras->locked;

    return err;
};

_SGL afxError _AfxOpenTextureRegion(afxRaster ras, afxRasterRegion const *rgn, afxRasterAccess flags, afxNat *size, afxNat* rowLen, void**ptr)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    AfxAssert(rgn);
    AfxAssert(ras->base.lodCnt > rgn->lodIdx);
    AfxAssertRange(ras->base.layerCnt, rgn->baseLayer, rgn->layerCnt);
    AfxAssertRange(ras->base.whd[0], rgn->offset[0], rgn->whd[0]);
    AfxAssertRange(ras->base.whd[1], rgn->offset[1], rgn->whd[1]);
    AfxAssertRange(ras->base.whd[2], rgn->offset[2], rgn->whd[2]);

    //afxDrawContext dctx = AfxGetObjectProvider(ras);

    if (!ras->base.maps)
        AfxBufferizeRaster(ras); // force texture allocation

    void *map = NIL;

    afxNat offset = AfxGetRasterOffset(ras, rgn->lodIdx, rgn->baseLayer, rgn->offset);

    if (rowLen)
        *rowLen = AfxMeasureRasterRow(ras, rgn->lodIdx);

    if (size)
        *size = AfxMeasureRasterRegion(ras, rgn);

    map = &(ras->base.maps[offset]);

    ++ras->locked;

    ras->lastUpdImgBase = AfxMin(ras->lastUpdImgBase, rgn->baseLayer);
    ras->lastUpdImgRange = AfxMax(ras->lastUpdImgRange, rgn->layerCnt);
    ras->lastUpdLodBase = AfxMin(ras->lastUpdLodBase, rgn->lodIdx);
    ras->lastUpdLodRange = AfxMax(ras->lastUpdLodRange, rgn->lodIdx + 1);

    ras->lastUpdOffset[0] = AfxMin(ras->lastUpdOffset[0], rgn->offset[0]);
    ras->lastUpdOffset[1] = AfxMin(ras->lastUpdOffset[1], rgn->offset[1]);
    ras->lastUpdOffset[2] = AfxMin(ras->lastUpdOffset[2], rgn->offset[2]);

    ras->lastUpdRange[0] = AfxMax(ras->lastUpdRange[0], rgn->whd[0]);
    ras->lastUpdRange[1] = AfxMax(ras->lastUpdRange[1], rgn->whd[1]);
    ras->lastUpdRange[2] = AfxMax(ras->lastUpdRange[2], rgn->whd[2]);

    if (flags & afxRasterAccess_W)
        ras->updFlags |= SGL_UPD_FLAG_DEVICE_FLUSH;

    if (flags & afxRasterAccess_X)
    {
        ras->updFlags |= SGL_UPD_FLAG_DEVICE_INST;
    }
    *ptr = map;
    return err;
}

_SGL afxError _SglRasDtor(afxRaster ras)
{
    afxError err = AFX_ERR_NONE;
    AfxEntry("ras=%p", ras);
    AfxAssertObjects(1, &ras, afxFcc_RAS);

    afxDrawContext dctx = AfxGetObjectProvider(ras);
    afxMmu mmu = AfxGetDrawContextMmu(dctx);
    AfxAssertObjects(1, &mmu, afxFcc_MMU);

    if (ras->glHandle)
    {
        _SglDctxDeleteGlRes(dctx, 1, (void*)ras->glHandle);
        ras->glHandle = 0;
    }

    if (ras->base.maps)
    {
        AfxDeallocate(mmu, ras->base.maps);
    }

    return err;
}

_SGL afxError _SglRasCtor(afxRaster ras, afxCookie const* cookie)
{
    AfxEntry("ras=%p", ras);

    afxError err = AFX_ERR_NONE;

    afxRasterInfo const *texi = ((afxRasterInfo const *)cookie->udd[1]) + cookie->no;

    afxDrawContext dctx = AfxGetObjectProvider(ras);
    afxMmu mmu = AfxGetDrawContextMmu(dctx);
    AfxAssertObjects(1, &mmu, afxFcc_MMU);

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

    if (ras->base.whd[1] == 1)
    {
        if (ras->base.whd[2] == 1)
            ras->base.flags |= afxRasterFlag_1D;
        else
            ras->base.flags |= afxRasterFlag_3D;
    }
    else
    {
        if (ras->base.whd[2] == 1)
            ras->base.flags |= afxRasterFlag_2D;
        else
            ras->base.flags |= afxRasterFlag_3D;
    }

    if (ras->base.layerCnt > 1)
        ras->base.flags |= afxRasterFlag_LAYERED;

    if (ras->base.lodCnt > 1)
        ras->base.flags |= afxRasterFlag_SUBSAMPLED;

    ras->base.maps = NIL;

    //ras->idd = NIL;

    if (!err)
    {
        ras->base.map = _AfxOpenTextureRegion;
        ras->base.unmap = _AfxCloseTextureRegion;
        
        ras->updFlags = SGL_UPD_FLAG_DEVICE_INST;

        //ras->lastUpdImgRange = AfxCountRasterLayers(ras);
        //ras->lastUpdLodRange = AfxCountRasterLods(ras);
        //AfxGetRasterExtent(ras, 0, ras->lastUpdRange);
    }

    if (err && ras->base.maps)
    {
        AfxDeallocate(mmu, ras->base.maps);
    }

    return err;
}

_SGL afxClassConfig const _SglRasClsConfig =
{
    .fcc = afxFcc_RAS,
    .name = "Raster",
    .unitsPerPage = 2,
    .size = sizeof(AFX_OBJECT(afxRaster)),
    .mmu = NIL,
    .ctor = (void*)_SglRasCtor,
    .dtor = (void*)_SglRasDtor
};
