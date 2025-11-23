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

AFX_OBJECT(amxDecoder)
{
    int a;
};

_AMX afxError _AmxMdecDtorCb(amxDecoder mdec)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MDEC, 1, &mdec);

    afxMixSystem msys = AfxGetHost(mdec);
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);



    return err;
}

_AMX afxError _AmxMdecCtorCb(amxDecoder mdec, void** args, afxUnit invokeNo)
{
    afxResult err = NIL;
    AFX_ASSERT_OBJECTS(afxFcc_MDEC, 1, &mdec);

    afxMixSystem msys = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    amxDecoderConfig const* cfg = ((amxDecoderConfig const *)args[1]) + invokeNo;
    AFX_ASSERT(cfg);



    return err;
}

_AMX afxClassConfig const _AMX_MDEC_CLASS_CONFIG =
{
    .fcc = afxFcc_MDEC,
    .name = "Decoder",
    .desc = "Media Decoder",
    .fixedSiz = sizeof(AFX_OBJECT(amxDecoder)),
    .ctor = (void*)_AmxMdecCtorCb,
    .dtor = (void*)_AmxMdecDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AMX afxError AmxAcquireDecoders(afxMixSystem msys, afxUnit cnt, amxDecoderConfig const cfgs[], amxDecoder decoders[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    AFX_ASSERT(decoders);
    AFX_ASSERT(cfgs);
    AFX_ASSERT(cnt);

    afxClass* cls = (afxClass*)_AmxMsysGetMdecClass(msys);
    AFX_ASSERT_CLASS(cls, afxFcc_MDEC);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)decoders, (void const*[]) { msys, (void*)cfgs }))
        AfxThrowError();

    return err;
}
