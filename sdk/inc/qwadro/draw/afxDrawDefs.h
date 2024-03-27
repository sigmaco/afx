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

// This section is part of SIGMA GL/2.

#ifndef AFX_DRAW_DEFS_H
#define AFX_DRAW_DEFS_H

#include "qwadro/mem/afxArray.h"
#include "qwadro/math/afxFrustum.h"
#include "qwadro/core/afxThread.h"
#include "qwadro/io/afxStream.h"
#include "qwadro/core/afxRestring.h"

#ifndef __e2draw__
#   ifdef _DEBUG
#       define AVX _AFXIMPORT extern 
#       define AVXINL _AFXIMPORT extern inline
#   else
#       define AVX _AFXIMPORT extern 
#       define AVXINL _AFXIMPORT extern inline
#   endif
#else
#   ifdef _DEBUG
#       define _AVX _AFXEXPORT
#       define AVX _AFXEXPORT extern 
#       define _AVXINL _AFXEXPORT inline
#       define AVXINL _AFXEXPORT extern inline
#   else
#       define _AVX _AFXEXPORT
#       define AVX _AFXEXPORT extern 
#       define _AVXINL _AFXEXPORT inline
#       define AVXINL _AFXEXPORT extern inline
#   endif
#endif//__e2draw__

// Object handles defined by SIGMA Draw System

AFX_DEFINE_HANDLE(afxDrawDevice);
AFX_DEFINE_HANDLE(afxDrawThread);
//typedef afxObject afxDrawThread;
AFX_DEFINE_HANDLE(afxDrawContext);
//typedef afxObject afxDrawContext;
AFX_DEFINE_HANDLE(afxDrawQueue);
//typedef afxObject afxDrawQueue;
AFX_DEFINE_HANDLE(afxDrawInput);
//typedef afxObject afxDrawInput;
AFX_DEFINE_HANDLE(afxDrawStream);
//typedef afxObject afxDrawStream;
AFX_DEFINE_HANDLE(afxDrawOutput);
//typedef afxObject afxDrawOutput;

AFX_DEFINE_HANDLE(afxDrawOperation);
AFX_DEFINE_HANDLE(afxDrawTechnique);
AFX_DEFINE_HANDLE(afxPipeline);
AFX_DEFINE_HANDLE(afxRasterizer);
AFX_DEFINE_HANDLE(afxVertexInput);
//typedef afxObject afxPipeline;
AFX_DEFINE_HANDLE(afxShader);
//typedef afxObject afxShader;
AFX_DEFINE_HANDLE(afxBindSchema);
//typedef afxObject afxBindSchema;
AFX_DEFINE_HANDLE(afxSampler);
//typedef afxObject afxSampler;

AFX_DEFINE_HANDLE(afxQueryPool);

AFX_DEFINE_HANDLE(afxBuffer);
//typedef afxObject afxBuffer;

AFX_DEFINE_HANDLE(afxVertexStream);

AFX_DEFINE_HANDLE(afxVertexBuffer);
AFX_DEFINE_HANDLE(afxIndexBuffer);

AFX_DEFINE_HANDLE(afxRaster);
//typedef afxObject afxRaster;
//typedef afxObject afxSurface;
AFX_DEFINE_HANDLE(afxCanvas);
//typedef afxObject afxCanvas;

AFX_DEFINE_HANDLE(afxFence);
AFX_DEFINE_HANDLE(afxSemaphore);

AFX_DEFINE_HANDLE(afxCamera);

AFX_DEFINE_HANDLE(afxTypography);

typedef afxNat afxSimd(afxWhd[3]);
typedef afxReal afxSimd(afxNdc[3]);

typedef afxNat32 afxCmdId;

AFX_DEFINE_STRUCT(afxFileDrop)
{
    afxNat                      x, y;
    afxArray                    files; // afxChar const*
};

