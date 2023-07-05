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

// This content is part of SIGMA Future Storage <https://sigmaco.org/future-storage>

#ifndef AFX_URD_H
#define AFX_URD_H

#include "afx/core/io/afxStream.h"
#include "afx/core/io/afxResource.h"

// URD --- UNIFORM RESOURCE DICTIONARY

AFX_DEFINE_STRUCT(afxMaterialMap0)
{
    afxString           usage; // 16 // usage
    afxConnection       subMtl; // afxMaterial
};

AFX_OBJECT(afxMaterial0)
{
    afxObject           obj;
    afxString           name; // 32

    afxNat              mapCnt;
    afxMaterialMap0      *maps;
    afxConnection       tex;
    void                *extData;

    // non-Granny stuff
    afxInt              s[3];
    afxReal             shininess;
};

AFX_DEFINE_HANDLE(afxUrd);

AFX_DEFINE_STRUCT(afxResourceHandler)
{
    afxFcc              type;
    afxError            (*upload)(afxResource res);
    afxError            (*download)(afxResource res);
};

AFX_DEFINE_STRUCT(afxResourceAllocation)
{
    afxBool             permanent;
    void                *meta;
    void                *data;
};

AFX_DEFINE_STRUCT(afxResourceLinkage)
{
    afxLinkage          urd; // afxUrd
    afxString           name; // 32
    afxResourceState    status; // initially idle
    afxNat              reqCnt; // initially zero
    afxTime             lastReqTime;
    afxTime             lastUpdTime;
    afxError            (*request)(afxResource res, afxNat cnt, void *udd);
    afxError            (*dispense)(afxResource res);
    afxChain            allocations;
};

AFX_DEFINE_STRUCT(afxResourceChain)
{
    afxFcc      type;
    afxChain    resources;
    afxLinkage  system;
};

AFX_DEFINE_STRUCT(afxFileBackedMaterial)
{
    afxFcc      type;
    afxChain    resources;
    afxLinkage  system;
};

AFX_DEFINE_STRUCT(afxFileBackedMesh)
{
    afxFcc      type;
    afxChain    resources;
    afxLinkage  system;
};

AFX_DEFINE_STRUCT(afxFileBackedModel)
{
    afxLinkage  res;
};

AFX_OBJECT(afxUrd)
{
    afxObject           obj; // AFX_FCC_URD
//#ifdef _AFX_URD_C
    afxUri              uri; // 128
    afxResourceChain    *chain;
    afxNat              chainCnt;
//#endif
};

AFX void*       AfxUrdGetSystem(afxUrd urd);
AFX void*       AfxUrdGetFileSystem(afxUrd urd);


#endif//AFX_URD_H