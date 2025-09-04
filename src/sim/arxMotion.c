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
//#define _ARX_SIMULATION_C
#define _ARX_MOTION_C
#define _ARX_ANIMATION_C
//#define _ARX_SKELETON_C
//#define _ARX_BODY_C
//#define _ARX_MODEL_C
//#define _ARX_MOTOR_C
#include "../../sim/impl/asxImplementation.h"

_ARX afxMask ArxGetMotionFlags(arxMotion mot)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MOT, 1, &mot);
    return mot->flags;
}

_ARXINL void ArxComputeMotionDisplacement(arxMotion mot, afxM4d m)
{
    afxError err = NIL;
    AFX_ASSERT_OBJECTS(afxFcc_MOT, 1, &mot);
    AFX_ASSERT(m);
    AfxComputeCompositeTransformM4d(&mot->displacement, m);
}

_ARXINL afxBool ArxFindMotionTransform(arxMotion mot, afxString const* seqId, afxUnit *seqIdx)
{
    afxError err = NIL;
    AFX_ASSERT_OBJECTS(afxFcc_MOT, 1, &mot);
    AFX_ASSERT(seqId);
    AFX_ASSERT(seqIdx);
    afxBool found = FALSE;
    afxUnit pivotCnt = mot->pivotCnt;

    //afxStringBase strb = _ArxGetPivotTagStringBase(AfxGetProvider(mot));

    for (afxUnit i = 0; i < pivotCnt; i++)
    {
        afxString s = mot->pivots[i];

        if (AfxCompareStrings(seqId, 0, TRUE, 1, &s, NIL))
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

_ARXINL afxBool ArxFindMotionVector(arxMotion mot, afxString const* seqId, afxUnit *seqIdx)
{
    afxError err = NIL;
    AFX_ASSERT_OBJECTS(afxFcc_MOT, 1, &mot);
    AFX_ASSERT(seqId);
    AFX_ASSERT(seqIdx);
    afxBool rslt = FALSE;
    afxUnit vecCnt = mot->vecCnt;

    afxStringBase strb = _ArxDinGetAnimUrnStringBase(AfxGetProvider(mot));

    for (afxUnit i = 0; i < vecCnt; i++)
    {
        afxString s = mot->vectors[i];

        if (s.len && (AfxCompareStrings(seqId, 0, TRUE, 1, &s, NIL)))
        {
            *seqIdx = i;
            rslt = TRUE;
            break;
        }
    }
    return rslt;
}

_ARX void ArxUpdateMotionVectors(arxMotion mot, afxUnit baseSeqIdx, afxUnit seqCnt, arxVectorialMotion const vectors[], afxUnit fetchRate)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MOT, 1, &mot);
    AFX_ASSERT_RANGE(mot->vecCnt, baseSeqIdx, seqCnt);

    for (afxUnit i = 0; i < seqCnt; i++)
    {
        afxUnit seqIdx = baseSeqIdx + i;
        arxVectorialMotion const* src = &vectors[i];
        arxVectorialMotion* t = &mot->vecCurve[seqIdx];
        *t = *src;
    }
}

_ARX void ArxUpdateMotionTransforms(arxMotion mot, afxUnit baseSeqIdx, afxUnit seqCnt, arxPivotalMotion const transforms[], afxUnit fetchRate)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MOT, 1, &mot);
    AFX_ASSERT_RANGE(mot->pivotCnt, baseSeqIdx, seqCnt);

    for (afxUnit i = 0; i < seqCnt; i++)
    {
        afxUnit seqIdx = baseSeqIdx + i;
        arxPivotalMotion const* src = &transforms[i];
        arxPivotalMotion* t = &mot->pivotCurve[seqIdx];
        *t = *src;
    }
}

_ARX afxBool ArxGetMotionId(arxMotion mot, afxString* id)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MOT, 1, &mot);
    AFX_ASSERT(id);
    *id = mot->id;
    return mot->id.len;
}

