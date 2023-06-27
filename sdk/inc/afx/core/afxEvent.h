/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                      S I G M A   T E C H N O L O G Y   G R O U P
 *
 *                                   Public Test Build
 *                      (c) 2017 SIGMA Co. & SIGMA Technology Group
 *                                    www.sigmaco.org
 */

#ifndef AFX_EVENT_H
#define AFX_EVENT_H

#include "afx/core/diag/afxDebug.h"

typedef enum afxEventType
{
    AFX_EVENT_NONE,

    AFX_EVENT_OBJ_DESTROYED, // before deletion
    AFX_EVENT_OBJ_RENAMED, // after rename

    AFX_EVENT_STRM_UPDATED,

    AFX_EVENT_KBD_PRESSED,
    AFX_EVENT_KBD_RELEASED,

    // events são mudanças de estado, não continuidade de estado. Use funções para trabalhar com continuidade de estado.
    AFX_EVENT_MSE_LMB_PRESSED,
    AFX_EVENT_MSE_RMB_PRESSED,
    AFX_EVENT_MSE_MMB_PRESSED,
    AFX_EVENT_MSE_XMB1_PRESSED,
    AFX_EVENT_MSE_XMB2_PRESSED,
    
    AFX_EVENT_MSE_LMB_RELEASED,
    AFX_EVENT_MSE_RMB_RELEASED,
    AFX_EVENT_MSE_MMB_RELEASED,
    AFX_EVENT_MSE_XMB1_RELEASED,
    AFX_EVENT_MSE_XMB2_RELEASED,
    
    AFX_EVENT_MSE_WHEEL_UPDATED,
    AFX_EVENT_MSE_AXIS_UPDATED,

    AFX_EVENT_NOD_DETACHED, // detached from a parent node.
    AFX_EVENT_NOD_ATTACHED, // attached to another node.
    AFX_EVENT_NOD_ENLARGED, // DAG enlarged by new child node attachment (reparented to).
    AFX_EVENT_NOD_CURTAILED, // DAG curtailed by child node isolation (removal).

    AFX_EVENT_APP_RUN,
    AFX_EVENT_APP_QUIT,

    AFX_EVENT_DOUT_DRAGNDROP,

    AFX_EVENT_TOTAL
} afxEventType;

AFX_DECLARE_STRUCT(afxEvent);
AFX_DECLARE_STRUCT(afxObject);

AFX_DEFINE_STRUCT(afxEvent)
{
    afxFcc          fcc;
    afxEventType    type;
    afxBool         posted, accepted;
    union
    {
        afxObject       *receiver;
        afxObject       *obj;
    };
    void            *udd;
};

AFXINL void AfxEventDeploy(afxEvent* ev, afxEventType type, afxObject *receiver, void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(ev);
    ev->fcc = AFX_FCC_EVNT;
    ev->type = type;
    ev->posted = FALSE;
    ev->accepted = FALSE;
    ev->receiver = receiver;
    ev->udd = udd;
}

AFXINL void AfxEventAccept(afxEvent* ev) { afxError err = AFX_ERR_NONE; AfxAssertType(ev, AFX_FCC_EVNT); ev->accepted = TRUE; }
AFXINL void AfxEventIgnore(afxEvent* ev) { afxError err = AFX_ERR_NONE; AfxAssertType(ev, AFX_FCC_EVNT); ev->accepted = FALSE; }

AFXINL afxBool AfxEventIsAccepted(afxEvent* ev) { afxError err = AFX_ERR_NONE; AfxAssertType(ev, AFX_FCC_EVNT); return ev->accepted; }

#endif//AFX_EVENT_H