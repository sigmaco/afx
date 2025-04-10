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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "../impl/afxIoImplKit.h"

typedef enum
{
    HOST_IPV4,
    HOST_IPV6,
    HOST_DOMAIN
} host_type_t;

typedef struct _url_field
{
    host_type_t host_type;
    char const *href;
    int hrefsiz;
    char const*schema;
    int schemasiz;
    char const*username;
    int usersiz;
    char const*password;
    int passsiz;
    char const*host;
    int hostsiz;
    char const*port;
    int portsiz;
    char const*path;
    int pathsiz;
    char const*fdir;
    int fdirsiz;
    char const*fobj;
    int fobjsiz;
    char const*fext;
    int fextsiz;
    char const*query;
    int querysiz;
    char const*fragment;
    int fragsiz;
} url_field_t;

// AUXILIAR FUNCTIONS /////////////////////////////////////////////////////////

#if 0
_AFXINL int host_is_ipv4(char const*str)
{
    if (!str) return 0;
    while (*str)
    {
        if ((*str >= '0' && *str <= '9') || *str == '.')
            str++;
        else
            return 0;
    }
    return 1;
}

_AFXINL char * strndup(const char *a, unsigned long long c)
{
    (void)a;
    (void)c;
    return NIL;
}

_AFXINL void parse_query(url_field_t *url, char *query)
{
    int length;
    int offset;
    char *chr;
    length = strlen(query);
    offset = 0;
    chr = strchr(query, '=');
    while (chr)
    {
        if (url->query)
            url->query = realloc(url->query, (url->query_num + 1) * sizeof(*url->query));
        else
            url->query = malloc(sizeof(*url->query));
        url->query[url->query_num].name = strndup(query, chr - query);
        query = chr + 1;
        chr = strchr(query, '&');
        if (chr)
        {
            url->query[url->query_num].value = strndup(query, chr - query);
            url->query_num++;
            query = chr + 1;
            chr = strchr(query, '=');
        }
        else
        {
            url->query[url->query_num].value = strndup(query, -1);
            url->query_num++;
            break;
        }
    }
}
#endif

