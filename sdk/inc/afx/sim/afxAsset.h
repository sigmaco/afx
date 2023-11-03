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

#ifndef AFX_ASSET_H
#define AFX_ASSET_H

#include "afx/core/afxClass.h"
#include "afx/sim/afxEntity.h"
#include "afx/sim/rendering/afxLight.h"
#include "afx/sim/rendering/afxCamera.h"
#include "afx/sim/afxSkeleton.h"
#include "afx/sim/modeling/afxModel.h"
#include "afx/sim/afxMaterial.h"
#include "afx/draw/afxDrawInput.h"
#include "afx/draw/afxDrawContext.h"
#include "afx/sim/anim/afxAnimator.h"
#include "afx/sim/rendering/afxRenderer.h"

AFX_DEFINE_STRUCT(afxCadToolInfo)
{
    afxString const*    name; // from art tool name
    afxNat              majorRev;
    afxNat              minorRev;
    afxM3d              xyzAxes; // right/x, up/y and back/z axes of art tool.    
    afxV3d              origin;
    afxReal             unitsPerMeter;
    void*               udd;
};

AFX_DEFINE_STRUCT(afxCadExporterInfo)
{
    afxString const*    name;
    afxNat              majorRev;
    afxNat              minorRev;
    afxNat              customization;
    afxNat              buildNum;
    void*               udd;
};

AFX_DEFINE_HANDLE(afxAsset);

AFX_DEFINE_STRUCT(afxCadResource)
{
    afxAsset        asset;
    afxUri          name; // resource name
    void*           obj;
};

AFX_OBJECT(afxAsset)
{
    afxUri              file;
    afxCadToolInfo*     toolInfo;
    afxCadExporterInfo* exporterInfo;
#if 0
    afxNat              texCnt;
    afxTexture*         texs;
    afxNat              mtlCnt;
    afxMaterial*        mtls;
    afxNat              mshdCnt;
    afxVertexData*        mshds;
    afxNat              mshtCnt;
    afxMeshTopology*    mshts;
    afxNat              mshCnt;
    afxMesh*            mshs;
    afxNat              sklCnt;
    afxSkeleton*        skls;
    afxNat              mdlCnt;
    afxModel*           mdls;
    afxNat              anitCnt;
    //track_group* anits;
    afxNat              animCnt;
    //afxAnimation*       anims;
    void*               udd;
#endif
    struct
    {
        afxFcc          type;
        afxNat          resCnt;
        afxCadResource* res;
    }                   *sets;
    afxNat              setCnt;
};

AFX_DEFINE_STRUCT(afxAssetBuilder)
{
    void(*GetName)(void* data, afxUri* name);
    afxNat(*CountSets)(void* data);
    void(*GetSetInfo)(void* data, afxNat setIdx, afxFcc* resType, afxNat* resCnt);
    void*(*GetResourceInfo)(void* data, afxFcc type, afxNat resIdx, afxUri* name);
    afxBool(*GetToolInfo)(afxCadToolInfo* toolInfo);
    afxBool(*GetExporterInfo)(afxCadExporterInfo* exporterInfo);
};

AFX afxError            AfxBuildAssets(afxSimulation sim, afxAssetBuilder const* cadb, afxNat cnt, void* data[], afxAsset cad[]);

AFX afxError            AfxLoadAssets(afxSimulation sim, afxError(*load)(afxUri const* file, afxFlags flags), afxFlags flags, afxNat cnt, afxUri const file[]);
AFX afxError            AfxStoreAssets(afxSimulation sim, afxError(*store)(afxUri const* file, afxFlags flags), afxFlags flags, afxNat cnt, afxUri const file[]);

AFX afxNat              AfxFindResources(afxAsset cad, afxFcc type, afxNat cnt, afxUri const name[], void* res[]);

#endif//AFX_ASSET_H