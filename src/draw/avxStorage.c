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

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>
// This software is part of Advanced Video Graphics Extensions & Experiments.

#define _AFX_CORE_C
//#define _AFX_DEVICE_C
#define _AFX_CONTEXT_C

#define _AVX_DRAW_C
#define _AVX_DRAW_SYSTEM_C
//#define _AVX_DRAW_DEVICE_C
//#define _AVX_DRAW_BRIDGE_C
//#define _AVX_DRAW_QUEUE_C
//#define _AVX_DRAW_SYSTEM_C
//#define _AVX_SURFACE_C
//#define _AVX_DRAW_INPUT_C
#define _AVX_BUFFER_C
#include "avxIcd.h"
#define _AUX_UX_C
#include "src/ux/auxIcd.h"

_AVX afxError _AvxDsysTransferCb_SW(afxDrawSystem dsys, avxTransference* ctrl, afxUnit opCnt, void const* ops)
{
    afxError err = { 0 };
    // @dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    AFX_ASSERT(opCnt);
    AFX_ASSERT(ctrl);
    AFX_ASSERT(ops);

    afxMask dsysIoExuMask = dsys->ioExuMask;
    afxMask exuMask = ctrl->exuMask;
    afxUnit exuCnt = dsys->bridgeCnt;
    afxUnit firstExuIdx = AfxRandom2(0, exuCnt - 1);

    AFX_ASSERT(!exuMask || (dsysIoExuMask & exuMask));

    afxBool queued = FALSE;

    while (1)
    {
        for (afxUnit exuIdx = firstExuIdx; exuIdx < exuCnt; exuIdx++)
        {
            firstExuIdx = 0;

            // Skip non-transfer-capable EXUs.
            if (!(dsysIoExuMask & AFX_BITMASK(exuIdx)))
                continue; // for

            if (exuMask && !(exuMask & AFX_BITMASK(exuIdx)))
                continue; // for

            // Try to pick one of the dedicated ones when EXUs are not specified.
            if ((!exuMask) && dsys->dedIoExuMask && (!(dsys->dedIoExuMask & AFX_BITMASK(exuIdx))))
                continue; // for
#if 0
            // if a mask is specified and it is not one of the existing dedicated EXUs in mask.
            if (dsys->dedIoExuMask)
            {
                if (exuMask)
                {
                    if ((exuMask & dsys->dedIoExuMask))
                    {
                        if (!(dsys->dedIoExuMask & AFX_BITMASK(exuIdx)))
                            continue;
                    }
                }
                else
                {

                }

                if (exuMask && (dsys->dedIoExuMask & AFX_BITMASK(exuIdx)))
                    continue;
            }
#endif

            afxDrawBridge dexu;
            if (!AvxGetDrawBridges(dsys, exuIdx, 1, &dexu))
            {
                AfxThrowError();
                continue;
            }

            afxError err2 = _AvxDexuTransferVideoMemory(dexu, ctrl, opCnt, ops);

            err = err2;

            if (!err2)
            {
                queued = TRUE;
                break; // for
            }

            if (err2 == afxError_TIMEOUT || err2 == afxError_BUSY)
                continue; // for

            AfxThrowError();
        }
        
        // Try again
        if (err == afxError_TIMEOUT || err == afxError_BUSY)
            continue; // while

        if (err || queued)
            break; // while --- find bridges
    }
    return err;
}

