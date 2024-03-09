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
    AfxAssert(uri);
    AfxAssert(str);

    afxString const *strA = AfxGetUriString(uri);

    return AfxCompareString(strA, str);
}

_AFXINL afxResult AfxCompareUriToStringCi(afxUri const *uri, afxString const *str)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(uri);
    AfxAssert(str);

    afxString const *strA = AfxGetUriString(uri);

    return AfxCompareStringCi(strA, str);
}

_AFXINL afxResult AfxCompareUri(afxUri const *uri, afxUri const *other)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(uri);
    AfxAssert(other);

    afxString const *strA = AfxGetUriString(uri), *strB = AfxGetUriString(other);
    return AfxCompareString(strA, strB);
}

_AFXINL afxResult AfxCompareUriCi(afxUri const *uri, afxUri const *other)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(uri);
    AfxAssert(other);

    afxString const *strA = AfxGetUriString(uri), *strB = AfxGetUriString(other);

    return AfxCompareStringCi(strA, strB);
}

_AFXINL afxRestring* AfxGetUriRestring(afxUri *uri)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(uri);
    return &(uri->str);
}

_AFXINL afxString const* AfxGetUriString(afxUri const *uri)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(uri);
    return &(uri->str.str);
}

_AFXINL void* AfxGetUriStorage(afxUri *uri, afxNat base)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(uri);
    return AfxGetStringStorage(&(uri->str), base);
}

_AFXINL void const* AfxGetUriData(afxUri const *uri, afxNat base)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(uri);
    return AfxGetStringData(&(uri->str.str), base);
}

_AFXINL afxNat AfxMeasureUri(afxUri const* uri)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(uri);
    return uri->schem + uri->user + uri->pass + uri->host + uri->port + uri->dir + uri->fname + uri->fext + uri->query + uri->frag;
}

_AFXINL afxNat AfxMeasurePath(afxUri const* uri)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(uri);
    return uri->dir + uri->fname + uri->fext;
}

_AFXINL afxNat AfxMeasureUriRequest(afxUri const* uri)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(uri);
    return uri->dir + uri->fname + uri->fext + uri->query/* + uri->frag*/;
}

_AFXINL afxNat AfxExcerptUriScheme(afxUri const* uri, afxUri* scheme)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(uri);
    AfxAssert(scheme);    
    AfxResetUri(scheme);
    afxNat offset = 0;
    afxNat len = (scheme->schem = uri->schem);
    afxNat cap = offset < uri->str.cap ? uri->str.cap - offset : 0;
    //AfxMakeRestring(&scheme->str, cap, len ? AfxGetStringData(&uri->str.str, offset) : NIL, len);
    AfxExcerptRestring(&uri->str, offset, len, &scheme->str);
    return len;
}

_AFXINL afxNat AfxExcerptUriUser(afxUri const* uri, afxUri* usr)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(uri);
    AfxAssert(usr);
    AfxResetUri(usr);
    afxNat offset = uri->schem;
    afxNat len = (usr->user = uri->user);
    afxNat cap = offset < uri->str.cap ? uri->str.cap - offset : 0;
    //AfxMakeRestring(&usr->str, cap, len ? AfxGetStringData(&uri->str.str, offset) : NIL, len);
    AfxExcerptRestring(&uri->str, offset, len, &usr->str);
    return len;
}

_AFXINL afxNat AfxExcerptUriPassword(afxUri const* uri, afxUri* pass)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(uri);
    AfxAssert(pass);
    AfxResetUri(pass);
    afxNat offset = uri->schem + uri->user;
    afxNat len = (pass->pass = uri->pass);
    afxNat cap = offset < uri->str.cap ? uri->str.cap - offset : 0;
    //AfxMakeRestring(&pass->str, cap, len ? AfxGetStringData(&uri->str.str, offset) : NIL, len);
    AfxExcerptRestring(&uri->str, offset, len, &pass->str);
    return len;
}

