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
 *                      (c) 2017 SIGMA Co. & SIGMA Technology Group
 *                                    www.sigmaco.org
 */

#ifndef AFX_BUFFER_H
#define AFX_BUFFER_H

#include "afx/draw/afxDrawDefs.h"

typedef enum afxBufferUsage
{
    AFX_BUF_USAGE_SRC              = AFX_FLAG(0), // source for transfer ops.
    AFX_BUF_USAGE_DST              = AFX_FLAG(1), // destination for transfer ops.
    AFX_BUF_USAGE_CONSTANT_TEXELS  = AFX_FLAG(2),
    AFX_BUF_USAGE_STORAGE_TEXELS   = AFX_FLAG(3),
    AFX_BUF_USAGE_CONSTANT         = AFX_FLAG(4), // 'uniform' constant buffer
    AFX_BUF_USAGE_STORAGE          = AFX_FLAG(5), // volatile storage buffer
    AFX_BUF_USAGE_INDICES          = AFX_FLAG(6), // vertex index buffer
    AFX_BUF_USAGE_VERTICES         = AFX_FLAG(7), // vertex buffer
    AFX_BUF_USAGE_INDIRECT         = AFX_FLAG(8),
} afxBufferUsage;

AFX_DEFINE_HANDLE(afxBuffer);

#define AFX_BUF_IDD 48

AFX_OBJECT(afxBuffer)
{
    afxObject           obj;
#ifdef _AFX_BUFFER_C
    afxSize             siz;
    afxBufferUsage      usage;
    afxByte             idd[AFX_BUF_IDD];
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

AFX void*           AfxBufferGetContext(afxBuffer buf);
AFX void*           AfxBufferGetDriver(afxBuffer buf);
AFX void*           AfxBufferGetDrawSystem(afxBuffer buf);
AFX void*           AfxBufferGetIdd(afxBuffer buf);

AFX void const*     AfxBufferGetData(afxBuffer buf, afxSize offset);
AFX afxSize         AfxBufferGetSize(afxBuffer buf);

AFX afxError        AfxBufferDump(afxBuffer buf, afxSize base, afxSize range, void *dst);
AFX afxError        AfxBufferDump2(afxBuffer buf, afxSize offset, afxSize stride, afxSize cnt, void *dst, afxSize dstStride);

AFX afxError        AfxBufferUpdate(afxBuffer buf, afxSize base, afxSize range, void const *src);
AFX afxError        AfxBufferUpdate2(afxBuffer buf, afxSize offset, afxSize stride, afxNat cnt, void const *src, afxSize srcStride);

#endif//AFX_BUFFER_H