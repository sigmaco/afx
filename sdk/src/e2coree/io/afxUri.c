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
 *                       (c) 2017 SIGMA, Engineering In Technology
 *                             <https://sigmaco.org/qwadro/>
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "qwadro/mem/afxMmu.h"
#include "qwadro/io/afxUri.h"

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

_AFXINL afxResult _AfxUrlParseRaw(const char *str, afxNat strLenTotal, url_field_t *url)
{
    afxNat strLen = strLenTotal;
    const char *pch, *start = str, *end = str + strLen;
    *url = (url_field_t) { 0 };

    if (str && str[0] && strLen)
    {
        url->href = str;
        url->hrefsiz = strLen;// strlen(str);

        pch = memchr(str, ':', strLen);   /* parse schema */

        if (pch && pch[1] == '/' && pch[2] == '/')
        {
            url->schema = str;
            url->schemasiz = pch - str;
            strLen -= url->schemasiz;
            str = pch + 3;

            pch = memchr(str, '@', strLen);   /* parse user info */

            if (pch)
            {
                pch = memchr(str, ':', strLen);

                if (pch)
                {
                    url->username = str;
                    url->usersiz = pch - str;
                    strLen -= url->usersiz;

                    str = pch + 1;
                    pch = memchr(str, '@', strLen);

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
                pch = memchr(str, ']', strLen);

                if (pch)
                {
                    url->host = str;
                    url->hostsiz = pch - str;
                    strLen -= url->hostsiz;
                    str = pch + 1;

                    if (str[0] == ':')
                    {
                        str++;
                        pch = memchr(str, '/', strLen);

                        if (pch)
                        {
                            url->port = str;
                            url->portsiz = pch - str;
                            strLen -= url->portsiz;
                            str = pch + 1;
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
                pch = memchr(str, ':', strLen);
                pch_slash = memchr(str, '/', strLen);

                if (pch && (!pch_slash || (pch_slash && pch < pch_slash)))
                {
                    url->host = str;
                    url->hostsiz = pch - str;
                    strLen -= url->hostsiz;
                    str = pch + 1;
                    pch = memchr(str, '/', strLen);

                    if (pch)
                    {
                        url->port = str;
                        url->portsiz = pch - str;
                        strLen -= url->portsiz;
                        str = pch + 1;
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
                    pch = memchr(str, '/', strLen);

                    if (pch)
                    {
                        url->host = str;
                        url->hostsiz = pch - str;
                        strLen -= url->hostsiz;
                        str = pch + 1;
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
            pch = memchr(str, '?', strLen);

            if (pch) // has query
            {
                //url->path = str;
                url->pathsiz = pch - url->path;
                strLen -= url->pathsiz;
                str = pch/* + 1*/;
                url->query = pch;
                pch = memchr(str, '#', strLen);

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
                pch = memchr(str, '#', strLen);

                if (pch) // has fragment but no query
                {
                    //url->path = str;
                    url->pathsiz = pch - url->path;
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
                    url->pathsiz = strLen - (str - start);//strlen(str);
                    strLen -= url->pathsiz;
                    //str = str + strLen - (str - start);//strlen(str);
                    url->fragment = 0;
                    url->fragsiz = 0;
                    strLen -= url->fragsiz;
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
                    url->fobj = url->path;
                    url->fobjsiz = url->pathsiz;
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

// READ-ONLY METHODS //////////////////////////////////////////////////////////

_AFXINL afxResult AfxCompareUriToString(afxUri const *uri, afxString const *str)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(uri, afxFcc_URI);
    AfxAssertType(str, afxFcc_STR);

    afxString const *strA = AfxGetUriString(uri);
    AfxAssertType(strA, afxFcc_STR);

    return AfxTestStringEquality(strA, str);
}

_AFXINL afxResult AfxCompareUriToStringCi(afxUri const *uri, afxString const *str)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(uri, afxFcc_URI);
    AfxAssertType(str, afxFcc_STR);

    afxString const *strA = AfxGetUriString(uri);
    AfxAssertType(strA, afxFcc_STR);

    return AfxTestStringEquivalence(strA, str);
}

_AFXINL afxResult AfxCompareUri(afxUri const *uri, afxUri const *other)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(uri, afxFcc_URI);
    AfxAssertType(other, afxFcc_URI);

    afxString const *strA = AfxGetUriString(uri), *strB = AfxGetUriString(other);
    AfxAssertType(strA, afxFcc_STR);
    AfxAssertType(strB, afxFcc_STR);
    return AfxTestStringEquality(strA, strB);
}

_AFXINL afxResult AfxCompareUriCi(afxUri const *uri, afxUri const *other)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(uri, afxFcc_URI);
    AfxAssertType(other, afxFcc_URI);

    afxString const *strA = AfxGetUriString(uri), *strB = AfxGetUriString(other);
    AfxAssertType(strA, afxFcc_STR);
    AfxAssertType(strB, afxFcc_STR);

    return AfxTestStringEquivalence(strA, strB);
}

_AFXINL afxBufferedString* AfxGetBufferedUriString(afxUri *uri)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(uri, afxFcc_URI);
    return &(uri->str);
}

_AFXINL afxString const* AfxGetUriString(afxUri const *uri)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(uri, afxFcc_URI);
    return &(uri->str.str);
}

_AFXINL void* AfxGetBufferedUriData(afxUri *uri, afxNat base)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(uri, afxFcc_URI);
    return AfxGetBufferedStringData(&(uri->str), base);
}

_AFXINL void const* AfxGetUriData(afxUri const *uri, afxNat base)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(uri, afxFcc_URI);
    return AfxGetStringData(&(uri->str.str), base);
}

_AFXINL afxNat AfxMeasureUri(afxUri const* uri)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(uri, afxFcc_URI);
    return uri->schem + uri->user + uri->pass + uri->host + uri->port + uri->dir + uri->fname + uri->fext + uri->query + uri->frag;
}

_AFXINL afxNat AfxMeasurePath(afxUri const* uri)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(uri, afxFcc_URI);
    return uri->dir + uri->fname + uri->fext;
}

_AFXINL afxNat AfxMeasureUriRequest(afxUri const* uri)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(uri, afxFcc_URI);
    return uri->dir + uri->fname + uri->fext + uri->query/* + uri->frag*/;
}

_AFXINL afxNat AfxGetUriScheme(afxUri *uri, afxUri const *src)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(src, afxFcc_URI);
    AfxAssert(uri);
    
    afxNat schem = src->schem;
    afxNat len = AfxUriFromStringRange(uri, &(src->str.str), 0, schem);
    AfxAssert(len == 0);
    len = AfxGetStringLength(&(uri->str.str));
    AfxAssert(len == uri->schem);
    AfxAssert(len == schem);
    return len;
}

_AFXINL afxNat AfxGetUriUsername(afxUri *uri, afxUri const *src)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(src, afxFcc_URI);
    AfxAssert(uri);

    afxNat user = src->user;
    afxNat len = AfxUriFromStringRange(uri, &(src->str.str), src->schem, user);
    AfxAssert(len == 0);
    len = AfxGetStringLength(&(uri->str.str));
    AfxAssert(len == uri->user);
    AfxAssert(len == user);
    return len;
}