_AFXINL afxNat AfxExcerptUriHost(afxUri const* uri, afxUri* host)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(uri);
    AfxAssert(host);
    AfxResetUri(host);
    afxNat offset = uri->schem + uri->user + uri->pass;
    afxNat len = (host->host = uri->host);
    afxNat cap = offset < uri->str.cap ? uri->str.cap - offset : 0;
    //AfxMakeRestring(&host->str, cap, len ? AfxGetStringData(&uri->str.str, offset) : NIL, len);
    AfxExcerptRestring(&uri->str, offset, len, &host->str);
    return len;
}

_AFXINL afxNat AfxExcerptUriPort(afxUri const* uri, afxUri* port)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(uri);
    AfxAssert(port);
    AfxResetUri(port);
    afxNat offset = uri->schem + uri->user + uri->pass + uri->host;
    afxNat len = (port->port = uri->port);
    afxNat cap = offset < uri->str.cap ? uri->str.cap - offset : 0;
    //AfxMakeRestring(&port->str, cap, len ? AfxGetStringData(&uri->str.str, offset) : NIL, len);
    AfxExcerptRestring(&uri->str, offset, len, &port->str);
    return len;
}

_AFXINL afxNat AfxExcerptUriDirectory(afxUri const* uri, afxUri* dir)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(uri);
    AfxAssert(dir);
    AfxResetUri(dir);
    afxNat offset = uri->schem + uri->user + uri->pass + uri->host + uri->port;
    afxNat len = (dir->dir = uri->dir);
    afxNat cap = offset < uri->str.cap ? uri->str.cap - offset : 0;
    //AfxMakeRestring(&dir->str, cap, len ? AfxGetStringData(&uri->str.str, offset) : NIL, len);
    AfxExcerptRestring(&uri->str, offset, len, &dir->str);
    return len;
}

_AFXINL afxNat AfxExcerptUriName(afxUri const* uri, afxUri* name)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(uri);
    AfxAssert(name);
    AfxResetUri(name);
    afxNat offset = uri->schem + uri->user + uri->pass + uri->host + uri->port + uri->dir;
    afxNat len = (name->fname = uri->fname);
    afxNat cap = offset < uri->str.cap ? uri->str.cap - offset : 0;
    //AfxMakeRestring(&name->str, cap, len ? AfxGetStringData(&uri->str.str, offset) : NIL, len);
    AfxExcerptRestring(&uri->str, offset, len, &name->str);
    return len;
}

_AFXINL afxNat AfxExcerptUriNameToString(afxUri const* uri, afxString* name)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(uri);
    AfxAssert(name);
    afxUri tmp;
    AfxExcerptUriName(uri, &tmp);
    AfxReflectString(AfxGetUriString(&tmp), name);
    return name->len;
}

_AFXINL afxNat AfxExcerptUriExtension(afxUri const* uri, afxBool skipDot, afxUri* ext)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(uri);
    AfxAssert(ext);
    AfxResetUri(ext);
    afxNat additive = skipDot && uri->fext ? 1 : 0;
    afxNat offset = uri->schem + uri->user + uri->pass + uri->host + uri->port + uri->dir + uri->fname + additive;
    afxNat len = (ext->fext = uri->fext - additive);
    afxNat cap = offset < uri->str.cap ? uri->str.cap - offset : 0;
    //AfxMakeRestring(&ext->str, cap, len ? AfxGetStringData(&uri->str.str, offset) : NIL, len);
    AfxExcerptRestring(&uri->str, offset, len, &ext->str);
    return len;
}

_AFXINL afxNat AfxExcerptUriFile(afxUri const* uri, afxUri* file)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(uri);
    AfxAssert(file);
    AfxResetUri(file);
    afxNat offset = uri->schem + uri->user + uri->pass + uri->host + uri->port + uri->dir;
    afxNat len = (file->fname = uri->fname) + (file->fext = uri->fext);
    afxNat cap = offset < uri->str.cap ? uri->str.cap - offset : 0;
    //AfxMakeRestring(&file->str, cap, len ? AfxGetStringData(&uri->str.str, offset) : NIL, len);
    AfxExcerptRestring(&uri->str, offset, len, &file->str);
    return len;
}

