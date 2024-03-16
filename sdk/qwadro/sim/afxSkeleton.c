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
 *                       (c) 2017 SIGMA, Engineering In Technology
 *                             <https://sigmaco.org/qwadro/>
 */

#define _AFX_SIM_C
#define _AFX_SKELETON_C
#define _AFX_SIMULATION_C
#include "qwadro/sim/afxSimulation.h"
#include "qwadro/math/afxQuaternion.h"
#include "qwadro/math/afxVector.h"
#include "qwadro/math/afxMatrix.h"
#include "qwadro/sim/awxAsset.h"
#include "qwadro/core/afxIndexedString.h"

_AKXINL afxM4d* AfxGetSkeletonIwm(afxSkeleton skl, afxNat jointIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &skl, afxFcc_SKL);
    AfxAssertRange(skl->jointCnt, jointIdx, 1);
    return &skl->iw[jointIdx];
}

_AKXINL afxBool AfxGetSkeletonId(afxSkeleton skl, afxString* id)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &skl, afxFcc_SKL);
    AfxAssert(id);
    return AfxResolveStrings2(skl->strc, 1, &skl->id, id);
}

_AKXINL afxBool AfxGetSkeletonJointTag(afxSkeleton skl, afxNat jointIdx, afxString* id)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &skl, afxFcc_SKL);
    AfxAssertRange(skl->jointCnt, jointIdx, 1);
    AfxAssert(id);
    return AfxResolveStrings2(skl->strc, 1, &skl->joint[jointIdx], id);
}

_AKXINL void AfxDetermineSkeletonErrorTolerance(afxSkeleton skl, afxReal allowedErr, afxReal* allowedErrEnd, afxReal* allowedErrScaler)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &skl, afxFcc_SKL);

    if (allowedErr > 0.0)
    {
        afxReal s = skl->allowedLodErrFadingFactor * allowedErr;
        allowedErrEnd[0] = s;
        allowedErrScaler[0] = 1.0 / (allowedErr - s);
    }
    else
    {
        allowedErrEnd[0] = 0.0;
        allowedErrScaler[0] = 0.0;
    }
}

_AKX afxNat AfxCountSkeletonJoints(afxSkeleton skl, afxReal allowedErr)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &skl, afxFcc_SKL);
    afxNat rslt = 0;

    if (allowedErr == 0.0 || !skl->lodType) rslt = skl->jointCnt;
    else
    {
        afxReal errEnd, errScaler;
        AfxDetermineSkeletonErrorTolerance(skl, allowedErr, &errEnd, &errScaler);

        afxNat jointCnt = skl->jointCnt;
        afxReal* lodError = skl->lodError;

        if (jointCnt >= 4)
        {
            while (lodError[rslt + 0] >= errEnd)
            {
                if (lodError[rslt + 1] < errEnd)
                {
                    ++rslt;
                    break;
                }

                if (lodError[rslt + 2] < errEnd)
                {
                    rslt += 2;
                    break;
                }

                if (lodError[rslt + 3] < errEnd)
                {
                    rslt += 3;
                    break;
                }

                rslt += 4;

                if (rslt >= jointCnt - 3)
                    break;
            }
        }

        for (; rslt < jointCnt; rslt++)
            if (lodError[rslt] < errEnd)
                break;
    }
    return rslt;
}

_AKX afxNat AfxFindSkeletonJoints(afxSkeleton skl, afxStringCatalog strc, afxNat cnt, afxString const ids[], afxNat indices[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &skl, afxFcc_SKL);
    AfxAssert(indices);
    AfxAssert(cnt);
    AfxAssert(ids);
    afxNat rslt = 0;
    afxNat jointCnt = skl->jointCnt;

    for (afxNat j = 0; j < cnt; j++)
    {
        afxNat jointIdx = AFX_INVALID_INDEX;
        afxString target;

        if (!strc)
            target = ids[j];
        else
            AfxResolveStrings2(strc, 1, &ids[j], &target);

        for (afxNat i = 0; i < jointCnt; i++)
        {
            afxString jointTag;

            if (AfxGetSkeletonJointTag(skl, i, &jointTag) && (0 == AfxCompareStringCi(&target, &jointTag)))
            {
                jointIdx = i;
                ++rslt;
                break;
            }
        }
        indices[j] = jointIdx;
    }
    return rslt;
}

_AKX afxNat AfxFindSkeletonJoint(afxSkeleton skl, afxString const* id)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &skl, afxFcc_SKL);
    afxNat jointIdx;
    
    if (!AfxFindSkeletonJoints(skl, NIL, 1, id, &jointIdx))
        jointIdx = AFX_INVALID_INDEX;
    
    return jointIdx;
}

_AKX void AfxGetSkeletonAttachmentOffset(afxSkeleton skl, afxNat jointIdx, awxPose const* pos, afxM4d const offset, afxM4d m, afxNat const* sparseArtArray, afxNat const* sparseArtArrayReverse)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &skl, afxFcc_SKL);
    afxM4d tmp;
    AfxGetWorldMatrixFromLocalPose(skl, jointIdx, pos, offset, tmp, sparseArtArray, sparseArtArrayReverse);
    AfxInvertAtm4d(tmp, m);
}

#if 0
// Amen

_AKX afxError AfxDeserializeSkeletons(afxStream in, afxSimulation sim, afxNat cnt, afxSkeleton dst[])
{
    afxError err = NIL;
    AfxAssertObjects(1, &in, afxFcc_IOS);
    AfxAssert(dst);
    AfxAssert(cnt);

    for (afxNat i = 0; i < cnt; i++)
    {
        _afxSerializedChunk chnkHdr;
        AfxRead(in, 1, sizeof(chnkHdr), &chnkHdr, sizeof(chnkHdr));
        AfxAssertType(&chnkHdr, afxFcc_SKL);

        _afxSerializedSkl chnk = { 0 };
        AfxRead(in, 1, sizeof(chnk), &chnk, sizeof(chnk));
        afxNat jointCnt = chnk.jointCnt;

        afxSkeletonBuilder sklb;
        AfxBeginSkeletonBuilding(&sklb, jointCnt, &chnk.name, chnk.lodType);
        
        AfxRead(in, jointCnt, sizeof(sklb.bones[0].local), &sklb.bones[0].local, sizeof(sklb.bones[0]));

        AfxRead(in, jointCnt, sizeof(sklb.bones[0].iw), &sklb.bones[0].iw, sizeof(sklb.bones[0]));

        AfxRead(in, jointCnt, sizeof(sklb.bones[0].parentIdx), &sklb.bones[0].parentIdx, sizeof(sklb.bones[0]));

        AfxRead(in, jointCnt, sizeof(sklb.bones[0].lodError), &sklb.bones[0].lodError, sizeof(sklb.bones[0]));

        AfxRead(in, jointCnt, sizeof(sklb.bones[0].id.str), &sklb.bones[0].id.str, sizeof(sklb.bones[0]));

        if (AfxBuildSkeletons(sim, NIL, 1, &sklb, &dst[i]))
            AfxThrowError();

        AfxEndSkeletonBuilding(&sklb);

        afxSkeleton skl = dst[i];
    }

    return err;
}

