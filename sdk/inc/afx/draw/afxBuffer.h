/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *              T H E   Q W A D R O   E X E C U T I O N   E C O S Y S T E M
 *
 *                                   Public Test Build
 *                   (c) 2017 SIGMA Technology Group � Federa��o SIGMA
 *                                    www.sigmaco.org
 */

// This section is part of SIGMA GL/2.

#ifndef AFX_BUFFER_H
#define AFX_BUFFER_H

#include "afx/draw/afxDrawDefs.h"

typedef enum afxBufferUsage
{
    afxBufferUsage_UPSTREAM          = AFX_BIT_OFFSET(0), // The buffer can be mapped for reading. May only be combined with DST.
    afxBufferUsage_DOWNSTREAM        = AFX_BIT_OFFSET(1), // The buffer can be mapped for writing. May only be combined with SRC.
    afxBufferUsage_MAP               = (afxBufferUsage_UPSTREAM | afxBufferUsage_DOWNSTREAM),
    afxBufferUsage_SRC               = AFX_BIT_OFFSET(2), // The buffer can be used as the source of a copy operation.
    afxBufferUsage_DST               = AFX_BIT_OFFSET(3), // The buffer can be used as the destination of a copy or write operation.
    afxBufferUsage_COPY              = (afxBufferUsage_SRC | afxBufferUsage_DST),
    afxBufferUsage_INDEX             = AFX_BIT_OFFSET(4), // The buffer can be used as an index buffer.
    afxBufferUsage_VERTEX            = AFX_BIT_OFFSET(5), // The buffer can be used as a vertex buffer.
    afxBufferUsage_UNIFORM           = AFX_BIT_OFFSET(6), // The buffer can be used as a uniform buffer.
    afxBufferUsage_STORAGE           = AFX_BIT_OFFSET(7), // The buffer can be used as a storage buffer.
    afxBufferUsage_INDIRECT          = AFX_BIT_OFFSET(8), // The buffer can be used as to store indirect command arguments.
    afxBufferUsage_QUERY             = AFX_BIT_OFFSET(9), // The buffer can be used to capture query results.
} afxBufferUsage;

typedef enum afxBufferMap
{
    AFX_BUF_MAP_R                   = AFX_BIT_OFFSET(0), // Only valid with buffers created with the MAP/UPSTREAM/READ usage.
    AFX_BUF_MAP_W                   = AFX_BIT_OFFSET(1), // Only valid with buffers created with the MAP/DOWSTREAM/WRITE usage.
    
    AFX_BUF_MAP_X                   = AFX_BIT_OFFSET(2),
    AFX_BUF_MAP_PERSISTENT          = AFX_BIT_OFFSET(3),
    AFX_BUF_MAP_COHERENT            = AFX_BIT_OFFSET(4),
} afxBufferMap;

#define AFX_BUF_IDD 48

AFX_DECLARE_STRUCT(_afxBufVmt);

#ifdef _AFX_BUFFER_C
AFX_OBJECT(afxBuffer)
#else
struct afxBaseBuffer
#endif
{
    _afxBufVmt const*   vmt;
    afxSize             siz;
    afxBufferUsage      usage;
    afxByte*            bytemap;
};

AFX_DEFINE_STRUCT(afxBufferSpecification)
{
    afxSize             siz;
    afxBufferUsage      usage;
    void const          *src;
};

AFX_DEFINE_STRUCT(afxBufferRef)
{
    afxBuffer           buf;
    afxFormat           fmt;
    afxSize             off;
    afxSize             range;
};

AFX_DEFINE_STRUCT(afxMappedBufferRange)
{
    afxMappedBufferRange*   next;
    afxBuffer               buf;
    afxSize                 offset;
    afxNat                  size;
};

AFX afxError        AfxAcquireBuffers(afxDrawContext dctx, afxNat cnt, afxBuffer buf[], afxBufferSpecification const spec[]);

AFX afxSize         AfxGetBufferSize(afxBuffer buf);

AFX void*           AfxMapBufferRange(afxBuffer buf, afxSize offset, afxNat range, afxFlags flags);
AFX void            AfxUnmapBufferRange(afxBuffer buf);

AFX afxError        AfxDumpBuffer(afxBuffer buf, afxSize offset, afxSize range, void *dst);
AFX afxError        AfxDumpBuffer2(afxBuffer buf, afxSize offset, afxSize stride, afxSize cnt, void *dst, afxSize dstStride);

AFX afxError        AfxUpdateBuffer(afxBuffer buf, afxSize offset, afxSize range, void const *src);
AFX afxError        AfxUpdateBuffer2(afxBuffer buf, afxSize offset, afxSize range, afxSize stride, void const *src, afxSize srcStride);

AFX_DEFINE_STRUCT(afxBufferRegion)
{
    afxNat32 base;
    afxNat32 range;
    afxNat32 stride;
    afxNat32 offset;
    afxNat32 unitSiz;
};

AFX afxError        AfxUpdateBufferRegion(afxBuffer buf, afxBufferRegion const* rgn, void const* src, afxSize srcStride);

#endif//AFX_BUFFER_H