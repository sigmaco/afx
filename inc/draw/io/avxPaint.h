/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                  Q W A D R O   E X E C U T I O N   E C O S Y S T E M
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

#ifndef AVX_PAINT_H
#define AVX_PAINT_H

#include "qwadro/inc/draw/afxDrawDefs.h"
#include "qwadro/inc/math/afxColor.h"

AFX_DEFINE_HANDLE(avxVectorizer);

AFX_DEFINE_STRUCT(avxImmVertex)
{
    afxV4d      posn; // in RW, W is the reciprocal of homogeneous W; known as RHW.
    afxV2d      uv;
    afxNat32    col; // emissive color
};

AFX_DEFINE_STRUCT(avxImm3dVertex)
{
    afxV3d      pos;
    afxV3d      nrm;
    afxV2d      uv;
    afxNat32    col;
};

typedef afxIndex avxVertexIndex;


AFX_OBJECT(avxVectorizer)
{
    int a;
};

AFX_DEFINE_STRUCT(avxGraph)
{
    int a;
};

AVX void        AvxCmdBeginImm2d(avxCmdb cmdb);
AVX void        AvxCmdEndImm2d(avxCmdb cmdb);
AVX void        AvxCmdTransformImmVertices(avxCmdb cmdb, afxM4d const m, afxNat cnt, avxImmVertex vertices[]);

AVX void        AvxImmSetColor(avxCmdb cmdb, afxColor const inner, afxColor const outer);
AVX void        AvxImmBindRasters();
AVX void        AvxImmBindRasterizer(avxCmdb cmdb, avxRasterizer razr);

/// The BeginPath() function opens a path bracket in the specified device context.
AVX afxBool     AvxBeginPath(avxVectorizer vecr);

/// The EndPath() function closes a path bracket and selects the path defined by the bracket into the specified device context.
AVX afxBool     AvxEndPath(avxVectorizer vecr);

/// The FillPath() function closes any open figures in the current path and fills the path's interior by using the current brush and polygon-filling mode.
AVX afxBool     AvxFillPath(avxVectorizer vecr);

/// The LineTo() function draws a line from the current position up to, but not including, the specified point.
AVX afxBool     AvxLineTo(avxVectorizer vecr, afxV2d const pos);

/// The MoveTo() function updates the current position to the specified point and optionally returns the previous position.
AVX afxBool     AvxMoveTo(avxVectorizer vecr, afxV2d const pos, afxV2d prev);

/// The AngleArc() function draws a line segment and an arc. The line segment is drawn from the current position to the beginning of the arc. The arc is drawn along the perimeter of a circle with the given radius and center. The length of the arc is defined by the given start and sweep angles.
AVX afxBool     AvxAngleArc(avxVectorizer vecr, afxV2d const pos, afxReal r, afxReal startAngle, afxReal sweepAngle);




AVX void AvxGraphRoundedRect(avxGraph* gra, afxV2d pos, afxV2d extent, afxReal cornerRadius);
AVX void AvxGraphSetFillColor(avxGraph* gra, afxColor const cor);

AVX void AvxGraphRect(avxGraph* gra, afxV2d pos, afxV2d extent);
AVX afxNat AvxGraphBoxGradient(avxGraph* gra, afxV2d pos, afxV2d extent, afxReal radiusunk, afxReal unk, afxColor src, afxColor dst);

AVX void AvxGraphWinding(avxGraph* gra, afxBool hole);

AVX void AvxGraphFillPaint(avxGraph* gra, afxNat paintId);

AVX afxNat AvxGraphLinearGradient(avxGraph* gra, afxV2d pos, afxV2d extent, afxColor src, afxColor dst);

AVX void AvxGraphStrokeColor(avxGraph* gra, afxColor cor);
AVX void AvxGraphStroke(avxGraph* gra);

AVX void AvxGraphFontSize(avxGraph* gra, afxReal siz);
AVX void AvxGraphFontFace(avxGraph* gra, afxString const* name);
AVX void AvxGraphTextAlign(avxGraph* gra, afxFlags mask);

AVX void AvxGraphFontBlur(avxGraph* gra, afxReal inten);
AVX void AvxGraphText(avxGraph* gra, afxV2d pos, afxReal length, afxString const* name);

#endif//AVX_PAINT_H
