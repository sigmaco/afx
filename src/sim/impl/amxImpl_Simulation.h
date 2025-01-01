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

#ifndef AMX_IMPL___SIMULATION_H
#define AMX_IMPL___SIMULATION_H

#include "qwadro/inc/sim/afxSimulation.h"

#define AMX_MAX_SIM_BRIDGE_PER_CONTEXT 16

AFX_DEFINE_STRUCT(_amxSimulationAcquisition)
{
    afxUnit             bridgeCnt;
    afxEngineFeatures   requirements;
    afxUnit             extensionCnt;
    afxString const*    extensions;
    void*               udd;

    afxDrawSystem       dsys;
    afxDrawInput        din;

    afxBox              extent;

    afxV3d              right;
    afxV3d              up;
    afxV3d              back;
    afxV3d              origin;
    afxReal             unitsPerMeter;
    afxReal             allowedLodErrFadingFactor;

    afxClassConfig const* curClsCfg;
    afxClassConfig const* motClsCfg;
    afxClassConfig const* capsClsCfg;
    afxClassConfig const* aniClsCfg;
    afxClassConfig const* anikClsCfg;
    afxClassConfig const* bodClsCfg;
    afxClassConfig const* motvClsCfg;
    afxClassConfig const* mdlClsCfg;
    afxClassConfig const* mtlClsCfg;
    afxClassConfig const* poseClsCfg;
    afxClassConfig const* plceClsCfg;
    
    afxClassConfig const* terClsCfg;
    afxClassConfig const* litClsCfg;
    afxClassConfig const* rndClsCfg;

    afxClassConfig const* mexuClsCfg;
};

#ifdef _AMX_SIMULATION_C
#ifdef _AMX_SIMULATION_IMPL
AFX_OBJECT(_amxSimulation)
#else
AFX_OBJECT(afxSimulation)
#endif
{
    afxBool         running;

    afxUnit         bridgeCnt;
    afxSimBridge*   bridges;
    afxEngineFeatures requirements;

    afxChain        classes;
    afxClass        mexuCls;

    afxClass      bodCls;
    afxClass      motoCls;
    afxClass      lights;

    afxClass      motCls;
    afxClass      motvCls;
    afxClass      aniCls;
    afxClass      anikCls;
    afxClass      nodCls;
    afxClass      renderers;
    afxClass      terCls;

    afxClass        sklCls;
    afxClass        poseCls;
    afxClass        plceCls;
    afxClass        curCls;

    afxClass        mtlCls;
    afxClass        mdlCls;

    afxStringBase   strbMdlSklMotUrns;

    afxV4d          right;
    afxV4d          up;
    afxV4d          back;
    afxM3d          basis;
    afxV4d          origin;
    afxBox         extent;
    afxReal         unitsPerMeter;
    afxReal         allowedLodErrFadingFactor;

    afxSize         stepNum, stepsPerSecond;
    afxSize         stepTime, swapTime;
    afxReal         stepDeltaTime;

    amxMotive       globalCapstanInterlinks;

    afxError(*waitCb)(afxSimulation,afxTime);

    afxDrawSystem  dsys;
    afxDrawInput    din;

    struct _afxSimIdd* idd;
    void* udd; // user-defined data
};
#endif//_AMX_SIMULATION_C

AMX afxReal _AmxGetAllowedLodErrorFadingFactor(afxSimulation sim);

AMX afxClass const* _AmxGetCurveClass(afxSimulation sim);
AMX afxClass const* _AmxGetPoseClass(afxSimulation sim);
AMX afxClass const* _AmxGetPlacementClass(afxSimulation sim);

AMX afxClass const* _AmxGetMaterialClass(afxSimulation sim);
AMX afxClass const* _AmxGetModelClass(afxSimulation sim);

AMX afxClass*       _AmxGetCapstanClass(afxSimulation sim);
AMX afxClass*       _AmxGetBodyClass(afxSimulation sim);
AMX afxClass*       _AmxGetTerrainClass(afxSimulation sim);

AMX afxClass const* _AmxGetMotionClass(afxSimulation sim);
AMX afxClass*       _AmxGetMotiveClass(afxSimulation sim);
AMX afxClass const* _AmxGetAnimationClass(afxSimulation sim);
AMX afxClass const* _AmxGetInstancedAnimationClass(afxSimulation sim);

AMX afxClass*       _AmxGetLightClass(afxSimulation sim);
AMX afxClass*       _AmxGetNodeClass(afxSimulation sim);
AMX afxClass*       _AmxGetRendererClass(afxSimulation sim);

AMX afxStringBase   AfxGetModelUrnStringBase(afxSimulation sim);

AMX afxClassConfig const _AMX_SIM_CLASS_CONFIG;

#endif//AMX_IMPL___SIMULATION_H
