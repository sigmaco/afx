/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                      S I G M A   T E C H N O L O G Y   G R O U P
 *
 *                                   Public Test Build
 *                               (c) 2017 Federação SIGMA
 *                                    www.sigmaco.org
 */

#ifndef AFX_RESOURCE_H
#define AFX_RESOURCE_H

#include "afx/core/io/afxUri.h"
#include "afx/core/io/afxXml.h"
#include "afx/core/time/afxTime.h"
#include "afx/core/afxModule.h"

// Base class for all resources.
// Resource is the base class for all resource types.
// Resources are primarily data containers.
// They are reference counted and freed when no longer in use.
// They are also loaded only once from disk, and further attempts to load the resource will return the same reference(all this in contrast to a Node, which is not reference counted and canv be instanced from disk as many times as desired).
// Resources canv be saved externally on disk or bundled into another object, such as a Node or another resource.

typedef enum afxResourceState
{
    AFX_RES_STATE_INITIAL,
    AFX_RES_STATE_PENDING,
    AFX_RES_STATE_LOADING,
    AFX_RES_STATE_READY
}afxResourceState;

AFX_DEFINE_HANDLE(afxResource);

AFX_DEFINE_STRUCT(afxResourceSpecification)
{
    afxUri const    *uri;
    afxError        (*load)(afxResource res);
    afxError        (*unload)(afxResource res);
};

AFX void*           AfxResourceGetSystem(afxResource res);
AFX void*           AfxResourceGetFileSystem(afxResource res);

AFX afxUri const*   AfxResourceGetUri(afxResource res);
AFX afxNat          AfxResourceExcerptName(afxResource res, afxUri *name);
AFX afxBool         AfxResourceIsReady(afxResource res);
AFX afxError        AfxResourceLoad(afxResource res); // find or load (if not found on dictionaries).
AFX afxError        AfxResourceRequest(afxResource res);
AFX afxError        AfxResourceDispense(afxResource res);
AFX afxError        AfxResourceUnload(afxResource res);

// Deveria emitir sinal AFX_EVENT_RES_CHANGED, para avisar observadores que o recurso necessita de ser reinstanciado.

AFX_OBJECT(afxResource)
{
    afxObject           obj;
    afxUri*             uri; // 128 // resource path
    afxResourceState    state;
    afxChain            instances;
    afxNat              reqCnt;
    afxLinkage          monitor; // queued for monitoring for external changes.
    afxTime             lastUpdTime;
    afxTime             lastReqTime; // will be used to liberate space if needed.
    afxError            (*load)(afxResource res);
    afxError            (*unload)(afxResource res);
    afxFcc              superset; // AFX_FCC_MSH, por exemplo
};

#endif//AFX_RESOURCE_H