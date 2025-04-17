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

#define _AMX_MIX_C
#define _AMX_VIDEO_C
#include "../impl/amxImplementation.h"

AFX_OBJECT(amxVideoDecoder)
{
    int a;
};

_AMX afxError _AmxViddDtorCb(amxVideoDecoder vidd)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_VIDD, 1, &vidd);

    afxMixSystem msys = AfxGetProvider(vidd);
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);



    return err;
}

_AMX afxError _AmxViddCtorCb(amxVideoDecoder vidd, void** args, afxUnit invokeNo)
{
    afxResult err = NIL;
    AFX_ASSERT_OBJECTS(afxFcc_VIDD, 1, &vidd);

    afxMixSystem msys = AfxGetProvider(vidd);
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    amxVideoInfo const *spec = ((amxVideoInfo const *)args[1]) + invokeNo;
    AFX_ASSERT(spec);



    return err;
}

_AMX afxClassConfig const _AMX_VIDD_CLASS_CONFIG =
{
    .fcc = afxFcc_VIDD,
    .name = "VideoDecoder",
    .desc = "Video Decoder",
    .fixedSiz = sizeof(AFX_OBJECT(amxVideoDecoder)),
    .ctor = (void*)_AmxViddCtorCb,
    .dtor = (void*)_AmxViddDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AMX afxError AmxAcquireVideoDecoders(afxMixSystem msys, afxUnit cnt, amxVideoInfo const info[], amxVideoDecoder decoders[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    AFX_ASSERT(decoders);
    AFX_ASSERT(info);
    AFX_ASSERT(cnt);

    afxClass* cls = (afxClass*)NIL; //_AmxMsysGetViddClass(msys);
    AFX_ASSERT_CLASS(cls, afxFcc_VIDD);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)decoders, (void const*[]) { msys, (void*)info }))
        AfxThrowError();

    return err;
}
