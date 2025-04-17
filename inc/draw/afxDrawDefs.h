/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *        Q W A D R O   V I D E O   G R A P H I C S   I N F R A S T R U C T U R E
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

  //////////////////////////////////////////////////////////////////////////////
 // SIGMA Unified High Fidelity Video Graphics Infrastructure                //
//////////////////////////////////////////////////////////////////////////////

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

/*
    The Qwadro draw I/O system refers to the overall framework or set of components involved in creating, managing, and rendering graphical content. 
    This can include both hardware and software elements that work together to produce and display visual data.

    In the draw system, graphics hardware refers to the physical components that handle the rendering of images and video.

    Graphics Processing Unit (GPU) is a specialized chip designed to accelerate rendering tasks by processing complex graphical computations.
    Graphics Card is a hardware component that includes the GPU, memory, and video output interfaces to drive displays and manage graphical output.

    Software components of the draw system include the tools and libraries used to create and manage graphical content.

    APIs and libraries like OpenGL, DirectX, Vulkan, or Metal that provide the functions and tools for rendering graphics.

    The rendering pipeline is a series of stages and processes that graphical data goes through to be rendered onto a screen.

    The integration and management aspect of the draw system involves coordinating between different components and ensuring efficient rendering and display.
    Driver Software manages communication between the system and graphics hardware.

    Draw I/O mechanisms are software systems that integrate various components to render complex scenes.
*/

#ifndef AVX_DRAW_DEFS_H
#define AVX_DRAW_DEFS_H

#include "qwadro/inc/exec/afxSystem.h"

#ifndef __e2targa__
#   ifdef _DEBUG
#       define AVX DLLIMPORT extern 
#       define AVXINL DLLIMPORT EMBED
#   else
#       define AVX DLLIMPORT extern 
#       define AVXINL DLLIMPORT EMBED
#   endif
#else
#   ifdef _DEBUG
#       define _AVX DLLEXPORT
#       define AVX DLLEXPORT extern 
#       define _AVXINL DLLEXPORT INLINE
#       define AVXINL DLLEXPORT EMBED
#   else
#       define _AVX DLLEXPORT
#       define AVX DLLEXPORT extern 
#       define _AVXINL DLLEXPORT INLINE
#       define AVXINL DLLEXPORT EMBED
#   endif
#endif//__e2draw__

typedef enum avxTopology
{
    avxTopology_TRI_LIST, // Mesh is made from triangles.
    avxTopology_TRI_STRIP,
    avxTopology_TRI_FAN,

    avxTopology_LINE_LIST, // Mesh is made from lines.
    avxTopology_LINE_STRIP, // Mesh is a line strip.

    avxTopology_POINT_LIST, // Mesh is made from points.

    avxTopology_LINE_LIST_ADJACENT,
    avxTopology_LINE_STRIP_ADJACENT,

    avxTopology_TRI_LIST_ADJACENT,
    avxTopology_TRI_STRIP_ADJACENT,

    avxTopology_PATCH_LIST,

    avxTopology_TOTAL
} avxTopology;

typedef enum avxStencilOp
{
    avxStencilOp_KEEP,
    avxStencilOp_ZERO,
    avxStencilOp_REPLACE,
    avxStencilOp_INC_N_CLAMP,
    avxStencilOp_DEC_N_CLAMP,
    avxStencilOp_INV,
    avxStencilOp_INC_AND_WRAP,
    avxStencilOp_DEC_AND_WRAP,

    avxStencilOp_TOTAL
} avxStencilOp;

