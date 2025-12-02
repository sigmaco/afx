/*
 *           ::::::::    :::::::::::    ::::::::    ::::     ::::       :::
 *          :+:    :+:       :+:       :+:    :+:   +:+:+: :+:+:+     :+: :+:
 *          +:+              +:+       +:+          +:+ +:+:+ +:+    +:+   +:+
 *          +#++:++#++       +#+       :#:          +#+  +:+  +#+   +#++:++#++:
 *                 +#+       +#+       +#+   +#+#   +#+       +#+   +#+     +#+
 *          #+#    #+#       #+#       #+#    #+#   #+#       #+#   #+#     #+#
 *           ########    ###########    ########    ###       ###   ###     ###
 *
 *                     S I G M A   T E C H N O L O G Y   G R O U P
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
#include "../exec/afxSystemDDK.h"

AFX_DEFINE_STRUCT(_afxIniRecord)
{
    afxString key;
    afxString value;
};

AFX_DEFINE_STRUCT(_afxIniPage)
{
    afxString name;
    afxUnit keyCnt;
    _afxIniRecord* keys;
};

_AFXINL _afxIniRecord* _AfxIniCreateRecord(afxManifest* ini, _afxIniPage* page, afxString const* key, afxString const* value)
{
    afxError err;

    if ((!(page->keyCnt % 8)) && AfxReallocate(sizeof(page->keys[0]) * (8 + page->keyCnt), NIL, AfxHere(), (void**)&page->keys)) AfxThrowError();
    else
    {
        _afxIniRecord* entry = &page->keys[page->keyCnt++];
        
        if (AfxCloneString(&entry->key, key)) AfxThrowError();
        else if (AfxCloneString(&entry->value, value))
                AfxThrowError();

        return entry;
    }
    return NIL;
}

_AFXINL _afxIniPage* _AfxIniCreatePage(afxManifest* ini, afxString const* block)
{
    afxError err;

    if ((!(ini->pageCnt % 8)) && AfxReallocate(sizeof(ini->pages[0]) * (8 + ini->pageCnt), NIL, AfxHere(), (void**)&ini->pages)) AfxThrowError();
    else
    {
        _afxIniPage* page = &ini->pages[ini->pageCnt++];
        page->keyCnt = 0;
        //AfxMakeString32(&page->name, block);

        if (AfxCloneString(&page->name, block)) AfxThrowError();
        else
        {
            if (AfxAllocate(8 * sizeof(page->keys[0]), 0, AfxHere(), (void**)&page->keys))
                AfxThrowError();
        }
        return page;
    }
    return NIL;
}

_AFXINL afxUnit AfxCountInitializationSections(afxManifest const* ini)
{
    return ini->pageCnt;
}

_AFXINL afxUnit AfxCountInitializationRecords(afxManifest const* ini, afxUnit secIdx)
{
    return ini->pages[secIdx].keyCnt;
}

_AFX afxBool AfxFindInitializationSection(afxManifest const* ini, afxString const* name, afxUnit* secIdx)
{
    afxError err = { 0 };
    AFX_ASSERT(ini);
    AFX_ASSERT(name);
    afxBool rslt = FALSE;

    for (afxUnit i = 0; i < ini->pageCnt; i++)
    {
        if (AfxCompareStrings(&ini->pages[i].name, 0, TRUE, 1, name, NIL))
        {
            AFX_ASSERT(secIdx);
            *secIdx = i;
            rslt = TRUE;
            break;
        }
    }
    return rslt;
}

_AFX afxBool AfxFindInitializationRecord(afxManifest const* ini, afxUnit secIdx, afxString const* name, afxUnit* recIdx)
{
    afxError err = { 0 };
    AFX_ASSERT(ini);
    AFX_ASSERT(name);
    afxBool rslt = FALSE;
    
    AFX_ASSERT_RANGE(ini->pageCnt, secIdx, 1);
    
    for (afxUnit i = 0; i < ini->pages[secIdx].keyCnt; i++)
    {
        if (AfxCompareStrings(&ini->pages[secIdx].keys[i].key, 0, TRUE, 1, name, NIL))
        {
            AFX_ASSERT(recIdx);
            *recIdx = i;
            rslt = TRUE;
            break;
        }
    }
    return rslt;
}

_AFX afxBool AfxGetManifestString(afxManifest const* ini, afxUnit secIdx, afxUnit recIdx, afxString* value)
{
    afxError err = { 0 };
    AFX_ASSERT(ini);
    AFX_ASSERT_RANGE(ini->pageCnt, secIdx, 1);
    AFX_ASSERT_RANGE(ini->pages[secIdx].keyCnt, recIdx, 1);
    afxBool rslt = FALSE;
    afxString const* val = &ini->pages[secIdx].keys[recIdx].value;

    if (!AfxIsStringEmpty(val))
        *value = *val, rslt = TRUE;

    return rslt;
}

_AFX afxBool AfxGetManifestUri(afxManifest const* ini, afxUnit secIdx, afxUnit recIdx, afxUri* uri)
{
    afxError err = { 0 };
    AFX_ASSERT(ini);
    AFX_ASSERT_RANGE(ini->pageCnt, secIdx, 1);
    AFX_ASSERT_RANGE(ini->pages[secIdx].keyCnt, recIdx, 1);
    AFX_ASSERT(uri);
    afxBool rslt = FALSE;
    afxString s;

    if ((rslt = AfxGetManifestString(ini, secIdx, recIdx, &s)))
        AfxWrapUriString(uri, &s);
    else
        AfxResetUris(1, uri);

    return rslt;
}

_AFX afxBool AfxGetManifestNat(afxManifest const* ini, afxUnit secIdx, afxUnit recIdx, afxUnit* value)
{
    afxError err = { 0 };
    AFX_ASSERT(ini);
    AFX_ASSERT_RANGE(ini->pageCnt, secIdx, 1);
    AFX_ASSERT_RANGE(ini->pages[secIdx].keyCnt, recIdx, 1);
    AFX_ASSERT(value);
    afxBool rslt = FALSE;
    afxString s;

    if ((rslt = AfxGetManifestString(ini, secIdx, recIdx, &s)))
    {
        afxString2048 bstr;
        AfxMakeString2048(&bstr, &s);
        *value = (afxUnit)atoi(bstr.s.start);
    }
    else *value = 0;

    return rslt;
}

_AFXINL _afxIniPage* _AfxIniFindPage(afxManifest const* ini, afxString const* name)
{
    afxString s;
    
    if (name)
        for (afxUnit i = 0; i < ini->pageCnt; i++)
            if (AfxCompareStrings(&ini->pages[i].name, 0, TRUE, 1, name, NIL))
                return &ini->pages[i];

    return NIL;
}

_AFXINL _afxIniRecord* _AfxIniFindRecord(afxManifest const* ini, _afxIniPage const* page, afxString const* key)
{
    afxString s;

    if (page && key)
        for (afxUnit i = 0; i < page->keyCnt; i++)
            if (AfxCompareStrings(&page->keys[i].key, 0, TRUE, 1, key, NIL))
                return &page->keys[i];
    
    return NIL;
}

_AFXINL _afxIniRecord* _AfxIniGetRecord(afxManifest const* ini, afxString const* page, afxString const* key)
{
    return _AfxIniFindRecord(ini, _AfxIniFindPage(ini, page), key);
}

_AFX afxError AfxDeployManifest(afxManifest* ini)
{
    afxError err = { 0 };
    ini->pageCnt = 0;
    
    if (AfxAllocate(8 * sizeof(ini->pages[0]), 0, AfxHere(), (void**)&ini->pages))
        AfxThrowError();

    return err;
}

_AFX void AfxDismantleManifest(afxManifest* ini)
{
    for (afxUnit i = 0; i < ini->pageCnt; i++)
    {
        _afxIniPage* page = &ini->pages[i];

        for (afxUnit j = 0; j < page->keyCnt; j++)
        {
            _afxIniRecord* key = &page->keys[j];
            AfxDeallocateString(&key->value);
            AfxDeallocateString(&key->key);
        }

        AfxDeallocateString(&page->name);
        AfxDeallocate((void**)&page->keys, AfxHere());
    }

    if (ini->pages)
    {
        AfxDeallocate((void**)&ini->pages, AfxHere());
        ini->pages = NIL;
    }
}

_AFX afxError AfxLoadInitializationFile(afxManifest* ini, afxUri const* uri)
{
    afxError err = { 0 };
    FILE* f;

    if (!(f = fopen(AfxGetUriData(uri, 0), "r")))
    {
        err = afxError_FILE_NOT_FOUND;
        return err;
    }
    else
    {
        enum { Section, Key, Value, Comment } state = Section;
        int c;
        afxUnit posn = 0;
        afxUnit spaces = 0;
        afxUnit line = 0;
        afxUnit bufSiz = 128 * sizeof(afxChar);
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
                        currPag = _AfxIniCreatePage(ini, &AFX_STRING(""));

                    afxString bufStr, valueStr;
                    AfxMakeString(&bufStr, 0, buf, 0);
                    AfxMakeString(&valueStr, 0, value, 0);

                    _AfxIniCreateRecord(ini, currPag, &bufStr, &valueStr);
                    value = NIL;
                }
                else if (state == Comment)
                {
                    if (!currPag)
                        currPag = _AfxIniCreatePage(ini, &AFX_STRING(""));

                    afxString bufStr;
                    AfxMakeString(&bufStr, 0, buf, 0);

                    _AfxIniCreateRecord(ini, currPag, &bufStr, &AFX_STRING(""));
                }
                else if (state == Section)
                {
                    AfxReportError("%u: Section '%s' missing ']' operator.", line, buf);
                }
                else if (state == Key && posn)
                {
                    AfxReportError("%u: Key '%s' missing '=' operator.", line, buf);
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
                AfxMakeString(&bufStr, 0, buf, 0);

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

_AFX afxError AfxStoreInitializationFile(afxManifest const* ini, afxUri const* uri)
{
    afxError err = { 0 };
    FILE* f;
    
    if (!(f = fopen(AfxGetUriData(uri, 0), "w+"))) AfxThrowError();
    else
    {
        for (afxUnit i = 0; i < ini->pageCnt; i++)
        {
            _afxIniPage* pag = &ini->pages[i];
            fprintf(f, i > 0 ? "\n[%.*s]\n" : "[%.*s]\n", AfxPushString(&pag->name));

            for (afxUnit q = 0; q < pag->keyCnt; q++)
            {
                _afxIniRecord* record = &pag->keys[q];

                if (record->key.start[0] == ';')
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

_AFX afxError AfxSetInitializationString(afxManifest* ini, afxString const* sec, afxString const* key, afxString* value)
{
    afxError err = { 0 };
    _afxIniPage* pag;

    if ((!(pag = _AfxIniFindPage(ini, sec))) && (!(pag = _AfxIniCreatePage(ini, sec)))) AfxThrowError();
    else
    {
        _afxIniRecord* record;

        if ((!(record = _AfxIniFindRecord(ini, pag, key))) && (!(record = _AfxIniCreateRecord(ini, pag, key, value))))
            AfxThrowError();
        else
        {
            AfxDeallocateString(&record->value);
            AfxCloneString(&record->value, value);
        }
    }
    return err;
}

_AFX afxBool AfxGetInitializationString(afxManifest const* ini, afxString const* sec, afxString const* key, afxString* value)
{
    afxBool rslt;
    _afxIniRecord* entry;

    if ((rslt = ((rslt = !!(entry = _AfxIniGetRecord(ini, sec, key))) && (!(AfxIsStringEmpty(&entry->value))))))
        *value = entry->value;

    return rslt;
}

_AFXINL afxBool AfxGetInitializationStringAt(afxManifest const* ini, afxUnit secIdx, afxString const* key, afxString* value)
{
    afxBool rslt;
    _afxIniRecord* entry;

    if ((rslt = ((rslt = !!(entry = _AfxIniFindRecord(ini, &ini->pages[secIdx], key))) && (!(AfxIsStringEmpty(&entry->value))))))
        *value = entry->value;

    return rslt;
}

_AFX afxBool AfxGetInitializationUri(afxManifest const* ini, afxString const* sec, afxString const* key, afxUri* value)
{
    afxBool rslt;
    _afxIniRecord* entry;
    afxString val;

    //if ((rslt = ((rslt = !!(entry = _AfxIniGetRecord(ini, sec, key))) && (!(AfxIsStringEmpty(&entry->value))))))
    if ((rslt = ((rslt = AfxGetInitializationString(ini, sec, key, &val)) && (!(AfxIsStringEmpty(&val))))))
        AfxWrapUriString(value, &val);
    else
        AfxResetUris(1, value);

    return rslt;
}

_AFX afxBool AfxGetInitializationUriAt(afxManifest const* ini, afxUnit secIdx, afxString const* key, afxUri* value)
{
    afxBool rslt;
    _afxIniRecord* entry;
    afxString val;

    //if ((rslt = ((rslt = !!(entry = _AfxIniFindRecord(ini, &ini->pages[secIdx], key))) && (!(AfxIsStringEmpty(&entry->value))))))
    if ((rslt = ((rslt = !!_AfxIniFindRecord(ini, &ini->pages[secIdx], key)) && (!(AfxIsStringEmpty(&val))))))
        AfxWrapUriString(value, &val);
    else
        AfxResetUris(1, value);

    return rslt;
}

_AFX afxBool AfxGetInitializationNat(afxManifest const* ini, afxString const* page, afxString const* key, afxUnit* value)
{
    afxBool rslt;
    afxString val;

    if ((rslt = ((rslt = AfxGetInitializationString(ini, page, key, &val)) && (!(AfxIsStringEmpty(&val))))))
    {
        afxString2048 bstr;
        AfxMakeString2048(&bstr, &val);
        *value = (afxUnit)atoi(bstr.s.start);
    }

    return rslt;
}

_AFX afxBool AfxGetInitializationNatAt(afxManifest const* ini, afxUnit secIdx, afxString const* key, afxUnit* value)
{
    afxBool rslt;
    afxString val;

    if ((rslt = ((rslt = !!_AfxIniFindRecord(ini, &ini->pages[secIdx], key)) && (!(AfxIsStringEmpty(&val))))))
    {
        afxString2048 bstr;
        AfxMakeString2048(&bstr, &val);
        *value = (afxUnit)atoi(bstr.s.start);
    }
    return rslt;
}

_AFX afxBool AfxGetInitializationInt(afxManifest const* ini, afxString const* page, afxString const* key, afxInt* value)
{
    afxBool rslt;
    afxString val;
    
    if ((rslt = ((rslt = AfxGetInitializationString(ini, page, key, &val)) && (!(AfxIsStringEmpty(&val))))))
        *value = atoi(val.start);

    return rslt;
}

_AFX afxBool AfxGetInitializationIntAt(afxManifest const* ini, afxUnit secIdx, afxString const* key, afxInt* value)
{
    afxBool rslt;
    afxString val;

    if ((rslt = ((rslt = !!_AfxIniFindRecord(ini, &ini->pages[secIdx], key)) && (!(AfxIsStringEmpty(&val))))))
        *value = atoi(val.start);

    return rslt;
}

_AFX afxBool AfxGetInitializationReal(afxManifest const* ini, afxString const* page, afxString const* key, afxReal* value)
{
    afxBool rslt;
    afxString val;
    
    if ((rslt = ((rslt = AfxGetInitializationString(ini, page, key, &val)) && (!(AfxIsStringEmpty(&val))))))
        *value = (afxReal)atof(val.start);

    return rslt;
}

_AFX afxBool AfxGetInitializationRealAt(afxManifest const* ini, afxUnit secIdx, afxString const* key, afxReal* value)
{
    afxBool rslt;
    afxString val;

    if ((rslt = ((rslt = !!_AfxIniFindRecord(ini, &ini->pages[secIdx], key)) && (!(AfxIsStringEmpty(&val))))))
        *value = (afxReal)atof(val.start);

    return rslt;
}

_AFX afxBool AfxGetInitializationReal64(afxManifest const* ini, afxString const* page, afxString const* key, afxReal64* value)
{
    afxBool rslt;
    afxString val;
    
    if ((rslt = ((rslt = AfxGetInitializationString(ini, page, key, &val)) && (!(AfxIsStringEmpty(&val))))))
        *value = (afxReal64)atof(val.start);

    return rslt;
}

_AFX afxBool AfxGetInitializationReal64At(afxManifest const* ini, afxUnit secIdx, afxString const* key, afxReal64* value)
{
    afxBool rslt;
    afxString val;

    if ((rslt = ((rslt = !!_AfxIniFindRecord(ini, &ini->pages[secIdx], key)) && (!(AfxIsStringEmpty(&val))))))
        *value = (afxReal64)atof(val.start);

    return rslt;
}

_AFX afxBool AfxGetInitializationBool(afxManifest const* ini, afxString const* page, afxString const* key, afxBool* value)
{
    afxBool rslt;
    afxString val;
    
    if ((rslt = ((rslt = AfxGetInitializationString(ini, page, key, &val)) && (!(AfxIsStringEmpty(&val))))) && ((0 == AfxCompareString(&val, 0, "false", 0, TRUE)) || (0 == AfxCompareString(&val, 0, "0", 0, TRUE))))
        *value = FALSE;
    else
        *value = TRUE;

    return rslt;
}

_AFX afxBool AfxGetInitializationBoolAt(afxManifest const* ini, afxUnit secIdx, afxString const* key, afxBool* value)
{
    afxBool rslt;
    afxString val;

    if ((rslt = ((rslt = AfxGetInitializationStringAt(ini, secIdx, key, &val)) && (!(AfxIsStringEmpty(&val))))) && ((0 == AfxCompareString(&val, 0, "false", 0, TRUE)) || (0 == AfxCompareString(&val, 0, "0", 0, TRUE))))
        *value = FALSE;
    else
        *value = TRUE;

    return rslt;
}
