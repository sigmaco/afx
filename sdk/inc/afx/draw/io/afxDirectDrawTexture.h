#ifndef AFX_DIRECTDRAW_TEXTURE_H
#define AFX_DIRECTDRAW_TEXTURE_H

#include "../afxDrawSystem.h"

#ifndef __e2ddtex__
#   define DDTEX _AFXIMPORT
#else
#   define DDTEX _AFXEXPORT
#endif

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

typedef struct DDTHeader
{
    afxNat32 Magic;
    afxInt8 TextureUsage;
    afxInt8 TextureAlphaUsage;
    afxInt8 TextureType;
    afxInt8 ImageCount;
    afxNat32 Width;
    afxNat32 Height;
} DDTHeader;

typedef struct DDTImageEntry
{
    afxNat32 Offset;
    afxNat32 Size;
} DDTImageEntry;

DDTEX afxTexture _AfxDdtUploadTexture(afxDrawSystem dsys, afxNat depth, afxUri const uri[]);

#endif//AFX_DIRECTDRAW_TEXTURE_H