#ifndef AFX_WAVEFRONT_OBJECT_H
#define AFX_WAVEFRONT_OBJECT_H

#include "afx/sim/afxSimulation.h"

#ifndef __e2cad__
#   ifdef _DEBUG
#       define CAD _AFXIMPORT extern 
#       define CADINL _AFXIMPORT extern inline
#   else
#       define CAD _AFXIMPORT extern 
#       define CADNL _AFXIMPORT extern inline
#   endif
#else
#   ifdef _DEBUG
#       define _CAD _AFXEXPORT
#       define CAD _AFXEXPORT extern 
#       define _CADINL _AFXEXPORT inline
#       define CADINL _AFXEXPORT extern inline
#   else
#       define _CAD _AFXEXPORT
#       define CAD _AFXEXPORT extern 
#       define _CADINL _AFXEXPORT inline
#       define CADINL _AFXEXPORT extern inline
#   endif
#endif//__e2cad__

CAD afxResult AfxLoadAssetsFromWavefrontObject(afxSimulation sim, afxUri const *uri, afxArray *assets);

CAD afxResult AfxSimulationLoadObjAssets(afxSimulation sim, afxUri const *uri, afxArray *assets);

#endif//AFX_WAVEFRONT_OBJECT_H