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

#ifndef ASX_SIM_DEFS_H
#define ASX_SIM_DEFS_H

// Tudo que é alocado independentemente de forma mecanizada DEVE integrar Acquire, Reacquire e Release.

// Uma entidade somente está em cena se está vinculada a um nódulo.


// Geometry refers to any object in space whose properties may be described according to the principles of the branch of mathematics known as geometry.
// Basically any kind of shape, including points, lines, triangles, quads, etc.

// Mesh is a collection of polygons arranged in such a way that each polygon shares at least one vertex with another polygon in that collection.
// You canv reach any polygon in a mesh from any other polygon in that mesh by travesring the edges and vertices that define those polygons.

// Model is a malleable representation of a person, thing or proposed structure, composed of one or more meshes.

// Shape is the form of an object or its external boundary, outline, or external surface, as opposed to other properties such as color, texture or material type.

#include "qwadro/inc/draw/afxDrawSystem.h"

#ifndef __e2synerg__
#   ifdef _DEBUG
#       define ASX DLLIMPORT extern 
#       define ASXINL DLLIMPORT EMBED
#   else
#       define ASX DLLIMPORT extern 
#       define ASXINL DLLIMPORT EMBED
#   endif
#else
#   ifdef _DEBUG
#       define _ASX DLLEXPORT
#       define ASX DLLEXPORT extern 
#       define _ASXINL DLLEXPORT INLINE
#       define ASXINL DLLEXPORT EMBED
#   else
#       define _ASX DLLEXPORT
#       define ASX DLLEXPORT extern 
#       define _ASXINL DLLEXPORT INLINE
#       define ASXINL DLLEXPORT EMBED
#   endif
#endif//__e2combo__


AFX_DEFINE_HANDLE(afxEngine);
AFX_DEFINE_HANDLE(afxSimBridge);
AFX_DEFINE_HANDLE(afxSimQueue);
AFX_DEFINE_HANDLE(afxContext);

AFX_DEFINE_HANDLE(akxLight);
AFX_DEFINE_HANDLE(akxRenderer);
AFX_DEFINE_HANDLE(akxEntity);
AFX_DEFINE_HANDLE(afxNode);
AFX_DEFINE_HANDLE(afxBody);
AFX_DEFINE_HANDLE(afxCapstan);
AFX_DEFINE_HANDLE(afxMotion);
AFX_DEFINE_HANDLE(afxAnimation);

AFX_DEFINE_HANDLE(afxTerrain);

AFX_DEFINE_HANDLE(afxCurve);
AFX_DEFINE_HANDLE(afxAttitude);
AFX_DEFINE_HANDLE(afxPosture);
AFX_DEFINE_HANDLE(afxPlacement);
AFX_DEFINE_HANDLE(afxPose);
AFX_DEFINE_HANDLE(afxSkeleton);

AFX_DEFINE_HANDLE(afxMaterial);
AFX_DEFINE_HANDLE(afxMorphology);
AFX_DEFINE_HANDLE(afxModel);

AFX_DEFINE_HANDLE(asxBuffer);

// TODO make akxTrackMask a class object.
AFX_DEFINE_STRUCT(akxTrackMask)
{
    afxReal     defWeight;
    afxUnit     boneCnt;
    afxReal*    boneWeights;
};

typedef enum afxAccumOp
{
    afxAccumOp_NONE, // no accumulation
    afxAccumOp_CONST, // constant extraction accumulation
    afxAccumOp_DELTA, // variable delta accumulation
} afxAccumOp;

#endif//ASX_SIM_DEFS_H
