/*
 *           ::::::::    :::::::::::    ::::::::    ::::     ::::       :::
 *          :+:    :+:       :+:       :+:    :+:   +:+:+: :+:+:+     :+: :+:
 *          +:+              +:+       +:+          +:+ +:+:+ +:+    +:+   +:+
 *          +#++:++#++       +#+       :#:          +#+  +:+  +#+   +#++:++#++:
 *                 +#+       +#+       +#+   +#+#   +#+       +#+   +#+     +#+
 *          #+#    #+#       #+#       #+#    #+#   #+#       #+#   #+#     #+#
 *           ########    ###########    ########    ###       ###   ###     ###
 *
 *                     S I G M A   T E C H N O L O G Y   G R O U P
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
#include "../io/afxIoDDK.h"

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
    afxByte *ptr;
};

enum xml_parser_offset {
    NO_CHARACTER = -1,
    CURRENT_CHARACTER = 0,
    NEXT_CHARACTER = 1,
};

static afxError AfxXmlNodeAttrDelete(afxXml* xml, afxXmlAttr* attr)
{
    afxError err = { 0 };
    AFX_ASSERT(attr);

    AfxPopLink(&attr->elem);

    if (AfxReclaimPoolUnits(&xml->attrs, AfxHere(), 1, (void**)&attr))
        AfxThrowError();

    return err;
}

static afxError AfxXmlNodeDelete(afxXml* xml, afxXmlNode* elem)
{
    afxError err = { 0 };
    AFX_ASSERT(elem);

    afxXmlAttr* a;
    AFX_ITERATE_CHAIN(a, elem, &elem->attributes)
    {
        if (AfxXmlNodeAttrDelete(xml, a))
            AfxThrowError();
    }
    AFX_ASSERT(AfxIsChainEmpty(&elem->attributes));

    afxXmlNode* c;
    AFX_ITERATE_CHAIN(c, parent, &elem->children)
    {
        if (AfxXmlNodeDelete(xml, c))
            AfxThrowError();
    }
    AFX_ASSERT(AfxIsChainEmpty(&elem->children));

    AfxPopLink(&elem->parent);

    if (AfxReclaimPoolUnits(&xml->elems, AfxHere(), 1, (void**)&elem))
        AfxThrowError();

    return err;
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
    parser->ptr = &parser->buffer[parser->position];
}

static void _AfxXmlParseSkipWhitespaces(struct xml_parser* parser) {

    while (isspace(parser->buffer[parser->position])) {
        if (parser->position + 1 >= parser->length)
        {
            parser->ptr = &parser->buffer[parser->position];
            return;
        }
        else {
            parser->position++;
        }
    }

    parser->ptr = &parser->buffer[parser->position];
}

static char* _AfxXmlNextAttrToken(char** rest)
// Replacement for the token loop that respects quoted values.
{
    if (!rest || !*rest) return NULL;

    char* p = *rest;

    // Skip leading whitespace
    while (*p && isspace(*p)) p++;

    if (*p == '\0') return NULL;

    char* start = p;
    int in_quote = 0;

    while (*p)
    {
        if (*p == '\'' || *p == '"')
        {
            if (in_quote == 0)
                in_quote = *p;  // Enter quote
            else if (in_quote == *p)
                in_quote = 0;  // Exit quote
        }
        else if (isspace(*p) && in_quote == 0)
        {
            break;  // End of token
        }
        p++;
    }

    if (*p)
    {
        *p = '\0';
        *rest = p + 1;
    }
    else
    {
        *rest = p;
    }

    return start;
}

static afxError _AfxXmlParseAttribs(afxXml* xml, afxXmlNode* elem, struct xml_parser* parser, afxString *tag_open)
{
    afxError err = { 0 };
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
    int position;

    afxString128 tmpStr;
    AfxMakeString128(&tmpStr, tag_open);
    tmp = (char*)AfxGetStringData(&tmpStr.s,0);

    rest = tmp;
    token = _AfxXmlNextAttrToken(&rest);
    //token = xml_strtok_r(tmp, " ", &rest); // skip the first value
    
    //if (token != NULL)
    {
        //tag_open->range = strlen(token);
        afxUnit trimRange = strlen(token);

        //for (token = xml_strtok_r(NULL, " ", &rest); token != NULL; token = xml_strtok_r(NULL, " ", &rest))
        while ((token = _AfxXmlNextAttrToken(&rest)) != NULL)
        {
            if (*token == '/')
            {
                AFX_ASSERT(!(*rest));
                break;
            }

            str_name;
            AfxAllocate(strlen(token) + 1, 0, AfxHere(), (void**)&str_name);
            str_content;
            AfxAllocate(strlen(token) + 1, 0, AfxHere(), (void**)&str_content);
#if 0
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
#else
#if !0
            // %s=\"%s\" wasn't working for some reason, ugly hack to make it work
            if (sscanf(token, "%[^=]=\"%[^\"]\"", str_name, str_content) != 2)
            {
                if (sscanf(token, "%[^=]='%[^']'", str_name, str_content) != 2)
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
#else
            char* eq = strchr(token, '=');
            if (eq)
            {
                *eq = '\0';
                strncpy(str_name, token, strlen(token) + 1);
                char* val = eq + 1;

                // Remove surrounding quotes if any
                if ((*val == '"' && val[strlen(val) - 1] == '"') ||
                    (*val == '\'' && val[strlen(val) - 1] == '\''))
                {
                    val[strlen(val) - 1] = '\0';
                    val++;
                }

                strncpy(str_content, val, strlen(token) + 1);
            }
            else
            {
                strncpy(str_name, token, strlen(token) + 1);
                str_content[0] = '\0';
            }

#endif
#endif

            position = token - tmp;
            start_name = &tag_open->start[position];
            start_content = &tag_open->start[position + strlen(str_name) + 2];

            afxXmlAttr* a;
            if (AfxRequestPoolUnits(&xml->attrs, AfxHere(), 1, NIL, (void**)&a))
            {
                AfxThrowError();
            }

            AfxMakeString(&a->name, 0, start_name, strlen(str_name));
            afxUnit str_content_len = strlen(str_content);
            AfxMakeString(&a->content, 0, start_content, str_content_len);
            AfxPushLink(&a->elem, &elem->attributes);
            
            AfxDeallocate((void**)&str_name, AfxHere());
            AfxDeallocate((void**)&str_content, AfxHere());
        }
        
        tag_open->len = trimRange;
    }
    return err;
}

static afxXmlNode* _AfxXmlParseNode(afxXml* xml, afxXmlNode* parent, struct xml_parser* parser)
{
    afxError err = { 0 };

    // NEW

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

    // Parse open tag
    // Parses an opening XML tag without attributes. Ex.: <TagName>

    _AfxXmlParseSkipWhitespaces(parser);

    /* Consume `<'
        */

    if (!('<' == _AfxXmlParsePeek(parser, CURRENT_CHARACTER)))
    {

        AfxThrowError();
    }
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

    afxXmlNode* elem;
    if (AfxRequestPoolUnits(&xml->elems, AfxHere(), 1, NIL, (void**)&elem))
    {
        AfxThrowError();
    }

    AfxMakeChain(&elem->attributes, elem);
    AfxMakeChain(&elem->children, elem);
    AfxPushLink(&elem->parent, parent ? &parent->children : NIL);
    elem->name = AFX_STRING_EMPTY;
    elem->content = AFX_STRING_EMPTY;
    
    if (err) AfxThrowError();
    else
    {
        original_length = tag_open.len;
        _AfxXmlParseAttribs(xml, elem, parser, &tag_open);
        //original_length = tag_open.range;

        // If tag ends with '/' it's self closing, skip content lookup. Drop '/' and go to node creation

        if (!(tag_open.len > 0 && '/' == tag_open.start[original_length - 1] /**(afxChar*)AfxGetStringData(&tag_open, original_length - 1)*/))
        {
            // If the content does not start with '<', a text content is assumed

            afxBool open = FALSE;

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

                    open = TRUE;
                }
            }
