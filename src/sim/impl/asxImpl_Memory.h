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

// This section is part of SIGMA Simulation Infrastructure.

#ifndef ASX_IMPL___STORAGE_H
#define ASX_IMPL___STORAGE_H

#include "qwadro/inc/sim/afxSimulation.h"

AFX_DEFINE_STRUCT(asxMemoryReq)
{
    afxSize     size;
    afxFlags    propFlags; // device, visible, coherent, etc.
};

AFX_DEFINE_STRUCT(_asxMemory)
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

#ifdef _ASX_BUFFER_C
#ifdef _ASX_BUFFER_IMPL
AFX_OBJECT(_asxBuffer)
#else
AFX_OBJECT(asxBuffer)
#endif
{
    asxBufferUsage  usage;
    asxBufferFlags  flags;
    afxString       tag; // debug
    void*           udd;

    afxMask         sharingMask;
    asxBuffer       base; // handle used by subacquision of another buffer
    afxSize         from; // offset used by subacquision of another buffer
    //afxUnit         size; // capacity
    asxFormat       fmt; // for tensor buffer

    // STORAGE
    // required size of a storage unit.
    afxUnit         reqSiz;
    // required alignment of a storage unit, which is presumedly constant.
    afxUnit         reqAlign;
    // required memory conditions for a storage unit.
    afxFlags        reqMemType;
    //_asxMemory  storage[1]; // non-sparse
    // the storage unit linked to this buffer.
    _asxMemory      mem;
    // the offset into the storage unit linked to this buffer.
    afxSize         memBase;
};
#endif//_ASX_BUFFER_C

ASX afxClassConfig const _ASX_SBUF_CLASS_CONFIG;

#endif//ASX_IMPL___STORAGE_H
