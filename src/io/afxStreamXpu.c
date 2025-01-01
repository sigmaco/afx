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

// This code is part of SIGMA Future Storage <https://sigmaco.org/future-storage>

#define _AFX_CORE_C
#define _AFX_STREAM_C
#include "../dev/afxIoImplKit.h"
#include "../dev/afxExecImplKit.h"

_AFX void _DecodeCmdRead(afxIoBridge exu, afxStdCmd const* cmd)
{
    afxError err = AFX_ERR_NONE;
    afxStream iob = cmd->Read.iob;
    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &iob);
    AFX_ASSERT(AfxIsStreamReadable(iob));

    if (cmd->Read.seek)
    {
        afxSize at = cmd->Read.at;
        if (AfxSeekStream(iob, at, afxSeekOrigin_BEGIN))
        {
            AfxThrowError();
            return;
        }
    }

    afxUnit rowStride = cmd->Read.rowStride;
    afxUnit rowCnt = cmd->Read.rowCnt;
    afxUnit range = rowStride * rowCnt;
    AFX_ASSERT(range);

    void* dst = cmd->Read.dst;
    afxUnit dstCap = cmd->Read.dstCap;
    AFX_ASSERT(dst);
    AFX_ASSERT(dstCap);

    afxUnit clampedOffRange = range;

    if ((clampedOffRange = AfxReadStream2(iob, range, rowStride, dst, dstCap)))
        AfxThrowError();
}

_AFX afxStdCmdList _AfxIobStdDecodeVmt =
{
    .Read = (void*)_DecodeCmdRead,
};

_AFX afxError _XpuRollStreams(afxIoBridge exu, afxUnit cnt, afxStream streams[])
{
    afxError err = AFX_ERR_NONE;

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxStream iob = streams[i];
        AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &iob);
        AFX_ASSERT(iob->state == 3);

        if (iob->state != 3)
        {
            AfxThrowError();
            continue;
        }

        afxStdCmd* cmdHdr;
        AfxChainForEveryLinkageB2F(&iob->commands, afxStdCmd, hdr.script, cmdHdr)
        {
            if (cmdHdr->hdr.id == NIL/*ZGL_CMD_END*/)
            {
                break;
            }

            if (iob->state != 3)
            {
                AfxThrowError();
                break;
            }

            // This shit fucks the CPU prediction. Anyway, we will just ignore it for now.
            _AfxIobStdDecodeVmt.f[cmdHdr->hdr.id](exu, cmdHdr);
        }

        if (!err)
        {
            iob->state = 2;
        }
#if 0
        if (err || mix->m.disposable)
        {
            AFX_ASSERT(mix->m.portId == spu->portId);
            mix->m.state = avxCmdbState_INVALID;
            afxSoundQueue sque = AfxGetProvider(mix);
            AFX_ASSERT_OBJECTS(afxFcc_SQUE, 1, &sque);

            afxUnit poolIdx = mix->m.poolIdx;

            AfxEnterSlockExclusive(&sque->m.cmdbReqLock);

            if (AfxPushQueue(&sque->m.cmdbRecycQue, &mix))
            {
                AfxDisposeObjects(1, (void**)&mix);
            }

            AfxExitSlockExclusive(&sque->m.cmdbReqLock);
        }
#endif
        AfxDisposeObjects(1, &iob);
    }
    return err;
}
