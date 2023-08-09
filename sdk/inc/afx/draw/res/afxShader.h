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

#ifndef AFX_SHADER_H
#define AFX_SHADER_H

#include "afx/draw/afxDrawDefs.h"
#include "afx/core/io/afxStream.h"
#include "afx/core/io/afxUri.h"

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

AFX_DEFINE_STRUCT(afxShaderResource)
{
    afxNat8                 set;
    afxNat8                 binding;
    afxShaderResourceType   type;
    afxNat8                 cnt;
    afxString*              name; // 16
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
    afxString*              name; // 16
};

AFX_DEFINE_STRUCT(afxShaderBlueprintInOut)
{
    afxNat8                 location;
    afxVertexFormat         fmt;
    afxString*              semantic; // 8
};

AFX_DECLARE_STRUCT(_afxShdVmt);

AFX_OBJECT(afxShader)
{
    afxInstance                   obj;
    _afxShdVmt const*           vmt;
    void*                       idd;
#ifdef _AFX_SHADER_C
    afxUri*                     uri;
    afxShaderStage              stage;

    afxByte*                    code;
    afxNat16                    codeLen;
    afxString*                  entry;

    afxNat8                     resDeclCnt;
    afxShaderBlueprintResource* resDecls;

    afxNat8                     ioDeclCnt;
    afxShaderBlueprintInOut*    ioDecls;
    afxPrimTopology             topology;
#endif
};

AFX afxDrawContext          AfxGetShaderContext(afxShader shd);

AFX afxNat                  AfxGetShaderInterfaceCount(afxShader shd);
AFX afxResult               AfxDescribeShaderInterfaces(afxShader shd, afxNat first, afxNat cnt, afxShaderResource rsrc[]);

AFX afxBool                 AfxShaderIsDummy(afxShader shd);
AFX afxShaderStage          AfxShaderGetStage(afxShader shd);
AFX afxError                AfxShaderDownload(afxShader shd, afxUri const *uri);
AFX afxError                AfxShaderSerialize(afxShader shd, afxStream *ios);

#endif//AFX_SHADER_H