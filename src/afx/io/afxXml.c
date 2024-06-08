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

// This code is part of SIGMA Future Storage <https://sigmaco.org/future-storage>

#define _CRT_SECURE_NO_WARNINGS 1
#include <assert.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define _AFX_XML_C
#include "qwadro/core/afxSystem.h"

static char* xml_strtok_r(char *str, const char *delim, char **nextp) {
    char *ret;

    if (str == NULL) {
        str = *nextp;
    }

    str += strspn(str, delim);

    if (*str == '\0') {
        return NULL;
    }

    ret = str;

    str += strcspn(str, delim);

    if (*str) {
        *str++ = '\0';
    }

    *nextp = str;

    return ret;
}

struct xml_parser {
    afxByte *buffer;
    afxSize position;
    afxSize length;
};

enum xml_parser_offset {
    NO_CHARACTER = -1,
    CURRENT_CHARACTER = 0,
    NEXT_CHARACTER = 1,
};

static size_t get_zero_terminated_array_attributes(afxXmlAttr** attributes)
{
    size_t elements = 0;

    while (attributes[elements]) {
        ++elements;
    }

    return elements;
}

static size_t get_zero_terminated_array_nodes(afxXmlNode** nodes)
{
    size_t elements = 0;

    while (nodes[elements]) {
        ++elements;
    }

    return elements;
}

static void AfxXmlNodeAttrDelete(afxXml* xml, afxXmlAttr* attribute)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(attribute);

    AfxDeallocate(attribute);
}

static void AfxXmlNodeDelete(afxXml* xml, afxXmlNode *node)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(node);

    afxXmlAttr** at = node->attributes;
    while (*at)
    {
        AfxXmlNodeAttrDelete(xml, *at);
        ++at;
    }

    AfxDeallocate(node->attributes);

    afxXmlNode** it = node->children;

    while (*it)
    {
        AfxXmlNodeDelete(xml, *it);
        ++it;
    }

    AfxDeallocate(node->children);

    AfxDeallocate(node);
}

static uint8_t _AfxXmlParsePeek(struct xml_parser* parser, size_t n) {
    size_t position = parser->position;

    while (position < parser->length) {
        if (!isspace(parser->buffer[position])) {
            if (n == 0) {
                return parser->buffer[position];
            }
            else {
                --n;
            }
        }

        position++;
    }

    return 0;
}


static void _AfxXmlParseConsume(struct xml_parser* parser, size_t n)
{
    parser->position += n;

    if (parser->position >= parser->length) {
        parser->position = parser->length - 1;
    }
}

static void _AfxXmlParseSkipWhitespaces(struct xml_parser* parser) {

    while (isspace(parser->buffer[parser->position])) {
        if (parser->position + 1 >= parser->length) {
            return;
        }
        else {
            parser->position++;
        }
    }
}

static afxXmlAttr** _AfxXmlParseAttribs(afxXml* xml, struct xml_parser* parser, afxString *tag_open, afxNat* baseTagIdx, afxNat* tagCnt)
{
    afxError err = AFX_ERR_NONE;
    (void)parser;
    char* tmp;
    char* rest = NULL;
    char* token;
    char* str_name;
    char* str_content;
    const afxChar* start_name;
    const afxChar* start_content;
    size_t old_elements;
    size_t new_elements;
    afxXmlAttr* new_attribute;
    afxXmlAttr** attributes;
    int position;

    attributes = AfxCoallocate(1, sizeof(afxXmlAttr*), 0, AfxHere());
    attributes[0] = 0;

    afxString128 tmpStr;
    AfxMakeString128(&tmpStr, tag_open);
    tmp = (char*)AfxGetStringStorage(&tmpStr.str,0);

    token = xml_strtok_r(tmp, " ", &rest); // skip the first value
    
    if (token != NULL)
    {
        afxNat baseTagIdx2 = xml->tempTagArr.cnt;
        afxNat tagCnt2 = 0;
        //tag_open->range = strlen(token);
        afxNat trimRange = strlen(token);

        for (token = xml_strtok_r(NULL, " ", &rest); token != NULL; token = xml_strtok_r(NULL, " ", &rest))
        {
            str_name = AfxAllocate(1, strlen(token) + 1, 0, AfxHere());
            str_content = AfxAllocate(1, strlen(token) + 1, 0, AfxHere());
            // %s=\"%s\" wasn't working for some reason, ugly hack to make it work
            if (sscanf(token, "%[^=]=\"%[^\"]", str_name, str_content) != 2)
            {
                if (sscanf(token, "%[^=]=\'%[^\']", str_name, str_content) != 2)
                {
                    AfxDeallocate(str_name);
                    AfxDeallocate(str_content);
                    continue;
                }
            }
            position = token - tmp;
            start_name = &tag_open->start[position];
            start_content = &tag_open->start[position + strlen(str_name) + 2];

            new_attribute = AfxAllocate(1, sizeof(afxXmlAttr), 0, AfxHere());
            AfxMakeString(&new_attribute->name, start_name, strlen(str_name));
            AfxMakeString(&new_attribute->content, start_content, strlen(str_content));

            old_elements = get_zero_terminated_array_attributes(attributes);
            new_elements = old_elements + 1;
            attributes = AfxReallocate(attributes, sizeof(struct xml_attributes*), (new_elements + 1), 0, AfxHere());

            attributes[new_elements - 1] = new_attribute;
            attributes[new_elements] = 0;

            afxNat addedIdx;
            afxXmlTag* tag = AfxInsertArrayUnits(&xml->tempTagArr, 1, &addedIdx, NIL);
            AfxAssert(addedIdx == baseTagIdx2 + tagCnt2);
            AfxMakeString(&tag->name, start_name, strlen(str_name));
            AfxMakeString(&tag->content, start_content, strlen(str_content));
            ++tagCnt2;
            

            AfxDeallocate(str_name);
            AfxDeallocate(str_content);
        }
        
        tag_open->len = trimRange;

        *baseTagIdx = baseTagIdx2;
        *tagCnt = tagCnt2;
    }
    else
    {
        *baseTagIdx = 0;
        *tagCnt = 0;
    }
    return attributes;
}

