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
#define _AFX_MODULE_C
#define _AFX_DRIVER_C
#define _AFX_DEVICE_C
#include "../dev/afxExecImplKit.h"

_AFX void* AfxGetModuleIdd(afxModule mdle)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdle, afxFcc_MDLE);
    return mdle->osHandle;
}

_AFX void AfxGetDriverUri(afxModule icd, afxUri* uri)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &icd, afxFcc_MDLE);
    AfxAssert(uri);
    afxUri name;
    AfxClipUriTarget(&name, &icd->path.uri);
    AfxCopyUri(uri, &name);
}

_AFX afxManifest const* AfxGetModuleManifest(afxModule icd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &icd, afxFcc_MDLE);
    return &icd->etc;
}

_AFX void AfxGetModuleVersion(afxModule icd, afxNat* verMajor, afxNat* verMinor, afxNat* verPatch)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &icd, afxFcc_MDLE);
    AfxAssert(verMajor || verMinor || verPatch);

    if (verMajor)
        *verMajor = icd->verMajor;

    if (verMinor)
        *verMinor = icd->verMinor;

    if (verPatch)
        *verPatch = icd->verPatch;
}

_AFX void AfxGetModuleInfo(afxModule icd, afxString* product, afxString* vendor, afxString* description)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &icd, afxFcc_MDLE);
    AfxAssert(product || vendor || description);

    if (product)
        AfxCopyString(product, &icd->product);

    if (vendor)
        AfxCopyString(vendor, &icd->vendor);

    if (description)
        AfxCopyString(description, &icd->description);
}

