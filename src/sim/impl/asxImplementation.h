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

#ifndef ASX_IMPLEMENTATION_H
#define ASX_IMPLEMENTATION_H

#include "../../impl/afxExecImplKit.h"
#include "../../draw/impl/avxImplementation.h"
#include "qwadro/inc/sim/afxEngine.h"
#include "qwadro/inc/sim/afxSimulation.h"
#include "asxImpl_Executor.h"
#include "asxImpl_Animation.h"
#include "asxImpl_Simulation.h"
#include "asxImpl_Body.h"
#include "asxImpl_Curve.h"
#include "asxImpl_Model.h"
#include "asxImpl_Context.h"

AFX_DEFINE_STRUCT(afxEngineInfo)
{
    afxDeviceInfo           dev;
    afxUnit                 portCnt;
    afxSimPortCaps const*portCaps;
    afxEngineFeatures       features;
    afxEngineLimits         limits;

    afxError(*stopCb)(afxEngine);
    afxError(*startCb)(afxEngine);
    afxError(*openSimCb)(afxEngine, afxSimulation, void** udd, afxUnit invokeNo); // unused if a custom dsys ctor is installed.
    afxError(*closeSimCb)(afxEngine, afxSimulation); // unused if a custom dsys ctor is installed.

    afxError(*closeMdgeCb)(afxEngine, afxSimBridge);
    afxError(*openMdgeCb)(afxEngine, afxSimBridge, afxSimBridgeConfig const*);
};

#ifdef _ASX_ENGINE_C
#ifdef _ASX_ENGINE_IMPL
AFX_OBJECT(_asxEngine)
#else
AFX_OBJECT(afxEngine)
#endif
{
    AFX_OBJECT(afxDevice) dev;

    afxEngineFeatures   features;
    afxEngineLimits     limits;
    afxUnit             portCnt;
    struct
    {
        afxSimPortCaps  caps;
    }*                  ports;

    afxBool             relinking;
    afxCondition        relinkedCnd;
    afxMutex            relinkedCndMtx;

    afxError(*stopCb)(afxEngine);
    afxError(*startCb)(afxEngine);
    afxError(*openSimCb)(afxEngine, afxSimulation, void** udd, afxUnit invokeNo); // unused if a custom dsys ctor is installed.
    afxError(*closeSimCb)(afxEngine, afxSimulation); // unused if a custom dsys ctor is installed.

    afxError(*closeMdgeCb)(afxEngine, afxSimBridge);
    afxError(*openMdgeCb)(afxEngine, afxSimBridge, afxSimBridgeConfig const*);

    struct _afxSdevIdd* idd;
};
#endif//_ASX_ENGINE_C

ASX afxError _AsxRegisterEngines(afxModule icd, afxUnit cnt, afxEngineInfo const infos[], afxEngine devices[]);
ASX afxError _AsxImplementSimulation(afxModule icd, afxClassConfig const* sdevCls, afxClassConfig const* simCls);

ASX afxClassConfig const _ASX_SDEV_CLASS_CONFIG;

ASX afxClassConfig const _ASX_NOD_CLASS_CONFIG;

ASX afxClassConfig const _ASX_LIT_CLASS_CONFIG;
ASX afxClassConfig const _ASX_RND_CLASS_CONFIG;
ASX afxClassConfig const _ASX_TER_CLASS_CONFIG;



ASX afxClass const* _AsxGetEngineClass(afxModule icd);

ASX afxClass const* _AsxGetSimulationClass(afxModule icd);

ASX afxBool _AsxGetIcd(afxUnit icdIdx, afxModule* driver);

#endif//ASX_IMPLEMENTATION_H
