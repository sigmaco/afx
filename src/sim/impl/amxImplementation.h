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

#ifndef AMX_IMPLEMENTATION_H
#define AMX_IMPLEMENTATION_H

#include "../../dev/afxExecImplKit.h"
#include "../../draw/impl/avxImplementation.h"
#include "qwadro/inc/sim/afxEngine.h"
#include "qwadro/inc/sim/afxSimulation.h"
#include "amxImpl_Executor.h"
#include "amxImpl_Animation.h"
#include "amxImpl_Simulation.h"
#include "amxImpl_Body.h"
#include "amxImpl_Curve.h"
#include "amxImpl_Model.h"
#include "amxImpl_Catalyst.h"

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

#ifdef _AMX_ENGINE_C
#ifdef _AMX_ENGINE_IMPL
AFX_OBJECT(_amxEngine)
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
#endif//_AMX_ENGINE_C

AMX afxError _AmxRegisterEngines(afxModule icd, afxUnit cnt, afxEngineInfo const infos[], afxEngine devices[]);
AMX afxError _AmxImplementSimulation(afxModule icd, afxClassConfig const* mdevCls, afxClassConfig const* simCls);

AMX afxClassConfig const _AMX_MDEV_CLASS_CONFIG;

AMX afxClassConfig const _AMX_NOD_CLASS_CONFIG;

AMX afxClassConfig const _AMX_LIT_CLASS_CONFIG;
AMX afxClassConfig const _AMX_RND_CLASS_CONFIG;
AMX afxClassConfig const _AMX_TER_CLASS_CONFIG;



AMX afxClass const* _AmxGetEngineClass(afxModule icd);

AMX afxClass const* _AmxGetSimulationClass(afxModule icd);

AMX afxBool AmxGetIcd(afxUnit icdIdx, afxModule* driver);

#endif//AMX_IMPLEMENTATION_H
