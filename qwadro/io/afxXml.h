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

/*
    In XML (Extensible Markup Language), several terms are commonly used (like element, tag, attribute, node, etc.) 
    and they can sometimes be confusing because they are closely related but refer to different parts of an XML document.

        Element	is a complete unit defined by tags (can contain attributes, text, other elements).
        Tag	is a syntax to mark the beginning or end of an element.
        Attribute is a key-value pair inside a tag providing additional info.
        Node is a point in the XML document tree (element, text, comment, etc.).
        Text is a content between tags.
        Comment is a note for humans, ignored by parser.
        CDATA is a section of text not parsed by the XML processor.
        Namespace is used to distinguish elements from different vocabularies.

    1. Element
    An element is the basic building block of an XML document. It consists of a start tag, content, and an end tag.

    Example:
        <title>XML Basics</title>

        title is the element name.

        "XML Basics" is the element content.

    2. Tag
    A tag is what you see inside the angle brackets (< >). It marks the start or end of an element.
    The three types of tags are: start tag (<title>), end tag (</title>), and empty tag/self-closing tag (<line-break />).

    3. Attribute
    An attribute provides additional information about an element. It's defined within the start tag.

    Example:
        <book genre="fiction" year="2023">

        genre and year are attribute names.

        "fiction" and "2023" are attribute values.

    4. Node
    A node is a generic term used for any single point in the XML document tree.
    This includes element nodes, text nodes, attribute nodes, comment nodes, and document nodes.
    Think of it like "everything in an XML document is a node".

    5. Text
    The actual content inside an element, not inside tags.

    Example:
        <title>XML Basics</title>
        
        "XML Basics" is a text node.

    6. Comment
    Notes added to the XML document for humans, ignored by parsers.

    Example:
        <!-- This is a comment -->

    7. CDATA Section
    Used to include content that should not be parsed by the XML parser (e.g., special characters).

    Example:
        <script><![CDATA[ if (a < b) alert("Hi"); ]]></script>

    8. Namespace
    Prevents element name conflicts by qualifying names with a URI.

    Example:
        <xhtml:div xmlns:xhtml="http://www.w3.org/1999/xhtml">
*/

#ifndef AFX_XML_H
#define AFX_XML_H

#include "qwadro/io/afxUri.h"
#include "qwadro/mem/afxArray.h"

// XML --- UNIFORM RESOURCE DICTIONARY

AFX_DEFINE_STRUCT(afxXmlAttr)
{
    afxString   name;
    afxString   content;
    afxLink     elem;
};

AFX_DEFINE_STRUCT(afxXmlNode)
{
    afxString   name;
    afxString   content;
    afxLink     parent;
    afxChain    children;
    afxChain    attributes;

    //afxXmlAttr          **attributes;
    //afxXmlNode          **children;

    struct
    {
        // TODO 2023/11/06
        afxUnit  firstChildIdx; // index into a pool of XML documento to avoid mallocs
        afxUnit  childCnt;
    } EXP;
};

AFX_DEFINE_STRUCT(afxXml)
{
    afxStream       file;
    afxStringBase   strb;
    afxPool         elems;
    afxPool         attrs;
    afxXmlNode      *root;
};

AFX afxError            AfxLoadXml(afxXml* xml, afxUri const *uri);
AFX afxError            AfxParseXml(afxXml* xml, void* buffer, afxUnit bufSiz);
AFX void                AfxCleanUpXml(afxXml* xml);

AFX afxXmlNode*         AfxGetXmlRoot(afxXml* xml);
AFX afxString const*    AfxGetXmlNodeName(afxXmlNode const *node);
AFX afxString const*    AfxGetXmlNodeContent(afxXmlNode const *node);
AFX afxUnit             AfxCountXmlChildNodes(afxXmlNode const *node);
AFX afxXmlNode const*   AfxGetXmlChildNode(afxXmlNode const *node, afxUnit idx);
AFX afxUnit             AfxCountXmlAttributes(afxXmlNode const *node);
AFX afxString const*    AfxXmlNodeGetAttributeName(afxXmlNode const *node, afxUnit idx);
AFX afxString const*    AfxXmlNodeGetAttributeContent(afxXmlNode const *node, afxUnit idx);

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

AFX afxUnit              AfxGetXmlTag(afxXml const* xml, afxUnit elemIdx, afxUnit tagIdx);

AFX afxXmlAttr const*    AfxGetXmlTagData(afxXml const* xml, afxUnit parentIdx, afxUnit childIdx, afxUnit tagIdx);
AFX afxXmlAttr*          AfxGetXmlTagMutableData(afxXml* xml, afxUnit parentIdx, afxUnit childIdx, afxUnit tagIdx);

AFX void                AfxQueryXmlElement(afxXml const* xml, afxUnit elemIdx, afxString* name, afxString* content);
AFX void                AfxQueryXmlAttribute(afxXml const* xml, afxUnit elemIdx, afxUnit tagIdx, afxString* name, afxString* content);

AFX afxUnit             AfxEnumerateXmlChildElements(afxXml const* xml, afxUnit parentIdx, afxUnit first, afxUnit cnt, afxUnit childIdx[]);

AFX afxXmlNode const*   AfxFindXmlChildElement(afxXmlNode const* parent, afxXmlNode const* last, afxString const* tag);
AFX afxXmlNode const*   AfxFindXmlAttributedChildElement(afxXmlNode const* parent, afxXmlNode const* last, afxString const* elem, afxString const* attrKey, afxString const* attrValue);

AFX afxUnit             AfxCountXmlChildElements(afxXmlNode const* parent, afxXmlNode const* last, afxString const* name);
AFX afxUnit             AfxCountXmlAttributedChildElements(afxXmlNode const* parent, afxXmlNode const* last, afxString const* elem, afxString const* attrKey, afxString const* attrValue);

#endif//AFX_XML_H
