
#ifndef RENDERER_H
#define RENDERER_H

#include "../../afxQwadro.h"

AFX_DEFINE_STRUCT(afxViewConstants) // frame
{
    afxM4d v;
    afxM4d p;
    afxM4d vp;
    afxV3d viewPos;
    afxV3d sunPos;
    afxV3d sunKd;
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
    afxM4d  m;
    afxM3d  n;
};

AFX_DEFINE_STRUCT(afxRenderer)
{
    afxSimulation       sim;
    afxDrawInput        din;
    afxDrawContext      dctx;
    afxDrawOutput       dout;

    afxCamera           cam;
    afxM4d              p;
    afxFrustum          viewVolume;
    afxArray            capturedNodes; // afxNode

    afxSkyType          skyType;
    afxVertexBuffer     skyGeom;
    afxTexture          skyTex;
    afxColor            skyEmissiveColor;
    afxColor            skyAmbientColor;
    afxPipeline         skyPip;
    afxSampler          skySmp;

    afxPipeline         wirePip;
    afxBool             drawVolumes;

    afxPipeline         basePip;

    struct
    {
        afxSurface      depth;
        afxCanvas       canv;

        afxBuffer       viewConstants; // p, v
        afxBuffer       shdConstants;
        afxBuffer       mtlConstants;
        afxBuffer       objConstants; // m

        afxLego         viewLego;
        afxLego         shdLego;
        afxLego         mtlLego;
        afxLego         objLego;
    }                   sets[2];
    afxNat              frameCnt;
    afxNat              frameIdx;

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

AFX afxError _AfxBuildRenderer(afxRenderer *renderer, afxDrawOutput dout, afxSimulation sim);
AFX afxError _AfxDismantleRenderer(afxRenderer *renderer);

AFX afxError _AfxRendererDoWork(afxDrawInput din, afxNat qid, afxRenderer *renderer);

#endif///RENDERER_H