/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *            Q W A D R O   M U L T I M E D I A   I N F R A S T R U C T U R E
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This software is part of Advanced Multimedia Extensions & Experiments.

#ifndef AMX_BRIDGE_DDK_H
#define AMX_BRIDGE_DDK_H

#include "amxQueueDDK.h"

AFX_DEFINE_STRUCT(_amxMexuAcq)
{
    afxMixDevice            mdev;
    //afxUnit                 portId;
    afxUnit                 exuIdx;
    afxUnit                 minQueCnt;
    afxClassConfig const*   mqueClsCfg;
    afxClassConfig const*   mixClsCfg;
};

AFX_DECLARE_STRUCT(amxMpu);

#ifdef _AMX_MIX_BRIDGE_C
#ifdef _AMX_MIX_BRIDGE_IMPL
AFX_OBJECT(_amxMixBridge)
#else
AFX_OBJECT(afxMixBridge)
#endif
{
    afxMixDevice  mdev;
    //afxUnit         portId;
    afxUnit         exuIdx;
    
    afxChain        classes;
    afxClass        mqueCls;

    afxError        (*pingCb)(afxMixBridge, afxUnit);

    // SPU
    afxCondition    schedCnd; // signaled when some task is scheduled to this bridge.
    afxMutex        schedCndMtx;
    afxBool         schedCnt;
    afxThread       worker;
    afxInt          (*workerProc)(afxMixBridge);
    _amxIoReqLut const*iorpVmt;
    afxBool         (*procCb)(amxMpu*);
    amxMpu*         mpu;
};
#endif//_AMX_MIX_BRIDGE_C

AMX afxClassConfig const _AMX_MEXU_CLASS_CONFIG;

AMX afxClass const* _AmxMexuGetMqueClass(afxMixBridge mexu);

AMX afxError _AmxMsysBridgeDevices(afxMixSystem msys, afxUnit cnt, _amxMexuAcq const configs[], afxMixBridge bridges[]);

AMX afxError _AmxMexuRemapBuffers(afxMixBridge mexu, afxBool unmap, afxUnit cnt, _amxBufferRemapping const maps[]);
AMX afxError _AmxMexuCohereMappedBuffers(afxMixBridge mexu, afxBool discard, afxUnit cnt, amxBufferedMap const maps[]);
AMX afxError _AmxMexuTransferMixMemory(afxMixBridge mexu, amxTransference* ctrl, afxUnit opCnt, void const* ops);
AMX afxError _AmxMexuSinkBuffers(afxMixBridge mexu, afxUnit cnt, amxFlush presentations[]);

AMX afxError _AmxMexu_PingCb(afxMixBridge mexu, afxUnit queIdx);

//AMX afxError _AmxMexuSubmitCallback(afxMixBridge mexu, afxError(*f)(void*, void*), void* udd);
AMX afxError _AmxMexuExecuteMixCommands(afxMixBridge mexu, afxUnit frameCnt, afxUnit cnt, afxMixContext mixers[]);

#endif//AMX_BRIDGE_DDK_H
