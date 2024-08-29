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

// This code is part of SIGMA Future Storage <https://sigmaco.org/future-storage>

// URD --- UNIFORM RESOURCE DICTIONARY

#ifndef AFX_URD_H
#define AFX_URD_H

#include "qwadro/io/afxStream.h"
#include "qwadro/io/afxResource.h"

AFX_DEFINE_STRUCT(afxUrdReference)
{
    afxNat32    segIdx;
    afxNat32    offset;
};

AFX afxBool     AfxOpenUrdSegment(afxUrd urd, afxNat segIdx, void *buf);
AFX afxBool     AfxOpenUrdSegments(afxUrd urd, afxNat firstSeg, afxNat segCnt, void *buf[]);

AFX void        AfxCloseUrdSegments(afxUrd urd, afxNat firstSeg, afxNat segCnt);
AFX void        AfxCloseAllUrdSegments(afxUrd urd);

AFX void* AfxDecodeUrdReference(void const **sections, afxUrdReference const* ref);

#endif//AFX_URD_H
