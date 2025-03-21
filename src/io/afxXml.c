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
#include "../impl/afxIoImplKit.h"

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
    AFX_ASSERT(attribute);

    AfxDeallocate((void**)&attribute, AfxHere());
}

static void AfxXmlNodeDelete(afxXml* xml, afxXmlNode *node)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(node);

    afxXmlAttr** at = node->attributes;
    while (*at)
    {
        AfxXmlNodeAttrDelete(xml, *at);
        ++at;
    }

    AfxDeallocate((void**)&node->attributes, AfxHere());

    afxXmlNode** it = node->children;

    while (*it)
    {
        AfxXmlNodeDelete(xml, *it);
        ++it;
    }

    AfxDeallocate((void**)&node->children, AfxHere());

    AfxDeallocate((void**)&node, AfxHere());
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

static afxXmlAttr** _AfxXmlParseAttribs(afxXml* xml, struct xml_parser* parser, afxString *tag_open, afxUnit* baseTagIdx, afxUnit* tagCnt)
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

    AfxCoallocate(1, sizeof(afxXmlAttr*), 0, AfxHere(), (void**)&attributes);
    attributes[0] = 0;

    afxString128 tmpStr;
    AfxMakeString128(&tmpStr, tag_open);
    tmp = (char*)AfxGetStringData(&tmpStr.str,0);

    token = xml_strtok_r(tmp, " ", &rest); // skip the first value
    
    if (token != NULL)
    {
        afxUnit baseTagIdx2 = xml->tempTagArr.pop;
        afxUnit tagCnt2 = 0;
        //tag_open->range = strlen(token);
        afxUnit trimRange = strlen(token);

        for (token = xml_strtok_r(NULL, " ", &rest); token != NULL; token = xml_strtok_r(NULL, " ", &rest))
        {
            str_name;
            AfxAllocate(strlen(token) + 1, 0, AfxHere(), (void**)&str_name);
            str_content;
            AfxAllocate(strlen(token) + 1, 0, AfxHere(), (void**)&str_content);
            // %s=\"%s\" wasn't working for some reason, ugly hack to make it work
            if (sscanf(token, "%[^=]=\"%[^\"]", str_name, str_content) != 2)
            {
                if (sscanf(token, "%[^=]=\'%[^\']", str_name, str_content) != 2)
                {
                    if (sscanf(token, "%[^=]", str_name, str_content) != 1)
                    {
                        AfxDeallocate((void**)&str_name, AfxHere());
                        AfxDeallocate((void**)&str_content, AfxHere());
                        continue;
                    }
                    else *str_content = '\0';
                }
            }
            position = token - tmp;
            start_name = &tag_open->start[position];
            start_content = &tag_open->start[position + strlen(str_name) + 2];

            new_attribute;
            AfxAllocate(sizeof(afxXmlAttr), 0, AfxHere(), (void**)&new_attribute);
            AfxMakeString(&new_attribute->name, 0, start_name, strlen(str_name));
            AfxMakeString(&new_attribute->content, 0, start_content, strlen(str_content));

            old_elements = get_zero_terminated_array_attributes(attributes);
            new_elements = old_elements + 1;
            AfxReallocate(sizeof(struct xml_attributes*) * (new_elements + 1), 0, AfxHere(), (void**)&attributes);

            attributes[new_elements - 1] = new_attribute;
            attributes[new_elements] = 0;

            afxUnit addedIdx;
            afxXmlTag* tag = AfxPushArrayUnits(&xml->tempTagArr, 1, &addedIdx, NIL);
            AFX_ASSERT(addedIdx == baseTagIdx2 + tagCnt2);
            AfxMakeString(&tag->name, 0, start_name, strlen(str_name));
            AfxMakeString(&tag->content, 0, start_content, strlen(str_content));
            ++tagCnt2;
            

            AfxDeallocate((void**)&str_name, AfxHere());
            AfxDeallocate((void**)&str_content, AfxHere());
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

static afxXmlNode* _AfxXmlParseNode(afxXml* xml, afxUnit parentIdx, struct xml_parser* parser)
{
    afxError err = AFX_ERR_NONE;

    // NEW

    afxUnit elemIdx;
    afxUnit baseTagIdx = 0;
    afxUnit tagCnt = 0;
    afxUnit baseChildIdx = 0;
    afxUnit childCnt = 0;
    AfxPushArrayUnits(&xml->tempElemArr, 1, &elemIdx, NIL);
    baseChildIdx = xml->tempElemArr.pop;

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
    AfxResetStrings(1, &tag_open);
    AfxResetStrings(1, &tag_close);
    AfxResetStrings(1, &content);

    size_t original_length;
    afxXmlAttr** attributes;

    afxXmlNode** children;
    AfxCoallocate(1, sizeof(afxXmlNode*), 0, AfxHere(), (void**)&children);
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
            AfxMakeString(&tag_open, 0, &parser->buffer[start], length);

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

        if (!(tag_open.len > 0 && '/' == tag_open.start[original_length - 1] /**(afxChar*)AfxGetStringData(&tag_open, original_length - 1)*/))
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
                    AfxMakeString(&content, 0, &parser->buffer[start], length);
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

                            AfxMakeString(&content, 0, &parser->buffer[start2], length2);
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
                        AfxReallocate(sizeof(afxXmlNode*) * (new_elements + 1), 0, AfxHere(), (void**)&children);

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

                        AfxMakeString(&tag_close, 0, &parser->buffer[start], length);
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

                    if (AFX_INVALID_INDEX == AfxCompareStrings(&tag_open, 0, FALSE, 1, &tag_close))
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

            afxXmlNode* node;
            AfxAllocate(sizeof(afxXmlNode), 0, AfxHere(), (void**)&node);
            AfxMakeString(&node->name, 0, tag_open.start, tag_open.len);
            AfxMakeString(&node->content, 0, content.start, content.len);
            node->attributes = attributes;
            node->children = children;

            // NEW
            
            afxXmlElement* elem = AfxGetArrayUnit(&xml->tempElemArr, elemIdx);
            elem->parentIdx = parentIdx;
            elem->baseTagIdx = tagCnt ? baseTagIdx : 0;
            elem->tagCnt = tagCnt;
            elem->baseChildIdx = childCnt ? baseChildIdx : 0;
            elem->childCnt = childCnt;
            AfxMakeString(&elem->name, 0, tag_open.start, tag_open.len);
            AfxMakeString(&elem->content, 0, content.start, content.len);

            //AfxLogComment("%u/%u; %u/%u; %u/%u; %.*s : %.8s;", elemIdx, elem->parentIdx, elem->baseTagIdx, elem->tagCnt, elem->baseChildIdx, elem->childCnt, AfxPushString(&elem->name), 0);

            
            AFX_ASSERT(!tagCnt || baseTagIdx == elem->baseTagIdx);
            AFX_ASSERT(!childCnt || baseChildIdx == elem->baseChildIdx);

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
        AfxDeallocate((void**)&children, AfxHere());
    }
    return 0;
}

_AFX afxError AfxParseXml(afxXml* xml, void* buffer, afxUnit length)
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
        AfxMakeArray(&xml->tempElemArr, sizeof(afxXmlElement), 100, NIL, 0);
        xml->tempTagArr;
        AfxMakeArray(&xml->tempTagArr, sizeof(afxXmlTag), 100, NIL, 0);

        /* Parse the root node
         */
        afxXmlNode *root = _AfxXmlParseNode(xml, AFX_INVALID_INDEX, &parser);

        if (!root)
        {
            AfxThrowError();

            AfxCleanUpArray(&xml->tempElemArr);
            AfxCleanUpArray(&xml->tempTagArr);
        }
        else
        {
            /* Return parsed document
             */

            xml->root = root;
            
            xml->elemCnt = xml->tempElemArr.pop;
            afxUnit elemCnt = xml->tempElemArr.pop;
            xml->tagCnt = xml->tempTagArr.pop;
            xml->tags = xml->tempTagArr.data;
            xml->elems = xml->tempElemArr.data;

            for (afxUnit i = 0; i < elemCnt; i++)
            {
                afxXmlElement*elem = &xml->elems[i];//AfxGetArrayUnit(&xml->tempElemArr, i);
                //AfxLogComment("%u/%u; %u/%u; %u/%u; %.*s : %.8s;", i, elem->parentIdx, elem->baseTagIdx, elem->tagCnt, elem->baseChildIdx, elem->childCnt, AfxPushString(&elem->name), 0);
            }
            int a = 1;

            afxArray sorted;
            AfxMakeArray(&sorted, sizeof(afxXmlElement), elemCnt, NIL, 0);
            AfxReserveArraySpace(&sorted, elemCnt);

            afxUnit* indicesMap;
            AfxAllocate(elemCnt * sizeof(indicesMap[0]), 0, AfxHere(), (void**)&indicesMap);

            afxUnit activeParentIdx = AFX_INVALID_INDEX;
            afxUnit remaningCnt = elemCnt;
            while (remaningCnt)
            {
                for (afxUnit i = 0; i < elemCnt; i++)
                {
                    afxXmlElement* elem = AfxGetArrayUnit(&xml->tempElemArr, i);

                    if (elem->parentIdx == activeParentIdx)
                    {
                        afxUnit idx;
                        afxXmlElement* elem2 = AfxPushArrayUnit(&sorted, &idx);
                        indicesMap[i] = idx;
                        //AFX_ASSERT(idx == i);
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

            for (afxUnit i = 0; i < elemCnt; i++)
            {
                afxXmlElement* old = AfxGetArrayUnit(&xml->tempElemArr, i);                
                afxXmlElement* neo = AfxGetArrayUnit(&sorted, indicesMap[i]);
                neo->baseChildIdx = indicesMap[old->baseChildIdx];
            }

            AfxDeallocate((void**)&indicesMap, AfxHere());

            xml->elemCnt = sorted.pop;
            xml->elems = sorted.data;

#if 0
            for (afxUnit i = 0; i < elemCnt; i++)
            {
                afxXmlElement*elem = &xml->elems[i];//AfxGetArrayUnit(&xml->tempElemArr, i);
                AfxLogEcho("# %u, @ %u, [ %u, %u ], [ %u, %u ], %.*s : %.8s;", i, elem->parentIdx, elem->baseTagIdx, elem->tagCnt, elem->baseChildIdx, elem->childCnt, AfxPushString(&elem->name), 0);
            }
#endif

            int b = 1;
        }
        AfxCleanUpArray(&xml->tempElemArr);
        //AfxCleanUpArray(&xml->tempTagArr);
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
    //AFX_ASSERT(xml->fcc == afxFcc_XML);
    return xml->root;
}

_AFX afxString const* AfxGetXmlNodeName(afxXmlNode const *node)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(node);
    return &node->name;
}

_AFX afxString const* AfxGetXmlNodeContent(afxXmlNode const *node)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(node);
    return &node->content;
}

_AFX afxUnit AfxCountXmlChildNodes(afxXmlNode const *node)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(node);
    return get_zero_terminated_array_nodes(node->children);
}