#if !0
            else open = TRUE;

            if (open)
#else
            else // <
#endif
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
                        
                        afxXmlNode* child = _AfxXmlParseNode(xml, elem, parser);

                        if (!child)
                        {
                            AfxThrowError();
                            break;
                        }
                    }
                }
            }

            if (!err)
            {
                // Parse close tag
                // Parses an closing XML tag without attributes. Ex.: </TagName>
                     
                _AfxXmlParseSkipWhitespaces(parser);

                // Consume '</'

                if (!('<' == _AfxXmlParsePeek(parser, CURRENT_CHARACTER)))
                    AfxThrowError();
                else if (!('/' == _AfxXmlParsePeek(parser, NEXT_CHARACTER)))
                    AfxThrowError();
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

                    if (!AfxCompareStrings(&tag_open, 0, FALSE, 1, &tag_close, NIL))
                    {
                        AfxThrowError();
                        AfxReportError(" %.*s %.*s", AfxPushString(&tag_open), AfxPushString(&tag_close));
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

            AfxMakeString(&elem->name, 0, tag_open.start, tag_open.len);
            AfxMakeString(&elem->content, 0, content.start, content.len);
            
            //AfxReportComment("%u/%u; %u/%u; %u/%u; %.*s : %.8s;", elemIdx, elem->parentIdx, elem->baseTagIdx, elem->tagCnt, elem->baseChildIdx, elem->childCnt, AfxPushString(&elem->name), 0);

            return elem;
        }
    }

    /* A failure occured, so free mem allocalted resources
     */
    if (err)
    {
        AfxXmlNodeDelete(xml, elem);

        if (AfxReclaimPoolUnits(&xml->elems, AfxHere(), 1, (void**)&elem))
            AfxThrowError();
    }
    return 0;
}