_AKX afxError AfxSerializeSkeletons(afxStream out, afxNat cnt, afxSkeleton const src[])
{
    afxError err = NIL;
    AfxAssertObjects(1, &out, afxFcc_IOS);
    AfxAssert(src);
    AfxAssert(cnt);

    for (afxNat i = 0; i < cnt; i++)
    {
        _afxSerializedChunk chnkHdr;
        chnkHdr.fcc = afxFcc_SKL;
        chnkHdr.len = 0;
        afxSize chnkHdrPos = AfxAskStreamPosn(out);
        AfxWrite(out, 1, sizeof(chnkHdr), &chnkHdr, sizeof(chnkHdr));
        afxSize chnkStartPos = AfxAskStreamPosn(out);
        
        afxSkeleton skl = src[i];
        _afxSerializedSkl chnk = { 0 };
        afxNat jointCnt = (chnk.jointCnt = skl->jointCnt);
        AfxReflectString(&skl->id, &chnk.name);
        AfxWrite(out, 1, sizeof(chnk), &chnk, sizeof(chnk));

        AfxWrite(out, jointCnt, sizeof(skl->local[0]), &skl->local[0], sizeof(skl->local[0]));

        AfxWrite(out, jointCnt, sizeof(skl->iw[0]), &skl->iw[0], sizeof(skl->iw[0]));

        AfxWrite(out, jointCnt, sizeof(skl->parentIdx[0]), &skl->parentIdx[0], sizeof(skl->parentIdx[0]));

        AfxWrite(out, jointCnt, sizeof(skl->lodError[0]), &skl->lodError[0], sizeof(skl->lodError[0]));

        AfxWrite(out, jointCnt, sizeof(skl->joint[0]), &skl->joint[0], sizeof(skl->joint[0]));


        chnkHdr.len = AfxAskStreamPosn(out) - chnkStartPos;
        AfxWriteAt(out, chnkHdrPos, 1, sizeof(chnkHdr), &chnkHdr, sizeof(chnkHdr));
        AfxSkipStream(out, (afxInt)chnkHdr.len);
    }

    return err;
}
#endif

_AKX void AfxBuildIndexedCompositeBufferTransposed(afxSkeleton skl, awxPoseBuffer const* posb, afxNat const* indices, afxNat idxCnt, afxReal buffer[][3][4])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &skl, afxFcc_SKL);
    AfxAssertRange(skl->jointCnt, idxCnt, 1);
    AfxAssertRange(posb->xformCnt, idxCnt, 1);
    AfxAssert(posb);
    AfxAssert(buffer);

    for (afxNat i = 0; i < idxCnt; i++)
    {
        Mmt.BuildSingleCompositeFromPoseBufferTranspose_Generic(skl->iw[indices[i]], posb->world[indices[i]], buffer[i]);
    }
}

_AKX void AfxBuildIndexedCompositeBuffer(afxSkeleton skl, awxPoseBuffer const* wp, afxNat const* indices, afxNat idxCnt, afxM4d buffer[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &skl, afxFcc_SKL);
    AfxAssertRange(skl->jointCnt, idxCnt, 1);
    AfxAssertRange(wp->xformCnt, idxCnt, 1);
    AfxAssert(wp);
    AfxAssert(buffer);

    for (afxNat i = 0; i < idxCnt; i++)
    {
        Mmt.BuildSingleCompositeFromPoseBuffer_Generic(skl->iw[indices[i]], wp->world[indices[i]], buffer[i]);
    }
}

void UpdatePoseBufferChildren(afxSkeleton Skeleton, afxNat ParentBone, awxPose *LocalPose, afxM4d const Offset4x4, awxPoseBuffer *Result)
{
    float Dest;

    afxNat v6 = Skeleton->jointCnt;
    afxNat v23 = Skeleton->jointCnt;

    if (ParentBone < 0 || ParentBone > v6)
    {
        afxError err;
        AfxThrowError();
    }
    else
    {
        afxM4d* v7 = Offset4x4;
        afxNat v8 = LocalPose->traversalId + 1;
        LocalPose->traversalId = v8;
        afxNat v28 = v8;

        if (!Offset4x4)
        {
            Offset4x4 = (const float *)AFX_M4D_IDENTITY;
            v7 = (const float *)AFX_M4D_IDENTITY;
        }

        if ((unsigned __int8)v7 & 0xF)
        {
            AfxCopyM4d(&Dest, v7);
            Offset4x4 = &Dest;
        }

        afxM4d* w = Result->world;
        afxM4d* composite = Result->composite;
        awxArticulation* v26 = LocalPose->xforms;
        afxM4d* iw = Skeleton->iw;
        afxNat* parentIdx = Skeleton->parentIdx;
        afxNat v12 = ParentBone;

        //World = (float *)&v9[4 * ParentBone];
        afxNat v24 = v23 - ParentBone;

        if (v24 > 0)
        {
            while (1)
            {
                afxNat v14 = parentIdx[v12];
                afxM4d* v15 = v14 == -1 ? Offset4x4 : &w[v14];
                afxTransform* v16 = &v26[v12].xform;

                if (composite)
                {
                    Mmt.BWP_Dispatch(v16, (const float *)iw[v12], composite[v12], w[v12], v15);
                }
                else
                {
                    Mmt.BWPNC_Dispatch(v16, v15, w[v12]);
                }

                v26[v12].traversalId = v28;
                ++v12;
                afxNat v17 = v24 - 1;
                v24 = v17;

                if (v17 <= 0)
                    break;

                while (1)
                {
                    afxNat v18 = parentIdx[v12];

                    if (v18 != -1 && v26[v18].traversalId == v28)
                        break;

                    ++v12;
                    --v17;
                    v24 = v17;

                    if (v17 <= 0)
                        goto LABEL_28;
                }

                if (v17 <= 0)
                    break;
            }
        }
    }
    int a;
LABEL_28:
    a = 1;
}

void IKUpdate(afxNat LinkCountInit, afxNat EEBoneIndex, const afxReal *DesiredPosition3, afxNat IterationCount, afxSkeleton Skeleton, const afxReal *ModelRootTransform, awxPose *LocalPose, awxPoseBuffer *PoseBuffer)
{
    afxNat Links[256];
    afxNat v9 = EEBoneIndex;
    afxNat v10 = 0;
    afxNat v11 = 0;
    afxNat LinkCount = 0;

    if (LinkCountInit >= 0)
    {
        do
        {
            Links[v11] = v9;
            v9 = Skeleton->parentIdx[v9];
            ++v10;
            
            if (v9 == -1)
                break;

            ++v11;
        } while (v11 <= LinkCountInit);
        LinkCount = v10;
    }

    if (IterationCount > 0)
    {
        afxNat v46 = IterationCount;

        do
        {
            afxNat v13 = 1;

            if (v10 > 1)
            {
                do
                {
                    afxReal* v14 = PoseBuffer->world[Links[0]];
                    afxReal v15 = v14[13];
                    afxNat v16 = Links[v13];
                    afxV3d CurrentPosition;
                    CurrentPosition[0] = v14[12];
                    afxReal v17 = v14[14];
                    CurrentPosition[1] = v15;
                    CurrentPosition[2] = v17;
                    afxTransform* v18 = &LocalPose->xforms[v16].xform;
                    afxReal* v19 = PoseBuffer->world[v16];
                    afxReal v20 = DesiredPosition3[0] - v19[12];
                    afxReal v21 = DesiredPosition3[1] - v19[13];
                    afxReal VectorToDesired_8 = DesiredPosition3[2] - v19[14];
                    afxReal v23 = v20;
                    afxReal v45 = v21;
                    afxReal v47 = v19[12] * 0.0;
                    afxReal VectorToDesired = v20 * v19[0] + VectorToDesired_8 * v19[2] + v21 * v19[1] + v47;
                    afxReal v48 = v19[13] * 0.0;
                    afxReal v25 = VectorToDesired_8 * v19[6] + v21 * v19[5] + v23 * v19[4] + v48;
                    afxReal v26 = v19[14] * 0.0;
                    afxReal v27 = v23 * v19[8] + VectorToDesired_8 * v19[10] + v45 * v19[9] + v26;
                    afxReal v28 = CurrentPosition[0] - v19[12];
                    afxReal v29 = CurrentPosition[1] - v19[13];
                    afxReal VectorToActual_8 = CurrentPosition[2] - v19[14];
                    afxReal v31 = v28;
                    v45 = v29;
                    afxReal VectorToActual = VectorToActual_8 * v19[2] + v29 * v19[1] + v28 * *v19 + v47;
                    afxReal v33 = VectorToActual_8 * v19[6] + v29 * v19[5] + v31 * v19[4] + v48;
                    afxReal v34 = v31 * v19[8] + VectorToActual_8 * v19[10] + v45 * v19[9] + v26;
                    afxReal v35 = VectorToActual + VectorToDesired;
                    afxReal v36 = v35 * 0.5;
                    afxReal VectorToActual_4 = (v33 + v25) * 0.5;
                    afxReal v38 = (v34 + v27) * 0.5;
                    afxQuat LocalRotationIncrement;
                    LocalRotationIncrement[0] = VectorToActual_4 * v27 - v38 * v25;
                    LocalRotationIncrement[1] = VectorToDesired * v38 - v27 * v36;
                    LocalRotationIncrement[2] = v25 * v36 - VectorToActual_4 * VectorToDesired;
                    afxReal v39 = v38 * v27 + VectorToActual_4 * v25 + v36 * VectorToDesired;
                    afxReal* v40 = v18->orientation;
                    afxReal v41 = 1.0 / sqrt(LocalRotationIncrement[2] * LocalRotationIncrement[2] + LocalRotationIncrement[1] * LocalRotationIncrement[1] + LocalRotationIncrement[0] * LocalRotationIncrement[0] + v39 * v39);
                    LocalRotationIncrement[0] = LocalRotationIncrement[0] * v41;
                    LocalRotationIncrement[1] = LocalRotationIncrement[1] * v41;
                    LocalRotationIncrement[2] = LocalRotationIncrement[2] * v41;
                    LocalRotationIncrement[3] = v41 * v39;
                    v18->flags = v18->flags | 2;
                    AfxMultiplyQuat(v40, v40, LocalRotationIncrement);
                    UpdatePoseBufferChildren(Skeleton, v16, LocalPose, ModelRootTransform, PoseBuffer);
                    ++v13;
                } while (v13 < LinkCount);

                v10 = LinkCount;
            }
            --v46;
        } while (v46);
    }
}