static afxXmlNode* _AfxXmlParseNode(afxXml* xml, afxNat parentIdx, struct xml_parser* parser)
{
    afxError err = AFX_ERR_NONE;

    // NEW

    afxNat elemIdx;
    afxNat baseTagIdx = 0;
    afxNat tagCnt = 0;
    afxNat baseChildIdx = 0;
    afxNat childCnt = 0;
    AfxInsertArrayUnits(&xml->tempElemArr, 1, &elemIdx, NIL);
    baseChildIdx = xml->tempElemArr.cnt;

    ////////////////////////////////////////////////////////////

    // PROBLEMAS ATUAIS 2023/08/04


    // VÁLIDO
    // <Pipeline name='yFlipped'>

    // INVÁLIDO - Não vai registrar o atributo.
    // <Pipeline name = 'yFlipped'>
    // <Pipeline name>

    // INVÁLIDO - Vai causar erro no fechamento do node.
    // <Pipeline name = 'yFlipped' >

    // INVÁLIDO - Vai concatenar a barra
    // <Enabled/>

    ////////////////////////////////////////////////////////////

    /* Setup variables
     */
    afxString tag_open;
    afxString tag_close;
    afxString content;
    AfxResetString(&tag_open);
    AfxResetString(&tag_close);
    AfxResetString(&content);

    size_t original_length;
    afxXmlAttr** attributes;

    afxXmlNode** children = AfxCoallocate(1, sizeof(afxXmlNode*), 0, AfxHere());
    children[0] = 0;

    // Parse open tag
    // Parses an opening XML tag without attributes. Ex.: <TagName>

    _AfxXmlParseSkipWhitespaces(parser);

    /* Consume `<'
        */

    if (!('<' == _AfxXmlParsePeek(parser, CURRENT_CHARACTER)))
        AfxThrowError();
    else
    {
        _AfxXmlParseConsume(parser, 1); // skip '<'

        // Consume tag name
        // Parses the name out of the an XML tag's ending. Ex.: TagName>
              
        size_t start = parser->position;
        size_t length = 0;

        // Parse until '>' or a whitespace is reached

        while (start + length < parser->length)
        {
            uint8_t current = _AfxXmlParsePeek(parser, CURRENT_CHARACTER);

            if (('>' == current) || isspace(current)) break;
            else
            {
                _AfxXmlParseConsume(parser, 1);
                length++;
            }
        }

        // Consume '>'

        if (!('>' == _AfxXmlParsePeek(parser, CURRENT_CHARACTER))) AfxThrowError();
        else
        {
            _AfxXmlParseConsume(parser, 1);

            // Map parsed tag name
            AfxMakeString(&tag_open, &parser->buffer[start], length);

            if (tag_open.start[3] == 'w')
            {
                int a = 0;
            }
        }
    }

    if (err) AfxThrowError();
    else
    {
        original_length = tag_open.len;
        attributes = _AfxXmlParseAttribs(xml, parser, &tag_open, &baseTagIdx, &tagCnt);
        //original_length = tag_open.range;

        // If tag ends with '/' it's self closing, skip content lookup. Drop '/' and go to node creation

        if (!(tag_open.len > 0 && '/' == tag_open.start[original_length - 1] /**(afxChar*)AfxGetStringStorage(&tag_open, original_length - 1)*/))
        {
            // If the content does not start with '<', a text content is assumed

            if (!('<' == _AfxXmlParsePeek(parser, CURRENT_CHARACTER)))
            {
                // Parses a tag's content. Ex.: any text between until next left angle brackets<
                
                _AfxXmlParseSkipWhitespaces(parser);

                size_t start = parser->position;
                size_t length = 0;

                // Consume until '<' is reached

                while (start + length < parser->length)
                {
                    uint8_t current = _AfxXmlParsePeek(parser, CURRENT_CHARACTER);

                    if ('<' == current) break;
                    else
                    {
                        _AfxXmlParseConsume(parser, 1);
                        length++;
                    }
                }

                // Next character must be an `<' or we have reached end of file

                if (!('<' == _AfxXmlParsePeek(parser, CURRENT_CHARACTER))) AfxThrowError();
                else
                {
                    // Ignore tailing whitespaces

                    while ((length > 0) && isspace(parser->buffer[start + length - 1]))
                        length--;

                    // Return mapped text
                    AfxMakeString(&content, &parser->buffer[start], length);
                }
            }
            else // <
            {
                while ('/' != _AfxXmlParsePeek(parser, NEXT_CHARACTER))
                {
                    if ('!' == _AfxXmlParsePeek(parser, NEXT_CHARACTER) && '[' == _AfxXmlParsePeek(parser, NEXT_CHARACTER + 1))
                    {
                        // Parses a tag's CDATA content. Ex.: <![CDATA[any text between until next left angle brackets]]>

                        _AfxXmlParseSkipWhitespaces(parser);
                        _AfxXmlParseConsume(parser, 3);

                        // Consume until '[' is reached
                        afxSize start = parser->position;
                        afxSize length = 0;

                        while (start + length < parser->length)
                        {
                            uint8_t current = _AfxXmlParsePeek(parser, CURRENT_CHARACTER);

                            if ('[' == current)
                            {
                                _AfxXmlParseConsume(parser, 1); // skip [
                                length++;
                                break;
                            }
                            else
                            {
                                _AfxXmlParseConsume(parser, 1);
                                length++;
                            }
                        }

                        afxSize start2 = parser->position;
                        afxSize length2 = 0;

                        // Consume until ']]>' is reached

                        uint8_t current = 0;
                        uint8_t prev1 = 0, prev2 = 0;

                        while (start2 + length2 < parser->length)
                        {
                            prev2 = prev1;
                            prev1 = current;
                            current = _AfxXmlParsePeek(parser, CURRENT_CHARACTER);

                            if ('>' == current && ']' == prev1 && ']' == prev2)
                            {
                                _AfxXmlParseConsume(parser, 1);
                                //length2++;
                                length2 -= 3; // ]]>
                                break;
                            }
                            else
                            {
                                _AfxXmlParseConsume(parser, 1);
                                length2++;
                            }
                        }

                        // Next character must be an '<' or we have reached end of file

                        if (!('<' == _AfxXmlParsePeek(parser, CURRENT_CHARACTER))) AfxThrowError();
                        else
                        {
                            // Return mapped CDATA

                            AfxMakeString(&content, &parser->buffer[start2], length2);
                        }
                    }
                    else
                    {
                        // Otherwise children are to be expected
                        // Parse child node
                        
                        afxXmlNode* child = _AfxXmlParseNode(xml, elemIdx, parser);

                        if (!child)
                        {
                            AfxThrowError();
                            break;
                        }

                        
                        ++childCnt;

                        // Grow child array :)

                        size_t old_elements = get_zero_terminated_array_nodes(children);
                        size_t new_elements = old_elements + 1;
                        children = AfxReallocate(children, sizeof(afxXmlNode*), (new_elements + 1), 0, AfxHere());

                        // Save child

                        children[new_elements - 1] = child;
                        children[new_elements] = 0;
                    }
                }
            }

            if (!err)
            {
                // Parse close tag
                // Parses an closing XML tag without attributes. Ex.: </TagName>
                     
                _AfxXmlParseSkipWhitespaces(parser);

                // Consume '</'

                if (!('<' == _AfxXmlParsePeek(parser, CURRENT_CHARACTER))) AfxThrowError();
                else if (!('/' == _AfxXmlParsePeek(parser, NEXT_CHARACTER))) AfxThrowError();
                else
                {
                    _AfxXmlParseConsume(parser, 2);

                    // Consume tag name
                    // Parses the name out of the an XML tag's ending. Ex.: TagName>

                    size_t start = parser->position;
                    size_t length = 0;

                    // Parse until '>' or a whitespace is reached

                    while (start + length < parser->length)
                    {
                        uint8_t current = _AfxXmlParsePeek(parser, CURRENT_CHARACTER);

                        if (('>' == current) || isspace(current)) break;
                        else
                        {
                            _AfxXmlParseConsume(parser, 1);
                            length++;
                        }
                    }

                    // Consume '>'

                    if (!('>' == _AfxXmlParsePeek(parser, CURRENT_CHARACTER))) AfxThrowError();
                    else
                    {
                        _AfxXmlParseConsume(parser, 1);

                        // Return parsed tag name

                        AfxMakeString(&tag_close, &parser->buffer[start], length);
                    }
                }

                if (err)
                {
                    AfxThrowError();
                }
                else
                {
                    /* Close tag has to match open tag
                     */

                    if (0 != AfxCompareString(&tag_open, &tag_close))
                    {
                        AfxThrowError();
                        AfxLogError(" %.*s %.*s", AfxPushString(&tag_open), AfxPushString(&tag_close));
                    }
                }
            }
        }

        /* Return parsed node
         */
        if (!err)
        {
            if (tag_open.start[3] == 'w')
            {
                int a = 0;
            }

            afxXmlNode* node = AfxAllocate(1, sizeof(afxXmlNode), 0, AfxHere());
            AfxMakeString(&node->name, tag_open.start, tag_open.len);
            AfxMakeString(&node->content, content.start, content.len);
            node->attributes = attributes;
            node->children = children;

            // NEW
            
            afxXmlElement* elem = AfxGetArrayUnit(&xml->tempElemArr, elemIdx);
            elem->parentIdx = parentIdx;
            elem->baseTagIdx = tagCnt ? baseTagIdx : 0;
            elem->tagCnt = tagCnt;
            elem->baseChildIdx = childCnt ? baseChildIdx : 0;
            elem->childCnt = childCnt;
            AfxMakeString(&elem->name, tag_open.start, tag_open.len);
            AfxMakeString(&elem->content, content.start, content.len);

            //AfxLogComment("%u/%u; %u/%u; %u/%u; %.*s : %.8s;", elemIdx, elem->parentIdx, elem->baseTagIdx, elem->tagCnt, elem->baseChildIdx, elem->childCnt, AfxPushString(&elem->name), 0);

            
            AfxAssert(!tagCnt || baseTagIdx == elem->baseTagIdx);
            AfxAssert(!childCnt || baseChildIdx == elem->baseChildIdx);

            return node;
        }
    }

    /* A failure occured, so free mem allocalted resources
     */
    if (err)
    {
        afxXmlNode** it = children;
        while (*it) {
            AfxXmlNodeDelete(xml, *it);
            ++it;
        }
        AfxDeallocate(children);
    }
    return 0;
}