_AFX afxXmlNode const* AfxGetXmlChildNode(afxXmlNode const *node, afxUnit idx)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(node);

    if (idx >= AfxCountXmlChildNodes(node))
    {
        return 0;
    }

    return node->children[idx];
}

_AFX afxUnit AfxCountXmlAttributes(afxXmlNode const *node)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(node);
    return get_zero_terminated_array_attributes(node->attributes);
}

_AFX afxString* AfxXmlNodeGetAttributeName(afxXmlNode const *node, afxUnit idx)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(node);

    if (idx >= AfxCountXmlAttributes(node))
    {
        return 0;
    }

    return &node->attributes[idx]->name;
}

_AFX afxString* AfxXmlNodeGetAttributeContent(afxXmlNode const *node, afxUnit idx)
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
    AFX_ASSERT(node);
    AFX_ASSERT(key);
    afxUnit cnt = AfxCountXmlAttributes(node);

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxXmlAttr const *a = node->attributes[i];

        if (0 == AfxCompareStrings(key, 0, ci, 1, &a->name))
            return TRUE;
    }
    return FALSE;
}

_AFX afxString const* AfxFindXmlAttribute(afxXmlNode const* node, afxString const* key, afxBool ci)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(node);
    AFX_ASSERT(key);
    afxUnit cnt = AfxCountXmlAttributes(node);

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxXmlAttr const *a = node->attributes[i];

        if (0 == AfxCompareStrings(key, 0, ci, 1, &a->name))
            return &a->content;
    }
    return NIL;
}

