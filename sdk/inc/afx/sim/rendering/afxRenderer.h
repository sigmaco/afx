/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                      S I G M A   T E C H N O L O G Y   G R O U P
 *
 *                                   Public Test Build
 *                               (c) 2017 Federação SIGMA
 *                                    www.sigmaco.org
 */

#ifndef RENDERER_H
#define RENDERER_H

#include "afx/sim/anim/afxBody.h"

AFX_DEFINE_HANDLE(afxRenderer)

typedef enum afxSkyType
{
    AFX_SKY_BOX = 1,
    AFX_SKY_DOME,
} afxSkyType;

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

AFX_DEFINE_STRUCT(afxObjectConstants)
{
    afxNat  boneCnt;
    afxM4d  m;
    afxM4d  w[32];
};

AFX_OBJECT(afxRenderer)
{
    afxObject           obj;
    void*               cachedSim;
    afxDrawContext      cachedDctx;
    afxDrawInput        din;
    afxDrawOutput       dout;

    afxNat              activeOutputBufIdx;
    afxDrawInput        activeInput;
    afxDrawScript       activeScript;
    afxCamera           activeCamera;
    afxRect             drawArea;
    afxCanvas           canv;
    afxNat              frameCnt;
    afxNat              frameIdx;
    struct
    {
        afxViewConstants    viewConstants;
        afxShaderConstants  shaderConstants;
        afxMaterialConstants materialConstants;
        afxObjectConstants  objConstants;
        afxBuffer           viewConstantsBuffer; // p, v
        afxBuffer           shdConstantsBuffer;
        afxBuffer           mtlConstantsBuffer;
        afxBuffer           objConstantsBuffer; // m
    }                       framesets[3];

    afxM4d              p;
    afxFrustum          viewVolume;
    afxArray            capturedNodes; // afxNode

    struct
    {
        afxSkyType          type;
        afxVertexBuffer     cube;
        afxTexture          cubemap;
        afxColor            emissiveColor;
        afxColor            ambientColor;
        afxPipeline    skyPip;
        afxSampler          smp;
    }                       sky;

    afxPipeline         wirePip;
    afxBool             drawVolumes;

    afxPipeline    rigidBodyPip;
    afxPipeline    skinnedBodyPip;

};

#if 0
// https://developer.nvidia.com/vulkan-shader-resource-binding

// example for typical loops in rendering
for each view{
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

AFX afxError AfxRendererBindOutput(afxRenderer rnd, afxDrawOutput dout);

AFX afxError AfxRendererBeginScene(afxRenderer rnd, afxCamera cam);
AFX afxError AfxRendererEndScene(afxRenderer rnd);

AFX afxError AfxRendererSetStar(afxRenderer rnd, afxV4d const pos, afxV3d const dir, afxV4d const Kd);

AFX afxError AfxRendererDrawSky(afxRenderer rnd, afxBool clear);
AFX afxError AfxRendererDrawBody(afxRenderer rnd, afxBody bod);

#endif///RENDERER_H