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

#ifndef AVX_BUFFER_OPS_H
#define AVX_BUFFER_OPS_H

#include "qwadro/inc/draw/io/afxBuffer.h"

  //////////////////////////////////////////////////////////////////////////////
 //// COMMANDS                                                             ////
//////////////////////////////////////////////////////////////////////////////

AVX afxCmdId                AvxCmdCopyBuffer
/// Copy data between afxBuffer regions.
(
    avxCmdb                 cmdb,
    afxBuffer               src, /// the source buffer.
    afxBuffer               dst, /// the destination buffer.
    afxUnit                  opCnt, /// the number of regions to copy.
    afxBufferCopy const     ops[] /// an array of structures specifying the regions to copy.
);

AVX afxCmdId                AvxCmdFillBuffer
/// Fill a region of a afxBuffer with a fixed value.
(
    avxCmdb                 cmdb,
    afxBuffer               buf, /// the buffer to be filled.
    afxUnit                  offset, /// the byte offset into the buffer at which to start filling, and must be a multiple of 4.
    afxUnit                  range, /// the number of bytes to fill, and must be either a multiple of 4, or 0 to fill the range from offset to the end of the buffer. If 0 is used and the remaining size of the buffer is not a multiple of 4, then the nearest smaller multiple is used.
    afxUnit                  value /// the 4-byte word written repeatedly to the buffer to fill size bytes of data. The data word is written to memory according to the host endianness.
);

AVX afxCmdId                AvxCmdClearBuffer
/// Fill a region of a afxBuffer with zeroes.
(
    avxCmdb                 cmdb,
    afxBuffer               buf, /// the buffer to be zeroed.
    afxUnit                  offset, /// the byte offset into the buffer at which to start filling, and must be a multiple of 4.
    afxUnit                  range /// the number of bytes to zero, and must be either a multiple of 4, or 0 to zero the range from offset to the end of the buffer. If 0 is used and the remaining size of the buffer is not a multiple of 4, then the nearest smaller multiple is used.    
);

AVX afxCmdId                AvxCmdUpdateBuffer
/// Update a afxBuffer's contents from host memory.
(
    avxCmdb                 cmdb,
    afxBuffer               buf, /// the buffer to be updated.
    afxUnit                  offset, /// the byte offset into the buffer to start updating, and must be a multiple of 4.
    afxUnit                  range, /// the number of bytes to update, and must be a multiple of 4.
    void const*             src /// the source data for the buffer update, and must be at least @range bytes in size.
);

AVX afxCmdId                AvxCmdDumpBuffer
/// Dump a afxBuffer's contents to host memory.
(
    avxCmdb                 cmdb,
    afxBuffer               buf, /// the buffer to be dumped.
    afxUnit                  offset, /// the byte offset into the buffer to start dumping, and must be a multiple of 4.
    afxUnit                  range, /// the number of bytes to dump, and must be a multiple of 4.
    void*                   dst /// the destination place for the buffer dump, and must be at least @range bytes in size.
);

#endif//AVX_BUFFER_OPS_H
