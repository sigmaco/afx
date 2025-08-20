/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *             Q W A D R O   R E N D E R I N G   I N F R A S T R U C T U R E
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

#ifndef ARX_RENDERABLE_MODEL_H
#define ARX_RENDERABLE_MODEL_H

#include "qwadro/inc/sim/afxSimDefs.h"
#include "qwadro/inc/draw/afxDrawDefs.h"
#include "qwadro/inc/mem/afxArray.h"
#include "qwadro/inc/render/cad/arxMesh.h"
#include "qwadro/inc/render/cad/arxModel.h"
#include "qwadro/inc/render/arxRenderable.h"

AFX_DEFINE_STRUCT(arxRenderableCache)
{
    afxLink arena;
    afxUnit size;
    afxLink owner;
    afxError(*free)(void*, arxRenderableCache);
};

AFX_DEFINE_STRUCT(arxRenderableMesh)
{
    arxMesh             msh;
    afxFlags            flags;
    arxMeshMorphing     interpolator;
    afxBox              bb; // can change when morphing.
    afxSphere           bs; // can change when morphing.
    arxRenderableCache  cache;
};

AFX_DEFINE_STRUCT(arxRenderableModel)
{
    arxModel            mdl;
    afxFlags            flags;
    afxChain            meshes; // arxRenderableMesh
    arxRenderableCache  skl;
};

ARX afxError ArxPhaseInModels(afxUnit cnt, arxModel models[]);
ARX afxError ArxPhaseOutModels(afxUnit cnt, arxModel models[]);

#endif//ARX_RENDERABLE_MODEL_H
