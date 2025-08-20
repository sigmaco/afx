/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *        Q W A D R O   V I D E O   G R A P H I C S   I N F R A S T R U C T U R E
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

  //////////////////////////////////////////////////////////////////////////////
 // GEOMETRIC PRIMITIVE TRANSFORMATION PIPELINE EXTENDED OPERATIONS          //
//////////////////////////////////////////////////////////////////////////////

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#ifndef AVX_TRANSFORMATION_EXT_H
#define AVX_TRANSFORMATION_EXT_H

#include "qwadro/inc/draw/avxTransformation.h"

// Specify polygon mode dynamically for a draw context.

AVX afxCmdId            AvxCmdChangeFillModeEXT
(
    afxDrawContext      dctx,
    avxFillMode         mode
);

AFX afxCmdId AvxCmdBeginTransformFeedbackEXT
(
    afxDrawContext dctx,
    afxUnit firstCounterBuffer,
    afxUnit counterBufferCount,
    avxBuffer const cnterBuffers[],
    afxSize const cnterBufferOffsets[]
);

AFX afxCmdId AvxCmdEndTransformFeedbackEXT
(
    afxDrawContext dctx,
    afxUnit firstCntBuf,
    afxUnit cnterBufCnt,
    avxBuffer const cnterBuffers[],
    afxSize const cnterBufOffsets[]
);

AFX afxCmdId AvxCmdBeginQueryIndexedEXT
(
    afxDrawContext dctx,
    avxQueryPool qryp,
    afxUnit query,
    afxBool precise,
    afxUnit index
);

AFX afxCmdId AvxCmdEndQueryIndexedEXT
(
    afxDrawContext dctx,
    avxQueryPool qryp,
    afxUnit query,
    afxUnit index
);

AFX afxCmdId AvxCmdBindTransformFeedbackBuffersEXT
(
    afxDrawContext dctx,
    afxUnit firstBinding,
    afxUnit bindingCount,
    avxBuffer const buffers[],
    afxSize const offsets[],
    afxSize const ranges[]
);

AFX afxCmdId AvxCmdDrawIndirectByteCountEXT
(
    afxDrawContext dctx,
    afxUnit instCnt,
    afxUnit firstInst,
    avxBuffer cnterBuf,
    afxSize cnterBufOffset,
    afxUnit cnterOffset,
    afxUnit vtxStride
);

#endif//AVX_TRANSFORMATION_EXT_H
