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

// This section is part of SIGMA Simulation Infrastructure.

#ifndef ASX_IMPL___CONTEXT_H
#define ASX_IMPL___CONTEXT_H

#include "qwadro/inc/sim/afxSimulation.h"

// A catalyst is an event or person causing a change.

typedef enum asxContextState
/// Each command buffer is always in one of the following states
{
    asxContextState_INITIAL,
    asxContextState_RECORDING,
    asxContextState_EXECUTABLE,
    asxContextState_PENDING,
    asxContextState_INVALID,

    asxContextState_INTERNAL_EXECUTING,
} asxContextState;

#ifdef _ASX_CONTEXT_C
#ifdef _ASX_CONTEXT_IMPL
AFX_OBJECT(_asxContext)
#else
AFX_OBJECT(afxContext)
#endif
{
    afxArena        cmdArena; // owned by dsys data for specific port
    afxChain        commands;
    asxContextState state;
    afxError        (*endCb)(afxContext);
    afxError        (*resetCb)(afxContext, afxBool freeMem, afxBool permanent);
    afxAtom32       submCnt; // number of submissions
    afxMask64       submQueMask; // one for each queue where this dctx was submitted into.

    afxUnit         portId;
    afxUnit         poolIdx;
    afxBool         disposable; // if true, at execution end, it is moved to invalid state and considered in recycle chain.

    struct
    {
        afxPose pose;

        afxModel skl;
        afxReal allowedErr;
        afxReal allowedErrScaler;
        afxReal allowedErrEnd;

        afxM4d displacement;
        afxUnit baseJntIdx;
        afxUnit jntCnt;
        afxUnit baseReqJntIdx;
        afxUnit reqJntCnt;
        afxUnit const* sparseJntMap;
    } pose;

    afxPlacement plce;
    afxUnit baseXformIdx;
    afxUnit xformCnt;

};
#endif//_ASX_CONTEXT

AFX_DEFINE_STRUCT(asxCmdHdr)
{
    afxLink script;
    afxUnit id;
    afxUnit siz;
};

