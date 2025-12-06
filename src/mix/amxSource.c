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
#define _AMX_MIX_SYSTEM_C
#define _AFX_CORE_C
#define _AFX_DEVICE_C
#define _AFX_CONTEXT_C
#define _AMX_MIX_DEVICE_C
#define _AMX_MIX_SYSTEM_C
#define _AMX_SINK_C
#define _AMX_MIX_CONTEXT_C
#define _AMX_TRACK_C
#define _AMX_AUDIO_C
#define _AMX_AUDIO_C
#include "amxIcd.h"


_AMX afxError _AmxMsrcDtorCb(amxSource msrc)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MSRC, 1, &msrc);



    return err;
}

_AMX afxError _AmxMsrcCtorCb(amxSource msrc, void** args, afxUnit invokeNo)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MSRC, 1, &msrc);

    afxMixSystem msys = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    //amxContextConfig const* cfg = ((amxContextConfig const *)args[1]) + invokeNo;
    //AFX_ASSERT(cfg);



    return err;
}

_AMX afxClassConfig const _AMX_MSRC_CLASS_CONFIG =
{
    .fcc = afxFcc_MSRC,
    .name = "MediaSource",
    .desc = "Media Source",
    //.fixedSiz = sizeof(AFX_OBJECT(amxTracker)),
    .ctor = (void*)_AmxMsrcCtorCb,
    .dtor = (void*)_AmxMsrcDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AMX afxError AfxAcquireMediaSource(afxMixSystem msys, afxUnit exuIdx, amxSource* source)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    AFX_ASSERT(source);

    afxClass* cls = (afxClass*)NIL;// _AmxMsysGetSesClass(msys);
    AFX_ASSERT_CLASS(cls, afxFcc_MSRC);
    
    amxSource msrc;
    if (AfxAcquireObjects(cls, 1, (afxObject*)&msrc, (void const*[]) { msys, &exuIdx }))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT_OBJECTS(afxFcc_MSRC, 1, &msrc);
    AFX_ASSERT(source);
    *source = msrc;
    return err;
}
