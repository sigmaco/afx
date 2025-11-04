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
#include "ddi/amxImplementation.h"

_AVX afxBool AmxIsCodecAcceptable(amxCodec mcdc, amxCodecFeatures const* features, amxCodecLimits const* limits)
{
    afxError err = AFX_ERR_NONE;
    // @mcdc must be a valid amxCodec handle.
    AFX_ASSERT_OBJECTS(afxFcc_MCDC, 1, &mcdc);
    AFX_ASSERT(limits);
    AFX_ASSERT(features);
    afxBool rslt = TRUE;

    if (features)
    {

    }

    if (limits)
    {

    }

    return rslt;
}

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

    afxModule icd = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &icd);
    //afxMixConfig const* cfg = ((afxMixConfig const *)args[1]) + invokeNo;
    //AFX_ASSERT(cfg);



    return err;
}

_AMX afxClassConfig const _AMX_MCDC_CLASS_CONFIG =
{
    .fcc = afxFcc_MCDC,
    .name = "MediaCodec",
    .desc = "Media Codec",
    //.fixedSiz = sizeof(AFX_OBJECT(amxTracker)),
    .ctor = (void*)_AmxMcdcCtorCb,
    .dtor = (void*)_AmxMcdcDtorCb
};

////////////////////////////////////////////////////////////////////////////////
// IMPLEMENTATION DISCOVERY                                                   //
////////////////////////////////////////////////////////////////////////////////

_AMX afxUnit AmxEnumerateCodecs(afxUnit icd, afxUnit first, afxUnit cnt, amxCodec codecs[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(codecs);
    AFX_ASSERT(cnt);
    afxUnit rslt = 0;

    afxModule mdle;
    while (_AmxGetIcd(icd, &mdle))
    {
        AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &mdle);
        AFX_ASSERT(AfxTestModule(mdle, afxModuleFlag_ICD | afxModuleFlag_AMX) == (afxModuleFlag_ICD | afxModuleFlag_AMX));
        afxClass const* cls = _AmxIcdGetMcdcClass(mdle);
        AFX_ASSERT_CLASS(cls, afxFcc_MCDC);
        rslt = AfxEnumerateObjects(cls, first, cnt, (afxObject*)codecs);
        // codecs must be an array of valid amxCodec handles.
        AFX_ASSERT_OBJECTS(afxFcc_MCDC, rslt, codecs);
        break;
    }
    return rslt;
}

_AMX afxUnit AmxInvokeCodecs(afxUnit icd, afxUnit first, void* udd, afxBool(*f)(void*, amxCodec), afxUnit cnt)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(cnt);
    AFX_ASSERT(f);
    afxUnit rslt = 0;

    afxModule mdle;
    while (_AmxGetIcd(icd, &mdle))
    {
        AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &mdle);
        AFX_ASSERT(AfxTestModule(mdle, afxModuleFlag_ICD | afxModuleFlag_AMX) == (afxModuleFlag_ICD | afxModuleFlag_AMX));
        afxClass const* cls = _AmxIcdGetMcdcClass(mdle);
        AFX_ASSERT_CLASS(cls, afxFcc_MCDC);
        rslt = AfxInvokeObjects(cls, (void*)f, udd, first, cnt);
        break;
    }
    return rslt;
}

_AMX afxUnit AmxEvokeCodecs(afxUnit icd, afxUnit first, void* udd, afxBool(*f)(void*, amxCodec), afxUnit cnt, amxCodec codecs[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(codecs);
    AFX_ASSERT(cnt);
    afxUnit rslt = 0;

    afxModule mdle;
    while (_AmxGetIcd(icd, &mdle))
    {
        AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &mdle);
        AFX_ASSERT(AfxTestModule(mdle, afxModuleFlag_ICD | afxModuleFlag_AMX) == (afxModuleFlag_ICD | afxModuleFlag_AMX));
        afxClass const* cls = _AmxIcdGetMcdcClass(mdle);
        AFX_ASSERT_CLASS(cls, afxFcc_MCDC);
        rslt = AfxEvokeObjects(cls, (void*)f, udd, first, cnt, (afxObject*)codecs);
        // @codecs must be an array of valid amxCodec handles.
        AFX_ASSERT_OBJECTS(afxFcc_MCDC, rslt, codecs);
        break;
    }
    return rslt;
}

_AMX afxUnit AmxChooseCodecs(afxUnit icd, amxCodecFeatures const* features, amxCodecLimits const* limits, void const* caps, afxUnit maxCnt, afxUnit mcdcId[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(limits);
    AFX_ASSERT(features);
    afxUnit rslt = 0;
    afxModule mdle;
    while (_AmxGetIcd(icd, &mdle))
    {
        AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &mdle);
        AFX_ASSERT(AfxTestModule(mdle, afxModuleFlag_ICD | afxModuleFlag_AMX) == (afxModuleFlag_ICD | afxModuleFlag_AMX));
        afxClass const* cls = _AmxIcdGetMcdcClass(mdle);
        AFX_ASSERT_CLASS(cls, afxFcc_MCDC);

        afxUnit i = 0;
        amxCodec mcdc;
        while (AfxEnumerateObjects(cls, i, 1, (afxObject*)&mcdc))
        {
            AFX_ASSERT_OBJECTS(afxFcc_MCDC, 1, &mcdc);

            if (features || limits)
                if (!AmxIsCodecAcceptable(mcdc, features, limits))
                    continue;
#if 0
            if (caps)
            {
                if ((mcdc->capabilities & caps->capabilities) != caps->capabilities)
                    continue;

                if ((mcdc->acceleration & caps->acceleration) != caps->acceleration)
                    continue;

                if (mcdc->minQueCnt < caps->minQueCnt)
                    continue;

                if (mcdc->maxQueCnt < caps->maxQueCnt)
                    continue;
            }
#endif

            {
                mcdcId[rslt] = i;
                ++rslt;

                if (maxCnt > rslt)
                    break;
            }
            i++;
        }
        break;
    }
    return rslt;
}

_AMX afxError _AmxRegisterCodecs(afxModule icd, afxUnit cnt, _amxCodecRegistration const infos[], amxCodec codecs[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &icd);
    AFX_ASSERT(codecs);
    AFX_ASSERT(infos);
    AFX_ASSERT(cnt);

    if (!AfxTestModule(icd, afxModuleFlag_ICD | afxModuleFlag_AMX))
    {
        AfxThrowError();
        return err;
    }

    afxClass* cls = (afxClass*)_AmxIcdGetMcdcClass(icd);
    AFX_ASSERT_CLASS(cls, afxFcc_MCDC);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)codecs, (void const*[]) { icd, infos, NIL }))
    {
        AfxThrowError();
        return err;
    }
    else
    {
        AFX_ASSERT_OBJECTS(afxFcc_MCDC, cnt, codecs);

    }
    return err;
}
