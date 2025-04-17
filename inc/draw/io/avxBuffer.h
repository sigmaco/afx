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

  //////////////////////////////////////////////////////////////////////////////
 // QWADRO VIDEO MEMORY BUFFER                                               //
//////////////////////////////////////////////////////////////////////////////

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
    avxBufferUsage_FEEDBACK = AFX_BIT(10), // Can be used to store primitive transformation output/feedback.
    avxBufferUsage_ARGUMENT = AFX_BIT(11) // Can be used as a argumentation buffer; ABO
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
    // A bitmask specifying allowed usages of the buffer.
    avxBufferUsage  usage;
    // A bitmask specifying additional parameters of the buffer.
    avxBufferFlags  flags;

    // A avxBuffer on which the view will be created.
    avxBuffer       base;
    // An offset in bytes from the base address of the buffer.
    afxSize         from;
    // The size in bytes of the buffer to be created.
    afxUnit         size;
    // A avxFormat describing the format of the data elements in the buffer.
    avxFormat       fmt; // used for TENSOR buffers.

    // A bitmask of device bridges that will access this buffer.
    afxMask         sharingMask;
    // A user-defined data.
    void*           udd;
    afxString       tag;

    // Possibly initial data, to avoid boilerplate with unconvenient mappings.
    void*           data;
    afxUnit         dataSiz;
    // Should this buffer be mapped at creation?
    afxBool         mapped;
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
// Structure specifying a avxBuffer-backed stream.
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
// Structure specifying a avxBuffer-backed memory map.
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

/*
    The AvxGetBufferCapacity() function returns the available capacity of a buffer, starting from a specified position. 
    It ensures that the requested capacity doesn't exceed the buffer's total capacity. This function is useful when 
    working with dynamic memory allocation or when you need to determine how much space is left in a buffer from a specific point, 
    helping you manage memory effectively when writing or updating buffer data.
    
    The function returns the available capacity of the buffer starting from the specified position by @from.
    If the @from value is greater than or equal to the buffer's total capacity, the function would return 0, 
    indicating there is no available space starting from that position.
*/

AVX afxUnit         AvxGetBufferCapacity
(
    // The buffer to be queried.
    avxBuffer       buf, 
    // The starting point in the buffer from which capacity is being measured.
    afxUnit         from
);

/*
    The AvxGetBufferUsage() function retrieves or queries specific usage flags associated with a buffer object. 
    It ensures that the buffer is valid before proceeding, and it allows filtering the buffer’s usage flags using a bitmask if needed. 
    If no bitmask is provided, it simply returns the entire usage flags of the buffer. This function is useful for checking or 
    validating how a buffer is intended to be used in a graphics pipeline.

    If a @usage pattern is specified, it returns the bitwise AND between buffer's usage and the specified usage. 
    This would allow checking if a specific usage flag is set in the buffer's usage flags.
    If usage is NIL, it simply returns the entire buffer's usage value.

    The function returns either the filtered usage flags (if a specific usage bitmask is specified),
    or, it returns the entire usage flag of the buffer (if no specific usage flag is specified).
*/

AVX avxBufferUsage  AvxGetBufferUsage
(
    // The buffer to be queried.
    avxBuffer       buf, 
    // A bitmask or a specific usage flag for the buffer.
    avxBufferUsage  usage
);

/*
    The AvxGetBufferAccess() function retrieves the access flags of a buffer. By performing a bitwise operation, 
    it checks whether the buffer has certain access permissions (such as read or write access). If the access flag is provided, 
    it returns whether the requested access type is set in the buffer’s flags. If no specific flag is requested, it returns the 
    buffer’s entire set of access flags. This function is useful for querying buffer access and ensuring the buffer is being used 
    correctly in the graphics pipeline.
*/

AVX avxBufferFlags  AvxGetBufferAccess
(
    // The buffer to be queried.
    avxBuffer       buf, 
    // A bitmask representing a specific access flag or a query for specific access flags.
    avxBufferFlags  access
);

/*
    The AvxSyncMappedBuffer() method synchronizes a buffer's state to ensure that it is ready for further operations (such as reading or writing).
    If synchronization fails, the function raises an error.

    This function is just a convenient way of calling AvxSyncBufferedMaps(), which actually performs the synchronization, for a single buffer.
    Otherwise, AvxSyncBufferedMaps() should be used to avoid contention.
*/

AVX afxError        AvxSyncMappedBuffer
(
    // A buffer object that needs to be synchronized.
    avxBuffer       buf, 
    // The starting point in the buffer (in terms of byte offset) where the synchronization should begin.
    afxSize         offset, 
    // The size or range (in bytes or units) to be synchronized, relative to the @offset.
    afxUnit         range
);

