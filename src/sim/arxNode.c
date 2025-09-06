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

#define _ASX_SIM_C
//#define _ASX_SIMULATION_C
#define _ASX_NODE_C
#define _ARX_POSE_C
#include "impl/asxImplementation.h"
#include "../render/ddi/arxImpl_Input.h"

_ARX afxError ArxReparentDagNode(arxNode nod, arxNode parent)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_NOD, 1, &nod);
    arxNode curr = AfxGetLinker(&nod->parent);

    if (curr != parent)
    {
        if (curr)
        {
            if (curr == AfxGetHost(nod))
            {
                AFX_ASSERT(curr == AfxGetHost(nod));
                AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &curr);
                AfxPopLink(&nod->parent);
                AFX_ASSERT(nod->flags & arxNodeFlag_ROOT);
                nod->flags &= ~arxNodeFlag_ROOT;
            }
            else
            {
                AFX_ASSERT_OBJECTS(afxFcc_NOD, 1, &curr);
                curr->ltaCnt -= nod->taCnt;
                AfxPopLink(&nod->parent);
            }
        }

        if (parent)
        {
            AFX_ASSERT_OBJECTS(afxFcc_NOD, 1, &parent);
            parent->ltaCnt += nod->taCnt;
            AfxPushLink(&nod->parent, &parent->children);
        }
        else
        {
            afxChain* dags = (afxChain*)_AsxSimGetDagRoots(AfxGetHost(nod));
            AfxPushLink(&nod->parent, dags);
            nod->flags |= arxNodeFlag_ROOT;
        }

        // TODO set flags

        nod->dirty = TRUE;
    }
    return err;
}

_ARX void ArxGetDagWeight(arxNode nod, afxReal* weight, arxTrackMask** trackMask)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_NOD, 1, &nod);
    
    if (weight)
        *weight = nod->weight;

    if (trackMask)
        *trackMask = nod->trackMask;
}

_ARX afxReal ArxResetDagWeight(arxNode nod, afxReal weight)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_NOD, 1, &nod);
    afxReal curr = nod->weight;
    nod->weight = weight;
    return curr;
}

_ARX arxTrackMask* ArxResetDagMask(arxNode nod, arxTrackMask* mask)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_NOD, 1, &nod);

    arxTrackMask* curr = nod->trackMask;

    nod->trackMask = mask;
    nod->ownedTrackMask = FALSE;
    // TODO reacquire/dispose

    return curr;
}

_ARX void ArxGetNodeTransform(arxNode nod, afxTransform* t)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_NOD, 1, &nod);
    AFX_ASSERT(t);
    *t = nod->t;
}

_ARX afxError ArxTransformNode(arxNode nod, afxTransform const* t)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_NOD, 1, &nod);

    afxBool shouldUpdate = !!(nod->t.flags);

    if (!t)
    {
        shouldUpdate = !!(nod->t.flags);
        AfxResetTransform(&nod->t);
    }
    else
    {
        shouldUpdate = TRUE;
        AfxCopyTransform(&nod->t, t);
    }

    if (shouldUpdate)
        nod->dirty = TRUE;

    return err;
}

_ARX void _ArxNodUpdateMatrix(arxNode nod)
{
    afxError err;
    AFX_ASSERT_OBJECTS(afxFcc_NOD, 1, &nod);

    arxNode parent = AfxGetLinker(&nod->parent);

    // We can't update the parent here to avoid retro-recursion. But it should be expected to have parent nodes before their children.

    // If there is a parent but it is not the host.
    if (parent && !(nod->flags & arxNodeFlag_ROOT))
    {
        AFX_ASSERT_OBJECTS(afxFcc_NOD, 1, &parent);
        _ArxNodUpdateMatrix(parent);
        AfxComputeCompositeTransformM4d(&nod->t, nod->m);
        AfxM4dMultiply(nod->w, nod->m, parent->w);
    }
    else
    {
        // if there is not parent, just copy it.
        AfxComputeCompositeTransformM4d(&nod->t, nod->m);
        AfxM4dCopy(nod->w, nod->m);
    }
#if 0
    arxNode child;
    AFX_ITERATE_CHAIN_B2F(AFX_OBJECT(arxNode), child, parent, &nod->children)
    {
        AFX_ASSERT_OBJECTS(afxFcc_NOD, 1, &child);
        _ArxNodUpdateMatrix(child);
    }
#endif
    int a = 1;
}