_AFXINL afxNat AfxExcerptUriPath(afxUri const* uri, afxUri* path)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(uri);
    AfxAssert(path);
    AfxResetUri(path);
    afxNat offset = uri->schem + uri->user + uri->pass + uri->host + uri->port;
    afxNat len = (path->dir = uri->dir) + (path->fname = uri->fname) + (path->fext = uri->fext);
    afxNat cap = offset < uri->str.cap ? uri->str.cap - offset : 0;
    //AfxMakeRestring(&path->str, cap, len ? AfxGetStringData(&uri->str.str, offset) : NIL, len);
    AfxExcerptRestring(&uri->str, offset, len, &path->str);
    return len;
}

_AFXINL afxNat AfxExcerptUriQuery(afxUri const* uri, afxBool skipSep, afxUri* query)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(uri);
    AfxAssert(query);
    AfxResetUri(query);
    afxNat additive = skipSep && uri->query ? 1 : 0;
    afxNat offset = uri->schem + uri->user + uri->pass + uri->host + uri->port + uri->dir + uri->fname + uri->fext + additive;
    afxNat len = (query->query = uri->query - additive);
    afxNat cap = offset < uri->str.cap ? uri->str.cap - offset : 0;
    //AfxMakeRestring(&query->str, cap, len ? AfxGetStringData(&uri->str.str, offset) : NIL, len);
    AfxExcerptRestring(&uri->str, offset, len, &query->str);
    return len;
}

_AFXINL afxNat AfxExcerptUriQueryToString(afxUri const* uri, afxBool skipSep, afxString* query)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(uri);
    AfxAssert(query);
    afxUri tmp;
    AfxExcerptUriQuery(uri, skipSep, &tmp);
    AfxReflectString(AfxGetUriString(&tmp), query);
    return query->len;
}

_AFXINL afxNat AfxExcerptUriFragment(afxUri const* uri, afxBool skipSep, afxUri* frag)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(uri);
    AfxAssert(frag);
    AfxResetUri(frag);
    afxNat additive = skipSep && uri->frag ? 1 : 0;
    afxNat offset = uri->schem + uri->user + uri->pass + uri->host + uri->port + uri->dir + uri->fname + uri->fext + uri->query + additive;
    afxNat len = (frag->frag = uri->frag - additive);
    afxNat cap = offset < uri->str.cap ? uri->str.cap - offset : 0;
    //AfxMakeRestring(&frag->str, cap, len ? AfxGetStringData(&uri->str.str, offset) : NIL, len);
    AfxExcerptRestring(&uri->str, offset, len, &frag->str);
    return len;
}

_AFXINL afxError AfxCopyUriScheme(afxUri *uri, afxUri const *src)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(uri);
    AfxAssert(src);

    afxUri excerpt;
    AfxExcerptUriScheme(src, &excerpt);

    if (AfxCopyUri(uri, &excerpt))
        AfxThrowError();

    return err;
}

_AFXINL afxError AfxCopyUriUsername(afxUri *uri, afxUri const *src)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(uri);
    AfxAssert(src);

    afxUri excerpt;
    AfxExcerptUriUser(src, &excerpt);

    if (AfxCopyUri(uri, &excerpt))
        AfxThrowError();

    return err;
}

_AFXINL afxError AfxCopyUriPassword(afxUri *uri, afxUri const *src)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(uri);
    AfxAssert(src);

    afxUri excerpt;
    AfxExcerptUriPassword(src, &excerpt);

    if (AfxCopyUri(uri, &excerpt))
        AfxThrowError();

    return err;
}

_AFXINL afxError AfxCopyUriHost(afxUri *uri, afxUri const *src)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(uri);
    AfxAssert(src);

    afxUri excerpt;
    AfxExcerptUriHost(src, &excerpt);

    if (AfxCopyUri(uri, &excerpt))
        AfxThrowError();

    return err;
}