typedef enum
{
    afxPrimTopology_POINT_LIST, // Mesh is made from points.

    afxPrimTopology_LINE_LIST, // Mesh is made from lines.
    afxPrimTopology_LINE_STRIP, // Mesh is a line strip.

    afxPrimTopology_TRI_LIST, // Mesh is made from triangles.
    afxPrimTopology_TRI_STRIP,
    afxPrimTopology_TRI_FAN,

    afxPrimTopology_LINE_LIST_ADJACENT,
    afxPrimTopology_LINE_STRIP_ADJACENT,

    afxPrimTopology_TRI_LIST_ADJACENT,
    afxPrimTopology_TRI_STRIP_ADJACENT,

    afxPrimTopology_PATCH_LIST,

    afxPrimTopology_TOTAL
} afxPrimTopology;

typedef enum
{
    afxFormat_REAL,
    afxFormat_NAT,
    afxFormat_INT,
    afxFormat_V2D,
    afxFormat_V2DN,
    afxFormat_V2DI,
    afxFormat_V3D,
    afxFormat_V3DN,
    afxFormat_V3DI,
    afxFormat_V4D,
    afxFormat_V4DN,
    afxFormat_V4DI,

    afxFormat_TOTAL
} afxFormat;


typedef enum afxStencilOp
{
    afxStencilOp_KEEP = 0,
    afxStencilOp_ZERO = 1,
    afxStencilOp_REPLACE = 2,
    afxStencilOp_INC_AND_CLAMP = 3,
    afxStencilOp_DEC_AND_CLAMP = 4,
    afxStencilOp_INVERT = 5,
    afxStencilOp_INC_AND_WRAP = 6,
    afxStencilOp_DEC_AND_WRAP = 7,

    afxStencilOp_TOTAL
} afxStencilOp;

typedef enum afxLogicOp
/// In the explanation of the logical operations, s represents the incoming color and d represents the color in the frame buffer.
/// Standard C-language operators are used.
/// As these bitwise operators suggest, the logical operation is applied independently to each bit pair of the source and destination colors.
{
    afxLogicOp_NOP,         // d
    afxLogicOp_CLEAR,       // 0
    afxLogicOp_SET,         // 1
    afxLogicOp_COPY,        // s
    afxLogicOp_COPY_INV,    // ~s
    afxLogicOp_INV,         // ~d
    afxLogicOp_AND,         // s & d
    afxLogicOp_NAND,        // ~(s & d)
    afxLogicOp_OR,          // s | d
    afxLogicOp_NOR,         // ~(s | d)
    afxLogicOp_XOR,         // s ^ d
    afxLogicOp_EQUIV,       // ~(s ^ d)

    afxLogicOp_AND_REV,     // s & ~d
    afxLogicOp_AND_INV,     // ~s & d
    afxLogicOp_OR_REV,      // s | ~d
    afxLogicOp_OR_INV,      // ~s | d
    
    afxLogicOp_TOTAL
} afxLogicOp;

typedef enum afxCullMode
{
    // NIL = DISABLED
    afxCullMode_FRONT = AfxGetBitOffset(0),
    afxCullMode_BACK  = AfxGetBitOffset(1),
    afxCullMode_BOTH  = afxCullMode_FRONT | afxCullMode_BACK, // sum to cull both faces --- who would use it?
    
    afxCullMode_TOTAL
} afxCullMode;

typedef enum
{
    afxFrontFace_CCW,
    afxFrontFace_CW,

    afxFrontFace_TOTAL
} afxFrontFace;

typedef enum
{
    afxFillMode_SOLID,
    afxFillMode_EDGE,
    afxFillMode_POINT,

    afxFillMode_TOTAL
} afxFillMode;