_ARX afxBool ArxUpdateNode(arxNode nod)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_NOD, 1, &nod);

    if (nod->dirty)
    {
        AfxComputeCompositeTransformM4d(&nod->t, nod->m);

        arxNode p = AfxGetLinker(&nod->parent);

        if (p && !(nod->flags & arxNodeFlag_ROOT))
        {
            AFX_ASSERT_OBJECTS(afxFcc_NOD, 1, &p);
            if (p->dirty) ArxUpdateNode(p);
            AfxM4dMultiplyAffine(nod->w, nod->m, p->w);
        }
        else
        {
            AfxM4dCopyAtm(nod->w, nod->m);
        }
        nod->dirty = 0;
    }
}

_ARX void ArxGetNodeMatrix(arxNode nod, afxM4d m)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_NOD, 1, &nod);
    AFX_ASSERT(m);
    if (nod->dirty) ArxUpdateNode(nod);
    AfxM4dCopy(m, nod->w);
}

_ARX afxError ArxComputeDagMotionVectors(arxNode nod, afxReal secsElapsed, afxBool inv, afxV3d translation, afxV3d rotation)
{
    // AfxQueryDagRootMotionVectors

    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_NOD, 1, &nod);

    switch (nod->type)
    {
    case arxNodeType_Leaf_AnimBlend:
    {
        ArxComputeBodyMotionVectors(nod->bod, secsElapsed, inv, translation, rotation);
        break;
    }
    case arxNodeType_Leaf_Pose:
    {
        AfxV3dCopy(translation, AFX_V4D_IDENTITY);
        AfxV3dCopy(rotation, AFX_V4D_IDENTITY);
        break;
    }
    case arxNodeType_Leaf_Callback:
    {
        if (nod->motionVectorsCb)
        {
            nod->motionVectorsCb(nod->udd, secsElapsed, translation, rotation, inv);
        }
        else
        {
            AfxV3dCopy(translation, AFX_V4D_IDENTITY);
            AfxV3dCopy(rotation, AFX_V4D_IDENTITY);
        }
        break;
    }
    case arxNodeType_Node_Crossfade:
    {
        arxNode first = AFX_REBASE(AfxGetFirstLink(&nod->children), AFX_OBJECT(arxNode), parent);
        arxNode last = AFX_REBASE(AfxGetLastLink(&nod->children), AFX_OBJECT(arxNode), parent);

        if (nod->children.cnt != 1)
        {
            afxReal defWeight  = nod->trackMask->defWeight;

            afxReal f;
            if (nod->trackMask)
                f = (1.0 - defWeight * nod->weightNone + defWeight * nod->weightAll);
            else
                f = nod->weightAll;

            if (f >= 0.001)
            {
                if (f <= 0.99900001)
                {
                    ArxComputeDagMotionVectors(first, secsElapsed, inv, translation, rotation);
                    afxReal fr = 1.0 - f;

                    AfxV3dScale(translation, translation, fr);
                    AfxV3dScale(rotation, rotation, fr);

                    afxV3d t1, r1;
                    ArxComputeDagMotionVectors(last, secsElapsed, inv, t1, r1);
                    AfxV3dMads(translation, f, translation, t1);
                    AfxV3dMads(rotation, f, rotation, r1);
                    break;
                }
                else
                {
                    ArxComputeDagMotionVectors(last, secsElapsed, inv, translation, rotation);
                    break;
                }
            }
        }
        
        ArxComputeDagMotionVectors(first, secsElapsed, inv, translation, rotation);

        break;
    }
    case arxNodeType_Node_WeightedBlend:
    {
        arxNode n = nod;

        while (n->children.cnt == 1)
        {
            n = AFX_REBASE(AfxGetFirstLink(&n->children), AFX_OBJECT(arxNode), parent);
        }

        AfxV3dCopy(translation, AFX_V4D_IDENTITY);
        AfxV3dCopy(rotation, AFX_V4D_IDENTITY);

        afxReal totalWeight = 0.f;

        arxNode it;
        AFX_ITERATE_CHAIN(AFX_OBJECT(arxNode), it, parent, &n->children)
        {
            afxReal w = it->weight;
            if (it->trackMask) w = w * it->trackMask->defWeight;

            if (AFX_ABS(w) >= 0.001)
            {
                afxV3d t1, r1;
                ArxComputeDagMotionVectors(it, secsElapsed, inv, t1, r1);
                AfxV3dMads(translation, w, translation, t1);
                AfxV3dMads(rotation, w, rotation, r1);
                totalWeight = w + totalWeight;
            }
        }

        if (totalWeight > 0.001)
        {
            afxReal twr = 1.0 / totalWeight;
            AfxV3dScale(translation, translation, twr);
            AfxV3dScale(rotation, rotation, twr);
        }

        break;
    }
    default: break;
    }

    return err;
}

