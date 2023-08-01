/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                      S I G M A   T E C H N O L O G Y   G R O U P
 *
 *                                   Public Test Build
 *                               (c) 2017 Federação SIGMA
 *                                    www.sigmaco.org
 */

#ifndef AFX_BUFFER_H
#define AFX_BUFFER_H

#include "afx/draw/afxDrawDefs.h"

typedef enum afxBufferUsage
{
    AFX_BUF_USAGE_SRC               = AFX_FLAG(0), // source for transfer ops.
    AFX_BUF_USAGE_DST               = AFX_FLAG(1), // destination for transfer ops.
    AFX_BUF_USAGE_CONSTANT_TEXELS   = AFX_FLAG(2),
    AFX_BUF_USAGE_STORAGE_TEXELS    = AFX_FLAG(3),
    AFX_BUF_USAGE_CONSTANT          = AFX_FLAG(4), // 'uniform' constant buffer
    AFX_BUF_USAGE_STORAGE           = AFX_FLAG(5), // volatile storage buffer
    AFX_BUF_USAGE_INDICES           = AFX_FLAG(6), // vertex index buffer
    AFX_BUF_USAGE_VERTICES          = AFX_FLAG(7), // vertex buffer
    AFX_BUF_USAGE_INDIRECT          = AFX_FLAG(8),
} afxBufferUsage;

typedef enum afxBufferMap
{
    AFX_BUF_MAP_R                   = AFX_FLAG(0),
    AFX_BUF_MAP_W                   = AFX_FLAG(1),
    AFX_BUF_MAP_X                   = AFX_FLAG(2),
    AFX_BUF_MAP_PERSISTENT = AFX_FLAG(1),
    AFX_BUF_MAP_COHERENT = AFX_FLAG(1),
} afxBufferMap;

#define AFX_BUF_IDD 48

AFX_DECLARE_STRUCT(_afxBufVmt);

AFX_OBJECT(afxBuffer)
{
    afxObject           obj;
    _afxBufVmt const*   vmt;
    void*               idd;
#ifdef _AFX_BUFFER_C
    afxSize             siz;
    afxBufferUsage      usage;
    afxByte*            bytemap;

    afxBool             locked;
    afxSize             lastUpdOffset, lastUpdRange;
#endif
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

AFX afxDrawContext  AfxGetBufferContext(afxBuffer buf);

AFX void*           AfxGetBufferIdd(afxBuffer buf);

AFX void const*     AfxBufferGetData(afxBuffer buf, afxSize offset);
AFX afxSize         AfxGetBufferSize(afxBuffer buf);

AFX void*           AfxMapBufferRange(afxBuffer buf, afxSize offset, afxNat range, afxFlags flags);
AFX void            AfxUnmapBufferRange(afxBuffer buf);

AFX afxError        AfxDumpBuffer(afxBuffer buf, afxSize base, afxSize range, void *dst);
AFX afxError        AfxDumpBuffer2(afxBuffer buf, afxSize offset, afxSize stride, afxSize cnt, void *dst, afxSize dstStride);

AFX afxError        AfxUpdateBuffer(afxBuffer buf, afxSize base, afxSize range, void const *src);
AFX afxError        AfxUpdateBuffer2(afxBuffer buf, afxSize offset, afxSize stride, afxNat cnt, void const *src, afxSize srcStride);

#endif//AFX_BUFFER_H