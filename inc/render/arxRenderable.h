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

/*
    A scene is a observation of a simulation.
*/

#ifndef ARX_RENDERABLE_H
#define ARX_RENDERABLE_H

#include "qwadro/inc/sim/afxSimDefs.h"
#include "qwadro/inc/draw/afxDrawDefs.h"
#include "qwadro/inc/mem/afxArray.h"

// In RW3, RpGeometry is like a arxMesh. 
// It holds vertices and triangles splited in a group of mesh sections (RpMeshHeader), 
// and morph targets, triangles, UVs, prelit, material list.
// RpGeometry is not a DAG object. It can be reused (ref-counted) by RpAtomic.
// RpGeometry has also renderization cache (RwResEntry).

// In RpGeometry, there is a list of RpTriangle, which has 3 vertex indices into morph vertices and 1 material index into material list.
// The conventional indexed triangular batched mesh is actually some type of extension to it.

// RpMorphTarget is suballocated in RpGeometry. It has a bounding sphere, a list of vertex positions and normals, a reference a parent RpGeometry.


// In RW3, RpMaterial has a RwTexture, RGBA color, RxPipeline, surface lighting coefficients.
// RpMaterial is ref-counted, reused by material list of RpGeometry.

// In RW3, RpClump is a world container for DAG objects, near to arxModel.
// It backlinks (like afxChain) the world containing it.
// It links (like afxChain) a list of RpAtomic, a list of RwCamera and a list of RpLight; both DAG objects.
// It has a culling callback (it is mainly an object for frustum culling, even not being a DAG object).

// In RW3, RpAtomic is like arxMeshRig.
// It is a DAG entity nested mainly in RpClump, but is also used by RpWorldSector.
// It backlinks (like afxChain) its RpClump and the RpWorldSector containing it.
// It references one RpGeometry and embeds a morph target interpolator.
// It has bounding box and sphere.
// It has renderization cache (RwResEntry).
// It has render pipeline, render callback and render counter.


#define ARX ASX
#define _ARX _ASX
#define ARXINL ASXINL
#define _ARXINL _ASXINL

AFX_DEFINE_HANDLE(arxRenderContext);

AFX_DEFINE_HANDLE(arxCamera);
AFX_DEFINE_HANDLE(arxMesh);
AFX_DEFINE_HANDLE(arxMeshTopology);
AFX_DEFINE_HANDLE(arxScene);

AFX_DEFINE_HANDLE(arxRenderable);
AFX_DEFINE_HANDLE(arxTechnique);

AFX_DEFINE_HANDLE(akxLight);
AFX_DEFINE_HANDLE(akxRenderer);
AFX_DEFINE_HANDLE(arxTerrain);
AFX_DEFINE_HANDLE(arxPlacement);
AFX_DEFINE_HANDLE(arxPose);
AFX_DEFINE_HANDLE(arxAttitude);
AFX_DEFINE_HANDLE(arxPosture);
AFX_DEFINE_HANDLE(arxSkeleton);
AFX_DEFINE_HANDLE(arxMaterial);
AFX_DEFINE_HANDLE(arxGeome);
AFX_DEFINE_HANDLE(arxModel);

AFX_DEFINE_HANDLE(arxMotive);

#endif//ARX_RENDERABLE_H
