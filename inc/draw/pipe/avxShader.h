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

#include "qwadro/inc/draw/afxDrawDefs.h"
#include "qwadro/inc/io/afxXml.h"
#include "qwadro/inc/base/afxFixedString.h"

typedef enum avxShaderResType
{
    avxShaderResType_INPUT = 0,
    avxShaderResType_OUTPUT = 1,
    avxShaderResType_SAMPLER = 2,
    avxShaderResType_COMBINED_IMAGE_SAMPLER = 3, // when an image is bound without a sampler
    avxShaderResType_SAMPLED_IMAGE = 4,
    avxShaderResType_STORAGE_IMAGE = 5,
    avxShaderResType_UNIFORM_TEXEL_BUFFER = 6,
    avxShaderResType_STORAGE_TEXEL_BUFFER = 7,
    avxShaderResType_CONSTANT_BUFFER = 8,
    avxShaderResType_STORAGE_BUFFER = 9,
    avxShaderResType_INPUT_ATTACHMENT = 10,
    avxShaderResType_PUSH_CONSTANT_BUFFER = 11,

    avxShaderResType_TOTAL
} avxShaderResType;

AFX_DEFINE_STRUCT(avxShaderResource)
{
    afxNat              set;
    afxNat              binding;
    avxShaderResType    type;
    afxNat              cnt;
    afxString16         name; // 16
};

AFX_DEFINE_STRUCT(avxShaderIoChannel)
{
    afxString8          semantic;
    afxNat              location; /// is the shader input location number for this attribute or output location for this channel.
    afxVertexFormat     fmt; /// is the size and type of the vertex attribute data or color data.
};

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

AVX afxError            AfxCompileShaders(afxDrawContext dctx, afxNat cnt, avxShaderConfig const cfg[], avxShader shaders[]);

AVX afxError            AfxCompileShadersFromFile(afxDrawContext dctx, afxNat cnt, afxUri const uri[], avxShader shaders[]);

#endif//AVX_SHADER_H
