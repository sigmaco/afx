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

#define _AFX_SIM_C
#define _AMX_SIMULATION_C
#define _AMX_MOTION_C
#define _AMX_ANIMATION_C
#define _AMX_SKELETON_C
#define _AMX_BODY_C
#define _AMX_MODEL_C
#define _AMX_MOTOR_C
#include "../dev/AmxImplKit.h"


struct animation_binding *NewAnimationBinding(struct animation_binding_identifier *ID);

#if 0
struct model_control_callbacks ControlledAnimationCallbacks =
{
  &PoseGetControlledPose,
  &CurveGetKnotCountDaIdentity,
  &AnimationInitializeBindingState,
  &AnimationAccumulateBindingState,
  &AnimationBuildDirect,
  &AnimationAccumulateLoopTransform,
  &AnimationCleanupBindingState
}; // idb
struct model_control_callbacks ControlledPoseCallbacks =
{
  &CurveGetKnotCountDaIdentity,
  &PoseGetControlledPose,
  &PoseInitializeBindingState,
  &PoseAccumulateBindingState,
  &PoseBuildDirect,
  NULL,
  NULL
}; // idb
#endif

_AMXINL void AfxComputeMotionDisplacement(afxMotion mot, afxM4d m)
{
    afxError err = NIL;
    AfxAssertObjects(1, &mot, afxFcc_MOT);
    AFX_ASSERT(m);
    AfxComputeCompositeTransformM4d(&mot->displacement, m);
}

_AMXINL afxBool AfxFindMotionTransform(afxMotion mot, afxString const* seqId, afxUnit *seqIdx)
{
    afxError err = NIL;
    AfxAssertObjects(1, &mot, afxFcc_MOT);
    AFX_ASSERT(seqId);
    AFX_ASSERT(seqIdx);
    afxBool found = FALSE;
    afxUnit pivotCnt = mot->pivotCnt;

    //afxStringBase strb = AfxGetPivotTagStringBase(AfxGetProvider(mot));

    for (afxUnit i = 0; i < pivotCnt; i++)
    {
        afxString s = mot->pivots[i];

        if (0 == AfxCompareStrings(seqId, TRUE, 1, &s))
        {
            *seqIdx = i;
            found = TRUE;
            break;
        }
    }

    if (!found)
        *seqIdx = AFX_INVALID_INDEX;

    return found;
}

_AMXINL afxBool AfxFindMotionVector(afxMotion mot, afxString const* seqId, afxUnit *seqIdx)
{
    afxError err = NIL;
    AfxAssertObjects(1, &mot, afxFcc_MOT);
    AFX_ASSERT(seqId);
    AFX_ASSERT(seqIdx);
    afxBool rslt = FALSE;
    afxUnit vecCnt = mot->vecCnt;

    afxStringBase strb = AfxGetMorphTagStringBase(AfxGetProvider(mot));

    for (afxUnit i = 0; i < vecCnt; i++)
    {
        afxString s = mot->vectors[i];

        if (s.len && (0 == AfxCompareStrings(seqId, TRUE, 1, &s)))
        {
            *seqIdx = i;
            rslt = TRUE;
            break;
        }
    }
    return rslt;
}

_AMX void AfxUpdateMotionVectors(afxMotion mot, afxUnit baseSeqIdx, afxUnit seqCnt, afxMotionVector const vectors[], afxUnit fetchRate)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mot, afxFcc_MOT);
    AFX_ASSERT_RANGE(mot->vecCnt, baseSeqIdx, seqCnt);

    for (afxUnit i = 0; i < seqCnt; i++)
    {
        afxUnit seqIdx = baseSeqIdx + i;
        afxMotionVector const* src = &vectors[i];
        afxMotionVector* t = &mot->vecCurve[seqIdx];
        *t = *src;
    }
}

_AMX void AfxUpdateMotionTransforms(afxMotion mot, afxUnit baseSeqIdx, afxUnit seqCnt, afxMotionTransform const transforms[], afxUnit fetchRate)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mot, afxFcc_MOT);
    AFX_ASSERT_RANGE(mot->pivotCnt, baseSeqIdx, seqCnt);

    for (afxUnit i = 0; i < seqCnt; i++)
    {
        afxUnit seqIdx = baseSeqIdx + i;
        afxMotionTransform const* src = &transforms[i];
        afxMotionTransform* t = &mot->pivotCurve[seqIdx];
        *t = *src;
    }
}

_AMX afxBool AfxGetMotionId(afxMotion mot, afxString* id)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mot, afxFcc_MOT);
    AFX_ASSERT(id);
    *id = mot->id;
    return mot->id.len;
}

