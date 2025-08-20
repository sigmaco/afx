/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *             Q W A D R O   R E N D E R I N G   I N F R A S T R U C T U R E
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This is just a straightforward rendering tool. Do not serve as model to a efficient renderer.

#ifndef AKX_RENDERER_H
#define AKX_RENDERER_H

#include "qwadro/inc/render/arxRenderable.h"

// clustered rendering entity
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

    arxModel        mdl;
    avxRaster       ras;
    arxAnimation    ani;
};

AFX_DEFINE_STRUCT(akxPlacement)
{
    afxUnit          id;
    afxTransform    t;
};

AFX_DEFINE_STRUCT(akxViewConstants) // frame
{
    afxM4d  v;
    afxM4d  iv;
    afxM4d  p;
    afxM4d  ip;
    afxM4d  pv;
    afxM4d  ipv;

    afxV4d  viewPos; // view point
    afxV2d  viewExtent;

    afxV3d starDir;
    afxV4d starPos;
    afxV4d starKd;
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
    void*    dinProc;
    arxRenderware din;
    afxSimulation sim;
};

AFX_OBJECT(akxRenderer)
{
    void*               cachedSim;
    afxDrawSystem      cachedDsys;
    arxRenderware        din;
    
    arxCamera           activeCam;
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

    arxTechnique    testDtec;
    arxTechnique    bodyDtec;
    arxTechnique    blinnTestRazrDtec;
    arxTechnique    tutCamUtilDtec;
    arxTechnique    lightingDtec;

    avxVertexInput      rigidVin;
    avxVertexInput   vin_p3n3t2_bi_mtl_mtx_jnt;
    avxVertexInput   vin_p3j1n3t2_bi_mtl_mtx_jnt;
    avxVertexInput   vin_p3j2n3t2_bi_mtl_mtx_jnt;
    avxVertexInput   vin_p3j3n3t2_bi_mtl_mtx_jnt;
    avxVertexInput   vin_p3j4n3t2_bi_mtl_mtx_jnt;
    avxVertexInput      skinnedVin;
    avxVertexInput      testVin;

    arxPose      lp;
    arxPlacement wp[2];
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

ARX afxError    ArxRegisterPrototype(afxUnit* protoId, afxString const* name, afxUri const* mdd, afxUri const* txd, afxUri const* and);

ARX afxError    ArxSpawnEntity(afxUnit protoId, afxTransform const* t);

ARX afxError    ArxSpawnAnimatedEntity(afxUnit protoId, afxTransform const* t);

ARX afxError    ArxBeginSceneRendering(akxRenderer rnd, arxCamera cam, afxRect const* drawArea, avxCanvas canv, afxDrawContext dctx);
ARX afxError    ArxEndSceneRendering(akxRenderer rnd, afxDrawContext dctx);

ARX afxError    ArxRendererSetStar(akxRenderer rnd, afxV4d const pos, afxV3d const dir, afxV4d const Kd);

ARX afxError    ArxCmdDrawBodies(afxDrawContext dctx, akxRenderer rnd, afxReal dt, afxUnit cnt, arxBody bodies[]);

ARX afxError    AkxTestDrawIndexed(akxRenderer rnd, afxDrawContext dctx);

ARX afxError    ArxBeginSceneCapture(akxRenderer scn, arxCamera cam, afxSimulation sim, afxDrawContext dctx, afxContext sctx);

////////////////////////////////////////////////////////////////////////////////
// MASSIVE OPERATIONS                                                         //
////////////////////////////////////////////////////////////////////////////////

ARX afxError    ArxAcquireRenderers(arxRenderware din, afxUnit cnt, akxRenderer rnd[], akxRendererConfig const config[]);


ARX void        ArxCmdRequestModel(arxRenderware din, afxUnit id);

#endif//AKX_RENDERER_H
