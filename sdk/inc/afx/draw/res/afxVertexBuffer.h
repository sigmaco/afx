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

#ifndef AFX_VERTEX_BUFFER_H
#define AFX_VERTEX_BUFFER_H

#include "../base/afxDrawDefs.h"

#include "../res/afxBuffer.h"

typedef enum afxVertexUsage
{
    AFX_VTX_USAGE_DYNAMIC       = AFX_FLAG(0), // The data store contents will be modified repeatedly and used many times.
    AFX_VTX_USAGE_STREAM        = AFX_FLAG(1), // The data store contents will be modified once and used at most a few times.
    //AFX_VTX_USAGE_STATIC = AFX_FLAG(2), // The data store contents will be modified once and used many times.

    AFX_VTX_USAGE_POS           = AFX_FLAG( 9),
    AFX_VTX_USAGE_NRM           = AFX_FLAG(10),
    AFX_VTX_USAGE_TAN           = AFX_FLAG(11),
    AFX_VTX_USAGE_BTN           = AFX_FLAG(12),
    AFX_VTX_USAGE_UV            = AFX_FLAG(13),

    AFX_VTX_USAGE_RASTERIZATION = AFX_FLAG(16),
} afxVertexUsage;

AFX_DEFINE_STRUCT(afxVertexDataSpecification)
{
    afxString const     *semantic;
    afxVertexFormat     fmt;
    afxVertexUsage      usage;
    void const          *src;
    afxVertexFormat     srcFmt;
};

AFX_DEFINE_HANDLE(afxVertexBuffer);

AFX afxError            AfxVertexBufferDump(afxVertexBuffer vbuf, afxNat rowIdx, afxNat baseVtx, afxNat vtxCnt, void *dst, afxNat dstVtxStride); // copy out
AFX afxNat              AfxVertexBufferFindArrange(afxVertexBuffer vbuf, afxString const *name);
AFX afxNat              AfxVertexBufferGetRowCount(afxVertexBuffer vbuf);
AFX void const*         AfxVertexBufferGetData(afxVertexBuffer vbuf, afxNat rowIdx, afxNat vtxIdx);
AFX afxVertexFormat     AfxVertexBufferGetFormat(afxVertexBuffer vbuf, afxNat rowIdx);
AFX afxString const*    AfxVertexBufferGetSemantic(afxVertexBuffer vbuf, afxNat rowIdx);
AFX afxNat              AfxVertexBufferGetRange(afxVertexBuffer vbuf, afxNat baseRow, afxNat rowCnt); // get whole memory range used by a arrange
AFX afxNat              AfxVertexBufferGetOffset(afxVertexBuffer vbuf, afxNat vtxIdx, afxNat rowIdx); // get whole memory range used by a arrange
AFX afxNat              AfxVertexBufferGetStride(afxVertexBuffer vbuf, afxNat rowIdx); // get element stride
AFX afxNat              AfxVertexBufferGetCount(afxVertexBuffer vbuf);
AFX afxNat              AfxVertexBufferGetPerVertexSize(afxVertexBuffer vbuf);
AFX afxError            AfxVertexBufferForEachVertex(afxVertexBuffer vbuf, afxNat rowIdx, afxNat baseVtx, afxNat vtxCnt, void (*f)(void const *vtx,void*data), void *data);
AFX afxError            AfxVertexBufferOptimize(afxVertexBuffer vbuf, afxNat rowIdx, afxBool favorSpeedOverSize);
AFX afxError            AfxVertexBufferUpdate(afxVertexBuffer vbuf, afxNat rowIdx, afxNat baseVtx, afxNat vtxCnt, void const *src, afxVertexFormat srcFmt); // copy into


AFX_DEFINE_STRUCT(afxVertexDataRow)
{
    afxString8          semantic;
    afxNat              offset;
    afxNat              stride;
    afxVertexUsage      usage;
    afxVertexFormat     fmt;

};

#ifndef AFX_DRAW_DRIVER_SRC

AFX_OBJECT(afxVertexBuffer)
{
    AFX_OBJECT(afxBuffer)   buf;
};

#endif


#endif//AFX_VERTEX_BUFFER_H