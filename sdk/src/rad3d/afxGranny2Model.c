#define _CRT_SECURE_NO_WARNINGS 1
#define WIN32_LEAN_AND_MEAN 1
#include <Windows.h>
#include "../dep/granny2/granny.h"
#pragma comment(lib, "../../dep/granny2/granny2")

#define _AFX_MOTION_C
#include "afxGranny2Model.h"
#include "qwadro/sim/modeling/afxMesh.h"
#include "qwadro/sim/modeling/awxVertexData.h"
#include "qwadro/sim/awxNode.h"
#include "qwadro/io/afxUri.h"
#include "qwadro/sim/afxSimulation.h"
#include "qwadro/core/afxSystem.h"
#include "qwadro/core/afxIndexedString.h"
#include "qwadro/math/afxQuaternion.h"
#include "qwadro/math/afxMatrix.h"
#include "qwadro/math/afxVector.h"

void BuildVertexDataFrom(afxSimulation sim, afxMeshBuilder* mshb, granny_vertex_data* gvd, granny_mesh* Mesh)
{
    afxError err = NIL;
    AfxAssertObjects(1, &sim, afxFcc_SIM);

}

awxVertexData Gr2VertexDataToQwadro(afxSimulation sim, afxStringCatalog strc, afxNat pivotCnt, granny_vertex_data* VertexData)
{
    afxError err = NIL;

    afxVertexBias rigidBias;
    rigidBias.pivotIdx = 0;
    rigidBias.weight = 1.f;
    afxVertex rigidVtx;
    rigidVtx.baseBiasIdx = 0;
    rigidVtx.biasCnt = 1;
    awxVertexDataSpec spec = { 0 };
    spec.attrCnt = 3;
    spec.baseAttrIdx = 0;
    spec.vtxCnt = VertexData->VertexCount;
    spec.biasCnt = 1;
    spec.biasSrc = &rigidBias;
    spec.biasSrcStride = 0;
    spec.vtxSrc = NIL;
    spec.vtxSrcStride = 0;

    awxVertexAttrSpec attrSpecs[16];
    afxNat attrCnt = 0;
    afxBool deformable = FALSE;
    afxBool rigid = TRUE;

    attrSpecs[attrCnt].id = "pos";
    attrSpecs[attrCnt].flags = awxVertexFlag_AFFINE | awxVertexFlag_LINEAR | awxVertexFlag_POSITIONAL;
    attrSpecs[attrCnt].fmt = afxVertexFormat_V3D;
    attrSpecs[attrCnt].usage = awxVertexUsage_POS | awxVertexUsage_POSITIONAL;
    attrSpecs[attrCnt].src = NIL;
    attrSpecs[attrCnt].srcStride = 0;
    ++attrCnt;

    if (pivotCnt > 1)
    {
        rigid = FALSE;

        attrSpecs[attrCnt].id = "pvt";
        attrSpecs[attrCnt].flags = awxVertexFlag_POSITIONAL;
        attrSpecs[attrCnt].fmt = pivotCnt > 2 ? afxVertexFormat_V4B : afxVertexFormat_BYTE;
        attrSpecs[attrCnt].usage = awxVertexUsage_JNT | awxVertexUsage_POSITIONAL;
        attrSpecs[attrCnt].src = NIL;
        attrSpecs[attrCnt].srcStride = 0;
        ++attrCnt;

        if (pivotCnt > 2)
        {
            deformable = TRUE;

            attrSpecs[attrCnt].id = "wgt";
            attrSpecs[attrCnt].flags = awxVertexFlag_NORMALIZED | awxVertexFlag_POSITIONAL;
            attrSpecs[attrCnt].fmt = afxVertexFormat_V4D;
            attrSpecs[attrCnt].usage = awxVertexUsage_WGT | awxVertexUsage_POSITIONAL;
            attrSpecs[attrCnt].src = NIL;
            attrSpecs[attrCnt].srcStride = 0;
            ++attrCnt;
        }
    }

    attrSpecs[attrCnt].id = "nrm";
    attrSpecs[attrCnt].flags = awxVertexFlag_LINEAR;
    attrSpecs[attrCnt].fmt = afxVertexFormat_V3D;
    attrSpecs[attrCnt].usage = awxVertexUsage_NRM | awxVertexUsage_TANGENT;
    attrSpecs[attrCnt].src = NIL;
    attrSpecs[attrCnt].srcStride = 0;
    ++attrCnt;

    attrSpecs[attrCnt].id = "uv";
    attrSpecs[attrCnt].flags = NIL;
    attrSpecs[attrCnt].fmt = afxVertexFormat_V2D;
    attrSpecs[attrCnt].usage = awxVertexUsage_UV;
    attrSpecs[attrCnt].src = NIL;
    attrSpecs[attrCnt].srcStride = 0;
    ++attrCnt;

    awxVertexData vtd;
    AwxAcquireVertexDatas(sim, strc, attrSpecs, 1, &spec, &vtd);

    switch (pivotCnt)
    {
    case 0:
    case 1:
    {
        afxNat bufferSize = spec.vtxCnt * sizeof(granny_pnt332_vertex);
        granny_pnt332_vertex* Vertices = AfxAllocate(NIL, bufferSize, 1, AFX_SIMD_ALIGN, AfxHint());
        GrannyConvertVertexLayouts(spec.vtxCnt, VertexData->VertexType, VertexData->Vertices, GrannyPNT332VertexType, Vertices);

        afxV4d hcw;
        AfxResetV4d(hcw);
        AwxFillVertexData(vtd, 0, 0, spec.vtxCnt, &hcw);

        AwxUpdateVertexData(vtd, 0, 0, spec.vtxCnt, Vertices->Position, sizeof(Vertices[0]));
        AwxUpdateVertexData(vtd, 1, 0, spec.vtxCnt, Vertices->Normal, sizeof(Vertices[0]));
        AwxUpdateVertexData(vtd, 2, 0, spec.vtxCnt, Vertices->UV, sizeof(Vertices[0]));

        AfxDeallocate(NIL, Vertices);
        break;
    }
#if 0
    case 1:
    {
        afxNat bufferSize = spec.vtxCnt * sizeof(granny_pwnt3132_vertex);
        granny_pwnt3132_vertex* Vertices = AfxAllocate(NIL, bufferSize, 1, AFX_SIMD_ALIGN, AfxHint());
        GrannyConvertVertexLayouts(spec.vtxCnt, VertexData->VertexType, VertexData->Vertices, GrannyPWNT3132VertexType, Vertices);

        afxV4d hcw;
        AfxResetV4d(hcw);
        AwxFillVertexData(vtd, 0, 0, spec.vtxCnt, &hcw);

        AwxUpdateVertexData(vtd, 0, 0, spec.vtxCnt, Vertices->Position, sizeof(Vertices[0]));
        AwxUpdateVertexData(vtd, 1, 0, spec.vtxCnt, &Vertices->BoneIndex, sizeof(Vertices[0]));
        AwxUpdateVertexData(vtd, 2, 0, spec.vtxCnt, Vertices->Normal, sizeof(Vertices[0]));
        AwxUpdateVertexData(vtd, 3, 0, spec.vtxCnt, Vertices->UV, sizeof(Vertices[0]));

        AfxDeallocate(NIL, Vertices);
        break;
    }
#endif
    case 2:
    {
        afxNat bufferSize = spec.vtxCnt * sizeof(granny_pwnt3232_vertex);
        granny_pwnt3232_vertex* Vertices = AfxAllocate(NIL, bufferSize, 1, AFX_SIMD_ALIGN, AfxHint());
        GrannyConvertVertexLayouts(spec.vtxCnt, VertexData->VertexType, VertexData->Vertices, GrannyPWNT3232VertexType, Vertices);

        afxV4d hcw;
        AfxResetV4d(hcw);
        AwxFillVertexData(vtd, 0, 0, spec.vtxCnt, &hcw);

        AwxUpdateVertexData(vtd, 0, 0, spec.vtxCnt, Vertices->Position, sizeof(Vertices[0]));
        AwxUpdateVertexData(vtd, 1, 0, spec.vtxCnt, Vertices->BoneIndices, sizeof(Vertices[0]));
        AwxUpdateVertexData(vtd, 2, 0, spec.vtxCnt, Vertices->BoneWeights, sizeof(Vertices[0]));
        AwxUpdateVertexData(vtd, 3, 0, spec.vtxCnt, Vertices->Normal, sizeof(Vertices[0]));
        AwxUpdateVertexData(vtd, 4, 0, spec.vtxCnt, Vertices->UV, sizeof(Vertices[0]));

        AfxDeallocate(NIL, Vertices);
        break;
    }
    case 3:
    case 4:
    {
        afxNat bufferSize = spec.vtxCnt * sizeof(granny_pwnt3432_vertex);
        granny_pwnt3432_vertex* Vertices = AfxAllocate(NIL, bufferSize, 1, AFX_SIMD_ALIGN, AfxHint());
        GrannyConvertVertexLayouts(spec.vtxCnt, VertexData->VertexType, VertexData->Vertices, GrannyPWNT3432VertexType, Vertices);

        afxV4d hcw;
        AfxResetV4d(hcw);
        AwxFillVertexData(vtd, 0, 0, spec.vtxCnt, &hcw);

        AwxUpdateVertexData(vtd, 0, 0, spec.vtxCnt, Vertices->Position, sizeof(Vertices[0]));
        AwxUpdateVertexData(vtd, 1, 0, spec.vtxCnt, Vertices->BoneIndices, sizeof(Vertices[0]));
        AwxUpdateVertexData(vtd, 2, 0, spec.vtxCnt, Vertices->BoneWeights, sizeof(Vertices[0]));
        AwxUpdateVertexData(vtd, 3, 0, spec.vtxCnt, Vertices->Normal, sizeof(Vertices[0]));
        AwxUpdateVertexData(vtd, 4, 0, spec.vtxCnt, Vertices->UV, sizeof(Vertices[0]));

        AfxDeallocate(NIL, Vertices);
        break;
    }
    default: AfxThrowError(); break;
    }

    return vtd;
}

