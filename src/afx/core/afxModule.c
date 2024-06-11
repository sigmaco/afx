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
#define WIN32_LEAN_AND_MEAN 1

#include <sys/stat.h>
#include <stdio.h>

#if (defined(_WIN32) || defined(_WIN64))
#   include <Windows.h>
#   include <Shlwapi.h>
#   include <combaseapi.h>
#else
#   include <unistd.h>
#endif

#define _AFX_CORE_C
#define _AFX_EXECUTABLE_C
#include "qwadro/core/afxSystem.h"

_AFX void* AfxGetModuleIdd(afxModule mdle)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdle, afxFcc_MDLE);
    return mdle->osHandle;
}

_AFX afxNat AfxFindSymbolAddresses(afxModule mdle, afxNat cnt, afxString const names[], void* addresses[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdle, afxFcc_MDLE);
    AfxAssert(names);
    AfxAssert(cnt);
    afxResult hitCnt = 0;

    void* osHandle = mdle->osHandle;
    afxBool demangle = mdle->demangle;

#ifdef AFX_OS_WIN
    PIMAGE_NT_HEADERS header = (PIMAGE_NT_HEADERS)((BYTE *)osHandle + ((PIMAGE_DOS_HEADER)osHandle)->e_lfanew);
    AfxAssert(header->Signature == IMAGE_NT_SIGNATURE);
    AfxAssert(header->OptionalHeader.NumberOfRvaAndSizes > 0);
    PIMAGE_EXPORT_DIRECTORY exports = (PIMAGE_EXPORT_DIRECTORY)((BYTE *)osHandle + header->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);
    AfxAssert(exports->AddressOfNames != 0);
    BYTE** symNames = (BYTE**)((int)osHandle + exports->AddressOfNames);

    for (afxNat j = 0; j < cnt; j++)
    {
        void* p = NIL;

        afxString128 fs;
        AfxMakeString128(&fs, &names[j]); // needed due to zero-terminate string-based shit M$ APIs

        if ((p = (void*)GetProcAddress(osHandle, AfxGetStringData(&fs.str.str, 0))))
        {
            hitCnt++;
        }
        else if (demangle)
        {
            for (afxNat i = 0; i < exports->NumberOfNames; i++)
            {
                void const* rawName = (BYTE *)osHandle + (int)symNames[i];
                afxString symNameStr;
                AfxMakeString(&symNameStr, rawName, 0);

                afxNat prefixPosn, suffixPosn;

                if ((AFX_INVALID_INDEX != (prefixPosn = AfxFindFirstChar(&symNameStr, 0, '_')) &&
                    (AFX_INVALID_INDEX != (suffixPosn = AfxFindLastChar(&symNameStr, 0, '@')))))
                {
                    AfxExcerptString(&symNameStr, prefixPosn + 1, suffixPosn - 1, &symNameStr);

                    if (0 == AfxCompareString(&symNameStr, &names[j]))
                    {
                        if ((p = (void*)GetProcAddress(osHandle, rawName)))
                        {
                            hitCnt++;
                        }
                        break;
                    }
                }
            }
        }

        addresses[j] = p;
    }

#else
#   error "Not implemented"
#endif
    return hitCnt;
}

_AFX afxResult AfxFindModuleSymbols(afxModule mdle, afxNat cnt, afxChar const *name[], void *syms[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdle, afxFcc_MDLE);

    AfxAssert(cnt);
    AfxAssert(name);
    afxResult hitCnt = 0;

    for (afxNat i = 0; i < cnt; i++)
    {
#ifdef AFX_OS_WIN
        syms[i] = (void*)GetProcAddress(mdle->osHandle, name[i]);
#else
        syms[i] = dlsym(mdle->osHandle, name[i]);
#endif

        if ((syms[i])) hitCnt++;
        else
        {
            syms[i] = NIL;
            //AfxLogError("Symbol %s not found.", name[i]);
        }
    }
    return hitCnt;
}

_AFX void* AfxFindModuleSymbol(afxModule mdle, afxChar const *name)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdle, afxFcc_MDLE);
    AfxAssert(name);
    void *sym = NIL;
    AfxFindModuleSymbols(mdle, 1, &name, &sym);
    return sym;
}