_AFX afxBool AfxExtractXmlAttributeI32(afxXmlNode const* node, afxString const* key, afxBool ci, afxInt32* value)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(node);
    AFX_ASSERT(key);

    afxBool rslt;
    afxString const *content;

    if ((rslt = !!(content = AfxFindXmlAttribute(node, key, ci))))
    {
        AFX_ASSERT(value);
        AfxScanString(content, "%i", value);
    }
    return rslt;
}

_AFX afxBool AfxExtractXmlAttributeN32(afxXmlNode const* node, afxString const* key, afxBool ci, afxUnit32* value)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(node);
    AFX_ASSERT(key);

    afxBool rslt;
    afxString const *content;

    if ((rslt = !!(content = AfxFindXmlAttribute(node, key, ci))))
    {
        AFX_ASSERT(value);
        AfxScanString(content, "%u", value);
    }
    return rslt;
}

_AFX afxBool AfxExtractXmlAttributeR32(afxXmlNode const* node, afxString const* key, afxBool ci, afxReal32* value)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(node);
    AFX_ASSERT(key);

    afxBool rslt;
    afxString const *content;

    if ((rslt = !!(content = AfxFindXmlAttribute(node, key, ci))))
    {
        AFX_ASSERT(value);
        AfxScanString(content, "%f", value);
    }
    return rslt;
}

