/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                      Q W A D R O   D R A W   I / O   S Y S T E M
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

  //////////////////////////////////////////////////////////////////////////////
 // The Qwadro Video Graphics Infrastructure                                 //
//////////////////////////////////////////////////////////////////////////////

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>
// This software is part of Advanced Video Graphics Extensions & Experiments.

#ifndef AVX_IMPL___TRANSFERENCE_H
#define AVX_IMPL___TRANSFERENCE_H

#include "../avxStorageImpl.h"

AVXINL void _AvxDpuUpdateRaster(avxDpu* dpu, avxRaster ras, afxUnit opCnt, avxRasterIo const ops[], afxByte const* src);
AVXINL void _AvxDpuDumpRaster(avxDpu* dpu, avxRaster ras, afxUnit opCnt, avxRasterIo const ops[], afxByte* dst);
AVXINL void _AvxDpuUnpackRaster(avxDpu* dpu, avxRaster ras, afxUnit opCnt, avxRasterIo const ops[], avxBuffer buf);
AVXINL void _AvxDpuPackRaster(avxDpu* dpu, avxRaster ras, afxUnit opCnt, avxRasterIo const ops[], avxBuffer buf);
AVXINL void _AvxDpuUploadRaster(avxDpu* dpu, avxRaster ras, afxUnit opCnt, avxRasterIo const ops[], afxStream in);
AVX void _AvxDpuDownloadRaster(avxDpu* dpu, avxRaster ras, afxUnit opCnt, avxRasterIo const ops[], afxStream out);
AVX void _AvxDpuCopyRaster(avxDpu* dpu, avxRaster dst, afxUnit opCnt, avxRasterCopy const ops[], avxRaster src);
AVX void _AvxDpuResolveRaster(avxDpu* dpu, avxRaster dst, afxUnit opCnt, avxRasterCopy const ops[], avxRaster src);
AVX void _AvxDpuBlitRaster(avxDpu* dpu, avxRaster dst, afxUnit opCnt, avxRasterBlit const ops[], avxRaster src, avxTexelFilter flt);
AVX void _AvxDpuClearRaster(avxDpu* dpu, avxRaster ras, avxClearValue const* clearVal, afxUnit baseLodIdx, afxUnit lodCnt, afxUnit baseLayer, afxUnit layerCnt);

AVX void _AvxFillBuffer(avxBuffer buf, afxUnit offset, afxUnit range, afxUnit data);
AVX void _AvxCopyBuffer(avxBuffer buf, afxUnit opCnt, avxBufferCopy const ops[], avxBuffer src);
AVX void _AvxDumpBuffer(avxBuffer buf, afxUnit opCnt, avxBufferIo const ops[], afxByte* dst);
AVX void _AvxUpdateBuffer(avxBuffer buf, afxUnit opCnt, avxBufferIo const ops[], afxByte const* src);
AVX void _AvxDownloadBuffer(avxBuffer buf, afxUnit opCnt, avxBufferIo const ops[], afxStream out);
AVX void _AvxUploadBuffer(avxBuffer buf, afxUnit opCnt, avxBufferIo const ops[], afxStream in);

#endif//AVX_IMPL___TRANSFERENCE_H
