/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *          Q W A D R O   4 D   R E N D E R I N G   I N F R A S T R U C T U R E
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This section is part of SIGMA Simulation Infrastructure.

#ifndef ARX_IMPL___STORAGE_H
#define ARX_IMPL___STORAGE_H

#include "qwadro/sim/afxSimulation.h"

AFX_DEFINE_STRUCT(arxMemoryReq)
{
    afxSize     size;
    afxFlags    propFlags; // device, visible, coherent, etc.
};

AFX_DEFINE_STRUCT(_arxMemory)
{
    // EXU controlling this unit
    afxLink iommu;
    // buffers linked/backed by this storage unit (sbuf.storage)
    //afxChain buffers;
    // the capacity of this unit
    afxUnit size;
    // the alignment of this unit
    afxUnit alignment;
    // binding
    afxUnit mmu; // memory holder
    //afxSize offset; // offset into memory holder bound to this storage block.
    // Persistent mapping required at acquisition. Do not allow unmapping.
    afxBool     permanentlyMapped;
    afxAtom32   pendingRemap;
    afxSize     mapOffset;
    afxUnit     mapRange;
    afxFlags    mapFlags;
    afxByte*    mapPtr; // used by mapping
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
    } hostedAlloc;
};

#ifdef _ARX_BUFFER_C
#ifdef _ARX_BUFFER_IMPL
AFX_OBJECT(_arxBuffer)
#else
AFX_OBJECT(arxBuffer)
#endif
{
    arxBufferUsage  usage;
    arxBufferFlags  flags;
    afxString       tag; // debug
    void*           udd;

    afxMask         sharingMask;
    arxBuffer       base; // handle used by subacquision of another buffer
    afxSize         from; // offset used by subacquision of another buffer
    //afxUnit         size; // capacity
    arxFormat       fmt; // for tensor buffer

    // STORAGE
    // required size of a storage unit.
    afxUnit         reqSiz;
    // required alignment of a storage unit, which is presumedly constant.
    afxUnit         reqAlign;
    // required memory conditions for a storage unit.
    afxFlags        reqMemType;
    //_asxMemory  storage[1]; // non-sparse
    // the storage unit linked to this buffer.
    _arxMemory      mem;
    // the offset into the storage unit linked to this buffer.
    afxSize         memBase;
};
#endif//_ASX_BUFFER_C

ARX afxClassConfig const _ARX_SBUF_CLASS_CONFIG;

#endif//ARX_IMPL___STORAGE_H
