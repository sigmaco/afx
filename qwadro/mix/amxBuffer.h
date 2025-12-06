/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *            Q W A D R O   M U L T I M E D I A   I N F R A S T R U C T U R E
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This software is part of Advanced Multimedia Extensions & Experiments.

#ifndef AMX_BUFFER_H
#define AMX_BUFFER_H

#include "qwadro/mix/amxFormat.h"

#define AMX_BUFFER_ALIGNMENT 16
#define AMX_ALIGN_BUFFERED(siz_) AFX_ALIGN_SIZE((siz_), AMX_BUFFER_ALIGNMENT)

typedef enum amxBufferUsage
{
    // Can be used as the "source" of a copy operation.
    amxBufferUsage_UPLOAD   = AFX_BITMASK(0),
    // Can be used as the "destination" of a copy or write operation.
    amxBufferUsage_DOWNLOAD = AFX_BITMASK(1),

    amxBufferUsage_SOURCE   = AFX_BITMASK(2),
    // The buffer can be bound for use as a sampled audio.
    amxBufferUsage_SAMPLE   = AFX_BITMASK(3),

    amxBufferUsage_MIX      = AFX_BITMASK(4),
    // The buffer can be bound for use as a storage buffer.
    amxBufferUsage_SINK     = AFX_BITMASK(5),

    amxBufferUsage_ADEC     = AFX_BITMASK(6),
    amxBufferUsage_AENC     = AFX_BITMASK(7),
    amxBufferUsage_VDEC     = AFX_BITMASK(8),
    amxBufferUsage_VENC     = AFX_BITMASK(9),

    // Can be used as to store indirect command arguments; aka tracker buffer.
    amxBufferUsage_INDIRECT = AFX_BITMASK(10),
} amxBufferUsage;

typedef enum amxBufferFlag
{
    // Mapped range is readable by CPU.
    amxBufferFlag_R         = AFX_BITMASK(0),
    // Mapped range is writeable by CPU.
    amxBufferFlag_W         = AFX_BITMASK(1),
    // Host-visible, mappable storage.
    amxBufferFlag_M         = (amxBufferFlag_R | amxBufferFlag_W),
    // Executable. Mapped range will be executable by GPU while persistently being read and/or write by CPU.
    amxBufferFlag_X         = AFX_BITMASK(2),
    // Coherent. Data written in mapped range will be immediately visible by GPU and CPU.
    amxBufferFlag_C         = AFX_BITMASK(3),

    // And, to make things compact, there are here convenient combinations frequently used. Thank me later.
    amxBufferFlag_RW = (amxBufferFlag_R | amxBufferFlag_W),
    amxBufferFlag_RX = (amxBufferFlag_R | amxBufferFlag_X),
    amxBufferFlag_RXC = (amxBufferFlag_RX | amxBufferFlag_C),
    amxBufferFlag_WX = (amxBufferFlag_W | amxBufferFlag_X),
    amxBufferFlag_WXC = (amxBufferFlag_WX | amxBufferFlag_C),
    amxBufferFlag_RWX = (amxBufferFlag_RW | amxBufferFlag_X),
    amxBufferFlag_RWXC = (amxBufferFlag_RWX | amxBufferFlag_C),
    amxBufferFlag_ACCESS = (amxBufferFlag_RWX | amxBufferFlag_C),

    // Sparse. The buffer will be backed using sparse memory, and can NOT be mapped.
    amxBufferFlag_S         = AFX_BITMASK(4),
    // Protected. The buffer will be backed using protected memory.
    amxBufferFlag_P         = AFX_BITMASK(5),
    
    // Hosted. Determine whether storage is local to the client.
    amxBufferFlag_H         = AFX_BITMASK(6),
    // Foreign. The buffer will be backed using external memory.
    amxBufferFlag_F         = AFX_BITMASK(7),

} amxBufferFlags;

