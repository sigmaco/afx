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

// This code is part of SIGMA Future Storage.

// In QWADRO, URI is a type of string specialized in dealing with file paths.
// It provides an object representation of a uniform resource identifier (URI) and easy access to the parts of the URI.

#ifndef AFX_URI_H
#define AFX_URI_H

#include "qwadro/inc/base/afxString.h"

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
    afxString   s;
    afxUnit8    sch, user, h, p, root, fname, fext, frag; // max AFX_U8_MAX long
    afxUnit16   dir, qry; // max AFX_U16_MAX long
};

AFX_DEFINE_STRUCT(afxUri8) { afxUri uri; afxByte buf[8]; };
AFX_DEFINE_STRUCT(afxUri32) { afxUri uri; afxByte buf[32]; };
AFX_DEFINE_STRUCT(afxUri128) { afxUri uri; afxByte buf[128]; };
AFX_DEFINE_STRUCT(afxUri2048) { afxUri uri; afxByte buf[2048]; };

#define afxFixedUri(varName_,capacity_) \
    afxChar varName_##Buf[capacity_]; afxUri varName_ = { .s.len = 0, .s.cap = sizeof(varName_##Buf), .s.start = varName_##Buf }; \


// Properties

AFXINL afxUnit          AfxGetUriLength(afxUri const* uri);
AFXINL afxUnit          AfxGetPathLength(afxUri const* uri);
AFXINL afxUnit          AfxGetUriRequestLength(afxUri const* uri);

AFXINL void const*      AfxGetUriData(afxUri const* uri, afxUnit base);
AFXINL afxString const* AfxGetUriString(afxUri const* uri);
AFXINL afxString*       AfxGetUriString2(afxUri* uri);

// Tests

AFXINL afxBool          AfxIsUriBlank(afxUri const* uri);
AFXINL afxBool          AfxUriIsEditable(afxUri const* uri);

/// Test whether the URI is an absolute URI, which contains a scheme, an authority, and a path. 
/// Otherwise, the URI is treated as relative and might omit the scheme or other URI components.
AFXINL afxBool          AfxIsUriAbsolute(afxUri const* uri);

AFXINL afxBool          AfxPathIsRelative(afxUri const* uri);

AFXINL afxBool          AfxIsUriToDevice(afxUri const* uri);

/// Determines whether the current URI is a base of the specified URI.
AFXINL afxBool          AfxIsBaseOfUri(afxUri const* uri, afxUri const* other);

/// Compares the specified parts of two URIs using the specified comparison rules.
AFXINL afxResult        AfxCompareUri(afxUri const* uri, afxUri const *other);
AFXINL afxResult        AfxCompareUriCi(afxUri const* uri, afxUri const *other);
AFXINL afxResult        AfxCompareUriString(afxUri const* uri, afxString const *str);
AFXINL afxResult        AfxCompareUriStringCi(afxUri const* uri, afxString const *str);

// Changes

AFXINL void             AfxResetUris(afxUnit cnt, afxUri uris[]); // WARNING string storage is resetted too. Any storage assigned will be nulled.
AFXINL void             AfxClearUris(afxUnit cnt, afxUri uris[]); // just zero ranges preserving storage.

AFXINL afxError         AfxCopyUri(afxUri* uri, afxUri const *src);
AFXINL void             AfxReplicateUri(afxUri* uri, afxUri const *in); // pointers and handle same data handled by 'in' without doing any copy or change.

/// Converts the internally stored URI to canonical form.
AFXINL void             AfxCanonicalizePath(afxUri* uri, afxBool microshit);
AFXINL void             AfxTransformPathString(afxString* str, afxBool out);

/// Parses the URI to ensure it contains all the parts required for a valid URI.
AFXINL void             AfxReparseUri(afxUri* uri);

AFXINL void             AfxFormatUri(afxUri* uri, afxChar const *fmt, ...);

// Deployment

AFXINL afxUri           AfxMakeUri2(afxUnit cap, void const* start, afxUnit len);
AFXINL afxUri*          AfxMakeUri(afxUri* uri, afxUnit cap, void const *start, afxUnit len); // will not set capacity (will be read only)
AFXINL afxUnit          AfxWrapUriString(afxUri* uri, afxString const *src); // will not set capacity  (will be read only)
AFXINL afxUnit          AfxWrapUriStringRange(afxUri* uri, afxString const *src, afxUnit offset, afxUnit len);