_AFX afxError AfxParseXml(afxXml* xml, void* buffer, afxNat length)
{
    afxError err = AFX_ERR_NONE;
    /* Initialize parser
     */
    struct xml_parser parser = {
        .buffer = buffer,
        .position = 0,
        .length = length
    };

    /* An empty buffer canv never contain a valid document
     */
    if (!length)
    {
        AfxThrowError();
    }
    else
    {
        xml->tempElemArr;
        AfxAllocateArray(&xml->tempElemArr, 100, sizeof(afxXmlElement), NIL);
        xml->tempTagArr;
        AfxAllocateArray(&xml->tempTagArr, 100, sizeof(afxXmlTag), NIL);

        /* Parse the root node
         */
        afxXmlNode *root = _AfxXmlParseNode(xml, AFX_INVALID_INDEX, &parser);

        if (!root)
        {
            AfxThrowError();

            AfxDeallocateArray(&xml->tempElemArr);
            AfxDeallocateArray(&xml->tempTagArr);
        }
        else
        {
            /* Return parsed document
             */

            xml->root = root;
            
            xml->elemCnt = xml->tempElemArr.cnt;
            afxNat elemCnt = xml->tempElemArr.cnt;
            xml->tagCnt = xml->tempTagArr.cnt;
            xml->tags = xml->tempTagArr.data;
            xml->elems = xml->tempElemArr.data;

            for (afxNat i = 0; i < elemCnt; i++)
            {
                afxXmlElement*elem = &xml->elems[i];//AfxGetArrayUnit(&xml->tempElemArr, i);
                //AfxLogComment("%u/%u; %u/%u; %u/%u; %.*s : %.8s;", i, elem->parentIdx, elem->baseTagIdx, elem->tagCnt, elem->baseChildIdx, elem->childCnt, AfxPushString(&elem->name), 0);
            }
            int a = 1;

            afxArray sorted;
            AfxAllocateArray(&sorted, elemCnt, sizeof(afxXmlElement), NIL);
            AfxReserveArraySpace(&sorted, elemCnt);

            afxNat* indicesMap = AfxAllocate(elemCnt, sizeof(indicesMap[0]), 0, AfxHere());

            afxNat activeParentIdx = AFX_INVALID_INDEX;
            afxNat remaningCnt = elemCnt;
            while (remaningCnt)
            {
                for (afxNat i = 0; i < elemCnt; i++)
                {
                    afxXmlElement* elem = AfxGetArrayUnit(&xml->tempElemArr, i);

                    if (elem->parentIdx == activeParentIdx)
                    {
                        afxNat idx;
                        afxXmlElement* elem2 = AfxInsertArrayUnit(&sorted, &idx);
                        indicesMap[i] = idx;
                        //AfxAssert(idx == i);
                        AfxReflectString(&elem->name, &elem2->name);
                        AfxReflectString(&elem->content, &elem2->content);
                        elem2->childCnt = elem->childCnt;
                        elem2->tagCnt = elem->tagCnt;
                        elem2->baseChildIdx = 0;
                        elem2->baseTagIdx = elem->baseTagIdx;

                        if (elem->parentIdx != AFX_INVALID_INDEX)
                            elem2->parentIdx = indicesMap[elem->parentIdx];
                        else
                            elem2->parentIdx = AFX_INVALID_INDEX;
                    }
                }

                if (activeParentIdx == AFX_INVALID_INDEX)
                    activeParentIdx = 0;
                else
                    ++activeParentIdx;

                remaningCnt--;
            }

            for (afxNat i = 0; i < elemCnt; i++)
            {
                afxXmlElement* old = AfxGetArrayUnit(&xml->tempElemArr, i);                
                afxXmlElement* neo = AfxGetArrayUnit(&sorted, indicesMap[i]);
                neo->baseChildIdx = indicesMap[old->baseChildIdx];
            }

            AfxDeallocate(indicesMap);

            xml->elemCnt = sorted.cnt;
            xml->elems = sorted.data;

#if 0
            for (afxNat i = 0; i < elemCnt; i++)
            {
                afxXmlElement*elem = &xml->elems[i];//AfxGetArrayUnit(&xml->tempElemArr, i);
                AfxLogEcho("# %u, @ %u, [ %u, %u ], [ %u, %u ], %.*s : %.8s;", i, elem->parentIdx, elem->baseTagIdx, elem->tagCnt, elem->baseChildIdx, elem->childCnt, AfxPushString(&elem->name), 0);
            }
#endif

            int b = 1;
        }
    }
    return err;
}