AFX_DEFINE_STRUCT(amxBufferedCopy)
// Structure specifying a amxBuffer-backed copy.
{
    amxBuffer       dst;
    // is the starting offset in bytes from the start of dstBuffer.
    afxSize         dstOffset;
    // is the number of bytes to copy.
    afxUnit         range;
    // is the starting offset in bytes from the start of srcBuffer.
    afxSize         srcOffset;
    amxBuffer       src;
};

#define AMX_BUFFERED_COPY(uSrcOffset, uDstOffset, uRange) \
    (amxBufferedCopy){ .srcOffset = (uSrcOffset), .dstOffset = (uDstOffset), .range = (uRange) }

AFX_DEFINE_STRUCT(amxBufferCopy)
// Structure specifying a amxBuffer-backed copy.
{
    // is the starting offset in bytes from the start of dstBuffer.
    afxSize         dstOffset;
    // is the starting offset in bytes from the start of srcBuffer.
    afxSize         srcOffset;
    // is the number of bytes to copy.
    afxUnit         range;
};

#define AMX_BUFFER_COPY(uSrcOffset, uDstOffset, uRange) \
    (amxBufferCopy){ .srcOffset = (uSrcOffset), .dstOffset = (uDstOffset), .range = (uRange) }

AFX_DEFINE_STRUCT(amxBufferIo)
// Structure specifying a amxBuffer-backed transfer.
{
    // is the starting offset in bytes from the start of dstBuffer.
    afxSize         dstOffset;
    // [!] only if supported by device, else case it must be 1.
    afxUnit         dstStride;
    // is the starting offset in bytes from the start of srcBuffer.
    afxSize         srcOffset;
    // [!] only if supported by device, else case it must be 1.
    afxUnit         srcStride;
    // is the number of rows to stream in/out.
    afxUnit         rowCnt;
};

#define AMX_BUFFER_IO(uSrcOffset, uDstOffset, uSrcStride, uDstStride, uRowCnt) \
    (amxBufferIo) { .srcOffset = (uSrcOffset), .dstOffset = (uDstOffset), .srcStride = (uSrcStride), \
                    .dstStride = (uDstStride), .rowCnt = (uRowCnt) }

#if 0
AFX_DEFINE_STRUCT(amxBufferIo)
{
    afxUnit         c, r, g;
    afxUnit         x, y, z;
    afxUnit         w, h, d;
    afxSize         offset;
    afxUnit         rowStride;
    afxUnit         rowCnt;
};
#endif

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

#define AMX_BUFFERED_STREAM(hBuf, uOffset, uRange, uStride) \
    (amxBufferedStream){ .buf = (hBuf), .offset = (uOffset), .range = (uRange), .stride = (uStride) }

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

#define AMX_BUFFERED_MAP(hBuf, uOffset, uRange, uFlags) \
    (amxBufferedMap){ .buf = (hBuf), .offset = (uOffset), .range = (uRange), .flags = (uFlags) }

AFX_DEFINE_STRUCT(amxMetabufferInfo)
{
    // A bitmask specifying additional parameters of the buffer.
    amxBufferFlags  flags;
    // A amxBuffer on which the view will be created.
    amxBuffer       base;
    // An offset in bytes from the base address of the buffer.
    afxSize         from;
    // The size in bytes of the buffer to be created.
    afxSize         range;
    // A amxFormat describing the format of the data elements in the buffer.
    amxFormat       fmt;
    // A user-defined data.
    void*           udd;
    afxString       tag;
};

AFX_DEFINE_STRUCT(amxBufferInfo)
{
    // The size in bytes of the buffer to be created.
    afxSize         size;
    // A bitmask specifying allowed usages of the buffer.
    amxBufferUsage  usage;
    // A bitmask specifying additional parameters of the buffer.
    amxBufferFlags  flags;
    // A bitmask of device bridges that will access this buffer.
    afxMask         sharingMask;

    // A amxFormat describing the format of the data elements in the buffer.
    amxFormat       fmt;
    // An user-defined data.
    void*           udd;
    afxString       tag;

    // Possibly initial data, to avoid boilerplate with unconvenient mappings.
    void*           data;
    afxUnit         dataSiz;
    // Should this buffer be mapped at creation?
    afxBool         mapped;
};

