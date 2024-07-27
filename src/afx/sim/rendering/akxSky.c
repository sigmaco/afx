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

#define _AFX_SIM_C
#define _AKX_SIMULATION_C
#include "../dev/AkxSimDevKit.h"
#include "qwadro/afxQwadro.h"
#include "qwadro/sim/rendering/akxSky.h"

_AKX void AfxStepSky(akxSky* sky, afxReal dt)
{
    sky->currRot += sky->rotSpeed * dt;
    AfxQuatRotationFromAxis(sky->rotQuat, sky->rotPivot, sky->currRot);
    AfxM4dRotationFromQuat(sky->rotMtx, sky->rotQuat);
}

_AKX afxError AfxDrawSky(avxCmdb cmdb, akxSky* sky)
{
    afxError err = AFX_ERR_NONE;

    AvxCmdApplyDrawTechnique(cmdb, sky->skyDtec, 0, sky->skyVin, NIL);

    AvxCmdBindSamplers(cmdb, 0, 1, 1, &sky->smp);
    AvxCmdBindRasters(cmdb, 0, 1, 1, &sky->cubemap, NIL);

    AvxCmdBindVertexSources(cmdb, 0, 1, (afxBuffer[]) { sky->cube }, NIL, NIL, (afxNat32[]) {sizeof(afxV3d)});
    //AvxCmdResetVertexStreams(cmdb, 1, NIL, (afxNat32[]) { sizeof(afxV3d) }, NIL);
    //AvxCmdResetVertexAttributes(cmdb, 1, NIL, (afxVertexFormat[]) { afxVertexFormat_V3D }, NIL, NIL);
    AvxCmdDraw(cmdb, 36, 1, 0, 0);
    return err;
}

_AKX afxError AfxBuildSkybox(akxSky* sky, afxSimulation sim, afxDrawInput din)
{
    afxError err = NIL;
    afxDrawContext dctx = sim->dctx;

    AfxV3dSet(sky->rotPivot, 0, 0, 1);
    sky->cubemapColorIntensity = 1.f;
    sky->rotSpeed = 0.f;
    sky->currRot = 0.f;

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

    sky->cubemap = AfxAssembleCubemapRasters(dctx, afxRasterUsage_SAMPLING, afxRasterFlag_CUBEMAP, &cubeDir, facesUri);
    //AfxFlipRaster(sky->cubemap, FALSE, TRUE);


    AfxSetRgba(sky->ambientColor, 0.1, 0.1, 0.1, 1);
    AfxSetRgba(sky->emissiveColor, 0.1, 0.1, 0.1, 1);

    const float SIZE = 500.f;
    afxReal skyboxVertices[] =
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

    //for (afxNat i = 0; i < sizeof(skyboxVertices) / sizeof(skyboxVertices[0]); i++)
    //    skyboxVertices[i] *= 100;

    afxBufferSpecification vboSpec = { 0 };
    vboSpec.siz = sizeof(skyboxVertices);
    vboSpec.src = skyboxVertices;
    vboSpec.usage = afxBufferUsage_VERTEX;

    AfxAcquireBuffers(dctx, 1, &vboSpec, &sky->cube);
    AfxAssertObjects(1, &sky->cube, afxFcc_BUF);

    afxUri uri;
    AfxMakeUri(&uri, 0, "../data/pipeline/skybox/skybox.xsh.xml", 0);
    AfxLoadDrawTechniques(din, 1, &uri, &sky->skyDtec);
    sky->type = akxSkyType_BOX;

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

    sky->skyVin = AfxAcquireVertexInput(dctx, 1, vinStreams, 1, vinAttrs);
    AfxAssertObjects(1, &sky->skyVin, afxFcc_VIN);

    avxSamplerConfig smpSpec = { 0 };
    smpSpec.magFilter = avxTexelFilter_LINEAR;
    smpSpec.minFilter = avxTexelFilter_LINEAR;
    smpSpec.mipFilter = avxTexelFilter_LINEAR;
    smpSpec.uvw[0] = avxTexelAddress_CLAMP;
    smpSpec.uvw[1] = avxTexelAddress_CLAMP;
    smpSpec.uvw[2] = avxTexelAddress_CLAMP;

    AfxAcquireSamplers(dctx, 1, &smpSpec, &sky->smp);
    AfxAssertObjects(1, &sky->smp, afxFcc_SAMP);

    return err;
}