_AFXINL afxNat AfxGetUriPassword(afxUri *uri, afxUri const *src)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(src, afxFcc_URI);
    AfxAssert(uri);

    afxNat pass = src->pass;
    afxNat len = AfxUriFromStringRange(uri, &(src->str.str), src->schem + src->user, pass);
    AfxAssert(len == 0);
    len = AfxGetStringLength(&(uri->str.str));
    AfxAssert(len == uri->pass);
    AfxAssert(len == pass);
    return len;
}

_AFXINL afxNat AfxGetUriHost(afxUri *uri, afxUri const *src)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(src, afxFcc_URI);
    AfxAssert(uri);

    afxNat host = src->host;
    afxNat len = AfxUriFromStringRange(uri, &(src->str.str), src->schem + src->user + src->pass, host);
    AfxAssert(len == 0);
    len = AfxGetStringLength(&(uri->str.str));
    AfxAssert(len == uri->host);
    AfxAssert(len == host);
    return len;
}

_AFXINL afxNat AfxGetUriPort(afxUri *uri, afxUri const *src)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(src, afxFcc_URI);
    AfxAssert(uri);

    afxNat port = src->port;
    afxNat len = AfxUriFromStringRange(uri, &(src->str.str), src->schem + src->user + src->pass + src->host, port);
    AfxAssert(len == 0);
    len = AfxGetStringLength(&(uri->str.str));
    AfxAssert(len == uri->port);
    AfxAssert(len == port);
    return len;
}

_AFXINL afxNat AfxGetUriDirectory(afxUri *uri, afxUri const *src)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(src, afxFcc_URI);
    AfxAssert(uri);

    afxNat dir = src->dir;
    afxNat len = AfxUriFromStringRange(uri, &(src->str.str), src->schem + src->user + src->pass + src->host + src->port, dir);
    AfxAssert(len == 0);
    len = AfxGetStringLength(&(uri->str.str));
    AfxAssert(len == uri->dir);
    AfxAssert(len == dir);
    return len;
}

