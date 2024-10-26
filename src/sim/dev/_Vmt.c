#define _AMX_SKELETON_C
#define _AMX_ANIMATION_C
#define _AMX_POSE_C
#define _AMX_MOTION_C
#include "../../dev/AmxImplKit.h"


_AMX void BeginLocalPoseAccumulation(afxPose LocalPose, afxUnit FirstBone, afxUnit BoneCount, const afxUnit *SparseBoneArray)
{
    //PushSampledFrame(LocalPose, FirstBone, BoneCount, SparseBoneArray);
    ++LocalPose->traversalId;
    // ???
}

_AMX void EndLocalPoseAccumulationLOD(afxPose LocalPose, afxUnit FirstBone, afxUnit BoneCount, afxSkeleton skl, afxReal AllowedError, const afxUnit *SparseBoneArray)
{
    afxReal AllowedErrorScaler;
    afxReal AllowedErrorEnd;
    afxError err = NIL;
    
    if (skl->lodType)
    {
        AfxQuerySkeletonErrorTolerance(skl, AllowedError, &AllowedErrorEnd, &AllowedErrorScaler); // TODO insert allowed error in skeleton due it be skl-dependent?
    }
    else
    {
        AllowedError = 0.0;
        AllowedErrorEnd = 0.0;
        AllowedErrorScaler = 0.0;
    }


    for (afxUnit artIdx = FirstBone; artIdx < BoneCount; artIdx++)
    {
        afxUnit jointIdx = (SparseBoneArray) ? SparseBoneArray[artIdx] : artIdx;
        akxArticulation* art = &LocalPose->arts[artIdx];
        afxTransform const* local = &skl->local[jointIdx];

        if (art->traversalId == LocalPose->traversalId)
        {
            if (art->weight < LocalPose->fillThreshold)
            {
                AfxAccumulateLocalTransform(LocalPose, artIdx, jointIdx, LocalPose->fillThreshold - art->weight, skl, afxQuatBlend_ACCUM_ADJACENT, local);
            }

            afxReal s = (AllowedError - skl->lodError[jointIdx]) * AllowedErrorScaler;

            if (s > 0.0)
            {
                if (s >= 0.99000001)
                {
                    art->weight = 1.0;
                    art->cnt = 1;
                    art->xform = skl->local[jointIdx];
                }
                else
                {
                    AfxAccumulateLocalTransform(LocalPose, artIdx, jointIdx, art->weight / (1.0 - s) * s, skl, afxQuatBlend_ADJACENT, local);
                }
            }

            afxReal wc;
            *(afxInt*)&wc = 1065353216;

            if (art->cnt != 1 || art->weight != wc)
            {
                afxReal s2 = 1.0 / art->weight;
                AfxV3dScale(art->xform.position, art->xform.position, s2);
                AfxM3dScale(art->xform.scaleShear, art->xform.scaleShear, s2);
            }

            afxReal sq2, sq = AfxV4dSq(art->xform.orientation);

            if (sq > 1.1 || sq < 0.89999998)
                sq2 = AfxRsqrtf(sq);
            else
                sq2 = (3.0 - sq) * (12.0 - (3.0 - sq) * (3.0 - sq) * sq) * 0.0625;

            AfxV4dScale(art->xform.orientation, art->xform.orientation, sq2);
        }
        else
        {
            art->xform = *local;
        }
    }
}

void QuaternionRotationDifference(afxQuat q, afxQuat const from, afxQuat const to)
{
    AfxQuatConj(q, from);
    AfxQuatMultiply(q, q, to);
}

void QuaternionDifferenceToAngularVelocity(afxV3d vel, afxQuat const from, afxQuat const to)
{
    afxQuat diff;
    QuaternionRotationDifference(diff, from, to);
    afxReal mag = AfxV3dMag(diff);
    afxReal s = (2 * AfxAtan2f(mag, diff[3]));
    
    if (mag != 0.0)
        s /= mag;

    AfxV3dScale(vel, diff, s);
}

