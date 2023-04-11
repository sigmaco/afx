/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                      S I G M A   T E C H N O L O G Y   G R O U P
 *
 *                                   Public Test Build
 *                      (c) 2017 SIGMA Co. & SIGMA Technology Group
 *                                    www.sigmaco.org
 */

#ifndef AFX_LEGO_H
#define AFX_LEGO_H

#include "afxPipelineRig.h"
#include "../res/afxBuffer.h"

// A GPUBindGroup defines a set of resources to be bound together in a group and how the resources are used in shader stages.

AFX_DEFINE_STRUCT(afxLegoPoint) // A GPUBindGroupEntry describes a single resource to be bound in a GPUBindGroup, and has the following members:
{
    union
    {
        struct
        {
            afxBuffer           buf; // The GPUBuffer to bind.
            afxNat              base; // The offset, in bytes, from the beginning of buffer to the beginning of the range exposed to the shader by the buffer binding.
            afxNat              range; // The size, in bytes, of the buffer binding. If undefined, specifies the range starting at offset and ending at the end of buffer.
        }                       data;
        struct
        {
            afxSampler          smp;
            afxTexture          tex;
        }                       img;
    }                           resource; // The resource to bind, which may be a GPUSampler, GPUTextureView, GPUExternalTexture, or GPUBufferBinding.
};

AFX_DEFINE_HANDLE(afxLego);

#ifndef AFX_DRAW_DRIVER_SRC

AFX_OBJECT(afxLego) // A GPUBindGroup defines a set of resources to be bound together in a group and how the resources are used in shader stages.
{
    afxObject               obj;
};

#endif

AFX afxError                AfxLegoUpdate(afxLego lego, afxNat cnt, afxLegoPoint const points[]);
AFX afxError                AfxLegoCopy(afxLego lego, afxLego in);
AFX afxLegoSchema const*    AfxLegoGetSchema(afxLego lego);

#endif//AFX_LEGO_H