_AFXINL afxNat AfxGetUriName(afxUri *uri, afxUri const *src)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(src, afxFcc_URI);
    AfxAssert(uri);

    afxNat fname = src->fname;
    afxNat len = AfxUriFromStringRange(uri, &(src->str.str), src->schem + src->user + src->pass + src->host + src->port + src->dir, fname);
    AfxAssert(len == 0);
    len = AfxGetStringLength(&(uri->str.str));
    AfxAssert(len == uri->fname);
    AfxAssert(len == fname);
    return len;
}

_AFXINL afxNat AfxGetUriNameString(afxUri const *src, afxString* name)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(src, afxFcc_URI);
    AfxAssert(name);
    afxUri tmp;
    AfxGetUriName(&tmp, src);
    AfxReplicateString(name, AfxGetUriString(&tmp));
    return name->len;
}

_AFXINL afxNat AfxGetUriExtension(afxUri *uri, afxUri const *src, afxBool skipDot)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(src, afxFcc_URI);
    AfxAssert(uri);

    afxNat additive = skipDot && src->fext ? 1 : 0;
    afxNat fext = src->fext - additive;
    afxNat len = AfxUriFromStringRange(uri, &(src->str.str), src->schem + src->user + src->pass + src->host + src->port + src->dir + src->fname + additive, fext);
    AfxAssert(len == 0);
    len = AfxGetStringLength(&(uri->str.str));
    AfxAssert(len == uri->fext);
    AfxAssert(len == fext);
    return len;
}

_AFXINL afxNat AfxGetUriObject(afxUri *uri, afxUri const *src)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(src, afxFcc_URI);
    AfxAssert(uri);

    afxNat fnameExt = src->fname + src->fext;
    afxNat len = AfxUriFromStringRange(uri, &(src->str.str), src->schem + src->user + src->pass + src->host + src->port + src->dir, fnameExt);
    AfxAssert(len == 0);
    len = AfxGetStringLength(&(uri->str.str));
    AfxAssert(len == uri->fname + uri->fext);
    AfxAssert(len == fnameExt);
    return len;
}

_AFXINL afxNat AfxGetUriPath(afxUri *uri, afxUri const *src)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(src, afxFcc_URI);
    AfxAssert(uri);

    afxNat range = AfxMeasurePath(src);
    afxNat len = AfxUriFromStringRange(uri, &(src->str.str), src->schem + src->user + src->pass + src->host + src->port, range);
    AfxAssert(len == 0);
    len = AfxGetStringLength(&(uri->str.str));
    AfxAssert(len == AfxMeasurePath(uri));
    AfxAssert(len == range);
    return len;
}

_AFXINL afxNat AfxGetUriDirectoryRW(afxUri *uri, afxUri *src)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(src, afxFcc_URI);
    AfxAssert(uri);

    AfxResetUri(uri);
    uri->dir = src->dir;
    AfxMakeBufferedString(&(uri->str), uri->dir, AfxGetBufferedUriData(src, src->schem + src->user + src->pass + src->host + src->port), uri->dir);
    afxNat len = AfxGetStringLength(&(uri->str.str));
    AfxAssert(len == uri->dir);
    return len;
}

_AFXINL afxNat AfxGetUriNameRW(afxUri *uri, afxUri *src)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(src, afxFcc_URI);
    AfxAssert(uri);

    AfxResetUri(uri);
    uri->fname = src->fname;
    AfxMakeBufferedString(&(uri->str), uri->fname, AfxGetBufferedUriData(src, src->schem + src->user + src->pass + src->host + src->port + src->dir), uri->fname);
    afxNat len = AfxGetStringLength(&(uri->str.str));
    AfxAssert(len == uri->fname);
    return len;
}

_AFXINL afxNat AfxGetUriExtensionRW(afxUri *uri, afxUri *src)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(src, afxFcc_URI);
    AfxAssert(uri);

    AfxResetUri(uri);
    uri->fext = src->fext;
    AfxMakeBufferedString(&(uri->str), uri->fext, AfxGetBufferedUriData(src, src->schem + src->user + src->pass + src->host + src->port + src->dir + src->fname), uri->fext);
    afxNat len = AfxGetStringLength(&(uri->str.str));
    AfxAssert(len == uri->fext);
    return len;
}

_AFXINL afxNat AfxGetUriObjectRW(afxUri *uri, afxUri *src)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(src, afxFcc_URI);
    AfxAssert(uri);

    AfxResetUri(uri);
    uri->fname = src->fname;
    uri->fext = src->fext;
    AfxMakeBufferedString(&(uri->str), uri->fname + uri->fext, AfxGetBufferedUriData(src, src->schem + src->user + src->pass + src->host + src->port + src->dir), uri->fname + uri->fext);
    afxNat len = AfxGetStringLength(&(uri->str.str));
    AfxAssert(len == uri->fname + uri->fext);
    return len;
}

