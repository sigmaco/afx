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
#define _ASX_POSE_C
#include "impl/asxImplementation.h"

_ASX void AsxGetDagWeight(afxNode nod, afxReal* weight, akxTrackMask** trackMask)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_NOD, 1, &nod);
    
    if (weight)
        *weight = nod->weight;

    if (trackMask)
        *trackMask = nod->trackMask;
}

_ASX afxReal AsxResetDagWeight(afxNode nod, afxReal weight)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_NOD, 1, &nod);
    afxReal curr = nod->weight;
    nod->weight = weight;
    return curr;
}

_ASX akxTrackMask* AsxResetDagMask(afxNode nod, akxTrackMask* mask)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_NOD, 1, &nod);

    akxTrackMask* curr = nod->trackMask;

    nod->trackMask = mask;
    nod->ownedTrackMask = FALSE;
    // TODO reacquire/dispose

    return curr;
}

_ASX void AsxGetNodeTransform(afxNode nod, afxTransform* t)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_NOD, 1, &nod);
    AFX_ASSERT(t);
    *t = nod->t;
}

_ASX afxBool AsxUpdateNode(afxNode nod)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_NOD, 1, &nod);

    if (nod->dirty)
    {
        AfxComputeCompositeTransformM4d(&nod->t, nod->m);

        afxNode p = AfxGetLinker(&nod->parent);

        if (p)
        {
            if (p->dirty) AsxUpdateNode(p);
            AfxM4dMultiplyAffine(nod->w, nod->m, p->w);
        }
        else
        {
            AfxM4dCopyAtm(nod->w, nod->m);
        }
        nod->dirty = 0;
    }
}

_ASX void AsxGetNodeMatrix(afxNode nod, afxM4d m)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_NOD, 1, &nod);
    AFX_ASSERT(m);
    if (nod->dirty) AsxUpdateNode(nod);
    AfxM4dCopy(m, nod->w);
}

_ASX afxError AsxTransformNode(afxNode nod, afxTransform const* t)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_NOD, 1, &nod);

    afxBool shouldUpdate = !!(nod->t.flags);

    if (!t)
    {
        AfxResetTransform(&nod->t);
    }
    else
    {
        AfxCopyTransform(&nod->t, t);
    }

    if (shouldUpdate)
        nod->dirty = TRUE;

    return err;
}

_ASX afxError AsxReparentDagNode(afxNode nod, afxNode parent)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_NOD, 1, &nod);
    afxNode curr = AfxGetLinker(&nod->parent);

    if (curr != parent)
    {
        if (curr)
        {
            if (curr == AfxGetProvider(nod))
            {
                AfxPopLink(&nod->parent);
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
            afxChain* dags = (afxChain*)_AsxSimGetDagRoots(AfxGetProvider(nod));
            AfxPushLink(&nod->parent, dags);
        }

        // TODO set flags

        nod->dirty = TRUE;
    }
    return err;
}

