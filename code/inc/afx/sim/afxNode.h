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

#include "afx/core/afxInstance.h"
#include "afx/core/afxString.h"
#include "afx/math/afxMathDefs.h"
#include "afx/math/afxAabb.h"
#include "afx/math/afxSphere.h"
#include "afxSimDefs.h"

// Frames define the spatial relationships between the objects to which they are attached.
// The newly created frame has no parent and is its own root.
// To be of any use the frame should be attached to an object; frames canv be added to cameras and lights.
// In addition, frames may be used in a plugin attached to geometry.

// Frames contain position and orientation information. They define object hierarchies.
// The frame contains two matrices: the local transformation matrix (LTM) and the modeling matrix.
// The modeling matrix describes how the frame (and hence the object attached to it) is transformed relative to its local origin while the LTM describes the absolute transformation from local to world coordinates.
// If the frame is not part of a hierarchy, or forms the hierarchy's root, the modeling and LT matrices are identical. Otherwise, the modeling matrix is taken to be relative to the frame's parent and the LTM becomes the concatenation of all the modeling matrices up to the root frame. This ensures that any change in the parent's position or orientation will transfer through to the child frames.

// Handling with afxNodes, the programmer shouldn't care about matrix pre/post-multiplication. RenderWare chose don't do it.

// afxNode defines the base class for all scene objects in QWADRO AFX.



// 3D positions and transformations exist within coordinate systems called spaces.
// World space is the coordinate system for the entire scene.Its origin is at the center of the scene.The grid you see in view windows shows the world space axes.
// Object space is the coordinate system from an object's point of view.The origin of object space is at the object's pivot point, and its axes are rotated with the object.
// Local space is similar to object space, however it uses the origin and axes of the object's parent node in the hierarchy of objects.This is useful when you haven't transformed the object itself, but it is part of a group that is transformed.

typedef enum afxNodeFlag
{
    AFX_NOD_FLAG_RESYNC_M       = (1 << 0), // local modeling matrix updated requested
    AFX_NOD_FLAG_RESYNC_W       = (1 << 1), // local-to-world transformation matrix (LTM) update requested
    AFX_NOD_FLAG_RESYNC         = (AFX_NOD_FLAG_RESYNC_M | AFX_NOD_FLAG_RESYNC_W),
    AFX_NOD_FLAG_RECOMP_AABB    = (1 << 2),
    AFX_NOD_FLAG_RIGIB_BODY     = (1 << 8) // canv't be scaled
} afxNodeFlag;

AFX_DEFINE_STRUCT(afxNodeSpecification)
{
    afxString const *name;
    afxNode         parent;
    afxAabb const*  (*measure)(afxNode nod);
    afxError        (*update)(afxNode nod, afxReal dt);
    afxV4d const    *m;
    afxReal const   *rotation;
    afxReal const   *translation;
    afxReal const   *scale;
    afxReal         lodError;
};

AFX_OBJECT(afxNode)
{
    afxNodeFlag         flags;
    afxString           name; // 32

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
    afxAabb const*      (*measure)(afxNode nod);
    afxError            (*update)(afxNode nod, afxReal dt);
};

AFX afxError        AfxAcquireNodes(afxSimulation sim, afxNat cnt, afxNode nod[], afxNodeSpecification const config[]);

// From 3ds Max
// To retrieve the local transformation matrix of a node (the transformation of a node relative to its parent) you must perform some matrix arithmetic on the node's world space transformation matrix.
// This is because 3ds Max does not store the local transformation matrix.

//static_assert(!(offsetof(AFX_DEFINE_OBJECT(afxNode), m) % AFX_SIMD_ALIGN), "");
//static_assert(!(offsetof(AFX_DEFINE_OBJECT(afxNode), w) % AFX_SIMD_ALIGN), "");

// A paradigm is a model for something which explains it or shows how it canv be produced.

AFX void            AfxNodeSetFlags(afxNode nod, afxNodeFlag flags);
AFX void            AfxNodeClearFlags(afxNode nod, afxNodeFlag flags);
AFX afxResult       AfxNodeTestFlags(afxNode nod, afxNodeFlag flags);

    // HIERARCHY OPS

AFX afxNode         AfxNodeGetParent(afxNode nod);
AFX void            AfxNodeReparent(afxNode nod, afxNode parent);
AFX void            AfxNodeOrphanize(afxNode nod);
AFX afxNat          AfxNodeCountDescendancy(afxNode nod);
AFX afxNat          AfxNodeCountChildren(afxNode nod);
AFX afxChain*       AfxNodeGetChildren(afxNode nod);
AFX afxResult       AfxNodeForEachChild(afxNode nod, afxResult(*f)(afxNode child, void*), void *data);

    // AFFINE TRANSFORMATION OPS

AFX void            AfxNodeAddTransform(afxNode nod, afxM4d const m);
AFX void            AfxNodeSetTransform(afxNode nod, afxM4d const m);
AFX void            AfxNodeResetTransform(afxNode nod); // make afxM4d m, afxQuat rot, identity;
AFX afxV4d const*   AfxNodeGetTransform(afxNode nod); // afxM4d m;
AFX void            AfxNodeCopyTransform(afxNode nod, afxM4d m); // afxM4d m;

AFX afxV4d const*   AfxNodeGetAbsoluteTransform(afxNode nod); // afxM4d w;
AFX void            AfxNodeCopyAbsoluteTransform(afxNode nod, afxM4d w); // afxM4d w;

AFX void            AfxNodeAddTranslation(afxNode nod, afxV4d const translation);
AFX void            AfxNodeSetTranslation(afxNode nod, afxV4d const translation);
AFX void            AfxNodeResetTranslation(afxNode nod);
AFX void            AfxNodeCopyTranslation(afxNode nod, afxV4d translation);
    
AFX void            AfxNodeAddMakeScale(afxNode nod, afxV3d const scale);
AFX void            AfxNodeSetScale(afxNode nod, afxV3d const scale);
AFX void            AfxNodeResetScale(afxNode nod);
AFX void            AfxNodeCopyScale(afxNode nod, afxV3d scale);

AFX void            AfxNodeAddRotation(afxNode nod, afxQuat const rotation);
AFX void            AfxNodeAddAxialRotation(afxNode nod, afxV3d const axis, afxReal radians);
AFX void            AfxNodeAddEulerRotation(afxNode nod, afxReal pitch, afxReal yaw, afxReal roll);
AFX void            AfxNodeSetRotation(afxNode nod, afxQuat const rotation);
AFX void            AfxNodeResetRotation(afxNode nod);
AFX void            AfxNodeCopyRotation(afxNode nod, afxQuat rotation);

#if 0
AFX void            AfxNodeReorientTo(afxNode nod, afxV3d const dir);
AFX void            AfxNodeReorientAt(afxNode nod, afxV3d const center);
#endif//0
AFX void            AfxNodeGetExtent(afxNode nod, afxAabb *aabb); // the volumetric area covered by this node. The sum of vertices of models, frustum of camera projection, light spheres, etc.
AFX void            AfxNodeGetAmplitude(afxNode nod, afxAabb *aabb); // the sum of extent with its children as a scope.

AFX void            AfxNodeUpdate(afxNode nod, afxReal dt);
AFX void            AfxNodeRequestResync(afxNode nod);

AFX afxString const* AfxNodeGetName(afxNode nod);

#endif//AFX_NODE_H
