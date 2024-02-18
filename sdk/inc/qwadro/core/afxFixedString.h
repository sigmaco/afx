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
 *                       (c) 2017 SIGMA, Engineering In Technology
 *                             <https://sigmaco.org/qwadro/>
 */

#ifndef AFX_FIXED_STRING_H
#define AFX_FIXED_STRING_H

#include "qwadro/core/afxRestring.h"

AFX_DEFINE_STRUCT(afxFixedString4)
{
    afxRestring   str;
    afxChar             buf[4];
};

AFX_DEFINE_STRUCT(afxFixedString8)
{
    afxRestring   str;
    afxChar             buf[8];
};

AFX_DEFINE_STRUCT(afxFixedString16)
{
    afxRestring   str;
    afxChar             buf[16];
};

AFX_DEFINE_STRUCT(afxFixedString32)
{
    afxRestring   str;
    afxChar             buf[32];
};

AFX_DEFINE_STRUCT(afxFixedString128)
{
    afxRestring   str;
    afxChar             buf[128];
};

AFX_DEFINE_STRUCT(afxFixedString2048)
{
    afxRestring   str;
    afxChar             buf[2048];
};

AFX_DEFINE_STRUCT(afxFixedString4096)
{
    afxRestring   str;
    afxChar             buf[4096];
};

AFX afxRestring*  AfxMakeFixedString4(afxFixedString4 *str, afxString const* src);
AFX afxRestring*  AfxMakeFixedString8(afxFixedString8 *str, afxString const* src);
AFX afxRestring*  AfxMakeFixedString16(afxFixedString16 *str, afxString const* src);
AFX afxRestring*  AfxMakeFixedString32(afxFixedString32 *str, afxString const* src);
AFX afxRestring*  AfxMakeFixedString128(afxFixedString128 *str, afxString const* src);
AFX afxRestring*  AfxMakeFixedString2048(afxFixedString2048 *str, afxString const* src);
AFX afxRestring*  AfxMakeFixedString4096(afxFixedString4096 *str, afxString const* src);

#endif//AFX_FIXED_STRING_H
