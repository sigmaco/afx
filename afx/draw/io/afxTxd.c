/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                  Q W A D R O   E X E C U T I O N   E C O S Y S T E M
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#define _AVX_DRAW_C
#define _AVX_TXD_C
#include "../../dev/AvxImplKit.h"
#include "qwadro/inc/draw/dev/afxDrawSystem.h"
#include "qwadro/inc/draw/io/afxTxd.h"
#include <d3d9.h>

#define MAKECHUNKID(vendorID, chunkID) (((vendorID & 0xFFFFFF) << 8) | (chunkID & 0xFF))
#define rwVENDORID_CORE 0x00

enum RwCorePluginID
{
    rwID_NAOBJECT = MAKECHUNKID(rwVENDORID_CORE, 0x00),
    rwID_STRUCT = MAKECHUNKID(rwVENDORID_CORE, 0x01),
    rwID_STRING = MAKECHUNKID(rwVENDORID_CORE, 0x02),
    rwID_EXTENSION = MAKECHUNKID(rwVENDORID_CORE, 0x03),

    rwID_CAMERA = MAKECHUNKID(rwVENDORID_CORE, 0x05),
    /**< RwCamera chunk. See \ref RwCameraStreamRead */

    rwID_TEXTURE = MAKECHUNKID(rwVENDORID_CORE, 0x06),
    /**< RwTexture chunk. See \ref RwTextureStreamRead */

    rwID_MATERIAL = MAKECHUNKID(rwVENDORID_CORE, 0x07),
    /**< RpMaterial chunk. See \ref RpMaterialStreamRead. */

    rwID_MATLIST = MAKECHUNKID(rwVENDORID_CORE, 0x08),
    rwID_ATOMICSECT = MAKECHUNKID(rwVENDORID_CORE, 0x09),
    rwID_PLANESECT = MAKECHUNKID(rwVENDORID_CORE, 0x0A),

    rwID_WORLD = MAKECHUNKID(rwVENDORID_CORE, 0x0B),
    /**< RpWorld chunk. See \ref RpWorldStreamRead. */

    rwID_SPLINE = MAKECHUNKID(rwVENDORID_CORE, 0x0C),
    /**< RpSpline chunk. See \ref RpSplineStreamRead */

    rwID_MATRIX = MAKECHUNKID(rwVENDORID_CORE, 0x0D),
    /**< RwMatrix chunk. See \ref RwMatrixStreamRead */

    rwID_FRAMELIST = MAKECHUNKID(rwVENDORID_CORE, 0x0E),

    rwID_GEOMETRY = MAKECHUNKID(rwVENDORID_CORE, 0x0F),
    /**< RpGeometry chunk. See \ref RpGeometryStreamRead. */

    rwID_CLUMP = MAKECHUNKID(rwVENDORID_CORE, 0x10),
    /**< RpClump chunk. See \ref RpClumpStreamRead. */

    rwID_LIGHT = MAKECHUNKID(rwVENDORID_CORE, 0x12),
    /**< RpLight chunk. See \ref RpLightStreamRead. */

    rwID_UNICODESTRING = MAKECHUNKID(rwVENDORID_CORE, 0x13),

    rwID_ATOMIC = MAKECHUNKID(rwVENDORID_CORE, 0x14),
    /**< RpAtomic chunk. See \ref RpAtomicStreamRead */

    rwID_TEXTURENATIVE = MAKECHUNKID(rwVENDORID_CORE, 0x15),

    rwID_TEXDICTIONARY = MAKECHUNKID(rwVENDORID_CORE, 0x16),
    /**< RwTexDictionary - platform specific texture dictionary.
     * See \ref RwTexDictionaryStreamRead. */

    rwID_ANIMDATABASE = MAKECHUNKID(rwVENDORID_CORE, 0x17),

    rwID_IMAGE = MAKECHUNKID(rwVENDORID_CORE, 0x18),
    /**< RwImage chunk. See \ref RwImageStreamRead */

    rwID_SKINANIMATION = MAKECHUNKID(rwVENDORID_CORE, 0x19),
    rwID_GEOMETRYLIST = MAKECHUNKID(rwVENDORID_CORE, 0x1A),

    rwID_ANIMANIMATION = MAKECHUNKID(rwVENDORID_CORE, 0x1B),
    /**< RtAnimAnimation chunk. See \ref RtAnimAnimationStreamRead. */
    rwID_HANIMANIMATION = MAKECHUNKID(rwVENDORID_CORE, 0x1B),
    /**< RtAnimAnimation chunk. For backwards compatibility. See \ref rwID_ANIMANIMATION. */

    rwID_TEAM = MAKECHUNKID(rwVENDORID_CORE, 0x1C),
    /**< \ref RpTeam chunk. See \ref RpTeamStreamRead */
    rwID_CROWD = MAKECHUNKID(rwVENDORID_CORE, 0x1D),

    rwID_DMORPHANIMATION = MAKECHUNKID(rwVENDORID_CORE, 0x1E),
    /**< RpDMorphAnimation - delta morph animation chunk. See \ref RpDMorphAnimationStreamRead */

    rwID_RIGHTTORENDER = MAKECHUNKID(rwVENDORID_CORE, 0x1f),

