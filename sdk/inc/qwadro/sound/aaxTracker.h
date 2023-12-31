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
 *                   (c) 2017 SIGMA Technology Group — Federação SIGMA
 *                                    www.sigmaco.org
 */

#ifndef AAX_TRACKER_H
#define AAX_TRACKER_H

#include "qwadro/sound/afxSoundDefs.h"

/// In a tracker, the music runs from top to bottom in an easily understood grid known as a pattern. 
/// Several patterns arranged in a certain order make up a song. 
/// Step-editing in a pattern grid lends itself well to a fast and immediate workflow.

AFX_OBJECT(aaxTracker)
{
    afxBool loaded;
    afxNat  sampleCnt;
    afxNat  patternCnt;
};

#endif//AAX_TRACKER_H
