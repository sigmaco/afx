/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                      Q W A D R O   D R A W   I / O   S Y S T E M
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

  //////////////////////////////////////////////////////////////////////////////
 // The Qwadro Video Graphics Infrastructure                                 //
//////////////////////////////////////////////////////////////////////////////

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>
// This software is part of Advanced Video Graphics Extensions & Experiments.

#ifndef AVX_STORAGE_DDK_H
#define AVX_STORAGE_DDK_H

#include "../exec/afxSystemDDK.h"
#include "qwadro/draw/afxDrawSystem.h"

typedef enum _avxMemFlag
{
    _avxMemFlag_MAPPEABLE = AFX_BITMASK(0), // visible to CPU
    _avxMemFlag_CACHED = AFX_BITMASK(1), // cached by CPU
    _avxMemFlag_ENCLAVED = AFX_BITMASK(2), // virtually separated portion
    _avxMemFlag_EMBEDDED = AFX_BITMASK(3) // on-chip memory
} _avxMemFlags;

AFX_DEFINE_STRUCT(_avxRasStorage)
{
    afxLink iommu;
    // binding
    afxUnit mmu; // memory holder
    afxSize size; // offset into memory holder bound to this storage block.
    // Persistent mapping required at acquisition. Do not allow unmapping.
    afxBool     permanentlyMapped;
    afxAtom32   pendingRemap;
    afxSize     mapOffset;
    afxUnit     mapRange;
    afxFlags    mapFlags;
    afxByte*    mapPtr; // used by mapping
    union
    {
        struct
        {
            union
            {
                afxSize     addr;
                afxByte*    bytemap;
                afxUnit8*   u8;
                afxUnit16*  u16;
                afxUnit32*  u32;
                afxInt8*    i8;
                afxInt16*   i16;
                afxInt32*   i32;
                afxReal32*  f32;
                afxReal64*  f64;
                afxV2d*     f32v2;
                afxV3d*     f32v3;
                afxV4d*     f32v4;
            };
            afxBool external;
        } host;
        struct
        {
            int fd;
        } fd;
        struct
        {
            afxString type;
            union
            {
                void* handle;
                void* name;
            };
        } w32;
    };
};

AFX_DEFINE_STRUCT(_avxBufStorage)
{
    afxLink iommu;
    // binding
    afxUnit mmu; // memory holder
    afxSize size; // offset into memory holder bound to this storage block.
    // Persistent mapping required at acquisition. Do not allow unmapping.
    afxBool     permanentlyMapped; // TODO: Eliminate it? Now we have ref-counted mapping.
    afxAtom32   pendingRemap;
    afxSize     mapOffset;
    afxUnit     mapRange;
    afxFlags    mapFlags;
    afxUnit     mapRefCnt;
    afxByte*    mapPtr; // used by mapping
    union
    {
        struct
        {
            union
            {
                afxSize     addr;
                afxByte*    bytemap;
                afxUnit8*   u8;
                afxUnit16*  u16;
                afxUnit32*  u32;
                afxInt8*    i8;
                afxInt16*   i16;
                afxInt32*   i32;
                afxReal32*  f32;
                afxReal64*  f64;
                afxV2d*     f32v2;
                afxV3d*     f32v3;
                afxV4d*     f32v4;
            };
            afxBool external;
        } host;
        struct
        {
            int fd;
        } fd;
        struct
        {
            afxString type;
            union
            {
                void* handle;
                void* name;
            };
        } w32;
    };
};

AFX_DEFINE_STRUCT(_avxBufferRemapping)
{
    avxBuffer       buf;
    afxSize         offset;
    afxUnit         range;
    avxMappingFlags flags;
    afxBool         unmap;
    void**          placeholder;
};

AVXINL void _AvxAllocateRasters(afxUnit cnt, avxRaster rasters[]);
AVXINL void _AvxAllocateBuffers(afxDrawSystem dsys, afxUnit cnt, avxBufferInfo const infos[], avxBuffer buffers[]);

AVXINL void* _AvxGetClientRasterData(avxRaster ras, afxSize from);

AVXINL void* _AvxGetClientBufferData(avxBuffer buf, afxSize from);

AVX afxError _AvxDsysCohereMappedBuffersCb_SW(afxDrawSystem dsys, afxBool invalidate, afxUnit cnt, avxBufferedMap const maps[]);
AVX afxError _AvxDsysTransferCb_SW(afxDrawSystem dsys, avxTransference* ctrl, afxUnit opCnt, void const* ops);
AVX afxError _AvxDsysRemapBuffersCb_SW(afxDrawSystem dsys, afxBool unmap, afxUnit cnt, _avxBufferRemapping const maps[]);

AVXINL afxError _AvxDsysDeallocateRastersCb_SW(afxDrawSystem dsys, afxUnit cnt, avxRaster rasters[]);
AVXINL afxError _AvxDsysAllocateRastersCb_SW(afxDrawSystem dsys, afxUnit cnt, avxRasterInfo const infos[], avxRaster rasters[]);
AVXINL afxError _AvxDsysDeallocateBuffersCb_SW(afxDrawSystem dsys, afxUnit cnt, avxBuffer buffers[]);
AVXINL afxError _AvxDsysAllocateBuffersCb_SW(afxDrawSystem dsys, afxUnit cnt, avxBufferInfo const infos[], avxBuffer buffers[]);

#endif//AVX_STORAGE_DDK_H