_AFXINL afxNat AfxGetUriPathRW(afxUri *uri, afxUri *src)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(src, afxFcc_URI);
    AfxAssert(uri);

    AfxResetUri(uri);
    uri->dir = src->dir;
    uri->fname = src->fname;
    uri->fext = src->fext;
    AfxMakeBufferedString(&(uri->str), src->dir + src->fname + src->fext, AfxGetBufferedUriData(src, src->schem + src->user + src->pass + src->host + src->port), src->dir + src->fname + src->fext);
    afxNat len = AfxGetStringLength(&(uri->str.str));
    AfxAssert(len == src->dir + src->fname + src->fext);
    return len;
}

_AFXINL afxNat AfxGetUriQuery(afxUri *uri, afxUri const *src, afxBool skipSep)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(src, afxFcc_URI);
    AfxAssert(uri);

    AfxResetUri(uri);
    afxNat additive = skipSep && src->query ? 1 : 0;
    uri->query = src->query - additive;
    AfxExcerptString(&(uri->str.str), &(src->str.str), src->schem + src->user + src->pass + src->host + src->port + src->dir + src->fname + src->fext + additive, uri->query);
    afxNat len = AfxGetStringLength(&(uri->str.str));
    AfxAssert(len == uri->query);
    return len;
}

_AFXINL afxNat AfxGetUriQueryString(afxUri const *uri, afxBool skipSep, afxString* query)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(uri, afxFcc_URI);
    AfxAssert(query);
    afxUri tmp;
    AfxGetUriQuery(&tmp, uri, skipSep);
    AfxReplicateString(query, AfxGetUriString(&tmp));
    return query->len;
}

_AFXINL afxNat AfxGetUriFragment(afxUri *uri, afxUri const *src, afxBool skipSep)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(src, afxFcc_URI);
    AfxAssert(uri);

    AfxResetUri(uri);
    afxNat additive = skipSep && src->frag ? 1 : 0;
    uri->frag = src->frag - additive;
    AfxExcerptString(&(uri->str.str), &(src->str.str), src->schem + src->user + src->pass + src->host + src->port + src->dir + src->fname + src->fext + src->query + additive, uri->frag);
    afxNat len = AfxGetStringLength(&(uri->str.str));
    AfxAssert(len == uri->frag);
    return len;
}

_AFXINL afxError AfxCopyUriScheme(afxUri *uri, afxUri const *src)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(uri, afxFcc_URI);
    AfxAssertType(src, afxFcc_URI);

    afxUri excerpt;
    AfxGetUriScheme(&excerpt, src);

    if (AfxCopyUri(uri, &excerpt))
        AfxThrowError();

    return err;
}

_AFXINL afxError AfxCopyUriUsername(afxUri *uri, afxUri const *src)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(uri, afxFcc_URI);
    AfxAssertType(src, afxFcc_URI);

    afxUri excerpt;
    AfxGetUriUsername(&excerpt, src);

    if (AfxCopyUri(uri, &excerpt))
        AfxThrowError();

    return err;
}

_AFXINL afxError AfxCopyUriPassword(afxUri *uri, afxUri const *src)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(uri, afxFcc_URI);
    AfxAssertType(src, afxFcc_URI);

    afxUri excerpt;
    AfxGetUriPassword(&excerpt, src);

    if (AfxCopyUri(uri, &excerpt))
        AfxThrowError();

    return err;
}

_AFXINL afxError AfxCopyUriHost(afxUri *uri, afxUri const *src)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(uri, afxFcc_URI);
    AfxAssertType(src, afxFcc_URI);

    afxUri excerpt;
    AfxGetUriHost(&excerpt, src);

    if (AfxCopyUri(uri, &excerpt))
        AfxThrowError();

    return err;
}

_AFXINL afxError AfxCopyUriPort(afxUri *uri, afxUri const *src)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(uri, afxFcc_URI);
    AfxAssertType(src, afxFcc_URI);

    afxUri excerpt;
    AfxGetUriPort(&excerpt, src);

    if (AfxCopyUri(uri, &excerpt))
        AfxThrowError();

    return err;
}

_AFXINL afxError AfxCopyUriDirectory(afxUri *uri, afxUri const *src)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(uri, afxFcc_URI);
    AfxAssertType(src, afxFcc_URI);

    afxUri excerpt;
    AfxGetUriDirectory(&excerpt, src);

    if (AfxCopyUri(uri, &excerpt))
        AfxThrowError();

    return err;
}

_AFXINL afxError AfxCopyUriObject(afxUri *uri, afxUri const *src)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(uri, afxFcc_URI);
    AfxAssertType(src, afxFcc_URI);

    afxUri excerpt;
    AfxGetUriObject(&excerpt, src);

    if (AfxCopyUri(uri, &excerpt))
        AfxThrowError();

    return err;
}