_AFXINL afxResult _AfxUrlParseRaw(const char *str, afxUnit strLenTotal, url_field_t *url)
{
    afxUnit strLen = strLenTotal;
    const char *pch, *start = str, *end = str + strLen;
    //*url = (url_field_t) { 0 };

    if (str && str[0] && strLen)
    {
        url->href = str;
        url->hrefsiz = strLen;// strlen(str);

        pch = memchr(str, ':', (end - str));   /* parse schema */

        if (pch && pch[1] == '/' && pch[2] == '/')
        {
            url->schema = str;
            url->schemasiz = pch - str;
            strLen -= url->schemasiz;
            str = pch + 3;

            pch = memchr(str, '@', (end - str));   /* parse user info */

            if (pch)
            {
                pch = memchr(str, ':', (end - str));

                if (pch)
                {
                    url->username = str;
                    url->usersiz = pch - str;
                    strLen -= url->usersiz;

                    str = pch + 1;
                    pch = memchr(str, '@', (end - str));

                    if (pch)
                    {
                        url->password = str;
                        url->passsiz = pch - str;
                        strLen -= url->passsiz;
                        str = pch + 1;
                    }
                    else goto __fail;
                }
                else goto __fail;
            }

            if (str[0] == '[')        /* parse host info */
            {
                str++;
                pch = memchr(str, ']', (end - str));

                if (pch)
                {
                    url->host = str;
                    url->hostsiz = pch - str;
                    strLen -= url->hostsiz;
                    str = pch + 1;

                    if (str[0] == ':')
                    {
                        str++;
                        pch = memchr(str, '/', (end - str));

                        if (pch)
                        {
                            url->port = str;
                            url->portsiz = pch - str;
                            strLen -= url->portsiz;
                            str = pch /*+ 1*/;
                        }
                        else
                        {
                            url->port = str;
                            url->portsiz = strLen - (str - start);// strlen(str);
                            strLen -= url->portsiz;
                            str = str + strLen - (str - start);//strlen(str);
                        }
                    }
                    url->host_type = HOST_IPV6;
                }
                else goto __fail;
            }
            else
            {
                const char *pch_slash;
                pch = memchr(str, ':', (end - str));
                pch_slash = memchr(str, '/', (end - str));

                if (pch && (!pch_slash || (pch_slash && pch < pch_slash)))
                {
                    url->host = str;
                    url->hostsiz = pch - str;
                    strLen -= url->hostsiz;
                    str = pch + 1;
                    pch = memchr(str, '/', (end - str));

                    if (pch)
                    {
                        url->port = str;
                        url->portsiz = pch - str;
                        strLen -= url->portsiz;
                        str = pch /*+ 1*/;
                    }
                    else
                    {
                        url->port = str;
                        url->portsiz = strLen - (str - start);//strlen(str);
                        strLen -= url->portsiz;
                        str = str + strLen - (str - start);//strlen(str);
                    }
                }
                else
                {
                    pch = memchr(str, '/', (end - str));

                    if (pch)
                    {
                        url->host = str;
                        url->hostsiz = pch - str;
                        strLen -= url->hostsiz;
                        str = pch/* + 1*/;
                    }
                    else
                    {
                        url->host = str;
                        url->hostsiz = strLen - (str - start);//strlen(str);
                        strLen -= url->hostsiz;
                        str = str + strLen - (str - start);//strlen(str);
                    }
                }
                url->host_type = 0;//host_is_ipv4(url->host) ? HOST_IPV4 : HOST_DOMAIN;
            }
        }

        if (str[0]) /* parse path, query and fragment */
        {
            url->path = str;
            pch = memchr(str, '?', (end - str));

            if (pch) // has query
            {
                //url->path = str;
                url->pathsiz = pch - str;
                strLen -= url->pathsiz;
                str = pch/* + 1*/;
                url->query = pch;
                pch = memchr(str, '#', (end - str));

                if (pch) // has fragment
                {
                    url->querysiz = pch - url->query;
                    strLen -= url->querysiz;
                    str = pch/* + 1*/;
                    url->fragment = pch;
                    url->fragsiz = end - url->fragment;//strlen(str);
                    strLen -= url->fragsiz;
                }
                else
                {
                    // has query but no fragment

                    //url->query = str;
                    url->querysiz = end - url->query;//strlen(str);
                    strLen -= url->querysiz;
                    //str = str + strLen - (str - start);//strlen(str);
                    url->fragment = 0;
                    url->fragsiz = 0;
                    strLen -= url->fragsiz;
                }
            }
            else
            {
                // doesnt has query
                url->query = 0;
                url->querysiz = 0;
                strLen -= url->querysiz;
                pch = memchr(str, '#', (end - str));

                if (pch) // has fragment but no query
                {
                    //url->path = str;
                    url->pathsiz = pch - str;
                    strLen -= url->pathsiz;
                    str = pch/* + 1*/;
                    url->fragment = pch;
                    url->fragsiz = end - url->fragment;//strlen(str);
                    strLen -= url->fragsiz;
                    //str = str + strLen - (str - start);//strlen(str);
                }
                else
                {
                    // doesnt has fragment nor query

                    //url->path = str;
                    url->pathsiz = (end - str);//strlen(str);
                    strLen -= url->pathsiz;
                    //str = str + strLen - (str - start);//strlen(str);
                    url->fragment = 0;
                    url->fragsiz = 0;
                }
            }

            // fdir, fname and fext
            if (url->pathsiz)
            {
                pch = url->path + url->pathsiz;

                while (url->path != pch)
                {
                    --pch;

                    if (*pch == '/' || *pch == '\\') // normalize this shit
                    {
                        // if path terminates with (back)slash, it is a directory
                        url->fdir = url->path;
                        url->fdirsiz = pch - url->fdir + 1;
                        break; // if a directory is found, there's no more chance to get a filename or file extension, so we break it here. We are looking for in reverse mode.
                    }
                }

                //pch = url->path + url->pathsiz;

                if (url->fdir)
                {
                    url->fobj = url->fdir + url->fdirsiz;
                    url->fobjsiz = (url->path + url->pathsiz) - url->fobj;
                }
                else
                {
                    pch = url->path;

                    if (*pch == '.' && url->pathsiz == 1)
                    {
                        url->fdir = pch;
                        url->fdirsiz = 1;
                        url->fobj = NIL;
                        url->fobjsiz = 0;
                    }
                    else
                    {
                        url->fobj = url->path;
                        url->fobjsiz = url->pathsiz;
                    }
                }
                 
                // ext
                if (url->fobjsiz)
                {
                    pch = url->fobj + url->fobjsiz;

                    while (url->fobj != pch)
                    {
                        --pch;

                        if (*pch == '.')
                        {
                            if (url->fobj != pch && pch != url->fobj + url->fobjsiz - 1) // dot file treatment
                            {
                                url->fext = pch;
                                url->fextsiz = (url->fobj + url->fobjsiz) - url->fext;
                                break;
                            }
                            else
                            {
                                url->fext = pch;
                                url->fextsiz = (url->fobj + url->fobjsiz) - url->fext;
                                //url->fobj = NIL;
                                //url->fobjsiz = 0;
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
    else
    {
__fail:
        return 0;
    }
    return 1;
}

////////////////////////////////////////////////////////////

#define _URI_AUTH_LEN(uri) (uri->user + uri->host + uri->port)
#define _URI_PATH_LEN(uri) (uri->root + uri->dir + uri->fname + uri->fext) 
#define _URI_REQ_LEN(uri) (_URI_PATH_LEN(uri) + uri->query) 

#define _URI_AUTH_OFF(uri) (uri->schem)
#define _URI_PATH_OFF(uri) (uri->schem + _URI_AUTH_LEN(uri)) 
#define _URI_QUERY_OFF(uri) (_URI_PATH_OFF(uri) + uri->root + uri->dir + uri->fname + uri->fext) 
#define _URI_FRAG_OFF(uri) (_URI_QUERY_OFF(uri) + uri->query) 

_AFXINL void const* AfxGetUriData(afxUri const* uri, afxUnit base)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(uri);
    return AfxGetStringData(&uri->str, base);
}

_AFXINL afxUnit AfxGetUriLength(afxUri const* uri)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(uri);
    return uri->schem + uri->user/* + uri->pass*/ + uri->host + uri->port + _URI_REQ_LEN(uri) + uri->frag;
}

_AFXINL afxUnit AfxGetPathLength(afxUri const* uri)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(uri);
    return _URI_PATH_LEN(uri);
}

_AFXINL afxUnit AfxGetUriRequestLength(afxUri const* uri)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(uri);
    return _URI_REQ_LEN(uri);
}

_AFXINL afxString const* AfxGetUriString(afxUri const* uri)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(uri);
    return &uri->str;
}

_AFXINL afxString* AfxGetUriString2(afxUri* uri)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(uri);
    return &uri->str;
}

// TESTING

_AFXINL afxBool AfxUriIsEditable(afxUri const* uri)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(uri);
    return AfxGetStringCapacity(&uri->str, 0);
}

_AFXINL afxBool AfxIsUriBlank(afxUri const* uri)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(uri);
    return 0 == (uri->schem + uri->user/* + uri->pass*/ + uri->host + uri->port + uri->root + uri->dir + uri->fname + uri->fext + uri->query + uri->frag);
}

// COMPARISON

_AFXINL afxResult AfxCompareUriString(afxUri const* uri, afxString const *str)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT2(uri, str);
    return !AfxIsAnValidIndex(AfxCompareStrings(AfxGetUriString(uri), 0, FALSE, 1, str));
}

_AFXINL afxResult AfxCompareUriStringCi(afxUri const* uri, afxString const *str)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT2(uri, str);
    return !AfxIsAnValidIndex(AfxCompareStrings(AfxGetUriString(uri), 0, TRUE, 1, str));
}

_AFXINL afxResult AfxCompareUri(afxUri const* uri, afxUri const *other)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT2(uri, other);
    return !AfxIsAnValidIndex(AfxCompareStrings(AfxGetUriString(uri), 0, FALSE, 1, AfxGetUriString(other)));
}

_AFXINL afxResult AfxCompareUriCi(afxUri const* uri, afxUri const *other)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT2(uri, other);
    return AfxCompareUriStringCi(uri, AfxGetUriString(other));
}

// CHANGE

_AFXINL void AfxResetUris(afxUnit cnt, afxUri uris[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(uris);

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxUri* uri = &uris[i];
        AfxResetStrings(1, &uri->str);
        uri->schem = 0;
        uri->user = 0;
        //uri->pass = 0;
        uri->host = 0;
        uri->port = 0;
        uri->dir = 0;
        uri->root = 0;
        uri->fname = 0;
        uri->fext = 0;
        uri->query = 0;
        uri->frag = 0;
    }
}

