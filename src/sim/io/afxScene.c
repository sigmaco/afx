/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *        Q W A D R O   V I D E O   G R A P H I C S   I N F R A S T R U C T U R E
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

#define _AFX_DRAW_C
#define _AVX_SCENE_C
#include "../impl/asxImplementation.h"
#include "../../draw/impl/avxImplementation.h"
#include "qwadro/inc/sim/io/afxScene.h"

/*
    An Octree is a tree data structure that is commonly used to partition a three-dimensional space by recursively 
    subdividing it into eight smaller regions (octants).

    An octant is one of the eight divisions of a three-dimensional coordinate system defined by the signs of the coordinates.
*/

AFX_DEFINE_STRUCT(afxOctant)
{
    afxBox      bounds;
    afxUnit     childIdx[8]; // child octant's index
    afxUnit     parentIdx; // parent octant's index
    afxChain    contents;
};

AFX_DEFINE_STRUCT(afxOctree)
{
    afxOctant   root; // root octant --- always exist
    afxPool     octants;
    afxBox      (*calc)(void* content);
};

ASX void AfxDeployOctree(afxOctree* tree, afxBox* bounds)
{
    tree->root.bounds = *bounds;
    tree->root.parentIdx = AFX_INVALID_INDEX;

    for (afxUnit i = 0; i < /* it is always a octree*/ 8; i++)
        tree->root.childIdx[i] = AFX_INVALID_INDEX;

    AfxDeployChain(&tree->root.contents, tree);
    AfxDeployPool(&tree->octants, sizeof(afxOctant), /* it is always a octree*/ 8, AFX_SIMD_ALIGNMENT);
}

ASX afxError AfxSubdivideOctree(afxOctree* tree, afxUnit octantIdx)
// Subdivide the node into 8 octants
{
    afxError err = NIL;

    afxOctant* node = NIL;
    AfxGetPoolUnit(&tree->octants, octantIdx, (void**)&node);
    AFX_ASSERT(node);

    if (node->childIdx[0] != AFX_INVALID_INDEX)
    {
        AfxThrowError();
        return err;
    }

    afxReal midX = (node->bounds.inf[0] + node->bounds.sup[0]) / 2;
    afxReal midY = (node->bounds.inf[1] + node->bounds.sup[1]) / 2;
    afxReal midZ = (node->bounds.inf[2] + node->bounds.sup[2]) / 2;

    afxBox bounds[8];
    for (afxUnit i = 0; i < 8; i++)
    {
        if (i & 1) bounds[i].inf[0] = midX;
        else bounds[i].sup[0] = midX;
        if (i & 2) bounds[i].inf[1] = midY;
        else bounds[i].sup[1] = midY;
        if (i & 4) bounds[i].inf[2] = midZ;
        else bounds[i].sup[2] = midZ;
    }

    // Create the 8 new children with updated bounding boxes
    for (afxUnit i = 0; i < 8; i++)
    {
        afxSize childIdx;
        afxOctant* child = NIL;
        if (!(child = AfxPushPoolUnit(&tree->octants, &childIdx)))
        {
            AfxThrowError();

            for (afxUnit j = i; j-- > 0;)
            {
                childIdx = j;
                AfxPopPoolUnit(&tree->octants, &childIdx);
                node->childIdx[j] = AFX_INVALID_INDEX;
            }
            break;
        }

        child->bounds = bounds[i];

        child->parentIdx = octantIdx;
        for (afxUnit j = 0; j < 8; j++)
            child->childIdx[j] = AFX_INVALID_INDEX;

        AfxDeployChain(&child->contents, child);

        node->childIdx[i] = childIdx;
    }
    return err;
}

