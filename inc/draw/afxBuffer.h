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
#define AFX_BUF_ALIGN(siz_) AFX_ALIGNED_SIZEOF((siz_), AFX_BUF_ALIGNMENT)

typedef enum afxBufferAccess
{
    afxBufferAccess_R       = AFX_BIT(0), // Mapped range is readable by CPU.
    afxBufferAccess_W       = AFX_BIT(1), // Mapped range is writeable by CPU.
    afxBufferAccess_RW      = (afxBufferAccess_R | afxBufferAccess_W), // Mapped range is readable and writeable by CPU.
    afxBufferAccess_X       = AFX_BIT(2), // Mapped range will be executable by GPU while being read and/or write by CPU.
    afxBufferAccess_COHERENT= (afxBufferAccess_X | AFX_BIT(3)), // Data written in mapped range will be immediately visible by GPU and CPU.
} afxBufferAccess;

typedef enum afxBufferUsage
{
    afxBufferUsage_SRC      = AFX_BIT(0), // The buffer can be used as the source of a copy operation.
    afxBufferUsage_DST      = AFX_BIT(1), // The buffer can be used as the destination of a copy or write operation.
    afxBufferUsage_VERTEX   = AFX_BIT(2), // The buffer can be used as a vertex buffer.
    afxBufferUsage_INDEX    = AFX_BIT(3), // The buffer can be used as an index buffer.
    afxBufferUsage_UNIFORM  = AFX_BIT(4), // The buffer can be used as a uniform buffer.
    afxBufferUsage_STORAGE  = AFX_BIT(5), // The buffer can be used as a storage buffer.
    afxBufferUsage_INDIRECT = AFX_BIT(6), // The buffer can be used as to store indirect command arguments.
    afxBufferUsage_QUERY    = AFX_BIT(7), // The buffer can be used to capture query results.
} afxBufferUsage;

AFX_DEFINE_STRUCT(afxBufferSpecification)
{
    afxNat          siz;
    afxBufferAccess access;
    afxBufferUsage  usage;
    void const*     src;
    void*           udd;
};

AFX_DEFINE_STRUCT(afxBufferCopy)
{
    afxSize             srcOffset; /// is the starting offset in bytes from the start of srcBuffer.
    afxSize             dstOffset; /// is the starting offset in bytes from the start of dstBuffer.
    afxNat              range; /// is the number of bytes to copy.
};

AFX_DEFINE_STRUCT(afxBufferIo)
{
    afxSize             srcOffset; /// is the starting offset in bytes from the start of srcBuffer.
    afxSize             dstOffset; /// is the starting offset in bytes from the start of dstBuffer.
    afxNat              srcStride; // [!] only if supported by device, else case it is 1.
    afxNat              dstStride; // [!] only if supported by device, else case it is 1.
    afxNat              rowCnt; /// is the number of rows to stream in/out.
};

AVX afxDrawContext  AfxGetBufferContext(afxBuffer buf);

AVX void*           AfxGetBufferUdd(afxBuffer buf);

AVX afxNat          AfxGetBufferCapacity(afxBuffer buf, afxNat from);

AVX afxBufferUsage  AfxGetBufferUsage(afxBuffer buf);
AVX afxBufferUsage  AfxTestBufferUsage(afxBuffer buf, afxBufferUsage usage);

AVX afxBufferAccess AfxGetBufferAccess(afxBuffer buf);
AVX afxBufferAccess AfxTestBufferAccess(afxBuffer buf, afxBufferAccess access);

AVX afxError        AfxMapBuffer(afxBuffer buf, afxNat offset, afxNat range, afxFlags flags, void** placeholder);
AVX void            AfxUnmapBuffer(afxBuffer buf, afxBool wait);

AVX afxError        AfxUploadBuffer(afxBuffer buf, afxNat portId, afxNat opCnt, afxBufferIo const ops[], afxStream in);
AVX afxError        AfxDownloadBuffer(afxBuffer buf, afxNat portId, afxNat opCnt, afxBufferIo const ops[], afxStream out);

AVX afxError        AfxDumpBuffer(afxBuffer buf, afxNat portId, afxNat opCnt, afxBufferIo const ops[], void *dst);
AVX afxError        AfxUpdateBuffer(afxBuffer buf, afxNat portId, afxNat opCnt, afxBufferIo const ops[], void const *src);

////////////////////////////////////////////////////////////////////////////////

AVX afxError        AfxAcquireBuffers(afxDrawContext dctx, afxNat cnt, afxBufferSpecification const spec[], afxBuffer buffers[]);

#endif//AVX_BUFFER_H
