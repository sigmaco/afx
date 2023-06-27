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

// Index buffer não armazena informação de tipo de primitivo porque, como o nome sugere, é um buffer de índice, não de primitivos.

AFX_DEFINE_HANDLE(afxIndexBuffer);

AFX_OBJECT(afxIndexBuffer)
{
    AFX_OBJECT(afxBuffer)   buf; // IBUF
#ifdef _AFX_INDEX_BUFFER_C
    afxNat                  regionCnt;
    struct
    {
        afxNat32            baseOffset;
        afxNat32            idxCnt;
        afxNat8             idxSiz;
    }                       *regions;
#endif
};

AFX_DEFINE_STRUCT(afxIndexBufferBlueprintRegion)
{
    afxNat32                idxCnt;
    void const              *src;
    afxNat                  srcStride;
};

AFX_DEFINE_STRUCT(afxIndexBufferBlueprint)
{
    afxDrawContext          dctx;
    afxArray                regions;
};

AFX void const*             AfxIndexBufferGetData(afxIndexBuffer ibuf, afxNat rgnIdx, afxNat baseIdx);
AFX afxNat                  AfxIndexBufferGetCap(afxIndexBuffer ibuf, afxNat rgnIdx);
AFX afxNat                  AfxIndexBufferGetStride(afxIndexBuffer ibuf, afxNat rgnIdx);
AFX afxError                AfxIndexBufferOptimize(afxIndexBuffer ibuf);

AFX afxError                AfxIndexBufferDump(afxIndexBuffer ibuf, afxNat rgnIdx, afxNat baseIdx, afxNat idxCnt, void *dst, afxNat dstIdxStride); // copy out
AFX afxError                AfxIndexBufferUpdate(afxIndexBuffer ibuf, afxNat rgnIdx, afxNat baseIdx, afxNat idxCnt, void const *src, afxNat srcIdxSiz); // copy into

AFX afxError                AfxIndexBufferForEachVertex(afxIndexBuffer ibuf, afxNat rgnIdx, afxNat baseIdx, afxNat idxCnt, afxVertexBuffer vbuf, afxNat rowIdx, afxNat baseVtx, void(*f)(void const *vtx, void*data), void *data);

AFX void                    AfxIndexBufferDescribeRegion(afxIndexBuffer ibuf, afxNat rgnIdx, afxNat32 *baseOffset, afxNat *idxCnt, afxNat *idxSiz);

////////////////////////////////////////////////////////////////////////////////
// INDEX BUFFER BLUEPRINT                                                     //
////////////////////////////////////////////////////////////////////////////////

AFXINL void                 AfxIndexBufferBlueprintBegin(afxIndexBufferBlueprint *blueprint, afxDrawContext dctx, afxNat estRgnCnt);
AFXINL void                 AfxIndexBufferBlueprintEnd(afxIndexBufferBlueprint *blueprint, afxNat cnt, afxIndexBuffer ibuf[]);
AFXINL void                 AfxIndexBufferBlueprintErase(afxIndexBufferBlueprint *blueprint);
AFXINL afxError             AfxIndexBufferBlueprintPushRegion(afxIndexBufferBlueprint *blueprint, afxNat32 idxCnt, void const *src, afxNat srcStride);

#endif//AFX_INDEX_BUFFER_H