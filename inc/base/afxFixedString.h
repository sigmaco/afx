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

#include "qwadro/inc/base/afxString.h"

AFX_DEFINE_STRUCT(afxString4)
{
    afxString     str;
    afxChar         buf[4];
};

AFX_DEFINE_STRUCT(afxString8)
{
    afxString     str;
    afxChar         buf[8];
};

AFX_DEFINE_STRUCT(afxString16)
{
    afxString     str;
    afxChar         buf[16];
};

AFX_DEFINE_STRUCT(afxString32)
{
    afxString     str;
    afxChar         buf[32];
};

AFX_DEFINE_STRUCT(afxString128)
{
    afxString     str;
    afxChar         buf[128];
};

AFX_DEFINE_STRUCT(afxString2048)
{
    afxString     str;
    afxChar         buf[2048];
};

AFX_DEFINE_STRUCT(afxString4096)
{
    afxString     str;
    afxChar         buf[4096];
};

AFX afxString*  AfxMakeString4(afxString4 *str, afxString const* src);
AFX afxString*  AfxMakeString8(afxString8 *str, afxString const* src);
AFX afxString*  AfxMakeString16(afxString16 *str, afxString const* src);
AFX afxString*  AfxMakeString32(afxString32 *str, afxString const* src);
AFX afxString*  AfxMakeString128(afxString128 *str, afxString const* src);
AFX afxString*  AfxMakeString2048(afxString2048 *str, afxString const* src);
AFX afxString*  AfxMakeString4096(afxString4096 *str, afxString const* src);

#endif//AFX_FIXED_STRING_H
