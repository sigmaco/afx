#define _CRT_SECURE_NO_WARNINGS 1
#define WIN32_LEAN_AND_MEAN 1
#include <Windows.h>
#include "./dep/granny2/granny.h"
#pragma comment(lib, "../../src/rad3d/dep/granny2/granny2")

#define _AFX_MOTION_C
#include "afxGranny2Model.h"
#include "qwadro/cad/afxMesh.h"
#include "qwadro/cad/akxVertexData.h"
#include "qwadro/sim/akxNode.h"
#include "qwadro/io/afxUri.h"
#include "qwadro/sim/afxSimulation.h"
#include "qwadro/core/afxSystem.h"
#include "qwadro/mem/afxMappedString.h"
#include "qwadro/math/afxQuaternion.h"
#include "qwadro/math/afxMatrix.h"
#include "qwadro/math/afxVector.h"

void BuildVertexDataFrom(afxSimulation sim, afxMeshBuilder* mshb, granny_vertex_data* gvd, granny_mesh* Mesh)
{
    afxError err = NIL;
    AfxAssertObjects(1, &sim, afxFcc_SIM);

}

akxVertexData Gr2VertexDataToQwadro(afxSimulation sim, afxStringBase strb, afxNat pivotCnt, granny_vertex_data* VertexData)
{
    afxError err = NIL;

    afxVertexBias rigidBias;
    rigidBias.pivotIdx = 0;
    rigidBias.weight = 1.f;
    afxVertex rigidVtx;
    rigidVtx.baseBiasIdx = 0;
    rigidVtx.biasCnt = 1;
    akxVertexDataSpec spec = { 0 };
    spec.attrCnt = 3;
    spec.baseAttrIdx = 0;
    spec.vtxCnt = VertexData->VertexCount;
    spec.biasCnt = 1;
    spec.biasSrc = &rigidBias;
    spec.biasSrcStride = 0;
    spec.vtxSrc = NIL;
    spec.vtxSrcStride = 0;

    akxVertexAttrSpec attrSpecs[16];
    afxNat attrCnt = 0;
    afxBool deformable = FALSE;
    afxBool rigid = TRUE;

    attrSpecs[attrCnt].id = "pos";
    attrSpecs[attrCnt].flags = akxVertexFlag_AFFINE | akxVertexFlag_LINEAR | akxVertexFlag_POSITIONAL;
    attrSpecs[attrCnt].fmt = afxVertexFormat_V3D;
    attrSpecs[attrCnt].usage = akxVertexUsage_POS | akxVertexUsage_POSITIONAL;
    attrSpecs[attrCnt].src = NIL;
    attrSpecs[attrCnt].srcStride = 0;
    ++attrCnt;

    if (pivotCnt > 1)
    {
        rigid = FALSE;

        attrSpecs[attrCnt].id = "pvt";
        attrSpecs[attrCnt].flags = akxVertexFlag_POSITIONAL;
        attrSpecs[attrCnt].fmt = pivotCnt > 2 ? afxVertexFormat_V4B : afxVertexFormat_BYTE;
        attrSpecs[attrCnt].usage = akxVertexUsage_JNT | akxVertexUsage_POSITIONAL;
        attrSpecs[attrCnt].src = NIL;
        attrSpecs[attrCnt].srcStride = 0;
        ++attrCnt;

        if (pivotCnt > 2)
        {
            deformable = TRUE;

            attrSpecs[attrCnt].id = "wgt";
            attrSpecs[attrCnt].flags = akxVertexFlag_NORMALIZED | akxVertexFlag_POSITIONAL;
            attrSpecs[attrCnt].fmt = afxVertexFormat_V4D;
            attrSpecs[attrCnt].usage = akxVertexUsage_WGT | akxVertexUsage_POSITIONAL;
            attrSpecs[attrCnt].src = NIL;
            attrSpecs[attrCnt].srcStride = 0;
            ++attrCnt;
        }
    }

    attrSpecs[attrCnt].id = "nrm";
    attrSpecs[attrCnt].flags = akxVertexFlag_LINEAR;
    attrSpecs[attrCnt].fmt = afxVertexFormat_V3D;
    attrSpecs[attrCnt].usage = akxVertexUsage_NRM | akxVertexUsage_TANGENT;
    attrSpecs[attrCnt].src = NIL;
    attrSpecs[attrCnt].srcStride = 0;
    ++attrCnt;

    attrSpecs[attrCnt].id = "uv";
    attrSpecs[attrCnt].flags = NIL;
    attrSpecs[attrCnt].fmt = afxVertexFormat_V2D;
    attrSpecs[attrCnt].usage = akxVertexUsage_UV;
    attrSpecs[attrCnt].src = NIL;
    attrSpecs[attrCnt].srcStride = 0;
    ++attrCnt;

    akxVertexData vtd;
    AkxAcquireVertexDatas(sim, strb, attrSpecs, 1, &spec, &vtd);

    switch (pivotCnt)
    {
    case 0:
    case 1:
    {
        afxNat bufferSize = spec.vtxCnt * sizeof(granny_pnt332_vertex);
        granny_pnt332_vertex* Vertices = AfxAllocate(bufferSize, 1, AFX_SIMD_ALIGN, AfxHere());
        GrannyConvertVertexLayouts(spec.vtxCnt, VertexData->VertexType, VertexData->Vertices, GrannyPNT332VertexType, Vertices);

        afxV4d hcw;
        AfxResetV4d(hcw);
        AkxFillVertexData(vtd, 0, 0, spec.vtxCnt, &hcw);

        AkxUpdateVertexData(vtd, 0, 0, spec.vtxCnt, Vertices->Position, sizeof(Vertices[0]));
        AkxUpdateVertexData(vtd, 1, 0, spec.vtxCnt, Vertices->Normal, sizeof(Vertices[0]));
        AkxUpdateVertexData(vtd, 2, 0, spec.vtxCnt, Vertices->UV, sizeof(Vertices[0]));

        AfxDeallocate(Vertices);
        break;
    }
#if 0
    case 1:
    {
        afxNat bufferSize = spec.vtxCnt * sizeof(granny_pwnt3132_vertex);
        granny_pwnt3132_vertex* Vertices = AfxAllocate(bufferSize, 1, AFX_SIMD_ALIGN, AfxHere());
        GrannyConvertVertexLayouts(spec.vtxCnt, VertexData->VertexType, VertexData->Vertices, GrannyPWNT3132VertexType, Vertices);

        afxV4d hcw;
        AfxResetV4d(hcw);
        AkxFillVertexData(vtd, 0, 0, spec.vtxCnt, &hcw);

        AkxUpdateVertexData(vtd, 0, 0, spec.vtxCnt, Vertices->Position, sizeof(Vertices[0]));
        AkxUpdateVertexData(vtd, 1, 0, spec.vtxCnt, &Vertices->BoneIndex, sizeof(Vertices[0]));
        AkxUpdateVertexData(vtd, 2, 0, spec.vtxCnt, Vertices->Normal, sizeof(Vertices[0]));
        AkxUpdateVertexData(vtd, 3, 0, spec.vtxCnt, Vertices->UV, sizeof(Vertices[0]));

        AfxDeallocate(Vertices);
        break;
    }
#endif
    case 2:
    {
        afxNat bufferSize = spec.vtxCnt * sizeof(granny_pwnt3232_vertex);
        granny_pwnt3232_vertex* Vertices = AfxAllocate(bufferSize, 1, AFX_SIMD_ALIGN, AfxHere());
        GrannyConvertVertexLayouts(spec.vtxCnt, VertexData->VertexType, VertexData->Vertices, GrannyPWNT3232VertexType, Vertices);

        afxV4d hcw;
        AfxResetV4d(hcw);
        AkxFillVertexData(vtd, 0, 0, spec.vtxCnt, &hcw);

        AkxUpdateVertexData(vtd, 0, 0, spec.vtxCnt, Vertices->Position, sizeof(Vertices[0]));
        AkxUpdateVertexData(vtd, 1, 0, spec.vtxCnt, Vertices->BoneIndices, sizeof(Vertices[0]));
        AkxUpdateVertexData(vtd, 2, 0, spec.vtxCnt, Vertices->BoneWeights, sizeof(Vertices[0]));
        AkxUpdateVertexData(vtd, 3, 0, spec.vtxCnt, Vertices->Normal, sizeof(Vertices[0]));
        AkxUpdateVertexData(vtd, 4, 0, spec.vtxCnt, Vertices->UV, sizeof(Vertices[0]));

        AfxDeallocate(Vertices);
        break;
    }
    case 3:
    case 4:
    {
        afxNat bufferSize = spec.vtxCnt * sizeof(granny_pwnt3432_vertex);
        granny_pwnt3432_vertex* Vertices = AfxAllocate(bufferSize, 1, AFX_SIMD_ALIGN, AfxHere());
        GrannyConvertVertexLayouts(spec.vtxCnt, VertexData->VertexType, VertexData->Vertices, GrannyPWNT3432VertexType, Vertices);

        afxV4d hcw;
        AfxResetV4d(hcw);
        AkxFillVertexData(vtd, 0, 0, spec.vtxCnt, &hcw);

        AkxUpdateVertexData(vtd, 0, 0, spec.vtxCnt, Vertices->Position, sizeof(Vertices[0]));
        AkxUpdateVertexData(vtd, 1, 0, spec.vtxCnt, Vertices->BoneIndices, sizeof(Vertices[0]));
        AkxUpdateVertexData(vtd, 2, 0, spec.vtxCnt, Vertices->BoneWeights, sizeof(Vertices[0]));
        AkxUpdateVertexData(vtd, 3, 0, spec.vtxCnt, Vertices->Normal, sizeof(Vertices[0]));
        AkxUpdateVertexData(vtd, 4, 0, spec.vtxCnt, Vertices->UV, sizeof(Vertices[0]));

        AfxDeallocate(Vertices);
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

afxSkeleton Gr2SkeletonToQwadro(afxSimulation sim, afxStringBase strb, granny_skeleton* Skeleton)
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
        AfxAssert(Bone->LocalTransform.Flags == t.flags);
        Bone->LocalTransform.Flags = t.flags;
        AfxResetBoneWithIw(&sklb, i, &id, Bone->ParentIndex, &t, Bone->InverseWorld4x4, Bone->LODError);
    }
    afxSkeleton skl;
    AfxBuildSkeletons(sim, strb, 1, &sklb, &skl);
    AfxEndSkeletonBuilding(&sklb);
    return skl;
}

afxMesh Gr2MeshToQwadro(afxSimulation sim, afxStringBase strb, granny_mesh* Mesh)
{
    afxError err = NIL;

    akxVertexData vtd = Gr2VertexDataToQwadro(sim, strb, Mesh->BoneBindingCount, Mesh->PrimaryVertexData);
    afxMeshTopology msht = Gr2TopologyToQwadro(sim, Mesh->PrimaryTopology);

    afxString pivots[256];

    afxMeshBlueprint blue = { 0 };
    blue.vertices = vtd;
    blue.topology = msht;
    blue.pivots = pivots;
    blue.biasCnt = Mesh->BoneBindingCount;
    blue.mtlCnt = Mesh->MaterialBindingCount;
    blue.strb = strb;
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

afxModel Gr2ModelToQwadro(afxSimulation sim, afxStringBase strb, granny_model* Model)
{
    afxError err = NIL;
    
    afxModelBlueprint blue = { 0 };
    blue.meshes = NIL;
    afxString tid;
    AfxMakeString(&tid, Model->Name, 0);
    AfxMakeString32(&blue.id, &tid);
    AfxSetTransformWithIdentityCheck(&blue.displacement, Model->InitialPlacement.Position, Model->InitialPlacement.Orientation, Model->InitialPlacement.ScaleShear);
    blue.rigCnt = Model->MeshBindingCount;
    blue.skl = Gr2SkeletonToQwadro(sim, strb, Model->Skeleton);
    blue.strb = strb;

    afxModel mdl;
    AfxAssembleModel(sim, 1, &blue, &mdl);

    for (afxNat i = 0; i < blue.rigCnt; i++)
    {
        afxMesh msh = Gr2MeshToQwadro(sim, strb, Model->MeshBindings[i].Mesh);
        AfxRigMeshes(mdl, blue.skl, i, 1, &msh);
    }

    return mdl;
}

afxModel AkxLoadModelsFromGrn3d2(afxSimulation sim, afxUri const *uri, afxNat mdlIdx)
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

    afxStringBase strb;
    AfxAcquireStringCatalogs(1, &strb);

    afxString id;
    afxTransform t;

    granny_model* Model = Info->Models[mdlIdx];

    afxModel mdl = Gr2ModelToQwadro(sim, strb, Model);

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
#if 0
        afxCurve tmp;
        AfxMakeCurveDaKC32f(&tmp, GrannyCurveGetDegree(Curve), GrannyCurveGetDimension(Curve), GrannyCurveGetKnotCount(Curve), d->Knots, d->Controls);
        afxCurveBlueprint cb;
        AfxBeginCurveCopy(&cb, &tmp);
        *c = AfxEndCurve(&cb, sim);
#endif
        afxCurveInfo info;
        AfxResetCurveInfo(&info, afxCurveFormat_DaK32fC32f, GrannyCurveGetDegree(Curve), GrannyCurveGetDimension(Curve), GrannyCurveGetKnotCount(Curve));
        info.knots = d->Knots;
        info.ctrls = d->Controls;
        
        if (AfxAcquireCurves(sim, 1, &info, c))
            AfxThrowError();

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

afxMotion Gr2MotionToQwadro(afxSimulation sim, afxStringBase strb, granny_track_group* TrackGroup)
{
    afxError err = NIL;

    afxString pivots[256];

    akxMotionBlueprint blue = { 0 };
    blue.strb = strb;
    blue.pivotId = pivots;
    blue.pivotLodError = TrackGroup->TransformLODErrors;
    blue.vecCnt = TrackGroup->VectorTrackCount;
    blue.pivotCnt = TrackGroup->TransformTrackCount;
    AfxSetTransformWithIdentityCheck(&blue.displacement, TrackGroup->InitialPlacement.Position, TrackGroup->InitialPlacement.Orientation, TrackGroup->InitialPlacement.ScaleShear);

    afxString tid;
    AfxMakeString(&tid, TrackGroup->Name, 0);
    AfxMakeString32(&blue.id, &tid);

    for (int i = 0; i < TrackGroup->TransformTrackCount; i++)
    {
        AfxMakeString(&pivots[i], TrackGroup->TransformTracks[i].Name, 0);
    }

    afxMotion mot;
    AfxAssembleMotions(sim, 1, &blue, &mot);
    mot->flags = TrackGroup->Flags;
    

    //mot->flags = TrackGroup->Flags;

    for (int i = 0; i < TrackGroup->TransformTrackCount; i++)
    {
        afxMask flags = TrackGroup->TransformTracks[i].Flags;

        akxMotionTransform src = { 0 };

        src.flags = flags;
        Gr2CurveToQwadro(sim, &TrackGroup->TransformTracks[i].OrientationCurve, &src.transmission);
        Gr2CurveToQwadro(sim, &TrackGroup->TransformTracks[i].PositionCurve, &src.translation);
        Gr2CurveToQwadro(sim, &TrackGroup->TransformTracks[i].ScaleShearCurve, &src.transmutation);

        AfxUpdateMotionTransforms(mot, i, 1, &src, 0);
    }
    
    int a = 4;
    return mot;
}

akxAnimation Gr2AnimToQwadro(afxSimulation sim, afxStringBase strb, granny_animation* Anim)
{
    afxError err = NIL;

    akxAnimationBlueprint anib = { 0 };
    anib.motions = NIL;
    anib.motSlotCnt = Anim->TrackGroupCount;
    anib.dur = Anim->Duration;
    anib.oversampling = Anim->Oversampling;
    anib.timeStep = Anim->TimeStep;
    anib.strb = strb;
    anib.strb2 = strb;

    afxUri uri, name;
    AfxMakeUri(&uri, Anim->Name, 0);
    AfxPickUriObject(&uri, &name);
    AfxMakeString32(&anib.id, AfxGetUriString(&name));

    akxAnimation ani;
    AkxAssembleAnimations(sim, 1, &anib, &ani);

    for (afxNat i = 0; i < anib.motSlotCnt; i++)
    {
        afxMotion mot = Gr2MotionToQwadro(sim, strb, Anim->TrackGroups[i]);
        AkxRelinkMotions(ani, i, 1, &mot);
    }

    return ani;
}

akxAnimation AkxLoadAnimationsFromGrn3d2(afxSimulation sim, afxUri const *uri, afxNat animIdx)
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

    afxStringBase strb;
    AfxAcquireStringCatalogs(1, &strb);

    granny_animation* Anim = Info->Animations[animIdx];

    akxAnimation ani = Gr2AnimToQwadro(sim, strb, Anim);

    GrannyFreeFile(GrannyFile);

    return ani;
}