afxMeshTopology Gr2TopologyToQwadro(afxSimulation sim, granny_tri_topology* Topology)
{
    afxError err = NIL;

    afxMeshTopologySpec spec = { 0 };
    spec.src = Topology->IndexCount ? Topology->Indices : Topology->Indices16;
    spec.srcIdxSiz = Topology->IndexCount ? sizeof(afxNat32) : sizeof(afxNat16);
    spec.surCnt = Topology->GroupCount;
    spec.triCnt = Topology->IndexCount ? Topology->IndexCount / 3 : Topology->Index16Count / 3;
    spec.vtxCnt = 0;

    afxMeshTopology msht;
    AfxAcquireMeshTopology(sim, 1, &spec, &msht);

    afxIndexedTriangle* tris = AfxGetMeshTriangles(msht, 0);
    afxNat baseTriIdx = 0;

    for (afxNat i = 0; i < spec.surCnt; i++)
    {
        afxMeshSurface* mshs = AfxGetMeshSurface(msht, i);
        //AfxAssignTypeFcc(mshs, afxFcc_MSHS);
        mshs->mtlIdx = i;
        mshs->triCnt = Topology->Groups[i].TriCount;
        mshs->baseTriIdx = Topology->Groups[i].TriFirst;
    }

    return msht;
}

