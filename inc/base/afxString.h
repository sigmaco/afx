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

typedef enum afxStringFlag
{
    // Treat as case-insensitive.
    afxStringFlag_CI    = AFX_BITMASK(0),
    // Go reversely/B2F.
    afxStringFlag_REV   = AFX_BITMASK(1),
    // Expects as UTF16
    afxStringFlag_UTF16 = AFX_BITMASK(2),
} afxStringFlags;

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
    .cap = 0, .start = (afxChar const*)(text_) }

AFX afxString const     AFX_STRING_EMPTY;

// NOTE: When 'cap' is zero, a string can't be modified.
//#define AfxAssertString(str_) AFX_ASSERT3(((afxChar*)&((str_)->fcc))[0] == 's', ((afxChar*)&((str_)->fcc))[1] == 't', ((afxChar*)&((str_)->fcc))[2] == 'r')

// used for formatted string input with %.*s specifier. // %.*s
#define AfxPushString(str_) ((str_) ? ((str_)->len) : 0), ((str_) ? ((str_)->start) : "")

AFX afxString*          AfxMakeString
// Wraps constant (read-only) data as a Qwadro string.
(
    afxString* s, 
    afxUnit cap, 
    void const *start, 
    afxUnit len
);

AFX afxChar const*      AfxGetStringData
(
    afxString const*    s, 
    afxUnit             base
);

AFX afxChar*            AfxGetStringStorage
(
    afxString const*    s, 
    afxUnit             base
);

AFX afxUnit             AfxGetStringLength
(
    afxString const*    s
);

AFX afxUnit             AfxGetStringCapacity
(
    afxString const*    s, 
    afxUnit             from
);

AFX afxBool             AfxIsStringEmpty
(
    afxString const*    s
);

/*
    The AfxExcerptString() function returns a substring view, by adjusting the pointer and length, without allocating anything.
    No allocation. The function simply returns a view into the source string.
    Safe length adjustment. Clamps length if it exceeds bounds.
*/

AFX afxString           AfxExcerptString
(
    afxString const*    src, 
    afxUnit             from, 
    afxUnit             len
);

/*
    The AfxExcerptStringLine() function extracts one line from a string view.
    Stops on either \r or \n, compatible with Windows/Unix line endings.
    Does not skip over the line break. It's up to the caller to handle from + line.len + 1, or adjust for \r\n if needed.
    Returns a view (no allocation), safe and fast.
    Use in loop to read line-by-line from an afxString.
*/

AFX afxString           AfxExcerptStringLine
(
    afxString const*    s,
    afxUnit             from
);

/*
    The AfxCopyString() function is just a wrapper for AfxCopySubstring() which automatically determines the length.

*/

AFX afxUnit             AfxCopyString
// Return the number of characters that could not be copied due to capacity limits.
(
    // The destination string.
    afxString*          dst,
    // The destination offset to start copying at.
    afxUnit             at,
    // The source string.
    afxString const*    src,
    // The source offset to start copying from.
    afxUnit             from
);

/*
    The AfxCopySubstring() function copies part of an source string into a destination string,
    starting at a specified position in the destination, and copying from a specified position in the source.
    It should respect the destination buffer's capacity and return the number of bytes not copied due to lack of space.

*/

AFX afxUnit             AfxCopySubstring
// Return the number of characters that could not be copied due to capacity limits.
(
    // The destination string.
    afxString*          dst,
    // The destination offset to start copying at.
    afxUnit             at,
    // The source string.
    afxString const*    src,
    // The source offset to start copying from.
    afxUnit             from,
    // How many characters to copy.
    afxUnit             len
);

/*
    The AfxMoveString() function is just a wrappter for AfxMoveSubstring() which automatically determine the length.
*/

AFX afxUnit             AfxMoveString
// Return the number of characters that could not be copied due to capacity limits.
(
    // The destination string.
    afxString*          dst,
    // The destination offset to start copying at.
    afxUnit             at,
    // The source string.
    afxString const*    src,
    // The source offset to start copying from.
    afxUnit             from
);

/*
    The AfxMoveSubstring() function is just like AfxCopySubstring(), except it performs
    overlapping and/or in-place memory-safe string movement, allowing the source and destination
    to be safely copied when both maps partially or totally the same buffer memory.
*/

AFX afxUnit             AfxMoveSubstring
// Return the number of characters that could not be copied due to capacity limits.
(
    // The destination string.
    afxString*          dst,
    // The destination offset to start copying at.
    afxUnit             at,
    // The source string.
    afxString const*    src,
    // The source offset to start copying from.
    afxUnit             from,
    // How many characters to copy.
    afxUnit             len
);

/*
    The AfxCatenateStrings() function appends/concatenates multiple strings into another string.
    Must return the number of characters that could not be appended due to capacity limits.
    This function is designed to be both efficient and informative for overflow detection.
*/

