/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *              T H E   Q W A D R O   E X E C U T I O N   E C O S Y S T E M
 *
 *                                   Public Test Build
 *                   (c) 2017 SIGMA Technology Group — Federação SIGMA
 *                                    www.sigmaco.org
 */


#define _AFX_SAMPLER_C
#include "afx/draw/res/afxSampler.h"
#include "../_classified/afxDrawClassified.h"

_AFX void AfxSamplerDescribe(afxSampler samp, afxSamplerConfig *spec)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(samp, AFX_FCC_SAMP);
    AfxAssert(spec);
    spec->magFilter = samp->magFilter;
    spec->minFilter = samp->minFilter;
    spec->mipmapFilter = samp->mipmapFilter;
    spec->uvw[0] = samp->uvw[0];
    spec->uvw[1] = samp->uvw[1];
    spec->uvw[2] = samp->uvw[2];
    spec->anisotropyEnabled = samp->anisotropyEnabled;
    spec->anisotropyMaxDegree = samp->anisotropyMaxDegree;
    spec->compareEnabled = samp->compareEnabled;
    spec->compareOp = samp->compareOp;
    AfxCopyColor(spec->borderColor, samp->borderColor);
    spec->unnormalizedCoords = samp->unnormalizedCoords;
}

_AFX afxDrawContext AfxGetSamplerContext(afxSampler samp)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(samp, AFX_FCC_SAMP);
    afxDrawContext dctx = AfxObjectGetProvider(&samp->obj);
    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);
    struct _afxDctxD *dctxD;
    _AfxGetDctxD(dctx, &dctxD, dsysD);
    AfxAssertType(dctxD, AFX_FCC_DCTX);
    return dctx;
}

_AFX afxError AfxAcquireSamplers(afxDrawContext dctx, afxNat cnt, afxSamplerConfig const spec[], afxSampler samp[])
{
    afxError err = AFX_ERR_NONE;
    AfxEntry("cnt=%u,spec=%p,spec=%p", cnt, spec, samp);
    AfxAssert(spec[0].magFilter < AFX_TEXEL_FLT_TOTAL);
    AfxAssert(spec[0].minFilter < AFX_TEXEL_FLT_TOTAL);
    AfxAssert(spec[0].mipmapFilter < AFX_TEXEL_FLT_TOTAL);
    AfxAssert(spec[0].uvw[0] < AFX_TEXEL_ADDR_TOTAL);
    AfxAssert(spec[0].uvw[1] < AFX_TEXEL_ADDR_TOTAL);
    AfxAssert(spec[0].uvw[2] < AFX_TEXEL_ADDR_TOTAL);
    AfxAssert(cnt);
    AfxAssert(samp);

    if (AfxClassAcquireObjects(AfxGetSamplerClass(dctx), NIL, cnt, spec, (afxInstance**)samp, AfxSpawnHint()))
        AfxThrowError();

    for (afxNat i = 0; i < cnt; i++)
        AfxTryAssertObject(samp[i], AFX_FCC_SAMP);

    return err;
}

_AFX afxBool _AfxSampEventHandler(afxInstance *obj, afxEvent *ev)
{
    afxError err = AFX_ERR_NONE;
    afxSampler samp = (void*)obj;
    AfxAssertObject(samp, AFX_FCC_SAMP);
    (void)ev;
    return FALSE;
}

_AFX afxBool _AfxSampEventFilter(afxInstance *obj, afxInstance *watched, afxEvent *ev)
{
    afxError err = AFX_ERR_NONE;
    afxSampler samp = (void*)obj;
    AfxAssertObject(samp, AFX_FCC_SAMP);
    (void)watched;
    (void)ev;
    return FALSE;
}

_AFX afxError _AfxSampDtor(afxSampler samp)
{
    afxError err = AFX_ERR_NONE;
    AfxEntry("samp=%p", samp);
    AfxAssertObject(samp, AFX_FCC_SAMP);

    afxDrawContext dctx = AfxObjectGetProvider(&samp->obj);
    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);
    struct _afxDctxD *dctxD;
    _AfxGetDctxD(dctx, &dctxD, dsysD);
    AfxAssertType(dctxD, AFX_FCC_DCTX);
    
    if (samp->vmt->dtor && samp->vmt->dtor(samp))
        AfxThrowError();

    AfxAssert(!samp->idd);

    return err;
}

_AFX afxError _AfxSampCtor(void *cache, afxNat idx, afxSampler samp, afxSamplerConfig const *specs)
{
    afxError err = AFX_ERR_NONE;
    AfxEntry("samp=%p", samp);
    (void)cache;
    AfxAssertObject(samp, AFX_FCC_SAMP);

    afxSamplerConfig const *spec = &specs[idx];
    AfxAssert(spec);

    samp->crc32 = 0;
    AfxCrc32(&samp->crc32, spec, sizeof(*spec));

    samp->magFilter = spec->magFilter;
    samp->minFilter = spec->minFilter;
    samp->mipmapFilter = spec->mipmapFilter;
    samp->uvw[0] = spec->uvw[0];
    samp->uvw[1] = spec->uvw[1];
    samp->uvw[2] = spec->uvw[2];
    samp->anisotropyEnabled = spec->anisotropyEnabled;
    samp->anisotropyMaxDegree = spec->anisotropyMaxDegree;
    samp->compareEnabled = spec->compareEnabled;
    samp->compareOp = spec->compareOp;
    AfxCopyColor(samp->borderColor, spec->borderColor);
    samp->unnormalizedCoords = spec->unnormalizedCoords;
    samp->lodBias = spec->lodBias;
    samp->minLod = spec->minLod;
    samp->maxLod = spec->maxLod;

    samp->idd = NIL;

    afxDrawContext dctx = AfxGetSamplerContext(samp);

    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);
    struct _afxDctxD *dctxD;
    _AfxGetDctxD(dctx, &dctxD, dsysD);
    AfxAssertType(dctxD, AFX_FCC_DCTX);

    if (dctxD->vmt->samp && dctxD->vmt->samp(samp)) AfxThrowError();
    else
    {
        AfxAssert(samp->vmt);
    }
    return err;
}

_AFX afxClassSpecification const _AfxSampClassSpec;

afxClassSpecification const _AfxSampClassSpec =
{
    AFX_FCC_SAMP,
    NIL,
    0,
    sizeof(AFX_OBJECT(afxSampler)),
    NIL,
    (void*)_AfxSampCtor,
    (void*)_AfxSampDtor,
    .event = _AfxSampEventHandler,
    .eventFilter = _AfxSampEventFilter,
    "afxSampler",
    NIL
};