afxSkeleton Gr2SkeletonToQwadro(afxSimulation sim, afxStringCatalog strc, granny_skeleton* Skeleton)
{
    afxError err = NIL;

    afxString pivots[256];

    afxString id;
    afxSkeletonBuilder sklb;
    AfxMakeString(&id, Skeleton->Name, 0);
    AfxBeginSkeletonBuilding(&sklb, Skeleton->BoneCount, &id, 0);

    for (granny_int32 i = 0; i < Skeleton->BoneCount; i++)
    {
        granny_bone* Bone = &Skeleton->Bones[i];
        AfxMakeString(&id, Bone->Name, 0);

        AfxMakeString(&pivots[i], Bone->Name, 0);

        afxTransform t;
        AfxSetTransformWithIdentityCheck(&t, Bone->LocalTransform.Position, Bone->LocalTransform.Orientation, Bone->LocalTransform.ScaleShear);
        AfxResetBoneWithIw(&sklb, i, &id, Bone->ParentIndex, &t, Bone->InverseWorld4x4, Bone->LODError);
    }
    afxSkeleton skl;
    AfxBuildSkeletons(sim, strc, 1, &sklb, &skl);
    AfxEndSkeletonBuilding(&sklb);
    return skl;
}

afxMesh Gr2MeshToQwadro(afxSimulation sim, afxStringCatalog strc, granny_mesh* Mesh)
{
    afxError err = NIL;

    awxVertexData vtd = Gr2VertexDataToQwadro(sim, strc, Mesh->BoneBindingCount, Mesh->PrimaryVertexData);
    afxMeshTopology msht = Gr2TopologyToQwadro(sim, Mesh->PrimaryTopology);

    afxString pivots[256];

    afxMeshBlueprint blue = { 0 };
    blue.vertices = vtd;
    blue.topology = msht;
    blue.pivots = pivots;
    blue.biasCnt = Mesh->BoneBindingCount;
    blue.mtlCnt = Mesh->MaterialBindingCount;
    blue.strc = strc;
    afxString tid;
    AfxMakeString(&tid, Mesh->Name, 0);
    AfxMakeString32(&blue.id, &tid);
    
    for (afxNat i = 0; i < blue.biasCnt; i++)
    {
        AfxMakeString(&pivots[i], Mesh->BoneBindings[i].BoneName, 0);
    }

    afxMesh msh;
    AfxAssembleMeshes(sim, 1, &blue, &msh);

    return msh;
}

