/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *         Q W A D R O   M U L T I M E D I A   U X   I N F R A S T R U C T U R E
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This software is part of Advanced User Experiences Extensions & Experiments.

  //////////////////////////////////////////////////////////////////////////////
 // Advanced User Experience                                                 //
//////////////////////////////////////////////////////////////////////////////

#ifndef AUX_FONT_DDK_H
#define AUX_FONT_DDK_H

#include "../exec/afxSystemDDK.h"
#include "qwadro/ux/afxUxDefs.h"
#include "qwadro/draw/avxViewport.h"
#include "qwadro/ux/afxShell.h"
//#include "qwadro/../../dep_/vgl1/vgl1.h"

AFX_DEFINE_STRUCT(_auxFontPage)
{

};

AFX_DEFINE_STRUCT(_auxFontImage)
{
    afxReal     height;
    afxReal     ascent;
    afxReal     descent;
    afxUnit     baseGlyph;
    afxUnit     glyphCnt;
    avxRaster   ras;
};

AFX_DECLARE_STRUCT(_auxIddFnt);
AFX_DECLARE_STRUCT(_auxDdiFnt);

#ifdef _AUX_FONT_IMPL
AFX_OBJECT(_auxFont)
#else
AFX_OBJECT(afxFont)
#endif
{
    _auxIddFnt*         idd;
    _auxDdiFnt const*   ddi;
    // User-defined data.
    void*               udd;
    // Debugging tag.
    afxString           tag;

    afxUnit         pagCnt;
    _auxFontPage*   pages;

    afxDrawSystem   dsys;
    afxBool         fntEnabled;
    avxPipeline     fntPip;
    avxSampler      fntSamp;
    avxBuffer       fntVbo;
    afxByte*        fntVboPtr;
    avxBufferedRing fntVboRng;
    afxUnit         fntStoreLen;
};

AUX afxClassConfig const _AUX_FNT_CLASS_CONFIG;

#endif//AUX_FONT_DDK_H
