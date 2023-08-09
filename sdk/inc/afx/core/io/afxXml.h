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

// This content is part of SIGMA Future Storage <https://sigmaco.org/future-storage>

#ifndef AFX_XML_H
#define AFX_XML_H

#include "afx/core/io/afxUri.h"

// XML --- UNIFORM RESOURCE DICTIONARY

AFX_DEFINE_STRUCT(afxXmlAttr)
{
    afxString name, content;
};

AFX_DEFINE_STRUCT(afxXmlNode)
{
    afxString           name, content;
    afxXmlAttr          **attributes;
    afxXmlNode          **children;
};

AFX_DEFINE_STRUCT(afxXml)
{
    _AFX_DBG_FCC; // AFX_FCC_XML
    struct
    {
        afxByte*    buffer;
        afxSize     length;
    }               buffer;
    afxXmlNode      *root;
};

AFX afxError            AfxLoadXml(afxXml* xml, afxUri const *uri);
AFX afxError            AfxParseXml(afxXml* xml, void* buffer, afxNat bufSiz);
AFX void                AfxReleaseXml(afxXml* xml);

AFX afxXmlNode*         AfxXmlGetRootNode(afxXml* xml);
AFX afxString const*    AfxXmlNodeGetName(afxXmlNode const *node);
AFX afxString const*    AfxXmlNodeGetContent(afxXmlNode const *node);
AFX afxNat              AfxXmlNodeGetChildrenCount(afxXmlNode const *node);
AFX afxXmlNode const*   AfxXmlNodeGetChild(afxXmlNode const *node, afxNat idx);
AFX afxNat              AfxXmlNodeGetAttributeCount(afxXmlNode const *node);
AFX afxString*          AfxXmlNodeGetAttributeName(afxXmlNode const *node, afxNat idx);
AFX afxString*          AfxXmlNodeGetAttributeContent(afxXmlNode const *node, afxNat idx);

AFX afxXmlNode const*   AfxXmlNodeFindChild(afxXmlNode const *base, afxString const *child, afxString const *attr, afxString const *value);

#endif//AFX_XML_H