afxModel Gr2ModelToQwadro(afxSimulation sim, afxStringCatalog strc, granny_model* Model)
{
    afxError err = NIL;
    
    afxModelBlueprint blue = { 0 };
    blue.meshes = NIL;
    afxString tid;
    AfxMakeString(&tid, Model->Name, 0);
    AfxMakeString32(&blue.id, &tid);
    AfxSetTransformWithIdentityCheck(&blue.init, Model->InitialPlacement.Position, Model->InitialPlacement.Orientation, Model->InitialPlacement.ScaleShear);
    blue.mshCnt = Model->MeshBindingCount;
    blue.skl = Gr2SkeletonToQwadro(sim, strc, Model->Skeleton);
    blue.strc = strc;

    afxModel mdl;
    AfxAssembleModel(sim, 1, &blue, &mdl);

    for (afxNat i = 0; i < blue.mshCnt; i++)
    {
        afxMesh msh = Gr2MeshToQwadro(sim, strc, Model->MeshBindings[i].Mesh);
        AfxRigMeshes(mdl, blue.skl, i, 1, &msh);
    }

    return mdl;
}

afxModel AwxLoadModelsFromGrn3d2(afxSimulation sim, afxUri const *uri, afxNat mdlIdx)
{
    afxError err = NIL;
    AfxAssertObjects(1, &sim, afxFcc_SIM);

    afxUri2048 uriOut;
    AfxMakeUri2048(&uriOut, NIL);
    AfxResolveUri(afxFileFlag_R, uri, &uriOut.uri);

    granny_file* GrannyFile = GrannyReadEntireFile(AfxGetUriData(&uriOut.uri, 0));
    granny_file_info* Info = GrannyGetFileInfo(GrannyFile);
    AfxAssert(Info);
    AfxAssertRange((afxNat)Info->ModelCount, mdlIdx, 1);

    afxStringCatalog strc;
    AfxAcquireStringCatalogs(1, &strc);

    afxString id;
    afxTransform t;

    granny_model* Model = Info->Models[mdlIdx];

    afxModel mdl = Gr2ModelToQwadro(sim, strc, Model);

    GrannyFreeFile(GrannyFile);

    return mdl;
}

void Gr2CurveToQwadro(afxSimulation sim, granny_curve2* Curve, afxCurve *c)
{
    afxError err = NIL;
    void* data = Curve->CurveData.Object;
    granny_curve_data_header* hdr = data;

    switch (hdr->Format)
    {
#if 0
    case afxCurveFormat_DaKeyframes32f:
    {
        granny_curve_data_da_keyframes32f* d = data;
        AfxSetUpCurve(c, afxCurveFormat_DaKeyframes32f, d->CurveDataHeader.Degree, d->Dimension, d->ControlCount);
        AfxCurveIsIdentity();
        break;
    }
#endif
    case afxCurveFormat_DaK32fC32f:
    {
        granny_curve_data_da_k32f_c32f* d = data;
        //AfxSetUpCurve(c, afxCurveFormat_DaK32fC32f, d->CurveDataHeader.Degree, 1, d->ControlCount);
        afxCurve tmp;
        AfxMakeCurveDaKC32f(&tmp, GrannyCurveGetDegree(Curve), GrannyCurveGetDimension(Curve), GrannyCurveGetKnotCount(Curve), d->Knots, d->Controls);
        afxCurveBlueprint cb;
        AfxBeginCurveCopy(&cb, &tmp);
        AfxEndCurve(&cb, c);



        break;
    }
#if 0
    case afxCurveFormat_DaConstant32f:
    {
        granny_curve_data_da_constant32f* d = data;
        break;
    }
    case afxCurveFormat_D3Constant32f:
    {
        granny_curve_data_d3_constant32f* d = data;
        break;
    }
    case afxCurveFormat_D4Constant32f:
    {
        granny_curve_data_d4_constant32f* d = data;
        break;
    }
#endif
    default: AfxThrowError(); break;
    }

}