typedef enum
/// Fragment output blending factors.
/// Rs0,Gs0,Bs0 and As0 represent the first source color R, G, B, and A components, respectively, for the fragment output location corresponding to the color attachment being blended.
/// Rs1,Gs1,Bs1 and As1 represent the second source color R, G, B, and A components, respectively, used in dual source blending modes, for the fragment output location corresponding to the color attachment being blended.
/// Rd,Gd,Bd and Ad represent the R, G, B, and A components of the destination color. That is, the color currently in the corresponding color attachment for this fragment/sample.
/// Rc,Gc,Bc and Ac represent the blend constant R, G, B, and A components, respectively.
{
    afxBlendFactor_ZERO,
    /// (0,0,0)
    /// 0

    afxBlendFactor_ONE,
    /// (1,1,1)
    /// 1

    afxBlendFactor_SRC_RGB,
    /// (Rs0,Gs0,Bs0)
    /// As0

    afxBlendFactor_ONE_MINUS_SRC_RGB,
    /// (1-Rs0,1-Gs0,1-Bs0)
    /// 1-As0

    afxBlendFactor_DST_RGB,
    /// (Rd,Gd,Bd)
    /// Ad

    afxBlendFactor_ONE_MINUS_DST_RGB,
    /// (1-Rd,1-Gd,1-Bd)
    /// 1-Ad

    afxBlendFactor_SRC_A,
    /// (As0,As0,As0)
    /// As0

    afxBlendFactor_ONE_MINUS_SRC_A,
    /// (1-As0,1-As0,1-As0)
    /// 1-As0

    afxBlendFactor_DST_A,
    /// (Ad,Ad,Ad)
    /// Ad

    afxBlendFactor_ONE_MINUS_DST_A,
    /// (1-Ad,1-Ad,1-Ad)
    /// 1-Ad

    afxBlendFactor_CONSTANT_RGB,
    /// (Rc,Gc,Bc)
    /// Ac

    afxBlendFactor_ONE_MINUS_CONST_RGB,
    /// (1-Rc,1-Gc,1-Bc)
    /// 1-Ac

    afxBlendFactor_CONST_A,
    /// (Ac,Ac,Ac)
    /// Ac

    afxBlendFactor_ONE_MINUS_CONST_A,
    /// (1-Ac,1-Ac,1-Ac)
    /// 1-Ac

    afxBlendFactor_SRC_A_SATURATE,
    /// (f,f,f); f = min(As0,1-Ad)
    /// 1

    afxBlendFactor_SRC1_RGB,
    /// (Rs1,Gs1,Bs1)
    /// As1

    afxBlendFactor_ONE_MINUS_SRC1_RGB,
    /// (1-Rs1,1-Gs1,1-Bs1)
    /// 1-As1

    afxBlendFactor_SRC1_A,
    /// (As1,As1,As1)
    /// As1

    afxBlendFactor_ONE_MINUS_SRC1_A,
    /// (1-As1,1-As1,1-As1)
    /// 1-As1
} afxBlendFactor;

typedef enum
/// Fragment output blending operations.
/// Once the source and destination blend factors have been selected, they along with the source and destination components are passed to the blending operations.
/// RGB and alpha components can use different operations.
{
    afxBlendOp_ADD,
    /// R = Rs0 × Sr + Rd × Dr
    /// G = Gs0 × Sg + Gd × Dg
    /// B = Bs0 × Sb + Bd × Db
    /// A = As0 × Sa + Ad × Da

    afxBlendOp_SUBTRACT,
    /// R = Rs0 × Sr - Rd × Dr
    /// G = Gs0 × Sg - Gd × Dg
    /// B = Bs0 × Sb - Bd × Db
    /// A = As0 × Sa - Ad × Da

    afxBlendOp_REVERSE_SUBTRACT,
    /// R = Rd × Dr - Rs0 × Sr
    /// G = Gd × Dg - Gs0 × Sg
    /// B = Bd × Db - Bs0 × Sb
    /// A = Ad × Da - As0 × Sa

    afxBlendOp_MIN,
    /// R = min(Rs0, Rd)
    /// G = min(Gs0, Gd)
    /// B = min(Bs0, Bd)
    /// A = min(As0, Ad)

    afxBlendOp_MAX
    /// R = max(Rs0, Rd)
    /// G = max(Gs0, Gd)
    /// B = max(Bs0, Bd)
    /// A = max(As0, Ad)
} afxBlendOp;

