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
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This code is part of SIGMA Future Storage <https://sigmaco.org/future-storage>

// In QWADRO, URI is a type of string specialized in dealing with file paths.
// It provides an object representation of a uniform resource identifier (URI) and easy access to the parts of the URI.

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

typedef enum afxUriPart
{
    afxUriPart_SCHEME   = AFX_BIT(0),
    afxUriPart_HOST     = AFX_BIT(1),
    afxUriPart_PORT     = AFX_BIT(2),
    afxUriPart_AUTHORITY= (afxUriPart_HOST | afxUriPart_PORT),
    afxUriPart_DIR      = AFX_BIT(3),
    afxUriPart_TARGET   = AFX_BIT(4),
    afxUriPart_FILEXT   = AFX_BIT(5),
    afxUriPart_FILE     = (afxUriPart_TARGET | afxUriPart_FILEXT),
    afxUriPart_PATH     = (afxUriPart_DIR | afxUriPart_FILE | afxUriPart_FILEXT),
    afxUriPart_QUERY    = AFX_BIT(6),
    afxUriPart_FRAGMENT = AFX_BIT(7)
} afxUriPart;

AFX_DEFINE_STRUCT(afxUri)
/// An object representing a uniform resource identifier (URI).
{
    afxRestring         str;
    afxNat8             schem, user, pass, host, port, fname, fext, frag; // max AFX_N8_MAX long
    afxNat16            dir, query; // max AFX_N16_MAX long
};

AFX_DEFINE_STRUCT(afxUri8) { afxUri uri; afxByte buf[8]; };
AFX_DEFINE_STRUCT(afxUri32) { afxUri uri; afxByte buf[32]; };
AFX_DEFINE_STRUCT(afxUri128) { afxUri uri; afxByte buf[128]; };
AFX_DEFINE_STRUCT(afxUri2048) { afxUri uri; afxByte buf[2048]; };

// Deployment

AFXINL afxUri*          AfxMakeUri8(afxUri8 *uri, afxUri const* src);
AFXINL afxUri*          AfxMakeUri32(afxUri32 *uri, afxUri const* src);
AFXINL afxUri*          AfxMakeUri128(afxUri128 *uri, afxUri const* src);
AFXINL afxUri*          AfxMakeUri2048(afxUri2048 *uri, afxUri const* src);

AFXINL void             AfxResetUri(afxUri *uri); // WARNING string storage is resetted too. Any storage assigned will be nulled.

AFXINL void             AfxMakeUri(afxUri *uri, void const *start, afxNat len); // will not set capacity (will be read only)
AFXINL afxNat           AfxMakeUriFromString(afxUri *uri, afxString const *src); // will not set capacity  (will be read only)
AFXINL afxNat           AfxMakeUriFromStringRange(afxUri *uri, afxString const *src, afxNat offset, afxNat len);
AFXINL afxNat           AfxMakeUriFromRestring(afxUri *uri, afxRestring const *src);
AFXINL afxNat           AfxMakeUriFromRestringRange(afxUri *uri, afxRestring const *src, afxNat offset, afxNat len);

AFXINL void             AfxMakeMutableUri(afxUri *uri, afxNat cap, void *start, afxNat len);

// Allocation

AFXINL void             AfxDeallocateUri(afxUri *uri);
AFXINL afxError         AfxAllocateUri(afxUri* uri, afxNat cap, void const *start, afxNat range);
AFXINL afxError         AfxReallocateUri(afxUri *uri, afxNat cap, void const *start, afxNat range);

AFXINL afxError         AfxDuplicateUri(afxUri* uri, afxUri const *in);
AFXINL afxError         AfxDuplicateUriPath(afxUri* uri, afxUri const *in);
AFXINL afxError         AfxDuplicateUriObject(afxUri* uri, afxUri const *in);

// Extraction

// Make a new URI excerpting another URI. Return the length of exceprt.
AFXINL afxNat           AfxPickUriScheme(afxUri const* uri, afxUri* schem); // "https"

/// Gets the Domain Name System (DNS) host name or IP address and the port number for a server.
AFXINL afxNat           AfxPickUriAuthority(afxUri const* uri, afxUri* host, afxUri* port);

