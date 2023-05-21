/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                      S I G M A   T E C H N O L O G Y   G R O U P
 *
 *                                   Public Test Build
 *                      (c) 2017 SIGMA Co. & SIGMA Technology Group
 *                                    www.sigmaco.org
 */

#include "sgl.h"

#include "afx/draw/pipelining/afxPipelineRig.h"
#include "afx/draw/afxDrawSystem.h"

typedef struct
{
    afxDrawContext dctx;
    afxNat legtCnt;
    afxLegoTemplate const *legt;
} _afxPiprParadigm;

_SGL afxResult _AfxPipelineRigEnumerateTemplates(afxPipelineRig pipr, afxNat first, afxNat cnt, afxLegoTemplate legt[])
{
    afxError err = NIL;
    AfxAssertObject(pipr, AFX_FCC_PIPR);
    AfxAssert(legt);
    AfxAssert(cnt);
    afxResult rslt = 0;
    
    for (afxNat i = 0; i < cnt; i++)
    {
        if (i >= cnt) break;
        else
        {
            legt[i] = pipr->legt[first + i];
            ++rslt;
        }
    }
    return rslt;
}

 
_SGL afxPiprImpl const _AfxStdPiprImpl;
afxPiprImpl const _AfxStdPiprImpl =
{
    _AfxPipelineRigEnumerateTemplates
};

_SGL afxBool _SglPiprEventHandler(afxObject *obj, afxEvent *ev)
{
    afxError err = NIL;
    afxPipelineRig pipr = (void*)obj;
    AfxAssertObject(pipr, AFX_FCC_PIPR);
    (void)ev;
    return FALSE;
}

_SGL afxBool _SglPiprEventFilter(afxObject *obj, afxObject *watched, afxEvent *ev)
{
    afxError err = NIL;
    afxPipelineRig pipr = (void*)obj;
    AfxAssertObject(pipr, AFX_FCC_PIPR);
    (void)watched;
    (void)ev;
    return FALSE;
}

_SGL afxError _AfxPiprDtor(afxPipelineRig pipr)
{
    AfxEntry("pipr=%p", pipr);
    afxError err = NIL;
    AfxAssertObject(pipr, AFX_FCC_PIPR);

    afxPipeline pip;
    AfxChainForEveryLinkage(&pipr->pipelines, AFX_OBJECT(afxPipeline), pipr, pip)
    {
        AfxAssertObject(pip, AFX_FCC_PIP);
        while (0 < AfxObjectRelease(&pip->res.obj));
    };

    AfxAssert(pipr->pipelines.cnt == 0);

    return err;
}

_SGL afxError _AfxPiprCtor(afxPipelineRig pipr, _afxPiprParadigm const *paradigm)
{
    AfxEntry("pipr=%p", pipr);
    afxError err = NIL;
    AfxAssertObject(pipr, AFX_FCC_PIPR);

    AfxChainDeploy(&pipr->pipelines, pipr);

    pipr->legtCnt = 0;

    for (afxNat i = 0; i < paradigm->legtCnt; i++)
    {
        afxLegoTemplate legt = paradigm->legt[i];
        AfxAssertObject(legt, AFX_FCC_LEGT);
        afxBool existing = FALSE;

        for (afxNat j = 0; j < pipr->legtCnt; j++)
        {
            if ((existing = (pipr->legt[j] == legt)))
                break;
        }

        if (!existing)
        {
            AfxObjectReacquire(&legt->obj, NIL, NIL, NIL, NIL);
            pipr->legt[pipr->legtCnt] = legt;
            ++pipr->legtCnt;
        }
    }

    afxNat32 crc = 0;
    AfxCrc32(&crc, pipr->legt, sizeof(pipr->legt[0]) * pipr->legtCnt);
    pipr->crc32 = crc;

    return err;
}

_SGL afxPipelineRig _SglDrawContextFindPipelineRig(afxDrawContext dctx, afxNat legtCnt, afxLegoTemplate legt[])
{
    afxError err = NIL;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    AfxAssert(legtCnt);
    AfxAssert(legt);

    afxNat32 crc = 0;
    AfxCrc32(&crc, legt, sizeof(legt[0]) * legtCnt);

    afxPipelineRig pipr;
    AfxChainForEveryLinkage(&(AfxDrawContextGetPipelineRigClass(dctx)->instances), AFX_OBJECT(afxPipelineRig), obj.cls, pipr)
    {
        AfxAssertObject(pipr, AFX_FCC_PIPR);

        if (pipr->crc32 == crc)
        {
            return pipr;
        }
    }
    return NIL;
}

_SGL afxPipelineRig _AfxDrawContextAcquirePipelineRig(afxDrawContext dctx, afxNat legtCnt, afxLegoTemplate legt[])
{
    afxError err = NIL;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    afxPipelineRig pipr = _SglDrawContextFindPipelineRig(dctx, legtCnt, legt);

    if (pipr)
    {
        AfxAssertObject(pipr, AFX_FCC_PIPR);
        AfxObjectReacquire(&pipr->obj, NIL, NIL, NIL, NIL);
    }
    else
    {
        _afxPiprParadigm paradigm =
        {
            dctx,
            legtCnt,
            legt
        };

        if (!(pipr = AfxObjectAcquire(AfxDrawContextGetPipelineRigClass(dctx), &paradigm, AfxSpawnHint())))
            AfxThrowError();

    }
    return pipr;
}

_SGL afxClassSpecification const _AfxPiprClassSpec;

afxClassSpecification const _AfxPiprClassSpec =
{
    AFX_FCC_PIPR,
    NIL,
    0,
    sizeof(AFX_OBJECT(afxPipelineRig)),
    NIL,
    (void*)_AfxPiprCtor,
    (void*)_AfxPiprDtor,
    .event = _SglPiprEventHandler,
    .eventFilter = _SglPiprEventFilter,
    "afxPipelineRig",
    &_AfxStdPiprImpl
};
