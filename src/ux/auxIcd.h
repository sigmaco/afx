/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                      Q W A D R O   U S E R   I / O   S Y S T E M
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This software is part of Advanced User Experiences Extensions & Experiments.

  //////////////////////////////////////////////////////////////////////////////
 // Advanced User Experience                                                 //
//////////////////////////////////////////////////////////////////////////////

#ifndef AUX_IMPLEMENTATION_H
#define AUX_IMPLEMENTATION_H

#include "qwadro/ux/afxShell.h"
#include "auxEnvironmentImpl.h"
#include "../hid/auxHidImpl.h"
#include "auxFontImpl.h"
#include "auxWidgetImpl.h"
#include "auxWindowImpl.h"
#include "auxShellImpl.h"
#include "../hid/auxDisplayImpl.h"

AFX_DEFINE_STRUCT(_afxShellImpl)
{
    afxClassConfig vduCls;
    afxClassConfig sesCls;
    afxError(*getInteropDoutCls)(afxDrawSystem dsys, afxString const* tool, afxClassConfig* cfg);
    afxError(*getInteropSinkCls)(afxMixSystem msys, afxString const* tool, afxClassConfig* cfg);
};

AUX afxClassConfig const _AUX_SSH_CLASS_CONFIG;

AUX afxError _AuxGetInteropDisplayClass(afxUnit icd, afxString const* tool, afxClassConfig* clsc);
AUX afxError _AuxGetInteropSurfaceClass(afxDrawSystem dsys, afxString const* tool, afxClassConfig* clsc);
AUX afxError _AuxGetInteropSinkClass(afxMixSystem msys, afxString const* tool, afxClassConfig* clsc);

AUX afxClass const* _AuxGetHidClass(afxModule icd);
AUX afxClass const* _AuxGetShellClass(afxModule icd);
AVX afxClass const* _AvxGetDisplayClass(afxModule icd);


AUX afxError _AuxRegisterHids(afxModule icd, afxUnit cnt, afxHidInfo const infos[], afxHid hids[]);
AUX afxError _AuxRegisterShells(afxModule icd, afxUnit cnt, afxShellInfo const infos[], afxShell shells[]);

AUX afxError _AuxImplementShell(afxModule icd, _afxShellImpl const* cfg);

AUX afxBool _AuxGetIcd(afxUnit icdIdx, afxModule* driver);

#endif//AUX_IMPLEMENTATION_H
