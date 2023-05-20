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
 *                      (c) 2017 SIGMA Co. & SIGMA Technology Group
 *                                    www.sigmaco.org
 */

// This content is part of SIGMA Future Storage <https://sigmaco.org/future-storage>

#ifndef AFX_URD_H
#define AFX_URD_H

#include "afx/core/io/afxStream.h"

// URD --- UNIFORM RESOURCE DICTIONARY

AFX_DEFINE_STRUCT(afxUrdNodeAttr)
{
    afxString name, content;
};

AFX_DEFINE_STRUCT(afxUrdNode)
{
    afxString           name, content;
    afxUrdNodeAttr      **attributes;
    afxUrdNode          **children;
};

AFX_DEFINE_HANDLE(afxUrd);

AFX_OBJECT(afxUrd)
{
    afxObject       obj; // AFX_FCC_URD
#ifdef _AFX_URD_C
    struct
    {
        afxByte*    buffer;
        afxSize     length;
    }               buffer;
    afxUrdNode      *root;
#endif
};

AFX void*       AfxUrdGetSystem(afxUrd urd);
AFX void*       AfxUrdGetFileSystem(afxUrd urd);

AFX afxUrdNode* AfxUrdGetRootNode(afxUrd urd);
AFX afxString const*  AfxUrdNodeGetName(afxUrdNode const *node);
AFX afxString const*  AfxUrdNodeGetContent(afxUrdNode const *node);
AFX afxNat      AfxUrdNodeGetChildrenCount(afxUrdNode const *node);
AFX afxUrdNode const* AfxUrdNodeGetChild(afxUrdNode const *node, afxNat idx);
AFX afxNat      AfxUrdNodeGetAttributeCount(afxUrdNode const *node);
AFX afxString*  AfxUrdNodeGetAttributeName(afxUrdNode const *node, afxNat idx);
AFX afxString*  AfxUrdNodeGetAttributeContent(afxUrdNode const *node, afxNat idx);

#endif//AFX_URD_H