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

// This software is part of Advanced User Experiences Extensions & Experiments.

#ifndef AUX_FONT_H
#define AUX_FONT_H

#include "qwadro/draw/afxDrawDefs.h"
#include "qwadro/draw/avxColor.h"
#include "qwadro/ux/afxUxDefs.h"

AFX_DEFINE_STRUCT(afxFontConfig)
{
    afxString       face;
    afxUnit         ascent;
    afxUnit         descent;
    afxUnit         lineGap;
    afxRect         bounds;
    
    afxDrawSystem   dsys;
    afxBool         isDevFont;
    afxUnit         typeBufCap;
};

AUX afxError AfxAcquireFonts
(
    afxUnit             cnt, 
    afxFontConfig const cfg[], 
    afxFont             fonts[]
);

////////////////////////////////////////////////////////////////////////////////

AFX afxError AfxTranscribe(afxFont fnt, afxRect const* area, afxUnit col, afxUnit row, afxString const* text);

AFX afxError AfxFlushDeviceFont(afxFont fnt, afxDrawContext dctx, avxViewport const* vp);

#endif//AUX_FONT_H