char IKUpdate2Bone(afxNat EEBoneIndex, const afxReal *DesiredPosition3, const afxReal *RestrictedMovementPlaneNormal3, afxSkeleton Skeleton, const afxReal *ModelRootTransform, awxPose *LocalPose, awxPoseBuffer *PoseBuffer, afxReal HyperExtensionStart, afxReal HyperExtensionScale)
{
    int v10; // ebx
    int v11; // esi
    int v12; // edi
    float *v13; // ebx
    float *v14; // eax
    float *v15; // ecx
    double v16; // st7
    double v17; // st7
    double v18; // st7
    double v19; // st6
    double v20; // st5
    double v21; // st7
    double v22; // st6
    double v23; // st5
    double v24; // st6
    double v25; // st5
    long double v26; // st7
    float *v27; // eax
    float v28; // eax
    float *v30; // ST40_4
    float *v31; // ST3C_4
    const float *v32; // ST38_4
    float *v33; // ST34_4
    const afxTransform *v34; // eax
    int v35; // ebx
    float *v36; // ST40_4
    float *v37; // ST3C_4
    const float *v38; // ST38_4
    const float *v39; // ST34_4
    const afxTransform *v40; // eax
    float *v41; // ST40_4
    float *v42; // ST3C_4
    const float *v43; // ST38_4
    const float *v44; // ST34_4
    const afxTransform *v45; // eax
    float *v46; // ST40_4
    float *v47; // ST3C_4
    const afxTransform *v48; // eax
    float *v49; // ST40_4
    const float *v50; // ST3C_4
    const afxTransform *v51; // eax
    float *v52; // ST40_4
    const float *v53; // ST3C_4
    const afxTransform *v54; // eax
    float *v55; // eax
    float *v56; // ebx
    int v57; // eax
    float Operand1; // [esp+18h] [ebp-88h]
    float v60; // [esp+1Ch] [ebp-84h]
    float v61; // [esp+20h] [ebp-80h]
    float *Transform; // [esp+24h] [ebp-7Ch]
    char v63; // [esp+2Bh] [ebp-75h]
    float Dest; // [esp+2Ch] [ebp-74h]
    float v65; // [esp+30h] [ebp-70h]
    float v66; // [esp+34h] [ebp-6Ch]
    float v67; // [esp+38h] [ebp-68h]
    int BoneIndex; // [esp+3Ch] [ebp-64h]
    float v69; // [esp+40h] [ebp-60h]
    int v70; // [esp+44h] [ebp-5Ch]
    float Operand2; // [esp+48h] [ebp-58h]
    float v72; // [esp+4Ch] [ebp-54h]
    float v73; // [esp+50h] [ebp-50h]
    float v74; // [esp+54h] [ebp-4Ch]
    unsigned int v75; // [esp+58h] [ebp-48h]
    float *v76; // [esp+5Ch] [ebp-44h]
    float v77; // [esp+60h] [ebp-40h]

    v10 = Skeleton->parentIdx[EEBoneIndex];
    v63 = 1;
    v70 = 152 * EEBoneIndex;
    BoneIndex = v10;

    afxError err;

    if (v10 < 0 || (v11 = Skeleton->jointCnt, v10 >= v11))
    {
        AfxThrowError();
        return 0;
    }
    v12 = Skeleton->parentIdx[v10];
    v75 = 152 * v10;
    
    if (v12 < 0 || v12 >= v11)
    {
        AfxThrowError();
        return 0;
    }

    Transform = ((afxReal*)(PoseBuffer->world[v12])) + 12;
    v13 = ((afxReal*)(PoseBuffer->world[v10])) + 12;
    v14 = ((afxReal*)(PoseBuffer->world[EEBoneIndex]));
    v15 = Transform;
    v16 = DesiredPosition3[0] - Transform[0];
    v76 = v14 + 12;
    v67 = 0.0;
    Operand1 = v16;
    v60 = DesiredPosition3[1] - Transform[1];
    v17 = DesiredPosition3[2] - Transform[2];
    v61 = v17;
    v69 = v17 * v61 + v60 * v60 + Operand1 * Operand1;
    v18 = Transform[0] - v13[0];
    v19 = Transform[1] - v13[1];
    v20 = Transform[2] - v13[2];
    *(float *)&Transform = v20 * v20 + v19 * v19 + v18 * v18;

    if (fabs(v69) >= 0.0000000099999999)
    {
        v23 = v13[2] - v14[14];
        v22 = v13[1] - v14[13];
        v21 = *v13 - v14[12];
        v67 = ((*(float *)&Transform - (v23 * v23 + v22 * v22 + v21 * v21)) / v69 + 1.0) * 0.5;
    }
    else
    {
        v63 = 0;
    }

    Operand2 = Operand1 * v67 + *v15;
    v72 = v60 * v67 + v15[1];
    v73 = v61 * v67 + v15[2];
    AfxCrossV3d(&Dest, &Operand1, RestrictedMovementPlaneNormal3);
    v24 = *(float *)&Transform - v67 * v67 * v69;
    v25 = *(float *)&Transform * HyperExtensionStart;
    
    if (v24 < v25)
    {
        v24 = v25 + (v24 - v25) * HyperExtensionScale;
        
        if (v24 < 0.0)
            v24 = 0.0;
    }

    v26 = sqrt(v24 / (v66 * v66 + v65 * v65 + Dest * Dest));
    Operand1 = Dest * v26 + Operand2;
    v60 = v65 * v26 + v72;
    v61 = v66 * v26 + v73;
    v27 = PoseBuffer->world[v12];
    Dest = Operand1 - v27[12];
    Transform = v27;
    v65 = v60 - v27[13];
    v66 = v61 - v27[14];
    TransposeVectorTransform4x3(&Dest, 0.0, v27);
    Operand1 = *v13 - Transform[12];
    v60 = v13[1] - Transform[13];
    v61 = v13[2] - Transform[14];
    TransposeVectorTransform4x3(&Operand1, 0.0, Transform);
    v61 = v61 + v66;
    Operand1 = (Operand1 + Dest) * 0.5;
    v60 = (v60 + v65) * 0.5;
    v61 = v61 * 0.5;
    AfxCrossV3d(&Operand2, &Operand1, &Dest);
    v74 = v61 * v66 + v60 * v65 + Operand1 * Dest;
    AfxNormalizeQuat(&Operand2, &Operand2);
    LocalPose->xforms[v12].xform.flags |= 2u;
    AfxMultiplyQuat(LocalPose->xforms[v12].xform.orientation, LocalPose->xforms[v12].xform.orientation, &Operand2);
    
    if (Skeleton->parentIdx[v12] == -1)
    {
        AfxCopyM4d(&v77, ModelRootTransform);
        Transform = &v77;
    }
    else
    {
        Transform = PoseBuffer->world[Skeleton->parentIdx[v12]];
    }
    
    if (PoseBuffer->composite)
    {
        v30 = PoseBuffer->world[v12];
        v31 = PoseBuffer->composite[v12];
        v32 = (const float *)Skeleton->iw[v12];
        v33 = Transform;
        v34 = &LocalPose->xforms[v12].xform;
        Mmt.BuildFullPoseBufferComposite_Generic(v34, v33, v32, v31, v30);
        v35 = BoneIndex;
        v36 = PoseBuffer->world[BoneIndex];
        v37 = PoseBuffer->composite[v35];
        v38 = (const float *)Skeleton->iw[v75 / 0x98];
        v39 = PoseBuffer->world[v12];
        v40 = &LocalPose->xforms[v35].xform;
        Mmt.BuildFullPoseBufferComposite_Generic(v40, v39, v38, v37, v36);
        v41 = PoseBuffer->world[EEBoneIndex];
        v42 = PoseBuffer->composite[EEBoneIndex];
        v43 = (const float *)((char *)Skeleton->iw[v70]);
        v44 = PoseBuffer->world[v35];
        v45 = &LocalPose->xforms[EEBoneIndex].xform;
        Mmt.BuildFullPoseBufferComposite_Generic(v45, v44, v43, v42, v41);
    }
    else
    {
        v46 = PoseBuffer->world[v12];
        v47 = Transform;
        v48 = &LocalPose->xforms[v12].xform;
        Mmt.BuildFullPoseBufferOnly_Generic(v48, v47, v46);
        v49 = PoseBuffer->world[BoneIndex];
        v50 = PoseBuffer->world[v12];
        v51 = &LocalPose->xforms[BoneIndex].xform;
        Mmt.BuildFullPoseBufferOnly_Generic(v51, v50, v49);
        v52 = PoseBuffer->world[EEBoneIndex];
        v53 = PoseBuffer->world[BoneIndex];
        v54 = &LocalPose->xforms[EEBoneIndex].xform;
        Mmt.BuildFullPoseBufferOnly_Generic(v54, v53, v52);
        v35 = BoneIndex;
    }
    
    v70 = (int)&LocalPose->xforms[v35].xform;
    v55 = PoseBuffer->world[v35];
    v56 = v55;
    Dest = *DesiredPosition3 - v55[12];
    v65 = DesiredPosition3[1] - v55[13];
    v66 = DesiredPosition3[2] - v55[14];
    TransposeVectorTransform4x3(&Dest, 0.0, v55);
    Operand1 = *v76 - v56[12];
    v60 = v76[1] - v56[13];
    v61 = v76[2] - v56[14];
    TransposeVectorTransform4x3(&Operand1, 0.0, v56);
    v61 = v61 + v66;
    Operand1 = (Operand1 + Dest) * 0.5;
    v60 = (v60 + v65) * 0.5;
    v61 = v61 * 0.5;
    AfxCrossV3d(&Operand2, &Operand1, &Dest);
    v74 = v61 * v66 + v60 * v65 + Operand1 * Dest;
    AfxNormalizeQuat(&Operand2, &Operand2);
    v57 = v70;
    LocalPose->xforms[v35].xform.flags |= 2u;
    AfxMultiplyQuat((float *)(v57 + 16), (const float *)(v57 + 16), &Operand2);
    UpdatePoseBufferChildren(Skeleton, v12, LocalPose, ModelRootTransform, PoseBuffer);
    return v63;
}