_ASX afxError AsxComputeDagMotionVectors(afxNode nod, afxReal secsElapsed, afxBool inv, afxV3d translation, afxV3d rotation)
{
    // AfxQueryDagRootMotionVectors

    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_NOD, 1, &nod);

    switch (nod->type)
    {
    case asxNodeType_Leaf_AnimBlend:
    {
        AsxComputeBodyMotionVectors(nod->animBlend.bod, secsElapsed, inv, translation, rotation);
        break;
    }
    case asxNodeType_Leaf_Pose:
    {
        AfxV3dCopy(translation, AFX_V4D_IDENTITY);
        AfxV3dCopy(rotation, AFX_V4D_IDENTITY);
        break;
    }
    case asxNodeType_Leaf_Callback:
    {
        if (nod->cb.motionVectors)
        {
            nod->cb.motionVectors(nod->cb.udd, secsElapsed, translation, rotation, inv);
        }
        else
        {
            AfxV3dCopy(translation, AFX_V4D_IDENTITY);
            AfxV3dCopy(rotation, AFX_V4D_IDENTITY);
        }
        break;
    }
    case asxNodeType_Node_Crossfade:
    {
        afxNode first = AFX_REBASE(AfxGetFirstLink(&nod->children), AFX_OBJECT(afxNode), parent);
        afxNode last = AFX_REBASE(AfxGetLastLink(&nod->children), AFX_OBJECT(afxNode), parent);

        if (nod->children.cnt != 1)
        {
            afxReal defWeight  = nod->crossfade.trackMask->defWeight;

            afxReal f;
            if (nod->crossfade.trackMask)
                f = (1.0 - defWeight * nod->crossfade.weightNone + defWeight * nod->crossfade.weightAll);
            else
                f = nod->crossfade.weightAll;

            if (f >= 0.001)
            {
                if (f <= 0.99900001)
                {
                    AsxComputeDagMotionVectors(first, secsElapsed, inv, translation, rotation);
                    afxReal fr = 1.0 - f;

                    AfxV3dScale(translation, translation, fr);
                    AfxV3dScale(rotation, rotation, fr);

                    afxV3d t1, r1;
                    AsxComputeDagMotionVectors(last, secsElapsed, inv, t1, r1);
                    AfxV3dMads(translation, f, translation, t1);
                    AfxV3dMads(rotation, f, rotation, r1);
                    break;
                }
                else
                {
                    AsxComputeDagMotionVectors(last, secsElapsed, inv, translation, rotation);
                    break;
                }
            }
        }
        
        AsxComputeDagMotionVectors(first, secsElapsed, inv, translation, rotation);

        break;
    }
    case asxNodeType_Node_WeightedBlend:
    {
        afxNode n = nod;

        while (n->children.cnt == 1)
        {
            n = AFX_REBASE(AfxGetFirstLink(&n->children), AFX_OBJECT(afxNode), parent);
        }

        AfxV3dCopy(translation, AFX_V4D_IDENTITY);
        AfxV3dCopy(rotation, AFX_V4D_IDENTITY);

        afxReal totalWeight = 0.f;

        afxNode it;
        AFX_ITERATE_CHAIN(AFX_OBJECT(afxNode), it, parent, &n->children)
        {
            afxReal w = it->weight;
            if (it->trackMask) w = w * it->trackMask->defWeight;

            if (AFX_ABS(w) >= 0.001)
            {
                afxV3d t1, r1;
                AsxComputeDagMotionVectors(it, secsElapsed, inv, t1, r1);
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

_ASX afxError AsxComputeDagMotionMatrix(afxNode nod, afxReal secsElapsed, afxBool inv, afxM4d const mm, afxM4d m)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_NOD, 1, &nod);
    AFX_ASSERT(mm);
    AFX_ASSERT(m);
    
    afxV3d t, r;
    AsxComputeDagMotionVectors(nod, secsElapsed, inv, t, r);
    AfxM4dApplyRigidMotion(m, mm, r, t);

    return err;
}

_ASX afxError AsxStepDag(afxNode nod, afxReal clock)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_NOD, 1, &nod);

    switch (nod->type)
    {
    case asxNodeType_Leaf_AnimBlend:
    {
        AfxStepBodyMotives(nod->animBlend.bod, clock);
        break;
    }
    case asxNodeType_Leaf_Callback:
    {
        if (nod->cb.setClock)
            nod->cb.setClock(nod->cb.udd, clock);

        break;
    }
    case asxNodeType_Node_WeightedBlend:
    {
        afxNode ch;
        AFX_ITERATE_CHAIN(AFX_OBJECT(afxNode), ch, parent, &nod->children)
        {
            AFX_ASSERT_OBJECTS(afxFcc_NOD, 1, &ch);
            AsxStepDag(ch, clock);
        }
        break;
    }
    default: break;
    }
    return err;
}

