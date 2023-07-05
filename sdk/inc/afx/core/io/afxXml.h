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

#ifndef AFX_XML_H
#define AFX_XML_H

#include "afx/core/io/afxStream.h"

// XML --- UNIFORM RESOURCE DICTIONARY

AFX_DEFINE_STRUCT(afxXmlNodeAttr)
{
    afxString name, content;
};

AFX_DEFINE_STRUCT(afxXmlNode)
{
    afxString           name, content;
    afxXmlNodeAttr      **attributes;
    afxXmlNode          **children;
};

AFX_DEFINE_HANDLE(afxXml);

AFX_OBJECT(afxXml)
{
    afxObject       obj; // AFX_FCC_XML
#ifdef _AFX_XML_C
    struct
    {
        afxByte*    buffer;
        afxSize     length;
    }               buffer;
    afxXmlNode      *root;
#endif
};

AFX void*       AfxXmlGetSystem(afxXml xml);
AFX void*       AfxXmlGetFileSystem(afxXml xml);

AFX afxXmlNode* AfxXmlGetRootNode(afxXml xml);
AFX afxString const*  AfxXmlNodeGetName(afxXmlNode const *node);
AFX afxString const*  AfxXmlNodeGetContent(afxXmlNode const *node);
AFX afxNat      AfxXmlNodeGetChildrenCount(afxXmlNode const *node);
AFX afxXmlNode const* AfxXmlNodeGetChild(afxXmlNode const *node, afxNat idx);
AFX afxNat      AfxXmlNodeGetAttributeCount(afxXmlNode const *node);
AFX afxString*  AfxXmlNodeGetAttributeName(afxXmlNode const *node, afxNat idx);
AFX afxString*  AfxXmlNodeGetAttributeContent(afxXmlNode const *node, afxNat idx);

AFX afxXmlNode const* AfxXmlNodeFindChild(afxXmlNode const *base, afxString const *child, afxString const *attr, afxString const *value);

#endif//AFX_XML_H