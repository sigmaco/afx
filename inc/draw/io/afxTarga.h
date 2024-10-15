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

// This code is part of Qwadro Draw System <https://sigmaco.org/qwadro>
// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#ifndef AFX_TARGA_H
#define AFX_TARGA_H

#include "qwadro/inc/draw/io/afxRaster.h"

// provisory as Qwadro formats are inherited from Vulkan.

typedef enum afxTargaCodec
{
    afxTargaCodec_NONE,
    afxTargaCodec_RLE8 = 1,
    afxTargaCodec_GDEFLATE,
} afxTargaCodec;

typedef enum afxTargaFlag
{
    afxTargaFlag_X_FLIPPED, // vertical origin is right to left.
    afxTargaFlag_Y_FLIPPED, // horizontal origin is top to bottom.
    afxTargaFlag_CUBEMAP, // layered image is treated as cubemap.
    afxTargaFlag_3D, // depth is treated as 3D depth instead of layers.
    afxTargaFlag_LAYER_MAJOR
} afxTargaFlags;

#pragma pack(push, 1)

AFX_DEFINE_STRUCT(urdTrunk_Targa)
{
    urdTrunk        trunk; // base trunk
    // Targa structure
    afxNat32        fmt;
    afxNat32        flags;
    afxNat32        resvd;
    afxNat32        xyz[3];
    afxNat16        baseLod;
    afxNat32        whd[3];
    afxNat16        lodCnt;
    afxNat32        rowStride; // bytes per row/scanline (W usually aligned/padded to some memory boundary)
    afxNat32        depthStride;
    afxUrdOrigin    data;
    afxNat32        udd[4];
};

#pragma pack(pop)

#endif//AFX_TARGA_H