    rwID_MTEFFECTNATIVE = MAKECHUNKID(rwVENDORID_CORE, 0x20),
    /**< \if xbox RpMTEffect - multi-texture effect chunk. See \ref RpMTEffectStreamRead \endif */
    /**< \if gcn  RpMTEffect - multi-texture effect chunk. See \ref RpMTEffectStreamRead \endif */

    rwID_MTEFFECTDICT = MAKECHUNKID(rwVENDORID_CORE, 0x21),
    /**< \if xbox RpMTEffectDict - multi-texture effect dictionary chunk. See \ref RpMTEffectDictStreamRead \endif */
    /**< \if gcn  RpMTEffectDict - multi-texture effect dictionary chunk. See \ref RpMTEffectDictStreamRead \endif */

    rwID_TEAMDICTIONARY = MAKECHUNKID(rwVENDORID_CORE, 0x22),
    /**< \ref RpTeamDictionary chunk. See \ref RpTeamDictionaryStreamRead */

    rwID_PITEXDICTIONARY = MAKECHUNKID(rwVENDORID_CORE, 0x23),
    /**< RwTexDictionary - platform independent texture dictionary. See \ref RtPITexDictionaryStreamRead. */

    rwID_TOC = MAKECHUNKID(rwVENDORID_CORE, 0x24),
    /**< RtTOC chunk. See \ref RtTOCStreamRead */

    rwID_PRTSTDGLOBALDATA = MAKECHUNKID(rwVENDORID_CORE, 0x25),
    /**< RpPrtStdEmitterClass, RpPrtStdParticleClass and RpPrtStdPropertyTable chunks.
     *   See \ref RpPrtStdEClassStreamRead, \ref RpPrtStdPClassStreamRead
     *   \ref RpPrtStdPropTabStreamRead and \ref RpPrtStdGlobalDataStreamRead */

    rwID_ALTPIPE = MAKECHUNKID(rwVENDORID_CORE, 0x26),
    rwID_PIPEDS = MAKECHUNKID(rwVENDORID_CORE, 0x27),
    rwID_PATCHMESH = MAKECHUNKID(rwVENDORID_CORE, 0x28),
    /**< RpPatchMesh chunk. See \ref RpPatchMeshStreamRead */

    rwID_CHUNKGROUPSTART = MAKECHUNKID(rwVENDORID_CORE, 0x29),
    rwID_CHUNKGROUPEND = MAKECHUNKID(rwVENDORID_CORE, 0x2A),

    rwID_UVANIMDICT = MAKECHUNKID(rwVENDORID_CORE, 0x2B),
    /**< UV anim dictionary chunk. See \ref RpUVAnimGetDictSchema */

    rwID_COLLTREE = MAKECHUNKID(rwVENDORID_CORE, 0x2C),

    rwID_ENVIRONMENT = MAKECHUNKID(rwVENDORID_CORE, 0x2D),
    /**< RpEnvironment chunk is used internally to convey art package background
       < settings to the Visualizer.*/

       /* Insert before MAX and increment MAX */
       rwID_COREPLUGINIDMAX = MAKECHUNKID(rwVENDORID_CORE, 0x2E)
};
typedef enum RwCorePluginID RwCorePluginID;

enum RwPlatformID
{
    rwID_PCD3D7 = 1,
    rwID_PCOGL,
    rwID_MAC,
    rwID_PS2,
    rwID_XBOX,
    rwID_GAMECUBE,
    rwID_SOFTRAS,
    rwID_PCD3D8,
    rwID_PCD3D9,
    rwID_PSP
};
typedef enum RwPlatformID RwPlatformID;

#define RWLIBRARYIDPACK( _version, _buildNum )                                 \
    ( ( ( ( (_version)-0x30000U ) & 0x3ff00U ) << 14 ) |                       \
      ( ( (_version)&0x0003fU ) << 16 ) | ( (_buildNum)&0xffffU ) )

#define RWLIBRARYIDUNPACKVERSION( _libraryID )                                 \
    ( ( ( ( ( _libraryID ) >> 14 ) & 0x3ff00U ) + 0x30000U ) |                 \
      ( ( ( _libraryID ) >> 16 ) & 0x0003fU ) )

#define RWLIBRARYIDUNPACKBUILDNUM( _libraryID ) ( (_libraryID)&0xffffU )

uint32_t RwStreamRead(afxStream stream, void *buffer, uint32_t length)
{
    return AfxReadStream(stream, length, 0, buffer);
}
afxBool _rwStreamReadChunkHeader(afxStream stream, uint32_t *type, uint32_t *length, uint32_t *version, uint32_t *buildNum);

afxBool RwStreamFindChunk(afxStream stream, uint32_t type, uint32_t *lengthOut, uint32_t *versionOut)
{
    uint32_t readType;
    uint32_t readLength;
    uint32_t readVersion;

    while (_rwStreamReadChunkHeader(stream, &readType, &readLength, &readVersion, NIL))
    {
        if (readType == type)
        {
            if (lengthOut)
                *lengthOut = readLength;

            if (versionOut)
                *versionOut = readVersion;

            return 1;
        }

        if (readLength > 0)
            AfxSeekStream(stream, readLength, afxSeekMode_RELATIVE);
    }
    return 0;
}

