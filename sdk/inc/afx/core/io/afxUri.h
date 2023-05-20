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
    afxFcc              fcc;
    afxString           str;
    struct
    {
        afxNat16        off;
        afxNat16        range;
    }                   schem, user, pass, host, port, dir, file, ext, query, frag;    
};

AFX_DEFINE_STRUCT(afxUri32)
{
    afxUri              uri;
    afxChar             buf[32];
};

AFX_DEFINE_STRUCT(afxUri64)
{
    afxUri              uri;
    afxChar             buf[64];
};

AFX_DEFINE_STRUCT(afxUri128)
{
    afxUri              uri;
    afxChar             buf[128];
};

AFX_DEFINE_STRUCT(afxUri256)
{
    afxUri              uri;
    afxChar             buf[256];
};

AFX_DEFINE_STRUCT(afxUri4096) // max capacity has no typed size.
{
    afxUri              uri;
    afxChar             buf[4096];
};

AFX afxUri*             AfxUri32(afxUri32 *uri, afxUri const *src); // deploy
AFX afxUri*             AfxUri64(afxUri64 *uri, afxUri const *src); // deploy
AFX afxUri*             AfxUri128(afxUri128 *uri, afxUri const *src); // deploy
AFX afxUri*             AfxUri256(afxUri256 *uri, afxUri const *src); // deploy
AFX afxUri*             AfxUri4096(afxUri4096 *uri, afxUri const *src); // deploy

AFX afxUri*             AfxUriExcerptHost(afxUri *uri, afxUri const *base);
AFX afxUri*             AfxUriExcerptPort(afxUri *uri, afxUri const *base);
AFX afxUri*             AfxUriExcerptUsername(afxUri *uri, afxUri const *base);
AFX afxUri*             AfxUriExcerptPassword(afxUri *uri, afxUri const *base);
AFX afxUri*             AfxUriExcerptPath(afxUri *uri, afxUri const *base);
AFX afxUri*             AfxUriExcerptDirectory(afxUri *uri, afxUri const *base);
AFX afxUri*             AfxUriExcerptFile(afxUri *uri, afxUri const *base); // file name + extension
AFX afxUri*             AfxUriExcerptName(afxUri *uri, afxUri const *base); // just file name, without extension
AFX afxUri*             AfxUriExcerptExtension(afxUri *uri, afxUri const *base, afxBool skipDot);
AFX afxUri*             AfxUriExcerptQuery(afxUri *uri, afxUri const *base);
AFX afxUri*             AfxUriExcerptFragment(afxUri *uri, afxUri const *base);
AFX afxUri*             AfxUriExcerptScheme(afxUri *uri, afxUri const *base); // "https"

AFX afxUri*             AfxUriExcerptPathWriteable(afxUri *uri, afxUri *base);
AFX afxUri*             AfxUriExcerptDirectoryWriteable(afxUri *uri, afxUri *base);
AFX afxUri*             AfxUriExcerptFileWriteable(afxUri *uri, afxUri *base); // file name + extension
AFX afxUri*             AfxUriExcerptNameWriteable(afxUri *uri, afxUri *base); // just file name, without extension
AFX afxUri*             AfxUriExcerptExtensionWriteable(afxUri *uri, afxUri *base);

AFX afxUri*             AfxUriReflect(afxUri *uri, afxUri const *in); // pointers and handle same data handled by 'in' without doing any copy or change.

AFX afxUri*             AfxUriReflectString(afxUri *uri, afxString const *src); // will not set capacity  (will be read only)
AFX afxUri*             AfxUriReflectStringWriteable(afxUri *uri, afxString const *src);

AFX afxUri*             AfxUriReflectData(afxUri *uri, void const *start, afxNat range); // will not set capacity (will be read only)
AFX afxUri*             AfxUriReflectDataWriteable(afxUri *uri, void *start, afxNat range);

AFX afxUri*             AfxUriReset(afxUri *uri); // WARNING string storaged is resetted too. Any storage assigned will be nulled.

// require initalized URI.

AFX afxUri*             AfxUriErase(afxUri *uri); // just zero ranges preserving storage.
AFX afxUri*             AfxUriFormat(afxUri *uri, afxChar const *fmt, ...);

AFX afxUri*             AfxUriCopy(afxUri *uri, afxUri const *src);
AFX afxUri*             AfxUriCopyHost(afxUri *uri, afxUri const *src);
AFX afxUri*             AfxUriCopyPort(afxUri *uri, afxUri const *src);
AFX afxUri*             AfxUriCopyUsername(afxUri *uri, afxUri const *src);
AFX afxUri*             AfxUriCopyPassword(afxUri *uri, afxUri const *src);
AFX afxUri*             AfxUriCopyPath(afxUri *uri, afxUri const *src);
AFX afxUri*             AfxUriCopyDirectory(afxUri *uri, afxUri const *src);
AFX afxUri*             AfxUriCopyFile(afxUri *uri, afxUri const *src); // file name + extension
AFX afxUri*             AfxUriCopyName(afxUri *uri, afxUri const *src); // just file name, without extension
AFX afxUri*             AfxUriCopyExtension(afxUri *uri, afxUri const *src);
AFX afxUri*             AfxUriCopyFragment(afxUri *uri, afxUri const *src);
AFX afxUri*             AfxUriCopyQuery(afxUri *uri, afxUri const *src);
AFX afxUri*             AfxUriCopyScheme(afxUri *uri, afxUri const *src);

AFX afxResult           AfxUriForEachQueryKey(afxUri const *uri, afxResult(*f)(afxString *key, afxString *value, void* data), void* data);

AFX afxChar*            AfxUriGetData(afxUri *uri);
AFX afxChar const*      AfxUriGetDataConst(afxUri const *uri);
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
AFX afxUri*             AfxUriNormalize(afxUri *uri);
AFX afxUri*             AfxUriReparse(afxUri *uri);

#endif//AFX_URI_H