void ComputePeriodicLoopVector(const akxPeriodicLoop *Loop, afxReal Seconds, afxV3d Result)
{
    afxReal v3 = Seconds * Loop->dZ;
    *Result = v3 * Loop->axis[0];
    Result[1] = v3 * Loop->axis[1];
    Result[2] = v3 * Loop->axis[2];
    afxReal v5 = (AfxCosf(Seconds * Loop->dAngle) - 1.0) * Loop->radius;
    *Result = v5 * Loop->basisX[0] + *Result;
    Result[1] = v5 * Loop->basisX[1] + Result[1];
    Result[2] = v5 * Loop->basisX[2] + Result[2];
    afxReal v6 = AfxSinf(Seconds * Loop->dAngle) * Loop->radius;
    *Result = v6 * Loop->basisY[0] + *Result;
    Result[1] = v6 * Loop->basisY[1] + Result[1];
    Result[2] = v6 * Loop->basisY[2] + Result[2];
}

void ComputePeriodicLoopLog(const akxPeriodicLoop *Loop, afxReal Seconds, afxV3d Result)
{
    afxReal v3 = Seconds * Loop->dAngle;
    *Result = v3 * Loop->axis[0];
    Result[1] = v3 * Loop->axis[1];
    Result[2] = v3 * Loop->axis[2];
}

void ComputeTotalDeltasFromBinding(struct animation_binding *Binding, afxV3d TotalPositionDelta, afxV3d TotalOrientationDelta)
{
    afxBool v3 = Binding->TrackBindingCount == 0;
    akxBoundMotionTransform* v4 = Binding->TrackBindings;
    afxMotionTransform const* mt = v4->srcTrack;

    if (v3 || (mt = v4->srcTrack) == 0)
    {
        AfxV3dZero(TotalPositionDelta);
    }
    else
    {
        afxQuat FirstQuaternion, LastQuaternion;
        afxCurve v6 = mt->translation;

        if (AfxIsCurveConstantOrIdentity(v6))
        {
            AfxV3dZero(TotalPositionDelta);
        }
        else
        {
            afxUnit v7 = AfxCountCurveKnots(v6);
            AfxExtractCurveKnotValue(v6, 0, FirstQuaternion, AFX_QUAT_IDENTITY);
            AfxExtractCurveKnotValue(v6, v7 - 1, LastQuaternion, AFX_QUAT_IDENTITY);
            AfxV3dSub(TotalPositionDelta, LastQuaternion, FirstQuaternion);
        }

        afxCurve v9 = mt->transmission;

        if (!AfxIsCurveConstantOrIdentity(v9))
        {
            afxUnit v10 = AfxCountCurveKnots(v9);
            AfxExtractCurveKnotValue(v9, 0, FirstQuaternion, AFX_QUAT_IDENTITY);
            AfxExtractCurveKnotValue(v9, v10 - 1, LastQuaternion, AFX_QUAT_IDENTITY);
            QuaternionDifferenceToAngularVelocity(TotalOrientationDelta, FirstQuaternion, LastQuaternion);
            return;
        }
    }
    AfxV3dZero(TotalOrientationDelta);
}

