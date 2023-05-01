#ifndef AFX_DIRECTDRAW_TEXTURE_H
#define AFX_DIRECTDRAW_TEXTURE_H

#include "afx/draw/afxDrawContext.h"

AFX afxError    AfxDrawContextUploadTexturesDdt(afxDrawContext dctx, afxNat cnt, afxUri const uri[], afxTexture tex[]);

AFX afxError    AfxTextureUploadDdt(afxTexture tex, afxUri const *uri);
AFX afxError    AfxTextureUploadRegionsDdt(afxTexture tex, afxNat cnt, afxTextureRegion const rgn[], afxUri const uri[]);

AFX afxError    AfxTextureDownloadDdt(afxTexture tex, afxUri const *uri);
AFX afxError    AfxTextureDownloadRegionsDdt(afxTexture tex, afxNat cnt, afxTextureRegion const rgn[], afxUri const uri[]);

#endif//AFX_DIRECTDRAW_TEXTURE_H