#ifndef RW_ASSET_SERIALIZER_H
#define RW_ASSET_SERIALIZER_H

#include "qwadro/io/afxStream.h"

#ifdef __e2renderware__
#    define RWU 
#    define RWU_INL
#else
#    define RWU
#    define RWU_INL
#endif

typedef enum
{
    RW_CHUNK_IDENT_STRUCT = 1,
    RW_CHUNK_IDENT_STRING,
    RW_CHUNK_IDENT_EXT,
    RW_CHUNK_IDENT_CAM = 5,
    RW_CHUNK_IDENT_TEX,
    RW_CHUNK_IDENT_MTL,
    RW_CHUNK_IDENT_MTL_TABLE,
    RW_CHUNK_IDENT_SCENIC_SECTOR,
    RW_CHUNK_IDENT_PLANE_SECTOR,
    RW_CHUNK_IDENT_NODE_NEST = 14,
    RW_CHUNK_IDENT_GEOM,
    RW_CHUNK_IDENT_BUNDLE,
    RW_CHUNK_IDENT_LIGHT = 18,
    RW_CHUNK_IDENT_COMPONENT = 20,
    RW_CHUNK_IDENT_NATIVE_TEX,
    RW_CHUNK_IDENT_TXD,
    RW_CHUNK_IDENT_GEOM_NEST = 26,
    RW_CHUNK_IDENT_PIPELINE_DATA = 31, // ext
    RW_CHUNK_IDENT_BIN_MESH = 1294, // etc
    RW_CHUNK_IDENT_NATIVE_DATA = 1296 // etc
} rwChunkIdent;

AFX_DEFINE_STRUCT(rwChunkHdr)
{
    rwChunkIdent ident;
    afxNat32 len, ver;
};

typedef afxByte rwChunkData[];

#endif//RW_ASSET_SERIALIZER_H