/*
 *           ::::::::    :::::::::::    ::::::::    ::::     ::::       :::
 *          :+:    :+:       :+:       :+:    :+:   +:+:+: :+:+:+     :+: :+:
 *          +:+              +:+       +:+          +:+ +:+:+ +:+    +:+   +:+
 *          +#++:++#++       +#+       :#:          +#+  +:+  +#+   +#++:++#++:
 *                 +#+       +#+       +#+   +#+#   +#+       +#+   +#+     +#+
 *          #+#    #+#       #+#       #+#    #+#   #+#       #+#   #+#     #+#
 *           ########    ###########    ########    ###       ###   ###     ###
 *
 *                     S I G M A   T E C H N O L O G Y   G R O U P
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

#include "../exec/afxSystemDDK.h"

_AFX afxString const qwadroSignature = AFX_STRING(
    "      ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::      \n"
    "     :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:     \n"
    "     +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+     \n"
    "     +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+     \n"
    "     +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+     \n"
    "     #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#     \n"
    "      ###### ###  ###   ###   ###     ### #########  ###    ###  ########      \n"
    "                                                                               \n"
    "              Q W A D R O   E X E C U T I O N   E C O S Y S T E M              \n"
    "                                                                               \n"
    "                               Public Test Build                               \n"
    "                           (c) 2017 SIGMA FEDERATION                           \n"
    "                                www.sigmaco.org                                \n"
    "                                                                               \n"
);

_AFX afxString const sigmaSignature = AFX_STRING(
    "       ::::::::    :::::::::::    ::::::::    ::::     ::::       :::          \n"
    "      :+:    :+:       :+:       :+:    :+:   +:+:+: :+:+:+     :+: :+:        \n"
    "      +:+              +:+       +:+          +:+ +:+:+ +:+    +:+   +:+       \n"
    "      +#++:++#++       +#+       :#:          +#+  +:+  +#+   +#++:++#++:      \n"
    "             +#+       +#+       +#+   +#+#   +#+       +#+   +#+     +#+      \n"
    "      #+#    #+#       #+#       #+#    #+#   #+#       #+#   #+#     #+#      \n"
    "       ########    ###########    ########    ###       ###   ###     ###      \n"
    "                                                                               \n"
    "              Q W A D R O   E X E C U T I O N   E C O S Y S T E M              \n"
    "                                                                               \n"
    "                               Public Test Build                               \n"
    "                           (c) 2017 SIGMA FEDERATION                           \n"
    "                                www.sigmaco.org                                \n"
    "                                                                               \n"
);

_AFX afxBool AfxTestSystemCompatibility(afxUnit verMajor, afxUnit verMinor, afxUnit verPatch)
{
    afxBool rslt = (verMajor == AFX_QWADRO_VER_MAJOR && verMinor == AFX_QWADRO_VER_MINOR && (verPatch == AFX_QWADRO_VER_PATCH));

    if (!rslt)
    {
        AfxReportWarn("ABI version (%d.%d.%d) doesn't match API version (%d.%d.%d).", AFX_QWADRO_VER_MAJOR, AFX_QWADRO_VER_MINOR, AFX_QWADRO_VER_PATCH, verMajor, verMinor, verPatch);
    }
    return 0;
}

_AFX void AfxGetSystemVersion(afxUnit* verMajor, afxUnit* verMinor, afxUnit* verPatch)
{
    *verMajor = AFX_QWADRO_VER_MAJOR;
    *verMinor = AFX_QWADRO_VER_MINOR;
    *verPatch = AFX_QWADRO_VER_PATCH;
}

_AFX afxString const* AfxGetSystemVersionString(void)
{
    static afxString const str = AFX_STRING(AFX_STRINGIFY(AFX_QWADRO_VER_MAJOR.AFX_QWADRO_VER_MINOR.AFX_QWADRO_VER_PATCH));
    return &str;
}
