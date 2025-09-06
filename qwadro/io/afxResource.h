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

// This code is part of SIGMA Future Storage.

#ifndef AFX_RESOURCE_H
#define AFX_RESOURCE_H

#include "qwadro/io/afxUri.h"
#include "qwadro/io/afxXml.h"
#include "qwadro/exec/afxTime.h"
#include "qwadro/exec/afxModule.h"

// Base class for all resources.
// Resource is the base class for all resource types.
// Resources are primarily data containers.
// They are reference counted and freed when no longer in use.
// They are also loaded only once from disk, and further attempts to load the resource will return the same reference(all this in contrast to a Node, which is not reference counted and canv be instanced from disk as many times as desired).
// Resources canv be saved externally on disk or bundled into another object, such as a Node or another resource.

#if 0

typedef enum afxResourceState
{
    AFX_RES_STATE_INITIAL,
    AFX_RES_STATE_PENDING,
    AFX_RES_STATE_LOADING,
    AFX_RES_STATE_READY
}afxResourceState;

AFX_DEFINE_STRUCT(afxResourceSpecification)
{
    afxUri const    *uri;
    afxError        (*load)(afxResource res);
    afxError        (*unload)(afxResource res);
};

AFX afxUri const*   AfxGetResourceUri(afxResource res);
AFX afxUnit          AfxExcerptResourceName(afxResource res, afxUri *name);
AFX afxBool         AfxResourceIsReady(afxResource res);
AFX afxError        AfxLoadResource(afxResource res); // find or load (if not found on dictionaries).
AFX afxError        AfxRequestResource(afxResource res);
AFX afxError        AfxDispenseResource(afxResource res);
AFX afxError        AfxUnloadResource(afxResource res);

// Deveria emitir sinal AFX_EVENT_RES_CHANGED, para avisar observadores que o recurso necessita de ser reinstanciado.

AFX_OBJECT(afxResource)
{
    afxObject           obj;
    afxUri*             uri; // 128 // resource path
    afxResourceState    state;
    afxChain            instances;
    afxUnit              reqCnt;
    afxLink          monitor; // queued for monitoring for external changes.
    afxTime             lastUpdTime;
    afxTime             lastReqTime; // will be used to liberate space if needed.
    afxError            (*load)(afxResource res);
    afxError            (*unload)(afxResource res);
    afxFcc              superset; // afxFcc_MSH, por exemplo


    //afxUri              *name;
    //afxUrd              *urd;
};

#endif

#endif//AFX_RESOURCE_H
