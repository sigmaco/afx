#ifndef AFX_DIRECTDRAW_SURFACE_H
#define AFX_DIRECTDRAW_SURFACE_H

#include "afx/draw/afxDrawContext.h"

_AFXIMPORT afxError    AfxLoadTexturesDds(afxDrawContext dctx, afxNat cnt, afxUri const uri[], afxTexture tex[]);

_AFXIMPORT afxError    AfxFetchTextureDds(afxTexture tex, afxUri const *uri);
_AFXIMPORT afxError    AfxFetchTextureRegionsDds(afxTexture tex, afxNat cnt, afxTextureRegion const rgn[], afxUri const uri[]);

_AFXIMPORT afxError    AfxPrintTextureDds(afxTexture tex, afxUri const *uri);
_AFXIMPORT afxError    AfxPrintTextureRegionsDds(afxTexture tex, afxNat cnt, afxTextureRegion const rgn[], afxUri const uri[]);

#endif//AFX_DIRECTDRAW_SURFACE_H