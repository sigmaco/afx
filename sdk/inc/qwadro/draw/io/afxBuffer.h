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

#ifdef _AFX_DRAW_C
#ifdef _AFX_BUFFER_C
#ifndef _AFX_BUFFER_IMPL
AFX_OBJECT(afxBuffer)
#else
struct afxBaseBuffer
#endif
{
    afxNat              siz;
    afxBufferUsage      usage;
    afxBufferAccess     access;
    afxByte*            bytemap;
    void*               (*map)(afxBuffer, afxNat offset, afxNat range, afxFlags flags);
    afxError            (*unmap)(afxBuffer);
};
#endif
#endif

AFX_DEFINE_STRUCT(afxBufferCopyOp)
{
    afxNat              srcOffset; /// is the starting offset in bytes from the start of srcBuffer.
    afxNat              dstOffset; /// is the starting offset in bytes from the start of dstBuffer.
    afxNat              range; /// is the number of bytes to copy.
};

AFX_DEFINE_STRUCT(afxBufferIoOp)
{
    afxNat              range; /// is the number of bytes to copy.
    afxNat              srcOffset; /// is the starting offset in bytes from the start of srcBuffer.
    afxNat              srcStride;
    afxNat              dstOffset; /// is the starting offset in bytes from the start of dstBuffer.
    afxNat              dstStride;
};

AFX_DEFINE_STRUCT(afxBufferSpecification)
{
    afxNat              siz;
    afxBufferAccess     access;
    afxBufferUsage      usage;
    void const*         src;
};

AFX_DEFINE_STRUCT(afxBufferRef)
{
    afxBuffer           buf;
    afxFormat           fmt;
    afxNat              off;
    afxNat              range;
};

AFX_DEFINE_STRUCT(afxMappedBufferRange)
{
    afxMappedBufferRange*   next;
    afxBuffer               buf;
    afxNat                  offset;
    afxNat                  size;
};

AFX afxError        AfxAcquireBuffers(afxDrawContext dctx, afxNat cnt, afxBufferSpecification const spec[], afxBuffer buffers[]);

AFX afxDrawContext  AfxGetBufferContext(afxBuffer buf);

AFX afxNat          AfxGetBufferCapacity(afxBuffer buf);

AFX afxBufferUsage  AfxGetBufferUsage(afxBuffer buf);
AFX afxBufferUsage  AfxTestBufferUsage(afxBuffer buf, afxBufferUsage usage);

AFX afxBufferAccess AfxGetBufferAccess(afxBuffer buf);
AFX afxBufferAccess AfxTestBufferAccess(afxBuffer buf, afxBufferAccess access);

AFX void*           AfxMapBufferRange(afxBuffer buf, afxNat offset, afxNat range, afxFlags flags);
AFX void            AfxUnmapBufferRange(afxBuffer buf);

AFX afxError        AfxDumpBuffer(afxBuffer buf, afxNat offset, afxNat range, void *dst);
AFX afxError        AfxDumpBuffer2(afxBuffer buf, afxNat offset, afxNat stride, afxNat cnt, void *dst, afxNat dstStride);

AFX afxError        AfxUpdateBuffer(afxBuffer buf, afxNat offset, afxNat range, void const *src);
AFX afxError        AfxUpdateBuffer2(afxBuffer buf, afxNat offset, afxNat range, afxNat stride, void const *src, afxNat srcStride);

AFX_DEFINE_STRUCT(afxBufferRegion)
{
    afxNat base;
    afxNat range;
    afxNat stride;
    afxNat offset;
    afxNat unitSiz;
};

AFX afxError        AfxUpdateBufferRegion(afxBuffer buf, afxBufferRegion const* rgn, void const* src, afxNat srcStride);


  //////////////////////////////////////////////////////////////////////////////
 //// COMMANDS                                                             ////
//////////////////////////////////////////////////////////////////////////////

AFX afxCmdId                AfxCmdCopyBuffer
/// Copy data between afxBuffer regions.
(
    afxDrawScript           dscr,
    afxBuffer               src, /// the source buffer.
    afxBuffer               dst, /// the destination buffer.
    afxNat                  opCnt, /// the number of regions to copy.
    afxBufferCopyOp const   ops[] /// an array of structures specifying the regions to copy.
);

AFX afxCmdId                AfxCmdCopyBufferRegion
/// Copy data between afxBuffer regions.
(
    afxDrawScript           dscr,
    afxBuffer               src, /// the source buffer.
    afxNat                  srcOffset, /// the starting offset in bytes from the start of srcBuffer.
    afxBuffer               dst, /// the destination buffer.
    afxNat                  dstOffset, /// the starting offset in bytes from the start of dstBuffer.
    afxNat                  range /// the number of bytes to copy.
);

AFX afxCmdId                AfxCmdFillBuffer
/// Fill a region of a afxBuffer with a fixed value.
(
    afxDrawScript           dscr,
    afxBuffer               buf, /// the buffer to be filled.
    afxNat                  offset, /// the byte offset into the buffer at which to start filling, and must be a multiple of 4.
    afxNat                  range, /// the number of bytes to fill, and must be either a multiple of 4, or 0 to fill the range from offset to the end of the buffer. If 0 is used and the remaining size of the buffer is not a multiple of 4, then the nearest smaller multiple is used.
    afxNat                  value /// the 4-byte word written repeatedly to the buffer to fill size bytes of data. The data word is written to memory according to the host endianness.
);

AFX afxCmdId                AfxCmdClearBuffer
/// Fill a region of a afxBuffer with zeroes.
(
    afxDrawScript           dscr,
    afxBuffer               buf, /// the buffer to be zeroed.
    afxNat                  offset, /// the byte offset into the buffer at which to start filling, and must be a multiple of 4.
    afxNat                  range /// the number of bytes to zero, and must be either a multiple of 4, or 0 to zero the range from offset to the end of the buffer. If 0 is used and the remaining size of the buffer is not a multiple of 4, then the nearest smaller multiple is used.    
);

AFX afxCmdId                AfxCmdUpdateBuffer
/// Update a afxBuffer's contents from host memory.
(
    afxDrawScript           dscr,
    afxBuffer               buf, /// the buffer to be updated.
    afxNat                  offset, /// the byte offset into the buffer to start updating, and must be a multiple of 4.
    afxNat                  range, /// the number of bytes to update, and must be a multiple of 4.
    void const*             src /// the source data for the buffer update, and must be at least @range bytes in size.
);

AFX afxCmdId                AfxCmdDumpBuffer
/// Dump a afxBuffer's contents to host memory.
(
    afxDrawScript           dscr,
    afxBuffer               buf, /// the buffer to be dumped.
    afxNat                  offset, /// the byte offset into the buffer to start dumping, and must be a multiple of 4.
    afxNat                  range, /// the number of bytes to dump, and must be a multiple of 4.
    void*                   dst /// the destination place for the buffer dump, and must be at least @range bytes in size.
);

#endif//AFX_BUFFER_H