_AFX afxBool AfxExtractXmlAttributeR64(afxXmlNode const* node, afxString const* key, afxBool ci, afxReal64* value)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(node);
    AFX_ASSERT(key);

    afxBool rslt;
    afxString const *content;

    if ((rslt = !!(content = AfxFindXmlAttribute(node, key, ci))))
    {
        AFX_ASSERT(value);
        AfxScanString(content, "%f", value);
    }
    return rslt;
}


_AFX afxBool AfxTestXmlRoot(afxXml const* xml, afxString const* name)
{
    afxError err = AFX_ERR_NONE;
    //AfxAssertType(xml, afxFcc_XML);
    return (0 == AfxCompareStrings(&(xml->root->name), 0, FALSE, 1, name));
}

_AFX afxBool AfxIndexXmlElements(afxXml const* xml, afxUnit parentIdx, afxUnit base, afxUnit cnt, afxString const names[], afxUnit* nameIdx)
{
    afxError err = AFX_ERR_NONE;
    //AfxAssertType(xml, afxFcc_XML);
    AFX_ASSERT_RANGE(xml->elemCnt, parentIdx, 1);
    AFX_ASSERT(names);
    AFX_ASSERT(cnt);
    afxBool rslt = FALSE;
    afxXmlElement const* parent = &xml->elems[parentIdx];

    for (afxUnit i = 0; i < cnt; i++)
    {
        if (0 == AfxCompareStrings(&xml->elems[parent->baseChildIdx].name, 0, FALSE, 1, &names[i]))
        {
            if (nameIdx)
                *nameIdx = i;

            return TRUE;
        }
    }
    return rslt;
}

_AFX afxBool AfxTestXmlTag(afxXml const* xml, afxUnit elemIdx, afxUnit cnt, afxString const names[], afxUnit* nameIdx)
{
    afxError err = AFX_ERR_NONE;
    //AfxAssertType(xml, afxFcc_XML);
    AFX_ASSERT_RANGE(xml->elemCnt, elemIdx, 1);
    AFX_ASSERT(names);
    AFX_ASSERT(cnt);
    afxBool rslt = FALSE;

    for (afxUnit i = 0; i < cnt; i++)
    {
        if (0 == AfxCompareStrings(&xml->elems[elemIdx].name, 0, FALSE, 1, &names[i]))
        {
            if (nameIdx)
                *nameIdx = i;

            return TRUE;
        }
    }
    return rslt;
}

