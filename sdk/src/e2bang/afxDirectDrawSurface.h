#ifndef AFX_DIRECTDRAW_SURFACE_H
#define AFX_DIRECTDRAW_SURFACE_H

#include "afx/draw/afxDrawContext.h"

_AFXIMPORT afxError    AfxLoadTexturesDds(afxDrawContext dctx, afxNat cnt, afxUri const uri[], afxRaster tex[]);

_AFXIMPORT afxError    AfxFetchRasterDds(afxRaster tex, afxUri const *uri);
_AFXIMPORT afxError    AfxFetchRasterRegionsDds(afxRaster tex, afxNat cnt, afxRasterRegion const rgn[], afxUri const uri[]);

_AFXIMPORT afxError    AfxPrintRasterDds(afxRaster tex, afxUri const *uri);
_AFXIMPORT afxError    AfxPrintRasterRegionsDds(afxRaster tex, afxNat cnt, afxRasterRegion const rgn[], afxUri const uri[]);

#endif//AFX_DIRECTDRAW_SURFACE_H