_AFXINL void AfxClearUris(afxUnit cnt, afxUri uris[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(uris);

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxUri* uri = &uris[i];
        AfxClearStrings(1, &uri->str);
        uri->schem = 0;
        uri->user = 0;
        //uri->pass = 0;
        uri->host = 0;
        uri->port = 0;
        uri->root = 0;
        uri->dir = 0;
        uri->fname = 0;
        uri->fext = 0;
        uri->query = 0;
        uri->frag = 0;
    }
}

_AFXINL afxUri* AfxMakeUri(afxUri* uri, afxUnit cap, void const* start, afxUnit len)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(uri);
    AfxMakeString(&uri->str, cap, (void*)start, len);
    AfxReparseUri(uri);
    int a = 1;
    return uri;
}

_AFXINL afxUri AfxMakeUri2(afxUnit cap, void const* start, afxUnit len)
{
    afxError err = AFX_ERR_NONE;
    afxUri uri;
    AfxMakeString(&uri.str, cap, (void*)start, len);
    AfxReparseUri(&uri);
    return uri;
}

_AFXINL afxUnit AfxWrapUriString(afxUri* uri, afxString const* src)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(uri);
    AfxReflectString(src, &uri->str);
    AfxReparseUri(uri);
    return AfxGetUriLength(uri);
}

_AFXINL afxUnit AfxWrapUriStringRange(afxUri* uri, afxString const* src, afxUnit offset, afxUnit len)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(uri);
    AfxMakeUri(uri, 0, AfxGetStringData(src, offset), len);
    AfxReparseUri(uri);
    return AfxGetUriLength(uri);
}

_AFXINL void AfxReplicateUri(afxUri* uri, afxUri const *in)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(uri);
    AFX_ASSERT(in);
    AfxReflectString(&in->str, &uri->str);
    uri->schem = in->schem;
    uri->user = in->user;
    //uri->pass = in->pass;
    uri->host = in->host;
    uri->port = in->port;
    uri->root = in->root;
    uri->dir = in->dir;
    uri->fname = in->fname;
    uri->fext = in->fext;
    uri->query = in->query;
    uri->frag = in->frag;
}

_AFXINL afxError AfxCopyUri(afxUri* uri, afxUri const *src)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(uri);
    AFX_ASSERT(AfxUriIsEditable(uri));
    AFX_ASSERT(src);
    afxUnit clampedOff = 0;

    if ((clampedOff = AfxCopyString(&uri->str, &src->str)))
    {
        AfxThrowError();
    }
    AfxReparseUri(uri); // Strings can have differently sized buffers. So we recalc it instead of just copy offsets and ranges.
    return err;
}

_AFXINL void AfxFormatUriArg(afxUri* uri, afxChar const *fmt, va_list args)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(uri);
    AFX_ASSERT(AfxUriIsEditable(uri));

    if (!fmt) AfxClearUris(1, uri);
    else
    {
        afxString *str = AfxGetUriString2(uri);
        AFX_ASSERT(str);
        AFX_ASSERT(fmt);
        AfxFormatStringArg(str, fmt, args);
        //AfxTransformPathString(str, FALSE);
        AfxReparseUri(uri);
        //AfxCanonicalizePath(uri, FALSE);
    }
}

_AFXINL void AfxFormatUri(afxUri* uri, afxChar const *fmt, ...)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(uri);
    AFX_ASSERT(AfxUriIsEditable(uri));

    if (!fmt) AfxClearUris(1, uri);
    else
    {
        va_list args;
        va_start(args, fmt);
        AfxFormatUriArg(uri, fmt, args);
        va_end(args);
    }
}

// EXTRACTION

_AFXINL afxUnit AfxExcerptUriSchema(afxUri const* uri, afxUri* scheme)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(uri);
    AFX_ASSERT(scheme);
    AfxResetUris(1, scheme);
    AfxExcerptString(&uri->str, 0, (scheme->schem = uri->schem), &scheme->str);
    return scheme->schem;
}

_AFXINL afxUnit AfxExcerptUriAuthority(afxUri const* uri, afxUri* user, afxUri* host, afxUri* port)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(user || host || port);
    AFX_ASSERT(uri);
    afxUnit totalLen = 0;
    afxUnit offset = 0;

    if (user)
    {
        AfxResetUris(1, user);
        offset = uri->schem;
        totalLen += uri->user;
        user->user = uri->user;
        AfxExcerptString(&uri->str, offset, totalLen, &user->str);
    }

    if (host)
    {
        if (host != user)
        {
            AfxResetUris(1, host);
            offset = uri->schem + uri->user;
            totalLen = uri->host;
        }
        else totalLen += uri->host;

        host->host = uri->host;
        AfxExcerptString(&uri->str, offset, totalLen, &host->str);
    }

    if (port)
    {
        if (port != host)
        {
            AfxResetUris(1, port);
            offset = uri->schem + uri->user + uri->host;
            totalLen = uri->port;
        }
        else totalLen += uri->port;

        port->port = uri->port;
        AfxExcerptString(&uri->str, offset, totalLen, &port->str);
    }
    return totalLen;
}

_AFXINL afxUnit AfxExcerptUriQuery(afxUri const* uri, afxBool skipSep, afxUri* query)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(uri);
    AFX_ASSERT(query);
    AfxResetUris(1, query);
    afxUnit additive = skipSep && uri->query ? 1 : 0;
    AfxExcerptString(&uri->str, _URI_QUERY_OFF(uri) + additive, (uri->query - additive), &query->str);
    return query->query;
}

_AFXINL afxUnit AfxExcerptUriFragment(afxUri const* uri, afxBool skipSep, afxUri* frag)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(uri);
    AFX_ASSERT(frag);
    AfxResetUris(1, frag);
    afxUnit additive = skipSep && uri->frag ? 1 : 0;
    AfxExcerptString(&uri->str, _URI_FRAG_OFF(uri) + additive, (uri->frag - additive), &frag->str);
    return frag->frag;
}

_AFXINL afxUnit AfxClipPathDirectory(afxUri* uri, afxUri const* from)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(from);
    AFX_ASSERT(uri);
    afxUri t;
    AfxResetUris(1, &t);
    afxUnit offset = from->schem + from->user/* + from->pass*/ + from->host + from->port + from->root;
    afxUnit len = (t.dir = from->dir);
    //afxUnit cap = offset < from->str.cap ? from->str.cap - offset : 0;
    //AfxMakeString(&dir->str, cap, len ? AfxGetStringData(&from->str.str, offset) : NIL, len);
    AfxExcerptString(&from->str, offset, len, &t.str);
    *uri = t;
    return len;
}