_ARX afxError ArxComputeDagMotionMatrix(arxNode nod, afxReal secsElapsed, afxBool inv, afxM4d const mm, afxM4d m)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_NOD, 1, &nod);
    AFX_ASSERT(mm);
    AFX_ASSERT(m);
    
    afxV3d t, r;
    ArxComputeDagMotionVectors(nod, secsElapsed, inv, t, r);
    AfxM4dApplyRigidMotion(m, mm, r, t);

    return err;
}

_ARX afxError ArxStepDag(arxNode nod, afxReal time)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_NOD, 1, &nod);

    switch (nod->type)
    {
    case arxNodeType_Leaf_AnimBlend:
    {
        ArxStepBodyMotives(nod->bod, time);
        break;
    }
    case arxNodeType_Leaf_Callback:
    {
        if (nod->setClockCb)
            nod->setClockCb(nod->udd, time);

        break;
    }
    case arxNodeType_Node_WeightedBlend:
    {
        arxNode ch;
        AFX_ITERATE_CHAIN(AFX_OBJECT(arxNode), ch, parent, &nod->children)
        {
            AFX_ASSERT_OBJECTS(afxFcc_NOD, 1, &ch);
            ArxStepDag(ch, time);
        }
        break;
    }
    default: break;
    }
    return err;
}

