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
 *                       (c) 2017 SIGMA, Engineering In Technology
 *                             <https://sigmaco.org/qwadro/>
 */

#define _AFX_SIM_C
#define _AFX_SIMULATION_C
#include "qwadro/sim/afxSimulation.h"
#include "qwadro/sim/rendering/awxSky.h"
#include "qwadro/draw/dev/afxDrawStream.h"
#include "qwadro/draw/pipe/afxDrawOps.h"
#include "qwadro/draw/pipe/afxVertexInput.h"
#include "qwadro/math/afxQuaternion.h"
#include "qwadro/math/afxMatrix.h"

_AKX void AfxStepSky(awxSky* sky, afxReal dt)
{
    sky->currRot += sky->rotSpeed * dt;
    AfxMakeQuatFromAxialRotation(sky->rotQuat, sky->rotPivot, sky->currRot);
    AfxRotationM4dFromQuat(sky->rotMtx, sky->rotQuat);
}

_AKX afxError AfxDrawSky(afxDrawStream diob, awxSky* sky)
{
    afxError err = AFX_ERR_NONE;

    AfxCmdBindPipeline(diob, 0, sky->skyPip);
    AfxCmdBindVertexInput(diob, sky->skyVin);
    AfxCmdBindRasters(diob, 0, 1, 1, &sky->smp, &sky->cubemap);

    AfxCmdBindVertexSources(diob, 0, 1, (afxBuffer[]) { sky->cube }, NIL, NIL, (afxNat32[]) {sizeof(afxV3d)});
    //AfxCmdResetVertexStreams(diob, 1, NIL, (afxNat32[]) { sizeof(afxV3d) }, NIL);
    //AfxCmdResetVertexAttributes(diob, 1, NIL, (afxVertexFormat[]) { afxVertexFormat_V3D }, NIL, NIL);
    AfxCmdDraw(diob, 0, 1, 0, 36);
    return err;
}

_AKX afxError AfxBuildSkybox(awxSky* sky, afxSimulation sim)
{
    afxError err = NIL;
    afxDrawContext dctx = sim->dctx;

    AfxSetV3d(sky->rotPivot, 0, 0, 1);
    sky->cubemapColorIntensity = 1.f;
    sky->rotSpeed = 0.f;
    sky->currRot = 0.f;

    // sky
    afxUri cubeUri[6];
#if !0
#if 0    
    AfxMakeUri(&cubeUri[0], "art/skybox/day/right.tga", 0);
    AfxMakeUri(&cubeUri[1], "art/skybox/day/left.tga", 0);
    AfxMakeUri(&cubeUri[3], "art/skybox/day/top.tga", 0);
    AfxMakeUri(&cubeUri[2], "art/skybox/day/bottom.tga", 0);
    AfxMakeUri(&cubeUri[4], "art/skybox/day/front.tga", 0);
    AfxMakeUri(&cubeUri[5], "art/skybox/day/back.tga", 0);
#else
    AfxMakeUri(&cubeUri[0], "art/skybox/purple/right.tga", 0); // x+
    AfxMakeUri(&cubeUri[1], "art/skybox/purple/left.tga", 0); // x-
    AfxMakeUri(&cubeUri[3], "art/skybox/purple/top.tga", 0);
    AfxMakeUri(&cubeUri[2], "art/skybox/purple/bottom.tga", 0); // y-
    AfxMakeUri(&cubeUri[4], "art/skybox/purple/front.tga", 0);
    AfxMakeUri(&cubeUri[5], "art/skybox/purple/back.tga", 0);

#endif
#else
    AfxMakeUri(&cubeUri[0], "art/skybox/envmap_interstellar/interstellar_rt.tga", 0); // x+
    AfxMakeUri(&cubeUri[1], "art/skybox/envmap_interstellar/interstellar_lf.tga", 0); // x-
    AfxMakeUri(&cubeUri[3], "art/skybox/envmap_interstellar/interstellar_up.tga", 0);
    AfxMakeUri(&cubeUri[2], "art/skybox/envmap_interstellar/interstellar_dn.tga", 0); // y-
    AfxMakeUri(&cubeUri[4], "art/skybox/envmap_interstellar/interstellar_ft.tga", 0);
    AfxMakeUri(&cubeUri[5], "art/skybox/envmap_interstellar/interstellar_bk.tga", 0);
#endif

    afxUri facesUri[6];
    AfxMakeUri(&facesUri[0], "right.tga", 0); // x+
    AfxMakeUri(&facesUri[1], "left.tga", 0); // x-
    AfxMakeUri(&facesUri[3], "top.tga", 0);
    AfxMakeUri(&facesUri[2], "bottom.tga", 0); // y-
    AfxMakeUri(&facesUri[4], "front.tga", 0);
    AfxMakeUri(&facesUri[5], "back.tga", 0);

    afxUri cubeDir;
    AfxMakeUri(&cubeDir, "art/skybox/purple", 0);

    sky->cubemap = AfxAssembleCubemapRasters(dctx, afxRasterUsage_SAMPLING, afxRasterFlag_CUBEMAP, &cubeDir, facesUri);
    //AfxFlipRaster(sky->cubemap, FALSE, TRUE);


    AfxSetColor(sky->ambientColor, 0.1, 0.1, 0.1, 1);
    AfxSetColor(sky->emissiveColor, 0.1, 0.1, 0.1, 1);

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
    AfxMakeUri(&uri, "data/pipeline/skybox/skybox.xsh.xml", 0);
    sky->skyPip = AfxAssemblePipelineFromXsh(dctx, NIL, &uri);
    sky->type = awxSkyType_BOX;

    afxVertexInputStream const vinStreams[] =
    {
        {
            .instanceRate = 0,
            .slotIdx = 0
        }
    };
    afxVertexInputAttr const vinAttrs[] =
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

    afxSamplerConfig smpSpec = { 0 };
    smpSpec.magFilter = afxTexelFilter_LINEAR;
    smpSpec.minFilter = afxTexelFilter_LINEAR;
    smpSpec.mipmapFilter = afxTexelFilter_LINEAR;
    smpSpec.uvw[0] = afxTexelAddress_CLAMP;
    smpSpec.uvw[1] = afxTexelAddress_CLAMP;
    smpSpec.uvw[2] = afxTexelAddress_CLAMP;

    AfxAcquireSamplers(dctx, 1, &smpSpec, &sky->smp);
    AfxAssertObjects(1, &sky->smp, afxFcc_SAMP);

    return err;
}