_AFXINL afxUnit AfxClipPathDirectory2(afxUri* uri, afxUri const* from, afxUnit seg)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(from);
    AFX_ASSERT(uri);
    afxUri t;
    AfxResetUris(1, &t);
    afxUnit offset = from->schem + from->user/* + from->pass*/ + from->host + from->port + from->root;
    afxUnit len = (t.dir = from->dir);
    //afxUnit cap = offset < from->str.cap ? from->str.cap - offset : 0;
    //AfxMakeString(&dir->str, cap, len ? AfxGetStringData(&from->str.str, offset) : NIL, len);
    
    afxChar const* p = AfxGetUriData(from, offset);
    afxChar const* p2 = p;
    afxUnit i = 0;
    afxUnit len2 = 0;
    afxChar ch;
    while ((ch = *p2++))
    {
        len2++;

        if ((ch == '/' || ch == '\\') && (++i > seg || !p2[0]))
            break;

        if (len2 > len)
            break;
    }

    AfxExcerptString(&from->str, offset, len2, &t.str);
    t.dir = len2;
    *uri = t;
    return len2;
}

_AFXINL afxUnit AfxExcerptPathBase(afxUri const* uri, afxUri* base)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(uri);
    AFX_ASSERT(base);
    afxUri t;
    AfxResetUris(1, &t);
    afxUnit offset = uri->schem + uri->user/* + from->pass*/ + uri->host + uri->port;
    afxUnit len = (t.root = uri->root) + (t.dir = uri->dir);
    //afxUnit cap = offset < from->str.cap ? from->str.cap - offset : 0;
    //AfxMakeString(&name->str, cap, len ? AfxGetStringData(&uri->str.str, offset) : NIL, len);
    AfxExcerptString(&uri->str, offset, len, &t.str);
    *base = t;
    return len;
}

_AFXINL afxUnit AfxExcerptPathSegments(afxUri const* uri, afxUri* root, afxUri* dir, afxUri* file, afxUri* ext)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(uri);
    afxUri t;
    AfxResetUris(1, &t);
    afxUnit offset = _URI_PATH_OFF(uri);
    afxUnit len = _URI_PATH_LEN(uri);

    if (root)
    {
        AfxResetUris(1, root);
        AfxExcerptString(&uri->str, _URI_PATH_OFF(uri), uri->root, &root->str);
        root->root = uri->root;
    }

    if (dir)
    {
        if (dir == root)
        {
            dir->str.len += uri->dir;
            dir->dir = uri->dir;
        }
        else
        {
            AfxResetUris(1, dir);
            AfxExcerptString(&uri->str, _URI_PATH_OFF(uri) + uri->root, uri->dir, &dir->str);
            dir->dir = uri->dir;
        }
    }

    if (file)
    {
        if (file == dir)
        {
            file->str.len += uri->fname;
            file->fname = uri->fname;
        }
        else
        {
            AfxResetUris(1, file);
            AfxExcerptString(&uri->str, _URI_PATH_OFF(uri) + uri->root + uri->dir, uri->fname, &file->str);
            file->fname = uri->fname;
        }
    }

    if (ext)
    {
        if (ext == file)
        {
            ext->str.len += uri->fext;
            ext->fext = uri->fext;
        }
        else
        {
            AfxResetUris(1, ext);
            AfxExcerptString(&uri->str, _URI_PATH_OFF(uri) + uri->root + uri->dir + uri->fname, uri->fext, &ext->str);
            ext->fext = uri->fext;
        }
    }
    return len;
}

_AFXINL afxUnit AfxSplitPath(afxUri const* uri, afxUri* root, afxUri* path)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(uri);
    afxUri t;
    AfxResetUris(1, &t);
    afxUnit offset = _URI_PATH_OFF(uri);
    afxUnit len = _URI_PATH_LEN(uri);

    if (root)
    {
        AfxResetUris(1, root);
        AfxExcerptString(&uri->str, offset, uri->root, &root->str);
        root->root = uri->root;
    }

    if (!path) len = uri->root;
    else
    {
        if (path != root)
        {
            AfxResetUris(1, path);
            len -= uri->root;
            AfxExcerptString(&uri->str, offset + uri->root, len, &path->str);
        }
        else
        {
            AfxExcerptString(&uri->str, offset, len, &path->str);
        }
        path->dir = uri->dir;
        path->fname = uri->fname;
        path->fext = uri->fext;
    }
    return len;
}

_AFXINL afxUnit AfxClipUriTarget(afxUri* uri, afxUri const* from)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(from);
    AFX_ASSERT(uri);
    afxUri t;
    AfxResetUris(1, &t);
    afxUnit offset = from->schem + from->user/* + from->pass*/ + from->host + from->port + from->root + from->dir;
    afxUnit len = (t.fname = from->fname);
    //afxUnit cap = offset < from->str.cap ? from->str.cap - offset : 0;
    //AfxMakeString(&name->str, cap, len ? AfxGetStringData(&uri->str.str, offset) : NIL, len);
    AfxExcerptString(&from->str, offset, len, &t.str);
    *uri = t;
    return len;
}

_AFXINL afxUnit AfxGetUriTargetString(afxUri const* uri, afxString* name)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(uri);
    AFX_ASSERT(name);
    afxUri tmp;
    AfxClipUriTarget(&tmp, uri);
    AfxReflectString(AfxGetUriString(&tmp), name);
    return name->len;
}

_AFXINL afxUnit AfxClipUriExtension(afxUri* uri, afxUri const* from, afxBool skipDot)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(from);
    AFX_ASSERT(uri);
    AfxResetUris(1, uri);
    afxUnit additive = skipDot && from->fext ? 1 : 0;
    afxUnit offset = from->schem + from->user/* + from->pass*/ + from->host + from->port + from->root + from->dir + from->fname + additive;
    afxUnit len = (uri->fext = from->fext - additive);
    //afxUnit cap = offset < from->str.cap ? from->str.cap - offset : 0;
    //AfxMakeString(&ext->str, cap, len ? AfxGetStringData(&from->str.str, offset) : NIL, len);
    AfxExcerptString(&from->str, offset, len, &uri->str);
    return len;
}

