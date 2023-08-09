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

#ifndef AFX_SIMULATION_H
#define AFX_SIMULATION_H

#include "afx/core/afxClass.h"
#include "afx/sim/afxEntity.h"
#include "afx/sim/rendering/afxLight.h"
#include "afx/sim/rendering/afxCamera.h"
#include "afx/sim/modeling/afxSkeleton.h"
#include "afx/sim/modeling/afxSkeletonBlueprint.h"
#include "afx/sim/modeling/afxModel.h"
#include "afx/sim/modeling/afxModelBlueprint.h"
#include "afx/sim/modeling/afxMeshBlueprint.h"
#include "afx/sim/modeling/afxMaterial.h"
#include "afx/draw/afxDrawInput.h"
#include "afx/draw/afxDrawContext.h"
#include "afx/sim/anim/afxBodyControl.h"
#include "afx/sim/rendering/afxRenderer.h"

// A computer simulation (or "sim") is an attempt to model a real-life or hypothetical situation on a computer so that it can be studied to see how the system works. By changing variables in the simulation, predictions may be made about the behaviour of the system. It is a tool to virtually investigate the behaviour of the system under study.

// space is a purely mathematical representation of a world within space and its interactions.

AFX_DEFINE_STRUCT(afxArtToolInfo)
{
    afxString const*    name; // from art tool name
    afxInt              majorRevision;
    afxInt              minorRevision;
    afxReal             unitsPerMeter;
    afxV3d              origin;
    afxV3d              right;
    afxV3d              up;
    afxV3d              back;
    void*               udd;
};

AFX_DEFINE_STRUCT(afxArtExporterInfo)
{
    afxString const*    name;
    afxInt              majorRevision;
    afxInt              minorRevision;
    afxInt              customization;
    afxInt              buildNum;
    void*               udd;
};

AFX_DEFINE_STRUCT(afxArtAssetInfo)
{
    afxArtToolInfo*     toolInfo;
    afxArtExporterInfo* exporterInfo;
    afxUri*             uri; // from file
    afxNat              texCnt;
    afxTexture*         texs;
    afxNat              mtlCnt;
    afxMaterial*        mtls;
    afxNat              sklCnt;
    afxSkeleton*        skls;
    afxNat              vdCnt;
    //granny_vertex_data* vertexDatas;
    afxNat              mshtCnt;
    afxMeshTopology*    mshts;
    afxNat              mshCnt;
    afxMesh*            mshs;
    afxNat              mdlCnt;
    afxModel*           mdls;
    afxNat              trackGroupCnt;
    //granny_track_group* trackGroups;
    afxNat              animCnt;
    //afxAnimation*       anims;
    void*               udd;
};




AFX_DEFINE_STRUCT(afxSimulationSpecification)
{
    afxUri const            *driver;
    afxAabb const           *bounding;
    afxDrawContext          dctx;
    afxDrawInput            din;

    afxNat                  maxBodCnt;
    afxNat                  maxEntCnt;
    afxNat                  maxCamCnt;
    afxNat                  maxLitCnt;
    afxNat                  maxMtlCnt;
    afxNat                  maxMshCnt;
    afxNat                  maxMdlCnt;
    afxNat                  maxSklCnt;
    afxNat                  maxNodCnt;
};

#ifndef _AFX_API_ONLY

AFX_OBJECT(afxSimulation)
{
    afxInstance       obj;
    afxChain        provisions;

    afxClass        bodObjt;
    afxClass        bodcObjt;
    afxClass        entObjt;
    afxClass        camObjt;
    afxClass        litObjt;

    afxClass        mtlObjt;
    afxClass        mshObjt;
    afxClass        mdlObjt;
    afxClass        sklObjt;
    afxClass        nodObjt;

    afxClass        rndObjt;

    afxContext    genrlMem;
    afxArray        roots;

    afxAabb         area;
    afxV4d          origin;

    afxSize         stepNum, stepsPerSecond;
    afxSize         stepTime, swapTime;
    afxReal         stepDeltaTime;

    afxBodyControlInterlink  globalBodyControlInterlinks;

    afxDrawContext  dctx;
};

#endif

AFX afxError        AfxSimulationAcquireRenderers(afxSimulation sim, afxNat cnt, void *spec, afxRenderer rnd[]);

AFX afxError        AfxSimulationAcquireSkeletons(afxSimulation sim, afxNat cnt, afxUri const uri[], afxSkeleton skl[]);
AFX afxError        AfxSimulationBuildSkeletons(afxSimulation sim, afxNat cnt, afxSkeletonBlueprint blueprint[], afxSkeleton skl[]);
AFX afxResult       AfxSimulationFindSkeletons(afxSimulation sim, afxNat cnt, afxUri const uri[], afxSkeleton skl[]);
AFX afxError        AfxSimulationUploadSkeletons(afxSimulation sim, afxNat cnt, afxUri const uri[], afxSkeleton skl[]);