// Function to insert an AABB into the Octree
void AfxOctreeInsertAabbs(afxOctree* tree, afxUnit octantIdx, afxBox* box)
{
    afxError err = NIL;

    // expand the tree if it does not fit.
    AfxAabbAbsorb(&tree->root.bounds, 1, box);

    afxOctant* node = NIL;
    if (octantIdx == AFX_INVALID_INDEX) node = &tree->root;
    else
    {
        AfxGetPoolUnit(&tree->octants, octantIdx, (void**)&node);
        AFX_ASSERT(node);
    }

    // Check if the AABB intersects with the node's bounding box. No intersection, don't insert.
    if (!AfxAabbContainsAabbs(&node->bounds, 1, box))
        return;

    // If the node doesn't have children, check if it's time to subdivide
    if (node->childIdx[0] == AFX_INVALID_INDEX)
        if (AfxSubdivideOctree(tree, octantIdx))
            AfxThrowError();

    // Insert the AABB into the children or store it in the node itself
    afxBool inserted = FALSE;
    for (afxUnit i = 0; i < 8; i++)
    {
        afxOctant* child = NIL;
        afxSize childIdx = node->childIdx[i];
        if (!AfxGetPoolUnit(&tree->octants, childIdx, (void**)&child))
        {
            AfxThrowError();
            break;
        }
        
        if (AfxAabbContainsAabbs(&child->bounds, 1, box))
        {
            AfxOctreeInsertAabbs(tree, childIdx, box);  // Insert into the appropriate child
            inserted = TRUE;
        }
    }

    // If the AABB is completely inside the node (and doesn't span children),
    // you could store it at the node's level (depending on your use case).
    // For now, we don't store data at the node level in this example.

    AFX_ASSERT(inserted);
}

void AfxOctreeInsertAtv3d(afxOctree* tree, afxUnit octantIdx, afxV3d p)
// Insert a point into the Octree
{
    afxError err = NIL;

    afxOctant* node = NIL;
    if (octantIdx == AFX_INVALID_INDEX) node = &tree->root;
    else
    {
        AfxGetPoolUnit(&tree->octants, octantIdx, (void**)&node);
        AFX_ASSERT(node);
    }

    // If the point is outside the bounding box, do not insert
    if (!AfxAabbContainsAtv3d(&node->bounds, 1, p)) return;

    // If the node doesn't have children, check if it's time to subdivide
    if (node->childIdx[0] == AFX_INVALID_INDEX)
        if (AfxSubdivideOctree(tree, octantIdx))
            AfxThrowError();

    // Insert the AABB into the children or store it in the node itself
    afxBool inserted = FALSE;
    for (afxUnit i = 0; i < 8; i++)
    {
        afxOctant* child = NIL;
        afxSize childIdx = node->childIdx[i];
        if (!AfxGetPoolUnit(&tree->octants, childIdx, (void**)&child))
        {
            AfxThrowError();
            break;
        }

        if (AfxAabbContainsAtv3d(&child->bounds, 1, p))
        {
            AfxOctreeInsertAabbs(tree, childIdx, p);  // Insert into the appropriate child
            inserted = TRUE;
            break;
        }
    }

    AFX_ASSERT(inserted);
}



AFX_OBJECT(afxScene)
{
    afxDrawInput    din;
    avxSkyType      skyType;
    afxMesh         skyMsh;
    afxColor        apexCol;
    afxColor        centreCol;
    struct
    {
        afxBuffer       cube;
        afxRaster       cubemap;
        afxReal         cubemapColorIntensity;
        afxV3d          fogColor;
        afxReal         fogUpper;
        afxReal         fogLower;
        afxColor        emissiveColor;
        afxColor        ambientColor;
        afxReal         currRot;
        afxReal         rotSpeed;
        afxV3d          rotPivot;
        afxQuat         rotQuat;
        afxM4d          rotMtx;
        afxDrawTechnique skyDtec;
        avxVertexDecl  skyVin;
        avxSampler      smp;
    }               sky;
};

afxReal const skyboxVertices[] =
{
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f
};

_ASX void AfxStepSky(afxScene scn, afxReal dt)
{
    scn->sky.currRot += scn->sky.rotSpeed * dt;
    AfxQuatRotationFromAxis(scn->sky.rotQuat, scn->sky.rotPivot, scn->sky.currRot);
    AfxM4dRotationFromQuat(scn->sky.rotMtx, scn->sky.rotQuat);
}

