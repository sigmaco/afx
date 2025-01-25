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

#define _CRT_SECURE_NO_WARNINGS 1
#define STB_SPRINTF_IMPLEMENTATION 1
#include "../../dep/stb_sprintf.h"

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>

#include "../impl/afxExecImplKit.h"

#if !0

#define do_div(_n_,_base_) _do_div(&(_n_),_base_)

uint32_t _do_div(uint64_t *n, uint32_t base)
{
    uint32_t remainder = *n % base;
    *n = *n / base;
    return remainder;
}

#endif

#if !0

/**
 * simple_strtoul - convert a string to an unsigned long
 * @cp: The start of the string
 * @endp: A pointer to the end of the parsed string will be placed here
 * @base: The number base to use
 */
unsigned long simple_strtoul(const char *cp, char **endp, unsigned int base)
{
    unsigned long result = 0, value;

    if (!base) {
        base = 10;
        if (*cp == '0') {
            base = 8;
            cp++;
            if ((*cp == 'x') && isxdigit(cp[1])) {
                cp++;
                base = 16;
            }
        }
    }
    while (isxdigit(*cp) &&
        (value = isdigit(*cp) ? *cp - '0' : toupper(*cp) - 'A' + 10) < base) {
        result = result * base + value;
        cp++;
    }
    if (endp)
        *endp = (char *)cp;
    return result;
}

/**
 * simple_strtol - convert a string to a signed long
 * @cp: The start of the string
 * @endp: A pointer to the end of the parsed string will be placed here
 * @base: The number base to use
 */
long simple_strtol(const char *cp, char **endp, unsigned int base)
{
    if (*cp == '-')
        return -(long)simple_strtoul(cp + 1, endp, base);
    return simple_strtoul(cp, endp, base);
}

/**
 * simple_strtoull - convert a string to an unsigned long long
 * @cp: The start of the string
 * @endp: A pointer to the end of the parsed string will be placed here
 * @base: The number base to use
 */
unsigned long long simple_strtoull(const char *cp, char **endp, unsigned int base)
{
    unsigned long long result = 0, value;

    if (!base) {
        base = 10;
        if (*cp == '0') {
            base = 8;
            cp++;
            if ((*cp == 'x') && isxdigit(cp[1])) {
                cp++;
                base = 16;
            }
        }
    }
    while (isxdigit(*cp) && (value = isdigit(*cp) ? *cp - '0' : (islower(*cp)
        ? toupper(*cp) : *cp) - 'A' + 10) < base) {
        result = result * base + value;
        cp++;
    }
    if (endp)
        *endp = (char *)cp;
    return result;
}

/**
 * simple_strtoll - convert a string to a signed long long
 * @cp: The start of the string
 * @endp: A pointer to the end of the parsed string will be placed here
 * @base: The number base to use
 */
long long simple_strtoll(const char *cp, char **endp, unsigned int base)
{
    if (*cp == '-')
        return -(long long)simple_strtoull(cp + 1, endp, base);
    return simple_strtoull(cp, endp, base);
}

static int skip_atoi(const char **s)
{
    int i = 0;

    while (isdigit(**s))
        i = i * 10 + *((*s)++) - '0';
    return i;
}

#define ZEROPAD	1		/* pad with zero */
#define SIGN	2		/* unsigned/signed long */
#define PLUS	4		/* show plus */
#define SPACE	8		/* space if plus */
#define LEFT	16		/* left justified */
#define SPECIAL	32		/* 0x */
#define LARGE	64		/* use 'ABCDEF' instead of 'abcdef' */
//#define INT_MAX 65000