AFX afxError        AfxSimulationAcquireMaterials(afxSimulation sim, afxNat cnt, afxUri const uri[], afxMaterial mtl[]);
AFX afxError        AfxSimulationBuildMaterials(afxSimulation sim, afxNat cnt, afxMaterialBlueprint const blueprint[], afxMaterial mtl[]);
AFX afxResult       AfxSimulationFindMaterials(afxSimulation sim, afxNat cnt, afxUri const uri[], afxMaterial mtl[]);
AFX afxError        AfxSimulationUploadMaterials(afxSimulation sim, afxNat cnt, afxUri const uri[], afxMaterial mtl[]);

AFX afxError        AfxSimulationAcquireMeshes(afxSimulation sim, afxNat cnt, afxUri const uri[], afxMesh msh[]);
AFX afxError        AfxSimulationBuildMeshes(afxSimulation sim, afxNat cnt, afxMeshBlueprint blueprint[], afxMesh msh[]);
AFX afxResult       AfxSimulationFindMeshes(afxSimulation sim, afxNat cnt, afxUri const uri[], afxMesh msh[]);
AFX afxError        AfxSimulationUploadMeshes(afxSimulation sim, afxNat cnt, afxUri const uri[], afxMesh msh[]);

AFX afxError        AfxSimulationAcquireModels(afxSimulation sim, afxNat cnt, afxUri const uri[], afxModel mdl[]);
AFX afxError        AfxSimulationBuildModels(afxSimulation sim, afxNat cnt, afxModelBlueprint blueprint[], afxModel mdl[]);
AFX afxResult       AfxSimulationFindModels(afxSimulation sim, afxNat cnt, afxUri const uri[], afxModel mdl[]);
AFX afxError        AfxSimulationUploadModels(afxSimulation sim, afxNat cnt, afxUri const uri[], afxModel mdl[]);

AFX afxError        AfxSimulationAcquireBodies(afxSimulation sim, afxNat cnt, afxModel mdl[], afxBody bod[]);

AFX afxCamera       AfxSimulationAcquireCamera(afxSimulation sim, afxString const *name, afxNode parent, afxBool perspective);
AFX afxEntity       AfxSimulationAcquireEntity(afxSimulation sim, afxString const *name, afxNode parent, afxBody bod);

AFX afxLight        AfxSimulationAcquireDirectionalLight(afxSimulation sim, afxColor color);
AFX afxLight        AfxSimulationAcquirePointLight(afxSimulation sim, afxColor color, afxReal range);
AFX afxLight        AfxSimulationAcquireSpotLight(afxSimulation sim, afxColor color, afxReal range, afxReal radians);
AFX afxLight        AfxSimulationAcquireAreaLight(afxSimulation sim, afxColor color);

AFX afxNode         AfxSimulationAcquireNode(afxSimulation sim, afxNodeSpecification const *spec);

AFX void*           AfxSimulationGetApplication(afxSimulation sim);

AFX afxContext       AfxSimulationGetMemory(afxSimulation sim);

AFX afxClass*       AfxSimulationGetBodyClass(afxSimulation sim);
AFX afxClass*       AfxSimulationGetBodyControlClass(afxSimulation sim);
AFX afxClass*       AfxSimulationGetCameraClass(afxSimulation sim);
AFX afxClass*       AfxSimulationGetEntityClass(afxSimulation sim);
AFX afxClass*       AfxSimulationGetLightClass(afxSimulation sim);
AFX afxClass*       AfxSimulationGetMaterialClass(afxSimulation sim);
AFX afxClass*       AfxSimulationGetMeshClass(afxSimulation sim);
AFX afxClass*       AfxSimulationGetModelClass(afxSimulation sim);
AFX afxClass*       AfxSimulationGetNodeClass(afxSimulation sim);
AFX afxClass*       AfxSimulationGetSkeletonClass(afxSimulation sim);
AFX afxClass*       AfxSimulationGetRendererClass(afxSimulation sim);

AFX afxNat          AfxSimulationEnumerateBodies(afxSimulation sim, afxNat base, afxNat cnt, afxBody bod[]);
AFX afxNat          AfxSimulationEnumerateCameras(afxSimulation sim, afxNat base, afxNat cnt, afxCamera cam[]);
AFX afxNat          AfxSimulationEnumerateEntities(afxSimulation sim, afxNat base, afxNat cnt, afxEntity ent[]);
AFX afxNat          AfxSimulationEnumerateMaterials(afxSimulation sim, afxNat base, afxNat cnt, afxMaterial mtl[]);
AFX afxNat          AfxSimulationEnumerateMeshes(afxSimulation sim, afxNat base, afxNat cnt, afxMesh msh[]);
AFX afxNat          AfxSimulationEnumerateModels(afxSimulation sim, afxNat base, afxNat cnt, afxModel mdl[]);
AFX afxNat          AfxSimulationEnumerateSkeletons(afxSimulation sim, afxNat base, afxNat cnt, afxSkeleton skl[]);
AFX afxNat          AfxSimulationForEachNode(afxSimulation sim, void (*f)(afxIterator *iter), void *data);

AFX afxError        _AfxSimulationProcess(afxSimulation sim);
AFX afxError        AfxSimulationRender(afxSimulation sim, afxCamera cam, afxV4d point, afxDrawInput din);
AFX afxError        AfxSimulationStep(afxSimulation sim);

AFX void            AfxSimulationRecenterAllBodyControlClocks(afxSimulation sim, afxReal dCurrentClock);

#endif//AFX_SIMULATION_H