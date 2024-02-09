#define _CRT_SECURE_NO_WARNINGS 1
#define WIN32_LEAN_AND_MEAN 1
#include <Windows.h>

#define _AFX_SKELETON_C
#include "qwadro/core/afxSystem.h"
#include "afxWavefrontObject.h"
#include "qwadro/sim/modeling/afxMesh.h"
#include "qwadro/sim/afxMaterial.h"
#include "qwadro/sim/awxAsset.h"
#include "../dep/glus/GL/glus.h"
#include "../dep/glus/GLUS/glus_wavefront.h"
#include "qwadro/io/afxUri.h"
#include "qwadro/draw/afxDrawSystem.h"
#include "qwadro/math/afxVector.h"
#include "qwadro/math/afxQuaternion.h"
#include "qwadro/math/afxMatrix.h"

//afxModule* e2object = NIL;

struct cadbData
{
    afxUri const* name;
    afxNat meshCnt;
    afxMesh* meshes;
    afxUri* meshNames;
    afxModel mdl;
    afxSkeleton skl;
};

_AFXEXPORT void CadbGetInfoObj(void* data2, afxNat *typeCnt, afxNat* resCnt, afxUri* name)
{
    struct cadbData* data = data2;

    afxNat cnt = 0;

    if (data->meshCnt)
        ++cnt;

    if (data->skl)
        ++cnt;

    if (data->mdl)
        ++cnt;

    if (typeCnt)
        *typeCnt = cnt;

    if (resCnt)
        *resCnt = data->meshCnt + 1 + 1;

    if (name && data->name)
        AfxGetUriPath(name, data->name);

}

_AFXEXPORT void CadbGetTypeInfoObj(void* data2, afxNat setIdx, afxFcc* resType, afxNat* resCnt)
{
    struct cadbData* data = data2;

    if (resType)
        *resType = setIdx == 0 ? afxFcc_MSH : setIdx == 1 ? afxFcc_MDL : setIdx == 2 ? afxFcc_SKL : NIL;

    if (resCnt)
        *resCnt = (setIdx == 0) ? data->meshCnt : (setIdx == 1) ? 1 : (setIdx == 2) ? 1 : 0;
}

_AFXEXPORT void* CadbGetResourceInfoObj(void* data2, afxFcc type, afxNat resIdx, afxUri* name)
{
    struct cadbData* data = data2;

    if (name)
    {
        if (type != afxFcc_MSH)
            AfxGetUriName(name, data->name);
        else
            AfxResetUri(name);//AfxGetUriName(name, &data->meshNames[resIdx]);
    }

    if (type == afxFcc_MSH)
        return (data->meshes[resIdx]);

    if (type == afxFcc_MDL)
        return data->mdl;
        
    if (type == afxFcc_SKL)
        return data->skl;

    return NIL;
}