static char *StringStreamRead(char *nativeString, void *stream, uint32_t length)
{
    char  multiByteString[64];
    char *baseString;
    afxBool  mallocced;

    if (!nativeString)
    {
        nativeString = AfxAllocate(length, 1, 0, AfxHere());

        if (!nativeString)
        {
            return 0;
        }
        mallocced = 1;
    }

    baseString = nativeString;

    while (length > 0)
    {
        uint32_t bytesToRead = (length > 64) ? 64 : length;
        uint32_t i;

        if (RwStreamRead(stream, multiByteString, bytesToRead) != bytesToRead)
        {
            return 0;
        }

        /* Reduce by the amount we read */
        length -= bytesToRead;

        for (i = 0; i < bytesToRead; i++)
        {
            baseString[i] = (char)(multiByteString[i]);
        }
        baseString += bytesToRead;
    }

    return (nativeString);
}

afxBool _rwStreamReadChunkHeader(afxStream stream, uint32_t *type, uint32_t *length, uint32_t *version, uint32_t *buildNum);

char* _rwStringStreamFindAndRead(char *string, afxStream stream)
{
    uint32_t readType;
    uint32_t readLength;
    uint32_t readVersion;

    while (_rwStreamReadChunkHeader(stream, &readType, &readLength, &readVersion, 0))
    {
        if (readType == rwID_STRING)
        {
            return (StringStreamRead(string, stream, readLength));
        }

        if (readLength > 0)
        {
            AfxSeekStream(stream, readLength, afxSeekMode_RELATIVE);
        }
    }
    return 0;
}

struct __rwMark
{
    uint32_t type;
    uint32_t length;
    uint32_t libraryID;
};

typedef struct RwChunkHeaderInfo
{
    afxNat32 type;      /**< chunk ID - see \ref RwStreamFindChunk */
    afxNat32 length;    /**< length of the chunk data in bytes */
    afxNat32 version;   /**< version of the chunk data.
                         *   See \ref RwEngineGetVersion. */
    afxNat32 buildNum;  /**< build number of the RenderWare libraries
                         *   previously used to stream out the data */
    afxBool isComplex;
} RwChunkHeaderInfo;

afxBool _rwStreamReadChunkHeader(afxStream stream, uint32_t *type, uint32_t *length, uint32_t *version, uint32_t *buildNum)
{
    struct __rwMark          mark = {0};
    RwChunkHeaderInfo chunkHdrInfo = {0};

    if (AfxStreamReachedEnd(stream))
        return 0;

    RwStreamRead(stream, &mark, sizeof(mark));

    chunkHdrInfo.type = mark.type;
    chunkHdrInfo.length = mark.length;

    /* Check for old library ID */
    if (!(mark.libraryID & 0xffff0000))
    {
        /* Just contains old-style version number */
        chunkHdrInfo.version = mark.libraryID << 8;
        chunkHdrInfo.buildNum = 0;
    }
    else
    {
        /* Unpack new library ID */
        chunkHdrInfo.version = RWLIBRARYIDUNPACKVERSION(mark.libraryID);
        chunkHdrInfo.buildNum = RWLIBRARYIDUNPACKBUILDNUM(mark.libraryID);
    }

    // TODO
    chunkHdrInfo.isComplex = 1; // ChunkIsComplex( &chunkHdrInfo );

    if (type)
        *type = chunkHdrInfo.type;

    if (length)
        *length = chunkHdrInfo.length;

    if (buildNum)
        *buildNum = chunkHdrInfo.buildNum;

    if (version)
        *version = chunkHdrInfo.version;

    return 1;
}

