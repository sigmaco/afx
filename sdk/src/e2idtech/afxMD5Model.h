#ifndef AFX_MD5_MODEL_H
#define AFX_MD5_MODEL_H

#include "afx/sim/afxSimulation.h"

_AFXIMPORT afxResult AfxLoadAssetsFromMD5Model(afxSimulation sim, afxUri const *uri, afxArray *assets);

_AFXIMPORT afxResult AfxSimulationLoadMD5Assets(afxSimulation sim, afxUri const *uri, afxArray *assets);

#endif//AFX_MD5_MODEL_H