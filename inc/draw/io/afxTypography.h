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

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#ifndef AFX_TYPOGRAPHY_H
#define AFX_TYPOGRAPHY_H

#include "qwadro/inc/draw/afxDrawDefs.h"

AFX_DEFINE_STRUCT(afxGlyph)
{
    afxUnit  origin[2];
    afxUnit  bearing[2]; // offset X/Y from baseline (origin). Ex.: originX + bearingX + width
    afxUnit  extent[2]; // char area
    afxUnit  advanceX; // size for horizontal advance after input.
    //afxUnit xMin; // = originX + bearingX
    //afxUnit xMax; // = originX + bearingX + width
    //afxUnit yMin; // = originY + bearingY
    //afxUnit yMax; // = originY + bearingY + height
};

#ifdef _AVX_DRAW_C
#ifdef _AFX_TYPOGRAPHY_C
AFX_OBJECT(afxTypography)
{
    afxString   id;
    afxUnit      charWidht;
    afxUnit      baseGlyphIdx; // usually used to skip first non-drawable (control chars)
    afxUnit      glyphCnt;
    afxGlyph*   glyphes;
    afxUnit      pageCnt;
    struct
    {
        afxUnit  baseGlyphIdx;
        afxUnit  glyphCnt;
    }*pages;
};
#endif//_AFX_TYPOGRAPHY_C
#endif//_AVX_DRAW_C

AVX afxTypography   AfxAcquireTypography(afxDrawContext dctx);

AVX afxBool         AfxGetGlyph(afxTypography typ, afxUnit codepoint, afxGlyph* glyph);

#endif//AFX_TYPOGRAPHY_H