_ASX afxError AfxDrawSky(afxDrawContext dctx, afxScene scn)
{
    afxError err = AFX_ERR_NONE;

    if (scn->skyType == avxSkyType_BOX)
    {
        AvxCmdApplyDrawTechnique(dctx, scn->sky.skyDtec, 0, scn->sky.skyVin, NIL);

        AvxCmdBindSamplers(dctx, 0, 1, 1, &scn->sky.smp);
        AvxCmdBindRasters(dctx, 0, 1, 1, &scn->sky.cubemap);

        AvxCmdBindVertexBuffers(dctx, 0, 1, (avxBufferedStream[]) { {.buf = scn->sky.cube, .stride = sizeof(afxV3d) } });
        //AvxCmdResetVertexStreams(dctx, 1, NIL, (afxUnit32[]) { sizeof(afxV3d) }, NIL);
        //AvxCmdResetVertexAttributes(dctx, 1, NIL, (afxVertexFormat[]) { afxVertexFormat_V3D }, NIL, NIL);
        AvxCmdDraw(dctx, 36, 1, 0, 0);
    }
    else
    {

    }
    return err;
}

_ASX afxError AfxReloadSkyVisual(afxScene scn, afxUri const* uri)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SCN, 1, &scn);
    
    afxDrawInput din = scn->din;
    AFX_ASSERT_OBJECTS(afxFcc_DIN, 1, &din);
    afxDrawSystem dsys = AfxGetDrawInputContext(din); // temp workaround to get a context.

    if (scn->skyType)
    {
        afxUri faces[6];
        AfxMakeUri(&faces[0], 0, "right.tga", 0); // x+
        AfxMakeUri(&faces[1], 0, "left.tga", 0); // x-
        AfxMakeUri(&faces[3], 0, "top.tga", 0);
        AfxMakeUri(&faces[2], 0, "bottom.tga", 0); // y-
        AfxMakeUri(&faces[4], 0, "front.tga", 0);
        AfxMakeUri(&faces[5], 0, "back.tga", 0);

        afxRasterInfo rasi = { 0 };
        rasi.flags = afxRasterFlag_CUBEMAP;
        rasi.usage = afxRasterUsage_SAMPLING;
        rasi.extent.d = 6;

        afxStream file = NIL;
        afxStreamInfo iobi = { 0 };
        iobi.usage = afxIoUsage_FILE;
        iobi.flags = afxIoFlag_READABLE;
        AfxAcquireStream(1, &iobi, &file);

        afxUnit portId = 0;

        afxUri2048 urib;
        AfxMakeUri2048(&urib, NIL);
        afxRaster cubemap = NIL;

        for (afxUnit i = 0; i < rasi.extent.d; i++)
        {
            AfxFormatUri(&urib.uri, "%.*s/%.*s", AfxPushString(&uri->str), AfxPushString(&faces[i].str));

            if (i == 0)
            {
                if (AfxLoadRasters(dsys, 1, &rasi, &urib.uri, &cubemap))
                {
                    AfxThrowError();
                    break;
                }
            }
            else
            {
                afxRasterIo iop = { 0 };

                if (AfxFetchRaster(cubemap, 1, &iop, 0, portId, &urib.uri)) AfxThrowError();
                else
                {
                    AfxWaitForDrawBridge(dsys, portId, AFX_TIME_INFINITE);
                }
            }
        }

        if (file)
            AfxDisposeObjects(1, &file);

        if (scn->sky.cubemap)
            AfxDisposeObjects(1, &scn->sky.cubemap);

        scn->sky.cubemap = cubemap;
    }
    return err;
}

_ASX afxError _AsxScnDtorCb(afxScene scn)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SCN, 1, &scn);

    if (scn->skyType == avxSkyType_BOX)
    {
        AfxDisposeObjects(1, &scn->sky.cubemap);
        AfxDisposeObjects(1, &scn->sky.cube);
        AfxDisposeObjects(1, &scn->sky.skyDtec);
        AfxDisposeObjects(1, &scn->sky.skyVin);
        AfxDisposeObjects(1, &scn->sky.smp);
    }

    return err;
}

