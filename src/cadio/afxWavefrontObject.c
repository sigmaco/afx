#define _CRT_SECURE_NO_WARNINGS 1
#define WIN32_LEAN_AND_MEAN 1
#include <Windows.h>

#define _AFX_SKELETON_C
#include "qwadro/afxQwadro.h"
#include "afxWavefrontObject.h"
#include "../../dep_/glus/GL/glus.h"
#include "../../dep_/glus/GLUS/glus_wavefront.h"

//afxModule* e2object = NIL;

DLLEXPORT akxVertexData ObjVertexDataToQwadro(afxSimulation sim, afxStringBase strb, afxNat pivotCnt, GLUSwavefront const* object)
{
    afxError err = NIL;

    akxVertexDataSpec spec = { 0 };
    spec.attrCnt = 3;
    spec.baseAttrIdx = 0;
    spec.vtxCnt = object->numberVertices;
    spec.biasCnt = 0;
    spec.biasSrc = NIL;
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
    attrSpecs[attrCnt].src = object->vertices;
    attrSpecs[attrCnt].srcStride = sizeof(afxV4d);
    ++attrCnt;

    attrSpecs[attrCnt].id = "nrm";
    attrSpecs[attrCnt].flags = akxVertexFlag_LINEAR;
    attrSpecs[attrCnt].fmt = afxVertexFormat_V3D;
    attrSpecs[attrCnt].usage = akxVertexUsage_NRM | akxVertexUsage_TANGENT;
    attrSpecs[attrCnt].src = object->normals;
    attrSpecs[attrCnt].srcStride = sizeof(afxV3d);
    ++attrCnt;

    attrSpecs[attrCnt].id = "uv";
    attrSpecs[attrCnt].flags = NIL;
    attrSpecs[attrCnt].fmt = afxVertexFormat_V2D;
    attrSpecs[attrCnt].usage = akxVertexUsage_UV;
    attrSpecs[attrCnt].src = object->texCoords;
    attrSpecs[attrCnt].srcStride = sizeof(afxV2d);
    ++attrCnt;

    akxVertexData vtd;
    AkxAcquireVertexDatas(sim, attrSpecs, 1, &spec, &vtd);

    return vtd;
}

DLLEXPORT afxMeshTopology ObjTopologyToQwadro(afxSimulation sim, GLUSwavefront const* object)
{
    afxError err = NIL;

    afxNat trisForSurfMap[256];
    afxNat surfToMtlMap[256];

    afxNat secCnt = 0, idxCnt = 0;
    GLUSgroupList* groupWalker = object->groups;
    while (groupWalker)
    {
        trisForSurfMap[secCnt] = groupWalker->group.numberIndices / 3;
        idxCnt += groupWalker->group.numberIndices;
        
        surfToMtlMap[secCnt] = AFX_INVALID_INDEX;

        GLUSmaterialList* mtlWalker = object->materials;

        afxNat mtlIdx = 0;
        while (mtlWalker)
        {
            if (groupWalker->group.material == &mtlWalker->material)
            {
                surfToMtlMap[secCnt] = mtlIdx;
            }
            ++mtlIdx;
            mtlWalker = mtlWalker->next;
        }

        ++secCnt;
        groupWalker = groupWalker->next;
    }

    akxMeshTopologyBlueprint spec = { 0 };
    spec.surfCnt = secCnt;
    spec.triCnt = idxCnt / 3;
    spec.vtxCnt = 0;
    spec.trisForSurfMap = trisForSurfMap;
    spec.surfToMtlMap = surfToMtlMap;

    // TODO default coverage to 0 to force single or INVALID to assign one for each one.

    afxMeshTopology msht;
    AfxAssembleMeshTopology(sim, 1, &spec, &msht);

    akxIndexedTriangle* tris = AfxGetMeshTriangles(msht, 0);
    afxNat baseTriIdx = 0;

    afxNat i = 0;
    groupWalker = object->groups;
    while (groupWalker)
    {
        akxMeshSurface* mshs = AfxGetMeshSurface(msht, i);
        //mshs->mtlIdx = i;
        //mshs->triCnt = groupWalker->group.numberIndices / 3;
        //mshs->baseTriIdx = baseTriIdx;
        ++i;

        AfxUpdateMeshIndices(msht, mshs->baseTriIdx, mshs->triCnt, groupWalker->group.indices, sizeof(groupWalker->group.indices[0]));
        baseTriIdx += mshs->triCnt;

        groupWalker = groupWalker->next;
    }
    AfxAssert(i == secCnt);

    return msht;
}