_AFX afxError AfxParseXml(afxXml* xml, void* buffer, afxUnit length)
{
    afxError err = { 0 };
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
        /* Parse the root node
         */
        afxXmlNode *root = _AfxXmlParseNode(xml, NIL, &parser);

        if (!root)
        {
            AfxThrowError();
        }
        else
        {
            /* Return parsed document
             */

            xml->root = root;
#if 0
            afxUnit elemCnt = xml->tempElemArr.pop;
            xml->elems = xml->tempElemArr.items;

            for (afxUnit i = 0; i < elemCnt; i++)
            {
                afxXmlElement*elem = &xml->elems[i];//AfxGetArrayUnit(&xml->tempElemArr, i);
                //AfxReportComment("%u/%u; %u/%u; %u/%u; %.*s : %.8s;", i, elem->parentIdx, elem->baseTagIdx, elem->tagCnt, elem->baseChildIdx, elem->childCnt, AfxPushString(&elem->name), 0);
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
                        afxXmlElement* elem2 = AfxPushArrayUnits(&sorted, 1, &idx, NIL, 0);
                        indicesMap[i] = idx;
                        //AFX_ASSERT(idx == i);
                        elem2->name = elem->name;
                        elem2->content = elem->content;
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
            xml->elems = sorted.items;

#endif
#if 0
            for (afxUnit i = 0; i < elemCnt; i++)
            {
                afxXmlElement*elem = &xml->elems[i];//AfxGetArrayUnit(&xml->tempElemArr, i);
                AfxReportMessage("# %u, @ %u, [ %u, %u ], [ %u, %u ], %.*s : %.8s;", i, elem->parentIdx, elem->baseTagIdx, elem->tagCnt, elem->baseChildIdx, elem->childCnt, AfxPushString(&elem->name), 0);
            }
#endif

            int b = 1;
        }
    }
    return err;
}

afxError _AfxXmlOpen(afxXml* xml, afxStream file)
{
    afxError err = { 0 };

    if (AfxParseXml(xml, AfxGetStreamBuffer(file, 0), AfxGetStreamLength(file)))
        AfxThrowError();

    return err;
}

_AFX afxXmlNode* AfxGetXmlRoot(afxXml* xml)
{
    afxError err = { 0 };
    //AFX_ASSERT(xml->fcc == afxFcc_XML);
    return xml->root;
}

_AFX afxString const* AfxGetXmlNodeName(afxXmlNode const *node)
{
    afxError err = { 0 };
    AFX_ASSERT(node);
    return &node->name;
}