_ARX afxError ArxSampleDag(arxNode nod, afxReal allowedErr, arxPose pose, afxUnit jntCnt, afxUnit const sparseJntMap[], void* cache)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_NOD, 1, &nod);

    afxSimulation sim = AfxGetHost(nod);
    arxRenderware rwe = AfxGetSimulationDrawInput(sim);

    switch (nod->type)
    {
    case arxNodeType_Leaf_AnimBlend:
    {
        arxBody bod = nod->bod;
        arxModel mdl;
        ArxGetBodyModel(bod, &mdl);

        afxUnit poseCap = ArxGetPoseCapacity(pose);
        AFX_ASSERT_CAPACITY(poseCap, jntCnt);

        ArxCommencePoseAccumulation(pose, 0, jntCnt, NIL);
        ArxAccumulateBodyAnimations(bod, 0, jntCnt, pose, allowedErr, sparseJntMap);
        ArxConcludePoseAccumulation(pose, 0, jntCnt, mdl, allowedErr, sparseJntMap);
        break;
    }
    case arxNodeType_Leaf_Pose:
    {
        afxUnit poseCap = ArxGetPoseCapacity(pose);
        AFX_ASSERT_CAPACITY(poseCap, jntCnt);
        ArxCopyPose(pose, 0, nod->pose, 0, jntCnt, sparseJntMap);
        break;
    }
    case arxNodeType_Leaf_Callback:
    {
        afxUnit poseCap = ArxGetPoseCapacity(pose);
        AFX_ASSERT_CAPACITY(poseCap, jntCnt);
        nod->sampleCb(nod->udd, allowedErr, pose, jntCnt, sparseJntMap);
        break;
    }
    case arxNodeType_Node_Crossfade:
    {
        afxUnit poseCap = ArxGetPoseCapacity(pose);
        AFX_ASSERT_CAPACITY(poseCap, jntCnt);

        arxNode first = AFX_REBASE(AfxGetFirstLink(&nod->children), AFX_OBJECT(arxNode), parent);
        arxNode last = AFX_REBASE(AfxGetLastLink(&nod->children), AFX_OBJECT(arxNode), parent);

        if (nod->children.cnt == 1)
        {
            ArxSampleDag(first, allowedErr, pose, jntCnt, sparseJntMap, cache);
            return err;
        }

        if (nod->trackMask)
        {
            if (nod->weightNone < 0.001 && nod->weightAll < 0.001)
            {
                ArxSampleDag(first, allowedErr, pose, jntCnt, sparseJntMap, cache);
                return err;
            }

            if (nod->weightNone <= 0.99900001)
                goto L26;
        }
        else if (nod->weightAll < 0.001)
        {
            ArxSampleDag(first, allowedErr, pose, jntCnt, sparseJntMap, cache);
            return err;
        }

        if (nod->weightAll > 0.99900001)
        {
            ArxSampleDag(last, allowedErr, pose, jntCnt, sparseJntMap, cache);
            return err;
        }

        arxPose poseA, poseB;
    L26:
        poseA = first->pose;
        poseB = last->pose;
        ArxSampleDag(first, allowedErr, poseA, jntCnt, sparseJntMap, cache);
        ArxSampleDag(last, allowedErr, poseB, jntCnt, sparseJntMap, cache);

        ArxCopyPose(pose, 0, poseA, 0, jntCnt, NIL);
        ArxModulatePose(pose, 0, poseB, 0, ArxGetPoseCapacity(pose), nod->weightNone, nod->weightAll, nod->trackMask, sparseJntMap);
        break;
    }
    case arxNodeType_Node_WeightedBlend:
    {
        arxNode n = nod;

        while (n->children.cnt == 1)
        {
            n = AFX_REBASE(AfxGetFirstLink(&n->children), AFX_OBJECT(arxNode), parent);
        }

        ArxCommencePoseAccumulation(pose, 0, jntCnt, sparseJntMap);

        arxNode child;
        AFX_ITERATE_CHAIN(AFX_OBJECT(arxNode), child, parent, &n->children)
        {
            afxReal w = child->weight;

            if (AFX_ABS(w) >= 0.001)
            {
                ArxSampleDag(child, allowedErr, child->pose, jntCnt, sparseJntMap, cache);

                for (afxUnit i = 0; i < jntCnt; i++)
                {
                    w = child->weight;

                    afxUnit jntIdx = i;
                    if (sparseJntMap) jntIdx = sparseJntMap[i];

                    arxTrackMask* trackMask = child->trackMask;
                    if (trackMask) w = w * trackMask->boneWeights[jntIdx];

                    ArxAccumulateLocalTransform(pose, i, jntIdx, w, n->skl, n->quatMode, &child->pose->arts[i].xform);
                }
            }
        }

        ArxConcludePoseAccumulation(pose, 0, jntCnt, n->skl, allowedErr, sparseJntMap);

        break;
    }
    default: AfxThrowError(); break;
    }

    return err;
}