_ASX afxError AsxSampleDag(afxNode nod, afxUnit jntCnt, afxReal allowedErr, afxUnit const sparseJntMap[], void* cache, afxPose* attitude)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_NOD, 1, &nod);

    afxSimulation sim = AfxGetProvider(nod);

    switch (nod->type)
    {
    case asxNodeType_Leaf_AnimBlend:
    {
        afxBody bod = nod->animBlend.bod;
        afxModel mdl;
        AfxGetBodyModel(bod, &mdl);

        afxPose pose;
        AfxAcquirePoses(sim, 1, &jntCnt, &pose);
        {
            AsxCommencePoseAccumulation(pose, 0, jntCnt, NIL);
            AfxAccumulateBodyAnimations(bod, 0, jntCnt, pose, allowedErr, sparseJntMap);
            AsxConcludePoseAccumulation(pose, 0, jntCnt, mdl, allowedErr, sparseJntMap);
        }
        *attitude = pose;
        break;
    }
    case asxNodeType_Leaf_Pose:
    {
        if (!sparseJntMap)
        {
            AfxReacquireObjects(1, &nod->pose.pose);
            *attitude = nod->pose.pose;
        }
        else
        {
            afxPose pose;
            AfxAcquirePoses(sim, 1, &jntCnt, &pose);
            AsxCopyPose(pose, 0, nod->pose.pose, 0, jntCnt, sparseJntMap);
            *attitude = pose;
        }
        break;
    }
    case asxNodeType_Leaf_Callback:
    {
        *attitude = nod->cb.sample(nod->cb.udd, jntCnt, allowedErr, sparseJntMap);
        break;
    }
    case asxNodeType_Node_Crossfade:
    {
        afxNode first = AFX_REBASE(AfxGetFirstLink(&nod->children), AFX_OBJECT(afxNode), parent);
        afxNode last = AFX_REBASE(AfxGetLastLink(&nod->children), AFX_OBJECT(afxNode), parent);

        if (nod->children.cnt == 1)
        {
            AsxSampleDag(first, jntCnt, allowedErr, sparseJntMap, cache, attitude);
            return err;
        }

        if (nod->crossfade.trackMask)
        {
            if (nod->crossfade.weightNone < 0.001 && nod->crossfade.weightAll < 0.001)
            {
                AsxSampleDag(first, jntCnt, allowedErr, sparseJntMap, cache, attitude);
                return err;
            }

            if (nod->crossfade.weightNone <= 0.99900001)
                goto L26;
        }
        else if (nod->crossfade.weightAll < 0.001)
        {
            AsxSampleDag(first, jntCnt, allowedErr, sparseJntMap, cache, attitude);
            return err;
        }

        if (nod->crossfade.weightAll > 0.99900001)
        {
            AsxSampleDag(last, jntCnt, allowedErr, sparseJntMap, cache, attitude);
            return err;
        }

        afxPose poseA, poseB;
    L26:
        AsxSampleDag(first, jntCnt, allowedErr, sparseJntMap, cache, &poseA);
        AsxSampleDag(last, jntCnt, allowedErr, sparseJntMap, cache, &poseB);

        afxBool isTempPoseA = TRUE;
        afxBool isTempPoseB = TRUE;

        if (isTempPoseA)
        {
            AsxModulatePose(poseA, 0, poseB, 0, AsxGetPoseCapacity(poseA), nod->crossfade.weightNone, nod->crossfade.weightAll, nod->crossfade.trackMask, sparseJntMap);
            *attitude = poseA;
        }
        else if (isTempPoseB)
        {
            AsxModulatePose(poseB, 0, poseA, 0, AsxGetPoseCapacity(poseB), 1.f - nod->crossfade.weightNone, 1.f - nod->crossfade.weightAll, nod->crossfade.trackMask, sparseJntMap);
            *attitude = poseB;
        }
        else
        {
            afxPose pose;
            AfxAcquirePoses(sim, 1, &jntCnt, &pose);
            AsxCopyPose(pose, 0, poseA, 0, jntCnt, NIL);
            AsxModulatePose(pose, 0, poseB, 0, AsxGetPoseCapacity(pose), nod->crossfade.weightNone, nod->crossfade.weightAll, nod->crossfade.trackMask, sparseJntMap);
            *attitude = pose;
            AfxDisposeObjects(1, &poseA);
            AfxDisposeObjects(1, &poseB);
        }
        break;
    }
    case asxNodeType_Node_WeightedBlend:
    {
        afxNode n = nod;

        while (n->children.cnt == 1)
        {
            n = AFX_REBASE(AfxGetFirstLink(&n->children), AFX_OBJECT(afxNode), parent);
        }

        afxPose pose;
        AfxAcquirePoses(sim, 1, &jntCnt, &pose);

        pose->fillThreshold = nod->weightedBlend.fillThreshold;
        AsxCommencePoseAccumulation(pose, 0, jntCnt, sparseJntMap);

        afxNode child;
        AFX_ITERATE_CHAIN(AFX_OBJECT(afxNode), child, parent, &n->children)
        {
            afxReal w = child->weight;

            if (AFX_ABS(w) >= 0.001)
            {
                afxPose pose2;
                AsxSampleDag(child, jntCnt, allowedErr, sparseJntMap, cache, pose2);

                for (afxUnit i = 0; i < jntCnt; i++)
                {
                    w = child->weight;

                    afxUnit jntIdx = i;
                    if (sparseJntMap) jntIdx = sparseJntMap[i];

                    akxTrackMask* trackMask = child->trackMask;
                    if (trackMask) w = w * trackMask->boneWeights[jntIdx];

                    AsxAccumulateLocalTransform(pose, i, jntIdx, w, n->weightedBlend.skl, n->weightedBlend.quatMode, &pose2->arts[i].xform);
                }

                AfxDisposeObjects(1, &pose2);
            }
        }

        AsxConcludePoseAccumulation(pose, 0, jntCnt, n->weightedBlend.skl, allowedErr, sparseJntMap);
        *attitude = pose;

        break;
    }
    default: AfxThrowError(); break;
    }

    return err;
}

