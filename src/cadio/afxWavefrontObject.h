#ifndef AFX_WAVEFRONT_OBJECT_H
#define AFX_WAVEFRONT_OBJECT_H

#include "qwadro/sim/dev/afxSimulation.h"
#include "qwadro/sim/io/akxAsset.h"

DLLEXPORT afxModel AfxLoadAssetsFromWavefrontObj(afxSimulation sim, afxUri const* file);

#endif//AFX_WAVEFRONT_OBJECT_H
