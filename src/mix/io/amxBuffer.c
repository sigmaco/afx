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
#define _AMX_BUFFER_C
#include "../impl/amxImplementation.h"

_AMX afxError _AmxMbufDtorCb(amxBuffer mbuf)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MBUF, 1, &mbuf);



    return err;
}

_AMX afxError _AmxMbufCtorCb(amxBuffer mbuf, void** args, afxUnit invokeNo)
{
    afxResult err = NIL;
    AFX_ASSERT_OBJECTS(afxFcc_MBUF, 1, &mbuf);

    afxMixSystem msys = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    amxBufferInfo const *spec = ((amxBufferInfo const *)args[1]) + invokeNo;
    AFX_ASSERT(spec);
    


    return err;
}

_AMX afxClassConfig const _AMX_MBUF_CLASS_CONFIG =
{
    .fcc = afxFcc_MBUF,
    .name = "MediaBuffer",
    .desc = "Multimedia Buffer",
    .fixedSiz = sizeof(AFX_OBJECT(amxBuffer)),
    .ctor = (void*)_AmxMbufCtorCb,
    .dtor = (void*)_AmxMbufDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AMX afxError AmxAcquireBuffers(afxMixSystem msys, afxUnit cnt, amxBufferInfo const infos[], amxBuffer buffers[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    AFX_ASSERT(buffers);
    AFX_ASSERT(infos);
    AFX_ASSERT(cnt);

    afxClass* cls = (afxClass*)_AmxGetBufferClass(msys);
    AFX_ASSERT_CLASS(cls, afxFcc_MBUF);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)buffers, (void const*[]) { msys, (void*)infos }))
        AfxThrowError();

    return err;
}