_AFX afxString const* AfxGetXmlNodeContent(afxXmlNode const *node)
{
    afxError err = { 0 };
    AFX_ASSERT(node);
    return &node->content;
}

_AFX afxUnit AfxCountXmlChildNodes(afxXmlNode const *node)
{
    afxError err = { 0 };
    AFX_ASSERT(node);
    return node->children.cnt;
}

_AFX afxXmlNode const* AfxGetXmlChildNode(afxXmlNode const *node, afxUnit idx)
{
    afxError err = { 0 };
    AFX_ASSERT(node);

    if (idx >= AfxCountXmlChildNodes(node))
    {
        return 0;
    }

    if (idx >= node->attributes.cnt)
    {
        return 0;
    }

    afxUnit i = 0;
    afxXmlNode const *c;
    AFX_ITERATE_CHAIN_B2F(c, parent, &node->children)
    {
        if (idx == i)
            return c;

        ++i;
    }
    return NIL;
}

_AFX afxUnit AfxCountXmlAttributes(afxXmlNode const *node)
{
    afxError err = { 0 };
    AFX_ASSERT(node);
    return node->attributes.cnt;
}

_AFX afxString const* AfxXmlNodeGetAttributeName(afxXmlNode const *node, afxUnit idx)
{
    afxError err = { 0 };
    AFX_ASSERT(node);

    if (idx >= node->attributes.cnt)
    {
        return 0;
    }

    afxUnit i = 0;
    afxXmlAttr const *a;
    AFX_ITERATE_CHAIN_B2F(a, elem, &node->attributes)
    {
        if (idx == i)
            return &a->name;

        ++i;
    }
    return NIL;
}

_AFX afxString const* AfxXmlNodeGetAttributeContent(afxXmlNode const *node, afxUnit idx)
{
    if (idx >= node->attributes.cnt)
    {
        return 0;
    }

    afxUnit i = 0;
    afxXmlAttr const *a;
    AFX_ITERATE_CHAIN_B2F(a, elem, &node->attributes)
    {
        if (idx == i)
            return &a->content;

        ++i;
    }
    return NIL;
}

_AFX afxBool AfxXmlNodeHasAttribute(afxXmlNode const* node, afxString const* key, afxBool ci)
{
    afxError err = { 0 };
    AFX_ASSERT(node);
    AFX_ASSERT(key);

    afxXmlAttr const *a;
    AFX_ITERATE_CHAIN_B2F(a, elem, &node->attributes)
    {
        if (AfxCompareStrings(key, 0, ci, 1, &a->name, NIL))
            return TRUE;
    }
    return FALSE;
}

_AFX afxString const* AfxFindXmlAttribute(afxXmlNode const* node, afxString const* key, afxBool ci)
{
    afxError err = { 0 };
    AFX_ASSERT(node);
    AFX_ASSERT(key);

    afxXmlAttr const *a;
    AFX_ITERATE_CHAIN_B2F(a, elem, &node->attributes)
    {
        if (AfxCompareStrings(key, 0, ci, 1, &a->name, NIL))
            return &a->content;
    }
    return NIL;
}

_AFX afxBool AfxExtractXmlAttributeI32(afxXmlNode const* node, afxString const* key, afxBool ci, afxInt32* value)
{
    afxError err = { 0 };
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
    afxError err = { 0 };
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
    afxError err = { 0 };
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
    afxError err = { 0 };
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
    afxError err = { 0 };
    //AfxAssertType(xml, afxFcc_XML);
    return (AfxCompareStrings(&(xml->root->name), 0, FALSE, 1, name, NIL));
}