_AKXINL void AfxComputePoseBufferNoCompositeLod(afxSkeleton skl, afxNat baseJoint, afxNat jointCnt, afxNat firstValidLocalArt, afxNat validLocalArtCnt, awxPose const *pos, afxM4d const offset, awxPoseBuffer *posb)
{
    // Should be compatible with void BuildPoseBufferNoCompositeLOD(const skeleton *Skeleton, int FirstBone, int BoneCount, int FirstValidLocalBone, int ValidLocalBoneCount, const granny::local_pose *LocalPose, const float *Offset4x4, world_pose *Result)

    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &skl, afxFcc_SKL);
    AfxAssertRange(skl->jointCnt, baseJoint, jointCnt);
    AfxAssertRange(posb->xformCnt, baseJoint, jointCnt);
    AfxAssertRange(pos->cap, baseJoint, jointCnt);

    if (!offset)
        offset = AFX_M4D_IDENTITY;
    
    afxM4d* w = posb->world;

    for (afxNat i = 0; i < jointCnt; i++)
    {
        afxNat jointIdx = baseJoint + i;
        AfxAssertRange(skl->jointCnt, jointIdx, 1);
        afxNat parentIdx = skl->parentIdx[jointIdx];

        if (parentIdx != AFX_INVALID_INDEX)
        {
            AfxAssertRange(skl->jointCnt, parentIdx, 1);
        }

        afxTransform *t = &pos->xforms[jointIdx].xform;

        if (validLocalArtCnt <= 0)
            t = &skl->local[jointIdx];

        Mmt.BWPNC_Dispatch(t, ((parentIdx == AFX_INVALID_INDEX) ? offset : w[parentIdx]), w[jointIdx]);
        --validLocalArtCnt;
    }
}

_AKXINL void AfxComputePoseBufferLod(afxSkeleton skl, afxNat baseJoint, afxNat jointCnt, afxNat firstValidLocalArt, afxNat validLocalArtCnt, awxPose const* pos, afxM4d const offset, awxPoseBuffer* posb)
{
    // Should be compatible with void BuildPoseBufferLOD(const skeleton *Skeleton, int FirstBone, int BoneCount, int FirstValidLocalBone, int ValidLocalBoneCount, const local_pose *LocalPose, const float *Offset4x4, world_pose *Result)

    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &skl, afxFcc_SKL);
    AfxAssertRange(skl->jointCnt, baseJoint, jointCnt);
    AfxAssertRange(posb->xformCnt, baseJoint, jointCnt);
    AfxAssertRange(pos->cap, baseJoint, jointCnt);

    if (posb->composite)
    {
        if (!offset)
            offset = AFX_M4D_IDENTITY;

        afxM4d* w = posb->world;
        afxM4d* iw = skl->iw;
        afxM4d* composite = posb->composite;

        for (afxNat i = 0; i < jointCnt; i++)
        {
            afxNat jointIdx = baseJoint + i;
            AfxAssertRange(skl->jointCnt, jointIdx, 1);
            afxNat parentIdx = skl->parentIdx[jointIdx];

            if (parentIdx != AFX_INVALID_INDEX)
            {
                AfxAssertRange(skl->jointCnt, parentIdx, 1);
            }

            afxTransform *t = &pos->xforms[jointIdx].xform;

            if (validLocalArtCnt <= 0)
                t = &skl->local[jointIdx];

            Mmt.BWP_Dispatch(t, iw[jointIdx], composite[jointIdx], w[jointIdx], ((parentIdx == AFX_INVALID_INDEX) ? offset : w[parentIdx]));
        }
    }
    else
    {
        AfxComputePoseBufferNoCompositeLod(skl, baseJoint, jointCnt, firstValidLocalArt, validLocalArtCnt, pos, offset, posb);
    }
}

