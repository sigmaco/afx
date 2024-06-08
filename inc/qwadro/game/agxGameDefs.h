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

#ifndef AFX_GAME_DEFS_H
#define AFX_GAME_DEFS_H

#include "../math/afxMathDefs.h"


#ifndef /*__e2gamee__*/__e2coree__
#   ifdef _DEBUG
#       define AGX DLLIMPORT extern 
#       define AGXINL DLLIMPORT extern inline
#   else
#       define AGX DLLIMPORT extern 
#       define AGXINL DLLIMPORT extern inline
#   endif
#else
#   ifdef _DEBUG
#       define _AGX DLLEXPORT
#       define AGX DLLEXPORT extern 
#       define _AGXINL DLLEXPORT inline
#       define AGXINL DLLEXPORT extern inline
#   else
#       define _AGX DLLEXPORT
#       define AGX DLLEXPORT extern 
#       define _AGXINL DLLEXPORT inline
#       define AGXINL DLLEXPORT extern inline
#   endif
#endif//__e2gamee__


AFX_DEFINE_HANDLE(agxGameObject);

#endif//AFX_GAME_DEFS_H