_AFX afxUnit AfxCountXmlChilds(afxXml const* xml, afxUnit elemIdx)
{
    afxError err = AFX_ERR_NONE;
    //AfxAssertType(xml, afxFcc_XML);
    AFX_ASSERT_RANGE(xml->elemCnt, elemIdx, 1);
    return xml->elems[elemIdx].childCnt;
}

_AFX afxUnit AfxCountXmlTags(afxXml const* xml, afxUnit elemIdx)
{
    afxError err = AFX_ERR_NONE;
    //AfxAssertType(xml, afxFcc_XML);
    AFX_ASSERT_RANGE(xml->elemCnt, elemIdx, 1);
    return xml->elems[elemIdx].tagCnt;
}

_AFX afxXmlTag* AfxGetXmlTagMutableData(afxXml* xml, afxUnit parentIdx, afxUnit childIdx, afxUnit tagIdx)
{
    afxError err = AFX_ERR_NONE;
    //AfxAssertType(xml, afxFcc_XML);
    AFX_ASSERT_RANGE(xml->elemCnt, parentIdx, 1);
    afxXmlElement const* parent = &xml->elems[parentIdx];
    AFX_ASSERT_RANGE(parent->childCnt, childIdx, 1);
    afxXmlElement const* child = &xml->elems[parent->baseChildIdx + childIdx];
    AFX_ASSERT_RANGE(child->tagCnt, tagIdx, 1);
    return &xml->tags[child->baseTagIdx + tagIdx];
}

_AFX afxXmlTag const* AfxGetXmlTagData(afxXml const* xml, afxUnit parentIdx, afxUnit childIdx, afxUnit tagIdx)
{
    afxError err = AFX_ERR_NONE;
    //AfxAssertType(xml, afxFcc_XML);
    AFX_ASSERT_RANGE(xml->elemCnt, parentIdx, 1);
    afxXmlElement const* parent = &xml->elems[parentIdx];
    AFX_ASSERT_RANGE(parent->childCnt, childIdx, 1);
    afxXmlElement const* child = &xml->elems[parent->baseChildIdx + childIdx];
    AFX_ASSERT_RANGE(child->tagCnt, tagIdx, 1);
    return &xml->tags[child->baseTagIdx + tagIdx];
}

_AFX afxUnit AfxGetXmlTag(afxXml const* xml, afxUnit elemIdx, afxUnit tagIdx)
{
    afxError err = AFX_ERR_NONE;
    //AfxAssertType(xml, afxFcc_XML);
    AFX_ASSERT_RANGE(xml->elemCnt, elemIdx, 1);
    afxXmlElement const* elem = &xml->elems[elemIdx];
    AFX_ASSERT_RANGE(elem->tagCnt, tagIdx, 1);
    return elem->baseTagIdx + tagIdx;
}

_AFX afxXmlElement* AfxGetXmlElementMutableData(afxXml* xml, afxUnit parentIdx, afxUnit childIdx)
{
    afxError err = AFX_ERR_NONE;
    //AfxAssertType(xml, afxFcc_XML);
    AFX_ASSERT_RANGE(xml->elemCnt, parentIdx, 1);
    afxXmlElement const* parent = &xml->elems[parentIdx];
    AFX_ASSERT_RANGE(parent->childCnt, childIdx, 1);
    return &xml->elems[parent->baseChildIdx + childIdx];
}

_AFX afxXmlElement const* AfxGetXmlElementData(afxXml const* xml, afxUnit parentIdx, afxUnit childIdx)
{
    afxError err = AFX_ERR_NONE;
    //AfxAssertType(xml, afxFcc_XML);
    AFX_ASSERT_RANGE(xml->elemCnt, parentIdx, 1);
    afxXmlElement const* parent = &xml->elems[parentIdx];
    AFX_ASSERT_RANGE(parent->childCnt, childIdx, 1);
    return &xml->elems[parent->baseChildIdx + childIdx];
}

