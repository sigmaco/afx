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

#define _CRT_SECURE_NO_WARNINGS
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "qwadro/core/afxSystem.h"

AFX_DEFINE_STRUCT(_afxIniRecord)
{
    afxString key;
    afxString value;
};

AFX_DEFINE_STRUCT(_afxIniPage)
{
    afxString name;
    afxNat keyCnt;
    _afxIniRecord* keys;
};

_AFXINL _afxIniRecord* _AfxIniCreateRecord(afxManifest* ini, _afxIniPage* page, afxString const* key, afxString const* value)
{
    afxError err;

    if ((!(page->keyCnt % 8)) && !(page->keys = AfxReallocate(page->keys, sizeof(_afxIniRecord), (8 + page->keyCnt), NIL, AfxHere()))) AfxThrowError();
    else
    {
        _afxIniRecord* entry = &page->keys[page->keyCnt++];

        if (!AfxCatalogStrings2(ini->strb, 1, key, &entry->key)) AfxThrowError();
        else if (!AfxCatalogStrings2(ini->strb, 1, value, &entry->value))
                AfxThrowError();

        return entry;
    }
    return NIL;
}

_AFXINL _afxIniPage* _AfxIniCreatePage(afxManifest* ini, afxString const* block)
{
    afxError err;

    if ((!(ini->pageCnt % 8)) && !(ini->pages = AfxReallocate(ini->pages, sizeof(_afxIniPage), (8 + ini->pageCnt), NIL, AfxHere()))) AfxThrowError();
    else
    {
        _afxIniPage* page = &ini->pages[ini->pageCnt++];
        page->keyCnt = 0;
        //AfxMakeString32(&page->name, block);

        if (!AfxCatalogStrings2(ini->strb, 1, block, &page->name)) AfxThrowError();
        else
        {
            if (!(page->keys = AfxAllocate(8, sizeof(_afxIniRecord), 0, AfxHere())))
                AfxThrowError();
        }
        return page;
    }
    return NIL;
}

_AFXINL afxNat AfxCountManifestPages(afxManifest const* ini)
{
    return ini->pageCnt;
}

_AFXINL afxNat AfxCountManifestRecords(afxManifest const* ini, afxNat pagIdx)
{
    return ini->pages[pagIdx].keyCnt;
}

_AFX afxBool AfxFindManifestPage(afxManifest const* ini, afxString const* name, afxNat* pagIdx)
{
    afxError err = NIL;
    AfxAssert(ini);
    AfxAssert(name);
    afxBool rslt = FALSE;
    afxString s;

    for (afxNat i = 0; i < ini->pageCnt; i++)
    {
        if (AfxResolveStrings2(ini->strb, 1, &ini->pages[i].name, &s) && (0 == AfxCompareString(&s, name)))
        {
            AfxAssert(pagIdx);
            *pagIdx = i;
            rslt = TRUE;
            break;
        }
    }
    return rslt;
}

_AFX afxBool AfxFindManifestRecord(afxManifest const* ini, afxNat pagIdx, afxString const* name, afxNat* recIdx)
{
    afxError err = NIL;
    AfxAssert(ini);
    AfxAssert(name);
    afxBool rslt = FALSE;
    afxString s;

    AfxAssertRange(ini->pageCnt, pagIdx, 1);
    
    for (afxNat i = 0; i < ini->pages[pagIdx].keyCnt; i++)
    {
        if (AfxResolveStrings2(ini->strb, 1, &ini->pages[pagIdx].keys[i].key, &s) && (0 == AfxCompareString(&s, name)))
        {
            AfxAssert(recIdx);
            *recIdx = i;
            rslt = TRUE;
            break;
        }
    }
    return rslt;
}

_AFX afxBool AfxGetManifestString(afxManifest const* ini, afxNat pagIdx, afxNat recIdx, afxString* value)
{
    afxError err = NIL;
    AfxAssert(ini);
    AfxAssertRange(ini->pageCnt, pagIdx, 1);
    AfxAssertRange(ini->pages[pagIdx].keyCnt, recIdx, 1);
    afxBool rslt = FALSE;
    afxString const* val = &ini->pages[pagIdx].keys[recIdx].value;

    if (!AfxStringIsEmpty(val))
        if (AfxResolveStrings2(ini->strb, 1, val, value))
            rslt = TRUE;

    return rslt;
}

