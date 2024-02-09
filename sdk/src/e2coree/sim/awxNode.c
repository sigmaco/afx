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
 *                       (c) 2017 SIGMA, Engineering In Technology
 *                             <https://sigmaco.org/qwadro/>
 */

#define _AFX_SIM_C
#define _AFX_SIMULATION_C
#define _AFX_NODE_C
#include "qwadro/sim/afxSimulation.h"
#include "qwadro/math/afxQuaternion.h"
#include "qwadro/math/afxMatrix.h"
#include "qwadro/math/afxVector.h"


_AFXINL afxError _AfxNodDtor(awxNode nod)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &nod, afxFcc_NOD);

    return err;
}

_AFXINL afxError _AfxNodCtor(awxNode nod, afxCookie const *cookie)
{
    afxError err = AFX_ERR_NONE;
    //AfxAssertObject(&nod->obj, afxFcc_NOD);
    //nod->obj.superset = afxFcc_NOD;
    //nod->root = paradigm->root;

    return err;
}

_AFXINL afxError AfxAcquireNodes(afxSimulation sim, afxNat cnt, awxNode nod[], afxNat const config[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);

    if (AfxAcquireObjects(&sim->nodes, cnt, (afxObject*)nod, (void const*[]) { (void*)config }))
        AfxThrowError();

    AfxAssertObjects(cnt, nod, afxFcc_NOD);

    return err;
}

_AFX afxClassConfig _AfxNodClsConfig =
{
    .fcc = afxFcc_NOD,
    .name = "Node",
    .unitsPerPage = 1,
    //.size = sizeof(AFX_OBJECT(awxNode)),
    .mmu = NIL,
    .ctor = (void*)_AfxNodCtor,
    .dtor = (void*)_AfxNodDtor
};