AFX_DEFINE_STRUCT(amxExobufferInfo)
{
    amxBufferInfo   info;
    // Possibly initial data, to avoid boilerplate with unconvenient mappings.
    void*           data;
    afxUnit         dataSiz;
    // Should this buffer be mapped at creation?
    afxBool         mapped;
};

AMX afxError        AmxAcquireBuffers
(
    afxMixSystem msys, 
    afxUnit cnt, 
    amxBufferInfo const infos[], 
    amxBuffer buffers[]
);

AMX afxError        AmxLoadBuffers
(
    afxMixSystem msys, 
    afxUnit cnt, 
    afxUri const uris[], 
    amxBuffer buffers[]
);

/*
    The AmxMapBuffers() function maps buffer regions to make them accessible for read/write operations in a graphics or compute system.
    It allows for mapping multiple buffer regions in parallel, managing synchronization, validation, and performing the actual mapping of memory.

    Each buffer mapping is validated, and if it's already mapped, the function ensures the new mapping is within the current mapped region.
    If the buffer has been allocated in host memory (buf->data), the function directly maps the buffer region.
    If the buffer is a MPU-side or managed buffer, the function defers the actual mapping to a later stage using a mix system operation.

    The function uses atomic operations to manage the pending mapping state of each buffer.
    The function waits for any ongoing mapping operations to complete before proceeding.

    The function first checks for a ICD callback function associated with the mix system.
    If the callback is available, it uses it to perform the mapping; otherwise, it falls back to using mix queues and bridges.
    If no callback is available, the function uses a mix queue and mix bridge to handle the buffer remapping.

    The function waits for the mix queue to complete the remapping operation before proceeding.
    After the mapping operations are completed, the function ensures that all buffers have their pendingRemap counters decremented.

    It also performs additional validation (if enabled) to ensure the buffer's mapped state is consistent.
*/

AMX afxError AmxMapBuffers
(
    // The mix system providing the buffers.
    afxMixSystem msys,
    // The number of mapping operations.
    afxUnit cnt,
    // An array of amxBufferedMap describing the mapping operations.
    amxBufferedMap maps[],
    // An array of placeholders for pointers to each mapping operation.
    // After mapping, each entry will point to a region in host memory corresponding to the mapped buffer segment.
    void** placeholders[]
);

/*
    The AmxUnmapBuffers() function unmaps buffers previously mapped in the graphics or compute system. It handles synchronization,
    error checking, and supports both host-side buffers and buffers that are managed by the mix system.

    The function iterates through the list of buffers and waits for any ongoing mapping operations to complete before unmapping the buffers.
    The function uses atomic operations to manage the pending remap state of each buffer and ensure synchronization during unmapping.

    If the mix system provides a ICD callback function, it is used to unmap the buffers.
    If no ICD callback is available, the function uses a mix queue to perform the unmap operation.

    After unmapping the buffers, the function performs final checks (if validation is enabled) and clears the pendingRemap state for each buffer.
*/

AMX afxError AmxUnmapBuffers
(
    // The mix system providing the buffers.
    afxMixSystem msys,
    // The number of unmapping operations.
    afxUnit cnt,
    // An array of amxBufferedMap describing the unmapping operations.
    amxBufferedMap maps[]
);

/*
    The AmxCohereMappedBuffers() function ensures that any pending operations involving mapped buffers are completed, typically flushing any
    modifications made to these buffers and synchronizing them with the MPU or mix system. The function validates the input, checks
    buffer states, and either uses a ICD callback or a mix queue to execute the flush operation.

    The function validates the provided buffers and ensures they are part of the correct mix system.

    If no ICD callback is available for flushing the maps, it uses a mix queue and bridge to synchronize the flush operation.

    Error handling and synchronization are key components of this function, ensuring that buffer operations are completed before continuing with other tasks.
*/