awxMotion Gr2MotionToQwadro(afxSimulation sim, afxStringCatalog strc, granny_track_group* TrackGroup)
{
    afxError err = NIL;

    afxString pivots[256];

    awxMotionBlueprint blue = { 0 };
    blue.strc = strc;
    blue.xformId = pivots;
    blue.xformLodError = TrackGroup->TransformLODErrors;
    blue.vecCnt = TrackGroup->VectorTrackCount;
    blue.xformCnt = TrackGroup->TransformTrackCount;

    afxString tid;
    AfxMakeString(&tid, TrackGroup->Name, 0);
    AfxMakeString32(&blue.id, &tid);

    for (afxNat i = 0; i < TrackGroup->TransformTrackCount; i++)
    {
        AfxMakeString(&pivots[i], TrackGroup->TransformTracks[i].Name, 0);
    }

    awxMotion mot;
    AfxAssembleMotions(sim, 1, &blue, &mot);

    //mot->flags = TrackGroup->Flags;

    for (afxNat i = 0; i < TrackGroup->TransformTrackCount; i++)
    {
        AfxMakeString(&tid, TrackGroup->TransformTracks[i].Name, 0);
        afxMask flags = TrackGroup->TransformTracks[i].Flags;

        afxCurve o, t, s;
        Gr2CurveToQwadro(sim, &TrackGroup->TransformTracks[i].OrientationCurve, &o);
        Gr2CurveToQwadro(sim, &TrackGroup->TransformTracks[i].PositionCurve, &t);
        Gr2CurveToQwadro(sim, &TrackGroup->TransformTracks[i].ScaleShearCurve, &s);

        AfxUpdateMotionTransforms(mot, i, 1, &flags, &o, &t, &s, 0);
    }
    
    int a = 4;
    return mot;
}

awxAnimation Gr2AnimToQwadro(afxSimulation sim, afxStringCatalog strc, granny_animation* Anim)
{
    afxError err = NIL;

    awxAnimationBlueprint anib = { 0 };
    anib.motions = NIL;
    anib.motSlotCnt = Anim->TrackGroupCount;
    anib.dur = Anim->Duration;
    anib.oversampling = Anim->Oversampling;
    anib.timeStep = Anim->TimeStep;
    anib.strc = strc;
    anib.strc2 = strc;

    afxUri uri, name;
    AfxMakeUri(&uri, Anim->Name, 0);
    AfxExcerptUriName(&uri, &name);
    AfxMakeString32(&anib.id, AfxGetUriString(&name));

    awxAnimation ani;
    AwxAssembleAnimations(sim, 1, &anib, &ani);

    for (afxNat i = 0; i < anib.motSlotCnt; i++)
    {
        awxMotion mot = Gr2MotionToQwadro(sim, strc, Anim->TrackGroups[i]);
        AwxRelinkMotions(ani, i, 1, &mot);
    }

    return ani;
}

awxAnimation AwxLoadAnimationsFromGrn3d2(afxSimulation sim, afxUri const *uri, afxNat animIdx)
{
    afxError err = NIL;
    AfxAssertObjects(1, &sim, afxFcc_SIM);

    afxUri2048 uriOut;
    AfxMakeUri2048(&uriOut, NIL);
    AfxResolveUri(afxFileFlag_R, uri, &uriOut.uri);

    granny_file* GrannyFile = GrannyReadEntireFile(AfxGetUriData(&uriOut.uri, 0));
    granny_file_info* Info = GrannyGetFileInfo(GrannyFile);
    AfxAssert(Info);
    AfxAssertRange((afxNat)Info->AnimationCount, animIdx, 1);

    afxStringCatalog strc;
    AfxAcquireStringCatalogs(1, &strc);

    granny_animation* Anim = Info->Animations[animIdx];

    awxAnimation ani = Gr2AnimToQwadro(sim, strc, Anim);

    GrannyFreeFile(GrannyFile);

    return ani;
}