_AFX afxBool AfxGetManifestUri(afxManifest const* ini, afxNat pagIdx, afxNat recIdx, afxUri* uri)
{
    afxError err = NIL;
    AfxAssert(ini);
    AfxAssertRange(ini->pageCnt, pagIdx, 1);
    AfxAssertRange(ini->pages[pagIdx].keyCnt, recIdx, 1);
    AfxAssert(uri);
    afxBool rslt = FALSE;
    afxString s;

    if ((rslt = AfxGetManifestString(ini, pagIdx, recIdx, &s)))
        AfxMakeUriFromString(uri, &s);
    else
        AfxResetUri(uri);

    return rslt;
}

_AFX afxBool AfxGetManifestNat(afxManifest const* ini, afxNat pagIdx, afxNat recIdx, afxNat* value)
{
    afxError err = NIL;
    AfxAssert(ini);
    AfxAssertRange(ini->pageCnt, pagIdx, 1);
    AfxAssertRange(ini->pages[pagIdx].keyCnt, recIdx, 1);
    AfxAssert(value);
    afxBool rslt = FALSE;
    afxString s;

    if ((rslt = AfxGetManifestString(ini, pagIdx, recIdx, &s)))
    {
        afxString2048 bstr;
        AfxMakeString2048(&bstr, &s);
        *value = (afxNat)atoi(bstr.str.str.start);
    }
    else *value = 0;

    return rslt;
}

_AFXINL _afxIniPage* _AfxIniFindPage(afxManifest const* ini, afxString const* name)
{
    afxString s;
    
    if (name)
        for (afxNat i = 0; i < ini->pageCnt; i++)
            if (AfxResolveStrings2(ini->strb, 1, &ini->pages[i].name, &s))
                if (0 == AfxCompareString(&s, name))
                    return &ini->pages[i];

    return NIL;
}

_AFXINL _afxIniRecord* _AfxIniFindRecord(afxManifest const* ini, _afxIniPage const* page, afxString const* key)
{
    afxString s;

    if (page && key)
        for (afxNat i = 0; i < page->keyCnt; i++)
            if (AfxResolveStrings2(ini->strb, 1, &page->keys[i].key, &s))
                if (0 == AfxCompareString(&s, key))
                    return &page->keys[i];
    
    return NIL;
}

_AFXINL _afxIniRecord* _AfxIniGetRecord(afxManifest const* ini, afxString const* page, afxString const* key)
{
    return _AfxIniFindRecord(ini, _AfxIniFindPage(ini, page), key);
}

_AFX afxError AfxSetUpIni(afxManifest* ini)
{
    afxError err = NIL;
    ini->pageCnt = 0;
    
    if (!(ini->pages = AfxAllocate(8, sizeof(_afxIniPage), 0, AfxHere())))
        AfxThrowError();

    AfxAcquireObjects(AfxGetStringBaseClass(), 1, (void*)&ini->strb, NIL);

    return err;
}

_AFX void AfxCleanUpIni(afxManifest* ini)
{
    for (afxNat i = 0; i < ini->pageCnt; i++)
    {
        _afxIniPage* page = &ini->pages[i];
        AfxDeallocate(page->keys);
    }
    AfxDeallocate(ini->pages);
    AfxReleaseObjects(1, &ini->strb);
}

