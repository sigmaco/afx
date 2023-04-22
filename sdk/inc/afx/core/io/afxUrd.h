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

#ifndef AFX_URD_H
#define AFX_URD_H

#include "afxStream.h"

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
    struct
    {
        afxByte*    buffer;
        afxSize     length;
    }               buffer;
    afxUrdNode      *root;
};

AFX void*       AfxUrdGetSystem(afxUrd urd);
AFX void*       AfxUrdGetFileSystem(afxUrd urd);

AFX afxUrdNode* AfxUrdGetRootNode(afxUrd urd);
AFX afxString*  AfxUrdNodeGetName(afxUrdNode *node);
AFX afxString*  AfxUrdNodeGetContent(afxUrdNode *node);
AFX afxNat      AfxUrdNodeGetChildrenCount(afxUrdNode *node);
AFX afxUrdNode* AfxUrdNodeGetChild(afxUrdNode *node, afxNat idx);
AFX afxNat      AfxUrdNodeGetAttributeCount(afxUrdNode *node);
AFX afxString*  AfxUrdNodeGetAttributeName(afxUrdNode *node, afxNat idx);
AFX afxString*  AfxUrdNodeGetAttributeContent(afxUrdNode *node, afxNat idx);

#endif//AFX_URD_H