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

_AFX afxStdCmd* _AfxIobPushCmd(afxStream iob, afxUnit id, afxUnit siz, afxCmdId* cmdId)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &iob);

    afxStdCmd* cmd = AfxRequestArena(&iob->cmdArena, siz, 1, NIL, 0);
    AFX_ASSERT(cmd);
    cmd->hdr.id = id;
    cmd->hdr.siz = siz;
    AFX_ASSERT(cmdId);
    *cmdId = AfxPushLink(&cmd->hdr.script, &iob->commands);
    iob->isUserBuf;
    return cmd;
}

////////////////////////////////////////////////////////////////////////////////

_AFX afxCmdId AfxCmdReadAt(afxStream iob, afxSize at, afxUnit rowStride, afxUnit rowCnt, afxUnit dstCap, void *dst)
{
    afxError err = { 0 };
    // iob must be a valid afxStream handle.
    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &iob);
    // iob must be in the recording state.
    AFX_ASSERT(iob->state == 1); // recording

    afxCmdId cmdId;
    afxStdCmd* cmd = _AfxIobPushCmd(iob, AFX_GET_STD_CMD_ID(Read), sizeof(cmd->Read), &cmdId);
    AFX_ASSERT(cmd);
    cmd->Read.iob = iob;
    cmd->Read.at = at;
    cmd->Read.rowStride = rowStride;
    cmd->Read.rowCnt = rowCnt;
    cmd->Read.dstCap = dstCap;
    cmd->Read.dst = dst;

    cmd->Read.seek = TRUE;
    return cmdId;
}

_AFX afxCmdId AfxCmdRead(afxStream iob, afxUnit rowStride, afxUnit rowCnt, afxUnit dstCap, void *dst)
{
    afxError err = { 0 };
    // iob must be a valid afxStream handle.
    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &iob);
    // iob must be in the recording state.
    AFX_ASSERT(iob->state == 1); // recording

    afxCmdId cmdId;
    afxStdCmd* cmd = _AfxIobPushCmd(iob, AFX_GET_STD_CMD_ID(Read), sizeof(cmd->Read), &cmdId);
    AFX_ASSERT(cmd);
    cmd->Read.iob = iob;
    cmd->Read.at = 0;
    cmd->Read.rowStride = rowStride;
    cmd->Read.rowCnt = rowCnt;
    cmd->Read.dstCap = dstCap;
    cmd->Read.dst = dst;

    cmd->Read.seek = FALSE;
    return cmdId;
}

_AFX afxCmdId AfxCmdWriteAt(afxStream iob, afxSize at, afxUnit rowStride, afxUnit rowCnt, void const* src, afxUnit srcSiz)
{
    afxError err = { 0 };
    // iob must be a valid afxStream handle.
    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &iob);
    // iob must be in the recording state.
    AFX_ASSERT(iob->state == 1); // recording

    afxCmdId cmdId;
    afxStdCmd* cmd = _AfxIobPushCmd(iob, AFX_GET_STD_CMD_ID(Write), sizeof(cmd->Write), &cmdId);
    AFX_ASSERT(cmd);
    cmd->Write.iob = iob;
    cmd->Write.at = at;
    cmd->Write.rowStride = rowStride;
    cmd->Write.rowCnt = rowCnt;
    cmd->Write.srcSiz = srcSiz;
    cmd->Write.src = src;

    cmd->Write.seek = TRUE;
    return cmdId;
}

_AFX afxCmdId AfxCmdWrite(afxStream iob, afxUnit rowStride, afxUnit rowCnt, void const* src, afxUnit srcSiz)
{
    afxError err = { 0 };
    // iob must be a valid afxStream handle.
    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &iob);
    // iob must be in the recording state.
    AFX_ASSERT(iob->state == 1); // recording

    afxCmdId cmdId;
    afxStdCmd* cmd = _AfxIobPushCmd(iob, AFX_GET_STD_CMD_ID(Write), sizeof(cmd->Write), &cmdId);
    AFX_ASSERT(cmd);
    cmd->Write.iob = iob;
    cmd->Write.at = 0;
    cmd->Write.rowStride = rowStride;
    cmd->Write.rowCnt = rowCnt;
    cmd->Write.srcSiz = srcSiz;
    cmd->Write.src = src;

    cmd->Write.seek = FALSE;
    return cmdId;
}