afxError _AfxXmlOpen(afxXml* xml, afxStream file)
{
    afxError err = AFX_ERR_NONE;

    if (AfxParseXml(xml, AfxGetStreamBuffer(file, 0), AfxGetStreamLength(file)))
        AfxThrowError();

    return err;
}

_AFX afxXmlNode* AfxGetXmlRoot(afxXml* xml)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(xml, afxFcc_XML);
    return xml->root;
}

_AFX afxString const* AfxGetXmlNodeName(afxXmlNode const *node)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(node);
    return &node->name;
}

_AFX afxString const* AfxGetXmlNodeContent(afxXmlNode const *node)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(node);
    return &node->content;
}

_AFX afxNat AfxCountXmlChildNodes(afxXmlNode const *node)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(node);
    return get_zero_terminated_array_nodes(node->children);
}

_AFX afxXmlNode const* AfxGetXmlChildNode(afxXmlNode const *node, afxNat idx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(node);

    if (idx >= AfxCountXmlChildNodes(node))
    {
        return 0;
    }

    return node->children[idx];
}

_AFX afxNat AfxCountXmlAttributes(afxXmlNode const *node)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(node);
    return get_zero_terminated_array_attributes(node->attributes);
}

_AFX afxString* AfxXmlNodeGetAttributeName(afxXmlNode const *node, afxNat idx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(node);

    if (idx >= AfxCountXmlAttributes(node))
    {
        return 0;
    }

    return &node->attributes[idx]->name;
}