#if 0
typedef enum _D3DFORMAT
{
    D3DFMT_UNKNOWN = 0,

    D3DFMT_R8G8B8 = 20,
    D3DFMT_A8R8G8B8 = 21,
    D3DFMT_X8R8G8B8 = 22,
    D3DFMT_R5G6B5 = 23,
    D3DFMT_X1R5G5B5 = 24,
    D3DFMT_A1R5G5B5 = 25,
    D3DFMT_A4R4G4B4 = 26,
    D3DFMT_R3G3B2 = 27,
    D3DFMT_A8 = 28,
    D3DFMT_A8R3G3B2 = 29,
    D3DFMT_X4R4G4B4 = 30,
    D3DFMT_A2B10G10R10 = 31,
    D3DFMT_A8B8G8R8 = 32,
    D3DFMT_X8B8G8R8 = 33,
    D3DFMT_G16R16 = 34,
    D3DFMT_A2R10G10B10 = 35,
    D3DFMT_A16B16G16R16 = 36,

    D3DFMT_A8P8 = 40,
    D3DFMT_P8 = 41,

    D3DFMT_L8 = 50,
    D3DFMT_A8L8 = 51,
    D3DFMT_A4L4 = 52,

    D3DFMT_V8U8 = 60,
    D3DFMT_L6V5U5 = 61,
    D3DFMT_X8L8V8U8 = 62,
    D3DFMT_Q8W8V8U8 = 63,
    D3DFMT_V16U16 = 64,
    D3DFMT_A2W10V10U10 = 67,

    D3DFMT_UYVY = MAKEFOURCC('U', 'Y', 'V', 'Y'),
    D3DFMT_R8G8_B8G8 = MAKEFOURCC('R', 'G', 'B', 'G'),
    D3DFMT_YUY2 = MAKEFOURCC('Y', 'U', 'Y', '2'),
    D3DFMT_G8R8_G8B8 = MAKEFOURCC('G', 'R', 'G', 'B'),
    D3DFMT_DXT1 = MAKEFOURCC('D', 'X', 'T', '1'),
    D3DFMT_DXT2 = MAKEFOURCC('D', 'X', 'T', '2'),
    D3DFMT_DXT3 = MAKEFOURCC('D', 'X', 'T', '3'),
    D3DFMT_DXT4 = MAKEFOURCC('D', 'X', 'T', '4'),
    D3DFMT_DXT5 = MAKEFOURCC('D', 'X', 'T', '5'),

    D3DFMT_D16_LOCKABLE = 70,
    D3DFMT_D32 = 71,
    D3DFMT_D15S1 = 73,
    D3DFMT_D24S8 = 75,
    D3DFMT_D24X8 = 77,
    D3DFMT_D24X4S4 = 79,
    D3DFMT_D16 = 80,

    D3DFMT_D32F_LOCKABLE = 82,
    D3DFMT_D24FS8 = 83,

    /* D3D9Ex only -- */
#if !defined(D3D_DISABLE_9EX)

    /* Z-Stencil formats valid for CPU access */
    D3DFMT_D32_LOCKABLE = 84,
    D3DFMT_S8_LOCKABLE = 85,

#endif // !D3D_DISABLE_9EX
    /* -- D3D9Ex only */


    D3DFMT_L16 = 81,

    D3DFMT_VERTEXDATA = 100,
    D3DFMT_INDEX16 = 101,
    D3DFMT_INDEX32 = 102,

    D3DFMT_Q16W16V16U16 = 110,

    D3DFMT_MULTI2_ARGB8 = MAKEFOURCC('M', 'E', 'T', '1'),

    // Floating point surface formats

    // s10e5 formats (16-bits per channel)
    D3DFMT_R16F = 111,
    D3DFMT_G16R16F = 112,
    D3DFMT_A16B16G16R16F = 113,

    // IEEE s23e8 formats (32-bits per channel)
    D3DFMT_R32F = 114,
    D3DFMT_G32R32F = 115,
    D3DFMT_A32B32G32R32F = 116,

    D3DFMT_CxV8U8 = 117,

    /* D3D9Ex only -- */
#if !defined(D3D_DISABLE_9EX)

    // Monochrome 1 bit per pixel format
    D3DFMT_A1 = 118,

    // 2.8 biased fixed point
    D3DFMT_A2B10G10R10_XR_BIAS = 119,


    // Binary format indicating that the data has no inherent type
    D3DFMT_BINARYBUFFER = 199,

#endif // !D3D_DISABLE_9EX
    /* -- D3D9Ex only */


    D3DFMT_FORCE_DWORD = 0x7fffffff
} D3DFORMAT;
#endif

enum RwRasterFormat
{
    rwRASTERFORMATDEFAULT = 0x0000, /* Whatever the hardware likes best */

    rwRASTERFORMAT1555 = 0x0100,    /**<16 bits - 1 bit alpha, 5 bits red, green and blue */
    rwRASTERFORMAT565 = 0x0200,     /**<16 bits - 5 bits red and blue, 6 bits green */
    rwRASTERFORMAT4444 = 0x0300,    /**<16 bits - 4 bits per component */
    rwRASTERFORMATLUM8 = 0x0400,    /**<Gray scale */
    rwRASTERFORMAT8888 = 0x0500,    /**<32 bits - 8 bits per component */
    rwRASTERFORMAT888 = 0x0600,     /**<24 bits - 8 bits per component */
    rwRASTERFORMAT16 = 0x0700,      /**<16 bits - undefined: useful for things like Z buffers */
    rwRASTERFORMAT24 = 0x0800,      /**<24 bits - undefined: useful for things like Z buffers */
    rwRASTERFORMAT32 = 0x0900,      /**<32 bits - undefined: useful for things like Z buffers */
    rwRASTERFORMAT555 = 0x0a00,     /**<16 bits - 5 bits red, green and blue */

    rwRASTERFORMATAUTOMIPMAP = 0x1000, /**<RenderWare generated the mip levels */

    rwRASTERFORMATPAL8 = 0x2000,    /**<8 bit palettised */
    rwRASTERFORMATPAL4 = 0x4000,    /**<4 bit palettised */

    rwRASTERFORMATMIPMAP = 0x8000,  /**<Mip mapping on */

    rwRASTERFORMATPIXELFORMATMASK = 0x0f00, /**<The pixel color format
                                             *  (excluding palettised bits) */
    rwRASTERFORMATMASK = 0xff00     /**<The whole format */

};
typedef enum RwRasterFormat RwRasterFormat;

#define rwTEXTUREBASENAMELENGTH 32
#define HAS_ALPHA           (1<<0)
#define IS_CUBE             (1<<1)
#define USE_AUTOMIPMAPGEN   (1<<2)
#define IS_COMPRESSED       (1<<3)

#pragma pack(push, 1)

