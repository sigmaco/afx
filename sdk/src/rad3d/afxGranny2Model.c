#define _CRT_SECURE_NO_WARNINGS 1
#define WIN32_LEAN_AND_MEAN 1
#include <Windows.h>
#include "../dep/granny2/granny.h"
#pragma comment(lib, "../../dep/granny2/granny2")

#include "afxGranny2Model.h"
#include "qwadro/sim/modeling/afxMesh.h"
#include "qwadro/sim/modeling/awxVertexData.h"
#include "qwadro/sim/awxNode.h"
#include "qwadro/io/afxUri.h"
#include "qwadro/sim/afxSimulation.h"
#include "qwadro/core/afxSystem.h"
#include "qwadro/core/afxIndexedString.h"

void BuildVertexDataFrom(afxSimulation sim, afxMeshBuilder* mshb, granny_vertex_data* gvd, granny_mesh* Mesh)
{
    afxError err = NIL;
    AfxAssertObjects(1, &sim, afxFcc_SIM);

}

awxVertexData Gr2VertexDataToQwadro(afxSimulation sim, afxStringCatalog strc, afxBool rigid, granny_vertex_data* VertexData)
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

    attrSpecs[0].id = "pos";
    attrSpecs[0].flags = awxVertexFlag_AFFINE | awxVertexFlag_LINEAR;
    attrSpecs[0].fmt = afxVertexFormat_V3D;
    attrSpecs[0].usage = awxVertexUsage_POS | awxVertexUsage_POSITIONAL;
    attrSpecs[0].src = NIL;
    attrSpecs[0].srcStride = 0;

    attrSpecs[1].id = "nrm";
    attrSpecs[1].flags = awxVertexFlag_LINEAR;
    attrSpecs[1].fmt = afxVertexFormat_V3D;
    attrSpecs[1].usage = awxVertexUsage_NRM | awxVertexUsage_TANGENT;
    attrSpecs[1].src = NIL;
    attrSpecs[1].srcStride = 0;

    attrSpecs[2].id = "uv";
    attrSpecs[2].flags = NIL;
    attrSpecs[2].fmt = afxVertexFormat_V2D;
    attrSpecs[2].usage = awxVertexUsage_UV;
    attrSpecs[2].src = NIL;
    attrSpecs[2].srcStride = 0;

    if (!rigid)
    {
        spec.biasCnt = spec.vtxCnt * 4;
        spec.biasSrc = NIL;
    }

    awxVertexData vtd;
    AwxAcquireVertexDatas(sim, strc, attrSpecs, 1, &spec, &vtd);

    if (rigid)
    {
        afxNat bufferSize = spec.vtxCnt * sizeof(granny_pnt332_vertex);
        granny_pnt332_vertex* Vertices = AfxAllocate(NIL, bufferSize, 1, 0, AfxHint());
        GrannyConvertVertexLayouts(spec.vtxCnt, VertexData->VertexType, VertexData->Vertices, GrannyPNT332VertexType, Vertices);

        afxV4d hcw;
        AfxResetV4d(hcw);
        AwxUpdateVertexData(vtd, 0, 0, spec.vtxCnt, &hcw, 0);

        AwxUpdateVertexData(vtd, 0, 0, spec.vtxCnt, VertexData->Vertices, sizeof(afxV3d));
        AwxUpdateVertexData(vtd, 1, 0, spec.vtxCnt, Vertices->Normal, sizeof(afxV3d));
        AwxUpdateVertexData(vtd, 2, 0, spec.vtxCnt, Vertices->UV, sizeof(afxV2d));

        AwxUpdateVertices(vtd, 0, spec.vtxCnt, &rigidVtx, 0);

        AfxDeallocate(NIL, Vertices);
    }
    else
    {
        afxNat bufferSize = spec.vtxCnt * sizeof(granny_pwnt3432_vertex);
        granny_pwnt3432_vertex* Vertices = AfxAllocate(NIL, bufferSize, 1, 0, AfxHint());
        GrannyConvertVertexLayouts(spec.vtxCnt, VertexData->VertexType, VertexData->Vertices, GrannyPWNT3432VertexType, Vertices);

        afxV4d hcw;
        AfxResetV4d(hcw);
        AwxUpdateVertexData(vtd, 0, 0, spec.vtxCnt, &hcw, 0);

        AwxUpdateVertexData(vtd, 0, 0, spec.vtxCnt, Vertices->Position, sizeof(afxV3d));
        AwxUpdateVertexData(vtd, 1, 0, spec.vtxCnt, Vertices->Normal, sizeof(afxV3d));
        AwxUpdateVertexData(vtd, 2, 0, spec.vtxCnt, Vertices->UV, sizeof(afxV2d));

        afxNat baseBias = 0;
        afxNat baseVtx = 0;

        for (afxNat j = 0; j < spec.vtxCnt; j++)
        {
            afxVertexBias bias[4];

            for (afxNat i = 0; i < 4; i++)
            {
                bias[0].pivotIdx = Vertices->BoneIndices[0];
                bias[0].weight = Vertices->BoneWeights[0];
                bias[1].pivotIdx = Vertices->BoneIndices[1];
                bias[1].weight = Vertices->BoneWeights[1];
                bias[2].pivotIdx = Vertices->BoneIndices[2];
                bias[2].weight = Vertices->BoneWeights[2];
                bias[3].pivotIdx = Vertices->BoneIndices[3];
                bias[3].weight = Vertices->BoneWeights[3];
            }

            afxVertex vtx;
            vtx.baseBiasIdx = baseBias;
            vtx.biasCnt = 4;

            AwxUpdateVertexBiases(vtd, baseBias, 4, bias, sizeof(bias[0]));
            AwxUpdateVertices(vtd, j, 1, &vtx, sizeof(vtx));
            baseBias += 4;
        }

        Vertices->BoneIndices;
        Vertices->BoneWeights;

        AfxDeallocate(NIL, Vertices);
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
        AfxAssignTypeFcc(mshs, afxFcc_MSHS);
        mshs->mtlIdx = i;
        mshs->triCnt = Topology->Groups[i].TriCount;
        mshs->baseTriIdx = Topology->Groups[i].TriFirst;
    }

    return msht;
}

