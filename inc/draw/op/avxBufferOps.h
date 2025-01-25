/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *        Q W A D R O   V I D E O   G R A P H I C S   I N F R A S T R U C T U R E
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#ifndef AVX_BUFFER_OPS_H
#define AVX_BUFFER_OPS_H

#include "qwadro/inc/draw/io/afxBuffer.h"

#define AVX_BUF_UPDATE_CAPACITY 65536

  //////////////////////////////////////////////////////////////////////////////
 //// COMMANDS                                                             ////
//////////////////////////////////////////////////////////////////////////////

AVX afxCmdId                AvxCmdClearBuffer
/// Fill a region of a afxBuffer with zeroes.
(
    afxDrawContext          dctx,
    afxBuffer               buf, /// the buffer to be zeroed.
    afxUnit                 offset, /// the byte offset into the buffer at which to start filling, and must be a multiple of 4.
    afxUnit                 range /// the number of bytes to zero, and must be either a multiple of 4, or 0 to zero the range from offset to the end of the buffer. If 0 is used and the remaining size of the buffer is not a multiple of 4, then the nearest smaller multiple is used.    
);

AVX afxCmdId                AvxCmdFillBuffer
/// Fill a region of a afxBuffer with a fixed value.
(
    afxDrawContext          dctx,
    afxBuffer               buf, /// the buffer to be filled.
    afxUnit                 offset, /// the byte offset into the buffer at which to start filling, and must be a multiple of 4.
    afxUnit                 range, /// the number of bytes to fill, and must be either a multiple of 4, or 0 to fill the range from offset to the end of the buffer. If 0 is used and the remaining size of the buffer is not a multiple of 4, then the nearest smaller multiple is used.
    afxUnit                 value /// the 4-byte word written repeatedly to the buffer to fill size bytes of data. The data word is written to memory according to the host endianness.
);

AVX afxCmdId                AvxCmdUpdateBuffer
/// Update a afxBuffer's contents from host memory.
(
    afxDrawContext          dctx,
    afxBuffer               buf, /// the buffer to be updated.
    afxUnit                 offset, /// the byte offset into the buffer to start updating, and must be a multiple of 4.
    afxUnit                 range, /// the number of bytes to update, and must be a multiple of 4.
    void const*             src /// the source data for the buffer update, and must be at least @range bytes in size.
);

AVX afxCmdId                AvxCmdCopyBuffer
/// Copy data between afxBuffer regions.
(
    afxDrawContext          dctx,
    afxBuffer               src, /// the source buffer.
    afxBuffer               dst, /// the destination buffer.
    afxUnit                 opCnt, /// the number of regions to copy.
    afxBufferCopy const     ops[] /// an array of structures specifying the regions to copy.
);

#endif//AVX_BUFFER_OPS_H
