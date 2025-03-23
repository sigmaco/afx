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

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#ifndef AVX_TRANSFORMATION_EXT_H
#define AVX_TRANSFORMATION_EXT_H

#include "qwadro/inc/draw/math/avxViewport.h"
#include "qwadro/inc/draw/afxDrawDefs.h"
#include "qwadro/inc/draw/io/avxRaster.h"
#include "qwadro/inc/draw/io/avxBuffer.h"
#include "qwadro/inc/draw/op/avxSampler.h"
#include "qwadro/inc/draw/op/avxQueryPool.h"

  //////////////////////////////////////////////////////////////////////////////
 //// COMMANDS                                                             ////
//////////////////////////////////////////////////////////////////////////////

AVX afxCmdId AvxCmdDeclareVertex(afxDrawContext dctx, avxVertexInput vin);

AFX void AvxCmdBeginTransformFeedbackEXT
(
    afxDrawContext dctx,
    afxUnit firstCounterBuffer,
    afxUnit counterBufferCount,
    avxBuffer const cnterBuffers[],
    afxSize const cnterBufferOffsets[]
);

AFX void AvxCmdEndTransformFeedbackEXT
(
    afxDrawContext dctx,
    afxUnit firstCntBuf,
    afxUnit cnterBufCnt,
    avxBuffer const cnterBuffers[],
    afxSize const cnterBufOffsets[]
);

AFX void AvxCmdBeginQueryIndexedEXT
(
    afxDrawContext dctx,
    avxQueryPool qryp,
    afxUnit query,
    afxBool precise,
    afxUnit index
);

AFX void AvxCmdEndQueryIndexedEXT
(
    afxDrawContext dctx,
    avxQueryPool qryp,
    afxUnit query,
    afxUnit index
);

AFX void AvxCmdBindTransformFeedbackBuffersEXT
(
    afxDrawContext dctx,
    afxUnit firstBinding,
    afxUnit bindingCount,
    avxBuffer const buffers[],
    afxSize const offsets[],
    afxSize const ranges[]
);

AFX void AvxCmdDrawIndirectByteCountEXT
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