typedef enum
{
    afxCompareOp_NEVER, // NIL = Never passes
    afxCompareOp_LESS, // Passes if the incoming depth value is less than the stored depth value.
    afxCompareOp_LEQUAL, // Passes if the incoming depth value is less than or equal to the stored depth value.
    afxCompareOp_EQUAL, // Passes if the incoming depth value is equal to the stored depth value.
    afxCompareOp_NOTEQUAL, // Passes if the incoming depth value is not equal to the stored depth value.
    afxCompareOp_GEQUAL, // Passes if the incoming depth value is greater than or equal to the stored depth value.
    afxCompareOp_GREATER, // Passes if the incoming depth value is greater than the stored depth value.
    afxCompareOp_ALWAYS, // Always passes.

    afxCompareOp_TOTAL
} afxCompareOp;

typedef enum afxVertexFormat
{
    //NIL,
    afxVertexFormat_REAL = 1,
    afxVertexFormat_V2D,
    afxVertexFormat_V3D,
    afxVertexFormat_V4D,
    afxVertexFormat_M2D,
    afxVertexFormat_M3D,
    afxVertexFormat_M4D,

    afxVertexFormat_BYTE,
    afxVertexFormat_V2B,
    afxVertexFormat_V3B,
    afxVertexFormat_V4B,
    
    afxVertexFormat_TOTAL
} afxVertexFormat;

typedef enum afxShaderStage
{
    // NIL = reserved for composing purposes.
    afxShaderStage_VERTEX = 1,
    afxShaderStage_FRAGMENT,
    afxShaderStage_PRIMITIVE,
    afxShaderStage_TESS_CTRL,
    afxShaderStage_TESS_EVAL,
    afxShaderStage_COMPUTE,

    afxShaderStage_TOTAL
} afxShaderStage;

AVX afxString const g_str_Qwadro;
AVX afxString const g_str_Draw;
AVX afxString const g_str_Shader;
AVX afxString const g_str_Include;
AVX afxString const g_str_Resource;
AVX afxString const g_str_In;
AVX afxString const g_str_Out;
AVX afxString const g_str_Stage;
AVX afxString const g_str_stage;
AVX afxString const g_str_id;
AVX afxString const g_str_name;
AVX afxString const g_str_entry;
AVX afxString const g_str_Entry;
AVX afxString const g_str_flag;
AVX afxString const g_str_Flag;
AVX afxString const g_str_uri;
AVX afxString const g_str_set;
AVX afxString const g_str_binding;
AVX afxString const g_str_type;
AVX afxString const g_str_count;
AVX afxString const g_str_location;
AVX afxString const g_str_format;
AVX afxString const g_str_topology;
AVX afxString const g_str_restart;

AVX afxString const g_str_Pipeline;
AVX afxString const g_str_DrawOperation;
AVX afxString const g_str_Operation;
AVX afxString const g_str_Technique;
AVX afxString const g_str_Pass;
AVX afxString const g_str_Rasterization;
AVX afxString const g_str_DepthHandling;
AVX afxString const g_str_Input;
AVX afxString const g_str_PrimitiveTopology;

AVX afxString const g_str_DepthClampEnable;
AVX afxString const g_str_RasterizerDiscardEnable;
AVX afxString const g_str_FillMode;
AVX afxString const g_str_CullMode;
AVX afxString const g_str_FrontFace;
AVX afxString const g_str_DepthBias;
AVX afxString const g_str_LineWidth;

AVX afxString const g_str_DepthTestEnable;
AVX afxString const g_str_DepthWriteEnable;
AVX afxString const g_str_DepthCompareOp;
AVX afxString const g_str_DepthBoundsTest;
AVX afxString const g_str_StencilTest;
AVX afxString const g_str_Front;
AVX afxString const g_str_Back;
AVX afxString const g_str_fail;
AVX afxString const g_str_pass;
AVX afxString const g_str_depthFail;
AVX afxString const g_str_compare;
AVX afxString const g_str_min;
AVX afxString const g_str_max;

AVX afxString const g_str_true;
AVX afxString const g_str_false;

AVX afxString const g_str_enable;
AVX afxString const g_str_constant;
AVX afxString const g_str_slope;
AVX afxString const g_str_clamp;


