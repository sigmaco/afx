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
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

#define _AVX_DRAW_C
#define _AVX_MESH_C
#define _AVX_MESH_TOPOLOGY_C
#include "../draw/impl/avxImplementation.h"

#define _AFX_SIM_C
#define _ASX_TERRAIN_C
#define _ASX_SIMULATION_C
#include "impl/asxImplementation.h"
#include "qwadro/inc/sim/afxTerrain.h"

AFX_DEFINE_STRUCT(asxQuadrant)
{
    afxRect     bounds;
    afxUnit     childIdx[4];
    afxUnit     parentIdx;
    afxChain    contents;
};

AFX_DEFINE_STRUCT(asxQuadtree)
{
    asxQuadrant root;
    afxPool     quadrants;
};

ASX void AfxDeployQuadtree(asxQuadtree* tree, afxRect* bounds)
{
    tree->root.bounds = *bounds;
    tree->root.parentIdx = AFX_INVALID_INDEX;

    for (afxUnit i = 0; i < /* it is always a quadtree*/ 4; i++)
        tree->root.childIdx[i] = AFX_INVALID_INDEX;

    AfxDeployChain(&tree->root.contents, tree);
    AfxDeployPool(&tree->quadrants, sizeof(asxQuadtree), /* it is always a quadtree*/ 4, AFX_SIMD_ALIGNMENT);
}

_ASX afxError AfxLoadHeighmap(afxTerrain ter, afxUnit secIdx, afxUri const* uri)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_TER, 1, &ter);
    AFX_ASSERT_RANGE(ter->secCnt, secIdx, 1);

    afxStream iob;
    if (AfxOpenFile(uri, afxFileFlag_RX, &iob))
    {
        AfxThrowError();
        return err;
    }
    afxTargaFile tga;
    AfxReadTarga(iob, &tga);

    afxByte* data;
    AfxAllocate(tga.decodedSiz, AFX_SIMD_ALIGNMENT, AfxHere(), (void**)&data);
    AfxDecodeTarga(iob, &tga, data);
    AfxDisposeObjects(1, &iob);

    avxFormatDescription pfd;
    AfxDescribePixelFormat(tga.fmt, &pfd);




    afxMesh msh;
    AfxGetTerrainMeshes(ter, secIdx, 1, &msh);
    afxMeshMorph mshm;
    AfxGetMeshMorphes(msh, 0, 1, &mshm);

    afxV3d* pos = AfxAccessVertexData(msh, 0, 0, 0);

    afxReal yScale = 64.0f / 256.0f, yShift = 16.0f;  // apply a scale+shift to the height data

    for (unsigned int i = 0; i < tga.width; i++)
    {
        for (unsigned int j = 0; j < tga.height; j++)
        {
            // retrieve texel for (i,j) tex coord
            unsigned char* texel = data + (j + tga.width * i) * pfd.compCnt;
            // raw height at coordinate
            unsigned char y = texel[0];

            // vertex
            AfxV3dSet(pos[i * j + j], -tga.height / 2.0f + i, (int)y * yScale - yShift, -tga.width / 2.0f + j);
        }
    }

    AfxDeallocate((void**)&data, AfxHere());

    return err;
}

_ASX afxUnit AfxGetTerrainMeshes(afxTerrain ter, afxUnit secIdx, afxUnit cnt, afxMesh meshes[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_TER, 1, &ter);
    AFX_ASSERT_RANGE(ter->secCnt, secIdx, cnt);
    afxUnit rslt = 0;

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxMesh msh = ter->sectors[secIdx].msh;

        if (msh)
        {
            AFX_ASSERT_OBJECTS(afxFcc_MSH, 1, &msh);
            ++rslt;
        }
        meshes[i] = msh;
    }
    return rslt;
}

_ASX afxError AfxResetTerrainSector(afxTerrain ter, afxUnit secIdx, afxMesh msh)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_TER, 1, &ter);
    
    if (ter->sectors[secIdx].msh != msh)
    {
        if (msh)
        {
            AfxReacquireObjects(1, &msh);
        }

        if (ter->sectors[secIdx].msh)
        {
            AfxDisposeObjects(1, &ter->sectors[secIdx].msh);
        }

        ter->sectors[secIdx].msh = msh;
    }
    return err;
}

_ASX afxError _AsxTerDtorCb(afxTerrain ter)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_TER, 1, &ter);

    afxObjectStash const stashes[] =
    {
        {
            .cnt = ter->secCnt,
            .siz = sizeof(ter->sectors[0]),
            .var = (void**)&ter->sectors
        }
    };

    if (AfxDeallocateInstanceData(ter, ARRAY_SIZE(stashes), stashes))
        AfxThrowError();

    return err;
}

