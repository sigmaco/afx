#define _AKX_SKELETON_C
#define _AKX_ANIMATION_C
#define _AKX_POSE_C
#define _AKX_MOTION_C
#include "AkxSimDevKit.h"


_AKX void BeginLocalPoseAccumulation(akxPose LocalPose, afxNat FirstBone, afxNat BoneCount, const afxNat *SparseBoneArray)
{
    //PushSampledFrame(LocalPose, FirstBone, BoneCount, SparseBoneArray);
    ++LocalPose->traversalId;
    // ???
}

_AKX void EndLocalPoseAccumulationLOD(akxPose LocalPose, afxNat FirstBone, afxNat BoneCount, afxSkeleton skl, afxReal AllowedError, const afxNat *SparseBoneArray)
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


    for (afxNat artIdx = FirstBone; artIdx < BoneCount; artIdx++)
    {
        afxNat jointIdx = (SparseBoneArray) ? SparseBoneArray[artIdx] : artIdx;
        akxArticulation* art = &LocalPose->arts[artIdx];
        afxTransform const* local = &skl->local[jointIdx];

        if (art->traversalId == LocalPose->traversalId)
        {
            if (art->weight < LocalPose->fillThreshold)
            {
                AfxAccumulateLocalTransform(LocalPose, artIdx, jointIdx, LocalPose->fillThreshold - art->weight, skl, akxQuatBlend_ACCUM_ADJACENT, local);
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
                    AfxAccumulateLocalTransform(LocalPose, artIdx, jointIdx, art->weight / (1.0 - s) * s, skl, akxQuatBlend_ADJACENT, local);
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
    akxMotionTransform const* mt = v4->srcTrack;

    if (v3 || (mt = v4->srcTrack) == 0)
    {
        AfxV3dZero(TotalPositionDelta);
    }
    else
    {
        afxQuat FirstQuaternion, LastQuaternion;
        afxCurve v6 = mt->translation;

        if (AfxCurveIsConstantOrIdentity(v6))
        {
            AfxV3dZero(TotalPositionDelta);
        }
        else
        {
            afxNat v7 = AfxCountCurveKnots(v6);
            AfxExtractCurveKnotValue(v6, 0, FirstQuaternion, AFX_QUAT_IDENTITY);
            AfxExtractCurveKnotValue(v6, v7 - 1, LastQuaternion, AFX_QUAT_IDENTITY);
            AfxV3dSub(TotalPositionDelta, LastQuaternion, FirstQuaternion);
        }

        afxCurve v9 = mt->transmission;

        if (!AfxCurveIsConstantOrIdentity(v9))
        {
            afxNat v10 = AfxCountCurveKnots(v9);
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
    akxMotor moto = intk->moto;

    if (intk->ca.AccumulationMode)
    {
        afxReal v6 = AkxGetMotorEffectiveWeight(moto);
        afxReal Weight = v6;

        if (v6 > 0.001)
        {
            akxBoundMotionTransform* v7 = intk->ca.Binding->TrackBindings;

            if (v7)
            {
                akxMotionTransform const* v8 = v7->srcTrack;

                if (v8)
                {
                    afxReal v9 = AkxGetMotorLocalDuration(moto);
                    afxReal LocalDuration = v9;

                    if (v9 > 0.0000099999997)
                    {
                        afxReal SecondsElapseda = AkxGetMotorSpeed(moto) * SecondsElapsed;
                        afxReal v10 = AkxQueryMotorRawLocalClock(moto);
                        afxReal EndLocalClock = v10;
                        afxBool FlipResult = Inverse;
                        afxReal StartLocalClock = v10 - SecondsElapseda;

                        if (StartLocalClock > (double)EndLocalClock)
                        {
                            EndLocalClock = v10 - SecondsElapseda;
                            StartLocalClock = v10;
                            FlipResult = Inverse == 0;
                        }

                        afxBool UnderflowLoop, OverflowLoop;
                        AkxQueryMotorIterationState(moto, &UnderflowLoop, &OverflowLoop);

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
                                afxReal LocalLoopCountb = v23 / AkxGetMotorLocalDuration(intk->moto);

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

                            if (!AfxCurveIsIdentity(v20))
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
