/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *              T H E   Q W A D R O   E X E C U T I O N   E C O S Y S T E M
 *
 *                                   Public Test Build
 *                   (c) 2017 SIGMA Technology Group — Federação SIGMA
 *                                    www.sigmaco.org
 */

#define _AFX_SIMULATION_C
#include "afx/sim/afxSimulation.h"
#include "afx/sim/rendering/afxSky.h"
#include "afx/draw/afxDrawScript.h"
#include "afx/math/afxQuaternion.h"
#include "afx/math/afxMatrix.h"

_AFX void AfxStepSky(afxSky* sky, afxReal dt)
{
    sky->currRot += sky->rotSpeed * dt;
    AfxQuatFromAxisAngle(sky->rotQuat, sky->rotPivot, sky->currRot);
    AfxM4dFromQuat(sky->rotMtx, sky->rotQuat);
}

_AFX afxError AfxDrawSky(afxDrawScript dscr, afxSky* sky)
{
    afxError err = AFX_ERR_NONE;

    AfxCmdBindPipeline(dscr, 0, sky->skyPip);
    AfxCmdBindTextures(dscr, 0, 1, 1, &sky->smp, &sky->cubemap);

    AfxCmdBindVertexSources(dscr, 0, 1, (afxBuffer[]) { sky->cube }, NIL, NIL);
    AfxCmdResetVertexStreams(dscr, 1, NIL, (afxNat32[]) { sizeof(afxV3d) }, NIL);
    AfxCmdResetVertexAttributes(dscr, 1, NIL, (afxVertexFormat[]) { afxVertexFormat_V3D }, NIL, NIL);
    AfxCmdDraw(dscr, 36, 1, 0, 0);
    
    return err;
}

_AFX afxError AfxBuildSkybox(afxSky* sky, afxSimulation sim)
{
    afxError err = NIL;
    afxDrawContext dctx = sim->dctx;

    AfxSetV3d(sky->rotPivot, 0, 0, 1);
    sky->cubemapColorIntensity = 1.f;
    sky->rotSpeed = 0.f;
    sky->currRot = 0.f;

    // sky
    afxUri cubeUri[6];
    AfxUriWrapLiteral(&cubeUri[0], "art/skybox/day/right.tga", 0);
    AfxUriWrapLiteral(&cubeUri[1], "art/skybox/day/left.tga", 0);
    AfxUriWrapLiteral(&cubeUri[2], "art/skybox/day/bottom.tga", 0);
    AfxUriWrapLiteral(&cubeUri[3], "art/skybox/day/top.tga", 0);
    AfxUriWrapLiteral(&cubeUri[4], "art/skybox/day/front.tga", 0);
    AfxUriWrapLiteral(&cubeUri[5], "art/skybox/day/back.tga", 0);
    sky->cubemap = AfxAssembleCubemapTexture(dctx, NIL, cubeUri);

    AfxSetColor(sky->ambientColor, 0.1, 0.1, 0.1, 1);
    AfxSetColor(sky->emissiveColor, 0.1, 0.1, 0.1, 1);

    afxReal skyboxVertices[] =
    {
        // positions          
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

    for (afxNat i = 0; i < sizeof(skyboxVertices) / sizeof(skyboxVertices[0]); i++)
        skyboxVertices[i] *= 100;

    afxBufferSpecification vboSpec = { 0 };
    vboSpec.siz = sizeof(skyboxVertices);
    vboSpec.src = skyboxVertices;
    vboSpec.usage = afxBufferUsage_VERTEX;

    AfxAcquireBuffers(dctx, 1, &sky->cube, &vboSpec);
    AfxAssertObjects(1, &sky->cube, afxFcc_BUF);

    afxUri uri;
    AfxUriWrapLiteral(&uri, "data/pipeline/skybox.xsh.xml", 0);
    sky->skyPip = AfxAssemblePipelineFromXsh(dctx, &uri);
    sky->type = afxSkyType_BOX;

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