_AFXINL afxError AfxCopyUriName(afxUri *uri, afxUri const *src)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(uri, afxFcc_URI);
    AfxAssertType(src, afxFcc_URI);

    afxUri excerpt;
    AfxGetUriName(&excerpt, src);

    if (AfxCopyUri(uri, &excerpt))
        AfxThrowError();

    return err;
}

_AFXINL afxError AfxCopyUriExtension(afxUri *uri, afxUri const *src, afxBool skipDot)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(uri, afxFcc_URI);
    AfxAssertType(src, afxFcc_URI);

    afxUri excerpt;
    AfxGetUriExtension(&excerpt, src, skipDot);

    if (AfxCopyUri(uri, &excerpt))
        AfxThrowError();

    return err;
}

_AFXINL afxError AfxCopyUriPath(afxUri *uri, afxUri const *src)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(uri, afxFcc_URI);
    AfxAssertType(src, afxFcc_URI);

    afxUri excerpt;
    AfxGetUriPath(&excerpt, src);

    if (AfxCopyUri(uri, &excerpt))
        AfxThrowError();

    return err;
}

_AFXINL afxError AfxCopyUriQuery(afxUri *uri, afxUri const *src, afxBool skipSep)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(uri, afxFcc_URI);
    AfxAssertType(src, afxFcc_URI);

    afxUri excerpt;
    AfxGetUriQuery(&excerpt, src, skipSep);

    if (AfxCopyUri(uri, &excerpt))
        AfxThrowError();

    return err;
}

_AFXINL afxError AfxCopyUriFragment(afxUri *uri, afxUri const *src, afxBool skipSep)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(uri, afxFcc_URI);
    AfxAssertType(src, afxFcc_URI);

    afxUri excerpt;
    AfxGetUriFragment(&excerpt, src, skipSep);
    
    if (AfxCopyUri(uri, &excerpt))
        AfxThrowError();

    return err;
}

_AFXINL afxBool AfxPathIsRelative(afxUri const *uri)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(uri, afxFcc_URI);

    afxUri excerpt;
    AfxGetUriPath(&excerpt, uri);
    afxByte const* data = AfxGetUriData(&excerpt, 0);

    return (!data || (data[0] != '/' && data[1] != ':'));
}

_AFXINL afxBool AfxUriIsEditable(afxUri const *uri)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(uri, afxFcc_URI);
    return AfxStringIsWriteable(&uri->str);
}

_AFXINL afxBool AfxUriIsBlank(afxUri const *uri)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(uri, afxFcc_URI);
    return 0 == (uri->schem + uri->user + uri->pass + uri->host + uri->port + uri->dir + uri->fname + uri->fext + uri->query + uri->frag);
}

_AFXINL afxNat AfxGetUriSchemeLength(afxUri const *uri)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(uri, afxFcc_URI);
    return uri->schem;
}

_AFXINL afxNat AfxGetUriPasswordLength(afxUri const *uri)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(uri, afxFcc_URI);
    return uri->pass;
}

_AFXINL afxNat AfxGetUriUsernameLength(afxUri const *uri)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(uri, afxFcc_URI);
    return uri->user;
}

_AFXINL afxNat AfxGetUriHostLength(afxUri const *uri)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(uri, afxFcc_URI);
    return uri->host;
}

_AFXINL afxNat AfxGetUriPortLength(afxUri const *uri)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(uri, afxFcc_URI);
    return uri->port;
}

_AFXINL afxNat AfxGetUriPathLength(afxUri const *uri)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(uri, afxFcc_URI);
    return uri->dir + uri->fname + uri->fext;
}

_AFXINL afxNat AfxGetUriDirectoryLength(afxUri const *uri)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(uri, afxFcc_URI);
    return uri->dir;
}

_AFXINL afxNat AfxGetFileNameLength(afxUri const *uri)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(uri, afxFcc_URI);
    return uri->fname;
}

_AFXINL afxNat AfxGetFileExtensionLength(afxUri const *uri)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(uri, afxFcc_URI);
    return uri->fext;
}

_AFXINL afxNat AfxGetUriQueryLength(afxUri const *uri)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(uri, afxFcc_URI);
    return uri->query;
}

_AFXINL afxNat AfxGetUriFragmentLength(afxUri const *uri)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(uri, afxFcc_URI);
    return uri->frag;
}

_AFXINL afxResult _AfxFindUriQueryAttribute(afxString *attr, afxString *value, void* data)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(attr);
    AfxAssert(value);
    AfxAssert(data);
    struct { afxString const* key; afxBufferedString* dst; afxBool success; } *data2 = data;

    if (0 == AfxTestStringEquality(data2->key, attr))
    {
        AfxCopyString(data2->dst, value);
        data2->success = TRUE;
        return 0; // break
    }
    return 1; // continue
}

