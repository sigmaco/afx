/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *          Q W A D R O   4 D   R E N D E R I N G   I N F R A S T R U C T U R E
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

#define _ARX_SCENE_C
#include "ddi/arxImpl_Input.h"
#include "../draw/ddi/avxImplementation.h"
#include "qwadro/render/arxScene.h"

AFX_OBJECT(arxScene)
{
    arxRenderware    rwe;
    arxSkyType      skyType;
    arxMesh         skyMsh;
    avxColor        apexCol;
    avxColor        centreCol;
    struct
    {
        avxBuffer       cube;
        avxRaster       cubemap;
        afxReal         cubemapColorIntensity;
        afxV3d          fogColor;
        afxReal         fogUpper;
        afxReal         fogLower;
        avxColor        emissiveColor;
        avxColor        ambientColor;
        afxReal         currRot;
        afxReal         rotSpeed;
        afxV3d          rotPivot;
        afxQuat         rotQuat;
        afxM4d          rotMtx;
        arxTechnique skyDtec;
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

_ARX void ArxStepSky(arxScene scn, afxReal dt)
{
    scn->sky.currRot += scn->sky.rotSpeed * dt;
    AfxQuatRotationFromAxis(scn->sky.rotQuat, scn->sky.rotPivot, scn->sky.currRot);
    AfxM4dRotationFromQuat(scn->sky.rotMtx, scn->sky.rotQuat);
}

_ARX afxError ArxDrawSky(afxDrawContext dctx, arxScene scn)
{
    afxError err = AFX_ERR_NONE;

    if (scn->skyType == arxSkyType_BOX)
    {
        AvxCmdUseDrawTechniqueSIGMA(dctx, scn->sky.skyDtec, 0, scn->sky.skyVin, NIL);

        AvxCmdBindSamplers(dctx, avxBus_DRAW, 0, 1, 1, &scn->sky.smp);
        AvxCmdBindRasters(dctx, avxBus_DRAW, 0, 1, 1, &scn->sky.cubemap);

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

_ARX afxError ArxReloadSkyVisual(arxScene scn, afxUri const* uri)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SCN, 1, &scn);
    
    arxRenderware rwe = scn->rwe;
    AFX_ASSERT_OBJECTS(afxFcc_RWE, 1, &rwe);
    afxDrawSystem dsys = ArxGetRenderwareDrawSystem(rwe); // temp workaround to get a context.

    if (scn->skyType)
    {
        afxUri faces[6];
        AfxMakeUri(&faces[0], 0, "right.tga", 0); // x+
        AfxMakeUri(&faces[1], 0, "left.tga", 0); // x-
        AfxMakeUri(&faces[3], 0, "top.tga", 0);
        AfxMakeUri(&faces[2], 0, "bottom.tga", 0); // y-
        AfxMakeUri(&faces[4], 0, "front.tga", 0);
        AfxMakeUri(&faces[5], 0, "back.tga", 0);

        avxRasterInfo rasi = { 0 };
        rasi.flags = avxRasterFlag_CUBEMAP;
        rasi.usage = avxRasterUsage_TEXTURE;
        rasi.whd.d = 6;

        afxStream file = NIL;
        afxStreamInfo iobi = { 0 };
        iobi.usage = afxIoUsage_FILE;
        iobi.flags = afxIoFlag_READABLE;
        AfxAcquireStream(1, &iobi, &file);

        afxUnit portId = 0;

        afxUri2048 urib;
        AfxMakeUri2048(&urib, NIL);
        avxRaster cubemap = NIL;

        for (afxUnit i = 0; i < rasi.whd.d; i++)
        {
            AfxFormatUri(&urib.uri, "%.*s/%.*s", AfxPushString(&uri->s), AfxPushString(&faces[i].s));

            if (i == 0)
            {
                if (AvxLoadRasters(dsys, 1, &rasi, &urib.uri, NIL, &cubemap))
                {
                    AfxThrowError();
                    break;
                }
            }
            else
            {
                avxRasterIo iop = { 0 };

                if (AvxReloadRaster(cubemap, 1, &iop, 0, &urib.uri, portId)) AfxThrowError();
                else
                {
                    AvxWaitForDrawBridges(dsys, AFX_TIMEOUT_INFINITE, portId);
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

_ARX afxError _ArxScnDtorCb(arxScene scn)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SCN, 1, &scn);

    if (scn->skyType == arxSkyType_BOX)
    {
        AfxDisposeObjects(1, &scn->sky.cubemap);
        AfxDisposeObjects(1, &scn->sky.cube);
        AfxDisposeObjects(1, &scn->sky.skyDtec);
        AfxDisposeObjects(1, &scn->sky.skyVin);
        AfxDisposeObjects(1, &scn->sky.smp);
    }

    return err;
}

_ARX afxError _ArxScnCtorCb(arxScene scn, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SCN, 1, &scn);

    arxRenderware rwe = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_RWE, 1, &rwe);
    arxSceneInfo const* info = AFX_CAST(arxSceneInfo const*, args[1]) + invokeNo;
    AFX_ASSERT(info);

    afxDrawSystem dsys = ArxGetRenderwareDrawSystem(rwe); // temp workaround to get a context.

    scn->rwe = rwe;

    AfxV3dSet(scn->sky.rotPivot, 0, 0, 1);
    scn->sky.cubemapColorIntensity = 1.f;
    scn->sky.rotSpeed = 0.f;
    scn->sky.currRot = 0.f;
    AvxMakeColor(scn->sky.ambientColor, 0.1, 0.1, 0.1, 1);
    AvxMakeColor(scn->sky.emissiveColor, 0.1, 0.1, 0.1, 1);
    
    scn->skyType = info->skyType;

    if (info->skyType == arxSkyType_BOX)
    {
        arxMeshBlueprint mshb = { 0 };
        mshb.attrCnt = 1;
        mshb.vtxCnt = ARRAY_SIZE(skyboxVertices) / 3;
        mshb.triCnt = mshb.vtxCnt / 3;
        ArxBuildMeshes(rwe, 1, &mshb, &scn->skyMsh);
        AFX_ASSERT_OBJECTS(afxFcc_MSH, 1, &scn->skyMsh);

        ArxFormatVertexAttribute(scn->skyMsh, 0, avxFormat_RGB32f, arxVertexFlag_POSITIONAL | arxVertexFlag_SPATIAL | arxVertexFlag_ATV, &AFX_STRING("pos"));

        ArxUpdateVertexData(scn->skyMsh, 0, 0, 0, mshb.vtxCnt, skyboxVertices, sizeof(skyboxVertices[0]) / 3);

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

        avxRasterInfo rasi = { 0 };
        rasi.flags = avxRasterFlag_CUBEMAP;
        rasi.usage = avxRasterUsage_TEXTURE;

        scn->sky.cubemap = AvxLoadSegmentedRaster(dsys, &rasi, &cubeDir, facesUri);
        //AfxFlipRaster(sky->cubemap, FALSE, TRUE);

        //for (afxUnit i = 0; i < sizeof(skyboxVertices) / sizeof(skyboxVertices[0]); i++)
        //    skyboxVertices[i] *= 100;

        afxUri uri;
        AfxMakeUri(&uri, 0, "../data/pipeline/skybox/skybox.xsh.xml", 0);
        ArxLoadDrawTechnique(rwe, &uri, &scn->sky.skyDtec);

        avxVertexLayout skyVtxl =
        {
            .binCnt = 1,
            .bins = { AVX_VERTEX_STREAM(0, 0, 0, 0, 1) },
            .attrs = { AVX_VERTEX_ATTR(0, 0, avxFormat_RGB32f) }
        };

        AvxDeclareVertexInputs(dsys, 1, &skyVtxl, &scn->sky.skyVin);
        AFX_ASSERT_OBJECTS(afxFcc_VIN, 1, &scn->sky.skyVin);

        avxSamplerConfig smpSpec = { 0 };
        smpSpec.magnify = avxTexelFilter_LINEAR;
        smpSpec.minify = avxTexelFilter_LINEAR;
        smpSpec.mipFlt = avxTexelFilter_LINEAR;
        smpSpec.uvw[0] = avxTexelWrap_EDGE;
        smpSpec.uvw[1] = avxTexelWrap_EDGE;
        smpSpec.uvw[2] = avxTexelWrap_EDGE;

        AvxDeclareSamplers(dsys, 1, &smpSpec, &scn->sky.smp);
        AFX_ASSERT_OBJECTS(afxFcc_SAMP, 1, &scn->sky.smp);
    }
    else
    {
        ArxBuildSphereMesh(rwe, 2.0, 20, 20, TRUE, &scn->skyMsh);

        AvxMakeColor(scn->apexCol, 0.f, 0.15f, 0.66f, 1.f);
        AvxMakeColor(scn->centreCol, 0.81f, 0.38f, 0.66f, 1.f);

        avxPipeline pip;
        avxPipelineConfig pipb = { 0 };
        AvxAssemblePipelines(dsys, 1, &pipb, &pip);
        
        afxUri uri;
        AfxMakeUri(&uri, 0, "../src/skydome/skydomeVs.glsl", 0);
        AvxUplinkPipelineFunction(pip, avxShaderType_VERTEX, &uri, NIL, NIL, NIL);
        AfxMakeUri(&uri, 0, "../src/skydome/skydomeFs.glsl", 0);
        AvxUplinkPipelineFunction(pip, avxShaderType_FRAGMENT, AfxUri("../src/skydome/skydomeFs.glsl"), NIL, NIL, NIL);

        struct
        {
            afxM4d p, v, m;
            afxV4d apexCol, centreCol;
        } data;
        avxBuffer buf;
        avxBufferInfo bufi = { 0 };
        bufi.size = sizeof(data);
        bufi.usage = avxBufferUsage_UNIFORM;
        bufi.flags = avxBufferFlag_W;
        AvxAcquireBuffers(dsys, 1, &bufi, &buf);
        
    }
    return err;
}

_ARX afxClassConfig const _ARX_SCN_CLASS_CONFIG =
{
    .fcc = afxFcc_SCN,
    .name = "Scene",
    .desc = "Scene",
    .fixedSiz = sizeof(AFX_OBJECT(arxScene)),
    .ctor = (void*)_ArxScnCtorCb,
    .dtor = (void*)_ArxScnDtorCb,
};

////////////////////////////////////////////////////////////////////////////////

_ARX afxError ArxAcquireScenes(arxRenderware rwe, afxUnit cnt, arxSceneInfo infos[], arxScene scenes[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_RWE, 1, &rwe);
    AFX_ASSERT(scenes);
    AFX_ASSERT(infos);
    AFX_ASSERT(cnt);

    afxClass* cls = (afxClass*)_ArxRweGetScnClass(rwe);
    AFX_ASSERT_CLASS(cls, afxFcc_SCN);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)scenes, (void const*[]) { rwe, infos }))
        AfxThrowError();

    return err;
}
