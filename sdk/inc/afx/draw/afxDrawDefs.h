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

#ifndef AFX_DRAW_DEFS_H
#define AFX_DRAW_DEFS_H

#include "afx/core/mem/afxArray.h"
#include "afx/math/volume/afxFrustum.h"
#include "afx/core/async/afxThread.h"
#include "afx/core/async/afxCondition.h"
#include "afx/core/io/afxStream.h"

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
{
    afxLogicOp_CLEAR = 0,
    afxLogicOp_AND = 1,
    afxLogicOp_AND_REVERSE = 2,
    afxLogicOp_COPY = 3,
    afxLogicOp_AND_INVERTED = 4,
    afxLogicOp_NO_OP = 5,
    afxLogicOp_XOR = 6,
    afxLogicOp_OR = 7,
    afxLogicOp_NOR = 8,
    afxLogicOp_EQUIVALENT = 9,
    afxLogicOp_INVERT = 10,
    afxLogicOp_OR_REVERSE = 11,
    afxLogicOp_COPY_INVERTED = 12,
    afxLogicOp_OR_INVERTED = 13,
    afxLogicOp_NAND = 14,
    afxLogicOp_SET = 15,

    afxLogicOp_TOTAL
} afxLogicOp;

typedef enum afxCullMode
{
    // NIL = DISABLED
    afxCullMode_FRONT = AFX_FLAG(0),
    afxCullMode_BACK  = AFX_FLAG(1),
    afxCullMode_BOTH  = (afxCullMode_FRONT | afxCullMode_BACK), // porque usar isto?
    // sum to cull both faces

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
{
    // NIL = DISABLED
    afxBlendFactor_ZERO = 1,
    afxBlendFactor_ONE,
    afxBlendFactor_SRC_RGB,
    afxBlendFactor_ONE_MINUS_SRC_RGB,
    afxBlendFactor_DST_RGB,
    afxBlendFactor_ONE_MINUS_DST_RGB,
    afxBlendFactor_SRC_A,
    afxBlendFactor_ONE_MINUS_SRC_A,
    afxBlendFactor_DST_A,
    afxBlendFactor_ONE_MINUS_DST_A,
    afxBlendFactor_CONSTANT_RGB,
    afxBlendFactor_ONE_MINUS_CONST_RGB,
    afxBlendFactor_CONST_A,
    afxBlendFactor_ONE_MINUS_CONST_A
} afxBlendFactor;

typedef enum
{
    afxBlendOp_ADD,
    afxBlendOp_SUBTRACT,
    afxBlendOp_REVERSE_SUBTRACT,
    afxBlendOp_MIN,
    afxBlendOp_MAX
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
    afxVertexFormat_REAL,
    afxVertexFormat_V2D,
    afxVertexFormat_V3D,
    afxVertexFormat_V4D,

    afxVertexFormat_V4B, // afxInt8[4]
    afxVertexFormat_V8B, // afxInt8[8]

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
AFX afxString const g_str_stage;
AFX afxString const g_str_name;
AFX afxString const g_str_entry;
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
AFX afxString const*        AfxStringifyLogicOp(afxLogicOp lo, afxString *out);
AFX afxString const*        AfxStringifyStencilOp(afxStencilOp so, afxString *out);
AFX afxString const*        AfxStringifyCompareOp(afxCompareOp co, afxString *out);
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