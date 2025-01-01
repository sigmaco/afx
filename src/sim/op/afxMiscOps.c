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

#define _AFX_CORE_C
#define _AFX_DEVICE_C
#define _AMX_ENGINE_C
#define _AMX_SIM_BRIDGE_C
#define _AMX_SIM_QUEUE_C
#define _AMX_CATALYST_C
#include "../impl/amxImplementation.h"


_AMX afxCmdId AmxCmdBindMemory(afxCatalyst cyst, afxUnit slot, void* ptr, afxSize siz)
{
    afxError err = AFX_ERR_NONE;
    /// cyst must be a valid afxCatalyst handle.
    AFX_ASSERT_OBJECTS(afxFcc_CYST, 1, &cyst);
    /// cyst must be in the recording state.
    AFX_ASSERT(cyst->state == amxCatalystState_RECORDING);

    afxCmdId cmdId;
    amxCmd* cmd = _AmxCystPushCmd(cyst, AMX_GET_STD_CMD_ID(BindMemory), sizeof(cmd->BindMemory), &cmdId);
    AFX_ASSERT(cmd);

    cmd->BindMemory.slot = slot;
    cmd->BindMemory.ptr = ptr;
    cmd->BindMemory.siz = siz;

    return cmdId;
}

_AMX afxCmdId AmxCmdBindBuffers(afxCatalyst cyst, afxUnit baseSlot, afxUnit cnt, afxBuffer buffers[], afxUnit const offsets[], afxUnit const ranges[])
{
    afxError err = AFX_ERR_NONE;
    /// cyst must be a valid afxCatalyst handle.
    AFX_ASSERT_OBJECTS(afxFcc_CYST, 1, &cyst);
    /// cyst must be in the recording state.
    AFX_ASSERT(cyst->state == amxCatalystState_RECORDING);

    afxCmdId cmdId;
    amxCmd* cmd = _AmxCystPushCmd(cyst, AMX_GET_STD_CMD_ID(BindBuffers), sizeof(cmd->BindBuffers) + (cnt * sizeof(cmd->BindBuffers.items[0])), &cmdId);
    AFX_ASSERT(cmd);

    cmd->BindBuffers.baseSlot = baseSlot;
    cmd->BindBuffers.cnt = cnt;

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxBuffer buf = buffers[i];
        afxUnit offset = offsets ? offsets[i] : 0;
        afxUnit range = ranges ? ranges[i] : 0;

        if (buf)
        {
            AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);
            afxUnit bufCap = AfxGetBufferCapacity(buf, 0);
            AFX_ASSERT_RANGE(bufCap, offset, range);

            offset = AfxMin(offset, bufCap - 1);
            range = AfxMin(range ? range : bufCap - offset, bufCap - offset);

        }
        else
        {
            AFX_ASSERT(offsets && offsets[i]);
            AFX_ASSERT(ranges && ranges[i]);
            AFX_ASSERT(offset);
            AFX_ASSERT(range);
        }
        cmd->BindBuffers.items[i].buf = buf;
        cmd->BindBuffers.items[i].offset = offset;
        cmd->BindBuffers.items[i].range = range;
    }
    return cmdId;
}
