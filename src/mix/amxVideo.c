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

AFX_OBJECT(amxVideo)
{
    afxUnit         latency; // number of buffers (e.g. 2)
    amxFormat       fmt; // type and bits per sample
    avxCanvasConfig ccfg;

    // video profile
    afxString       codec;
    afxUnit         subsampling[3]; // 4:2:0, 4:2:2, 4:4:4 --- nothing means monochrome.
    afxUnit         lumaBitDepth; // 8, 10, 12
    afxUnit         chromaBitDepth; // 8, 10, 12
    struct
    {
        avxBuffer   buf;
        afxSize     bufOffset;
        afxSize     bufSize;
        amxBuffer   mbuf;
        afxSize     mbufOffset;
        afxSize     mbufSize;
        avxRaster   pic;
    } bufs[2];
    afxUnit bufCnt;
};

_AMX afxError AmxBindVideoPicture(amxVideo vid, afxUnit bufIdx, avxRaster pic)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_VID, 1, &vid);

    if (vid->bufs[bufIdx].pic)
        AfxDisposeObjects(1, &vid->bufs[bufIdx].pic);

    vid->bufs[bufIdx].pic = pic;

    if (pic)
        AfxReacquireObjects(1, &pic);

    return err;
}

_AMX afxError _AmxVidDtorCb(amxVideo vid)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_VID, 1, &vid);

    afxMixSystem msys = AfxGetHost(vid);
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);



    return err;
}

_AMX afxError _AmxVidCtorCb(amxVideo vid, void** args, afxUnit invokeNo)
{
    afxResult err = NIL;
    AFX_ASSERT_OBJECTS(afxFcc_VID, 1, &vid);

    afxMixSystem msys = AfxGetHost(vid);
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    amxVideoInfo const *spec = ((amxVideoInfo const *)args[1]) + invokeNo;
    AFX_ASSERT(spec);



    return err;
}

_AMX afxClassConfig const _AMX_VID_CLASS_CONFIG =
{
    .fcc = afxFcc_VID,
    .name = "Video",
    .desc = "Video",
    .fixedSiz = sizeof(AFX_OBJECT(amxVideo)),
    .ctor = (void*)_AmxVidCtorCb,
    .dtor = (void*)_AmxVidDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AMX afxError AmxAcquireVideos(afxMixSystem msys, afxUnit cnt, amxVideoInfo const info[], amxVideo videos[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    AFX_ASSERT(videos);
    AFX_ASSERT(info);
    AFX_ASSERT(cnt);

    afxClass* cls = (afxClass*)_AmxMsysGetVidClass(msys);
    AFX_ASSERT_CLASS(cls, afxFcc_VID);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)videos, (void const*[]) { msys, (void*)info }))
        AfxThrowError();

    return err;
}