/// Gets the user name, password, or other user-specific information associated with the specified URI.
AFXINL afxNat           AfxPickUriUserInfo(afxUri const* uri, afxUri* usr, afxUri* pass);

AFXINL afxNat           AfxClipUriPath(afxUri* uri, afxUri const* from);
AFXINL afxNat           AfxClipUriDirectory(afxUri* uri, afxUri const* from);
AFXINL afxNat           AfxClipUriFile(afxUri* uri, afxUri const* from); // file name + extension (if any)
AFXINL afxNat           AfxClipUriTarget(afxUri* uri, afxUri const* from); // just file name, without extension
AFXINL afxNat           AfxClipUriExtension(afxUri* uri, afxUri const* from, afxBool skipDot);
AFXINL afxNat           AfxClipUriQuery(afxUri* uri, afxUri const* from, afxBool skipSep);
AFXINL afxNat           AfxClipUriFragment(afxUri* uri, afxUri const* from, afxBool skipSep);

AFXINL afxNat           AfxGetUriTargetString(afxUri const *src, afxString* name);
AFXINL afxNat           AfxGetUriQueryString(afxUri const *uri, afxBool skipSep, afxString* query);

AFXINL void             AfxReplicateUri(afxUri *uri, afxUri const *in); // pointers and handle same data handled by 'in' without doing any copy or change.

// require initalized URI.

/// Parses the URI to ensure it contains all the parts required for a valid URI.
AFXINL afxNat           AfxParseUri(afxUri* uri, afxString const* from); // make URI from a Qwadro string.

/// Compares the specified parts of two URIs using the specified comparison rules.
AFXINL afxResult        AfxCompareUri(afxUri const *uri, afxUri const *other);
AFXINL afxResult        AfxCompareUriCi(afxUri const *uri, afxUri const *other);
AFXINL afxResult        AfxCompareUriString(afxUri const *uri, afxString const *str);
AFXINL afxResult        AfxCompareUriStringCi(afxUri const *uri, afxString const *str);

AFXINL void             AfxClearUri(afxUri *uri); // just zero ranges preserving storage.
AFXINL void             AfxFormatUri(afxUri *uri, afxChar const *fmt, ...);

AFXINL afxError         AfxCopyUri(afxUri *uri, afxUri const *src);
AFXINL afxError         AfxCopyUriPath(afxUri *uri, afxUri const *src);
AFXINL afxError         AfxCopyUriDirectory(afxUri *uri, afxUri const *src);
AFXINL afxError         AfxCopyUriFile(afxUri *uri, afxUri const *src); // file name + extension
AFXINL afxError         AfxCopyUriTarget(afxUri *uri, afxUri const *src); // just file name, without extension
AFXINL afxError         AfxCopyUriExtension(afxUri *uri, afxUri const *src, afxBool skipDot);
AFXINL afxError         AfxCopyUriQuery(afxUri *uri, afxUri const *src, afxBool skipSep);

AFXINL afxNat           AfxMeasureUri(afxUri const* uri);
AFXINL afxNat           AfxMeasurePath(afxUri const* uri);
AFXINL afxNat           AfxMeasureUriRequest(afxUri const* uri);

AFXINL void const*      AfxGetUriData(afxUri const *uri, afxNat base);
AFXINL afxString const* AfxGetUriString(afxUri const *uri);
AFXINL void*            AfxGetUriStorage(afxUri *uri, afxNat base);
AFXINL afxRestring*     AfxGetUriRestring(afxUri *uri);

AFXINL afxBool          AfxUriIsBlank(afxUri const *uri);
AFXINL afxBool          AfxUriIsEditable(afxUri const *uri);

/// Determines whether the current URI is a base of the specified URI.
AFXINL afxBool          AfxIsBaseOfUri(afxUri const *uri, afxUri const* other);

/// Test whether the URI is an absolute URI, which contains a scheme, an authority, and a path. 
/// Otherwise, the URI is treated as relative and might omit the scheme or other URI components.
AFXINL afxBool          AfxIsUriAbsolute(afxUri const *uri);

AFXINL afxBool          AfxPathIsRelative(afxUri const *uri);

/// Converts the internally stored URI to canonical form.
AFXINL void             AfxCanonicalizePath(afxUri* uri, afxBool microshit);

AFXINL void             AfxReparseUri(afxUri *uri);

#endif//AFX_URI_H
