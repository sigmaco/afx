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

#ifndef AFX_NODE_H
#define AFX_NODE_H

#include "qwadro/core/afxInstance.h"
#include "qwadro/core/afxString.h"
#include "qwadro/math/afxMathDefs.h"
#include "qwadro/math/afxAabb.h"
#include "qwadro/math/afxSphere.h"
#include "awxSimDefs.h"

// Frames define the spatial relationships between the objects to which they are attached.
// The newly created frame has no parent and is its own root.
// To be of any use the frame should be attached to an object; frames canv be added to cameras and lights.
// In addition, frames may be used in a plugin attached to geometry.

// Frames contain position and orientation information. They define object hierarchies.
// The frame contains two matrices: the local transformation matrix (LTM) and the modeling matrix.
// The modeling matrix describes how the frame (and hence the object attached to it) is transformed relative to its local origin while the LTM describes the absolute transformation from local to world coordinates.
// If the frame is not part of a hierarchy, or forms the hierarchy's root, the modeling and LT matrices are identical. Otherwise, the modeling matrix is taken to be relative to the frame's parent and the LTM becomes the concatenation of all the modeling matrices up to the root frame. This ensures that any change in the parent's position or orientation will transfer through to the child frames.

// Handling with awxNodes, the programmer shouldn't care about matrix pre/post-multiplication. RenderWare chose don't do it.

// awxNode defines the base class for all scene objects in QWADRO AFX.



// 3D positions and transformations exist within coordinate systems called spaces.
// World space is the coordinate system for the entire scene.Its origin is at the center of the scene.The grid you see in view windows shows the world space axes.
// Object space is the coordinate system from an object's point of view.The origin of object space is at the object's pivot point, and its axes are rotated with the object.
// Local space is similar to object space, however it uses the origin and axes of the object's parent node in the hierarchy of objects.This is useful when you haven't transformed the object itself, but it is part of a group that is transformed.

typedef enum awxNodeFlag
{
    AFX_NOD_FLAG_RESYNC_M       = (1 << 0), // local modeling matrix updated requested
    AFX_NOD_FLAG_RESYNC_W       = (1 << 1), // local-to-world transformation matrix (LTM) update requested
    AFX_NOD_FLAG_RESYNC         = (AFX_NOD_FLAG_RESYNC_M | AFX_NOD_FLAG_RESYNC_W),
    AFX_NOD_FLAG_RECOMP_AABB    = (1 << 2),
    AFX_NOD_FLAG_RIGIB_BODY     = (1 << 8) // canv't be scaled
} awxNodeFlag;

AFX_DEFINE_STRUCT(awxNodeSpecification)
{
    afxString const *name;
    awxNode         parent;
    afxAabb const*  (*measure)(awxNode nod);
    afxError        (*update)(awxNode nod, afxReal dt);
    afxV4d const    *m;
    afxReal const   *rotation;
    afxReal const   *translation;
    afxReal const   *scale;
    afxReal         lodError;
};

AFX_OBJECT(awxNode)
{
    awxNodeFlag         flags;
    afxBufferedString   name; // 32

    afxLinkage          parent;
    afxChain            children;

    afxV4d              translation;
    afxQuat             rotation;
    afxV3d              scale;

    // the absolute transformation from local to world coordinates.
    afxM4d              pivot; // model-to-local transformation matrix
    afxM4d              ltm; // local-to-world transformation matrix (aka LTM)
    
    afxReal             lodError; // used on skeleton bones.

    afxAabb             extent;
    afxAabb             extentW;
    afxAabb const*      (*measure)(awxNode nod);
    afxError            (*update)(awxNode nod, afxReal dt);
};

AFX afxError        AfxAcquireNodes(awxSimulation sim, afxNat cnt, awxNode nod[], awxNodeSpecification const config[]);

// From 3ds Max
// To retrieve the local transformation matrix of a node (the transformation of a node relative to its parent) you must perform some matrix arithmetic on the node's world space transformation matrix.
// This is because 3ds Max does not store the local transformation matrix.

