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
 *                   (c) 2017 SIGMA Technology Group — Federação SIGMA
 *                                    www.sigmaco.org
 */

#ifndef AFX_CAD_DEFS_H
#define AFX_CAD_DEFS_H

// Tudo que é alocado independentemente de forma mecanizada DEVE integrar Acquire, Reacquire e Release.

// Uma entidade somente está em cena se está vinculada a um nódulo.


// Geometry refers to any object in space whose properties may be described according to the principles of the branch of mathematics known as geometry.
// Basically any kind of shape, including points, lines, triangles, quads, etc.

// Mesh is a collection of polygons arranged in such a way that each polygon shares at least one vertex with another polygon in that collection.
// You canv reach any polygon in a mesh from any other polygon in that mesh by travesring the edges and vertices that define those polygons.

// Model is a malleable representation of a person, thing or proposed structure, composed of one or more meshes.

// Shape is the form of an object or its external boundary, outline, or external surface, as opposed to other properties such as color, texture or material type.

#include "../math/afxMathDefs.h"

AFX_DEFINE_HANDLE(awxSimulation);

AFX_DEFINE_HANDLE(awxMaterial);

AFX_DEFINE_HANDLE(awxMesh);
AFX_DEFINE_HANDLE(awxVertexData);
AFX_DEFINE_HANDLE(awxMeshTopology);

AFX_DEFINE_HANDLE(awxModel);
AFX_DEFINE_HANDLE(afxTxd);

AFX_DEFINE_HANDLE(awxLight);

AFX_DEFINE_HANDLE(awxRenderer);

AFX_DEFINE_HANDLE(awxEntity);

AFX_DEFINE_HANDLE(awxSkeleton);

AFX_DEFINE_HANDLE(awxNode);

AFX_DEFINE_HANDLE(awxMotion);
AFX_DEFINE_HANDLE(awxAnimation);

#endif//AFX_CAD_DEFS_H
