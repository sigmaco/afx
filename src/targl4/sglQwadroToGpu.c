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

#define _AFX_CORE_C
//#define _AFX_EXECUTABLE_C
//#define _AFX_ICD_C
#define _AVX_DRAW_C
#define _AFX_THREAD_C
#define _AVX_DRAW_THREAD_C
#define _AVX_DRAW_SYSTEM_C
#define _AFX_SYSTEM_C
#define _AFX_DEVICE_C
#define _AVX_DRAW_DRIVER_C
#include "sgl.h"
#include "sglDdrv.h"
#include "qwadro/core/afxSystem.h"
#pragma comment(lib, "opengl32")

#define _CRT_SECURE_NO_WARNINGS 1
#define WIN32_LEAN_AND_MEAN 1
#include <dwmapi.h>
#include <shlwapi.h>
#include <shellapi.h>
#include <assert.h>

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
    
    if (AfxRasterIs1d(ras)) // Y
    {
        if (ras->base.whd[2] > 1)
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

        if (!AfxRasterIs3d(ras))
        {
            if (cubemap)
            {
                if (ras->base.whd[2] > 6)
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
                if (ras->base.whd[2] > 1)
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

_SGL GLenum AfxToGlFrontFace(afxFrontFace ff)
{
    static GLenum const v[] =
    {
        GL_CCW,
        GL_CW
    };

    return v[ff];
};

_SGL GLenum AfxToGlCullMode(afxCullMode cm)
{
    afxError err;

    static GLenum const v[] =
    {
        NIL,
        GL_FRONT,
        GL_BACK,
        GL_FRONT_AND_BACK
    };
    AfxAssert(v[afxCullMode_BACK] == GL_BACK);
    return v[cm];
};

_SGL GLenum AfxToGlFillMode(afxFillMode pm)
{
    static GLenum const v[] =
    {
        GL_FILL,
        GL_LINE,
        GL_POINT
    };

    return v[pm];
};

_SGL GLenum SglToGlCompareOp(afxCompareOp co)
{
    static GLenum const v[] =
    {
        GL_NEVER,
        GL_LESS,
        GL_LEQUAL,
        GL_EQUAL,
        GL_NOTEQUAL,
        GL_GEQUAL,
        GL_GREATER,
        GL_ALWAYS
    };

    return v[co];
};

_SGL GLuint AfxToGlShaderStage(afxShaderStage s)
{
    static GLuint const v[] =
    {
        NIL,
        GL_VERTEX_SHADER,
        GL_FRAGMENT_SHADER,
        GL_GEOMETRY_SHADER,
        GL_TESS_CONTROL_SHADER,
        GL_TESS_EVALUATION_SHADER,
        GL_COMPUTE_SHADER,
        NIL
    };

    return v[s];
};

_SGL GLuint AfxToGlShaderStageBit(afxShaderStage s)
{
    static GLuint const v[] =
    {
        NIL,
        GL_VERTEX_SHADER_BIT,
        GL_FRAGMENT_SHADER_BIT,
        GL_GEOMETRY_SHADER_BIT,
        GL_TESS_CONTROL_SHADER_BIT,
        GL_TESS_EVALUATION_SHADER_BIT,
        GL_COMPUTE_SHADER_BIT,
        NIL
    };

    return v[s];
};

_SGL GLenum AfxToGlTopology(afxPrimTopology pm)
{
    static GLuint const v[] =
    {
        GL_POINTS,

        GL_LINES,
        GL_LINE_STRIP,

        GL_TRIANGLES,
        GL_TRIANGLE_STRIP,
        GL_TRIANGLE_FAN,

        GL_LINES_ADJACENCY,
        GL_LINE_STRIP_ADJACENCY,

        GL_TRIANGLES_ADJACENCY,
        GL_TRIANGLE_STRIP_ADJACENCY,

        GL_PATCHES
    };

    return v[pm];
};

_SGL GLenum AfxToGlBufferUsage(afxBufferUsage usage)
{
    static GLenum const v[] =
    {
        GL_COPY_READ_BUFFER,
        GL_COPY_WRITE_BUFFER,
        GL_PIXEL_PACK_BUFFER,
        GL_PIXEL_UNPACK_BUFFER,
        GL_UNIFORM_BUFFER,
        GL_SHADER_STORAGE_BUFFER,
        GL_ARRAY_BUFFER,
        GL_ELEMENT_ARRAY_BUFFER,
        GL_DRAW_INDIRECT_BUFFER
    };
    //errado
    return v[usage];
};

_SGL GLenum AfxToGlStencilOp(afxStencilOp so)
{
    static GLenum const v[] =
    {
        GL_KEEP,
        GL_ZERO,
        GL_REPLACE,
        GL_INCR,
        GL_DECR,
        GL_INVERT,
        GL_INCR_WRAP,
        GL_DECR_WRAP
    };

    return v[so];
}

_SGL GLenum AfxToGlBlendOp(afxBlendOp f)
{
    static GLenum const v[] =
    {
        GL_FUNC_ADD,
        GL_FUNC_SUBTRACT,
        GL_FUNC_REVERSE_SUBTRACT,
        GL_MIN,
        GL_MAX
    };

    return v[f];
}

_SGL GLenum AfxToGlLogicOp(afxLogicOp f)
{
    afxError err;
    static GLenum const v[] =
    {
        GL_NOOP,
        GL_CLEAR,
        GL_SET,
        GL_COPY,
        GL_COPY_INVERTED,
        GL_INVERT,
        GL_AND,
        GL_NAND,
        GL_OR,
        GL_NOR,
        GL_XOR,
        GL_EQUIV,
        GL_AND_REVERSE,
        GL_AND_INVERTED,
        GL_OR_REVERSE,
        GL_OR_INVERTED
    };
    AfxAssert(v[afxLogicOp_NOP] == GL_NOOP);
    return v[f];
}

_SGL GLenum AfxToGlBlendFactor(afxBlendFactor f)
{
    static GLenum const v[] =
    {
        GL_ZERO,
        GL_ONE,
        GL_SRC_COLOR,
        GL_ONE_MINUS_SRC_COLOR,
        GL_DST_COLOR,
        GL_ONE_MINUS_DST_COLOR,
        GL_SRC_ALPHA,
        GL_ONE_MINUS_SRC_ALPHA,
        GL_DST_ALPHA,
        GL_ONE_MINUS_DST_ALPHA,
        GL_CONSTANT_COLOR,
        GL_ONE_MINUS_CONSTANT_COLOR,
        GL_CONSTANT_ALPHA,
        GL_ONE_MINUS_CONSTANT_ALPHA,
        GL_SRC_ALPHA_SATURATE,
        GL_SRC1_COLOR,
        GL_ONE_MINUS_SRC1_COLOR,
        GL_SRC1_ALPHA,
        GL_ONE_MINUS_SRC1_ALPHA
    };

    return v[f];
}

_SGL void AfxToGlVertexFormat(afxVertexFormat fmt, GLint* siz, GLenum* typ, afxNat32* stride)
{
    GLint const sizv[] =
    {
        0,

        1,
        2,
        3,
        4,

        4,
        9,
        16,

        1,
        2,
        3,
        4
    };

    //AFX_STATIC_ASSERT(afxVertexFormat_V4D == 4, "");

    GLenum const typv[] =
    {
        GL_INVALID_ENUM,

        GL_FLOAT,
        GL_FLOAT,
        GL_FLOAT,
        GL_FLOAT,

        GL_FLOAT,
        GL_FLOAT,
        GL_FLOAT,

        GL_UNSIGNED_BYTE,
        GL_UNSIGNED_BYTE,
        GL_UNSIGNED_BYTE,
        GL_UNSIGNED_BYTE,
    };

    afxNat32 const stridv[] =
    {
        0,

        sizeof(afxReal),
        sizeof(afxV2d),
        sizeof(afxV3d),
        sizeof(afxV4d),

        sizeof(afxM2d),
        sizeof(afxM3d),
        sizeof(afxM4d),

        sizeof(afxByte),
        sizeof(afxByte[2]),
        sizeof(afxByte[3]),
        sizeof(afxByte[4]),
    };

    if (siz)
        *siz = sizv[fmt];

    if (typ)
        *typ = typv[fmt];

    if (stride)
        *stride = stridv[fmt];

    //AFX_STATIC_ASSERT(afxVertexFormat_TOTAL == 5, "");

}