_AVX afxError _AvxDsysRemapBuffersCb_SW(afxDrawSystem dsys, afxBool unmap, afxUnit cnt, _avxBufferRemapping const maps[])
{
    afxError err = { 0 };
    // @dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    AFX_ASSERT(cnt);
    AFX_ASSERT(maps);

    afxError queErr;
    afxBool queued = FALSE;

    afxMask dedIoExuMask;
    afxMask ioExuMask = _AvxDsysGetIoExuMask(dsys, &dedIoExuMask);
    afxUnit exuIdx = 0;
    afxDrawBridge dexu;
    afxUnit exuCnt;

    // Firstly, try to put them in a dedicated queue.
    if (dedIoExuMask)
    {
        exuCnt = AvxChooseDrawBridges(dsys, AFX_INVALID_INDEX, avxAptitude_DMA, dedIoExuMask, 0, 0, NIL);
        AFX_ASSERT(exuCnt);
        exuIdx = 0;
        while (AvxChooseDrawBridges(dsys, AFX_INVALID_INDEX, avxAptitude_DMA, dedIoExuMask, exuIdx++, 1, &dexu))
        {
            queErr = _AvxDexuRemapBuffers(dexu, unmap, cnt, maps);
            err = queErr;

            if (!queErr)
            {
                queued = TRUE;
                break; // while
            }

            if (queErr == afxError_TIMEOUT || queErr == afxError_BUSY)
                continue; // while

            AfxThrowError();
        }
    }

    // If we can not put them in a dedicated queue, try to put them in a shared one.
    if (!queued)
    {
        exuCnt = AvxChooseDrawBridges(dsys, AFX_INVALID_INDEX, avxAptitude_DMA, ioExuMask, 0, 0, NIL);
        AFX_ASSERT(exuCnt);
        exuIdx = 0;
        while (AvxChooseDrawBridges(dsys, AFX_INVALID_INDEX, avxAptitude_DMA, ioExuMask, exuIdx++, 1, &dexu))
        {
            queErr = _AvxDexuRemapBuffers(dexu, unmap, cnt, maps);
            err = queErr;

            if (!queErr)
            {
                queued = TRUE;
                break; // while
            }

            if (queErr == afxError_TIMEOUT || queErr == afxError_BUSY)
                continue; // while

            AfxThrowError();
        }
    }
    return err;
}

_AVX afxError _AvxDsysCohereMappedBuffersCb_SW(afxDrawSystem dsys, afxBool invalidate, afxUnit cnt, avxBufferedMap const maps[])
{
    afxError err = { 0 };
    // @dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    AFX_ASSERT(cnt);
    AFX_ASSERT(maps);

    afxError queErr;
    afxBool queued = FALSE;

    afxMask dedIoExuMask;
    afxMask ioExuMask = _AvxDsysGetIoExuMask(dsys, &dedIoExuMask);
    afxUnit exuIdx = 0;
    afxDrawBridge dexu;
    afxUnit exuCnt;

    // Firstly, try to put them in a dedicated queue.
    if (dedIoExuMask)
    {
        exuCnt = AvxChooseDrawBridges(dsys, AFX_INVALID_INDEX, avxAptitude_DMA, dedIoExuMask, 0, 0, NIL);
        AFX_ASSERT(exuCnt);
        exuIdx = 0;
        while (AvxChooseDrawBridges(dsys, AFX_INVALID_INDEX, avxAptitude_DMA, dedIoExuMask, exuIdx++, 1, &dexu))
        {
            queErr = _AvxDexuCohereMappedBuffers(dexu, invalidate, cnt, maps);
            err = queErr;

            if (!queErr)
            {
                queued = TRUE;
                break; // while
            }

            if (queErr == afxError_TIMEOUT || queErr == afxError_BUSY)
                continue; // while

            AfxThrowError();
        }
    }

    // If we can not put them in a dedicated queue, try to put them in a shared one.
    if (!queued)
    {
        exuCnt = AvxChooseDrawBridges(dsys, AFX_INVALID_INDEX, avxAptitude_DMA, ioExuMask, 0, 0, NIL);
        AFX_ASSERT(exuCnt);
        exuIdx = 0;
        while (AvxChooseDrawBridges(dsys, AFX_INVALID_INDEX, avxAptitude_DMA, ioExuMask, exuIdx++, 1, &dexu))
        {
            queErr = _AvxDexuCohereMappedBuffers(dexu, invalidate, cnt, maps);
            err = queErr;

            if (!queErr)
            {
                queued = TRUE;
                break; // while
            }

            if (queErr == afxError_TIMEOUT || queErr == afxError_BUSY)
                continue; // while

            AfxThrowError();
        }
    }
    return err;
}
