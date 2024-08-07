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

#ifndef AFX_CAD_DEFS_H
#define AFX_CAD_DEFS_H

// Tudo que � alocado independentemente de forma mecanizada DEVE integrar Acquire, Reacquire e Release.

// Uma entidade somente est� em cena se est� vinculada a um n�dulo.


// Geometry refers to any object in space whose properties may be described according to the principles of the branch of mathematics known as geometry.
// Basically any kind of shape, including points, lines, triangles, quads, etc.

// Mesh is a collection of polygons arranged in such a way that each polygon shares at least one vertex with another polygon in that collection.
// You canv reach any polygon in a mesh from any other polygon in that mesh by travesring the edges and vertices that define those polygons.

// Model is a malleable representation of a person, thing or proposed structure, composed of one or more meshes.

// Shape is the form of an object or its external boundary, outline, or external surface, as opposed to other properties such as color, texture or material type.

#include "qwadro/draw/afxDrawSystem.h"
#include "qwadro/sound/afxSoundSystem.h"

#ifndef __e2sim__
#   ifdef _DEBUG
#       define AKX DLLIMPORT extern 
#       define AKXINL DLLIMPORT EMBED
#   else
#       define AKX DLLIMPORT extern 
#       define AKXINL DLLIMPORT EMBED
#   endif
#else
#   ifdef _DEBUG
#       define _AKX DLLEXPORT
#       define AKX DLLEXPORT extern 
#       define _AKXINL DLLEXPORT INLINE
#       define AKXINL DLLEXPORT EMBED
#   else
#       define _AKX DLLEXPORT
#       define AKX DLLEXPORT extern 
#       define _AKXINL DLLEXPORT INLINE
#       define AKXINL DLLEXPORT EMBED
#   endif
#endif//__e2sim__


AFX_DEFINE_HANDLE(afxMaterial);

AFX_DEFINE_HANDLE(afxMesh);
AFX_DEFINE_HANDLE(akxVertexData);
AFX_DEFINE_HANDLE(afxMeshTopology);

AFX_DEFINE_HANDLE(afxModel);
AFX_DEFINE_HANDLE(afxTxd);

AFX_DEFINE_HANDLE(akxLight);

AFX_DEFINE_HANDLE(akxRenderer);

AFX_DEFINE_HANDLE(akxEntity);

AFX_DEFINE_HANDLE(afxSkeleton);

AFX_DEFINE_HANDLE(akxNode);

AFX_DEFINE_HANDLE(afxBody);

AFX_DEFINE_HANDLE(akxMotor);

AFX_DEFINE_HANDLE(afxMotion);
AFX_DEFINE_HANDLE(akxAnimation);

AFX_DEFINE_STRUCT(akxTrackMask)
{
    afxReal     defWeight;
    afxNat      boneCnt;
    afxReal*    boneWeights;
};

typedef enum accumulation_mode
{
    NoAccumulation = 0x0,
    ConstantExtractionAccumulation = 0x1,
    VariableDeltaAccumulation = 0x2,
} accumulation_mode;

#endif//AFX_CAD_DEFS_H