AMX afxError AmxCohereMappedBuffers
(
    // The mix system providing the buffers.
    afxMixSystem   msys,
    // A boolean specifying that any host modification must be discarded.
    afxBool         discard,
    // The number of buffered maps.
    afxUnit         cnt,
    // An array of buffered map operations.
    amxBufferedMap const maps[]
);

AMX afxError AmxCopyBuffers
(
    afxMixSystem msys, 
    afxMask exuMask, 
    afxUnit cnt, 
    amxBufferedCopy const ops[]
);

////////////////////////////////////////////////////////////////////////////////

AMX afxMixSystem    AmxGetBufferHost(amxBuffer buf);

AMX void*           AmxGetBufferUdd(amxBuffer buf);

AMX afxSize         AmxGetBufferCapacity(amxBuffer buf, afxSize from);

AMX amxBufferUsage  AmxGetBufferUsage(amxBuffer buf, amxBufferUsage mask);

AMX amxBufferFlags  AmxGetBufferFlags(amxBuffer buf, amxBufferFlags mask);

// Query an address of a buffer used to access the memory bound to this buffer on the device.

AMX afxSize AmxGetBufferAddress
(
    // The buffer to be queried.
    amxBuffer buf,
    // The starting point in the buffer from which address is being queried.
    afxSize from
);

/*
    The AmxGetBufferMap() function provides direct access to a mapped region of a buffer.
    It's used after a buffer has already been mapped using AmxMapBuffers.
    Returns a pointer to a sub-region of a previously mapped buffer.
*/

AMX void*           AmxGetBufferMap
(
    // An handle to a previously acquired and mapped buffer.
    amxBuffer buf,
    // Byte offset into the buffer's memory where your region starts.
    afxSize offset,
    // Size in bytes of the memory region which the access is wanted.
    afxUnit range
);

/*
    The AmxMapBuffer() method maps a buffer to memory, making it accessible for CPU operations. It validates inputs (buffer validity,
    alignment, range), prepares the mapping structure, and calls the system's mapping function to map the buffer. The function also
    includes error handling and validation to ensure that the mapping process is carried out safely and efficiently.

    This function is just a convenient way of calling AmxMapBuffers(), which is called to perform the actual mapping of the buffer,
    for a single buffer. Otherwise, AmxMapBuffers() should be used to avoid contention.
*/

AMX afxError        AmxMapBuffer
(
    // The buffer that needs to be mapped.
    amxBuffer       buf,
    // The starting byte offset where the buffer mapping should begin.
    afxSize         offset,
    // The size (in bytes or units) of the portion of the buffer to map, starting from the @offset.
    afxUnit         range,
    // Flags that define the special behaviors for the mapping.
    afxFlags        flags,
    // A pointer to a pointer (void**), which will be used to store the address of the mapped memory.
    // After the mapping, this pointer will point to the memory region that has been mapped.
    void**          placeholder
);

/*
    The AmxUnmapBuffer() method unmaps a buffer from memory. It ensures that any changes to the buffer are synchronized before releasing
    the memory access. The function validates the buffer and mixing system, calls the unmapping process, and handles errors if the unmap
    operation fails. The wait flag is not used in this function's implementation, but it could be useful for managing synchronization in
    a more complex implementation.

    This function is just a convenient way of calling AmxUnmapBuffers(), which is called to perform the actual unmapping of the buffer,
    for a single buffer. Otherwise, AmxUnmapBuffers() should be used to avoid contention.
*/

AMX afxError        AmxUnmapBuffer
(
    // The buffer to unmap.
    amxBuffer       buf,
    // A flag indicating whether the operation should wait for synchronization.
    afxBool         wait
);

/*
    The AmxCohereMappedBuffer() method performs any coherence in the mapped range. When @sync is FALSE,
    it ensures any changes to a buffer's data are committed or flushed to memory.
    When @sync is TRUE, it synchronizes a buffer's state to ensure that it is ready for further operations (such as reading or writing).
    And if any errors occur during this process, they are raised and returned.

    This function is just a convenient way of calling AmxCohereMappedBuffers(), which carries out the synchronization, for a single buffer.
    Otherwise, AmxCohereMappedBuffers() should be used to avoid contention.
*/