AFX afxUnit             AfxCatenateStrings
// Return the number of characters that could not be appended due to capacity limits.
(
    // The destination string.
    afxString*          s, 
    // The count of strings.
    afxUnit             cnt, 
    // An array of source strings.
    afxString const     src[]
);

/*
    The AfxCatenateString() function is just a wrapper which automatically 
    wraps the a raw string into a Qwadro string and calls AfxCatenateStrings().

    This function uses a hybrid design to give it flexibility while staying efficient and safe.
    If the length is zero, treat the string as null-terminated and measure it. Otherwise, treat it as a fixed-size buffer.
*/

AFX afxUnit             AfxCatenateString
// Return the number of characters that could not be appended due to capacity limits.
(
    // The destination string.
    afxString*          s, 
    // The start of the raw string.
    afxChar const*      start,
    // The optional length of the raw string.
    // If not specified, it will be measured internally.
    afxUnit             len
);

/*
    The AfxInsertString() function inserts multiple strings into an existing afxString at a specific offset.
    Returns the number of characters that could not be inserted due to capacity limits. Inserts a null terminator if possible.

    Inserts as many characters as capacity allows preserving and shifting trailing content.
    Adds a null terminator if space allows. Returns the number of characters not inserted.
*/

AFX afxUnit             AfxInsertString
// Returns the number of characters not inserted.
(
    // The destination string.
    afxString*          dst,
    // The starting position at destination string.
    afxUnit             at,
    // The number of source strings to insert.
    afxUnit             cnt,
    // An array of source strings to insert.
    afxString const     src[]
);

/*
    The AfxEraseString() erases a portion of the string starting at at, for up to len characters.
    Shifts the tail left to fill the gap. Returns the number of characters actually erased.
*/

AFX afxUnit             AfxEraseString
(
    // The string to be partially or totally erased.
    afxString*          s,
    // The position to start erasing from.
    afxUnit             from,
    // The length to be erased.
    afxUnit             range
);

/*
    The AfxCompareString() function just wraps a raw string into a afxString and pass it to AfxCompareSubstrings().
    Return zero if strings are equal.
*/

AFX afxResult           AfxCompareString
(
    // The Qwadro string.
    afxString const*    s,

    // The offset where to start comparing within the Qwadro string.
    afxUnit             from,

    // The pointer to the raw string to be compared against.
    afxChar const*      start,

    // The length of the raw string to compare (the number of characters).
    afxUnit             len,

    // A flag that specifies whether the comparison should be case-insensitive or not.
    afxBool             ci
);

/*
    The AfxCompareStrings() function is just a wrapper for AfxCompareSubstrings() which automatically determines the length.
*/

AFX afxBool             AfxCompareStrings
(
    // The source string.
    afxString const*    s,

    // The position in the string from which comparison should start.
    afxUnit             from,

    // Case-insensitive flag.
    afxBool             ci,

    // The number of strings in the @others array.
    afxUnit             cnt,

    // The array of strings to compare against.
    afxString const     others[],

    // A pointer to store the index of the matched string.
    // Can be NIL if you only want the boolean result.
    afxUnit*            matchedIdx
);

/*
    The AfxCompareSubstrings() compares a string starting at a given position and specified length with each string in an
    array of strings, with an optional case-insensitive comparison. If any match is found, return TRUE and optionally the index
    of the first matched string from the array into @matchedIdx. Otherwise, return FALSE and into @matchedIdx AFX_INVALID_INDEX.

    This function can be used in scenarios like switch/case statements where you want to check if a string matches one of several possible options.
    Return the index of the first matched other string or AFX_INVALID_INDEX. Can be used in switch/case statements.
*/

AFX afxBool             AfxCompareSubstrings
(
    // The source string.
    afxString const*    s,

    // The position in the string from which comparison should start.
    afxUnit             from,

    // The length in the string up to where comparison should end.
    afxUnit             len,

    // Case-insensitive flag.
    afxBool             ci,

    // The number of strings in the @others array.
    afxUnit             cnt,

    // The array of strings to compare against.
    afxString const     others[],

    // A pointer to store the index of the matched string.
    // Can be NIL if you only want the boolean result.
    afxUnit*            matchedIdx
);

/*
    The AfxFindChar() function looks for the position for the first or last occurence of a char, in else case, 
    it should be AFX_INVALID_INDEX. When reverse is TRUE, it finds the last occurrence of the character starting from from.
*/

AFX afxBool             AfxFindChar
// Returns TRUE if a character is found.
(
    // The source string.
    afxString const*    s, 
    // The position to start searching from.
    afxUnit             from, 
    // The character to be searched.
    afxInt              ch, 
    // Flag indicating to use case-insensite comparison.
    afxBool             ci, 
    // Flag indicating to go reversely (backward to start).
    afxBool             reversely, 
    // A pointer to hold the position of a found char.
    afxUnit*            posn
);