_AFX afxString* AfxXmlNodeGetAttributeContent(afxXmlNode const *node, afxNat idx)
{
    if (idx >= AfxCountXmlAttributes(node))
    {
        return 0;
    }

    return &node->attributes[idx]->content;
}

_AFX afxBool AfxXmlNodeHasAttribute(afxXmlNode const* node, afxString const* key, afxBool ci)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(node);
    AfxAssert(key);
    afxNat cnt = AfxCountXmlAttributes(node);

    for (afxNat i = 0; i < cnt; i++)
    {
        afxXmlAttr const *a = node->attributes[i];

        if ((ci && (0 == AfxCompareStringCi(key, &a->name))) || (0 == AfxCompareString(key, &a->name)))
            return TRUE;
    }
    return FALSE;
}

_AFX afxString const* AfxFindXmlAttribute(afxXmlNode const* node, afxString const* key, afxBool ci)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(node);
    AfxAssert(key);
    afxNat cnt = AfxCountXmlAttributes(node);

    for (afxNat i = 0; i < cnt; i++)
    {
        afxXmlAttr const *a = node->attributes[i];

        if ((ci && (0 == AfxCompareStringCi(key, &a->name))) || (0 == AfxCompareString(key, &a->name)))
            return &a->content;
    }
    return NIL;
}

