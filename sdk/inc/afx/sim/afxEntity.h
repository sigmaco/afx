/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *              T H E   Q W A D R O   E X E C U T I O N   E C O S Y S T E M
 *
 *                                   Public Test Build
 *                   (c) 2017 SIGMA Technology Group — Federação SIGMA
 *                                    www.sigmaco.org
 */

#ifndef AFX_ENTITY_H
#define AFX_ENTITY_H

#include "anim/afxBody.h"
#include "afxNode.h"

enum blend_dag_node_type
{
    DagNodeType_Leaf_AnimationBlend = 0x0,
    DagNodeType_Leaf_LocalPose = 0x1,
    DagNodeType_Leaf_Callback = 0x2,
    DagNodeType_OnePastLastLeafType = 0x3,
    DagNodeType_Node_Crossfade = 0x4,
    DagNodeType_Node_WeightedBlend = 0x5,
    DagNodeType_OnePastLast = 0x6,
};

enum quaternion_mode
{
    BlendQuaternionDirectly = 0x0,
    BlendQuaternionInverted = 0x1,
    BlendQuaternionNeighborhooded = 0x2,
    BlendQuaternionAccumNeighborhooded = 0x3,
};

AFX_DEFINE_STRUCT(afxDagAnimation)
{
    afxBody bod;
    afxReal fillThreshold;
    afxBool owned;
};

AFX_DEFINE_STRUCT(afxDagPose)
{
    afxPose *lp;
    afxBool owned;
};

AFX_DEFINE_STRUCT(afxDagVmt)
{
    afxPose*(*sample)(void*, afxInt, afxReal, afxInt const*);
    void(*setClock)(void*, afxReal);
    void(*motionVectors)(void*, afxReal, afxReal*, afxReal*, afxBool);
    void *udd;
};

AFX_DEFINE_STRUCT(afxDagCrossfade)
{
    afxReal weightNone;
    afxReal weightAll;
    void/*granny::track_mask*/ *trackMask;
    afxBool owned;
};

AFX_DEFINE_STRUCT(afxDagWeighted)
{
    afxSkeleton skl;
    enum quaternion_mode QuatMode;
    afxReal fillThreshold;
};

AFX_DEFINE_STRUCT(afxDagNode)
{
    afxDagNode *parent;
    afxNat childCnt;
    afxDagNode **children;
    afxReal Weight;
    void/*granny::track_mask*/ *trackMask;
    afxBool ownedTrackMask;
    enum blend_dag_node_type Type;
    union
    {
        afxDagAnimation AnimationBlend;
        afxDagPose LocalPose;
        afxDagVmt Callback;
        afxDagCrossfade Crossfade;
        afxDagWeighted WeightedBlend;
    };
};

AFX_OBJECT(afxEntity)
{
    AFX_OBJECT(afxNode) nod;
};

AFX afxNode     AfxEntityGetNode(afxEntity ent);

#endif//AFX_ENTITY_H