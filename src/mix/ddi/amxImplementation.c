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

// This code is part of SIGMA A4D <https://sigmaco.org/a4d>
// This software is part of Advanced Multimedia Extensions & Experiments.

#define _AFX_SYSTEM_C
#define _AFX_MODULE_C
#define _AMX_MIX_C
#define _AFX_DEVICE_C
#define _AMX_MIX_DEVICE_C
//#define _AMX_MIX_BRIDGE_C
//#define _AMX_MIX_SYSTEM_C
//#define _AMX_MIX_QUEUE_C
//#define _AMX_BUFFER_C
//#define _AMX_AUDIO_C
//#define _AMX_VOICE_C
//#define _AMX_MIX_CONTEXT_C
//#define _AMX_SINK_C
#include "amxImplementation.h"

_AMX afxClass const* _AmxIcdGetAudioServiceClass(afxModule icd)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &icd);

    if (!AfxTestModule(icd, afxModuleFlag_ICD | afxModuleFlag_AMX))
    {
        AfxThrowError();
        return NIL;
    }
    afxClass const* cls = &icd->icd.asiCls;
    AFX_ASSERT_CLASS(cls, afxFcc_ASI);
    return cls;
}

_AMX afxClass const* _AmxIcdGetMcdcClass(afxModule icd)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &icd);

    if (!AfxTestModule(icd, afxModuleFlag_ICD | afxModuleFlag_AMX))
    {
        AfxThrowError();
        return NIL;
    }
    afxClass const* cls = &icd->icd.mcdcCls;
    AFX_ASSERT_CLASS(cls, afxFcc_MDEV);
    return cls;
}

_AMX afxClass const* _AmxIcdGetMdevClass(afxModule icd)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &icd);

    if (!AfxTestModule(icd, afxModuleFlag_ICD | afxModuleFlag_AMX))
    {
        AfxThrowError();
        return NIL;
    }
    afxClass const* cls = &icd->icd.mdevCls;
    AFX_ASSERT_CLASS(cls, afxFcc_MDEV);
    return cls;
}

_AMX afxClass const* _AmxIcdGetMsysClass(afxModule icd)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &icd);

    if (!AfxTestModule(icd, afxModuleFlag_ICD | afxModuleFlag_AMX))
    {
        AfxThrowError();
        return NIL;
    }
    afxClass const* cls = &icd->icd.msysCls;
    AFX_ASSERT_CLASS(cls, afxFcc_MSYS);
    return cls;
}

_AMX afxError _AmxRegisterAudioServices(afxModule icd, afxUnit cnt, afxMixDeviceInfo const infos[], afxDevice devices[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &icd);
    AFX_ASSERT(devices);
    AFX_ASSERT(infos);
    AFX_ASSERT(cnt);

    if (!AfxTestModule(icd, afxModuleFlag_ICD | afxModuleFlag_AMX))
    {
        AfxThrowError();
        return err;
    }

    afxClass* cls = (afxClass*)_AmxIcdGetAudioServiceClass(icd);
    AFX_ASSERT_CLASS(cls, afxFcc_MDEV);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)devices, (void const*[]) { icd, infos, NIL }))
    {
        AfxThrowError();
        return err;
    }
    else
    {
        AFX_ASSERT_OBJECTS(afxFcc_ASI, cnt, devices);
    }
    return err;
}

