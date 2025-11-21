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

#include "qwadro/base/afxString.h"

#define afxFixedString(varName_,capacity_) \
    afxChar varName_##Buf[capacity_]; afxString varName_ = { .len = 0, .cap = sizeof(varName_##Buf), .start = varName_##Buf }; \

AFX_DEFINE_STRUCT(afxString4)
{
    afxString   s;
    afxChar     buf[4];
};

AFX_DEFINE_STRUCT(afxString8)
{
    afxString   s;
    afxChar     buf[8];
};

AFX_DEFINE_STRUCT(afxString16)
{
    afxString   s;
    afxChar     buf[16];
};

AFX_DEFINE_STRUCT(afxString32)
{
    afxString   s;
    afxChar     buf[32];
};

AFX_DEFINE_STRUCT(afxString64)
{
    afxString   s;
    afxChar     buf[64];
};

AFX_DEFINE_STRUCT(afxString128)
{
    afxString   s;
    afxChar     buf[128];
};

AFX_DEFINE_STRUCT(afxString256)
{
    afxString   s;
    afxChar     buf[256];
};

AFX_DEFINE_STRUCT(afxString512)
{
    afxString   s;
    afxChar     buf[512];
};

AFX_DEFINE_STRUCT(afxString1024)
{
    afxString   s;
    afxChar     buf[1024];
};

AFX_DEFINE_STRUCT(afxString2048)
{
    afxString   s;
    afxChar     buf[2048];
};

AFX_DEFINE_STRUCT(afxString4096)
{
    afxString   s;
    afxChar     buf[4096];
};

AFX afxString*  AfxMakeString4(afxString4 *s, afxString const* src);
AFX afxString*  AfxMakeString8(afxString8 *s, afxString const* src);
AFX afxString*  AfxMakeString16(afxString16 *s, afxString const* src);
AFX afxString*  AfxMakeString32(afxString32 *s, afxString const* src);
AFX afxString*  AfxMakeString64(afxString64 *s, afxString const* src);
AFX afxString*  AfxMakeString128(afxString128 *s, afxString const* src);
AFX afxString*  AfxMakeString256(afxString256 *s, afxString const* src);
AFX afxString*  AfxMakeString512(afxString512 *s, afxString const* src);
AFX afxString*  AfxMakeString1024(afxString1024 *s, afxString const* src);
AFX afxString*  AfxMakeString2048(afxString2048 *s, afxString const* src);
AFX afxString*  AfxMakeString4096(afxString4096 *s, afxString const* src);

#endif//AFX_FIXED_STRING_H