_AFXINL afxUnit AfxClipUriFile(afxUri* uri, afxUri const* from)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(from);
    AFX_ASSERT(uri);
    AfxResetUris(1, uri);
    afxUnit offset = from->schem + from->user/* + from->pass*/ + from->host + from->port + from->root + from->dir;
    afxUnit len = (uri->fname = from->fname) + (uri->fext = from->fext);
    //afxUnit cap = offset < from->str.cap ? from->str.cap - offset : 0;
    //AfxMakeString(&uri->str, cap, len ? AfxGetStringData(&from->str.str, offset) : NIL, len);
    AfxExcerptString(&from->str, offset, len, &uri->str);
    return len;
}

_AFXINL afxUnit AfxClipUriPath(afxUri* uri, afxUri const* from)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(from);
    AFX_ASSERT(uri);
    AfxResetUris(1, uri);
    afxUnit offset = from->schem + from->user/* + from->pass*/ + from->host + from->port;
    afxUnit len = (uri->root = from->root) + (uri->dir = from->dir) + (uri->fname = from->fname) + (uri->fext = from->fext);
    AfxExcerptString(&from->str, _URI_PATH_OFF(from), len, &uri->str);
    return len;
}

_AFX afxUnit AfxClipPath2(afxUri* uri, afxUri const* from, afxUnit seg)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(from);
    AFX_ASSERT(uri);
    afxUnit offset = _URI_PATH_OFF(from);
    afxUnit len = _URI_PATH_LEN(from);

    afxChar const* p = AfxGetUriData(from, offset);
    afxChar const* p2 = p;
    afxUnit i = 0;
    afxUnit len2 = 0;
    afxChar pch;
    while ((pch = *p2++))
    {
        len2++;

        if (((pch == '/' || pch == '\\') && (++i > seg || !p2[0])))
            break;
        
        if ((len2 >= len))
            break;
    }

    afxUri t;
    AfxResetUris(1, &t);

    if (len2)
    {
        AfxExcerptString(&from->str, offset, len2, &t.str);

        if (i)
            t.dir = len2;
        else
            t.fname = len2;
    }
    *uri = t;
    return len2;
}

_AFXINL afxUnit AfxGetUriQueryString(afxUri const* uri, afxBool skipSep, afxString* query)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(uri);
    AFX_ASSERT(query);
    afxUri tmp;
    AfxExcerptUriQuery(uri, skipSep, &tmp);
    AfxReflectString(AfxGetUriString(&tmp), query);
    return query->len;
}

_AFXINL afxUnit _AfxGetPathDriveNo_W32(afxUri const* uri)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(uri);

    afxUri path;
    AfxClipUriPath(&path, uri);
    afxByte const* p = AfxGetUriData(&path, 0);

    if (p && p[0] && p[1] == ':')
    {
        afxChar l = *p;
        
        if (*p >= 'a' && l <= 'z')
            return l - 'a';

        if (l >= 'A' && l <= 'Z')
            return l - 'A';
    }
    return AFX_INVALID_INDEX;
}

_AFXINL afxUnit AfxGetPathDriveNo(afxUri const* uri)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(uri);
    return _AfxGetPathDriveNo_W32(uri);
}

#define IsNetDrive(no_) FALSE

_AFXINL afxBool AfxPathIsNetwork_W32(afxUri const* uri)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(uri);
    afxBool rslt = FALSE;

    afxUri path;
    AfxClipUriPath(&path, uri);
    afxByte const* p = AfxGetUriData(&path, 0);

    if (p)
    {
        afxUnit no;

        if ((*p == '\\') && (p[1] == '\\') && (p[2] != '?') || (no = _AfxGetPathDriveNo_W32(uri), IsNetDrive(no)))
            rslt = TRUE;
    }
    return rslt;
}

_AFXINL afxBool _AfxPathIsRelative_W32(afxUri const* uri)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(uri);

    afxUri path;
    afxUnit len = AfxClipUriPath(&path, uri);
    afxByte const* data = AfxGetUriData(&path, 0);

    return !len || !data[0] || (data[0] != '\\' && AFX_INVALID_INDEX == AfxGetPathDriveNo(&path));
}

_AFXINL afxBool AfxPathIsRelative(afxUri const* uri)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(uri);

    afxUri path;
    afxUnit len = AfxClipUriPath(&path, uri);
    afxByte const* data = AfxGetUriData(&path, 0);

    return !len || !data[0] || (data[0] != '/' && (data[0] != '\\' && AFX_INVALID_INDEX == AfxGetPathDriveNo(&path)));
}

_AFXINL afxUnit SpanRoot(afxChar const* start, afxUnit pathLen)
{
    afxUnit rslt = 0;
    afxUnit sepCnt = 0;
    afxUnit dot = FALSE;
    afxUnit letter = FALSE;
    afxChar const* p = start;
    afxUnit i = 0;

    while (pathLen < i)
    {
        afxChar ch = *p++;

        if ((ch == '/' || ch == '\\'))
        {
            ++sepCnt;

            if (sepCnt == 1 && letter) break;
            else if (dot)
            {
                if (sepCnt == 4)
                    break;
            }
        }
        else if (i == 1 && ch == ':' && sepCnt == 0 && AfxIsalpha(start[0]))
        {
            letter = TRUE;
        }
        else if (i == 2 && ch == '.' && sepCnt == 2)
        {
            dot = TRUE;
        }
        i++;
    }

    if (dot)
        rslt += 1; // '.'
    else if (letter)
        rslt += 2; // "C:"

    rslt += sepCnt;
    
    return rslt;
}

_AFXINL afxUnit SpanRoot2(afxChar const* start, afxUnit pathLen)
{
    afxUnit rslt = 0;
    afxUnit sepCnt = 0;
    afxUnit dot = FALSE;
    afxUnit letter = FALSE;
    afxChar const* p = start;
    afxUnit i = 0;

    while (pathLen < i)
    {
        afxChar ch = *p++;

        if ((ch == '/' || ch == '\\'))
        {
            ++sepCnt;

            if (sepCnt == 1 && letter) break;
            else if (dot)
            {
                if (sepCnt == 4)
                    break;
            }
        }
        else if (i == 1 && ch == ':' && sepCnt == 0 && AfxIsalpha(start[0]))
        {
            letter = TRUE;
        }
        else if (i == 2 && ch == '.' && sepCnt == 2)
        {
            dot = TRUE;
        }
        i++;
    }

    if (dot)
        rslt += 1; // '.'
    else if (letter)
        rslt += 2; // "C:"

    rslt += sepCnt;

    return rslt;
}

_AFXINL afxChar* SkipPathRoot(afxUri const* uri)
{
    afxChar const* p = AfxGetUriData(uri, 0);
    
}