_ASXINL afxError _AsxNodDtorCb(afxNode nod)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_NOD, 1, &nod);

    afxNodular* nodr;
    AFX_ITERATE_CHAIN(afxNodular, nodr, nod, &nod->nodulars)
    {
        //AFX_ASSERT_OBJECTS(afxFcc_NOD, 1, &nodr);
        
        AfxPopLink(&nodr->nod);
    }

    afxNode child;
    AFX_ITERATE_CHAIN(AFX_OBJECT(afxNode), child, parent, &nod->children)
    {
        AFX_ASSERT_OBJECTS(afxFcc_NOD, 1, &child);
        AsxReparentDagNode(child, NIL);
    }

    AsxReparentDagNode(nod, NIL);

    switch (nod->type)
    {
    case asxNodeType_Leaf_AnimBlend:
    {
        break;
    }
    case asxNodeType_Leaf_Pose:
    {
        break;
    }
    case asxNodeType_Leaf_Callback:
    {
        break;
    }
    case asxNodeType_Node_Crossfade:
    {
        break;
    }
    case asxNodeType_Node_WeightedBlend:
    {
        break;
    }
    default:
        break;
    }

    return err;
}

_ASXINL afxError _AsxNodCtorCb(afxNode nod, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_NOD, 1, &nod);

    afxSimulation sim = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);

    AfxDeployChain(&nod->children, nod);
    AfxResetLink(&nod->parent);
    afxChain* dags = (afxChain*)_AsxSimGetDagRoots(AfxGetProvider(nod));
    AfxPushLink(&nod->parent, dags);

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

_ASX afxClassConfig const _ASX_NOD_CLASS_CONFIG =
{
    .fcc = afxFcc_NOD,
    .name = "Node",
    .desc = "Node",
    .fixedSiz = sizeof(AFX_OBJECT(afxNode)),
    .ctor = (void*)_AsxNodCtorCb,
    .dtor = (void*)_AsxNodDtorCb
};

