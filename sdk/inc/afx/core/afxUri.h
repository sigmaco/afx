/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *              T H E   Q W A D R O   E X E C U T I O N   E C O S Y S T E M
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

AFX void                AfxUri128(afxUri128 *uri);
AFX void                AfxUri2048(afxUri2048 *uri);

AFX void                AfxUriDeallocate(afxUri *uri);
AFX afxError            AfxUriAllocate(afxUri* uri, afxNat cap, void const *start, afxNat range);
AFX afxError            AfxUriReallocate(afxUri *uri, afxNat cap, void const *start, afxNat range);

AFX afxError            AfxCloneUri(afxUri* uri, afxUri const *in);
AFX afxError            AfxCloneUriPath(afxUri* uri, afxUri const *in);
AFX afxError            AfxCloneUriName(afxUri* uri, afxUri const *in);

// Make a new URI excerpting another URI.
AFX afxNat              AfxExcerptUriHost(afxUri *uri, afxUri const *parent);
AFX afxNat              AfxExcerptUriPort(afxUri *uri, afxUri const *parent);
AFX afxNat              AfxExcerptUriUsername(afxUri *uri, afxUri const *parent);
AFX afxNat              AfxExcerptUriPassword(afxUri *uri, afxUri const *parent);
AFX afxNat              AfxExcerptUriPath(afxUri *uri, afxUri const *parent);
AFX afxNat              AfxExcerptUriDirectory(afxUri *uri, afxUri const *parent);
AFX afxNat              AfxExcerptUriObject(afxUri *uri, afxUri const *parent); // file name + extension (if any)
AFX afxNat              AfxExcerptUriName(afxUri *uri, afxUri const *parent); // just file name, without extension
AFX afxNat              AfxExcerptUriExtension(afxUri *uri, afxUri const *parent, afxBool skipDot);
AFX afxNat              AfxExcerptUriQuery(afxUri *uri, afxUri const *parent, afxBool skipSep);
AFX afxNat              AfxExcerptUriFragment(afxUri *uri, afxUri const *parent, afxBool skipSep);
AFX afxNat              AfxExcerptUriScheme(afxUri *uri, afxUri const *parent); // "https"

AFX afxNat              AfxExcerptUriPathRW(afxUri *uri, afxUri *parent);
AFX afxNat              AfxExcerptUriDirectoryRW(afxUri *uri, afxUri *parent);
AFX afxNat              AfxExcerptUriObjectRW(afxUri *uri, afxUri *parent); // file name + extension
AFX afxNat              AfxExcerptUriNameRW(afxUri *uri, afxUri *parent); // just file name, without extension
AFX afxNat              AfxExcerptUriExtensionRW(afxUri *uri, afxUri *parent);

AFX void                AfxReflectUri(afxUri *uri, afxUri const *in); // pointers and handle same data handled by 'in' without doing any copy or change.

AFX void                AfxReflectUriString(afxUri *uri, afxString const *src); // will not set capacity  (will be read only)

AFX void                AfxUriWrapLiteral(afxUri *uri, void const *start, afxNat range); // will not set capacity (will be read only)
AFX void                AfxUriWrapLiteralRW(afxUri *uri, void *start, afxNat range);

AFX void                AfxUriWrapBuffer(afxUri *uri, void const *start, afxNat cap);

AFX void                AfxResetUri(afxUri *uri); // WARNING string storage is resetted too. Any storage assigned will be nulled.

// require initalized URI.

AFX afxBool             AfxUriIsEquivalent(afxUri const *uri, afxUri const *other);
AFX afxBool             AfxUriIsEquivalentCi(afxUri const *uri, afxUri const *other);
AFX afxBool             AfxUriIsEquivalentToString(afxUri const *uri, afxString const *str);
AFX afxBool             AfxUriIsEquivalentToStringCi(afxUri const *uri, afxString const *str);

AFX void                AfxEraseUri(afxUri *uri); // just zero ranges preserving storage.
AFX void                AfxFormatUri(afxUri *uri, afxChar const *fmt, ...);

AFX afxError            AfxCopyUri(afxUri *uri, afxUri const *src);
AFX afxError            AfxCopyUriHost(afxUri *uri, afxUri const *src);
AFX afxError            AfxCopyUriPort(afxUri *uri, afxUri const *src);
AFX afxError            AfxCopyUriUsername(afxUri *uri, afxUri const *src);
AFX afxError            AfxCopyUriPassword(afxUri *uri, afxUri const *src);
AFX afxError            AfxCopyUriPath(afxUri *uri, afxUri const *src);
AFX afxError            AfxCopyUriDirectory(afxUri *uri, afxUri const *src);
AFX afxError            AfxCopyUriObject(afxUri *uri, afxUri const *src); // file name + extension
AFX afxError            AfxCopyUriName(afxUri *uri, afxUri const *src); // just file name, without extension
AFX afxError            AfxCopyUriExtension(afxUri *uri, afxUri const *src, afxBool skipDot);
AFX afxError            AfxCopyUriFragment(afxUri *uri, afxUri const *src, afxBool skipSep);
AFX afxError            AfxCopyUriQuery(afxUri *uri, afxUri const *src, afxBool skipSep);
AFX afxError            AfxCopyUriScheme(afxUri *uri, afxUri const *src);

AFX afxResult           AfxUriForEachQueryKey(afxUri const *uri, afxResult(*f)(afxString *key, afxString *value, void* data), void* data);

AFX void*               AfxGetUriData(afxUri *uri, afxNat base);
AFX void const*         AfxGetUriDataConst(afxUri const *uri, afxNat base);
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
AFX void                AfxNormalizeUriPath(afxUri *uri);
AFX void                AfxReparseUri(afxUri *uri);

#endif//AFX_URI_H