_AFX afxBool AfxExtractXmlAttributeI32(afxXmlNode const* node, afxString const* key, afxBool ci, afxInt32* value)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(node);
    AfxAssert(key);

    afxBool rslt;
    afxString const *content;

    if ((rslt = !!(content = AfxFindXmlAttribute(node, key, ci))))
    {
        AfxAssert(value);
        AfxScanString(content, "%i", value);
    }
    return rslt;
}

_AFX afxBool AfxExtractXmlAttributeN32(afxXmlNode const* node, afxString const* key, afxBool ci, afxNat32* value)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(node);
    AfxAssert(key);

    afxBool rslt;
    afxString const *content;

    if ((rslt = !!(content = AfxFindXmlAttribute(node, key, ci))))
    {
        AfxAssert(value);
        AfxScanString(content, "%u", value);
    }
    return rslt;
}

_AFX afxBool AfxExtractXmlAttributeR32(afxXmlNode const* node, afxString const* key, afxBool ci, afxReal32* value)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(node);
    AfxAssert(key);

    afxBool rslt;
    afxString const *content;

    if ((rslt = !!(content = AfxFindXmlAttribute(node, key, ci))))
    {
        AfxAssert(value);
        AfxScanString(content, "%f", value);
    }
    return rslt;
}

_AFX afxBool AfxExtractXmlAttributeR64(afxXmlNode const* node, afxString const* key, afxBool ci, afxReal64* value)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(node);
    AfxAssert(key);

    afxBool rslt;
    afxString const *content;

    if ((rslt = !!(content = AfxFindXmlAttribute(node, key, ci))))
    {
        AfxAssert(value);
        AfxScanString(content, "%f", value);
    }
    return rslt;
}


_AFX afxBool AfxTestXmlRoot(afxXml const* xml, afxString const* name)
{
    afxError err = AFX_ERR_NONE;
    //AfxAssertType(xml, afxFcc_XML);
    return (0 == AfxCompareString(&(xml->root->name), name));
}

_AFX afxBool AfxIndexXmlElements(afxXml const* xml, afxNat parentIdx, afxNat base, afxNat cnt, afxString const names[], afxNat* nameIdx)
{
    afxError err = AFX_ERR_NONE;
    //AfxAssertType(xml, afxFcc_XML);
    AfxAssertRange(xml->elemCnt, parentIdx, 1);
    AfxAssert(names);
    AfxAssert(cnt);
    afxBool rslt = FALSE;
    afxXmlElement const* parent = &xml->elems[parentIdx];

    for (afxNat i = 0; i < cnt; i++)
    {
        if (0 == AfxCompareString(&xml->elems[parent->baseChildIdx].name, &names[i]))
        {
            if (nameIdx)
                *nameIdx = i;

            return TRUE;
        }
    }
    return rslt;
}

_AFX afxBool AfxTestXmlTag(afxXml const* xml, afxNat elemIdx, afxNat cnt, afxString const names[], afxNat* nameIdx)
{
    afxError err = AFX_ERR_NONE;
    //AfxAssertType(xml, afxFcc_XML);
    AfxAssertRange(xml->elemCnt, elemIdx, 1);
    AfxAssert(names);
    AfxAssert(cnt);
    afxBool rslt = FALSE;

    for (afxNat i = 0; i < cnt; i++)
    {
        if (0 == AfxCompareString(&xml->elems[elemIdx].name, &names[i]))
        {
            if (nameIdx)
                *nameIdx = i;

            return TRUE;
        }
    }
    return rslt;
}

_AFX afxNat AfxCountXmlChilds(afxXml const* xml, afxNat elemIdx)
{
    afxError err = AFX_ERR_NONE;
    //AfxAssertType(xml, afxFcc_XML);
    AfxAssertRange(xml->elemCnt, elemIdx, 1);
    return xml->elems[elemIdx].childCnt;
}

_AFX afxNat AfxCountXmlTags(afxXml const* xml, afxNat elemIdx)
{
    afxError err = AFX_ERR_NONE;
    //AfxAssertType(xml, afxFcc_XML);
    AfxAssertRange(xml->elemCnt, elemIdx, 1);
    return xml->elems[elemIdx].tagCnt;
}

_AFX afxXmlTag* AfxGetXmlTagMutableData(afxXml* xml, afxNat parentIdx, afxNat childIdx, afxNat tagIdx)
{
    afxError err = AFX_ERR_NONE;
    //AfxAssertType(xml, afxFcc_XML);
    AfxAssertRange(xml->elemCnt, parentIdx, 1);
    afxXmlElement const* parent = &xml->elems[parentIdx];
    AfxAssertRange(parent->childCnt, childIdx, 1);
    afxXmlElement const* child = &xml->elems[parent->baseChildIdx + childIdx];
    AfxAssertRange(child->tagCnt, tagIdx, 1);
    return &xml->tags[child->baseTagIdx + tagIdx];
}