typedef struct _rwD3D9NativeTexture _rwD3D9NativeTexture;
struct _rwD3D9NativeTexture
{
    afxInt32 id; /* RwPlatformID,(rwID_D3D9) defined in batype.h */
    afxInt32 filterAndAddress; /* Same as in babintex.c */
    afxChar  name[rwTEXTUREBASENAMELENGTH]; /* Texture name */
    afxChar  mask[rwTEXTUREBASENAMELENGTH]; /* Texture mask name */
};

typedef struct _rwD3D9NativeRaster _rwD3D9NativeRaster;
struct _rwD3D9NativeRaster
{
    afxNat32    format;         /* Raster format flags */
    D3DFORMAT   d3dFormat;      /* D3D pixel format */
    afxNat16    width;          /* Raster width */
    afxNat16    height;         /* Raster height */
    afxNat8     depth;          /* Raster depth */
    afxNat8     numMipLevels;   /* The number of mip levels to load */
    afxNat8     type;           /* The raster type */
    afxNat8     flags;          /* This raster has an alpha component, automipmapgen, etc */
};

/*
    rwCHUNKHEADERSIZE
        _rwD3D9NativeTexture
        _rwD3D9NativeRaster

        if ((format & rwRASTERFORMATPAL4) || (format & rwRASTERFORMATPAL8))
        {
            Load/Save palette
        }

        afxNat32 - mip level size in bytes     --
        Data - mip level data                   | numMipLevels times
                    |                           |
                    --------------<--------------

 */

#pragma pack(pop)

afxBool _rwD3D9NativeTextureRead(afxStream stream, _rwD3D9NativeRaster* nativeRaster, avxSamplerConfig* sampi, afxRasterInfo* rasi)
{
    afxError err = NIL;
    afxRaster raster = NIL;
    afxDrawContext dctx;

    afxRasterInfo info = { 0 };
    info.whd[0] = nativeRaster->width;
    info.whd[1] = nativeRaster->height;
    info.whd[2] = nativeRaster->depth;
    info.lodCnt = AfxMax(1, nativeRaster->numMipLevels);

    afxBool automipmapgen = (nativeRaster->flags & USE_AUTOMIPMAPGEN);

    info.flags |= (nativeRaster->flags & IS_CUBE) != 0 ? afxRasterFlag_CUBEMAP : NIL;

    if (nativeRaster->flags & IS_COMPRESSED)
    {
        afxNat32 dxtFormat;
        dxtFormat = nativeRaster->d3dFormat;

        switch (nativeRaster->d3dFormat)
        {
        case D3DFMT_DXT1: info.fmt = afxPixelFormat_DXT1; break;
        case D3DFMT_DXT3: info.fmt = afxPixelFormat_DXT3; break;
        case D3DFMT_DXT5: info.fmt = afxPixelFormat_DXT5; break;
        case D3DFMT_A8R8G8B8: info.fmt = afxPixelFormat_ARGB8; break;
        case D3DFMT_A1R5G5B5: info.fmt = afxPixelFormat_RGB5A1; break;
        case D3DFMT_A4R4G4B4: info.fmt = afxPixelFormat_RGBA4; break;
        case D3DFMT_A2R10G10B10: info.fmt = afxPixelFormat_RGB10A2; break;
        case D3DFMT_A2B10G10R10: info.fmt = afxPixelFormat_RGB10A2; break;
        case D3DFMT_A8R3G3B2: info.fmt = afxPixelFormat_RG8; break;
        };
    }

    AfxAcquireRasters(dctx, 1, &info, &raster);
    //raster = RwRasterCreate(nativeRaster.width, nativeRaster.height, (RwInt32)nativeRaster.depth, nativeRaster.type | nativeRaster.format | rwRASTERDONTALLOCATE);

    afxNat face = info.whd[2];

    /* Load mips into raster */
    do
    {
        for (afxNat i = 0; i < info.lodCnt; i++)
        {
            afxSize    size;

            /* Size in bytes of mip to load */
            if (AfxReadStream(stream, sizeof(size), 0, &size))
            {
                AfxThrowError();
            }

            afxRasterIo iop = { 0 };
            iop.rgn.whd[0] = info.whd[0];
            iop.rgn.whd[1] = info.whd[1];
            iop.rgn.whd[2] = 1;
            iop.rgn.origin[2] = face;
            iop.rgn.lodIdx = i;
            iop.rowCnt = info.whd[1];
            iop.offset = AfxGetStreamPosn(stream);

            afxNat pitch = info.whd[0] >> iop.rgn.lodIdx;

            if (!(nativeRaster->flags & IS_COMPRESSED))
            {
                size = pitch * (1 >> 3);
            }
            else
            {
                afxNat shift;

                /* A DXT compressed format */
                if (D3DFMT_DXT1 == nativeRaster->d3dFormat)
                {
                    shift = 3; /* 64Bits / 8 = 8. 2^3 = 8 */
                }
                else
                {
                    shift = 4; /* 128Bits / 8 = 16. 2^4 = 16 */
                }

                pitch = pitch >> 2;
                size = pitch << shift;
                iop.rowStride = pitch;

                AfxAssert(8 <= size);

                //RWRETURN(size);
            }

            // Read the mip level
            AfxUploadRaster(raster, 0, 1, &iop, stream);
        }
    } while (--face);

    return 0;
}

