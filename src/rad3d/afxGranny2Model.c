#define _CRT_SECURE_NO_WARNINGS 1
#define WIN32_LEAN_AND_MEAN 1
#include <Windows.h>
#include "../../dep_/granny2/granny.h"
#pragma comment(lib, "../../../dep_/granny2/granny2")

#define _AKX_MOTION_C
#include "afxGranny2Model.h"
#include "../src/afx/sim/dev/AkxSimDevKit.h"
#include "qwadro/afxQwadro.h"

void BuildVertexDataFrom(afxSimulation sim, afxMeshBuilder* mshb, granny_vertex_data* gvd, granny_mesh* Mesh)
{
    afxError err = NIL;
    AfxAssertObjects(1, &sim, afxFcc_SIM);

}

akxVertexData Gr2VertexDataToQwadro(afxSimulation sim, afxNat pivotCnt, granny_vertex_data* VertexData)
{
    afxError err = NIL;

    akxVertexDataSpec spec = { 0 };
    spec.attrCnt = 3;
    spec.baseAttrIdx = 0;
    spec.vtxCnt = VertexData->VertexCount;
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

    spec.attrCnt = attrCnt;

    akxVertexData vtd;
    AkxAcquireVertexDatas(sim, attrSpecs, 1, &spec, &vtd);

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

    afxNat surfToMtlMap[256];
    afxNat baseTrisForSurfMap[256];
    afxNat trisForSurfMap[256];
    akxMeshTopologyBlueprint spec = { 0 };
    spec.src = Topology->IndexCount ? Topology->Indices : Topology->Indices16;
    spec.srcIdxSiz = Topology->IndexCount ? sizeof(afxNat32) : sizeof(afxNat16);
    spec.surfCnt = Topology->GroupCount;
    spec.triCnt = Topology->IndexCount ? Topology->IndexCount / 3 : Topology->Index16Count / 3;
    spec.vtxCnt = 0;
    spec.surfToMtlMap = surfToMtlMap;
    spec.baseTrisForSurfMap = baseTrisForSurfMap;
    spec.trisForSurfMap = trisForSurfMap;

    for (afxNat i = 0; i < spec.surfCnt; i++)
    {
        surfToMtlMap[i] = Topology->Groups[i].MaterialIndex;
        baseTrisForSurfMap[i] = Topology->Groups[i].TriFirst;
        trisForSurfMap[i] = Topology->Groups[i].TriCount;
    }

    afxMeshTopology msht;
    AfxAssembleMeshTopology(sim, 1, &spec, &msht);

    akxIndexedTriangle* tris = AfxGetMeshTriangles(msht, 0);
    afxNat baseTriIdx = 0;

    for (afxNat i = 0; i < spec.surfCnt; i++)
    {
        akxMeshSurface* mshs = AfxGetMeshSurface(msht, i);
        //AfxAssignTypeFcc(mshs, afxFcc_MSHS);
        //mshs->mtlIdx = i;
        //mshs->triCnt = Topology->Groups[i].TriCount;
        //mshs->baseTriIdx = Topology->Groups[i].TriFirst;
    }

    return msht;
}

afxMesh Gr2MeshToQwadro(afxSimulation sim, granny_mesh* Mesh)
{
    afxError err = NIL;

    akxVertexData vtd = Gr2VertexDataToQwadro(sim, Mesh->BoneBindingCount, Mesh->PrimaryVertexData);
    afxMeshTopology msht = Gr2TopologyToQwadro(sim, Mesh->PrimaryTopology);

    afxString pivots[256];

    akxMeshBlueprint blue = { 0 };
    blue.vtd = vtd;
    blue.topology = msht;
    blue.biases = pivots;
    blue.biasCnt = Mesh->BoneBindingCount;
    blue.mtlCnt = Mesh->MaterialBindingCount;
    afxString tid;
    AfxMakeString(&tid, 0, Mesh->Name, 0);
    AfxMakeString32(&blue.id, &tid);
    
    for (afxNat i = 0; i < blue.biasCnt; i++)
    {
        AfxMakeString(&pivots[i], 0, Mesh->BoneBindings[i].BoneName, 0);
    }

    afxMesh msh;
    AfxAssembleMeshes(sim, 1, &blue, &msh);
    AfxReleaseObjects(1, &vtd);
    AfxReleaseObjects(1, &msht);

    return msh;
}