_AFX afxCmdId AfxCmdMerge(afxStream iob, afxStream src, afxSize from, afxUnit range)
{
    afxError err = { 0 };
    // iob must be a valid afxStream handle.
    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &iob);
    // iob must be in the recording state.
    AFX_ASSERT(iob->state == 1); // recording

    afxCmdId cmdId;
    afxStdCmd* cmd = _AfxIobPushCmd(iob, AFX_GET_STD_CMD_ID(Merge), sizeof(cmd->Merge), &cmdId);
    AFX_ASSERT(cmd);
    cmd->Merge.iob = iob;
    cmd->Merge.at = 0;
    cmd->Merge.from = from;
    cmd->Merge.range = range;
    cmd->Merge.src = src;

    cmd->Merge.seek = FALSE;
    return cmdId;
}

_AFX afxCmdId AfxCmdMergeAt(afxStream iob, afxSize at, afxStream src, afxSize from, afxUnit range)
{
    afxError err = { 0 };
    // iob must be a valid afxStream handle.
    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &iob);
    // iob must be in the recording state.
    AFX_ASSERT(iob->state == 1); // recording

    afxCmdId cmdId;
    afxStdCmd* cmd = _AfxIobPushCmd(iob, AFX_GET_STD_CMD_ID(Merge), sizeof(cmd->Merge), &cmdId);
    AFX_ASSERT(cmd);
    cmd->Merge.iob = iob;
    cmd->Merge.at = at;
    cmd->Merge.from = from;
    cmd->Merge.range = range;
    cmd->Merge.src = src;

    cmd->Merge.seek = TRUE;
    return cmdId;
}

_AFX afxCmdId AfxCmdDecode(afxStream iob, afxSize at, afxUnit encSiz, afxFcc codec, afxUnit stop0, afxUnit stop1, afxUnit stop2, void* dst)
{
    afxError err = { 0 };
    // iob must be a valid afxStream handle.
    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &iob);
    // iob must be in the recording state.
    AFX_ASSERT(iob->state == 1); // recording

    afxCmdId cmdId;
    afxStdCmd* cmd = _AfxIobPushCmd(iob, AFX_GET_STD_CMD_ID(Decode), sizeof(cmd->Decode), &cmdId);
    AFX_ASSERT(cmd);
    cmd->Decode.iob = iob;
    cmd->Decode.at = at;
    cmd->Decode.encSiz = encSiz;
    cmd->Decode.codec = codec;
    cmd->Decode.stop0 = stop0;
    cmd->Decode.stop1 = stop1;
    cmd->Decode.stop2 = stop2;
    cmd->Decode.dst = dst;

    cmd->Decode.seek = TRUE;
    return cmdId;
}

_AFX afxCmdId AfxCmdEncode(afxStream iob, afxSize at, afxUnit decSiz, afxFcc codec, afxUnit stop0, afxUnit stop1, afxUnit stop2, void* src)
{
    afxError err = { 0 };
    // iob must be a valid afxStream handle.
    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &iob);
    // iob must be in the recording state.
    AFX_ASSERT(iob->state == 1); // recording

    afxCmdId cmdId;
    afxStdCmd* cmd = _AfxIobPushCmd(iob, AFX_GET_STD_CMD_ID(Encode), sizeof(cmd->Encode), &cmdId);
    AFX_ASSERT(cmd);
    cmd->Encode.iob = iob;
    cmd->Encode.at = at;
    cmd->Encode.decSiz = decSiz;
    cmd->Encode.codec = codec;
    cmd->Encode.stop0 = stop0;
    cmd->Encode.stop1 = stop1;
    cmd->Encode.stop2 = stop2;
    cmd->Encode.src = src;

    cmd->Encode.seek = TRUE;
    return cmdId;
}
