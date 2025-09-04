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
#define _ARX_CURVE_C
#define _ARX_SIMULATION_C
#define _ARX_MOTION_C
#define _ARX_ANIMATION_C
#define _ARX_SKELETON_C
#define _ARX_BODY_C
#define _ARX_MODEL_C
#define _ARX_MOTOR_C
#include "../../sim/impl/asxImplementation.h"

ARX afxError ArxWriteMappedStrings(afxStream out, afxString* sdb, afxUnit cnt, afxString const strings[]);
ARX afxError ArxReadMappedStrings(afxStream in, afxString const* sdb, afxUnit cnt, afxString strings[]);

#pragma pack(push, 1)

AFX_DEFINE_STRUCT(K4D_CUR_HDR)
// SIGMA FEDERATION, SERIALIZED 4D GEOMETRIC CURVE HEADER
{
    afxUnit16       flags;
    afxUnit8        fmt;
    afxUnit8        dimens;
    afxUnit32       degree;
    afxUnit32       knotCnt;
    afxUnit32       ctrlCnt;
    afxUnit32       knotDirOffset;
    afxUnit32       ctrlDirOffset;
};

AFX_DEFINE_STRUCT(K4D_MOT_VEC_HDR)
// SIGMA FEDERATION, SERIALIZED 4D VECTORIAL MOTION HEADER
{
    afxUnit         seqKey;
    afxInt          dimension;
    afxUnit32       valueCurIdx;
};

AFX_DEFINE_STRUCT(K4D_MOT_PVT_HDR)
// SIGMA FEDERATION, SERIALIZED 4D PIVOTAL MOTION HEADER
{
    afxUnit32       flags;
    afxUnit32       transmissionCurIdx;
    afxUnit32       translationCurIdx;
    afxUnit32       transmutationCurIdx;
};

AFX_DEFINE_STRUCT(K4D_MOT_HDR)
// SIGMA FEDERATION, SERIALIZED 4D MOTION HEADER
{
    afxUnit32       flags;
    afxUnit32       totalCurCnt;
    afxUnit32       curDir;
    afxUnit32       pvtCnt; // circuit count
    afxUnit32       pvtDir; // circuit id compatible with skeleton joints.
    afxUnit32       pvtInfoDir;
    afxUnit32       pvtLodErrDir;
    afxUnit32       vecCnt;
    afxUnit32       vecDir;
    afxUnit32       vecInfoDir;
    afxTransform    displacement;
    afxUnit32       rootMotPvtFlags;
    afxUnit32       rootTransmissionCurIdx;
    afxUnit32       rootTranslationCurIdx;
    afxUnit32       rootTransmutationCurIdx;
    afxV3d          loopTranslation;
    afxReal         periodicLoopRadius;
    afxReal         periodicLoop_dAngle;
    afxReal         periodicLoop_dZ;
    afxV3d          periodicLoopBasisX;
    afxV3d          periodicLoopBasisY;
    afxV3d          periodicLoopAxis;
};

AFX_DEFINE_STRUCT(K4D_ANI_HDR)
// SIGMA FEDERATION, SERIALIZED 4D ANIMATION HEADER
{
    afxUnit8        fcc[4];
    afxUnit32       hdrSiz;
    afxUnit32       hdrVer;
    afxUnit32       segCnt;
    afxUnit32       segStart;
    afxUnit32       sdbSiz;
    afxUnit32       sdbStart;

    afxM3d          basis;
    afxV3d          origin;
    afxReal         unitsPerMeter;

    afxReal         dur;
    afxReal         timeStep;
    afxReal         oversampling;

    afxUnit32       flags;
    afxUnit32       motCnt;
    afxUnit32       motIdBase;
    afxUnit32       motInfoBase;
};

#pragma pack(pop)

