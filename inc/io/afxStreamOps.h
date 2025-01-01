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

// afxStream, data I/O stream, is the base object for objects representing files in Qwadro.

#ifndef AFX_STREAM_OPS_H
#define AFX_STREAM_OPS_H

#include "qwadro/inc/io/afxStream.h"

AFX afxCmdId        AfxCmdReadAt(afxStream iob, afxSize at, afxUnit rowStride, afxUnit rowCnt, afxUnit dstCap, void *dst);
AFX afxCmdId        AfxCmdRead(afxStream iob, afxUnit rowStride, afxUnit rowCnt, afxUnit dstCap, void *dst);

AFX afxCmdId        AfxCmdWriteAt(afxStream iob, afxSize at, afxUnit rowStride, afxUnit rowCnt, void const* src, afxUnit srcSiz);
AFX afxCmdId        AfxCmdWrite(afxStream iob, afxUnit rowStride, afxUnit rowCnt, void const* src, afxUnit srcSiz);

AFX afxCmdId        AfxCmdMerge(afxStream iob, afxStream src, afxSize from, afxUnit range);
AFX afxCmdId        AfxCmdMergeAt(afxStream iob, afxSize at, afxStream src, afxSize from, afxUnit range);

AFX afxCmdId        AfxCmdDecode(afxStream iob, afxSize at, afxUnit encSiz, afxFcc codec, afxUnit stop0, afxUnit stop1, afxUnit stop2, void* dst);
AFX afxCmdId        AfxCmdEncode(afxStream iob, afxSize at, afxUnit decSiz, afxFcc codec, afxUnit stop0, afxUnit stop1, afxUnit stop2, void* dst);

#endif//AFX_STREAM_OPS_H