void AnimationAccumulateLoopTransform(akxMotive *intk, afxReal SecondsElapsed, afxReal *TotalWeight, afxV3d ResultTranslation, afxV3d ResultRotation, afxBool Inverse)
{
    afxMotor moto = intk->moto;

    if (intk->ca.AccumulationMode)
    {
        afxMotorState ms;
        AfxQueryMotorState(moto, &ms);

        afxReal v6 = ms.effectiveWeight;
        afxReal Weight = v6;

        if (v6 > 0.001)
        {
            akxBoundMotionTransform* v7 = intk->ca.Binding->TrackBindings;

            if (v7)
            {
                afxMotionTransform const* v8 = v7->srcTrack;

                if (v8)
                {
                    afxReal v9 = ms.localDur;
                    afxReal LocalDuration = v9;

                    if (v9 > 0.0000099999997)
                    {
                        afxReal SecondsElapseda = ms.speed * SecondsElapsed;
                        afxReal v10 = ms.localClockRaw;
                        afxReal EndLocalClock = v10;
                        afxBool FlipResult = Inverse;
                        afxReal StartLocalClock = v10 - SecondsElapseda;

                        if (StartLocalClock > (double)EndLocalClock)
                        {
                            EndLocalClock = v10 - SecondsElapseda;
                            StartLocalClock = v10;
                            FlipResult = Inverse == 0;
                        }

                        afxBool UnderflowLoop = ms.underflow, OverflowLoop = ms.overflow;

                        if (!UnderflowLoop)
                        {
                            if (EndLocalClock < 0.0)
                                return;

                            if (StartLocalClock < 0.0)
                                StartLocalClock = 0.0;
                        }

                        if (!OverflowLoop)
                        {
                            if (StartLocalClock > (double)LocalDuration)
                                return;

                            if (EndLocalClock > (double)LocalDuration)
                                EndLocalClock = LocalDuration;
                        }

                        afxQuat BaseOrientation;
                        afxV3d TotalOrientationDelta;

                        if (intk->ca.AccumulationMode == 1)
                        {
                            afxReal LocalLoopCounta = EndLocalClock - StartLocalClock;
                            akxPeriodicLoop const* v22 = intk->ca.Binding->ID.Animation->motSlots[intk->ca.Binding->ID.motIdx].mot->periodicLoop;

                            if (v22)
                            {
                                afxReal LoopCount = Weight;

                                if (FlipResult)
                                    LoopCount = -Weight;

                                ComputePeriodicLoopVector(v22, LocalLoopCounta, BaseOrientation);
                                AfxAddScaledV3d(ResultTranslation, ResultTranslation, BaseOrientation, LoopCount);
                                ComputePeriodicLoopLog(v22, LocalLoopCounta, TotalOrientationDelta);
                                AfxAddScaledV3d(ResultRotation, ResultRotation, TotalOrientationDelta, LoopCount);
                            }
                            else
                            {
                                afxReal v23 = LocalLoopCounta * Weight;
                                afxReal LocalLoopCountb = v23 / ms.localDur;

                                if (FlipResult)
                                    LocalLoopCountb = -LocalLoopCountb;

                                AfxAddScaledV3d(ResultTranslation, ResultTranslation, intk->ca.Binding->ID.Animation->motSlots[intk->ca.Binding->ID.motIdx].mot->loopTranslation, LocalLoopCountb);
                            }
                        }
                        else
                        {
                            if (intk->ca.AccumulationMode != 2)
                                return;

                            afxReal v12 = 1.0 / LocalDuration;
                            afxReal v13 = v12;
                            afxReal Float = v12 * StartLocalClock;
                            afxInt v15 = Float;
                            afxInt v16 = -v15;
                            afxReal v17 = StartLocalClock - (double)v15 * LocalDuration;
                            afxReal v18 = v13 * EndLocalClock;
                            afxInt LocalLoopCount = v18;
                            sample_context Context;
                            Context.LocalDuration = LocalDuration;
                            Context.LocalClock = EndLocalClock - (double)LocalLoopCount * LocalDuration;
                            Context.FrameIndex = 0;
                            Context.UnderflowLoop = 0;
                            Context.OverflowLoop = 0;
                            afxQuat EndOrientation;
                            afxV3d EndPosition;
                            Smt.SampleTrackPOLocal(&Context, v8, v7, EndPosition, EndOrientation);
                            Context.LocalClock = v17;
                            afxQuat StartOrientation;
                            afxV3d StartPosition;
                            Smt.SampleTrackPOLocal(&Context, v8, v7, StartPosition, StartOrientation);
                            
                            afxV3d ResidualPositionDelta, ResidualOrientationDelta;
                            ResidualPositionDelta[0] = EndPosition[0] - StartPosition[0];
                            ResidualPositionDelta[1] = EndPosition[1] - StartPosition[1];
                            ResidualPositionDelta[2] = EndPosition[2] - StartPosition[2];
                            QuaternionDifferenceToAngularVelocity(ResidualOrientationDelta, StartOrientation, EndOrientation);

                            if (LocalLoopCount + v16)
                            {
                                ComputeTotalDeltasFromBinding(intk->ca.Binding, BaseOrientation, TotalOrientationDelta);
                                afxReal v19 = (double)(LocalLoopCount + v16);
                                AfxAddScaledV3d(ResidualOrientationDelta, ResidualOrientationDelta, TotalOrientationDelta, v19);
                                AfxAddScaledV3d(ResidualPositionDelta, ResidualPositionDelta, BaseOrientation, v19);
                            }

                            if (FlipResult)
                            {
                                AfxV3dNeg(ResidualOrientationDelta, ResidualOrientationDelta);
                                AfxV3dNeg(ResidualPositionDelta, ResidualPositionDelta);
                            }

                            afxCurve v20 = v8->transmission;

                            if (!AfxIsCurveIdentity(v20))
                            {
                                AfxExtractCurveKnotValue(v20, 0, BaseOrientation, AFX_QUAT_IDENTITY);
                                AfxQuatRotateV3dArray(BaseOrientation, 1, &ResidualPositionDelta, &ResidualPositionDelta);
                                AfxQuatRotateV3dArray(BaseOrientation, 1, &ResidualOrientationDelta, &ResidualOrientationDelta);
                            }

                            AfxQuatConj(StartOrientation, StartOrientation);
                            AfxQuatRotateV3dArray(StartOrientation, 1, &ResidualPositionDelta, &ResidualPositionDelta);

                            AfxAddScaledV3d(ResultRotation, ResultRotation, ResidualOrientationDelta, Weight);
                            AfxAddScaledV3d(ResultTranslation, ResultTranslation, ResidualPositionDelta, Weight);
                        }
                        *TotalWeight += Weight;
                    }
                }
            }
        }
    }
}