_ARX afxError _ArxMotCtorCb(arxMotion mot, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MOT, 1, &mot);

    afxSimulation sim = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);
    arxMotionBlueprint const* motb = args[1];
    motb += invokeNo;

    if (!AfxCatalogStrings(_ArxDinGetAnimUrnStringBase(sim), 1, &motb->id.s, &mot->id))
        AfxThrowError();

    afxUnit vecCnt = motb->vecCnt;
    afxUnit pivotCnt = motb->pivotCnt;

    if (vecCnt)
    {
        AfxAllocate(vecCnt * sizeof(mot->vecCurve[0]), 0, AfxHere(), (void**)&mot->vecCurve);
        AfxAllocate(vecCnt * sizeof(mot->vectors[0]), 0, AfxHere(), (void**)&mot->vectors);
    }

    if (pivotCnt)
    {
        AfxAllocate(pivotCnt * sizeof(mot->pivotCurve[0]), 0, AfxHere(), (void**)&mot->pivotCurve);
        AfxAllocate(pivotCnt * sizeof(mot->pivots[0]), 0, AfxHere(), (void**)&mot->pivots);

        if (motb->incPivotLodError)
        {
            AfxAllocate(pivotCnt * sizeof(mot->pivotLodError[0]), 0, AfxHere(), (void**)&mot->pivotLodError);
        }
    }

    afxString s;

    if (!mot->vectors)
    {
        for (afxUnit i = 0; i < vecCnt; i++)
            AfxResetStrings(1, &mot->vectors[i]);
    }
    else if (!AfxCatalogStrings(_ArxDinGetAnimUrnStringBase(sim), vecCnt, motb->vectors, mot->vectors)) // AfxRegisterMorphTags is temp
        AfxThrowError();

    if ((mot->vecCnt = vecCnt))
    {
        for (afxUnit i = 0; i < vecCnt; i++)
        {
            arxVectorialMotion* t = &mot->vecCurve[i];
        }

        afxString id;
        arxCurve value;
        afxUnit key;
        afxInt dim;
        //ArxUpdateMotionVectors(mot, 0, vecCnt, &id, &key, &dim, &value, 0);
    }

    if (!mot->pivots)
    {
        for (afxUnit i = 0; i < pivotCnt; i++)
            AfxResetStrings(1, &mot->pivots[i]);
    }
    else if (!AfxCatalogStrings(_ArxDinGetAnimUrnStringBase(sim), pivotCnt, motb->pivots, mot->pivots))
        AfxThrowError();

    if ((mot->pivotCnt = pivotCnt))
    {
        for (afxUnit i = 0; i < pivotCnt; i++)
        {
            arxPivotalMotion* t = &mot->pivotCurve[i];
            
        }

        afxString id;
        afxFlags flag;
        arxCurve orient, scale, pos;
        //ArxUpdateMotionTransforms(mot, 0, xformMotCnt, &id, &flag, &orient, &pos, &scale, 0);
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

    ArxGetMotionId(mot, &s);
    AfxReportMessage("Motion <%.*s> assembled. %p\n    Listing %u transform circuits:", AfxPushString(&s), mot, mot->pivotCnt);

    for (afxUnit i = 0; i < mot->pivotCnt; i++)
    {
        s = mot->pivots[i];
        AfxReportMessage("\t%u <%.*s> %x", i, AfxPushString(&s), mot->pivotCurve[i].flags);
    }
#endif

    return err;
}

_ARX afxError _ArxMotDtorCb(arxMotion mot)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MOT, 1, &mot);

    if (mot->pivots)
        AfxDeallocate((void**)&mot->pivots, AfxHere());

    if (mot->pivotLodError)
        AfxDeallocate((void**)&mot->pivotLodError, AfxHere());

    if (mot->pivotCurve)
    {
        for (afxUnit i = 0; i < mot->pivotCnt; i++)
        {
            arxPivotalMotion* t = &mot->pivotCurve[i];
            arxCurve curves[3] = { t->translation, t->transmission, t->transmutation};
            AfxDisposeObjects(3, curves);
        }
        AfxDeallocate((void**)&mot->pivotCurve, AfxHere());
    }

    if (mot->vectors)
        AfxDeallocate((void**)&mot->vectors, AfxHere());

    if (mot->vecCurve)
    {
        for (afxUnit i = 0; i < mot->vecCnt; i++)
        {
            arxVectorialMotion* v = &mot->vecCurve[i];
            AfxDisposeObjects(1, &v->value);
        }
        AfxDeallocate((void**)&mot->vecCurve, AfxHere());
    }

    return err;
}

_ARX afxClassConfig const _ARX_MOT_CLASS_CONFIG =
{
    .fcc = afxFcc_MOT,
    .name = "Motion",
    .desc = "Full Motion Gesture",
    .fixedSiz = sizeof(AFX_OBJECT(arxMotion)),
    .ctor = (void*)_ArxMotCtorCb,
    .dtor = (void*)_ArxMotDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_ARX afxError ArxAssembleMotions(afxSimulation sim, afxUnit cnt, arxMotionBlueprint const blueprints[], arxMotion motions[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);

    afxClass* cls = (afxClass*)_ArxGetMotionClass(sim);
    AFX_ASSERT_CLASS(cls, afxFcc_MOT);

    if (AfxAcquireObjects(cls, 1, (afxObject*)motions, (void const*[]) { sim, blueprints }))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT_OBJECTS(afxFcc_MOT, cnt, motions);

    return err;
}