_AFX afxNat AfxGetSymbolAddresses2(afxModule mdle, afxBool demangle, afxNat cnt, afxString const names[], void* addresses[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdle, afxFcc_MDLE);
    AfxAssert(names);
    AfxAssert(cnt);
    afxResult hitCnt = 0;

    if (demangle) hitCnt = AfxGetSymbolAddresses(mdle, cnt, names, addresses);
    else
    {
        void* osHandle = mdle->osHandle;

#ifdef AFX_OS_WIN
        PIMAGE_NT_HEADERS header = (PIMAGE_NT_HEADERS)((BYTE *)osHandle + ((PIMAGE_DOS_HEADER)osHandle)->e_lfanew);
        AfxAssert(header->Signature == IMAGE_NT_SIGNATURE);
        AfxAssert(header->OptionalHeader.NumberOfRvaAndSizes > 0);
        PIMAGE_EXPORT_DIRECTORY exports = (PIMAGE_EXPORT_DIRECTORY)((BYTE *)osHandle + header->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);
        AfxAssert(exports->AddressOfNames != 0);
        BYTE** symNames = (BYTE**)((int)osHandle + exports->AddressOfNames);

        for (afxNat j = 0; j < cnt; j++)
        {
            void* p = NIL;

            afxString128 fs;
            AfxMakeString128(&fs, &names[j]); // needed due to zero-terminate string-based shit M$ APIs

            if ((p = (void*)GetProcAddress(osHandle, AfxGetStringData(&fs.str.str, 0))))
            {
                hitCnt++;
            }
            else if (demangle)
            {
                for (afxNat i = 0; i < exports->NumberOfNames; i++)
                {
                    void const* rawName = (BYTE *)osHandle + (int)symNames[i];
                    afxString symNameStr;
                    AfxMakeString(&symNameStr, rawName, 0);

                    afxNat prefixPosn, suffixPosn;

                    if ((AFX_INVALID_INDEX != (prefixPosn = AfxFindFirstChar(&symNameStr, 0, '_')) &&
                        (AFX_INVALID_INDEX != (suffixPosn = AfxFindLastChar(&symNameStr, 0, '@')))))
                    {
                        AfxExcerptString(&symNameStr, prefixPosn + 1, suffixPosn - 1, &symNameStr);

                        if (0 == AfxCompareString(&symNameStr, &names[j]))
                        {
                            if ((p = (void*)GetProcAddress(osHandle, rawName)))
                            {
                                hitCnt++;
                            }
                            break;
                        }
                    }
                }
            }

            addresses[j] = p;
        }

#else
#   error "Not implemented"
#endif
    }
    return hitCnt;
}

_AFX afxNat AfxGetSymbolAddresses(afxModule mdle, afxNat cnt, afxString const names[], void* addresses[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdle, afxFcc_MDLE);
    AfxAssert(addresses);
    AfxAssert(names);
    AfxAssert(cnt);
    afxResult hitCnt = 0;

    void* osHandle = mdle->osHandle;

    afxString128 s;
    AfxMakeString128(&s, NIL);

    for (afxNat i = 0; i < cnt; i++)
    {
        AfxCopyString(&s.str, &names[i]);

#ifdef AFX_OS_WIN
        addresses[i] = (void*)GetProcAddress((void*)osHandle, s.buf);
#else
        addresses[i] = dlsym((void*)osHandle, s.buf);
#endif

        if ((addresses[i])) hitCnt++;
        else
        {
            addresses[i] = NIL;
        }
    }
    return hitCnt;
}

_AFX void* AfxGetSymbolAddress(afxModule mdle, afxString const* name)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdle, afxFcc_MDLE);
    AfxAssert(name);
    void *sym = NIL;
    AfxGetSymbolAddresses(mdle, 1, name, &sym);
    return sym;
}

_AFX void AfxGetModulePath(afxModule mdle, afxUri* uri)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdle, afxFcc_MDLE);
    AfxAssert(uri);
    AfxCopyUri(uri, &mdle->path.uri);
}

_AFX afxError _AfxMdleCtor(afxModule mdle, afxCookie const* cookie)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdle, afxFcc_MDLE);

    afxUri const* uri = ((afxUri const*)cookie->udd[1]) + cookie->no;

    afxBool isEmpty = !uri || AfxUriIsBlank(uri);
    AfxAssert(!isEmpty);
    afxResult success = FALSE;
    afxResult loaded = FALSE;
    
    afxUri128 absUri;
    AfxMakeUri128(&absUri, uri);

    //AfxResolveUri(afxFileFlag_R, uri, &absUri.uri);

    //afxString1024 str;
    //AfxString1024(&str, 0, 0);
    afxUri path;
    //AfxCopyString(AfxPickUriPath(&absUri, &tmp), &str.str);
    AfxClipUriPath(&path, &absUri.uri);
    afxChar const *pathStr = AfxGetUriData(&path, 0);

    HMODULE osHandle;// = GetModuleHandleA(AfxGetStringData(pathStr, 0));

    //if (!oshandle)
    {
#ifdef AFX_OS_WIN
        osHandle = LoadLibraryA(pathStr);
#else
        osHandle = dlopen(pathStr, RTLD_NOW);
#endif

        if (!(loaded = !!(osHandle)))
            AfxThrowError();
    }

    if (osHandle)
    {
        success = TRUE;
        AfxMakeUri128(&mdle->path, uri);
        mdle->osHandle = osHandle;
        mdle->hasBeenLoaded = loaded;
        mdle->demangle = TRUE;

        if (FALSE == success)
        {
            if (loaded)
            {
#ifdef AFX_OS_WIN
                FreeLibrary(osHandle);
#else
                dlclose(osHandle);
#endif
            }
        }
    }
    return err;
}

