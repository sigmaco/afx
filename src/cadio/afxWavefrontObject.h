#ifndef AFX_WAVEFRONT_OBJECT_H
#define AFX_WAVEFRONT_OBJECT_H

#include "qwadro/sim/afxSimulation.h"
#include "qwadro/sim/awxAsset.h"

DLLEXPORT afxError AfxLoadAssetsFromWavefrontObj(afxSimulation sim, afxFlags flags, afxNat cnt, afxUri const file[], awxAsset cad[]);


#endif//AFX_WAVEFRONT_OBJECT_H