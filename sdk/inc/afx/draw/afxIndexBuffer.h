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
 *                   (c) 2017 SIGMA Technology Group — Federação SIGMA
 *                                    www.sigmaco.org
 */

// This section is part of SIGMA GL.

#ifndef AFX_INDEX_BUFFER_H
#define AFX_INDEX_BUFFER_H

#include "afx/draw/afxVertexBuffer.h"

// Index buffer não armazena informação de tipo de primitivo porque, como o nome sugere, é um buffer de índice, não de primitivos.

#ifdef _AFX_INDEX_BUFFER_C
AFX_OBJECT(afxIndexBuffer)
#else
struct afxBaseIndexBuffer
#endif
{
    afxBuffer               buf; // IBUF
    afxNat                  regionCnt;
    struct
    {
        afxNat32            baseOffset;
        afxNat32            idxCnt;
        afxNat8             idxSiz;
    }                       *regions;
};

AFX_DEFINE_STRUCT(afxIndexBufferBlueprintRegion)
{
    afxNat32                idxCnt;
    void const              *src;
    afxNat                  srcIdxSiz;
    afxNat                  srcStride;
};

AFX_DEFINE_STRUCT(afxIndexBufferBlueprint)
{
    afxDrawContext          dctx;
    afxArray                regions;
};

AFX afxError                AfxBuildIndexBuffers(afxDrawContext dctx, afxNat cnt, afxIndexBuffer ibuf[], afxIndexBufferBlueprint const blueprint[]);

//AFX void const*             AfxIndexBufferGetData(afxIndexBuffer ibuf, afxNat rgnIdx, afxNat baseIdx);
AFX afxNat                  AfxIndexBufferGetCap(afxIndexBuffer ibuf, afxNat rgnIdx);
AFX afxNat                  AfxIndexBufferGetStride(afxIndexBuffer ibuf, afxNat rgnIdx);
AFX afxNat                  AfxIndexBufferGetOffset(afxIndexBuffer ibuf, afxNat rgnIdx, afxNat baseIdx);
AFX afxError                AfxIndexBufferOptimize(afxIndexBuffer ibuf);

AFX afxError                AfxIndexBufferDump(afxIndexBuffer ibuf, afxNat rgnIdx, afxNat baseIdx, afxNat idxCnt, void *dst, afxNat dstIdxStride); // copy out
AFX afxError                AfxIndexBufferUpdate(afxIndexBuffer ibuf, afxNat rgnIdx, afxNat baseIdx, afxNat idxCnt, void const *src, afxNat srcIdxSiz); // copy into

AFX afxError                AfxIndexBufferForEachVertex(afxIndexBuffer ibuf, afxNat rgnIdx, afxNat baseIdx, afxNat idxCnt, afxVertexBuffer vbuf, afxNat rowIdx, afxNat baseVtx, void(*f)(void const *vtx, void*data), void *data);

AFX void                    AfxIndexBufferDescribeRegion(afxIndexBuffer ibuf, afxNat rgnIdx, afxNat32* baseOffset, afxNat32* idxCnt, afxNat32* idxSiz);

////////////////////////////////////////////////////////////////////////////////
// INDEX BUFFER BLUEPRINT                                                     //
////////////////////////////////////////////////////////////////////////////////

AFXINL void                 AfxIndexBufferBlueprintBegin(afxIndexBufferBlueprint *blueprint, afxDrawContext dctx, afxNat estRgnCnt);
AFXINL void                 AfxIndexBufferBlueprintEnd(afxIndexBufferBlueprint *blueprint, afxNat cnt, afxIndexBuffer ibuf[]);
AFXINL void                 AfxIndexBufferBlueprintErase(afxIndexBufferBlueprint *blueprint);
AFXINL afxError             AfxIndexBufferBlueprintPushRegion(afxIndexBufferBlueprint *blueprint, afxNat32 idxCnt, void const *src, afxNat srcIdxSiz, afxNat srcStride);

#endif//AFX_INDEX_BUFFER_H