_ARX afxError AfxSerializeCurves(afxStream out, afxUnit cnt, arxCurve curves[])
{
    afxError err = NIL;
    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &out);
    AFX_ASSERT_OBJECTS(afxFcc_CUR, cnt, curves);
    afxUnit curIdx = 0;

    K4D_CUR_HDR* curHdrs;
    AfxAllocate(cnt * sizeof(curHdrs[0]), 0, AfxHere(), (void**)&curHdrs);

    afxSize curHdrOffBkp = AfxAskStreamPosn(out);
    // skip the room to the headers
    AfxAdvanceStream(out, cnt * sizeof(curHdrs[0]));
    afxSize endOfHdrs = AfxAskStreamPosn(out);

    do
    {
        arxCurve cur = curves[curIdx];

        K4D_CUR_HDR* hdr = &curHdrs[curIdx];
        hdr->fmt = cur->fmt;
        hdr->dimens = cur->dimens;
        hdr->flags = cur->flags;
        hdr->knotCnt = cur->knotCnt;
        hdr->ctrlCnt = cur->ctrlCnt;
        hdr->degree = cur->degree;

        if (cur->knotCnt)
        {
            hdr->knotDirOffset = AfxAskStreamPosn(out);
            AfxWriteStream(out, cur->knotCnt * sizeof(afxReal), 0, ArxGetCurveKnots(cur));
        }

        if (cur->ctrlCnt)
        {
            hdr->ctrlDirOffset = AfxAskStreamPosn(out);
            AfxWriteStream(out, cur->ctrlCnt * sizeof(afxReal), 0, ArxGetCurveControls(cur));
        }

    } while (!err && (++curIdx < cnt));

    // write out all headers
    afxUnit bkpPos = AfxAskStreamPosn(out);
    AfxSeekStream(out, curHdrOffBkp, afxSeekOrigin_BEGIN);
    if (AfxWriteStream(out, cnt * sizeof(curHdrs[0]), 0, curHdrs))
        AfxThrowError();

    AfxSeekStream(out, bkpPos, afxSeekOrigin_BEGIN);

    AfxDeallocate((void**)&curHdrs, AfxHere());

    return err;
}