_AFXEXPORT afxError AfxLoadAssetsFromWavefrontObj(afxSimulation sim, afxFlags flags, afxNat cnt, afxUri const file[], awxAsset cad[])
{
    afxError err = AFX_ERR_NONE;

    // a entire file should be an afxModel.
    // each object should be an afxMesh, if none at least one afxMesh must pack all groups.
    // each group should be an afxMeshSurface
    // .obj doesn't contains skeletal info, so must have one single afxMeshPivot bound to all groups.

    AfxAssert(sim);
    afxMmu mmu = AfxGetSimulationMmu(sim);
    afxStringCatalog strc;
    AfxAcquireStringCatalogs(1, &strc);

    for (afxNat i = 0; i < cnt; i++)
    {

        GLUSscene g_wavefrontScene;
#if !0
        afxUri file2;
        afxFixedUri2048 uri2;
        AfxMakeFixedUri2048(&uri2, NIL);
        AfxGetUriPath(&file2, &file[i]);
        AfxResolveUri(afxFileFlag_R, &file2, &uri2.uri);

        if (!_ldrWavefrontLoadScene(AfxGetBufferedUriData(&uri2.uri, 0), &g_wavefrontScene, NIL)) AfxThrowError();
        else
        {
            afxUri mdlNameUri;
            AfxGetUriName(&mdlNameUri, &file2);
            afxString const*mdlName = AfxGetUriString(&mdlNameUri);

            afxNat meshCnt = 0, totalArtCnt = 0;
            GLUSobjectList* objectWalker = g_wavefrontScene.objectList;
            while (objectWalker)
            {
                meshCnt++;
                GLUSgroupList* groupWalker = objectWalker->object.groups;
                while (groupWalker)
                {
                    ++totalArtCnt;
                    groupWalker = groupWalker->next;
                }
                objectWalker = objectWalker->next;
            }

            afxTransform t;
            AfxResetTransform(&t);
            afxSkeletonBuilder sklb = { 0 };
            AfxBeginSkeletonBuilding(&sklb, meshCnt, AfxGetUriString(&file2), NIL);
            AfxResetTransform(&t); // stack corruption after calling BeginSkeletonBuilding

            afxArray builders;
            AfxAllocateArray(&builders, meshCnt, sizeof(afxMeshBuilder), NIL);
            afxNat meshIdx = 0;
            objectWalker = g_wavefrontScene.objectList;
            while (objectWalker)
            {
                afxNat vtxCnt = objectWalker->object.numberVertices;
                afxNat triCnt = 0;
                afxNat surfaceCnt = 0;
                afxV4d* posn = objectWalker->object.vertices;
                afxV3d* nrm = objectWalker->object.normals;
                afxV2d* uv = objectWalker->object.texCoords;

                GLUSgroupList* groupWalker = objectWalker->object.groups;
                while (groupWalker)
                {
                    ++surfaceCnt;
                    triCnt += groupWalker->group.numberIndices;
                    groupWalker = groupWalker->next;
                }
                AfxAssert(triCnt % 3 == 0);
                triCnt /= 3;

                afxNat mshIdx2 = 0;
                afxMeshBuilder *mshb2 = AfxInsertArrayUnits(&builders, 1, &mshIdx2, NIL);
                AfxAssert(mshIdx2 == meshIdx);
                afxString tmp;
                AfxMakeString(&tmp, objectWalker->object.name, 0);
                AfxBeginMeshBuilding(mshb2, &tmp, vtxCnt, triCnt, surfaceCnt, 1);
                
                AfxResetBone(&sklb, meshIdx, &tmp, AFX_INVALID_INDEX, &t, &t);
                
                AfxRenameVertexPivots(mshb2, 0, 1, &tmp);

                afxNat pivotIdx = 0;
                AfxAssert(pivotIdx != AFX_INVALID_INDEX);
                afxNat biasCnt = 1;
                afxNat baseBiasIdx = AfxAddVertexBiases(mshb2, biasCnt, (afxNat const[]) { pivotIdx }, (afxReal const[]) { 1.f });
                AfxAssert(baseBiasIdx == 0);
                afxNat baseVtxIdx = 0;
                AfxUpdateVertices(mshb2, baseVtxIdx, vtxCnt, NIL, NIL);
                AfxAssert(baseVtxIdx != AFX_INVALID_INDEX);
                AfxUpdateVertexPositions4(mshb2, baseVtxIdx, vtxCnt, posn, sizeof(posn[0]));

                if (nrm)
                    AfxUpdateVertexNormals(mshb2, baseVtxIdx, vtxCnt, nrm, sizeof(nrm[0]));

                if (uv)
                    AfxUpdateVertexWraps(mshb2, baseVtxIdx, vtxCnt, uv, sizeof(uv[0]));

                afxNat surfIdx = 0;
                afxNat addedTris = 0;
                groupWalker = objectWalker->object.groups;
                while (groupWalker)
                {
                    afxString tmp;
                    AfxMakeString(&tmp, groupWalker->group.materialName, 0);
                    //AfxAssert(pivotIdx == artIdx);
                    
                    afxNat origIdxCnt = groupWalker->group.numberIndices;
                    AfxAssert(origIdxCnt % 3 == 0);
                    afxIndexedTriangle tri;

                    for (afxNat j = 0; j < origIdxCnt / 3; j++)
                    {
                        tri[0] = groupWalker->group.indices[j * 3 + 0];
                        tri[1] = groupWalker->group.indices[j * 3 + 1];
                        tri[2] = groupWalker->group.indices[j * 3 + 2];

                        AfxEmitTriangles(mshb2, surfIdx, addedTris, 1, &tri);
                        ++addedTris;
                    }
                    //AfxEmitTriangles(mshb2, surfIdx, origIdxCnt / 3, groupWalker->group.indices);
                    ++surfIdx;
                    groupWalker = groupWalker->next;
                }
                ++meshIdx;
                objectWalker = objectWalker->next;
            }



#if 0
            afxArray meshRes = AfxAllocateArray(meshCnt, sizeof(&objectWalker->object), NIL);
            afxArray meshNames = AfxAllocateArray(meshCnt, sizeof(afxUri), NIL);
            AfxReserveArraySpace(&meshRes, meshCnt);
            AfxReserveArraySpace(&meshNames, meshCnt);

            objectWalker = g_wavefrontScene.objectList;
            while (objectWalker)
            {
                afxNat idx;
                GLUSwavefront*ptr = &objectWalker->object;
                AfxInsertArrayUnits(&meshRes, 1, &idx, &ptr);
                AfxMakeUri(AfxInsertArrayUnit(&meshNames, &idx), objectWalker->object.name, 0);
                AfxAdvertise("%.*s", AfxPushString(AfxGetUriString(AfxGetArrayUnit(&meshNames, idx))));
                objectWalker = objectWalker->next;
            }
#endif
            //afxArray meshes = AfxAllocateArray(meshCnt, sizeof(afxMesh), NIL);
            //
            
            afxArray meshes;
            AfxAllocateArray(&meshes, meshCnt, sizeof(afxMesh), NIL);
            AfxReserveArraySpace(&meshes, meshCnt);

            if (AfxBuildMeshes(sim, strc, meshCnt, builders.data, meshes.data))
                AfxThrowError();

            //AfxRenormalizeMeshes(meshCnt, meshes.data);
            AfxAssertObjects(meshCnt, meshes.data, afxFcc_MSH);
            //AfxDeallocateArray(&meshRes);
            meshes.cnt = meshCnt;

            // build the skeleton

            afxSkeleton skl = NIL;
            
            if (AfxBuildSkeletons(sim, strc, 1, &sklb, &skl))
                AfxThrowError();

            AfxAssertObjects(1, &skl, afxFcc_SKL);
            AfxEndSkeletonBuilding(&sklb);

            // assemble all things into a model

            afxModel mdl;
            afxModelBlueprint mdlb = { 0 };
            AfxResetTransform(&mdlb.init);
            mdlb.skl = skl;
            mdlb.mshCnt = meshCnt;
            mdlb.baseMshIdx = 0;
            AfxMakeFixedString32(&mdlb.id, mdlName);
            AfxAssembleModel(sim, strc, meshes.data, 1, &mdlb, &mdl);
            AfxAssertObjects(1, &mdl, afxFcc_MDL);

            awxAssetBuilder cadb = { 0 };
            cadb.GetInfo = CadbGetInfoObj;
            cadb.GetResourceInfo = CadbGetResourceInfoObj;
            cadb.GetTypeInfo = CadbGetTypeInfoObj;
            
            struct cadbData cadbD = { 0 };
            cadbD.name = &file2;
            cadbD.meshCnt = meshCnt;
            cadbD.meshes = meshes.data;
            cadbD.meshNames = NIL;// meshNames.data;
            cadbD.mdl = mdl;
            cadbD.skl = skl;

            if (AfxBuildAssets(sim, &cadb, 1, (void*[]) { &cadbD }, cad))
                AfxThrowError();

            AfxReleaseObjects(meshCnt, meshes.data);
            AfxDeallocateArray(&meshes);
            //AfxDeallocateArray(&meshNames);
            AfxReleaseObjects(1, (void*[]) { skl });
            AfxReleaseObjects(1, (void*[]) { mdl });

            _ldrWavefrontDestroyScene(&(g_wavefrontScene));
        }
#endif//0
    }
    return err;
}
