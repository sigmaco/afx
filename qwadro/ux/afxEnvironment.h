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

// This software is part of Advanced MMUX Extensions & Experiments.

  //////////////////////////////////////////////////////////////////////////////
 // Advanced User Experience                                                 //
//////////////////////////////////////////////////////////////////////////////

#ifndef AUX_ENVIRONMENT_H
#define AUX_ENVIRONMENT_H

#include "qwadro/ux/afxUxDefs.h"
#include "qwadro/draw/avxViewport.h"
#include "qwadro/hid/afxKeyboard.h"
#include "qwadro/ux/afxShell.h"

#define AFX_MAX_USERS_PER_ENVIRONMENT (2)

/*
    A hypervisor, also known as a virtual machine monitor or virtualizer, is a type of computer software, firmware or hardware that 
    creates and runs virtual machines. A computer on which a hypervisor runs one or more virtual machines is called a host machine 
    or virtualization server, and each virtual machine is called a guest machine. The hypervisor presents the guest operating systems 
    with a virtual operating platform and manages the execution of the guest operating systems.

    A virtual desktop refers to a desktop OS, such as Windows 10, that runs on top of an enterprise hypervisor. Users employ a virtual 
    desktop infrastructure (VDI) to access virtual desktops through thin clients. A remote desktop protocol transmits screen images and 
    keyboard and.

    When people talk about "desktop environments", they often mix them up with several related but distinct layers of the graphical stack.
    A desktop environment is the full, integrated graphical user experience.
    A desktop environment usually includes:
     - A window manager
     - A panel/taskbar
     - A launcher/menu
     - A settings center
     - File manager
     - Theme system
     - Notification service
     - Background services (like portals, power manager…)

    It's the high-level layer.

    A window manager controls window placement, focus, tiling, decorations, and resize behavior.
    A DE includes a WM, but you can also run a WM alone without a DE.

    The display server (or display protocol layer) is the backend that speaks to your hardware and coordinates screens and input.
    On Wayland, the compositor is also the display server.
    A compositor draws windows into an off-screen buffer and blends them onto the screen.
    On Wayland, the compositor = the display server = usually the WM too.

    Graphical toolkits provide GUI widgets (buttons, menus) and applications use them.
    A desktop environment normally standardizes on one (GNOME uses GTK, KDE uses Qt).

    Some environments are "shells" but not full desktop environments. It is also refered to as application environment (or shell layers).
    These sit between the WM and the full desktop environment concept.

    Login/session environments are responsible for user login and starting the graphical session.
    These choose which desktop environment or WM to start.

    You can run environments in different session modes (or session types).
    Wayland session, X11 session, Mir session (historical Ubuntu), fbdev/direct-to-framebuffer (no WM) to name a few of them.
*/

typedef enum afxEnvironmentType
{
    // The environment is a GUI that represents a workspace on a computer screen, 
    // allowing users to interact with files, applications, and system functions through visual elements like icons and windows. 
    // It simplifies navigation and task management, making it easier for users to access and organize their digital resources.
    afxEnvironmentType_DESKTOP,
    // The environment is a computer assuming to have exclusive control over the display.
    afxEnvironmentType_TERMINAL,
    // The environment is a head-mounted device with visor offering or assisting virtual or augmented reality.
    afxEnvironmentType_VR,
    afxEnvironmentType_AR,
} afxEnvironmentType;

typedef enum afxSessionState
{
    afxSessionState_NONE,
    afxSessionState_CONNECTING,
    afxSessionState_SOCKET_CONNECTED,
    afxSessionState_BANNER_RECEIVED,
    afxSessionState_INITIAL_KEX,
    afxSessionState_KEXINIT_RECEIVED,
    afxSessionState_DH,
    afxSessionState_AUTHENTICATING,
    afxSessionState_AUTHENTICATED,
    afxSessionState_ERROR,
    afxSessionState_DISCONNECTED
} afxSessionState;

typedef enum afxAuthMethod
{
    afxAuthMethod_NONE, // Try to authenticate through the "none" method.
    afxAuthMethod_PASSWORD, // Try to authenticate by password.
    afxAuthMethod_PRIVKEY, // Authenticate with public/private key or certificate.
    afxAuthMethod_PASSPHRASE, // Tries to automatically authenticate with public key and "none".
    afxAuthMethod_PUBKEY, // Try to authenticate with the given public key.

    afxAuthMethod_TOTAL
} afxAuthMethod;