_ARX afxError AfxSerializeMotions(afxStream out, afxString* sdb, afxUnit cnt, arxMotion motions[])
{
    afxError err = NIL;
    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &out);
    AFX_ASSERT_OBJECTS(afxFcc_MOT, cnt, motions);
    afxUnit motIdx = 0;

    K4D_MOT_HDR* motHdrs;
    AfxAllocate(cnt * sizeof(motHdrs[0]), 0, AfxHere(), (void**)&motHdrs);

    afxSize motHdrOffBkp = AfxAskStreamPosn(out);
    // skip the room to the headers
    AfxAdvanceStream(out, cnt * sizeof(motHdrs[0]));
    afxSize endOfHdrs = AfxAskStreamPosn(out);

    do
    {
        arxMotion mot = motions[motIdx];

        K4D_MOT_HDR motHdr = { 0 };

        motHdr.flags = mot->flags;
        motHdr.pvtCnt = mot->pivotCnt;
        motHdr.vecCnt = mot->vecCnt;
        motHdr.displacement = mot->displacement;

        AfxV3dCopy(motHdr.loopTranslation, mot->loopTranslation);

        if (mot->periodicLoop)
        {
            motHdr.periodicLoopRadius = mot->periodicLoop->radius;
            motHdr.periodicLoop_dAngle = mot->periodicLoop->dAngle;
            motHdr.periodicLoop_dZ = mot->periodicLoop->dZ;
            AfxV3dCopy(motHdr.periodicLoopBasisX, mot->periodicLoop->basisX);
            AfxV3dCopy(motHdr.periodicLoopBasisY, mot->periodicLoop->basisY);
            AfxV3dCopy(motHdr.periodicLoopAxis, mot->periodicLoop->axis);
        }

        if (mot->pivotCnt)
        {
            // write pivots' identifier strings.
            motHdr.pvtDir = AfxAskStreamPosn(out);
            ArxWriteMappedStrings(out, sdb, mot->pivotCnt, mot->pivots);
        }

        if (mot->vecCnt)
        {
            // write vectors' identifier strings.
            motHdr.vecDir = AfxAskStreamPosn(out);
            ArxWriteMappedStrings(out, sdb, mot->vecCnt, mot->vectors);
        }

        // make room for and prepare pivots and vectors

        afxUnit curIdx = 0;
        afxUnit totalCurCnt = (mot->pivotCnt * 3) + (mot->vecCnt * 1) + (mot->root ? 3 : 0);
        arxCurve* curves = NIL;
        K4D_MOT_PVT_HDR* pvtHdrs = NIL;
        K4D_MOT_VEC_HDR* vecHdrs = NIL;

        if (totalCurCnt)
        {
            AfxAllocate(totalCurCnt * sizeof(curves[0]), 0, AfxHere(), (void**)&curves);
        }

        if (mot->pivotCnt)
        {
            AfxAllocate(mot->pivotCnt * sizeof(pvtHdrs[0]), 0, AfxHere(), (void**)&pvtHdrs);

            // prepare pivots
            for (afxUnit i = 0; i < mot->pivotCnt; i++)
            {
                K4D_MOT_PVT_HDR* hdr = &pvtHdrs[i];
                hdr->flags = mot->pivotCurve[i].flags;

                hdr->transmissionCurIdx = curIdx++;
                curves[hdr->transmissionCurIdx] = mot->pivotCurve[i].transmission;
                hdr->translationCurIdx = curIdx++;
                curves[hdr->translationCurIdx] = mot->pivotCurve[i].translation;
                hdr->transmutationCurIdx = curIdx++;
                curves[hdr->transmutationCurIdx] = mot->pivotCurve[i].transmutation;
            }
        }

        if (mot->vecCnt)
        {
            AfxAllocate(mot->vecCnt * sizeof(vecHdrs[0]), 0, AfxHere(), (void**)&vecHdrs);

            // prepare vectors
            for (afxUnit i = 0; i < mot->vecCnt; i++)
            {
                K4D_MOT_VEC_HDR* hdr = &vecHdrs[i];
                hdr->seqKey = mot->vecCurve[i].seqKey;
                hdr->dimension = mot->vecCurve[i].dimension;
                hdr->valueCurIdx = curIdx++;
                curves[hdr->valueCurIdx] = mot->vecCurve[i].value;
            }
        }

        // prepare root curves
        if (mot->root)
        {
            motHdr.rootMotPvtFlags = mot->root->flags;
            motHdr.rootTransmissionCurIdx = curIdx++;
            curves[motHdr.rootTransmissionCurIdx] = mot->root->transmission;
            motHdr.rootTranslationCurIdx = curIdx++;
            curves[motHdr.rootTranslationCurIdx] = mot->root->translation;
            motHdr.rootTransmutationCurIdx = curIdx++;
            curves[motHdr.rootTransmutationCurIdx] = mot->root->transmutation;
        }

        // write all curves
        motHdr.curDir = AfxAskStreamPosn(out);
        AFX_ASSERT(curIdx == totalCurCnt);
        motHdr.totalCurCnt = curIdx;
        AfxSerializeCurves(out, motHdr.totalCurCnt, curves);

        AfxDeallocate((void**)&curves, AfxHere());

        // write pivots' headers

        if (mot->pivotCnt)
        {
            // write pivot LOD error

            if (mot->pivotLodError)
            {
                motHdr.pvtLodErrDir = AfxAskStreamPosn(out);

                if (AfxWriteStream(out, mot->pivotCnt * sizeof(mot->pivotLodError[0]), 0, mot->pivotLodError))
                    AfxThrowError();
            }

            // go back, write pivot headers and return.

            // make room
            motHdr.pvtInfoDir = AfxAskStreamPosn(out);
            //AfxAdvanceStream(out, sizeof(pvtHdrs[0]) * mot->pivotCnt);

            afxSize bkpPos = AfxAskStreamPosn(out);
            //AfxSeekStream(out, motHdr.pvtInfoDir, afxSeekOrigin_BEGIN);
            AfxWriteStream(out, sizeof(pvtHdrs[0]) * mot->pivotCnt, 0, pvtHdrs);
            //AfxSeekStream(out, bkpPos, afxSeekOrigin_BEGIN);

            AfxDeallocate((void**)&pvtHdrs, AfxHere());
        }

        // write vectors' headers

        if (mot->vecCnt)
        {
            // make room
            motHdr.vecInfoDir = AfxAskStreamPosn(out);
            AfxAdvanceStream(out, sizeof(vecHdrs[0]) * mot->vecCnt);

            afxUnit bkpPos = AfxAskStreamPosn(out);
            AfxSeekStream(out, motHdr.vecInfoDir, afxSeekOrigin_BEGIN);
            AfxWriteStream(out, sizeof(vecHdrs[0]) * mot->vecCnt, 0, vecHdrs);
            AfxSeekStream(out, bkpPos, afxSeekOrigin_BEGIN);

            AfxDeallocate((void**)&vecHdrs, AfxHere());
        }

        motHdrs[motIdx] = motHdr;

    } while (!err && (++motIdx < cnt));

    // write out all motions' headers
    afxUnit bkpPos = AfxAskStreamPosn(out);
    AfxSeekStream(out, motHdrOffBkp, afxSeekOrigin_BEGIN);

    if (AfxWriteStream(out, cnt * sizeof(motHdrs[0]), 0, motHdrs))
        AfxThrowError();

    AFX_ASSERT(endOfHdrs == AfxAskStreamPosn(out));

    AfxSeekStream(out, bkpPos, afxSeekOrigin_BEGIN);

    AfxDeallocate((void**)&motHdrs, AfxHere());

    return err;
}