_ARX afxError ArxRelinkDagPose(arxNode nod, arxPose pose)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_NOD, 1, &nod);
    arxPose curr = nod->pose;

    if (pose != curr)
    {
        if (curr)
        {
            AFX_ASSERT_OBJECTS(afxFcc_POSE, 1, &curr);
            AfxDisposeObjects(1, &curr);
            nod->flags &= ~arxNodeFlag_POSE;
        }

        nod->pose = pose;

        if (pose)
        {
            AFX_ASSERT_OBJECTS(afxFcc_POSE, 1, &pose);
            AfxReacquireObjects(1, &pose);
            nod->flags |= arxNodeFlag_POSE;
        }
    }
    return err;
}

_ASXINL afxError _AsxNodDtorCb(arxNode nod)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_NOD, 1, &nod);

    arxNodular* nodr;
    AFX_ITERATE_CHAIN(arxNodular, nodr, nod, &nod->nodulars)
    {
        //AFX_ASSERT_OBJECTS(afxFcc_NOD, 1, &nodr);
        
        AfxPopLink(&nodr->nod);
    }

    arxNode child;
    AFX_ITERATE_CHAIN(AFX_OBJECT(arxNode), child, parent, &nod->children)
    {
        AFX_ASSERT_OBJECTS(afxFcc_NOD, 1, &child);
        ArxReparentDagNode(child, NIL);
    }

    ArxReparentDagNode(nod, NIL);

    switch (nod->type)
    {
    case arxNodeType_Leaf_AnimBlend:
    {
        break;
    }
    case arxNodeType_Leaf_Pose:
    {
        break;
    }
    case arxNodeType_Leaf_Callback:
    {
        break;
    }
    case arxNodeType_Node_Crossfade:
    {
        break;
    }
    case arxNodeType_Node_WeightedBlend:
    {
        break;
    }
    default:
        break;
    }

    return err;
}

_ASXINL afxError _AsxNodCtorCb(arxNode nod, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_NOD, 1, &nod);

    afxSimulation sim = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);

    AfxDeployChain(&nod->children, nod);
    AfxResetLink(&nod->parent);
    
    afxChain* dags = (afxChain*)_AsxSimGetDagRoots(AfxGetHost(nod));
    AfxPushLink(&nod->parent, dags);
    nod->flags = arxNodeFlag_ROOT;

    AfxDeployChain(&nod->nodulars, nod);

    nod->ownedTrackMask = FALSE;
    nod->trackMask = NIL;
    nod->weight = 1.f;
    nod->type = 6;

    ///////////
    AfxResetTransform(&nod->t);
    AfxM4dCopy(nod->m, AFX_M4D_IDENTITY);
    AfxM4dCopy(nod->w, AFX_M4D_IDENTITY);
    nod->dirty = 0;
    nod->taCnt = 0;
    nod->ltaCnt = 0;

    return err;
}

_ARX afxClassConfig const _ASX_NOD_CLASS_CONFIG =
{
    .fcc = afxFcc_NOD,
    .name = "Node",
    .desc = "Node",
    .fixedSiz = sizeof(AFX_OBJECT(arxNode)),
    .ctor = (void*)_AsxNodCtorCb,
    .dtor = (void*)_AsxNodDtorCb
};

_ASXINL afxError ArxAcquireNodes(afxSimulation sim, afxUnit cnt, arxNode nod[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);

    if (AfxAcquireObjects((afxClass *)_ArxGetNodeClass(sim), cnt, (afxObject*)nod, (void const*[]) { sim }))
        AfxThrowError();

    AFX_ASSERT_OBJECTS(afxFcc_NOD, cnt, nod);

    return err;
}

