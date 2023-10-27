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

#ifndef AFX_VERTEX_H
#define AFX_VERTEX_H

#include "afx/sim/afxSimDefs.h"
#include "afx/draw/afxDrawDefs.h"

typedef afxNat8 afxVertexIndex8;
typedef afxNat16 afxVertexIndex16;
typedef afxNat32 afxVertexIndex32;
typedef afxVertexIndex8 afxIndexedPoint8[1];
typedef afxVertexIndex16 afxIndexedPoint16[1];
typedef afxVertexIndex32 afxIndexedPoint32[1];
typedef afxVertexIndex8 afxIndexedLine8[2];
typedef afxVertexIndex16 afxIndexedLine16[2];
typedef afxVertexIndex32 afxIndexedLine32[2];
typedef afxVertexIndex8 afxIndexedTriangle8[3];
typedef afxVertexIndex16 afxIndexedTriangle16[3];
typedef afxVertexIndex32 afxIndexedTriangle32[3];

typedef enum afxVertexUsage2
{
    AFX_VTX_USAGE2_POS,
    AFX_VTX_USAGE2_WGT,
    AFX_VTX_USAGE2_IDX,
    
    AFX_VTX_USAGE2_TAN,
    AFX_VTX_USAGE2_BIN,
    AFX_VTX_USAGE2_NRM,

    AFX_VTX_USAGE2_TEX,
    AFX_VTX_USAGE2_COL,

    AFX_VTX_USAGE2_TOTAL
} afxVertexUsage2;

AFX_DEFINE_STRUCT(afxVertexLayout)
{
    afxNat          compCnt;
    afxVertexUsage2  usage[AFX_VTX_USAGE2_TOTAL];
    afxVertexFormat fmt[AFX_VTX_USAGE2_TOTAL];
};

AFX afxBool AfxSimulationDescribeVertexLayout(afxSimulation sim, afxNat idx, afxVertexLayout *layout);
AFX afxBool AfxSimulationChooseVertexLayout(afxSimulation sim, afxVertexLayout const *layout, afxNat *idx);

//AFX afxNat AfxGetVertexSize(afxVertexLayout const *layout);
AFX afxNat AfxGetVertexPositionalSize(afxVertexLayout const *layout);
AFX afxNat AfxGetVertexSpatialSize(afxVertexLayout const *layout);
AFX afxNat AfxGetVertexAggregatedSize(afxVertexLayout const *layout);
AFX afxNat AfxGetVertexComponentCount(afxVertexLayout const *layout);

AFX afxNat AfxGetVertexComponentOffset(afxVertexLayout const *layout, afxVertexUsage2 usage);
AFX afxVertexFormat AfxGetVertexComponentFormat(afxVertexLayout const *layout, afxVertexUsage2 usage);
AFX afxBool AfxGetVertexComponent(afxVertexLayout const *layout, afxVertexUsage2 usage, afxNat *idx);

AFX void AfxCopyVertexComponents(afxVertexUsage2 first, afxVertexUsage2 cnt, afxNat vtxCnt, afxVertexLayout const *dstLayout, void *dstData, afxVertexLayout const *srcLayout, void const *src);
AFX void AfxCopyVertexComponents2(afxVertexUsage2 first, afxVertexUsage2 cnt, afxNat vtxCnt, afxVertexLayout const *dstLayout, void *dstData, afxVertexFormat const srcFmt[], void const *src);
AFX void AfxAccumulateVertexComponents(afxVertexUsage2 first, afxVertexUsage2 cnt, afxNat vtxCnt, afxVertexLayout const *dstLayout, void *dstData, afxVertexLayout const *srcLayout, void const *src);
AFX void AfxAccumulateVertexComponents2(afxVertexUsage2 first, afxVertexUsage2 cnt, afxNat vtxCnt, afxVertexLayout const *dstLayout, void *dstData, afxVertexFormat const srcFmt[], void const *src);

AFX void AfxCopyVertices(afxNat vtxCnt, afxVertexLayout const *layout, void *dst, afxVertexLayout const *srcLayout, void const *src);
AFX void AfxMergeVertices(afxNat vtxCnt, afxVertexLayout const *layout, void *dst, afxVertexLayout const *srcLayout, void const *src);

#endif//AFX_VERTEX_H
