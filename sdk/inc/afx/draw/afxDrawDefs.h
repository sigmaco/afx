/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                      S I G M A   T E C H N O L O G Y   G R O U P
 *
 *                                   Public Test Build
 *                      (c) 2017 SIGMA Co. & SIGMA Technology Group
 *                                    www.sigmaco.org
 */

#ifndef AFX_DRAW_DEFS_H
#define AFX_DRAW_DEFS_H

#include "afx/core/mem/afxArray.h"
#include "afx/math/volume/afxFrustum.h"
#include "afx/core/async/afxThread.h"
#include "afx/core/async/afxCondition.h"
#include "afx/core/io/afxStream.h"

AFX_DEFINE_HANDLE(afxDrawSystem); // exception

typedef afxNat afxSimd(afxWhd[3]);
typedef afxReal afxSimd(afxNdc[3]);

AFX_DEFINE_STRUCT(afxFileDrop)
{
    afxNat                      x, y;
    afxArray(afxChar const*)    files;
};

typedef afxNat32 afxVertexIndex;
typedef afxVertexIndex afxPoint[1];
typedef afxVertexIndex afxLine[2];
typedef afxVertexIndex afxTriangle[3];

#define AFX_INVALID_VTX_IDX (afxVertexIndex)0xffffffff

typedef enum
{
    AFX_PRIM_TOPOLOGY_POINT_LIST, // Mesh is made from points.

    AFX_PRIM_TOPOLOGY_LINE_LIST, // Mesh is made from lines.
    AFX_PRIM_TOPOLOGY_LINE_STRIP, // Mesh is a line strip.

    AFX_PRIM_TOPOLOGY_TRI_LIST, // Mesh is made from triangles.
    AFX_PRIM_TOPOLOGY_TRI_STRIP,
    AFX_PRIM_TOPOLOGY_TRI_FAN,

    AFX_PRIM_TOPOLOGY_LINE_LIST_ADJACENT,
    AFX_PRIM_TOPOLOGY_LINE_STRIP_ADJACENT,

    AFX_PRIM_TOPOLOGY_TRI_LIST_ADJACENT,
    AFX_PRIM_TOPOLOGY_TRI_STRIP_ADJACENT,

    AFX_PRIM_TOPOLOGY_PATCH_LIST,

    AFX_PRIM_TOPOLOGY_TOTAL
} afxPrimTopology;

typedef enum
{
    AFX_FMT_REAL,
    AFX_FMT_NAT,
    AFX_FMT_INT,
    AFX_FMT_V2D,
    AFX_FMT_V2DN,
    AFX_FMT_V2DI,
    AFX_FMT_V3D,
    AFX_FMT_V3DN,
    AFX_FMT_V3DI,
    AFX_FMT_V4D,
    AFX_FMT_V4DN,
    AFX_FMT_V4DI,

    AFX_FMT_TOTAL
} afxFormat;


typedef enum afxStencilOp
{
    AFX_STENCIL_OP_KEEP = 0,
    AFX_STENCIL_OP_ZERO = 1,
    AFX_STENCIL_OP_REPLACE = 2,
    AFX_STENCIL_OP_INC_AND_CLAMP = 3,
    AFX_STENCIL_OP_DEC_AND_CLAMP = 4,
    AFX_STENCIL_OP_INVERT = 5,
    AFX_STENCIL_OP_INC_AND_WRAP = 6,
    AFX_STENCIL_OP_DEC_AND_WRAP = 7,

    AFX_STENCIL_OP_TOTAL
} afxStencilOp;

typedef enum afxLogicOp
{
    AFX_LOGIC_OP_CLEAR = 0,
    AFX_LOGIC_OP_AND = 1,
    AFX_LOGIC_OP_AND_REVERSE = 2,
    AFX_LOGIC_OP_COPY = 3,
    AFX_LOGIC_OP_AND_INVERTED = 4,
    AFX_LOGIC_OP_NO_OP = 5,
    AFX_LOGIC_OP_XOR = 6,
    AFX_LOGIC_OP_OR = 7,
    AFX_LOGIC_OP_NOR = 8,
    AFX_LOGIC_OP_EQUIVALENT = 9,
    AFX_LOGIC_OP_INVERT = 10,
    AFX_LOGIC_OP_OR_REVERSE = 11,
    AFX_LOGIC_OP_COPY_INVERTED = 12,
    AFX_LOGIC_OP_OR_INVERTED = 13,
    AFX_LOGIC_OP_NAND = 14,
    AFX_LOGIC_OP_SET = 15,

    AFX_LOGIC_OP_TOTAL
} afxLogicOp;