static char * number(char * buf, char * end, long long num, int base, int size, int precision, int type)
{
    char c, sign, tmp[66];
    const char *digits;
    const char small_digits[] = "0123456789abcdefghijklmnopqrstuvwxyz";
    const char large_digits[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    int i;

    digits = (type & LARGE) ? large_digits : small_digits;
    if (type & LEFT)
        type &= ~ZEROPAD;
    if (base < 2 || base > 36)
        return 0;
    c = (type & ZEROPAD) ? '0' : ' ';
    sign = 0;
    if (type & SIGN) {
        if (num < 0) {
            sign = '-';
            num = -num;
            size--;
        }
        else if (type & PLUS) {
            sign = '+';
            size--;
        }
        else if (type & SPACE) {
            sign = ' ';
            size--;
        }
    }
    if (type & SPECIAL) {
        if (base == 16)
            size -= 2;
        else if (base == 8)
            size--;
    }
    i = 0;
    if (num == 0)
        tmp[i++] = '0';
    else while (num != 0)
        tmp[i++] = digits[do_div((num), base)];
    if (i > precision)
        precision = i;
    size -= precision;
    if (!(type&(ZEROPAD + LEFT))) {
        while (size-- > 0) {
            if (buf <= end)
                *buf = ' ';
            ++buf;
        }
    }
    if (sign) {
        if (buf <= end)
            *buf = sign;
        ++buf;
    }
    if (type & SPECIAL) {
        if (base == 8) {
            if (buf <= end)
                *buf = '0';
            ++buf;
        }
        else if (base == 16) {
            if (buf <= end)
                *buf = '0';
            ++buf;
            if (buf <= end)
                *buf = digits[33];
            ++buf;
        }
    }
    if (!(type & LEFT)) {
        while (size-- > 0) {
            if (buf <= end)
                *buf = c;
            ++buf;
        }
    }
    while (i < precision--) {
        if (buf <= end)
            *buf = '0';
        ++buf;
    }
    while (i-- > 0) {
        if (buf <= end)
            *buf = tmp[i];
        ++buf;
    }
    while (size-- > 0) {
        if (buf <= end)
            *buf = ' ';
        ++buf;
    }
    return buf;
}

/**
* vsnprintf - Format a string and place it in a buffer
* @buf: The buffer to place the result into
* @size: The size of the buffer, including the trailing null space
* @fmt: The format string to use
* @args: Arguments for the format string
*
* Call this function if you are already dealing with a va_list.
* You probably want snprintf instead.
 */

//int vsnprintf(char *buf, size_t size, const char *fmt, va_list args)
_AFXINL afxResult AfxVsnprintf(afxChar* buf, size_t size, afxChar const* fmt, va_list args)
{
    return stbsp_vsnprintf(buf, size, fmt, args);

    int len;
    unsigned long long num;
    int i, base;
    char *str, *end, c;
    const char *s;

    int flags;		/* flags to number() */

    int field_width;	/* width of output field */
    int precision;		/* min. # of digits for integers; max
                   number of chars for from string */
    int qualifier;		/* 'h', 'l', or 'L' for integer fields */
                /* 'z' support added 23/7/1999 S.H.    */
                /* 'z' changed to 'Z' --davidm 1/25/99 */

    str = buf;
    end = buf + size - 1;

    if (end < buf - 1) {
        end = ((void *)-1);
        size = end - buf + 1;
    }

    for (; *fmt; ++fmt) {
        if (*fmt != '%') {
            if (str <= end)
                *str = *fmt;
            ++str;
            continue;
        }

        /* process flags */
        flags = 0;
    repeat:
        ++fmt;		/* this also skips first '%' */
        switch (*fmt) {
        case '-': flags |= LEFT; goto repeat;
        case '+': flags |= PLUS; goto repeat;
        case ' ': flags |= SPACE; goto repeat;
        case '#': flags |= SPECIAL; goto repeat;
        case '0': flags |= ZEROPAD; goto repeat;
        }

        /* get field width */
        field_width = -1;
        if (isdigit(*fmt))
            field_width = skip_atoi(&fmt);
        else if (*fmt == '*') {
            ++fmt;
            /* it's the next argument */
            field_width = va_arg(args, int);
            if (field_width < 0) {
                field_width = -field_width;
                flags |= LEFT;
            }
        }

        /* get the precision */
        precision = -1;
        if (*fmt == '.') {
            ++fmt;
            if (isdigit(*fmt))
                precision = skip_atoi(&fmt);
            else if (*fmt == '*') {
                ++fmt;
                /* it's the next argument */
                precision = va_arg(args, int);
            }
            if (precision < 0)
                precision = 0;
        }

        /* get the conversion qualifier */
        qualifier = -1;
        if (*fmt == 'h' || *fmt == 'l' || *fmt == 'L' || *fmt == 'Z') {
            qualifier = *fmt;
            ++fmt;
            if (qualifier == 'l' && *fmt == 'l') {
                qualifier = 'L';
                ++fmt;
            }
        }

        /* default base */
        base = 10;

        switch (*fmt) {
        case 'c':
            if (!(flags & LEFT)) {
                while (--field_width > 0) {
                    if (str <= end)
                        *str = ' ';
                    ++str;
                }
            }
            c = (unsigned char)va_arg(args, int);
            if (str <= end)
                *str = c;
            ++str;
            while (--field_width > 0) {
                if (str <= end)
                    *str = ' ';
                ++str;
            }
            continue;

        case 's':
            s = va_arg(args, char *);
            if (!s)
                s = "<NULL>";

            len = strnlen(s, precision);

            if (!(flags & LEFT)) {
                while (len < field_width--) {
                    if (str <= end)
                        *str = ' ';
                    ++str;
                }
            }
            for (i = 0; i < len; ++i) {
                if (str <= end)
                    *str = *s;
                ++str; ++s;
            }
            while (len < field_width--) {
                if (str <= end)
                    *str = ' ';
                ++str;
            }
            continue;

        case 'p':
            if (field_width == -1) {
                field_width = 2 * sizeof(void *);
                flags |= ZEROPAD;
            }
            str = number(str, end,
                (unsigned long)va_arg(args, void *),
                16, field_width, precision, flags);
            continue;


        case 'n':
            /* FIXME:
            * What does C99 say about the overflow case here? */
            if (qualifier == 'l') {
                long * ip = va_arg(args, long *);
                *ip = (str - buf);
            }
            else if (qualifier == 'Z') {
                size_t * ip = va_arg(args, size_t *);
                *ip = (str - buf);
            }
            else {
                int * ip = va_arg(args, int *);
                *ip = (str - buf);
            }
            continue;

        case '%':
            if (str <= end)
                *str = '%';
            ++str;
            continue;

            /* integer number formats - set up the flags and "break" */
        case 'o':
            base = 8;
            break;

        case 'X':
            flags |= LARGE;
        case 'x':
            base = 16;
            break;

        case 'd':
        case 'i':
            flags |= SIGN;
        case 'u':
            break;

        default:
            if (str <= end)
                *str = '%';
            ++str;
            if (*fmt) {
                if (str <= end)
                    *str = *fmt;
                ++str;
            }
            else {
                --fmt;
            }
            continue;
        }
        if (qualifier == 'L')
            num = va_arg(args, long long);
        else if (qualifier == 'l') {
            num = va_arg(args, unsigned long);
            if (flags & SIGN)
                num = (signed long)num;
        }
        else if (qualifier == 'Z') {
            num = va_arg(args, size_t);
        }
        else if (qualifier == 'h') {
            num = (unsigned short)va_arg(args, int);
            if (flags & SIGN)
                num = (signed short)num;
        }
        else {
            num = va_arg(args, unsigned int);
            if (flags & SIGN)
                num = (signed int)num;
        }
        str = number(str, end, num, base,
            field_width, precision, flags);
    }
    if (str <= end)
        *str = '\0';
    else if (size > 0)
        /* don't write out a null byte if the buf size is zero */
        *end = '\0';
    /* the trailing null byte doesn't count towards the total
    * ++str;
    */
    return str - buf;
}

/**
 * snprintf - Format a string and place it in a buffer
 * @buf: The buffer to place the result into
 * @size: The size of the buffer, including the trailing null space
 * @fmt: The format string to use
 * @...: Arguments for the format string
 */

//int snprintf(char * buf, size_t size, const char *fmt, ...)
_AFXINL afxResult AfxSnprintf(afxChar* buf, afxUnit size, afxChar const* fmt, ...)
{
    va_list va;
    va_start(va, fmt);

    afxResult result = stbsp_vsnprintf(buf, size, fmt, va);
    va_end(va);
    return result;
}

/**
 * vsprintf - Format a string and place it in a buffer
 * @buf: The buffer to place the result into
 * @fmt: The format string to use
 * @args: Arguments for the format string
 *
 * Call this function if you are already dealing with a va_list.
 * You probably want sprintf instead.
 */

//int vsprintf(char *buf, const char *fmt, va_list args)
_AFXINL afxResult AfxVsprintf(afxChar* buf, afxChar const* fmt, va_list args)
{
    return stbsp_vsprintf(buf, fmt, args);
}

/**
 * sprintf - Format a string and place it in a buffer
 * @buf: The buffer to place the result into
 * @fmt: The format string to use
 * @...: Arguments for the format string
 */

//int sprintf(char * buf, const char *fmt, ...)
_AFXINL afxResult AfxSprintf(afxChar* buf, afxChar const* fmt, ...)
{
    va_list args;
    int i;

    va_start(args, fmt);
    i = AfxVsprintf(buf, fmt, args);
    va_end(args);
    return i;
}

/**
 * vsscanf - Unformat a buffer into a list of arguments
 * @buf:	input buffer
 * @fmt:	format of buffer
 * @args:	arguments
 */

//int vsscanf(const char * buf, const char * fmt, va_list args)
_AFXINL afxResult AfxVsscanf(afxChar const* buf, afxChar const* fmt, va_list args)
{
    return vsscanf(buf, fmt, args);

    const char *str = buf;
    char *next;
    int num = 0;
    int qualifier;
    int base;
    int field_width = -1;
    int is_sign = 0;

    while (*fmt && *str) {
        /* skip any white space in format */
        /* white space in format matchs any amount of
         * white space, including none, in the input.
         */
        if (isspace(*fmt)) {
            while (isspace(*fmt))
                ++fmt;
            while (isspace(*str))
                ++str;
        }

        /* anything that is not a conversion must match exactly */
        if (*fmt != '%' && *fmt) {
            if (*fmt++ != *str++)
                break;
            continue;
        }

        if (!*fmt)
            break;
        ++fmt;

        /* skip this conversion.
         * advance both strings to next white space
         */
        if (*fmt == '*') {
            while (!isspace(*fmt) && *fmt)
                fmt++;
            while (!isspace(*str) && *str)
                str++;
            continue;
        }

        /* get field width */
        if (isdigit(*fmt))
            field_width = skip_atoi(&fmt);

        /* get conversion qualifier */
        qualifier = -1;
        if (*fmt == 'h' || *fmt == 'l' || *fmt == 'L' || *fmt == 'Z') {
            qualifier = *fmt;
            fmt++;
        }
        base = 10;
        is_sign = 0;

        if (!*fmt || !*str)
            break;

        switch (*fmt++) {
        case 'c':
        {
            char *s = (char *)va_arg(args, char*);
            if (field_width == -1)
                field_width = 1;
            do {
                *s++ = *str++;
            } while (field_width-- > 0 && *str);
            num++;
        }
        continue;
        case 's':
        {
            char *s = (char *)va_arg(args, char *);
            if (field_width == -1)
                field_width = INT_MAX;
            /* first, skip leading white space in buffer */
            while (isspace(*str))
                str++;

            /* now copy until next white space */
            while (*str && !isspace(*str) && field_width--) {
                *s++ = *str++;
            }
            *s = '\0';
            num++;
        }
        continue;
        case 'n':
            /* return number of characters read so far */
        {
            int *i = (int *)va_arg(args, int*);
            *i = str - buf;
        }
        continue;
        case 'o':
            base = 8;
            break;
        case 'x':
        case 'X':
            base = 16;
            break;
        case 'd':
        case 'i':
            is_sign = 1;
        case 'u':
            break;
        case '%':
            /* looking for '%' in str */
            if (*str++ != '%')
                return num;
            continue;
        default:
            /* invalid format; stop here */
            return num;
        }

        /* have some sort of integer conversion.
         * first, skip white space in buffer.
         */
        while (isspace(*str))
            str++;

        if (!*str || !isdigit(*str))
            break;

        switch (qualifier) {
        case 'h':
            if (is_sign) {
                short *s = (short *)va_arg(args, short *);
                *s = (short)simple_strtol(str, &next, base);
            }
            else {
                unsigned short *s = (unsigned short *)va_arg(args, unsigned short *);
                *s = (unsigned short)simple_strtoul(str, &next, base);
            }
            break;
        case 'l':
            if (is_sign) {
                long *l = (long *)va_arg(args, long *);
                *l = simple_strtol(str, &next, base);
            }
            else {
                unsigned long *l = (unsigned long*)va_arg(args, unsigned long*);
                *l = simple_strtoul(str, &next, base);
            }
            break;
        case 'L':
            if (is_sign) {
                long long *l = (long long*)va_arg(args, long long *);
                *l = simple_strtoll(str, &next, base);
            }
            else {
                unsigned long long *l = (unsigned long long*) va_arg(args, unsigned long long*);
                *l = simple_strtoull(str, &next, base);
            }
            break;
        case 'Z':
        {
            size_t *s = (size_t*)va_arg(args, size_t*);
            *s = (size_t)simple_strtoul(str, &next, base);
        }
        break;
        default:
            if (is_sign) {
                int *i = (int *)va_arg(args, int*);
                *i = (int)simple_strtol(str, &next, base);
            }
            else {
                unsigned int *i = (unsigned int*)va_arg(args, unsigned int*);
                *i = (unsigned int)simple_strtoul(str, &next, base);
            }
            break;
        }
        num++;

        if (!next)
            break;
        str = next;
    }
    return num;
}

/**
 * sscanf - Unformat a buffer into a list of arguments
 * @buf:	input buffer
 * @fmt:	formatting of buffer
 * @...:	resulting arguments
 */

//int sscanf(const char * buf, const char * fmt, ...)
_AFXINL afxResult AfxSscanf(afxChar const* buf, afxChar const* fmt, ...)
{
    va_list args;
    int i;

    va_start(args, fmt);
    i = AfxVsscanf(buf, fmt, args);
    va_end(args);
    return i;
}

#endif//0

#if 0 // RAW ANSI STRING FUNCTIONS

AFXINL afxResult AfxStrcmp(afxChar const* str, afxChar const* str2)
{
    AfxAssert2(str, str2);

    while (*(str)++ == *(str2)++)
        if (*(str - 1) == 0)
            return 0;

    return (*(str - 1) - *(str2 - 1));
}

AFXINL void AfxStrcpy(afxChar* str, afxChar const* from)
{
    AfxAssert2(str, from);
    while ((*(str)++ = *(from)++));
}

AFXINL afxResult AfxStrlen(afxChar const* str)
{
    AFX_ASSERT(str);
    afxChar const* x = str;
    while (*(x)++);
    return (x - str - 1);
}

AFXINL void AfxStrcat(afxChar* str, afxChar const* str2)
{
    AfxAssert2(str, str2);
    AfxStrcpy(str + AfxStrlen(str), str2);
}

#define TOUPPER(CH) (((CH) >= 'a' && (CH) <= 'z') ? ((CH) - 'a' + 'A') : (CH))

AFXINL afxResult AfxStricmp(afxChar const* str, afxChar const* str2)
{
    AfxAssert2(str, str2);

    while (*(str2) != '\0' && TOUPPER(*(str)) == TOUPPER(*(str2)))
        ++(str), ++(str2);

    return (TOUPPER(*(str)) - TOUPPER(*(str2)));
}

AFXINL afxChar* AfxConvertRawStringCpToUtf8(afxChar* str, afxInt cp)
{
    afxInt n;

    if (cp < 0x80) n = 1;
    else if (cp < 0x800) n = 2;
    else if (cp < 0x10000) n = 3;
    else if (cp < 0x200000) n = 4;
    else if (cp < 0x4000000) n = 5;
    else if (cp <= 0x7fffffff) n = 6;
    else n = 0;

    str[n] = '\0';

    switch (n)
    {
    case 6: str[5] = 0x80 | (cp & 0x3f); cp = cp >> 6; cp |= 0x4000000;
    case 5: str[4] = 0x80 | (cp & 0x3f); cp = cp >> 6; cp |= 0x200000;
    case 4: str[3] = 0x80 | (cp & 0x3f); cp = cp >> 6; cp |= 0x10000;
    case 3: str[2] = 0x80 | (cp & 0x3f); cp = cp >> 6; cp |= 0x800;
    case 2: str[1] = 0x80 | (cp & 0x3f); cp = cp >> 6; cp |= 0xc0;
    case 1: str[0] = (afxChar)cp;
    }
    return str;
}

AFXINL void AfxStrncpy(afxChar const* str, afxUnit len, afxChar* to)
{
    AfxAssert3(to, str, len);
    afxUnit i;

    for (i = 0; i < len && str[i] != '\0'; ++(i))
        to[i] = str[i];

    for (; i < len; ++(i))
        to[i] = '\0';
}

AFXINL afxResult AfxStrnicmp(afxChar const* str, afxChar const* str2, afxUnit len)
{
    afxInt f, l;

    do
    {
        if (((f = (afxByte)(*(str++))) >= 'A') && (f <= 'Z')) f -= 'A' - 'a';
        if (((l = (afxByte)(*(str2++))) >= 'A') && (l <= 'Z')) l -= 'A' - 'a';
    } while (--len && f && (f == l));

    return (f - l);
}

AFXINL afxChar* AfxStrchr(afxChar const* str, afxInt ch)
{
    while (*(str) && *(str) != (afxChar)ch)
        ++(str);

    return ((*(str) == (afxChar)ch) ? (afxChar*)str : NIL);
}

AFXINL afxChar* AfxStrrchr(afxChar const* str, afxInt ch)
{
    afxChar* start = (afxChar*)str;
    while (*(str)++);
    while (--(str) != start && *(str) != (afxChar)ch);
    return ((*(str) == (afxChar)ch) ? (afxChar*)str : NIL);
}

AFXINL afxChar* AfxStrstr(afxChar const* str, afxChar const* str2)
{
    afxChar* cp = (afxChar*)str;

    if (!(*(str2)))
        return (afxChar*)str;

    while (*(cp))
    {
        afxChar* s1 = cp, *s2 = (afxChar*)str2;

        while (*(s1) && *(s2) && !(*(s1)-*(s2)))
            ++(s1), ++(s2);

        if (!(*(s2)))
            return (cp);

        ++(cp);
    }
    return NIL;
}

#else // RAW UNICODE STRING FUNCTIONS

#define MAXLN (15 * 1024) // Excedia o limite da stack de função
#define ISSPACE " \t\n\r\f\v"

static char *
_getbase(char *p, int *basep)
{
    if (p[0] == '0') {
        switch (p[1]) {
        case 'x':
            *basep = 16;
            break;
        case 't': case 'n':
            *basep = 10;
            break;
        case 'o':
            *basep = 8;
            break;
        default:
            *basep = 10;
            return (p);
        }
        return (p + 2);
    }
    *basep = 10;
    return (p);
}

static int
#ifdef HAVE_QUAD
_atob(u_quad_t *vp, char *p, int base)
{
    u_quad_t value, v1, v2;
#else
_atob(unsigned long *vp, char *p, int base)
{
    unsigned long value, v1, v2;
#endif
    char *q, tmp[20];
    int digit;

    if (p[0] == '0' && (p[1] == 'x' || p[1] == 'X')) {
        base = 16;
        p += 2;
    }

    if (base == 16 && (q = strchr(p, '.')) != 0) {
        if ((afxUnit)(q - p) > sizeof(tmp) - 1)
            return (0);

        strncpy(tmp, p, q - p);
        tmp[q - p] = '\0';
        if (!_atob(&v1, tmp, 16))
            return (0);

        q++;
        if (strchr(q, '.'))
            return (0);

        if (!_atob(&v2, q, 16))
            return (0);
        *vp = (v1 << 16) + v2;
        return (1);
    }

    value = *vp = 0;
    for (; *p; p++) {
        if (*p >= '0' && *p <= '9')
            digit = *p - '0';
        else if (*p >= 'a' && *p <= 'f')
            digit = *p - 'a' + 10;
        else if (*p >= 'A' && *p <= 'F')
            digit = *p - 'A' + 10;
        else
            return (0);

        if (digit >= base)
            return (0);
        value *= base;
        value += digit;
    }
    *vp = value;
    return (1);
}

int
atob(uint32_t *vp, char *p, int base)
{
#ifdef HAVE_QUAD
    u_quad_t v;
#else
    unsigned long  v;
#endif

    if (base == 0)
        p = _getbase(p, &base);
    if (_atob(&v, p, base)) {
        *vp = v;
        return (1);
    }
    return (0);
}

// mirror dot fsf dot org
#if 0
int ___vsscanf(const char *buf, const char *s, va_list ap)
{
    afxSize    noassign, width, lflag, base, count;
    const char     *tc;
    char           *t, tmp[MAXLN];

    count = noassign = width = lflag = 0;
    while (*s && *buf) {
        while (isspace(*s))
            s++;
        if (*s == '%') {
            s++;
            for (; *s; s++) {
                if (strchr("zdscrouxcsefg%", *s))
                    break;
                if (*s == '*')
                    noassign = 1;
                else if (*s == 'l' || *s == 'L')
                    lflag = 1;
                else if (*s >= '1' && *s <= '9') {
                    for (tc = s; isdigit(*s); s++);
                    strncpy(tmp, tc, s - tc);
                    tmp[s - tc] = '\0';
                    atob((uint32_t*)&width, tmp, 10);
                    s--;
                }
            }
            if (*s == 'z') // QWADRO String
            {
                while (isspace(*buf))
                    buf++;

                if (!width)
                    width = strcspn(buf, ISSPACE);

                if (!noassign)
                {
                    afxString* str = va_arg(ap, afxString*);
                    AfxCopyStringLiteral(str, width, buf);
                    AfxGetStringData(str)[width] = '\0';
                    //strncpy(t = va_arg(ap, char *), buf, width);
                    //t[width] = '\0';
                }

                buf += width;
            }
            else if (*s == 's') {
                while (isspace(*buf))
                    buf++;
                if (!width)
                    width = strcspn(buf, ISSPACE);
                if (!noassign) {
                    strncpy(t = va_arg(ap, char *), buf, width);
                    t[width] = '\0';
                }
                buf += width;
            }
            else if (*s == 'c') {
                if (!width)
                    width = 1;
                if (!noassign) {
                    strncpy(t = va_arg(ap, char *), buf, width);
                    t[width] = '\0';
                }
                buf += width;
            }
            else if (strchr("dobxu", *s)) {
                while (isspace(*buf))
                    buf++;
                if (*s == 'd' || *s == 'u')
                    base = 10;
                else if (*s == 'x')
                    base = 16;
                else if (*s == 'o')
                    base = 8;
                else if (*s == 'b')
                    base = 2;
                else
                    base = 0;

                if (!width) {
                    if (isspace(*(s + 1)) || *(s + 1) == 0)
                        width = strcspn(buf, ISSPACE);
                    else
                        width = strchr(buf, *(s + 1)) - buf;
                }
                strncpy(tmp, buf, width);
                tmp[width] = '\0';
                buf += width;
                if (!noassign)
                    atob(va_arg(ap, uint32_t *), tmp, (int)base);
            }
            if (!noassign)
                count++;
            width = noassign = lflag = 0;
            s++;
        }
        else {
            while (isspace(*buf))
                buf++;
            if (*s != *buf)
                break;
            else
                s++, buf++;
        }
    }
    return ((int)count);
}

afxInt AfxSscanf(afxChar const* str, afxChar const* fmt, ...)
{
    afxInt count;
    va_list ap;
    va_start(ap, fmt);
    count = ___vsscanf(str, fmt, ap);
    va_end(ap);
    return (count);
}

afxInt AfxSprintf(afxChar* str, afxChar const* fmt, ...)
{
    AFX_ASSERT(str);
    afxInt _Result;
    va_list _ArgList;
    __crt_va_start(_ArgList, fmt);
    _Result = vsprintf(str, fmt, _ArgList);
    __crt_va_end(_ArgList);
    return _Result;
}

#endif

_AFXINL void* utf8codepoint(const void* str, afxInt32* out_codepoint)
{
    const afxChar* s = (const afxChar*)str;

    if (0xf0 == (0xf8 & s[0]))
    {
        // 4 byte utf8 codepoint
        *out_codepoint = ((0x07 & s[0]) << 18) | ((0x3f & s[1]) << 12) |
            ((0x3f & s[2]) << 6) | (0x3f & s[3]);
        s += 4;
    }
    else if (0xe0 == (0xf0 & s[0]))
    {
        // 3 byte utf8 codepoint
        *out_codepoint =
            ((0x0f & s[0]) << 12) | ((0x3f & s[1]) << 6) | (0x3f & s[2]);
        s += 3;
    }
    else if (0xc0 == (0xe0 & s[0]))
    {
        // 2 byte utf8 codepoint
        *out_codepoint = ((0x1f & s[0]) << 6) | (0x3f & s[1]);
        s += 2;
    }
    else
    {
        // 1 byte utf8 codepoint otherwise
        *out_codepoint = s[0];
        s += 1;
    }
    return (void*)s;
}

_AFXINL size_t utf8codepointsize(afxInt32 ch)
{
    if (0 == ((afxInt32)0xffffff80 & ch)) return 1;
    else if (0 == ((afxInt32)0xfffff800 & ch)) return 2;
    else if (0 == ((afxInt32)0xffff0000 & ch)) return 3;
    else // if (0 == ((afxInt32)0xffe00000 & chr))
        return 4;
}

_AFXINL afxInt32 utf8lwrcodepoint(afxInt32 cp)
{
    if (((0x0041 <= cp) && (0x005a >= cp)) || ((0x00c0 <= cp) && (0x00d6 >= cp)) || ((0x00d8 <= cp) && (0x00de >= cp)) || ((0x0391 <= cp) && (0x03a1 >= cp)) || ((0x03a3 <= cp) && (0x03ab >= cp))) cp += 32;
    else if (((0x0100 <= cp) && (0x012f >= cp)) || ((0x0132 <= cp) && (0x0137 >= cp)) || ((0x014a <= cp) && (0x0177 >= cp)) || ((0x0182 <= cp) && (0x0185 >= cp)) || ((0x01a0 <= cp) && (0x01a5 >= cp)) || ((0x01de <= cp) && (0x01ef >= cp)) || ((0x01f8 <= cp) && (0x021f >= cp)) || ((0x0222 <= cp) && (0x0233 >= cp)) || ((0x0246 <= cp) && (0x024f >= cp)) || ((0x03d8 <= cp) && (0x03ef >= cp))) cp |= 0x1;
    else if (((0x0139 <= cp) && (0x0148 >= cp)) || ((0x0179 <= cp) && (0x017e >= cp)) || ((0x01af <= cp) && (0x01b0 >= cp)) || ((0x01b3 <= cp) && (0x01b6 >= cp)) || ((0x01cd <= cp) && (0x01dc >= cp)))
    {
        cp += 1;
        cp &= ~0x1;
    }
    else
    {
        switch (cp)
        {
        default: break;
        case 0x0178: cp = 0x00ff; break;
        case 0x0243: cp = 0x0180; break;
        case 0x018e: cp = 0x01dd; break;
        case 0x023d: cp = 0x019a; break;
        case 0x0220: cp = 0x019e; break;
        case 0x01b7: cp = 0x0292; break;
        case 0x01c4: cp = 0x01c6; break;
        case 0x01c7: cp = 0x01c9; break;
        case 0x01ca: cp = 0x01cc; break;
        case 0x01f1: cp = 0x01f3; break;
        case 0x01f7: cp = 0x01bf; break;
        case 0x0187: cp = 0x0188; break;
        case 0x018b: cp = 0x018c; break;
        case 0x0191: cp = 0x0192; break;
        case 0x0198: cp = 0x0199; break;
        case 0x01a7: cp = 0x01a8; break;
        case 0x01ac: cp = 0x01ad; break;
        case 0x01af: cp = 0x01b0; break;
        case 0x01b8: cp = 0x01b9; break;
        case 0x01bc: cp = 0x01bd; break;
        case 0x01f4: cp = 0x01f5; break;
        case 0x023b: cp = 0x023c; break;
        case 0x0241: cp = 0x0242; break;
        case 0x03fd: cp = 0x037b; break;
        case 0x03fe: cp = 0x037c; break;
        case 0x03ff: cp = 0x037d; break;
        case 0x037f: cp = 0x03f3; break;
        case 0x0386: cp = 0x03ac; break;
        case 0x0388: cp = 0x03ad; break;
        case 0x0389: cp = 0x03ae; break;
        case 0x038a: cp = 0x03af; break;
        case 0x038c: cp = 0x03cc; break;
        case 0x038e: cp = 0x03cd; break;
        case 0x038f: cp = 0x03ce; break;
        case 0x0370: cp = 0x0371; break;
        case 0x0372: cp = 0x0373; break;
        case 0x0376: cp = 0x0377; break;
        case 0x03f4: cp = 0x03d1; break;
        case 0x03cf: cp = 0x03d7; break;
        case 0x03f9: cp = 0x03f2; break;
        case 0x03f7: cp = 0x03f8; break;
        case 0x03fa: cp = 0x03fb; break;
        };
    }
    return cp;
}

_AFXINL afxInt32 utf8uprcodepoint(afxInt32 cp)
{
    if (((0x0061 <= cp) && (0x007a >= cp)) || ((0x00e0 <= cp) && (0x00f6 >= cp)) || ((0x00f8 <= cp) && (0x00fe >= cp)) || ((0x03b1 <= cp) && (0x03c1 >= cp)) || ((0x03c3 <= cp) && (0x03cb >= cp))) cp -= 32;
    else if (((0x0100 <= cp) && (0x012f >= cp)) || ((0x0132 <= cp) && (0x0137 >= cp)) || ((0x014a <= cp) && (0x0177 >= cp)) || ((0x0182 <= cp) && (0x0185 >= cp)) || ((0x01a0 <= cp) && (0x01a5 >= cp)) || ((0x01de <= cp) && (0x01ef >= cp)) || ((0x01f8 <= cp) && (0x021f >= cp)) || ((0x0222 <= cp) && (0x0233 >= cp)) || ((0x0246 <= cp) && (0x024f >= cp)) || ((0x03d8 <= cp) && (0x03ef >= cp))) cp &= ~0x1;
    else if (((0x0139 <= cp) && (0x0148 >= cp)) || ((0x0179 <= cp) && (0x017e >= cp)) || ((0x01af <= cp) && (0x01b0 >= cp)) || ((0x01b3 <= cp) && (0x01b6 >= cp)) || ((0x01cd <= cp) && (0x01dc >= cp)))
    {
        cp -= 1;
        cp |= 0x1;
    }
    else
    {
        switch (cp)
        {
        default: break;
        case 0x00ff: cp = 0x0178; break;
        case 0x0180: cp = 0x0243; break;
        case 0x01dd: cp = 0x018e; break;
        case 0x019a: cp = 0x023d; break;
        case 0x019e: cp = 0x0220; break;
        case 0x0292: cp = 0x01b7; break;
        case 0x01c6: cp = 0x01c4; break;
        case 0x01c9: cp = 0x01c7; break;
        case 0x01cc: cp = 0x01ca; break;
        case 0x01f3: cp = 0x01f1; break;
        case 0x01bf: cp = 0x01f7; break;
        case 0x0188: cp = 0x0187; break;
        case 0x018c: cp = 0x018b; break;
        case 0x0192: cp = 0x0191; break;
        case 0x0199: cp = 0x0198; break;
        case 0x01a8: cp = 0x01a7; break;
        case 0x01ad: cp = 0x01ac; break;
        case 0x01b0: cp = 0x01af; break;
        case 0x01b9: cp = 0x01b8; break;
        case 0x01bd: cp = 0x01bc; break;
        case 0x01f5: cp = 0x01f4; break;
        case 0x023c: cp = 0x023b; break;
        case 0x0242: cp = 0x0241; break;
        case 0x037b: cp = 0x03fd; break;
        case 0x037c: cp = 0x03fe; break;
        case 0x037d: cp = 0x03ff; break;
        case 0x03f3: cp = 0x037f; break;
        case 0x03ac: cp = 0x0386; break;
        case 0x03ad: cp = 0x0388; break;
        case 0x03ae: cp = 0x0389; break;
        case 0x03af: cp = 0x038a; break;
        case 0x03cc: cp = 0x038c; break;
        case 0x03cd: cp = 0x038e; break;
        case 0x03ce: cp = 0x038f; break;
        case 0x0371: cp = 0x0370; break;
        case 0x0373: cp = 0x0372; break;
        case 0x0377: cp = 0x0376; break;
        case 0x03d1: cp = 0x03f4; break;
        case 0x03d7: cp = 0x03cf; break;
        case 0x03f2: cp = 0x03f9; break;
        case 0x03f8: cp = 0x03f7; break;
        case 0x03fb: cp = 0x03fa; break;
        };
    }
    return cp;
}

_AFXINL afxChar* AfxStrdup(afxChar const* src)
{
    return _strdup(src);
}

_AFXINL afxResult AfxStrncmp(afxChar const* str, afxChar const* str2, afxUnit len)
{
    return strncmp(str, str2, len);
#if 0
    unsigned char c1 = '\0';
    unsigned char c2 = '\0';
    if (len >= 4)
    {
        size_t n4 = len >> 2;
        do
        {
            c1 = (unsigned char)*str++;
            c2 = (unsigned char)*str2++;
            if (c1 == '\0' || c1 != c2)
                return c1 - c2;
            c1 = (unsigned char)*str++;
            c2 = (unsigned char)*str2++;
            if (c1 == '\0' || c1 != c2)
                return c1 - c2;
            c1 = (unsigned char)*str++;
            c2 = (unsigned char)*str2++;
            if (c1 == '\0' || c1 != c2)
                return c1 - c2;
            c1 = (unsigned char)*str++;
            c2 = (unsigned char)*str2++;
            if (c1 == '\0' || c1 != c2)
                return c1 - c2;
        } while (--n4 > 0);
        len &= 3;
    }
    while (len > 0)
    {
        c1 = (unsigned char)*str++;
        c2 = (unsigned char)*str2++;
        if (c1 == '\0' || c1 != c2)
            return c1 - c2;
        len--;
    }
    return c1 - c2;
#endif
}

_AFXINL afxResult AfxStrcmp(afxChar const* str, afxChar const* str2)
{
    return strcmp(str, str2);
#if 0
    const afxByte* s1 = (const afxByte*)str;
    const afxByte* s2 = (const afxByte*)str2;

    while (('\0' != *s1) || ('\0' != *s2))
    {
        if (*s1 < *s2)
            return -1;
        else if (*s1 > * s2)
            return 1;

        s1++;
        s2++;
    }
    return 0; // both utf8 strings matched
#endif
}

_AFXINL afxInt AfxStrcoll(afxChar const* str1, afxChar const* str2)
{
    return strcoll(str1, str2);
}

_AFXINL afxChar* AfxStrcpy(afxChar* dst, afxChar const* src)
{
    return strcpy(dst, src);
#if 0
    afxChar* d = (afxChar*)str;
    const afxChar* s = (const afxChar*)from;

    while ('\0' != *s)
    {
        *d++ = *s++;
    }

    *d = '\0';
#endif
}

_AFXINL afxResult AfxCountRawStringCodepoints(afxChar const* str)
{
    const afxByte* s = (const afxByte*)str;
    afxResult length = 0;

    while ('\0' != *s)
    {
        if (0xf0 == (0xf8 & *s)) s += 4; // 4-byte utf8 code point (began with 0b11110xxx)
        else if (0xe0 == (0xf0 & *s)) s += 3; // 3-byte utf8 code point (began with 0b1110xxxx)
        else if (0xc0 == (0xe0 & *s)) s += 2; // 2-byte utf8 code point (began with 0b110xxxxx)
        else // if (0x00 == (0x80 & *s))
            s += 1; // 1-byte ascii (began with 0b0xxxxxxx)

        length++; // no matter the bytes we marched s forward by, it was only 1 utf8 codepoint
    }
    return length;
}

_AFXINL afxResult AfxCountMeasuredRawStringCodepoints(afxChar const* str, afxSize siz)
{
    const afxByte* s = (const afxByte*)str;
    afxResult length = 0;

    while ('\0' != *s && (s < (((const afxByte*)str) + siz)))
    {
        if (0xf0 == (0xf8 & *s)) s += 4; // 4-byte utf8 code point (began with 0b11110xxx)
        else if (0xe0 == (0xf0 & *s)) s += 3; // 3-byte utf8 code point (began with 0b1110xxxx)
        else if (0xc0 == (0xe0 & *s)) s += 2; // 2-byte utf8 code point (began with 0b110xxxxx)
        else // if (0x00 == (0x80 & *s))
            s += 1; // 1-byte ascii (began with 0b0xxxxxxx)

        length++; // no matter the bytes we marched s forward by, it was only 1 utf8 codepoint
    }
    return length;
}

_AFXINL afxResult AfxStrlen(afxChar const* str)
{
    return strlen(str);
#if 0
    const char *s = (const char *)str;
    size_t size = 0;
    while (size < SIZE_MAX && '\0' != s[size]) {
        size++;
    }
    return size;
#endif
}

_AFXINL afxSize AfxStrspn(afxChar const* str, afxChar const* control)
{
    return strspn(str, control);
}

_AFXINL afxSize AfxStrnspn(afxChar const* str, afxSize strLen, afxChar const* control, afxSize controlLen)
{
    afxSize n = 0;
    while (n < strLen && AfxMemchr(control, ((afxChar*)str)[n], controlLen)) ++n;
    return n;
}

_AFXINL afxSize AfxStrcspn(afxChar const* str, afxChar const* control)
{
    return strcspn(str, control);
}

_AFXINL afxSize AfxStrncspn(afxChar const* str, afxSize strLen, afxChar const* control, afxSize controlLen)
{
    afxSize n = 0;
    if (!controlLen)
        controlLen = AfxStrlen(control);
    while (n < strLen && !AfxMemchr(control, ((afxChar*)str)[n], controlLen)) ++n;
    return n;
}

_AFXINL afxChar* AfxStrcat(afxChar* dst, afxChar const* src)
{
    return strcat(dst, src);
#if 0
    char* d = (char*)dst;
    const char* s = (const char*)src;

    while ('\0' != *d) // find the null terminating byte in dst
        d++;

    while ('\0' != *s) // overwriting the null terminating byte in dst, append src byte-by-byte
        *d++ = *s++;

    *d = '\0'; // write out a new null terminating byte into dst
#endif
}

_AFXINL afxChar* AfxStrncat(afxChar* dst, afxChar const* src, afxUnit len)
{
    return strncat(dst, src, len);
#if 0
    char* d = (char*)dst;
    const char* s = (const char*)src;

    while ('\0' != *d) // find the null terminating byte in dst
        d++;

    while ('\0' != *s) // overwriting the null terminating byte in dst, append src byte-by-byte
        *d++ = *s++;

    *d = '\0'; // write out a new null terminating byte into dst
#endif
}

_AFXINL afxResult AfxStricmp(afxChar const* str, afxChar const* str2)
{
    return _stricmp(str, str2);
#if 0
    const void* h = str;

    if ('\0' == *((const char*)str2)) // if needle has no utf8 codepoints before the null terminating byte then return haystack
        return 1;

    for (;;)
    {
        const void* n = str2;
        afxInt32 h_cp, n_cp;

        // Get the next code point and track it
        const void* nextH = h = utf8codepoint(h, &h_cp);
        n = utf8codepoint(n, &n_cp);

        while ((0 != h_cp) && (0 != n_cp))
        {
            h_cp = utf8lwrcodepoint(h_cp);
            n_cp = utf8lwrcodepoint(n_cp);

            if (h_cp != n_cp) // if we find a mismatch, bail out!
                break;

            h = utf8codepoint(h, &h_cp);
            n = utf8codepoint(n, &n_cp);
        }

        if (0 == n_cp)
            return 0; // we found the whole utf8 string for needle in haystack at maybeMatch, so return it

        if (0 == h_cp)
            return -1; // no match

        // Roll back to the next code point in the haystack to test
        h = nextH;
    }
#endif
}

_AFXINL afxChar* AfxConvertRawStringCpToUtf8(afxChar* str, afxInt cp)
{
    afxInt n;

    if (cp < 0x80) n = 1;
    else if (cp < 0x800) n = 2;
    else if (cp < 0x10000) n = 3;
    else if (cp < 0x200000) n = 4;
    else if (cp < 0x4000000) n = 5;
    else if (cp <= 0x7fffffff) n = 6;
    else n = 0;

    str[n] = '\0';

    switch (n)
    {
    case 6: str[5] = 0x80 | (cp & 0x3f); cp = cp >> 6; cp |= 0x4000000;
    case 5: str[4] = 0x80 | (cp & 0x3f); cp = cp >> 6; cp |= 0x200000;
    case 4: str[3] = 0x80 | (cp & 0x3f); cp = cp >> 6; cp |= 0x10000;
    case 3: str[2] = 0x80 | (cp & 0x3f); cp = cp >> 6; cp |= 0x800;
    case 2: str[1] = 0x80 | (cp & 0x3f); cp = cp >> 6; cp |= 0xc0;
    case 1: str[0] = (afxChar)cp;
    }
    return str;
}

_AFXINL afxChar* AfxStrncpy(afxChar* dst, afxChar const* src, afxSize len)
{
    return strncpy(dst, src, len);
#if 0
    afxChar* d = (afxChar*)to;
    const afxChar* s = (const afxChar*)str;
    afxUnit index;

    for (index = 0; index < len; index++) // overwriting anything previously in dst, write byte-by-byte from src
    {
        d[index] = s[index];

        if ('\0' == s[index])
            break;
    }

    // append null terminating byte
    for (; index < len; index++)
        d[index] = 0;

    return index;
#endif
}

_AFXINL afxResult AfxStrnicmp(afxChar const* str, afxChar const* str2, afxSize len)
{
    return _strnicmp(str, str2, len);
#if 0
    afxInt32 src1_cp, src2_cp, src1_orig_cp, src2_orig_cp;

    do
    {
        const afxByte* const s1 = (const afxByte*)str;
        const afxByte* const s2 = (const afxByte*)str2;

        if (0 == len) // first check that we have enough bytes left in n to contain an entire codepoint
            return 0;

        if ((1 == len) && ((0xc0 == (0xe0 & *s1)) || (0xc0 == (0xe0 & *s2))))
        {
            const afxInt32 c1 = (0xe0 & *s1);
            const afxInt32 c2 = (0xe0 & *s2);

            if (c1 < c2) return -1;
            else if (c1 > c2) return 1;
            else return 0;
        }

        if ((2 >= len) && ((0xe0 == (0xf0 & *s1)) || (0xe0 == (0xf0 & *s2))))
        {
            const afxInt32 c1 = (0xf0 & *s1);
            const afxInt32 c2 = (0xf0 & *s2);

            if (c1 < c2) return -1;
            else if (c1 > c2) return 1;
            else return 0;
        }

        if ((3 >= len) && ((0xf0 == (0xf8 & *s1)) || (0xf0 == (0xf8 & *s2))))
        {
            const afxInt32 c1 = (0xf8 & *s1);
            const afxInt32 c2 = (0xf8 & *s2);

            if (c1 < c2) return -1;
            else if (c1 > c2) return 1;
            else return 0;
        }

        str = utf8codepoint(str, &src1_cp);
        str2 = utf8codepoint(str2, &src2_cp);
        len -= utf8codepointsize(src1_cp);

        // Take a copy of src1 & src2
        src1_orig_cp = src1_cp;
        src2_orig_cp = src2_cp;

        // Lower srcs if required
        src1_cp = utf8lwrcodepoint(src1_cp);
        src2_cp = utf8lwrcodepoint(src2_cp);

        // Check if the lowered codepoints match
        if ((0 == src1_orig_cp) && (0 == src2_orig_cp)) return 0;
        else if (src1_cp == src2_cp)
            continue;

        // If they don't match, then we return which of the original's are less
        if (src1_orig_cp < src2_orig_cp) return -1;
        else if (src1_orig_cp > src2_orig_cp)
            return 1;

    } while (0 < len);

    // both utf8 strings matched
    return 0;
#endif
}

_AFXINL afxChar* AfxStrstr(afxChar const* str, afxChar const* excerpt);

_AFXINL afxChar* AfxStrchr(afxChar const* str, afxInt ch)
{
    return strchr(str, ch);
#if 0
    afxChar c[5] = { '\0', '\0', '\0', '\0', '\0' };

    if (0 == ch)
    {
        const afxChar* s = (const afxChar*)str;

        while ('\0' != *s) // being asked to return position of null terminating byte, so just run s to the end, and return!
            s++;

        return (void*)s;
    }
    else if (0 == ((afxInt32)0xffffff80 & ch)) c[0] = (afxChar)ch; // 1-byte/7-bit ascii (0b0xxxxxxx)
    else if (0 == ((afxInt32)0xfffff800 & ch))
    {
        // 2-byte/11-bit utf8 code point (0b110xxxxx 0b10xxxxxx)
        c[0] = 0xc0 | (afxChar)(ch >> 6);
        c[1] = 0x80 | (afxChar)(ch & 0x3f);
    }
    else if (0 == ((afxInt32)0xffff0000 & ch))
    {
        // 3-byte/16-bit utf8 code point (0b1110xxxx 0b10xxxxxx 0b10xxxxxx)
        c[0] = 0xe0 | (afxChar)(ch >> 12);
        c[1] = 0x80 | (afxChar)((ch >> 6) & 0x3f);
        c[2] = 0x80 | (afxChar)(ch & 0x3f);
    }
    else // if (0 == ((afxInt32)0xffe00000 & ch))
    {
        // 4-byte/21-bit utf8 code point (0b11110xxx 0b10xxxxxx 0b10xxxxxx 0b10xxxxxx)
        c[0] = 0xf0 | (afxChar)(ch >> 18);
        c[1] = 0x80 | (afxChar)((ch >> 12) & 0x3f);
        c[2] = 0x80 | (afxChar)((ch >> 6) & 0x3f);
        c[3] = 0x80 | (afxChar)(ch & 0x3f);
    }

    // we've made c into a 2 utf8 codepoint string, one for the chr we are seeking, another for the null terminating byte. Now use utf8str to search
    return AfxStrstr(str, c);
#endif
}

_AFXINL afxChar* AfxStrrchr(afxChar const* str, afxInt ch)
{
    return strrchr(str, ch);
#if 0
    const afxChar* s = (const char*)str;
    const afxChar* match = NIL;
    afxChar c[5] = { '\0', '\0', '\0', '\0', '\0' };

    if (0 == ch)
    {
        while ('\0' != *s) // being asked to return position of null terminating byte, so just run s to the end, and return!
            s++;

        return (void*)s;
    }
    else if (0 == ((int)0xffffff80 & ch)) c[0] = (afxChar)ch; // 1-byte/7-bit ascii (0b0xxxxxxx)
    else if (0 == ((int)0xfffff800 & ch))
    {
        // 2-byte/11-bit utf8 code point (0b110xxxxx 0b10xxxxxx)
        c[0] = 0xc0 | (char)(ch >> 6);
        c[1] = 0x80 | (char)(ch & 0x3f);
    }
    else if (0 == ((int)0xffff0000 & ch))
    {
        // 3-byte/16-bit utf8 code point (0b1110xxxx 0b10xxxxxx 0b10xxxxxx)
        c[0] = 0xe0 | (char)(ch >> 12);
        c[1] = 0x80 | (char)((ch >> 6) & 0x3f);
        c[2] = 0x80 | (char)(ch & 0x3f);
    }
    else // if (0 == ((int)0xffe00000 & chr))
    {
        // 4-byte/21-bit utf8 code point (0b11110xxx 0b10xxxxxx 0b10xxxxxx 0b10xxxxxx)
        c[0] = 0xf0 | (char)(ch >> 18);
        c[1] = 0x80 | (char)((ch >> 12) & 0x3f);
        c[2] = 0x80 | (char)((ch >> 6) & 0x3f);
        c[3] = 0x80 | (char)(ch & 0x3f);
    }

    // we've created a 2 utf8 codepoint string in c that is the utf8 character asked for by chr, and a null terminating byte

    while ('\0' != *s)
    {
        size_t offset = 0;

        while (s[offset] == c[offset])
            offset++;

        if ('\0' == c[offset])
        {
            // we found a matching utf8 code point
            match = s;
            s += offset;
        }
        else
        {
            s += offset;

            if ('\0' != *s) // need to march s along to next utf8 codepoint start (the next byte that doesn't match 0b10xxxxxx)
            {
                do
                {
                    s++;
                } while (0x80 == (0xc0 & *s));
            }
        }
    }

    return (afxChar*)match; // return the last match we found (or 0 if no match was found)
#endif
}

_AFXINL afxChar* AfxStrpbrk(afxChar const* str1, afxChar const* stopset)
{
    // The strpbrk("string pointer break") function is related to strcspn, 
    // except that it returns a pointer to the first character in string that is a member of the set stopset instead of the length of the initial substring.
    // It returns a null pointer if no such character from stopset is found.
    return strpbrk(str1, stopset);
}

_AFXINL afxChar* AfxStrnstr(afxChar const* str, afxUnit strLen, afxChar const* substr, afxUnit substrLen)
{
    // C doesn't have a equivalent function

    afxChar const *p = str;
    strLen = strLen ? strLen : AfxStrlen(str);
    afxSize const len2 = substrLen ? substrLen : AfxStrlen(substr);

    if (!len2)
        return (afxChar*)str;

    afxUnit i = 0;

    for (; (strLen >= len2) && ((p = AfxStrchr(p, *substr)) != 0); p++)
    {
        if (AfxStrncmp(p, substr, len2) == 0)
            return (char *)p;
    }
    return NIL;
}

_AFXINL afxChar* AfxStrstr(afxChar const* str, afxChar const* excerpt)
{
    return strstr(str, excerpt);
#if 0
    const afxChar* h = (const afxChar*)str;
    afxInt32 throwaway_codepoint;

    // if needle has no utf8 codepoints before the null terminating
    // byte then return haystack
    if ('\0' == *((const afxChar*)excerpt)) {
        return (afxChar*)str;
    }

    while ('\0' != *h) {
        const afxChar* maybeMatch = h;
        const afxChar* n = (const afxChar*)excerpt;

        while (*h == *n && (*h != '\0' && *n != '\0'))
        {
            n++;
            h++;
        }

        if ('\0' == *n) return (afxChar*)maybeMatch; // we found the whole utf8 string for needle in haystack at maybeMatch, so return it
        else {
            // h could be in the middle of an unmatching utf8 codepoint, so we need to march it on to the next character beginning starting from the current character
            h = (const afxChar*)utf8codepoint(maybeMatch, &throwaway_codepoint);
        }
    }
    return NIL; // no match
#endif
}

_AFXINL afxBool AfxIsalpha(afxInt ch)
{
    return isalpha(ch);
}

_AFXINL afxBool AfxIsalnum(afxInt ch)
{
    return isalnum(ch);
}

_AFXINL afxBool AfxIsdigit(afxInt ch)
{
    return isdigit(ch);
}

_AFXINL afxBool AfxIsxdigit(afxInt ch)
{
    return isxdigit(ch);
}

_AFXINL afxBool AfxIscntrl(afxInt ch)
{
    return iscntrl(ch);
}

_AFXINL afxBool AfxIslower(afxInt ch)
{
    return islower(ch);
}

_AFXINL afxBool AfxIsupper(afxInt ch)
{
    return isupper(ch);
}

_AFXINL afxBool AfxIsspace(afxInt ch)
{
    return isspace(ch);
}

_AFXINL afxInt AfxTolower(afxInt ch)
{
    return tolower(ch);
}

_AFXINL afxInt AfxToupper(afxInt ch)
{
    return toupper(ch);
}

#endif//0
