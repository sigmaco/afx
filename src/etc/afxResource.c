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

// This code is part of SIGMA Future Storage <https://sigmaco.org/future-storage>


#include <sys/stat.h>
#include "../impl/afxIoImplKit.h"

_AFXINL afxError AfxUnloadResource(afxResource res)
{
    afxError err = { 0 };
    AfxAssertObject(res, afxFcc_RES);
    
    if (res->unload)
        if (res->unload(res))
            AfxThrowError();

    return err;
}

_AFXINL afxError AfxLoadResource(afxResource res)
{
    afxError err = { 0 };
    AfxAssertObject(res, afxFcc_RES);

    if (res->load)
        if (res->load(res))
            AfxThrowError();

    return err;
}

_AFXINL afxError AfxRequestResource(afxResource res)
{
    afxError err = { 0 };
    AfxAssertObject(res, afxFcc_RES);
    res->lastReqTime = AfxGetTimer();

    if (res->state == AFX_RES_STATE_INITIAL)
        res->state = AFX_RES_STATE_PENDING;

    return err;
}

_AFXINL afxBool AfxResourceIsReady(afxResource res)
{
    afxError err = { 0 };
    AfxAssertObject(res, afxFcc_RES);
    return (res->state == AFX_RES_STATE_READY);
}

_AFXINL afxUnit AfxExcerptResourceName(afxResource res, afxUri *name)
{
    afxError err = { 0 };
    AfxAssertObject(res, afxFcc_RES);
    AFX_ASSERT(name);
    return AfxPickUriObject(name, res->uri);
}

_AFXINL afxUri const* AfxGetResourceUri(afxResource res)
{
    afxError err = { 0 };
    AfxAssertObject(res, afxFcc_RES);
    return res->uri;
}

_AFX afxBool _AfxResEventHandler(afxObject obj, afxEvent *ev)
{
    afxError err = { 0 };
    afxResource res = (void*)obj;
    AfxAssertObject(res, afxFcc_RES);
    (void)ev;
    return FALSE;
}

_AFX afxBool _AfxResEventFilter(afxObject obj, afxObject watched, afxEvent *ev)
{
    afxError err = { 0 };
    afxResource res = (void*)obj;
    AfxAssertObject(res, afxFcc_RES);
    (void)watched;
    (void)ev;
    return FALSE;
}

_AFX afxError _AfxResDtor(afxResource res)
{
    afxError err = { 0 };
    AfxEntry("res=%p", res);
    AfxAssertObject(res, afxFcc_RES);

    if (res->monitor.chain)
        AfxPopLink(&res->monitor);

    return err;
}

_AFX afxError _AfxResCtor(void *cache, afxUnit idx, afxResource res, afxResourceParadigm const *paradigms)
{
    AfxEntry("res=%p", res);
    afxError err = { 0 };
    AfxAssertObject(res, afxFcc_RES);
    afxResourceParadigm const *paradigm = &paradigms[idx];
    
    if (paradigm->spec)
    {
        res->uri = AfxDuplicateUri(paradigm->spec->uri);
        res->load = paradigm->spec->load;
        res->unload = paradigm->spec->unload;
    }
    else
    {
        res->uri = NIL;
        res->load = NIL;
        res->unload = NIL;
    }

        
    res->state = AFX_RES_STATE_INITIAL;
    res->lastUpdTime = 0;
    res->lastReqTime = 0;

    AfxPushLink(&res->monitor, NIL);
    
    return err;
}

_AFXINL afxResource AfxAcquireResource(afxResourceSpecification const *spec)
{
    afxError err = { 0 };
    afxResource res;

    afxResourceParadigm paradigm = 
    {
        spec
    };

    if (AfxClassAcquireObjects(AfxGetResourceClass(), NIL, 1, &paradigm, (afxHandle**)&res, AfxHere()))
        AfxThrowError();
    else
    {
        AfxReportMessage("Resource %p[%.4s]^%i acquired.", res, (afxChar const*)&(res->superset), res->obj.refCnt);
    }
    return res;
}

AFX afxClassConfig const _AfxResClassSpec;

afxClassConfig const _AfxResClassSpec =
{
    afxFcc_RES,
    NIL,
    sizeof(AFX_OBJECT(afxResource)),
    (void*)_AfxResCtor,
    (void*)_AfxResDtor,
    .event = _AfxResEventHandler,
    .eventFilter = _AfxResEventFilter,
    "afxResource",
    NIL
};
