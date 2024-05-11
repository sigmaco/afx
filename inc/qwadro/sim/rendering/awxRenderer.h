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
 *                       (c) 2017 SIGMA, Engitech, Scitech, Serpro
 *                             <https://sigmaco.org/qwadro/>
 */

// This is just a straightforward rendering tool. Do not serve as model to a efficient renderer.

#ifndef RENDERER_H
#define RENDERER_H

#include "qwadro/sim/motion/awxBody.h"
#include "qwadro/sim/rendering/awxSky.h"

AFX_DEFINE_STRUCT(awxViewConstants) // frame
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

AFX_DEFINE_STRUCT(awxShaderConstants) // pass
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

AFX_DEFINE_STRUCT(awxInstanceConstants)
{
    afxM4d  m;
    afxM4d  w[32];
};

AFX_DEFINE_STRUCT(awxRendererConfig)
{
    afxDrawInputProc    dinProc;
};

AFX_OBJECT(awxRenderer)
{
    void*               cachedSim;
    afxDrawContext      cachedDctx;
    afxDrawInput        din;
    
    afxCamera           activeCam;
    afxRect             drawArea;
    afxCanvas           canv;
    afxNat              frameCnt;
    afxNat              frameIdx;
    struct
    {
        awxViewConstants    viewConstants;
        awxShaderConstants  shaderConstants;
        afxMaterialConstants materialConstants;
        awxInstanceConstants  objConstants;
        afxBuffer           viewConstantsBuffer; // p, v
        afxBuffer           shdConstantsBuffer;
        afxBuffer           mtlConstantsBuffer;
        afxBuffer           objConstantsBuffer; // m

    }                       framesets[3];

    //afxM4d              p;
    //afxFrustum          viewVolume;
    afxArray            capturedNodes; // awxNode

    awxSky                  sky;
    afxBool                 skyEnabled;

    afxPipeline         wirePip;
    afxBool             drawVolumes;

    afxBuffer  testIbo;
    afxBuffer testVbo;
    afxPipeline testPip;

    afxPipeline    rigidBodyPip;
    afxPipeline    skinnedBodyPip;

    afxPipeline    blinnTestPip;
    afxPipeline    tutCamUtil;
    
    afxPipeline lighting;

    awxPose*      lp;
    awxPoseBuffer* wp;
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

AKX afxError AwxCmdBeginSceneRendering(afxDrawStream diob, awxRenderer rnd, afxCamera cam, afxRect const* drawArea, afxCanvas canv);
AKX afxError AwxCmdEndSceneRendering(afxDrawStream diob, awxRenderer rnd);

AKX afxError AfxRendererSetStar(awxRenderer rnd, afxV4d const pos, afxV3d const dir, afxV4d const Kd);

AKX afxError AwxCmdDrawBodies(afxDrawStream diob, awxRenderer rnd, afxReal dt, afxNat cnt, awxBody bodies[]);

AKX afxError AwxCmdDrawTestIndexed(afxDrawStream diob, awxRenderer rnd);

////////////////////////////////////////////////////////////////////////////////
// MASSIVE OPERATIONS                                                         //
////////////////////////////////////////////////////////////////////////////////

AKX afxError AwxAcquireRenderers(afxSimulation sim, afxNat cnt, awxRenderer rnd[], awxRendererConfig const config[]);


AKX void    AwxCmdRequestModel(afxSimulation sim, afxNat id);

#endif//RENDERER_H
