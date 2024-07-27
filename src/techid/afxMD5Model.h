#ifndef AFX_MD5_MODEL_H
#define AFX_MD5_MODEL_H

#include "qwadro/sim/dev/afxSimulation.h"
#include "qwadro/sim/io/akxAsset.h"

DLLEXPORT afxError AfxLoadAssetsFromMd5(afxSimulation sim, afxFlags flags, afxNat cnt, afxUri const file[], akxAsset cad[]);

#endif//AFX_MD5_MODEL_H