AMX afxError        AmxCohereMappedBuffer
(
    // The buffer that needs to be flushed.
    amxBuffer       buf,
    // The starting point (in bytes) of the portion of the buffer that should be flushed.
    afxSize         offset,
    // The size (in bytes or units) of the portion of the buffer to flush, starting from the @offset.
    afxUnit         range,
    // The optional flags.
    afxFlags        flags,
    // A boolean specifying if any host modification should be discarded.
    afxBool         discard
);

/*
    The AmxUploadBuffer() method uploads data to a buffer. It validates the operations, prepares a transfer structure, and then
    performs the actual data upload. The function handles synchronization and ensures that errors are caught and thrown appropriately.
    The use of execution units allows for parallel or distributed uploads, such as using a MPU to handle the operations.
*/

AMX afxError        AmxUploadBuffer
(
    // The destination buffer to which data will be uploaded.
    amxBuffer       buf,
    // The number of operations to be performed on the buffer.
    afxUnit         opCnt,
    // An array of operations (@opCnt number of operations) to be performed on the buffer.
    // Each operation specifies where the data will be uploaded within the buffer and how the data should be handled.
    amxBufferIo const ops[],
    // The source stream from which the data will be transferred into the buffer.
    afxStream       in,
    // The execution unit index. This typically refers to the specific MPU or compute unit that will handle the operation.
    afxMask         exuMask
);

/*
    The AmxDownloadBuffer() method downloads data from a buffer. It validates the operations, prepares a transfer structure, and then
    performs the actual data transfer. It also handles synchronization and error handling, ensuring that the data is downloaded to the
    specified output stream. The function is optimized for use in parallel computing systems, with the ability to specify the execution
    unit responsible for the operation.
*/

AMX afxError        AmxDownloadBuffer
(
    // The source buffer from which data will be downloaded.
    amxBuffer       buf,
    // The number of operations to be performed on the buffer.
    afxUnit         opCnt,
    // An array of operations to be performed on the buffer. 
    // Each operation specifies how data should be extracted from the buffer.
    amxBufferIo const ops[],
    // The destination stream where the data from the buffer will be transferred.
    afxStream       out,
    // The execution unit index. This refers to the specific MPU or compute unit responsible for the operation.
    afxMask         exuMask
);

/*
    The AmxDumpBuffer() method dumps data from a buffer to a specified destination. It validates the operations, prepares a transfer
    structure, and then performs the actual data transfer. It also handles synchronization and error handling, ensuring that the data
    is dumped to the specified destination. The function is optimized for use in parallel computing systems, with the ability to specify
    the execution unit responsible for the operation.
*/

AMX afxError        AmxDumpBuffer
(
    // The source buffer from which data will be dumped.
    amxBuffer       buf,
    // The number of operations to be performed on the buffer.
    afxUnit         opCnt,
    // An array of operations to be performed on the buffer. 
    // Each operation specifies how data should be dumped from the buffer.
    amxBufferIo const ops[],
    // A pointer to the destination where the buffer data will be dumped.
    void*           dst,
    // The execution unit index (e.g., MPU or compute unit) responsible for the operation.
    afxMask         exuMask
);

AMX afxError        AmxUpdateBuffer
(
    // The target buffer that will be updated with new data.
    amxBuffer       buf,
    // The number of operations to be performed on the buffer.
    afxUnit         opCnt,
    // An array of operations to be performed on the buffer. 
    // Each operation specifies how the buffer should be updated.
    amxBufferIo const ops[],
    // A pointer to the source data from which the buffer will be updated.
    void const*     src,
    // The execution unit index (e.g., MPU or compute unit) that will perform the transfer.
    afxMask         exuMask
);

#endif//AMX_BUFFER_H
