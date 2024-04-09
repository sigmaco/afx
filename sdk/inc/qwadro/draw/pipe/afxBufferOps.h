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

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#ifndef AFX_BUFFER_OPS_H
#define AFX_BUFFER_OPS_H

#include "qwadro/draw/io/afxBuffer.h"

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

  //////////////////////////////////////////////////////////////////////////////
 //// COMMANDS                                                             ////
//////////////////////////////////////////////////////////////////////////////

AVX afxCmdId                AfxCmdCopyBuffer
/// Copy data between afxBuffer regions.
(
    afxDrawStream           diob,
    afxBuffer               src, /// the source buffer.
    afxBuffer               dst, /// the destination buffer.
    afxNat                  opCnt, /// the number of regions to copy.
    afxBufferCopyOp const   ops[] /// an array of structures specifying the regions to copy.
);

AVX afxCmdId                AfxCmdCopyBufferRegion
/// Copy data between afxBuffer regions.
(
    afxDrawStream           diob,
    afxBuffer               src, /// the source buffer.
    afxNat                  srcOffset, /// the starting offset in bytes from the start of srcBuffer.
    afxBuffer               dst, /// the destination buffer.
    afxNat                  dstOffset, /// the starting offset in bytes from the start of dstBuffer.
    afxNat                  range /// the number of bytes to copy.
);

AVX afxCmdId                AfxCmdFillBuffer
/// Fill a region of a afxBuffer with a fixed value.
(
    afxDrawStream           diob,
    afxBuffer               buf, /// the buffer to be filled.
    afxNat                  offset, /// the byte offset into the buffer at which to start filling, and must be a multiple of 4.
    afxNat                  range, /// the number of bytes to fill, and must be either a multiple of 4, or 0 to fill the range from offset to the end of the buffer. If 0 is used and the remaining size of the buffer is not a multiple of 4, then the nearest smaller multiple is used.
    afxNat                  value /// the 4-byte word written repeatedly to the buffer to fill size bytes of data. The data word is written to memory according to the host endianness.
);

AVX afxCmdId                AfxCmdClearBuffer
/// Fill a region of a afxBuffer with zeroes.
(
    afxDrawStream           diob,
    afxBuffer               buf, /// the buffer to be zeroed.
    afxNat                  offset, /// the byte offset into the buffer at which to start filling, and must be a multiple of 4.
    afxNat                  range /// the number of bytes to zero, and must be either a multiple of 4, or 0 to zero the range from offset to the end of the buffer. If 0 is used and the remaining size of the buffer is not a multiple of 4, then the nearest smaller multiple is used.    
);

AVX afxCmdId                AfxCmdUpdateBuffer
/// Update a afxBuffer's contents from host memory.
(
    afxDrawStream           diob,
    afxBuffer               buf, /// the buffer to be updated.
    afxNat                  offset, /// the byte offset into the buffer to start updating, and must be a multiple of 4.
    afxNat                  range, /// the number of bytes to update, and must be a multiple of 4.
    void const*             src /// the source data for the buffer update, and must be at least @range bytes in size.
);

AVX afxCmdId                AfxCmdDumpBuffer
/// Dump a afxBuffer's contents to host memory.
(
    afxDrawStream           diob,
    afxBuffer               buf, /// the buffer to be dumped.
    afxNat                  offset, /// the byte offset into the buffer to start dumping, and must be a multiple of 4.
    afxNat                  range, /// the number of bytes to dump, and must be a multiple of 4.
    void*                   dst /// the destination place for the buffer dump, and must be at least @range bytes in size.
);

#endif//AFX_BUFFER_OPS_H
