#ifndef AFX_MD5_MODEL_H
#define AFX_MD5_MODEL_H

#include "qwadro/sim/awxSimulation.h"
#include "qwadro/sim/awxAsset.h"

_AFXEXPORT afxError AfxLoadAssetsFromMd5(awxSimulation sim, afxFlags flags, afxNat cnt, afxUri const file[], awxAsset cad[]);

#endif//AFX_MD5_MODEL_H