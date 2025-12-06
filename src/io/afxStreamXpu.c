/*
 *           ::::::::    :::::::::::    ::::::::    ::::     ::::       :::
 *          :+:    :+:       :+:       :+:    :+:   +:+:+: :+:+:+     :+: :+:
 *          +:+              +:+       +:+          +:+ +:+:+ +:+    +:+   +:+
 *          +#++:++#++       +#+       :#:          +#+  +:+  +#+   +#++:++#++:
 *                 +#+       +#+       +#+   +#+#   +#+       +#+   +#+     +#+
 *          #+#    #+#       #+#       #+#    #+#   #+#       #+#   #+#     #+#
 *           ########    ###########    ########    ###       ###   ###     ###
 *
 *                     S I G M A   T E C H N O L O G Y   G R O U P
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This code is part of SIGMA Future Storage <https://sigmaco.org/future-storage>

#define _AFX_CORE_C
#define _AFX_STREAM_C
#include "afxIoDDK.h"
#include "../exec/afxSystemDDK.h"

_AFX void _DecodeCmdRead(afxIoBridge exu, afxStdCmd const* cmd)
{
    afxError err = { 0 };
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
    afxError err = { 0 };

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
        AFX_ITERATE_CHAIN_B2F(cmdHdr, hdr.script, &iob->commands)
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
            mix->m.state = avxContextState_INVALID;
            afxMixQueue sque = AfxGetHost(mix);
            AFX_ASSERT_OBJECTS(afxFcc_MQUE, 1, &sque);

            afxUnit poolIdx = mix->m.poolIdx;

            AfxLockFutex(&sque->m.cmdbReqLock);

            if (AfxPushQueue(&sque->m.cmdbRecycQue, &mix))
            {
                AfxDisposeObjects(1, (void**)&mix);
            }

            AfxUnlockFutex(&sque->m.cmdbReqLock);
        }
#endif
        AfxDisposeObjects(1, &iob);
    }
    return err;
}