_AFX afxError AfxIniLoadFromFile(afxManifest* ini, afxUri const* uri)
{
    afxError err = NIL;
    FILE* f;

    if (!(f = fopen(AfxGetUriData(uri, 0), "r"))) AfxThrowError();
    else
    {
        enum { Section, Key, Value, Comment } state = Section;
        int c;
        afxNat posn = 0;
        afxNat spaces = 0;
        afxNat line = 0;
        afxNat bufSiz = 128 * sizeof(afxChar);
        afxChar* buf = malloc(bufSiz);
        afxChar* value = NIL;

        _afxIniPage* currPag = NIL;

        memset(buf, '\0', bufSiz);

        while ((c = getc(f)) != EOF)
        {
            if (posn > bufSiz - 2)
            {
                bufSiz += 128 * sizeof(char);
                size_t value_offset = value == NIL ? 0 : value - buf;
                buf = realloc(buf, bufSiz);
                memset(buf + posn, '\0', bufSiz - posn);
                if (value != NIL)
                    value = buf + value_offset;
            }

            switch (c)
            {
            case ' ':
                switch (state)
                {
                case Value: if (value[0] != '\0') spaces++; break;
                default: if (buf[0] != '\0') spaces++; break;
                }
                break;
            case ';':
                if (state == Value)
                {
                    buf[posn++] = c;
                    break;
                }
                else
                {
                    state = Comment;
                    buf[posn++] = c;

                    while (c != EOF && c != '\n')
                    {
                        c = getc(f);
                        if (c != EOF && c != '\n') buf[posn++] = c;
                    }
                }
            case '\n':
            case EOF:
                line++;
                if (state == Value)
                {
                    if (!currPag)
                        currPag = _AfxIniCreatePage(ini, &AfxString(""));

                    afxString bufStr, valueStr;
                    AfxMakeString(&bufStr, buf, 0);
                    AfxMakeString(&valueStr, value, 0);

                    _AfxIniCreateRecord(ini, currPag, &bufStr, &valueStr);
                    value = NIL;
                }
                else if (state == Comment)
                {
                    if (!currPag)
                        currPag = _AfxIniCreatePage(ini, &AfxString(""));

                    afxString bufStr;
                    AfxMakeString(&bufStr, buf, 0);

                    _AfxIniCreateRecord(ini, currPag, &bufStr, &AfxString(""));
                }
                else if (state == Section)
                {
                    AfxLogError("%u: Section '%s' missing ']' operator.", line, buf);
                }
                else if (state == Key && posn)
                {
                    AfxLogError("%u: Key '%s' missing '=' operator.", line, buf);
                }
                memset(buf, '\0', bufSiz);
                state = Key;
                posn = 0;
                spaces = 0;
                break;
            case '[':
                state = Section;
                break;
            case ']':
            {
                afxString bufStr;
                AfxMakeString(&bufStr, buf, 0);

                currPag = _AfxIniCreatePage(ini, &bufStr);
                memset(buf, '\0', bufSiz);
                posn = 0;
                spaces = 0;
                state = Key;
                break;
            }
            case '=':
                if (state == Key)
                {
                    state = Value;
                    buf[posn++] = '\0';
                    value = buf + posn;
                    spaces = 0;
                    continue;
                }
            default:
                for (; spaces > 0; spaces--) buf[posn++] = ' ';
                buf[posn++] = c;
                break;
            }
        }
        free(buf);
        fclose(f);
    }
    return err;
}

_AFX afxError AfxIniSaveToFile(afxManifest const* ini, afxUri const* uri)
{
    afxError err = NIL;
    FILE* f;
    
    if (!(f = fopen(AfxGetUriData(uri, 0), "w+"))) AfxThrowError();
    else
    {
        for (afxNat i = 0; i < ini->pageCnt; i++)
        {
            _afxIniPage* pag = &ini->pages[i];
            fprintf(f, i > 0 ? "\n[%.*s]\n" : "[%.*s]\n", AfxPushString(&pag->name));

            for (afxNat q = 0; q < pag->keyCnt; q++)
            {
                _afxIniRecord* record = &pag->keys[q];

                if (record->key.buf[0] == ';')
                {
                    fprintf(f, "%.*s\n", AfxPushString(&record->key));
                }
                else
                {
                    fprintf(f, "%.*s = %.*s\n", AfxPushString(&record->key), AfxPushString(&record->value));
                }
            }
        }
        fclose(f);
    }
    return err;
}

_AFX afxBool AfxIniHasKey(afxManifest const* ini, afxString const* page, afxString const* key)
{
    return (!!_AfxIniGetRecord(ini, page, key));
}

