/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                      S I G M A   T E C H N O L O G Y   G R O U P
 *
 *                                   Public Test Build
 *                      (c) 2017 SIGMA Co. & SIGMA Technology Group
 *                                    www.sigmaco.org
 */

#ifndef AFX_SKELETON_H
#define AFX_SKELETON_H

#include "afxNode.h"

/*
// Skeleton for characters and animated objects.
// Skeleton provides a hierarchical interface for managing bones, including pose, rest and animation (see Animation).


The granny_skeleton structure is a directly accessible collection of bones that describe the internal structure of a granny_model.

Each granny_bone structure in the granny_skeleton's bone array stores the transform for that bone in two different ways.
First, LocalTransform is the decomposed transform of the bone relative to its parent bone (given by ParentIndex).
If it has no parent (its ParentIndex field is equal to GrannyNoParentBone), then it is relative to the origin.

Second, InverseWorld4x4 is a 4x4 matrix that is the inverted world-space transform for the bone in the default pose of the skeleton (ie., the pose in which the skeleton was originally modeled).

LocalTransform is used primarily in compositing animations, and InverseWorld4x4 is used primarily for mesh deformation, which is why the two are stored in different formats.

Each bone also optionally contains light or camera data.
These fields point to light and camera data associated with the bone, but are not present if the bone was neither a light nor a camera.
*/

AFX_DEFINE_HANDLE(afxSkeleton);

AFX_OBJECT(afxSkeleton)
{
    AFX_OBJECT(afxNode) nod;
    afxNat              boneCnt;
    afxNode             *bones; // afxNode
    afxInt              lodType;
};

AFX afxNode     AfxSkeletonGetNode(afxSkeleton skl);
AFX afxSkeleton AfxSkeletonClone(afxSkeleton skl);

AFX afxNat      AfxSkeletonGetBoneCount(afxSkeleton skl);
AFX afxNode     AfxSkeletonGetBone(afxSkeleton skl, afxNat boneIdx);

#endif//AFX_SKELETON_H