_AVX afxError _AvxTxdStdDtor(avxTxd txd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &txd, afxFcc_TXD);

    for (afxNat i = 0; i < txd->texCnt; i++)
    {
        AfxReleaseObjects(1, &txd->texs[i].ras);
        AfxReleaseObjects(1, &txd->texs[i].samp);
        AfxDeallocateString(&txd->texs[i].urn);
    }
    AfxDeallocate(txd->texs);

    return err;
}

_AVX afxError _AvxTxdStdCtor(avxTxd txd, void** args, afxNat invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &txd, afxFcc_TXD);

    avxTxdInfo const *txdi = ((avxTxdInfo const *)args[1]) + invokeNo;

    afxDrawContext dctx = AfxGetParent(0);

    afxNat texCnt = txdi->texCnt;

    txd->texs = AfxAllocate(texCnt, sizeof(txd->texs[0]), 0, AfxHere());
    AfxAssert(txd->texs);
    AfxZero2(txd->texs, sizeof(txd->texs[0]), texCnt);

    afxRaster rasters[256];
    avxSampler samplers[256];

    if (AfxAcquireRasters(dctx, texCnt, txdi->rasters, rasters))
        AfxThrowError();

    if (AfxAcquireSamplers(dctx, texCnt, txdi->samplers, samplers))
        AfxThrowError();

    for (afxNat i = 0; i < txdi->texCnt; i++)
    {
        txd->texs[i].ras = rasters[i];
        txd->texs[i].samp = samplers[i];
        txd->texs[i].reqCnt = 0;
        AfxCloneString(&txd->texs[i].urn, &txdi->names[i]);
    }

    return err;
}

_AVX afxClassConfig const _AvxTxdClsCfg =
{
    .fcc = afxFcc_TXD,
    .name = "Txd",
    .desc = "Texture Dictionary",
    .fixedSiz = sizeof(AFX_OBJECT(avxTxd)),
    .ctor = (void*)_AvxTxdStdCtor,
    .dtor = (void*)_AvxTxdStdDtor
};

////////////////////////////////////////////////////////////////////////////////

_AVX afxError AfxAcquireTxd(afxDrawInput din, avxTxdInfo const* info, avxTxd* txd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);

    afxClass* cls = (afxClass*)AfxGetTxdClass(din);
    AfxAssertClass(cls, afxFcc_TXD);

    if (AfxAcquireObjects(cls, 1, (afxObject*)txd, (void const*[]) { din, info }))
        AfxThrowError();

    AfxAssertObjects(1, txd, afxFcc_TXD);

    return err;
}

