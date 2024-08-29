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

#define _AMX_SIM_C
#define _AMX_SIMULATION_C
#define _AMX_NODE_C
#include "../dev/AmxImplKit.h"


_AMXINL afxError _AfxNodDtor(akxNode nod)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &nod, afxFcc_NOD);

    return err;
}

_AMXINL afxError _AfxNodCtor(akxNode nod, void** args, afxNat invokeNo)
{
    afxError err = AFX_ERR_NONE;
    //AfxAssertObject(&nod->obj, afxFcc_NOD);
    //nod->obj.superset = afxFcc_NOD;
    //nod->root = paradigm->root;

    return err;
}

_AMXINL afxError AfxAcquireNodes(afxSimulation sim, afxNat cnt, akxNode nod[], afxNat const config[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);

    if (AfxAcquireObjects(&sim->nodes, cnt, (afxObject*)nod, (void const*[]) { (void*)config }))
        AfxThrowError();

    AfxAssertObjects(cnt, nod, afxFcc_NOD);

    return err;
}

_AMX afxClassConfig _AfxNodMgrCfg =
{
    .fcc = afxFcc_NOD,
    .name = "Node",
    //.fixedSiz = sizeof(AFX_OBJECT(akxNode)),
    .ctor = (void*)_AfxNodCtor,
    .dtor = (void*)_AfxNodDtor
};
