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
#include "../dev/AvxImplKit.h"
#include "qwadro/inc/cad/afxTxd.h"
#include <d3d9.h>

#ifdef _AVX_TXD_C

AFX_OBJECT(afxTxd)
{
    afxUri128           uri;
    afxStringBase       strb;
    afxNat              texCnt;
    afxString*          texNames; // nested names to speed up searches.
    struct
    {
        avxSampler      samp;
        afxRasterInfo   rasi;
        afxUrdOrigin    atFile;
    }*                  texInfos;
    afxRaster*          rasters;
};

#endif//_AVX_TXD_C

#pragma pack(push, 1)

AFX_DEFINE_STRUCT(urdTrunk_Txd)
{
    // .trunk.fcc = [ 't', 'x', 'd', '\0' ]
    urdTrunk        trunk;
    afxNat32        texCnt;
    afxNat32        texNames; // offset to the first urdString[] containing the texture names.
};

AFX_DEFINE_STRUCT(urdString)
{
    afxNat32        len;
    afxNat32        start; // offset where string data starts in file.
};

AFX_DEFINE_STRUCT(urdTrunk_TxdTex)
{
    avxTexelAddress uvw[3];
    avxTexelFilter  mipFlt;
    avxTexelFilter  minFlt;
    avxTexelFilter  magFlt;

    afxNat32        whd[3];
    afxNat32        lodCnt;
    afxNat32        usage;
    afxNat32        flags;
    afxNat32        fmt;
    afxNat32        rowStride;
    afxNat32        depthStride;
    afxUrdOrigin    data;
    afxNat32        udd[4];
};

#pragma pack(pop)

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

#define RWLIBRARYIDPACK( _version, _buildNum )                                 \
    ( ( ( ( (_version)-0x30000U ) & 0x3ff00U ) << 14 ) |                       \
      ( ( (_version)&0x0003fU ) << 16 ) | ( (_buildNum)&0xffffU ) )

#define RWLIBRARYIDUNPACKVERSION( _libraryID )                                 \
    ( ( ( ( ( _libraryID ) >> 14 ) & 0x3ff00U ) + 0x30000U ) |                 \
      ( ( ( _libraryID ) >> 16 ) & 0x0003fU ) )

#define RWLIBRARYIDUNPACKBUILDNUM( _libraryID ) ( (_libraryID)&0xffffU )

uint32_t RwStreamRead(afxStream stream, void *buffer, uint32_t length)
{
    return length - AfxReadStream(stream, length, 0, buffer);
}

afxBool _rwStreamReadChunkHeader(afxStream stream, uint32_t *type, uint32_t *length, uint32_t *version, uint32_t *buildNum)
{
    struct __rwMark          mark = { 0 };
    RwChunkHeaderInfo chunkHdrInfo = { 0 };

    if (AfxHasStreamReachedEnd(stream))
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
            AfxSeekStream(stream, readLength, afxSeekOrigin_CURR);
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

char* _rwStringStreamFindAndRead(char *string, afxStream stream)
{
    uint32_t readType;
    uint32_t readLength;
    uint32_t readVersion;

    while (_rwStreamReadChunkHeader(stream, &readType, &readLength, &readVersion, 0))
    {
        if (readType == afxChunkId_STRING)
        {
            return (StringStreamRead(string, stream, readLength));
        }

        if (readLength > 0)
        {
            AfxSeekStream(stream, readLength, afxSeekOrigin_CURR);
        }
    }
    return 0;
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

#pragma pack(push, 1)

 typedef struct _rwD3D9NativeTexture
{
    afxInt32 id; /* RwPlatformID,(rwID_D3D9) defined in batype.h */
    afxInt32 filterAndAddress; /* Same as in babintex.c */
    afxChar  name[32]; /* Texture name */
    afxChar  mask[32]; /* Texture mask name */
} _rwD3D9NativeTexture;

 typedef struct _rwD3D9NativeRaster
{
    afxNat32    format;         /* Raster format flags */
    D3DFORMAT   d3dFormat;      /* D3D pixel format */
    afxNat16    width;          /* Raster width */
    afxNat16    height;         /* Raster height */
    afxNat8     depth;          /* Raster depth */
    afxNat8     numMipLevels;   /* The number of mip levels to load */
    afxNat8     type;           /* The raster type */
    afxNat8     flags;          /* This raster has an alpha component, automipmapgen, etc */
} _rwD3D9NativeRaster;

#define HAS_ALPHA           (1<<0)
#define IS_CUBE             (1<<1)
#define USE_AUTOMIPMAPGEN   (1<<2)
#define IS_COMPRESSED       (1<<3)

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

_AVX afxError AfxRenameTextures(afxTxd txd, afxNat first, afxNat cnt, afxString const* name)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &txd, afxFcc_TXD);

    AfxDecatalogStrings(txd->strb, cnt, &txd->texNames[first]);
    AfxCatalogStrings(txd->strb, cnt, name, &txd->texNames[first]);

    return err;
}

