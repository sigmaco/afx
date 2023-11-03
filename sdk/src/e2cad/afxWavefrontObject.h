#ifndef AFX_WAVEFRONT_OBJECT_H
#define AFX_WAVEFRONT_OBJECT_H

#include "afx/sim/afxSimulation.h"
#include "afx/sim/afxAsset.h"

_AFXEXPORT afxError AfxLoadAssetsFromWavefrontObj(afxSimulation sim, afxFlags flags, afxNat cnt, afxUri const file[], afxAsset cad[]);


#endif//AFX_WAVEFRONT_OBJECT_H