_AFX afxError _AfxMdleDtor(afxModule mdle)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdle, afxFcc_MDLE);

    //if (mdle->osHandle != AfxApplicationGet()->e2coree->osHandle)

    if (mdle->hasBeenLoaded)
    {
#ifdef AFX_OS_WIN
        FreeLibrary(mdle->osHandle);
#else
        dlclose(mdle->osHandle);
#endif
    }
    return err;
}

_AFX afxClassConfig const _AfxMdleMgrCfg =
{
    .fcc = afxFcc_MDLE,
    .name = "Module",
    .desc = "Executable & Linkable Modulation",
    .unitsPerPage = 2,
    .size = sizeof(AFX_OBJECT(afxModule)),
    .mmu = NIL,
    .ctor = (void*)_AfxMdleCtor,
    .dtor = (void*)_AfxMdleDtor
};

////////////////////////////////////////////////////////////////////////////////

_AFX afxError AfxLoadModule(afxUri const* uri, afxFlags flags, afxModule* mdle)
{
    afxError err = AFX_ERR_NONE;
    afxModule mdle2 = NIL;

    AfxLogEcho("Loading module... <%.*s>", AfxPushString(uri ? AfxGetUriString(uri) : &AFX_STR_EMPTY));

    if (AfxFindModule(uri, &mdle2))
    {
        if (AfxReacquireObjects(1, (void*[]) { mdle2 }))
            AfxThrowError();
    }
    else
    {
        afxManager* cls = AfxGetModuleClass();
        AfxAssertClass(cls, afxFcc_MDLE);

        if (AfxAcquireObjects(cls, 1, (afxObject*)&mdle2, (void const*[]) { NIL, uri }))
            AfxThrowError();
    }

    AfxAssert(mdle);
    *mdle = mdle2;
    return err;
}

_AFX afxNat AfxInvokeModules(afxNat first, afxNat cnt, afxBool(*f)(afxModule, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(f);
    afxManager* cls = AfxGetModuleClass();
    AfxAssertClass(cls, afxFcc_MDLE);
    return cnt ? AfxInvokeObjects(cls, first, cnt, (void*)f, udd) : 0;
}

_AFX afxNat AfxEnumerateModules(afxNat first, afxNat cnt, afxModule executables[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(executables);
    afxManager* cls = AfxGetModuleClass();
    AfxAssertClass(cls, afxFcc_MDLE);
    return cnt ? AfxEnumerateObjects(cls, first, cnt, (afxObject*)executables) : 0;
}

_AFX afxNat AfxCountModules(void)
{
    afxError err = AFX_ERR_NONE;
    afxManager* cls = AfxGetModuleClass();
    AfxAssertClass(cls, afxFcc_MDLE);
    return AfxCountObjects(cls);
}

_AFX afxBool AfxFindModule(afxUri const *uri, afxModule* mdle)
{
    afxError err = AFX_ERR_NONE;
    afxBool found = FALSE;
    AfxAssert(uri);

    afxNat i = 0;
    afxModule mdle2;
    while (AfxEnumerateModules(i, 1, &mdle2))
    {
        AfxAssertObjects(1, &mdle2, afxFcc_MDLE);

        afxUri exeFile;

        if (uri->fext)
        {
            AfxClipUriFile(&exeFile, &mdle2->path.uri);

            if (0 == AfxCompareUri(&exeFile, uri))
            {
                found = TRUE;
                break;
            }
        }
        else
        {
            AfxClipUriTarget(&exeFile, &mdle2->path.uri);

            if (0 == AfxCompareUri(&exeFile, uri))
            {
                found = TRUE;
                break;
            }
        }
        ++i;
    }
    AfxAssert(mdle);
    *mdle = found ? mdle2 : NIL;
    return found;
}
