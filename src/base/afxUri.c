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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "../exec/afxSystemDDK.h"

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
    char const*h;
    int hostsiz;
    char const*p;
    int portsiz;
    char const*path;
    int pathsiz; // whole path size
    char const*root;
    int rootsiz;
    char const*fdir;
    int fdirsiz;
    char const*fobj;
    int fobjsiz;
    char const*fext;
    int fextsiz;
    char const*qry;
    int querysiz;
    char const*fragment;
    int fragsiz;
} url_field_t;

// AUXILIAR FUNCTIONS /////////////////////////////////////////////////////////

#if 0
_AFXINL int host_is_ipv4(char const*s)
{
    if (!s) return 0;
    while (*s)
    {
        if ((*s >= '0' && *s <= '9') || *s == '.')
            s++;
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
        if (url->qry)
            url->qry = realloc(url->qry, (url->query_num + 1) * sizeof(*url->qry));
        else
            url->qry = malloc(sizeof(*url->qry));
        url->qry[url->query_num].name = strndup(query, chr - query);
        query = chr + 1;
        chr = strchr(query, '&');
        if (chr)
        {
            url->qry[url->query_num].value = strndup(query, chr - query);
            url->query_num++;
            query = chr + 1;
            chr = strchr(query, '=');
        }
        else
        {
            url->qry[url->query_num].value = strndup(query, -1);
            url->query_num++;
            break;
        }
    }
}
#endif

