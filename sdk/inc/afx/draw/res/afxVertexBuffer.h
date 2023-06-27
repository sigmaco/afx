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

#include "afx/draw/afxDrawDefs.h"

#include "afx/draw/res/afxBuffer.h"

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

AFX_DEFINE_STRUCT(afxVertexRowSpecification)
{
    afxString const         *semantic;
    afxVertexFormat         fmt;
    afxVertexUsage          usage;
    void const              *src;
    afxVertexFormat         srcFmt;
};

AFX_DEFINE_STRUCT(afxVertexRowBlueprint)
{
    afxString8              semantic; // 8
    afxVertexFormat         fmt;
    afxVertexUsage          usage;
    void const              *src;
    afxVertexFormat         srcFmt;
};

AFX_DEFINE_STRUCT(afxVertexBufferBlueprint)
{
    afxFcc                  fcc; // VBUB
    afxNat                  cap;
    afxNat                  rowCnt;
    afxVertexRowBlueprint   row[8];
};

AFX_DEFINE_HANDLE(afxVertexBuffer);

AFX_DECLARE_STRUCT(afxVertexDataRow);

AFX_OBJECT(afxVertexBuffer)
{
    AFX_OBJECT(afxBuffer)   buf;
#ifdef _AFX_VERTEX_BUFFER_C
    afxNat                  cap;
    afxNat                  rowCnt;
    afxVertexDataRow        *row;
#endif
};

AFX afxError                AfxVertexBufferDescribeRow(afxVertexBuffer vbuf, afxNat rowIdx, afxVertexRowSpecification *spec);
AFX afxError                AfxVertexBufferDump(afxVertexBuffer vbuf, afxNat rowIdx, afxNat baseVtx, afxNat vtxCnt, void *dst, afxNat dstVtxStride); // copy out
AFX afxNat                  AfxVertexBufferFindArrange(afxVertexBuffer vbuf, afxString const *name);
AFX afxNat                  AfxVertexBufferGetRowCount(afxVertexBuffer vbuf);
AFX void const*             AfxVertexBufferGetData(afxVertexBuffer vbuf, afxNat rowIdx, afxNat vtxIdx);
AFX afxVertexFormat         AfxVertexBufferGetFormat(afxVertexBuffer vbuf, afxNat rowIdx);
AFX afxString const*        AfxVertexBufferGetSemantic(afxVertexBuffer vbuf, afxNat rowIdx);
AFX afxNat                  AfxVertexBufferGetRange(afxVertexBuffer vbuf, afxNat baseRow, afxNat rowCnt); // get whole memory range used by a arrange
AFX afxNat                  AfxVertexBufferGetOffset(afxVertexBuffer vbuf, afxNat vtxIdx, afxNat rowIdx); // get whole memory range used by a arrange
AFX afxNat                  AfxVertexBufferGetStride(afxVertexBuffer vbuf, afxNat rowIdx); // get element stride
AFX afxNat                  AfxVertexBufferGetCap(afxVertexBuffer vbuf);
AFX afxNat                  AfxVertexBufferGetPerVertexSize(afxVertexBuffer vbuf);
AFX afxError                AfxVertexBufferForEachVertex(afxVertexBuffer vbuf, afxNat rowIdx, afxNat baseVtx, afxNat vtxCnt, void (*f)(void const *vtx,void*data), void *data);
AFX afxError                AfxVertexBufferOptimize(afxVertexBuffer vbuf, afxNat rowIdx, afxBool favorSpeedOverSize);
AFX afxError                AfxVertexBufferUpdate(afxVertexBuffer vbuf, afxNat rowIdx, afxNat baseVtx, afxNat vtxCnt, void const *src, afxVertexFormat srcFmt); // copy into

////////////////////////////////////////////////////////////////////////////////
// VERTEX BUFFER BLUEPRINT                                                    //
////////////////////////////////////////////////////////////////////////////////

AFXINL void                 AfxVertexBufferBlueprintReset(afxVertexBufferBlueprint *blueprint, afxNat cap);
AFXINL void                 AfxVertexBufferBlueprintDiscard(afxVertexBufferBlueprint *blueprint);
AFXINL void                 AfxVertexBufferBlueprintErase(afxVertexBufferBlueprint *blueprint);

AFXINL afxError             AfxVertexBufferBlueprintAddRow(afxVertexBufferBlueprint *blueprint, afxString const *semantic, afxVertexFormat fmt, afxVertexUsage usage, void const *src, afxVertexFormat srcFmt);
AFXINL afxError             AfxVertexBufferBlueprintResetRow(afxVertexBufferBlueprint *blueprint, afxNat idx, afxString const *semantic, afxVertexFormat fmt, afxVertexUsage usage, void const *src, afxVertexFormat srcFmt);

#endif//AFX_VERTEX_BUFFER_H