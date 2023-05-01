
#define _AFX_IOS_IMPL
#define _AFX_RES_IMPL
#include "sgl.h"

#include "afx/draw/afxDrawSystem.h"
#include "afx/core/afxSystem.h"
#include "../src/e2coree/draw/afxDrawParadigms.h"
#include "afx/draw/io/afxTarga.h"

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

        {   GL_R8,                  GL_RED,             GL_UNSIGNED_BYTE        },
        {   GL_RG8,                 GL_RG,              GL_UNSIGNED_BYTE        },
        {   GL_RGB8,                GL_RGB,             GL_UNSIGNED_BYTE        },
        {   GL_RGBA8,               GL_RGBA,            GL_UNSIGNED_BYTE        },

        {   GL_RG8,                 GL_RG,              GL_UNSIGNED_BYTE        },
        {   GL_RGB8,                GL_BGR,             GL_UNSIGNED_BYTE        },
        {   GL_RGBA8,               GL_BGRA,            GL_UNSIGNED_BYTE        },

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

    return min == AFX_TEXEL_FLT_LINEAR ? linear[mip] : nearest[mip];
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

_SGL void SglDetermineGlTargetInternalFormatType(afxTexture tex, GLenum *target, GLint *intFmt, GLenum *fmt, GLenum *type)
{
    afxError err = NIL;
    AfxAssert(target);
    AfxAssert(intFmt);
    AfxAssert(fmt);
    AfxAssert(type);
    afxResult cubemap = AfxTextureTestUsageFlags(tex, AFX_TEX_CUBEMAP);
    AfxAssert(tex->whd[0]); // always have at least one dimension.
    
    if (!(tex->whd[1] > 1)) // Y
    {
        if (tex->layerCnt > 1)
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
        
        if (!(tex->whd[2] > 1))
        {
            if (cubemap)
            {
                if (tex->layerCnt / 6 > 1)
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
                if (tex->layerCnt > 1)
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

    SglToGlFormat(tex->fmt, intFmt, fmt, type);
}

_SGL afxError _SglTexInstDevice(afxTexture tex, afxNat unit, sglVmt const* gl)
{
    afxError err = NIL;

    AfxAssert((tex->updFlags & SGL_UPD_FLAG_DEVICE_INST) == SGL_UPD_FLAG_DEVICE_INST);

    //gl->TexStorage2D();

    afxNat const lvlCnt = AfxTextureGetLodCount(tex);
    AfxAssert(lvlCnt);
    gl->TexParameteri(tex->glTarget, GL_TEXTURE_BASE_LEVEL, 0); _SglThrowErrorOccuried();
    gl->TexParameteri(tex->glTarget, GL_TEXTURE_MAX_LEVEL, (lvlCnt - 1)); _SglThrowErrorOccuried();

#if 0 // to be removed

    if (tex->glTarget == GL_TEXTURE_CUBE_MAP)
    {
        gl->TexParameteri(tex->glTarget, GL_TEXTURE_MAG_FILTER, SglToGlTexelFilterMode(AFX_TEXEL_FLT_LINEAR)); _SglThrowErrorOccuried();
        gl->TexParameteri(tex->glTarget, GL_TEXTURE_MIN_FILTER, SglToGlTexelFilterMode(AFX_TEXEL_FLT_LINEAR)); _SglThrowErrorOccuried();

        gl->TexParameteri(tex->glTarget, GL_TEXTURE_WRAP_S, SglToGlTexWrapMode(AFX_TEXEL_ADDR_CLAMP)); _SglThrowErrorOccuried();
        gl->TexParameteri(tex->glTarget, GL_TEXTURE_WRAP_T, SglToGlTexWrapMode(AFX_TEXEL_ADDR_CLAMP)); _SglThrowErrorOccuried();
        gl->TexParameteri(tex->glTarget, GL_TEXTURE_WRAP_R, SglToGlTexWrapMode(AFX_TEXEL_ADDR_CLAMP)); _SglThrowErrorOccuried();
    }
    else
    {
        gl->TexParameteri(tex->glTarget, GL_TEXTURE_MAG_FILTER, SglToGlTexelFilterMode(AFX_TEXEL_FLT_POINT)); _SglThrowErrorOccuried();
        gl->TexParameteri(tex->glTarget, GL_TEXTURE_MIN_FILTER, SglToGlTexelFilterMode(AFX_TEXEL_FLT_POINT)); _SglThrowErrorOccuried();

        gl->TexParameteri(tex->glTarget, GL_TEXTURE_WRAP_S, SglToGlTexWrapMode(AFX_TEXEL_ADDR_REPEAT)); _SglThrowErrorOccuried();
        gl->TexParameteri(tex->glTarget, GL_TEXTURE_WRAP_T, SglToGlTexWrapMode(AFX_TEXEL_ADDR_REPEAT)); _SglThrowErrorOccuried();
        gl->TexParameteri(tex->glTarget, GL_TEXTURE_WRAP_R, SglToGlTexWrapMode(AFX_TEXEL_ADDR_REPEAT)); _SglThrowErrorOccuried();
    }
#endif

    afxWhd extent;

    afxBool const isRasterBuffer = AfxTextureTestUsageFlags(tex, AFX_TEX_USAGE_SURFACE_RASTER);
    afxBool const isDepthBuffer = AfxTextureTestUsageFlags(tex, AFX_TEX_USAGE_SURFACE_DEPTH);
    AfxAssert(!(isRasterBuffer && isDepthBuffer)); // can't be both at same time
    afxBool const isRenderBuffer = (isRasterBuffer || isDepthBuffer);

    if (tex->swizzling)
    {
        GLenum arrayedSwizzle[4];
        SglToGlColorSwizzling(tex->swizzling, arrayedSwizzle);
        gl->TexParameteriv(tex->glTarget, GL_TEXTURE_SWIZZLE_RGBA, (GLint*)arrayedSwizzle); _SglThrowErrorOccuried();
    }

    afxBool const isCubemap = AfxTextureTestUsageFlags(tex, AFX_TEX_CUBEMAP);
    AfxAssert(!(isCubemap && tex->whd[2] > 1)); // can't be both 3D and cubemap at same time

    if (lvlCnt > 1)
    {
        // what if we want to upload it?
        //gl->GenerateMipmap(tex->glTarget); _SglThrowErrorOccuried();
    }

    void const* data;
    
    switch (tex->glTarget)
    {
    case GL_TEXTURE_CUBE_MAP:
    {
        AfxAssert(isCubemap);
        AfxAssert(tex->layerCnt == 6);

        for (afxNat i = 0; i < lvlCnt; i++)
        {
            for (afxNat j = 0; j < tex->layerCnt; j++)
            {
                AfxTextureGetExtent(tex, i, extent);
                data = AfxTextureGetData(tex, i, j, NIL);
                AfxAssert(data);
                gl->TexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + j, i, tex->glIntFmt, extent[0], extent[1], 0, tex->glFmt, tex->glType, data); _SglThrowErrorOccuried();
            }
        }
        break;
    }
    case GL_TEXTURE_3D:
    {
        for (afxNat i = 0; i < lvlCnt; i++)
        {
            AfxTextureGetExtent(tex, i, extent);
            data = AfxTextureGetData(tex, i, 0, NIL);
            AfxAssert(data);
            gl->TexImage3D(tex->glTarget, i, tex->glIntFmt, extent[0], extent[1], extent[2], 0, tex->glFmt, tex->glType, data); _SglThrowErrorOccuried();
        }
        break;
    }
    case GL_TEXTURE_2D_ARRAY:
    {
        AfxAssert(tex->layerCnt > 1);

        for (afxNat i = 0; i < lvlCnt; i++)
        {
            for (afxNat j = 0; j < tex->layerCnt; j++)
            {
                AfxTextureGetExtent(tex, i, extent);
                data = AfxTextureGetData(tex, i, j, NIL);
                AfxAssert(data);
                gl->TexImage3D(tex->glTarget, i, tex->glIntFmt, extent[0], extent[1], 1, 0, tex->glFmt, tex->glType, data); _SglThrowErrorOccuried();
            }
        }
        break;
    }
    case GL_TEXTURE_2D:
    {
        for (afxNat i = 0; i < lvlCnt; i++)
        {
            AfxTextureGetExtent(tex, i, extent);
            data = AfxTextureGetData(tex, i, 0, NIL);
            AfxAssert(data);
            gl->TexImage2D(tex->glTarget, i, tex->glIntFmt, extent[0], extent[1], 0, tex->glFmt, tex->glType, data); _SglThrowErrorOccuried();
        }
        break;
    }
    case GL_TEXTURE_1D_ARRAY:
    {
        AfxAssert(tex->layerCnt > 1);

        for (afxNat i = 0; i < lvlCnt; i++)
        {
            for (afxNat j = 0; j < tex->layerCnt; j++)
            {
                AfxTextureGetExtent(tex, i, extent);
                data = AfxTextureGetData(tex, i, j, NIL);
                AfxAssert(data);
                gl->TexImage2D(tex->glTarget, i, tex->glIntFmt, extent[0], extent[1], 0, tex->glFmt, tex->glType, data); _SglThrowErrorOccuried();
            }
        }
        break;
    }
    case GL_TEXTURE_1D:
    {
        for (afxNat i = 0; i < lvlCnt; i++)
        {
            AfxTextureGetExtent(tex, i, extent);
            data = AfxTextureGetData(tex, i, 0, NIL);
            AfxAssert(data);
            gl->TexImage1D(tex->glTarget, i, tex->glIntFmt, extent[0], 0, tex->glFmt, tex->glType, data); _SglThrowErrorOccuried();
        }
        break;
    }
    default:
        AfxThrowError();
        break;
    }

    {
        //gl->BindTexture(tex->glTarget, 0); _SglThrowErrorOccuried();
    }
    AfxAdvertise("tex %p, gl/target %x, gl/intFmt %x, gl/fmt %x, gl/type %x", tex->glTarget, tex->glIntFmt, tex->glFmt, tex->glType);
    AfxEcho("tex %p, instanced.", tex);

    // build or rebuild forces total data update
    tex->lastUpdOffset[0] = (tex->lastUpdOffset[1] = (tex->lastUpdOffset[2] = 0));
    tex->lastUpdRange[0] = (tex->lastUpdRange[1] = (tex->lastUpdRange[2] = 0));
    tex->updFlags &= ~(SGL_UPD_FLAG_DEVICE_INST | SGL_UPD_FLAG_DEVICE_FLUSH);
    return err;
}

_SGL afxError _SglTexFlushDevice(afxTexture tex, sglVmt const* gl)
{
    afxError err = NIL;

    AfxAssert((tex->updFlags & SGL_UPD_FLAG_DEVICE_FLUSH) == SGL_UPD_FLAG_DEVICE_FLUSH);

    afxWhd const xyz = { 0, 0, 0 };
    afxWhd extent;
    //AfxImage.GetExtent(&tex->img, whd);

    afxBool const isCubemap = AfxTextureTestUsageFlags(tex, AFX_TEX_CUBEMAP);
    
    afxNat const lvlCnt = AfxTextureGetLodCount(tex);
    AfxAssert(lvlCnt);

    void const* data;

    switch (tex->glTarget)
    {
    case GL_TEXTURE_CUBE_MAP:
    {
        AfxAssert(isCubemap);
        AfxAssert(tex->layerCnt == 6);

        for (afxNat i = 0; i < lvlCnt; i++)
        {
            for (afxNat j = 0; j < tex->layerCnt; j++)
            {
                AfxTextureGetExtent(tex, i, extent);
                data = AfxTextureGetData(tex, i, j, xyz);
                AfxAssert(data);
                gl->TexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + j, i, xyz[0], xyz[1], extent[0], extent[1], tex->glFmt, tex->glType, data); _SglThrowErrorOccuried();
            }
        }
        break;
    }
    case GL_TEXTURE_3D:
    {
        for (afxNat i = 0; i < lvlCnt; i++)
        {
            AfxTextureGetExtent(tex, i, extent);
            data = AfxTextureGetData(tex, i, 0, xyz);
            AfxAssert(data);
            gl->TexSubImage3D(tex->glTarget, i, xyz[0], xyz[1], xyz[2], extent[0], extent[1], extent[2], tex->glFmt, tex->glType, data); _SglThrowErrorOccuried();
        }
        break;
    }
    case GL_TEXTURE_2D_ARRAY:
    {
        AfxAssert(tex->layerCnt > 1);

        for (afxNat i = 0; i < lvlCnt; i++)
        {
            for (afxNat j = 0; j < tex->layerCnt; j++)
            {
                AfxTextureGetExtent(tex, i, extent);
                data = AfxTextureGetData(tex, i, j, xyz);
                AfxAssert(data);
                gl->TexSubImage3D(tex->glTarget, i, xyz[0], xyz[1], j, extent[0], extent[1], 1, tex->glFmt, tex->glType, data); _SglThrowErrorOccuried();
            }
        }
        break;
    }
    case GL_TEXTURE_2D:
    {
        for (afxNat i = 0; i < lvlCnt; i++)
        {
            AfxTextureGetExtent(tex, i, extent);
            data = AfxTextureGetData(tex, i, 0, xyz);
            AfxAssert(data);
            gl->TexSubImage2D(tex->glTarget, i, xyz[0], xyz[1], extent[0], extent[1], tex->glFmt, tex->glType, data); _SglThrowErrorOccuried();
        }
        break;
    }
    case GL_TEXTURE_1D_ARRAY:
    {
        AfxAssert(tex->layerCnt > 1);

        for (afxNat i = 0; i < lvlCnt; i++)
        {
            for (afxNat j = 0; j < tex->layerCnt; j++)
            {
                AfxTextureGetExtent(tex, i, extent);
                data = AfxTextureGetData(tex, i, j, xyz);
                AfxAssert(data);
                gl->TexSubImage2D(tex->glTarget, i, xyz[0], j, extent[0], 1, tex->glFmt, tex->glType, data); _SglThrowErrorOccuried();
            }
        }
        break;
    }
    case GL_TEXTURE_1D:
    {
        for (afxNat i = 0; i < lvlCnt; i++)
        {
            AfxTextureGetExtent(tex, i, extent);
            data = AfxTextureGetData(tex, i, 0, xyz);
            AfxAssert(data);
            gl->TexSubImage1D(tex->glTarget, i, xyz[0], extent[0], tex->glFmt, tex->glType, data); _SglThrowErrorOccuried();
        }
        break;
    }
    default:
        AfxThrowError();
        break;
    }
    tex->lastUpdOffset[0] = (tex->lastUpdOffset[1] = (tex->lastUpdOffset[2] = 0));
    tex->lastUpdRange[0] = (tex->lastUpdRange[1] = (tex->lastUpdRange[2] = 0));
    tex->updFlags &= ~(SGL_UPD_FLAG_DEVICE_FLUSH);
    return err;
}

_SGL afxError _SglTexDtor(afxTexture tex)
{
    afxError err = NIL;
    AfxEntry("img=%p", tex);
    AfxAssertObject(tex, AFX_FCC_TEX);

    if (tex->glHandle)
    {
        afxDrawContext dctx = AfxObjectGetProvider(&tex->res.obj);
        AfxAssertObject(dctx, AFX_FCC_DCTX);
        sglVmt const* gl = &(dctx->queues[0]->wglVmt);

        gl->DeleteTextures(1, &(tex->glHandle)); _SglThrowErrorOccuried();
        tex->glHandle = NIL;
    }

    return err;
}

_SGL afxError _SglDqueBindAndSyncTex(afxDrawQueue dque, afxNat unit, afxTexture tex)
{
    //AfxEntry("img=%p", img);
    afxError err = NIL;
    sglVmt const* gl = &dque->wglVmt;

    if (tex)
    {
        AfxAssertObject(tex, AFX_FCC_TEX);

        if ((tex->updFlags & SGL_UPD_FLAG_DEVICE))
        {
            if ((tex->updFlags & SGL_UPD_FLAG_DEVICE_INST))
            {
                if (tex->glHandle)
                {
                    gl->DeleteTextures(1, &(tex->glHandle)); _SglThrowErrorOccuried();
                    tex->glHandle = NIL;
                }

                SglDetermineGlTargetInternalFormatType(tex, &tex->glTarget, &tex->glIntFmt, &tex->glFmt, &tex->glType);

                AfxAssert(NIL == tex->glHandle);
                gl->GenTextures(1, &(tex->glHandle)); _SglThrowErrorOccuried();
                gl->ActiveTexture(GL_TEXTURE0 + unit); _SglThrowErrorOccuried();
                gl->BindTexture(tex->glTarget, tex->glHandle); _SglThrowErrorOccuried();
                AfxAssert(gl->IsTexture(tex->glHandle));

                _SglTexInstDevice(tex, unit, gl); // already clear flags
            }
            else if ((tex->updFlags & SGL_UPD_FLAG_DEVICE_FLUSH))
            {
                AfxAssert(tex->glHandle);
                gl->ActiveTexture(GL_TEXTURE0 + unit); _SglThrowErrorOccuried();
                gl->BindTexture(tex->glTarget, tex->glHandle); _SglThrowErrorOccuried();

                _SglTexFlushDevice(tex, gl); // already clear flags
            }
        }
        else
        {
            AfxAssert(tex->glHandle);
            AfxAssert(tex->glTarget);

            if (gl->BindTextureUnit)
            {
                gl->BindTextureUnit(unit, tex->glHandle); _SglThrowErrorOccuried();
            }
            else
            {
                gl->ActiveTexture(GL_TEXTURE0 + unit); _SglThrowErrorOccuried();
                gl->BindTexture(tex->glTarget, tex->glHandle); _SglThrowErrorOccuried();
            }
        }
    }
    else
    {
        if (gl->BindTextureUnit)
        {
            gl->BindTextureUnit(unit, 0); _SglThrowErrorOccuried();
        }
        else
        {
            gl->ActiveTexture(GL_TEXTURE0 + unit); _SglThrowErrorOccuried();
            gl->BindTexture(GL_TEXTURE_1D, 0); _SglThrowErrorOccuried();
        }
    }
}

_SGL afxBool _AfxTextureTestFlags(afxTexture tex, afxTextureFlag flags)
{
    afxError err = NIL;
    AfxAssertObject(tex, AFX_FCC_TEX);
    return tex->flags & flags;
}

_SGL afxNat _AfxTextureGetLodCount(afxTexture tex)
{
    afxError err = NIL;
    AfxAssertObject(tex, AFX_FCC_TEX);
    return tex->lodCnt;
}

_SGL afxNat _AfxTextureGetLayerCount(afxTexture tex)
{
    afxError err = NIL;
    AfxAssertObject(tex, AFX_FCC_TEX);
    return tex->layerCnt;
}

_SGL afxColorSwizzling const* _AfxTextureGetColorSwizzling(afxTexture tex)
{
    afxError err = NIL;
    AfxAssertObject(tex, AFX_FCC_TEX);
    return tex->swizzling;
}

_SGL afxNat _AfxTextureGetSampleCount(afxTexture tex)
{
    afxError err = NIL;
    AfxAssertObject(tex, AFX_FCC_TEX);
    return tex->sampleCnt;
}

_SGL afxResult _AfxTextureTestUsageFlags(afxTexture tex, afxFlags flags)
{
    afxError err = NIL;
    AfxAssertObject(tex, AFX_FCC_TEX);
    return tex->usage & flags;
}

_SGL afxPixelFormat _AfxTextureGetFormat(afxTexture tex)
{
    afxError err = NIL;
    AfxAssertObject(tex, AFX_FCC_TEX);
    return tex->fmt;
}

_SGL afxError _AfxTextureGenerateLods(afxTexture tex, afxNat base, afxNat cnt)
{
    afxError err = NIL;
    AfxAssertObject(tex, AFX_FCC_TEX);
    AfxAssert(cnt);
    AfxThrowError();
    return err;
}

_SGL afxNat* _AfxTextureGetExtent(afxTexture tex, afxNat lod, afxWhd extent)
{
    afxError err = NIL;
    AfxAssertObject(tex, AFX_FCC_TEX);
    AfxAssert(tex->lodCnt > lod);
    AfxAssert(extent);
    extent[0] = tex->whd[0] >> lod;
    extent[1] = tex->whd[1] >> lod;
    extent[2] = tex->whd[2] >> lod;
    return extent;
}

_SGL afxError _AfxTextureSetExtent(afxTexture tex, afxNat layerCnt, afxWhd const extent)
{
    afxError err = NIL;
    AfxAssertObject(tex, AFX_FCC_TEX);
    AfxAssert(layerCnt);
    AfxAssert(extent);
    AfxAssert(extent[0]);
    AfxAssert(extent[1]);
    AfxAssert(extent[2]);
    AfxAssert(extent[0] != tex->whd[0] || extent[1] != tex->whd[1] || extent[2] != tex->whd[2]);

    if (tex->locked) AfxThrowError();
    else
    {
        tex->locked = TRUE;

        afxNat bpp = AfxPixelFormatGetBpp(tex->fmt);
        afxNat pixelSiz = bpp / AFX_BYTE_SIZE;
        afxNat baseLayerSiz = (extent[2] * (extent[1] * (extent[0] * pixelSiz)));
        afxNat layerSum = baseLayerSiz * layerCnt;
        afxNat baseLevelSiz = layerSum;

        afxNat totalSiz = 0;

        for (afxNat i = 0; i < tex->lodCnt; i++)
            totalSiz += baseLevelSiz >> i;

        afxDrawContext dctx = AfxTextureGetContext(tex);
        AfxAssertObject(dctx, AFX_FCC_DCTX);
        afxMemory mem = AfxDrawContextGetMemory(dctx);
        AfxAssertObject(mem, AFX_FCC_MEM);

        void *bytemap;

        if (!(bytemap = AfxReallocate(mem, tex->bytemap, totalSiz, AfxSpawnHint()))) AfxThrowError();
        else
        {
            tex->bytemap = bytemap;
        }

        if (!err)
        {
            tex->whd[0] = extent[0];
            tex->whd[1] = extent[1];
            tex->whd[2] = extent[2];
            tex->layerCnt = layerCnt;

            tex->lastUpdOffset[0] = tex->lastUpdOffset[1] = tex->lastUpdOffset[2] = 0;
            tex->lastUpdRange[0] = extent[0];
            tex->lastUpdRange[1] = extent[1];
            tex->lastUpdRange[2] = extent[2];

            tex->updFlags |= SGL_UPD_FLAG_DEVICE_INST;
        }

        tex->locked = FALSE;
    }
    return err;
}

_SGL afxSize _AfxTextureMeasure(afxTexture tex, afxNat baseLod, afxNat lodCnt, afxNat layerCnt, afxWhd const offset, afxWhd const extent) // can't be multiple because regions couldn't be continous.
{
    afxError err = NIL;
    AfxAssertObject(tex, AFX_FCC_TEX);
    AfxAssert(tex->lodCnt >= baseLod);
    AfxAssert(tex->lodCnt >= lodCnt);
    AfxAssert(tex->layerCnt >= layerCnt);
    AfxAssert(extent[0]);
    AfxAssert(extent[1]);
    AfxAssert(extent[2]);
    afxSize range = 0;

    afxNat bpp = AfxPixelFormatGetBpp(tex->fmt);
    afxNat pixelSiz = bpp / AFX_BYTE_SIZE;
    afxNat layerSiz = (tex->whd[2] * (tex->whd[1] * (tex->whd[0] * pixelSiz)));
    afxNat layerSum = layerSiz * tex->layerCnt;

    for (afxNat i = 0; i < lodCnt - 1; i++)
    {
        afxNat levelSiz = layerSum >> (baseLod + i);
        range += levelSiz;
    }

    AfxAssert(tex->whd[0] >> (baseLod + (lodCnt - 1)) > offset[0]);
    AfxAssert(tex->whd[1] >> (baseLod + (lodCnt - 1)) > offset[1]);
    AfxAssert(tex->whd[2] >> (baseLod + (lodCnt - 1)) > offset[2]);
    AfxAssert(tex->whd[0] >> (baseLod + (lodCnt - 1)) >= extent[0]);
    AfxAssert(tex->whd[1] >> (baseLod + (lodCnt - 1)) >= extent[1]);
    AfxAssert(tex->whd[2] >> (baseLod + (lodCnt - 1)) >= extent[2]);

    range += (offset[2] * (offset[1] * (offset[0] * pixelSiz)));
    range += (extent[2] * (extent[1] * (extent[0] * pixelSiz)));
    return range;
}

_SGL afxSize _AfxTextureGetOffset(afxTexture tex, afxNat lod, afxNat layer, afxWhd const offset)
{
    afxError err = NIL;
    AfxAssertObject(tex, AFX_FCC_TEX);
    AfxAssert(tex->lodCnt > lod);
    AfxAssert(tex->layerCnt > layer);
    AfxAssert(tex->whd[0] >> lod > offset[0]);
    AfxAssert(tex->whd[1] >> lod > offset[1]);
    AfxAssert(tex->whd[2] >> lod > offset[2]);
    afxSize offset2 = 0;

    afxNat bpp = AfxPixelFormatGetBpp(tex->fmt);
    afxNat pixelSiz = bpp / AFX_BYTE_SIZE;
    afxNat layerSiz = (tex->whd[2] * (tex->whd[1] * (tex->whd[0] * pixelSiz)));
    afxNat layerSum = layerSiz * tex->layerCnt;

    for (afxNat i = 0; i < lod; i++)
    {
        afxNat levelSiz = layerSum >> i;
        offset2 += levelSiz;
    }
    offset2 += (layer * layerSiz) + (offset[2] * (offset[1] * (offset[0] * pixelSiz)));

    // another way
    offset2 = ((layer * layerSiz) + (offset[2] * (offset[1] * (offset[0] * pixelSiz)))) >> lod;
    return offset2;
}

_SGL void _AfxTextureUnmap(afxTexture tex)
{
    afxError err = NIL;
    AfxAssertObject(tex, AFX_FCC_TEX);
    tex->updFlags |= SGL_UPD_FLAG_DEVICE_FLUSH;
    tex->locked = FALSE;
};

_SGL void* _AfxTextureMap(afxTexture tex, afxTextureRegion const *rgn, afxSize *size)
{
    afxError err = NIL;
    AfxAssertObject(tex, AFX_FCC_TEX);
    // TODO implement lodIdx
    void *map = NIL;

    if (tex->locked) AfxThrowError();
    else
    {
        tex->locked = TRUE;

        AfxAssert(rgn);
        afxNat lod = rgn->lod;
        afxNat layerCnt = rgn->layerCnt;
        AfxAssert(tex->lodCnt > lod);
        AfxAssert(tex->whd[0] > rgn->offset[0]);
        AfxAssert(tex->whd[1] > rgn->offset[1]);
        AfxAssert(tex->whd[2] > rgn->offset[2]);
        AfxAssert(rgn->extent[0]);
        AfxAssert(rgn->extent[1]);
        AfxAssert(rgn->extent[2]);
        AfxAssert(tex->whd[0] >> lod >= rgn->extent[0]);
        AfxAssert(tex->whd[1] >> lod >= rgn->extent[1]);
        AfxAssert(tex->whd[2] >> lod >= rgn->extent[2]);

        afxSize offset = _AfxTextureGetOffset(tex, rgn->lod, rgn->baseLayer, rgn->offset);
        
        if (size)
        {
            afxSize range = _AfxTextureMeasure(tex, rgn->lod, 1, rgn->layerCnt, rgn->offset, rgn->extent);
            *size = range;
        }

        map = &(tex->bytemap[offset]);

        tex->lastUpdOffset[0] = 0;
        tex->lastUpdOffset[1] = 0;
        tex->lastUpdOffset[2] = 0;

        tex->lastUpdRange[0] = tex->whd[0];
        tex->lastUpdRange[1] = tex->whd[1];
        tex->lastUpdRange[2] = tex->whd[2];

        //tex->locked = FALSE;
    }
    return map;
}

_SGL void const* _AfxTextureGetData(afxTexture tex, afxNat lod, afxNat layer, afxWhd const offset)
{
    afxError err = NIL;
    AfxAssertObject(tex, AFX_FCC_TEX);
    AfxAssert(tex->lodCnt > lod);
    AfxAssert(tex->layerCnt > layer);
    AfxAssert(tex->whd[0] > offset[0]);
    AfxAssert(tex->whd[1] > offset[1]);
    AfxAssert(tex->whd[2] > offset[2]);

    afxSize off = _AfxTextureGetOffset(tex, lod, layer, offset);
    afxByte *bytemap = &(tex->bytemap[off]);
    return bytemap;
}

_SGL afxError _AfxTextureUpdateRegions(afxTexture tex, afxNat cnt, afxTextureRegion const rgn[], void const *src[], afxPixelFormat const fmt[])
{
    afxError err = NIL;
    AfxAssertObject(tex, AFX_FCC_TEX);
    AfxAssert(cnt);
    AfxAssert(rgn);
    AfxAssert(src);
    AfxAssert(fmt);

    for (afxNat i = 0; i < cnt; i++)
    {
        AfxAssert(rgn[i].lod == 0);
        AfxAssert(tex->layerCnt > rgn[i].baseLayer);
        AfxAssert(tex->layerCnt >= rgn[i].layerCnt);
        AfxAssert(tex->whd[0] > rgn[i].offset[0]);
        AfxAssert(tex->whd[1] > rgn[i].offset[1]);
        AfxAssert(tex->whd[2] > rgn[i].offset[2]);
        AfxAssert(tex->whd[0] >= rgn[i].extent[0]);
        AfxAssert(tex->whd[1] >= rgn[i].extent[1]);
        AfxAssert(tex->whd[2] >= rgn[i].extent[2]);
        AfxAssert(tex->whd[0] >= rgn[i].offset[0] + rgn[i].extent[0]);
        AfxAssert(tex->whd[1] >= rgn[i].offset[1] + rgn[i].extent[1]);
        AfxAssert(tex->whd[2] >= rgn[i].offset[2] + rgn[i].extent[2]);
        
        afxSize offset = _AfxTextureGetOffset(tex, rgn[i].lod, rgn[i].baseLayer, rgn[i].offset);
        afxSize range = _AfxTextureMeasure(tex, rgn[i].lod, 1, rgn[i].layerCnt, rgn[i].offset, rgn[i].extent);
        afxByte *bytemap = &(tex->bytemap[offset]);

        AfxAssert(fmt[i] == tex->fmt);
        AfxCopy(bytemap, src[i], range);
    }

    tex->updFlags |= SGL_UPD_FLAG_DEVICE_FLUSH;
    return err;
}

_SGL afxError _AfxTexDtor(afxTexture tex)
{
    afxError err = NIL;
    AfxEntry("tex=%p", tex);
    AfxAssertObject(tex, AFX_FCC_TEX);

    afxDrawContext dctx = AfxTextureGetContext(tex);
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    afxMemory mem = AfxDrawContextGetMemory(dctx);
    AfxAssertObject(mem, AFX_FCC_MEM);

    if (tex->glHandle)
    {
        _SglEnqueueGlResourceDeletion(dctx, 0, 1, tex->glHandle);
        tex->glHandle = 0;
    }

    if (tex->sidecar)
        AfxDeallocate(mem, tex->sidecar);

    if (tex->bytemap)
        AfxDeallocate(mem, tex->bytemap);

    return err;
}

_SGL afxTexImpl const _AfxStdTexImpl;
afxTexImpl const _AfxStdTexImpl =
{
    _AfxTextureGetColorSwizzling,
    _AfxTextureGetData,
    //_AfxTextureMeasure,
    _AfxTextureGetExtent,
    _AfxTextureGenerateLods,
    _AfxTextureGetFormat,
    _AfxTextureGetLodCount,
    _AfxTextureGetLayerCount,
    _AfxTextureGetSampleCount,
    _AfxTextureMap,
    _AfxTextureSetExtent,
    _AfxTextureTestFlags,
    _AfxTextureTestUsageFlags,
    _AfxTextureUnmap,
    _AfxTextureUpdateRegions
};

_SGL afxError _AfxTexCtor(afxTexture tex, afxTextureParadigm *paradigm)
{
    AfxEntry("tex=%p", tex);
    afxError err = 0;

    afxTextureBlueprint const *texb = paradigm->blueprint;

    afxDrawContext dctx = paradigm->dctx;
    AfxAssertObject(dctx, AFX_FCC_DCTX);

    tex->sidecar = NIL;
    tex->swizzling = &AFX_STD_COLOR_SWIZZLING;
    tex->sampleCnt = 1;
    tex->linearTiling = FALSE;

    AfxAssert(texb->layerCnt);
    tex->layerCnt = texb->layerCnt;
    AfxAssert(texb->lodCnt == 1);
    tex->lodCnt = texb->lodCnt;

    tex->usage = texb->usage;

    tex->fmt = texb->fmt;

    tex->whd[0] = texb->whd[0];
    tex->whd[1] = texb->whd[1];
    tex->whd[2] = texb->whd[2];

    afxNat pixelSiz = AfxPixelFormatGetBpp(tex->fmt) / AFX_BYTE_SIZE;
    afxNat layerSiz = (tex->whd[2] * (tex->whd[1] * (tex->whd[0] * pixelSiz)));
    afxNat layerSum = layerSiz * tex->layerCnt;
    afxNat lodSum = 0;

    for (afxNat i = 0; i < tex->lodCnt; i++)
        lodSum += layerSum >> i;

    afxMemory mem = AfxDrawContextGetMemory(dctx);
    AfxAssertObject(mem, AFX_FCC_MEM);

    if (!(tex->bytemap = AfxAllocate(mem, lodSum, AfxSpawnHint()))) AfxThrowError();
    else
    {
        tex->lastUpdOffset[0] = tex->lastUpdOffset[1] = tex->lastUpdOffset[2] = 0;
        tex->lastUpdRange[0] = tex->whd[0];
        tex->lastUpdRange[1] = tex->whd[1];
        tex->lastUpdRange[2] = tex->whd[2];
        tex->locked = FALSE;

        tex->glFmt = 0;
        tex->glIntFmt = 0;
        tex->glTarget = 0;
        tex->glType = 0;
        tex->glHandle = 0;
        tex->updFlags = SGL_UPD_FLAG_DEVICE_INST;

        for (afxNat i = 0; i < tex->lodCnt; i++)
        {
            for (afxNat j = 0; j < tex->layerCnt; j++)
            {
                afxTextureLayerBlueprint const *layer = &(texb->layers[j]);
                AfxAssert(layer);
                //AfxAssert(layer->src);

                if (layer->srcIsUrl)
                {
                    afxTextureRegion rgn;
                    rgn.lod = i;
                    rgn.baseLayer = j;
                    rgn.layerCnt = 1;
                    rgn.offset[0] = 0;
                    rgn.offset[1] = 0;
                    rgn.offset[2] = 0;
                    AfxTextureGetExtent(tex, i, rgn.extent);
                    AfxTextureUploadRegions(tex, 1, &rgn, &layer->uri.uri);
                }
                else
                {
                    if (layer->src)
                    {
                        afxTextureRegion rgn;
                        rgn.lod = i;
                        rgn.baseLayer = j;
                        rgn.layerCnt = 1;
                        rgn.offset[0] = 0;
                        rgn.offset[1] = 0;
                        rgn.offset[2] = 0;
                        AfxTextureGetExtent(tex, i, rgn.extent);
                        AfxTextureUpdateRegions(tex, 1, &rgn, (void const**)&layer->src, &layer->fmt);
                    }
                }
            }
        }
        
        if (err)
            AfxDeallocate(mem, tex->bytemap);
    }
    return err;
}

_SGL afxResult _AfxDrawContextBuildTextures(afxDrawContext dctx, afxNat cnt, afxTextureBlueprint const texb[], afxTexture tex[])
{
    afxError err = NIL;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    AfxAssert(texb);
    AfxAssert(texb->whd);
    AfxEntry("dctx=%p,fmt=%x,whd=[%d,%d,%d],lodCnt=%u,usage=%x", dctx, texb->fmt, texb->whd[0], texb->whd[1], texb->whd[2], texb->lodCnt, texb->usage);
    
    AfxAssert(texb->fmt);
    AfxAssert(texb->whd[0]);
    AfxAssert(texb->whd[1]);
    AfxAssert(texb->whd[2]);
    afxResult sucCnt = 0;

    afxSystem dsys = AfxDrawContextGetDrawSystem(dctx);
    AfxAssertObject(dsys, AFX_FCC_DSYS);

    for (afxNat i = 0; i < cnt; i++)
    {
        afxTextureParadigm paradigm =
        {
            .res = { AfxObjectGetProvider(&dsys->obj), NIL },
            dctx,
            &texb[i]
        };

        if (!(tex[i] = AfxObjectAcquire(AfxDrawContextGetTextureClass(dctx), &paradigm, AfxSpawnHint()))) AfxThrowError();
        else
        {
            ++sucCnt;
        }

    }
    return sucCnt;
}

_SGL void _AfxIteratorCompareResourceNameCi(afxIterator *iter)
{
    struct { afxUri const *name; afxObject *obj; } *data = iter->udd;
    afxResource res = iter->voidItem;

    afxUri name;
    AfxResourceExcerptName(res, &name);

    if (!AfxUriIsBlank(&name))
    {
        if (0 == AfxStringCompareCi(AfxUriGetStringConst(data->name), AfxUriGetStringConst(&name)))
        {
            data->obj = &res->obj;
            iter->abort = TRUE;
        }
    }
}

_SGL afxResult _AfxDrawContextFindTextures(afxDrawContext dctx, afxNat cnt, afxUri const name[], afxTexture tex[])
{
    afxError err = NIL;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    afxResult foundCnt = 0;

    for (afxNat i = 0; i < cnt; i++)
    {
        AfxAssert(!AfxUriIsBlank(&name[i]));

        struct { afxUri const *name; afxObject *obj; } data = { &name[i], NIL };
        AfxClassForEveryInstance(&dctx->texClass, FALSE, _AfxIteratorCompareResourceNameCi, &data);

        if ((tex[i] = (afxTexture)data.obj))
            ++foundCnt;
    }
    return foundCnt;
}

_SGL afxClassSpecification const _AfxTexClassSpec;

afxClassSpecification const _AfxTexClassSpec =
{
    AFX_FCC_TEX,
    NIL,
    0,
    sizeof(AFX_OBJECT(afxTexture)),
    NIL,
    (void*)_AfxTexCtor,
    (void*)_AfxTexDtor,
    "Texture",
    &_AfxStdTexImpl
};
