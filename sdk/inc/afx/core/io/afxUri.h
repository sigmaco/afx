/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                      S I G M A   T E C H N O L O G Y   G R O U P
 *
 *                                   Public Test Build
 *                      (c) 2017 SIGMA Co. & SIGMA Technology Group
 *                                    www.sigmaco.org
 */

// This content is part of SIGMA Future Storage <https://sigmaco.org/future-storage>

#ifndef AFX_URI_H
#define AFX_URI_H

#include "afx/core/afxString.h"

#define AFX_URI_FILE_EXT_SEP '.'
#define AFX_URI_PATH_HERE '.'
#define AFX_URI_PATH_SEP '/'
#define AFX_URI_PATH_ORIGIN '/'
#define AFX_URI_PATH_SEP_WIN '\\'
#define AFX_URI_SCHEME_SEP ':'
#define AFX_URI_BASE_LEN 4096

// TODO: Make a afxPath structure to ease composition and decomposition of paths.

// In QWADRO, URI is a type of string specialized in dealing with file path

AFX_DEFINE_STRUCT(afxUri)
{
    _AFX_DBG_FCC;
    afxString           str;
    afxNat8             schem, user, pass, host, port; // 6 chars
    afxNat16            dir, fname;
    afxNat8             fext;
    afxNat16            query, frag; // 8 chars
    
};

AFX_DEFINE_STRUCT(afxUri128)
{
    afxUri              uri;
    afxByte             buf[128];
};

AFX_DEFINE_STRUCT(afxUri2048)
{
    afxUri              uri;
    afxByte             buf[2048];
};

AFX void                AfxUri128(afxUri128 *uri);
AFX void                AfxUri2048(afxUri2048 *uri);

AFX void                AfxUriDeallocate(afxUri *uri);
AFX afxUri*             AfxUriAllocate(afxNat cap, void const *start, afxNat range);
AFX afxUri*             AfxUriReallocate(afxUri *uri, afxNat cap, void const *start, afxNat range);

AFX afxUri*             AfxUriClone(afxUri const *uri);
AFX afxUri*             AfxUriClonePath(afxUri const *uri);
AFX afxUri*             AfxUriCloneName(afxUri const *uri);

AFX afxNat              AfxUriExcerptHost(afxUri const *uri, afxUri *host);
AFX afxNat              AfxUriExcerptPort(afxUri const *uri, afxUri *port);
AFX afxNat              AfxUriExcerptUsername(afxUri const *uri, afxUri *usrnam);
AFX afxNat              AfxUriExcerptPassword(afxUri const *uri, afxUri *passwd);
AFX afxNat              AfxUriExcerptPath(afxUri const *uri, afxUri *path);
AFX afxNat              AfxUriExcerptDirectory(afxUri const *uri, afxUri *dir);
AFX afxNat              AfxUriExcerptObject(afxUri const *uri, afxUri *file); // file name + extension (if any)
AFX afxNat              AfxUriExcerptName(afxUri const *uri, afxUri *name); // just file name, without extension
AFX afxNat              AfxUriExcerptExtension(afxUri const *uri, afxUri *ext, afxBool skipDot);
AFX afxNat              AfxUriExcerptQuery(afxUri const *uri, afxUri *query, afxBool skipSep);
AFX afxNat              AfxUriExcerptFragment(afxUri const *uri, afxUri *frag, afxBool skipSep);
AFX afxNat              AfxUriExcerptScheme(afxUri const *uri, afxUri *scheme); // "https"

AFX afxNat              AfxUriExcerptPathRW(afxUri *uri, afxUri *path);
AFX afxNat              AfxUriExcerptDirectoryRW(afxUri *uri, afxUri *dir);
AFX afxNat              AfxUriExcerptObjectRW(afxUri *uri, afxUri *file); // file name + extension
AFX afxNat              AfxUriExcerptNameRW(afxUri *uri, afxUri *name); // just file name, without extension
AFX afxNat              AfxUriExcerptExtensionRW(afxUri *uri, afxUri *ext);

AFX void                AfxUriReflect(afxUri *uri, afxUri const *in); // pointers and handle same data handled by 'in' without doing any copy or change.