_AFX afxUnit AfxGetXmlChild(afxXml const* xml, afxUnit elemIdx, afxUnit childIdx)
{
    afxError err = AFX_ERR_NONE;
    //AfxAssertType(xml, afxFcc_XML);
    AFX_ASSERT_RANGE(xml->elemCnt, elemIdx, 1);
    afxXmlElement const* parent = &xml->elems[elemIdx];
    AFX_ASSERT_RANGE(parent->childCnt, childIdx, 1);
    return parent->baseChildIdx + childIdx;
}

_AFX void AfxQueryXmlElement(afxXml const* xml, afxUnit elemIdx, afxString* name, afxString* content)
{
    afxError err = AFX_ERR_NONE;
    //AfxAssertType(xml, afxFcc_XML);
    AFX_ASSERT_RANGE(xml->elemCnt, elemIdx, 1);
    afxXmlElement const* elem = &xml->elems[elemIdx];

    if (name)
        AfxReflectString(&elem->name, name);

    if (content)
        AfxReflectString(&elem->content, content);
}

_AFX void AfxQueryXmlTag(afxXml const* xml, afxUnit elemIdx, afxUnit tagIdx, afxString* name, afxString* content)
{
    afxError err = AFX_ERR_NONE;
    //AfxAssertType(xml, afxFcc_XML);
    AFX_ASSERT_RANGE(xml->elemCnt, elemIdx, 1);
    afxXmlElement const* elem = &xml->elems[elemIdx];
    AFX_ASSERT_RANGE(elem->tagCnt, tagIdx, 1);
    afxXmlTag const* tag = &xml->tags[elem->baseTagIdx + tagIdx];

    if (name)
        AfxReflectString(&tag->name, name);

    if (content)
        AfxReflectString(&tag->content, content);
}

_AFX afxUnit AfxEnumerateXmlElements(afxXml const* xml, afxUnit parentIdx, afxUnit first, afxUnit cnt, afxUnit childIdx[])
{
    afxError err = AFX_ERR_NONE;
    //AfxAssertType(xml, afxFcc_XML);
    AFX_ASSERT_RANGE(xml->elemCnt, parentIdx, 1);
    AFX_ASSERT(cnt);
    
    afxUnit childCnt = xml->elems[parentIdx].childCnt;
    afxUnit firstChildIdx = xml->elems[parentIdx].baseChildIdx;
    afxUnit foundCnt = 0;

    for (afxUnit i = 0; i < childCnt; i++)
    {
        childIdx[i] = firstChildIdx + i;

        if (foundCnt == cnt)
            break;
    }
    return foundCnt;
}

_AFX afxUnit AfxFindXmlElements(afxXml const* xml, afxUnit parentIdx, afxString const* name, afxUnit first, afxUnit cnt, afxUnit childIdx[])
{
    afxError err = AFX_ERR_NONE;
    //AfxAssertType(xml, afxFcc_XML);
    AFX_ASSERT_RANGE(xml->elemCnt, parentIdx, 1);
    //AFX_ASSERT(childIdx); // this function is used to count the number of entries too.
    //AFX_ASSERT(name);
    //AFX_ASSERT(cnt);

    afxUnit childCnt = xml->elems[parentIdx].childCnt;
    afxUnit firstChildIdx = xml->elems[parentIdx].baseChildIdx;
    afxUnit foundCnt = 0;

    for (afxUnit i = 0; i < childCnt; i++)
    {
        if (0 == AfxCompareStrings(&xml->elems[firstChildIdx + first + i].name, 0, FALSE, 1, name))
            ++foundCnt;

        if (cnt && foundCnt == cnt)
            break;
    }
    return foundCnt;
}

