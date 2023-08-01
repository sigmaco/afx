#ifndef AFX_DIRECTDRAW_TEXTURE_H
#define AFX_DIRECTDRAW_TEXTURE_H

#include "afx/draw/afxDrawContext.h"

_AFXIMPORT afxError    AfxLoadTexturesDdt(afxDrawContext dctx, afxNat cnt, afxUri const uri[], afxTexture tex[]);

_AFXIMPORT afxError    AfxFetchTextureDdt(afxTexture tex, afxUri const *uri);
_AFXIMPORT afxError    AfxFetchTextureRegionsDdt(afxTexture tex, afxNat cnt, afxTextureRegion const rgn[], afxUri const uri[]);

_AFXIMPORT afxError    AfxPrintTextureDdt(afxTexture tex, afxUri const *uri);
_AFXIMPORT afxError    AfxPrintTextureRegionsDdt(afxTexture tex, afxNat cnt, afxTextureRegion const rgn[], afxUri const uri[]);

#endif//AFX_DIRECTDRAW_TEXTURE_H