_ARX afxError ArxAcquireJunctionNode(afxSimulation sim, arxNode parent, arxModel skl, afxQuatBlend blendOp, afxReal fillThreshold, arxNode* node)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);

    arxNode nod;
    ArxAcquireNodes(sim, 1, &nod);

    nod->type = arxNodeType_Node_WeightedBlend;

    AfxReacquireObjects(1, &skl);
    nod->skl = skl;
    nod->quatMode = blendOp;
    nod->fillThreshold = fillThreshold;

    ArxReparentDagNode(nod, parent);

    AFX_ASSERT(node);
    *node = nod;

    return err;
}

_ARX afxError ArxAcquireCrossfadeNode(afxSimulation sim, arxNode parent, arxNode a, arxNode b, afxReal weightNone, afxReal weightAll, arxTrackMask* trackMask, arxNode* node)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);

    arxNode nod;
    ArxAcquireNodes(sim, 1, &nod);

    nod->type = arxNodeType_Node_Crossfade;

    if (a)
    {
        ArxReparentDagNode(a, nod);
    }

    if (b)
    {
        ArxReparentDagNode(b, nod);
    }

    nod->weightNone = weightNone;
    nod->weightAll = weightAll;
    nod->trackMask = trackMask;
    nod->ownedTrackMask = FALSE;

    ArxReparentDagNode(nod, parent);

    AFX_ASSERT(node);
    *node = nod;

    return err;
}

_ARX afxError ArxAcquireCallbackNode(afxSimulation sim, arxNode parent, afxError(*sample)(void*, afxReal, arxPose, afxUnit, afxUnit const*), void(*setClock)(void*, afxReal), void(*motionVectors)(void*, afxReal, afxReal*, afxReal*, afxBool), void* udd, arxNode* node)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);

    arxNode nod;
    ArxAcquireNodes(sim, 1, &nod);

    nod->type = arxNodeType_Leaf_Callback;

    nod->sampleCb = sample;
    nod->setClockCb = setClock;
    nod->motionVectorsCb = motionVectors;
    nod->udd = udd;

    ArxReparentDagNode(nod, parent);

    AFX_ASSERT(node);
    *node = nod;

    return err;
}

_ARX afxError ArxAcquirePoseNode(afxSimulation sim, arxNode parent, arxPose pose, arxNode* node)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);

    arxNode nod;
    ArxAcquireNodes(sim, 1, &nod);

    nod->type = arxNodeType_Leaf_Pose;

    AfxReacquireObjects(1, &pose);
    nod->pose = pose;
    //nod->pose.owned = TRUE;

    ArxReparentDagNode(nod, parent);

    AFX_ASSERT(node);
    *node = nod;

    return err;
}

_ARX afxError ArxAcquireAnimationNode(afxSimulation sim, arxNode parent, arxBody bod, afxReal fillThreshold, arxNode* node)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);

    arxNode nod;
    ArxAcquireNodes(sim, 1, &nod);

    nod->type = arxNodeType_Leaf_AnimBlend;

    AfxReacquireObjects(1, &bod);
    nod->bod = bod;
    nod->fillThreshold = fillThreshold; // 0.2
    //nod->animBlend.owned = TRUE;

    ArxReparentDagNode(nod, parent);

    AFX_ASSERT(node);
    *node = nod;

    return err;
}

_ARX afxError ArxAcquirePlacementNode(afxSimulation sim, arxNode parent, arxPlacement plce, arxNode* node)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);

    arxNode nod;
    ArxAcquireNodes(sim, 1, &nod);

    nod->type = arxNodeType_OnePastLast;

    if (plce)
    {
        AfxReacquireObjects(1, &plce);
    }
    else
    {
        afxUnit artCnt = 256;
        //ArxAcquirePlacements(sim, 1, &artCnt, NIL, &plce);
    }
    //nod->plce = plce;
    //nod->pose.owned = TRUE;

    ArxReparentDagNode(nod, parent);

    AFX_ASSERT(node);
    *node = nod;

    return err;
}

