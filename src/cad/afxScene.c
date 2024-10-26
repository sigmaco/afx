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

#define _AFX_DRAW_C
#define _AVX_SCENE_C
#include "../dev/AvxImplKit.h"

AFX_OBJECT(afxScene)
{
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
        avxVertexInput  skyVin;
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

_AVX void AfxStepSky(afxScene scn, afxReal dt)
{
    scn->sky.currRot += scn->sky.rotSpeed * dt;
    AfxQuatRotationFromAxis(scn->sky.rotQuat, scn->sky.rotPivot, scn->sky.currRot);
    AfxM4dRotationFromQuat(scn->sky.rotMtx, scn->sky.rotQuat);
}

_AVX afxError AfxDrawSky(avxCmdb cmdb, afxScene scn)
{
    afxError err = AFX_ERR_NONE;

    if (scn->skyType == avxSkyType_BOX)
    {
        AvxCmdApplyDrawTechnique(cmdb, scn->sky.skyDtec, 0, scn->sky.skyVin, NIL);

        AvxCmdBindSamplers(cmdb, 0, 1, 1, &scn->sky.smp);
        AvxCmdBindRasters(cmdb, 0, 1, 1, &scn->sky.cubemap, NIL);

        AvxCmdBindVertexSources(cmdb, 0, 1, (afxBuffer[]) { scn->sky.cube }, NIL, NIL, (afxUnit32[]) { sizeof(afxV3d) });
        //AvxCmdResetVertexStreams(cmdb, 1, NIL, (afxUnit32[]) { sizeof(afxV3d) }, NIL);
        //AvxCmdResetVertexAttributes(cmdb, 1, NIL, (afxVertexFormat[]) { afxVertexFormat_V3D }, NIL, NIL);
        AvxCmdDraw(cmdb, 36, 1, 0, 0);
    }
    else
    {

    }
    return err;
}

_AVX afxError AfxReloadSkyVisual(afxScene scn, afxUri const* uri)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &scn, afxFcc_SCN);
    
    afxDrawContext dctx;
    afxDrawInput din = AfxGetProvider(scn);
    AfxAssertObjects(1, &din, afxFcc_DIN);
    AfxGetDrawInputContext(din, &dctx); // temp workaround to get a context.

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
        iobi.usage = afxStreamUsage_FILE;
        iobi.flags = afxStreamFlag_READABLE;
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
                if (AfxLoadRasters(dctx, 1, &rasi, &urib.uri, &cubemap))
                {
                    AfxThrowError();
                    break;
                }
            }
            else
            {
                afxRasterIo iop = { 0 };

                if (AfxFetchRaster(cubemap, 1, &iop, 0, &urib.uri, portId)) AfxThrowError();
                else
                {
                    AfxWaitForDrawBridge(dctx, portId, AFX_TIME_INFINITE);
                }
            }
        }

        if (file)
            AfxReleaseObjects(1, &file);

        if (scn->sky.cubemap)
            AfxReleaseObjects(1, &scn->sky.cubemap);

        scn->sky.cubemap = cubemap;
    }
    return err;
}

_AVX afxError _AvxScnDtorCb(afxScene scn)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &scn, afxFcc_SCN);

    if (scn->skyType == avxSkyType_BOX)
    {
        AfxReleaseObjects(1, &scn->sky.cubemap);
        AfxReleaseObjects(1, &scn->sky.cube);
        AfxReleaseObjects(1, &scn->sky.skyDtec);
        AfxReleaseObjects(1, &scn->sky.skyVin);
        AfxReleaseObjects(1, &scn->sky.smp);
    }

    return err;
}

