#ifndef AFX_DIRECTDRAW_SURFACE_H
#define AFX_DIRECTDRAW_SURFACE_H

#include "../afxDrawContext.h"

AFX afxError    AfxDrawContextUploadTexturesDds(afxDrawContext dctx, afxNat cnt, afxUri const uri[], afxTexture tex[]);

AFX afxError    AfxTextureUploadDds(afxTexture tex, afxUri const *uri);
AFX afxError    AfxTextureUploadRegionsDds(afxTexture tex, afxNat cnt, afxTextureRegion const rgn[], afxUri const uri[]);

AFX afxError    AfxTextureDownloadDds(afxTexture tex, afxUri const *uri);
AFX afxError    AfxTextureDownloadRegionsDds(afxTexture tex, afxNat cnt, afxTextureRegion const rgn[], afxUri const uri[]);

#endif//AFX_DIRECTDRAW_SURFACE_H