_AFXINL afxResult AfxUriForEachQueryKey(afxUri const *uri, afxResult(*f)(afxString *attr, afxString *value, void* data), void* data)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(uri, afxFcc_URI);
    AfxAssert(f);
    afxResult cnt = 0;

    afxUri query2;
    AfxGetUriQuery(&query2, uri, FALSE);
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

            if (!f(&attr.str, &value.str, data)) // faz sentido se é o último?
                break;

            break;
        }
    }
#endif
    return cnt;
}

_AFXINL afxResult AfxUriMapQueryPairs(afxUri const *uri, afxNat base, afxNat cnt, afxString keys[], afxString values[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(uri, afxFcc_URI);
    AfxAssert(keys);
    AfxAssert(base == 0);
    AfxAssert(cnt);
    (void)values;
    afxResult rslt = 0;

    afxUri query2;
    AfxGetUriQuery(&query2, uri, FALSE);
    afxChar const* start = AfxGetUriData(&query2,0);
    afxChar const *chr = start;

    while (1)
    {

    }
    return rslt;
}

_AFXINL void AfxNormalizePath(afxUri *uri) // c:\shit\\\by\\M$\foo.poo -> /c/shit/by/M$/unshitted/foo.poo
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(uri, afxFcc_URI);
    AfxAssert(AfxUriIsEditable(uri));
    afxUri path, query, frag;
    
    if (AfxGetUriPathRW(&path, uri))
    {
        AfxGetUriQuery(&query, uri, FALSE);
        AfxGetUriFragment(&frag, uri, FALSE);

        afxBufferedString *pathStr = AfxGetBufferedUriString(&path);

        // TODO move query and fragment to end of path, once normalization never increase size of string.
            
        afxChar *ptr, *end;
        afxInt len;
        afxChar dst[4096];
        afxChar const *src2 = AfxGetStringData(&pathStr->str, 0);

        // Check for maximum filename length
        if (!src2)
        {
            AfxThrowError(); //errno = EINVAL;
            return;
        }

        // Remove drive letter from filename (e.g. c:)
        if (src2[0] != 0 && src2[1] == ':')
            if (0)
                src2 += 2;

        // Initialize buffer
        ptr = dst;
        end = dst + sizeof(dst);

        // Add current directory to filename if relative path
        if (*src2 == '/' || *src2 == '\\')
        {
            //char cxmlir[FILENAME_MAX];

            // Do not add current directory if it is root directory
            len = strlen("");

            if (len > 1)
            {
                memcpy(ptr, "", len);
                ptr += len;
            }
        }

        while (*src2)
        {
            // Parse path separator
            if (*src2 == '/' || *src2 == '\\')
                src2++;

            if (ptr == end)
            {
                AfxThrowError(); //errno = ENAMETOOLONG;
                return;
            }

            //if (fsys->realCfg.rootEmptyPath)
            if (ptr != dst)
                * ptr++ = '/';

            // Parse next name part in path
            len = 0;
            while (*src2 && *src2 != '/' && *src2 != '\\')
            {
                // We do not allow control characters in filenames
                if (*src2 > 0 && *src2 < ' ')
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
            if (len == 0) ptr--;
            else if (len == 1 && src2[-1] == '.') ptr -= 2;
            else if (len == 2 && src2[-1] == '.' && src2[-2] == '.')
            {
                ptr -= 4;

                if (ptr < dst)
                {
                    AfxThrowError(); //errno = EINVAL;
                    return;
                }
                while (*ptr != '/') ptr--;
            }
        }

        // Convert empty filename to /
        if (ptr == dst)
        {
            //if (fsys->realCfg.rootEmptyPath)
            //*ptr++ = '/';
        }

        // Terminate string
        if (ptr == end)
        {
            AfxThrowError(); //errno = ENAMETOOLONG;
            return;
        }

        *ptr = 0;

        _AfxWriteString(pathStr, 0, dst, ptr - dst); // TODO move query and fragment to end of path, once normalization never increase size of string.
        afxByte *_bytemap = AfxGetBufferedStringData(pathStr, 0);
        _bytemap[ptr - dst] = '\0';
        uri->str.str.len = /*uri->str._range -*/ (ptr - dst); // se os seguintes componentes foram escritos incorretamente, mova isto para depois da concatenação deles.
#if 0
        uri->dir.base = path.;
        uri->dir.range;
        uri->file.base;
        uri->file.range;
        uri->ext.base;
        uri->ext.range;
#endif
        if (AfxGetUriQueryLength(&query))
            AfxAppendString(pathStr, AfxGetUriString(&query));

        if (AfxGetUriFragmentLength(&frag))
            AfxAppendString(pathStr, AfxGetUriString(&frag));

        AfxReparseUri(uri);
    }
}

_AFXINL void AfxReparseUri(afxUri *uri)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(uri, afxFcc_URI);

    afxChar* data = AfxGetBufferedStringData(&uri->str, 0);

    url_field_t split = { 0 };
    _AfxUrlParseRaw(data, AfxGetStringLength(&uri->str.str), &split);

    uri->schem = split.schemasiz;
    uri->user = split.usersiz;
    uri->pass = split.passsiz;
    uri->host = split.hostsiz;
    uri->port = split.portsiz;
    uri->dir = split.fdirsiz;
    uri->fname = split.fobjsiz - split.fextsiz;
    uri->fext = split.fextsiz;
    uri->query = split.querysiz;
    uri->frag = split.fragsiz;

    if (split.fobjsiz)
    {
        if (data[uri->schem + uri->user + uri->pass + uri->host + uri->port + uri->dir] == '/' || data[uri->schem + uri->user + uri->pass + uri->host + uri->port + uri->dir] == '\\') // '\\' is unneeded once it is already normalized
        {
            uri->fname--, uri->dir++;
            split.fobjsiz--, split.fdirsiz++;
        }
    }
}