/*
    The AfxFindSubstring() looks for a single substring within a given string starting from offset, with case-insensitive option.
    Returns an excerpt view (not a copy), a valid afxString slice if found, otherwise an empty one.
    ci flag allows case-insensitive comparison.
    No memory allocation.
    .cap is 0 because it’s just a view.
*/

AFX afxString           AfxFindSubstring
(
    // The source string.
    afxString const*    s, 
    // The position to start looking from.
    afxUnit             from,
    // Flag indicating case-insensitive comparison.
    afxBool             ci,
    // The substring to look for.
    afxString const*    substring
);

/*
    The AfxFindSubstrings() function searches a string for the first matching substring from a list.
    Returns an afxString wrapping the first matching substring.
    Supports case-insensitive matching via ci.
    matchedIdx gives the index of the matching substring in substrings[], or AFX_INVALID_INDEX if none match.
    Does not allocate memory. Wraps the pointer directly.
*/

AFX afxString           AfxFindSubstrings
(
    afxString const*    s, 
    afxUnit             from, 
    afxBool             ci, 
    afxUnit             cnt, 
    afxString const     substrings[], 
    afxUnit*            matchedIdx
);

AFX afxUnit             AfxFormatString
(
    afxString*          s, 
    afxChar const*      fmt, 
                        ...
);

AFX afxUnit             AfxFormatStringArg
(
    afxString*          s, 
    afxChar const*      fmt, 
    va_list             args
);

AFX afxResult           AfxScanString
(
    afxString const*    s, 
    afxChar const*      fmt, 
                        ...
);

AFX afxResult           AfxScanStringArg
(
    afxString const*    s, 
    afxChar const*      fmt, 
    va_list             args
);

AFX afxResult           AfxDumpString
(
    afxString const*    s, 
    afxUnit             base, 
    afxUnit             range, 
    void*               dst
);

/*
    The AfxDumpStringAsUtf16() function provides a platform-independent way to export 
    Qwadro's native (UTF-8) strings to the wide-character (UTF-16) format. It handles 
    1- to 4-byte UTF-8 sequences and converts them to a UTF-16 wide character representation.

    Returns the length of the exported string as wide-character (UTF-16) string.
*/

AFX afxUnit             AfxDumpStringAsUtf16
(
    afxString const*    s, 
    afxUnit             bufCap, 
    wchar_t*            wideBuf
);

/*
    The AfxLoadStringAsUtf16() function provides a platform-independent way to convert 
    wide-character (UTF-16) strings to UTF-8 (multibyte) strings. It can handle all Unicode 
    characters (up to 0x10FFFF) and properly encodes them into UTF-8.

    Returns the length of the imported string as Qwadro native (UTF-8) string.
*/

AFX afxUnit             AfxLoadStringAsUtf16
(
    afxString*          s, 
    wchar_t const*      wideStr, 
    afxUnit             wideStrLen
);

// TODO GetAs(s, szScheme, pvData) -> GetAs(s, "%x", data)
// TODO GetAsMeasured(s, szScheme, nLen, pvData) -> GetAs(s, "%.*s", data)
AFX afxError            AfxGetStringAsHex(afxString const* s, afxUnit32 *value);
AFX afxError            AfxGetStringAsReal(afxString const* s, afxReal *value);

AFX afxError            AfxCloneString(afxString* s, afxString const* in); // wraps the same content mapped or buffered by other string as writeable data.
AFX afxError            AfxCloneSubstring(afxString* s, afxString const* in, afxUnit base, afxUnit len); // wraps the same content mapped or buffered by other string as writeable data.

AFX void                AfxDeallocateString(afxString* s);
AFX afxError            AfxAllocateString(afxString* s, afxUnit cap, void const *start, afxUnit len);
AFX afxError            AfxReallocateString(afxString* s, afxUnit cap, void const *start, afxUnit len);

AFX afxUnit             _AfxWriteString(afxString* s, afxUnit base, void const *src, afxUnit len); // copy into

AFX afxError            AfxReadString(afxString* s, afxStream in, afxUnit len);

////////////////////////////////////////////////////////////////////////////////

/*
    The AfxResetStrings() function resets an array of afxStrings.
    Resets each string in the array by nulling the start pointer and setting len and cap to 0.
    Does not deallocate or free memory; it simply clears the metadata, so the strings appear "empty".
*/

AFX void                AfxResetStrings
(
    // The number of strings to reset.
    afxUnit             cnt, 
    // An array of strings to reset.
    afxString           strings[]
);

/*
    The AfxClearStrings() function clears string contents but preserves buffers.
    Similar to AfxResetStrings, but instead of nulling everything, it preserves the buffer and only clears the length, 
    making the strings logically empty.

    Keeps the buffer (start) and cap untouched.
    Sets .len = 0 so the string is empty from the framework's perspective.
    Optionally null-terminates at index 0 for safety with legacy functions or interop with C APIs.
*/

AFX void                AfxClearStrings
(
    afxUnit             cnt, 
    afxString           strings[]
);


#endif//AFX_STRING_H