afxSkeleton Gr2SkeletonToQwadro(afxSimulation sim, granny_skeleton* Skeleton)
{
    afxError err = NIL;

    afxString pivots[256];
    afxM4d iw[256];
    afxReal lodErr[256];
    afxTransform local[256];
    afxNat parentIdx[256];

    afxString id;
    akxSkeletonBlueprint sklb = { 0 };
    sklb.lodType = Skeleton->LODType;
    AfxMakeString(&id, 0, Skeleton->Name, 0);
    AfxMakeString32(&sklb.id, &id);
    sklb.jointCnt = Skeleton->BoneCount;
    sklb.local = local;
    sklb.iw = iw;
    sklb.lodErr = lodErr;
    sklb.parentIdx = parentIdx;
    sklb.joints = pivots;

    for (granny_int32 i = 0; i < Skeleton->BoneCount; i++)
    {
        granny_bone* Bone = &Skeleton->Bones[i];

        afxTransform t;
        AfxSetTransformWithIdentityCheck(&t, Bone->LocalTransform.Position, Bone->LocalTransform.Orientation, Bone->LocalTransform.ScaleShear);
        //AfxAssert(Bone->LocalTransform.Flags == t.flags);
        //AfxLogEcho("%x %x", Bone->LocalTransform.Flags, t.flags);
        Bone->LocalTransform.Flags = t.flags;

        AfxM4dCopy(iw[i], Bone->InverseWorld4x4);
        parentIdx[i] = Bone->ParentIndex;
        lodErr[i] = Bone->LODError;
        local[i] = t;
        AfxMakeString(&pivots[i], 0, Bone->Name, 0);
    }
    afxSkeleton skl;
    AfxAssembleSkeletons(sim, 1, &sklb, &skl);

#if 0
    akxSkeletonBlueprint sklb2 = { 0 };
    sklb2.joints = pivots;
    sklb2.jointCnt = Skeleton->BoneCount;
    sklb2.lodType = Skeleton->LODType;
    sklb2.scalable = TRUE;
    sklb2.deformable = TRUE;
    AfxMakeString32(&sklb2.id, &id);
    AfxAssembleSkeletons(sim, strb, 1, &sklb2, &skl);
#endif
    return skl;
}

