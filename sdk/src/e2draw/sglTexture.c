/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *              T H E   Q W A D R O   E X E C U T I O N   E C O S Y S T E M
 *
 *                                   Public Test Build
 *                   (c) 2017 SIGMA Technology Group — Federação SIGMA
 *                                    www.sigmaco.org
 */

#define _AFX_IOS_IMPL
#define _AFX_RES_IMPL
#define _AFX_TEXTURE_C
#define _AFX_SURFACE_C
#define _AFX_DRAW_CONTEXT_C
#define _AFX_DRAW_QUEUE_C

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
    afxError err = AFX_ERR_NONE;
    AfxAssert(target);
    AfxAssert(intFmt);
    AfxAssert(fmt);
    AfxAssert(type);
    afxResult cubemap = AfxTestTexture(tex, AFX_TEX_FLAG_CUBEMAP);
    AfxAssert(tex->whd[0]); // always have at least one dimension.
    
    if (!(tex->whd[1] > 1)) // Y
    {
        if (tex->imgCnt > 1)
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
                if (tex->imgCnt / 6 > 1)
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
                if (tex->imgCnt > 1)
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

_SGL afxError _SglTexInstDevice(afxTexture tex, glVmt const* gl) // tex must be bound.
{
    afxError err = AFX_ERR_NONE;
    sglTexIdd *idd = tex->idd;

    AfxAssert((idd->updFlags & SGL_UPD_FLAG_DEVICE_INST) == SGL_UPD_FLAG_DEVICE_INST);

    //gl->TexStorage2D();

    afxNat const lvlCnt = AfxGetTextureLodCount(tex);
    AfxAssert(lvlCnt);
    gl->TexParameteri(idd->glTarget, GL_TEXTURE_BASE_LEVEL, 0); _SglThrowErrorOccuried();
    gl->TexParameteri(idd->glTarget, GL_TEXTURE_MAX_LEVEL, (lvlCnt - 1)); _SglThrowErrorOccuried();

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

    if (tex->swizzling)
    {
        GLenum arrayedSwizzle[4];
        SglToGlColorSwizzling(tex->swizzling, arrayedSwizzle);
        gl->TexParameteriv(idd->glTarget, GL_TEXTURE_SWIZZLE_RGBA, (GLint*)arrayedSwizzle); _SglThrowErrorOccuried();
    }

    afxBool const isSurface = AfxTestTexture(tex, AFX_TEX_USAGE_DRAW);
    afxBool const isCubemap = AfxTestTexture(tex, AFX_TEX_FLAG_CUBEMAP);
    AfxAssert(!(isCubemap && tex->whd[2] > 1)); // can't be both 3D and cubemap at same time

    if (lvlCnt > 1)
    {
        // what if we want to upload it?
        //gl->GenerateMipmap(tex->glTarget); _SglThrowErrorOccuried();
    }

    afxTextureRegion rgn;
    afxNat rgnSize;
    void const* data;
    
    switch (idd->glTarget)
    {
    case GL_TEXTURE_CUBE_MAP:
    {
        AfxAssert(isCubemap);
        AfxAssert(tex->imgCnt == 6);

        for (afxNat j = 0; j < tex->imgCnt; j++)
        {
            for (afxNat i = 0; i < lvlCnt; i++)
            {
                rgn.lodIdx = i;
                rgn.baseImg = j;
                rgn.imgCnt = 1;
                rgn.offset[0] = idd->lastUpdOffset[0];
                rgn.offset[1] = idd->lastUpdOffset[1];
                rgn.offset[2] = idd->lastUpdOffset[2];
                rgn.extent[0] = idd->lastUpdRange[0];
                rgn.extent[1] = idd->lastUpdRange[1];
                rgn.extent[2] = idd->lastUpdRange[2];

                data = NIL;

                //if (!isSurface)
                    data = AfxOpenTextureRegion(tex, &rgn, AFX_TEX_OPEN_R, &rgnSize);

                AfxAssert(data);
                gl->TexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + rgn.baseImg, rgn.lodIdx, idd->glIntFmt, rgn.extent[0], rgn.extent[1], 0, idd->glFmt, idd->glType, data); _SglThrowErrorOccuried();
                
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
            rgn.offset[0] = idd->lastUpdOffset[0];
            rgn.offset[1] = idd->lastUpdOffset[1];
            rgn.offset[2] = idd->lastUpdOffset[2];
            rgn.extent[0] = idd->lastUpdRange[0];
            rgn.extent[1] = idd->lastUpdRange[1];
            rgn.extent[2] = idd->lastUpdRange[2];

            data = NIL;

            //if (!isSurface)
                data = AfxOpenTextureRegion(tex, &rgn, AFX_TEX_OPEN_R, &rgnSize);

            AfxAssert(data);
            gl->TexImage3D(idd->glTarget, rgn.lodIdx, idd->glIntFmt, rgn.extent[0], rgn.extent[1], rgn.extent[2], 0, idd->glFmt, idd->glType, data); _SglThrowErrorOccuried();
            

            //if (!isSurface)
                AfxCloseTextureRegion(tex, &rgn);
        }
        break;
    }
    case GL_TEXTURE_2D_ARRAY:
    {
        AfxAssert(tex->imgCnt > 1);

        for (afxNat i = 0; i < lvlCnt; i++)
        {
            rgn.lodIdx = i;
            rgn.baseImg = i;
            rgn.imgCnt = tex->imgCnt;
            rgn.offset[0] = idd->lastUpdOffset[0];
            rgn.offset[1] = idd->lastUpdOffset[1];
            rgn.offset[2] = idd->lastUpdOffset[2];
            rgn.extent[0] = idd->lastUpdRange[0];
            rgn.extent[1] = idd->lastUpdRange[1];
            rgn.extent[2] = idd->lastUpdRange[2];

            data = NIL;

            //if (!isSurface)
                data = AfxOpenTextureRegion(tex, &rgn, AFX_TEX_OPEN_R, &rgnSize);

            AfxAssert(data);
            gl->TexImage3D(idd->glTarget, rgn.lodIdx, idd->glIntFmt, rgn.extent[0], rgn.extent[1], tex->imgCnt, 0, idd->glFmt, idd->glType, data); _SglThrowErrorOccuried();

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
            rgn.offset[0] = idd->lastUpdOffset[0];
            rgn.offset[1] = idd->lastUpdOffset[1];
            rgn.offset[2] = idd->lastUpdOffset[2];
            rgn.extent[0] = idd->lastUpdRange[0];
            rgn.extent[1] = idd->lastUpdRange[1];
            rgn.extent[2] = idd->lastUpdRange[2];

            data = NIL;

            //if (!isSurface)
                data = AfxOpenTextureRegion(tex, &rgn, AFX_TEX_OPEN_R, &rgnSize);

            AfxAssert(data);
            gl->TexImage2D(idd->glTarget, rgn.lodIdx, idd->glIntFmt, rgn.extent[0], rgn.extent[1], 0, idd->glFmt, idd->glType, data); _SglThrowErrorOccuried();
            
            //if (!isSurface)
                AfxCloseTextureRegion(tex, &rgn);
        }
        break;
    }
    case GL_TEXTURE_1D_ARRAY:
    {
        AfxAssert(tex->imgCnt > 1);

        for (afxNat i = 0; i < lvlCnt; i++)
        {
            rgn.lodIdx = i;
            rgn.baseImg = 0;
            rgn.imgCnt = tex->imgCnt;
            rgn.offset[0] = idd->lastUpdOffset[0];
            rgn.offset[1] = idd->lastUpdOffset[1];
            rgn.offset[2] = idd->lastUpdOffset[2];
            rgn.extent[0] = idd->lastUpdRange[0];
            rgn.extent[1] = idd->lastUpdRange[1];
            rgn.extent[2] = idd->lastUpdRange[2];

            data = NIL;

            //if (!isSurface)
                data = AfxOpenTextureRegion(tex, &rgn, AFX_TEX_OPEN_R, &rgnSize);

            AfxAssert(data);
            gl->TexImage2D(idd->glTarget, rgn.lodIdx, idd->glIntFmt, rgn.extent[0], tex->imgCnt, 0, idd->glFmt, idd->glType, data); _SglThrowErrorOccuried();
            
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
            rgn.offset[0] = idd->lastUpdOffset[0];
            rgn.offset[1] = idd->lastUpdOffset[1];
            rgn.offset[2] = idd->lastUpdOffset[2];
            rgn.extent[0] = idd->lastUpdRange[0];
            rgn.extent[1] = idd->lastUpdRange[1];
            rgn.extent[2] = idd->lastUpdRange[2];

            data = NIL;

            //if (!isSurface)
                data = AfxOpenTextureRegion(tex, &rgn, AFX_TEX_OPEN_R, &rgnSize);

            AfxAssert(data);
            gl->TexImage1D(idd->glTarget, rgn.lodIdx, idd->glIntFmt, rgn.extent[0], 0, idd->glFmt, idd->glType, data); _SglThrowErrorOccuried();
            
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
    idd->lastUpdOffset[0] = (idd->lastUpdOffset[1] = (idd->lastUpdOffset[2] = 0));
    idd->lastUpdRange[0] = (idd->lastUpdRange[1] = (idd->lastUpdRange[2] = 0));
    idd->updFlags &= ~(SGL_UPD_FLAG_DEVICE_INST | SGL_UPD_FLAG_DEVICE_FLUSH);
    return err;
}

_SGL afxError _SglTexFlushDevice(afxTexture tex, glVmt const* gl) // tex must be bound
{
    afxError err = AFX_ERR_NONE;
    sglTexIdd *idd = tex->idd;

    AfxAssert((idd->updFlags & SGL_UPD_FLAG_DEVICE_FLUSH) == SGL_UPD_FLAG_DEVICE_FLUSH);

    afxWhd const xyz = { 0, 0, 0 };
    afxWhd extent;
    //AfxImage.GetExtent(&tex->img, whd);

    afxBool const isSurface = AfxTestTexture(tex, AFX_TEX_USAGE_DRAW);
    afxBool const isCubemap = AfxTestTexture(tex, AFX_TEX_FLAG_CUBEMAP);
    
    afxNat const lvlCnt = AfxGetTextureLodCount(tex);
    AfxAssert(lvlCnt);

    afxTextureRegion rgn;
    afxNat rgnSize;
    void const* data;

    switch (idd->glTarget)
    {
    case GL_TEXTURE_CUBE_MAP:
    {
        AfxAssert(isCubemap);
        AfxAssert(tex->imgCnt == 6);
        
        for (afxNat j = 0; j < tex->imgCnt; j++)
        {
            for (afxNat i = 0; i < lvlCnt; i++)
            {
                rgn.lodIdx = i;
                rgn.baseImg = j;
                rgn.imgCnt = 1;
                rgn.offset[0] = idd->lastUpdOffset[0];
                rgn.offset[1] = idd->lastUpdOffset[1];
                rgn.offset[2] = idd->lastUpdOffset[2];
                rgn.extent[0] = idd->lastUpdRange[0];
                rgn.extent[1] = idd->lastUpdRange[1];
                rgn.extent[2] = idd->lastUpdRange[2];

                data = NIL;

                //if (!isSurface)
                    data = AfxOpenTextureRegion(tex, &rgn, AFX_TEX_OPEN_R, &rgnSize);

                AfxAssert(data);
                gl->TexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + rgn.baseImg, rgn.lodIdx, rgn.offset[0], rgn.offset[1], rgn.extent[0], rgn.extent[1], idd->glFmt, idd->glType, data); _SglThrowErrorOccuried();

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
            rgn.offset[0] = idd->lastUpdOffset[0];
            rgn.offset[1] = idd->lastUpdOffset[1];
            rgn.offset[2] = idd->lastUpdOffset[2];
            rgn.extent[0] = idd->lastUpdRange[0];
            rgn.extent[1] = idd->lastUpdRange[1];
            rgn.extent[2] = idd->lastUpdRange[2];

            data = NIL;

            //if (!isSurface)
                data = AfxOpenTextureRegion(tex, &rgn, AFX_TEX_OPEN_R, &rgnSize);

            AfxAssert(data);
            gl->TexSubImage3D(idd->glTarget, rgn.lodIdx, rgn.offset[0], rgn.offset[1], rgn.offset[2], rgn.extent[0], rgn.extent[1], rgn.extent[2], idd->glFmt, idd->glType, data); _SglThrowErrorOccuried();

            //if (!isSurface)
                AfxCloseTextureRegion(tex, &rgn);
        }
        break;
    }
    case GL_TEXTURE_2D_ARRAY:
    {
        AfxAssert(tex->imgCnt > 1);

        for (afxNat i = 0; i < lvlCnt; i++)
        {
            rgn.lodIdx = i;
            rgn.baseImg = 0;
            rgn.imgCnt = tex->imgCnt;
            rgn.offset[0] = idd->lastUpdOffset[0];
            rgn.offset[1] = idd->lastUpdOffset[1];
            rgn.offset[2] = idd->lastUpdOffset[2];
            rgn.extent[0] = idd->lastUpdRange[0];
            rgn.extent[1] = idd->lastUpdRange[1];
            rgn.extent[2] = idd->lastUpdRange[2];

            data = NIL;

            //if (!isSurface)
                data = AfxOpenTextureRegion(tex, &rgn, AFX_TEX_OPEN_R, &rgnSize);

            AfxAssert(data);
            gl->TexSubImage3D(idd->glTarget, rgn.lodIdx, rgn.offset[0], rgn.offset[1], rgn.baseImg, rgn.extent[0], rgn.extent[1], rgn.imgCnt, idd->glFmt, idd->glType, data); _SglThrowErrorOccuried();

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
            rgn.offset[0] = idd->lastUpdOffset[0];
            rgn.offset[1] = idd->lastUpdOffset[1];
            rgn.offset[2] = idd->lastUpdOffset[2];
            rgn.extent[0] = idd->lastUpdRange[0];
            rgn.extent[1] = idd->lastUpdRange[1];
            rgn.extent[2] = idd->lastUpdRange[2];

            data = NIL;

            //if (!isSurface)
                data = AfxOpenTextureRegion(tex, &rgn, AFX_TEX_OPEN_R, &rgnSize);

            AfxAssert(data);
            gl->TexSubImage2D(idd->glTarget, rgn.lodIdx, rgn.offset[0], rgn.offset[1], rgn.extent[0], rgn.extent[1], idd->glFmt, idd->glType, data); _SglThrowErrorOccuried();

            //if (!isSurface)
                AfxCloseTextureRegion(tex, &rgn);
        }
        break;
    }
    case GL_TEXTURE_1D_ARRAY:
    {
        AfxAssert(tex->imgCnt > 1);

        for (afxNat i = 0; i < lvlCnt; i++)
        {
            rgn.lodIdx = i;
            rgn.baseImg = 0;
            rgn.imgCnt = tex->imgCnt;
            rgn.offset[0] = idd->lastUpdOffset[0];
            rgn.offset[1] = idd->lastUpdOffset[1];
            rgn.offset[2] = idd->lastUpdOffset[2];
            rgn.extent[0] = idd->lastUpdRange[0];
            rgn.extent[1] = idd->lastUpdRange[1];
            rgn.extent[2] = idd->lastUpdRange[2];

            data = NIL;

            //if (!isSurface)
                data = AfxOpenTextureRegion(tex, &rgn, AFX_TEX_OPEN_R, &rgnSize);

            AfxAssert(data);
            gl->TexSubImage2D(idd->glTarget, rgn.lodIdx, rgn.offset[0], rgn.baseImg, rgn.extent[0], rgn.imgCnt, idd->glFmt, idd->glType, data); _SglThrowErrorOccuried();

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
            rgn.offset[0] = idd->lastUpdOffset[0];
            rgn.offset[1] = idd->lastUpdOffset[1];
            rgn.offset[2] = idd->lastUpdOffset[2];
            rgn.extent[0] = idd->lastUpdRange[0];
            rgn.extent[1] = idd->lastUpdRange[1];
            rgn.extent[2] = idd->lastUpdRange[2];

            data = NIL;

            //if (!isSurface)
                data = AfxOpenTextureRegion(tex, &rgn, AFX_TEX_OPEN_R, &rgnSize);

            AfxAssert(data);
            gl->TexSubImage1D(idd->glTarget, rgn.lodIdx, rgn.offset[0], rgn.extent[0], idd->glFmt, idd->glType, data); _SglThrowErrorOccuried();

            //if (!isSurface)
                AfxCloseTextureRegion(tex, &rgn);
        }
        break;
    }
    default:
        AfxThrowError();
        break;
    }
    idd->lastUpdOffset[0] = (idd->lastUpdOffset[1] = (idd->lastUpdOffset[2] = 0));
    idd->lastUpdRange[0] = (idd->lastUpdRange[1] = (idd->lastUpdRange[2] = 0));
    idd->updFlags &= ~(SGL_UPD_FLAG_DEVICE_FLUSH);
    return err;
}

_SGL afxError _SglTexReinstantiateIdd(afxTexture tex, afxNat unit, glVmt const* gl)
{
    AfxEntry("tex=%p", tex);
    afxError err = AFX_ERR_NONE;
    afxDrawContext dctx = AfxGetTextureContext(tex);

    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);
    struct _afxDctxD *dctxD;
    _AfxGetDctxD(dctx, &dctxD, dsysD);
    AfxAssertType(dctxD, AFX_FCC_DCTX);

    afxContext mem = AfxGetDrawContextMemory(dctx);
    AfxAssertObjects(1, &mem, AFX_FCC_MEM);
    sglTexIdd *idd;

    if (!(idd = tex->idd))
    {
        if (!(tex->idd = (idd = AfxAllocate(mem, sizeof(*idd), 0, AfxSpawnHint())))) AfxThrowError();
        else
        {
            *idd = (sglTexIdd) { 0 };

            idd->updFlags = SGL_UPD_FLAG_DEVICE_INST;

            idd->lastUpdImgRange = AfxGetTextureImageCount(tex);
            idd->lastUpdLodRange = AfxGetTextureLodCount(tex);
            AfxGetTextureExtent(tex, 0, idd->lastUpdRange);

        }
    }
    else
    {
        if (idd->glHandle)
        {
            gl->DeleteTextures(1, &idd->glHandle); _SglThrowErrorOccuried();
            idd->glHandle = 0;
        }
        *idd = (sglTexIdd) { 0 };
        idd->updFlags = SGL_UPD_FLAG_DEVICE_INST;
    }

    if (!err && idd && !idd->glHandle)
    {
        SglDetermineGlTargetInternalFormatType(tex, &idd->glTarget, &idd->glIntFmt, &idd->glFmt, &idd->glType);

        AfxAssert(NIL == idd->glHandle);
        gl->GenTextures(1, &(idd->glHandle)); _SglThrowErrorOccuried();
        gl->ActiveTexture(GL_TEXTURE0 + unit); _SglThrowErrorOccuried();
        gl->BindTexture(idd->glTarget, idd->glHandle); _SglThrowErrorOccuried();
        AfxAssert(gl->IsTexture(idd->glHandle));

        _SglTexInstDevice(tex, gl); // already clear flags
        AfxEcho("afxTexture %p hardware-side data instanced.", tex);
    }
    return err;
}

_SGL afxError _SglDqueBindAndSyncTex(afxDrawQueue dque, afxNat unit, afxTexture tex, glVmt const* gl)
{
    //AfxEntry("img=%p", img);
    afxError err = AFX_ERR_NONE;
    sglDqueIdd* dqueIdd = dque->idd;

    if (tex)
    {
        AfxAssertObject(tex, AFX_FCC_TEX);

        sglTexIdd*idd = tex->idd;

        if (!tex->idd || idd->updFlags & SGL_UPD_FLAG_DEVICE_INST)
        {
            if (_SglTexReinstantiateIdd(tex, unit, gl))
                AfxThrowError();

            idd = tex->idd;
        }

        if (!tex->idd) AfxThrowError();
        else
        {
            idd = tex->idd;

            if ((idd->updFlags & SGL_UPD_FLAG_DEVICE_FLUSH))
            {
                AfxAssert(idd->glHandle);
                gl->ActiveTexture(GL_TEXTURE0 + unit); _SglThrowErrorOccuried();
                gl->BindTexture(idd->glTarget, idd->glHandle); _SglThrowErrorOccuried();

                _SglTexFlushDevice(tex, gl); // already clear flags
            }
            else
            {
                AfxAssert(idd->glHandle);
                AfxAssert(idd->glTarget);

                if (gl->BindTextureUnit)
                {
                    gl->BindTextureUnit(unit, idd->glHandle); _SglThrowErrorOccuried();
                }
                else
                {
                    gl->ActiveTexture(GL_TEXTURE0 + unit); _SglThrowErrorOccuried();
                    gl->BindTexture(idd->glTarget, idd->glHandle); _SglThrowErrorOccuried();
                }
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
    AfxAssertObject(tex, AFX_FCC_TEX);
    sglTexIdd *idd = tex->idd;

    if (idd)
    {
        --idd->locked;
    }
    return err;
};

_SGL afxError _AfxOpenTextureRegion(afxTexture tex, afxTextureRegion const *rgn, afxTextureOpenFlags flags, afxNat *size, void**ptr)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(tex, AFX_FCC_TEX);
    AfxAssert(rgn);
    AfxAssert(tex->lodCnt > rgn->lodIdx);
    AfxAssertRange(tex->imgCnt, rgn->baseImg, rgn->imgCnt);
    AfxAssertRange(tex->whd[0], rgn->offset[0], rgn->extent[0]);
    AfxAssertRange(tex->whd[1], rgn->offset[1], rgn->extent[1]);
    AfxAssertRange(tex->whd[2], rgn->offset[2], rgn->extent[2]);

    afxDrawContext dctx = AfxGetTextureContext(tex);

    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);
    struct _afxDctxD *dctxD;
    _AfxGetDctxD(dctx, &dctxD, dsysD);
    AfxAssertType(dctxD, AFX_FCC_DCTX);


    if (!tex->maps)
        AfxBufferizeTexture(tex); // force texture allocation

    void *map = NIL;

    afxNat offset = AfxLocateTextureRegion(tex, rgn->lodIdx, rgn->baseImg, rgn->offset);

    if (size)
        *size = AfxMeasureTextureRegion(tex, rgn);

    map = &(tex->maps[offset]);

    sglTexIdd *idd = tex->idd;

    if (idd)
    {
        ++idd->locked;

        idd->lastUpdImgBase = AfxMini(idd->lastUpdImgBase, rgn->baseImg);
        idd->lastUpdImgRange = AfxMaxi(idd->lastUpdImgRange, rgn->imgCnt);
        idd->lastUpdLodBase = AfxMini(idd->lastUpdLodBase, rgn->lodIdx);
        idd->lastUpdLodRange = AfxMaxi(idd->lastUpdLodRange, rgn->lodIdx);

        idd->lastUpdOffset[0] = AfxMini(idd->lastUpdOffset[0], rgn->offset[0]);
        idd->lastUpdOffset[1] = AfxMini(idd->lastUpdOffset[1], rgn->offset[1]);
        idd->lastUpdOffset[2] = AfxMini(idd->lastUpdOffset[2], rgn->offset[2]);

        idd->lastUpdRange[0] = AfxMaxi(idd->lastUpdRange[0], rgn->extent[0]);
        idd->lastUpdRange[1] = AfxMaxi(idd->lastUpdRange[1], rgn->extent[1]);
        idd->lastUpdRange[2] = AfxMaxi(idd->lastUpdRange[2], rgn->extent[2]);

        if (flags & AFX_TEX_OPEN_W)
            idd->updFlags |= SGL_UPD_FLAG_DEVICE_FLUSH;

        if (flags & AFX_TEX_OPEN_X)
        {
            idd->updFlags |= SGL_UPD_FLAG_DEVICE_INST;

            if (tex->flags & AFX_TEX_USAGE_DRAW)
            {
                sglSurfIdd *surfIdd = ((afxSurface)tex)->idd;
                
                if (surfIdd)
                {
                    surfIdd->updFlags |= SGL_UPD_FLAG_DEVICE_INST;
                }
            }
        }
    }
    *ptr = map;
    return err;
}

_SGL afxError _SglTexDtor(afxTexture tex)
{
    afxError err = AFX_ERR_NONE;
    AfxEntry("img=%p", tex);
    AfxAssertObject(tex, AFX_FCC_TEX);

    sglTexIdd *idd = tex->idd;

    if (idd)
    {
        afxDrawContext dctx = AfxGetTextureContext(tex);

        afxDrawSystem dsys;
        AfxGetDrawSystem(&dsys);
        AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
        struct _afxDsysD* dsysD;
        _AfxGetDsysD(dsys, &dsysD);
        AfxAssertType(dsysD, AFX_FCC_DSYS);
        struct _afxDctxD *dctxD;
        _AfxGetDctxD(dctx, &dctxD, dsysD);
        AfxAssertType(dctxD, AFX_FCC_DCTX);

        afxContext mem = AfxGetDrawContextMemory(dctx);
        AfxAssertObjects(1, &mem, AFX_FCC_MEM);

        if (idd->glHandle)
        {
            _SglDeleteGlRes(dctx, 1, idd->glHandle);
            idd->glHandle = 0;
        }
        AfxDeallocate(mem, idd);
        tex->idd = NIL;
    }

    return err;
}

_SGL _afxTexVmt _SglTexVmt =
{
    _SglTexDtor,
    _AfxOpenTextureRegion,
    _AfxCloseTextureRegion
};

_SGL afxError _SglTexCtor(afxTexture tex)
{
    AfxEntry("tex=%p", tex);
    afxError err = AFX_ERR_NONE;
    tex->idd = NIL;
    tex->vmt = &_SglTexVmt;
    return err;
}
