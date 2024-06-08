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

#define _CRT_SECURE_NO_WARNINGS 1
#include <stdio.h>
#define WIN32_LEAN_AND_MEAN 1
#include <Windows.h>
#include <hidusage.h>

#define _AFX_CORE_C
#define _AFX_DEVICE_C

#define _AUX_UX_C
#define _AUX_HID_C
#define _AUX_GAMEPAD_C
#define _AUX_CONTROLLER_C
#include "qwadro/ux/afxShell.h"


_AUX afxError _AuxCtrlDtor(afxController ctrl)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ctrl, afxFcc_CTRL);
    return err;
}

_AUX afxResult _AuxCtrlCtor(afxController ctrl, afxCookie const *cookie)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ctrl, afxFcc_CTRL);

    afxNat const *ports = cookie->udd[0];
    afxNat port = (afxNat)ports[cookie->no];

    ctrl->hid.port = port;
    ctrl->hid.flags = NIL;

    return err;
}

_AUX afxClassConfig const _AuxCtrlMgrCfg =
{
    .fcc = afxFcc_CTRL,
    .name = "Controller",
    .desc = "HID/Controller Handling",
    .unitsPerPage = 1,
    .size = sizeof(AFX_OBJECT(afxController)),
    .mmu = NIL,
    .ctor = (void*)_AuxCtrlCtor,
    .dtor = (void*)_AuxCtrlDtor
};

////////////////////////////////////////////////////////////////////////////////

#if 0
_AUX afxError AfxAcquireControllers(afxNat cnt, afxNat const port[], afxController controllers[])
{
    afxError err = AFX_ERR_NONE;
    afxManager* cls = AfxGetControllerManager();
    AfxAssertClass(cls, afxFcc_CTRL);

    for (afxNat i = 0; i < cnt; i++)
    {
        afxController tmp;
        
        if ((tmp = AfxGetController(port ? port[i] : 0)))
        {
            if (AfxReacquireObjects(1, (afxObject*)&tmp)) AfxThrowError();
            else
            {
                controllers[i] = tmp;
            }
        }
        else
        {
            if (AfxAcquireObjects(cls, 1, (afxObject*)&controllers[i], (void const*[]) { port ? (void*)&port[i] : 0 }))
                AfxThrowError();
        }

        if (err)
        {
            AfxReleaseObjects(i, (afxObject*)controllers);
            break;
        }
    }
    return err;
}

_AUX afxController AfxGetController(afxNat port)
{
    afxError err = AFX_ERR_NONE;
    afxNat i = 0;
    afxController ctrl;
    while (AfxEnumerateControllers(i, 1, &ctrl))
    {
        AfxAssertObjects(1, &ctrl, afxFcc_CTRL);

        if (ctrl->hid.port == port)
            return ctrl;

        ++i;
    }
    return NIL;
}
#endif