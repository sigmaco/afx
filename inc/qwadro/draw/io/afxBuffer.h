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

#include "qwadro/draw/afxDrawDefs.h"

#define AFX_BUF_ALIGNMENT 64
#define AFX_BUF_ALIGN(siz_) AFX_ALIGN((siz_), AFX_BUF_ALIGNMENT)

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
};

AFX_DEFINE_STRUCT(afxBufferRegion)
{
    afxNat base;
    afxNat range;
    afxNat stride;
    afxNat offset;
    afxNat unitSiz;
};

AFX_DEFINE_STRUCT(afxBufferIoOp)
{
    afxNat              srcOffset; /// is the starting offset in bytes from the start of srcBuffer.
    afxNat              srcStride;
    afxNat              dstOffset; /// is the starting offset in bytes from the start of dstBuffer.
    afxNat              dstStride;
    afxNat              range; /// is the number of bytes to copy.
};

#ifdef _AVX_DRAW_C
#ifdef _AVX_BUFFER_C
#ifndef _AVX_BUFFER_IMPL
AFX_OBJECT(afxBuffer)
#else
struct afxBaseBuffer
#endif
{
    afxNat          cap;
    afxBufferUsage  usage;
    afxBufferAccess access;
    afxByte*        bytemap;
    afxSize         mappedOffset;
    afxNat          mappedRange;
    afxFlags        mappedFlags;
    afxAtom32       pendingRemap;
    afxError        (*remap)(afxBuffer,afxNat,afxNat,afxFlags,afxNat*,afxNat*);
    afxError        (*unmap)(afxBuffer,afxNat*,afxNat*);
};
#endif//_AVX_BUFFER_C
#endif//_AVX_DRAW_C

AVX afxDrawContext  AfxGetBufferContext(afxBuffer buf);

AVX afxNat          AfxGetBufferCapacity(afxBuffer buf);

AVX afxBufferUsage  AfxGetBufferUsage(afxBuffer buf);
AVX afxBufferUsage  AfxTestBufferUsage(afxBuffer buf, afxBufferUsage usage);

AVX afxBufferAccess AfxGetBufferAccess(afxBuffer buf);
AVX afxBufferAccess AfxTestBufferAccess(afxBuffer buf, afxBufferAccess access);

AVX void*           AfxMapBuffer(afxBuffer buf, afxNat offset, afxNat range, afxFlags flags);
AVX void            AfxUnmapBuffer(afxBuffer buf, afxBool wait);

AVX afxError        AfxDumpBuffer(afxBuffer buf, afxNat offset, afxNat range, void *dst);
AVX afxError        AfxDumpBuffer2(afxBuffer buf, afxNat offset, afxNat stride, afxNat cnt, void *dst, afxNat dstStride);

AVX afxError        AfxUpdateBuffer(afxBuffer buf, afxNat offset, afxNat range, void const *src);
AVX afxError        AfxUpdateBuffer2(afxBuffer buf, afxNat offset, afxNat range, afxNat stride, void const *src, afxNat srcStride);

AVX afxError        AfxUpdateBufferRegion(afxBuffer buf, afxBufferRegion const* rgn, void const* src, afxNat srcStride);

AVX afxError        AfxUploadBuffer(afxBuffer buf, afxNat opCnt, afxBufferIoOp const ops[], afxStream in);
AVX afxError        AfxDownloadBuffer(afxBuffer buf, afxNat opCnt, afxBufferIoOp const ops[], afxStream out);

////////////////////////////////////////////////////////////////////////////////

AVX afxError        AfxAcquireBuffers(afxDrawContext dctx, afxNat cnt, afxBufferSpecification const spec[], afxBuffer buffers[]);

#endif//AVX_BUFFER_H