_AFX afxUnit AfxFindXmlTaggedElements(afxXml const* xml, afxUnit parentIdx, afxUnit first, afxString const* elem, afxString const* tag, afxUnit cnt, afxString const value[], afxUnit childIdx[])
{
    afxError err = AFX_ERR_NONE;
    //AfxAssertType(xml, afxFcc_XML);
    AFX_ASSERT_RANGE(xml->elemCnt, parentIdx, 1);
    //AFX_ASSERT(childIdx); // this function is used to count the number of entries too.
    //AFX_ASSERT(elem);
    //AFX_ASSERT(tag);
    //AFX_ASSERT(cnt);

    afxBool ignoreName = !elem || AfxIsStringEmpty(elem);
    afxBool ignoreValue = !value || AfxIsStringEmpty(value);
    afxBool ignoreTag = !tag || AfxIsStringEmpty(tag) || (value && AfxIsStringEmpty(value));
    afxXmlElement* parent = &xml->elems[parentIdx];
    afxUnit childCnt = parent->childCnt;
    afxUnit firstChildIdx = parent->baseChildIdx;
    afxUnit foundCnt = 0;

    for (afxUnit i = 0; i < childCnt; i++)
    {
        afxUnit currElemIdx = firstChildIdx + first + i;
        afxXmlElement* currElem = &xml->elems[currElemIdx];

        if (!ignoreName && 0 != AfxCompareStrings(&currElem->name, 0, FALSE, 1, elem))
            continue;

        if (ignoreTag)
        {
            if (childIdx)
                childIdx[foundCnt] = currElemIdx;

            ++foundCnt;
        }
        else
        {
            afxUnit tagCnt = currElem->tagCnt;
            afxUnit firstTagIdx = currElem->baseTagIdx;

            for (afxUnit j = 0; j < tagCnt; j++)
            {
                afxXmlTag* currTag = &xml->tags[firstTagIdx + j];

                if (0 == AfxCompareStrings(&currTag->name, 0, FALSE, 1, tag))
                {
                    if (ignoreValue || 0 == AfxCompareStrings(value, 0, FALSE, 1, &currTag->content))
                    {
                        if (childIdx)
                            childIdx[foundCnt] = currElemIdx;

                        ++foundCnt;
                        break; // j
                    }
                }
            }
        }

        if (cnt && foundCnt == cnt)
            break;
    }
    return foundCnt;
}


_AFX afxXmlNode const* AfxXmlNodeFindChild(afxXmlNode const *base, afxString const *child, afxString const *attr, afxString const *value)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(base);
    afxUnit cnt = AfxCountXmlChildNodes(base);

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxXmlNode const *n = base->children[i];

        if (0 == AfxCompareStrings(child, 0, FALSE, 1, &n->name))
        {
            if (!attr || AfxIsStringEmpty(attr))
                return n;

            afxUnit cnt2 = AfxCountXmlAttributes(n);

            for (afxUnit j = 0; j < cnt2; j++)
            {
                afxXmlAttr const *a = n->attributes[j];

                if (0 == AfxCompareStrings(attr, 0, FALSE, 1, &a->name))
                {
                    if (!value || AfxIsStringEmpty(value))
                        return n;

                    if (0 == AfxCompareStrings(value, 0, FALSE, 1, &a->content))
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

    if (xml->elems)
    {
        afxArray arr;
        AfxMakeArray(&arr, sizeof(xml->elems[0]), xml->elemCnt, xml->elems, xml->elemCnt);
        AfxCleanUpArray(&arr);
    }

    if (xml->tags)
    {
        afxArray arr;
        AfxMakeArray(&arr, sizeof(xml->tags[0]), xml->tagCnt, xml->tags, xml->tagCnt);
        AfxCleanUpArray(&arr);
    }


    if (xml->file)
        AfxResetStream(xml->file);

}

_AFX afxError AfxLoadXml(afxXml* xml, afxUri const *uri)
{
    afxError err = AFX_ERR_NONE;
    *xml = (afxXml) { 0 };

    //AfxEntry("uri:%.*s", AfxPushString(uri ? AfxGetUriString(uri) : &AFX_STRING_EMPTY));
    afxStream file;
    afxStreamInfo iobi = { 0 };
    iobi.usage = afxIoUsage_FILE;
    iobi.flags = afxIoFlag_READABLE;
    AfxAcquireStream(1, &iobi, &file);

    if (AfxReloadFile(file, uri)) AfxThrowError();
    else
    {
        AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &file);

        //AfxAssignTypeFcc(xml, afxFcc_XML);

        if (_AfxXmlOpen(xml, file))
            AfxThrowError();

        xml->file = (afxStream)file;
    }
    return err;
}