_AFXINL afxBool _AfxPathIsRoot_W32(afxUri const* uri)
{
    /*
        Returns TRUE for paths such as "", "X:" or "\\server\share". 
        Paths such as "..\path2" or "\\server" return FALSE.
    */

    afxError err = NIL;
    AFX_ASSERT(uri);
    afxBool rslt = FALSE;
    afxChar const* p = AfxGetUriData(uri, 0);

    if (p && *p)
    {
        if (AfxIsalpha(*p) && p[1] == ':' && p[2] == '\\' && !p[3])
            return TRUE;

        if (*p == '\\')
        {
            if (!p[1])
                return TRUE;

            if (p[1] == '\\')
            {
                afxChar const* v5 = (p + 2);

                if (p[2] != '?')
                {
                    afxUnit i = 0;

                    while (*v5)
                    {
                        if (*v5 == '\\' && (++i > 1 || !v5[1]))
                            return rslt;

                        v5 = v5++;
                    }
                    rslt = TRUE;
                }
            }
        }
    }
    return rslt;
}

_AFXINL afxBool AfxPathIsRoot(afxUri const* uri)
{
    afxError err = NIL;
    AFX_ASSERT(uri);
    return _AfxPathIsRoot_W32(uri);
}



_AFXINL afxBool AfxPathIsDevice(afxUri const* uri)
{
    afxError err = NIL;
    AFX_ASSERT(uri);
    afxBool rslt = FALSE;
    
    afxUri host, path, dev;
    AfxClipUriPath(&host, uri);
    AfxClipUriPath(&path, uri);
    AfxClipPathDirectory(&dev, uri);
    afxChar const* h = AfxGetUriData(&host, 0);
    afxChar const* p = AfxGetUriData(&path, 0);

    if (0 == AfxCompareUriString(&host, &AFX_STRING("//.")))
    {
        afxChar const* v5 = p;
        afxUnit i = 0;

        while (*v5)
        {
            if (*v5 == '/' && (++i > 1 || !v5[1]))
                return rslt;

            v5 = v5++;
        }
    }
    return rslt;
}

_AFXINL afxUnit AfxClipUriDevice(afxUri* uri, afxUri const* from)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(from);
    AFX_ASSERT(uri);
    afxUri t;
    AfxResetUris(1, &t);
    afxUnit len = 0;
    afxUri dev;

    afxUnit devLen = 0;

    if (!(devLen = AfxClipPathDirectory2(&dev, from, 3))) AfxResetUris(1, uri);
    else
    {
        if (!AfxIsAnValidIndex(AfxCompareSubstrings(&AFX_STRING("//./"), 0, 4, FALSE, 1, &dev.str))) AfxResetUris(1, uri);
        else
        {
            len = AfxGetUriLength(&dev);
            AFX_ASSERT(from->schem + from->user/* + from->pass*/ == 0);
            AfxExcerptString(&from->str, from->schem + from->user/* + from->pass*/, len, &t.str);
            t.dir = AfxGetUriLength(&dev);
            *uri = t;
        }
    }
    return len;
}

_AFXINL afxBool _AfxPathIsServer_W32(afxUri const* uri)
{
    // Returns TRUE if the string is a valid path for a server only (no share name), or FALSE otherwise.

    afxError err = NIL;
    AFX_ASSERT(uri);
    afxBool rslt = FALSE;
    afxChar const* p = AfxGetUriData(uri, 0);

    if (p)
    {
        afxChar const* v2 = NIL;

        if (*p == '\\' && p[1] == '\\' && p[2] != '?')
        {
            rslt = TRUE;
            v2 = p + 2;
        }

        if (rslt && AfxStrchr(v2, '\\'))
            rslt = FALSE;
    }
    return rslt;
}

_AFXINL afxBool AfxPathIsServer(afxUri const* uri)
{
    return _AfxPathIsServer_W32(uri);
}

_AFXINL afxBool AfxIsUriAbsolute(afxUri const* uri)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(uri);
    return !AfxPathIsRelative(uri);
}

_AFXINL afxBool AfxIsUriToDevice(afxUri const* uri)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(uri);
    afxUri hostPort;
    AfxExcerptUriAuthority(uri, NIL, &hostPort, &hostPort);
    AfxCompareUriString(&hostPort, &AFX_STRING("//."));
    //AfxClipPathDirectory();
    return AfxCompareString(&uri->str, uri->schem, "//.", 0, TRUE) && AfxCompareString(&uri->str, uri->schem + uri->host, "/", 0, TRUE);
}

_AFXINL afxBool AfxIsBaseOfUri(afxUri const* uri, afxUri const* other)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(other);
    AFX_ASSERT(uri);

    AfxThrowError();

    return err;
}

_AFXINL afxResult _AfxFindUriQueryAttribute(afxString *attr, afxString *value, void* data)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(attr);
    AFX_ASSERT(value);
    AFX_ASSERT(data);
    struct { afxString const* key; afxString* dst; afxBool success; } *data2 = data;

    if (0 == AfxCompareStrings(data2->key, 0, FALSE, 1, attr))
    {
        AfxCopyString(data2->dst, value);
        data2->success = TRUE;
        return 0; // break
    }
    return 1; // continue
}

_AFXINL afxResult AfxUriForEachQueryKey(afxUri const* uri, afxResult(*f)(afxString *attr, afxString *value, void* data), void* data)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(uri);
    AFX_ASSERT(f);
    afxResult cnt = 0;

    afxUri query2;
    AfxExcerptUriQuery(uri, FALSE, &query2);
    afxChar const* query = AfxGetUriData(&query2,0);
#if 0
    afxString64 attr;
    afxString512 value;
    AfxString64(&attr, NIL);
    AfxString512(&value, NIL);

    char *chr;
    chr = strchr(query, '=');
    while (chr)
    {
        AfxCopyStringLiteral(&attr.str, 0, chr - query, query);
        query = chr + 1;
        chr = strchr(query, '&');
        if (chr)
        {
            AfxCopyStringLiteral(&value.str, 0, chr - query, query);
            query = chr + 1;
            chr = strchr(query, '=');

            cnt++;

            if (!f(&attr.str, &value.str, data))
                break;
        }
        else
        {
            AfxCopyStringLiteral(&value.str, 0, 0, query);

            cnt++;

            if (!f(&attr.str, &value.str, data)) // faz sentido se � o �ltimo?
                break;

            break;
        }
    }
#endif
    return cnt;
}

