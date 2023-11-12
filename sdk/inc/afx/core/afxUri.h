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
 *                   (c) 2017 SIGMA Technology Group — Federação SIGMA
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

AFXINL void             AfxUri128(afxUri128 *uri);
AFXINL void             AfxUri2048(afxUri2048 *uri);

AFXINL void             AfxUriDeallocate(afxUri *uri);
AFXINL afxError         AfxUriAllocate(afxUri* uri, afxNat cap, void const *start, afxNat range);
AFXINL afxError         AfxUriReallocate(afxUri *uri, afxNat cap, void const *start, afxNat range);

AFXINL afxError         AfxCloneUri(afxUri* uri, afxUri const *in);
AFXINL afxError         AfxCloneUriPath(afxUri* uri, afxUri const *in);
AFXINL afxError         AfxCloneUriName(afxUri* uri, afxUri const *in);

// Make a new URI excerpting another URI.
AFXINL afxNat           AfxGetUriHost(afxUri *uri, afxUri const *parent);
AFXINL afxNat           AfxGetUriPort(afxUri *uri, afxUri const *parent);
AFXINL afxNat           AfxGetUriUsername(afxUri *uri, afxUri const *parent);
AFXINL afxNat           AfxGetUriPassword(afxUri *uri, afxUri const *parent);
AFXINL afxNat           AfxGetUriPath(afxUri *uri, afxUri const *parent);
AFXINL afxNat           AfxGetUriDirectory(afxUri *uri, afxUri const *parent);
AFXINL afxNat           AfxGetUriObject(afxUri *uri, afxUri const *parent); // file name + extension (if any)
AFXINL afxNat           AfxGetUriName(afxUri *uri, afxUri const *parent); // just file name, without extension
AFXINL afxNat           AfxGetUriExtension(afxUri *uri, afxUri const *parent, afxBool skipDot);
AFXINL afxNat           AfxGetUriQuery(afxUri *uri, afxUri const *parent, afxBool skipSep);
AFXINL afxNat           AfxGetUriFragment(afxUri *uri, afxUri const *parent, afxBool skipSep);
AFXINL afxNat           AfxGetUriScheme(afxUri *uri, afxUri const *parent); // "https"

AFXINL afxNat           AfxGetUriPathRW(afxUri *uri, afxUri *parent);
AFXINL afxNat           AfxGetUriDirectoryRW(afxUri *uri, afxUri *parent);
AFXINL afxNat           AfxGetUriObjectRW(afxUri *uri, afxUri *parent); // file name + extension
AFXINL afxNat           AfxGetUriNameRW(afxUri *uri, afxUri *parent); // just file name, without extension
AFXINL afxNat           AfxGetUriExtensionRW(afxUri *uri, afxUri *parent);

AFXINL void             AfxReplicateUri(afxUri *uri, afxUri const *in); // pointers and handle same data handled by 'in' without doing any copy or change.

AFXINL afxNat           AfxUriFromString(afxUri *uri, afxString const *src); // will not set capacity  (will be read only)
AFXINL afxNat           AfxUriFromStringRange(afxUri *uri, afxString const *src, afxNat offset, afxNat range);

AFXINL void             AfxMakeUri(afxUri *uri, void const *start, afxNat range); // will not set capacity (will be read only)
AFXINL void             AfxMakeMutableUri(afxUri *uri, void *start, afxNat range);

AFXINL void             AfxMakeBufferedUri(afxUri *uri, void const *start, afxNat cap);

AFXINL void             AfxResetUri(afxUri *uri); // WARNING string storage is resetted too. Any storage assigned will be nulled.

// require initalized URI.

AFXINL afxResult        AfxCompareUri(afxUri const *uri, afxUri const *other);
AFXINL afxResult        AfxCompareUriCi(afxUri const *uri, afxUri const *other);
AFXINL afxResult        AfxCompareUriToString(afxUri const *uri, afxString const *str);
AFXINL afxResult        AfxCompareUriToStringCi(afxUri const *uri, afxString const *str);

AFXINL void             AfxClearUri(afxUri *uri); // just zero ranges preserving storage.
AFXINL void             AfxFormatUri(afxUri *uri, afxChar const *fmt, ...);

AFXINL afxError         AfxCopyUri(afxUri *uri, afxUri const *src);
AFXINL afxError         AfxCopyUriHost(afxUri *uri, afxUri const *src);
AFXINL afxError         AfxCopyUriPort(afxUri *uri, afxUri const *src);
AFXINL afxError         AfxCopyUriUsername(afxUri *uri, afxUri const *src);
AFXINL afxError         AfxCopyUriPassword(afxUri *uri, afxUri const *src);
AFXINL afxError         AfxCopyUriPath(afxUri *uri, afxUri const *src);
AFXINL afxError         AfxCopyUriDirectory(afxUri *uri, afxUri const *src);
AFXINL afxError         AfxCopyUriObject(afxUri *uri, afxUri const *src); // file name + extension
AFXINL afxError         AfxCopyUriName(afxUri *uri, afxUri const *src); // just file name, without extension
AFXINL afxError         AfxCopyUriExtension(afxUri *uri, afxUri const *src, afxBool skipDot);
AFXINL afxError         AfxCopyUriFragment(afxUri *uri, afxUri const *src, afxBool skipSep);
AFXINL afxError         AfxCopyUriQuery(afxUri *uri, afxUri const *src, afxBool skipSep);
AFXINL afxError         AfxCopyUriScheme(afxUri *uri, afxUri const *src);

AFXINL afxResult        AfxUriForEachQueryKey(afxUri const *uri, afxResult(*f)(afxString *key, afxString *value, void* data), void* data);

AFXINL afxNat           AfxMeasureUri(afxUri const* uri);
AFXINL afxNat           AfxMeasurePath(afxUri const* uri);
AFXINL afxNat           AfxMeasureUriRequest(afxUri const* uri);

AFXINL void*            AfxGetMutableUriData(afxUri *uri, afxNat base);
AFXINL void const*      AfxGetUriData(afxUri const *uri, afxNat base);
AFXINL afxString*       AfxGetMutableUriString(afxUri *uri);
AFXINL afxString const* AfxGetUriString(afxUri const *uri);

AFXINL afxBool          AfxUriHasUsername(afxUri const *uri);
AFXINL afxBool          AfxUriHasPassword(afxUri const *uri);
AFXINL afxBool          AfxUriHasHost(afxUri const *uri);
AFXINL afxBool          AfxUriHasPort(afxUri const *uri);
AFXINL afxBool          AfxUriHasFragment(afxUri const *uri);
AFXINL afxBool          AfxUriHasPath(afxUri const *uri);
AFXINL afxBool          AfxUriHasDirectory(afxUri const *uri);
AFXINL afxBool          AfxUriHasName(afxUri const *uri);
AFXINL afxBool          AfxUriHasExtension(afxUri const *uri);
AFXINL afxBool          AfxUriHasQuery(afxUri const *uri);
AFXINL afxBool          AfxUriHasScheme(afxUri const *uri);

AFXINL afxBool          AfxUriIsBlank(afxUri const *uri);
AFXINL afxBool          AfxUriIsWriteable(afxUri const *uri);
AFXINL afxBool          AfxPathIsRelative(afxUri const *uri);
AFXINL void             AfxNormalizePath(afxUri *uri);
AFXINL void             AfxReparseUri(afxUri *uri);

#endif//AFX_URI_H