_AFXINL afxError AfxCopyUriPort(afxUri *uri, afxUri const *src)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(uri);
    AfxAssert(src);

    afxUri excerpt;
    AfxExcerptUriPort(src, &excerpt);

    if (AfxCopyUri(uri, &excerpt))
        AfxThrowError();

    return err;
}

_AFXINL afxError AfxCopyUriDirectory(afxUri *uri, afxUri const *src)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(uri);
    AfxAssert(src);

    afxUri excerpt;
    AfxExcerptUriDirectory(src, &excerpt);

    if (AfxCopyUri(uri, &excerpt))
        AfxThrowError();

    return err;
}

_AFXINL afxError AfxCopyUriObject(afxUri *uri, afxUri const *src)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(uri);
    AfxAssert(src);

    afxUri excerpt;
    AfxExcerptUriFile(src, &excerpt);

    if (AfxCopyUri(uri, &excerpt))
        AfxThrowError();

    return err;
}

_AFXINL afxError AfxCopyUriName(afxUri *uri, afxUri const *src)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(uri);
    AfxAssert(src);

    afxUri excerpt;
    AfxExcerptUriName(src, &excerpt);

    if (AfxCopyUri(uri, &excerpt))
        AfxThrowError();

    return err;
}

_AFXINL afxError AfxCopyUriExtension(afxUri *uri, afxUri const *src, afxBool skipDot)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(uri);
    AfxAssert(src);

    afxUri excerpt;
    AfxExcerptUriExtension(src, skipDot, &excerpt);

    if (AfxCopyUri(uri, &excerpt))
        AfxThrowError();

    return err;
}

_AFXINL afxError AfxCopyUriPath(afxUri *uri, afxUri const *src)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(uri);
    AfxAssert(src);

    afxUri excerpt;
    AfxExcerptUriPath(src, &excerpt);

    if (AfxCopyUri(uri, &excerpt))
        AfxThrowError();

    return err;
}

_AFXINL afxError AfxCopyUriQuery(afxUri *uri, afxUri const *src, afxBool skipSep)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(uri);
    AfxAssert(src);

    afxUri excerpt;
    AfxExcerptUriQuery(src, skipSep, &excerpt);

    if (AfxCopyUri(uri, &excerpt))
        AfxThrowError();

    return err;
}

_AFXINL afxError AfxCopyUriFragment(afxUri *uri, afxUri const *src, afxBool skipSep)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(uri);
    AfxAssert(src);

    afxUri excerpt;
    AfxExcerptUriFragment(src, skipSep, &excerpt);
    
    if (AfxCopyUri(uri, &excerpt))
        AfxThrowError();

    return err;
}

_AFXINL afxBool AfxPathIsRelative(afxUri const *uri)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(uri);

    afxUri excerpt;
    AfxExcerptUriPath(uri, &excerpt);
    afxByte const* data = AfxGetUriData(&excerpt, 0);

    return (!data || (data[0] != '/' && data[1] != ':'));
}

_AFXINL afxBool AfxUriIsEditable(afxUri const *uri)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(uri);
    return AfxStringIsWriteable(&uri->str);
}

_AFXINL afxBool AfxUriIsBlank(afxUri const *uri)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(uri);
    return 0 == (uri->schem + uri->user + uri->pass + uri->host + uri->port + uri->dir + uri->fname + uri->fext + uri->query + uri->frag);
}

_AFXINL afxResult _AfxFindUriQueryAttribute(afxString *attr, afxString *value, void* data)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(attr);
    AfxAssert(value);
    AfxAssert(data);
    struct { afxString const* key; afxRestring* dst; afxBool success; } *data2 = data;

    if (0 == AfxCompareString(data2->key, attr))
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
    AfxAssert(uri);
    AfxAssert(f);
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
    AfxAssert(uri);
    AfxAssert(keys);
    AfxAssert(base == 0);
    AfxAssert(cnt);
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

