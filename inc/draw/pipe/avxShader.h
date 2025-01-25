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

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#ifndef AVX_SHADER_H
#define AVX_SHADER_H

#include "qwadro/inc/draw/afxDrawDefs.h"
#include "qwadro/inc/io/afxXml.h"
#include "qwadro/inc/base/afxFixedString.h"

typedef enum avxShaderParam
{
    avxShaderParam_IN = 0,
    avxShaderParam_OUT = 1,
    avxShaderParam_SAMPLER = 2, // sampler only
    avxShaderParam_TEXTURE = 3, // combined image-sampler
    avxShaderParam_RASTER = 4, // raster only
    avxShaderParam_IMAGE = 5,
    avxShaderParam_TFBO = 6,
    avxShaderParam_TSBO = 7,
    avxShaderParam_UNIFORM = 8, // UBO
    avxShaderParam_BUFFER = 9, // SSBO
    avxShaderParam_INPUT_ATTACHMENT = 10,
    avxShaderParam_PUSH = 11, // push constant buffer object
    avxShaderParam_FEEDBACK = 12, // transform feedback buffer

    avxShaderParam_TOTAL
} avxShaderParam;

AFX_DEFINE_STRUCT(avxShaderResource)
{
    afxUnit         set;
    afxUnit         binding;
    avxShaderParam  type;
    afxUnit         cnt;
    afxString16     name; // 16
};

AFX_DEFINE_STRUCT(avxShaderIoChannel)
{
    afxString16     semantic;
    afxUnit         location; /// is the shader input location number for this attribute or output location for this channel.
    avxFormat       fmt; /// is the size and type of the vertex attribute data or color data.
};

AFX_DEFINE_STRUCT(avxShaderConfig)
{
    afxUri          uri;
    afxString       code;
    afxUnit         verMajor;
    afxUnit         verMinor;
    afxBool         extended;
};

AVX avxShaderStage  AfxGetShaderStage(avxShader shd);

AVX afxUnit         AfxCountShaderIoChannels(avxShader shd);
AVX afxUnit         AfxGetShaderIoChannels(avxShader shd, afxUnit first, afxUnit cnt, avxShaderIoChannel channels[]);

AVX afxUnit         AfxCountShaderInterfaces(avxShader shd);
AVX afxResult       AfxDescribeShaderInterfaces(avxShader shd, afxUnit first, afxUnit cnt, avxShaderResource rsrc[]);

AVX afxBool         AfxShaderIsDummy(avxShader shd);

AVX afxError        AfxDumpShaderCode(avxShader shd, afxArray* arr);

AVX afxError        AfxPrintShader(avxShader shd, afxUri const *uri);
AVX afxError        AfxSerializeShader(avxShader shd, afxStream ios);

////////////////////////////////////////////////////////////////////////////////

AVX afxError        AfxAcquireShaders(afxDrawSystem dsys, afxUnit cnt, afxUri const uris[], afxString const codes[], avxShader shaders[]);

#endif//AVX_SHADER_H
