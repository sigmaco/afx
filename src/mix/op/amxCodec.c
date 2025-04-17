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
#define _AMX_AUDIO_C
#include "../impl/amxImplementation.h"

_AMX afxError _AmxMcdcDtorCb(amxCodec mcdc)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MCDC, 1, &mcdc);



    return err;
}

_AMX afxError _AmxMcdcCtorCb(amxCodec mcdc, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MCDC, 1, &mcdc);

    afxMixSystem msys = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    //afxMixConfig const* cfg = ((afxMixConfig const *)args[1]) + invokeNo;
    //AFX_ASSERT(cfg);



    return err;
}

_AMX afxClassConfig const _AMX_MCDC_CLASS_CONFIG =
{
    .fcc = afxFcc_MCDC,
    .name = "MediaCodec",
    .desc = "Media Codec",
    //.fixedSiz = sizeof(AFX_OBJECT(amxSession)),
    .ctor = (void*)_AmxMcdcCtorCb,
    .dtor = (void*)_AmxMcdcDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AMX afxError AfxAcquireMediaCodec(afxMixSystem msys, afxUnit exuIdx, amxCodec* codec)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    AFX_ASSERT(codec);

    afxClass* cls = (afxClass*)NIL;// _AmxGetMediaSessionClass(msys);
    AFX_ASSERT_CLASS(cls, afxFcc_MCDC);
    
    amxCodec mcdc;
    if (AfxAcquireObjects(cls, 1, (afxObject*)&mcdc, (void const*[]) { msys, &exuIdx }))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT_OBJECTS(afxFcc_MCDC, 1, &mcdc);
    AFX_ASSERT(codec);
    *codec = mcdc;
    return err;
}