_AFX afxXmlTag const* AfxGetXmlTagData(afxXml const* xml, afxNat parentIdx, afxNat childIdx, afxNat tagIdx)
{
    afxError err = AFX_ERR_NONE;
    //AfxAssertType(xml, afxFcc_XML);
    AfxAssertRange(xml->elemCnt, parentIdx, 1);
    afxXmlElement const* parent = &xml->elems[parentIdx];
    AfxAssertRange(parent->childCnt, childIdx, 1);
    afxXmlElement const* child = &xml->elems[parent->baseChildIdx + childIdx];
    AfxAssertRange(child->tagCnt, tagIdx, 1);
    return &xml->tags[child->baseTagIdx + tagIdx];
}

_AFX afxNat AfxGetXmlTag(afxXml const* xml, afxNat elemIdx, afxNat tagIdx)
{
    afxError err = AFX_ERR_NONE;
    //AfxAssertType(xml, afxFcc_XML);
    AfxAssertRange(xml->elemCnt, elemIdx, 1);
    afxXmlElement const* elem = &xml->elems[elemIdx];
    AfxAssertRange(elem->tagCnt, tagIdx, 1);
    return elem->baseTagIdx + tagIdx;
}

_AFX afxXmlElement* AfxGetXmlElementMutableData(afxXml* xml, afxNat parentIdx, afxNat childIdx)
{
    afxError err = AFX_ERR_NONE;
    //AfxAssertType(xml, afxFcc_XML);
    AfxAssertRange(xml->elemCnt, parentIdx, 1);
    afxXmlElement const* parent = &xml->elems[parentIdx];
    AfxAssertRange(parent->childCnt, childIdx, 1);
    return &xml->elems[parent->baseChildIdx + childIdx];
}

_AFX afxXmlElement const* AfxGetXmlElementData(afxXml const* xml, afxNat parentIdx, afxNat childIdx)
{
    afxError err = AFX_ERR_NONE;
    //AfxAssertType(xml, afxFcc_XML);
    AfxAssertRange(xml->elemCnt, parentIdx, 1);
    afxXmlElement const* parent = &xml->elems[parentIdx];
    AfxAssertRange(parent->childCnt, childIdx, 1);
    return &xml->elems[parent->baseChildIdx + childIdx];
}

_AFX afxNat AfxGetXmlChild(afxXml const* xml, afxNat elemIdx, afxNat childIdx)
{
    afxError err = AFX_ERR_NONE;
    //AfxAssertType(xml, afxFcc_XML);
    AfxAssertRange(xml->elemCnt, elemIdx, 1);
    afxXmlElement const* parent = &xml->elems[elemIdx];
    AfxAssertRange(parent->childCnt, childIdx, 1);
    return parent->baseChildIdx + childIdx;
}

_AFX void AfxQueryXmlElement(afxXml const* xml, afxNat elemIdx, afxString* name, afxString* content)
{
    afxError err = AFX_ERR_NONE;
    //AfxAssertType(xml, afxFcc_XML);
    AfxAssertRange(xml->elemCnt, elemIdx, 1);
    afxXmlElement const* elem = &xml->elems[elemIdx];

    if (name)
        AfxReflectString(&elem->name, name);

    if (content)
        AfxReflectString(&elem->content, content);
}

_AFX void AfxQueryXmlTag(afxXml const* xml, afxNat elemIdx, afxNat tagIdx, afxString* name, afxString* content)
{
    afxError err = AFX_ERR_NONE;
    //AfxAssertType(xml, afxFcc_XML);
    AfxAssertRange(xml->elemCnt, elemIdx, 1);
    afxXmlElement const* elem = &xml->elems[elemIdx];
    AfxAssertRange(elem->tagCnt, tagIdx, 1);
    afxXmlTag const* tag = &xml->tags[elem->baseTagIdx + tagIdx];

    if (name)
        AfxReflectString(&tag->name, name);

    if (content)
        AfxReflectString(&tag->content, content);
}

_AFX afxNat AfxEnumerateXmlElements(afxXml const* xml, afxNat parentIdx, afxNat first, afxNat cnt, afxNat childIdx[])
{
    afxError err = AFX_ERR_NONE;
    //AfxAssertType(xml, afxFcc_XML);
    AfxAssertRange(xml->elemCnt, parentIdx, 1);
    AfxAssert(cnt);
    
    afxNat childCnt = xml->elems[parentIdx].childCnt;
    afxNat firstChildIdx = xml->elems[parentIdx].baseChildIdx;
    afxNat foundCnt = 0;

    for (afxNat i = 0; i < childCnt; i++)
    {
        childIdx[i] = firstChildIdx + i;

        if (foundCnt == cnt)
            break;
    }
    return foundCnt;
}

