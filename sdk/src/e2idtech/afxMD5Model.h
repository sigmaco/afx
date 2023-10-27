#ifndef AFX_MD5_MODEL_H
#define AFX_MD5_MODEL_H

#include "afx/sim/afxSimulation.h"
#include "afx/sim/afxAsset.h"

_AFXEXPORT afxError AfxLoadAssetsFromMd5(afxSimulation sim, afxFlags flags, afxNat cnt, afxUri const file[], afxAsset cad[]);

#endif//AFX_MD5_MODEL_H