DLLEXPORT afxMesh ObjMeshToQwadro(afxSimulation sim, afxStringBase strb, GLUSwavefront const* object)
{
    afxError err = NIL;

    akxVertexData vtd = ObjVertexDataToQwadro(sim, strb, 0, object);
    afxMeshTopology msht = ObjTopologyToQwadro(sim, object);

    afxString pivots[256];

    akxMeshBlueprint blue = { 0 };
    blue.vtd = vtd;
    blue.topology = msht;
    blue.biases = pivots;
    blue.biasCnt = 1;
    blue.mtlCnt = AfxCountMeshSurfaces(msht);
    afxString tid;
    AfxMakeString(&tid, 0, object->name, 0);
    AfxMakeString32(&blue.id, &tid);

    for (afxNat i = 0; i < blue.biasCnt; i++)
    {
        AfxMakeString(&pivots[i], 0, object->name, 0);
    }

    afxMesh msh;
    AfxAssembleMeshes(sim, 1, &blue, &msh);
    AfxReleaseObjects(1, &msht);
    AfxReleaseObjects(1, &vtd);

    return msh;
}

DLLEXPORT afxSkeleton ObjSkeletonToQwadro(afxSimulation sim, afxStringBase strb, afxString const* name, GLUSscene* g_wavefrontScene)
{
    afxError err = AFX_ERR_NONE;

    afxString pivots[256];
    afxM4d iw[256];
    afxReal lodErr[256];
    afxTransform local[256];
    afxNat parentIdx[256];

    akxSkeletonBlueprint sklb = { 0 };
    AfxMakeString32(&sklb.id, name);
    sklb.jointCnt = 0;
    //sklb.local = local;
    //sklb.iw = iw;
    //sklb.lodErr = lodErr;
    //sklb.parentIdx = parentIdx;
    sklb.joints = pivots;
    
    afxNat artCnt = 0;
    GLUSobjectList* objectWalker = g_wavefrontScene->objectList;
    while (objectWalker)
    {
        AfxMakeString(&pivots[artCnt], 0, objectWalker->object.name, 0);
        artCnt++;
        objectWalker = objectWalker->next;
    }

    sklb.jointCnt = artCnt;


    afxSkeleton skl;
    AfxAssembleSkeletons(sim, 1, &sklb, &skl);
    return skl;
}

DLLEXPORT afxModel AfxLoadAssetsFromWavefrontObj(afxSimulation sim, afxUri const* file)
{
    afxError err = AFX_ERR_NONE;

    // a entire file should be an afxModel.
    // each object should be an afxMesh, if none at least one afxMesh must pack all groups.
    // each group should be an akxMeshSurface
    // .obj doesn't contains skeletal info, so must have one single akxMeshBias bound to all groups.

    AfxAssert(sim);
    afxMmu mmu = AfxGetSimulationMmu(sim);
    afxStringBase strb;
    AfxAcquireStringCatalogs(1, &strb);

    GLUSscene g_wavefrontScene;
#if !0
    afxUri file2;
    afxUri2048 uri2;
    AfxMakeUri2048(&uri2, NIL);
    AfxClipUriPath(&file2, file);
    AfxResolveUri(afxFileFlag_R, &file2, &uri2.uri);

    if (!_ldrWavefrontLoadScene(AfxGetUriData(&uri2.uri, 0), &g_wavefrontScene, NIL)) AfxThrowError();
    else
    {
        afxUri mdlNameUri;
        AfxClipUriTarget(&mdlNameUri, &file2);
        afxString const*mdlName = AfxGetUriString(&mdlNameUri);

        afxNat meshCnt = 0;
        GLUSobjectList* objectWalker = g_wavefrontScene.objectList;
        while (objectWalker)
        {
            meshCnt++;
            objectWalker = objectWalker->next;
        }

        akxModelBlueprint blue = { 0 };
        blue.meshes = NIL;
        AfxMakeString32(&blue.id, mdlName);
        AfxResetTransform(&blue.displacement);
        blue.rigCnt = meshCnt;
        blue.skl = ObjSkeletonToQwadro(sim, strb, mdlName, &g_wavefrontScene);

        afxModel mdl;
        AfxAssembleModel(sim, 1, &blue, &mdl);
        AfxReleaseObjects(1, &blue.skl);

        afxNat i = 0;
        objectWalker = g_wavefrontScene.objectList;
        while (objectWalker)
        {
            afxMesh msh = ObjMeshToQwadro(sim, strb, &objectWalker->object);
            AfxRigMeshes(mdl, blue.skl, i++, 1, &msh);
            AfxReleaseObjects(1, &msh);
            objectWalker = objectWalker->next;
        }
        AfxAssert(i == meshCnt);

        _ldrWavefrontDestroyScene(&(g_wavefrontScene));
        return mdl;
    }
#endif//0
    return NIL;
}
