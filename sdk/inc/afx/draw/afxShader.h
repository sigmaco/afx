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

// This section is part of SIGMA GL/2.

#ifndef AFX_SHADER_H
#define AFX_SHADER_H

#include "afx/draw/afxDrawDefs.h"
#include "afx/core/afxStream.h"
#include "afx/core/afxUri.h"

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
    afxContext              ctx;
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
    afxNat8                 set;
    afxNat8                 binding;
    afxShaderResourceType   type;
    afxNat8                 cnt;
    afxString               name; // 16
};

AFX_DEFINE_STRUCT(afxShaderInOut)
{
    afxNat8                 location;
    afxVertexFormat         fmt;
};

AFX_DEFINE_STRUCT(afxShaderBlueprintResource)
{
    afxNat8                 set;
    afxNat8                 binding;
    afxShaderResourceType   type;
    afxNat8                 cnt;
    afxString               name; // 16
};

AFX_DEFINE_STRUCT(afxShaderBlueprintInOut)
{
    afxNat8                 location;
    afxVertexFormat         fmt;
    afxString               semantic; // 8
};

#ifdef _AFX_SHADER_C
AFX_OBJECT(afxShader)
#else
struct afxBaseShader
#endif
{
    afxUri                      uri;
    afxShaderStage              stage;

    afxByte*                    code;
    afxNat16                    codeLen;
    afxString                   entry;

    afxNat8                     resDeclCnt;
    afxShaderBlueprintResource* resDecls;

    afxNat8                     ioDeclCnt;
    afxShaderBlueprintInOut*    ioDecls;
    afxPrimTopology             topology;
};

AFX_DEFINE_STRUCT(afxShaderBuilder)
{
    void        (*GetInfo)(void* data, afxNat* codeLen, afxNat* ioCnt, afxNat* resCnt);
    void        (*GetCode)(void* data, afxByte* code);    
};

AFX afxError                AfxBuildShaders(afxDrawContext dctx, afxNat cnt, afxShader shd[], afxShaderBlueprint const blueprint[]);

/// Builds a shader from a eXtensible Shader Language (.xsh) file, part of Qwadro.
AFX afxShader               AfxCompileShaderFromXml(afxDrawContext dctx, afxXmlNode const* xmlNode);
AFX afxShader               AfxCompileShaderFromXsh(afxDrawContext dctx, afxUri const* uri);

AFX afxNat                  AfxCountShaderInterfaces(afxShader shd);
AFX afxResult               AfxDescribeShaderInterfaces(afxShader shd, afxNat first, afxNat cnt, afxShaderResource rsrc[]);

AFX afxBool                 AfxShaderIsDummy(afxShader shd);
AFX afxShaderStage          AfxGetShaderStage(afxShader shd);
AFX afxError                AfxPrintShader(afxShader shd, afxUri const *uri);
AFX afxError                AfxSerializeShader(afxShader shd, afxStream *ios);

#endif//AFX_SHADER_H