afxReal GetTrackMaskBoneWeight(const akxTrackMask *Mask, afxUnit BoneIndex)
{
    double result; // st7

    if (BoneIndex >= Mask->boneCnt)
        result = Mask->defWeight;
    else
        result = Mask->boneWeights[BoneIndex];

    return result;
}

void AnimationAccumulateBindingState(akxMotive* Binding, afxUnit FirstBone, afxUnit BoneCount, afxPose Result, afxReal AllowedError, const afxUnit *SparseBoneArray)
{
    afxSimulation sim = AfxGetProvider(Binding->moto);

    afxReal AllowedErrorEnd, AllowedErrorScaler;
    AfxQuerySimulationErrorTolerance(sim, AllowedError, &AllowedErrorEnd, &AllowedErrorScaler);

    afxMotor moto = Binding->moto;
    afxMotorState ms;
    AfxQueryMotorState(moto, &ms);
    afxReal ControlWeight = ms.effectiveWeight;

    if (ControlWeight > 0.001)
    {
        struct sample_context Context;
        Context.LocalClock = ms.localClockClamped;
        Context.FrameIndex = (signed int)Context.LocalClock / Binding->ca.Binding->ID.Animation->dur;
        Context.LocalDuration = ms.localDur;
        Context.UnderflowLoop = ms.underflow;
        Context.OverflowLoop = ms.overflow;
        
        for (afxUnit i = FirstBone; i < FirstBone + BoneCount; i++)
        {
            afxUnit SkeletonBoneIndex = i;

            if (SparseBoneArray)
                SkeletonBoneIndex = SparseBoneArray[i];

            //struct animation_binding* v12 = Binding->ca.Binding->TrackBindings;
            akxBoundMotionTransform* v14 = &Binding->ca.Binding->TrackBindings[SkeletonBoneIndex];
            afxMotionTransform const* v13 = v14->srcTrack;

            if (v13)
            {
                akxTrackMask const* v15 = (const akxTrackMask *)Binding->ca.TrackMask;
                afxReal Weight = ControlWeight;

                if (v15)
                    Weight = GetTrackMaskBoneWeight(v15, v14->srcTrackIdx) * ControlWeight;

                akxTrackMask const* v16 = (const akxTrackMask *)Binding->ca.ModelMask;

                if (v16)
                    Weight = GetTrackMaskBoneWeight(v16, SkeletonBoneIndex) * Weight;

                if (Weight > 0.001)
                {
                    afxTransform SampledTransform;

                    if (AllowedErrorEnd < (double)v14->lodErr)
                    {
                        if (AllowedError < (double)v14->lodErr)
                        {
                            SampledTransform.flags = 0;
                            Smt.SampleTrackUUULocal(&Context, v13, v14, &SampledTransform);
                        }
                        else
                        {
                            afxReal BlendFactor = (AllowedError - v14->lodErr) * AllowedErrorScaler;
                            Smt.SampleTrackUUULocal(&Context, v13, v14, &SampledTransform);
                            AfxMixTransform(&SampledTransform, &SampledTransform, &v14->lodTransform, BlendFactor);
                        }
                    }
                    else
                    {
                        AfxCopyTransform(&SampledTransform, &v14->lodTransform);
                    }

                    if (Binding->ca.AccumulationMode == 2 && !SkeletonBoneIndex)
                    {
                        SampledTransform.position[2] = 0.0;
                        SampledTransform.position[1] = 0.0;
                        SampledTransform.position[0] = 0.0;
                        SampledTransform.orientation[0] = 0.0;
                        SampledTransform.orientation[1] = 0.0;
                        SampledTransform.orientation[2] = 0.0;
                        SampledTransform.orientation[3] = 1.0;
                    }

                    afxSkeleton skl;
                    AfxGetBodySkeleton(AfxGetLinker(&Binding->bod), &skl);
                    AfxAccumulateLocalTransform(Result, i, SkeletonBoneIndex, Weight, skl, v14->quatMode, &SampledTransform);
                }
            }
        }
    }
}