_AFXINL void AfxFormatUri(afxUri *uri, afxChar const *fmt, ...)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(uri, afxFcc_URI);
    AfxAssert(AfxUriIsEditable(uri));

    AfxAssert(fmt);

    afxBufferedString *str = AfxGetBufferedUriString(uri);
    AfxAssertType(str, afxFcc_STR);

    va_list args;
    va_start(args, fmt);
    AfxFormatStringArg(str, fmt, args);
    va_end(args);
    AfxReparseUri(uri);
    AfxNormalizePath(uri);
}

_AFXINL afxError AfxCopyUri(afxUri *uri, afxUri const *src)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(src, afxFcc_URI);
    AfxAssertType(uri, afxFcc_URI);
    AfxAssert(AfxUriIsEditable(uri));
    
    if (AfxCopyString(&uri->str, &src->str.str))
        AfxThrowError();

    AfxReparseUri(uri); // Strings can have differently sized buffers. So we recalc it instead of just copy offsets and ranges.

    return err;
}

_AFXINL void AfxReplicateUri(afxUri *uri, afxUri const *in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(in, afxFcc_URI);
    AfxAssert(uri);

    AfxResetUri(uri);
    AfxReplicateString(&(uri->str.str), &(in->str.str));

    AfxAssignTypeFcc(uri, afxFcc_URI);
    uri->schem = in->schem;
    uri->user = in->user;
    uri->pass = in->pass;
    uri->host = in->host;
    uri->port = in->port;
    uri->dir = in->dir;
    uri->fname = in->fname;
    uri->fext = in->fext;
    uri->query = in->query;
    uri->frag = in->frag;
}

_AFXINL afxNat AfxUriFromString(afxUri *uri, afxString const *src)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(uri);
    
    AfxResetUri(uri);
    AfxReplicateString(&uri->str.str, src);
    AfxReparseUri(uri);
    //AfxNormalizePath(uri);  // can't change static data, will cause access violation
    return AfxMeasureUri(uri);
}

_AFXINL afxNat AfxUriFromStringRange(afxUri *uri, afxString const *src, afxNat offset, afxNat len)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(uri);

    AfxResetUri(uri);
    afxNat len2 = AfxExcerptString(&uri->str.str, src, offset, len);
    AfxReparseUri(uri);
    //AfxNormalizePath(uri);  // can't change static data, will cause access violation
    return len2;
}

_AFXINL void AfxMakeUri(afxUri *uri, void const *start, afxNat len)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(uri);

    AfxResetUri(uri);
    AfxMakeString(&uri->str.str, start, len);
    AfxReparseUri(uri);
    //AfxNormalizePath(uri); // can't change static data, will cause access violation
}

_AFXINL void AfxMakeBufferedUri(afxUri *uri, afxNat cap, void *start, afxNat len)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(uri);
    AfxResetUri(uri);
    AfxMakeBufferedString(&uri->str, cap, start, len);
    AfxAssert(AfxUriIsEditable(uri));
    AfxReparseUri(uri);
    AfxNormalizePath(uri);
}

_AFXINL void AfxResetUri(afxUri *uri)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(uri);
    AfxAssignTypeFcc(uri, afxFcc_URI);

    AfxResetBufferedString(&uri->str);

    uri->schem = 0;
    uri->user = 0;
    uri->pass = 0;
    uri->host = 0;
    uri->port = 0;
    uri->dir = 0;
    uri->fname = 0;
    uri->fext = 0;
    uri->query = 0;
    uri->frag = 0;
}