typedef enum avxLogicOp
/// In the explanation of the logical operations, s represents the incoming color and d represents the color in the frame buffer.
/// Standard C-language operators are used.
/// As these bitwise operators suggest, the logical operation is applied independently to each bit pair of the source and destination colors.
{
    avxLogicOp_COPY,        // s
    avxLogicOp_COPY_INV,    // ~s
    avxLogicOp_INV,         // ~d
    avxLogicOp_AND,         // s & d
    avxLogicOp_NAND,        // ~(s & d)
    avxLogicOp_OR,          // s | d
    avxLogicOp_NOR,         // ~(s | d)
    avxLogicOp_XOR,         // s ^ d
    avxLogicOp_EQUIV,       // ~(s ^ d)

    avxLogicOp_AND_REV,     // s & ~d
    avxLogicOp_AND_INV,     // ~s & d
    avxLogicOp_OR_REV,      // s | ~d
    avxLogicOp_OR_INV,      // ~s | d

    avxLogicOp_NOP,         // d
    avxLogicOp_CLEAR,       // 0
    avxLogicOp_SET,         // 1

    avxLogicOp_TOTAL
} avxLogicOp;

typedef enum avxFaceMask
{
    avxFaceMask_NONE,
    avxFaceMask_FRONT   = AFX_BIT(0),
    avxFaceMask_BACK    = AFX_BIT(1),
    avxFaceMask_BOTH    = (avxFaceMask_FRONT | avxFaceMask_BACK),
} avxFaceMask;

typedef enum avxCullMode
/// Bitmask controlling triangle culling.
/// Once the orientation of triangles is determined, they are culled according to the cull mode.
/// Following culling, fragments are produced for any triangles which have not been discarded.
{
    avxCullMode_NONE, // specifies that no triangles are discarded.
    avxCullMode_FRONT   = AFX_BIT(0), // specifies that front-facing triangles are discarded.
    avxCullMode_BACK    = AFX_BIT(1), // specifies that back-facing triangles are discarded.
    avxCullMode_BOTH    = avxCullMode_FRONT | avxCullMode_BACK, // specifies that all triangles are discarded.
    
    avxCullMode_TOTAL
} avxCullMode;

typedef enum avxFrontFace
{
    avxFrontFace_CCW,
    avxFrontFace_CW,

    avxFrontFace_TOTAL
} avxFrontFace;

typedef enum avxFillMode
{
    avxFillMode_FACE,
    avxFillMode_LINE,
    avxFillMode_POINT,

    avxFillMode_TOTAL
} avxFillMode;

typedef enum avxBlendFactor
/// Fragment output blending factors.
/// Rs0,Gs0,Bs0 and As0 represent the first source color R, G, B, and A components, respectively, for the fragment output location corresponding to the color attachment being blended.
/// Rs1,Gs1,Bs1 and As1 represent the second source color R, G, B, and A components, respectively, used in dual source blending modes, for the fragment output location corresponding to the color attachment being blended.
/// Rd,Gd,Bd and Ad represent the R, G, B, and A components of the destination color. That is, the color currently in the corresponding color attachment for this fragment/sample.
/// Rc,Gc,Bc and Ac represent the blend constant R, G, B, and A components, respectively.
{
    avxBlendFactor_ZERO,
    // (0,0,0)
    // 0

    avxBlendFactor_ONE,
    // (1,1,1)
    // 1

    avxBlendFactor_SRC_RGB,
    // (Rs0,Gs0,Bs0)
    // As0

    avxBlendFactor_ONE_MINUS_SRC_RGB,
    // (1-Rs0,1-Gs0,1-Bs0)
    // 1-As0

    avxBlendFactor_DST_RGB,
    // (Rd,Gd,Bd)
    // Ad

    avxBlendFactor_ONE_MINUS_DST_RGB,
    // (1-Rd,1-Gd,1-Bd)
    // 1-Ad

    avxBlendFactor_SRC_A,
    // (As0,As0,As0)
    // As0

    avxBlendFactor_ONE_MINUS_SRC_A,
    // (1-As0,1-As0,1-As0)
    // 1-As0

    avxBlendFactor_DST_A,
    // (Ad,Ad,Ad)
    // Ad

    avxBlendFactor_ONE_MINUS_DST_A,
    // (1-Ad,1-Ad,1-Ad)
    // 1-Ad

    avxBlendFactor_CONST_RGB,
    // (Rc,Gc,Bc)
    // Ac

    avxBlendFactor_ONE_MINUS_CONST_RGB,
    // (1-Rc,1-Gc,1-Bc)
    // 1-Ac

    avxBlendFactor_CONST_A,
    // (Ac,Ac,Ac)
    // Ac

    avxBlendFactor_ONE_MINUS_CONST_A,
    // (1-Ac,1-Ac,1-Ac)
    // 1-Ac

    avxBlendFactor_SRC_A_SATURATE,
    // (f,f,f); f = min(As0,1-Ad)
    // 1

    avxBlendFactor_SRC1_RGB,
    // (Rs1,Gs1,Bs1)
    // As1

    avxBlendFactor_ONE_MINUS_SRC1_RGB,
    // (1-Rs1,1-Gs1,1-Bs1)
    // 1-As1

    avxBlendFactor_SRC1_A,
    // (As1,As1,As1)
    // As1

    avxBlendFactor_ONE_MINUS_SRC1_A,
    // (1-As1,1-As1,1-As1)
    // 1-As1

    avxBlendFactor_TOTAL
} avxBlendFactor;

