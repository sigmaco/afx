/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *          Q W A D R O   4 D   R E N D E R I N G   I N F R A S T R U C T U R E
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This file is part of Advanced Renderware Extensions & Experiments for Qwadro.

#ifndef ARX_RENDERABLE_MODEL_H
#define ARX_RENDERABLE_MODEL_H

#include "qwadro/sim/arxSimDefs.h"
#include "qwadro/draw/afxDrawDefs.h"
#include "qwadro/mem/afxArray.h"
#include "qwadro/cad/arxMesh.h"
#include "qwadro/cad/arxModel.h"
#include "qwadro/cad/arxMaterial.h"
#include "qwadro/render/arxRenderable.h"

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

AFX_DEFINE_STRUCT(arxRenderModel)
{
    arxModel            mdl;
    afxFlags            flags;
    afxChain            meshes; // arxRenderableMesh
    arxRenderableCache  skl;
};

ARX afxError ArxPhaseInModels(afxUnit cnt, arxModel models[]);
ARX afxError ArxPhaseOutModels(afxUnit cnt, arxModel models[]);

#endif//ARX_RENDERABLE_MODEL_H