afxModel Gr2ModelToQwadro(afxSimulation sim, granny_model* Model)
{
    afxError err = NIL;
    
    akxModelBlueprint blue = { 0 };
    blue.meshes = NIL;
    afxString tid;
    AfxMakeString(&tid, 0, Model->Name, 0);
    AfxMakeString32(&blue.id, &tid);
    AfxSetTransformWithIdentityCheck(&blue.displacement, Model->InitialPlacement.Position, Model->InitialPlacement.Orientation, Model->InitialPlacement.ScaleShear);
    blue.rigCnt = Model->MeshBindingCount;
    blue.skl = Gr2SkeletonToQwadro(sim, Model->Skeleton);

    afxModel mdl;
    AfxAssembleModel(sim, 1, &blue, &mdl);
    AfxReleaseObjects(1, &blue.skl);

    for (afxNat i = 0; i < blue.rigCnt; i++)
    {
        afxMesh msh = Gr2MeshToQwadro(sim, Model->MeshBindings[i].Mesh);
        AfxRigMeshes(mdl, blue.skl, i, 1, &msh);
        AfxReleaseObjects(1, &msh);
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

    afxV3d atv;
    afxM3d ltm, iltm;
    GrannyComputeBasisConversion(Info, 1.0, AFX_V3D_ZERO, AFX_V3D_X, AFX_V3D_Y, AFX_V3D_Z, atv, ltm, iltm);
    //GrannyTransformFile(Info, atv, ltm, iltm, 1e-5f, 1e-5f, 3);

    afxString id;
    afxTransform t;

    granny_model* Model = Info->Models[mdlIdx];

    afxModel mdl = Gr2ModelToQwadro(sim, Model);

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
    case akxCurveFormat_DaKeyframes32f:
    {
        granny_curve_data_da_keyframes32f* d = data;
        
        akxCurveInfo info = { 0 };
        AfxResetCurveInfo(&info, akxCurveFormat_DaKeyframes32f, GrannyCurveGetDegree(Curve), GrannyCurveGetDimension(Curve), GrannyCurveGetKnotCount(Curve));
        info.ctrls = d->Controls;

        if (AfxAcquireCurves(sim, 1, &info, c))
            AfxThrowError();

        break;
    }
#endif
    case akxCurveFormat_DaK32fC32f:
    {
        granny_curve_data_da_k32f_c32f* d = data;
        //AfxSetUpCurve(c, akxCurveFormat_DaK32fC32f, d->CurveDataHeader.Degree, 1, d->ControlCount);
#if 0
        afxCurve tmp;
        AfxMakeCurveDaKC32f(&tmp, GrannyCurveGetDegree(Curve), GrannyCurveGetDimension(Curve), GrannyCurveGetKnotCount(Curve), d->Knots, d->Controls);
        akxCurveBlueprint cb;
        AfxBeginCurveCopy(&cb, &tmp);
        *c = AfxEndCurve(&cb, sim);
#endif
        akxCurveInfo info = { 0 };
        AfxResetCurveInfo(&info, akxCurveFormat_DaK32fC32f, GrannyCurveGetDegree(Curve), GrannyCurveGetDimension(Curve), GrannyCurveGetKnotCount(Curve));
        info.knots = d->Knots;
        info.ctrls = d->Controls;
        
        if (AfxAcquireCurves(sim, 1, &info, c))
            AfxThrowError();

        break;
    }
#if 0
    case akxCurveFormat_DaIdentity:
    {
        granny_curve_data_da_identity* d = data;

        akxCurveInfo info = { 0 };
        AfxResetCurveInfo(&info, akxCurveFormat_DaIdentity, GrannyCurveGetDegree(Curve), GrannyCurveGetDimension(Curve), GrannyCurveGetKnotCount(Curve));
        
        if (AfxAcquireCurves(sim, 1, &info, c))
            AfxThrowError();

        break;
    };
    case akxCurveFormat_DaConstant32f:
    {
        granny_curve_data_da_constant32f* d = data;

        akxCurveInfo info = { 0 };
        AfxResetCurveInfo(&info, akxCurveFormat_DaConstant32f, GrannyCurveGetDegree(Curve), GrannyCurveGetDimension(Curve), GrannyCurveGetKnotCount(Curve));
        info.ctrls = d->Controls;

        if (AfxAcquireCurves(sim, 1, &info, c))
            AfxThrowError();

        break;
    }
    case akxCurveFormat_D3Constant32f:
    {
        granny_curve_data_d3_constant32f* d = data;

        akxCurveInfo info = { 0 };
        AfxResetCurveInfo(&info, akxCurveFormat_D3Constant32f, GrannyCurveGetDegree(Curve), GrannyCurveGetDimension(Curve), GrannyCurveGetKnotCount(Curve));
        info.ctrls = d->Controls;

        if (AfxAcquireCurves(sim, 1, &info, c))
            AfxThrowError();

        break;
    }
    case akxCurveFormat_D4Constant32f:
    {
        granny_curve_data_d4_constant32f* d = data;

        akxCurveInfo info = { 0 };
        AfxResetCurveInfo(&info, akxCurveFormat_D4Constant32f, GrannyCurveGetDegree(Curve), GrannyCurveGetDimension(Curve), GrannyCurveGetKnotCount(Curve));
        info.ctrls = d->Controls;

        if (AfxAcquireCurves(sim, 1, &info, c))
            AfxThrowError();

        break;
    }
#endif
    default: AfxThrowError(); break;
    }

}

