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

#include "qwadro/inc/io/afxUri.h"
#include "qwadro/inc/mem/afxArray.h"

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
        afxNat  firstChildIdx; // index into a pool of XML documento to avoid mallocs
        afxNat  childCnt;
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
    afxNat      parentIdx;
    afxNat      baseTagIdx;
    afxNat      tagCnt;
    afxNat      baseChildIdx;
    afxNat      childCnt;
};

AFX_DEFINE_STRUCT(afxXml)
{
    afxStream       file;
    afxXmlNode      *root;

    afxNat          elemCnt;
    union
    {
        afxXmlElement*  elems;
        afxXmlElement*  elems300[30];
    };
    afxNat          tagCnt;
    union
    {
        afxXmlTag*      tags;
        afxXmlTag*      tags30[30];
    };

    afxArray        tempElemArr;
    afxArray        tempTagArr;
};

AFX afxError            AfxLoadXml(afxXml* xml, afxUri const *uri);
AFX afxError            AfxParseXml(afxXml* xml, void* buffer, afxNat bufSiz);
AFX void                AfxCleanUpXml(afxXml* xml);

AFX afxXmlNode*         AfxGetXmlRoot(afxXml* xml);
AFX afxString const*    AfxGetXmlNodeName(afxXmlNode const *node);
AFX afxString const*    AfxGetXmlNodeContent(afxXmlNode const *node);
AFX afxNat              AfxCountXmlChildNodes(afxXmlNode const *node);
AFX afxXmlNode const*   AfxGetXmlChildNode(afxXmlNode const *node, afxNat idx);
AFX afxNat              AfxCountXmlAttributes(afxXmlNode const *node);
AFX afxString*          AfxXmlNodeGetAttributeName(afxXmlNode const *node, afxNat idx);
AFX afxString*          AfxXmlNodeGetAttributeContent(afxXmlNode const *node, afxNat idx);

AFX afxBool             AfxXmlNodeHasAttribute(afxXmlNode const* node, afxString const* key, afxBool ci);

AFX afxString const*    AfxFindXmlAttribute(afxXmlNode const* node, afxString const* key, afxBool ci);

AFX afxBool             AfxExtractXmlAttributeI32(afxXmlNode const* node, afxString const* key, afxBool ci, afxInt32* value);
AFX afxBool             AfxExtractXmlAttributeN32(afxXmlNode const* node, afxString const* key, afxBool ci, afxNat32* value);
AFX afxBool             AfxExtractXmlAttributeR32(afxXmlNode const* node, afxString const* key, afxBool ci, afxReal32* value);
AFX afxBool             AfxExtractXmlAttributeR64(afxXmlNode const* node, afxString const* key, afxBool ci, afxReal64* value);


AFX afxXmlNode const*   AfxXmlNodeFindChild(afxXmlNode const *base, afxString const *child, afxString const *attr, afxString const *value);

AFX afxBool             AfxTestXmlRoot(afxXml const* xml, afxString const* name);

AFX afxNat              AfxCountXmlChilds(afxXml const* xml, afxNat elemIdx);
AFX afxNat              AfxCountXmlTags(afxXml const* xml, afxNat elemIdx);

AFX afxNat              AfxGetXmlChild(afxXml const* xml, afxNat elemIdx, afxNat childIdx);

AFX afxXmlElement const*AfxGetXmlElementData(afxXml const* xml, afxNat parentIdx, afxNat childIdx);
AFX afxXmlElement*      AfxGetXmlElementMutableData(afxXml* xml, afxNat parentIdx, afxNat childIdx);

AFX afxNat              AfxGetXmlTag(afxXml const* xml, afxNat elemIdx, afxNat tagIdx);

AFX afxXmlTag const*    AfxGetXmlTagData(afxXml const* xml, afxNat parentIdx, afxNat childIdx, afxNat tagIdx);
AFX afxXmlTag*          AfxGetXmlTagMutableData(afxXml* xml, afxNat parentIdx, afxNat childIdx, afxNat tagIdx);

AFX void                AfxQueryXmlElement(afxXml const* xml, afxNat elemIdx, afxString* name, afxString* content);
AFX void                AfxQueryXmlTag(afxXml const* xml, afxNat elemIdx, afxNat tagIdx, afxString* name, afxString* content);

AFX afxNat              AfxEnumerateXmlElements(afxXml const* xml, afxNat parentIdx, afxNat first, afxNat cnt, afxNat childIdx[]);
AFX afxNat              AfxFindXmlElements(afxXml const* xml, afxNat parentIdx, afxString const* name, afxNat first, afxNat cnt, afxNat childIdx[]);
AFX afxNat              AfxFindXmlTaggedElements(afxXml const* xml, afxNat parentIdx, afxNat first, afxString const* elem, afxString const* tag, afxNat cnt, afxString const value[], afxNat childIdx[]);

#endif//AFX_XML_H
