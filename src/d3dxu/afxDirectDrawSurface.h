#ifndef AFX_DIRECTDRAW_SURFACE_H
#define AFX_DIRECTDRAW_SURFACE_H

#include "qwadro/draw/avxDrawContext.h"

DLLIMPORT afxError    AfxLoadTexturesDds(avxDrawContext dctx, afxNat cnt, afxUri const uri[], avxRaster tex[]);

DLLIMPORT afxError    AfxFetchRasterDds(avxRaster tex, afxUri const *uri);
DLLIMPORT afxError    AfxFetchRasterRegionsDds(avxRaster tex, afxNat cnt, avxRasterRegion const rgn[], afxUri const uri[]);

DLLIMPORT afxError    AfxPrintRasterDds(avxRaster tex, afxUri const *uri);
DLLIMPORT afxError    AfxPrintRasterRegionsDds(avxRaster tex, afxNat cnt, avxRasterRegion const rgn[], afxUri const uri[]);

#endif//AFX_DIRECTDRAW_SURFACE_H