typedef enum avxBlendOp
/// Fragment output blending operations.
/// Once the source and destination blend factors have been selected, they along with the source and destination components are passed to the blending operations.
/// RGB and alpha components can use different operations.
{
    avxBlendOp_ADD,
    // R = Rs0 × Sr + Rd × Dr
    // G = Gs0 × Sg + Gd × Dg
    // B = Bs0 × Sb + Bd × Db
    // A = As0 × Sa + Ad × Da

    avxBlendOp_SUB,
    // R = Rs0 × Sr - Rd × Dr
    // G = Gs0 × Sg - Gd × Dg
    // B = Bs0 × Sb - Bd × Db
    // A = As0 × Sa - Ad × Da

    avxBlendOp_REV_SUB,
    // R = Rd × Dr - Rs0 × Sr
    // G = Gd × Dg - Gs0 × Sg
    // B = Bd × Db - Bs0 × Sb
    // A = Ad × Da - As0 × Sa

    avxBlendOp_MIN,
    // R = min(Rs0, Rd)
    // G = min(Gs0, Gd)
    // B = min(Bs0, Bd)
    // A = min(As0, Ad)

    avxBlendOp_MAX,
    // R = max(Rs0, Rd)
    // G = max(Gs0, Gd)
    // B = max(Bs0, Bd)
    // A = max(As0, Ad)

    avxBlendOp_TOTAL
} avxBlendOp;

typedef enum avxCompareOp
{
    avxCompareOp_ALWAYS, // Always passes.
    avxCompareOp_GREATER, // Passes if the incoming depth value is greater than the stored depth value.
    avxCompareOp_GEQUAL, // Passes if the incoming depth value is greater than or equal to the stored depth value.
    avxCompareOp_EQUAL, // Passes if the incoming depth value is equal to the stored depth value.
    avxCompareOp_NOTEQUAL, // Passes if the incoming depth value is not equal to the stored depth value.
    avxCompareOp_LEQUAL, // Passes if the incoming depth value is less than or equal to the stored depth value.
    avxCompareOp_LESS, // Passes if the incoming depth value is less than the stored depth value.
    avxCompareOp_NEVER, // NIL = Never passes

    avxCompareOp_TOTAL
} avxCompareOp;

typedef enum avxShaderType
{
    // NIL = reserved for composing purposes.
    avxShaderType_VERTEX = 1,
    avxShaderType_FRAGMENT,
    avxShaderType_PRIMITIVE,
    avxShaderType_TESS_CTRL,
    avxShaderType_TESS_EVAL,
    avxShaderType_COMPUTE,

    avxShaderType_TOTAL
} avxShaderType;

// Object handles defined by SIGMA Draw System

