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

/**
    # The Qwadro String

    The Qwadro string, represented by the afxString structure, is a used for representing text, 
    and it can encode any character in the Unicode character set using a variable-length 
    encoding system, composed of one up to four bytes (8 to 32 bits). It is compatible with UTF-8.

    Unicode is a standard that assigns a unique code to every character in every language, symbol, 
    and special character, which helps to unify character representation across the world.
    UTF-8 encodes these Unicode characters efficiently in variable-length formats, making it 
    scalable for various languages and scripts.

    ## How must it works

    * Characters are encoded in 1 to 4 bytes.
    * The first 128 characters (which correspond to standard ASCII) are encoded in a single byte.
    * Characters beyond the ASCII range use 2, 3, or 4 bytes to represent a single character.
    * UTF-8 is compatible with ASCII for characters that fall within the ASCII range (0 to 127). 
      These characters use exactly 1 byte, and this makes it easy for UTF-8 to be used in systems 
      that previously only supported ASCII.
    * For characters that fall within the basic ASCII range, UTF-8 uses only 1 byte, making it space-efficient.
    * For other characters (e.g., accented letters, symbols, or non-Latin scripts), UTF-8 uses 2, 3, or 4 bytes.

    ## Example

    The character ASCII code 65, the 'A' letter, in UTF-8 is represented as a single byte: 0x41.
    The character U+00E9, the small E with acute, in UTF-8 is represented as two bytes: 0xC3 0xA9.
    The character U+1D11E, the musical symbol, will be encoded in four bytes: 0xF0 0x9D 0x84 0x9E.

    ## Proposal

    * UTF-8 has a wide compatibility, and is the most commonly used encoding on the web.
    * UTF-8 is backward compatible with ASCII, which makes it easy to integrate into existing systems.
    * UTF-8 adapts the number of bytes used for encoding based on the complexity of the character.
    * UTF-8 can represent any character from virtually any written language in the world.
*/

#ifndef AFX_STRING_H
#define AFX_STRING_H

#include "qwadro/inc/base/afxRawString.h"
#include "qwadro/inc/base/afxFcc.h"

#define AFX_MAX_STRING_LENGTH (AFX_U32_MAX)
#define AFX_MAX_STRING_CAPACITY (AFX_MAX_STRING_LENGTH + 1)

AFX_DEFINE_STRUCT(afxString)
{
    afxUnit              len;
    afxUnit              cap; // buffered
    union
    {
        afxChar const*  start; // wrapped or externally buffered.
        afxSize         offset;
        afxChar*        buf;
    };
};

#define AFX_STRING(text_) (afxString) { \
    .len = (afxUnit)((sizeof((text_)) / sizeof((text_)[0])) - sizeof(afxChar)), \
    .cap = 0, \
    .start = (afxChar const*)(text_) \
    }

AFX afxString const     AFX_STRING_EMPTY;

// NOTE: When 'cap' is zero, a string can't be modified.
//#define AfxAssertString(str_) AFX_ASSERT3(((afxChar*)&((str_)->fcc))[0] == 's', ((afxChar*)&((str_)->fcc))[1] == 't', ((afxChar*)&((str_)->fcc))[2] == 'r')

// used for formatted string input with %.*s specifier. // %.*s
#define AfxPushString(str_) ((str_) ? ((str_)->len) : 0), ((str_) ? ((str_)->start) : "")

AFXINL void             AfxResetStrings(afxUnit cnt, afxString strings[]);

AFXINL void             AfxClearStrings(afxUnit cnt, afxString strings[]);

AFXINL afxString*       AfxMakeString(afxString* s, afxUnit cap, void const *start, afxUnit len); // wraps constant (read-only) data as a Qwadro string.

// Makes a excerpt string from another string.
AFXINL void             AfxReflectString(afxString const* s, afxString* reflection); // wraps the same content mapped or buffered by other string as read-only data.
AFXINL afxUnit          AfxExcerptString(afxString const* src, afxUnit base, afxUnit len, afxString* selection); // wraps the same content mapped or buffered by other string as read-only data. Return clamped off range if any.
AFX afxUnit             AfxExcerptStringLine(afxString const* s, afxUnit base, afxString* excerpt); // wraps the same content mapped or buffered by %s until a '\n' char has been reached.

AFXINL afxChar const*   AfxGetStringData(afxString const* s, afxUnit base);
AFXINL afxChar*         AfxGetStringStorage(afxString const* s, afxUnit base);

AFXINL afxUnit          AfxGetStringLength(afxString const* s);
AFXINL afxUnit          AfxGetStringCapacity(afxString const* s, afxUnit from);

AFXINL afxBool          AfxIsStringEmpty(afxString const* s);

AFX afxResult           AfxDumpString(afxString const* s, afxUnit base, afxUnit range, void *dst); // copy out

AFX afxResult           AfxScanString(afxString const* s, afxChar const* fmt, ...);
AFX afxResult           AfxScanStringArg(afxString const* s, afxChar const* fmt, va_list args);

AFX afxUnit             AfxFormatString(afxString* s, afxChar const* fmt, ...);
AFX afxUnit             AfxFormatStringArg(afxString* s, afxChar const* fmt, va_list args);

/*
    The AfxCompareString() function compares a portion of a Qwadro string, starting from a specified index, 
    against another raw string for a given length. The comparison can be case-insensitive.

    The result indicate whether the raw string is equal to, less than, or greater than the Qwadro string to compare against.
*/

AFXINL afxResult    AfxCompareString
(
    // The Qwadro string.
    afxString const*s, 

    // The offset where to start comparing within the Qwadro string.
    afxUnit         base, 

    // The pointer to the raw string to be compared against.
    afxChar const*  start, 
    
    // The length of the raw string to compare (the number of characters).
    afxUnit         len, 

    // A flag that specifies whether the comparison should be case-insensitive or not.
    afxBool         ci
);

