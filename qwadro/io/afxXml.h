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

#ifndef AFX_XML_H
#define AFX_XML_H

#include "qwadro/io/afxUri.h"
#include "qwadro/mem/afxArray.h"

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

    struct
    {
        // TODO 2023/11/06
        afxUnit  firstChildIdx; // index into a pool of XML documento to avoid mallocs
        afxUnit  childCnt;
    } EXP;
};

AFX_DEFINE_STRUCT(afxXmlTag)
{
    afxString   name;
    afxString   content;
};

AFX_DEFINE_STRUCT(afxXmlElement)
{
    afxString   name;
    afxString   content;
    afxUnit      parentIdx;
    afxUnit      baseTagIdx;
    afxUnit      tagCnt;
    afxUnit      baseChildIdx;
    afxUnit      childCnt;
};

AFX_DEFINE_STRUCT(afxXml)
{
    afxStream       file;
    afxXmlNode      *root;

    afxUnit          elemCnt;
    union
    {
        afxXmlElement*  elems;
        afxXmlElement*  elems300[30];
    };
    afxUnit          tagCnt;
    union
    {
        afxXmlTag*      tags;
        afxXmlTag*      tags30[30];
    };

    afxArray        tempElemArr;
    afxArray        tempTagArr;
};

AFX afxError            AfxLoadXml(afxXml* xml, afxUri const *uri);
AFX afxError            AfxParseXml(afxXml* xml, void* buffer, afxUnit bufSiz);
AFX void                AfxCleanUpXml(afxXml* xml);

AFX afxXmlNode*         AfxGetXmlRoot(afxXml* xml);
AFX afxString const*    AfxGetXmlNodeName(afxXmlNode const *node);
AFX afxString const*    AfxGetXmlNodeContent(afxXmlNode const *node);
AFX afxUnit              AfxCountXmlChildNodes(afxXmlNode const *node);
AFX afxXmlNode const*   AfxGetXmlChildNode(afxXmlNode const *node, afxUnit idx);
AFX afxUnit              AfxCountXmlAttributes(afxXmlNode const *node);
AFX afxString*          AfxXmlNodeGetAttributeName(afxXmlNode const *node, afxUnit idx);
AFX afxString*          AfxXmlNodeGetAttributeContent(afxXmlNode const *node, afxUnit idx);

AFX afxBool             AfxXmlNodeHasAttribute(afxXmlNode const* node, afxString const* key, afxBool ci);

AFX afxString const*    AfxFindXmlAttribute(afxXmlNode const* node, afxString const* key, afxBool ci);

AFX afxBool             AfxExtractXmlAttributeI32(afxXmlNode const* node, afxString const* key, afxBool ci, afxInt32* value);
AFX afxBool             AfxExtractXmlAttributeN32(afxXmlNode const* node, afxString const* key, afxBool ci, afxUnit32* value);
AFX afxBool             AfxExtractXmlAttributeR32(afxXmlNode const* node, afxString const* key, afxBool ci, afxReal32* value);
AFX afxBool             AfxExtractXmlAttributeR64(afxXmlNode const* node, afxString const* key, afxBool ci, afxReal64* value);


AFX afxXmlNode const*   AfxXmlNodeFindChild(afxXmlNode const *base, afxString const *child, afxString const *attr, afxString const *value);

AFX afxBool             AfxTestXmlRoot(afxXml const* xml, afxString const* name);

AFX afxUnit              AfxCountXmlChilds(afxXml const* xml, afxUnit elemIdx);
AFX afxUnit              AfxCountXmlTags(afxXml const* xml, afxUnit elemIdx);

AFX afxUnit              AfxGetXmlChild(afxXml const* xml, afxUnit elemIdx, afxUnit childIdx);

AFX afxXmlElement const*AfxGetXmlElementData(afxXml const* xml, afxUnit parentIdx, afxUnit childIdx);
AFX afxXmlElement*      AfxGetXmlElementMutableData(afxXml* xml, afxUnit parentIdx, afxUnit childIdx);

AFX afxUnit              AfxGetXmlTag(afxXml const* xml, afxUnit elemIdx, afxUnit tagIdx);

AFX afxXmlTag const*    AfxGetXmlTagData(afxXml const* xml, afxUnit parentIdx, afxUnit childIdx, afxUnit tagIdx);
AFX afxXmlTag*          AfxGetXmlTagMutableData(afxXml* xml, afxUnit parentIdx, afxUnit childIdx, afxUnit tagIdx);

AFX void                AfxQueryXmlElement(afxXml const* xml, afxUnit elemIdx, afxString* name, afxString* content);
AFX void                AfxQueryXmlTag(afxXml const* xml, afxUnit elemIdx, afxUnit tagIdx, afxString* name, afxString* content);

AFX afxUnit              AfxEnumerateXmlElements(afxXml const* xml, afxUnit parentIdx, afxUnit first, afxUnit cnt, afxUnit childIdx[]);
AFX afxUnit              AfxFindXmlElements(afxXml const* xml, afxUnit parentIdx, afxString const* name, afxUnit first, afxUnit cnt, afxUnit childIdx[]);
AFX afxUnit              AfxFindXmlTaggedElements(afxXml const* xml, afxUnit parentIdx, afxUnit first, afxString const* elem, afxString const* tag, afxUnit cnt, afxString const value[], afxUnit childIdx[]);

#endif//AFX_XML_H
