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

#ifndef AFX_EVENT_H
#define AFX_EVENT_H

#include "qwadro/core/afxDebug.h"

typedef enum afxEventId
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
    AFX_EVENT_APP_EXIT,

    AFX_EVENT_DOUT_DRAGNDROP,

    AFX_EVENT_TOTAL
} afxEventId;

AFX_DECLARE_STRUCT(afxEvent);
//AFX_DECLARE_STRUCT(afxHandle);

AFX_DEFINE_STRUCT(afxEvent)
{
    _AFX_DBG_FCC;
    afxEventId      id;
    afxBool         posted, accepted;
    union
    {
        void   *receiver;
        void   *obj;
    };
    void            *udd[4];
};

AFXINL void AfxEventDeploy(afxEvent* ev, afxEventId type, void *receiver, void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(ev);
#if _AFX_DEBUG
    ev->fcc = afxFcc_EVNT;
#endif
    ev->id = type;
    ev->posted = FALSE;
    ev->accepted = FALSE;
    ev->receiver = receiver;
    ev->udd[0] = udd;
    
}

// Sets the accept flag of the event object, the equivalent of calling setAccepted(true).
// Setting the accept parameter indicates that the event receiver wants the event. Unwanted events might be propagated to the parent widget.

AFXINL void AfxAcceptEvent(afxEvent* ev) { afxError err = AFX_ERR_NONE; AfxAssertType(ev, afxFcc_EVNT); ev->accepted = TRUE; }

// Clears the accept flag parameter of the event object, the equivalent of calling setAccepted(false).
// Clearing the accept parameter indicates that the event receiver does not want the event. Unwanted events might be propagated to the parent widget.

AFXINL void AfxIgnoreEvent(afxEvent* ev) { afxError err = AFX_ERR_NONE; AfxAssertType(ev, afxFcc_EVNT); ev->accepted = FALSE; }

// This property holds the accept flag of the event object.
// Setting the accept parameter indicates that the event receiver wants the event. Unwanted events might be propagated to the parent widget. By default, isAccepted() is set to true, but don't rely on this as subclasses may choose to clear it in their constructor.

AFXINL afxBool AfxEventIsAccepted(afxEvent* ev) { afxError err = AFX_ERR_NONE; AfxAssertType(ev, afxFcc_EVNT); return ev->accepted; }

#endif//AFX_EVENT_H
