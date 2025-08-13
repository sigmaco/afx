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

// This is just a straightforward rendering tool. Do not serve as model to a efficient renderer.

#ifndef RENDERER_H
#define RENDERER_H

#include "qwadro/inc/sim/body/afxBody.h"
#include "qwadro/inc/sim/render/arxRenderContext.h"

// clustered rendering entity
AFX_DEFINE_HANDLE(afxRenderable);
AFX_OBJECT(afxRenderable)
{
    int a;
};

AFX_DEFINE_STRUCT(akxProto)
{
    afxString32     name;
    afxUri128       mdd;
    afxUri128       txd;
    afxUri128       andUri;

    afxModel        mdl;
    avxRaster       ras;
    afxAnimation    ani;
};

AFX_DEFINE_STRUCT(akxPlacement)
{
    afxUnit          id;
    afxTransform    t;
};

AFX_DEFINE_STRUCT(akxShaderConstants) // pass
{
    afxV3d wirecolor;
};

AFX_DEFINE_STRUCT(akxMaterialConstants)
{
    //avxColor    Ka; // ambient color
    afxV3d  Kd; // diffuse color
    afxV3d  Ks; // specular color
    afxReal Ns; // specular color is weighted by the specular exponent Ns. Ranges between [0...1000].
    afxReal d; // dissolution. 1.0 means fully opaque.
    afxReal Ni; // index of refraction
    afxUnit  illum; // illumination model. 0 = diffuse color, 1 = diffuse color and ambient, etc.
    afxBool hasDiffTex;
    afxBool hasSpecTex;
};

AFX_DEFINE_STRUCT(akxInstanceConstants)
{
    afxM4d  m[AVX_BUFFER_UPDATE_CAPACITY / sizeof(afxM4d)];
};

AFX_DEFINE_STRUCT(akxRendererConfig)
{
    afxDrawInputProc    dinProc;
    afxDrawInput din;
};

AFX_OBJECT(akxRenderer)
{
    void*               cachedSim;
    afxDrawSystem      cachedDsys;
    afxDrawInput        din;
    
    avxCamera           activeCam;
    afxRect             drawArea;
    avxCanvas           canv;
    afxUnit              frameCnt;
    afxUnit              frameIdx;
    struct
    {
        akxViewConstants    viewConstants;
        akxShaderConstants  shaderConstants;
        akxMaterialConstants materialConstants;
        akxInstanceConstants  objConstants;
        avxBuffer           viewUbo; // p, v
        avxBuffer           shdUbo;
        avxBuffer           mtlUbo;
        afxUnit             mtlBufOffset;
        avxBuffer           objUbo; // m
        afxUnit             mtxBufOffset;
        afxUnit             mtxBufPop;

        avxBuffer           biasMapUbo;
        afxUnit             biasMapOffset;
        afxUnit             biasMapPop;

        avxBuffer           icbo; // indirect indexed draw buffer
        afxUnit             icbOffset;
        afxUnit             indDrawCnt;


        afxByte* icboPtr;
        afxByte* mtboPtr;
        afxByte* bmboPtr;
        afxByte* mtlboPtr;

        avxFence            drawCompletedFence;
        avxFence            framePresentedFence;
    }                       framesets[3];

    afxBool                 doMdi;

    //afxM4d              p;
    //afxFrustum          viewVolume;
    afxArray            capturedNodes; // afxNode

    avxPipeline         wirePip;
    afxBool             drawVolumes;

    avxBuffer  testIbo;
    avxBuffer testVbo;

    afxDrawTechnique    testDtec;
    afxDrawTechnique    bodyDtec;
    afxDrawTechnique    blinnTestRazrDtec;
    afxDrawTechnique    tutCamUtilDtec;
    afxDrawTechnique    lightingDtec;

    avxVertexInput      rigidVin;
    avxVertexInput   vin_p3n3t2_bi_mtl_mtx_jnt;
    avxVertexInput   vin_p3j1n3t2_bi_mtl_mtx_jnt;
    avxVertexInput   vin_p3j2n3t2_bi_mtl_mtx_jnt;
    avxVertexInput   vin_p3j3n3t2_bi_mtl_mtx_jnt;
    avxVertexInput   vin_p3j4n3t2_bi_mtl_mtx_jnt;
    avxVertexInput      skinnedVin;
    avxVertexInput      testVin;

    afxPose      lp;
    afxPlacement wp[2];
};

#if 0
// https://developer.nvidia.com/vulkan-shader-resource-binding

// example for typical loops in rendering
for each view {
  bind view resources          // camera, environment...
  for each shader {
    bind shader pipeline
    bind shader resources      // shader control values
    for each material {
      bind material resources  // material parameters and textures
      for each object {
        bind object resources  // object transforms
        draw object
      }
    }
  }
}
#endif

ASX afxError    AsxRegisterPrototype(afxUnit* protoId, afxString const* name, afxUri const* mdd, afxUri const* txd, afxUri const* and);

ASX afxError    AsxSpawnEntity(afxUnit protoId, afxTransform const* t);

ASX afxError    AsxSpawnAnimatedEntity(afxUnit protoId, afxTransform const* t);

ASX afxError    AsxBeginSceneRendering(akxRenderer rnd, avxCamera cam, afxRect const* drawArea, avxCanvas canv, afxDrawContext dctx);
ASX afxError    AsxEndSceneRendering(akxRenderer rnd, afxDrawContext dctx);

ASX afxError    AfxRendererSetStar(akxRenderer rnd, afxV4d const pos, afxV3d const dir, afxV4d const Kd);

ASX afxError    AmxCmdDrawBodies(afxDrawContext dctx, akxRenderer rnd, afxReal dt, afxUnit cnt, afxBody bodies[]);

ASX afxError    AkxTestDrawIndexed(akxRenderer rnd, afxDrawContext dctx);

ASX afxError    AsxBeginSceneCapture(akxRenderer scn, avxCamera cam, afxSimulation sim, afxDrawContext dctx, afxContext sctx);

////////////////////////////////////////////////////////////////////////////////
// MASSIVE OPERATIONS                                                         //
////////////////////////////////////////////////////////////////////////////////

ASX afxError    AsxAcquireRenderers(afxSimulation sim, afxUnit cnt, akxRenderer rnd[], akxRendererConfig const config[]);


ASX void        AmxCmdRequestModel(afxSimulation sim, afxUnit id);

#endif//RENDERER_H
