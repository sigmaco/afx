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
 *                   (c) 2017 SIGMA Technology Group — Federação SIGMA
 *                                    www.sigmaco.org
 */

#define _AFX_SIM_C
#define _AFX_ENTITY_C
#include "qwadro/sim/afxSimulation.h"

_AFX afxError _AfxEntDtor(awxEntity ent)
{
    AfxEntry("ent=%p", ent);
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ent, afxFcc_ENT);

    return err;
}

_AFX afxError _AfxEntCtor(awxEntity ent, afxCookie const *cookie)
{
    AfxEntry("ent=%p", ent);
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ent, afxFcc_ENT);

    awxBody bod = *(awxBody*)cookie->udd[1];
    (void)bod;

    return err;
}

////////////////////////////////////////////////////////////////////////////////
// MASSIVE OPERATIONS                                                         //
////////////////////////////////////////////////////////////////////////////////

_AFX afxError AfxAcquireEntities(afxSimulation sim, afxNat cnt, awxEntity ent[], awxBody bod[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);

    for (afxNat i = 0; i < cnt; i++)
        if (AfxAcquireObjects(AfxGetEntityClass(sim), 1, (afxObject*)&ent[i], (void const*[]) { NIL, &bod[i]}))
            AfxThrowError();

    return err;
}

_AFX afxClassConfig _AfxEntClsConfig =
{
    .fcc = afxFcc_ENT,
    .name = "Entity",
    .unitsPerPage = 1,
    .size = sizeof(AFX_OBJECT(awxEntity)),
    .mmu = NIL,
    .ctor = (void*)_AfxEntCtor,
    .dtor = (void*)_AfxEntDtor
};