_AVX afxError _AvxScnCtorCb(afxScene scn, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &scn, afxFcc_SCN);

    afxDrawInput din = args[0];
    AfxAssertObjects(1, &din, afxFcc_DIN);
    afxSceneInfo const* info = AFX_CAST(afxSceneInfo const*, args[1]) + invokeNo;

    afxDrawContext dctx;
    AfxGetDrawInputContext(din, &dctx); // temp workaround to get a context.

    AfxV3dSet(scn->sky.rotPivot, 0, 0, 1);
    scn->sky.cubemapColorIntensity = 1.f;
    scn->sky.rotSpeed = 0.f;
    scn->sky.currRot = 0.f;
    AfxColorSet(scn->sky.ambientColor, 0.1, 0.1, 0.1, 1);
    AfxColorSet(scn->sky.emissiveColor, 0.1, 0.1, 0.1, 1);
    
    scn->skyType = info->skyType;

    if (info->skyType == avxSkyType_BOX)
    {
        afxGeometry geo;
        afxGeometryInfo geob = { 0 };
        geob.attrCnt = 1;
        geob.vtxCnt = ARRAY_SIZE(skyboxVertices) / 3;
        AfxAcquireGeometries(din, 1, &geob, &geo);
        AfxAssertObjects(1, &geo, afxFcc_GEO);

        AfxResetGeometry(geo, 0, afxVertexFormat_V3D, afxVertexFlag_POSITIONAL | afxVertexFlag_SPATIAL | afxVertexFlag_ATV, &AfxString("pos"));

        AfxUpdateGeometry(geo, 0, 0, geob.vtxCnt, skyboxVertices, sizeof(skyboxVertices[0]) / 3);

        afxMeshBlueprint mshb = { 0 };
        mshb.geo = geo;
        mshb.triCnt = geob.vtxCnt / 3;
        AfxAssembleMeshes(din, 1, &mshb, &scn->skyMsh);
        AfxAssertObjects(1, &scn->skyMsh, afxFcc_MSH);
        AfxReleaseObjects(1, &geo);

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

        scn->sky.cubemap = AfxLoadCubemapRaster(dctx, &rasi, &cubeDir, facesUri);
        //AfxFlipRaster(sky->cubemap, FALSE, TRUE);

        //for (afxUnit i = 0; i < sizeof(skyboxVertices) / sizeof(skyboxVertices[0]); i++)
        //    skyboxVertices[i] *= 100;

        afxUri uri;
        AfxMakeUri(&uri, 0, "../data/pipeline/skybox/skybox.xsh.xml", 0);
        AfxLoadDrawTechnique(din, &uri, &scn->sky.skyDtec);

        avxVertexInputStream const vinStreams[] =
        {
            {
                .instanceRate = 0,
                .slotIdx = 0
            }
        };
        avxVertexInputAttr const vinAttrs[] =
        {
            {
                .location = 0,
                .streamIdx = 0,
                .offset = 0,
                .fmt = afxVertexFormat_V3D
            }
        };

        scn->sky.skyVin = AfxAcquireVertexInput(dctx, 1, vinStreams, 1, vinAttrs);
        AfxAssertObjects(1, &scn->sky.skyVin, afxFcc_VIN);

        avxSamplerInfo smpSpec = { 0 };
        smpSpec.magnify = avxTexelFilter_LINEAR;
        smpSpec.minify = avxTexelFilter_LINEAR;
        smpSpec.mipify = avxTexelFilter_LINEAR;
        smpSpec.uvw[0] = avxTexelAddress_CLAMP;
        smpSpec.uvw[1] = avxTexelAddress_CLAMP;
        smpSpec.uvw[2] = avxTexelAddress_CLAMP;

        AfxAcquireSamplers(dctx, 1, &smpSpec, &scn->sky.smp);
        AfxAssertObjects(1, &scn->sky.smp, afxFcc_SAMP);
    }
    else
    {
        scn->skyMsh = AfxBuildSphereMesh(din, 2.0, 20, 20, TRUE);

        AfxColorSet(scn->apexCol, 0.f, 0.15f, 0.66f, 1.f);
        AfxColorSet(scn->centreCol, 0.81f, 0.38f, 0.66f, 1.f);

        avxPipeline pip;
        avxPipelineBlueprint pipb = { 0 };
        AfxAssemblePipelines(dctx, 1, &pipb, &pip);
        
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
        bufi.bufCap = sizeof(data);
        bufi.usage = afxBufferUsage_UNIFORM;
        bufi.flags = afxBufferFlag_W;
        AfxAcquireBuffers(dctx, 1, &bufi, &buf);
        
    }
    return err;
}

_AVX afxClassConfig const _AvxScnClsCfg =
{
    .fcc = afxFcc_SCN,
    .name = "Scene",
    .desc = "Scene",
    .fixedSiz = sizeof(AFX_OBJECT(afxScene)),
    .ctor = (void*)_AvxScnCtorCb,
    .dtor = (void*)_AvxScnDtorCb,
};

////////////////////////////////////////////////////////////////////////////////

_AVX afxError AfxAcquireScenes(afxDrawInput din, afxUnit cnt, afxSceneInfo infos[], afxScene scenes[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);
    AFX_ASSERT(scenes);
    AFX_ASSERT(infos);
    AFX_ASSERT(cnt);

    afxClass* cls = (afxClass*)AfxGetSceneClass(din);
    AfxAssertClass(cls, afxFcc_SCN);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)scenes, (void const*[]) { din, infos }))
        AfxThrowError();

    return err;
}
