//#include "afxDirectDrawTexture.h"
#include "qwadro/core/afxApplication.h"

#define DDT_MAGIC 0x33535452

#define DDT_DIFFUSE 0
#define DDT_DIFFUSE2 1
#define DDT_BUMP 6
#define DDT_BUMP2 7
#define DDT_CUBE 8 // to-do, not yet supported

#define DDT_NONE 0
#define DDT_PLAYER 1
#define DDT_TRANSPARENT 4
#define DDT_BLEND 8

#define DDT_BGRA 1
#define DDT_DXT1 4
#define DDT_GREYSCALE 7
#define DDT_DXT3 8
#define DDT_DXT5 9

typedef struct ddtHdr
{
    afxNat32 fcc;
    afxInt8 usage;
    afxInt8 alphaUsage;
    afxInt8 type;
    afxInt8 layerCnt;
    afxNat32 w;
    afxNat32 h;
} DDTHeader;

typedef struct DdtLayer
{
    afxNat32 offset;
    afxNat32 siz;
} DDTImageEntry;

_AFXEXPORT afxError AfxLoadTexturesDdt(afxDrawContext dctx, afxNat cnt, afxUri const uri[], afxRaster tex[]);

_AFXEXPORT afxError AfxFetchRasterDdt(afxRaster tex, afxUri const *uri);
_AFXEXPORT afxError AfxFetchRasterRegionsDdt(afxRaster tex, afxNat cnt, afxRasterRegion const rgn[], afxUri const uri[]);

_AFXEXPORT afxError AfxPrintRasterDdt(afxRaster tex, afxUri const *uri);
_AFXEXPORT afxError AfxPrintRasterRegionsDdt(afxRaster tex, afxNat cnt, afxRasterRegion const rgn[], afxUri const uri[]);
