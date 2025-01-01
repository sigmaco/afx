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

#ifndef AMX_IMPL___CONTEXT_H
#define AMX_IMPL___CONTEXT_H

#include "qwadro/inc/sim/afxSimulation.h"

// A catalyst is an event or person causing a change.

typedef enum amxCatalystState
/// Each command buffer is always in one of the following states
{
    amxCatalystState_INITIAL,
    amxCatalystState_RECORDING,
    amxCatalystState_EXECUTABLE,
    amxCatalystState_PENDING,
    amxCatalystState_INVALID,

    amxCatalystState_INTERNAL_EXECUTING,
} amxCatalystState;

#ifdef _AMX_CATALYST_C
#ifdef _AMX_CATALYST_IMPL
AFX_OBJECT(_amxCatalyst)
#else
AFX_OBJECT(afxCatalyst)
#endif
{
    afxArena        cmdArena; /// owned by dsys data for specific port
    afxChain        commands;
    amxCatalystState state;
    afxError        (*endCb)(afxCatalyst);
    afxError        (*resetCb)(afxCatalyst, afxBool freeMem, afxBool permanent);
    afxAtom32       submCnt; /// number of submissions
    afxMask64       submQueMask; /// one for each queue where this dctx was submitted into.

    afxUnit         portId;
    afxUnit         poolIdx;
    afxBool         disposable; /// if true, at execution end, it is moved to invalid state and considered in recycle chain.

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
#endif//_AMX_CATALYST

AFX_DEFINE_STRUCT(amxCmdHdr)
{
    afxLink script;
    afxUnit id;
    afxUnit siz;
};

AFX_DEFINE_UNION(amxCmd)
{
    amxCmdHdr hdr;

    struct
    {
        amxCmdHdr hdr;

        afxPose src;
        afxUnit from;
        afxPose dst;
        afxUnit base;
        afxUnit cnt;
    } CopyPose;
    struct
    {
        amxCmdHdr hdr;

        afxPose pose;
        afxV3d translation;
        afxV3d rotation;
    } ApplyRootMotionVectors;
    struct
    {
        amxCmdHdr hdr;

        afxUnit baseSlot;
        afxUnit cnt;
        struct
        {
            afxBuffer buf;
            afxUnit offset;
            afxUnit range;
        } items[];
    } BindBuffers;
    struct
    {
        amxCmdHdr   hdr;

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
        amxCmdHdr hdr;

        afxUnit slot;
        void* ptr;
        afxUnit siz;
    } BindMemory;
    struct
    {
        amxCmdHdr hdr;

        afxBody bod;
        afxUnit pivotCnt;
        afxM4d offset;
        afxPose scratch;
        afxPlacement plce;
        afxReal allowedErr;
    } SampleBodyAnimationsAccelerated;
    struct
    {
        amxCmdHdr hdr;

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
        amxCmdHdr hdr;

        afxBody bod;
        afxUnit basePivotIdx;
        afxUnit pivotCnt;
        afxPose pose;
        afxReal allowedErr;
        afxUnit const* sparseBoneArray;
    } SampleBodyAnimations;
    struct
    {
        amxCmdHdr hdr;

        afxModel mdl;
        afxUnit rigIdx;
        afxPlacement plce;
        afxUnit cnt;
        afxUnit bufIdx;
        afxUnit bufOffset;
    } BuildIndexedCompositeBuffer;
};

AFX_DEFINE_UNION(amxCmdList)
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
        void* BuildIndexedCompositeBuffer;
    };
    void(*f[])(void*, amxCmd const*);
};

#define AMX_GET_STD_CMD_ID(cmdName_) (offsetof(amxCmdList, cmdName_) / sizeof(void*))

AMX amxCmd* _AmxCystPushCmd(afxCatalyst, afxUnit id, afxUnit siz, afxCmdId* cmdId);
AMX afxError _AmxCystResetCb(afxCatalyst, afxBool freeMem, afxBool permanent);
AMX afxError _AmxCystEndCb(afxCatalyst);

AMX amxCatalystState _AmxGetCatalystStatus(afxCatalyst cyst);

AMX afxClassConfig const _AMX_CYST_CLASS_CONFIG;

#endif//AMX_IMPL___CONTEXT_H
