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
#include "afx/draw/pipelining/afxPipelineRig.h"

AFX_DEFINE_HANDLE(afxShader);

#ifndef AFX_DRAW_DRIVER_SRC

AFX_OBJECT(afxShader)
{
    afxObject               obj;
};

#endif

AFX_DEFINE_STRUCT(afxShaderSource) // source for blueprints
{
    afxFcc                  type; // NIL (raw data), URI or IOS
    union
    {
        struct
        {
            void const*     start;
            afxSize         range;
        }                   data;
        struct
        {
            afxStream       ios;
            afxSize         offset;
            afxNat          range;
        }                   stream;
        afxObject           *obj;
        afxUri              uri; // map subentry uri to don't use in-place URI storage.
    };
};

AFX_DEFINE_STRUCT(afxShaderResourceDecl)
{
    afxNat                  set;
    afxNat                  binding;
    afxFlags                visibility; // A bitset of the members of afxShaderStage.
    afxShaderResourceType   type;
    afxNat                  cnt;
    afxString16             name; // by QWADRO
};

AFX_DEFINE_STRUCT(afxShaderInDecl)
{
    afxNat                  location;
    afxVertexFormat         fmt;
};

AFX_DEFINE_STRUCT(afxShaderOutDecl)
{
    afxNat                  location;
    afxVertexFormat         fmt;
};

AFX_DEFINE_STRUCT(afxShaderBlueprint)
{
    afxFcc                  fcc;
    afxString32             name;
    afxShaderStage          stage;

    afxNat                  srcCnt;
    afxShaderSource         srcs[16];
    afxString8              entry;
    
    afxNat                  resDeclCnt;
    afxShaderResourceDecl   resDecls[8];

    afxNat                  inDeclCnt;
    afxShaderInDecl         inDecls[8];

    afxNat                  outDeclCnt;
    afxShaderOutDecl        outDecls[8];
    // push constants?
    // specialization?
};

AFX void*                   AfxShaderGetContext(afxShader shd);
AFX void*                   AfxShaderGetDriver(afxShader shd);
AFX void*                   AfxShaderGetDrawSystem(afxShader shd);

AFX afxShaderStage          AfxShaderGetStage(afxShader shd);
AFX afxError                AfxShaderDownload(afxShader shd, afxUri const *uri);
AFX afxError                AfxShaderSerialize(afxShader shd, afxStream ios);

////////////////////////////////////////////////////////////////////////////////
// SHADER BLUEPRINT                                                           //
////////////////////////////////////////////////////////////////////////////////

AFXINL afxError                 AfxShaderBlueprintReset(afxShaderBlueprint *blueprint, afxString const *name, afxShaderStage stage, afxString const *entry);
AFXINL afxError                 AfxShaderBlueprintReflectUrd(afxShaderBlueprint *blueprint, afxUrdNode const *node);

AFXINL afxError                 AfxShaderBlueprintRename(afxShaderBlueprint *blueprint, afxString const *name);
AFXINL afxError                 AfxShaderBlueprintChooseEntryPoint(afxShaderBlueprint *blueprint, afxString const *entry);
AFXINL afxError                 AfxShaderBlueprintSetStage(afxShaderBlueprint *blueprint, afxShaderStage stage);

AFXINL afxShaderSource*         AfxShaderBlueprintAddCode(afxShaderBlueprint *blueprint, void const *start, afxNat range);
AFXINL afxShaderSource*         AfxShaderBlueprintAddCodeFromStream(afxShaderBlueprint *blueprint, afxStream ios, afxSize offset, afxNat range);
AFXINL afxShaderSource*         AfxShaderBlueprintAddCodeFromResource(afxShaderBlueprint *blueprint, afxUri const *uri);

AFXINL afxShaderResourceDecl*   AfxShaderBlueprintDeclareResource(afxShaderBlueprint *blueprint, afxNat set, afxNat binding, afxShaderResourceType type, afxNat cnt, afxString const *name);
AFXINL afxShaderInDecl*         AfxShaderBlueprintDeclareIn(afxShaderBlueprint *blueprint, afxNat location, afxVertexFormat format);
AFXINL afxShaderOutDecl*        AfxShaderBlueprintDeclareOut(afxShaderBlueprint *blueprint, afxNat location, afxVertexFormat format);

#endif//AFX_SHADER_H