/*
    The AfxCompareStrings() function compares a string starting at a given position with each string in an 
    array of strings, with an optional case-insensitive comparison. If any match is found, return the index 
    of the first matched string from the array. Otherwise, return AFX_INVALID_INDEX.

    This function can be used in scenarios like switch/case statements where you want to check if a string matches one of several possible options.
    Return the index of the first matched other string or AFX_INVALID_INDEX. Can be used in switch/case statements.
*/

AFX afxBool         AfxCompareStrings
(
    // The source string.
    afxString const*s, 
    
    // The position in the string from which comparison should start.
    afxUnit         base, 
    
    // Case-insensitive flag.
    afxBool         ci, 
    
    // The number of strings in the @others array.
    afxUnit         cnt, 
    
    // The array of strings to compare against.
    afxString const others[],
    
    // A pointer to store the index of the matched string.
    // Can be NIL if you only want the boolean result.
    afxUnit*        matchedIdx
);

/*
    The AfxCompareSubstrings() compare a substring of @s starting at @base and having a length of @len with each string in @others. 
    Similar to AfxCompareStrings, it also supports case-insensitive comparisons and returns the index of the first match 
    or AFX_INVALID_INDEX.

    This function works similarly to AfxCompareStrings(), but instead of comparing the full string starting at base, 
    it compares a substring of s with a specified length @len, avoiding the need to excerpt it.

    This function can be used in scenarios like switch/case statements where you want to check if a substring matches one of several possible options.
*/

AFXINL afxBool      AfxCompareSubstrings
(
    // The source string.
    afxString const*s, 

    // The position in the string from which comparison should start.
    afxUnit         base, 

    // The length in the string up to where comparison should end.
    afxUnit         len, 

    // Case-insensitive flag.
    afxBool         ci, 

    // The number of strings in the @others array.
    afxUnit         cnt, 

    // The array of strings to compare against.
    afxString const others[],

    // A pointer to store the index of the matched string.
    // Can be NIL if you only want the boolean result.
    afxUnit*        matchedIdx
);

AFX afxUnit             AfxCatenateStrings(afxString* s, afxUnit cnt, afxString const src[]); // Return clamped off (non-copied) length if any.
AFX afxUnit             AfxCatenateString(afxString* s, afxChar const* start, afxUnit len); // Return clamped off (non-copied) length if any.

AFX afxUnit             AfxSpanString(afxString* s, afxString const* excerpt);
AFX afxUnit             AfxSpanString2(afxString* s, afxString const* stopset);

AFX afxUnit             AfxCopyString(afxString* s, afxString const* in); // Return clamped off (non-copied) length if any.
AFX afxUnit             AfxCopySubstring(afxString* s, afxString const* in, afxUnit base, afxUnit len); // Return clamped off (non-copied) length if any.

AFX afxError            AfxCloneString(afxString* s, afxString const* in); // wraps the same content mapped or buffered by other string as writeable data.
AFX afxError            AfxCloneSubstring(afxString* s, afxString const* in, afxUnit base, afxUnit len); // wraps the same content mapped or buffered by other string as writeable data.

AFX void                AfxDeallocateString(afxString* s);
AFX afxError            AfxAllocateString(afxString* s, afxUnit cap, void const *start, afxUnit len);
AFX afxError            AfxReallocateString(afxString* s, afxUnit cap, void const *start, afxUnit len);

AFX afxUnit             _AfxWriteString(afxString* s, afxUnit base, void const *src, afxUnit len); // copy into

AFX afxUnit             AfxInsertString(afxString* s, afxUnit at, afxString const* include);
AFX afxUnit             AfxEraseString(afxString* s, afxUnit at, afxUnit len);

// return the position of found char/substring, in else case, it should be AFX_INVALID_INDEX.
AFXINL afxBool          AfxFindFirstChar(afxString const* s, afxUnit from, afxInt ch, afxUnit* posn);
AFXINL afxBool          AfxFindLastChar(afxString const* s, afxUnit from, afxInt ch, afxUnit* posn);
AFXINL afxUnit          AfxFindSubstring(afxString const* s, afxString const* excerpt);

    // TODO GetAs(s, szScheme, pvData) -> GetAs(s, "%x", data)
    // TODO GetAsMeasured(s, szScheme, nLen, pvData) -> GetAs(s, "%.*s", data)
AFX afxError            AfxGetStringAsHex(afxString const* s, afxUnit32 *value);
AFX afxError            AfxGetStringAsReal(afxString const* s, afxReal *value);

AFXINL afxError         AfxReadString(afxString* s, afxStream in, afxUnit len);

/*
    The AfxExportString16SIGMA() function provides a platform-independent way to export 
    Qwadro's native (UTF-8) strings to the wide-character (UTF-16) format. It handles 
    1- to 4-byte UTF-8 sequences and converts them to a UTF-16 wide character representation.

    Returns the length of the exported string as wide-character (UTF-16) string.
*/

AFXINL afxUnit AfxExportString16SIGMA
(
    afxString const*s, 
    afxUnit         bufCap, 
    wchar_t*        wideBuf
);

/*
    The AfxImportString16SIGMA() function provides a platform-independent way to convert 
    wide-character (UTF-16) strings to UTF-8 (multibyte) strings. It can handle all Unicode 
    characters (up to 0x10FFFF) and properly encodes them into UTF-8.

    Returns the length of the imported string as Qwadro native (UTF-8) string.
*/

AFXINL afxUnit AfxImportString16SIGMA
(
    afxString*      s, 
    wchar_t const*  wideStr, 
    afxUnit         wideStrLen
);

#endif//AFX_STRING_H