_ARX afxBool _ArxCaptureNodCb(arxNode nod, void** udd)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_NOD, 1, &nod);

    afxFrustum* frustum = udd[0];
    afxReal lodErr = *(afxReal*)udd[1];
    afxArray* pvs = udd[2];

    //AfxDoesFrustumCullAabbs(&frustum, 1, &aabb);

    if (1) // do visibility culling
    {
        afxUnit arrel;

        if (!AfxPushArrayUnits(pvs, 1, &arrel, &nod, sizeof(nod))) AfxThrowError();
        else
        {

        }
    }

    return TRUE;
}

_ARX afxError ArxCaptureNodes(afxSimulation sim, afxBool(*node)(arxNode nod, void *udd), afxArray* pvs)
{
    afxError err = AFX_ERR_NONE;
    // sim must be a valid afxSimulation handle.
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);
    AFX_ASSERT(pvs);
    AFX_ASSERT(pvs->cap);
    AFX_ASSERT(node);

    // TODO: Move to implementation

    afxReal lodErr = 0.f;

    if (!node)
        node = (void*)_ArxCaptureNodCb;

    void** udd = (void*[]) { node, &lodErr, pvs };

    arxNode root;
    afxChain const* dags = _AsxSimGetDagRoots(sim);
    AFX_ITERATE_CHAIN_B2F(AFX_OBJECT(arxNode), root, parent, dags)
    {
        AFX_ASSERT_OBJECTS(afxFcc_NOD, 1, &root);

        if (!node(root, udd))
            continue;

        arxNode nod;
        AFX_ITERATE_CHAIN(AFX_OBJECT(arxNode), nod, parent, &root->children)
        {
            AFX_ASSERT_OBJECTS(afxFcc_NOD, 1, &nod);

            if (!node(nod, udd))
                continue;
        }
    }
    return err;
}

_ARX afxError ArxCaptureNodesInFrustum(afxSimulation sim, afxFrustum const* f, afxArray* pvs)
{
    afxError err = AFX_ERR_NONE;
    // sim must be a valid afxSimulation handle.
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);
    AFX_ASSERT(pvs);
    AFX_ASSERT(pvs->cap);
    AFX_ASSERT(f);

    arxNode root;
    afxChain const* dags = _AsxSimGetDagRoots(sim);
    AFX_ITERATE_CHAIN_B2F(AFX_OBJECT(arxNode), root, parent, dags)
    {
        AFX_ASSERT_OBJECTS(afxFcc_NOD, 1, &root);
        
        // Test volume in frustum
        //if (0) continue;

        afxUnit arrel;
        if (!AfxPushArrayUnits(pvs, 1, &arrel, &root, sizeof(root)))
            AfxThrowError();

        arxNode nod;
        AFX_ITERATE_CHAIN(AFX_OBJECT(arxNode), nod, parent, &root->children)
        {
            AFX_ASSERT_OBJECTS(afxFcc_NOD, 1, &nod);

            afxUnit arrel2;
            if (!AfxPushArrayUnits(pvs, 1, &arrel2, &nod, sizeof(nod)))
                AfxThrowError();

            arxNode child;
            AFX_ITERATE_CHAIN2(AFX_OBJECT(arxNode), child, parent, &nod->children)
            {
                AFX_ASSERT_OBJECTS(afxFcc_NOD, 1, &child);

                afxUnit arrel3;
                if (!AfxPushArrayUnits(pvs, 1, &arrel3, &child, sizeof(child)))
                    AfxThrowError();
            }
        }
    }
    return err;
}