_ASX afxError _AsxTerCtorCb(afxTerrain ter, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_TER, 1, &ter);

    afxSimulation sim = args[0];
    afxUnit secCnt = *(afxUnit*)(args[1]);

    afxDrawSystem dsys = AfxGetSimulationDrawSystem(sim);

    secCnt = AfxMin(1, secCnt);
    ter->secCnt = secCnt;

    afxObjectStash const stashes[] =
    {
        {
            .cnt = secCnt,
            .siz = sizeof(ter->sectors[0]),
            .var = (void**)&ter->sectors
        }
    };

    if (AfxAllocateInstanceData(ter, ARRAY_SIZE(stashes), stashes)) AfxThrowError();
    else
    {
        for (afxUnit i = 0; i < secCnt; i++)
        {
            ter->sectors[i].msh = NIL;
        }

        if (err && AfxDeallocateInstanceData(ter, ARRAY_SIZE(stashes), stashes))
            AfxThrowError();
    }

    return err;
}

_ASX afxClassConfig const _ASX_TER_CLASS_CONFIG =
{
    .fcc = afxFcc_TER,
    .name = "Terrain",
    .desc = "Managed Terrain",
    .fixedSiz = sizeof(AFX_OBJECT(afxTerrain)),
    .ctor = (void*)_AsxTerCtorCb,
    .dtor = (void*)_AsxTerDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_ASX afxError AfxAcquireTerrain(afxSimulation sim, afxUnit secCnt, afxTerrain* terrain)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);

    if (AfxAcquireObjects(_AsxGetTerrainClass(sim), 1, (afxObject*)terrain, (void const*[]) { sim, &secCnt } ))
        AfxThrowError();

    return err;
}

_ASX afxError AfxGenerateTerrain(afxSimulation sim, afxWhd const whd, afxTerrain* terrain)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);

    afxUnit width = whd.w, height = whd.h, depth = whd.d;

    afxMesh msh;
    //msh = AfxBuildPlaneMesh(sim->din, 256, 256, 256, 256);
    //msh = AfxBuildParallelepipedMesh(sim->din, AfxWhd(10, 10, 10), 10, 10);
    //msh = AfxBuildCubeMesh(sim->din, 10, 1);
    AfxBuildGridMesh(sim, 100, 100, 1000, 1000, &msh);
    //msh = AfxBuildGridMesh(sim->din, 10, 10, 10, 10);
    //msh = AfxBuildDiscMesh(sim->din, 10, 10);
    //AfxInvertMeshTopology(msh);

    afxTerrain ter;
    AfxAcquireTerrain(sim, 1, &ter);
    AfxResetTerrainSector(ter, 0, msh);
    AfxDisposeObjects(1, &msh);
    *terrain = ter;

    return err;
}

_ASX afxError AfxGenerateHeightmappedTerrain(afxSimulation sim, afxUri const* uri, afxTerrain* terrain)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);

    afxRaster ras;
    AfxLoadRasters(sim->dsys, 1, NIL, uri, &ras);
    AfxPrintRaster(ras, NIL, 1, 0, AfxUri("../heightmap_dump.tga"));

    afxStream iob;
    afxStreamInfo iobi = { 0 };
    iobi.usage = afxIoUsage_FILE;
    AfxAcquireStream(1, &iobi, &iob);
    AfxReopenFile(iob, uri, afxFileFlag_R);

    afxTargaFile tga;
    AfxReadTarga(iob, &tga);

    afxByte* data;
    AfxAllocate(tga.decodedSiz, AFX_SIMD_ALIGNMENT, AfxHere(), (void**)&data);
    AfxDecodeTarga(iob, &tga, data);

    avxFormatDescription pfd;
    AfxDescribePixelFormat(tga.fmt, &pfd);

    //tga.width = tga.width, tga.height = tga.height / 8;

    afxMesh msh;
    AfxBuildGridMesh(sim, tga.width, tga.height, tga.width * 10, tga.height * 10, &msh);

    afxTerrain ter;
    AfxAcquireTerrain(sim, 1, &ter);
    AfxResetTerrainSector(ter, 0, msh);

    //afxMesh msh;
    AfxGetTerrainMeshes(ter, 0, 1, &msh);
    afxMeshMorph mshm;
    AfxGetMeshMorphes(msh, 0, 1, &mshm);

    afxV3d* pos = AfxAccessVertexData(msh, 0, 0, 0);

    afxReal yScale = 64.0f / 256.0f, yShift = 16.0f;  // apply a scale+shift to the height data

    afxUnit vtxIdx = 0;
    for (unsigned int i = 0; i < tga.height; i++)
    {
        for (unsigned int j = 0; j < tga.width; j++)
        {
            unsigned char* pixelOffset = data + (j + tga.width * i) * pfd.stride;
            unsigned char y = pixelOffset[0];
#if 0
#if 0
            // vertex
            AfxV3dSet(pos[vtxIdx++],
                (-tga.height / 2.0f + tga.height * i / (float)tga.height), // vx
                ((int)y * yScale - yShift), // vy
                (-tga.width / 2.0f + tga.width * j / (float)tga.width));   // vz
#else
            // vertex
            AfxV3dSet(pos[vtxIdx++],
                (-tga.width / 2.0f + tga.width * j / (float)tga.width), // vx
                ((int)y * yScale - yShift), // vy
                (-tga.height / 2.0f + tga.height * i / (float)tga.height));   // vz
#endif
#endif
            pos[(j + tga.width * i)][1] = ((afxReal)y * yScale - yShift);

        }
    }
    *terrain = ter;
    return err;
}
