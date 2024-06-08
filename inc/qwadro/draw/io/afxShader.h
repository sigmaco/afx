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
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#ifndef AVX_SHADER_H
#define AVX_SHADER_H

#include "qwadro/draw/afxDrawDefs.h"
#include "qwadro/io/afxXml.h"
#include "qwadro/core/afxFixedString.h"

typedef enum afxShaderResourceType
{
    AFX_SHD_RES_TYPE_INPUT = 0,
    AFX_SHD_RES_TYPE_OUTPUT = 1,
    AFX_SHD_RES_TYPE_SAMPLER = 2,
    AFX_SHD_RES_TYPE_COMBINED_IMAGE_SAMPLER = 3, // when an image is bound without a sampler
    AFX_SHD_RES_TYPE_SAMPLED_IMAGE = 4,
    AFX_SHD_RES_TYPE_STORAGE_IMAGE = 5,
    AFX_SHD_RES_TYPE_UNIFORM_TEXEL_BUFFER = 6,
    AFX_SHD_RES_TYPE_STORAGE_TEXEL_BUFFER = 7,
    AFX_SHD_RES_TYPE_CONSTANT_BUFFER = 8,
    AFX_SHD_RES_TYPE_STORAGE_BUFFER = 9,
    AFX_SHD_RES_TYPE_INPUT_ATTACHMENT = 10,
    AFX_SHD_RES_TYPE_PUSH_CONSTANT_BUFFER = 11,

    AFX_SHD_RES_TYPE_TOTAL
} afxShaderResourceType;

AFX_DEFINE_STRUCT(afxShaderBlueprint)
{
    afxFcc                  fcc;
    afxDrawContext          dctx;
    afxMmu                  mmu;
    afxUri128               uri; // 128
    afxShaderStage          stage;

    afxString8              entry; // 8
    afxArray                codes; // afxByte    
    afxArray                inOuts; // afxShaderBlueprintInOut
    afxArray                resources; // afxShaderBlueprintResource
    afxPrimTopology         topology;

    // push constants?
    // specialization?
};

AFX_DEFINE_STRUCT(afxShaderResource)
{
    afxNat                  set;
    afxNat                  binding;
    afxShaderResourceType   type;
    afxNat                  cnt;
    afxString               name; // 16
};

AFX_DEFINE_STRUCT(afxShaderBlueprintResource)
{
    afxNat                  set;
    afxNat                  binding;
    afxShaderResourceType   type;
    afxNat                  cnt;
    afxRestring             name; // 16
};

AFX_DEFINE_STRUCT(afxShaderIoChannel)
{
    afxNat                  location; /// is the shader input location number for this attribute or output location for this channel.
    afxVertexFormat         fmt; /// is the size and type of the vertex attribute data or color data.
    afxNat                  stream;
    afxString               semantic;
};

#ifdef _AVX_DRAW_C
#ifdef _AVX_SHADER_C
#ifndef _AVX_SHADER_IMPL
AFX_OBJECT(afxShader)
#else
struct afxBaseShader
#endif
{
    afxUri                  uri;
    afxShaderStage          stage;

    afxNat                  verMajor;
    afxNat                  verMinor;
    afxBool                 extended;
    afxByte*                code;
    afxNat16                codeLen;
    afxString               entry;

    afxNat8                 resDeclCnt;
    afxShaderBlueprintResource* resDecls;

    afxNat8                 ioDeclCnt;
    afxShaderIoChannel*     ioDecls;
    afxPrimTopology         topology;

    afxStringBase           strb;
};
#endif//_AVX_SHADER_C
#endif//_AVX_DRAW_C

AVX afxShaderStage      AfxGetShaderStage(afxShader shd);

AVX afxBool             AfxGetShaderStringBase(afxShader shd, afxStringBase* base);

AVX afxNat              AfxCountShaderIoChannels(afxShader shd);
AVX afxNat              AfxGetShaderIoChannels(afxShader shd, afxNat first, afxNat cnt, afxShaderIoChannel channels[]);

AVX afxNat              AfxCountShaderInterfaces(afxShader shd);
AVX afxResult           AfxDescribeShaderInterfaces(afxShader shd, afxNat first, afxNat cnt, afxShaderResource rsrc[]);

AVX afxBool             AfxShaderIsDummy(afxShader shd);

AVX afxError            AfxDumpShaderCode(afxShader shd, afxArray* arr);

AVX afxError            AfxPrintShader(afxShader shd, afxUri const *uri);
AVX afxError            AfxSerializeShader(afxShader shd, afxStream ios);

////////////////////////////////////////////////////////////////////////////////

AFX_DEFINE_STRUCT(afxShaderConfig)
{
    afxShaderStage      stage;
    afxUri              uri;
    //afxString           code;
    //afxString           entry;
    afxNat              includeCnt;
    afxUri const*       includes;
    afxNat              verMajor;
    afxNat              verMinor;
    afxBool             extended;
};

AVX afxError            AfxCompileShaders(afxDrawContext dctx, afxNat cnt, afxShaderConfig const cfg[], afxShader shaders[]);

AVX afxError            AfxCompileShadersFromFile(afxDrawContext dctx, afxNat cnt, afxUri const uri[], afxShader shaders[]);

/// Builds a shader from a eXtensible Shader Language (.xsh) file, part of Qwadro.
AVX afxShader           AfxCompileShaderFromXml(afxDrawContext dctx, afxNat specIdx, afxXml const* xml, afxNat elemIdx);
AVX afxShader           AfxCompileShaderFromXsh(afxDrawContext dctx, afxUri const* uri);

AVX afxError            AfxCompileShadersFromXsh(afxDrawContext dctx, afxNat cnt, afxUri const uri[], afxShader shaders[]);

#endif//AVX_SHADER_H
