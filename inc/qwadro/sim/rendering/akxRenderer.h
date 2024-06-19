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

#include "qwadro/space/afxBody.h"
#include "qwadro/sim/rendering/akxSky.h"

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

AFX_DEFINE_STRUCT(afxMaterialConstants)
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
};

AFX_OBJECT(akxRenderer)
{
    void*               cachedSim;
    afxDrawContext      cachedDctx;
    afxDrawInput        din;
    
    avxCamera           activeCam;
    afxRect             drawArea;
    avxCanvas           canv;
    afxNat              frameCnt;
    afxNat              frameIdx;
    struct
    {
        akxViewConstants    viewConstants;
        akxShaderConstants  shaderConstants;
        afxMaterialConstants materialConstants;
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
    avxRasterizer testRazr;

    avxRasterizer    rigidBodyRazr;
    avxRasterizer    skinnedBodyRazr;

    avxRasterizer    blinnTestRazr;
    avxRasterizer    tutCamUtil;
    
    avxRasterizer lighting;

    akxPose      lp;
    akxPoseBuffer wp;
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

AKX afxError AkxCmdBeginSceneRendering(avxCmdb cmdb, akxRenderer rnd, avxCamera cam, afxRect const* drawArea, avxCanvas canv);
AKX afxError AkxCmdEndSceneRendering(avxCmdb cmdb, akxRenderer rnd);

AKX afxError AfxRendererSetStar(akxRenderer rnd, afxV4d const pos, afxV3d const dir, afxV4d const Kd);

AKX afxError AkxCmdDrawBodies(avxCmdb cmdb, akxRenderer rnd, afxReal dt, afxNat cnt, afxBody bodies[]);

AKX afxError AkxCmdDrawTestIndexed(avxCmdb cmdb, akxRenderer rnd);

////////////////////////////////////////////////////////////////////////////////
// MASSIVE OPERATIONS                                                         //
////////////////////////////////////////////////////////////////////////////////

AKX afxError AkxAcquireRenderers(afxSimulation sim, afxNat cnt, akxRenderer rnd[], akxRendererConfig const config[]);


AKX void    AkxCmdRequestModel(afxSimulation sim, afxNat id);

#endif//RENDERER_H