AFXINL afxUri*          AfxMakeUri8(afxUri8 *uri, afxUri const* src);
AFXINL afxUri*          AfxMakeUri32(afxUri32 *uri, afxUri const* src);
AFXINL afxUri*          AfxMakeUri128(afxUri128 *uri, afxUri const* src);
AFXINL afxUri*          AfxMakeUri2048(afxUri2048 *uri, afxUri const* src);

AFXINL afxUri*          AfxFormatUri8(afxUri8 *uri, afxChar const *fmt, ...);
AFXINL afxUri*          AfxFormatUri32(afxUri32 *uri, afxChar const *fmt, ...);
AFXINL afxUri*          AfxFormatUri128(afxUri128 *uri, afxChar const *fmt, ...);
AFXINL afxUri*          AfxFormatUri2048(afxUri2048 *uri, afxChar const *fmt, ...);

// Allocation

AFXINL void             AfxDeallocateUri(afxUri* uri);
AFXINL afxError         AfxAllocateUri(afxUri* uri, afxUnit cap, void const *start, afxUnit range);
AFXINL afxError         AfxReallocateUri(afxUri* uri, afxUnit cap, void const *start, afxUnit range);

AFXINL afxError         AfxDuplicateUri(afxUri* uri, afxUri const *in);
AFXINL afxError         AfxDuplicateUriPath(afxUri* uri, afxUri const *in);
AFXINL afxError         AfxDuplicateUriObject(afxUri* uri, afxUri const *in);

// Extraction

// Make a new URI excerpting another URI. Return the length of exceprt.
AFXINL afxUnit          AfxExcerptUriSchema(afxUri const* uri, afxUri* schem); // "https"

/// Gets the Domain Name System (DNS) host name or IP address and the port number for a server.
AFXINL afxUnit          AfxExcerptUriAuthority(afxUri const* uri, afxUri* user, afxUri* host, afxUri* port);

AFXINL afxUnit          AfxExcerptUriQuery(afxUri const* uri, afxBool skipSep, afxUri* query);
AFXINL afxUnit          AfxExcerptUriFragment(afxUri const* uri, afxBool skipSep, afxUri* frag);

AFXINL afxUnit          AfxClipUriPath(afxUri* uri, afxUri const* from);
AFXINL afxUnit          AfxClipPath2(afxUri* uri, afxUri const* from, afxUnit seg);
AFXINL afxUnit          AfxClipPathDirectory(afxUri* uri, afxUri const* from);
AFXINL afxUnit          AfxClipPathDirectory2(afxUri* uri, afxUri const* from, afxUnit seg);
AFXINL afxUnit          AfxClipUriFile(afxUri* uri, afxUri const* from); // file name + extension (if any)
AFXINL afxUnit          AfxClipUriTarget(afxUri* uri, afxUri const* from); // just file name, without extension
AFXINL afxUnit          AfxClipUriExtension(afxUri* uri, afxUri const* from, afxBool skipDot);

AFXINL afxUnit          AfxClipUriQuery(afxUri* uri, afxUri const* from, afxBool skipSep);
AFXINL afxUnit          AfxClipUriFragment(afxUri* uri, afxUri const* from, afxBool skipSep);

AFXINL afxUnit          AfxClipUriDevice(afxUri* uri, afxUri const* from);

AFXINL afxUnit          AfxGetUriTargetString(afxUri const *src, afxString* name);
AFXINL afxUnit          AfxGetUriQueryString(afxUri const* uri, afxBool skipSep, afxString* query);

AFX_DEFINE_STRUCT(afxUriPathInfo)
{
    afxUri path;
    afxUri root;
    afxUri dir;
    afxUri fname;
    afxUri fext;
};

AFXINL afxUnit AfxPickPath(afxUri const* uri, afxUri root, afxUri dir, afxUri file, afxUri fext, afxBool skipDot);
AFXINL afxUnit AfxTrimPath(afxUri const* uri, afxBool root, afxBool dir, afxBool file, afxBool ext);
AFXINL afxUnit AfxSplitPath(afxUri const* uri, afxUri* root, afxUri* path);
AFXINL afxUnit AfxPopPathRoot(afxUri* uri);

AFXINL afxUnit AfxExcerptPathBase(afxUri const* uri, afxUri* base); // Excerpts the //./dev/dir/to/
AFXINL afxUnit AfxExcerptPathSegments(afxUri const* uri, afxUri* root, afxUri* dir, afxUri* file, afxUri* ext);

#define AfxUri(_text_) AfxMakeUri((afxUri[]){ 0 }, 0, (_text_), ((sizeof((_text_)) / sizeof((_text_)[0])) - sizeof(afxChar)))

#endif//AFX_URI_H