_ARX afxError ArxCaptureNodesInSphere(afxSimulation sim, afxSphere const* area, afxArray* pvs)
{
    afxError err = AFX_ERR_NONE;
    // sim must be a valid afxSimulation handle.
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);
    AFX_ASSERT(pvs);
    AFX_ASSERT(pvs->cap);
    AFX_ASSERT(area);

    arxNode root;
    afxChain const* dags = _AsxSimGetDagRoots(sim);
    AFX_ITERATE_CHAIN_B2F(AFX_OBJECT(arxNode), root, parent, dags)
    {
        AFX_ASSERT_OBJECTS(afxFcc_NOD, 1, &root);

        if (-1 == AfxTestSphereEnglobingPoint(area, root->w[3]))
            continue;

        afxUnit arrel;
        if (!AfxPushArrayUnits(pvs, 1, &arrel, &root, sizeof(root)))
            AfxThrowError();

        arxNode nod;
        AFX_ITERATE_CHAIN(AFX_OBJECT(arxNode), nod, parent, &root->children)
        {
            AFX_ASSERT_OBJECTS(afxFcc_NOD, 1, &nod);

            if (-1 == AfxTestSphereEnglobingPoint(area, nod->w[3]))
                continue;

            afxUnit arrel2;
            if (!AfxPushArrayUnits(pvs, 1, &arrel2, &nod, sizeof(nod)))
                AfxThrowError();

            arxNode child;
            AFX_ITERATE_CHAIN2(AFX_OBJECT(arxNode), child, parent, &nod->children)
            {
                AFX_ASSERT_OBJECTS(afxFcc_NOD, 1, &child);

                if (-1 == AfxTestSphereEnglobingPoint(area, child->w[3]))
                    continue;

                afxUnit arrel3;
                if (!AfxPushArrayUnits(pvs, 1, &arrel3, &child, sizeof(child)))
                    AfxThrowError();
            }
        }
    }
}

_ARX void _ArxNodUpdatePose(arxNode nod)
{
    afxError err;
    AFX_ASSERT_OBJECTS(afxFcc_NOD, 1, &nod);

}

_ARX void _ArxNodUpdatePlacement(arxNode nod, afxReal allowedErr)
{
    afxError err;
    AFX_ASSERT_OBJECTS(afxFcc_NOD, 1, &nod);

    if (nod->type == arxNodeType_Leaf_AnimBlend)
    {
        arxBody bod = nod->bod;
        arxModel skl;
        ArxGetBodyModel(bod, &skl);

        arxPose pose;
        arxPlacement plce;
        ArxGetBodyPose(bod, &pose, &plce);

        afxUnit jntCnt = ArxCountJoints(skl, 0);
        afxReal sampleLodErr = 0.0;
        afxUnit sampledJntCnt = ArxCountJoints(skl, sampleLodErr);

        if (!ArxCountBodyMotives(bod))
        {
            ArxBuildPlacement(plce, NIL, skl, 0, jntCnt, 0, jntCnt, nod->w, FALSE);
        }
        else
        {
            //afxReal allowedErr = 0.f;
            afxUnit lodJntCnt = ArxCountJoints(skl, allowedErr);
            ArxCommencePoseAccumulation(pose, 0, jntCnt, NIL);
            ArxAccumulateBodyAnimations(bod, 0, jntCnt, pose, allowedErr, NIL);
            ArxConcludePoseAccumulation(pose, 0, jntCnt, skl, allowedErr, NIL);
            ArxBuildPlacement(plce, pose, skl, 0, jntCnt, 0, lodJntCnt, nod->w, FALSE);
        }
    }
}

_ARX afxError ArxUpdateDags(afxReal allowedErr, afxBool matrices, afxBool poses, afxBool placements, afxUnit cnt, arxNode nodes[])
{
    afxError err;
    AFX_ASSERT(nodes);
    AFX_ASSERT(cnt);

    for (afxUnit i = 0; i < cnt; i++)
    {
        arxNode nod = nodes[i];
        if (!nod) continue;
        AFX_ASSERT_OBJECTS(afxFcc_NOD, 1, &nod);
        
        if (nod->dirty)
        {
            //_ArxNodUpdateMatrix(nod);
            ArxUpdateNode(nod);
            _ArxNodUpdatePlacement(nod, allowedErr);
        }
    }
    return err;
}