_ASXINL afxError AfxAcquireNodes(afxSimulation sim, afxUnit cnt, afxNode nod[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);

    if (AfxAcquireObjects((afxClass *)_AsxGetNodeClass(sim), cnt, (afxObject*)nod, (void const*[]) { sim }))
        AfxThrowError();

    AFX_ASSERT_OBJECTS(afxFcc_NOD, cnt, nod);

    return err;
}

_ASX afxError AsxAcquireJunctionNode(afxSimulation sim, afxNode parent, afxModel skl, afxQuatBlend blendOp, afxReal fillThreshold, afxNode* node)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);

    afxNode nod;
    AfxAcquireNodes(sim, 1, &nod);

    nod->type = asxNodeType_Node_WeightedBlend;

    AfxReacquireObjects(1, &skl);
    nod->weightedBlend.skl = skl;
    nod->weightedBlend.quatMode = blendOp;
    nod->weightedBlend.fillThreshold = fillThreshold;

    AsxReparentDagNode(nod, parent);

    AFX_ASSERT(node);
    *node = nod;

    return err;
}

_ASX afxError AsxAcquireCrossfadeNode(afxSimulation sim, afxNode parent, afxNode a, afxNode b, afxReal weightNone, afxReal weightAll, akxTrackMask* trackMask, afxNode* node)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);

    afxNode nod;
    AfxAcquireNodes(sim, 1, &nod);

    nod->type = asxNodeType_Node_Crossfade;

    if (a)
    {
        AsxReparentDagNode(a, nod);
    }

    if (b)
    {
        AsxReparentDagNode(b, nod);
    }

    nod->crossfade.weightNone = weightNone;
    nod->crossfade.weightAll = weightAll;
    nod->crossfade.trackMask = trackMask;
    nod->crossfade.owned = FALSE;

    AsxReparentDagNode(nod, parent);

    AFX_ASSERT(node);
    *node = nod;

    return err;
}

_ASX afxError AsxAcquireCallbackNode(afxSimulation sim, afxNode parent, afxPose(*sample)(void*, afxInt, afxReal, afxInt const*), void(*setClock)(void*, afxReal), void(*motionVectors)(void*, afxReal, afxReal*, afxReal*, afxBool), void* udd, afxNode* node)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);

    afxNode nod;
    AfxAcquireNodes(sim, 1, &nod);

    nod->type = asxNodeType_Leaf_Callback;

    nod->cb.sample = sample;
    nod->cb.setClock = setClock;
    nod->cb.motionVectors = motionVectors;
    nod->cb.udd = udd;

    AsxReparentDagNode(nod, parent);

    AFX_ASSERT(node);
    *node = nod;

    return err;
}

_ASX afxError AsxAcquirePoseNode(afxSimulation sim, afxNode parent, afxPose pose, afxNode* node)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);

    afxNode nod;
    AfxAcquireNodes(sim, 1, &nod);

    nod->type = asxNodeType_Leaf_Pose;

    AfxReacquireObjects(1, &pose);
    nod->pose.pose = pose;
    //nod->pose.owned = TRUE;

    AsxReparentDagNode(nod, parent);

    AFX_ASSERT(node);
    *node = nod;

    return err;
}

_ASX afxError AsxAcquireAnimationNode(afxSimulation sim, afxNode parent, afxBody bod, afxReal fillThreshold, afxNode* node)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);

    afxNode nod;
    AfxAcquireNodes(sim, 1, &nod);

    nod->type = asxNodeType_Leaf_AnimBlend;

    AfxReacquireObjects(1, &bod);
    nod->animBlend.bod = bod;
    nod->animBlend.fillThreshold = fillThreshold; // 0.2
    //nod->animBlend.owned = TRUE;

    AsxReparentDagNode(nod, parent);

    AFX_ASSERT(node);
    *node = nod;

    return err;
}

