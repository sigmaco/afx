#ifndef AFX_DIRECTDRAW_TEXTURE_H
#define AFX_DIRECTDRAW_TEXTURE_H

#include "qwadro/draw/avxDrawContext.h"

DLLIMPORT afxError    AfxLoadTexturesDdt(avxDrawContext dctx, afxNat cnt, afxUri const uri[], avxRaster tex[]);

DLLIMPORT afxError    AfxFetchRasterDdt(avxRaster tex, afxUri const *uri);
DLLIMPORT afxError    AfxFetchRasterRegionsDdt(avxRaster tex, afxNat cnt, avxRasterRegion const rgn[], afxUri const uri[]);

DLLIMPORT afxError    AfxPrintRasterDdt(avxRaster tex, afxUri const *uri);
DLLIMPORT afxError    AfxPrintRasterRegionsDdt(avxRaster tex, afxNat cnt, avxRasterRegion const rgn[], afxUri const uri[]);

#endif//AFX_DIRECTDRAW_TEXTURE_H