typedef enum afxSeatCaps
{
    afxSeatCap_KEYBOARD,
    afxSeatCap_MOUSE,
    afxSeatCap_GAMEPAD,
} afxSeatCaps;

AFX_DEFINE_STRUCT(afxSeatConfig)
{
    afxString           u;
    afxFlags            flags;
    afxUnit             portIdx;
    afxUnit             queueCnt;
    afxReal const*      queuePriority;

    // SSH
    // Note from libssh
    // Most server implementations do not permit changing the username during authentication.
    // The username should only be set with ssh_options_set() only before you connect to the server.
    afxUri          host;
    afxAuthMethod   method;
    afxString       username;
    afxString       credential;
};

AFX_DEFINE_STRUCT(afxEnvironmentConfig)
// The system-wide settings and parameters.
{
    afxUnit             verMajor;
    afxUnit             verMinor;
    // The friendly name for this environment.
    afxString           name;
    // The features to be enabled for this environment.
    afxShellFeatures    features;
    // The number of environment extensions to be enabled.
    afxUnit             extCnt;
    // An array of Qwadro strings containing the names of extensions to enable for the environment.
    afxString const*    exts;
    // The draw system attached to the environment.
    afxDrawSystem       dsys;
    // The mix system attached to the environment.
    afxMixSystem        msys;
    // The number of seats.
    afxUnit             seatCnt;
    // An array of configurations for each seat.
    afxSeatConfig       seats[AFX_MAX_USERS_PER_ENVIRONMENT];
    // User-defined data attached to the environment.
    void*               udd;
    // Debugging string attached to the environment.
    afxString           tag;

    afxUnit             ddevId;
    afxUnit             vduIdx;
    afxUnit             sdevId;
    afxUnit             soutIdx;
};

AUX afxError AfxConfigureEnvironment
(
    afxUnit icd, 
    afxEnvironmentConfig const* cfg
);

AUX afxError AfxAcquireEnvironment
(
    afxUnit icd,
    afxEnvironmentConfig const* cfg, 
    afxEnvironment* environment
);

AUX afxBool AfxGetEnvironment
(
    afxEnvironment* environment
);

AUX afxUnit AfxGetEnvironmentId
(
    // can be NIL
    afxEnvironment env
);

////////////////////////////////////////////////////////////////////////////////

AUX afxBool         AfxGetEnvironmentVideo(afxDrawSystem* system);

AUX afxBool         AfxGetEnvironmentAudio(afxMixSystem* system, afxSink* sink);

AUX afxError        AfxBeginFrame(afxEnvironment env);
AUX afxError        AfxWaitFrame(afxEnvironment env);
AUX afxError        AfxEndFrame(afxEnvironment env);

AUX afxError        AfxReconnectKeyboard(afxEnvironment env, afxUnit portIdx, afxUnit hidNo);

AUX afxTime         AfxDoUx(afxFlags flags, afxUnit64 timeout);

AUX afxError        AfxTakeFullscreen(afxWindow wnd, afxBool fullscreen);

AUX afxBool         AfxGetCursorPlacement(afxUnit seat, afxRect* rc, afxWindow wnd, afxRect* onFrame, afxRect* onSurface);

AUX afxBool         AfxHasClipboardContent(afxUnit seat, afxUnit slot, afxFlags flags);
AUX afxUnit         AfxGetClipboardContent(afxUnit seat, afxUnit slot, afxFlags flags, afxString* buf);
AUX afxError        AfxSetClipboardContent(afxUnit seat, afxUnit slot, afxFlags flags, afxString const* text);

AUX afxError        AfxFocusWindow(afxUnit seat, afxWindow wnd, afxFlags flags);
AUX afxBool         AfxGetFocusedWindow(afxUnit seat, afxWindow* window);

AUX afxError        AfxDrawBackgroundEXT(afxDrawContext dctx, afxFlags flags);

#endif//AUX_ENVIRONMENT_H
