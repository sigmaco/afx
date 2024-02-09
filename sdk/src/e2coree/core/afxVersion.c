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

#include "qwadro/core/afxVersion.h"
#include "qwadro/core/afxDebug.h"

_AFX afxBool AfxVersionsMatch_(afxInt verMajor, afxInt verMinor, afxInt customization, afxInt buildNum)
{
    afxBool rslt = (verMajor == AFX_QWADRO_VER_MAJOR && verMinor == AFX_QWADRO_VER_MINOR && (customization == AFX_QWADRO_CUSTOMIZATION) && buildNum == AFX_QWADRO_BUILD_NUM);

    if (!rslt)
    {
        AfxLogAdvertence(AfxHint(), "ABI version (%d.%d.%d.%d) doesn't match API version (%d.%d.%d.%d).", AFX_QWADRO_VER_MAJOR, AFX_QWADRO_VER_MINOR, AFX_QWADRO_CUSTOMIZATION, AFX_QWADRO_BUILD_NUM, verMajor, verMinor, customization, buildNum);
    }
    return 0;
}

_AFX void AfxGetVersion(afxInt *verMajor, afxInt *verMinor, afxInt *customization, afxInt *buildNum)
{
    *verMajor = AFX_QWADRO_VER_MAJOR;
    *verMinor = AFX_QWADRO_VER_MINOR;
    *customization = AFX_QWADRO_CUSTOMIZATION;
    *buildNum = AFX_QWADRO_BUILD_NUM;
}

_AFX afxString const* AfxGetVersionString(void)
{
    static afxString const str = AFX_STRING(AFX_QWADRO_VER_STRING);
    return &str;
}
