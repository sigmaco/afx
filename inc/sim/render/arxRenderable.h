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

/*
    A scene is a observation of a simulation.
*/

#ifndef ARX_RENDERABLE_H
#define ARX_RENDERABLE_H

#include "qwadro/inc/sim/afxSimDefs.h"
#include "qwadro/inc/draw/afxDrawDefs.h"
#include "qwadro/inc/mem/afxArray.h"

// In RW3, RpGeometry is like a afxMesh. 
// It holds vertices and triangles splited in a group of mesh sections (RpMeshHeader), 
// and morph targets, triangles, UVs, prelit, material list.
// RpGeometry is not a DAG object. It can be reused (ref-counted) by RpAtomic.
// RpGeometry has also renderization cache (RwResEntry).

// In RpGeometry, there is a list of RpTriangle, which has 3 vertex indices into morph vertices and 1 material index into material list.
// The conventional indexed triangular batched mesh is actually some type of extension to it.

// RpMorphTarget is suballocated in RpGeometry. It has a bounding sphere, a list of vertex positions and normals, a reference a parent RpGeometry.


// In RW3, RpMaterial has a RwTexture, RGBA color, RxPipeline, surface lighting coefficients.
// RpMaterial is ref-counted, reused by material list of RpGeometry.

// In RW3, RpClump is a world container for DAG objects, near to afxModel.
// It backlinks (like afxChain) the world containing it.
// It links (like afxChain) a list of RpAtomic, a list of RwCamera and a list of RpLight; both DAG objects.
// It has a culling callback (it is mainly an object for frustum culling, even not being a DAG object).

// In RW3, RpAtomic is like afxMeshRig.
// It is a DAG entity nested mainly in RpClump, but is also used by RpWorldSector.
// It backlinks (like afxChain) its RpClump and the RpWorldSector containing it.
// It references one RpGeometry and embeds a morph target interpolator.
// It has bounding box and sphere.
// It has renderization cache (RwResEntry).
// It has render pipeline, render callback and render counter.



#endif//ARX_RENDERABLE_H
