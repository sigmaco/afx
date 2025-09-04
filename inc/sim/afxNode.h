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
#include "qwadro/inc/math/coll/afxBox.h"
#include "qwadro/inc/math/coll/afxFrustum.h"
#include "qwadro/inc/math/coll/afxSphere.h"
#include "qwadro/inc/sim/afxSimDefs.h"
#include "qwadro/inc/render/cad/arxPose.h"
#include "qwadro/inc/render/cad/arxPlacement.h"

#define ASX_NODE_DEFAULT_FILL_THRESHOLD (0.2)

typedef enum asxNodeType
{
    asxNodeType_Leaf_AnimBlend,
    asxNodeType_Leaf_Pose,
    asxNodeType_Leaf_Callback,
    asxNodeType_OnePastLastLeafType,
    asxNodeType_Node_Crossfade,
    asxNodeType_Node_WeightedBlend,
    asxNodeType_OnePastLast,
} asxNodeType;

typedef enum asxNodeFlag
{
    // Node is a root node; has not parent node, it is the absolute parent node.
    asxNodeFlag_ROOT = AFX_BITMASK(0),
    // Node is a fork node; has child nodes.
    asxNodeFlag_FORK = AFX_BITMASK(1),
    // Node is a leaf; is the end of the DAG.
    asxNodeFlag_LEAF = AFX_BITMASK(2),

    // Node requires synchronization.
    asxNodeFlag_SYNC = AFX_BITMASK(3),
    // Node is parent of other nodes in its DAG.
    asxNodeFlag_PARENT = AFX_BITMASK(3),
} asxNodeFlags;

AFX_OBJECT(afxNode)
{
#if 0
    asxNode*     parent;
    afxUnit          childCnt;
    asxNode**    children;
#else
    // It seems like allocation-based children were just a overcomplicated way of doing it as it is ALWAYS a DAG (not double reference or reentrance allowed).
    afxLink         parent;
    afxChain        children;
#endif
    afxChain        nodulars;

    union
    {
        struct
        {
            arxBody bod;
            afxReal fillThreshold;
            //afxBool owned;
        } animBlend;
        struct
        {
            arxPose pose;
            //afxBool owned;
        } pose;
        struct
        {
            arxPose(*sample)(void*, afxInt, afxReal, afxInt const*);
            void(*setClock)(void*, afxReal);
            void(*motionVectors)(void*, afxReal, afxReal*, afxReal*, afxBool);
            void *udd;
        } cb;

        struct
        {
            afxReal weightNone;
            afxReal weightAll;
            akxTrackMask*trackMask;
            afxBool owned;
        } crossfade;
        struct
        {
            arxModel skl;
            afxQuatBlend quatMode;
            afxReal fillThreshold;
        } weightedBlend;
    };
    asxNodeType  type;
    afxReal         weight;
    akxTrackMask*   trackMask;
    afxBool         ownedTrackMask;

    // ARX
    afxBool dirty;
    // Unlike RenderWare, Qwadro uses a afxTransform to work with node transformation to avoid orthonormalizations required in RW.
    afxTransform t;
    // Like RenderWare, Qwadro uses two matrices but both are cached once it uses a afxTransform to do transformations.
    afxM4d w, m;
    //arxPlacement plce;
    afxUnit ltaCnt;
    afxUnit taCnt;
};

AFX_DEFINE_STRUCT(afxNodular)
{
    afxLink     nod;
    void        (*sync)(afxNodular*);
    afxFlags    flags;
    afxMask     mask;
};

ASX afxError AsxCreateDagAnimationBlend(afxNode parent, arxBody bod, afxNode* node);
ASX afxError AsxCreateDagAnimationBlend2(afxNode parent, arxModel mdl, afxNode* node);

ASX afxError AsxCreateDagPose(afxNode parent, arxPose pose, afxNode* node);
ASX afxError AsxCreateDagCallback(afxNode parent, void(*sample()), void(*clock()), void(*motions()), void* udd, afxNode* node);

ASX afxError AsxCreateDagCrossfade(afxNode a, afxNode b, afxReal weightNone, afxReal weightAll, akxTrackMask* mask, afxUnit jntCnt, afxNode* node);

ASX afxError AsxCreateDagWeightedBlend(afxNode parent, arxModel skl, afxReal fillThreshold, afxQuatBlend blend, afxUnit jntCnt, afxNode* node);

ASX afxError AsxDeleteDagNode(afxNode nod);

ASX afxError AsxReparentDagNode(afxNode nod, afxNode parent);

ASX afxError AsxTransformNode(afxNode nod, afxTransform const* t);

ASX afxError AsxStepDag(afxNode nod, afxReal /* NOT delta*/ clock);

ASX afxError AsxSampleDag(afxNode nod, afxUnit jntCnt, afxReal allowedErr, afxUnit const sparseJntMap[], void* cache, arxPose* pose);

ASX afxError AsxComputeDagMotionVectors(afxNode nod, afxReal secsElapsed, afxBool inv, afxV3d translation, afxV3d rotation);

ASX afxError AsxComputeDagMotionMatrix(afxNode nod, afxReal secsElapsed, afxBool inv, afxM4d const mm, afxM4d m);

ASX void    AsxGetNodeTransform(afxNode nod, afxTransform* t);
ASX void    AsxGetNodeMatrix(afxNode nod, afxM4d m);

////////////////////////////////////////////////////////////////////////////////

ASX afxError AsxAcquireJunctionNode(afxSimulation sim, afxNode parent, arxModel skl, afxQuatBlend blendOp, afxReal fillThreshold, afxNode* node);

ASX afxError AsxAcquireCrossfadeNode(afxSimulation sim, afxNode parent, afxNode a, afxNode b, afxReal weightNone, afxReal weightAll, akxTrackMask* trackMask, afxNode* node);

ASX afxError AsxAcquireCallbackNode(afxSimulation sim, afxNode parent, arxPose(*sample)(void*, afxInt, afxReal, afxInt const*), void(*setClock)(void*, afxReal), void(*motionVectors)(void*, afxReal, afxReal*, afxReal*, afxBool), void* udd, afxNode* node);

ASX afxError AsxAcquirePoseNode(afxSimulation sim, afxNode parent, arxPose pose, afxNode* node);

ASX afxError AsxAcquireAnimationNode(afxSimulation sim, afxNode parent, arxBody bod, afxReal fillThreshold, afxNode* node);

ASX afxError AsxAcquirePlacementNode(afxSimulation sim, afxNode parent, arxPlacement plce, afxNode* node);

ASX afxError AsxCaptureNodesInFrustum(afxSimulation sim, afxFrustum const* f, afxArray* pvs);
ASX afxError AsxUpdateDags(afxReal allowedErr, afxBool matrices, afxBool poses, afxBool placements, afxUnit cnt, afxNode nodes[]);

#endif//ASX_NODE_H
