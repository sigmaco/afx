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
#include "qwadro/core/afxClass.h"
#include "qwadro/core/afxSystem.h"

_AFX afxNat AfxFindSymbolAddresses(afxExecutable exe, afxNat cnt, afxString const names[], void* addresses[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &exe, afxFcc_EXE);
    AfxAssert(names);
    AfxAssert(cnt);
    afxResult hitCnt = 0;

    void* osHandle = exe->osHandle;
    afxBool demangle = exe->demangle;

#ifdef AFX_PLATFORM_WIN
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

_AFX afxResult AfxFindModuleSymbols(afxExecutable exe, afxNat cnt, afxChar const *name[], void *syms[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &exe, afxFcc_EXE);

    AfxAssert(cnt);
    AfxAssert(name);
    afxResult hitCnt = 0;

    for (afxNat i = 0; i < cnt; i++)
    {
#ifdef AFX_PLATFORM_WIN
        syms[i] = (void*)GetProcAddress(exe->osHandle, name[i]);
#else
        syms[i] = dlsym(exe->osHandle, name[i]);
#endif

        if ((syms[i])) hitCnt++;
        else
        {
            syms[i] = NIL;
            //AfxError("Symbol %s not found.", name[i]);
        }
    }
    return hitCnt;
}

_AFX void* AfxFindExecutableSymbol(afxExecutable exe, afxChar const *name)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &exe, afxFcc_EXE);
    AfxAssert(name);
    void *sym = NIL;
    AfxFindModuleSymbols(exe, 1, &name, &sym);
    return sym;
}

_AFX afxUri const* AfxGetExecutablePath(afxExecutable exe)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &exe, afxFcc_EXE);
    return &exe->path.uri;
}

_AFX afxError _AfxExeCtor(afxExecutable exe, afxCookie const* cookie)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &exe, afxFcc_EXE);

    afxUri const* uri = ((afxUri const*)cookie->udd[1]) + cookie->no;

    AfxEntry("uri:%.*s", AfxPushString(uri ? AfxGetUriString(uri) : &AFX_STR_EMPTY));

    afxBool isEmpty = !uri || AfxUriIsBlank(uri);
    AfxAssert(!isEmpty);
    afxResult success = FALSE;
    afxResult loaded = FALSE;
    
    afxUri128 absUri;
    AfxMakeUri128(&absUri, NIL);

    AfxResolveUri(afxFileFlag_R, uri, &absUri.uri);

    //afxString1024 str;
    //AfxString1024(&str, 0, 0);
    afxUri path;
    //AfxCopyString(AfxExcerptUriPath(&absUri, &tmp), &str.str);
    AfxExcerptUriPath(&absUri.uri, &path);
    afxChar const *pathStr = AfxGetUriData(&path, 0);

    HMODULE osHandle;// = GetModuleHandleA(AfxGetStringData(pathStr, 0));

    //if (!oshandle)
    {
#ifdef AFX_PLATFORM_WIN
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
        AfxMakeUri128(&exe->path, uri);
        exe->osHandle = osHandle;
        exe->hasBeenLoaded = loaded;
        exe->demangle = TRUE;

        if (FALSE == success)
        {
            if (loaded)
            {
#ifdef AFX_PLATFORM_WIN
                FreeLibrary(osHandle);
#else
                dlclose(osHandle);
#endif
            }
        }
    }
    return err;
}

_AFX afxError _AfxExeDtor(afxExecutable exe)
{
    AfxEntry("exe=%p", exe);
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &exe, afxFcc_EXE);

    //if (exe->osHandle != AfxApplicationGet()->e2coree->osHandle)

    if (exe->hasBeenLoaded)
    {
#ifdef AFX_PLATFORM_WIN
        FreeLibrary(exe->osHandle);
#else
        dlclose(exe->osHandle);
#endif
    }
    return err;
}

_AFX afxClassConfig const _AfxMdleClsConfig =
{
    .fcc = afxFcc_EXE,
    .name = "Executable & Linkable Module",
    .unitsPerPage = 2,
    .size = sizeof(AFX_OBJECT(afxExecutable)),
    .mmu = NIL,
    .ctor = (void*)_AfxExeCtor,
    .dtor = (void*)_AfxExeDtor
};

////////////////////////////////////////////////////////////////////////////////

_AFX afxError AfxLoadExecutables(afxNat cnt, afxUri const uri[], afxExecutable executables[])
{
    AfxEntry("cnt=%u,executables=%p,uri=%p", cnt, executables, uri);
    afxError err = AFX_ERR_NONE;

    afxClass* cls = AfxGetExecutableClass();
    AfxAssertClass(cls, afxFcc_EXE);

    for (afxNat i = 0; i < cnt; i++)
    {
        afxExecutable tmp = AfxFindExecutable(&uri[i]);

        if (tmp)
        {
            if (AfxReacquireObjects(1, (afxObject*)&tmp)) AfxThrowError();
            else
            {
                executables[i] = tmp;
            }
        }
        else
        {
            if (AfxAcquireObjects(cls, 1, (afxObject*)&executables[i], (void const*[]) { NIL, &uri[i] }))
                AfxThrowError();
        }

        if (err)
        {
            AfxReleaseObjects(i, (afxObject*)executables);
            break;
        }
    }
    return err;
}

_AFX afxNat AfxInvokeExecutables(afxNat first, afxNat cnt, afxBool(*f)(afxExecutable, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(f);
    afxClass* cls = AfxGetExecutableClass();
    AfxAssertClass(cls, afxFcc_EXE);
    return cnt ? AfxInvokeInstances(cls, first, cnt, (void*)f, udd) : 0;
}

_AFX afxNat AfxEnumerateExecutables(afxNat first, afxNat cnt, afxExecutable executables[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(executables);
    afxClass* cls = AfxGetExecutableClass();
    AfxAssertClass(cls, afxFcc_EXE);
    return cnt ? AfxEnumerateInstances(cls, first, cnt, (afxObject*)executables) : 0;
}

_AFX afxNat AfxCountExecutables(void)
{
    afxError err = AFX_ERR_NONE;
    afxClass* cls = AfxGetExecutableClass();
    AfxAssertClass(cls, afxFcc_EXE);
    return AfxCountInstances(cls);
}

_AFX afxExecutable AfxFindExecutable(afxUri const *uri)
{
    afxError err = AFX_ERR_NONE;
    
    afxNat i = 0;
    afxExecutable exe;
    while (AfxEnumerateExecutables(i, 1, &exe))
    {
        AfxAssertObjects(1, &exe, afxFcc_EXE);

        if (0 == AfxCompareUri(AfxGetExecutablePath(exe), uri))
            return exe;

        ++i;
    }
    return NIL;
}