_ASX afxError AsxAcquirePlacementNode(afxSimulation sim, afxNode parent, afxPlacement plce, afxNode* node)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);

    afxNode nod;
    AfxAcquireNodes(sim, 1, &nod);

    nod->type = asxNodeType_OnePastLast;

    if (plce)
    {
        AfxReacquireObjects(1, &plce);
    }
    else
    {
        afxUnit artCnt = 256;
        //AfxAcquirePlacements(sim, 1, &artCnt, NIL, &plce);
    }
    //nod->plce = plce;
    //nod->pose.owned = TRUE;

    AsxReparentDagNode(nod, parent);

    AFX_ASSERT(node);
    *node = nod;

    return err;
}

_ASX afxBool _AsxCaptureNodCb(afxNode nod, void** udd)
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

_ASX afxError AsxCaptureNodes(afxSimulation sim, afxBool(*node)(afxNode nod, void *udd), afxArray* pvs)
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
        node = (void*)_AsxCaptureNodCb;

    void** udd = (void*[]) { node, &lodErr, pvs };

    afxNode root;
    afxChain const* dags = _AsxSimGetDagRoots(sim);
    AFX_ITERATE_CHAIN_B2F(AFX_OBJECT(afxNode), root, parent, dags)
    {
        AFX_ASSERT_OBJECTS(afxFcc_NOD, 1, &root);

        if (!node(root, udd))
            continue;

        afxNode nod;
        AFX_ITERATE_CHAIN(AFX_OBJECT(afxNode), nod, parent, &root->children)
        {
            AFX_ASSERT_OBJECTS(afxFcc_NOD, 1, &nod);

            if (!node(nod, udd))
                continue;
        }
    }
    return err;
}