_AMX afxError _AmxRegisterAudioStreamInterface(afxModule icd, afxClassConfig const* asiCls)
{
    afxError err = NIL;
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &icd);
    AFX_ASSERT(asiCls);

    if (!AfxTestModule(icd, afxModuleFlag_ICD))
    {
        AFX_ASSERT(AfxTestModule(icd, afxModuleFlag_ICD));
        AfxThrowError();
        return NIL;
    }

    afxClassConfig clsCfg;

    clsCfg = *asiCls;
    AFX_ASSERT(clsCfg.fcc == afxFcc_ASI);
    //AFX_ASSERT(clsCfg.fixedSiz >= _AVX_VDU_CLASS_CONFIG.fixedSiz);
    if ((clsCfg.fcc != afxFcc_ASI) //||
        //(_AVX_VDU_CLASS_CONFIG.fixedSiz > clsCfg.fixedSiz)
        )
    {
        AfxThrowError();
    }

    // open URI through ASI by id.
    // AfxOpenAudioSink(vaioId, "//./out0"); // speaker
    // AfxOpenAudioSink(vaioId, "//./out1"); // headphone
    // AfxOpenAudioSink(vaioId, "//./in0"); // aux
    // AfxOpenAudioSink(vaioId, "//./in1"); // mic
    // AfxOpenAudioSink(vaioId, "//./in2"); // headmic
    // AfxOpenAudioSink(vaioId, "//./in3"); // mix

    afxSystem sys;
    AfxGetSystem(&sys);
    AFX_ASSERT_OBJECTS(afxFcc_SYS, 1, &sys);

    if (AfxMountClass(&icd->icd.asiCls, (afxClass*)_AfxSysGetDevClass(sys), &icd->classes, &clsCfg)) // require base*
    {
        AfxThrowError();
    }
    else
    {
        AfxPushLink(&icd->icd.amx, &sys->amxIcdChain);
        icd->flags |= afxModuleFlag_AMX;
    }

    AFX_ASSERT(AfxTestModule(icd, afxModuleFlag_ICD | afxModuleFlag_AMX));

    return err;
}

_AMX afxError _AmxImplementMixSystem(afxModule icd, _amxMixSystemImplementation const* cfg)
{
    afxError err = NIL;
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &icd);
    //AFX_ASSERT((mdevCls && msysCls));

    if (!AfxTestModule(icd, afxModuleFlag_ICD))
    {
        AFX_ASSERT(AfxTestModule(icd, afxModuleFlag_ICD));
        AfxThrowError();
        return NIL;
    }

    afxClassConfig clsCfg;

    clsCfg = _AMX_ASIO_CLASS_CONFIG;
    if (AfxMountClass(&icd->icd.asiCls, (afxClass*)NIL, &icd->classes, &clsCfg)) // require base*
    {
        AfxThrowError();
        return err;
    }

    clsCfg = cfg->mcdcCls.fcc ? cfg->mcdcCls : _AMX_MCDC_CLASS_CONFIG;
    AFX_ASSERT(clsCfg.fcc == afxFcc_MCDC);
    AFX_ASSERT(clsCfg.fixedSiz >= _AMX_MCDC_CLASS_CONFIG.fixedSiz);
    if ((clsCfg.fcc != afxFcc_MCDC) ||
        (_AMX_MCDC_CLASS_CONFIG.fixedSiz > clsCfg.fixedSiz))
    {
        AfxThrowError();
        AfxDismountClass(&icd->icd.asiCls);
        return err;
    }
    else if (AfxMountClass(&icd->icd.mcdcCls, (afxClass*)NIL, &icd->classes, &clsCfg)) // require base*
    {
        AfxThrowError();
        AfxDismountClass(&icd->icd.asiCls);
        return err;
    }

    afxSystem sys;
    AfxGetSystem(&sys);
    AFX_ASSERT_OBJECTS(afxFcc_SYS, 1, &sys);

    clsCfg = cfg->mdevCls.fcc ? cfg->mdevCls : _AMX_MDEV_CLASS_CONFIG;
    AFX_ASSERT(clsCfg.fcc == afxFcc_MDEV);
    AFX_ASSERT(clsCfg.fixedSiz >= _AMX_MDEV_CLASS_CONFIG.fixedSiz);
    if ((clsCfg.fcc != afxFcc_MDEV) ||
        (_AMX_MDEV_CLASS_CONFIG.fixedSiz > clsCfg.fixedSiz))
    {
        AfxThrowError();
        AfxDismountClass(&icd->icd.asiCls);
        AfxDismountClass(&icd->icd.mcdcCls);
        return err;
    }
    else if (AfxMountClass(&icd->icd.mdevCls, (afxClass*)_AfxSysGetDevClass(sys), &icd->classes, &clsCfg)) // require base*
    {
        AfxThrowError();
        AfxDismountClass(&icd->icd.asiCls);
        AfxDismountClass(&icd->icd.mcdcCls);
        return err;
    }

    clsCfg = cfg->msysCls.fcc ? cfg->msysCls : _AMX_MSYS_CLASS_CONFIG;
    AFX_ASSERT(clsCfg.fcc == afxFcc_MSYS);
    AFX_ASSERT(clsCfg.fixedSiz >= _AMX_MSYS_CLASS_CONFIG.fixedSiz);
    if ((clsCfg.fcc != afxFcc_MSYS) ||
        (_AMX_MSYS_CLASS_CONFIG.fixedSiz > clsCfg.fixedSiz))
    {
        AfxThrowError();
        AfxDismountClass(&icd->icd.asiCls);
        AfxDismountClass(&icd->icd.mcdcCls);
        AfxDismountClass(&icd->icd.mdevCls);
        return err;
    }
    else if (AfxMountClass(&icd->icd.msysCls, NIL, &icd->classes, &clsCfg)) // require base*
    {
        AfxThrowError();
        AfxDismountClass(&icd->icd.asiCls);
        AfxDismountClass(&icd->icd.mcdcCls);
        AfxDismountClass(&icd->icd.mdevCls);
        return err;
    }

    AfxPushLink(&icd->icd.amx, &sys->amxIcdChain);
    icd->flags |= afxModuleFlag_AMX;

    if (err)
    {
        AfxDismountClass(&icd->icd.asiCls);
        AfxDismountClass(&icd->icd.mcdcCls);
        AfxDismountClass(&icd->icd.mdevCls);
        AfxDismountClass(&icd->icd.msysCls);
    }

    AFX_ASSERT(AfxTestModule(icd, afxModuleFlag_ICD | afxModuleFlag_AMX));

    return err;
}

