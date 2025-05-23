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

// Frames define the spatial relationships between the objects to which they are attached.
// The newly created frame has no parent and is its own root.
// To be of any use the frame should be attached to an object; frames can be added to cameras and lights.
// In addition, frames may be used in a plugin attached to geometry.

// Frames contain position and orientation information. They define object hierarchies.
// The frame contains two matrices: the local transformation matrix (LTM) and the modeling matrix.
// The modeling matrix describes how the frame (and hence the object attached to it) is transformed relative to its local origin while the LTM describes the absolute transformation from local to world coordinates.
// If the frame is not part of a hierarchy, or forms the hierarchy's root, the modeling and LT matrices are identical. Otherwise, the modeling matrix is taken to be relative to the frame's parent and the LTM becomes the concatenation of all the modeling matrices up to the root frame. This ensures that any change in the parent's position or orientation will transfer through to the child frames.

// Handling with afxNodes, the programmer shouldn't care about matrix pre/post-multiplication. RenderWare chose don't do it.

// afxNode defines the base class for all scene objects in QWADRO ASX.



// 3D positions and transformations exist within coordinate systems called spaces.
// World space is the coordinate system for the entire scene. Its origin is at the center of the scene. The grid you see in view windows shows the world space axes.
// Object space is the coordinate system from an object's point of view. The origin of object space is at the object's pivot point, and its axes are rotated with the object.
// Local space is similar to object space, however it uses the origin and axes of the object's parent node in the hierarchy of objects.This is useful when you haven't transformed the object itself, but it is part of a group that is transformed.

#ifndef ASX_NODE_H
#define ASX_NODE_H

#include "qwadro/inc/base/afxObject.h"
#include "qwadro/inc/base/afxString.h"
#include "qwadro/inc/math/afxMathDefs.h"
#include "qwadro/inc/math/bound/afxBox.h"
#include "qwadro/inc/math/bound/afxSphere.h"
#include "qwadro/inc/sim/afxSimDefs.h"
#include "qwadro/inc/sim/body/afxPose.h"
#include "qwadro/inc/sim/body/afxPlacement.h"

typedef enum afxDagNodeType
{
    afxDagNodeType_Leaf_AnimBlend,
    afxDagNodeType_Leaf_LocalAttitude,
    afxDagNodeType_Leaf_Callback,
    afxDagNodeType_OnePastLastLeafType,
    afxDagNodeType_Node_Crossfade,
    afxDagNodeType_Node_WeightedBlend,
    afxDagNodeType_OnePastLast,
} afxDagNodeType;

AFX_DEFINE_STRUCT(afxDagNode)
{
    afxDagNode*     parent;
    afxUnit          childCnt;
    afxDagNode**    children;
    afxReal         weight;
    void/*track_mask*/*trackMask;
    afxBool         ownedTrackMask;
    afxDagNodeType  type;
    union
    {
        struct
        {
            afxBody bod;
            afxReal fillThreshold;
            afxBool owned;
        } animBlend;
        struct
        {
            afxPose pose;
            afxBool owned;
        } pose;
        struct
        {
            afxPose(*sample)(void*, afxInt, afxReal, afxInt const*);
            void(*setClock)(void*, afxReal);
            void(*motionVectors)(void*, afxReal, afxReal*, afxReal*, afxBool);
            void *udd;
        } cb;
        struct
        {
            afxReal weightNone;
            afxReal weightAll;
            void/*track_mask*/*trackMask;
            afxBool owned;
        } crossfade;
        struct
        {
            afxSkeleton skl;
            afxQuatBlend quatMode;
            afxReal fillThreshold;
        } weightedBlend;
    };
};


#endif//ASX_NODE_H