_AFXINL afxResult AfxUriMapQueryPairs(afxUri const* uri, afxUnit base, afxUnit cnt, afxString keys[], afxString values[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(uri);
    AFX_ASSERT(keys);
    AFX_ASSERT(base == 0);
    AFX_ASSERT(cnt);
    (void)values;
    afxResult rslt = 0;

    afxUri query2;
    AfxExcerptUriQuery(uri, FALSE, &query2);
    afxChar const* start = AfxGetUriData(&query2,0);
    afxChar const *chr = start;

    while (1)
    {

    }
    return rslt;
}

_AFXINL void AfxTransformPathString(afxString* str, afxBool out)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(str);
    afxUnit srcLen = AfxGetStringLength(str);

    if (srcLen)
    {
        afxChar *ptr, *end;
        afxInt len;
        afxChar dst[4096];
        afxChar const *src2 = AfxGetStringData(str, 0);

        afxChar const dirSep = out ? '\\' : '/';

        // Check for maximum filename length
        if (!src2)
        {
            AfxThrowError(); //errno = EINVAL;
            return;
        }

        ptr = dst;

        if (srcLen > 1)
        {
            if (!out) // overwrite C: by //./c
            {
                if (AfxIsalpha(src2[0]) && src2[1] == ':')
                {
                    dst[0] = '/';
                    dst[1] = '/';
                    dst[2] = '.';
                    dst[3] = '/';
                    dst[4] = AfxTolower(src2[0]);
                    src2 += 2;
                    ptr = &dst[5];

                    if (srcLen > 2 && (src2[0] == '\\' || src2[0] == '/'))
                    {
                        *ptr++ = '/';
                        src2++;
                    }
                }
            }

            if (srcLen > 4)
            {
                if (src2[0] == '/' && src2[1] == '/' && src2[2] == '.' && src2[3] == '/' && AfxIsalpha(src2[4]))
                {
                    if (!out)
                    {
                        dst[0] = src2[0];
                        dst[1] = src2[1];
                        dst[2] = src2[2];
                        dst[3] = src2[3];
                        dst[4] = AfxTolower(src2[4]);
                        src2 += 5;
                        ptr = &dst[5];
                    }
                    else  // to WIN32
                    {
                        dst[0] = AfxToupper(src2[4]);
                        dst[1] = ':';
                        ptr = &dst[2];
                        src2 += 5;

                        if (srcLen > 5 && (src2[0] == '\\' || src2[0] == '/'))
                        {
                            *ptr++ = '\\';
                            src2++;
                        }
                    }
                }
            }
        }

        end = dst + sizeof(dst);

        while (src2[0])
        {
            // Parse path separator
            if (src2[0] == '/' || src2[0] == '\\')
            {
                src2++;

                if (ptr == end)
                {
                    AfxThrowError(); //errno = ENAMETOOLONG;
                    return;
                }
                *ptr++ = /*'/'*/dirSep;
            }

            // Parse next name part in path
            len = 0;
            while (src2[0] && src2[0] != '/' && src2[0] != '\\')
            {
                if (src2[0] > 0 && src2[0] < ' ') // is a ctrl char?
                {
                    AfxThrowError(); //errno = EINVAL;
                    AfxCatchError(err);
                    return;
                }

                if (ptr == end)
                {
                    AfxThrowError(); //errno = ENAMETOOLONG;
                    return;
                }
                *ptr++ = *src2++;
                len++;
            }

            // Handle empty name parts and '.' and '..'
            if (len == 0 && src2[0] && (src2[-1] == '/' || src2[-1] == '\\'))
                ptr--;
            else if (len == 1 && src2[-1] == '.')
                ptr -= 2;
            else if (len == 2 && src2[-1] == '.' && src2[-2] == '.')
            {
#if !0
                ptr -= 4;

                if (ptr < dst)
                {
                    //AfxThrowError(); //errno = EINVAL;
                    ptr = dst;
                    return;
                }
#else
                if (!(ptr - 4 < dst))
                {
                    ptr -= 4;
                }
                else
                {
                    ptr -= 2;
                    break;
                }
#endif
                while (ptr[0] != '/' && ptr[0] != '\\') ptr--;
            }
        }

        // Convert empty filename to /
        if (ptr == dst)
        {
            //*ptr++ = /*'/'*/dirSep;
        }

        // Terminate string
        if (ptr == end)
        {
            AfxThrowError(); //errno = ENAMETOOLONG;
            return;
        }

        *ptr = 0;

        _AfxWriteString(str, 0, dst, AfxMin(ptr - &dst[0], str->cap)); // TODO move query and fragment to end of path, once normalization never increase size of string.
        afxChar *_bytemap = AfxGetStringStorage(str, 0);
        //_bytemap[ptr - dst] = '\0';
        str->len = /*uri->str._range -*/ (ptr - dst); // se os seguintes componentes foram escritos incorretamente, mova isto para depois da concatena��o deles.
    }
}

_AFXINL void AfxCanonicalizePath(afxUri* uri, afxBool microshit)
{
    // c:\shit\\\by\\M$\foo.poo -> /c/shit/by/M$/unshitted/foo.poo

    //return;
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(uri);
    AFX_ASSERT(AfxUriIsEditable(uri));
    afxUri path, query, frag;
    
    if (AfxClipUriPath(&path, uri))
    {
        AfxExcerptUriQuery(uri, FALSE, &query);
        AfxExcerptUriFragment(uri, FALSE, &frag);

        afxString* pathStr = AfxGetUriString2(&path);

        // TODO move query and fragment to end of path, once normalization never increase size of string.
        AfxTransformPathString(pathStr, microshit);
#if 0
        uri->dir.base = path.;
        uri->dir.range;
        uri->file.base;
        uri->file.range;
        uri->ext.base;
        uri->ext.range;
#endif
        if (query.query)
            AfxCatenateStrings(pathStr, 1, AfxGetUriString(&query));

        if (frag.frag)
            AfxCatenateStrings(pathStr, 1, AfxGetUriString(&frag));

        AfxReparseUri(uri);
    }
}

