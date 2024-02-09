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

// This content is part of SIGMA Future Storage <https://sigmaco.org/future-storage>

#ifndef AFX_URI_H
#define AFX_URI_H

#include "qwadro/core/afxBufferedString.h"

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
    afxNat8             schem, user, pass, host, port, fext;
    afxNat16            dir, fname;
    afxNat16            query, frag;
    afxBufferedString   str;
};

AFX_DEFINE_STRUCT(afxFixedUri8)
{
    afxUri              uri;
    afxByte             buf[8];
};

AFX_DEFINE_STRUCT(afxFixedUri128)
{
    afxUri              uri;
    afxByte             buf[128];
};

AFX_DEFINE_STRUCT(afxFixedUri2048)
{
    afxUri              uri;
    afxByte             buf[2048];
};

AFXINL afxUri*          AfxMakeFixedUri8(afxFixedUri8 *uri, afxUri const* src);
AFXINL afxUri*          AfxMakeFixedUri128(afxFixedUri128 *uri, afxUri const* src);
AFXINL afxUri*          AfxMakeFixedUri2048(afxFixedUri2048 *uri, afxUri const* src);

AFXINL void             AfxDeallocateUri(afxUri *uri);
AFXINL afxError         AfxAllocateUri(afxUri* uri, afxNat cap, void const *start, afxNat range);
AFXINL afxError         AfxReallocateUri(afxUri *uri, afxNat cap, void const *start, afxNat range);

AFXINL afxError         AfxCloneUri(afxUri* uri, afxUri const *in);
AFXINL afxError         AfxCloneUriPath(afxUri* uri, afxUri const *in);
AFXINL afxError         AfxCloneUriName(afxUri* uri, afxUri const *in);

// Make a new URI excerpting another URI.
AFXINL afxNat           AfxGetUriHost(afxUri *uri, afxUri const *src);
AFXINL afxNat           AfxGetUriPort(afxUri *uri, afxUri const *src);
AFXINL afxNat           AfxGetUriUsername(afxUri *uri, afxUri const *src);
AFXINL afxNat           AfxGetUriPassword(afxUri *uri, afxUri const *src);
AFXINL afxNat           AfxGetUriPath(afxUri *uri, afxUri const *src);
AFXINL afxNat           AfxGetUriDirectory(afxUri *uri, afxUri const *src);
AFXINL afxNat           AfxGetUriObject(afxUri *uri, afxUri const *src); // file name + extension (if any)
AFXINL afxNat           AfxGetUriName(afxUri *uri, afxUri const *src); // just file name, without extension
AFXINL afxNat           AfxGetUriNameString(afxUri const *src, afxString* name);
AFXINL afxNat           AfxGetUriExtension(afxUri *uri, afxUri const *src, afxBool skipDot);
AFXINL afxNat           AfxGetUriQuery(afxUri *uri, afxUri const *src, afxBool skipSep);
AFXINL afxNat           AfxGetUriQueryString(afxUri const *uri, afxBool skipSep, afxString* query);
AFXINL afxNat           AfxGetUriFragment(afxUri *uri, afxUri const *src, afxBool skipSep);
AFXINL afxNat           AfxGetUriScheme(afxUri *uri, afxUri const *src); // "https"

AFXINL afxNat           AfxGetUriPathRW(afxUri *uri, afxUri *src);
AFXINL afxNat           AfxGetUriDirectoryRW(afxUri *uri, afxUri *src);
AFXINL afxNat           AfxGetUriObjectRW(afxUri *uri, afxUri *src); // file name + extension
AFXINL afxNat           AfxGetUriNameRW(afxUri *uri, afxUri *src); // just file name, without extension
AFXINL afxNat           AfxGetUriExtensionRW(afxUri *uri, afxUri *src);

AFXINL void             AfxReplicateUri(afxUri *uri, afxUri const *in); // pointers and handle same data handled by 'in' without doing any copy or change.

AFXINL afxNat           AfxUriFromString(afxUri *uri, afxString const *src); // will not set capacity  (will be read only)
AFXINL afxNat           AfxUriFromStringRange(afxUri *uri, afxString const *src, afxNat offset, afxNat len);

AFXINL void             AfxMakeUri(afxUri *uri, void const *start, afxNat len); // will not set capacity (will be read only)

AFXINL void             AfxMakeBufferedUri(afxUri *uri, afxNat cap, void *start, afxNat len);

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

AFXINL void*            AfxGetBufferedUriData(afxUri *uri, afxNat base);
AFXINL void const*      AfxGetUriData(afxUri const *uri, afxNat base);
AFXINL afxBufferedString*AfxGetBufferedUriString(afxUri *uri);
AFXINL afxString const* AfxGetUriString(afxUri const *uri);

AFXINL afxNat           AfxGetUriUsernameLength(afxUri const *uri);
AFXINL afxNat           AfxGetUriPasswordLength(afxUri const *uri);
AFXINL afxNat           AfxGetUriHostLength(afxUri const *uri);
AFXINL afxNat           AfxGetUriPortLength(afxUri const *uri);
AFXINL afxNat           AfxGetUriFragmentLength(afxUri const *uri);
AFXINL afxNat           AfxGetUriPathLength(afxUri const *uri);
AFXINL afxNat           AfxGetUriDirectoryLength(afxUri const *uri);
AFXINL afxNat           AfxGetFileNameLength(afxUri const *uri);
AFXINL afxNat           AfxGetFileExtensionLength(afxUri const *uri);
AFXINL afxNat           AfxGetUriQueryLength(afxUri const *uri);
AFXINL afxNat           AfxGetUriSchemeLength(afxUri const *uri);

AFXINL afxBool          AfxUriIsBlank(afxUri const *uri);
AFXINL afxBool          AfxUriIsEditable(afxUri const *uri);
AFXINL afxBool          AfxPathIsRelative(afxUri const *uri);
AFXINL void             AfxNormalizePath(afxUri *uri);
AFXINL void             AfxReparseUri(afxUri *uri);

#endif//AFX_URI_H
