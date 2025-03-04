/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                  Q W A D R O   E X E C U T I O N   E C O S Y S T E M
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

#define _ASX_C
#define _ASX_BUFFER_C
#include "../impl/asxImplementation.h"

_ASX afxError _AsxSbufDtorCb(asxBuffer sbuf)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SBUF, 1, &sbuf);



    return err;
}

_ASX afxError _AsxSbufCtorCb(asxBuffer sbuf, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SBUF, 1, &sbuf);

    afxSimulation sim = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);
    afxCurveInfo const* info = ((afxCurveInfo const*)args[1]) + invokeNo;



    return err;
}

_ASX afxClassConfig const _ASX_SBUF_CLASS_CONFIG =
{
    .fcc = afxFcc_SBUF,
    .name = "SimBuffer",
    .desc = "Sample Buffer",
    .fixedSiz = sizeof(AFX_OBJECT(asxBuffer)),
    .ctor = (void*)_AsxSbufCtorCb,
    .dtor = (void*)_AsxSbufDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_ASX afxError AsxAcquireBuffers(afxSimulation sim, afxUnit cnt, asxBufferInfo const info[], asxBuffer buffers[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);

    afxClass* cls = (afxClass*)_AsxGetBufferClass(sim);
    AFX_ASSERT_CLASS(cls, afxFcc_SBUF);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)buffers, (void const*[]) { sim, info }))
        AfxThrowError();

    return err;
}
