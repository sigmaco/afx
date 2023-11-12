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

#include "afx/draw/afxDrawSystem.h"
#include "afx/core/afxSystem.h"

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

_SGL void SglDetermineGlTargetInternalFormatType(afxTexture tex, GLenum *target, GLint *intFmt, GLenum *fmt, GLenum *type)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(target);
    AfxAssert(intFmt);
    AfxAssert(fmt);
    AfxAssert(type);
    afxResult cubemap = AfxTestTexture(tex, AFX_TEX_FLAG_CUBEMAP);
    AfxAssert(tex->base.whd[0]); // always have at least one dimension.
    
    if (!(tex->base.whd[1] > 1)) // Y
    {
        if (tex->base.imgCnt > 1)
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
        
        if (!(tex->base.whd[2] > 1))
        {
            if (cubemap)
            {
                if (tex->base.imgCnt / 6 > 1)
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
                if (tex->base.imgCnt > 1)
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

    SglToGlFormat(tex->base.fmt, intFmt, fmt, type);
}

_SGL afxError _SglTexInstDevice(afxTexture tex, glVmt const* gl) // tex must be bound.
{
    afxError err = AFX_ERR_NONE;
    
    AfxAssert((tex->updFlags & SGL_UPD_FLAG_DEVICE_INST) == SGL_UPD_FLAG_DEVICE_INST);

    //gl->TexStorage2D();

    afxNat const lvlCnt = AfxGetTextureLodCount(tex);
    AfxAssert(lvlCnt);
    gl->TexParameteri(tex->glTarget, GL_TEXTURE_BASE_LEVEL, 0); _SglThrowErrorOccuried();
    gl->TexParameteri(tex->glTarget, GL_TEXTURE_MAX_LEVEL, (lvlCnt - 1)); _SglThrowErrorOccuried();

#if 0 // to be removed

    if (tex->glTarget == GL_TEXTURE_CUBE_MAP)
    {
        gl->TexParameteri(tex->glTarget, GL_TEXTURE_MAG_FILTER, SglToGlTexelFilterMode(afxTexelFilter_LINEAR)); _SglThrowErrorOccuried();
        gl->TexParameteri(tex->glTarget, GL_TEXTURE_MIN_FILTER, SglToGlTexelFilterMode(afxTexelFilter_LINEAR)); _SglThrowErrorOccuried();

        gl->TexParameteri(tex->glTarget, GL_TEXTURE_WRAP_S, SglToGlTexWrapMode(afxTexelAddress_CLAMP)); _SglThrowErrorOccuried();
        gl->TexParameteri(tex->glTarget, GL_TEXTURE_WRAP_T, SglToGlTexWrapMode(afxTexelAddress_CLAMP)); _SglThrowErrorOccuried();
        gl->TexParameteri(tex->glTarget, GL_TEXTURE_WRAP_R, SglToGlTexWrapMode(afxTexelAddress_CLAMP)); _SglThrowErrorOccuried();
    }
    else
    {
        gl->TexParameteri(tex->glTarget, GL_TEXTURE_MAG_FILTER, SglToGlTexelFilterMode(afxTexelFilter_POINT)); _SglThrowErrorOccuried();
        gl->TexParameteri(tex->glTarget, GL_TEXTURE_MIN_FILTER, SglToGlTexelFilterMode(afxTexelFilter_POINT)); _SglThrowErrorOccuried();

        gl->TexParameteri(tex->glTarget, GL_TEXTURE_WRAP_S, SglToGlTexWrapMode(afxTexelAddress_REPEAT)); _SglThrowErrorOccuried();
        gl->TexParameteri(tex->glTarget, GL_TEXTURE_WRAP_T, SglToGlTexWrapMode(afxTexelAddress_REPEAT)); _SglThrowErrorOccuried();
        gl->TexParameteri(tex->glTarget, GL_TEXTURE_WRAP_R, SglToGlTexWrapMode(afxTexelAddress_REPEAT)); _SglThrowErrorOccuried();
    }
#endif

    //afxWhd extent;

    if (tex->base.swizzling)
    {
        GLenum arrayedSwizzle[4];
        SglToGlColorSwizzling(tex->base.swizzling, arrayedSwizzle);
        gl->TexParameteriv(tex->glTarget, GL_TEXTURE_SWIZZLE_RGBA, (GLint*)arrayedSwizzle); _SglThrowErrorOccuried();
    }

    //afxBool const isSurface = AfxTestTexture(tex, afxTextureFlag_DRAW);
    afxBool const isCubemap = AfxTestTexture(tex, AFX_TEX_FLAG_CUBEMAP);
    AfxAssert(!(isCubemap && tex->base.whd[2] > 1)); // can't be both 3D and cubemap at same time

    if (lvlCnt > 1)
    {
        // what if we want to upload it?
        //gl->GenerateMipmap(tex->glTarget); _SglThrowErrorOccuried();
    }

    afxTextureRegion rgn;
    afxNat rgnSize;
    void const* data;
    
    switch (tex->glTarget)
    {
    case GL_TEXTURE_CUBE_MAP:
    {
        AfxAssert(isCubemap);
        AfxAssert(tex->base.imgCnt == 6);

        for (afxNat j = 0; j < tex->base.imgCnt; j++)
        {
            for (afxNat i = 0; i < lvlCnt; i++)
            {
                rgn.lodIdx = i;
                rgn.baseImg = j;
                rgn.imgCnt = 1;
                rgn.offset[0] = 0;
                rgn.offset[1] = 0;
                rgn.offset[2] = 0;
                rgn.extent[0] = tex->base.whd[0];
                rgn.extent[1] = tex->base.whd[1];
                rgn.extent[2] = tex->base.whd[2];

                data = NIL;

                //if (!isSurface)
                    data = AfxOpenTextureRegion(tex, &rgn, AFX_TEX_OPEN_R, &rgnSize);

                AfxAssert(data);
                gl->TexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + rgn.baseImg, rgn.lodIdx, tex->glIntFmt, rgn.extent[0], rgn.extent[1], 0, tex->glFmt, tex->glType, data); _SglThrowErrorOccuried();
                
                //if (!isSurface)
                    AfxCloseTextureRegion(tex, &rgn);                
            }
        }
        break;
    }
    case GL_TEXTURE_3D:
    {
        for (afxNat i = 0; i < lvlCnt; i++)
        {
            rgn.lodIdx = i;
            rgn.baseImg = 0;
            rgn.imgCnt = 1;
            rgn.offset[0] = 0;
            rgn.offset[1] = 0;
            rgn.offset[2] = 0;
            rgn.extent[0] = tex->base.whd[0];
            rgn.extent[1] = tex->base.whd[1];
            rgn.extent[2] = tex->base.whd[2];

            data = NIL;

            //if (!isSurface)
                data = AfxOpenTextureRegion(tex, &rgn, AFX_TEX_OPEN_R, &rgnSize);

            AfxAssert(data);
            gl->TexImage3D(tex->glTarget, rgn.lodIdx, tex->glIntFmt, rgn.extent[0], rgn.extent[1], rgn.extent[2], 0, tex->glFmt, tex->glType, data); _SglThrowErrorOccuried();
            

            //if (!isSurface)
                AfxCloseTextureRegion(tex, &rgn);
        }
        break;
    }
    case GL_TEXTURE_2D_ARRAY:
    {
        AfxAssert(tex->base.imgCnt > 1);

        for (afxNat i = 0; i < lvlCnt; i++)
        {
            rgn.lodIdx = i;
            rgn.baseImg = 0;
            rgn.imgCnt = 1;
            rgn.offset[0] = 0;
            rgn.offset[1] = 0;
            rgn.offset[2] = 0;
            rgn.extent[0] = tex->base.whd[0];
            rgn.extent[1] = tex->base.whd[1];
            rgn.extent[2] = tex->base.whd[2];

            data = NIL;

            //if (!isSurface)
                data = AfxOpenTextureRegion(tex, &rgn, AFX_TEX_OPEN_R, &rgnSize);

            AfxAssert(data);
            gl->TexImage3D(tex->glTarget, rgn.lodIdx, tex->glIntFmt, rgn.extent[0], rgn.extent[1], tex->base.imgCnt, 0, tex->glFmt, tex->glType, data); _SglThrowErrorOccuried();

            //if (!isSurface)
                AfxCloseTextureRegion(tex, &rgn);
        }
        break;
    }
    case GL_TEXTURE_2D:
    {
        for (afxNat i = 0; i < lvlCnt; i++)
        {
            rgn.lodIdx = i;
            rgn.baseImg = 0;
            rgn.imgCnt = 1;
            rgn.offset[0] = 0;
            rgn.offset[1] = 0;
            rgn.offset[2] = 0;
            rgn.extent[0] = tex->base.whd[0];
            rgn.extent[1] = tex->base.whd[1];
            rgn.extent[2] = tex->base.whd[2];

            data = NIL;

            //if (!isSurface)
                data = AfxOpenTextureRegion(tex, &rgn, AFX_TEX_OPEN_R, &rgnSize);

            AfxAssert(data);
            gl->TexImage2D(tex->glTarget, rgn.lodIdx, tex->glIntFmt, rgn.extent[0], rgn.extent[1], 0, tex->glFmt, tex->glType, data); _SglThrowErrorOccuried();
            
            //if (!isSurface)
                AfxCloseTextureRegion(tex, &rgn);
        }
        break;
    }
    case GL_TEXTURE_1D_ARRAY:
    {
        AfxAssert(tex->base.imgCnt > 1);

        for (afxNat i = 0; i < lvlCnt; i++)
        {
            rgn.lodIdx = i;
            rgn.baseImg = 0;
            rgn.imgCnt = 1;
            rgn.offset[0] = 0;
            rgn.offset[1] = 0;
            rgn.offset[2] = 0;
            rgn.extent[0] = tex->base.whd[0];
            rgn.extent[1] = tex->base.whd[1];
            rgn.extent[2] = tex->base.whd[2];

            data = NIL;

            //if (!isSurface)
                data = AfxOpenTextureRegion(tex, &rgn, AFX_TEX_OPEN_R, &rgnSize);

            AfxAssert(data);
            gl->TexImage2D(tex->glTarget, rgn.lodIdx, tex->glIntFmt, rgn.extent[0], tex->base.imgCnt, 0, tex->glFmt, tex->glType, data); _SglThrowErrorOccuried();
            
            //if (!isSurface)
                AfxCloseTextureRegion(tex, &rgn);
        }
        break;
    }
    case GL_TEXTURE_1D:
    {
        for (afxNat i = 0; i < lvlCnt; i++)
        {
            rgn.lodIdx = i;
            rgn.baseImg = 0;
            rgn.imgCnt = 1;
            rgn.offset[0] = 0;
            rgn.offset[1] = 0;
            rgn.offset[2] = 0;
            rgn.extent[0] = tex->base.whd[0];
            rgn.extent[1] = tex->base.whd[1];
            rgn.extent[2] = tex->base.whd[2];

            data = NIL;

            //if (!isSurface)
                data = AfxOpenTextureRegion(tex, &rgn, AFX_TEX_OPEN_R, &rgnSize);

            AfxAssert(data);
            gl->TexImage1D(tex->glTarget, rgn.lodIdx, tex->glIntFmt, rgn.extent[0], 0, tex->glFmt, tex->glType, data); _SglThrowErrorOccuried();
            
            //if (!isSurface)
                AfxCloseTextureRegion(tex, &rgn);
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
    //AfxAdvertise("tex %p, gl/target %x, gl/intFmt %x, gl/fmt %x, gl/type %x", tex->glTarget, tex->glIntFmt, tex->glFmt, tex->glType);
    
    // build or rebuild forces total data update
    tex->lastUpdOffset[0] = (tex->lastUpdOffset[1] = (tex->lastUpdOffset[2] = 0));
    tex->lastUpdRange[0] = (tex->lastUpdRange[1] = (tex->lastUpdRange[2] = 0));
    tex->updFlags &= ~(SGL_UPD_FLAG_DEVICE_INST | SGL_UPD_FLAG_DEVICE_FLUSH);
    return err;
}

_SGL afxError _SglTexFlushDevice(afxTexture tex, glVmt const* gl) // tex must be bound
{
    afxError err = AFX_ERR_NONE;

    AfxAssert((tex->updFlags & SGL_UPD_FLAG_DEVICE_FLUSH) == SGL_UPD_FLAG_DEVICE_FLUSH);

    //afxWhd const xyz = { 0, 0, 0 };
    //afxWhd extent;
    //AfxImage.GetExtent(&tex->img, whd);

    //afxBool const isSurface = AfxTestTexture(tex, afxTextureFlag_DRAW);
    afxBool const isCubemap = AfxTestTexture(tex, AFX_TEX_FLAG_CUBEMAP);
    
    afxNat const lvlCnt = AfxGetTextureLodCount(tex);
    AfxAssert(lvlCnt);

    afxTextureRegion rgn;
    afxNat rgnSize;
    void const* data;

    switch (tex->glTarget)
    {
    case GL_TEXTURE_CUBE_MAP:
    {
        AfxAssert(isCubemap);
        AfxAssert(tex->base.imgCnt == 6);
        
        for (afxNat j = 0; j < tex->base.imgCnt; j++)
        {
            for (afxNat i = 0; i < lvlCnt; i++)
            {
                rgn.lodIdx = i;
                rgn.baseImg = j;
                rgn.imgCnt = 1;
                rgn.offset[0] = tex->lastUpdOffset[0];
                rgn.offset[1] = tex->lastUpdOffset[1];
                rgn.offset[2] = tex->lastUpdOffset[2];
                rgn.extent[0] = tex->lastUpdRange[0];
                rgn.extent[1] = tex->lastUpdRange[1];
                rgn.extent[2] = tex->lastUpdRange[2];

                data = NIL;

                //if (!isSurface)
                    data = AfxOpenTextureRegion(tex, &rgn, AFX_TEX_OPEN_R, &rgnSize);

                AfxAssert(data);
                gl->TexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + rgn.baseImg, rgn.lodIdx, rgn.offset[0], rgn.offset[1], rgn.extent[0], rgn.extent[1], tex->glFmt, tex->glType, data); _SglThrowErrorOccuried();

                //if (!isSurface)
                    AfxCloseTextureRegion(tex, &rgn);
            }
        }
        break;
    }
    case GL_TEXTURE_3D:
    {
        for (afxNat i = 0; i < lvlCnt; i++)
        {
            rgn.lodIdx = i;
            rgn.baseImg = 0;
            rgn.imgCnt = 1;
            rgn.offset[0] = tex->lastUpdOffset[0];
            rgn.offset[1] = tex->lastUpdOffset[1];
            rgn.offset[2] = tex->lastUpdOffset[2];
            rgn.extent[0] = tex->lastUpdRange[0];
            rgn.extent[1] = tex->lastUpdRange[1];
            rgn.extent[2] = tex->lastUpdRange[2];

            data = NIL;

            //if (!isSurface)
                data = AfxOpenTextureRegion(tex, &rgn, AFX_TEX_OPEN_R, &rgnSize);

            AfxAssert(data);
            gl->TexSubImage3D(tex->glTarget, rgn.lodIdx, rgn.offset[0], rgn.offset[1], rgn.offset[2], rgn.extent[0], rgn.extent[1], rgn.extent[2], tex->glFmt, tex->glType, data); _SglThrowErrorOccuried();

            //if (!isSurface)
                AfxCloseTextureRegion(tex, &rgn);
        }
        break;
    }
    case GL_TEXTURE_2D_ARRAY:
    {
        AfxAssert(tex->base.imgCnt > 1);

        for (afxNat i = 0; i < lvlCnt; i++)
        {
            rgn.lodIdx = i;
            rgn.baseImg = 0;
            rgn.imgCnt = tex->base.imgCnt;
            rgn.offset[0] = tex->lastUpdOffset[0];
            rgn.offset[1] = tex->lastUpdOffset[1];
            rgn.offset[2] = tex->lastUpdOffset[2];
            rgn.extent[0] = tex->lastUpdRange[0];
            rgn.extent[1] = tex->lastUpdRange[1];
            rgn.extent[2] = tex->lastUpdRange[2];

            data = NIL;

            //if (!isSurface)
                data = AfxOpenTextureRegion(tex, &rgn, AFX_TEX_OPEN_R, &rgnSize);

            AfxAssert(data);
            gl->TexSubImage3D(tex->glTarget, rgn.lodIdx, rgn.offset[0], rgn.offset[1], rgn.baseImg, rgn.extent[0], rgn.extent[1], rgn.imgCnt, tex->glFmt, tex->glType, data); _SglThrowErrorOccuried();

            //if (!isSurface)
                AfxCloseTextureRegion(tex, &rgn);
        }
        break;
    }
    case GL_TEXTURE_2D:
    {
        for (afxNat i = 0; i < lvlCnt; i++)
        {
            rgn.lodIdx = 0;
            rgn.baseImg = i;
            rgn.imgCnt = 1;
            rgn.offset[0] = tex->lastUpdOffset[0];
            rgn.offset[1] = tex->lastUpdOffset[1];
            rgn.offset[2] = tex->lastUpdOffset[2];
            rgn.extent[0] = tex->lastUpdRange[0];
            rgn.extent[1] = tex->lastUpdRange[1];
            rgn.extent[2] = tex->lastUpdRange[2];

            data = NIL;

            //if (!isSurface)
                data = AfxOpenTextureRegion(tex, &rgn, AFX_TEX_OPEN_R, &rgnSize);

            AfxAssert(data);
            gl->TexSubImage2D(tex->glTarget, rgn.lodIdx, rgn.offset[0], rgn.offset[1], rgn.extent[0], rgn.extent[1], tex->glFmt, tex->glType, data); _SglThrowErrorOccuried();

            //if (!isSurface)
                AfxCloseTextureRegion(tex, &rgn);
        }
        break;
    }
    case GL_TEXTURE_1D_ARRAY:
    {
        AfxAssert(tex->base.imgCnt > 1);

        for (afxNat i = 0; i < lvlCnt; i++)
        {
            rgn.lodIdx = i;
            rgn.baseImg = 0;
            rgn.imgCnt = tex->base.imgCnt;
            rgn.offset[0] = tex->lastUpdOffset[0];
            rgn.offset[1] = tex->lastUpdOffset[1];
            rgn.offset[2] = tex->lastUpdOffset[2];
            rgn.extent[0] = tex->lastUpdRange[0];
            rgn.extent[1] = tex->lastUpdRange[1];
            rgn.extent[2] = tex->lastUpdRange[2];

            data = NIL;

            //if (!isSurface)
                data = AfxOpenTextureRegion(tex, &rgn, AFX_TEX_OPEN_R, &rgnSize);

            AfxAssert(data);
            gl->TexSubImage2D(tex->glTarget, rgn.lodIdx, rgn.offset[0], rgn.baseImg, rgn.extent[0], rgn.imgCnt, tex->glFmt, tex->glType, data); _SglThrowErrorOccuried();

            //if (!isSurface)
                AfxCloseTextureRegion(tex, &rgn);
        }
        break;
    }
    case GL_TEXTURE_1D:
    {
        for (afxNat i = 0; i < lvlCnt; i++)
        {
            rgn.lodIdx = i;
            rgn.baseImg = 0;
            rgn.imgCnt = 1;
            rgn.offset[0] = tex->lastUpdOffset[0];
            rgn.offset[1] = tex->lastUpdOffset[1];
            rgn.offset[2] = tex->lastUpdOffset[2];
            rgn.extent[0] = tex->lastUpdRange[0];
            rgn.extent[1] = tex->lastUpdRange[1];
            rgn.extent[2] = tex->lastUpdRange[2];

            data = NIL;

            //if (!isSurface)
                data = AfxOpenTextureRegion(tex, &rgn, AFX_TEX_OPEN_R, &rgnSize);

            AfxAssert(data);
            gl->TexSubImage1D(tex->glTarget, rgn.lodIdx, rgn.offset[0], rgn.extent[0], tex->glFmt, tex->glType, data); _SglThrowErrorOccuried();

            //if (!isSurface)
                AfxCloseTextureRegion(tex, &rgn);
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

_SGL afxError _SglTexReinstantiateIdd(afxTexture tex, afxNat unit, glVmt const* gl)
{
    AfxEntry("tex=%p", tex);
    afxError err = AFX_ERR_NONE;
    afxDrawContext dctx = AfxGetObjectProvider(tex);
    afxContext mem = AfxGetDrawContextMemory(dctx);
    AfxAssertObjects(1, &mem, afxFcc_CTX);
    
    if (tex->glHandle)
    {
        gl->DeleteTextures(1, &tex->glHandle); _SglThrowErrorOccuried();
        tex->glHandle = 0;
    }

    tex->updFlags = SGL_UPD_FLAG_DEVICE_INST;
    
    if (!err && !tex->glHandle)
    {
        SglDetermineGlTargetInternalFormatType(tex, &tex->glTarget, &tex->glIntFmt, &tex->glFmt, &tex->glType);

        AfxAssert(NIL == tex->glHandle);
        gl->GenTextures(1, &(tex->glHandle)); _SglThrowErrorOccuried();
        gl->ActiveTexture(GL_TEXTURE0 + unit); _SglThrowErrorOccuried();
        gl->BindTexture(tex->glTarget, tex->glHandle); _SglThrowErrorOccuried();
        AfxAssert(gl->IsTexture(tex->glHandle));

        _SglTexInstDevice(tex, gl); // already clear flags
        AfxEcho("afxTexture %p hardware-side data instanced.", tex);
    }
    return err;
}

_SGL afxError _SglDpuBindAndSyncTex(sglDpuIdd* dpu, afxNat unit, afxTexture tex, glVmt const* gl)
{
    //AfxEntry("img=%p", img);
    afxError err = AFX_ERR_NONE;
    (void)dpu;

    if (tex)
    {
        AfxAssertObjects(1, &tex, afxFcc_TEX);

        if (!tex->glHandle || tex->updFlags & SGL_UPD_FLAG_DEVICE_INST)
        {
            if (_SglTexReinstantiateIdd(tex, unit, gl))
                AfxThrowError();
        }

        if ((tex->updFlags & SGL_UPD_FLAG_DEVICE_FLUSH))
        {
            AfxAssert(tex->glHandle);
            gl->ActiveTexture(GL_TEXTURE0 + unit); _SglThrowErrorOccuried();
            gl->BindTexture(tex->glTarget, tex->glHandle); _SglThrowErrorOccuried();

            _SglTexFlushDevice(tex, gl); // already clear flags
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
    return err;
}

_SGL afxError _AfxCloseTextureRegion(afxTexture tex, afxTextureRegion const *rgn)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &tex, afxFcc_TEX);
    (void)rgn;
    
    --tex->locked;

    return err;
};

_SGL afxError _AfxOpenTextureRegion(afxTexture tex, afxTextureRegion const *rgn, afxTextureOpenFlags flags, afxNat *size, void**ptr)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &tex, afxFcc_TEX);
    AfxAssert(rgn);
    AfxAssert(tex->base.lodCnt > rgn->lodIdx);
    AfxAssertRange(tex->base.imgCnt, rgn->baseImg, rgn->imgCnt);
    AfxAssertRange(tex->base.whd[0], rgn->offset[0], rgn->extent[0]);
    AfxAssertRange(tex->base.whd[1], rgn->offset[1], rgn->extent[1]);
    AfxAssertRange(tex->base.whd[2], rgn->offset[2], rgn->extent[2]);

    //afxDrawContext dctx = AfxGetObjectProvider(tex);

    if (!tex->base.maps)
        AfxBufferizeTexture(tex); // force texture allocation

    void *map = NIL;

    afxNat offset = AfxLocateTextureRegion(tex, rgn->lodIdx, rgn->baseImg, rgn->offset);

    if (size)
        *size = AfxMeasureTextureRegion(tex, rgn);

    map = &(tex->base.maps[offset]);

    ++tex->locked;

    tex->lastUpdImgBase = AfxMini(tex->lastUpdImgBase, rgn->baseImg);
    tex->lastUpdImgRange = AfxMaxi(tex->lastUpdImgRange, rgn->imgCnt);
    tex->lastUpdLodBase = AfxMini(tex->lastUpdLodBase, rgn->lodIdx);
    tex->lastUpdLodRange = AfxMaxi(tex->lastUpdLodRange, rgn->lodIdx);

    tex->lastUpdOffset[0] = AfxMini(tex->lastUpdOffset[0], rgn->offset[0]);
    tex->lastUpdOffset[1] = AfxMini(tex->lastUpdOffset[1], rgn->offset[1]);
    tex->lastUpdOffset[2] = AfxMini(tex->lastUpdOffset[2], rgn->offset[2]);

    tex->lastUpdRange[0] = AfxMaxi(tex->lastUpdRange[0], rgn->extent[0]);
    tex->lastUpdRange[1] = AfxMaxi(tex->lastUpdRange[1], rgn->extent[1]);
    tex->lastUpdRange[2] = AfxMaxi(tex->lastUpdRange[2], rgn->extent[2]);

    if (flags & AFX_TEX_OPEN_W)
        tex->updFlags |= SGL_UPD_FLAG_DEVICE_FLUSH;

    if (flags & AFX_TEX_OPEN_X)
    {
        tex->updFlags |= SGL_UPD_FLAG_DEVICE_INST;
    }
    *ptr = map;
    return err;
}

_SGL afxError _SglTexDtor(afxTexture tex)
{
    afxError err = AFX_ERR_NONE;
    AfxEntry("tex=%p", tex);
    AfxAssertObjects(1, &tex, afxFcc_TEX);

    afxDrawContext dctx = AfxGetObjectProvider(tex);
    afxContext mem = AfxGetDrawContextMemory(dctx);
    AfxAssertObjects(1, &mem, afxFcc_CTX);

    if (tex->glHandle)
    {
        _SglDctxDeleteGlRes(dctx, 1, tex->glHandle);
        tex->glHandle = 0;
    }

    if (tex->base.maps)
    {
        AfxDeallocate(mem, tex->base.maps);
    }

    return err;
}

_SGL afxError _SglTexCtor(afxTexture tex, afxCookie const* cookie)
{
    AfxEntry("tex=%p", tex);

    afxError err = AFX_ERR_NONE;

    afxTextureInfo const *texi = ((afxTextureInfo const *)cookie->udd[0]) + cookie->no;

    afxDrawContext dctx = AfxGetObjectProvider(tex);
    afxContext mem = AfxGetDrawContextMemory(dctx);
    AfxAssertObjects(1, &mem, afxFcc_CTX);

    tex->base.swizzling = NIL; //&AFX_STD_COLOR_SWIZZLING;
    tex->base.sampleCnt = AfxMaxi(texi->sampleCnt, 1);
    //tex->base.linearlyTiled = FALSE;

    tex->base.imgCnt = AfxMaxi(texi->imgCnt, 1);
    tex->base.lodCnt = AfxMaxi(texi->lodCnt, 1);

    tex->base.flags = texi->usage;

    tex->base.fmt = texi->fmt;

    tex->base.whd[0] = AfxMaxi(texi->whd[0], 1);
    tex->base.whd[1] = AfxMaxi(texi->whd[1], 1);
    tex->base.whd[2] = AfxMaxi(texi->whd[2], 1);

    tex->base.maps = NIL;

    //tex->idd = NIL;

    if (!err)
    {
        tex->base.map = _AfxOpenTextureRegion;
        tex->base.unmap = _AfxCloseTextureRegion;
        
        tex->updFlags = SGL_UPD_FLAG_DEVICE_INST;

        //tex->lastUpdImgRange = AfxGetTextureImageCount(tex);
        //tex->lastUpdLodRange = AfxGetTextureLodCount(tex);
        //AfxGetTextureExtent(tex, 0, tex->lastUpdRange);
    }

    if (err && tex->base.maps)
    {
        AfxDeallocate(mem, tex->base.maps);
    }

    return err;
}

_SGL afxClassConfig _SglTexClsConfig =
{
    .fcc = afxFcc_TEX,
    .name = "Texture",
    .unitsPerPage = 32,
    .size = sizeof(AFX_OBJECT(afxTexture)),
    .ctx = NIL,
    .ctor = (void*)_SglTexCtor,
    .dtor = (void*)_SglTexDtor
};
