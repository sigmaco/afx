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
#include "qwadro/base/afxFixedString.h"

typedef enum avxShaderResourceType
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
} avxShaderResourceType;

AFX_DEFINE_STRUCT(avxShaderBlueprint)
{
    afxFcc                  fcc;
    afxDrawContext          dctx;
    afxMmu                  mmu;
    afxUri128               uri; // 128
    avxShaderStage          stage;

    afxString8              entry; // 8
    afxArray                codes; // afxByte    
    afxArray                inOuts; // avxShaderBlueprintInOut
    afxArray                resources; // avxShaderBlueprintResource
    avxTopology         topology;

    // push constants?
    // specialization?
};

AFX_DEFINE_STRUCT(avxShaderResource)
{
    afxNat                  set;
    afxNat                  binding;
    avxShaderResourceType   type;
    afxNat                  cnt;
    afxString               name; // 16
};

AFX_DEFINE_STRUCT(avxShaderBlueprintResource)
{
    afxNat                  set;
    afxNat                  binding;
    avxShaderResourceType   type;
    afxNat                  cnt;
    afxString16             name; // 16
};

AFX_DEFINE_STRUCT(avxShaderIoChannel)
{
    afxNat                  location; /// is the shader input location number for this attribute or output location for this channel.
    afxVertexFormat         fmt; /// is the size and type of the vertex attribute data or color data.
    afxNat                  stream;
    afxString               semantic;
};

AVX avxShaderStage      AfxGetShaderStage(avxShader shd);

AVX afxNat              AfxCountShaderIoChannels(avxShader shd);
AVX afxNat              AfxGetShaderIoChannels(avxShader shd, afxNat first, afxNat cnt, avxShaderIoChannel channels[]);

AVX afxNat              AfxCountShaderInterfaces(avxShader shd);
AVX afxResult           AfxDescribeShaderInterfaces(avxShader shd, afxNat first, afxNat cnt, avxShaderResource rsrc[]);

AVX afxBool             AfxShaderIsDummy(avxShader shd);

AVX afxError            AfxDumpShaderCode(avxShader shd, afxArray* arr);

AVX afxError            AfxPrintShader(avxShader shd, afxUri const *uri);
AVX afxError            AfxSerializeShader(avxShader shd, afxStream ios);

////////////////////////////////////////////////////////////////////////////////

AFX_DEFINE_STRUCT(avxShaderConfig)
{
    avxShaderStage      stage;
    afxUri              uri;
    //afxString           code;
    //afxString           entry;
    afxNat              includeCnt;
    afxUri const*       includes;
    afxNat              verMajor;
    afxNat              verMinor;
    afxBool             extended;
};

AVX afxError            AfxCompileShaders(afxDrawContext dctx, afxNat cnt, avxShaderConfig const cfg[], avxShader shaders[]);

AVX afxError            AfxCompileShadersFromFile(afxDrawContext dctx, afxNat cnt, afxUri const uri[], avxShader shaders[]);

/// Builds a shader from a eXtensible Shader Language (.xsh) file, part of Qwadro.
AVX avxShader           AfxCompileShaderFromXml(afxDrawContext dctx, afxNat specIdx, afxXml const* xml, afxNat elemIdx);
AVX avxShader           AfxCompileShaderFromXsh(afxDrawContext dctx, afxUri const* uri);

AVX afxError            AfxCompileShadersFromXsh(afxDrawContext dctx, afxNat cnt, afxUri const uri[], avxShader shaders[]);

#endif//AVX_SHADER_H
