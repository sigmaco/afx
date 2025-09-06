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
 // DRAWING PIPELINE PROGRAMMABLE FUNCTION LIBRARY                           //
//////////////////////////////////////////////////////////////////////////////

// This software is part of Advanced Video Graphics Extensions & Experiments.

#ifndef AVX_SHADER_H
#define AVX_SHADER_H

#include "qwadro/draw/avxFormat.h"
#include "qwadro/io/afxXml.h"
#include "qwadro/base/afxFixedString.h"

typedef enum avxShaderParam
{
    avxShaderParam_IN = 0,
    avxShaderParam_OUT = 1,
    avxShaderParam_SAMPLER = 2, // sampler only
    avxShaderParam_TEXTURE = 3, // combined image-sampler
    avxShaderParam_RASTER = 4, // raster only
    avxShaderParam_IMAGE = 5,
    avxShaderParam_FETCH = 6,
    avxShaderParam_TSBO = 7,
    avxShaderParam_UNIFORM = 8, // UBO
    avxShaderParam_BUFFER = 9, // SSBO
    avxShaderParam_STORAGE = avxShaderParam_BUFFER,
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
    afxString16     name; // 16
    afxUnit         cnt;
};

AFX_DEFINE_STRUCT(avxShaderIoChannel)
{
    afxUnit         location; // is the shader input location number for this attribute or output location for this channel.
    avxFormat       fmt; // is the size and type of the vertex attribute data or color data.
    afxString16     semantic;
};

AFX_DEFINE_STRUCT(avxShaderConfig)
{
    afxUri          uri;
    afxString       code;
    afxUnit         verMajor;
    afxUnit         verMinor;
    afxBool         extended;
    afxString       tag;
};

AFX_DEFINE_STRUCT(avxShaderConfig2)
{
    afxFlags        flags;
    avxShaderType   stage;
    avxShaderType   nextStage;
    afxString       code;
    afxString       fn;
};

AVX afxError        AvxAcquireShaders
(
    afxDrawSystem dsys, 
    afxUnit cnt, 
    afxUri const uris[], 
    afxString const codes[], 
    avxShader shaders[]
);

////////////////////////////////////////////////////////////////////////////////

AVX afxDrawSystem   AvxGetShaderHost
(
    avxShader       shd
);

AVX avxShaderType   AvxGetShaderStage
(
    avxShader shd
);

AVX afxUnit         AvxQueryShaderIoChannels
(
    avxShader shd, 
    afxUnit first, 
    afxUnit cnt, 
    avxShaderIoChannel channels[]
);

AVX afxUnit         AvxQueryShaderInterfaces
(
    avxShader shd, 
    afxUnit first, 
    afxUnit cnt, 
    avxShaderResource rsrc[]
);

AVX afxBool         AvxShaderIsDummy
(
    avxShader shd
);

AVX afxError        AvxDumpShaderCode
(
    avxShader shd, 
    afxArray* arr
);

AVX afxError        AvxPrintShader
(
    avxShader shd, 
    afxUri const *uri
);

AVX afxError        AvxSerializeShader
(
    avxShader shd, 
    afxStream ios
);

#endif//AVX_SHADER_H
