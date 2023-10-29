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
 *                   (c) 2017 SIGMA Technology Group — Federaзгo SIGMA
 *                                    www.sigmaco.org
 */

// This section is part of SIGMA GL.

#ifndef AFX_DRAW_DEFS_H
#define AFX_DRAW_DEFS_H

#include "afx/core/afxArray.h"
#include "afx/math/afxFrustum.h"
#include "afx/core/afxThread.h"
#include "afx/core/afxCondition.h"
#include "afx/core/afxStream.h"

// Object handles defined by SIGMA Future Draw Systems

AFX_DEFINE_HANDLE(afxDrawSystem);
//typedef afxHandle afxDrawSystem;
AFX_DEFINE_HANDLE(afxDrawDevice);
AFX_DEFINE_HANDLE(afxDrawThread);
//typedef afxHandle afxDrawThread;
AFX_DEFINE_HANDLE(afxDrawContext);
//typedef afxHandle afxDrawContext;
AFX_DEFINE_HANDLE(afxDrawQueue);
//typedef afxHandle afxDrawQueue;
AFX_DEFINE_HANDLE(afxDrawInput);
//typedef afxHandle afxDrawInput;
AFX_DEFINE_HANDLE(afxDrawScript);
//typedef afxHandle afxDrawScript;
AFX_DEFINE_HANDLE(afxDrawOutput);
//typedef afxHandle afxDrawOutput;

AFX_DEFINE_HANDLE(afxDrawOperation);
AFX_DEFINE_HANDLE(afxPipeline);
//typedef afxHandle afxPipeline;
AFX_DEFINE_HANDLE(afxShader);
//typedef afxHandle afxShader;
AFX_DEFINE_HANDLE(afxPipelineRig);
//typedef afxHandle afxPipelineRig;
AFX_DEFINE_HANDLE(afxSampler);
//typedef afxHandle afxSampler;

AFX_DEFINE_HANDLE(afxBuffer);
//typedef afxHandle afxBuffer;
AFX_DEFINE_HANDLE(afxIndexBuffer);
//typedef afxHandle afxIndexBuffer;
AFX_DEFINE_HANDLE(afxVertexBuffer);
//typedef afxHandle afxVertexBuffer;

AFX_DEFINE_HANDLE(afxTexture);
//typedef afxHandle afxTexture;
AFX_DEFINE_HANDLE(afxSurface);
//typedef afxHandle afxSurface;
AFX_DEFINE_HANDLE(afxCanvas);
//typedef afxHandle afxCanvas;

typedef afxNat afxSimd(afxWhd[3]);
typedef afxReal afxSimd(afxNdc[3]);

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
    afxCullMode_FRONT = AFX_BIT_OFFSET(0),
    afxCullMode_BACK  = AFX_BIT_OFFSET(1),
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
    // NIL = DISABLED
    afxBlendFactor_ZERO = 1,
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
    /// R = Rs0 Ч Sr + Rd Ч Dr
    /// G = Gs0 Ч Sg + Gd Ч Dg
    /// B = Bs0 Ч Sb + Bd Ч Db
    /// A = As0 Ч Sa + Ad Ч Da

    afxBlendOp_SUBTRACT,
    /// R = Rs0 Ч Sr - Rd Ч Dr
    /// G = Gs0 Ч Sg - Gd Ч Dg
    /// B = Bs0 Ч Sb - Bd Ч Db
    /// A = As0 Ч Sa - Ad Ч Da

    afxBlendOp_REVERSE_SUBTRACT,
    /// R = Rd Ч Dr - Rs0 Ч Sr
    /// G = Gd Ч Dg - Gs0 Ч Sg
    /// B = Bd Ч Db - Bs0 Ч Sb
    /// A = Ad Ч Da - As0 Ч Sa

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
    afxVertexFormat_REAL = 1,
    afxVertexFormat_V2D,
    afxVertexFormat_V3D,
    afxVertexFormat_V4D,

    //afxVertexFormat_V4B, // afxInt8[4]
    //afxVertexFormat_V8B, // afxInt8[8]

    afxVertexFormat_TOTAL
} afxVertexFormat;

typedef enum afxShaderStage
{
    // NIL = reserved for composing purposes.
    afxShaderStage_VERTEX = 1,
    afxShaderStage_PIXEL,
    afxShaderStage_GEOMETRY,
    afxShaderStage_TESS_CTRL,
    afxShaderStage_TESS_EVAL,
    afxShaderStage_COMPUTE,

    afxShaderStage_TOTAL
} afxShaderStage;

