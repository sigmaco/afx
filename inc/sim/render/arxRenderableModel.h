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

#ifndef ARX_MESH_H
#define ARX_MESH_H

#include "qwadro/inc/sim/afxSimDefs.h"
#include "qwadro/inc/draw/afxDrawDefs.h"
#include "qwadro/inc/mem/afxArray.h"
#include "qwadro/inc/sim/io/afxMesh.h"

#define ARX ASX
#define _ARX _ASX
#define ARXINL ASXINL
#define _ARXINL _ASXINL

AFX_DEFINE_STRUCT(arxRenderableCache)
{
    afxLink arena;
    afxUnit size;
    afxLink owner;
    afxError(*free)(void*, arxRenderableCache);
};

AFX_DEFINE_STRUCT(arxRenderableMesh)
{
    afxMesh             msh;
    afxFlags            flags;
    afxMeshMorphing     interpolator;
    afxBox              bb; // can change when morphing.
    afxSphere           bs; // can change when morphing.
    arxRenderableCache  cache;
};

AFX_DEFINE_STRUCT(arxRenderableModel)
{
    afxModel            mdl;
    afxFlags            flags;
    afxChain            meshes; // arxRenderableMesh
    arxRenderableCache  skl;
};

ARX afxError ArxPhaseInModels(afxUnit cnt, afxModel models[]);
ARX afxError ArxPhaseOutModels(afxUnit cnt, afxModel models[]);

#endif//ARX_MESH_H
