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
 // The Unified Shell                                                        //
//////////////////////////////////////////////////////////////////////////////

/// The Unified I/O System, also called Unified Shell, is a full integration between Qwadro systems to improve the user experience and unify it with all the applications

#ifndef AUX_SHELL_H
#define AUX_SHELL_H

#include "qwadro/ux/afxUxDefs.h"
// ux
#include "qwadro/hid/afxHid.h"
#include "qwadro/hid/afxGamepad.h"
#include "qwadro/ux/afxWindow.h"
#include "qwadro/ux/afxWidget.h"

AFX_DEFINE_STRUCT(afxFileDrop)
{
    afxUnit  x, y;
    afxArray files; // afxChar const*
};

AFX_DEFINE_STRUCT(afxShellFeatures)
{
    // The shell supports background drawing.
    afxBool8 drawableBg;
    // The shell supports windowing.
    afxBool8 windowing;
    // The shell supports window positioning.
    afxBool8 placement;
    // The shell supports window decorations.
    afxBool decor;
    // The shell supports widgets.
    afxBool8 widgets;
    // The shell supports themes.
    afxBool8 themes;
    // The shell supports fonts.
    afxBool8 fonts;
    // The shell supports device fonts.
    afxBool8 devFonts;
    // The shell supports clipboard.
    afxBool8 clipboard;
    // The shell supports multiple clipboards.
    afxBool8 multiClipboards;
    // The shell supports VR sessions.
    afxBool vr;
};

AFX_DEFINE_STRUCT(afxShellLimits)
{
    afxUnit maxWndCnt;
    afxUnit maxWidCnt;
    afxUnit maxFntCnt;
    afxUnit maxThemCnt;
    afxUnit numClipboards;
    afxUnit maxSeats;
};

AFX_DEFINE_STRUCT(afxShellInfo)
{
    afxString32     urn;
    afxString128    name;
    afxDeviceType   type;
    afxAcceleration accel;
    afxDeviceStatus status;
    afxUnit         hwVndId;
    afxUnit         hwVndDevId;
    afxUnit32       swVndId;
    afxUnit32       swVndDrvId;
    afxUnit         drvVer;
    afxUnit         apiVer;
};

  //////////////////////////////////////////////////////////////////////////////
 // SHELL HANDLING                                                           //
//////////////////////////////////////////////////////////////////////////////

AUX afxError AfxDescribeShell
(
    afxShell ssh, 
    afxShellInfo* desc
);

AUX afxBool AfxIsShellPrompt
(
    afxShell ssh
);

AUX void AfxQueryShellFeatures
(
    // A handle for a shell to query for its features.
    afxShell ssh,

    // A pointer to a afxShellFeatures structure where the results will be stored.
    afxShellFeatures* features
);

AUX void AfxQueryShellLimits
(
    // The shell to query for its limits.
    afxShell ssh,

    // A afxShellLimits structure where the shell's limits will be stored.
    afxShellLimits* limits
);

AUX afxBool AfxIsShellAcceptable
(
    // The shell you are considering for use in your application.
    afxShell ssh,

    // A afxShellFeatures structure containing the required features that the shell must support.
    afxShellFeatures const* features,

    // A afxShellLimits structure containing the hardware limits that the shell should meet or exceed.
    afxShellLimits const* limits
);

  //////////////////////////////////////////////////////////////////////////////
 // IMPLEMENTATION DISCOVERY //////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

AUX afxUnit AfxEnumerateShells
(
    // The ordinal identifier for the installable client driver (ICD).
    afxUnit icd,

    // The number ordinal for the first shell to start the iteration from.
    afxUnit first,

    // The maximum number of shells to be retrieved.
    afxUnit cnt,

    // An array that will be populated with the shells found during the enumeration.
    afxShell shells[]
);

AUX afxUnit AfxInvokeShells
(
    // The ordinal identifier for the installable client driver (ICD).
    afxUnit icd,

    // The ordinal number for the first shell to start the iteration from.
    afxUnit first,

    // A pointer to user-defined data that will be passed to the callback function for each shell.
    void* udd,

    // The callback function that will be invoked for each shell during enumeration.
    afxBool(*f)(void*, afxShell),

    // The maximum number of shells to be considered during the invocation.
    afxUnit cnt
);

AUX afxUnit AfxEvokeShells
(
    // The ordinal identifier for the installable client driver (ICD).
    afxUnit icd,

    // The ordinal number for the first shell to start the iteration from.
    afxUnit first,

    // A pointer to user-defined data that will be passed to the callback function for each shell.
    void* udd,

    // The callback function that will be invoked for each shell during enumeration.
    afxBool(*f)(void*, afxShell),

    // The maximum number of shells to be considered during the invocation.
    afxUnit cnt,

    // An array that will be populated with the shells found during the enumeration process.
    afxShell shells[]
);

AUX afxUnit AfxChooseShells
(
    // The ordinal identifier for the installable client driver (ICD).
    afxUnit icd,

    // A structure that specifies the features that the shells must support.
    afxShellFeatures const* features,

    // A structure that defines the limits that the shells should meet.
    afxShellLimits const* limits,

    // The operations or features that the shell must supports.
    amxPortInfo const* caps,

    // The maximum number of shells to be selected and returned in the @sshIds array.
    afxUnit maxCnt,

    // An array that will be populated with the device IDs of the selected shells.
    afxUnit sshIds[]
);

#endif//AUX_SHELL_H
