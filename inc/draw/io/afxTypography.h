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
    afxNat  origin[2];
    afxNat  bearing[2]; // offset X/Y from baseline (origin). Ex.: originX + bearingX + width
    afxNat  extent[2]; // char area
    afxNat  advanceX; // size for horizontal advance after input.
    //afxNat xMin; // = originX + bearingX
    //afxNat xMax; // = originX + bearingX + width
    //afxNat yMin; // = originY + bearingY
    //afxNat yMax; // = originY + bearingY + height
};

#ifdef _AVX_DRAW_C
#ifdef _AFX_TYPOGRAPHY_C
AFX_OBJECT(afxTypography)
{
    afxString   id;
    afxNat      charWidht;
    afxNat      baseGlyphIdx; // usually used to skip first non-drawable (control chars)
    afxNat      glyphCnt;
    afxGlyph*   glyphes;
    afxNat      pageCnt;
    struct
    {
        afxNat  baseGlyphIdx;
        afxNat  glyphCnt;
    }*pages;
};
#endif//_AFX_TYPOGRAPHY_C
#endif//_AVX_DRAW_C

AVX afxTypography   AfxAcquireTypography(afxDrawContext dctx);

AVX afxBool         AfxGetGlyph(afxTypography typ, afxNat codepoint, afxGlyph* glyph);

#endif//AFX_TYPOGRAPHY_H