_AFX afxError AfxIniSetString(afxManifest* ini, afxString const* page, afxString const* key, afxString* value)
{
    afxError err = NIL;
    _afxIniPage* pag;

    if ((!(pag = _AfxIniFindPage(ini, page))) && (!(pag = _AfxIniCreatePage(ini, page)))) AfxThrowError();
    else
    {
        _afxIniRecord* record;

        if ((!(record = _AfxIniFindRecord(ini, pag, key))) && (!(record = _AfxIniCreateRecord(ini, pag, key, value))))
            AfxThrowError();
        else
            AfxCatalogStrings2(ini->strb, 1, value, &record->value);
    }
    return err;
}

_AFX afxBool AfxIniGetString(afxManifest const* ini, afxString const* page, afxString const* key, afxString* value)
{
    afxBool rslt;
    _afxIniRecord* entry;

    if ((rslt = ((rslt = !!(entry = _AfxIniGetRecord(ini, page, key))) && (!(AfxStringIsEmpty(&entry->value))))))
        AfxResolveStrings2(ini->strb, 1, &entry->value, value);

    return rslt;
}

_AFXINL afxBool AfxIniGetStringIndexed(afxManifest const* ini, afxNat pagIdx, afxString const* key, afxString* value)
{
    afxBool rslt;
    _afxIniRecord* entry;

    if ((rslt = ((rslt = !!(entry = _AfxIniFindRecord(ini, &ini->pages[pagIdx], key))) && (!(AfxStringIsEmpty(&entry->value))))))
        AfxResolveStrings2(ini->strb, 1, &entry->value, value);

    return rslt;
}

_AFX afxBool AfxIniGetUri(afxManifest const* ini, afxString const* page, afxString const* key, afxUri* value)
{
    afxBool rslt;
    _afxIniRecord* entry;
    afxString val;

    //if ((rslt = ((rslt = !!(entry = _AfxIniGetRecord(ini, page, key))) && (!(AfxStringIsEmpty(&entry->value))))))
    if ((rslt = ((rslt = AfxIniGetString(ini, page, key, &val)) && (!(AfxStringIsEmpty(&val))))))
        AfxMakeUriFromString(value, &val);
    else
        AfxResetUri(value);

    return rslt;
}

_AFX afxBool AfxIniGetUriIndexed(afxManifest const* ini, afxNat pagIdx, afxString const* key, afxUri* value)
{
    afxBool rslt;
    _afxIniRecord* entry;
    afxString val;

    //if ((rslt = ((rslt = !!(entry = _AfxIniFindRecord(ini, &ini->pages[pagIdx], key))) && (!(AfxStringIsEmpty(&entry->value))))))
    if ((rslt = ((rslt = !!_AfxIniFindRecord(ini, &ini->pages[pagIdx], key)) && (!(AfxStringIsEmpty(&val))))))
        AfxMakeUriFromString(value, &val);
    else
        AfxResetUri(value);

    return rslt;
}

_AFX afxBool AfxIniGetNat(afxManifest const* ini, afxString const* page, afxString const* key, afxNat* value)
{
    afxBool rslt;
    afxString val;

    if ((rslt = ((rslt = AfxIniGetString(ini, page, key, &val)) && (!(AfxStringIsEmpty(&val))))))
    {
        afxString2048 bstr;
        AfxMakeString2048(&bstr, &val);
        *value = (afxNat)atoi(bstr.str.str.start);
    }

    return rslt;
}

_AFX afxBool AfxIniGetNatIndexed(afxManifest const* ini, afxNat pagIdx, afxString const* key, afxNat* value)
{
    afxBool rslt;
    afxString val;

    if ((rslt = ((rslt = !!_AfxIniFindRecord(ini, &ini->pages[pagIdx], key)) && (!(AfxStringIsEmpty(&val))))))
    {
        afxString2048 bstr;
        AfxMakeString2048(&bstr, &val);
        *value = (afxNat)atoi(bstr.str.str.start);
    }
    return rslt;
}

