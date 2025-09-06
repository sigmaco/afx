/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *        Q W A D R O   V I D E O   G R A P H I C S   I N F R A S T R U C T U R E
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This software is part of Advanced Multimedia UX Extensions & Experiments.

#ifndef AUX_PAINT_H
#define AUX_PAINT_H

#include "qwadro/draw/afxDrawDefs.h"
#include "qwadro/draw/avxColor.h"
#include "qwadro/ux/afxUxDefs.h"

AFX_DEFINE_HANDLE(avxVectorizer);

AFX_DEFINE_STRUCT(avxImmVertex)
{
    afxV4d      posn; // in RW, W is the reciprocal of homogeneous W; known as RHW.
    afxV2d      uv;
    afxUnit32    col; // emissive color
};

AFX_DEFINE_STRUCT(avxImm3dVertex)
{
    afxV3d      pos;
    afxV3d      nrm;
    afxV2d      uv;
    afxUnit32    col;
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

AUX void        AuxCmdBeginImm2d(afxDrawContext dctx);
AUX void        AuxCmdEndImm2d(afxDrawContext dctx);
AUX void        AuxCmdTransformImmVertices(afxDrawContext dctx, afxM4d const m, afxUnit cnt, avxImmVertex vertices[]);

AUX void        AuxImmSetColor(afxDrawContext dctx, avxColor const inner, avxColor const outer);
AUX void        AuxImmBindRasters();
AUX void        AuxImmBindRasterizer(afxDrawContext dctx, avxRasterizer razr);

/// The BeginPath() function opens a path bracket in the specified device context.
AUX afxBool     AuxBeginPath(avxVectorizer vecr);

/// The EndPath() function closes a path bracket and selects the path defined by the bracket into the specified device context.
AUX afxBool     AuxEndPath(avxVectorizer vecr);

/// The FillPath() function closes any open figures in the current path and fills the path's interior by using the current brush and polygon-filling mode.
AUX afxBool     AuxFillPath(avxVectorizer vecr);

/// The LineTo() function draws a line from the current position up to, but not including, the specified point.
AUX afxBool     AuxLineTo(avxVectorizer vecr, afxV2d const pos);

/// The MoveTo() function updates the current position to the specified point and optionally returns the previous position.
AUX afxBool     AuxMoveTo(avxVectorizer vecr, afxV2d const pos, afxV2d prev);

/// The AngleArc() function draws a line segment and an arc. The line segment is drawn from the current position to the beginning of the arc. The arc is drawn along the perimeter of a circle with the given radius and center. The length of the arc is defined by the given start and sweep angles.
AUX afxBool     AuxAngleArc(avxVectorizer vecr, afxV2d const pos, afxReal r, afxReal startAngle, afxReal sweepAngle);




AUX void AuxGraphRoundedRect(avxGraph* gra, afxV2d pos, afxV2d extent, afxReal cornerRadius);
AUX void AuxGraphSetFillColor(avxGraph* gra, avxColor const cor);

AUX void AuxGraphRect(avxGraph* gra, afxV2d pos, afxV2d extent);
AUX afxUnit AuxGraphBoxGradient(avxGraph* gra, afxV2d pos, afxV2d extent, afxReal radiusunk, afxReal unk, avxColor src, avxColor dst);

AUX void AuxGraphWinding(avxGraph* gra, afxBool hole);

AUX void AuxGraphFillPaint(avxGraph* gra, afxUnit paintId);

AUX afxUnit AuxGraphLinearGradient(avxGraph* gra, afxV2d pos, afxV2d extent, avxColor src, avxColor dst);

AUX void AuxGraphStrokeColor(avxGraph* gra, avxColor cor);
AUX void AuxGraphStroke(avxGraph* gra);

AUX void AuxGraphFontSize(avxGraph* gra, afxReal siz);
AUX void AuxGraphFontFace(avxGraph* gra, afxString const* name);
AUX void AuxGraphTextAlign(avxGraph* gra, afxFlags mask);

AUX void AuxGraphFontBlur(avxGraph* gra, afxReal inten);
AUX void AuxGraphText(avxGraph* gra, afxV2d pos, afxReal length, afxString const* name);

#endif//AUX_PAINT_H