typedef enum afxCullMode
{
    // NIL = DISABLED
    AFX_CULL_MODE_FRONT = AFX_FLAG(0),
    AFX_CULL_MODE_BACK  = AFX_FLAG(1),
    AFX_CULL_MODE_BOTH  = (AFX_CULL_MODE_FRONT | AFX_CULL_MODE_BACK), // porque usar isto?
    // sum to cull both faces

    AFX_CULL_MODE_TOTAL
} afxCullMode;

typedef enum
{
    AFX_FRONT_FACE_CCW,
    AFX_FRONT_FACE_CW,

    AFX_FRONT_FACE_TOTAL
} afxFrontFace;

typedef enum
{
    AFX_FILL_MODE_SOLID,
    AFX_FILL_MODE_EDGE,
    AFX_FILL_MODE_POINT,

    AFX_FILL_MODE_TOTAL
} afxFillMode;

typedef enum
{
    // NIL = DISABLED
    AFX_BLEND_FACTOR_ZERO = 1,
    AFX_BLEND_FACTOR_ONE,
    AFX_BLEND_FACTOR_SRC_RGB,
    AFX_BLEND_FACTOR_ONE_MINUS_SRC_RGB,
    AFX_BLEND_FACTOR_DST_RGB,
    AFX_BLEND_FACTOR_ONE_MINUS_DST_RGB,
    AFX_BLEND_FACTOR_SRC_A,
    AFX_BLEND_FACTOR_ONE_MINUS_SRC_A,
    AFX_BLEND_FACTOR_DST_A,
    AFX_BLEND_FACTOR_ONE_MINUS_DST_A,
    AFX_BLEND_FACTOR_CONSTANT_RGB,
    AFX_BLEND_FACTOR_ONE_MINUS_CONST_RGB,
    AFX_BLEND_FACTOR_CONST_A,
    AFX_BLEND_FACTOR_ONE_MINUS_CONST_A
} afxBlendFactor;

typedef enum
{
    AFX_BLEND_OP_ADD,
    AFX_BLEND_OP_SUBTRACT,
    AFX_BLEND_OP_REVERSE_SUBTRACT,
    AFX_BLEND_OP_MIN,
    AFX_BLEND_OP_MAX
} afxBlendOp;

typedef enum
{
    AFX_COMPARE_OP_NEVER, // NIL = Never passes
    AFX_COMPARE_OP_LESS, // Passes if the incoming depth value is less than the stored depth value.
    AFX_COMPARE_OP_LEQUAL, // Passes if the incoming depth value is less than or equal to the stored depth value.
    AFX_COMPARE_OP_EQUAL, // Passes if the incoming depth value is equal to the stored depth value.
    AFX_COMPARE_OP_NOTEQUAL, // Passes if the incoming depth value is not equal to the stored depth value.
    AFX_COMPARE_OP_GEQUAL, // Passes if the incoming depth value is greater than or equal to the stored depth value.
    AFX_COMPARE_OP_GREATER, // Passes if the incoming depth value is greater than the stored depth value.
    AFX_COMPARE_OP_ALWAYS, // Always passes.

    AFX_COMPARE_OP_TOTAL
} afxCompareOp;

typedef enum afxShaderStage
{
    AFX_SHADER_STAGE_COMPUTE    = AFX_FLAG(0),
    AFX_SHADER_STAGE_VERTEX     = AFX_FLAG(1),
    AFX_SHADER_STAGE_TESS_CTRL  = AFX_FLAG(2),
    AFX_SHADER_STAGE_TESS_EVAL  = AFX_FLAG(3),
    AFX_SHADER_STAGE_GEOMETRY   = AFX_FLAG(4),
    AFX_SHADER_STAGE_PIXEL      = AFX_FLAG(5),

    AFX_SHADER_STAGE_TOTAL      = 6
} afxShaderStage;

typedef enum afxVertexFormat
{
    AFX_VTX_FMT_X32,
    AFX_VTX_FMT_XY32,
    AFX_VTX_FMT_XYZ32,
    AFX_VTX_FMT_XYZW32,
    AFX_VTX_FMT_XYZW32Q, // quaternion

    AFX_VTX_FMT_TOTAL
} afxVertexFormat;

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

#endif//AFX_DRAW_DEFS_H