AFX afxString const g_str_Qwadro;
AFX afxString const g_str_Draw;
AFX afxString const g_str_Shader;
AFX afxString const g_str_Include;
AFX afxString const g_str_Resource;
AFX afxString const g_str_In;
AFX afxString const g_str_Out;
AFX afxString const g_str_Stage;
AFX afxString const g_str_stage;
AFX afxString const g_str_name;
AFX afxString const g_str_entry;
AFX afxString const g_str_Entry;
AFX afxString const g_str_flag;
AFX afxString const g_str_Flag;
AFX afxString const g_str_uri;
AFX afxString const g_str_set;
AFX afxString const g_str_binding;
AFX afxString const g_str_type;
AFX afxString const g_str_count;
AFX afxString const g_str_location;
AFX afxString const g_str_format;
AFX afxString const g_str_topology;
AFX afxString const g_str_restart;

AFX afxString const g_str_Pipeline;
AFX afxString const g_str_DrawOperation;
AFX afxString const g_str_Operation;
AFX afxString const g_str_Technique;
AFX afxString const g_str_Pass;
AFX afxString const g_str_Rasterization;
AFX afxString const g_str_DepthHandling;
AFX afxString const g_str_Input;
AFX afxString const g_str_PrimitiveTopology;

AFX afxString const g_str_DepthClampEnable;
AFX afxString const g_str_RasterizerDiscardEnable;
AFX afxString const g_str_FillMode;
AFX afxString const g_str_CullMode;
AFX afxString const g_str_FrontFace;
AFX afxString const g_str_DepthBias;
AFX afxString const g_str_LineWidth;

AFX afxString const g_str_DepthTestEnable;
AFX afxString const g_str_DepthWriteEnable;
AFX afxString const g_str_DepthCompareOp;
AFX afxString const g_str_DepthBoundsTest;
AFX afxString const g_str_StencilTest;
AFX afxString const g_str_Front;
AFX afxString const g_str_Back;
AFX afxString const g_str_fail;
AFX afxString const g_str_pass;
AFX afxString const g_str_depthFail;
AFX afxString const g_str_compare;
AFX afxString const g_str_min;
AFX afxString const g_str_max;

AFX afxString const g_str_true;
AFX afxString const g_str_false;

AFX afxString const g_str_enable;
AFX afxString const g_str_constant;
AFX afxString const g_str_slope;
AFX afxString const g_str_clamp;


AFX afxPrimTopology         AfxFindPrimitiveTopology(afxString const *str);
AFX afxLogicOp              AfxFindLogicOp(afxString const *str);
AFX afxStencilOp            AfxFindStencilOp(afxString const *str);
AFX afxCompareOp            AfxFindCompareOp(afxString const *str);
AFX afxCullMode             AfxFindCullMode(afxString const *str);
AFX afxFrontFace            AfxFindFrontFace(afxString const *str);
AFX afxFillMode             AfxFindFillMode(afxString const *str);
AFX afxShaderStage          AfxFindShaderStage(afxString const *str);
AFX afxVertexFormat         AfxFindVertexFormat(afxString const *str);

AFX afxString const*        AfxStringifyPrimitiveTopology(afxPrimTopology pt, afxString *out);
AFX afxString const*        AfxStringifyLogicOp(afxLogicOp f, afxString *out);
AFX afxString const*        AfxStringifyStencilOp(afxStencilOp f, afxString *out);
AFX afxString const*        AfxStringifyCompareOp(afxCompareOp f, afxString *out);
AFX afxString const*        AfxStringifyCullMode(afxCullMode cm, afxString *out);
AFX afxString const*        AfxStringifyFrontFace(afxFrontFace ff, afxString *out);
AFX afxString const*        AfxStringifyFillMode(afxFillMode fm, afxString *out);
AFX afxString const*        AfxStringifyShaderStage(afxShaderStage ss, afxString *out);
AFX afxString const*        AfxStringifyVertexFormat(afxVertexFormat fmt, afxString *out);

AFX afxNat AfxVertexFormatGetSize(afxVertexFormat fmt);

AFX afxResult       AfxResolveFormat(afxFormat fmt, afxFormat *subfmt, afxSize *subSiz, afxSize *vecSiz, afxSize *siz);

AFX afxError _AfxAssertWhd(afxWhd total_, afxWhd base_, afxWhd range_, afxHint const hint);
#define                     AfxAssertWhd(max_,base_,range_) _AfxAssertWhd((max_),(base_),(range_))

#endif//AFX_DRAW_DEFS_H