_AFXINL void AfxClearUri(afxUri *uri)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(uri, afxFcc_URI);

    AfxClearBufferedString(&uri->str);

    uri->schem = 0;
    uri->user = 0;
    uri->pass = 0;
    uri->host = 0;
    uri->port = 0;
    uri->dir = 0;
    uri->fname = 0;
    uri->fext = 0;
    uri->query = 0;
    uri->frag = 0;
}

_AFXINL void AfxDeallocateUri(afxUri *uri)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(uri, afxFcc_URI);
    //AfxAssert(uri->str.flags & _AFX_STR_FLAG_ALLOCED);
    
    AfxDeallocateString(&uri->str);
    AfxResetUri(uri);
}

_AFXINL afxError AfxAllocateUri(afxUri* uri, afxNat cap, void const *start, afxNat range)
{
    afxError err = AFX_ERR_NONE;
    AfxResetUri(uri);

    if (AfxAllocateString(&uri->str, cap, start, range))
        AfxThrowError();

    AfxReparseUri(uri);
    return err;
}

#if 0
_AFXINL afxError AfxReallocateUri(afxUri *uri, afxNat cap, void const *start, afxNat range)
{
    afxError err = AFX_ERR_NONE;
    AfxTryAssertType(uri, afxFcc_URI);
    void *buf = NIL;

    if (start && !range)
        range = AfxStrlen(start);

    AfxAssert(!range || (range && start));

    {
        AfxAssertType(uri, afxFcc_URI);
        //AfxAssert(uri->str.flags & _AFX_STR_FLAG_ALLOCED);

        afxNat effectiveCap = cap ? cap : range;
        AfxAssert(effectiveCap);

        if (AfxReallocateString(&uri->str, effectiveCap, start, range))
             AfxThrowError();

        AfxReparseUri(uri);
    }
    return err;
}
#endif

_AFXINL afxError AfxCloneUriPath(afxUri* uri, afxUri const *in)
{
    afxError err = AFX_ERR_NONE;

    if (!in) AfxResetUri(uri);
    else
    {
        AfxAssertType(in, afxFcc_URI);
        afxUri excerpt;
        afxNat len = AfxGetUriPath(&excerpt, in);

        if (!len) AfxResetUri(uri);
        else
        {
            if (AfxCloneUri(uri, &excerpt))
                AfxThrowError();
        }
    }
    return err;
}

_AFXINL afxError AfxCloneUriName(afxUri* uri, afxUri const *in)
{
    afxError err = AFX_ERR_NONE;

    if (!in) AfxResetUri(uri);
    else
    {
        AfxAssertType(in, afxFcc_URI);
        afxUri excerpt;
        afxNat len = AfxGetUriName(&excerpt, in);

        if (!len) AfxResetUri(uri);
        else
        {
            if (AfxCloneUri(uri, &excerpt))
                AfxThrowError();
        }
    }
    return err;
}

_AFXINL afxError AfxCloneUri(afxUri* uri, afxUri const *in)
{
    afxError err = AFX_ERR_NONE;

    if (!in) AfxResetUri(uri);
    else
    {
        AfxAssertType(in, afxFcc_URI);
        afxNat len = AfxMeasureUri(in);

        if (!len) AfxResetUri(uri);
        else
        {
            AfxAssert(len);
            void const *strData = AfxGetStringData(&in->str.str, 0);
            AfxAssert(strData);

            if (AfxAllocateUri(uri, 0, strData, len))
                AfxThrowError();
        }
    }
    return err;
}

_AFX afxUri* AfxMakeFixedUri8(afxFixedUri8 *uri, afxUri const* src)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(uri);
    uri->buf[0] = '\0';
    AfxMakeBufferedUri(&uri->uri, sizeof(uri->buf), uri->buf, 0);

    if (src)
        AfxCopyUri(&uri->uri, src);

    return &uri->uri;
}

_AFX afxUri* AfxMakeFixedUri128(afxFixedUri128 *uri, afxUri const* src)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(uri);
    uri->buf[0] = '\0';
    AfxMakeBufferedUri(&uri->uri, sizeof(uri->buf), uri->buf, 0);

    if (src)
        AfxCopyUri(&uri->uri, src);

    return &uri->uri;
}

_AFX afxUri* AfxMakeFixedUri2048(afxFixedUri2048 *uri, afxUri const* src)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(uri);
    uri->buf[0] = '\0';
    AfxMakeBufferedUri(&uri->uri, sizeof(uri->buf), uri->buf, 0);

    if (src)
        AfxCopyUri(&uri->uri, src);

    return &uri->uri;
}
