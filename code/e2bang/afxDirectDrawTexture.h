#ifndef AFX_DIRECTDRAW_TEXTURE_H
#define AFX_DIRECTDRAW_TEXTURE_H

#include "qwadro/draw/avxDrawContext.h"

_AFXIMPORT afxError    AfxLoadTexturesDdt(avxDrawContext dctx, afxNat cnt, afxUri const uri[], avxRaster tex[]);

_AFXIMPORT afxError    AfxFetchRasterDdt(avxRaster tex, afxUri const *uri);
_AFXIMPORT afxError    AfxFetchRasterRegionsDdt(avxRaster tex, afxNat cnt, avxRasterRegion const rgn[], afxUri const uri[]);

_AFXIMPORT afxError    AfxPrintRasterDdt(avxRaster tex, afxUri const *uri);
_AFXIMPORT afxError    AfxPrintRasterRegionsDdt(avxRaster tex, afxNat cnt, avxRasterRegion const rgn[], afxUri const uri[]);

#endif//AFX_DIRECTDRAW_TEXTURE_H