_ARX afxError AfxSerializeAnimation(arxAnimation ani, afxStream out)
{
    afxError err = NIL;
    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &out);
    AFX_ASSERT_OBJECTS(afxFcc_ANI, 1, &ani);
    afxUnit mdlIdx = 0;

    afxString4096 sdb;
    AfxMakeString4096(&sdb, NIL);

    K4D_ANI_HDR aniHdr = { 0 };
    aniHdr.fcc[0] = 'k';
    aniHdr.fcc[1] = '4';
    aniHdr.fcc[2] = 'd';
    aniHdr.fcc[3] = '\0';
    aniHdr.hdrSiz = sizeof(aniHdr) - sizeof(urdMark);
    AfxM3dReset(aniHdr.basis);
    AfxV3dZero(aniHdr.origin);
    aniHdr.unitsPerMeter = 1.f;
    aniHdr.flags = ani->flags;
    aniHdr.motCnt = ani->motSlotCnt;
    aniHdr.dur = ani->dur;
    aniHdr.oversampling = ani->oversampling;
    aniHdr.timeStep = ani->timeStep;
    
    afxSize hdrPosBkp = AfxAskStreamPosn(out);
    AfxAdvanceStream(out, sizeof(aniHdr));

    aniHdr.motIdBase = AfxAskStreamPosn(out);

    afxUnit totalCurCnt = 0;
    afxUnit uniqueMotCnt = 0;
    arxMotion motions[256];

    for (afxUnit i = 0; i < aniHdr.motCnt; i++)
    {
        arxMotion mot = ani->motSlots[i].mot;

        if (mot)
        {
            motions[uniqueMotCnt++] = mot;
            ArxWriteMappedStrings(out, &sdb.s, 1, &mot->id);
        }
    }

    aniHdr.motInfoBase = AfxAskStreamPosn(out);

    if (AfxSerializeMotions(out, &sdb.s, uniqueMotCnt, motions))
        AfxThrowError();

    {
        // write string database
        aniHdr.sdbStart = AfxAskStreamPosn(out);
        aniHdr.sdbSiz = sdb.s.len;

        if (AfxWriteStream(out, aniHdr.sdbSiz, 0, sdb.s.start))
        {
            AfxThrowError();
            return err;
        }
    }

    // write animation header
    aniHdr.hdrSiz = AfxAskStreamPosn(out) - hdrPosBkp - sizeof(urdMark);

    if (AfxWriteStreamAt(out, hdrPosBkp, sizeof(aniHdr), 0, &aniHdr))
        AfxThrowError();

    AfxAdvanceStream(out, aniHdr.hdrSiz);
    return err;
}

_ARX afxError ArxArchiveAnimation(arxAnimation ani, afxUri const* uri)
{
    afxError err = NIL;
    afxStream iob;
    afxStreamInfo info = { 0 };
    info.flags = afxIoFlag_WRITEABLE;
    info.usage = afxIoUsage_FILE;
    AfxAcquireStream(1, &info, &iob);

    AfxReopenFile(iob, uri, afxFileFlag_W);
    AfxSerializeAnimation(ani, iob);
    AfxDisposeObjects(1, &iob);

    return err;
}

_ARX afxError ArxUploadAnimation(afxSimulation sim, afxString const* urn, afxStream in, arxAnimation* animation)
{
    afxError err = NIL;
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);
    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &in);
    AFX_ASSERT(animation);

    // Qwadro doesn't use afxChunkID_STRUCT

    afxArena arena;
    AfxMakeArena(&arena, NIL, AfxHere());

#pragma push(pack, 1)
    K4D_ANI_HDR aniHdr;
