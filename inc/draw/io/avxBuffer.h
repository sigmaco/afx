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

#ifndef AVX_BUFFER_H
#define AVX_BUFFER_H

#include "qwadro/inc/draw/afxDrawDefs.h"
#include "qwadro/inc/draw/io/avxFormat.h"

#define AVX_BUFFER_ALIGNMENT 64
#define AVX_BUF_ALIGN(siz_) AFX_ALIGNED_SIZE((siz_), AVX_BUFFER_ALIGNMENT)

typedef enum avxBufferUsage
{
    avxBufferUsage_UPLOAD = AFX_BIT(0), // Can be used as the "source" of a copy operation.
    avxBufferUsage_DOWNLOAD = AFX_BIT(1), // Can be used as the "destination" of a copy or write operation.
    avxBufferUsage_VERTEX = AFX_BIT(2), // Can be used as a vertex buffer; aka VBO.
    avxBufferUsage_INDEX = AFX_BIT(3), // Can be used as an index buffer; aka IBO.
    avxBufferUsage_UNIFORM = AFX_BIT(4), // Can be used as a uniform buffer; aka UBO.
    avxBufferUsage_STORAGE = AFX_BIT(5), // Can be used as a storage buffer; aka SSBO.
    avxBufferUsage_INDIRECT = AFX_BIT(6), // Can be used as to store indirect command arguments; aka DBO.
    avxBufferUsage_QUERY = AFX_BIT(7), // Can be used to capture query results; aka QBO.    
    avxBufferUsage_FETCH = AFX_BIT(8), // Can be used to fetch tensor (usually texels) data; aka TFBO.
    avxBufferUsage_TENSOR = AFX_BIT(9), // Can be used to read/write tensor data; aka storable TBO.
    avxBufferUsage_FEEDBACK = AFX_BIT(10) // Can be used to store primitive transformation output/feedback.
} avxBufferUsage;

typedef enum avxBufferFlags
{
    // Mapped range is readable by CPU.
    avxBufferFlag_R         = AFX_BIT(0),

    // Mapped range is writeable by CPU.
    avxBufferFlag_W         = AFX_BIT(1),

    // Executable. Mapped range will be executable by GPU while persistently being read and/or write by CPU.
    avxBufferFlag_X         = AFX_BIT(2),

    // Coherent. Data written in mapped range will be immediately visible by GPU and CPU.
    avxBufferFlag_C         = AFX_BIT(3),

    // Hosted. Determine whether storage is local to the client.
    avxBufferFlag_H         = AFX_BIT(4),

    // And, to make things compact, there are here convenient combinations frequently used. Thank me later.
    avxBufferFlag_RW        = (avxBufferFlag_R | avxBufferFlag_W),
    avxBufferFlag_RX        = (avxBufferFlag_R | avxBufferFlag_X),
    avxBufferFlag_RXC       = (avxBufferFlag_RX | avxBufferFlag_C),
    avxBufferFlag_WX        = (avxBufferFlag_W | avxBufferFlag_X),
    avxBufferFlag_WXC       = (avxBufferFlag_WX | avxBufferFlag_C),
    avxBufferFlag_RWX       = (avxBufferFlag_RW | avxBufferFlag_X),
    avxBufferFlag_RWXC      = (avxBufferFlag_RWX | avxBufferFlag_C),

    avxBufferFlag_ACCESS    = (avxBufferFlag_RWX | avxBufferFlag_C)
} avxBufferFlags;

typedef enum avxBufferMapFlag
{
    avxBufferMapFlag_FLUSH  = AFX_BIT(0), // explicitly flushed
    avxBufferMapFlag_UNSYNC = AFX_BIT(1), // unsynchronized with previous operations
} avxBufferMapFlags;

AFX_DEFINE_STRUCT(avxBufferInfo)
{
    // The size in bytes of the buffer to be created.
    afxUnit         cap;
    // A bitmask specifying allowed usages of the buffer.
    avxBufferUsage  usage;
    // A bitmask specifying additional parameters of the buffer.
    avxBufferFlags  flags;

    // A avxBuffer on which the view will be created.
    avxBuffer       base;
    // An offset in bytes from the base address of the buffer.
    afxSize         from;
    // A avxFormat describing the format of the data elements in the buffer.
    avxFormat       fmt; // used for TENSOR buffers.

    // A bitmask of device bridges that will access this buffer.
    afxMask         sharingMask;
    // A user-defined data.
    void*           udd;
    afxString       tag;
};

AFX_DEFINE_STRUCT(avxBufferCopy)
{
    afxSize         srcOffset; // is the starting offset in bytes from the start of srcBuffer.
    afxSize         dstOffset; // is the starting offset in bytes from the start of dstBuffer.
    afxUnit         range; // is the number of bytes to copy.
};