AVX afxPrimTopology         AfxFindPrimitiveTopology(afxString const *str);
AVX afxLogicOp              AfxFindLogicOp(afxString const *str);
AVX afxStencilOp            AfxFindStencilOp(afxString const *str);
AVX afxCompareOp            AfxFindCompareOp(afxString const *str);
AVX afxCullMode             AfxFindCullMode(afxString const *str);
AVX afxFrontFace            AfxFindFrontFace(afxString const *str);
AVX afxFillMode             AfxFindFillMode(afxString const *str);
AVX afxShaderStage          AfxFindShaderStage(afxString const *str);
AVX afxVertexFormat         AfxFindVertexFormat(afxString const *str);

AVX afxString const*        AfxStringifyPrimitiveTopology(afxPrimTopology pt, afxRestring *out);
AVX afxString const*        AfxStringifyLogicOp(afxLogicOp f, afxRestring *out);
AVX afxString const*        AfxStringifyStencilOp(afxStencilOp f, afxRestring *out);
AVX afxString const*        AfxStringifyCompareOp(afxCompareOp f, afxRestring *out);
AVX afxString const*        AfxStringifyCullMode(afxCullMode cm, afxRestring *out);
AVX afxString const*        AfxStringifyFrontFace(afxFrontFace ff, afxRestring *out);
AVX afxString const*        AfxStringifyFillMode(afxFillMode fm, afxRestring *out);
AVX afxString const*        AfxStringifyShaderStage(afxShaderStage ss, afxRestring *out);
AVX afxString const*        AfxStringifyVertexFormat(afxVertexFormat fmt, afxRestring *out);

AVX afxNat AfxVertexFormatGetSize(afxVertexFormat fmt);

AVX afxResult       AfxResolveFormat(afxFormat fmt, afxFormat *subfmt, afxSize *subSiz, afxSize *vecSiz, afxSize *siz);

AVX afxError _AfxAssertWhd(afxWhd total_, afxWhd base_, afxWhd range_, afxHint const hint);
//#define                     AfxAssertWhd(max_,base_,range_) _AfxAssertWhd((max_),(base_),(range_))

AVXINL void AfxMinWhd(afxNat whd[3], afxNat const a[3], afxNat const b[3])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(whd);
    AfxAssert(a);
    AfxAssert(b);
    whd[0] = a[0] < b[0] ? a[0] : b[0];
    whd[1] = a[1] < b[1] ? a[1] : b[1];
    whd[2] = a[2] < b[2] ? a[2] : b[2];
}

AVXINL void AfxMaxWhd(afxNat whd[3], afxNat const a[3], afxNat const b[3])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(whd);
    AfxAssert(a);
    AfxAssert(b);
    whd[0] = a[0] > b[0] ? a[0] : b[0];
    whd[1] = a[1] > b[1] ? a[1] : b[1];
    whd[2] = a[2] > b[2] ? a[2] : b[2];
}

AVXINL afxNat AfxMagWhd(afxNat const whd[3])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(whd);
    return whd[0] * whd[1] * whd[2];
}

AVXINL void AfxZeroWhd(afxNat whd[3])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(whd);
    whd[0] = 0;
    whd[1] = 0;
    whd[2] = 0;
}

AVXINL void AfxCopyWhd(afxNat whd[3], afxNat const in[3])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(whd);
    AfxAssert(in);
    whd[0] = in[0];
    whd[1] = in[1];
    whd[2] = in[2];
}

AVXINL void AfxClampWhd(afxNat whd[3], afxNat const in[3], afxNat const min[3], afxNat const max[3])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(whd);
    AfxAssert(in);
    AfxAssert(min);
    AfxAssert(max);
    AfxAssert(min[0] <= max[0]);
    AfxAssert(min[1] <= max[1]);
    AfxAssert(min[2] <= max[2]);
    AfxMaxWhd(whd, min, in);
    AfxMinWhd(whd, max, whd);
}

#endif//AFX_DRAW_DEFS_H
