/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *              T H E   Q W A D R O   E X E C U T I O N   E C O S Y S T E M
 *
 *                                   Public Test Build
 *                   (c) 2017 SIGMA Technology Group — Federação SIGMA
 *                                    www.sigmaco.org
 */

#ifndef AFX_LIGHT_H
#define AFX_LIGHT_H

#include "afx/sim/afxNode.h"
#include "afx/core/mem/afxArray.h"

#define AFX_LIT_ALIGN AFX_SIMD_ALIGN

typedef enum
{
    /// A light placed infinitely far away. It affects everything in the scene.
    AFX_LIGHT_DIRECTIONAL,

    /// A light that shines equally in all directions from its location, affecting all objects within its Range.
    AFX_LIGHT_POINT,

    /// A light that shines everywhere within a cone defined by Spot Angle and Range. Only objects within this region are affected by the light.
    AFX_LIGHT_SPOT,

    /// A light that shines in all directions to one side of a rectangular area of a plane. The rectangle is defined by the Width and Height properties.
    /// Area lights are only available during lightmap baking and have no effect on objects at runtime.
    AFX_LIGHT_AREA
} afxLightType;

AFX_OBJECT(afxLight)
{
    AFX_OBJECT(afxNode) nod;
    afxLightType            type;

    afxV4d                  color;
    afxReal                 intensity;
    afxReal                 range,
                            angle;

    afxInt                  cullingMask;
    afxBool                 drawHalo;

    void                    *surfaceDecal;
    void                    *flareTex;
};


AFX afxNode     AfxLightGetNode(afxLight lit);
AFX afxResult   AfxLightCaptureAffectedNodes(afxLight lit, afxNode root, afxArray *capturedNods);

#endif//AFX_LIGHT_H