_AFX afxNat AfxFindXmlElements(afxXml const* xml, afxNat parentIdx, afxString const* name, afxNat first, afxNat cnt, afxNat childIdx[])
{
    afxError err = AFX_ERR_NONE;
    //AfxAssertType(xml, afxFcc_XML);
    AfxAssertRange(xml->elemCnt, parentIdx, 1);
    AfxAssert(childIdx);
    AfxAssert(name);
    AfxAssert(cnt);

    afxNat childCnt = xml->elems[parentIdx].childCnt;
    afxNat firstChildIdx = xml->elems[parentIdx].baseChildIdx;
    afxNat foundCnt = 0;

    for (afxNat i = 0; i < childCnt; i++)
    {
        if (0 == AfxCompareString(&xml->elems[firstChildIdx + first + i].name, name))
            ++foundCnt;

        if (foundCnt == cnt)
            break;
    }
    return foundCnt;
}

_AFX afxNat AfxFindXmlTaggedElements(afxXml const* xml, afxNat parentIdx, afxNat first, afxString const* elem, afxString const* tag, afxNat cnt, afxString const value[], afxNat childIdx[])
{
    afxError err = AFX_ERR_NONE;
    //AfxAssertType(xml, afxFcc_XML);
    AfxAssertRange(xml->elemCnt, parentIdx, 1);
    AfxAssert(childIdx);
    AfxAssert(elem);
    AfxAssert(tag);
    AfxAssert(cnt);

    afxBool ignoreName = !elem || AfxStringIsEmpty(elem);
    afxBool ignoreValue = !value || AfxStringIsEmpty(value);
    afxBool ignoreTag = !tag || AfxStringIsEmpty(tag) || (value && AfxStringIsEmpty(value));
    afxXmlElement* parent = &xml->elems[parentIdx];
    afxNat childCnt = parent->childCnt;
    afxNat firstChildIdx = parent->baseChildIdx;
    afxNat foundCnt = 0;

    for (afxNat i = 0; i < childCnt; i++)
    {
        afxNat currElemIdx = firstChildIdx + first + i;
        afxXmlElement* currElem = &xml->elems[currElemIdx];

        if (!ignoreName && AfxCompareString(&currElem->name, elem))
            continue;

        if (ignoreTag)
        {
            childIdx[foundCnt] = currElemIdx;
            ++foundCnt;
        }
        else
        {
            afxNat tagCnt = currElem->tagCnt;
            afxNat firstTagIdx = currElem->baseTagIdx;

            for (afxNat j = 0; j < tagCnt; j++)
            {
                afxXmlTag* currTag = &xml->tags[firstTagIdx + j];

                if (0 == AfxCompareString(&currTag->name, tag))
                {
                    if (ignoreValue || 0 == AfxCompareString(value, &currTag->content))
                    {
                        childIdx[foundCnt] = currElemIdx;
                        ++foundCnt;
                        break; // j
                    }
                }
            }
        }

        if (foundCnt == cnt)
            break;
    }
    return foundCnt;
}


_AFX afxXmlNode const* AfxXmlNodeFindChild(afxXmlNode const *base, afxString const *child, afxString const *attr, afxString const *value)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(base);
    afxNat cnt = AfxCountXmlChildNodes(base);

    for (afxNat i = 0; i < cnt; i++)
    {
        afxXmlNode const *n = base->children[i];

        if (0 == AfxCompareString(child, &n->name))
        {
            if (!attr || AfxStringIsEmpty(attr))
                return n;

            afxNat cnt2 = AfxCountXmlAttributes(n);

            for (afxNat j = 0; j < cnt2; j++)
            {
                afxXmlAttr const *a = n->attributes[j];

                if (0 == AfxCompareString(attr, &a->name))
                {
                    if (!value || AfxStringIsEmpty(value))
                        return n;

                    if (0 == AfxCompareString(value, &a->content))
                        return n;
                }
            }
        }
    }
    return NIL;
}

_AFX void AfxCleanUpXml(afxXml* xml)
{
    afxError err = AFX_ERR_NONE;
    //AfxAssertType(xml, afxFcc_XML);

    AfxXmlNodeDelete(xml, xml->root);
}

_AFX afxError AfxLoadXml(afxXml* xml, afxUri const *uri)
{
    afxError err = AFX_ERR_NONE;

    //AfxEntry("uri:%.*s", AfxPushString(uri ? AfxGetUriString(uri) : &AFX_STR_EMPTY));
    afxStream file;

    if (!(file = AfxLoadFile(uri))) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &file, afxFcc_IOB);

        //AfxAssignTypeFcc(xml, afxFcc_XML);

        if (_AfxXmlOpen(xml, file))
            AfxThrowError();

        AfxCloseStream(file);
    }
    return err;
}