#if 0
_AFX afxBool AfxIndexXmlElements(afxXml const* xml, afxUnit parentIdx, afxUnit base, afxUnit cnt, afxString const names[], afxUnit* nameIdx)
{
    afxError err = { 0 };
    //AfxAssertType(xml, afxFcc_XML);
    AFX_ASSERT_RANGE(xml->elemCnt, parentIdx, 1);
    AFX_ASSERT(names);
    AFX_ASSERT(cnt);
    afxBool rslt = FALSE;
    afxXmlElement const* parent = &xml->elems[parentIdx];

    for (afxUnit i = 0; i < cnt; i++)
    {
        if (AfxCompareStrings(&xml->elems[parent->baseChildIdx].name, 0, FALSE, 1, &names[i], NIL))
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
    afxError err = { 0 };
    //AfxAssertType(xml, afxFcc_XML);
    AFX_ASSERT_RANGE(xml->elemCnt, elemIdx, 1);
    AFX_ASSERT(names);
    AFX_ASSERT(cnt);
    afxBool rslt = FALSE;

    for (afxUnit i = 0; i < cnt; i++)
    {
        if (AfxCompareStrings(&xml->elems[elemIdx].name, 0, FALSE, 1, &names[i], NIL))
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
    afxError err = { 0 };
    //AfxAssertType(xml, afxFcc_XML);
    AFX_ASSERT_RANGE(xml->elemCnt, elemIdx, 1);
    return xml->elems[elemIdx].childCnt;
}

_AFX afxUnit AfxCountXmlTags(afxXml const* xml, afxUnit elemIdx)
{
    afxError err = { 0 };
    //AfxAssertType(xml, afxFcc_XML);
    AFX_ASSERT_RANGE(xml->elemCnt, elemIdx, 1);
    return xml->elems[elemIdx].tagCnt;
}

_AFX afxXmlAttr* AfxGetXmlTagMutableData(afxXml* xml, afxUnit parentIdx, afxUnit childIdx, afxUnit tagIdx)
{
    afxError err = { 0 };
    //AfxAssertType(xml, afxFcc_XML);
    AFX_ASSERT_RANGE(xml->elemCnt, parentIdx, 1);
    afxXmlElement const* parent = &xml->elems[parentIdx];
    AFX_ASSERT_RANGE(parent->childCnt, childIdx, 1);
    afxXmlElement const* child = &xml->elems[parent->baseChildIdx + childIdx];
    AFX_ASSERT_RANGE(child->tagCnt, tagIdx, 1);
    return &xml->tags[child->baseTagIdx + tagIdx];
}

_AFX afxXmlAttr const* AfxGetXmlTagData(afxXml const* xml, afxUnit parentIdx, afxUnit childIdx, afxUnit tagIdx)
{
    afxError err = { 0 };
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
    afxError err = { 0 };
    //AfxAssertType(xml, afxFcc_XML);
    AFX_ASSERT_RANGE(xml->elemCnt, elemIdx, 1);
    afxXmlElement const* elem = &xml->elems[elemIdx];
    AFX_ASSERT_RANGE(elem->tagCnt, tagIdx, 1);
    return elem->baseTagIdx + tagIdx;
}

_AFX afxXmlElement* AfxGetXmlElementMutableData(afxXml* xml, afxUnit parentIdx, afxUnit childIdx)
{
    afxError err = { 0 };
    //AfxAssertType(xml, afxFcc_XML);
    AFX_ASSERT_RANGE(xml->elemCnt, parentIdx, 1);
    afxXmlElement const* parent = &xml->elems[parentIdx];
    AFX_ASSERT_RANGE(parent->childCnt, childIdx, 1);
    return &xml->elems[parent->baseChildIdx + childIdx];
}

_AFX afxXmlElement const* AfxGetXmlElementData(afxXml const* xml, afxUnit parentIdx, afxUnit childIdx)
{
    afxError err = { 0 };
    //AfxAssertType(xml, afxFcc_XML);
    AFX_ASSERT_RANGE(xml->elemCnt, parentIdx, 1);
    afxXmlElement const* parent = &xml->elems[parentIdx];
    AFX_ASSERT_RANGE(parent->childCnt, childIdx, 1);
    return &xml->elems[parent->baseChildIdx + childIdx];
}

_AFX afxUnit AfxGetXmlChild(afxXml const* xml, afxUnit elemIdx, afxUnit childIdx)
{
    afxError err = { 0 };
    //AfxAssertType(xml, afxFcc_XML);
    AFX_ASSERT_RANGE(xml->elemCnt, elemIdx, 1);
    afxXmlElement const* parent = &xml->elems[elemIdx];
    AFX_ASSERT_RANGE(parent->childCnt, childIdx, 1);
    return parent->baseChildIdx + childIdx;
}

_AFX void AfxQueryXmlElement(afxXml const* xml, afxUnit elemIdx, afxString* name, afxString* content)
{
    afxError err = { 0 };
    //AfxAssertType(xml, afxFcc_XML);
    AFX_ASSERT_RANGE(xml->elemCnt, elemIdx, 1);
    afxXmlElement const* elem = &xml->elems[elemIdx];

    if (name)
        *name = elem->name;

    if (content)
        *content = elem->content;
}

_AFX void AfxQueryXmlAttribute(afxXml const* xml, afxUnit elemIdx, afxUnit tagIdx, afxString* name, afxString* content)
{
    afxError err = { 0 };
    //AfxAssertType(xml, afxFcc_XML);
    AFX_ASSERT_RANGE(xml->elemCnt, elemIdx, 1);
    afxXmlElement const* elem = &xml->elems[elemIdx];
    AFX_ASSERT_RANGE(elem->tagCnt, tagIdx, 1);
    afxXmlAttr const* tag = &xml->tags[elem->baseTagIdx + tagIdx];

    if (name)
        *name = tag->name;

    if (content)
        *content = tag->content;
}

_AFX afxUnit AfxEnumerateXmlChildElements(afxXml const* xml, afxUnit parentIdx, afxUnit first, afxUnit cnt, afxUnit childIdx[])
{
    afxError err = { 0 };
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
#endif

_AFX afxXmlNode const* AfxFindXmlChildElement(afxXmlNode const* parent, afxXmlNode const* last, afxString const* tag)
{
    afxError err = { 0 };
    //AfxAssertType(xml, afxFcc_XML);
    AFX_ASSERT(parent);
    //AFX_ASSERT(childIdx); // this function is used to count the number of entries too.
    //AFX_ASSERT(name);
    //AFX_ASSERT(cnt);

    afxXmlNode const* ch = NIL;
    afxUnit foundCnt = 0;

    if (last)
    {
        afxLink const* prevLink = &last->parent;
        while ((prevLink = AfxGetPrevLink(prevLink)))
        {
            afxXmlNode const* prev = AFX_GET_LINKED_OBJECT(afxXmlNode const, prevLink, parent);

            if (AfxCompareStrings(&prev->name, 0, FALSE, 1, tag, NIL))
            {
                if (!foundCnt)
                    ch = prev;

                ++foundCnt;
            }
        }
    }
    else
    {
        afxXmlNode const* child;
        AFX_ITERATE_CHAIN_B2F(child, parent, &parent->children)
        {
            if (AfxCompareStrings(&child->name, 0, FALSE, 1, tag, NIL))
            {
                if (!foundCnt)
                    ch = child;

                ++foundCnt;
            }
        }
    }

    return ch;
}

_AFX afxXmlNode const* AfxFindXmlAttributedChildElement(afxXmlNode const* parent, afxXmlNode const* last, afxString const* elem, afxString const* attrKey, afxString const* attrValue)
{
    afxError err = { 0 };
    //AfxAssertType(xml, afxFcc_XML);
    AFX_ASSERT(parent);
    //AFX_ASSERT(childIdx); // this function is used to count the number of entries too.
    //AFX_ASSERT(elem);
    //AFX_ASSERT(tag);
    //AFX_ASSERT(cnt);

    afxBool ignoreName = !elem || AfxIsStringEmpty(elem);
    afxBool ignoreValue = !attrValue || AfxIsStringEmpty(attrValue);
    afxBool ignoreAttr = !attrKey || AfxIsStringEmpty(attrKey) || (attrValue && AfxIsStringEmpty(attrValue));
    
    afxXmlNode const* ch = NIL;
    afxUnit foundCnt = 0;

    if (last)
    {
        afxLink const* prevLink = &last->parent;
        while ((prevLink = AfxGetPrevLink(prevLink)))
        {
            afxXmlNode const* prev = AFX_GET_LINKED_OBJECT(afxXmlNode const, prevLink, parent);

            if (!ignoreName && !AfxCompareStrings(&prev->name, 0, FALSE, 1, elem, NIL))
                continue;

            if (ignoreAttr)
            {
                if (!foundCnt)
                    ch = prev;

                ++foundCnt;
            }
            else
            {
                afxXmlAttr const* a;
                AFX_ITERATE_CHAIN_B2F_B(a, elem, &prev->attributes)
                {
                    if (AfxCompareStrings(&a->name, 0, FALSE, 1, attrKey, NIL))
                    {
                        if (ignoreValue || AfxCompareStrings(attrValue, 0, FALSE, 1, &a->content, NIL))
                        {
                            if (!foundCnt)
                                ch = prev;
                            ++foundCnt;
                            break; // iterate attributes
                        }
                    }
                }
            }
        }
    }
    else
    {
        afxXmlNode const* child;
        AFX_ITERATE_CHAIN_B2F(child, parent, &parent->children)
        {
            if (!ignoreName && !AfxCompareStrings(&child->name, 0, FALSE, 1, elem, NIL))
                continue;

            if (ignoreAttr)
            {
                if (!foundCnt)
                    ch = child;

                ++foundCnt;
            }
            else
            {
                afxXmlAttr const* a;
                AFX_ITERATE_CHAIN_B2F_B(a, elem, &child->attributes)
                {
                    if (AfxCompareStrings(&a->name, 0, FALSE, 1, attrKey, NIL))
                    {
                        if (ignoreValue || AfxCompareStrings(attrValue, 0, FALSE, 1, &a->content, NIL))
                        {
                            if (!foundCnt)
                                ch = child;

                            ++foundCnt;
                            break; // iterate attributes
                        }
                    }
                }
            }
        }
    }
    return ch;
}

_AFX afxUnit AfxCountXmlChildElements(afxXmlNode const* parent, afxXmlNode const* last, afxString const* name)
{
    afxError err = { 0 };
    //AfxAssertType(xml, afxFcc_XML);
    AFX_ASSERT(parent);
    //AFX_ASSERT(childIdx); // this function is used to count the number of entries too.
    //AFX_ASSERT(name);
    //AFX_ASSERT(cnt);

    afxXmlNode const* ch = NIL;
    afxUnit foundCnt = 0;

    if (last)
    {
        afxLink const* prevLink = &last->parent;
        while ((prevLink = AfxGetPrevLink(prevLink)))
        {
            afxXmlNode const* prev = AFX_GET_LINKED_OBJECT(afxXmlNode const, prevLink, parent);

            if (AfxCompareStrings(&prev->name, 0, FALSE, 1, name, NIL))
            {
                if (!foundCnt)
                    ch = prev;

                ++foundCnt;
            }
        }
    }
    else
    {
        afxXmlNode const* child;
        AFX_ITERATE_CHAIN_B2F(child, parent, &parent->children)
        {
            if (AfxCompareStrings(&child->name, 0, FALSE, 1, name, NIL))
            {
                if (!foundCnt)
                    ch = child;

                ++foundCnt;
            }
        }
    }
    return foundCnt;
}

_AFX afxUnit AfxCountXmlAttributedChildElements(afxXmlNode const* parent, afxXmlNode const* last, afxString const* elem, afxString const* attrKey, afxString const* attrValue)
{
    afxError err = { 0 };
    //AfxAssertType(xml, afxFcc_XML);
    AFX_ASSERT(parent);
    //AFX_ASSERT(childIdx); // this function is used to count the number of entries too.
    //AFX_ASSERT(elem);
    //AFX_ASSERT(tag);
    //AFX_ASSERT(cnt);

    afxBool ignoreName = !elem || AfxIsStringEmpty(elem);
    afxBool ignoreValue = !attrValue || AfxIsStringEmpty(attrValue);
    afxBool ignoreAttr = !attrKey || AfxIsStringEmpty(attrKey) || (attrValue && AfxIsStringEmpty(attrValue));

    afxXmlNode const* ch = NIL;
    afxUnit foundCnt = 0;

    if (last)
    {
        afxLink const* prevLink = &last->parent;
        while ((prevLink = AfxGetPrevLink(prevLink)))
        {
            afxXmlNode const* prev = AFX_GET_LINKED_OBJECT(afxXmlNode const, prevLink, parent);

            if (!ignoreName && !AfxCompareStrings(&prev->name, 0, FALSE, 1, elem, NIL))
                continue;

            if (ignoreAttr)
            {
                if (!foundCnt)
                    ch = prev;

                ++foundCnt;
            }
            else
            {
                afxXmlAttr const* a;
                AFX_ITERATE_CHAIN_B2F_B(a, elem, &prev->attributes)
                {
                    if (AfxCompareStrings(&a->name, 0, FALSE, 1, attrKey, NIL))
                    {
                        if (ignoreValue || AfxCompareStrings(attrValue, 0, FALSE, 1, &a->content, NIL))
                        {
                            if (!foundCnt)
                                ch = prev;
                            ++foundCnt;
                            break; // iterate attributes
                        }
                    }
                }
            }
        }
    }
    else
    {
        afxXmlNode const* child;
        AFX_ITERATE_CHAIN_B2F(child, parent, &parent->children)
        {
            if (!ignoreName && !AfxCompareStrings(&child->name, 0, FALSE, 1, elem, NIL))
                continue;

            if (ignoreAttr)
            {
                if (!foundCnt)
                    ch = child;

                ++foundCnt;
            }
            else
            {
                afxXmlAttr const* a;
                AFX_ITERATE_CHAIN_B2F_B(a, elem, &child->attributes)
                {
                    if (AfxCompareStrings(&a->name, 0, FALSE, 1, attrKey, NIL))
                    {
                        if (ignoreValue || AfxCompareStrings(attrValue, 0, FALSE, 1, &a->content, NIL))
                        {
                            if (!foundCnt)
                                ch = child;

                            ++foundCnt;
                            break; // iterate attributes
                        }
                    }
                }
            }
        }
    }
    return foundCnt;
}

_AFX afxXmlNode const* AfxXmlNodeFindChild(afxXmlNode const *base, afxString const *child, afxString const *attr, afxString const *value)
{
    afxError err = { 0 };
    AFX_ASSERT(base);

    afxXmlNode const *n;
    AFX_ITERATE_CHAIN_B2F(n, parent, &base->children)
    {
        if (AfxCompareStrings(child, 0, FALSE, 1, &n->name, NIL))
        {
            if (!attr || AfxIsStringEmpty(attr))
                return n;

            afxXmlAttr const *a;
            AFX_ITERATE_CHAIN_B2F_B(a, elem, &n->attributes)
            {
                if (AfxCompareStrings(attr, 0, FALSE, 1, &a->name, NIL))
                {
                    if (!value || AfxIsStringEmpty(value))
                        return n;

                    if (AfxCompareStrings(value, 0, FALSE, 1, &a->content, NIL))
                        return n;
                }
            }
        }
    }
    return NIL;
}

_AFX void AfxCleanUpXml(afxXml* xml)
{
    afxError err = { 0 };
    //AfxAssertType(xml, afxFcc_XML);

    if (xml->root)
    {
        AfxXmlNodeDelete(xml, xml->root);
        xml->root = NIL;
    }

    AfxExhaustPool(&xml->elems, FALSE);
    AfxExhaustPool(&xml->attrs, FALSE);

    //AfxDisposeObjects(1, &xml->strb);

    if (xml->file)
        AfxResetStream(xml->file);
}

_AFX void AfxMakeXml(afxXml* xml)
{
    afxError err = { 0 };
    //AfxAssertType(xml, afxFcc_XML);

    xml->root = NIL;

    if (AfxSetUpPool(&xml->elems, sizeof(afxXmlNode), 1, 0))
        AfxThrowError();

    if (AfxSetUpPool(&xml->attrs, sizeof(afxXmlAttr), 1, 0))
        AfxThrowError();

    //AfxDisposeObjects(1, &xml->strb);

    xml->file = NIL;
}

_AFX afxError AfxLoadXml(afxXml* xml, afxUri const *uri)
{
    afxError err = { 0 };
    *xml = (afxXml) { 0 };

    AfxMakeXml(xml);

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

    if (err)
        AfxCleanUpXml(xml);

    return err;
}
