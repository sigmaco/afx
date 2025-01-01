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
#include "qwadro/inc/draw/io/afxPixel.h"

#define AFX_BUF_ALIGNMENT 64
#define AFX_BUF_ALIGN(siz_) AFX_ALIGNED_SIZE((siz_), AFX_BUF_ALIGNMENT)

typedef enum afxBufferFlags
{
    // Mapped range is readable by CPU.
    afxBufferFlag_R             = AFX_BIT(0),

    // Mapped range is writeable by CPU.
    afxBufferFlag_W             = AFX_BIT(1),

    // Mapped range is readable and writeable by CPU.
    afxBufferFlag_RW            = (afxBufferFlag_R | afxBufferFlag_W),

    // Mapped range will be executable by GPU while being read and/or write by CPU.
    afxBufferFlag_X             = AFX_BIT(2),
    afxBufferFlag_RWX           = (afxBufferFlag_RW | afxBufferFlag_X),

    // Data written in mapped range will be immediately visible by GPU and CPU.
    afxBufferFlag_COHERENT      = (afxBufferFlag_X | AFX_BIT(3)),

    afxBufferFlag_ACCESS_MASK   = (afxBufferFlag_RWX | afxBufferFlag_COHERENT)
} afxBufferFlags;

typedef enum afxBufferUsage
{
    afxBufferUsage_UPLOAD   = AFX_BIT(0), // Can be used as the "source" of a copy operation.
    afxBufferUsage_DOWNLOAD = AFX_BIT(1), // Can be used as the "destination" of a copy or write operation.
    afxBufferUsage_VERTEX   = AFX_BIT(2), // Can be used as a vertex buffer; aka VBO.
    afxBufferUsage_INDEX    = AFX_BIT(3), // Can be used as an index buffer; aka IBO.
    afxBufferUsage_UNIFORM  = AFX_BIT(4), // Can be used as a uniform buffer; aka UBO.
    afxBufferUsage_STORAGE  = AFX_BIT(5), // Can be used as a storage buffer; aka SSBO.
    afxBufferUsage_INDIRECT = AFX_BIT(6), // Can be used as to store indirect command arguments; aka DBO.
    afxBufferUsage_QUERY    = AFX_BIT(7), // Can be used to capture query results; aka QBO.    
    afxBufferUsage_TENSOR   = AFX_BIT(8) // Can be used to fetch (usually texels) data; aka TBO.
} afxBufferUsage;

AFX_DEFINE_STRUCT(afxBufferInfo)
{
    // A bitmask specifying allowed usages of the buffer.
    afxBufferUsage  usage;

    // A avxFormat describing the format of the data elements in the buffer.
    avxFormat       fmt; // used for TENSOR buffers.

    // The size in bytes of the buffer to be created.
    afxUnit         cap;

    // A bitmask specifying additional parameters of the buffer.
    afxBufferFlags  flags;

    // A bitmask of device bridges that will access this buffer.
    afxMask         sharingMask;

    // A afxBuffer on which the view will be created.
    afxBuffer       src;

    // An offset in bytes from the base address of the buffer.
    afxSize         start;

    // A user-defined data.
    void*           udd;
};

AFX_DEFINE_STRUCT(afxBufferCopy)
{
    afxSize         srcOffset; /// is the starting offset in bytes from the start of srcBuffer.
    afxSize         dstOffset; /// is the starting offset in bytes from the start of dstBuffer.
    afxUnit         range; /// is the number of bytes to copy.
};

AFX_DEFINE_STRUCT(afxBufferIo)
{
    afxSize         srcOffset; /// is the starting offset in bytes from the start of srcBuffer.
    afxSize         dstOffset; /// is the starting offset in bytes from the start of dstBuffer.
    afxUnit         srcStride; // [!] only if supported by device, else case it must be 1.
    afxUnit         dstStride; // [!] only if supported by device, else case it must be 1.
    afxUnit         rowCnt; /// is the number of rows to stream in/out.
};

AVX afxDrawSystem  AfxGetBufferContext(afxBuffer buf);

AVX void*           AfxGetBufferUdd(afxBuffer buf);

AVX afxUnit         AfxGetBufferCapacity(afxBuffer buf, afxUnit from);

AVX afxBufferUsage  AfxGetBufferUsage(afxBuffer buf);
AVX afxBufferUsage  AfxTestBufferUsage(afxBuffer buf, afxBufferUsage usage);

AVX afxBufferFlags  AfxGetBufferAccess(afxBuffer buf);
AVX afxBufferFlags  AfxTestBufferAccess(afxBuffer buf, afxBufferFlags access);

AVX afxError        AfxMapBuffer(afxBuffer buf, afxUnit offset, afxUnit range, afxFlags flags, void** placeholder);
AVX void            AfxUnmapBuffer(afxBuffer buf, afxBool wait);

AVX afxError        AfxUploadBuffer(afxBuffer buf, afxUnit opCnt, afxBufferIo const ops[], afxUnit exuIdx, afxStream in);
AVX afxError        AfxDownloadBuffer(afxBuffer buf, afxUnit opCnt, afxBufferIo const ops[], afxUnit exuIdx, afxStream out);

AVX afxError        AfxDumpBuffer(afxBuffer buf, afxUnit opCnt, afxBufferIo const ops[], afxUnit exuIdx, void* dst);
AVX afxError        AfxUpdateBuffer(afxBuffer buf, afxUnit opCnt, afxBufferIo const ops[], afxUnit exuIdx, void const* src);

////////////////////////////////////////////////////////////////////////////////

AVX afxError        AfxAcquireBuffers(afxDrawSystem dsys, afxUnit cnt, afxBufferInfo const infos[], afxBuffer buffers[]);

#endif//AVX_BUFFER_H
