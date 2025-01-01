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

// This code is part of SIGMA Future Storage.

#ifndef AFX_RING_H
#define AFX_RING_H

#include "qwadro/inc/exec/afxAtomic.h"

AFX_DEFINE_STRUCT(afxRing)
{
    afxUnit     unitSiz;
    afxUnit32   cap;
    afxUnit32   readIdx;
    afxUnit32   writeIdx;
    afxAtom32   readableItemCnt;
    afxByte*    data;
};


AFX afxError    AfxDeployRing(afxRing* ring, afxUnit unitSiz, afxUnit32 cap);

AFX void        AfxDismantleRing(afxRing* ring);

// ---- Producer ---- //

AFX afxUnit32   AfxGetRingWriteIndex(afxRing* ring);

AFX void*       AfxGetRingWritePtr(afxRing* ring);

AFX afxUnit32   AfxGetRingNbWritableItems(afxRing* ring);

AFX void        AfxIncrementRingWriteIndex(afxRing* ring, afxUnit32 itemCnt);

AFX afxUnit32   AfxGetRingReadIndex(afxRing* ring);

AFX void const* AfxGetRingReadPtr(afxRing* ring);

AFX void const* AfxPeekRing(afxRing* ring, afxUnit32 offset);

AFX void        AfxIncrementRingReadIndex(afxRing* ring, afxUnit32 itemCnt);

AFX afxUnit32   AfxGetRingNbReadableItems(afxRing* ring);

AFX afxUnit32   AfxGetRingSize(afxRing* ring);

AFX afxBool     AfxGrowRing(afxRing* ring, afxUnit32 growBy);

#endif//AFX_RING_H
