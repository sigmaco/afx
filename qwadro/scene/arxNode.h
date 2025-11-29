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

#ifndef ARX_NODE_H
#define ARX_NODE_H

// Frames define the spatial relationships between the objects to which they are attached.
// The newly created frame has no parent and is its own root.
// To be of any use the frame should be attached to an object; frames can be added to cameras and lights.
// In addition, frames may be used in a plugin attached to geometry.

// Frames contain position and orientation information. They define object hierarchies.
// The frame contains two matrices: the local transformation matrix (LTM) and the modeling matrix.
// The modeling matrix describes how the frame (and hence the object attached to it) is transformed relative to its local origin while the LTM describes the absolute transformation from local to world coordinates.
// If the frame is not part of a hierarchy, or forms the hierarchy's root, the modeling and LT matrices are identical. Otherwise, the modeling matrix is taken to be relative to the frame's parent and the LTM becomes the concatenation of all the modeling matrices up to the root frame. This ensures that any change in the parent's position or orientation will transfer through to the child frames.

// Handling with afxNodes, the programmer shouldn't care about matrix pre/post-multiplication. RenderWare chose don't do it.

// arxNode defines the base class for all scene objects in QWADRO ASX.



// 3D positions and transformations exist within coordinate systems called spaces.
// World space is the coordinate system for the entire scene. Its origin is at the center of the scene. The grid you see in view windows shows the world space axes.
// Object space is the coordinate system from an object's point of view. The origin of object space is at the object's pivot point, and its axes are rotated with the object.
// Local space is similar to object space, however it uses the origin and axes of the object's parent node in the hierarchy of objects.This is useful when you haven't transformed the object itself, but it is part of a group that is transformed.

#include "qwadro/base/afxObject.h"
#include "qwadro/base/afxString.h"
#include "qwadro/math/afxMathDefs.h"
#include "qwadro/coll/afxBox.h"
#include "qwadro/coll/afxFrustum.h"
#include "qwadro/coll/afxSphere.h"
#include "qwadro/sim/arxSimDefs.h"
#include "qwadro/cad/arxPose.h"
#include "qwadro/scene/arxPlacement.h"

#define ARX_NODE_DEFAULT_FILL_THRESHOLD (0.2)

typedef enum arxNodeType
{
    arxNodeType_Leaf_AnimBlend,
    arxNodeType_Leaf_Pose,
    arxNodeType_Leaf_Callback,
    arxNodeType_OnePastLastLeafType,
    arxNodeType_Node_Crossfade,
    arxNodeType_Node_WeightedBlend,
    arxNodeType_OnePastLast,
} arxNodeType;

typedef enum arxNodeFlag
{
    // Node is a root node; has not parent node, it is the absolute parent node.
    arxNodeFlag_ROOT = AFX_BITMASK(0),
    // Node is a fork node; has child nodes.
    arxNodeFlag_FORK = AFX_BITMASK(1),
    // Node is a leaf; is the end of the DAG.
    arxNodeFlag_LEAF = AFX_BITMASK(2),

    // Node requires synchronization.
    arxNodeFlag_SYNC = AFX_BITMASK(3),
    // Node is parent of other nodes in its DAG.
    arxNodeFlag_PARENT = AFX_BITMASK(4),

    // Node has a persistent arxPose attached.
    arxNodeFlag_POSE    = AFX_BITMASK(5),
    // Node has a arxPuppet attached.
    arxNodeFlag_BODY    = AFX_BITMASK(6),
    // Node has custom callback attached.
    arxNodeFlag_CUSTOM  = AFX_BITMASK(7),

    // Node is a crossfade node.
    arxNodeFlag_CROSS   = AFX_BITMASK(8),

    // Node is a pose blending node.
    arxNodeFlag_BLEND   = AFX_BITMASK(9)
} arxNodeFlag;

AFX_DEFINE_STRUCT(arxNodular)
{
    afxLink     nod;
    void        (*sync)(arxNodular*);
    afxFlags    flags;
    afxMask     mask;
};

ARX afxError ArxReparentDagNode(arxNode nod, arxNode parent);

ARX afxError ArxTransformNode(arxNode nod, afxTransform const* t);
ARX void    ArxGetNodeTransform(arxNode nod, afxTransform* t);

ARX afxError ArxStepDag(arxNode nod, afxReal /* NOT delta*/ time);

ARX afxError ArxSampleDag(arxNode nod, afxReal allowedErr, arxPose pose, afxUnit jntCnt, afxUnit const sparseJntMap[], void* cache);

ARX afxError ArxComputeDagMotionVectors(arxNode nod, afxReal secsElapsed, afxBool inv, afxV3d translation, afxV3d rotation);

ARX afxError ArxComputeDagMotionMatrix(arxNode nod, afxReal secsElapsed, afxBool inv, afxM4d const mm, afxM4d m);

ARX void    ArxGetNodeMatrix(arxNode nod, afxM4d m);

ARX afxError ArxRelinkDagPose(arxNode nod, arxPose pose);

////////////////////////////////////////////////////////////////////////////////

ARX afxError ArxAcquireJunctionNode(arxScenario scio, arxNode parent, arxModel skl, afxQuatBlend blendOp, afxReal fillThreshold, arxNode* node);
ARX afxError ArxAcquireCrossfadeNode(arxScenario scio, arxNode parent, arxNode a, arxNode b, afxReal weightNone, afxReal weightAll, arxTrackMask* trackMask, arxNode* node);
ARX afxError ArxAcquireCallbackNode(arxScenario scio, arxNode parent, afxError(*sample)(void*, afxReal, arxPose, afxUnit, afxUnit const*), void(*setClock)(void*, afxReal), void(*motionVectors)(void*, afxReal, afxReal*, afxReal*, afxBool), void* udd, arxNode* node);
ARX afxError ArxAcquirePoseNode(arxScenario scio, arxNode parent, arxPose pose, arxNode* node);
ARX afxError ArxAcquireAnimationNode(arxScenario scio, arxNode parent, arxPuppet bod, afxReal fillThreshold, arxNode* node);
ARX afxError ArxAcquirePlacementNode(arxScenario scio, arxNode parent, arxPlacement plce, arxNode* node);

ARX afxError ArxCaptureNodes(arxScenario scio, afxBool(*node)(arxNode nod, void *udd), afxArray* pvs);
ARX afxError ArxCaptureNodesInFrustum(arxScenario scio, afxFrustum const* f, afxArray* pvs);
ARX afxError ArxCaptureNodesInSphere(arxScenario scio, afxSphere const* area, afxArray* pvs);
ARX afxError ArxUpdateDags(afxReal allowedErr, afxBool matrices, afxBool poses, afxBool placements, afxUnit cnt, arxNode nodes[]);

#endif//ARX_NODE_H
