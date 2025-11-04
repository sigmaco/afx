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

// This file is part of Advanced Renderware Extensions & Experiments for Qwadro.

// Welcome to Department of Redundancy Department.

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

#include "qwadro/draw/afxDrawSystem.h"

#ifndef __e2kinee__
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
#endif//__e2kinee__


AFX_DEFINE_HANDLE(arxEngine);
AFX_DEFINE_HANDLE(arxSimBridge);
AFX_DEFINE_HANDLE(arxSimQueue);
AFX_DEFINE_HANDLE(arxContext);

AFX_DEFINE_HANDLE(arxEntity);
AFX_DEFINE_HANDLE(arxNode);
AFX_DEFINE_HANDLE(arxBody);
AFX_DEFINE_HANDLE(arxCapstan);
AFX_DEFINE_HANDLE(arxMotion);
AFX_DEFINE_HANDLE(arxAnimation);


AFX_DEFINE_HANDLE(arxCurve);

// TODO make arxTrackMask a class object.
AFX_DEFINE_STRUCT(arxTrackMask)
{
    afxReal     defWeight;
    afxUnit     boneCnt;
    afxReal*    boneWeights;
};

typedef enum arxAccumOp
{
    arxAccumOp_NONE, // no accumulation
    arxAccumOp_CONST, // constant extraction accumulation
    arxAccumOp_DELTA, // variable delta accumulation
} arxAccumOp;

#endif//ASX_SIM_DEFS_H
