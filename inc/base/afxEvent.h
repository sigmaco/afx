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

#ifndef AFX_EVENT_H
#define AFX_EVENT_H

#include "qwadro/inc/base/afxDebug.h"

typedef enum afxEventId
{
    AFX_EVENT_NONE,

    afxThreadEvent_RUN = 1,
    afxThreadEvent_QUIT,

    AFX_EVENT_OBJ_DESTROYED, // before deletion
    AFX_EVENT_OBJ_RENAMED, // after rename

    AFX_EVENT_STRM_UPDATED,

    afxEvent_DRAW,
    afxEvent_SOUND,
    afxEvent_UX,

    AFX_EVENT_TOTAL
} afxEventId;

AFX_DECLARE_STRUCT(afxEvent);

AFX_DEFINE_STRUCT(afxEvent)
{
    afxEventId      id;
    afxUnit          siz;
    afxUnit32        tid; // poster
    //afxBool         posted;
    afxBool         accepted;
    void*           udd[4];
};

AFX_DEFINE_STRUCT(afxPostedEvent)
{
    afxObject       receiver;
    afxUnit          priority;
    afxUnit          siz;
    afxByte         ev[];
};

#endif//AFX_EVENT_H
