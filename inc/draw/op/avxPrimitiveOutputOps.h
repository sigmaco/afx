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

#ifndef AVX_PRIMITIVE_OUTPUT_OPS_H
#define AVX_PRIMITIVE_OUTPUT_OPS_H

#include "qwadro/inc/math/afxViewport.h"
#include "qwadro/inc/draw/afxDrawDefs.h"
#include "qwadro/inc/draw/io/afxRaster.h"
#include "qwadro/inc/draw/io/afxBuffer.h"
#include "qwadro/inc/draw/pipe/avxSampler.h"
#include "qwadro/inc/draw/pipe/avxQueryPool.h"

  //////////////////////////////////////////////////////////////////////////////
 //// COMMANDS                                                             ////
//////////////////////////////////////////////////////////////////////////////

AVX afxCmdId AvxCmdBindVertexInput(afxDrawContext dctx, avxVertexDecl vin);

AFX void AvxCmdBeginTransformFeedbackEXT
(
    afxDrawContext dctx,
    afxUnit firstCounterBuffer,
    afxUnit counterBufferCount,
    afxBuffer const cnterBuffers[],
    afxSize const cnterBufferOffsets[]
);

AFX void AvxCmdEndTransformFeedbackEXT
(
    afxDrawContext dctx,
    afxUnit firstCntBuf,
    afxUnit cnterBufCnt,
    afxBuffer const cnterBuffers[],
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
    afxBuffer const buffers[],
    afxSize const offsets[],
    afxSize const ranges[]
);

AFX void AvxCmdDrawIndirectByteCountEXT
(
    afxDrawContext dctx,
    afxUnit instCnt,
    afxUnit firstInst,
    afxBuffer cnterBuf,
    afxSize cnterBufOffset,
    afxUnit cnterOffset,
    afxUnit vtxStride
);

#endif//AVX_PRIMITIVE_OUTPUT_OPS_H
