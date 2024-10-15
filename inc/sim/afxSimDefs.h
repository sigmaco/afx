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

#ifndef AMX_SIM_DEFS_H
#define AMX_SIM_DEFS_H

// Tudo que é alocado independentemente de forma mecanizada DEVE integrar Acquire, Reacquire e Release.

// Uma entidade somente está em cena se está vinculada a um nódulo.


// Geometry refers to any object in space whose properties may be described according to the principles of the branch of mathematics known as geometry.
// Basically any kind of shape, including points, lines, triangles, quads, etc.

// Mesh is a collection of polygons arranged in such a way that each polygon shares at least one vertex with another polygon in that collection.
// You canv reach any polygon in a mesh from any other polygon in that mesh by travesring the edges and vertices that define those polygons.

// Model is a malleable representation of a person, thing or proposed structure, composed of one or more meshes.

// Shape is the form of an object or its external boundary, outline, or external surface, as opposed to other properties such as color, texture or material type.

#include "qwadro/inc/draw/afxDrawSystem.h"
#include "qwadro/inc/sound/dev/afxSoundSystem.h"

#ifndef __e2combo__
#   ifdef _DEBUG
#       define AMX DLLIMPORT extern 
#       define AMXINL DLLIMPORT EMBED
#   else
#       define AMX DLLIMPORT extern 
#       define AMXINL DLLIMPORT EMBED
#   endif
#else
#   ifdef _DEBUG
#       define _AMX DLLEXPORT
#       define AMX DLLEXPORT extern 
#       define _AMXINL DLLEXPORT INLINE
#       define AMXINL DLLEXPORT EMBED
#   else
#       define _AMX DLLEXPORT
#       define AMX DLLEXPORT extern 
#       define _AMXINL DLLEXPORT INLINE
#       define AMXINL DLLEXPORT EMBED
#   endif
#endif//__e2combo__


AFX_DEFINE_HANDLE(afxComboDevice);
AFX_DEFINE_HANDLE(afxComboBridge);
AFX_DEFINE_HANDLE(afxMathQueue);

AFX_DEFINE_HANDLE(akxLight);
AFX_DEFINE_HANDLE(akxRenderer);
AFX_DEFINE_HANDLE(akxEntity);
AFX_DEFINE_HANDLE(akxNode);
AFX_DEFINE_HANDLE(afxBody);
AFX_DEFINE_HANDLE(afxMotor);
AFX_DEFINE_HANDLE(afxMotion);
AFX_DEFINE_HANDLE(afxAnimation);

AFX_DEFINE_HANDLE(afxTerrain);

AFX_DEFINE_STRUCT(akxTrackMask)
{
    afxReal     defWeight;
    afxNat      boneCnt;
    afxReal*    boneWeights;
};

typedef enum akxAccumulateOp
{
    akxAccumulateOp_NONE, // no accumulation
    akxAccumulateOp_CONST, // constant extraction accumulation
    akxAccumulateOp_DELTA, // variable delta accumulation
} akxAccumulateOp;

#endif//AMX_SIM_DEFS_H