_AVX afxError AfxOpenTxd(afxDrawInput din, afxUri const* uri, avxTxd* dict)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);

    afxDrawContext dctx;
    AfxGetDrawInputContext(din, &dctx);

    afxStream iob;

    if (AfxOpenFile(uri, afxIoFlag_RX, &iob)) AfxThrowError();
    else
    {
        afxNat32 type;
        afxNat32 len;
        afxNat32 ver;

        if ((_rwStreamReadChunkHeader(iob, &type, &len, &ver, 0) && type == /*rwID_TEXDICTIONARY*/0x0016) &&
            (_rwStreamReadChunkHeader(iob, &type, &len, &ver, 0) && type == /*rwID_STRUCT*/0x0001))
        {
            struct/*_rwStreamTexDictionary*/
            {
#ifdef AFX_LE
                afxNat16           numTextures;
                afxNat16           deviceId;
#else
                afxNat16           deviceId;
                afxNat16           numTextures;
#endif
            } binTexDict = { 0 };
            afxNat16 myId = 0;
            //_rwDeviceSystemRequest(device, rwDEVICESYSTEMGETID, &myId, NULL, 0);
            myId = myId & 0xff;
            // We allow for the case where no ids are defined. This will break if an old texture dictionary is seen which has more than 65535 textures, but we hope this is an unusual case.

            AfxAssert(len <= sizeof(binTexDict));

            if (AfxReadStream(iob, len, 0, &binTexDict) ||
                ((myId != 0) && (binTexDict.deviceId != 0) && (myId != binTexDict.deviceId))) AfxThrowError();
            else
            {
                afxString names[256];
                afxRasterInfo rasi[256] = { 0 };
                avxSamplerConfig sampi[256] = { 0 };
                afxNat i = 0;

                afxSize posnBkp = AfxGetStreamPosn(iob);

                while (binTexDict.numTextures > i++)
                {
                    if ((!_rwStreamReadChunkHeader(iob, &type, &len, &ver, 0) && type == /*rwID_TEXTURENATIVE*/0x0015) ||
                        (!_rwStreamReadChunkHeader(iob, &type, &len, &ver, 0) && type == /*rwID_STRUCT*/0x0001)) AfxThrowError();
                    else
                    {
                        struct/*_rwD3D9NativeTexture*/
                        {
                            afxInt32 id; // RwPlatformID,(rwID_D3D9) defined in batype.h
                            afxInt32 filterAndAddress;
                            afxChar  name[32];
                            afxChar  mask[32];
                        } nativeTexture;

                        if (AfxReadStream(iob, sizeof(nativeTexture), 0, &nativeTexture))
                        {
                            AfxThrowError();
                        }

                        AfxMakeString(&names[i], 0, nativeTexture.name, 0);

                        avxSamplerConfig sampc = { 0 };

                        switch (nativeTexture.filterAndAddress & 0xFF)
                        {
                        case 1: sampc.minFilter = (sampc.magFilter = (sampc.mipFilter = avxTexelFilter_POINT)); break; // rwFILTERNEAREST, /**<Point sampled */
                        case 2: sampc.minFilter = (sampc.magFilter = (sampc.mipFilter = avxTexelFilter_LINEAR)); break;// rwFILTERLINEAR, /**<Bilinear */
                        case 3: sampc.mipFilter = avxTexelFilter_POINT; break; // rwFILTERMIPNEAREST, /**<Point sampled per pixel mip map */
                        case 4: sampc.mipFilter = avxTexelFilter_LINEAR; break; // rwFILTERMIPLINEAR, /**<Bilinear per pixel mipmap */
                        case 5: sampc.minFilter = (sampc.magFilter = avxTexelFilter_LINEAR); sampc.mipFilter = avxTexelFilter_POINT; break; // rwFILTERLINEARMIPNEAREST, /**<MipMap interp point sampled */
                        case 6: sampc.minFilter = (sampc.magFilter = avxTexelFilter_LINEAR); sampc.mipFilter = avxTexelFilter_LINEAR; break; // rwFILTERLINEARMIPLINEAR, /**<Trilinear */
                        default: AfxThrowError(); break;
                        }

                        switch ((nativeTexture.filterAndAddress >> 8) & 0x0F)
                        {
                        case 1: sampc.uvw[0] = (sampc.uvw[1] = (sampc.uvw[2] = (avxTexelAddress_REPEAT))); break; // rwTEXTUREADDRESSWRAP,      /**<UV wraps (tiles) */
                        case 2: sampc.uvw[0] = (sampc.uvw[1] = (sampc.uvw[2] = (avxTexelAddress_MIRROR))); break; // rwTEXTUREADDRESSMIRROR,    /**<Alternate UV is flipped */
                        case 3: sampc.uvw[0] = (sampc.uvw[1] = (sampc.uvw[2] = (avxTexelAddress_CLAMP))); break; // rwTEXTUREADDRESSCLAMP,     /**<UV is clamped to 0-1 */
                        case 4: sampc.uvw[0] = (sampc.uvw[1] = (sampc.uvw[2] = (avxTexelAddress_BORDER))); break; // rwTEXTUREADDRESSBORDER, /**<Border color takes effect outside of 0-1 */
                        default: AfxThrowError(); break;
                        }

                        switch ((nativeTexture.filterAndAddress >> 12) & 0x0F)
                        {
                        case 1: sampc.uvw[0] = (sampc.uvw[1] = (sampc.uvw[2] = (avxTexelAddress_REPEAT))); break; // rwTEXTUREADDRESSWRAP,      /**<UV wraps (tiles) */
                        case 2: sampc.uvw[0] = (sampc.uvw[1] = (sampc.uvw[2] = (avxTexelAddress_MIRROR))); break; // rwTEXTUREADDRESSMIRROR,    /**<Alternate UV is flipped */
                        case 3: sampc.uvw[0] = (sampc.uvw[1] = (sampc.uvw[2] = (avxTexelAddress_CLAMP))); break; // rwTEXTUREADDRESSCLAMP,     /**<UV is clamped to 0-1 */
                        case 4: sampc.uvw[0] = (sampc.uvw[1] = (sampc.uvw[2] = (avxTexelAddress_BORDER))); break; // rwTEXTUREADDRESSBORDER, /**<Border color takes effect outside of 0-1 */
                        default: AfxThrowError(); break;
                        }

                        sampi[i] = sampc;

                        if (rwID_PCD3D9 != nativeTexture.id)
                        {
                            AfxThrowError();
                        }

                        struct/*_rwD3D9NativeRaster*/
                        {
                            afxNat32 format;
                            D3DFORMAT d3dFormat; //  D3D pixel format
                            afxNat16 width;
                            afxNat16 height;
                            afxNat8 depth;
                            afxNat8 numMipLevels;
                            afxNat8 type;
                            afxNat8 flags; // This raster has an alpha component, automipmapgen, etc.
                        } nativeRaster;

                        if (AfxReadStream(iob, sizeof(nativeRaster), 0, &nativeRaster))
                        {
                            AfxThrowError();
                        }

                        rasi[i].whd[0] = nativeRaster.width;
                        rasi[i].whd[1] = nativeRaster.height;
                        rasi[i].whd[2] = nativeRaster.depth;
                        rasi[i].lodCnt = AfxMax(1, nativeRaster.numMipLevels);

                        afxBool automipmapgen = (nativeRaster.flags & USE_AUTOMIPMAPGEN);

                        rasi[i].flags |= (nativeRaster.flags & IS_CUBE) != 0 ? afxRasterFlag_CUBEMAP : NIL;

                        if (nativeRaster.flags & IS_COMPRESSED)
                        {
                            afxNat32 dxtFormat;
                            dxtFormat = nativeRaster.d3dFormat;

                            switch (nativeRaster.d3dFormat)
                            {
                            case D3DFMT_DXT1: rasi[i].fmt = afxPixelFormat_DXT1; break;
                            case D3DFMT_DXT3: rasi[i].fmt = afxPixelFormat_DXT3; break;
                            case D3DFMT_DXT5: rasi[i].fmt = afxPixelFormat_DXT5; break;
                            case D3DFMT_A8R8G8B8: rasi[i].fmt = afxPixelFormat_ARGB8; break;
                            case D3DFMT_A1R5G5B5: rasi[i].fmt = afxPixelFormat_RGB5A1; break;
                            case D3DFMT_A4R4G4B4: rasi[i].fmt = afxPixelFormat_RGBA4; break;
                            case D3DFMT_A2R10G10B10: rasi[i].fmt = afxPixelFormat_RGB10A2; break;
                            case D3DFMT_A2B10G10R10: rasi[i].fmt = afxPixelFormat_RGB10A2; break;
                            case D3DFMT_A8R3G3B2: rasi[i].fmt = afxPixelFormat_RG8; break;
                            };
                        }

                        afxNat face = rasi[i].whd[2];

                        /* Load mips into raster */
                        do
                        {
                            for (afxNat i = 0; i < rasi[i].lodCnt; i++)
                            {
                                afxSize    size;

                                /* Size in bytes of mip to load */
                                if (AfxReadStream(iob, sizeof(size), 0, &size))
                                {
                                    AfxThrowError();
                                }
                                AfxAdvanceStream(iob, size);
                            }
                        } while (--face);

                        //_rwPluginRegistryReadDataChunks(&textureTKList, stream, newTex);

                    }
                }

                afxRaster rasters[256];
                AfxAcquireRasters(dctx, binTexDict.numTextures, rasi, rasters);

                AfxSeekStreamFromBegin(iob, posnBkp);

                while (binTexDict.numTextures > i++)
                {
                    if ((!_rwStreamReadChunkHeader(iob, &type, &len, &ver, 0) && type == /*rwID_TEXTURENATIVE*/0x0015) ||
                        (!_rwStreamReadChunkHeader(iob, &type, &len, &ver, 0) && type == /*rwID_STRUCT*/0x0001)) AfxThrowError();
                    else
                    {
                        struct/*_rwD3D9NativeTexture*/
                        {
                            afxInt32 id; // RwPlatformID,(rwID_D3D9) defined in batype.h
                            afxInt32 filterAndAddress;
                            afxChar  name[32];
                            afxChar  mask[32];
                        } nativeTexture;

                        if (AfxAdvanceStream(iob, sizeof(nativeTexture)))
                        {
                            AfxThrowError();
                        }

                        struct/*_rwD3D9NativeRaster*/
                        {
                            afxNat32 format;
                            D3DFORMAT d3dFormat; //  D3D pixel format
                            afxNat16 width;
                            afxNat16 height;
                            afxNat8 depth;
                            afxNat8 numMipLevels;
                            afxNat8 type;
                            afxNat8 flags; // This raster has an alpha component, automipmapgen, etc.
                        } nativeRaster;

                        if (AfxReadStream(iob, sizeof(nativeRaster), 0, &nativeRaster))
                        {
                            AfxThrowError();
                        }

                        //if (!_rwD3D9NativeTextureRead(iob, &nativeRaster, &sampi[i], &rasi[i]))
                        {
                            afxNat face = rasi[i].whd[2];

                            /* Load mips into raster */
                            do
                            {
                                for (afxNat i = 0; i < rasi[i].lodCnt; i++)
                                {
                                    afxSize    size;

                                    /* Size in bytes of mip to load */
                                    if (AfxReadStream(iob, sizeof(size), 0, &size))
                                    {
                                        AfxThrowError();
                                    }

                                    afxRasterIo iop = { 0 };
                                    iop.rgn.whd[0] = rasi[i].whd[0];
                                    iop.rgn.whd[1] = rasi[i].whd[1];
                                    iop.rgn.whd[2] = 1;
                                    iop.rgn.origin[2] = face;
                                    iop.rgn.lodIdx = i;
                                    iop.rowCnt = rasi[i].whd[1];
                                    iop.offset = AfxGetStreamPosn(iob);

                                    afxNat pitch = rasi[i].whd[0] >> iop.rgn.lodIdx;

                                    if (!(nativeRaster.flags & IS_COMPRESSED))
                                    {
                                        size = pitch * (1 >> 3);
                                    }
                                    else
                                    {
                                        afxNat shift;

                                        /* A DXT compressed format */
                                        if (D3DFMT_DXT1 == nativeRaster.d3dFormat)
                                        {
                                            shift = 3; /* 64Bits / 8 = 8. 2^3 = 8 */
                                        }
                                        else
                                        {
                                            shift = 4; /* 128Bits / 8 = 16. 2^4 = 16 */
                                        }

                                        pitch = pitch >> 2;
                                        size = pitch << shift;

                                        AfxAssert(8 <= size);

                                        //RWRETURN(size);
                                    }

                                    iop.rowStride = pitch;

                                    // Read the mip level
                                    AfxUploadRaster(rasters[i], 0, 1, &iop, iob);
                                }
                            } while (--face);

                        }

                        //_rwPluginRegistryReadDataChunks(&textureTKList, stream, newTex);

                    }
                }
                    
                //_rwPluginRegistryReadDataChunks(&texDictTKList, stream, texDict)
            }
        }
        AfxReleaseObjects(1, &iob);
    }

    return err;
}