_AMX afxBool _AmxGetIcd(afxUnit icdIdx, afxModule* driver)
{
    afxError err = AFX_ERR_NONE;
    afxBool found = FALSE;

    afxSystem sys;
    AfxGetSystem(&sys);
    AFX_ASSERT_OBJECTS(afxFcc_SYS, 1, &sys);

    afxModule icd = NIL;
    while ((icdIdx < sys->amxIcdChain.cnt) && (icd = AFX_REBASE(AfxFindFirstLink(&sys->amxIcdChain, icdIdx), AFX_OBJ(afxModule), icd.amx)))
    {
        AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &icd);

        if (AfxTestModule(icd, afxModuleFlag_ICD | afxModuleFlag_AMX))
            found = TRUE;
        else
            icd = NIL;

        break;
    }
    AFX_ASSERT(driver);
    *driver = icd;
    return found;
}

_AMX afxError amxIcdHook(afxModule icd, afxUri const* manifest)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &icd);

    afxClassConfig mdevClsCfg = _AMX_MDEV_CLASS_CONFIG;
    afxClassConfig msysClsCfg = _AMX_MSYS_CLASS_CONFIG;

    _amxMixSystemImplementation mimpl = { 0 };
    mimpl.mcdcCls = _AMX_MCDC_CLASS_CONFIG;
    mimpl.mdevCls = _AMX_MDEV_CLASS_CONFIG;
    mimpl.msysCls = _AMX_MSYS_CLASS_CONFIG;

    if (_AmxImplementMixSystem(icd, &mimpl))
    {
        AfxThrowError();
        return err;
    }

    static afxMixFeatures features = { 0 };

    // In Qwadro AFX, I used to name things based on bad experiences solving issues.
    // Vaio has been named after a bad experience making way for interop with a mix system.
    // Targa has been named after a bad experience handling graphical operations with images originating from left/bottom side.
    // Kurwa has been named after a bad experience when implementing curve-based motion in simulation framework.

    _amxMixDeviceRegistration mdevInfos[] =
    {
        {
            .dev.urn = AFX_STRING("vaio"),
            .dev.type = afxDeviceType_SOUND,

            .features = features,

            //.dev.ioctl = (void*)_ZalSdevIoctrlCb,
            //.relinkAsioCb = _ZalRelinkAsioWasapiCb,

            .capabilities = amxAptitude_SFX | amxAptitude_DMA,
            .minQueCnt = 2,
            .maxQueCnt = 16,
        },
    };

    afxMixDevice mdevices[ARRAY_SIZE(mdevInfos)];

    if (_AmxRegisterMixDevices(icd, ARRAY_SIZE(mdevInfos), mdevInfos, mdevices))
    {
        AfxThrowError();
    }

    return err;
}

