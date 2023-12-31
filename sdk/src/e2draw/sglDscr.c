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

#include "sgl.h"
#include "qwadro/afxQwadro.h"

_SGL afxError _SglDscrResetCb(afxDrawScript dscr)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state != afxDrawScriptState_PENDING);

    _sglCmd const* cmdHdr;
    AfxChainForEveryLinkageB2F(&dscr->commands, _sglCmd, script, cmdHdr)
    {
        if (cmdHdr->id == SGL_CMD_END)
            break;

        if (cmdHdr->id == SGL_CMD_BEGIN_SYNTHESIS)
        {
            _sglCmdBeginSynthesis const *cmd = AFX_REBASE(cmdHdr, _sglCmdBeginSynthesis, cmd);
        }
    }

    AfxZero(1, sizeof(dscr->levelCaches[0]), dscr->levelCaches);

#if 0
    while (1)
    {
        afxChain *commands = &idd->commands;
        afxLinkage *first = AfxGetLastLinkage(commands), *dummy = AfxGetChainAnchor(commands);

        if (first == dummy) break;
        else
        {
            _sglCmd *cmd = AFX_REBASE(first, _sglCmd, script);
            AfxPopLinkage(&cmd->script);

            //if (cmd != &dscr->base.cmdEnd)
            {
                //AfxDeallocate(all, cmd);
                //AfxAssertType(idd->cmdArena, afxFcc_AREN);
                AfxRecycleArenaUnit(&idd->cmdArena, cmd, cmd->siz);
            }
        }
    }
#else
    AfxExhaustArena(&dscr->base.cmdArena);
#endif

    AfxTakeChain(&dscr->commands, dscr);

    return err;
}

SGL afxError _SglDscrEndCb(afxDrawScript dscr); // no sglDrawCommands.c

_SGL afxError _SglDscrBeginCb(afxDrawScript dscr, afxBool permanent)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    dscr->base.disposable = !permanent;
    return err;
}

_SGL afxError _SglDscrDtor(afxDrawScript dscr)
{
    AfxEntry("dscr=%p", dscr);
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);

    while (1)
    {
        if (dscr->base.state != afxDrawScriptState_PENDING) break;
        else
        {
            // AfxPopLinkage(&dscr->base.queue); // we can't do it here. We need wait for draw context to liberate it.

            //afxDrawContext dctx = AfxGetObjectProvider(&dscr->base.obj);
            //AfxAssertType(dctxD, afxFcc_DCTX);
            //AfxDrawContextProcess(dctx); // process until draw context ends its works and unlock this script.
            //AfxYieldThreading();
        }
        break;
    }

    afxDrawInput din = dscr->base.din;
    AfxAssertObjects(1, &din, afxFcc_DIN);

    for (afxNat i = 0; i < din->base.scripts.cnt; i++)
    {
        afxDrawScript dscr2 = *(afxDrawScript*)AfxGetArrayUnit(&din->base.scripts, i);

        if (dscr2 == dscr)
        {
            *((afxDrawScript*)&din->base.scripts.bytemap[din->base.scripts.unitSiz * i]) = NIL;
        }
    }

    AfxRecycleDrawScript(dscr, TRUE);

    //if (dscr->base.vmt->dtor && dscr->base.vmt->dtor(dscr))
        //AfxThrowError();

    //AfxAssert(!dscr->base.idd);

    AfxDeallocateArena(&dscr->base.cmdArena);

    return err;
}

_SGL afxError _SglDscrCtor(afxDrawScript dscr, afxCookie const* cookie)
{
    AfxEntry("dscr=%p", dscr);
    afxError err = AFX_ERR_NONE;
    //AfxAssertObjects(1, &dscr, afxFcc_DSCR);

    afxDrawContext dctx = cookie->udd[0];
    afxNat portIdx = ((afxNat*)cookie->udd[1])[cookie->no];
    afxDrawInput din = ((afxDrawInput*)cookie->udd[2])[cookie->no];
    
    AfxAssertObjects(1, &din, afxFcc_DIN);

    afxDrawContext dctx2 = AfxGetDrawInputContext(din);
    AfxAssert(dctx2 == dctx);
    afxMmu mmu = AfxGetDrawContextMmu(dctx);

    dscr->base.submRefCnt = 0;
    dscr->base.portIdx = portIdx;
    AfxAllocateArena(mmu, &dscr->base.cmdArena, NIL, AfxHint());

    dscr->base.din = din;

    afxNat index = 0;
    AfxInsertArrayUnits(&din->base.scripts, 1, &index, &dscr);

    dscr->base.disposable = TRUE;

    dscr->base.state = afxDrawScriptState_INITIAL;
    
    dscr->base.stdCmds = NIL;

    AfxTakeChain(&dscr->commands, dscr);
    
    dscr->base.beginCb = _SglDscrBeginCb;
    dscr->base.endCb = _SglDscrEndCb;
    dscr->base.resetCb = _SglDscrResetCb;

    dscr->base.stdCmds = &_SglEncodeCmdVmt;

    {

        AfxAssert(dscr->base.stdCmds);
    }

    if (err)
        AfxDeallocateArena(&dscr->base.cmdArena);

    return err;
}

_SGL afxClassConfig _SglDscrClsConfig =
{
    .fcc = afxFcc_DSCR,
    .name = "Draw Script",
    .unitsPerPage = 2,
    .size = sizeof(AFX_OBJECT(afxDrawScript)),
    .mmu = NIL,
    .ctor = (void*)_SglDscrCtor,
    .dtor = (void*)_SglDscrDtor
};