_AFXINL void AfxReparseUri(afxUri* uri)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(uri);

    if (!AfxGetStringLength(&uri->str))
    {
        uri->schem = 0;
        uri->user = 0;
        //uri->pass = 0;
        uri->host = 0;
        uri->port = 0;
        uri->root = 0;
        uri->dir = 0;
        uri->fname = 0;
        uri->fext = 0;
        uri->query = 0;
        uri->frag = 0;
    }
    else
    {
        afxChar const* data = AfxGetStringData(&uri->str, 0);
        afxUnit dataLen = AfxGetStringLength(&uri->str);
        afxChar const* p = data;
#if 0
        struct uri split2 = { 0 };
        uriparse(p, 0, &split2);
        uriparse("E:/a/b/c.ext", 0, &split2);
        uriparse("://sigmaco.org", 0, &split2);
        uriparse("https://sigmaco.org", 0, &split2);
        uriparse("https://sigmaco.org/p/a", 0, &split2);
        uriparse("https://sigmaco.org/p/a?q", 0, &split2);
        uriparse("https://sigmaco.org/p/a#f", 0, &split2);
        uriparse("https://sigmaco.org/p/a?q#f", 0, &split2);
        uriparse("//./e/dir/file.ext", 0, &split2);
        uriparse("file.ext", 0, &split2);
        uriparse("/e/dir/file.ext", 0, &split2);
        uriparse("/./e/dir/file.ext", 0, &split2);
        uriparse(".ext", 0, &split2);
        uriparse("/", 0, &split2);
        uriparse("https://sigmaco.org/p/a?q#f", 0, &split2);
        uriparse(p, 0, &split2);


#endif

        url_field_t split = { 0 };

        if (dataLen)
            _AfxUrlParseRaw(data, dataLen, &split);

        uri->schem = split.schemasiz;
        uri->user = split.usersiz;
        //uri->pass = split.passsiz;
        uri->user += split.passsiz;

        uri->host = split.hostsiz;
        uri->port = split.portsiz;
        uri->root = 0;
        uri->dir = split.fdirsiz;
        uri->fname = split.fobjsiz - split.fextsiz;
        uri->fext = split.fextsiz;
        uri->query = split.querysiz;
        uri->frag = split.fragsiz;

        if (!split.hostsiz && split.fdirsiz)
        {
            afxUnit dirOff = uri->schem + uri->user + uri->host + uri->port;

            if (data[dirOff] == '/')
            {
                uri->root = 1;
                uri->dir -= 1;
                dirOff += 1;
                
                if (split.fdirsiz >= 4 && data[dirOff] == '/' && data[dirOff + 1] == '.' && data[dirOff + 2] == '/' && AfxIsalpha(data[dirOff + 3]))
                {
                    afxUnit len = split.fdirsiz - 4;
                    afxUnit i = 4;

                    while (1)
                    {
                        char ch = data[dirOff + i];
                        afxBool breake = FALSE;

                        if (AfxIsalnum(ch)) ++i;
                        else breake = TRUE;

                        if (i == len || breake)
                        {
                            uri->root += i;
                            uri->dir -= i;
                            break;
                        }
                    }
                }
            }
        }

        if (split.fobjsiz)
        {
            afxChar const* data = AfxGetStringData(&uri->str, 0);

            if (data[uri->schem + uri->user + uri->host + uri->port + uri->root + uri->dir] == '/' || data[uri->schem + uri->user + uri->host + uri->port + uri->root + uri->dir] == '\\') // '\\' is unneeded once it is already normalized
            {
                uri->fname--, uri->dir++;
                split.fobjsiz--, split.fdirsiz++;
            }
        }
        int a = 1;
    }
}



_AFXINL void AfxDeallocateUri(afxUri* uri)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(uri);
    //AFX_ASSERT(uri->str.flags & _AFX_STR_FLAG_ALLOCED);
    
    AfxDeallocateString(&uri->str);
    AfxResetUris(1, uri);
}

_AFXINL afxError AfxAllocateUri(afxUri* uri, afxUnit cap, void const *start, afxUnit range)
{
    afxError err = AFX_ERR_NONE;
    AfxResetUris(1, uri);

    if (AfxAllocateString(&uri->str, cap, start, range))
        AfxThrowError();

    AfxReparseUri(uri);
    return err;
}

#if 0
_AFXINL afxError AfxReallocateUri(afxUri* uri, afxUnit cap, void const *start, afxUnit range)
{
    afxError err = AFX_ERR_NONE;
    void *buf = NIL;

    if (start && !range)
        range = AfxStrlen(start);

    AFX_ASSERT(!range || (range && start));

    {
        AFX_ASSERT(uri);
        //AFX_ASSERT(uri->str.flags & _AFX_STR_FLAG_ALLOCED);

        afxUnit effectiveCap = cap ? cap : range;
        AFX_ASSERT(effectiveCap);

        if (AfxReallocateString(&uri->str, effectiveCap, start, range))
             AfxThrowError();

        AfxReparseUri(uri);
    }
    return err;
}
#endif

_AFXINL afxError AfxDuplicateUri(afxUri* uri, afxUri const *in)
{
    afxError err = AFX_ERR_NONE;

    if (!in) AfxResetUris(1, uri);
    else
    {
        AFX_ASSERT(in);
        afxUnit len = AfxGetUriLength(in);

        if (!len) AfxResetUris(1, uri);
        else
        {
            AFX_ASSERT(len);
            void const *strData = AfxGetStringData(&in->str, 0);
            AFX_ASSERT(strData);

            if (AfxAllocateUri(uri, 0, strData, len))
                AfxThrowError();
        }
    }
    return err;
}

_AFX afxUri* AfxMakeUri8(afxUri8 *uri, afxUri const* src)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(uri);
    uri->buf[0] = '\0';
    AfxMakeUri(&uri->uri, sizeof(uri->buf), uri->buf, 0);

    if (src)
        AfxCopyUri(&uri->uri, src);

    return &uri->uri;
}

_AFX afxUri* AfxMakeUri32(afxUri32 *uri, afxUri const* src)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(uri);
    uri->buf[0] = '\0';
    AfxMakeUri(&uri->uri, sizeof(uri->buf), uri->buf, 0);

    if (src)
        AfxCopyUri(&uri->uri, src);

    return &uri->uri;
}

_AFX afxUri* AfxMakeUri128(afxUri128 *uri, afxUri const* src)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(uri);
    uri->buf[0] = '\0';
    AfxMakeUri(&uri->uri, sizeof(uri->buf), uri->buf, 0);

    if (src)
        AfxCopyUri(&uri->uri, src);

    return &uri->uri;
}

_AFX afxUri* AfxMakeUri2048(afxUri2048 *uri, afxUri const* src)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(uri);
    uri->buf[0] = '\0';
    AfxMakeUri(&uri->uri, sizeof(uri->buf), uri->buf, 0);

    if (src)
        AfxCopyUri(&uri->uri, src);

    return &uri->uri;
}