AFX_DEFINE_STRUCT(avxBufferIo)
{
    afxSize         srcOffset; // is the starting offset in bytes from the start of srcBuffer.
    afxSize         dstOffset; // is the starting offset in bytes from the start of dstBuffer.
    afxUnit         srcStride; // [!] only if supported by device, else case it must be 1.
    afxUnit         dstStride; // [!] only if supported by device, else case it must be 1.
    afxUnit         rowCnt; // is the number of rows to stream in/out.
};

AFX_DEFINE_STRUCT(avxBufferedStream)
// Structured specifying a avxBuffer-backed stream.
{
    // A buffer handle.
    avxBuffer       buf;
    // The start of buffer.
    afxSize         offset;
    // The size in bytes of data from buffer.
    afxUnit         range;
    // The byte stride between consecutive elements within the buffer.
    afxUnit         stride;
};

AFX_DEFINE_STRUCT(avxBufferedMap)
// Structured specifying a avxBuffer-backed memory map.
{
    // A buffer handle.
    avxBuffer       buf;
    // The start of buffer.
    afxSize         offset;
    // The size in bytes of data from buffer.
    afxUnit         range;
    // A bitmask of flags specifying additional parameters of the memory map operation.
    afxFlags        flags;
};

AFX_DEFINE_STRUCT(avxBufferRemap)
{
    avxBuffer       buf;
    afxSize         offset;
    afxUnit         range;
    avxBufferMapFlags flags;
    void**          placeholder;
};

AVX afxDrawSystem   AvxGetBufferContext(avxBuffer buf);

AVX void*           AvxGetBufferUdd(avxBuffer buf);

AVX afxUnit         AvxGetBufferCapacity(avxBuffer buf, afxUnit from);

AVX avxBufferUsage  AvxGetBufferUsage(avxBuffer buf);
AVX avxBufferUsage  AvxTestBufferUsage(avxBuffer buf, avxBufferUsage usage);

AVX avxBufferFlags  AvxGetBufferAccess(avxBuffer buf);
AVX avxBufferFlags  AvxTestBufferAccess(avxBuffer buf, avxBufferFlags access);

AVX afxError        AvxSyncBuffer(avxBuffer buf, afxSize offset, afxUnit range);
AVX afxError        AvxFlushBuffer(avxBuffer buf, afxSize offset, afxUnit range);

AVX afxError        AvxMapBuffer(avxBuffer buf, afxSize offset, afxUnit range, afxFlags flags, void** placeholder);
AVX afxError        AvxUnmapBuffer(avxBuffer buf, afxBool wait);
AVX afxBool         AvxIsBufferMapped(avxBuffer buf, afxSize offset, afxUnit range);

AVX afxError        AvxUploadBuffer(avxBuffer buf, afxUnit opCnt, avxBufferIo const ops[], afxUnit exuIdx, afxStream in);
AVX afxError        AvxDownloadBuffer(avxBuffer buf, afxUnit opCnt, avxBufferIo const ops[], afxUnit exuIdx, afxStream out);

AVX afxError        AvxDumpBuffer(avxBuffer buf, afxUnit opCnt, avxBufferIo const ops[], afxUnit exuIdx, void* dst);
AVX afxError        AvxUpdateBuffer(avxBuffer buf, afxUnit opCnt, avxBufferIo const ops[], afxUnit exuIdx, void const* src);

////////////////////////////////////////////////////////////////////////////////

AVX afxError        AvxAcquireBuffers(afxDrawSystem dsys, afxUnit cnt, avxBufferInfo const infos[], avxBuffer buffers[]);

/*
    AvxFlushBufferMaps guarantees that host writes to the memory ranges described by maps are made available to the host memory domain,
    such that they can be made available to the device memory domain via memory domain operations using the WRITE access type.
*/

AVX afxError        AvxFlushBufferMaps(afxDrawSystem dsys, afxUnit cnt, avxBufferedMap const maps[]);

/*
    AvxSyncBufferMaps guarantees that device writes to the memory ranges described by maps,
    which have been made available to the host memory domain using the WRITE and READ access types,
    are made visible to the host. If a range of non-coherent memory is written by the host and then
    invalidated without first being flushed, its contents are undefined.
*/

AVX afxError        AvxSyncBufferMaps(afxDrawSystem dsys, afxUnit cnt, avxBufferedMap const maps[]);

AVX afxError        AvxMapBuffers(afxDrawSystem dsys, afxUnit cnt, avxBufferRemap maps[]);

AVX afxError        AvxUnmapBuffers(afxDrawSystem dsys, afxUnit cnt, avxBuffer buffers[]);

#endif//AVX_BUFFER_H
