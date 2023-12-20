#ifndef AFX_WAVEFRONT_OBJECT_H
#define AFX_WAVEFRONT_OBJECT_H

#include "qwadro/sim/awxSimulation.h"
#include "qwadro/sim/awxAsset.h"

_AFXEXPORT afxError AfxLoadAssetsFromWavefrontObj(awxSimulation sim, afxFlags flags, afxNat cnt, afxUri const file[], awxAsset cad[]);


#endif//AFX_WAVEFRONT_OBJECT_H