AFX_DEFINE_UNION(asxCmd)
{
    asxCmdHdr hdr;
    struct
    {
        asxCmdHdr hdr;

        afxPose src;
        afxUnit from;
        afxPose dst;
        afxUnit base;
        afxUnit cnt;
    } CopyPose;
    struct
    {
        asxCmdHdr hdr;

        afxPose pose;
        afxV3d translation;
        afxV3d rotation;
    } ApplyRootMotionVectors;
    struct
    {
        asxCmdHdr hdr;

        afxUnit baseSlot;
        afxUnit cnt;
        struct
        {
            avxBuffer buf;
            afxUnit offset;
            afxUnit range;
        } /*AFX_SIMD*/ items[];
    } BindBuffers;
    struct
    {
        asxCmdHdr hdr;

        afxUnit     baseIdx;
        afxUnit     cnt;
        struct
        {
            afxCurve cur;
            afxUnit a;
            afxUnit b;
        }       items[];
    } BindCurves;
    struct
    {
        asxCmdHdr hdr;

        afxUnit slot;
        void* ptr;
        afxUnit siz;
    } BindMemory;
    struct
    {
        asxCmdHdr hdr;

        afxBody bod;
        afxUnit pivotCnt;
        afxM4d displace;
        afxPose scratch;
        afxPlacement plce;
        afxReal allowedErr;
    } SampleBodyAnimationsAccelerated;
    struct
    {
        asxCmdHdr hdr;

        afxBody bod;
        afxCapstan moto;
        afxUnit basePivotIdx;
        afxUnit pivotCnt;
        afxPose pose;
        afxReal allowedErr;
        afxUnit const* sparseBoneArray;
    } SampleSingleBodyAnimation;
    struct
    {
        asxCmdHdr hdr;

        afxBody bod;
        afxUnit basePivotIdx;
        afxUnit pivotCnt;
        afxPose pose;
        afxReal allowedErr;
        afxUnit const* sparseBoneArray;
    } SampleBodyAnimations;
    struct
    {
        asxCmdHdr hdr;

        afxModel mdl;
        afxTransform displacement;
    } DisplaceModel;
    struct
    {
        asxCmdHdr hdr;

        afxModel mdl;
        afxUnit rigIdx;
        afxPlacement plce;
        afxUnit cnt;
        afxUnit bufIdx;
        afxSize bufOffset;
    } BuildRiggedMeshCompositeMatrices;
    struct
    {
        asxCmdHdr hdr;

        afxModel mdl;
        afxUnit baseJntIdx;
        afxUnit jntCnt;
        afxM4d displace;
        afxPlacement plce;
        afxBool rigid;
        afxPose pose;
    } RebuildPose;
    struct
    {
        asxCmdHdr hdr;

        afxModel mdl;
        afxUnit baseJntIdx;
        afxUnit jntCnt;
        afxUnit idxSiz;
        afxByte idxData[];
    } ReparentJoints;
    struct
    {
        asxCmdHdr hdr;

        afxModel mdl;
        afxUnit baseJntIdx;
        afxUnit jntCnt;
        afxTransform transforms[];
    } ResetJointTransforms;
    struct
    {
        asxCmdHdr hdr;

        afxModel mdl;
        afxUnit baseJntIdx;
        afxUnit jntCnt;
        afxUnit mtxSiz;
        afxByte mtxData[];
    } ResetJointInversors;
    struct
    {
        asxCmdHdr hdr;

        afxModel mdl;
        afxUnit baseJntIdx;
        afxUnit jntCnt;
        afxReal lodErrors[];
    } ResetJointLodErrors;
    struct
    {
        asxCmdHdr hdr;

        afxModel mdl;
        afxUnit baseJntIdx;
        afxUnit jntCnt;
        afxPose pose;
    } BuildRestPose;
    struct
    {
        asxCmdHdr hdr;

        afxModel mdl;
        afxUnit baseJntIdx;
        afxUnit jntCnt;
        afxUnit baseReqJnt;
        afxUnit reqJntCnt;
        afxPose pose;
        afxM4d displace;
        afxBool noComposite;
        afxPlacement plce;
    } BuildPlacement;
    struct
    {
        asxCmdHdr hdr;

        afxUnit bufIdx;
        afxSize offset;
        afxModel mdl;
        afxPlacement plce;
        afxUnit baseJnt;
        afxUnit jntCnt;
        afxBool /*3x4*/transposed;
    } BuildCompositeMatrices;
    struct
    {
        asxCmdHdr hdr;

        afxModel mdl;
        afxPlacement plce;
        afxUnit jntCnt;
        afxBool /*3x4*/transposed;
        afxUnit bufIdx;
        afxSize offset;
        afxUnit jntMap[];
    } BuildIndexedCompositeMatrices;
};

AFX_DEFINE_UNION(asxCmdList)
{
    struct
    {
        void* End; // end of context

        void* CopyPose;
        void* ApplyRootMotionVectors;
        void* BindMemory;
        void* BindBuffers;
        void* BindCurves;
        void* SampleBodyAnimationsAccelerated;
        void* SampleSingleBodyAnimation;
        void* SampleBodyAnimations;
        void* DisplaceModel;
        void* BuildRiggedMeshCompositeMatrices;
        void* RebuildPose;
        void* ReparentJoints;
        void* ResetJointTransforms;
        void* ResetJointInversors;
        void* ResetJointLodErrors;
        void* BuildRestPose;
        void* BuildPlacement;
        void* BuildCompositeMatrices;
        void* BuildIndexedCompositeMatrices;
    };
    void(*f[])(void*, asxCmd const*);
};

#define ASX_GET_STD_CMD_ID(cmdName_) (offsetof(asxCmdList, cmdName_) / sizeof(void*))

ASX asxCmd* _AsxCtxPushCmd(afxContext, afxUnit id, afxUnit siz, afxCmdId* cmdId);
ASX afxError _AsxCtxResetCb(afxContext, afxBool freeMem, afxBool permanent);
ASX afxError _AsxCtxEndCb(afxContext);

ASX asxContextState _AsxGetCatalystStatus(afxContext ctx);

ASX afxClassConfig const _ASX_CTX_CLASS_CONFIG;

#endif//ASX_IMPL___CONTEXT_H