_AKX void AfxComputePoseBuffer(afxSkeleton skl, afxNat baseJoint, afxNat jointCnt, awxPose const* pos, afxM4d const offset, awxPoseBuffer* posb)
{
    // Should be compatible with void BuildPoseBuffer(const skeleton *Skeleton, int FirstBone, int BoneCount, const local_pose *LocalPose, const float *Offset4x4, world_pose *Result)

    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &skl, afxFcc_SKL);
    AfxAssertRange(pos->cap, baseJoint, jointCnt);
    AfxAssertRange(skl->jointCnt, baseJoint, jointCnt);
    AfxAssertRange(posb->xformCnt, baseJoint, jointCnt);
    AfxComputePoseBufferLod(skl, baseJoint, jointCnt, baseJoint, jointCnt, pos, offset, posb);
}

_AKX void AfxComputeRestPoseBuffer(afxSkeleton skl, afxNat baseJoint, afxNat jointCnt, afxM4d const offset, awxPoseBuffer* posb)
{
    // Should be compatible with void BuildRestPoseBuffer(const skeleton *Skeleton, int FirstBone, int BoneCount, const float *Offset4x4, world_pose *Result)

    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &skl, afxFcc_SKL);
    AfxAssertRange(skl->jointCnt, baseJoint, jointCnt);
    AfxAssertRange(posb->xformCnt, baseJoint, jointCnt);

    if (!offset)
        offset = AFX_M4D_IDENTITY;

    afxM4d* w = posb->world;
    afxM4d* iw = skl->iw;
    afxM4d* composite = posb->composite;
    afxTransform* lt = skl->local;

    for (afxNat i = 0; i < jointCnt; i++)
    {
        afxNat jointIdx = baseJoint + i;
        AfxAssertRange(skl->jointCnt, jointIdx, 1);
        afxNat parentIdx = skl->parentIdx[jointIdx];
        
        if (parentIdx != AFX_INVALID_INDEX)
        {
            AfxAssertRange(skl->jointCnt, parentIdx, 1);
        }

        if (posb->composite)
        {
            Mmt.BWP_Dispatch(&lt[jointIdx], iw[jointIdx], composite[jointIdx], w[jointIdx], ((AFX_INVALID_INDEX == parentIdx) ? offset : w[parentIdx]));
        }
        else
        {
            Mmt.BWPNC_Dispatch(&lt[jointIdx], ((AFX_INVALID_INDEX == parentIdx) ? offset : w[parentIdx]), w[jointIdx]);
        }
    }
}

_AKX void AfxComputeRestLocalPose(afxSkeleton skl, afxNat baseJoint, afxNat jointCnt, awxPose* pos)
{
    // Should be compatible with void BuildRestLocalPose(const skeleton *Skeleton, int FirstBone, int BoneCount, local_pose *LocalPose)

    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &skl, afxFcc_SKL);
    AfxAssertRange(pos->cap, baseJoint, jointCnt);
    AfxAssertRange(skl->jointCnt, baseJoint, jointCnt);

    for (afxNat i = jointCnt; i-- > 0;)
    {
        afxNat jointIdx = baseJoint + i;
        AfxAssertRange(skl->jointCnt, jointIdx, 1);
        awxArticulation *t = &pos->xforms[jointIdx];
        t->cnt = 1;
        t->weight = 1.0;
        AfxCopyTransform(&t->xform, &skl->local[jointIdx]);
    }
}

_AKX void AfxLocalPoseFromPoseBuffer(afxSkeleton skl, awxPose* pos, awxPoseBuffer const* posb, afxM4d const offset, afxNat firstBone, afxNat jointCnt)
{
    // Should be compatible with void LocalPoseFromPoseBuffer(const skeleton *Skeleton, local_pose *Result, const world_pose *PoseBuffer, const float *Offset4x4, int FirstBone, int BoneCount)

    afxError err = AFX_ERR_NONE;
    AfxAssert(pos);
    AfxAssert(posb);
    AfxAssertObjects(1, &skl, afxFcc_SKL);
    AfxAssertRange(skl->jointCnt, firstBone, jointCnt);
    AfxAssertRange(pos->cap, firstBone, jointCnt);
    AfxAssertRange(posb->xformCnt, firstBone, jointCnt);

    if (!offset)
        offset = AFX_M4D_IDENTITY;

    for (afxNat i = jointCnt; i-- > 0;)
    {
        afxNat jointIdx = firstBone + i;
        AfxAssertRange(skl->jointCnt, jointIdx, 1);
        afxNat parentIdx = skl->parentIdx[jointIdx];
        afxM4d InverseParent;

        if (parentIdx == AFX_INVALID_INDEX) AfxInvertAtm4d(offset, InverseParent);
        else
        {
            AfxAssertRange(skl->jointCnt, parentIdx, 1);
            AfxInvertAtm4d(posb->world[parentIdx], InverseParent);
        }

        afxM4d Local;
        AfxMultiplyAtm4d(Local, posb->world[jointIdx], InverseParent);
        
        afxM3d Linear;
        AfxSetM3d(Linear, Local[0], Local[1], Local[2]);

        afxM3d Q, Scale;

        if (!AfxPolarDecomposeM3d(Linear, 0.0000099999997, Q, Scale))
            AfxError("Can't accurately decompose MAX transform Q");

        afxV3d Position;
        AfxCopyV3d(Position, Local[3]);
        afxQuat Orientation;
        AfxQuatFromM3d(Orientation, Q);
        AfxNormalizeQuat(Orientation, Orientation);
        afxTransform* t = AfxGetPoseTransform(pos, jointIdx);
        AfxSetTransformWithIdentityCheck(t, Position, Orientation, Scale);
    }
}

_AKX void AfxLocalPoseFromPoseBufferNoScale(afxSkeleton skl, awxPose* lp, awxPoseBuffer const* wp, afxM4d const offset, afxNat firstBone, afxNat jointCnt)
{
    // Should be compatible with void LocalPoseFromPoseBufferNoScale(const skeleton *Skeleton, local_pose *Result, const world_pose *PoseBuffer, const float *Offset4x4, int FirstBone, int BoneCount)

    afxError err = AFX_ERR_NONE;
    AfxAssert(wp);
    AfxAssert(lp);
    AfxAssertObjects(1, &skl, afxFcc_SKL);
    AfxAssertRange(skl->jointCnt, firstBone, jointCnt);
    AfxAssertRange(lp->cap, firstBone, jointCnt);
    AfxAssertRange(wp->xformCnt, firstBone, jointCnt);

    if (!offset)
        offset = AFX_M4D_IDENTITY;

    for (afxNat i = jointCnt; i-- > 0;)
    {
        afxNat jointIdx = firstBone + i;
        afxNat parentIdx = skl->parentIdx[jointIdx];
        afxM4d InverseParent;

        if (parentIdx == AFX_INVALID_INDEX) AfxInvertAtm4d(offset, InverseParent);
        else
        {
            AfxAssertRange(skl->jointCnt, parentIdx, 1);
            AfxInvertAtm4d(wp->world[parentIdx], InverseParent);
        }

        afxM4d Local;
        AfxMultiplyAtm4d(Local, wp->world[jointIdx], InverseParent);
        afxTransform* t = AfxGetPoseTransform(lp, jointIdx);

        AfxCopyV3d(t->position, Local[3]);
        afxM3d Rotation;
        AfxSetM3dTransposed(Rotation, Local[0], Local[1], Local[2]);
        AfxQuatFromM3d(t->orientation, Rotation);
        AfxNormalizeQuat(t->orientation, t->orientation);
        AfxResetM3d(t->scaleShear);
        t->flags = afxTransformFlags_TRANSLATED | afxTransformFlags_ROTATED;
    }
}

