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

#ifndef AVX_RASTERIZATION_EXT_H
#define AVX_RASTERIZATION_EXT_H

#include "qwadro/inc/draw/op/avxRasterization.h"

/*
    The AvxCmdAdjustCurtainsSIGMA() operation adjusts exclusive scissor rectangles dynamically for a draw context.

    The curtain test behaves like the scissor test, except that the curtain test fails for pixels inside the corresponding rectangle and passes for pixels outside the rectangle. 
    If the same rectangle is used for both the scissor and curtain tests, the curtain test will pass if and only if the scissor test fails.
*/

AVX afxCmdId AvxCmdAdjustCurtainsSIGMA
(
    afxDrawContext      dctx, 
    // the first curtain rectangle whose state is updated by the command.
    afxUnit             baseIdx,
     // the number of curtain rectangles updated by the command.
    afxUnit             cnt,
     // an array of afxRect structures defining curtain rectangles.
    afxRect const       rects[]
);

/*
    The AvxCmdSetRasterizationSamplesEXT() operation specifies the rasterization samples dynamically for a draw context.
*/

AVX afxCmdId AvxCmdSetRasterizationSamplesEXT
(
    afxDrawContext      dctx,
    afxUnit             sampleLvl
);

/*
    The AvxCmdSetSampleMaskEXT() operation specifies the sample mask dynamically for a draw context.
*/

AVX afxCmdId AvxCmdSetSampleMaskEXT
(
    afxDrawContext      dctx,
    afxUnit             sampleLvl,
    // An array of mask values, where the array size is based on the sampleLvl parameter.
    afxMask const       sampleMasks[AVX_MAX_SAMPLE_MASKS]
);

/*
    The AvxCmdSwitchAlphaToCoverageEXT() operation specifies the alpha to coverage enable state dynamically for a draw context.
*/

AVX afxCmdId AvxCmdSwitchAlphaToCoverageEXT
(
    afxDrawContext      dctx,
    afxBool             enable
);

/*
    The AvxCmdSwitchAlphaToOneEXT() operation specifies the alpha to one enable state dynamically for a draw context.
*/

AVX afxCmdId AvxCmdSwitchAlphaToOneEXT
(
    afxDrawContext      dctx,
    afxBool             enable
);

/*
    The AvxCmdSwitchColorBlendingEXT() operation specifies the blend state for each attachment dynamically for a draw context.
*/

AVX afxCmdId AvxCmdSwitchColorBlendingEXT
(
    afxDrawContext      dctx,
    afxUnit             first,
    afxUnit             cnt,
    afxBool const       enabled[]
);

/*
    The AvxCmdChangeColorBlendEquationEXT() operation specifies the blend factors and operations dynamically for a draw context.
*/

AVX afxCmdId AvxCmdChangeColorBlendEquationEXT
(
    afxDrawContext      dctx,
    afxUnit             first,
    afxUnit             cnt,
    avxColorBlend const equations[]
);

/*
    The AvxCmdSetColorWriteMaskEXT() operation specifies the color write masks for each attachment dynamically for a draw context.
*/

AVX afxCmdId AvxCmdSetColorWriteMaskEXT
(
    afxDrawContext      dctx,
    afxUnit             first,
    afxUnit             cnt,
    avxColorMask const  writeMasks[]
);

/*
    The AvxCmdSwitchLogicOpEXT() operation specifies dynamically whether logical operations are enabled for a draw context.
*/

AVX afxCmdId AvxCmdSwitchLogicOpEXT
(
    afxDrawContext      dctx,
    afxBool             enabled
);

/*
    The AvxCmdSetDepthClampEnableEXT() operation specifies dynamically whether depth clamping is enabled in the draw context.
*/

AVX afxCmdId AvxCmdSetDepthClampEnableEXT
(
    afxDrawContext      dctx,
    afxBool             enabled
);

/*
    The AvxCmdDiscardRectanglesEXT() operation discards rectangles dynamically for a draw context.
*/

AVX afxCmdId AvxCmdDiscardRectanglesEXT
(
    afxDrawContext      dctx,
    // The first discard rectangle whose state is updated by the command.
    afxUnit             first,
    // The number of discard rectangles whose state are updated by the command.
    afxUnit             cnt,
    // An array of afxRect structures specifying discard rectangles.
    afxRect const*      rects
);

#endif//AVX_RASTERIZATION_EXT_H