AFX_DEFINE_HANDLE(afxDrawPlatform);
AFX_DEFINE_HANDLE(afxDrawDevice);
AFX_DEFINE_HANDLE(afxDrawSystem);
AFX_DEFINE_HANDLE(afxDrawThread);
AFX_DEFINE_HANDLE(afxDrawBridge);
AFX_DEFINE_HANDLE(afxDisplay);
AFX_DEFINE_HANDLE(afxDrawQueue);
AFX_DEFINE_HANDLE(afxDrawInput);
AFX_DEFINE_HANDLE(afxDrawContext);
AFX_DEFINE_HANDLE(afxDrawOutput);
AFX_DEFINE_HANDLE(avxPipeline);
AFX_DEFINE_HANDLE(avxRasterizer);
AFX_DEFINE_HANDLE(avxVertexInput);
AFX_DEFINE_HANDLE(avxShader);
AFX_DEFINE_HANDLE(avxLigature);
AFX_DEFINE_HANDLE(avxSampler);
AFX_DEFINE_HANDLE(avxQueryPool);
AFX_DEFINE_HANDLE(avxBuffer);
AFX_DEFINE_HANDLE(avxRaster);
AFX_DEFINE_HANDLE(avxCanvas);

AFX_DEFINE_HANDLE(afxDrawOperation);
AFX_DEFINE_HANDLE(afxDrawTechnique);
AFX_DEFINE_HANDLE(afxTypography);
AFX_DEFINE_HANDLE(avxCamera);
AFX_DEFINE_HANDLE(avxTxd);
AFX_DEFINE_HANDLE(afxVertexStream);
AFX_DEFINE_HANDLE(afxVertexBuffer);
AFX_DEFINE_HANDLE(afxIndexBuffer);

AFX_DEFINE_HANDLE(afxGeometry);
AFX_DEFINE_HANDLE(afxMesh);
AFX_DEFINE_HANDLE(afxMeshTopology);
AFX_DEFINE_HANDLE(afxScene);

AFX_DEFINE_STRUCT(avxViewConstants)
{
    afxM4d  v, iv;
    afxM4d  p, ip;
    afxM4d  pv, ipv;
    afxV4d  viewPos; // view point
    afxV2d  viewExtent;
};

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


AVX avxTopology             AfxFindPrimitiveTopology(afxString const *str);
AVX avxLogicOp              AfxFindLogicOp(afxString const *str);
AVX avxStencilOp            AfxFindStencilOp(afxString const *str);
AVX avxCompareOp            AfxFindCompareOp(afxString const *str);
AVX avxCullMode             AfxFindCullMode(afxString const *str);
AVX avxFrontFace            AfxFindFrontFace(afxString const *str);
AVX avxFillMode             AfxFindFillMode(afxString const *str);
AVX avxShaderType           AfxFindShaderStage(afxString const *str);
#if 0
AVX avxFormat               AfxFindVertexFormat(afxString const *str);
#endif

AVX afxString const*        AfxStringifyPrimitiveTopology(avxTopology pt, afxString *out);
AVX afxString const*        AfxStringifyLogicOp(avxLogicOp f, afxString *out);
AVX afxString const*        AfxStringifyStencilOp(avxStencilOp f, afxString *out);
AVX afxString const*        AfxStringifyCompareOp(avxCompareOp f, afxString *out);
AVX afxString const*        AfxStringifyCullMode(avxCullMode cm, afxString *out);
AVX afxString const*        AfxStringifyFrontFace(avxFrontFace ff, afxString *out);
AVX afxString const*        AfxStringifyFillMode(avxFillMode fm, afxString *out);
AVX afxString const*        AfxStringifyShaderStage(avxShaderType ss, afxString *out);
#if 0
AVX afxString const*        AfxStringifyVertexFormat(afxVertexFormat fmt, afxString *out);

AVX afxUnit AfxVertexFormatGetSize(afxVertexFormat fmt);

AVX afxResult       AfxResolveFormat(afxFormat fmt, afxFormat *subfmt, afxSize *subSiz, afxSize *vecSiz, afxSize *siz);
#endif

#endif//AVX_DRAW_DEFS_H