#pragma pop(pack)

    afxString4096 sdb;
    AfxMakeString4096(&sdb, NIL);

    afxString strings[256];
    arxMotion motions[256];
    arxModelFlags rigFlags[256];

    if (AfxReadStream(in, sizeof(aniHdr), 0, &aniHdr))
        AfxThrowError();

    afxSize posBkp = AfxAskStreamPosn(in);
    AfxSeekStream(in, aniHdr.sdbStart, afxSeekOrigin_BEGIN);
    AfxReadString(&sdb.s, in, aniHdr.sdbSiz);

    arxAnimation ani;
    arxAnimationBlueprint anib = { 0 };
    anib.dur = aniHdr.dur;
    anib.timeStep = aniHdr.timeStep;
    anib.oversampling = aniHdr.oversampling;
    anib.motSlotCnt = aniHdr.motCnt;
    AfxMakeString32(&anib.id, urn);
    ArxAssembleAnimations(sim, 1, &anib, &ani);

    AfxSeekStream(in, aniHdr.motIdBase, afxSeekOrigin_BEGIN);

    if (ArxReadMappedStrings(in, &sdb.s, aniHdr.motCnt, strings))
        AfxThrowError();

    AfxSeekStream(in, aniHdr.motInfoBase, afxSeekOrigin_BEGIN);

    K4D_MOT_HDR* motHdrs = AfxRequestFromArena(&arena, sizeof(motHdrs[0]), aniHdr.motCnt, NIL, 0);

    if (AfxReadStream(in, aniHdr.motCnt * sizeof(motHdrs[0]), 0, motHdrs))
        AfxThrowError();

    arxMotionBlueprint* motbs = AfxRequestFromArena(&arena, sizeof(motbs[0]), aniHdr.motCnt, NIL, 1);

    for (afxUnit i = 0; i < aniHdr.motCnt; i++)
    {
        K4D_MOT_HDR* motHdr = &motHdrs[i];
        arxMotionBlueprint* motb = &motbs[i];

        afxString* pivots = AfxRequestFromArena(&arena, sizeof(pivots[0]), motHdr->pvtCnt, NIL, 0);;
        afxString* vectors = motHdr->vecCnt ? AfxRequestFromArena(&arena, sizeof(vectors[0]), motHdr->vecCnt, NIL, 0) : NIL;

        motb->displacement = motHdr->displacement;
        motb->incPivotLodError = !!motHdr->pvtLodErrDir;
        motb->pivotCnt = motHdr->pvtCnt;
        motb->vecCnt = motHdr->vecCnt;
        motb->pivots = pivots;
        motb->vectors = vectors;
        AfxMakeString32(&motb->id, &strings[i]);

        if (motb->pivotCnt)
        {
            AfxSeekStream(in, motHdr->pvtDir, afxSeekOrigin_BEGIN);
            ArxReadMappedStrings(in, &sdb.s, motHdr->pvtCnt, pivots);
        }

        if (motb->vecCnt)
        {
            AfxSeekStream(in, motHdr->vecDir, afxSeekOrigin_BEGIN);
            ArxReadMappedStrings(in, &sdb.s, motHdr->vecCnt, vectors);
        }
    }

    ArxAssembleMotions(sim, aniHdr.motCnt, motbs, motions);
    
    for (afxUnit i = 0; i < aniHdr.motCnt; i++)
    {
        K4D_MOT_HDR* motHdr = &motHdrs[i];

        AfxV3dCopy(motions[i]->loopTranslation, motHdr->loopTranslation);

        K4D_CUR_HDR* curHdrs = AfxRequestFromArena(&arena, sizeof(curHdrs[0]), motHdr->totalCurCnt, NIL, 0);
        arxCurveInfo* curis = AfxRequestFromArena(&arena, sizeof(curis[0]), motHdr->totalCurCnt, NIL, 0);
        arxCurve curvesStatic[256];
        arxCurve* curves = curvesStatic;// AfxRequestFromArena(&arena, motHdr->totalCurCnt * sizeof(curves[0]));
        AfxReadStreamAt(in, motHdr->curDir, motHdr->totalCurCnt * sizeof(curHdrs[0]), 0, curHdrs);
        
        for (afxUnit j = 0; j < motHdr->totalCurCnt; j++)
        {
            arxCurveInfo* curi = &curis[j];
            *curi = (arxCurveInfo) { 0 };
            K4D_CUR_HDR* curHdr = &curHdrs[j];

            curi->knotCnt = curHdr->knotCnt;
            curi->degree = curHdr->degree;
            curi->fmt = curHdr->fmt;
            curi->dimens = curHdr->dimens;

            if (curHdr->ctrlCnt)
            {
                afxReal* ctrls = AfxRequestFromArena(&arena, sizeof(ctrls[0]), curHdr->ctrlCnt, NIL, 0);
                AfxReadStreamAt(in, curHdr->ctrlDirOffset, curHdr->ctrlCnt * sizeof(ctrls[0]), 0, ctrls);
                curi->ctrls = ctrls;
            }

            if (curHdr->knotCnt)
            {
                afxReal* knots = AfxRequestFromArena(&arena, sizeof(knots[0]), curHdr->knotCnt, NIL, 0);
                AfxReadStreamAt(in, curHdr->knotDirOffset, curHdr->knotCnt * sizeof(knots[0]), 0, knots);
                curi->knots = knots;
            }
        }

        ArxAcquireCurves(sim, motHdr->totalCurCnt, curis, curves);
#if 0
        for (afxUnit j = 0; j < motHdr->totalCurCnt; j++)
        {
            K4D_CUR_HDR* curHdr = &curHdrs[j];

            if (curHdr->ctrlCnt)
            {
                afxReal* ctrls = AfxRequestFromArena(&arena, curHdr->ctrlCnt * sizeof(ctrls[0]));
                AfxSeekStream(in, curHdr->ctrlDirOffset, afxSeekOrigin_BEGIN);
                AfxReadStream(in, curHdr->ctrlCnt * sizeof(ctrls[0]), 0, ctrls);
            }

            if (curHdr->knotCnt)
            {
                afxReal* knots = AfxRequestFromArena(&arena, curHdr->knotCnt * sizeof(knots[0]));
                AfxSeekStream(in, curHdr->knotDirOffset, afxSeekOrigin_BEGIN);
                AfxReadStream(in, curHdr->knotCnt * sizeof(knots[0]), 0, knots);
            }
        }
#endif
        K4D_MOT_PVT_HDR* pvtHdrs = AfxRequestFromArena(&arena, sizeof(pvtHdrs[0]), motHdr->pvtCnt, NIL, 0);
        AfxReadStreamAt(in, motHdr->pvtInfoDir, motHdr->pvtCnt * sizeof(pvtHdrs[0]), 0, pvtHdrs);

        for (afxUnit j = 0; j < motHdr->pvtCnt; j++)
        {
            K4D_MOT_PVT_HDR* pvtHdr = &pvtHdrs[j];

            arxPivotalMotion mt = { 0 };
            mt.flags = pvtHdr->flags;
            mt.translation = curves[pvtHdr->translationCurIdx];
            mt.transmission = curves[pvtHdr->transmissionCurIdx];
            mt.transmutation = curves[pvtHdr->transmutationCurIdx];
            ArxUpdateMotionTransforms(motions[i], j, 1, &mt, sizeof(mt));
        }

        if (motHdr->pvtLodErrDir)
        {
            AfxReadStreamAt(in, motHdr->pvtLodErrDir, motHdr->pvtCnt * sizeof(motions[i]->pivotLodError[0]), 0, motions[i]->pivotLodError);
        }

        if (motHdr->vecCnt)
        {
            K4D_MOT_VEC_HDR* vecHdrs = AfxRequestFromArena(&arena, sizeof(vecHdrs[0]), motHdr->vecCnt, NIL, 0);
            AfxReadStreamAt(in, motHdr->vecInfoDir, motHdr->vecCnt * sizeof(vecHdrs[0]), 0, vecHdrs);

            for (afxUnit j = 0; j < motHdr->vecCnt; j++)
            {
                K4D_MOT_VEC_HDR* vecHdr = &vecHdrs[j];

                arxVectorialMotion mv = { 0 };
                mv.seqKey = vecHdr->seqKey;
                mv.dimension = vecHdr->dimension;
                mv.value = curves[vecHdr->valueCurIdx];
                ArxUpdateMotionVectors(motions[i], j, 1, &mv, sizeof(mv));
            }
        }

        //AfxDisposeObjects(motHdr->totalCurCnt, curves);

        ArxRelinkMotions(ani, i, 1, &motions[i]);
    }

    AfxDismantleArena(&arena);

    *animation = ani;

    return err;
}

_ARX afxError ArxLoadAnimation(afxSimulation sim, afxString const* urn, afxUri const* uri, arxAnimation* animation)
{
    afxError err = NIL;
    afxStream iob;
    afxStreamInfo info = { 0 };
    info.flags = afxIoFlag_READABLE;
    info.usage = afxIoUsage_FILE;
    AfxAcquireStream(1, &info, &iob);

    AfxReopenFile(iob, uri, afxFileFlag_R);
    ArxUploadAnimation(sim, urn, iob, animation);
    AfxDisposeObjects(1, &iob);

    return err;
}
