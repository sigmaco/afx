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

#include "qwadro/core/afxRestring.h"

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
    afxNat8             schem, user, pass, host, port, fext;
    afxNat16            dir, fname;
    afxNat16            query, frag;
    afxRestring   str;
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

// Make a new URI excerpting another URI. Return the length of exceprt.
AFXINL afxNat           AfxExcerptUriScheme(afxUri const* uri, afxUri* schem); // "https"
AFXINL afxNat           AfxExcerptUriHost(afxUri const* uri, afxUri* host);
AFXINL afxNat           AfxExcerptUriPort(afxUri const* uri, afxUri* port);
AFXINL afxNat           AfxExcerptUriUser(afxUri const* uri, afxUri* usr);
AFXINL afxNat           AfxExcerptUriPassword(afxUri const* uri, afxUri* pass);
AFXINL afxNat           AfxExcerptUriPath(afxUri const* uri, afxUri* path);
AFXINL afxNat           AfxExcerptUriDirectory(afxUri const* uri, afxUri* dir);
AFXINL afxNat           AfxExcerptUriFile(afxUri const* uri, afxUri* file); // file name + extension (if any)
AFXINL afxNat           AfxExcerptUriName(afxUri const* uri, afxUri* name); // just file name, without extension
AFXINL afxNat           AfxExcerptUriNameToString(afxUri const *src, afxString* name);
AFXINL afxNat           AfxExcerptUriExtension(afxUri const* uri, afxBool skipDot, afxUri* ext);
AFXINL afxNat           AfxExcerptUriQuery(afxUri const* uri, afxBool skipSep, afxUri* query);
AFXINL afxNat           AfxExcerptUriQueryToString(afxUri const *uri, afxBool skipSep, afxString* query);
AFXINL afxNat           AfxExcerptUriFragment(afxUri const* uri, afxBool skipSep, afxUri* frag);

AFXINL void             AfxReplicateUri(afxUri *uri, afxUri const *in); // pointers and handle same data handled by 'in' without doing any copy or change.

AFXINL afxNat           AfxUriFromString(afxUri *uri, afxString const *src); // will not set capacity  (will be read only)
AFXINL afxNat           AfxUriFromStringRange(afxUri *uri, afxString const *src, afxNat offset, afxNat len);

AFXINL afxNat           AfxMakeUriFromRestring(afxUri *uri, afxRestring const *src);
AFXINL afxNat           AfxMakeUriFromRestringRange(afxUri *uri, afxRestring const *src, afxNat offset, afxNat len);

AFXINL void             AfxMakeUri(afxUri *uri, void const *start, afxNat len); // will not set capacity (will be read only)

AFXINL void             AfxMakeMutableUri(afxUri *uri, afxNat cap, void *start, afxNat len);

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

AFXINL void*            AfxGetUriStorage(afxUri *uri, afxNat base);
AFXINL void const*      AfxGetUriData(afxUri const *uri, afxNat base);
AFXINL afxRestring*AfxGetUriRestring(afxUri *uri);
AFXINL afxString const* AfxGetUriString(afxUri const *uri);

AFXINL afxBool          AfxUriIsBlank(afxUri const *uri);
AFXINL afxBool          AfxUriIsEditable(afxUri const *uri);
AFXINL afxBool          AfxPathIsRelative(afxUri const *uri);
AFXINL void             AfxCanonicalizePath(afxUri* uri, afxBool microshit);
AFXINL void             AfxReparseUri(afxUri *uri);

#endif//AFX_URI_H
