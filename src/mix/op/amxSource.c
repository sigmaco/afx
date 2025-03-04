/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *       Q W A D R O   S O U N D   S Y N T H E S I S   I N F R A S T R U C T U R E
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

 // This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

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
#define _AMX_WAVEFORM_C
#include "../impl/amxImplementation.h"


_AMX afxError _AmxMsrcDtorCb(amxSource msrc)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MSRC, 1, &msrc);



    return err;
}

_AMX afxError _AmxMsrcCtorCb(amxSource msrc, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MSRC, 1, &msrc);

    afxMixSystem msys = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    //afxMixConfig const* cfg = ((afxMixConfig const *)args[1]) + invokeNo;
    //AFX_ASSERT(cfg);



    return err;
}

_AMX afxClassConfig const _AMX_MSRC_CLASS_CONFIG =
{
    .fcc = afxFcc_MSRC,
    .name = "MediaSource",
    .desc = "Media Source",
    //.fixedSiz = sizeof(AFX_OBJECT(amxSession)),
    .ctor = (void*)_AmxMsrcCtorCb,
    .dtor = (void*)_AmxMsrcDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AMX afxError AfxAcquireMediaSource(afxMixSystem msys, afxUnit exuIdx, amxSource* source)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    AFX_ASSERT(source);

    afxClass* cls = (afxClass*)NIL;// _AmxGetMediaSessionClass(msys);
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
