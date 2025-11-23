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

// This software is part of Advanced Multimedia Extensions & Experiments.

#define _AMX_MIX_C
#define _AMX_VIDEO_C
#include "amxIcd.h"

AFX_OBJECT(amxEncoder)
{
    int a;
};

_AMX afxError _AmxMencDtorCb(amxEncoder menc)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MENC, 1, &menc);

    afxMixSystem msys = AfxGetHost(menc);
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);



    return err;
}

_AMX afxError _AmxMencCtorCb(amxEncoder menc, void** args, afxUnit invokeNo)
{
    afxResult err = NIL;
    AFX_ASSERT_OBJECTS(afxFcc_MENC, 1, &menc);

    afxMixSystem msys = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    amxEncoderConfig const* cfg = ((amxEncoderConfig const *)args[1]) + invokeNo;
    AFX_ASSERT(cfg);



    return err;
}

_AMX afxClassConfig const _AMX_MENC_CLASS_CONFIG =
{
    .fcc = afxFcc_MENC,
    .name = "Encoder",
    .desc = "Media Encoder",
    .fixedSiz = sizeof(AFX_OBJECT(amxEncoder)),
    .ctor = (void*)_AmxMencCtorCb,
    .dtor = (void*)_AmxMencDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AMX afxError AmxAcquireEncoders(afxMixSystem msys, afxUnit cnt, amxEncoderConfig const cfgs[], amxEncoder encoders[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    AFX_ASSERT(encoders);
    AFX_ASSERT(cfgs);
    AFX_ASSERT(cnt);

    afxClass* cls = (afxClass*)_AmxMsysGetMencClass(msys);
    AFX_ASSERT_CLASS(cls, afxFcc_MENC);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)encoders, (void const*[]) { msys, (void*)cfgs }))
        AfxThrowError();

    return err;
}
