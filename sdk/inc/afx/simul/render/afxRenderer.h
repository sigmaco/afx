
#ifndef RENDERER_H
#define RENDERER_H

#include "../../afxQwadro.h"

AFX_DEFINE_STRUCT(afxViewConstants)
{
    afxV3d camPos;
    afxM4d c;
    afxM4d v;
    afxM4d p;
    afxM4d vp;
};

AFX_DEFINE_STRUCT(afxShaderConstants)
{
    afxV3d wirecolor;
};

AFX_DEFINE_STRUCT(afxMaterialConstants)
{
    afxColor color;
    afxBool hasDiffTex;
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
        afxSurface  depth;
        afxCanvas   canv;

        afxBuffer   viewConstants; // p, v
        afxBuffer   shdConstants;
        afxBuffer   mtlConstants;
        afxBuffer   objConstants; // m

        afxLego     viewLego;
        afxLego     shdLego;
        afxLego     mtlLego;
        afxLego     objLego;
    }               sets[2];

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

//afxResult _AfxSetUpRenderer(afxDrawInput din, afxDrawOutput dout);

#endif///RENDERER_H