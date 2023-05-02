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
        afxNat          off, range;
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

AFX afxUri*             AfxUriForkHost(afxUri *uri, afxUri const *base);
AFX afxUri*             AfxUriForkPort(afxUri *uri, afxUri const *base);
AFX afxUri*             AfxUriForkUsername(afxUri *uri, afxUri const *base);
AFX afxUri*             AfxUriForkPassword(afxUri *uri, afxUri const *base);
AFX afxUri*             AfxUriForkPath(afxUri *uri, afxUri const *base);
AFX afxUri*             AfxUriForkDirectory(afxUri *uri, afxUri const *base);
AFX afxUri*             AfxUriForkFile(afxUri *uri, afxUri const *base); // file name + extension
AFX afxUri*             AfxUriForkName(afxUri *uri, afxUri const *base); // just file name, without extension
AFX afxUri*             AfxUriForkExtension(afxUri *uri, afxUri const *base);
AFX afxUri*             AfxUriForkQuery(afxUri *uri, afxUri const *base);
AFX afxUri*             AfxUriForkFragment(afxUri *uri, afxUri const *base);
AFX afxUri*             AfxUriForkScheme(afxUri *uri, afxUri const *base); // "https"

AFX afxUri*             AfxUriForkPathEditable(afxUri *uri, afxUri *base);
AFX afxUri*             AfxUriForkDirectoryEditable(afxUri *uri, afxUri *base);
AFX afxUri*             AfxUriForkFileEditable(afxUri *uri, afxUri *base); // file name + extension
AFX afxUri*             AfxUriForkNameEditable(afxUri *uri, afxUri *base); // just file name, without extension
AFX afxUri*             AfxUriForkExtensionEditable(afxUri *uri, afxUri *base);

AFX afxUri*             AfxUriReset(afxUri *uri);
AFX afxUri*             AfxUriMapString(afxUri *uri, afxString const *src);
AFX afxUri*             AfxUriMapStringConst(afxUri *uri, afxString const *src); // will not set capacity  (will be read only)
AFX afxUri*             AfxUriMapData(afxUri *uri, afxChar *start, afxNat range);
AFX afxUri*             AfxUriMapConstData(afxUri *uri, afxChar const *start, afxNat range); // will not set capacity (will be read only)

AFX afxUri*             AfxUriErase(afxUri *uri);
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