_AKX void AfxGetWorldMatrixFromLocalPose(afxSkeleton skl, afxNat jointIdx, awxPose const* lp, afxM4d const offset, afxM4d m, afxNat const* sparseBoneArray, afxNat const* sparseBoneArrayReverse)
{
    // Should be compatible with void GetWorldMatrixFromLocalPose(const skeleton *Skeleton, int BoneIndex, const local_pose *LocalPose, const float *Offset4x4, float *Result4x4, const int *SparseBoneArray, const int *SparseBoneArrayReverse)

    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &skl, afxFcc_SKL);
    AfxAssertRange(lp->cap, jointIdx, 1);

    AfxResetM4d(m);

    for (afxNat i = jointIdx; i != AFX_INVALID_INDEX; i = skl->parentIdx[i])
    {
        AfxAssertRange(skl->jointCnt, i, 1);
        afxTransform* t = AfxGetPoseTransform(lp, sparseBoneArrayReverse ? sparseBoneArrayReverse[i] : i);
        afxM4d tmp, tmp2;
        AfxComputeTransformM4d(t, tmp);
        AfxMultiplyAtm4d(tmp2, m, tmp);
        AfxCopyM4d(m, tmp2);
    }

    if (offset)
    {
        afxM4d tmp2;
        AfxMultiplyAtm4d(tmp2, m, offset);
        AfxCopyM4d(m, tmp2);
    }
}

#if 0
_AKXINL void _AfxSklCtorBubbleSortOnLod(afxSkeletonBlueprint *Builder, afxSkeletonBone *Bones, afxNat* remappingTable)
{
    afxSkeletonBone Temp;

    afxArena SortArena;
    AfxAllocateArena(NIL, &SortArena, NIL, AfxHint());
    afxNat *OldToNewMap = AfxRequestArenaUnit(&SortArena, sizeof(afxNat) * Builder->bones.cnt);
    afxNat *NewToOldMap = AfxRequestArenaUnit(&SortArena, sizeof(afxNat) * Builder->bones.cnt);
    afxNat v9 = 0;
    
    if (Builder->bones.cnt > 0)
    {
        afxNat *v10 = NewToOldMap;
        do
        {
            *(int *)((char *)v10 + (char *)OldToNewMap - (char *)NewToOldMap) = AFX_INVALID_INDEX;
            *v10 = v9++;
            ++v10;
        } while (v9 < Builder->bones.cnt);
    }
    
    afxByte* v11 = AfxRequestArenaUnit(&SortArena, sizeof(afxSkeletonBone) * Builder->bones.cnt);

    for (afxNat i = Builder->bones.cnt; i--> 0;)
    {
        Bones[i].name;
    }

    afxSkeletonBone* v12 = Bones;

    if (152 * Builder->bones.cnt)
    {
        afxNat v13 = v11 - (afxByte *)Bones;
        afxNat v14 = 152 * Builder->bones.cnt;

        do
        {
            *((afxByte *)&v12->name + v13) = v12->name;
            v12 = (afxSkeletonBone *)((char *)v12 + 1);
            --v14;
        } while (v14);
    }

    afxNat v15 = Builder->bones.cnt - 1;

    if (v15 >= 0)
    {
        afxNat v35 = Builder->bones.cnt;

        do
        {
            afxNat v16 = 0;
            afxNat v17 = v15 - 1;
            afxNat v32 = v15 - 1;

            if (v15 >= 4)
            {
                afxSkeletonBone *v18 = Bones + 1;
                afxNat32 v34 = (unsigned int)v15 >> 2;
                afxNat *v19 = NewToOldMap + 2;
                afxNat j = 4 * ((unsigned int)v15 >> 2);

                do
                {
                    if (v18[-1].lodError < (double)v18->lodError)
                    {
                        afxNat v20 = *(v19 - 2);
                        *(v19 - 2) = *(v19 - 1);
                        *(v19 - 1) = v20;
                        memcpy(&Temp, &v18[-1], sizeof(Temp));
                        memcpy(&v18[-1], v18, sizeof(afxSkeletonBone));
                        memcpy(v18, &Temp, sizeof(afxSkeletonBone));
                    }

                    if (v18->lodError < (double)v18[1].lodError)
                    {
                        afxNat v21 = *(v19 - 1);
                        *(v19 - 1) = *v19;
                        *v19 = v21;
                        memcpy(&Temp, v18, sizeof(Temp));
                        memcpy(v18, &v18[1], sizeof(afxSkeletonBone));
                        memcpy(&v18[1], &Temp, sizeof(afxSkeletonBone));
                    }

                    if (v18[1].lodError < (double)v18[2].lodError)
                    {
                        afxNat v22 = *v19;
                        *v19 = v19[1];
                        v19[1] = v22;
                        memcpy(&Temp, &v18[1], sizeof(Temp));
                        memcpy(&v18[1], &v18[2], sizeof(afxSkeletonBone));
                        memcpy(&v18[2], &Temp, sizeof(afxSkeletonBone));
                    }

                    if (v18[2].lodError < (double)v18[3].lodError)
                    {
                        afxNat v23 = v19[1];
                        v19[1] = v19[2];
                        v19[2] = v23;
                        memcpy(&Temp, &v18[2], sizeof(Temp));
                        memcpy(&v18[2], &v18[3], sizeof(afxSkeletonBone));
                        memcpy(&v18[3], &Temp, sizeof(afxSkeletonBone));
                    }
                    v19 += 4;
                    v18 += 4;
                    --v34;
                } while (v34);

                v16 = j;
                v17 = v32;
            }

            if (v16 <= v17)
            {
                afxSkeletonBone *v24 = &Bones[v16 + 1];

                do
                {
                    if (v24[-1].lodError < (double)v24->lodError)
                    {
                        afxNat v25 = NewToOldMap[v16];
                        NewToOldMap[v16] = NewToOldMap[v16 + 1];
                        NewToOldMap[v16 + 1] = v25;
                        memcpy(&Temp, &v24[-1], sizeof(Temp));
                        memcpy(&v24[-1], v24, sizeof(afxSkeletonBone));
                        memcpy(v24, &Temp, sizeof(afxSkeletonBone));
                    }
                    v17 = v32;
                    ++v16;
                    ++v24;
                } while (v16 <= v32);

            }
            v15 = v17;
            --v35;
        } while (v35);
    }

    for (afxNat i = 0; i < Builder->bones.cnt; ++i)
        OldToNewMap[NewToOldMap[i]] = i;

    if (remappingTable)
    {
        for (afxNat k = 0; k < Builder->bones.cnt; ++k)
            remappingTable[k] = OldToNewMap[remappingTable[k]];
    }

    for (afxNat i = 0; i < Builder->bones.cnt; ++i)
    {
        if (AFX_INVALID_INDEX != Bones[i].parentIdx)
            Bones[i].parentIdx = OldToNewMap[Bones[i].parentIdx];
    }

    AfxDeallocateArena(&SortArena);
}
#endif

