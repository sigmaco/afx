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

#ifndef AFX_SHADER_H
#define AFX_SHADER_H

#include "afx/draw/afxDrawDefs.h"
#include "afx/core/io/afxStream.h"
#include "afx/core/io/afxUri.h"

AFX_DEFINE_HANDLE(afxShader);

#ifndef AFX_DRAW_DRIVER_SRC

AFX_OBJECT(afxShader)
{
    afxObject               obj;
};

#endif

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

AFX_DEFINE_STRUCT(afxShaderBlueprint)
{
    afxFcc                  fcc;
    afxDrawContext          dctx;
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

AFX void*                   AfxShaderGetContext(afxShader shd);
AFX void*                   AfxShaderGetDriver(afxShader shd);
AFX void*                   AfxShaderGetDrawSystem(afxShader shd);

AFX afxNat                  AfxShaderGetInterfaceCount(afxShader shd);
AFX afxResult               AfxShaderDescribeInterfaces(afxShader shd, afxNat first, afxNat cnt, afxShaderResource rsrc[]);

AFX afxBool                 AfxShaderIsDummy(afxShader shd);
AFX afxShaderStage          AfxShaderGetStage(afxShader shd);
AFX afxError                AfxShaderDownload(afxShader shd, afxUri const *uri);
AFX afxError                AfxShaderSerialize(afxShader shd, afxStream ios);

////////////////////////////////////////////////////////////////////////////////
// SHADER BLUEPRINT                                                           //
////////////////////////////////////////////////////////////////////////////////

AFX void                AfxShaderBlueprintBegin(afxShaderBlueprint* blueprint, afxDrawContext dctx, afxShaderStage stage, afxUri const *uri, afxString const *entry, afxNat estCodeLen, afxNat estIoCnt, afxNat estResCnt);
AFX afxError            AfxShaderBlueprintEnd(afxShaderBlueprint *blueprint, afxNat cnt, afxShader shd[]);
AFX void                AfxShaderBlueprintErase(afxShaderBlueprint *blueprint);

AFX void                AfxShaderBlueprintRename(afxShaderBlueprint *blueprint, afxUri const *uri);
AFX void                AfxShaderBlueprintChooseEntryPoint(afxShaderBlueprint *blueprint, afxString const *entry);
AFX void                AfxShaderBlueprintSetStage(afxShaderBlueprint *blueprint, afxShaderStage stage);

AFX afxError            AfxShaderBlueprintAddCode(afxShaderBlueprint *blueprint, void const *start, afxNat range);
AFX afxError            AfxShaderBlueprintAddCodeFromStream(afxShaderBlueprint *blueprint, afxStream ios, afxSize offset, afxNat range);
AFX afxError            AfxShaderBlueprintAddCodeFromResource(afxShaderBlueprint *blueprint, afxUri const *uri);

AFX afxError            AfxShaderBlueprintDeclareResource(afxShaderBlueprint *blueprint, afxNat set, afxNat binding, afxShaderResourceType type, afxNat cnt, afxString const *name);
AFX afxError            AfxShaderBlueprintDeclareInOut(afxShaderBlueprint *blueprint, afxNat location, afxVertexFormat fmt, afxString const *semantic);

#endif//AFX_SHADER_H