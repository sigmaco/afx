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
 *                       (c) 2017 SIGMA, Engineering In Technology
 *                             <https://sigmaco.org/qwadro/>
 */

#include "salSdev.h"


_A4D afxError _SalSbufDtor(afxSoundBuffer sbuf)
{
    afxError err = AFX_ERR_NONE;
    AfxEntry("sbuf=%p", sbuf);

    afxSoundContext sctx = AfxGetObjectProvider(sbuf);
    afxMmu mmu = AfxGetSoundContextMmu(sctx);
    AfxAssertObjects(1, &mmu, afxFcc_MMU);

    if (sbuf->glHandle)
    {
        _SalSctxDeleteGlRes(dctx, 0, sbuf->glHandle);
        sbuf->glHandle = 0;
    }

    if (sbuf->base.bytemap)
    {
        AfxDeallocate(mmu, sbuf->base.bytemap);
    }
    return err;
}

_A4D afxError _SalSbufCtor(afxSoundBuffer sbuf, afxCookie const* cookie)
{
    AfxEntry("sbuf=%p", sbuf);
    afxResult err = NIL;
    AfxAssertObjects(1, &sbuf, afxFcc_SBUF);

    afxSoundBufferSpecification const *spec = ((afxSbufferSpecification const *)cookie->udd[0]) + cookie->no;

    AfxAssert(spec);

    sbuf->base.siz = spec->siz;
    sbuf->base.usage = spec->usage;
    sbuf->base.access = spec->access;

    afxSoundContext sctx = AfxGetObjectProvider(sbuf);
    afxMmu mmu = AfxGetSoundContextMmu(sctx);
    AfxAssertObjects(1, &mmu, afxFcc_MMU);

    if (!(sbuf->base.bytemap = AfxAllocate(mmu, sizeof(afxByte), sbuf->base.siz, 0, AfxHint()))) AfxThrowError();
    else
    {
        if (spec->src)
            AfxCopy2(1, sbuf->base.siz, spec->src, sbuf->base.bytemap);

        sbuf->lastUpdOffset = 0;
        sbuf->lastUpdRange = sbuf->base.siz;
        sbuf->locked = FALSE;

        sbuf->glHandle = NIL;
        sbuf->updFlags = SGL_UPD_FLAG_DEVICE_INST;

        sbuf->base.map = _SalMapSbufferRange;
        sbuf->base.unmap = _SalUnmapSbufferRange;

        if (err)
            AfxDeallocate(mmu, sbuf->base.bytemap);
    }
    return err;
}

_A4D afxClassConfig _SalSbufClsConfig =
{
    .fcc = afxFcc_SBUF,
    .name = "Sound Buffer",
    .unitsPerPage = 1,
    .size = sizeof(AFX_OBJECT(afxSoundBuffer)),
    .mmu = NIL,
    .ctor = (void*)_SalSbufCtor,
    .dtor = (void*)_SalSbufDtor
};