_AKXINL void _AfxSklCtorWriteBone(afxSkeletonBuilder const* sklb, afxSkeleton skl, afxNat* boneWrittenMap, afxNat inBoneIdx, afxNat* outBoneIdx)
{
    afxError err = NIL;    
    afxNat idx = boneWrittenMap[inBoneIdx];

    if (idx == (afxNat)-2)
    {
        AfxError("Circular parenting chain detected at bone %d", inBoneIdx);
        AfxThrowError();
    }
    else if (idx == AFX_INVALID_INDEX)
    {
        boneWrittenMap[inBoneIdx] = (afxNat)-2; // temp

        afxNat parentIdx = sklb->bones[inBoneIdx].parentIdx;
        afxNat parentIdxWritten = AFX_INVALID_INDEX;

        if (parentIdx != AFX_INVALID_INDEX)
        {
            AfxAssertRange(skl->jointCnt, parentIdx, 1);

            _AfxSklCtorWriteBone(sklb, skl, boneWrittenMap, parentIdx, outBoneIdx);
            parentIdxWritten = boneWrittenMap[parentIdx];
        }

        if (skl->parentIdx)
            skl->parentIdx[*outBoneIdx] = parentIdxWritten;

        if (skl->local)
            AfxCopyTransform(&skl->local[*outBoneIdx], &sklb->bones[inBoneIdx].local);

        if (skl->iw)
            AfxCopyM4d(skl->iw[*outBoneIdx], sklb->bones[inBoneIdx].iw);

        if (skl->lodError)
            skl->lodError[*outBoneIdx] = sklb->bones[inBoneIdx].lodError;

        if (skl->joint)
            if (!AfxCatalogStrings2(skl->strc, 1, &sklb->bones[inBoneIdx].id.str.str, &skl->joint[*outBoneIdx]))
                AfxThrowError();

        if (skl->udd)
            skl->udd[*outBoneIdx] = sklb->bones[inBoneIdx].extendedData;

        //if (sklb->IndexRemapping)
            //sklb->IndexRemapping(data, inBoneIdx, *outBoneIdx);

        boneWrittenMap[inBoneIdx] = (*outBoneIdx)++;
    }
}

_AKX afxError _AfxSklCtor(afxSkeleton skl, afxCookie const *cookie)
{
    AfxEntry("skl=%p", skl);
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &skl, afxFcc_SKL);

    afxSimulation sim = cookie->udd[0];
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    afxStringCatalog strc = cookie->udd[1];
    afxSkeletonBlueprint const* bluep = cookie->udd[2];
    bluep += cookie->no;
    AfxAssert(bluep);

    if (!(skl->strc = strc))
        AfxResetString(&skl->id);
    else if (!AfxCatalogStrings2(strc, 1, &bluep->id.str.str, &skl->id))
        AfxThrowError();
    else
        AfxReacquireObjects(1, (void*[]) { strc });

    skl->allowedLodErrFadingFactor = sim->allowedLodErrFadingFactor;

    if (!err)
    {
        afxMmu mmu = AfxGetSimulationMmu(sim);

        afxNat jointCnt = bluep->jointCnt;
        afxNat lodType = bluep->lodType;

        skl->jointCnt = jointCnt;
        skl->lodType = lodType;

        if (!(skl->parentIdx = AfxAllocate(mmu, jointCnt, sizeof(skl->parentIdx[0]), 0, AfxHint()))) AfxThrowError();
        else
        {
            for (afxNat i = 0; i < jointCnt; i++)
                skl->parentIdx[i] = AFX_INVALID_INDEX; // to avoid crashes we must set it.

            if (!(skl->local = AfxAllocate(mmu, jointCnt, sizeof(skl->local[0]), AFX_SIMD_ALIGN, AfxHint()))) AfxThrowError();
            else
            {
                for (afxNat i = 0; i < jointCnt; i++)
                    AfxResetTransform(&skl->local[i]);

                if (!(skl->iw = AfxAllocate(mmu, jointCnt, sizeof(skl->iw[0]), AFX_SIMD_ALIGN, AfxHint()))) AfxThrowError();
                else
                {
                    for (afxNat i = 0; i < jointCnt; i++)
                        AfxResetM4d(skl->iw[i]);

                    if (!(skl->lodError = AfxAllocate(mmu, jointCnt, sizeof(skl->lodError[0]), AFX_SIMD_ALIGN, AfxHint()))) AfxThrowError();
                    else
                    {
                        for (afxNat i = 0; i < jointCnt; i++)
                            skl->lodError[i] = -1.0;

                        skl->joint = NIL;

                        if (strc && !(skl->joint = AfxAllocate(mmu, jointCnt, sizeof(skl->joint[0]), 0, AfxHint()))) AfxThrowError();
                        else
                        {
                            if (!bluep->joints)
                            {
                                for (afxNat i = 0; i < jointCnt; i++)
                                    AfxResetString(&skl->joint[i]); // to avoid crashes we must set it.
                            }
                            else
                            {
                                if (!AfxCatalogStrings2(strc, jointCnt, bluep->joints, skl->joint))
                                    AfxThrowError();
                            }

                            skl->udd = NIL;

                            if (err && skl->joint)
                                AfxDeallocate(mmu, skl->joint);
                        }

                        if (err && skl->lodError)
                            AfxDeallocate(mmu, skl->lodError);
                    }

                    if (err && skl->iw)
                        AfxDeallocate(mmu, skl->iw);
                }

                if (err && skl->local)
                    AfxDeallocate(mmu, skl->local);
            }

            if (err && skl->parentIdx)
                AfxDeallocate(mmu, skl->parentIdx);
        }
        
        
        AfxAssert(jointCnt);
        //skl->bones = NIL;

        //if (!(skl->bones = AfxAllocate(mmu, jointCnt, sizeof(skl->bones[0]), 0, AfxHint()))) AfxThrowError();
        //else
        {
#if 0
            afxNat *boneWrittenMap = AfxAllocate(mmu, jointCnt, sizeof(boneWrittenMap[0]), 0, AfxHint());
            AfxAssert(boneWrittenMap);

            for (afxNat i = 0; i < jointCnt; ++i)
                boneWrittenMap[i] = (afxNat)AFX_INVALID_INDEX;
#endif

            // estimate lod

            //for (afxNat i = 0; i < skl->jointCnt; i++)
            {
                // to avoid crashes we must set it.
                //skl->bones[i].idStrIdx = AFX_INVALID_INDEX;
                //skl->bones[i].parentIdx = AFX_INVALID_INDEX;
            }

#if 0
            afxNat outBoneIdx = 0;

            if (!skipBoneWrite)
            {
                for (afxNat i = 0; i < skl->jointCnt; i++)
                {
                    if (AFX_INVALID_INDEX == sklb->bones[i].parentIdx)
                        _AfxSklCtorWriteBone(sklb, skl, boneWrittenMap, i, &outBoneIdx);
                }

                for (afxNat i = 0; i < skl->jointCnt; i++)
                {
                    if (AFX_INVALID_INDEX != sklb->bones[i].parentIdx)
                        _AfxSklCtorWriteBone(sklb, skl, boneWrittenMap, i, &outBoneIdx);
                }
            }

            //_AfxSklCtorBubbleSortOnLOD(blueprint, skl->bones, remapTable);
#endif
            
            //skl->jointCnt = jointCnt;
            AfxAssert(skl->jointCnt == jointCnt);

#if 0
            AfxDeallocate(mmu, boneWrittenMap);
#endif
            
            //if (err && skl->bones)
                //AfxDeallocate(mmu, skl->bones);
        }
    }
    return err;
}

_AKX afxError _AfxSklDtor(afxSkeleton skl)
{
    AfxEntry("skl=%p", skl);
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &skl, afxFcc_SKL);

    afxSimulation sim = AfxGetObjectProvider(skl);
    afxMmu mmu = AfxGetSimulationMmu(sim);

    //for (afxNat i = 0; i < skl->jointCnt; i++)
        //AfxDeallocateString(&skl->bones[i].id);

    //if (skl->bones)
        //AfxDeallocate(mmu, skl->bones);

    if (skl->joint)
        AfxDeallocate(mmu, skl->joint);

    if (skl->lodError)
        AfxDeallocate(mmu, skl->lodError);

    if (skl->iw)
        AfxDeallocate(mmu, skl->iw);

    if (skl->local)
        AfxDeallocate(mmu, skl->local);

    if (skl->parentIdx)
        AfxDeallocate(mmu, skl->parentIdx);

    if (skl->strc)
        AfxReleaseObjects(1, (void*[]) { skl->strc });

    return err;
}

_AKX afxClassConfig _AfxSklClsConfig =
{
    .fcc = afxFcc_SKL,
    .name = "Skeleton",
    .unitsPerPage = 2,
    .size = sizeof(AFX_OBJECT(afxSkeleton)),
    .mmu = NIL,
    .ctor = (void*)_AfxSklCtor,
    .dtor = (void*)_AfxSklDtor
};