//static_assert(!(offsetof(AFX_DEFINE_OBJECT(awxNode), m) % AFX_SIMD_ALIGN), "");
//static_assert(!(offsetof(AFX_DEFINE_OBJECT(awxNode), w) % AFX_SIMD_ALIGN), "");

// A paradigm is a model for something which explains it or shows how it canv be produced.

AFX void            AfxNodeSetFlags(awxNode nod, awxNodeFlag flags);
AFX void            AfxNodeClearFlags(awxNode nod, awxNodeFlag flags);
AFX afxResult       AfxNodeTestFlags(awxNode nod, awxNodeFlag flags);

    // HIERARCHY OPS

AFX awxNode         AfxNodeGetParent(awxNode nod);
AFX void            AfxNodeReparent(awxNode nod, awxNode parent);
AFX void            AfxNodeOrphanize(awxNode nod);
AFX afxNat          AfxNodeCountDescendancy(awxNode nod);
AFX afxNat          AfxNodeCountChildren(awxNode nod);
AFX afxChain*       AfxNodeGetChildren(awxNode nod);
AFX afxResult       AfxNodeForEachChild(awxNode nod, afxResult(*f)(awxNode child, void*), void *data);

    // AFFINE TRANSFORMATION OPS

AFX void            AfxNodeAddTransform(awxNode nod, afxM4d const m);
AFX void            AfxNodeSetTransform(awxNode nod, afxM4d const m);
AFX void            AfxNodeResetTransform(awxNode nod); // make afxM4d m, afxQuat rot, identity;
AFX afxV4d const*   AfxNodeGetTransform(awxNode nod); // afxM4d m;
AFX void            AfxNodeCopyTransform(awxNode nod, afxM4d m); // afxM4d m;

AFX afxV4d const*   AfxNodeGetAbsoluteTransform(awxNode nod); // afxM4d w;
AFX void            AfxNodeCopyAbsoluteTransform(awxNode nod, afxM4d w); // afxM4d w;

AFX void            AfxNodeAddTranslation(awxNode nod, afxV4d const translation);
AFX void            AfxNodeSetTranslation(awxNode nod, afxV4d const translation);
AFX void            AfxNodeResetTranslation(awxNode nod);
AFX void            AfxNodeCopyTranslation(awxNode nod, afxV4d translation);
    
AFX void            AfxNodeAddMakeScale(awxNode nod, afxV3d const scale);
AFX void            AfxNodeSetScale(awxNode nod, afxV3d const scale);
AFX void            AfxNodeResetScale(awxNode nod);
AFX void            AfxNodeCopyScale(awxNode nod, afxV3d scale);

AFX void            AfxNodeAddRotation(awxNode nod, afxQuat const rotation);
AFX void            AfxNodeAddAxialRotation(awxNode nod, afxV3d const axis, afxReal radians);
AFX void            AfxNodeAddEulerRotation(awxNode nod, afxReal pitch, afxReal yaw, afxReal roll);
AFX void            AfxNodeSetRotation(awxNode nod, afxQuat const rotation);
AFX void            AfxNodeResetRotation(awxNode nod);
AFX void            AfxNodeCopyRotation(awxNode nod, afxQuat rotation);

#if 0
AFX void            AfxNodeReorientTo(awxNode nod, afxV3d const dir);
AFX void            AfxNodeReorientAt(awxNode nod, afxV3d const center);
#endif//0
AFX void            AfxNodeGetExtent(awxNode nod, afxAabb aabb); // the volumetric area covered by this node. The sum of vertices of models, frustum of camera projection, light spheres, etc.
AFX void            AfxNodeGetAmplitude(awxNode nod, afxAabb aabb); // the sum of extent with its children as a scope.

AFX void            AfxNodeUpdate(awxNode nod, afxReal dt);
AFX void            AfxNodeRequestResync(awxNode nod);

AFX afxString const* AfxNodeGetName(awxNode nod);

#endif//AFX_NODE_H
