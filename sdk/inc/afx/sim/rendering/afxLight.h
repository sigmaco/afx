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
 *                               (c) 2017 Federação SIGMA
 *                                    www.sigmaco.org
 */

#ifndef AFX_LIGHT_H
#define AFX_LIGHT_H

#include "afx/sim/afxNode.h"
#include "afx/core/mem/afxArray.h"

#define AFX_LIT_ALIGN AFX_SIMD_ALIGN

AFX_DEFINE_HANDLE(afxLight);

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
AFX afxResult   AfxLightCaptureAffectedNodes(afxLight lit, afxNode root, afxArray(afxNode) *captures);

#endif//AFX_LIGHT_H