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

#define _ASX_SIM_C
//#define _ASX_SIMULATION_C
#define _ASX_NODE_C
#include "impl/asxImplementation.h"


_ASXINL afxError _AfxNodDtor(afxNode nod)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_NOD, 1, &nod);

    return err;
}

_ASXINL afxError _AfxNodCtor(afxNode nod, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;
    //AfxAssertObject(&nod->obj, afxFcc_NOD);
    //nod->obj.superset = afxFcc_NOD;
    //nod->root = paradigm->root;

    return err;
}

_ASXINL afxError AfxAcquireNodes(afxSimulation sim, afxUnit cnt, afxNode nod[], afxUnit const config[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);

    if (AfxAcquireObjects((afxClass *)_AsxGetNodeClass(sim), cnt, (afxObject*)nod, (void const*[]) { (void*)config }))
        AfxThrowError();

    AFX_ASSERT_OBJECTS(afxFcc_NOD, cnt, nod);

    return err;
}

_ASX afxClassConfig const _ASX_NOD_CLASS_CONFIG =
{
    .fcc = afxFcc_NOD,
    .name = "Node",
    //.fixedSiz = sizeof(AFX_OBJECT(afxNode)),
    .ctor = (void*)_AfxNodCtor,
    .dtor = (void*)_AfxNodDtor
};