_AFXINL afxResult _AfxUrlParseRaw(const char *s, afxUnit strLenTotal, url_field_t *url)
// This is a "you gave me bytes, I'll give you fields" mode low-level C function.
{
    afxUnit strLen = strLenTotal;
    const char *pch, *start = s, *end = s + strLen;
    //*url = (url_field_t) { 0 };

    if (s && s[0] && strLen)
    {
        url->href = s;
        url->hrefsiz = strLen;// strlen(s);

        pch = memchr(s, ':', (end - s));   /* parse schema */

        if (pch && pch[1] == '/' && pch[2] == '/')
        {
            url->schema = s;
            url->schemasiz = pch - s;
            strLen -= url->schemasiz;
            s = pch + 3;

            pch = memchr(s, '@', (end - s));   /* parse user info */

            if (pch)
            {
                pch = memchr(s, ':', (end - s));

                if (pch)
                {
                    url->username = s;
                    url->usersiz = pch - s;
                    strLen -= url->usersiz;

                    s = pch + 1;
                    pch = memchr(s, '@', (end - s));

                    if (pch)
                    {
                        url->password = s;
                        url->passsiz = pch - s;
                        strLen -= url->passsiz;
                        s = pch + 1;
                    }
                    else goto __fail;
                }
                else goto __fail;
            }

            if (s[0] == '[')        /* parse host info */
            {
                s++;
                pch = memchr(s, ']', (end - s));

                if (pch)
                {
                    url->h = s;
                    url->hostsiz = pch - s;
                    strLen -= url->hostsiz;
                    s = pch + 1;

                    if (s[0] == ':')
                    {
                        s++;
                        pch = memchr(s, '/', (end - s));

                        if (pch)
                        {
                            url->p = s;
                            url->portsiz = pch - s;
                            strLen -= url->portsiz;
                            s = pch /*+ 1*/;
                        }
                        else
                        {
                            url->p = s;
                            url->portsiz = strLen - (s - start);// strlen(s);
                            strLen -= url->portsiz;
                            s = s + strLen - (s - start);//strlen(s);
                        }
                    }
                    url->host_type = HOST_IPV6;
                }
                else goto __fail;
            }
            else
            {
                const char *pch_slash;
                pch = memchr(s, ':', (end - s));
                pch_slash = memchr(s, '/', (end - s));

                if (pch && (!pch_slash || (pch_slash && pch < pch_slash)))
                {
                    url->h = s;
                    url->hostsiz = pch - s;
                    strLen -= url->hostsiz;
                    s = pch + 1;
                    pch = memchr(s, '/', (end - s));

                    if (pch)
                    {
                        url->p = s;
                        url->portsiz = pch - s;
                        strLen -= url->portsiz;
                        s = pch /*+ 1*/;
                    }
                    else
                    {
                        url->p = s;
                        url->portsiz = strLen - (s - start);//strlen(s);
                        strLen -= url->portsiz;
                        s = s + strLen - (s - start);//strlen(s);
                    }
                }
                else
                {
                    pch = memchr(s, '/', (end - s));

                    if (pch)
                    {
                        url->h = s;
                        url->hostsiz = pch - s;
                        strLen -= url->hostsiz;
                        s = pch/* + 1*/;
                    }
                    else
                    {
                        url->h = s;
                        url->hostsiz = strLen - (s - start);//strlen(s);
                        strLen -= url->hostsiz;
                        s = s + strLen - (s - start);//strlen(s);
                    }
                }
                url->host_type = 0;//host_is_ipv4(url->h) ? HOST_IPV4 : HOST_DOMAIN;
            }
        }

        if (s[0]) /* parse path, query and fragment */
        {
            url->path = s;
            pch = memchr(s, '?', (end - s));

            if (pch) // has query
            {
                //url->path = s;
                url->pathsiz = pch - s;
                strLen -= url->pathsiz;
                s = pch/* + 1*/;
                url->qry = pch;
                pch = memchr(s, '#', (end - s));

                if (pch) // has fragment
                {
                    url->querysiz = pch - url->qry;
                    strLen -= url->querysiz;
                    s = pch/* + 1*/;
                    url->fragment = pch;
                    url->fragsiz = end - url->fragment;//strlen(s);
                    strLen -= url->fragsiz;
                }
                else
                {
                    // has query but no fragment

                    //url->qry = s;
                    url->querysiz = end - url->qry;//strlen(s);
                    strLen -= url->querysiz;
                    //s = s + strLen - (s - start);//strlen(s);
                    url->fragment = 0;
                    url->fragsiz = 0;
                    strLen -= url->fragsiz;
                }
            }
            else
            {
                // doesnt has query
                url->qry = 0;
                url->querysiz = 0;
                strLen -= url->querysiz;
                pch = memchr(s, '#', (end - s));

                if (pch) // has fragment but no query
                {
                    //url->path = s;
                    url->pathsiz = pch - s;
                    strLen -= url->pathsiz;
                    s = pch/* + 1*/;
                    url->fragment = pch;
                    url->fragsiz = end - url->fragment;//strlen(s);
                    strLen -= url->fragsiz;
                    //s = s + strLen - (s - start);//strlen(s);
                }
                else
                {
                    // doesnt has fragment nor query

                    //url->path = s;
                    url->pathsiz = (end - s);//strlen(s);
                    strLen -= url->pathsiz;
                    //s = s + strLen - (s - start);//strlen(s);
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

#define _offset(ptr, base) ((ptr) ? (afxUnit8)((ptr) - (base)) : 0)

static void _parse_authority(afxString authority, afxUri* uri, char const* base) {
    const char* s = authority.start;
    const char* end = s + authority.len;

    if (!authority.len)
        return;

    const char* at = memchr(s, '@', end - s);
    const char* host_start = s;

    if (at && at < end) {
        uri->user = _offset(s, base);
        host_start = at + 1;
    }

    const char* colon = memchr(host_start, ':', end - host_start);
    if (colon && colon < end) {
        uri->h = _offset(host_start, base);
        if (colon + 1 < end)
            uri->p = _offset(colon + 1, base);
    }
    else {
        uri->h = _offset(host_start, base);
    }
}

static void _parse_path_and_file(afxString path, afxUri* uri, char const* base) {
    if (!path.len) return;

    const char* p = path.start;
    const char* end = p + path.len;

    const char* root = NULL;
    afxUnit root_len = 0;

    // --- MS-DOS drive letter (e.g., C:)
    if ((end - p) >= 2 &&
        ((p[0] >= 'A' && p[0] <= 'Z') || (p[0] >= 'a' && p[0] <= 'z')) &&
        p[1] == ':') {
        root = p;
        root_len = 2;
    }
    // --- Unix-style root or //./ prefix
    else if (p[0] == '/' || p[0] == '\\') {
        root = p;
        root_len = 1;

        if ((end - p) >= 7 &&
            (p[1] == '/' || p[1] == '\\') &&
            p[2] == '.' &&
            (p[3] == '/' || p[3] == '\\')) {
            const char* ext = p + 4;
            while (ext < end && *ext != '/' && *ext != '\\') ++ext;
            root_len = ext - p;
        }
    }

    if (root)
        uri->root = (afxUnit8)(root - base);

    const char* content = p + root_len;
    if (content >= end) return;

    const char* last_slash = NULL;
    const char* last_dot = NULL;

    for (const char* it = end - 1; it >= content; --it) {
        if (!last_dot && *it == '.') last_dot = it;
        if (*it == '/' || *it == '\\') {
            last_slash = it;
            break;
        }
    }

    // --- directory
    if (last_slash) {
        uri->dir = (afxUnit16)(last_slash - base + 1);

        if (last_slash + 1 < end) {
            uri->fname = (afxUnit8)((last_slash + 1) - base);
        }
        else {
            uri->fname = 0;
            uri->fext = 0;
            return;
        }
    }
    else {
        uri->dir = 0;
        if (content < end) {
            uri->fname = (afxUnit8)(content - base);
        }
        else {
            uri->fname = 0;
            uri->fext = 0;
            return;
        }
    }

    // --- extension
    if (uri->fname) {
        const char* fname_start = base + uri->fname;
        const char* fname_end = end;

        if (last_dot &&
            last_dot > fname_start &&
            last_dot < fname_end - 1) {
            uri->fext = (afxUnit8)(last_dot - base);
        }
        else {
            uri->fext = 0;
        }
    }
    else {
        uri->fext = 0;
    }
}

#if 0
afxResult _AfxParseUriParts(afxUri* uri, afxString const* src)
{
    if (!src || !uri)
        return 0;

    url_field_t temp = { 0 };

    const char *cur = src->start;
    const char *end = src->start + src->len;

    // 1. Parse scheme
    const char *pch = strchr(cur, ':');
    if (pch && (pch[1] == '/' || pch[1] == '\\')) {
        temp.schema = cur;
        temp.schemasiz = pch - cur;
        cur = pch + 1;

        // support '://'
        if (*cur == '/' || *cur == '\\') ++cur;
        if (*cur == '/' || *cur == '\\') ++cur;

        // 2. Optional userinfo
        const char *at = strchr(cur, '@');
        const char *slash = strchr(cur, '/');
        if (at && (!slash || at < slash)) {
            const char *colon = memchr(cur, ':', at - cur);
            if (colon) {
                temp.username = cur;
                temp.usersiz = colon - cur;
                // password skipped
            }
            else {
                temp.username = cur;
                temp.usersiz = at - cur;
            }
            cur = at + 1;
        }

        // 3. Host and port
        if (*cur == '[') { // IPv6
            ++cur;
            const char *close = strchr(cur, ']');
            if (!close) return 0;
            temp.h = cur;
            temp.hostsiz = close - cur;
            cur = close + 1;
        }
        else {
            const char *colon = strchr(cur, ':');
            const char *slash2 = strchr(cur, '/');
            if (colon && (!slash2 || colon < slash2)) {
                temp.h = cur;
                temp.hostsiz = colon - cur;
                cur = colon + 1;
                const char *port_end = slash2 ? slash2 : end;
                temp.p = cur;
                temp.portsiz = port_end - cur;
                cur = port_end;
            }
            else {
                const char *host_end = slash2 ? slash2 : end;
                temp.h = cur;
                temp.hostsiz = host_end - cur;
                cur = host_end;
            }
        }
    }

    // 4. Query and fragment
    const char *query = memchr(cur, '?', end - cur);
    const char *frag = memchr(cur, '#', end - cur);

    const char *path_end = query ? query : (frag ? frag : end);
    const char *path = cur;

    // 5. Root detection
    if ((path_end - path) >= 2 &&
        ((path[0] >= 'A' && path[0] <= 'Z') || (path[0] >= 'a' && path[0] <= 'z')) &&
        path[1] == ':') {
        temp.root = path;
        temp.rootsiz = 2;
        cur = path + 2;
    }
    else if (*path == '/' || *path == '\\') {
        temp.root = path;
        temp.rootsiz = 1;

        if ((path_end - path) >= 7 &&
            (path[1] == '/' || path[1] == '\\') &&
            path[2] == '.' &&
            (path[3] == '/' || path[3] == '\\')) {
            const char *ext = path + 4;
            while (ext < path_end && *ext != '/' && *ext != '\\') ++ext;
            temp.rootsiz = ext - path;
        }

        cur = path + temp.rootsiz;
    }
    else {
        cur = path;
    }

    // 6. Directory, filename, extension
    const char *last_slash = NULL, *last_dot = NULL;
    for (const char *it = path_end - 1; it >= cur; --it) {
        if (!last_dot && *it == '.') last_dot = it;
        if (*it == '/' || *it == '\\') {
            last_slash = it;
            break;
        }
    }

    if (last_slash) {
        temp.fdir = cur;
        temp.fdirsiz = last_slash - cur + 1;
        if (last_slash + 1 < path_end) {
            temp.fobj = last_slash + 1;
            temp.fobjsiz = path_end - (last_slash + 1);
        }
    }
    else {
        temp.fobj = cur;
        temp.fobjsiz = path_end - cur;
    }

    if (temp.fobj && last_dot && last_dot > temp.fobj && last_dot < path_end - 1) {
        temp.fext = last_dot;
        temp.fextsiz = path_end - last_dot;
    }

    // 7. Assign query and fragment
    if (query) {
        const char *query_end = frag ? frag : end;
        temp.qry = query;
        temp.querysiz = query_end - query;
    }
    if (frag) {
        temp.fragment = frag;
        temp.fragsiz = end - frag;
    }

    // 8. Final offsets into afxUri
    uri->s.start = src->start;
    uri->s.len = (afxUnit)(end - src->start);
    uri->s.cap = uri->s.len;

    uri->sch = temp.schemasiz;
    uri->user = temp.usersiz;
    uri->h = temp.hostsiz;
    uri->p = temp.portsiz;
    uri->root = temp.rootsiz;
    uri->dir = temp.fdirsiz;
    uri->fname = temp.fobjsiz;
    uri->fext = temp.fextsiz;
    uri->qry = temp.querysiz;
    uri->frag = temp.fragsiz;

    return 1;
}
#endif
afxResult _AfxParseUriParts(afxUri* uri)
{
    if (!uri) return 0;

    afxChar const* s = uri->s.start;
    afxUnit len = uri->s.len;
    uri->s.start = s;
    uri->s.len = len;
    //uri->s.cap = len;

    uri->sch = uri->user = uri->h = uri->p = uri->root = 0;
    uri->dir = uri->fname = uri->fext = uri->qry = uri->frag = 0;

    char const *cur = s;
    char const *end = s + len;

    // --- Scheme detection (e.g., file://, http://, etc.)
    char const *colon = memchr(cur, ':', end - cur);
    if (colon && colon[1] == '/' && colon[2] == '/') {
        uri->sch = (afxUnit8)(colon - cur);
        cur = colon + 3; // Skip "://"

        // --- User info
        char const *at = memchr(cur, '@', end - cur);
        char const *slash = memchr(cur, '/', end - cur);
        if (at && (!slash || at < slash)) {
            char const *colon2 = memchr(cur, ':', at - cur);
            uri->user = (afxUnit8)((colon2 ? colon2 : at) - cur);
            cur = at + 1;
        }

        // --- Host and Port
        if (*cur == '[') {
            char const *close = memchr(cur, ']', end - cur);
            if (!close) return 0;
            uri->h = (afxUnit8)(close - cur - 1);
            cur = close + 1;
        }
        else {
            char const *host_start = cur;
            while (cur < end && *cur != ':' && *cur != '/' && *cur != '?' && *cur != '#') ++cur;
            uri->h = (afxUnit8)(cur - host_start);
        }

        if (*cur == ':') {
            ++cur;
            char const *port_start = cur;
            while (cur < end && *cur != '/' && *cur != '?' && *cur != '#') ++cur;
            uri->p = (afxUnit8)(cur - port_start);
        }
    }

    // --- Path Start
    char const *path = cur;
    char const *query = memchr(path, '?', end - path);
    char const *frag = memchr(path, '#', end - path);
    char const *path_end = query ? query : (frag ? frag : end);

    // --- Detect Root (e.g., "/", "C:", "//./")
    if ((path_end - path) >= 2 &&
        ((path[0] >= 'A' && path[0] <= 'Z') || (path[0] >= 'a' && path[0] <= 'z')) &&
        path[1] == ':') {
        uri->root = 2;
        path += 2;
    }
    else if ((path_end - path) >= 1 && (path[0] == '/' || path[0] == '\\')) {
        uri->root = 1;
        if ((path_end - path) >= 4 &&
            (path[1] == '/' || path[1] == '\\') &&
            path[2] == '.' &&
            (path[3] == '/' || path[3] == '\\')) {
            // Special Windows-style path "//./dev"
            char const *it = path + 4;
            while (it < path_end && *it != '/' && *it != '\\') ++it;
            uri->root = (afxUnit8)(it - path);
        }
        path += uri->root;
    }

    // --- Reverse search for last '/' or '.' in path
    char const *last_slash = NULL;
    char const *last_dot = NULL;
    for (char const *it = path_end - 1; it >= path; --it) {
        if (!last_dot && *it == '.') last_dot = it;
        if (*it == '/' || *it == '\\') {
            last_slash = it;
            break;
        }
    }

    // --- Directory
    // Calculate directory first
    if (last_slash != NULL) {
        // If there's a slash, the directory is everything before it
        uri->dir = (afxUnit16)(last_slash - path + 1); // Length of directory
    }
    else {
        // If there's no slash, we set fdir to 0 (no directory)
        uri->dir = 0;
    }

    char const* fname_start = last_slash ? last_slash + 1 : path;
    afxUnit fname_len = (afxUnit)(path_end - fname_start);
    afxUnit fext_len = 0;

    if (last_dot &&
        last_dot >= fname_start &&
        last_dot < path_end) // Not at very end
    {
        uri->fname = (afxUnit8)(last_dot - fname_start);
        uri->fext = (afxUnit8)(path_end - last_dot);
    }
    else {
        uri->fname = (afxUnit8)(path_end - fname_start);
        uri->fext = 0;
    }

    afxError err = { 0 };
    AFX_ASSERT(uri->fext <= 5l);

    // --- Query
    if (query) {
        char const *qend = frag ? frag : end;
        uri->qry = (afxUnit16)(qend - query);
    }

    // --- Fragment
    if (frag) {
        uri->frag = (afxUnit8)(end - frag);
    }

    return 1;
}

////////////////////////////////////////////////////////////

#define _URI_AUTH_LEN(uri) (uri->user + uri->h + uri->p)
#define _URI_PATH_LEN(uri) (uri->root + uri->dir + uri->fname + uri->fext) 
#define _URI_REQ_LEN(uri) (_URI_PATH_LEN(uri) + uri->qry) 

#define _URI_AUTH_OFF(uri) (uri->sch)
#define _URI_PATH_OFF(uri) (uri->sch + _URI_AUTH_LEN(uri)) 
#define _URI_QUERY_OFF(uri) (_URI_PATH_OFF(uri) + uri->root + uri->dir + uri->fname + uri->fext) 
#define _URI_FRAG_OFF(uri) (_URI_QUERY_OFF(uri) + uri->qry) 

_AFXINL void const* AfxGetUriData(afxUri const* uri, afxUnit base)
{
    afxError err = { 0 };
    AFX_ASSERT(uri);
    return AfxGetStringData(&uri->s, base);
    int a = sizeof(afxUri);
}

_AFXINL afxUnit AfxGetUriLength(afxUri const* uri)
{
    afxError err = { 0 };
    AFX_ASSERT(uri);
    return uri->sch + uri->user/* + uri->pass*/ + uri->h + uri->p + _URI_REQ_LEN(uri) + uri->frag;
}

_AFXINL afxUnit AfxGetPathLength(afxUri const* uri)
{
    afxError err = { 0 };
    AFX_ASSERT(uri);
    return _URI_PATH_LEN(uri);
}

_AFXINL afxUnit AfxGetUriRequestLength(afxUri const* uri)
{
    afxError err = { 0 };
    AFX_ASSERT(uri);
    return _URI_REQ_LEN(uri);
}

_AFXINL afxString const* AfxGetUriString(afxUri const* uri)
{
    afxError err = { 0 };
    AFX_ASSERT(uri);
    return &uri->s;
}

_AFXINL afxString* AfxGetUriString2(afxUri* uri)
{
    afxError err = { 0 };
    AFX_ASSERT(uri);
    return &uri->s;
}

// TESTING

_AFXINL afxBool AfxUriIsEditable(afxUri const* uri)
{
    afxError err = { 0 };
    AFX_ASSERT(uri);
    return AfxGetStringCapacity(&uri->s, 0);
}

_AFXINL afxBool AfxIsUriBlank(afxUri const* uri)
{
    afxError err = { 0 };
    AFX_ASSERT(uri);
    return 0 == (uri->sch + uri->user/* + uri->pass*/ + uri->h + uri->p + uri->root + uri->dir + uri->fname + uri->fext + uri->qry + uri->frag);
}

// COMPARISON

_AFXINL afxResult AfxCompareUriString(afxUri const* uri, afxString const *s)
{
    afxError err = { 0 };
    AFX_ASSERT2(uri, s);
    return !AfxCompareStrings(AfxGetUriString(uri), 0, FALSE, 1, s, NIL);
}

_AFXINL afxResult AfxCompareUriStringCi(afxUri const* uri, afxString const *s)
{
    afxError err = { 0 };
    AFX_ASSERT2(uri, s);
    return !AfxCompareStrings(AfxGetUriString(uri), 0, TRUE, 1, s, NIL);
}

_AFXINL afxResult AfxCompareUri(afxUri const* uri, afxUri const *other)
{
    afxError err = { 0 };
    AFX_ASSERT2(uri, other);
    return !AfxCompareStrings(AfxGetUriString(uri), 0, FALSE, 1, AfxGetUriString(other), NIL);
}

_AFXINL afxResult AfxCompareUriCi(afxUri const* uri, afxUri const *other)
{
    afxError err = { 0 };
    AFX_ASSERT2(uri, other);
    return AfxCompareUriStringCi(uri, AfxGetUriString(other));
}

// CHANGE

_AFXINL void AfxResetUris(afxUnit cnt, afxUri uris[])
{
    afxError err = { 0 };
    AFX_ASSERT(uris);

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxUri* uri = &uris[i];
        AfxResetStrings(1, &uri->s);
        uri->sch = 0;
        uri->user = 0;
        //uri->pass = 0;
        uri->h = 0;
        uri->p = 0;
        uri->dir = 0;
        uri->root = 0;
        uri->fname = 0;
        uri->fext = 0;
        uri->qry = 0;
        uri->frag = 0;
    }
}

_AFXINL void AfxClearUris(afxUnit cnt, afxUri uris[])
{
    afxError err = { 0 };
    AFX_ASSERT(uris);

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxUri* uri = &uris[i];
        AfxClearStrings(1, &uri->s);
        uri->sch = 0;
        uri->user = 0;
        //uri->pass = 0;
        uri->h = 0;
        uri->p = 0;
        uri->root = 0;
        uri->dir = 0;
        uri->fname = 0;
        uri->fext = 0;
        uri->qry = 0;
        uri->frag = 0;
    }
}

_AFXINL afxUri* AfxMakeUri(afxUri* uri, afxUnit cap, void const* start, afxUnit len)
{
    afxError err = { 0 };
    AFX_ASSERT(uri);
    AfxMakeString(&uri->s, cap, (void*)start, len);
    AfxReparseUri(uri);
    int a = 1;
    return uri;
}

_AFXINL afxUri AfxMakeUri2(afxUnit cap, void const* start, afxUnit len)
{
    afxError err = { 0 };
    afxUri uri;
    AfxMakeString(&uri.s, cap, (void*)start, len);
    AfxReparseUri(&uri);
    return uri;
}

_AFXINL afxUnit AfxWrapUriString(afxUri* uri, afxString const* src)
{
    afxError err = { 0 };
    AFX_ASSERT(uri);
    uri->s = *src;
    AfxReparseUri(uri);
    return AfxGetUriLength(uri);
}

_AFXINL afxUnit AfxWrapUriStringRange(afxUri* uri, afxString const* src, afxUnit offset, afxUnit len)
{
    afxError err = { 0 };
    AFX_ASSERT(uri);
    AfxMakeUri(uri, 0, AfxGetStringData(src, offset), len);
    AfxReparseUri(uri);
    return AfxGetUriLength(uri);
}

_AFXINL void AfxReplicateUri(afxUri* uri, afxUri const *in)
{
    afxError err = { 0 };
    AFX_ASSERT(uri);
    AFX_ASSERT(in);
    uri->s = in->s;
    uri->sch = in->sch;
    uri->user = in->user;
    //uri->pass = in->pass;
    uri->h = in->h;
    uri->p = in->p;
    uri->root = in->root;
    uri->dir = in->dir;
    uri->fname = in->fname;
    uri->fext = in->fext;
    uri->qry = in->qry;
    uri->frag = in->frag;
}

_AFXINL afxError AfxCopyUri(afxUri* uri, afxUri const *src)
{
    afxError err = { 0 };
    AFX_ASSERT(uri);
    AFX_ASSERT(AfxUriIsEditable(uri));
    AFX_ASSERT(src);
    afxUnit clampedOff = 0;

    if ((clampedOff = AfxCopyString(&uri->s, 0, &src->s, 0)))
    {
        AfxThrowError();
    }
    AfxReparseUri(uri); // Strings can have differently sized buffers. So we recalc it instead of just copy offsets and ranges.
    return err;
}

_AFXINL void AfxFormatUriArg(afxUri* uri, afxChar const *fmt, va_list args)
{
    afxError err = { 0 };
    AFX_ASSERT(uri);
    AFX_ASSERT(AfxUriIsEditable(uri));

    if (!fmt) AfxClearUris(1, uri);
    else
    {
        afxString *s = AfxGetUriString2(uri);
        AFX_ASSERT(s);
        AFX_ASSERT(fmt);
        AfxFormatStringArg(s, fmt, args);
        //AfxTransformPathString(s, FALSE);
        AfxReparseUri(uri);
        //AfxCanonicalizePath(uri, FALSE);
    }
}

_AFXINL void AfxFormatUri(afxUri* uri, afxChar const *fmt, ...)
{
    afxError err = { 0 };
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
    afxError err = { 0 };
    AFX_ASSERT(uri);
    AFX_ASSERT(scheme);
    AfxResetUris(1, scheme);
    scheme->s = AfxExcerptString(&uri->s, 0, (scheme->sch = uri->sch));
    return scheme->sch;
}

_AFXINL afxUnit AfxExcerptUriAuthority(afxUri const* uri, afxUri* user, afxUri* host, afxUri* port)
{
    afxError err = { 0 };
    AFX_ASSERT(user || host || port);
    AFX_ASSERT(uri);
    afxUnit totalLen = 0;
    afxUnit offset = 0;

    if (user)
    {
        AfxResetUris(1, user);
        offset = uri->sch;
        totalLen += uri->user;
        user->user = uri->user;
        user->s = AfxExcerptString(&uri->s, offset, totalLen);
    }

    if (host)
    {
        if (host != user)
        {
            AfxResetUris(1, host);
            offset = uri->sch + uri->user;
            totalLen = uri->h;
        }
        else totalLen += uri->h;

        host->h = uri->h;
        host->s = AfxExcerptString(&uri->s, offset, totalLen);
    }

    if (port)
    {
        if (port != host)
        {
            AfxResetUris(1, port);
            offset = uri->sch + uri->user + uri->h;
            totalLen = uri->p;
        }
        else totalLen += uri->p;

        port->p = uri->p;
        port->s = AfxExcerptString(&uri->s, offset, totalLen);
    }
    return totalLen;
}

_AFXINL afxUnit AfxExcerptUriQuery(afxUri const* uri, afxBool skipSep, afxUri* query)
{
    afxError err = { 0 };
    AFX_ASSERT(uri);
    AFX_ASSERT(query);
    AfxResetUris(1, query);
    afxUnit additive = skipSep && uri->qry ? 1 : 0;
    query->s = AfxExcerptString(&uri->s, _URI_QUERY_OFF(uri) + additive, (uri->qry - additive));
    return query->qry;
}

_AFXINL afxUnit AfxExcerptUriFragment(afxUri const* uri, afxBool skipSep, afxUri* frag)
{
    afxError err = { 0 };
    AFX_ASSERT(uri);
    AFX_ASSERT(frag);
    AfxResetUris(1, frag);
    afxUnit additive = skipSep && uri->frag ? 1 : 0;
    frag->s = AfxExcerptString(&uri->s, _URI_FRAG_OFF(uri) + additive, (uri->frag - additive));
    return frag->frag;
}

_AFXINL afxUnit AfxClipPathDirectory(afxUri* uri, afxUri const* from)
{
    afxError err = { 0 };
    AFX_ASSERT(from);
    AFX_ASSERT(uri);
    afxUri t;
    AfxResetUris(1, &t);
    afxUnit offset = from->sch + from->user/* + from->pass*/ + from->h + from->p + from->root;
    afxUnit len = (t.dir = from->dir);
    //afxUnit cap = offset < from->s.cap ? from->s.cap - offset : 0;
    //AfxMakeString(&dir->s, cap, len ? AfxGetStringData(&from->s.s, offset) : NIL, len);
    t.s = AfxExcerptString(&from->s, offset, len);
    *uri = t;
    return len;
}

_AFXINL afxUnit AfxClipPathDirectory2(afxUri* uri, afxUri const* from, afxUnit seg)
{
    afxError err = { 0 };
    AFX_ASSERT(from);
    AFX_ASSERT(uri);
    afxUri t;
    AfxResetUris(1, &t);
    afxUnit offset = from->sch + from->user/* + from->pass*/ + from->h + from->p + from->root;
    afxUnit len = (t.dir = from->dir);
    //afxUnit cap = offset < from->s.cap ? from->s.cap - offset : 0;
    //AfxMakeString(&dir->s, cap, len ? AfxGetStringData(&from->s.s, offset) : NIL, len);
    
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

    t.s = AfxExcerptString(&from->s, offset, len2);
    t.dir = len2;
    *uri = t;
    return len2;
}

_AFXINL afxUnit AfxExcerptPathBase(afxUri const* uri, afxUri* base)
{
    afxError err = { 0 };
    AFX_ASSERT(uri);
    AFX_ASSERT(base);
    afxUri t;
    AfxResetUris(1, &t);
    afxUnit offset = uri->sch + uri->user/* + from->pass*/ + uri->h + uri->p;
    afxUnit len = (t.root = uri->root) + (t.dir = uri->dir);
    //afxUnit cap = offset < from->s.cap ? from->s.cap - offset : 0;
    //AfxMakeString(&name->s, cap, len ? AfxGetStringData(&uri->s.s, offset) : NIL, len);
    t.s = AfxExcerptString(&uri->s, offset, len);
    *base = t;
    return len;
}

_AFXINL afxUnit AfxExcerptPathSegments(afxUri const* uri, afxUri* root, afxUri* dir, afxUri* file, afxUri* ext)
{
    afxError err = { 0 };
    AFX_ASSERT(uri);
    afxUri t;
    AfxResetUris(1, &t);
    afxUnit offset = _URI_PATH_OFF(uri);
    afxUnit len = _URI_PATH_LEN(uri);

    if (root)
    {
        AfxResetUris(1, root);
        root->s = AfxExcerptString(&uri->s, _URI_PATH_OFF(uri), uri->root);
        root->root = uri->root;
    }

    if (dir)
    {
        if (dir == root)
        {
            dir->s.len += uri->dir;
            dir->dir = uri->dir;
        }
        else
        {
            AfxResetUris(1, dir);
            dir->s = AfxExcerptString(&uri->s, _URI_PATH_OFF(uri) + uri->root, uri->dir);
            dir->dir = uri->dir;
        }
    }

    if (file)
    {
        if (file == dir)
        {
            file->s.len += uri->fname;
            file->fname = uri->fname;
        }
        else
        {
            AfxResetUris(1, file);
            file->s = AfxExcerptString(&uri->s, _URI_PATH_OFF(uri) + uri->root + uri->dir, uri->fname);
            file->fname = uri->fname;
        }
    }

    if (ext)
    {
        if (ext == file)
        {
            ext->s.len += uri->fext;
            ext->fext = uri->fext;
        }
        else
        {
            AfxResetUris(1, ext);
            ext->s = AfxExcerptString(&uri->s, _URI_PATH_OFF(uri) + uri->root + uri->dir + uri->fname, uri->fext);
            ext->fext = uri->fext;
        }
    }
    return len;
}

_AFXINL afxUnit AfxSplitPath(afxUri const* uri, afxUri* root, afxUri* path)
{
    afxError err = { 0 };
    AFX_ASSERT(uri);
    afxUri t;
    AfxResetUris(1, &t);
    afxUnit offset = _URI_PATH_OFF(uri);
    afxUnit len = _URI_PATH_LEN(uri);

    if (root)
    {
        AfxResetUris(1, root);
        root->s = AfxExcerptString(&uri->s, offset, uri->root);
        root->root = uri->root;
    }

    if (!path) len = uri->root;
    else
    {
        if (path != root)
        {
            AfxResetUris(1, path);
            len -= uri->root;
            path->s = AfxExcerptString(&uri->s, offset + uri->root, len);
        }
        else
        {
            path->s = AfxExcerptString(&uri->s, offset, len);
        }
        path->dir = uri->dir;
        path->fname = uri->fname;
        path->fext = uri->fext;
    }
    return len;
}

_AFXINL afxUnit AfxClipUriTarget(afxUri* uri, afxUri const* from)
{
    afxError err = { 0 };
    AFX_ASSERT(from);
    AFX_ASSERT(uri);
    afxUri t;
    AfxResetUris(1, &t);
    afxUnit offset = from->sch + from->user/* + from->pass*/ + from->h + from->p + from->root + from->dir;
    afxUnit len = (t.fname = from->fname);
    //afxUnit cap = offset < from->s.cap ? from->s.cap - offset : 0;
    //AfxMakeString(&name->s, cap, len ? AfxGetStringData(&uri->s.s, offset) : NIL, len);
    t.s = AfxExcerptString(&from->s, offset, len);
    *uri = t;
    return len;
}

_AFXINL afxUnit AfxGetUriTargetString(afxUri const* uri, afxString* name)
{
    afxError err = { 0 };
    AFX_ASSERT(uri);
    AFX_ASSERT(name);
    afxUri tmp;
    AfxClipUriTarget(&tmp, uri);
    *name = *AfxGetUriString(&tmp);
    return name->len;
}

_AFXINL afxUnit AfxClipUriExtension(afxUri* uri, afxUri const* from, afxBool skipDot)
{
    afxError err = { 0 };
    AFX_ASSERT(from);
    AFX_ASSERT(uri);
    AfxResetUris(1, uri);
    afxUnit additive = skipDot && from->fext ? 1 : 0;
    afxUnit offset = from->sch + from->user/* + from->pass*/ + from->h + from->p + from->root + from->dir + from->fname + additive;
    afxUnit len = (uri->fext = from->fext - additive);
    //afxUnit cap = offset < from->s.cap ? from->s.cap - offset : 0;
    //AfxMakeString(&ext->s, cap, len ? AfxGetStringData(&from->s.s, offset) : NIL, len);
    uri->s = AfxExcerptString(&from->s, offset, len);
    return len;
}

_AFXINL afxUnit AfxClipUriFile(afxUri* uri, afxUri const* from)
{
    afxError err = { 0 };
    AFX_ASSERT(from);
    AFX_ASSERT(uri);
    AfxResetUris(1, uri);
    afxUnit offset = from->sch + from->user/* + from->pass*/ + from->h + from->p + from->root + from->dir;
    afxUnit len = (uri->fname = from->fname) + (uri->fext = from->fext);
    //afxUnit cap = offset < from->s.cap ? from->s.cap - offset : 0;
    //AfxMakeString(&uri->s, cap, len ? AfxGetStringData(&from->s.s, offset) : NIL, len);
    uri->s = AfxExcerptString(&from->s, offset, len);
    return len;
}

_AFXINL afxUnit AfxClipUriPath(afxUri* uri, afxUri const* from)
{
    afxError err = { 0 };
    AFX_ASSERT(from);
    AFX_ASSERT(uri);
    AfxResetUris(1, uri);
    afxUnit offset = from->sch + from->user/* + from->pass*/ + from->h + from->p;
    afxUnit len = (uri->root = from->root) + (uri->dir = from->dir) + (uri->fname = from->fname) + (uri->fext = from->fext);
    uri->s = AfxExcerptString(&from->s, _URI_PATH_OFF(from), len);
    return len;
}

_AFX afxUnit AfxClipPath2(afxUri* uri, afxUri const* from, afxUnit seg)
{
    afxError err = { 0 };
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
        t.s = AfxExcerptString(&from->s, offset, len2);

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
    afxError err = { 0 };
    AFX_ASSERT(uri);
    AFX_ASSERT(query);
    afxUri tmp;
    AfxExcerptUriQuery(uri, skipSep, &tmp);
    *query = *AfxGetUriString(&tmp);
    return query->len;
}

_AFXINL afxUnit _AfxGetPathDriveNo_W32(afxUri const* uri)
{
    afxError err = { 0 };
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
    afxError err = { 0 };
    AFX_ASSERT(uri);
    return _AfxGetPathDriveNo_W32(uri);
}

#define IsNetDrive(no_) FALSE

_AFXINL afxBool AfxPathIsNetwork_W32(afxUri const* uri)
{
    afxError err = { 0 };
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
    afxError err = { 0 };
    AFX_ASSERT(uri);

    afxUri path;
    afxUnit len = AfxClipUriPath(&path, uri);
    afxByte const* data = AfxGetUriData(&path, 0);

    return !len || !data[0] || (data[0] != '\\' && AFX_INVALID_INDEX == AfxGetPathDriveNo(&path));
}

_AFXINL afxBool AfxPathIsRelative(afxUri const* uri)
{
    afxError err = { 0 };
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

    afxError err = { 0 };
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
    afxError err = { 0 };
    AFX_ASSERT(uri);
    return _AfxPathIsRoot_W32(uri);
}



_AFXINL afxBool AfxPathIsDevice(afxUri const* uri)
{
    afxError err = { 0 };
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
    afxError err = { 0 };
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
        if (!AfxCompareSubstrings(&AFX_STRING("//./"), 0, 4, FALSE, 1, &dev.s, NIL)) AfxResetUris(1, uri);
        else
        {
            len = AfxGetUriLength(&dev);
            AFX_ASSERT(from->sch + from->user/* + from->pass*/ == 0);
            t.s = AfxExcerptString(&from->s, from->sch + from->user/* + from->pass*/, len);
            t.dir = AfxGetUriLength(&dev);
            *uri = t;
        }
    }
    return len;
}

_AFXINL afxBool _AfxPathIsServer_W32(afxUri const* uri)
{
    // Returns TRUE if the string is a valid path for a server only (no share name), or FALSE otherwise.

    afxError err = { 0 };
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
    afxError err = { 0 };
    AFX_ASSERT(uri);
    return !AfxPathIsRelative(uri);
}

_AFXINL afxBool AfxIsUriToDevice(afxUri const* uri)
{
    afxError err = { 0 };
    AFX_ASSERT(uri);
    afxUri hostPort;
    AfxExcerptUriAuthority(uri, NIL, &hostPort, &hostPort);
    AfxCompareUriString(&hostPort, &AFX_STRING("//."));
    //AfxClipPathDirectory();
    return AfxCompareString(&uri->s, uri->sch, "//.", 0, TRUE) && AfxCompareString(&uri->s, uri->sch + uri->h, "/", 0, TRUE);
}

_AFXINL afxBool AfxIsBaseOfUri(afxUri const* uri, afxUri const* other)
{
    afxError err = { 0 };
    AFX_ASSERT(other);
    AFX_ASSERT(uri);

    AfxThrowError();

    return err;
}

_AFXINL afxResult _AfxFindUriQueryAttribute(afxString *attr, afxString *value, void* data)
{
    afxError err = { 0 };
    AFX_ASSERT(attr);
    AFX_ASSERT(value);
    AFX_ASSERT(data);
    struct { afxString const* key; afxString* dst; afxBool success; } *data2 = data;

    if (AfxCompareStrings(data2->key, 0, FALSE, 1, attr, NIL))
    {
        AfxCopyString(data2->dst, 0, value, 0);
        data2->success = TRUE;
        return 0; // break
    }
    return 1; // continue
}

_AFXINL afxResult AfxUriForEachQueryKey(afxUri const* uri, afxResult(*f)(afxString *attr, afxString *value, void* data), void* data)
{
    afxError err = { 0 };
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
        AfxCopyStringLiteral(&attr.s, 0, chr - query, query);
        query = chr + 1;
        chr = strchr(query, '&');
        if (chr)
        {
            AfxCopyStringLiteral(&value.s, 0, chr - query, query);
            query = chr + 1;
            chr = strchr(query, '=');

            cnt++;

            if (!f(&attr.s, &value.s, data))
                break;
        }
        else
        {
            AfxCopyStringLiteral(&value.s, 0, 0, query);

            cnt++;

            if (!f(&attr.s, &value.s, data)) // faz sentido se é o último?
                break;

            break;
        }
    }
#endif
    return cnt;
}

_AFXINL afxResult AfxUriMapQueryPairs(afxUri const* uri, afxUnit base, afxUnit cnt, afxString keys[], afxString values[])
{
    afxError err = { 0 };
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

_AFXINL void AfxTransformPathString(afxString* s, afxBool out)
{
    afxError err = { 0 };
    AFX_ASSERT(s);
    afxUnit srcLen = AfxGetStringLength(s);

    if (srcLen)
    {
        afxChar *ptr, *end;
        afxInt len;
        afxChar dst[4096];
        afxChar const *src2 = AfxGetStringData(s, 0);

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

        _AfxWriteString(s, 0, dst, AFX_MIN(ptr - &dst[0], s->cap)); // TODO move query and fragment to end of path, once normalization never increase size of string.
        afxChar *_bytemap = AfxGetStringStorage(s, 0);
        //_bytemap[ptr - dst] = '\0';
        s->len = /*uri->s._range -*/ (ptr - dst); // se os seguintes componentes foram escritos incorretamente, mova isto para depois da concatenação deles.
    }
}

_AFXINL void AfxCanonicalizePath(afxUri* uri, afxBool microshit)
{
    // c:\shit\\\by\\M$\foo.poo -> /c/shit/by/M$/unshitted/foo.poo

    //return;
    afxError err = { 0 };
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
        if (query.qry)
            AfxCatenateStrings(pathStr, 1, AfxGetUriString(&query));

        if (frag.frag)
            AfxCatenateStrings(pathStr, 1, AfxGetUriString(&frag));

        *uri = path;
        AfxReparseUri(uri);
        int a = 1;
    }
}

_AFXINL void AfxReparseUri(afxUri* uri)
{
    afxError err = { 0 };
    AFX_ASSERT(uri);

    if (!AfxGetStringLength(&uri->s))
    {
        uri->sch = 0;
        uri->user = 0;
        //uri->pass = 0;
        uri->h = 0;
        uri->p = 0;
        uri->root = 0;
        uri->dir = 0;
        uri->fname = 0;
        uri->fext = 0;
        uri->qry = 0;
        uri->frag = 0;
    }
    else
    {
        afxChar const* data = AfxGetStringData(&uri->s, 0);
        afxUnit dataLen = AfxGetStringLength(&uri->s);
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
        afxString s = uri->s;
        _AfxParseUriParts(uri);
        return;

        url_field_t split = { 0 };

        if (dataLen)
            _AfxUrlParseRaw(data, dataLen, &split);

        uri->sch = split.schemasiz;
        uri->user = split.usersiz;
        //uri->pass = split.passsiz;
        uri->user += split.passsiz;

        uri->h = split.hostsiz;
        uri->p = split.portsiz;
        uri->root = 0;
        uri->dir = split.fdirsiz;
        uri->fname = split.fobjsiz - split.fextsiz;
        uri->fext = split.fextsiz;
        uri->qry = split.querysiz;
        uri->frag = split.fragsiz;

        if (!split.hostsiz && split.fdirsiz)
        {
            afxUnit dirOff = uri->sch + uri->user + uri->h + uri->p;

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
            afxChar const* data = AfxGetStringData(&uri->s, 0);

            if (data[uri->sch + uri->user + uri->h + uri->p + uri->root + uri->dir] == '/' || data[uri->sch + uri->user + uri->h + uri->p + uri->root + uri->dir] == '\\') // '\\' is unneeded once it is already normalized
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
    afxError err = { 0 };
    AFX_ASSERT(uri);
    //AFX_ASSERT(uri->s.flags & _AFX_STR_FLAG_ALLOCED);
    
    AfxDeallocateString(&uri->s);
    AfxResetUris(1, uri);
}

_AFXINL afxError AfxAllocateUri(afxUri* uri, afxUnit cap, void const *start, afxUnit range)
{
    afxError err = { 0 };
    AfxResetUris(1, uri);

    if (AfxAllocateString(&uri->s, cap, start, range))
        AfxThrowError();

    AFX_ASSERT(uri->s.len == range);

    AfxReparseUri(uri);
    return err;
}

#if 0
_AFXINL afxError AfxReallocateUri(afxUri* uri, afxUnit cap, void const *start, afxUnit range)
{
    afxError err = { 0 };
    void *buf = NIL;

    if (start && !range)
        range = AfxStrlen(start);

    AFX_ASSERT(!range || (range && start));

    {
        AFX_ASSERT(uri);
        //AFX_ASSERT(uri->s.flags & _AFX_STR_FLAG_ALLOCED);

        afxUnit effectiveCap = cap ? cap : range;
        AFX_ASSERT(effectiveCap);

        if (AfxReallocateString(&uri->s, effectiveCap, start, range))
             AfxThrowError();

        AfxReparseUri(uri);
    }
    return err;
}
#endif

_AFXINL afxError AfxDuplicateUri(afxUri* uri, afxUri const *in)
{
    afxError err = { 0 };

    if (!in) AfxResetUris(1, uri);
    else
    {
        AFX_ASSERT(in);
        afxUnit len = AfxGetUriLength(in);

        if (!len) AfxResetUris(1, uri);
        else
        {
            AFX_ASSERT(len);
            void const *strData = AfxGetStringData(&in->s, 0);
            AFX_ASSERT(strData);

            if (AfxAllocateUri(uri, 0, strData, len))
                AfxThrowError();

            AFX_ASSERT(uri->s.len == len);
        }
    }
    return err;
}

_AFX afxUri* AfxMakeUri8(afxUri8 *uri, afxUri const* src)
{
    afxError err = { 0 };
    AFX_ASSERT(uri);
    uri->buf[0] = '\0';
    AfxMakeUri(&uri->uri, sizeof(uri->buf), uri->buf, 0);

    if (src)
        AfxCopyUri(&uri->uri, src);

    return &uri->uri;
}

_AFX afxUri* AfxMakeUri32(afxUri32 *uri, afxUri const* src)
{
    afxError err = { 0 };
    AFX_ASSERT(uri);
    uri->buf[0] = '\0';
    AfxMakeUri(&uri->uri, sizeof(uri->buf), uri->buf, 0);

    if (src)
        AfxCopyUri(&uri->uri, src);

    return &uri->uri;
}

_AFX afxUri* AfxMakeUri128(afxUri128 *uri, afxUri const* src)
{
    afxError err = { 0 };
    AFX_ASSERT(uri);
    uri->buf[0] = '\0';
    AfxMakeUri(&uri->uri, sizeof(uri->buf), uri->buf, 0);

    if (src)
        AfxCopyUri(&uri->uri, src);

    return &uri->uri;
}

_AFX afxUri* AfxMakeUri2048(afxUri2048 *uri, afxUri const* src)
{
    afxError err = { 0 };
    AFX_ASSERT(uri);
    uri->buf[0] = '\0';
    AfxMakeUri(&uri->uri, sizeof(uri->buf), uri->buf, 0);

    if (src)
        AfxCopyUri(&uri->uri, src);

    return &uri->uri;
}
