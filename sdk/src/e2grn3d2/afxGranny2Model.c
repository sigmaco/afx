#define _CRT_SECURE_NO_WARNINGS 1
#define WIN32_LEAN_AND_MEAN 1
#include <Windows.h>
#include "../dep/granny2/granny.h"
#pragma comment(lib, "../../dep/granny2/granny2")

#include "afxGranny2Model.h"
#include "qwadro/sim/modeling/afxMesh.h"
#include "qwadro/sim/awxNode.h"
#include "qwadro/io/afxUri.h"
#include "qwadro/sim/afxSimulation.h"
#include "qwadro/core/afxSystem.h"

void BuildVertexDataFrom(afxSimulation sim, afxMeshBuilder* mshb, granny_vertex_data* gvd, granny_mesh* Mesh)
{
    afxError err = NIL;
    AfxAssertObjects(1, &sim, afxFcc_SIM);

    
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

    granny_skeleton* Skeleton = Model->Skeleton;
    
    afxSkeletonBuilder sklb;
    AfxMakeString(&id, Skeleton->Name, 0);
    AfxBeginSkeletonBuilding(&sklb, Skeleton->BoneCount, &id, 0);

    for (granny_int32 i = 0; i < Skeleton->BoneCount; i++)
    {
        granny_bone* Bone = &Skeleton->Bones[i];
        AfxMakeString(&id, Bone->Name, 0);
        AfxSetTransformWithIdentityCheck(&t, Bone->LocalTransform.Position, Bone->LocalTransform.Orientation, Bone->LocalTransform.ScaleShear);        
        AfxResetBoneWithIw(&sklb, i, &id, Bone->ParentIndex, &t, Bone->InverseWorld4x4);
    }
    afxSkeleton skl;
    AfxBuildSkeletons(sim, strc, 1, &sklb, &skl);
    AfxEndSkeletonBuilding(&sklb);

    afxArray meshBldArray;
    AfxAllocateArray(&meshBldArray, Model->MeshBindingCount, sizeof(afxMeshBuilder), NIL);

    for (granny_int32 i = 0; i < Model->MeshBindingCount; i++)
    {
        granny_model_mesh_binding* Binding = &Model->MeshBindings[i];
        granny_mesh* Mesh = Binding->Mesh;

        afxNat arrelIdx;
        afxMeshBuilder* mshb = AfxInsertArrayUnit(&meshBldArray, &arrelIdx);
        AfxMakeString(&id, Mesh->Name, 0);
        AfxBeginMeshBuilding(mshb, &id, GrannyGetMeshVertexCount(Mesh), GrannyGetMeshTriangleCount(Mesh), GrannyGetMeshTriangleGroupCount(Mesh), Mesh->BoneBindingCount);

        afxBool isRigid = GrannyMeshIsRigid(Mesh);

        afxNat vtxCnt = GrannyGetMeshVertexCount(Mesh);
        //AfxAddVertexBiases(mshb, 1, );

        struct granny_vertex_weight_arrays* vwas = AfxAllocate(NIL, vtxCnt, sizeof(vwas[0]), 0, AfxHint());
        GrannyCopyMeshVertices(Mesh, GrannyVertexWeightArraysType, vwas);

        for (afxNat j = 0; j < vtxCnt; j++)
        {
            vwas[j];
            int a = 1;
        }

        AfxDeallocate(NIL, vwas);

        if (isRigid)
        {
            AfxAddVertexBiases(mshb, 1, (afxNat[]) { 0 }, (afxReal[]) { 1.f });
            
            for (afxNat j = 0; j < vtxCnt; j++)
                AfxUpdateVertices(mshb, 0, j, (afxNat[]) { 0 }, (afxNat[]) { 1 });
        }
        else
        {

        }

        granny_vertex_data* gvd = Mesh->PrimaryVertexData;
        afxByte* gvdBytemap = gvd->Vertices;

        //AfxUpdateVertexPositions(mshb, 0, vtxCnt, gvdBytemap, );
        //AfxUpdateVertexNormals(mshb, 0, vtxCnt, gvdBytemap, );
        //AfxUpdateVertexWraps(mshb, 0, vtxCnt, gvdBytemap, );

        for (granny_int32 j = 0; j < Mesh->PrimaryTopology->GroupCount; j++)
        {
            granny_tri_material_group* grp = GrannyGetMeshTriangleGroups(Mesh);
            grp += j;
            afxNat* tris = GrannyGetMeshIndices(Mesh);
            tris += grp->TriFirst * (3 * sizeof(int));
            AfxEmitTriangles(mshb, grp->MaterialIndex, grp->TriFirst, grp->TriCount, &tris[0]);
        }

        for (granny_int32 j = 0; j < Mesh->BoneBindingCount; j++)
        {
            AfxMakeString(&id, Mesh->BoneBindings[j].BoneName, 0);
            AfxRenameVertexPivots(mshb, 0, j, &id);
        }

        AfxEndMeshBuilding(mshb);

    }
    
    afxArray meshArray;
    AfxAllocateArray(&meshArray, Model->MeshBindingCount, sizeof(afxMesh), NIL);

    AfxMakeString(&id, Model->Name, 0);
    AfxSetTransformWithIdentityCheck(&t, Model->InitialPlacement.Position, Model->InitialPlacement.Orientation, Model->InitialPlacement.ScaleShear);

    afxModelBlueprint mdlb = { 0 };
    AfxMakeFixedString32(&mdlb.id, &id);
    mdlb.skl = skl;
    mdlb.init = t;
    mdlb.mshCnt = Model->MeshBindingCount;
    mdlb.baseMshIdx = 0;

    afxModel mdl;
    AfxAssembleModel(sim, strc, meshArray.data, 1, &mdlb, &mdl);

    AfxDeallocateArray(&meshArray);

    GrannyFreeFile(GrannyFile);
}

afxError AwxLoadAnimationsFromGrn3d2(afxSimulation sim, afxUri const *uri, afxNat baseMdl, afxNat mdlCnt, afxModel models[])
{
    afxError err = NIL;
    AfxAssertObjects(1, &sim, afxFcc_SIM);

    granny_file* GrannyFile = GrannyReadEntireFile(AfxGetUriData(uri, 0));
    granny_file_info* Info = GrannyGetFileInfo(GrannyFile);



    GrannyFreeFile(GrannyFile);
}