_AVX afxError _AvxTxdStdDtorCb(afxTxd txd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &txd, afxFcc_TXD);

    afxObjectStash const stashes[] =
    {
        {
            .cnt = txd->texCnt,
            .siz = sizeof(txd->texInfos[0]),
            .var = (void**)&txd->texInfos
        },
        {
            .cnt = txd->texCnt,
            .siz = sizeof(txd->texNames[0]),
            .var = (void**)&txd->texNames
        },
        {
            .cnt = txd->texCnt,
            .siz = sizeof(txd->rasters[0]),
            .var = (void**)&txd->rasters
        }
    };
    AfxDeallocateInstanceData(txd, AFX_COUNTOF(stashes), stashes);

    return err;
}

_AVX afxError _AvxTxdStdCtorCb(afxTxd txd, void** args, afxNat invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &txd, afxFcc_TXD);

    avxTxdInfo const *txdi = ((avxTxdInfo const *)args[1]) + invokeNo;

    //afxDrawContext dctx = AfxGetProvider(0);

    afxNat texCnt = txdi->texCnt;
    txd->texCnt = texCnt;

    afxObjectStash const stashes[] =
    {
        {
            .cnt = texCnt,
            .siz = sizeof(txd->texInfos[0]),
            .var = (void**)&txd->texInfos
        },
        {
            .cnt = texCnt,
            .siz = sizeof(txd->texNames[0]),
            .var = (void**)&txd->texNames
        },
        {
            .cnt = texCnt,
            .siz = sizeof(txd->rasters[0]),
            .var = (void**)&txd->rasters
        }
    };

    if (AfxAllocateInstanceData(txd, AFX_COUNTOF(stashes), stashes)) AfxThrowError();
    else
    {
        afxRaster rasters[256];
        avxSampler samplers[256];
#if 0
        if (AfxAcquireRasters(dctx, texCnt, txdi->rasters, rasters))
            AfxThrowError();

        if (AfxAcquireSamplers(dctx, texCnt, txdi->samplers, samplers))
            AfxThrowError();
#endif
        for (afxNat i = 0; i < texCnt; i++)
        {
            AfxResetString(&txd->texNames[i]);
            txd->rasters[i] = NIL;
            AfxZero(&txd->texInfos[i], sizeof(txd->texInfos[0]));
        }

        AfxAcquireStringCatalogs(1, &txd->strb);

        if (err)
            AfxDeallocateInstanceData(txd, AFX_COUNTOF(stashes), stashes);
    }
    return err;
}

_AVX afxClassConfig const _AvxTexClsCfg =
{
    .fcc = afxFcc_TEX,
    .name = "Texture",
    .desc = "Texture"
};