AFX void                AfxUriReflectString(afxUri *uri, afxString const *src); // will not set capacity  (will be read only)

AFX void                AfxUriWrapLiteral(afxUri *uri, void const *start, afxNat range); // will not set capacity (will be read only)
AFX void                AfxUriWrapLiteralRW(afxUri *uri, void *start, afxNat range);

AFX void                AfxUriWrapBuffer(afxUri *uri, void const *start, afxNat cap);

AFX void                AfxUriReset(afxUri *uri); // WARNING string storage is resetted too. Any storage assigned will be nulled.

// require initalized URI.

AFX afxBool             AfxUriIsEquivalent(afxUri const *uri, afxUri const *other);
AFX afxBool             AfxUriIsEquivalentCi(afxUri const *uri, afxUri const *other);
AFX afxBool             AfxUriIsEquivalentToString(afxUri const *uri, afxString const *str);
AFX afxBool             AfxUriIsEquivalentToStringCi(afxUri const *uri, afxString const *str);

AFX void                AfxUriErase(afxUri *uri); // just zero ranges preserving storage.
AFX void                AfxUriFormat(afxUri *uri, afxChar const *fmt, ...);

AFX afxError            AfxUriCopy(afxUri *uri, afxUri const *src);
AFX afxError            AfxUriCopyHost(afxUri *uri, afxUri const *src);
AFX afxError            AfxUriCopyPort(afxUri *uri, afxUri const *src);
AFX afxError            AfxUriCopyUsername(afxUri *uri, afxUri const *src);
AFX afxError            AfxUriCopyPassword(afxUri *uri, afxUri const *src);
AFX afxError            AfxUriCopyPath(afxUri *uri, afxUri const *src);
AFX afxError            AfxUriCopyDirectory(afxUri *uri, afxUri const *src);
AFX afxError            AfxUriCopyObject(afxUri *uri, afxUri const *src); // file name + extension
AFX afxError            AfxUriCopyName(afxUri *uri, afxUri const *src); // just file name, without extension
AFX afxError            AfxUriCopyExtension(afxUri *uri, afxUri const *src, afxBool skipDot);
AFX afxError            AfxUriCopyFragment(afxUri *uri, afxUri const *src, afxBool skipSep);
AFX afxError            AfxUriCopyQuery(afxUri *uri, afxUri const *src, afxBool skipSep);
AFX afxError            AfxUriCopyScheme(afxUri *uri, afxUri const *src);

AFX afxResult           AfxUriForEachQueryKey(afxUri const *uri, afxResult(*f)(afxString *key, afxString *value, void* data), void* data);

AFX void*               AfxUriGetData(afxUri *uri, afxNat base);
AFX void const*         AfxUriGetDataConst(afxUri const *uri, afxNat base);
AFX afxString*          AfxUriGetString(afxUri *uri);
AFX afxString const*    AfxUriGetStringConst(afxUri const *uri);

AFX afxBool             AfxUriHasUsername(afxUri const *uri);
AFX afxBool             AfxUriHasPassword(afxUri const *uri);
AFX afxBool             AfxUriHasHost(afxUri const *uri);
AFX afxBool             AfxUriHasPort(afxUri const *uri);
AFX afxBool             AfxUriHasFragment(afxUri const *uri);
AFX afxBool             AfxUriHasPath(afxUri const *uri);
AFX afxBool             AfxUriHasDirectory(afxUri const *uri);
AFX afxBool             AfxUriHasName(afxUri const *uri);
AFX afxBool             AfxUriHasExtension(afxUri const *uri);
AFX afxBool             AfxUriHasQuery(afxUri const *uri);
AFX afxBool             AfxUriHasScheme(afxUri const *uri);

AFX afxBool             AfxUriIsBlank(afxUri const *uri);
AFX afxBool             AfxUriIsWriteable(afxUri const *uri);
AFX afxBool             AfxUriIsPathRelative(afxUri const *uri);
AFX void                AfxUriNormalizePath(afxUri *uri);
AFX void                AfxUriReparse(afxUri *uri);

#endif//AFX_URI_H