_AFXINL void AfxCanonicalizePath(afxUri* uri, afxBool microshit)
{
    // c:\shit\\\by\\M$\foo.poo -> /c/shit/by/M$/unshitted/foo.poo

    afxError err = AFX_ERR_NONE;
    AfxAssert(uri);
    AfxAssert(AfxUriIsEditable(uri));
    afxUri path, query, frag;
    
    afxChar const dirSep = microshit ? '\\' : '/';

    if (AfxExcerptUriPath(uri, &path))
    {
        AfxExcerptUriQuery(uri, FALSE, &query);
        AfxExcerptUriFragment(uri, FALSE, &frag);

        afxRestring *pathStr = AfxGetUriRestring(&path);

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
        
        if (!microshit)
            if (src2[0] != 0 && src2[1] == ':')
                src2 += 2;
        
        // Initialize buffer
        ptr = dst;
        end = dst + sizeof(dst);

#if 0
        // Add current directory to filename if relative path
        if (src2[0] == '/' || src2[0] == '\\')
        {
            struct thread *t = self();
            // Do not add current directory if it is root directory
            len = strlen(t->curdir);

            if (len > 1)
            {
                memcpy(p, t->curdir, len);
                p += len;
            }
        }
#endif

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
                // We do not allow control characters in filenames
                if (src2[0] > 0 && src2[0] < ' ')
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
            if (len == 0 && src2[-1] != '/' && src2[-1] != '\\') ptr--;
            else if (len == 1 && src2[-1] == '.') ptr -= 2;
            else if (len == 2 && src2[-1] == '.' && src2[-2] == '.')
            {
                ptr -= 4;

                if (ptr < dst)
                {
                    AfxThrowError(); //errno = EINVAL;
                    return;
                }
                while (ptr[0] != /*'/'*/dirSep) ptr--;
            }
        }

        // Convert empty filename to /
        if (ptr == dst)
        {
            *ptr++ = /*'/'*/dirSep;
        }

        // Terminate string
        if (ptr == end)
        {
            AfxThrowError(); //errno = ENAMETOOLONG;
            return;
        }

        *ptr = 0;

        _AfxWriteString(pathStr, 0, dst, ptr - dst); // TODO move query and fragment to end of path, once normalization never increase size of string.
        afxByte *_bytemap = AfxGetStringStorage(pathStr, 0);
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
        if (query.query)
            AfxConcatenateString(pathStr, AfxGetUriString(&query));

        if (frag.frag)
            AfxConcatenateString(pathStr, AfxGetUriString(&frag));

        AfxReparseUri(uri);
    }
}

_AFXINL void AfxReparseUri(afxUri *uri)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(uri);

    afxChar* data = AfxGetStringStorage(&uri->str, 0);

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
    AfxAssert(uri);
    AfxAssert(AfxUriIsEditable(uri));

    AfxAssert(fmt);

    afxRestring *str = AfxGetUriRestring(uri);
    AfxAssert(str);

    va_list args;
    va_start(args, fmt);
    AfxFormatStringArg(str, fmt, args);
    va_end(args);
    AfxReparseUri(uri);
    //AfxCanonicalizePath(uri, FALSE);
}

_AFXINL afxError AfxCopyUri(afxUri *uri, afxUri const *src)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(src);
    AfxAssert(uri);
    AfxAssert(AfxUriIsEditable(uri));
    afxNat clampedOff = 0;

    if ((clampedOff = AfxCopyString(&uri->str, &src->str.str)))
    {
        AfxThrowError();
        
    }
    AfxReparseUri(uri); // Strings can have differently sized buffers. So we recalc it instead of just copy offsets and ranges.
    return err;
}

_AFXINL void AfxReplicateUri(afxUri *uri, afxUri const *in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(in);
    AfxAssert(uri);

    AfxResetUri(uri);
    AfxReflectString(&(in->str.str), &(uri->str.str));

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
    AfxReflectString(src, &uri->str.str);
    AfxReparseUri(uri);
    //AfxCanonicalizePath(uri);  // can't change static data, will cause access violation
    return AfxMeasureUri(uri);
}

