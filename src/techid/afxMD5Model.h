#ifndef AFX_MD5_MODEL_H
#define AFX_MD5_MODEL_H

#include "qwadro/sim/afxSimulation.h"
#include "qwadro/sim/awxAsset.h"

DLLEXPORT afxError AfxLoadAssetsFromMd5(afxSimulation sim, afxFlags flags, afxNat cnt, afxUri const file[], awxAsset cad[]);

#endif//AFX_MD5_MODEL_H