int audio_ringbuffer_init(AudioRingBuffer* rb, void* bufPtr, afxUnit bufStride, size_t capacity_frames, size_t channels) {
    rb->capacity = capacity_frames;
    rb->channels = channels;
    rb->write_pos = 0;
    rb->read_pos = 0;
    rb->buffer = bufPtr;
    rb->bufStride = bufStride;
    return rb->buffer ? 0 : -1;
}

void audio_ringbuffer_free(AudioRingBuffer* rb) {
    rb->buffer = NULL;
}

void audio_ringbuffer_write(AudioRingBuffer* rb, const afxByte* input, afxUnit srcStride, size_t frames) {
    for (size_t i = 0; i < frames; ++i) {
        size_t index = (rb->write_pos % rb->capacity) * rb->channels;
        AfxCopy(&rb->buffer[rb->bufStride * index], &input[srcStride * i * rb->channels], sizeof(AFX_MAX(1, AFX_MIN(rb->bufStride, srcStride))) * rb->channels);
        rb->write_pos++;

        // Overwrite oldest data if buffer is full
        if (rb->write_pos - rb->read_pos > rb->capacity) {
            rb->read_pos = rb->write_pos - rb->capacity;
        }
    }
}

size_t audio_ringbuffer_read(AudioRingBuffer* rb, afxByte* output, afxUnit dstStride, size_t max_frames) {
    size_t available = rb->write_pos - rb->read_pos;
    size_t to_read = (available < max_frames) ? available : max_frames;

    for (size_t i = 0; i < to_read; ++i) {
        size_t index = ((rb->read_pos + i) % rb->capacity) * rb->channels;
        AfxCopy(&output[dstStride * i * rb->channels], &rb->buffer[rb->bufStride * index], sizeof(AFX_MAX(1, AFX_MIN(rb->bufStride, dstStride))) * rb->channels);
    }

    rb->read_pos += to_read;
    return to_read;
}

size_t audio_ringbuffer_available(const AudioRingBuffer* rb)
{
    /*
        works only if write_pos is always ahead of or equal to read_pos, and your ring buffer is using monotonically increasing counters (not wrapping indices).
        However, in most ring buffer implementations, the read and write positions wrap around the buffer size, meaning the indices reset to zero after reaching the end of the buffer array. In that case, the function above would break or give incorrect results.
    */
    return rb->write_pos - rb->read_pos;
}

size_t audio_ringbuffer_writable(const AudioRingBuffer* rb) {
    return rb->capacity - audio_ringbuffer_available(rb);
}


/*
    AudioRingBuffer rb;
    audio_ringbuffer_init(&rb, 48000, 2); // 1 sec stereo buffer

    // Simulate writing 512 stereo frames
    float input[512 * 2] = { fill with samples };
    audio_ringbuffer_write(&rb, input, 512);

    // Read up to 256 frames
    float output[256 * 2];
    size_t read = audio_ringbuffer_read(&rb, output, 256);
*/