/*
    The AvxFlushMappedBuffer() method ensures any changes to a buffer's data are committed or flushed to memory. 
    And if any errors occur during this process, they are raised and returned.

    This function is just a convenient way of calling AvxFlushBufferedMaps(), which carries out the synchronization, for a single buffer.
    Otherwise, AvxFlushBufferedMaps() should be used to avoid contention.
*/

AVX afxError        AvxFlushMappedBuffer
(
    // The buffer that needs to be flushed.
    avxBuffer       buf, 
    // The starting point (in bytes) of the portion of the buffer that should be flushed.
    afxSize         offset, 
    // The size (in bytes or units) of the portion of the buffer to flush, starting from the @offset.
    afxUnit         range
);

/*
    The AvxMapBuffer() method maps a buffer to memory, making it accessible for CPU operations. It validates inputs (buffer validity, 
    alignment, range), prepares the mapping structure, and calls the system's mapping function to map the buffer. The function also 
    includes error handling and validation to ensure that the mapping process is carried out safely and efficiently.
    
    This function is just a convenient way of calling AvxMapBuffers(), which is called to perform the actual mapping of the buffer,
    for a single buffer. Otherwise, AvxMapBuffers() should be used to avoid contention.
*/

AVX afxError        AvxMapBuffer
(
    // The buffer that needs to be mapped.
    avxBuffer       buf, 
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
    The AvxUnmapBuffer() method unmaps a buffer from memory. It ensures that any changes to the buffer are synchronized before releasing 
    the memory access. The function validates the buffer and drawing system, calls the unmapping process, and handles errors if the unmap 
    operation fails. The wait flag is not used in this function's implementation, but it could be useful for managing synchronization in 
    a more complex implementation.

    This function is just a convenient way of calling AvxUnmapBuffers(), which is called to perform the actual unmapping of the buffer,
    for a single buffer. Otherwise, AvxUnmapBuffers() should be used to avoid contention.
*/

AVX afxError        AvxUnmapBuffer
(
    // The buffer to unmap.
    avxBuffer       buf, 
    // A flag indicating whether the operation should wait for synchronization.
    afxBool         wait
);

AVX afxBool         AvxIsBufferMapped(avxBuffer buf, afxSize offset, afxUnit range);

/*
    The AvxUploadBuffer() method uploads data to a buffer. It validates the operations, prepares a transfer structure, and then 
    performs the actual data upload. The function handles synchronization and ensures that errors are caught and thrown appropriately. 
    The use of execution units allows for parallel or distributed uploads, such as using a GPU to handle the operations.
*/

AVX afxError        AvxUploadBuffer
(
    // The destination buffer to which data will be uploaded.
    avxBuffer       buf, 
    // The source stream from which the data will be transferred into the buffer.
    afxStream       in, 
    // The number of operations to be performed on the buffer.
    afxUnit         opCnt, 
    // An array of operations (@opCnt number of operations) to be performed on the buffer.
    // Each operation specifies where the data will be uploaded within the buffer and how the data should be handled.
    avxBufferIo const ops[], 
    // The execution unit index. This typically refers to the specific GPU or compute unit that will handle the operation.
    afxMask         exuMask
);

/*
    The AvxDownloadBuffer() method downloads data from a buffer. It validates the operations, prepares a transfer structure, and then 
    performs the actual data transfer. It also handles synchronization and error handling, ensuring that the data is downloaded to the 
    specified output stream. The function is optimized for use in parallel computing systems, with the ability to specify the execution 
    unit responsible for the operation.
*/

AVX afxError        AvxDownloadBuffer
(
    // The source buffer from which data will be downloaded.
    avxBuffer       buf, 
    // The destination stream where the data from the buffer will be transferred.
    afxStream       out, 
    // The number of operations to be performed on the buffer.
    afxUnit         opCnt, 
    // An array of operations to be performed on the buffer. 
    // Each operation specifies how data should be extracted from the buffer.
    avxBufferIo const ops[], 
    // The execution unit index. This refers to the specific GPU or compute unit responsible for the operation.
    afxMask         exuMask
);

/*
    The AvxDumpBuffer() method dumps data from a buffer to a specified destination. It validates the operations, prepares a transfer 
    structure, and then performs the actual data transfer. It also handles synchronization and error handling, ensuring that the data 
    is dumped to the specified destination. The function is optimized for use in parallel computing systems, with the ability to specify 
    the execution unit responsible for the operation.
*/

AVX afxError        AvxDumpBuffer
(
    // The source buffer from which data will be dumped.
    avxBuffer       buf, 
    // A pointer to the destination where the buffer data will be dumped.
    void*           dst, 
    // The number of operations to be performed on the buffer.
    afxUnit         opCnt, 
    // An array of operations to be performed on the buffer. 
    // Each operation specifies how data should be dumped from the buffer.
    avxBufferIo const ops[], 
    // The execution unit index (e.g., GPU or compute unit) responsible for the operation.
    afxMask         exuMask
);

AVX afxError        AvxUpdateBuffer
(
    // The target buffer that will be updated with new data.
    avxBuffer       buf, 
    // A pointer to the source data from which the buffer will be updated.
    void const*     src, 
    // The number of operations to be performed on the buffer.
    afxUnit         opCnt, 
    // An array of operations to be performed on the buffer. 
    // Each operation specifies how the buffer should be updated.
    avxBufferIo const ops[], 
    // The execution unit index (e.g., GPU or compute unit) that will perform the transfer.
    afxMask         exuMask
);

////////////////////////////////////////////////////////////////////////////////

AVX afxError        AvxAcquireBuffers(afxDrawSystem dsys, afxUnit cnt, avxBufferInfo const infos[], avxBuffer buffers[]);

/*
    The AvxFlushBufferedMaps() function ensures that any pending operations involving mapped buffers are completed, typically flushing any 
    modifications made to these buffers and synchronizing them with the GPU or draw system. The function validates the input, checks 
    buffer states, and either uses a ICD callback or a draw queue to execute the flush operation.

    The function validates the provided buffers and ensures they are part of the correct draw system.

    If no ICD callback is available for flushing the maps, it uses a draw queue and bridge to synchronize the flush operation.

    Error handling and synchronization are key components of this function, ensuring that buffer operations are completed before continuing with other tasks.
*/

AVX afxError        AvxFlushBufferedMaps
(
    // The draw system providing the buffers.
    afxDrawSystem   dsys, 
    // The number of buffered maps.
    afxUnit         cnt, 
    // An array of buffered map operations.
    avxBufferedMap const maps[]
);

/*
    The AvxSyncBufferedMaps() function synchronizes buffer maps between the host (CPU) and the GPU or another compute system. 
    It ensures that the mapped buffers are correctly handled, and any changes are synchronized, either through a ICD callback function or 
    a default mechanism.
*/

AVX afxError        AvxSyncBufferedMaps
(
    // The draw system providing the buffers.
    afxDrawSystem   dsys, 
    // The number of buffered maps.
    afxUnit         cnt, 
    // An array of buffered map operations.
    avxBufferedMap const maps[]
);

/*
    The AvxMapBuffers() function maps buffer regions to make them accessible for read/write operations in a graphics or compute system. 
    It allows for mapping multiple buffer regions in parallel, managing synchronization, validation, and performing the actual mapping of memory.

    Each buffer mapping is validated, and if it's already mapped, the function ensures the new mapping is within the current mapped region.
    If the buffer has been allocated in host memory (buf->data), the function directly maps the buffer region.
    If the buffer is a GPU-side or managed buffer, the function defers the actual mapping to a later stage using a draw system operation.

    The function uses atomic operations to manage the pending mapping state of each buffer.
    The function waits for any ongoing mapping operations to complete before proceeding.

    The function first checks for a ICD callback function associated with the draw system.
    If the callback is available, it uses it to perform the mapping; otherwise, it falls back to using draw queues and bridges.
    If no callback is available, the function uses a draw queue and draw bridge to handle the buffer remapping.

    The function waits for the draw queue to complete the remapping operation before proceeding.
    After the mapping operations are completed, the function ensures that all buffers have their pendingRemap counters decremented.

    It also performs additional validation (if enabled) to ensure the buffer's mapped state is consistent.
*/

AVX afxError        AvxMapBuffers(afxDrawSystem dsys, afxUnit cnt, avxBufferedMap maps[], void** placeholders[]);

/*
    The AvxUnmapBuffers() function unmaps buffers previously mapped in the graphics or compute system. It handles synchronization, 
    error checking, and supports both host-side buffers and buffers that are managed by the draw system.

    The function iterates through the list of buffers and waits for any ongoing mapping operations to complete before unmapping the buffers.
    The function uses atomic operations to manage the pending remap state of each buffer and ensure synchronization during unmapping.

    If the draw system provides a ICD callback function, it is used to unmap the buffers.
    If no ICD callback is available, the function uses a draw queue to perform the unmap operation.

    After unmapping the buffers, the function performs final checks (if validation is enabled) and clears the pendingRemap state for each buffer.
*/

AVX afxError        AvxUnmapBuffers(afxDrawSystem dsys, afxUnit cnt, avxBufferedMap maps[]);

#endif//AVX_BUFFER_H
