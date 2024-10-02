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

#include "qwadro/inc/sim/afxBody.h"
#include "qwadro/inc/sim/rendering/akxSky.h"

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
    afxRaster       ras;
    afxAnimation    ani;
};

AFX_DEFINE_STRUCT(akxPlacement)
{
    afxNat          id;
    afxTransform    t;
};

AMX afxError AkxRegisterPrototype(afxNat* protoId, afxString const* name, afxUri const* mdd, afxUri const* txd, afxUri const* and);

AMX afxError AkxSpawnEntity(afxNat protoId, afxTransform const* t);

AMX afxError AkxSpawnAnimatedEntity(afxNat protoId, afxTransform const* t);

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
    //afxColor    Ka; // ambient color
    afxV3d  Kd; // diffuse color
    afxV3d  Ks; // specular color
    afxReal Ns; // specular color is weighted by the specular exponent Ns. Ranges between [0...1000].
    afxReal d; // dissolution. 1.0 means fully opaque.
    afxReal Ni; // index of refraction
    afxNat  illum; // illumination model. 0 = diffuse color, 1 = diffuse color and ambient, etc.
    afxBool hasDiffTex;
    afxBool hasSpecTex;
};

AFX_DEFINE_STRUCT(akxInstanceConstants)
{
    afxM4d  m;
    afxM4d  w[32];
};

AFX_DEFINE_STRUCT(akxRendererConfig)
{
    afxDrawInputProc    dinProc;
    afxDrawInput din;
};

AFX_OBJECT(akxRenderer)
{
    void*               cachedSim;
    afxDrawContext      cachedDctx;
    afxDrawInput        din;
    
    afxCamera           activeCam;
    afxRect             drawArea;
    avxCanvas           canv;
    afxNat              frameCnt;
    afxNat              frameIdx;
    struct
    {
        akxViewConstants    viewConstants;
        akxShaderConstants  shaderConstants;
        akxMaterialConstants materialConstants;
        akxInstanceConstants  objConstants;
        afxBuffer           viewConstantsBuffer; // p, v
        afxBuffer           shdConstantsBuffer;
        afxBuffer           mtlConstantsBuffer;
        afxBuffer           objConstantsBuffer; // m

    }                       framesets[3];

    //afxM4d              p;
    //afxFrustum          viewVolume;
    afxArray            capturedNodes; // akxNode

    akxSky                  sky;
    afxBool                 skyEnabled;

    avxPipeline         wirePip;
    afxBool             drawVolumes;

    afxBuffer  testIbo;
    afxBuffer testVbo;

    afxDrawTechnique    testDtec;
    afxDrawTechnique    bodyDtec;
    afxDrawTechnique    blinnTestRazrDtec;
    afxDrawTechnique    tutCamUtilDtec;
    afxDrawTechnique    lightingDtec;

    avxVertexInput      rigidVin;
    avxVertexInput      skinnedVin;
    avxVertexInput      testVin;

    afxPose      lp;
    afxPoseBuffer wp;
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

AMX afxError    AkxCmdBeginSceneRendering(avxCmdb cmdb, akxRenderer rnd, afxCamera cam, afxRect const* drawArea, avxCanvas canv);
AMX afxError    AkxCmdEndSceneRendering(avxCmdb cmdb, akxRenderer rnd);

AMX afxError    AfxRendererSetStar(akxRenderer rnd, afxV4d const pos, afxV3d const dir, afxV4d const Kd);

AMX afxError    AkxCmdDrawBodies(avxCmdb cmdb, akxRenderer rnd, afxReal dt, afxNat cnt, afxBody bodies[]);

AMX afxError    AkxCmdDrawTestIndexed(avxCmdb cmdb, akxRenderer rnd);


AMX afxError    AfxBufferizeMesh(afxDrawInput din, afxMesh msh);

AMX afxError    AkxBufferizeVertexData(afxDrawInput din, afxGeometry vtd);

AMX afxError    AkxCmdBindVertexDataCache(avxCmdb cmdb, afxNat slotIdx, afxGeometry vtd);

AMX afxError    AkxBeginSceneCapture(akxRenderer scn, afxCamera cam, afxSimulation sim, avxCmdb cmdb);

////////////////////////////////////////////////////////////////////////////////
// MASSIVE OPERATIONS                                                         //
////////////////////////////////////////////////////////////////////////////////

AMX afxError    AkxAcquireRenderers(afxSimulation sim, afxNat cnt, akxRenderer rnd[], akxRendererConfig const config[]);


AMX void        AkxCmdRequestModel(afxSimulation sim, afxNat id);

#endif//RENDERER_H