_AVX afxClassConfig const _AvxTxdClsCfg =
{
    .fcc = afxFcc_TXD,
    .name = "Txd",
    .desc = "Texture Dictionary",
    .fixedSiz = sizeof(AFX_OBJECT(afxTxd)),
    .ctor = (void*)_AvxTxdStdCtorCb,
    .dtor = (void*)_AvxTxdStdDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AVX afxError AfxAcquireTxd(afxDrawInput din, avxTxdInfo const* info, afxTxd* txd)
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

_AVX afxError AfxLoadTxd(afxDrawInput din, afxStream in, afxTxd* dict)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);
    AfxAssertObjects(1, &in, afxFcc_IOB);
    AfxAssert(AfxIsStreamReadable(in));
    
    afxDrawContext dctx;
    AfxGetDrawInputContext(din, &dctx);

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

    urdMark txdHdr, hdr;
    if (!AfxFetchNextStreamChunk(in, afxChunkId_RW_TXD, &txdHdr)) AfxThrowError();
    else if (!AfxFetchNextStreamChunk(in, afxChunkId_STRUCT, &hdr)) AfxThrowError();
    else
    {
        AfxAssert(hdr.siz <= sizeof(binTexDict));
        if (AfxReadStream(in, hdr.siz, 0, &binTexDict) || ((myId != 0) && (binTexDict.deviceId != 0) && (myId != binTexDict.deviceId)))
            AfxThrowError();
    }

    if (!err)
    {
        afxString names[256];
        afxRasterInfo rasi[256] = { 0 };
        avxSamplerInfo sampi[256] = { 0 };
        afxNat texIdx = 0;

        afxTxd txd;
        avxTxdInfo txdi = { 0 };
        txdi.texCnt = binTexDict.numTextures;
        AfxAcquireTxd(din, &txdi, &txd);

        afxSize posnBkp = AfxAskStreamPosn(in);
        afxNat numTexs = binTexDict.numTextures;

        afxNat fsegCnt = 0;
        urdSegment fsegs[256];

        for (; texIdx < numTexs; texIdx++)
        {
            urdMark rasHdr, hdr;
            if (!AfxFetchNextStreamChunk(in, afxChunkId_RW_RASTER, &rasHdr)) AfxThrowError();
            else if (!AfxFetchNextStreamChunk(in, afxChunkId_STRUCT, &hdr))
                AfxThrowError();

            if (!err)
            {
                // Call the driver to read the texture
                {
                    struct/*_rwD3D9NativeTexture*/
                    {
                        afxInt32 id; // RwPlatformID,(rwID_D3D9) defined in batype.h
                        afxInt32 filterAndAddress;
                        afxChar  name[32];
                        afxChar  mask[32];
                    } nativeTexture;

                    if (AfxReadStream(in, sizeof(nativeTexture), 0, &nativeTexture))
                    {
                        AfxThrowError();
                    }

                    AfxMakeString(&names[texIdx], 0, nativeTexture.name, 0);
                            
                    avxSamplerInfo sampc = { 0 };

                    switch (nativeTexture.filterAndAddress & 0xFF)
                    {
                    case 0: sampc.minFilter = (sampc.magFilter = (sampc.mipFilter = avxTexelFilter_POINT)); break;
                    case 1: sampc.minFilter = (sampc.magFilter = (sampc.mipFilter = avxTexelFilter_POINT)); break; // rwFILTERNEAREST, /**<Point sampled */
                    case 2: sampc.minFilter = (sampc.magFilter = (sampc.mipFilter = avxTexelFilter_LINEAR)); break;// rwFILTERLINEAR, /**<Bilinear */
                    case 3: sampc.mipFilter = avxTexelFilter_POINT; break; // rwFILTERMIPNEAREST, /**<Point sampled per pixel mip map */
                    case 4: sampc.mipFilter = avxTexelFilter_LINEAR; break; // rwFILTERMIPLINEAR, /**<Bilinear per pixel mipmap */
                    case 5: sampc.minFilter = (sampc.magFilter = avxTexelFilter_LINEAR); sampc.mipFilter = avxTexelFilter_POINT; break; // rwFILTERLINEARMIPNEAREST, /**<MipMap interp point sampled */
                    case 6: sampc.minFilter = (sampc.magFilter = (sampc.mipFilter = avxTexelFilter_LINEAR)); break; // rwFILTERLINEARMIPLINEAR, /**<Trilinear */
                    default: AfxThrowError(); break;
                    }

                    switch ((nativeTexture.filterAndAddress >> 8) & 0x0F)
                    {
                    case 0: sampc.uvw[0] = avxTexelAddress_REPEAT; break;
                    case 1: sampc.uvw[0] = avxTexelAddress_REPEAT; break; // rwTEXTUREADDRESSWRAP,      /**<UV wraps (tiles) */
                    case 2: sampc.uvw[0] = avxTexelAddress_MIRROR; break; // rwTEXTUREADDRESSMIRROR,    /**<Alternate UV is flipped */
                    case 3: sampc.uvw[0] = avxTexelAddress_CLAMP; break; // rwTEXTUREADDRESSCLAMP,     /**<UV is clamped to 0-1 */
                    case 4: sampc.uvw[0] = avxTexelAddress_BORDER; break; // rwTEXTUREADDRESSBORDER, /**<Border color takes effect outside of 0-1 */
                    default: AfxThrowError(); break;
                    }

                    switch ((nativeTexture.filterAndAddress >> 12) & 0x0F)
                    {
                    case 0: sampc.uvw[1] = avxTexelAddress_REPEAT; break;
                    case 1: sampc.uvw[1] = avxTexelAddress_REPEAT; break; // rwTEXTUREADDRESSWRAP,      /**<UV wraps (tiles) */
                    case 2: sampc.uvw[1] = avxTexelAddress_MIRROR; break; // rwTEXTUREADDRESSMIRROR,    /**<Alternate UV is flipped */
                    case 3: sampc.uvw[1] = avxTexelAddress_CLAMP; break; // rwTEXTUREADDRESSCLAMP,     /**<UV is clamped to 0-1 */
                    case 4: sampc.uvw[1] = avxTexelAddress_BORDER; break; // rwTEXTUREADDRESSBORDER, /**<Border color takes effect outside of 0-1 */
                    default: AfxThrowError(); break;
                    }

                    sampi[texIdx] = sampc;

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

                    if (AfxReadStream(in, sizeof(nativeRaster), 0, &nativeRaster))
                    {
                        AfxThrowError();
                    }

                    rasi[texIdx].whd[0] = AfxMax(1, nativeRaster.width);
                    rasi[texIdx].whd[1] = AfxMax(1, nativeRaster.height);
                    rasi[texIdx].whd[2] = AfxMax(1, nativeRaster.depth);
                    rasi[texIdx].lodCnt = AfxMax(1, nativeRaster.numMipLevels);

                    afxBool automipmapgen = (nativeRaster.flags & USE_AUTOMIPMAPGEN);

                    rasi[texIdx].flags |= (nativeRaster.flags & IS_CUBE) != 0 ? afxRasterFlag_CUBEMAP : NIL;

                    //if (nativeRaster.flags & IS_COMPRESSED)
                    {
                        afxNat32 dxtFormat;
                        dxtFormat = nativeRaster.d3dFormat;

                        switch (nativeRaster.d3dFormat)
                        {
                        case D3DFMT_DXT1: rasi[texIdx].fmt = avxFormat_DXT1; break;
                        case D3DFMT_DXT3: rasi[texIdx].fmt = avxFormat_DXT3; break;
                        case D3DFMT_DXT5: rasi[texIdx].fmt = avxFormat_DXT5; break;
                        case D3DFMT_A8R8G8B8: rasi[texIdx].fmt = avxFormat_BGRA8; break;
                        case D3DFMT_A1R5G5B5: rasi[texIdx].fmt = avxFormat_RGB5A1; break;
                        case D3DFMT_A4R4G4B4: rasi[texIdx].fmt = avxFormat_RGBA4; break;
                        case D3DFMT_A2R10G10B10: rasi[texIdx].fmt = avxFormat_A2RGB10; break;
                        case D3DFMT_A2B10G10R10: rasi[texIdx].fmt = avxFormat_A2RGB10; break;
                        case D3DFMT_A8R3G3B2: rasi[texIdx].fmt = avxFormat_RG8; break;
                        };
                    }

                    avxFormatDescription pfd;
                    AfxDescribePixelFormat(rasi[texIdx].fmt, &pfd);

                    rasi[texIdx].whd[2] = nativeRaster.flags & IS_CUBE ? 6 : 1;
                    afxNat face = rasi[texIdx].whd[2];



                    afxUri uri;
                    AfxMakeUri(&uri, 0, nativeTexture.name, 0);
                    AfxRenameTextures(txd, texIdx, 1, &uri);

                    AfxAcquireRasters(dctx, 1, &rasi[texIdx], &txd->rasters[texIdx]);

#if 0
                    /* Load mips into raster */
                    do
                    {
                        fsegs[fsegCnt].start = AfxAskStreamPosn(iob);
                        fsegs[fsegCnt].encSiz = 0;

                        for (afxNat i = 0; i < rasi[texIdx].lodCnt; i++)
                        {
                            afxSize    size;

                            /* Size in bytes of mip to load */
                            if (AfxReadStream(iob, sizeof(size), 0, &size))
                            {
                                AfxThrowError();
                            }
                            fsegs[fsegCnt].encSiz += size;
                            AfxAdvanceStream(iob, size);
                        }


                        fsegs[fsegCnt].decSiz = fsegs[fsegCnt].encSiz;
                        ++fsegCnt;

                    } while (--face);
#else

                    //if (!_rwD3D9NativeTextureRead(iob, &nativeRaster, &sampi[i], &rasi[i]))
                    {
                        /* Load mips into raster */
                        for (afxNat face = 0; face < rasi[texIdx].whd[2]; face++)
                        {
                            afxWhd whd;
                            AfxWhdCopy(whd, rasi[texIdx].whd);


                            for (afxNat i = 0; i < rasi[texIdx].lodCnt; i++)
                            {
                                afxSize size;

                                /* Size in bytes of mip to load */
                                if (AfxReadStream(in, sizeof(size), 0, &size))
                                {
                                    AfxThrowError();
                                }

                                afxRasterIo iop = { 0 };
                                iop.rgn.lodIdx = i;
                                iop.rgn.origin[2] = face;
                                iop.rgn.whd[0] = whd[0];
                                iop.rgn.whd[1] = whd[1];
                                iop.rgn.whd[2] = 1;
                                iop.offset = AfxAskStreamPosn(in);
                                //iop.rowStride = size % iop.rgn.whd[1];// *(iop.rgn.whd[0] * pfd.stride / 8);
                                iop.rowCnt = iop.rgn.whd[1];
                                        
                                iop.decSiz = size;

                                afxNat pitch = iop.rgn.whd[0] >> iop.rgn.lodIdx;

                                if (!(nativeRaster.flags & IS_COMPRESSED))
                                {
                                    iop.rowStride = (iop.rgn.whd[0] * pfd.bpp) / 8;
                                }
                                else
                                {
                                    /* A DXT compressed format */
                                    /* 64Bits / 8 = 8. 2^3 = 8 */
                                    /* 128Bits / 8 = 16. 2^4 = 16 */
                                    afxNat bpp = (D3DFMT_DXT1 == nativeRaster.d3dFormat) ? 8 : 16;

                                    iop.rowStride = (iop.rgn.whd[0] + 3) / 4 * bpp;
                                    AfxAssert(8 <= iop.rowStride);
                                }

                                afxSize posn = AfxAskStreamPosn(in);
                                // Read the mip level
                                AfxUploadRaster(txd->rasters[texIdx], 1, &iop, in, 0);
                                AfxWaitForDrawBridge(dctx, 0, AFX_TIME_INFINITE);
                                //AfxSeekStream(iob, posn + size, afxSeekOrigin_BEGIN);

#if 0
                                afxUri2048 uri;
                                AfxMakeUri2048(&uri, NIL);
                                AfxFormatUri(&uri.uri, "../tmp/test-txd-dump-tex%d.tga", texIdx);
                                AfxPrintRaster(rasters[texIdx], NIL, 0, &uri.uri, 0);
#endif

                                AfxWhdHalf(whd, whd);
                            }
                        }
                    }
#endif
                }

                //_rwPluginRegistryReadDataChunks(&textureTKList, stream, newTex);
                urdMark extHdr;
                if (AfxFetchNextStreamChunk(in, afxChunkId_EXTENSION, &extHdr))
                {
                    AfxSkipSerializedExtensions(in, &extHdr);
                }
            }
        }

        //_rwPluginRegistryReadDataChunks(&texDictTKList, stream, texDict)
        urdMark extHdr;        
        if (AfxFetchNextStreamChunk(in, afxChunkId_EXTENSION, &extHdr))
        {
            AfxSkipSerializedExtensions(in, &extHdr);
        }
    }
    return err;
}

_AVX afxError AfxOpenTxd(afxDrawInput din, afxUri const* uri, afxTxd* dict)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);

    afxDrawContext dctx;
    AfxGetDrawInputContext(din, &dctx);

    afxStream iob;
    afxStreamInfo iobi = { 0 };
    iobi.usage = afxStreamUsage_FILE;
    iobi.flags = afxStreamFlag_READABLE;
    AfxAcquireStream(1, &iobi, &iob);

    if (AfxReopenFile(iob, uri, afxFileFlag_RX)) AfxThrowError();
    else
    {
        if (AfxLoadTxd(din, iob, dict))
            AfxThrowError();

        AfxReleaseObjects(1, &iob);
    }

    return err;
}
