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

#ifndef AFX_INDEX_BUFFER_H
#define AFX_INDEX_BUFFER_H

#include "afx/draw/res/afxVertexBuffer.h"

// Index buffer n�o armazena informa��o de tipo de primitivo porque, como o nome sugere, � um buffer de �ndice, n�o de primitivos.

AFX_DEFINE_HANDLE(afxIndexBuffer);

AFX_OBJECT(afxIndexBuffer)
{
    AFX_OBJECT(afxBuffer)   buf; // IBUF
#ifdef _AFX_INDEX_BUFFER_C
    afxNat                  idxCnt;
    afxNat                  idxSiz;
#endif
};

AFX_DEFINE_STRUCT(afxIndexBufferSpecification)
{
    afxNat                  cnt;
    afxNat                  siz;
    void const              *src;
    afxNat                  stride;
};

AFX void const*             AfxIndexBufferGetData(afxIndexBuffer ibuf, afxNat baseIdx);
AFX afxNat                  AfxIndexBufferGetCount(afxIndexBuffer ibuf);
AFX afxNat                  AfxIndexBufferGetStride(afxIndexBuffer ibuf);
AFX afxError                AfxIndexBufferOptimize(afxIndexBuffer ibuf);

AFX afxError                AfxIndexBufferDump(afxIndexBuffer ibuf, afxNat baseIdx, afxNat idxCnt, void *dst, afxNat dstIdxStride); // copy out
AFX afxError                AfxIndexBufferUpdate(afxIndexBuffer ibuf, afxNat baseIdx, afxNat idxCnt, void const *src, afxNat srcIdxSiz); // copy into

AFX afxError                AfxIndexBufferForEachVertex(afxIndexBuffer ibuf, afxNat baseIdx, afxNat idxCnt, afxVertexBuffer vbuf, afxNat rowIdx, afxNat baseVtx, void(*f)(void const *vtx, void*data), void *data);

#endif//AFX_INDEX_BUFFER_H