_AFX afxBool AfxIniGetInt(afxManifest const* ini, afxString const* page, afxString const* key, afxInt* value)
{
    afxBool rslt;
    afxString val;
    
    if ((rslt = ((rslt = AfxIniGetString(ini, page, key, &val)) && (!(AfxStringIsEmpty(&val))))))
        *value = atoi(val.start);

    return rslt;
}

_AFX afxBool AfxIniGetIntIndexed(afxManifest const* ini, afxNat pagIdx, afxString const* key, afxInt* value)
{
    afxBool rslt;
    afxString val;

    if ((rslt = ((rslt = !!_AfxIniFindRecord(ini, &ini->pages[pagIdx], key)) && (!(AfxStringIsEmpty(&val))))))
        *value = atoi(val.start);

    return rslt;
}

_AFX afxBool AfxIniGetReal(afxManifest const* ini, afxString const* page, afxString const* key, afxReal* value)
{
    afxBool rslt;
    afxString val;
    
    if ((rslt = ((rslt = AfxIniGetString(ini, page, key, &val)) && (!(AfxStringIsEmpty(&val))))))
        *value = (afxReal)atof(val.start);

    return rslt;
}

_AFX afxBool AfxIniGetRealIndexed(afxManifest const* ini, afxNat pagIdx, afxString const* key, afxReal* value)
{
    afxBool rslt;
    afxString val;

    if ((rslt = ((rslt = !!_AfxIniFindRecord(ini, &ini->pages[pagIdx], key)) && (!(AfxStringIsEmpty(&val))))))
        *value = (afxReal)atof(val.start);

    return rslt;
}

_AFX afxBool AfxIniGetReal64(afxManifest const* ini, afxString const* page, afxString const* key, afxReal64* value)
{
    afxBool rslt;
    afxString val;
    
    if ((rslt = ((rslt = AfxIniGetString(ini, page, key, &val)) && (!(AfxStringIsEmpty(&val))))))
        *value = (afxReal64)atof(val.start);

    return rslt;
}

_AFX afxBool AfxIniGetReal64Indexed(afxManifest const* ini, afxNat pagIdx, afxString const* key, afxReal64* value)
{
    afxBool rslt;
    afxString val;

    if ((rslt = ((rslt = !!_AfxIniFindRecord(ini, &ini->pages[pagIdx], key)) && (!(AfxStringIsEmpty(&val))))))
        *value = (afxReal64)atof(val.start);

    return rslt;
}

_AFX afxBool AfxIniGetBool(afxManifest const* ini, afxString const* page, afxString const* key, afxBool* value)
{
    afxBool rslt;
    afxString val;
    
    if ((rslt = ((rslt = AfxIniGetString(ini, page, key, &val)) && (!(AfxStringIsEmpty(&val))))) && ((0 == AfxCompareStringCil(&val, 0, "false", 0)) || (0 == AfxCompareStringCil(&val, 0, "0", 0))))
        *value = FALSE;
    else if ((rslt = ((rslt = AfxIniGetString(ini, page, key, &val)) && (!(AfxStringIsEmpty(&val))))) && ((0 == AfxCompareStringCil(&val, 0, "true", 0)) || (0 == AfxCompareStringCil(&val, 0, "1", 0))))
        *value = TRUE;

    return rslt;
}

_AFX afxBool AfxIniGetBoolIndexed(afxManifest const* ini, afxNat pagIdx, afxString const* key, afxBool* value)
{
    afxBool rslt;
    afxString val;

    if ((rslt = ((rslt = AfxIniGetStringIndexed(ini, pagIdx, key, &val)) && (!(AfxStringIsEmpty(&val))))) && ((0 == AfxCompareStringCil(&val, 0, "false", 0)) || (0 == AfxCompareStringCil(&val, 0, "0", 0))))
        *value = FALSE;
    else if ((rslt = ((rslt = AfxIniGetStringIndexed(ini, pagIdx, key, &val)) && (!(AfxStringIsEmpty(&val))))) && ((0 == AfxCompareStringCil(&val, 0, "true", 0)) || (0 == AfxCompareStringCil(&val, 0, "1", 0))))
        *value = TRUE;

    return rslt;
}
