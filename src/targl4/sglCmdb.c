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
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

#include "sgl.h"
#include "qwadro/afxQwadro.h"

_SGL afxError _SglCmdbResetCb(avxCmdb cmdb)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cmdb->base.state != avxCmdbState_PENDING);

    _sglCmd const* cmdHdr;
    AfxChainForEveryLinkageB2F(&cmdb->commands, _sglCmd, script, cmdHdr)
    {
        if (cmdHdr->id == NIL/*SGL_CMD_END*/)
            break;

        //if (cmdHdr->id == SGL_CMD_BEGIN_SYNTHESIS)
        {
            //_sglCmdBeginSynthesis const *cmd = AFX_REBASE(cmdHdr, _sglCmdBeginSynthesis, cmd);
        }
    }

    AfxZero2(1, sizeof(cmdb->levelCaches[0]), cmdb->levelCaches);

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

            //if (cmd != &cmdb->base.cmdEnd)
            {
                //AfxDeallocate(all, cmd);
                //AfxAssertType(idd->cmdArena, afxFcc_AREN);
                AfxRecycleArenaUnit(&idd->cmdArena, cmd, cmd->siz);
            }
        }
    }
#else
    AfxExhaustArena(&cmdb->base.cmdArena);
#endif

    AfxSetUpChain(&cmdb->commands, cmdb);

    return err;
}

SGL afxError _SglCmdbEndCb(avxCmdb cmdb); // no sglDrawCommands.c

_SGL afxError _SglCmdbBeginCb(avxCmdb cmdb, afxBool permanent)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cmdb, afxFcc_CMDB);
    cmdb->base.disposable = !permanent;
    return err;
}

_SGL afxError _SglCmdbDtor(avxCmdb cmdb)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cmdb, afxFcc_CMDB);

    while (AfxLoadAtom32(&cmdb->base.submCnt))
    {
        AfxYield();
    }

    if (_AvxCmdbStdImplementation.dtor(cmdb))
        AfxThrowError();

    return err;
}

_SGL afxError _SglCmdbCtor(avxCmdb cmdb, afxCookie const* cookie)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cmdb, afxFcc_CMDB);

    if (_AvxCmdbStdImplementation.ctor(cmdb, cookie)) AfxThrowError();
    else
    {
        AfxSetUpChain(&cmdb->commands, cmdb);
        AfxSetUpChain(&cmdb->echoes, cmdb);

        cmdb->base.beginCb = _SglCmdbBeginCb;
        cmdb->base.endCb = _SglCmdbEndCb;
        cmdb->base.resetCb = _SglCmdbResetCb;

        _SglEncodeCmdVmt.Transformation = _SglEncodeCmdTransformationVmt;
        _SglEncodeCmdVmt.Rasterization = _SglEncodeCmdRasterizationVmt;
        _SglEncodeCmdVmt.buf = _SglEncodeCmdBufferVmt;
        _SglEncodeCmdVmt.ras = _SglEncodeCmdRasterVmt;

        cmdb->base.stdCmds = &_SglEncodeCmdVmt;

        if (err && _AvxCmdbStdImplementation.dtor(cmdb))
            AfxThrowError();
    }
    return err;
}

_SGL afxClassConfig _SglCmdbMgrCfg =
{
    .fcc = afxFcc_CMDB,
    .name = "CmdBuffer",
    .desc = "Draw I/O Bufferization",
    .unitsPerPage = 2,
    .size = sizeof(AFX_OBJECT(avxCmdb)),
    .mmu = NIL,
    .ctor = (void*)_SglCmdbCtor,
    .dtor = (void*)_SglCmdbDtor
};
