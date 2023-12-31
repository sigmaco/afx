#ifndef AFX_DIRECTDRAW_SURFACE_H
#define AFX_DIRECTDRAW_SURFACE_H

#include "qwadro/draw/avxDrawContext.h"

_AFXIMPORT afxError    AfxLoadTexturesDds(avxDrawContext dctx, afxNat cnt, afxUri const uri[], avxRaster tex[]);

_AFXIMPORT afxError    AfxFetchRasterDds(avxRaster tex, afxUri const *uri);
_AFXIMPORT afxError    AfxFetchRasterRegionsDds(avxRaster tex, afxNat cnt, avxRasterRegion const rgn[], afxUri const uri[]);

_AFXIMPORT afxError    AfxPrintRasterDds(avxRaster tex, afxUri const *uri);
_AFXIMPORT afxError    AfxPrintRasterRegionsDds(avxRaster tex, afxNat cnt, avxRasterRegion const rgn[], afxUri const uri[]);

#endif//AFX_DIRECTDRAW_SURFACE_H