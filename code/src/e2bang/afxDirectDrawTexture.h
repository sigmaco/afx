#ifndef AFX_DIRECTDRAW_TEXTURE_H
#define AFX_DIRECTDRAW_TEXTURE_H

#include "afx/draw/afxDrawContext.h"

_AFXIMPORT afxError    AfxLoadTexturesDdt(afxDrawContext dctx, afxNat cnt, afxUri const uri[], afxRaster tex[]);

_AFXIMPORT afxError    AfxFetchRasterDdt(afxRaster tex, afxUri const *uri);
_AFXIMPORT afxError    AfxFetchRasterRegionsDdt(afxRaster tex, afxNat cnt, afxRasterRegion const rgn[], afxUri const uri[]);

_AFXIMPORT afxError    AfxPrintRasterDdt(afxRaster tex, afxUri const *uri);
_AFXIMPORT afxError    AfxPrintRasterRegionsDdt(afxRaster tex, afxNat cnt, afxRasterRegion const rgn[], afxUri const uri[]);

#endif//AFX_DIRECTDRAW_TEXTURE_H