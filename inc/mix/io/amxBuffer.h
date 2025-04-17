/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *       Q W A D R O   S O U N D   S Y N T H E S I S   I N F R A S T R U C T U R E
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

#ifndef AMX_BUFFER_H
#define AMX_BUFFER_H

#include "qwadro/inc/mix/afxMixDefs.h"

#define AMX_BUFFER_ALIGNMENT 16
#define AMX_BUF_ALIGN(siz_) AFX_ALIGNED_SIZE((siz_), AMX_BUFFER_ALIGNMENT)

typedef enum amxBufferUsage
{
    amxBufferUsage_UPLOAD   = AFX_BIT(0), // Can be used as the "source" of a copy operation.
    amxBufferUsage_DOWNLOAD = AFX_BIT(1), // Can be used as the "destination" of a copy or write operation.
    amxBufferUsage_SOURCE   = AFX_BIT(2),
    amxBufferUsage_SAMPLE   = AFX_BIT(3),
    amxBufferUsage_MIX      = AFX_BIT(4),
    amxBufferUsage_SINK     = AFX_BIT(5),
    amxBufferUsage_AUDIO_DEC = AFX_BIT(6),
    amxBufferUsage_AUDIO_ENC = AFX_BIT(7),
    amxBufferUsage_VIDEO_DEC = AFX_BIT(8),
    amxBufferUsage_VIDEO_ENC = AFX_BIT(9),
} amxBufferUsage;

typedef enum amxBufferFlag
{
    // Mapped range is readable by CPU.
    amxBufferFlag_R         = AFX_BIT(0),

    // Mapped range is writeable by CPU.
    amxBufferFlag_W         = AFX_BIT(1),

    // Executable. Mapped range will be executable by GPU while persistently being read and/or write by CPU.
    amxBufferFlag_X         = AFX_BIT(2),

    // Coherent. Data written in mapped range will be immediately visible by GPU and CPU.
    amxBufferFlag_C         = AFX_BIT(3),

    // Hosted. Determine whether storage is local to the client.
    amxBufferFlag_H         = AFX_BIT(4),

    // And, to make things compact, there are here convenient combinations frequently used. Thank me later.
    amxBufferFlag_RW        = (amxBufferFlag_R | amxBufferFlag_W),
    amxBufferFlag_RX        = (amxBufferFlag_R | amxBufferFlag_X),
    amxBufferFlag_RXC       = (amxBufferFlag_RX | amxBufferFlag_C),
    amxBufferFlag_WX        = (amxBufferFlag_W | amxBufferFlag_X),
    amxBufferFlag_WXC       = (amxBufferFlag_WX | amxBufferFlag_C),
    amxBufferFlag_RWX       = (amxBufferFlag_RW | amxBufferFlag_X),
    amxBufferFlag_RWXC      = (amxBufferFlag_RWX | amxBufferFlag_C),

    amxBufferFlag_ACCESS    = (amxBufferFlag_RWX | amxBufferFlag_C)
} amxBufferFlags;

AFX_DEFINE_STRUCT(amxBufferInfo)
{
    afxUnit         size;
    amxBufferUsage  usage;
    amxBufferFlags  flags;
    amxFormat       fmt;
    void*           udd;
};

AFX_DEFINE_STRUCT(amxBufferCopy)
{
    afxUnit         range; // is the number of bytes to copy.
    afxSize         from; // is the starting offset in bytes from the start of srcBuffer.
    afxSize         to; // is the starting offset in bytes from the start of dstBuffer.
};

AFX_DEFINE_STRUCT(amxBufferIo)
{
    afxSize         srcOffset; // is the starting offset in bytes from the start of srcBuffer.
    afxSize         dstOffset; // is the starting offset in bytes from the start of dstBuffer.
    afxUnit         srcStride; // [!] only if supported by device, else case it must be 1.
    afxUnit         dstStride; // [!] only if supported by device, else case it must be 1.
    afxUnit         rowCnt; // is the number of rows to stream in/out.
};

AFX_DEFINE_STRUCT(amxBufferedStream)
// Structured specifying a amxBuffer-backed stream.
{
    // A buffer handle.
    amxBuffer       buf;
    // The start of buffer.
    afxSize         offset;
    // The size in bytes of data from buffer.
    afxUnit         range;
    // The byte stride between consecutive elements within the buffer.
    afxUnit         stride;
};

AFX_DEFINE_STRUCT(amxBufferedMap)
// Structured specifying a amxBuffer-backed memory map.
{
    // A buffer handle.
    amxBuffer       buf;
    // The start of buffer.
    afxSize         offset;
    // The size in bytes of data from buffer.
    afxUnit         range;
    // A bitmask of flags specifying additional parameters of the memory map operation.
    afxFlags        flags;
};

AMX afxMixSystem    AmxGetBufferContext(amxBuffer buf);

AMX void*           AmxGetBufferUdd(amxBuffer buf);

AMX afxUnit         AmxGetBufferCapacity(amxBuffer buf, afxUnit from);

AMX amxBufferUsage  AmxGetBufferUsage(amxBuffer buf, amxBufferUsage usage);

AMX amxBufferFlags  AmxGetBufferAccess(amxBuffer buf, amxBufferFlags access);

////////////////////////////////////////////////////////////////////////////////

AMX afxError        AmxAcquireBuffers(afxMixSystem msys, afxUnit cnt, amxBufferInfo const infos[], amxBuffer buffers[]);

#endif//AMX_BUFFER_H
