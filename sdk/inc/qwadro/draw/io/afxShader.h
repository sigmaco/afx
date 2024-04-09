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

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#ifndef AFX_SHADER_H
#define AFX_SHADER_H

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

AFX_DEFINE_STRUCT(afxShaderInOut)
{
    afxNat                  location;
    afxVertexFormat         fmt;
};

AFX_DEFINE_STRUCT(afxShaderBlueprintResource)
{
    afxNat                  set;
    afxNat                  binding;
    afxShaderResourceType   type;
    afxNat                  cnt;
    afxRestring       name; // 16
};

AFX_DEFINE_STRUCT(afxShaderBlueprintInOut)
{
    afxNat                  location;
    afxVertexFormat         fmt;
    afxNat                  stream;
    afxRestring       semantic; // 8
};

#ifdef _AFX_DRAW_C
#ifdef _AFX_SHADER_C
#ifndef _AFX_SHADER_IMPL
AFX_OBJECT(afxShader)
#else
struct afxBaseShader
#endif
{
    afxUri                      uri;
    afxShaderStage              stage;

    afxByte*                    code;
    afxNat16                    codeLen;
    afxRestring           entry;

    afxNat8                     resDeclCnt;
    afxShaderBlueprintResource* resDecls;

    afxNat8                     ioDeclCnt;
    afxShaderBlueprintInOut*    ioDecls;
    afxPrimTopology             topology;
};
#endif
#endif

AFX_DEFINE_STRUCT(afxShaderBuilder)
{
    void        (*GetInfo)(void* data, afxNat* codeLen, afxNat* ioCnt, afxNat* resCnt);
    void        (*GetCode)(void* data, afxByte* code);    
};

AVX afxNat                  AfxCountShaderInterfaces(afxShader shd);
AVX afxResult               AfxDescribeShaderInterfaces(afxShader shd, afxNat first, afxNat cnt, afxShaderResource rsrc[]);

AVX afxBool                 AfxShaderIsDummy(afxShader shd);
AVX afxShaderStage          AfxGetShaderStage(afxShader shd);
AVX afxError                AfxPrintShader(afxShader shd, afxUri const *uri);
AVX afxError                AfxSerializeShader(afxShader shd, afxStream ios);

////////////////////////////////////////////////////////////////////////////////

AVX afxError                AfxCompileShaders(afxDrawContext dctx, afxNat cnt, afxShaderBlueprint const blueprint[], afxShader shaders[]);

/// Builds a shader from a eXtensible Shader Language (.xsh) file, part of Qwadro.
AVX afxShader               AfxCompileShaderFromXml(afxDrawContext dctx, afxNat specIdx, afxXml const* xml, afxNat elemIdx);
AVX afxShader               AfxCompileShaderFromXsh(afxDrawContext dctx, afxUri const* uri);

AVX afxError                AfxCompileShadersFromXsh(afxDrawContext dctx, afxNat cnt, afxUri const uri[], afxShader shaders[]);

#endif//AFX_SHADER_H
