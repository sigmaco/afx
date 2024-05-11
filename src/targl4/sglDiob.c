/*
 *             :::::::::::     :::     :::::::::   ::::::::      :::
 *                 :+:       :+: :+:   :+:    :+: :+:    :+:   :+: :+:
 *                 +:+      +:+   +:+  +:+    +:+ +:+         +:+   +:+
 *                 +#+     +#++:++#++: +#++:++#:  :#:        +#++:++#++:
 *                 +#+     +#+     +#+ +#+    +#+ +#+   +#+# +#+     +#+
 *                 #+#     #+#     #+# #+#    #+# #+#    #+# #+#     #+#
 *                 ###     ###     ### ###    ###  ########  ###     ###
 *
 *                  Q W A D R O   E X E C U T I O N   E C O S Y S T E M
 *
 *                                   Public Test Build
 *                       (c) 2017 SIGMA, Engitech, Scitech, Serpro
 *                             <https://sigmaco.org/qwadro/>
 */

#include "sgl.h"
#include "qwadro/afxQwadro.h"

_SGL afxError _SglDiobResetCb(afxDrawStream diob)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(diob->base.state != afxDrawStreamState_PENDING);

    _sglCmd const* cmdHdr;
    AfxChainForEveryLinkageB2F(&diob->commands, _sglCmd, script, cmdHdr)
    {
        if (cmdHdr->id == NIL/*SGL_CMD_END*/)
            break;

        //if (cmdHdr->id == SGL_CMD_BEGIN_SYNTHESIS)
        {
            //_sglCmdBeginSynthesis const *cmd = AFX_REBASE(cmdHdr, _sglCmdBeginSynthesis, cmd);
        }
    }

    AfxZero2(1, sizeof(diob->levelCaches[0]), diob->levelCaches);

#if 0
    while (1)
    {
        afxChain *commands = &idd->commands;
        afxLinkage *first = AfxGetLastLinkage(commands), *dummy = AfxGetAnchor(commands);

        if (first == dummy) break;
        else
        {
            _sglCmd *cmd = AFX_REBASE(first, _sglCmd, script);
            AfxPopLinkage(&cmd->script);

            //if (cmd != &diob->base.cmdEnd)
            {
                //AfxDeallocate(all, cmd);
                //AfxAssertType(idd->cmdArena, afxFcc_AREN);
                AfxRecycleArenaUnit(&idd->cmdArena, cmd, cmd->siz);
            }
        }
    }
#else
    AfxExhaustArena(&diob->base.cmdArena);
#endif

    AfxSetUpChain(&diob->commands, diob);

    return err;
}

SGL afxError _SglDiobEndCb(afxDrawStream diob); // no sglDrawCommands.c

_SGL afxError _SglDiobBeginCb(afxDrawStream diob, afxBool permanent)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &diob, afxFcc_DIOB);
    diob->base.disposable = !permanent;
    return err;
}

_SGL afxError _SglDiobDtor(afxDrawStream diob)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &diob, afxFcc_DIOB);

    AfxRecycleDrawStreams(TRUE, 1, &diob);

    if (_AvxDiobStdImplementation.dtor(diob))
        AfxThrowError();

    return err;
}

_SGL afxError _SglDiobCtor(afxDrawStream diob, afxCookie const* cookie)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &diob, afxFcc_DIOB);

    if (_AvxDiobStdImplementation.ctor(diob, cookie)) AfxThrowError();
    else
    {
        afxDrawBridge ddge = cookie->udd[0];
        AfxAssertObjects(1, &ddge, afxFcc_DDGE);
        afxNat queIdx = *(afxNat*)cookie->udd[1];

        afxDrawContext dctx = ddge->base.dctx;

        AfxSetUpChain(&diob->commands, diob);
        AfxSetUpChain(&diob->echoes, diob);

        diob->base.beginCb = _SglDiobBeginCb;
        diob->base.endCb = _SglDiobEndCb;
        diob->base.resetCb = _SglDiobResetCb;

        _SglEncodeCmdVmt.Transformation = _SglEncodeCmdTransformationVmt;
        _SglEncodeCmdVmt.Rasterization = _SglEncodeCmdRasterizationVmt;
        _SglEncodeCmdVmt.buf = _SglEncodeCmdBufferVmt;
        _SglEncodeCmdVmt.ras = _SglEncodeCmdRasterVmt;

        diob->base.stdCmds = &_SglEncodeCmdVmt;

        if (err && _AvxDiobStdImplementation.dtor(diob))
            AfxThrowError();
    }
    return err;
}

_SGL afxClassConfig _SglDiobMgrCfg =
{
    .fcc = afxFcc_DIOB,
    .name = "DrawStream",
    .desc = "Draw I/O Bufferization",
    .unitsPerPage = 2,
    .size = sizeof(AFX_OBJECT(afxDrawStream)),
    .mmu = NIL,
    .ctor = (void*)_SglDiobCtor,
    .dtor = (void*)_SglDiobDtor
};
