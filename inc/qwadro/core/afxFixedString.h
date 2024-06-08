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

#ifndef AFX_FIXED_STRING_H
#define AFX_FIXED_STRING_H

#include "qwadro/core/afxRestring.h"

AFX_DEFINE_STRUCT(afxString4)
{
    afxRestring     str;
    afxChar         buf[4];
};

AFX_DEFINE_STRUCT(afxString8)
{
    afxRestring     str;
    afxChar         buf[8];
};

AFX_DEFINE_STRUCT(afxString16)
{
    afxRestring     str;
    afxChar         buf[16];
};

AFX_DEFINE_STRUCT(afxString32)
{
    afxRestring     str;
    afxChar         buf[32];
};

AFX_DEFINE_STRUCT(afxString128)
{
    afxRestring     str;
    afxChar         buf[128];
};

AFX_DEFINE_STRUCT(afxString2048)
{
    afxRestring     str;
    afxChar         buf[2048];
};

AFX_DEFINE_STRUCT(afxString4096)
{
    afxRestring     str;
    afxChar         buf[4096];
};

AFX afxRestring*  AfxMakeString4(afxString4 *str, afxString const* src);
AFX afxRestring*  AfxMakeString8(afxString8 *str, afxString const* src);
AFX afxRestring*  AfxMakeString16(afxString16 *str, afxString const* src);
AFX afxRestring*  AfxMakeString32(afxString32 *str, afxString const* src);
AFX afxRestring*  AfxMakeString128(afxString128 *str, afxString const* src);
AFX afxRestring*  AfxMakeString2048(afxString2048 *str, afxString const* src);
AFX afxRestring*  AfxMakeString4096(afxString4096 *str, afxString const* src);

#endif//AFX_FIXED_STRING_H