akxMotion Gr2MotionToQwadro(afxSimulation sim, granny_track_group* TrackGroup)
{
    afxError err = NIL;

    afxString vectors[256];
    afxString pivots[256];

    akxMotionBlueprint blue = { 0 };
    blue.pivots = pivots;
    blue.pivotLodError = TrackGroup->TransformLODErrors;
    blue.incPivotLodError = TrackGroup->TransformLODErrorCount;
    blue.vecCnt = TrackGroup->VectorTrackCount;
    blue.vectors = vectors;
    blue.pivotCnt = TrackGroup->TransformTrackCount;
    AfxSetTransformWithIdentityCheck(&blue.displacement, TrackGroup->InitialPlacement.Position, TrackGroup->InitialPlacement.Orientation, TrackGroup->InitialPlacement.ScaleShear);

    afxString tid;
    AfxMakeString(&tid, 0, TrackGroup->Name, 0);
    AfxMakeString32(&blue.id, &tid);

    for (int i = 0; i < TrackGroup->TransformTrackCount; i++)
    {
        AfxMakeString(&pivots[i], 0, TrackGroup->TransformTracks[i].Name, 0);
    }

    for (int i = 0; i < TrackGroup->VectorTrackCount; i++)
    {
        AfxMakeString(&vectors[i], 0, TrackGroup->VectorTracks[i].Name, 0);
    }

    akxMotion mot;
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

    for (int i = 0; i < TrackGroup->VectorTrackCount; i++)
    {
        akxMotionVector src = { 0 };

        src.seqKey = TrackGroup->VectorTracks[i].TrackKey;
        src.dimension = TrackGroup->VectorTracks[i].Dimension;
        Gr2CurveToQwadro(sim, &TrackGroup->VectorTracks[i].ValueCurve, &src.value);

        AfxUpdateMotionVectors(mot, i, 1, &src, 0);
    }

    int a = 4;
    return mot;
}

akxAnimation Gr2AnimToQwadro(afxSimulation sim, granny_animation* Anim)
{
    afxError err = NIL;

    akxAnimationBlueprint anib = { 0 };
    anib.motions = NIL;
    anib.motSlotCnt = Anim->TrackGroupCount;
    anib.dur = Anim->Duration;
    anib.oversampling = Anim->Oversampling;
    anib.timeStep = Anim->TimeStep;

    afxUri uri, name;
    AfxMakeUri(&uri, 0, Anim->Name, 0);
    AfxClipUriTarget(&name, &uri);
    AfxMakeString32(&anib.id, AfxGetUriString(&name));

    akxAnimation ani;
    AkxAssembleAnimations(sim, 1, &anib, &ani);

    for (afxNat i = 0; i < anib.motSlotCnt; i++)
    {
        akxMotion mot = Gr2MotionToQwadro(sim, Anim->TrackGroups[i]);
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

    afxV3d atv;
    afxM3d ltm, iltm;
    GrannyComputeBasisConversion(Info, 1.0, AFX_V3D_ZERO, AFX_V3D_X, AFX_V3D_Y, AFX_V3D_Z, atv, ltm, iltm);
    //GrannyTransformFile(Info, atv, ltm, iltm, 1e-5f, 1e-5f, 3);

    granny_animation* Anim = Info->Animations[animIdx];

    akxAnimation ani = Gr2AnimToQwadro(sim, Anim);

    GrannyFreeFile(GrannyFile);

    return ani;
}
