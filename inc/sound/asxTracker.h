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

#ifndef ASX_TRACKER_H
#define ASX_TRACKER_H

#include "qwadro/inc/sound/afxSoundDefs.h"

/// In a tracker, the music runs from top to bottom in an easily understood grid known as a pattern. 
/// Several patterns arranged in a certain order make up a song. 
/// Step-editing in a pattern grid lends itself well to a fast and immediate workflow.

AFX_OBJECT(asxTracker)
{
    afxBool loaded;
    afxUnit  sampleCnt;
    afxUnit  patternCnt;
    void*   udd;
};



ASX afxError AsxAcquireTrackers(afxSoundContext sctx, afxUnit cnt, asxTracker trackers[]);

#endif//ASX_TRACKER_H