afxSkeleton Gr2SkeletonToQwadro(afxSimulation sim, afxStringCatalog strc, granny_skeleton* Skeleton)
{
    afxError err = NIL;

    afxString id;
    afxSkeletonBuilder sklb;
    AfxMakeString(&id, Skeleton->Name, 0);
    AfxBeginSkeletonBuilding(&sklb, Skeleton->BoneCount, &id, 0);

    for (granny_int32 i = 0; i < Skeleton->BoneCount; i++)
    {
        granny_bone* Bone = &Skeleton->Bones[i];
        AfxMakeString(&id, Bone->Name, 0);
        afxTransform t;
        AfxSetTransformWithIdentityCheck(&t, Bone->LocalTransform.Position, Bone->LocalTransform.Orientation, Bone->LocalTransform.ScaleShear);
        AfxResetBoneWithIw(&sklb, i, &id, Bone->ParentIndex, &t, Bone->InverseWorld4x4);
    }
    afxSkeleton skl;
    AfxBuildSkeletons(sim, strc, 1, &sklb, &skl);
    AfxEndSkeletonBuilding(&sklb);
    return skl;
}

afxMesh Gr2MeshToQwadro(afxSimulation sim, afxStringCatalog strc, granny_mesh* Mesh)
{
    afxError err = NIL;

    awxVertexData vtd = Gr2VertexDataToQwadro(sim, strc, GrannyMeshIsRigid(Mesh), Mesh->PrimaryVertexData);
    afxMeshTopology msht = Gr2TopologyToQwadro(sim, Mesh->PrimaryTopology);

    afxString pivots[265];

    afxMeshBlueprint blue = { 0 };
    blue.vertices = vtd;
    blue.topology = msht;
    blue.pivots = pivots;
    blue.pivotCnt = Mesh->BoneBindingCount;
    blue.mtlCnt = Mesh->MaterialBindingCount;
    blue.strc = strc;
    afxString tid;
    AfxMakeString(&tid, Mesh->Name, 0);
    AfxMakeFixedString32(&blue.id, &tid);
    
    for (afxNat i = 0; i < blue.pivotCnt; i++)
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
    AfxMakeFixedString32(&blue.id, &tid);
    AfxSetTransformWithIdentityCheck(&blue.init, Model->InitialPlacement.Position, Model->InitialPlacement.Orientation, Model->InitialPlacement.ScaleShear);
    blue.mshCnt = Model->MeshBindingCount;
    blue.skl = Gr2SkeletonToQwadro(sim, strc, Model->Skeleton);
    blue.strc = strc;

    afxModel mdl;
    AfxAssembleModel(sim, 1, &blue, &mdl);

    for (afxNat i = 0; i < blue.mshCnt; i++)
    {
        afxMesh msh = Gr2MeshToQwadro(sim, strc, Model->MeshBindings[i].Mesh);
        AfxRelinkMeshes(mdl, blue.skl, i, 1, &msh);
    }

    return mdl;
}

afxModel AwxLoadModelsFromGrn3d2(afxSimulation sim, afxUri const *uri, afxNat mdlIdx)
{
    afxError err = NIL;
    AfxAssertObjects(1, &sim, afxFcc_SIM);

    afxFixedUri2048 uriOut;
    AfxMakeFixedUri2048(&uriOut, NIL);
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

afxError AwxLoadAnimationsFromGrn3d2(afxSimulation sim, afxUri const *uri, afxNat baseMdl, afxNat mdlCnt, afxModel models[])
{
    afxError err = NIL;
    AfxAssertObjects(1, &sim, afxFcc_SIM);

    granny_file* GrannyFile = GrannyReadEntireFile(AfxGetUriData(uri, 0));
    granny_file_info* Info = GrannyGetFileInfo(GrannyFile);



    GrannyFreeFile(GrannyFile);
}
