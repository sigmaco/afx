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
 *                       (c) 2017 SIGMA, Engineering In Technology
 *                             <https://sigmaco.org/qwadro/>
 */

// This section is part of SIGMA GL/2.

#ifndef AFX_BUFFER_H
#define AFX_BUFFER_H

#include "qwadro/draw/afxDrawDefs.h"

typedef enum afxBufferAccess
{
    afxBufferAccess_R       = AfxGetBitOffset(0), // Mapped range is readable by CPU.
    afxBufferAccess_W       = AfxGetBitOffset(1), // Mapped range is writeable by CPU.
    afxBufferAccess_RW      = (afxBufferAccess_R | afxBufferAccess_W), // Mapped range is readable and writeable by CPU.
    afxBufferAccess_X       = AfxGetBitOffset(2), // Mapped range will be executable by GPU while being read and/or write by CPU.
    afxBufferAccess_COHERENT= (afxBufferAccess_X | AfxGetBitOffset(3)), // Data written in mapped range will be immediately visible by GPU and CPU.
} afxBufferAccess;

typedef enum afxBufferUsage
{
    afxBufferUsage_VERTEX   = AfxGetBitOffset(0), // The buffer can be used as a vertex buffer.
    afxBufferUsage_INDEX    = AfxGetBitOffset(1), // The buffer can be used as an index buffer.
    afxBufferUsage_UNIFORM  = AfxGetBitOffset(2), // The buffer can be used as a uniform buffer.
    afxBufferUsage_STORAGE  = AfxGetBitOffset(3), // The buffer can be used as a storage buffer.
    afxBufferUsage_INDIRECT = AfxGetBitOffset(4), // The buffer can be used as to store indirect command arguments.
    afxBufferUsage_QUERY    = AfxGetBitOffset(5), // The buffer can be used to capture query results.
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

#ifdef _AFX_DRAW_C
#ifdef _AFX_BUFFER_C
#ifndef _AFX_BUFFER_IMPL
AFX_OBJECT(afxBuffer)
#else
struct afxBaseBuffer
#endif
{
    afxNat          siz;
    afxBufferUsage  usage;
    afxBufferAccess access;
    afxByte*        bytemap;
    void*           (*map)(afxBuffer, afxNat offset, afxNat range, afxFlags flags);
    afxError        (*unmap)(afxBuffer);
};
#endif
#endif

AVX afxDrawContext  AfxGetBufferContext(afxBuffer buf);

AVX afxNat          AfxGetBufferCapacity(afxBuffer buf);

AVX afxBufferUsage  AfxGetBufferUsage(afxBuffer buf);
AVX afxBufferUsage  AfxTestBufferUsage(afxBuffer buf, afxBufferUsage usage);

AVX afxBufferAccess AfxGetBufferAccess(afxBuffer buf);
AVX afxBufferAccess AfxTestBufferAccess(afxBuffer buf, afxBufferAccess access);

AVX void*           AfxMapBufferRange(afxBuffer buf, afxNat offset, afxNat range, afxFlags flags);
AVX void            AfxUnmapBufferRange(afxBuffer buf);

AVX afxError        AfxDumpBuffer(afxBuffer buf, afxNat offset, afxNat range, void *dst);
AVX afxError        AfxDumpBuffer2(afxBuffer buf, afxNat offset, afxNat stride, afxNat cnt, void *dst, afxNat dstStride);

AVX afxError        AfxUpdateBuffer(afxBuffer buf, afxNat offset, afxNat range, void const *src);
AVX afxError        AfxUpdateBuffer2(afxBuffer buf, afxNat offset, afxNat range, afxNat stride, void const *src, afxNat srcStride);

AVX afxError        AfxUpdateBufferRegion(afxBuffer buf, afxBufferRegion const* rgn, void const* src, afxNat srcStride);

////////////////////////////////////////////////////////////////////////////////

AVX afxError        AfxAcquireBuffers(afxDrawContext dctx, afxNat cnt, afxBufferSpecification const spec[], afxBuffer buffers[]);

#endif//AFX_BUFFER_H