_ASX afxError _AsxScnCtorCb(afxScene scn, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SCN, 1, &scn);

    afxSimulation sim = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);
    afxSceneInfo const* info = AFX_CAST(afxSceneInfo const*, args[1]) + invokeNo;
    AFX_ASSERT(info);

    afxDrawInput din = info->din;
    afxDrawSystem dsys = AfxGetDrawInputContext(din); // temp workaround to get a context.

    scn->din = din;

    AfxV3dSet(scn->sky.rotPivot, 0, 0, 1);
    scn->sky.cubemapColorIntensity = 1.f;
    scn->sky.rotSpeed = 0.f;
    scn->sky.currRot = 0.f;
    AfxColorSet(scn->sky.ambientColor, 0.1, 0.1, 0.1, 1);
    AfxColorSet(scn->sky.emissiveColor, 0.1, 0.1, 0.1, 1);
    
    scn->skyType = info->skyType;

    if (info->skyType == avxSkyType_BOX)
    {
        afxMeshBlueprint mshb = { 0 };
        mshb.attrCnt = 1;
        mshb.vtxCnt = ARRAY_SIZE(skyboxVertices) / 3;
        mshb.triCnt = mshb.vtxCnt / 3;
        AfxCompileMeshes(sim, 1, &mshb, &scn->skyMsh);
        AFX_ASSERT_OBJECTS(afxFcc_MSH, 1, &scn->skyMsh);

        AfxFormatVertexAttribute(scn->skyMsh, 0, avxFormat_RGB32f, afxVertexFlag_POSITIONAL | afxVertexFlag_SPATIAL | afxVertexFlag_ATV, &AfxString("pos"));

        AfxUpdateVertexData(scn->skyMsh, 0, 0, 0, mshb.vtxCnt, skyboxVertices, sizeof(skyboxVertices[0]) / 3);

        // sky
        afxUri cubeUri[6];
#if !0
#if 0    
        AfxMakeUri(&cubeUri[0], 0, "../art/skybox/day/right.tga", 0);
        AfxMakeUri(&cubeUri[1], 0, "../art/skybox/day/left.tga", 0);
        AfxMakeUri(&cubeUri[3], 0, "../art/skybox/day/top.tga", 0);
        AfxMakeUri(&cubeUri[2], 0, "../art/skybox/day/bottom.tga", 0);
        AfxMakeUri(&cubeUri[4], 0, "../art/skybox/day/front.tga", 0);
        AfxMakeUri(&cubeUri[5], 0, "../art/skybox/day/back.tga", 0);
#else
        AfxMakeUri(&cubeUri[0], 0, "../art/skybox/purple/right.tga", 0); // x+
        AfxMakeUri(&cubeUri[1], 0, "../art/skybox/purple/left.tga", 0); // x-
        AfxMakeUri(&cubeUri[3], 0, "../art/skybox/purple/top.tga", 0);
        AfxMakeUri(&cubeUri[2], 0, "../art/skybox/purple/bottom.tga", 0); // y-
        AfxMakeUri(&cubeUri[4], 0, "../art/skybox/purple/front.tga", 0);
        AfxMakeUri(&cubeUri[5], 0, "../art/skybox/purple/back.tga", 0);

#endif
#else
        AfxMakeUri(&cubeUri[0], 0, "../art/skybox/envmap_interstellar/interstellar_rt.tga", 0); // x+
        AfxMakeUri(&cubeUri[1], 0, "../art/skybox/envmap_interstellar/interstellar_lf.tga", 0); // x-
        AfxMakeUri(&cubeUri[3], 0, "../art/skybox/envmap_interstellar/interstellar_up.tga", 0);
        AfxMakeUri(&cubeUri[2], 0, "../art/skybox/envmap_interstellar/interstellar_dn.tga", 0); // y-
        AfxMakeUri(&cubeUri[4], 0, "../art/skybox/envmap_interstellar/interstellar_ft.tga", 0);
        AfxMakeUri(&cubeUri[5], 0, "../art/skybox/envmap_interstellar/interstellar_bk.tga", 0);
#endif

        afxUri facesUri[6];
        AfxMakeUri(&facesUri[0], 0, "right.tga", 0); // x+
        AfxMakeUri(&facesUri[1], 0, "left.tga", 0); // x-
        AfxMakeUri(&facesUri[3], 0, "top.tga", 0);
        AfxMakeUri(&facesUri[2], 0, "bottom.tga", 0); // y-
        AfxMakeUri(&facesUri[4], 0, "front.tga", 0);
        AfxMakeUri(&facesUri[5], 0, "back.tga", 0);

        afxUri cubeDir;
        AfxMakeUri(&cubeDir, 0, "../art/skybox/purple", 0);

        afxRasterInfo rasi = { 0 };
        rasi.flags = afxRasterFlag_CUBEMAP;
        rasi.usage = afxRasterUsage_SAMPLING;

        scn->sky.cubemap = AfxLoadCubemapRaster(dsys, &rasi, &cubeDir, facesUri);
        //AfxFlipRaster(sky->cubemap, FALSE, TRUE);

        //for (afxUnit i = 0; i < sizeof(skyboxVertices) / sizeof(skyboxVertices[0]); i++)
        //    skyboxVertices[i] *= 100;

        afxUri uri;
        AfxMakeUri(&uri, 0, "../data/pipeline/skybox/skybox.xsh.xml", 0);
        AfxLoadDrawTechnique(din, &uri, &scn->sky.skyDtec);

        avxVertexFetch const vinStreams[] =
        {
            {
                .instanceRate = 0,
                .srcIdx = 0
            }
        };
        avxVertexInput const vinAttrs[] =
        {
            {
                .location = 0,
                .srcIdx = 0,
                .offset = 0,
                .fmt = avxFormat_RGB32f
            }
        };

        AfxDeclareVertexLayout(dsys, 1, vinStreams, 1, vinAttrs, &scn->sky.skyVin);
        AFX_ASSERT_OBJECTS(afxFcc_VIN, 1, &scn->sky.skyVin);

        avxSamplerInfo smpSpec = { 0 };
        smpSpec.magnify = avxTexelFilter_LINEAR;
        smpSpec.minify = avxTexelFilter_LINEAR;
        smpSpec.mipify = avxTexelFilter_LINEAR;
        smpSpec.uvw[0] = avxTexelAddress_CLAMP;
        smpSpec.uvw[1] = avxTexelAddress_CLAMP;
        smpSpec.uvw[2] = avxTexelAddress_CLAMP;

        AfxDeclareSamplers(dsys, 1, &smpSpec, &scn->sky.smp);
        AFX_ASSERT_OBJECTS(afxFcc_SAMP, 1, &scn->sky.smp);
    }
    else
    {
        scn->skyMsh = AfxBuildSphereMesh(sim, 2.0, 20, 20, TRUE);

        AfxColorSet(scn->apexCol, 0.f, 0.15f, 0.66f, 1.f);
        AfxColorSet(scn->centreCol, 0.81f, 0.38f, 0.66f, 1.f);

        avxPipeline pip;
        avxPipelineBlueprint pipb = { 0 };
        AfxAssemblePipelines(dsys, 1, &pipb, &pip);
        
        afxUri uri;
        AfxMakeUri(&uri, 0, "../src/skydome/skydomeVs.glsl", 0);
        AfxUplinkPipelineFunction(pip, avxShaderStage_VERTEX, &uri, NIL, NIL, NIL);
        AfxMakeUri(&uri, 0, "../src/skydome/skydomeFs.glsl", 0);
        AfxUplinkPipelineFunction(pip, avxShaderStage_FRAGMENT, AfxUri("../src/skydome/skydomeFs.glsl"), NIL, NIL, NIL);

        struct
        {
            afxM4d p, v, m;
            afxV4d apexCol, centreCol;
        } data;
        afxBuffer buf;
        afxBufferInfo bufi = { 0 };
        bufi.cap = sizeof(data);
        bufi.usage = afxBufferUsage_UNIFORM;
        bufi.flags = afxBufferFlag_W;
        AfxAcquireBuffers(dsys, 1, &bufi, &buf);
        
    }
    return err;
}

_ASX afxClassConfig const _ASX_SCN_CLASS_CONFIG =
{
    .fcc = afxFcc_SCN,
    .name = "Scene",
    .desc = "Scene",
    .fixedSiz = sizeof(AFX_OBJECT(afxScene)),
    .ctor = (void*)_AsxScnCtorCb,
    .dtor = (void*)_AsxScnDtorCb,
};

////////////////////////////////////////////////////////////////////////////////

_ASX afxError AfxAcquireScenes(afxSimulation sim, afxUnit cnt, afxSceneInfo infos[], afxScene scenes[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);
    AFX_ASSERT(scenes);
    AFX_ASSERT(infos);
    AFX_ASSERT(cnt);

    afxClass* cls = (afxClass*)_AsxGetSceneClass(sim);
    AFX_ASSERT_CLASS(cls, afxFcc_SCN);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)scenes, (void const*[]) { sim, infos }))
        AfxThrowError();

    return err;
}