_AFXINL afxNat AfxMakeUriFromRestring(afxUri *uri, afxRestring const *src)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(uri);

    AfxResetUri(uri);
    AfxReflectRestring(src, &uri->str);
    AfxReparseUri(uri);
    AfxCanonicalizePath(uri, FALSE);  // can't change static data, will cause access violation
    return AfxMeasureUri(uri);
}

_AFXINL afxNat AfxUriFromStringRange(afxUri *uri, afxString const *src, afxNat offset, afxNat len)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(uri);

    AfxResetUri(uri);
    afxNat len2 = AfxExcerptString(src, offset, len, &uri->str.str);
    AfxReparseUri(uri);
    //AfxCanonicalizePath(uri);  // can't change static data, will cause access violation
    return len2;
}

_AFXINL afxNat AfxMakeUriFromRestringRange(afxUri *uri, afxRestring const *src, afxNat offset, afxNat len)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(uri);

    AfxResetUri(uri);
    afxNat len2 = AfxExcerptString(&src->str, offset, len, &uri->str.str);
    uri->str.cap = src->cap;
    AfxReparseUri(uri);
    AfxCanonicalizePath(uri, FALSE);  // can't change static data, will cause access violation
    return len2;
}

_AFXINL void AfxMakeUri(afxUri *uri, void const *start, afxNat len)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(uri);
    AfxResetUri(uri);
    AfxMakeString(&uri->str.str, start, len);

    if (uri->str.str.len)
        AfxReparseUri(uri);

    //AfxCanonicalizePath(uri); // can't change static data, it will cause access violation
}

_AFXINL void AfxMakeMutableUri(afxUri *uri, afxNat cap, void *start, afxNat len)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(uri);
    AfxResetUri(uri);
    AfxMakeRestring(&uri->str, cap, start, len);
    AfxAssert(AfxUriIsEditable(uri));

    if (uri->str.str.len)
    {
        AfxReparseUri(uri);
        AfxCanonicalizePath(uri, FALSE);
    }
}

_AFXINL void AfxResetUri(afxUri *uri)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(uri);

    AfxResetRestring(&uri->str);

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
    AfxAssert(uri);

    AfxClearString(&uri->str);

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
    AfxAssert(uri);
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
    void *buf = NIL;

    if (start && !range)
        range = AfxStrlen(start);

    AfxAssert(!range || (range && start));

    {
        AfxAssert(uri);
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
        AfxAssert(in);
        afxUri excerpt;
        afxNat len = AfxExcerptUriPath(in, &excerpt);

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
        AfxAssert(in);
        afxUri excerpt;
        afxNat len = AfxExcerptUriName(in, &excerpt);

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
        AfxAssert(in);
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

_AFX afxUri* AfxMakeUri8(afxUri8 *uri, afxUri const* src)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(uri);
    uri->buf[0] = '\0';
    AfxMakeMutableUri(&uri->uri, sizeof(uri->buf), uri->buf, 0);

    if (src)
        AfxCopyUri(&uri->uri, src);

    return &uri->uri;
}

_AFX afxUri* AfxMakeUri128(afxUri128 *uri, afxUri const* src)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(uri);
    uri->buf[0] = '\0';
    AfxMakeMutableUri(&uri->uri, sizeof(uri->buf), uri->buf, 0);

    if (src)
        AfxCopyUri(&uri->uri, src);

    return &uri->uri;
}

_AFX afxUri* AfxMakeUri2048(afxUri2048 *uri, afxUri const* src)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(uri);
    uri->buf[0] = '\0';
    AfxMakeMutableUri(&uri->uri, sizeof(uri->buf), uri->buf, 0);

    if (src)
        AfxCopyUri(&uri->uri, src);

    return &uri->uri;
}
