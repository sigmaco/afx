#ifndef AFX_GRANNY2_MODEL_H
#define AFX_GRANNY2_MODEL_H

#include "qwadro/sim/dev/afxSimulation.h"


#define GRANNY2 DLLEXPORT

#if 0
#ifndef __e2granny2__
#   define GRANNY2 DLLIMPORT
#else
#   define GRANNY2 DLLEXPORT
#endif
#endif

GRANNY2 afxResult AfxLoadAssetsFromGranny2File(afxSimulation sim, afxUri const *uri, afxArray *assets);

GRANNY2 afxModel AkxLoadModelsFromGrn3d2(afxSimulation sim, afxUri const *uri, afxNat mdlIdx);

GRANNY2 akxAnimation AkxLoadAnimationsFromGrn3d2(afxSimulation sim, afxUri const *uri, afxNat animIdx);

#endif//AFX_GRANNY2_MODEL_H