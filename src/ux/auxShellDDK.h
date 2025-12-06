/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                  Q W A D R O   M M U X   I N F R A S T R U C T U R E         
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This software is part of Advanced User Experiences Extensions & Experiments.

  //////////////////////////////////////////////////////////////////////////////
 // Advanced User Experience                                                 //
//////////////////////////////////////////////////////////////////////////////

#ifndef AUX_SHELL_DDK_H
#define AUX_SHELL_DDK_H

#include "../exec/afxSystemDDK.h"
#include "qwadro/ux/afxUxDefs.h"
#include "qwadro/draw/avxViewport.h"
#include "qwadro/ux/afxShell.h"
//#include "qwadro/../../dep_/vgl1/vgl1.h"

AFX_DEFINE_STRUCT(_auxShellAcq)
{
    afxDeviceInfo           dev;
    afxClassConfig const*   kbdClsCfg;
    afxClassConfig const*   mseClsCfg;
    afxClassConfig const*   padClsCfg;
    afxClassConfig const*   sesClsCfg;
    afxClassConfig const*   widClsCfg;
    afxClassConfig const*   wndClsCfg;
    afxClassConfig const*   xssClsCfg;
};

#ifdef _AUX_SHELL_C
#ifndef _AFX_DEVICE_C
#   error "Require afxDevice implementation"
#endif
AFX_OBJECT(afxShell)
{
    AFX_OBJ(afxDevice)  dev;
    afxClass            kbdCls;
    afxClass            padCls;
    afxClass            sesCls;

    afxClassConfig      wndClsCfg;
    afxClassConfig      xssClsCfg;

};
#endif//_AUX_SHELL_C

AUX afxClassConfig const _AUX_THEM_CLASS_CONFIG;

AUX afxClass const* AfxGetKeyboardClass(afxShell ssh);
AUX afxClass const* AfxGetControllerClass(afxShell ssh);
AUX afxClass const* _AuxIcdGetEnvClass(afxModule icd);

#endif//AUX_SHELL_DDK_H