_AMX afxError _AmxMotCtorCb(afxMotion mot, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mot, afxFcc_MOT);

    afxSimulation sim = args[0];
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    afxMotionBlueprint const* motb = args[1];
    motb += invokeNo;

    if (!AfxCatalogStrings(AfxGetModelUrnStringBase(sim), 1, &motb->id.str, &mot->id))
        AfxThrowError();

    afxUnit vecCnt = motb->vecCnt;
    afxUnit pivotCnt = motb->pivotCnt;

    mot->vecCurve = vecCnt ? AfxAllocate(vecCnt, sizeof(mot->vecCurve[0]), 0, AfxHere()) : NIL;
    mot->vectors = vecCnt ? AfxAllocate(vecCnt, sizeof(mot->vectors[0]), 0, AfxHere()) : NIL;
    mot->pivotCurve = pivotCnt ? AfxAllocate(pivotCnt, sizeof(mot->pivotCurve[0]), 0, AfxHere()) : NIL;
    mot->pivots = pivotCnt ? AfxAllocate(pivotCnt, sizeof(mot->pivots[0]), 0, AfxHere()) : NIL;
    mot->pivotLodError = motb->incPivotLodError && pivotCnt ? AfxAllocate(pivotCnt, sizeof(mot->pivotLodError[0]), 0, AfxHere()) : NIL;

    afxString s;

    if (!mot->vectors)
    {
        for (afxUnit i = 0; i < vecCnt; i++)
            AfxResetStrings(1, &mot->vectors[i]);
    }
    else if (!AfxCatalogStrings(AfxGetMorphTagStringBase(AfxGetSimulationDrawInput(sim)), vecCnt, motb->vectors, mot->vectors)) // AfxRegisterMorphTags is temp
        AfxThrowError();

    if ((mot->vecCnt = vecCnt))
    {
        for (afxUnit i = 0; i < vecCnt; i++)
        {
            afxMotionVector* t = &mot->vecCurve[i];
        }

        afxString id;
        afxCurve value;
        afxUnit key;
        afxInt dim;
        //AfxUpdateMotionVectors(mot, 0, vecCnt, &id, &key, &dim, &value, 0);
    }

    if (!mot->pivots)
    {
        for (afxUnit i = 0; i < pivotCnt; i++)
            AfxResetStrings(1, &mot->pivots[i]);
    }
    else if (!AfxCatalogStrings(AfxGetPivotTagStringBase(AfxGetSimulationDrawInput(sim)), pivotCnt, motb->pivots, mot->pivots))
        AfxThrowError();

    if ((mot->pivotCnt = pivotCnt))
    {
        for (afxUnit i = 0; i < pivotCnt; i++)
        {
            afxMotionTransform* t = &mot->pivotCurve[i];
            
        }

        afxString id;
        afxFlags flag;
        afxCurve orient, scale, pos;
        //AfxUpdateMotionTransforms(mot, 0, xformMotCnt, &id, &flag, &orient, &pos, &scale, 0);
    }

    if (mot->pivotLodError)
    {
        if (motb->pivotLodError) for (afxUnit i = 0; i < pivotCnt; i++)
            mot->pivotLodError[i] = motb->pivotLodError[i];
        else for (afxUnit i = 0; i < pivotCnt; i++)
            mot->pivotLodError[i] = 1.9140881e38;
    }

    AfxCopyTransform(&mot->displacement, &motb->displacement);

    mot->flags = 2;
    mot->periodicLoop = 0;
    mot->root = NIL;
    AfxV3dZero(mot->loopTranslation);

#if !0
    // echo

    AfxGetMotionId(mot, &s);
    AfxLogEcho("Motion <%.*s> assembled. %p\n    Listing %u transform circuits:", AfxPushString(&s), mot, mot->pivotCnt);

    for (afxUnit i = 0; i < mot->pivotCnt; i++)
    {
        s = mot->pivots[i];
        AfxLogEcho("\t%u <%.*s> %x", i, AfxPushString(&s), mot->pivotCurve[i].flags);
    }
#endif

    return err;
}

_AMX afxError _AmxMotDtorCb(afxMotion mot)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mot, afxFcc_MOT);

    if (mot->pivots)
        AfxDeallocate(mot->pivots);

    if (mot->pivotLodError)
        AfxDeallocate(mot->pivotLodError);

    if (mot->pivotCurve)
    {
        for (afxUnit i = 0; i < mot->pivotCnt; i++)
        {
            afxMotionTransform* t = &mot->pivotCurve[i];
            afxCurve curves[3] = { t->translation, t->transmission, t->transmutation};
            AfxReleaseObjects(3, curves);
        }
        AfxDeallocate(mot->pivotCurve);
    }

    if (mot->vectors)
        AfxDeallocate(mot->vectors);

    if (mot->vecCurve)
    {
        for (afxUnit i = 0; i < mot->vecCnt; i++)
        {
            afxMotionVector* v = &mot->vecCurve[i];
            AfxReleaseObjects(1, &v->value);
        }
        AfxDeallocate(mot->vecCurve);
    }

    return err;
}

_AMX afxClassConfig _AmxMotMgrCfg =
{
    .fcc = afxFcc_MOT,
    .name = "Motion",
    .desc = "Kinematic Motion",
    .fixedSiz = sizeof(AFX_OBJECT(afxMotion)),
    .ctor = (void*)_AmxMotCtorCb,
    .dtor = (void*)_AmxMotDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AMX afxError AfxAssembleMotions(afxSimulation sim, afxUnit cnt, afxMotionBlueprint const blueprints[], afxMotion motions[])
{
    afxError err = AFX_ERR_NONE;
    
    if (AfxAcquireObjects(&sim->motMgr, 1, (afxObject*)motions, (void const*[]) { sim, blueprints }))
        AfxThrowError();

    return err;
}