_AFX afxNat AfxFindSymbolAddresses(afxModule mdle, afxNat cnt, afxString const names[], void* addresses[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdle, afxFcc_MDLE);
    AfxAssert(names);
    AfxAssert(cnt);
    afxResult hitCnt = 0;

    BYTE* peImgBase = mdle->osHandle;
    afxBool demangle = mdle->demangle;

#ifdef AFX_OS_WIN
    PIMAGE_NT_HEADERS header = (PIMAGE_NT_HEADERS)(peImgBase + ((PIMAGE_DOS_HEADER)peImgBase)->e_lfanew);
    AfxAssert(header->Signature == IMAGE_NT_SIGNATURE);
    AfxAssert(header->OptionalHeader.NumberOfRvaAndSizes > 0);
    PIMAGE_EXPORT_DIRECTORY exports = (PIMAGE_EXPORT_DIRECTORY)(peImgBase + header->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);
    AfxAssert(exports->AddressOfNames != 0);
    DWORD* symNames = (DWORD*)(peImgBase + exports->AddressOfNames);

    for (afxNat j = 0; j < cnt; j++)
    {
        void* p = NIL;

        afxString128 fs;
        AfxMakeString128(&fs, &names[j]); // needed due to zero-terminate string-based shit M$ APIs

        if ((p = (void*)GetProcAddress((HMODULE)peImgBase, AfxGetStringData(&fs.str, 0))))
        {
            hitCnt++;
        }
        else if (demangle)
        {
            for (afxNat i = 0; i < exports->NumberOfNames; i++)
            {
                void const* rawName = peImgBase + symNames[i];
                afxString symNameStr;
                AfxMakeString(&symNameStr, 0, rawName, 0);

                afxNat prefixPosn, suffixPosn;

                if ((AFX_INVALID_INDEX != (prefixPosn = AfxFindFirstChar(&symNameStr, 0, '_')) &&
                    (AFX_INVALID_INDEX != (suffixPosn = AfxFindLastChar(&symNameStr, 0, '@')))))
                {
                    AfxExcerptString(&symNameStr, prefixPosn + 1, suffixPosn - 1, &symNameStr);

                    if (0 == AfxCompareString(&symNameStr, &names[j]))
                    {
                        if ((p = (void*)GetProcAddress((HMODULE)peImgBase, rawName)))
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
        DWORD* symNames = (DWORD*)((afxSize)osHandle + exports->AddressOfNames);

        for (afxNat j = 0; j < cnt; j++)
        {
            void* p = NIL;

            afxString128 fs;
            AfxMakeString128(&fs, &names[j]); // needed due to zero-terminate string-based shit M$ APIs

            if ((p = (void*)GetProcAddress(osHandle, AfxGetStringData(&fs.str, 0))))
            {
                hitCnt++;
            }
            else if (demangle)
            {
                for (afxNat i = 0; i < exports->NumberOfNames; i++)
                {
                    void const* rawName = (BYTE *)osHandle + symNames[i];
                    afxString symNameStr;
                    AfxMakeString(&symNameStr, 0, rawName, 0);

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

_AFX afxError _AfxMdleCtorCb(afxModule mdle, void** args, afxNat invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdle, afxFcc_MDLE);

    afxUri const* uri = ((afxUri const*)args[1]) + invokeNo;
    afxFlags flags = args[2] ? *(afxFlags*)args[2] : NIL;
    void* osHandle = (args[3]);

    AfxDeployChain(&mdle->devices, mdle);
    mdle->attached = FALSE;

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

    AfxResetString(&mdle->vendor);
    AfxResetString(&mdle->description);
    AfxResetString(&mdle->product);

    //HMODULE osHandle;// = GetModuleHandleA(AfxGetStringData(pathStr, 0));

    //if (!oshandle)
    {
#if 0
#ifdef AFX_OS_WIN
        osHandle = LoadLibraryExA(AfxGetUriData(&libpath.uri, 0), NIL, NIL);
#else
        osHandle = dlopen(AfxGetUriData(&libpath.uri, 0), RTLD_NOW);
#endif
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
        mdle->type = afxModuleType_DLL;

        if (flags & AFX_BIT(8))
        {
            mdle->type = afxModuleType_SYS;

        }

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

_AFX afxError _AfxMdleDtorCb(afxModule mdle)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdle, afxFcc_MDLE);

    //if (mdle->osHandle != AfxApplicationGet()->e2coree->osHandle)

    // objects will be released at class drop.

    afxDevice dev;
    AfxChainForEveryLinkage(&mdle->devices, AFX_OBJ(afxDevice), icd, dev)
    {
        AfxAssertObjects(1, &dev, afxFcc_DEV);
        while (AfxReleaseObjects(1, &dev));
};
    AfxAssert(AfxChainIsEmpty(&mdle->devices));

    AfxDismantleManifest(&mdle->etc);

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
    .desc = "Executable & Linkable Module",
    .fixedSiz = sizeof(AFX_OBJECT(afxModule)),
    .ctor = (void*)_AfxMdleCtorCb,
    .dtor = (void*)_AfxMdleDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AFX afxNat AfxInvokeModules(afxNat first, afxNat cnt, afxBool(*f)(afxModule, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(f);
    afxClass* cls = AfxGetModuleClass();
    AfxAssertClass(cls, afxFcc_MDLE);
    return cnt ? AfxInvokeClassInstances(cls, first, cnt, (void*)f, udd) : 0;
}

_AFX afxNat AfxEnumerateModules(afxNat first, afxNat cnt, afxModule executables[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(executables);
    afxClass* cls = AfxGetModuleClass();
    AfxAssertClass(cls, afxFcc_MDLE);
    return cnt ? AfxEnumerateClassInstances(cls, first, cnt, (afxObject*)executables) : 0;
}

_AFX afxBool AfxFindModule(afxUri const* uri, afxModule* module)
{
    afxError err = AFX_ERR_NONE;
    afxBool found = FALSE;
    AfxAssert(uri);
    
    afxUri fname;
    AfxClipUriFile(&fname, uri);
    afxUri2048 uriz;
    AfxMakeUri2048(&uriz, &fname);
#if AFX_OS_WIN
    HMODULE hMod = GetModuleHandleA(AfxGetUriData(&uriz.uri, 0));
#else
    void* hMod = NIL;
    AfxThrowError();
#endif
    afxNat i = 0;
    afxModule mdle;
    while (AfxEnumerateModules(i, 1, &mdle))
    {
        AfxAssertObjects(1, &mdle, afxFcc_MDLE);
        
        if (hMod == AfxGetModuleIdd(mdle))
        {
            found = TRUE;
            break;
        }
        ++i;
    }

    AfxAssert(module);
    *module = found ? mdle : NIL;
    return found;
}

_AFX afxBool _AfxFindAndLoadDllCb(void* udd, afxUri const* path, afxUri const* osPath)
{
    afxError err = AFX_ERR_NONE;
    afxBool next = TRUE;
    HMODULE hMod = LoadLibraryExA(AfxGetUriData(osPath, 0), NIL, NIL);

    if (hMod)
    {
        afxFlags flags = *(afxFlags*)((void**)udd)[0];
        afxModule* pVar = ((void**)udd)[1];
        afxClass* cls = AfxGetModuleClass();
        AfxAssertClass(cls, afxFcc_MDLE);
        afxModule mdle = NIL;

        if (AfxAcquireObjects(cls, 1, (afxObject*)&mdle, (void const*[]) { NIL, path, &flags, hMod }))
        {
            AfxThrowError();
            FreeLibrary(hMod);
        }
        else
        {
            AfxAssert(pVar);
            *pVar = mdle;
            next = FALSE; // break
        }
    }
    return next;
}

_AFX afxError AfxLoadModule(afxUri const* uri, afxFlags flags, afxModule* module)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(uri);
    afxModule mdle = NIL;
    HMODULE hMod = NIL;

    afxClass* cls = AfxGetModuleClass();
    AfxAssertClass(cls, afxFcc_MDLE);

    if (!uri) AfxThrowError();
    else
    {
        afxUri2048 urib;
        AfxMakeUri2048(&urib, uri);

        if (!urib.uri.fext)
        {
            AfxFormatUri(&urib.uri, "%.*s.dll", AfxPushString(&uri->str));
        }

        AfxLogEcho("Loading... Executable And Linkage Module <%.*s>", AfxPushString(uri ? AfxGetUriString(&urib.uri) : &AFX_STR_EMPTY));

        if (!AfxFindFiles(&urib.uri, afxFileFlag_R, _AfxFindAndLoadDllCb, (void*[]) { &flags, &mdle }) || !mdle) AfxThrowError();
        else
        {
            AfxAssertObjects(1, &mdle, afxFcc_MDLE);
            AfxAssert(module);
            *module = mdle;
        }
    }
    return err;
}

_AFX afxError _AfxAttachDriver(afxModule mdle, afxUri const* manifest, void* udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdle, afxFcc_MDLE);

    if (!mdle->attached)
    {
        AfxLogEcho("Loading... Installable Client Driver <%.*s>", AfxPushString(&mdle->path.uri.str));

        afxError(*icdHookFn)(afxModule, afxUri const*) = AfxFindModuleSymbol(mdle, "afxIcdHook");

        if (!icdHookFn) AfxThrowError();
        else
        {
            mdle->icdHookFn = icdHookFn;

            afxModuleType mdleType = mdle->type;
            mdle->type = afxModuleType_ICD;

            if (icdHookFn(mdle, manifest)) AfxThrowError();
            else
            {
                afxString128 s, devDesc, providerName, providerUrl;
                AfxMakeString128(&s, NIL);
                AfxMakeString128(&devDesc, NIL);
                AfxMakeString128(&providerName, NIL);
                AfxMakeString128(&providerUrl, NIL);
                afxNat verMajor, verMinor, verPatch;

                AfxGetModuleVersion(mdle, &verMajor, &verMinor, &verPatch);
                AfxGetModuleInfo(mdle, &s.str, &providerName.str, &devDesc.str);

                afxDevice dev;
                AfxChainForEveryLinkageB2F(&mdle->devices, AFX_OBJ(afxDevice), icd, dev)
                {
                    AfxAssertObjects(1, &dev, afxFcc_DEV);

                    AfxLogY("\t<//./%.*s/>(\"%.*s\") %.*s %u.%u.%u %.*s",
                        AfxPushString(&dev->urn.uri.str), AfxPushString(&s.str), AfxPushString(&devDesc.str), verMajor, verMinor, verPatch, AfxPushString(&providerName.str));
                };

                mdle->attached = TRUE;
            }

            if (err)
                mdle->type = mdleType;
        }
    }
    return err;
}
