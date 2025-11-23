/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                      Q W A D R O   D R A W   I / O   S Y S T E M
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

  //////////////////////////////////////////////////////////////////////////////
 // The Qwadro Video Graphics Infrastructure                                 //
//////////////////////////////////////////////////////////////////////////////

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>
// This software is part of Advanced Video Graphics Extensions & Experiments.

#ifndef AVX_DRAW_BRIDGE_IMPL_H
#define AVX_DRAW_BRIDGE_IMPL_H

#include "avxQueueImpl.h"

AFX_DEFINE_STRUCT(_avxDexuAcquisition)
{
    afxDrawDevice           ddev;
    //afxUnit                 portId;
    afxUnit                 exuIdx;
    afxUnit                 minQueCnt;
    afxClassConfig const*   dqueClsCfg;
    afxClassConfig const*   dctxClsCfg;
};

AFX_DECLARE_STRUCT(avxDpu);

AFX_DECLARE_STRUCT(_avxIddDexu);
AFX_DEFINE_STRUCT(_avxDdiDexu)
{
    afxError(*pingCb)(afxDrawBridge, afxUnit);
    afxError(*waitCb)(afxDrawBridge, afxUnit64);

};

#ifdef _AVX_DRAW_BRIDGE_C
#ifdef _AVX_DRAW_BRIDGE_IMPL
AFX_OBJECT(_avxDrawBridge)
#else
AFX_OBJECT(afxDrawBridge)
#endif
{
    _avxIddDexu*        idd;
    _avxDdiDexu const*  ddi;
    // User-defined data.
    void*               udd;
    // Debugging tag.
    afxString           tag;

    afxDrawDevice       ddev;
    //afxUnit         portId;
    afxUnit             exuIdx;    
    afxChain            classes;
    afxClass            dqueCls;
    // one command pool per bridge to avoid thread interation.
    afxClass            dctxCls;

    // DPU
    afxCondition        schedCnd; // signaled when some task is scheduled to this bridge.
    afxMutex            schedCndMtx;
    afxBool             schedCnt;
    afxThread           worker;
    afxInt              (*workerProc)(afxDrawBridge);
    _avxIoReqLut const* iorpVmt;
    _avxCmdLut const*   cmdVmt;
    afxBool             (*procCb)(avxDpu*);
    avxDpu*             dpu;
};
#endif//_AVX_DRAW_BRIDGE_C

AVX afxClassConfig const _AVX_DEXU_CLASS_CONFIG;

//AVX afxUnit _AvxCountDrawQueues(afxDrawBridge dexu, afxUnit baseQueIdx);
AVX afxClass const* _AvxDexuGetDctxClass(afxDrawBridge dexu);
AVX afxClass const* _AvxDexuGetDqueClass(afxDrawBridge dexu);

AVX afxError _AvxDexuRemapBuffers(afxDrawBridge dexu, afxBool unmap, afxUnit cnt, _avxBufferRemapping const maps[]);
AVX afxError _AvxDexuCohereMappedBuffers(afxDrawBridge dexu, afxBool discard, afxUnit cnt, avxBufferedMap const maps[]);
AVX afxError _AvxDexuTransferVideoMemory(afxDrawBridge dexu, avxTransference* ctrl, afxUnit opCnt, void const* ops);
AVX afxError _AvxDexuPresentBuffers(afxDrawBridge dexu, afxUnit cnt, avxPresentation presentations[]);

AVX afxError _AvxDexu_PingCb(afxDrawBridge dexu, afxUnit queIdx);

AVX afxError        _AvxAcquireDrawQueues
(
    afxDrawSystem   dsys,
    afxUnit         cnt,
    afxMask const   bridges[],
    afxDrawQueue    queues[]
);

AVX afxError _AvxDsysBridgeDevices(afxDrawSystem dsys, afxUnit cnt, _avxDexuAcquisition const configs[], afxDrawBridge bridges[]);

#endif//AVX_DRAW_BRIDGE_IMPL_H