_ASX afxError AsxCaptureNodesInFrustum(afxSimulation sim, afxFrustum const* f, afxArray* pvs)
{
    afxError err = AFX_ERR_NONE;
    // sim must be a valid afxSimulation handle.
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);
    AFX_ASSERT(pvs);
    AFX_ASSERT(pvs->cap);
    AFX_ASSERT(f);

    afxNode root;
    afxChain const* dags = _AsxSimGetDagRoots(sim);
    AFX_ITERATE_CHAIN_B2F(AFX_OBJECT(afxNode), root, parent, dags)
    {
        AFX_ASSERT_OBJECTS(afxFcc_NOD, 1, &root);
        
        // Test volume in frustum
        //if (0) continue;

        afxUnit arrel;
        if (!AfxPushArrayUnits(pvs, 1, &arrel, &root, sizeof(root)))
            AfxThrowError();

        afxNode nod;
        AFX_ITERATE_CHAIN(AFX_OBJECT(afxNode), nod, parent, &root->children)
        {
            AFX_ASSERT_OBJECTS(afxFcc_NOD, 1, &nod);

            afxUnit arrel2;
            if (!AfxPushArrayUnits(pvs, 1, &arrel2, &nod, sizeof(nod)))
                AfxThrowError();

            afxNode child;
            AFX_ITERATE_CHAIN2(AFX_OBJECT(afxNode), child, parent, &nod->children)
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

_ASX afxError AfxCaptureNodesInSphere(afxSimulation sim, afxSphere const* area, afxArray* pvs)
{
    afxError err = AFX_ERR_NONE;
    // sim must be a valid afxSimulation handle.
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);
    AFX_ASSERT(pvs);
    AFX_ASSERT(pvs->cap);
    AFX_ASSERT(area);

    afxNode root;
    afxChain const* dags = _AsxSimGetDagRoots(sim);
    AFX_ITERATE_CHAIN_B2F(AFX_OBJECT(afxNode), root, parent, dags)
    {
        AFX_ASSERT_OBJECTS(afxFcc_NOD, 1, &root);

        if (-1 == AfxTestSphereEnglobingPoint(area, root->w[3]))
            continue;

        afxUnit arrel;
        if (!AfxPushArrayUnits(pvs, 1, &arrel, &root, sizeof(root)))
            AfxThrowError();

        afxNode nod;
        AFX_ITERATE_CHAIN(AFX_OBJECT(afxNode), nod, parent, &root->children)
        {
            AFX_ASSERT_OBJECTS(afxFcc_NOD, 1, &nod);

            if (-1 == AfxTestSphereEnglobingPoint(area, nod->w[3]))
                continue;

            afxUnit arrel2;
            if (!AfxPushArrayUnits(pvs, 1, &arrel2, &nod, sizeof(nod)))
                AfxThrowError();

            afxNode child;
            AFX_ITERATE_CHAIN2(AFX_OBJECT(afxNode), child, parent, &nod->children)
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

_ASX void _AsxNodUpdateMatrix(afxNode nod)
{
    afxError err;
    AFX_ASSERT_OBJECTS(afxFcc_NOD, 1, &nod);

    afxNode parent = AfxGetLinker(&nod->parent);

    // We can't update the parent here to avoid retro-recursion. But it should be expected to have parent nodes before their children.

    if (parent)
    {
        AfxComputeCompositeTransformM4d(&nod->t, nod->m);
        AfxM4dMultiply(nod->w, nod->m, parent->w);
    }
    else
    {
        // if there is not parent, just copy it.
        AfxComputeCompositeTransformM4d(&nod->t, nod->m);
        AfxM4dCopy(nod->w, nod->m);
    }

    afxNode child;
    AFX_ITERATE_CHAIN_B2F(AFX_OBJECT(afxNode), child, parent, &nod->children)
    {
        AFX_ASSERT_OBJECTS(afxFcc_NOD, 1, &child);
        _AsxNodUpdateMatrix(child);
    }
}

_ASX void _AsxNodUpdatePose(afxNode nod)
{
    afxError err;
    AFX_ASSERT_OBJECTS(afxFcc_NOD, 1, &nod);

}

_ASX void _AsxNodUpdatePlacement(afxNode nod, afxReal allowedErr)
{
    afxError err;
    AFX_ASSERT_OBJECTS(afxFcc_NOD, 1, &nod);

    if (nod->type == asxNodeType_Leaf_AnimBlend)
    {
        afxBody bod = nod->animBlend.bod;
        afxModel skl;
        AfxGetBodyModel(bod, &skl);

        afxPose pose;
        afxPlacement plce;
        AfxGetBodyPose(bod, &pose, &plce);

        afxUnit jntCnt = AsxCountJoints(skl, 0);
        afxReal sampleLodErr = 0.0;
        afxUnit sampledJntCnt = AsxCountJoints(skl, sampleLodErr);

        if (!AfxCountBodyMotives(bod))
        {
            AfxBuildPlacement(plce, NIL, skl, 0, jntCnt, 0, jntCnt, nod->w, FALSE);
        }
        else
        {
            //afxReal allowedErr = 0.f;
            afxUnit lodJntCnt = AsxCountJoints(skl, allowedErr);
            AsxCommencePoseAccumulation(pose, 0, jntCnt, NIL);
            AfxAccumulateBodyAnimations(bod, 0, jntCnt, pose, allowedErr, NIL);
            AsxConcludePoseAccumulation(pose, 0, jntCnt, skl, allowedErr, NIL);
            AfxBuildPlacement(plce, pose, skl, 0, jntCnt, 0, lodJntCnt, nod->w, FALSE);
        }
    }
}

_ASX afxError AsxUpdateDags(afxReal allowedErr, afxBool matrices, afxBool poses, afxBool placements, afxUnit cnt, afxNode nodes[])
{
    afxError err;
    AFX_ASSERT(nodes);
    AFX_ASSERT(cnt);

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxNode nod = nodes[i];
        if (!nod) continue;
        AFX_ASSERT_OBJECTS(afxFcc_NOD, 1, &nod);
        
        if (nod->dirty)
        {
            _AsxNodUpdateMatrix(nod);
            _AsxNodUpdatePlacement(nod, allowedErr);
        }
    }
    return err;
}
