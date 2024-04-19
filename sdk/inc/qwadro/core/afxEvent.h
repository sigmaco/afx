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

    afxThreadEvent_RUN = 1,
    afxThreadEvent_QUIT,

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

AFX_DEFINE_STRUCT(afxEvent)
{
    //_AFX_DBG_FCC;
    afxEventId      id;
    afxNat32        tid; // poster
    //afxBool         posted;
    afxBool         accepted;
    void*           udd[4];
};

AFX_DEFINE_STRUCT(afxPostedEvent)
{
    afxObject       receiver;
    afxNat          priority;
    afxNat          siz;
    afxByte         ev[];
};

#endif//AFX_EVENT_H