void _SetUpSampleContext(sample_context* ctx, afxMotor moto)
{
    afxMotorState ms;
    AfxQueryMotorState(moto, &ms);
    ctx->LocalClock = ms.localClockClamped;
    ctx->LocalDuration = ms.localDur;
    ctx->FrameIndex = 0;//Binding->ca.Binding->ID.Animation->dur / ctx->LocalClock;
    ctx->UnderflowLoop = ms.underflow;
    ctx->OverflowLoop = ms.overflow;
}

void AnimationBuildDirect(akxMotive* Binding, afxUnit pivotCnt, afxM4d const offset, afxPoseBuffer Result, afxReal allowedErr)
{
    struct animation_binding* cab = Binding->ca.Binding;
    afxBody bod = AfxGetLinker(&Binding->bod);
    afxSkeleton skl;
    AfxGetBodySkeleton(bod, &skl);
    afxTransform const* local = AfxGetSkeletonTransform(skl, 0);
    afxUnit* parentIdx = AfxGetSkeletonParentIndexes(skl, 0);

    afxReal allowedErrEnd, allowedErrScaler;
    AfxQuerySkeletonErrorTolerance(skl, allowedErr, &allowedErrEnd, &allowedErrScaler);

    sample_context Context;
    _SetUpSampleContext(&Context, Binding->moto);

    afxM4d* w = AfxGetWorldMatrices(Result, 0);
    afxM4d* composite = AfxGetCompositeMatrices(Result, 0);
    afxM4d const* iw = AfxGetSkeletonMatrices(skl, 0);

    if (pivotCnt > cab->TrackBindingCount)
        pivotCnt = cab->TrackBindingCount;

    akxTrackMask const* trackMask = Binding->ca.TrackMask;
    akxTrackMask const* modelMask = Binding->ca.ModelMask;

    for (afxUnit pivotIdx = 0; pivotIdx < pivotCnt; pivotIdx++)
    {
        akxBoundMotionTransform const* bound = &cab->TrackBindings[pivotIdx];
        afxMotionTransform const* mt = bound->srcTrack;
        afxV4d const* parent;

        if (parentIdx[pivotIdx] == AFX_INVALID_INDEX)
            parent = offset;
        else
            parent = w[parentIdx[pivotIdx]];

        if (pivotIdx == 0 && Binding->ca.AccumulationMode == 2 && mt)
        {
            Smt.SampleTrackIIU(&Context, mt, bound, &local[pivotIdx], iw[pivotIdx], parent, w[pivotIdx], composite ? composite[pivotIdx] : NIL);
        }
        else
        {
            if (trackMask && (GetTrackMaskBoneWeight(trackMask, bound->srcTrackIdx) < 0.001))
            {
                if (!composite)
                    BuildFullWorldPoseOnly_Generic(&local[pivotIdx], parent, w[pivotIdx]);
                else
                    BuildFullWorldPoseComposite_Generic(&local[pivotIdx], parent, iw[pivotIdx], composite ? composite[pivotIdx] : NIL, w[pivotIdx]);
            }
            else
            {
                if ((!modelMask) || GetTrackMaskBoneWeight(modelMask, pivotIdx) >= 0.001)
                {
                    afxReal lodErr = bound->lodErr;

                    if (allowedErrEnd < lodErr)
                    {
                        if (allowedErr < lodErr)
                        {
                            bound->sampler(&Context, mt, bound, &local[pivotIdx], iw[pivotIdx], parent, w[pivotIdx], composite ? composite[pivotIdx] : NIL);
                        }
                        else
                        {
                            afxReal blendFactor = (allowedErr - lodErr) * allowedErrScaler;
                            Smt.SampleTrackUUUBlendWithTime0(&Context, mt, bound, &local[pivotIdx], iw[pivotIdx], parent, w[pivotIdx], composite ? composite[pivotIdx] : NIL, blendFactor);
                        }
                    }
                    else
                    {
                        Smt.SampleTrackUUUAtTime0(&Context, mt, bound, &local[pivotIdx], iw[pivotIdx], parent, w[pivotIdx], composite ? composite[pivotIdx] : NIL);
                    }
                }
            }
        }
    }
}

void CopyLODErrorValuesToAnimation(struct animation_binding *AnimationBinding, afxReal ManualScaler, afxBool OnlyCopyIfGreater)
{
    afxMotion v3 = AnimationBinding->ID.Animation->motSlots[AnimationBinding->ID.motIdx].mot;

    if (v3->pivotLodError)
    {
        for (afxUnit i = 0; i < AnimationBinding->TrackBindingCount; i++)
        {
            akxBoundMotionTransform *v6 = AnimationBinding->TrackBindings;
            afxUnit SourceTrackIndex = v6[i].srcTrackIdx;

            if (SourceTrackIndex != AFX_INVALID_INDEX)
            {
                afxReal LODError = ManualScaler * v6[i].lodErr;
                afxReal* pivotLodError = v3->pivotLodError;

                if (OnlyCopyIfGreater)
                {
                    afxBool v11 = LODError < pivotLodError[SourceTrackIndex];
                    afxBool v12 = LODError == pivotLodError[SourceTrackIndex];

                    if (!v11 && !v12)
                        pivotLodError[SourceTrackIndex] = LODError;
                }
                else
                {
                    pivotLodError[SourceTrackIndex] = LODError;
                }
            }
        }
    }
}

