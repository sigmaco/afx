#ifndef AFX_WAVEFRONT_OBJECT_H
#define AFX_WAVEFRONT_OBJECT_H

#include "qwadro/sim/afxSimulation.h"
#include "qwadro/sim/akxAsset.h"

DLLEXPORT afxError AfxLoadAssetsFromWavefrontObj(afxSimulation sim, afxFlags flags, afxNat cnt, afxUri const file[], akxAsset cad[]);


#endif//AFX_WAVEFRONT_OBJECT_H