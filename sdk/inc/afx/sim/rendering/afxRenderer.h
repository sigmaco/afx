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

#ifndef RENDERER_H
#define RENDERER_H

#include "afx/sim/anim/afxBody.h"
#include "afx/sim/rendering/afxSky.h"

AFX_DEFINE_STRUCT(afxViewConstants) // frame
{
    afxM4d  v;
    afxM4d  iv;
    afxM4d  p;
    afxM4d  ip;
    afxM4d  vp;
    afxM4d  ivp;

    afxV4d  viewpoint; // view point
    afxV2d  viewExtent;

    afxV3d starDir;
    afxV4d starPos;
    afxV4d starKd;
};

AFX_DEFINE_STRUCT(afxShaderConstants) // pass
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

AFX_DEFINE_STRUCT(afxInstanceConstants)
{
    afxNat  boneCnt;
    afxM4d  m;
    afxM4d  w[32];
};

AFX_DEFINE_STRUCT(afxRendererConfig)
{
    afxError (*dinProc)(afxDrawInput din, afxNat thrUnitIdx);
};

AFX_OBJECT(afxRenderer)
{
    afxInstance           obj;
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
        afxViewConstants    viewConstants;
        afxShaderConstants  shaderConstants;
        afxMaterialConstants materialConstants;
        afxInstanceConstants  objConstants;
        afxBuffer           viewConstantsBuffer; // p, v
        afxBuffer           shdConstantsBuffer;
        afxBuffer           mtlConstantsBuffer;
        afxBuffer           objConstantsBuffer; // m

        //afxTexture          depthSurf;
    }                       framesets[3];

    afxM4d              p;
    afxFrustum          viewVolume;
    afxArray            capturedNodes; // afxNode

    afxSky                  sky;
    afxBool                 skyEnabled;

    afxPipeline         wirePip;
    afxBool             drawVolumes;

    afxBuffer  testIbo;
    afxBuffer testVbo;
    afxPipeline testPip;

    afxPipeline    rigidBodyPip;
    afxPipeline    skinnedBodyPip;
    
    afxPose*      lp;
    afxWorldPose* wp;
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

AFX afxError AfxAcquireRenderers(afxSimulation sim, afxNat cnt, afxRenderer rnd[], afxRendererConfig const config[]);

AFX afxError AfxBeginSceneRendering(afxDrawScript dscr, afxRenderer rnd, afxCamera cam, afxRect const* drawArea, afxTexture surf);
AFX afxError AfxEndSceneRendering(afxDrawScript dscr, afxRenderer rnd);

AFX afxError AfxRendererSetStar(afxRenderer rnd, afxV4d const pos, afxV3d const dir, afxV4d const Kd);

AFX afxError AfxDrawBodies(afxDrawScript dscr, afxRenderer rnd, afxNat cnt, afxBody bodies[]);

AFX afxError AfxDrawTestIndexed(afxDrawScript dscr, afxRenderer rnd);

#endif///RENDERER_H