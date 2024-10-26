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

#ifndef AVX_BUFFER_H
#define AVX_BUFFER_H

#include "qwadro/inc/draw/afxDrawDefs.h"

#define AFX_BUF_ALIGNMENT 64
#define AFX_BUF_ALIGN(siz_) AFX_ALIGNED_SIZE((siz_), AFX_BUF_ALIGNMENT)

typedef enum afxBufferFlags
{
    afxBufferFlag_R             = AFX_BIT(0), // Mapped range is readable by CPU.
    afxBufferFlag_W             = AFX_BIT(1), // Mapped range is writeable by CPU.
    afxBufferFlag_RW            = (afxBufferFlag_R | afxBufferFlag_W), // Mapped range is readable and writeable by CPU.
    afxBufferFlag_X             = AFX_BIT(2), // Mapped range will be executable by GPU while being read and/or write by CPU.
    afxBufferFlag_RWX           = (afxBufferFlag_RW | afxBufferFlag_X),
    afxBufferFlag_COHERENT      = (afxBufferFlag_X | AFX_BIT(3)), // Data written in mapped range will be immediately visible by GPU and CPU.
    afxBufferFlag_ACCESS_MASK   = (afxBufferFlag_RWX | afxBufferFlag_COHERENT)
} afxBufferFlags;

typedef enum afxBufferUsage
{
    afxBufferUsage_SRC      = AFX_BIT(0), // Can be used as the source of a copy operation.
    afxBufferUsage_DST      = AFX_BIT(1), // Can be used as the destination of a copy or write operation.
    afxBufferUsage_VERTEX   = AFX_BIT(2), // Can be used as a vertex buffer; aka VBO.
    afxBufferUsage_INDEX    = AFX_BIT(3), // Can be used as an index buffer; aka IBO.
    afxBufferUsage_UNIFORM  = AFX_BIT(4), // Can be used as a uniform buffer; aka UBO.
    afxBufferUsage_STORAGE  = AFX_BIT(5), // Can be used as a storage buffer; aka SSBO.
    afxBufferUsage_INDIRECT = AFX_BIT(6), // Can be used as to store indirect command arguments; aka DBO.
    afxBufferUsage_QUERY    = AFX_BIT(7), // Can be used to capture query results; aka QBO.    
    afxBufferUsage_MATRIX   = AFX_BIT(8) // Can be used to fetch (usually texels) data; aka TBO.
} afxBufferUsage;

AFX_DEFINE_STRUCT(afxBufferInfo)
{
    afxUnit         bufCap; // capacity
    afxBufferUsage  usage;
    afxBufferFlags  flags;
    avxFormat       fmt; // used for MATRIX/FETCH buffers.
    void*           udd;
};

AFX_DEFINE_STRUCT(afxBufferCopy)
{
    afxSize         srcOffset; /// is the starting offset in bytes from the start of srcBuffer.
    afxSize         dstOffset; /// is the starting offset in bytes from the start of dstBuffer.
    afxUnit         range; /// is the number of bytes to copy.
};

AFX_DEFINE_STRUCT(afxBufferIo)
{
    afxSize         srcOffset; /// is the starting offset in bytes from the start of srcBuffer.
    afxSize         dstOffset; /// is the starting offset in bytes from the start of dstBuffer.
    afxUnit         srcStride; // [!] only if supported by device, else case it must be 1.
    afxUnit         dstStride; // [!] only if supported by device, else case it must be 1.
    afxUnit         rowCnt; /// is the number of rows to stream in/out.
};

AVX afxDrawContext  AfxGetBufferContext(afxBuffer buf);

AVX void*           AfxGetBufferUdd(afxBuffer buf);

AVX afxUnit          AfxGetBufferCapacity(afxBuffer buf, afxUnit from);

AVX afxBufferUsage  AfxGetBufferUsage(afxBuffer buf);
AVX afxBufferUsage  AfxTestBufferUsage(afxBuffer buf, afxBufferUsage usage);

AVX afxBufferFlags  AfxGetBufferAccess(afxBuffer buf);
AVX afxBufferFlags  AfxTestBufferAccess(afxBuffer buf, afxBufferFlags access);

AVX afxError        AfxMapBuffer(afxBuffer buf, afxUnit offset, afxUnit range, afxFlags flags, void** placeholder);
AVX void            AfxUnmapBuffer(afxBuffer buf, afxBool wait);

AVX afxError        AfxUploadBuffer(afxBuffer buf, afxUnit opCnt, afxBufferIo const ops[], afxStream in, afxUnit portId);
AVX afxError        AfxDownloadBuffer(afxBuffer buf, afxUnit opCnt, afxBufferIo const ops[], afxStream out, afxUnit portId);

AVX afxError        AfxDumpBuffer(afxBuffer buf, afxUnit opCnt, afxBufferIo const ops[], void* dst, afxUnit portId);
AVX afxError        AfxUpdateBuffer(afxBuffer buf, afxUnit opCnt, afxBufferIo const ops[], void const* src, afxUnit portId);

////////////////////////////////////////////////////////////////////////////////

AVX afxError        AfxAcquireBuffers(afxDrawContext dctx, afxUnit cnt, afxBufferInfo const infos[], afxBuffer buffers[]);

#endif//AVX_BUFFER_H
