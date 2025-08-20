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

#ifndef ASX_BUFFER_H
#define ASX_BUFFER_H

#include "qwadro/inc/math/afxSphere.h"
#include "qwadro/inc/sim/afxSimDefs.h"

typedef enum asxFormat
{
    asxFormat_DaKeyframes32f, // Da, Keyframes32f
    asxFormat_DaK32fC32f, // Da, K32f, C32f
    asxFormat_DaIdentity, // Da, Identity
    asxFormat_DaConstant32f, // Da, Constant32f
    asxFormat_D3Constant32f, // D3, Constant32f
    asxFormat_D4Constant32f, // D4, Constant32f
    // still unsupported
    asxFormat_DaK16uC16u, // Da, K16u, C16u
    asxFormat_DaK8uC8u, // Da, K8u, C8u
    asxFormat_D4nK16uC15u, // D4n, K16u, C15u
    asxFormat_D4nK8uC7u, // D4n, K8u, C7u
    asxFormat_D3K16uC16u, // D3, K16u, C16u
    asxFormat_D3K8uC8u, // D3, K8u, C8u
    asxFormat_D9i1K16uC16u, // D9i1, K16u, C16u    
    asxFormat_D9i3K16uC16u, // D9i3, K16u, C16u
    asxFormat_D9i1K8uC8u, // D9i1, K8u, C8u
    asxFormat_D9i3K8uC8u, // D9i3, K8u, C8u
    asxFormat_D3i1K32fC32f, // D3i1, K32f, C32f
    asxFormat_D3i1K16uC16u, // D3i1, K16u, C16u
    asxFormat_D3i1K8uC8u // D3i1, K8u, C8u    

    // Let curve formats at first to be retrocompatible with old code.



} asxFormat;

typedef enum asxBufferUsage
{
    asxBufferUsage_VERTEX = AFX_BITMASK(0), // used to hold mesh triangulation vertices
    asxBufferUsage_INDEX = AFX_BITMASK(1), // used to hold mesh triangulation indices
    asxBufferUsage_DRAW = AFX_BITMASK(2), // used to be drawn by draw system
    asxBufferUsage_CURVE = AFX_BITMASK(3), // used to hold geometric curves
    asxBufferUsage_MATRIX = AFX_BITMASK(4)
} asxBufferUsage;

typedef enum asxBufferFlag
{
    asxBufferFlag_R = AFX_BITMASK(0),
    asxBufferFlag_W = AFX_BITMASK(1),
    asxBufferFlag_X = AFX_BITMASK(2),
    asxBufferFlag_RW = (asxBufferFlag_R | asxBufferFlag_W),
    asxBufferFlag_WX = (asxBufferFlag_W | asxBufferFlag_X),
    asxBufferFlag_RWX = (asxBufferFlag_R | asxBufferFlag_W | asxBufferFlag_X),
    asxBufferFlag_ACCESS = (asxBufferFlag_R | asxBufferFlag_W | asxBufferFlag_X)
} asxBufferFlags;

AFX_DEFINE_STRUCT(asxBufferInfo)
{
    // The size in bytes of the buffer to be created.
    afxUnit         size;
    // A bitmask specifying allowed usages of the buffer.
    asxBufferUsage  usage;
    // A bitmask specifying additional parameters of the buffer.
    asxBufferFlags  flags;
    // A user-defined data.
    void*           udd;
    afxString       tag;

    // A bitmask of device bridges that will access this buffer.
    afxMask         sharingMask;
    // A asxBuffer on which the view will be created.
    asxBuffer       base;
    // An offset in bytes from the base address of the buffer.
    afxSize         from;
    // A asxFormat describing the format of the data elements in the buffer.
    asxFormat       fmt; // used for TENSOR buffers.

    // Should this buffer be mapped at creation?
    afxBool         mapped;
    // Possibly initial data, to avoid boilerplate with inconvenient mappings.
    void*           data;
    afxUnit         dataSiz;
};

AFX_DEFINE_STRUCT(asxBufferCopy)
{
    afxSize         srcOffset; // is the starting offset in bytes from the start of srcBuffer.
    afxSize         dstOffset; // is the starting offset in bytes from the start of dstBuffer.
    afxUnit         range; // is the number of bytes to copy.
};

AFX_DEFINE_STRUCT(asxBufferIo)
{
    afxSize         srcOffset; // is the starting offset in bytes from the start of srcBuffer.
    afxSize         dstOffset; // is the starting offset in bytes from the start of dstBuffer.
    afxUnit         srcStride; // [!] only if supported by device, else case it must be 1.
    afxUnit         dstStride; // [!] only if supported by device, else case it must be 1.
    afxUnit         rowCnt; // is the number of rows to stream in/out.
};

ASX afxSimulation   AsxGetBufferContext(asxBuffer sbuf);

ASX void*           AsxGetBufferUdd(asxBuffer sbuf);

ASX afxUnit         AsxGetBufferCapacity(asxBuffer sbuf, afxUnit from);

ASX asxBufferUsage  AsxGetBufferUsage(asxBuffer sbuf, asxBufferUsage usage);

ASX asxBufferFlags  AsxGetBufferAccess(asxBuffer sbuf, asxBufferFlags access);

ASX void*           AsxGetBufferData(asxBuffer sbuf, afxSize from);

////////////////////////////////////////////////////////////////////////////////

ASX afxError        AsxAcquireBuffers(afxSimulation sim, afxUnit cnt, asxBufferInfo const infos[], asxBuffer buffers[]);

#endif//ASX_BUFFER_H
