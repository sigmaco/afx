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

#ifndef ASX_IMPL___SIMULATION_H
#define ASX_IMPL___SIMULATION_H

#include "qwadro/inc/sim/afxSimulation.h"

#define ASX_MAX_SIM_BRIDGE_PER_CONTEXT 16

AFX_DEFINE_STRUCT(_asxSimulationAcquisition)
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
    afxClassConfig const* mshClsCfg;
    afxClassConfig const* mtlClsCfg;
    afxClassConfig const* poseClsCfg;
    afxClassConfig const* plceClsCfg;
    
    afxClassConfig const* terClsCfg;
    afxClassConfig const* litClsCfg;
    afxClassConfig const* rndClsCfg;

    afxClassConfig const* sexuClsCfg;
};

#ifdef _ASX_SIMULATION_C
#ifdef _ASX_SIMULATION_IMPL
AFX_OBJECT(_asxSimulation)
#else
AFX_OBJECT(afxSimulation)
#endif
{
    afxBool         running;

    afxUnit         bridgeCnt;
    afxSimBridge*   bridges;
    afxEngineFeatures requirements;

    afxChain        classes;
    afxClass        sexuCls;

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
    afxClass    scnCls;

    afxClass        sklCls;
    afxClass        poseCls;
    afxClass        plceCls;
    afxClass        curCls;

    afxClass        mtlCls;
    afxClass        mshCls;
    afxClass        mdlCls;

    afxStringBase   strbMdlSklMotUrns;
    afxStringBase   strbJointBiasesTags;
    afxStringBase   strbMorphTags;

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

    asxMotive       globalCapstanInterlinks;

    afxError(*waitCb)(afxSimulation,afxTime);

    afxDrawSystem  dsys;
    afxDrawInput    din;

    struct _afxSimIdd* idd;
    void* udd; // user-defined data
};
#endif//_ASX_SIMULATION_C

ASX afxReal _AsxGetAllowedLodErrorFadingFactor(afxSimulation sim);

ASX afxClass const* _AsxGetCurveClass(afxSimulation sim);
ASX afxClass const* _AsxGetPoseClass(afxSimulation sim);
ASX afxClass const* _AsxGetPlacementClass(afxSimulation sim);

ASX afxClass const* _AsxGetMaterialClass(afxSimulation sim);
ASX afxClass const* _AsxGetModelClass(afxSimulation sim);
ASX afxClass const* _AsxGetMeshClass(afxSimulation sim);

ASX afxClass*       _AsxGetCapstanClass(afxSimulation sim);
ASX afxClass*       _AsxGetBodyClass(afxSimulation sim);
ASX afxClass*       _AsxGetTerrainClass(afxSimulation sim);

ASX afxClass const* _AsxGetMotionClass(afxSimulation sim);
ASX afxClass*       _AsxGetMotiveClass(afxSimulation sim);
ASX afxClass const* _AsxGetAnimationClass(afxSimulation sim);
ASX afxClass const* _AsxGetInstancedAnimationClass(afxSimulation sim);

ASX afxClass*       _AsxGetLightClass(afxSimulation sim);
ASX afxClass*       _AsxGetNodeClass(afxSimulation sim);
ASX afxClass*       _AsxGetRendererClass(afxSimulation sim);

ASX afxClass const* _AsxGetSceneClass(afxSimulation sim);

ASX afxStringBase   _AsxGetModelUrnStringBase(afxSimulation sim);
ASX afxStringBase   _AsxGetPivotTagStringBase(afxSimulation sim);
ASX afxStringBase   _AsxGetMorphTagStringBase(afxSimulation sim);


ASX afxClassConfig const _ASX_SIM_CLASS_CONFIG;

#endif//ASX_IMPL___SIMULATION_H
