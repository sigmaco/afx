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
 *                   (c) 2017 SIGMA Technology Group — Federação SIGMA
 *                                    www.sigmaco.org
 */

#ifndef AFX_ASSET_H
#define AFX_ASSET_H

#include "afx/core/afxClass.h"
#include "afx/sim/afxEntity.h"
#include "afx/core/afxSource.h"
#include "afx/sim/rendering/afxLight.h"
#include "afx/sim/rendering/afxCamera.h"
#include "afx/sim/afxSkeleton.h"
#include "afx/sim/modeling/afxModel.h"
#include "afx/sim/afxMaterial.h"
#include "afx/draw/afxDrawInput.h"
#include "afx/draw/afxDrawContext.h"
#include "afx/sim/anim/afxAnimator.h"
#include "afx/sim/rendering/afxRenderer.h"
#include "afx/sim/anim/afxAnimation.h"

AFX_DEFINE_STRUCT(afxCadToolInfo)
{
    afxString const*    name; // from art tool name
    afxNat              majorRev;
    afxNat              minorRev;
    afxV3d              right; // X-vector (aka right) of art tool.
    afxV3d              up; // Y-vector (aka up) of art tool.
    afxV3d              at; // Z-vector (aka at) of art tool.
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

AFX_DEFINE_STRUCT(afxResourceSlot)
{
    afxString32     id;
    void*           obj;
    afxUri          uri;
};

AFX_OBJECT(afxAsset)
{
    afxUri              file;
    afxCadToolInfo*     toolInfo;
    afxCadExporterInfo* exporterInfo;

    afxNat              resCap;
    afxResourceSlot*    resSlots;
    struct
    {
        afxFcc          fcc;
        afxNat          firstResIdx;
        afxNat          resCnt;
    }                  *nests;
    afxNat              nestCnt;
#if 0
    afxNat              texCnt;
    afxTexture*         texs;
    afxNat              mtlCnt;
    afxMaterial*        mtls;
    afxNat              mshdCnt;
    afxVertexData*      mshds;
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
    }                  *sets;
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

AFX afxNat              AfxFindResources(afxAsset cad, afxFcc type, afxNat cnt, afxUri const name[], void* res[]);

AFX afxNat              AfxFindResourceNests(afxAsset cad, afxNat cnt, afxFcc const fcc[], afxNat nestIdx[]);
AFX afxNat              AfxFindResources2(afxAsset cad, afxFcc nest, afxNat cnt, afxString const id[], void* res[]);

AFX afxNat              AfxFindTextures(afxAsset cad, afxNat cnt, afxString const id[], afxTexture tex[]);
AFX afxNat              AfxFindMaterials(afxAsset cad, afxNat cnt, afxString const id[], afxMaterial mt[]);
AFX afxNat              AfxFindVertexDatas(afxAsset cad, afxNat cnt, afxString const id[], afxVertexData vtd[]);
AFX afxNat              AfxFindTopologies(afxAsset cad, afxNat cnt, afxString const id[], afxMeshTopology msht[]);
AFX afxNat              AfxFindMeshes(afxAsset cad, afxNat cnt, afxString const id[], afxMesh msh[]);
AFX afxNat              AfxFindSkeletons(afxAsset cad, afxNat cnt, afxString const id[], afxSkeleton skl[]);
AFX afxNat              AfxFindModels(afxAsset cad, afxNat cnt, afxString const id[], afxModel mdl[]);
AFX afxNat              AfxFindAnimations(afxAsset cad, afxNat cnt, afxString const id[], afxAnimation anim[]);

////////////////////////////////////////////////////////////////////////////////
// MASSIVE OPERATIONS                                                         //
////////////////////////////////////////////////////////////////////////////////

AFX afxError            AfxAcquireAssets(afxSimulation sim, afxNat cnt, afxNat const nestCnt[], afxNat const resCap[], afxAsset cad[]);
AFX afxError            AfxBuildAssets(afxSimulation sim, afxAssetBuilder const* cadb, afxNat cnt, void* data[], afxAsset cad[]);

AFX afxError            AfxLoadAssets(afxSimulation sim, afxError(*load)(afxUri const* file, afxFlags flags), afxFlags flags, afxNat cnt, afxUri const file[]);
AFX afxError            AfxStoreAssets(afxSimulation sim, afxError(*store)(afxUri const* file, afxFlags flags), afxFlags flags, afxNat cnt, afxUri const file[]);

#endif//AFX_ASSET_H