////////////////////////////////////////////////////////////////////////////////

_AKX afxError AfxAcquireSkeletons(afxSimulation sim, afxStringCatalog strc, afxNat cnt, afxSkeletonBlueprint const sklb[], afxSkeleton skeletons[])
{
    //AfxEntry("sim=%p,name=%.*s,jointCnt=%u,boneName=%p,boneParentIdx=%p,bonePos=%p,boneOrient=%p,boneScale=%p", sim, AfxPushString(name), jointCnt, boneName, boneParentIdx, bonePos, boneOrient, boneScale);
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    AfxAssert(skeletons);
    AfxAssert(sklb);
    AfxAssert(cnt);

    if (AfxAcquireObjects(AfxGetSkeletonClass(sim), cnt, (afxObject*)skeletons, (void const*[]) { sim, strc, (void*)sklb, FALSE }))
        AfxThrowError();

    return err;
}

_AKX afxError AfxBuildSkeletons(afxSimulation sim, afxStringCatalog strc, afxNat cnt, afxSkeletonBuilder const builders[], afxSkeleton skeletons[])
{
    //AfxEntry("sim=%p,name=%.*s,jointCnt=%u,boneName=%p,boneParentIdx=%p,bonePos=%p,boneOrient=%p,boneScale=%p", sim, AfxPushString(name), jointCnt, boneName, boneParentIdx, bonePos, boneOrient, boneScale);
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    AfxAssert(builders);
    AfxAssert(cnt);
    AfxAssert(skeletons);

    afxMmu mmu = AfxGetSimulationMmu(sim);

    for (afxNat i = 0; i < cnt; i++)
    {
        afxSkeletonBuilder const* sklb = &builders[i];
        afxSkeletonBlueprint bluep = { 0 };
        bluep.jointCnt = sklb->jointCnt;
        AfxMakeString32(&bluep.id, &sklb->id.str.str);
        bluep.lodType = sklb->lodType;

        if (AfxAcquireSkeletons(sim, strc, 1, &bluep, &skeletons[i]))
            AfxThrowError();

        afxSkeleton skl = skeletons[i];

        afxNat *boneWrittenMap = AfxAllocate(mmu, skl->jointCnt, sizeof(boneWrittenMap[0]), 0, AfxHint());
        AfxAssert(boneWrittenMap);

        for (afxNat i = 0; i < skl->jointCnt; ++i)
            boneWrittenMap[i] = (afxNat)AFX_INVALID_INDEX;

        afxNat outBoneIdx = 0;

        for (afxNat i = 0; i < skl->jointCnt; i++)
        {
            if (AFX_INVALID_INDEX == sklb->bones[i].parentIdx)
                _AfxSklCtorWriteBone(sklb, skl, boneWrittenMap, i, &outBoneIdx);
        }

        for (afxNat i = 0; i < skl->jointCnt; i++)
        {
            if (AFX_INVALID_INDEX != sklb->bones[i].parentIdx)
                _AfxSklCtorWriteBone(sklb, skl, boneWrittenMap, i, &outBoneIdx);
        }
        
        //_AfxSklCtorBubbleSortOnLOD(blueprint, skl->bones, remapTable);

        AfxDeallocate(mmu, boneWrittenMap);

        afxString s;

        if (!skl->strc)
            AfxResetString(&s);
        else
            AfxResolveStrings2(skl->strc, 1, &skl->id, &s);

        AfxEcho("Skeleton <%.*s> assembled. %p\n    Listing %u joints:", AfxPushString(&s), skl, skl->jointCnt);

        for (afxNat i = 0; i < skl->jointCnt; i++)
        {
            if (!skl->strc)
                AfxResetString(&s);
            else
                AfxResolveStrings2(skl->strc, 1, &skl->joint[i], &s);

            AfxLogMessageFormatted(0xFF, "\n    %u <%.*s> %i", i, AfxPushString(&s), skl->parentIdx[i]);
        }

    }

    return err;
}

_AKX void AfxTransformSkeletons(afxM3d const ltm, afxM3d const iltm, afxReal linearTol, afxV4d const atv, afxReal affineTol, afxNat cnt, afxSkeleton skeletons[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(ltm);
    AfxAssert(iltm);
    AfxAssert(atv);
    AfxAssert(cnt);
    AfxAssert(skeletons);
    (void)affineTol;
    (void)linearTol;

    for (afxNat i = 0; i < cnt; i++)
    {
        afxSkeleton skl = skeletons[i];
        AfxAssertObjects(1, &skl, afxFcc_SKL);
        afxNat jointCnt = skl->jointCnt;

        for (afxNat j = 0; j < jointCnt; j++)
        {
            AfxAssimilateTransforms(ltm, iltm, atv, 1, &skl->local[j], &skl->local[j]);
            AfxAssimilateAtm4d(ltm, iltm, atv, 1, &skl->iw[j], &skl->iw[j]);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

_AKXINL afxError AfxBeginSkeletonBuilding(afxSkeletonBuilder* sklb, afxNat jointCnt, afxString const* id, afxNat lodType)
{
    afxError err = NIL;
    AfxAssert(sklb);
    
    AfxAssignFcc(sklb, afxFcc_SKLB);
    AfxAssert(jointCnt);
    sklb->jointCnt = jointCnt;
    sklb->bones = AfxAllocate(NIL, jointCnt, sizeof(sklb->bones[0]), 0, AfxHint());

    for (afxNat i = 0; i < jointCnt; i++)
    {
        sklb->bones[i].extendedData = NIL;
        AfxResetTransform(&sklb->bones[i].local);
        sklb->bones[i].lodError = -1.0;
        sklb->bones[i].parentIdx = AFX_INVALID_INDEX;
        AfxResetM4d(sklb->bones[i].iw);
        AfxMakeString32(&sklb->bones[i].id, NIL);
    }

    AfxMakeString32(&sklb->id, id);
    sklb->lodType = lodType;
    return err;
}

_AKXINL void AfxEndSkeletonBuilding(afxSkeletonBuilder* sklb)
{
    afxError err = NIL;
    AfxAssertType(sklb, afxFcc_SKLB);

    if (sklb->bones)
    {
        AfxDeallocate(NIL, sklb->bones);
        sklb->bones = NIL;
    }
    AfxAssignFcc(sklb, NIL);
}

_AKXINL void AfxResetBoneWithIw(afxSkeletonBuilder const* sklb, afxNat jointIdx, afxString const* id, afxNat parentIdx, afxTransform const* lt, afxM4d const iw, afxReal lodError)
{
    afxError err = NIL;
    AfxAssertType(sklb, afxFcc_SKLB);
    AfxAssertRange(sklb->jointCnt, jointIdx, 1);
    sklb->bones[jointIdx].lodError = lodError;
    sklb->bones[jointIdx].parentIdx = parentIdx;
    AfxCopyTransform(&sklb->bones[jointIdx].local, lt);
    AfxCopyM4d(sklb->bones[jointIdx].iw, iw);
    AfxMakeString32(&sklb->bones[jointIdx].id, id);
}

_AKXINL void AfxResetBone(afxSkeletonBuilder const* sklb, afxNat jointIdx, afxString const* id, afxNat parentIdx, afxTransform const* lt, afxTransform const* wt, afxReal lodError)
{
    afxError err = NIL;
    AfxAssertType(sklb, afxFcc_SKLB);
    AfxAssertRange(sklb->jointCnt, jointIdx, 1);
    AfxAssert(id && !AfxStringIsEmpty(id));
    afxM4d w, iw;
    AfxComputeTransformM4d(wt, w);
    AfxInvertM4d(w, iw);
    AfxResetBoneWithIw(sklb, jointIdx, id, parentIdx, lt, iw, lodError);
}
