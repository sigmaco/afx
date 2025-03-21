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
#define _AFX_SYSTEM_C
#define _AFX_DRIVER_C
#define _AFX_DEVICE_C
#include "../impl/afxExecImplKit.h"

_AFX afxModuleFlags AfxTestModule(afxModule mdle, afxModuleFlags bitmask)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &mdle);
    return mdle->flags & bitmask;
}

_AFX void* AfxGetModuleIdd(afxModule mdle)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &mdle);
    return mdle->osHandle;
}

_AFX void AfxGetDriverUri(afxModule icd, afxUri* uri)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &icd);
    AFX_ASSERT(uri);
    afxUri name;
    AfxClipUriTarget(&name, &icd->path.uri);
    AfxCopyUri(uri, &name);
}

_AFX afxManifest const* AfxGetModuleManifest(afxModule icd)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &icd);
    return &icd->etc;
}

_AFX void AfxGetModuleVersion(afxModule icd, afxUnit* verMajor, afxUnit* verMinor, afxUnit* verPatch)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &icd);
    AFX_ASSERT(verMajor || verMinor || verPatch);

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
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &icd);
    AFX_ASSERT(product || vendor || description);

    if (product)
        AfxCopyString(product, &icd->product);

    if (vendor)
        AfxCopyString(vendor, &icd->vendor);

    if (description)
        AfxCopyString(description, &icd->description);
}

_AFX afxUnit AfxFindSymbolAddresses(afxModule mdle, afxUnit cnt, afxString const names[], void* addresses[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &mdle);
    AFX_ASSERT(names);
    AFX_ASSERT(cnt);
    afxResult hitCnt = 0;

    BYTE* peImgBase = mdle->osHandle;
    afxBool demangle = mdle->demangle;

#ifdef AFX_OS_WIN
    PIMAGE_NT_HEADERS header = (PIMAGE_NT_HEADERS)(peImgBase + ((PIMAGE_DOS_HEADER)peImgBase)->e_lfanew);
    AFX_ASSERT(header->Signature == IMAGE_NT_SIGNATURE);
    AFX_ASSERT(header->OptionalHeader.NumberOfRvaAndSizes > 0);
    PIMAGE_EXPORT_DIRECTORY exports = (PIMAGE_EXPORT_DIRECTORY)(peImgBase + header->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);
    AFX_ASSERT(exports->AddressOfNames != 0);
    DWORD* symNames = (DWORD*)(peImgBase + exports->AddressOfNames);

    for (afxUnit j = 0; j < cnt; j++)
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
            for (afxUnit i = 0; i < exports->NumberOfNames; i++)
            {
                void const* rawName = peImgBase + symNames[i];
                afxString symNameStr;
                AfxMakeString(&symNameStr, 0, rawName, 0);

                afxUnit prefixPosn, suffixPosn;

                if (AfxFindFirstChar(&symNameStr, 0, '_', &prefixPosn) &&
                    AfxFindLastChar(&symNameStr, 0, '@', &suffixPosn))
                {
                    AfxExcerptString(&symNameStr, prefixPosn + 1, suffixPosn - 1, &symNameStr);

                    if (0 == AfxCompareStrings(&symNameStr, 0, FALSE, 1, &names[j]))
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

_AFX afxResult AfxFindModuleSymbols(afxModule mdle, afxUnit cnt, afxChar const *name[], void *syms[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &mdle);

    AFX_ASSERT(cnt);
    AFX_ASSERT(name);
    afxResult hitCnt = 0;

    for (afxUnit i = 0; i < cnt; i++)
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
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &mdle);
    AFX_ASSERT(name);
    void *sym = NIL;
    AfxFindModuleSymbols(mdle, 1, &name, &sym);
    return sym;
}

_AFX afxUnit AfxGetSymbolAddresses2(afxModule mdle, afxBool demangle, afxUnit cnt, afxString const names[], void* addresses[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &mdle);
    AFX_ASSERT(names);
    AFX_ASSERT(cnt);
    afxResult hitCnt = 0;

    if (demangle) hitCnt = AfxGetSymbolAddresses(mdle, cnt, names, addresses);
    else
    {
        void* osHandle = mdle->osHandle;

#ifdef AFX_OS_WIN
        PIMAGE_NT_HEADERS header = (PIMAGE_NT_HEADERS)((BYTE *)osHandle + ((PIMAGE_DOS_HEADER)osHandle)->e_lfanew);
        AFX_ASSERT(header->Signature == IMAGE_NT_SIGNATURE);
        AFX_ASSERT(header->OptionalHeader.NumberOfRvaAndSizes > 0);
        PIMAGE_EXPORT_DIRECTORY exports = (PIMAGE_EXPORT_DIRECTORY)((BYTE *)osHandle + header->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);
        AFX_ASSERT(exports->AddressOfNames != 0);
        DWORD* symNames = (DWORD*)((afxSize)osHandle + exports->AddressOfNames);

        for (afxUnit j = 0; j < cnt; j++)
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
                for (afxUnit i = 0; i < exports->NumberOfNames; i++)
                {
                    void const* rawName = (BYTE *)osHandle + symNames[i];
                    afxString symNameStr;
                    AfxMakeString(&symNameStr, 0, rawName, 0);

                    afxUnit prefixPosn, suffixPosn;

                    if (AfxFindFirstChar(&symNameStr, 0, '_', &prefixPosn) &&
                        AfxFindLastChar(&symNameStr, 0, '@', &suffixPosn))
                    {
                        AfxExcerptString(&symNameStr, prefixPosn + 1, suffixPosn - 1, &symNameStr);

                        if (0 == AfxCompareStrings(&symNameStr, 0, FALSE, 1, &names[j]))
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

_AFX afxUnit AfxGetSymbolAddresses(afxModule mdle, afxUnit cnt, afxString const names[], void* addresses[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &mdle);
    AFX_ASSERT(addresses);
    AFX_ASSERT(names);
    AFX_ASSERT(cnt);
    afxResult hitCnt = 0;

    void* osHandle = mdle->osHandle;

    afxString128 s;
    AfxMakeString128(&s, NIL);

    for (afxUnit i = 0; i < cnt; i++)
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
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &mdle);
    AFX_ASSERT(name);
    void *sym = NIL;
    AfxGetSymbolAddresses(mdle, 1, name, &sym);
    return sym;
}

_AFX void AfxGetModulePath(afxModule mdle, afxUri* uri)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &mdle);
    AFX_ASSERT(uri);
    AfxCopyUri(uri, &mdle->path.uri);
}

_AFX afxError _AfxMdleDtorCb(afxModule mdle)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &mdle);

    //if (mdle->osHandle != AfxApplicationGet()->e2coree->osHandle)

    // objects will be released at class drop.

    afxDevice dev;
    AFX_ITERATE_CHAIN(&mdle->devices, AFX_OBJ(afxDevice), icd, dev)
    {
        AFX_ASSERT_OBJECTS(afxFcc_DEV, 1, &dev);
        while (AfxDisposeObjects(1, &dev));
};
    AFX_ASSERT(AfxIsChainEmpty(&mdle->devices));

    AfxDeregisterChainedClasses(&mdle->classes);

    AfxDismantleManifest(&mdle->etc);

#ifdef AFX_OS_WIN
    FreeLibrary(mdle->osHandle);
#else
    dlclose(mdle->osHandle);
#endif

    return err;
}

_AFX afxError _AfxMdleCtorCb(afxModule mdle, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &mdle);

    afxUri const* uri = AFX_CAST(afxUri const*, args[1]) + invokeNo;
    afxModuleFlags const* pFlags = AFX_CAST(afxModuleFlags const*, args[2]) + invokeNo;
    void* osHandle = AFX_CAST(void**, args[3])[invokeNo];

    AfxDeployChain(&mdle->classes, mdle);
    AfxDeployChain(&mdle->devices, mdle);
    mdle->attached = FALSE;
    mdle->etc = (afxManifest) { 0 };

    AfxResetStrings(1, &mdle->vendor);
    AfxResetStrings(1, &mdle->description);
    AfxResetStrings(1, &mdle->product);

    AfxMakeUri128(&mdle->path, uri);
    mdle->osHandle = osHandle;
    mdle->demangle = TRUE;
    mdle->flags = afxModuleFlag_DLL;

    // ICD stuff

    {
        AfxZero(&mdle->icd, sizeof(mdle->icd));

        AfxPushLink(&mdle->icd.avx, NIL);
        AfxPushLink(&mdle->icd.amx, NIL);
        AfxPushLink(&mdle->icd.asx, NIL);
        AfxPushLink(&mdle->icd.aux, NIL);

        afxError(*scmHookFn)(afxModule, afxUri const*) = AfxFindModuleSymbol(mdle, "afxScmHook");

        if (scmHookFn)
        {
            mdle->icd.scmHookFn = scmHookFn;
            mdle->flags |= afxModuleFlag_SYS;
        }

        afxError(*icdHookFn)(afxModule, afxUri const*) = AfxFindModuleSymbol(mdle, "afxIcdHook");

        if (icdHookFn)
        {
            mdle->icd.icdHookFn = icdHookFn;
            mdle->flags |= afxModuleFlag_ICD;
        }

        if (err)
        {
            AfxDeregisterChainedClasses(&mdle->classes);
            AfxPopLink(&mdle->icd.avx);
            AfxPopLink(&mdle->icd.amx);
            AfxPopLink(&mdle->icd.asx);
            AfxPopLink(&mdle->icd.aux);
        }
    }
    return err;
}

_AFX afxClassConfig const _AFX_MDLE_CLASS_CONFIG =
{
    .fcc = afxFcc_MDLE,
    .name = "Module",
    .desc = "Executable/Linkable Module",
    .fixedSiz = sizeof(AFX_OBJECT(afxModule)),
    .ctor = (void*)_AfxMdleCtorCb,
    .dtor = (void*)_AfxMdleDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AFX afxUnit AfxInvokeModules(afxUnit first, afxUnit cnt, afxBool(*f)(afxModule, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(cnt);
    AFX_ASSERT(f);
    afxClass* cls = AfxGetModuleClass();
    AFX_ASSERT_CLASS(cls, afxFcc_MDLE);
    return cnt ? AfxInvokeObjects(cls, first, cnt, (void*)f, udd) : 0;
}

_AFX afxUnit AfxEnumerateModules(afxUnit first, afxUnit cnt, afxModule executables[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(cnt);
    AFX_ASSERT(executables);
    afxClass* cls = AfxGetModuleClass();
    AFX_ASSERT_CLASS(cls, afxFcc_MDLE);
    return cnt ? AfxEnumerateObjects(cls, first, cnt, (afxObject*)executables) : 0;
}

_AFX afxBool AfxFindModule(afxUri const* uri, afxModule* module)
{
    afxError err = AFX_ERR_NONE;
    afxBool found = FALSE;
    AFX_ASSERT(uri);
    
#if AFX_OS_WIN
    afxUri fname;
    AfxClipUriFile(&fname, uri);
    afxUri2048 uriz;
    AfxMakeUri2048(&uriz, &fname); // needed to null-terminate a URI string.
    HMODULE hMod = GetModuleHandleA(AfxGetUriData(&uriz.uri, 0));

    afxUnit i = 0;
    afxModule mdle;
    while (AfxEnumerateModules(i, 1, &mdle))
    {
        AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &mdle);

        if (hMod == AfxGetModuleIdd(mdle))
        {
            found = TRUE;
            break;
        }
        ++i;
    }
#else
    afxUnit i = 0;
    afxModule mdle;
    while (AfxEnumerateModules(i, 1, &mdle))
    {
        AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &mdle);

        if (AfxCompareStrings(&uri->str, FALSE, 1, &mdle->path.uri.str))
        {
            found = TRUE;
            break;
        }
        ++i;
    }
#endif

    AFX_ASSERT(module);
    *module = found ? mdle : NIL;
    return found;
}

_AFX afxBool _AfxFindAndLoadDllCb(void* udd, afxUnit diskId, afxUnit endpointIdx, afxUri const* path, afxUri const* osPath)
{
    afxError err = AFX_ERR_NONE;
    afxBool next = TRUE;
#ifdef AFX_OS_WIN
    HMODULE hMod = LoadLibraryExA(AfxGetUriData(osPath, 0), NIL, NIL);
#else
    void* hMod = dlopen(AfxGetUriData(&libpath.uri, 0), RTLD_NOW);
#endif

    if (hMod)
    {
        void** pVar = ((void**)udd)[0];
        afxUri* url = ((void**)udd)[1];
        afxUri* resolvedUrl = ((void**)udd)[2];

        AfxCopyUri(resolvedUrl, osPath);
        AfxCopyUri(url, path);

        AFX_ASSERT(pVar);
        *pVar = (void*)hMod;
        next = FALSE; // break
    }
    return next;
}

_AFX afxError AfxLoadModule(afxUri const* uri, afxFlags flags, afxModule* module)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(uri);
    if (!uri || AfxIsUriBlank(uri))
    {
        AfxThrowError();
        return err;
    }
    
    afxUri2048 urib;
    AfxMakeUri2048(&urib, NIL);

    // add the default filename extension if it is not specified.

    if (uri->fext)
        AfxCopyUri(&urib.uri, uri);
    else
        AfxFormatUri(&urib.uri, "%.*s.dll", AfxPushString(&uri->str));

    AfxLogEcho("Loading... <%.*s>", AfxPushString(uri ? AfxGetUriString(&urib.uri) : &AFX_STRING_EMPTY));

    // find for the module file.

#ifdef AFX_OS_WIN
    HMODULE hMod = NIL;
#else
    void* hMod = NIL;
#endif

    afxUri2048 osPath, path;
    AfxMakeUri2048(&path, NIL);
    AfxMakeUri2048(&osPath, NIL);

    if (!AfxFindFiles(&urib.uri, afxFileFlag_R, _AfxFindAndLoadDllCb, (void*[]) { &hMod, &path.uri, &osPath.uri }))
    {
        AfxThrowError();
        return err;
    }
    
    // Try to reacquire a already existing module object for such module.

    afxUnit i = 0;
    afxModule mdle;
    while (AfxEnumerateModules(i, 1, &mdle))
    {
        AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &mdle);

        if (hMod == AfxGetModuleIdd(mdle))
        {
            AfxReacquireObjects(1, &mdle);
            AFX_ASSERT(module);
            *module = mdle;
            return err;
        }
        ++i;
    }

    // acquire a module object.

    afxClass* cls = AfxGetModuleClass();
    AFX_ASSERT_CLASS(cls, afxFcc_MDLE);

    if (AfxAcquireObjects(cls, 1, (afxObject*)&mdle, (void const*[]) { NIL, &path.uri, &flags, &hMod }))
    {
        AfxThrowError();
        FreeLibrary(hMod);
        return err;
    }
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &mdle);

    if (flags & AFX_BIT(8))
    {
        mdle->flags |= afxModuleFlag_SYS;
    }

    if (mdle->icd.icdHookFn)
    {
        // We must call it out of object acquisition to avoid deadlocks when a module object try to load another module.

        afxUri dev;
        AfxExcerptPathSegments(&mdle->path.uri, NIL, NIL, &dev, NIL);

        AfxLogEcho("Installing... <%.*s>", AfxPushString(&dev.str));

        if (mdle->icd.icdHookFn(mdle, NIL))
        {
            AfxThrowError();
            AfxDisposeObjects(1, &mdle);
            return err;
        }
        else
        {
            afxString128 s, devDesc, providerName, providerUrl;
            AfxMakeString128(&s, NIL);
            AfxMakeString128(&devDesc, NIL);
            AfxMakeString128(&providerName, NIL);
            AfxMakeString128(&providerUrl, NIL);
            afxUnit verMajor, verMinor, verPatch;

            AfxGetModuleVersion(mdle, &verMajor, &verMinor, &verPatch);
            AfxGetModuleInfo(mdle, &s.str, &providerName.str, &devDesc.str);

#define AFX_ITERATE_CHAIN_(ch_, type_, offset_, lnk_) \
    for (afxLink const* _curr##lnk_ = (ch_)->anchor.next, *_next##lnk_ = (afxLink*)NIL; \
    (lnk_ = (type_*)AFX_REBASE(_curr##lnk_, type_, offset_)), (_next##lnk_ = (_curr##lnk_)->next), ((_curr##lnk_) != &(ch_)->anchor); \
    (_curr##lnk_ = _next##lnk_))

#define AFX_ITERATE_CHAIN_B2F_(ch_, type_, offset_, lnk_) \
    for (afxLink const *_last##lnk_ = (afxLink *)NIL, *_curr##lnk_ = (ch_)->anchor.prev; \
         (_curr##lnk_) != &(ch_)->anchor; \
         (_last##lnk_ = (_curr##lnk_)->prev), \
         (lnk_ = (type_ *)AFX_REBASE(_curr##lnk_, type_, offset_)), \
         (_curr##lnk_ = _last##lnk_))


            afxDevice dev;
            AFX_ITERATE_CHAIN_(&mdle->devices, AFX_OBJ(afxDevice), icd, dev)
            {
                AFX_ASSERT_OBJECTS(afxFcc_DEV, 1, &dev);

                AfxLogY("\t<//./%.*s/>(\"%.*s\") %.*s %u.%u.%u %.*s",
                    AfxPushString(&dev->urn.uri.str), AfxPushString(&s.str), AfxPushString(&devDesc.str), verMajor, verMinor, verPatch, AfxPushString(&providerName.str));
                break;
            };

            mdle->attached = TRUE;
        }
    }

    if (mdle->icd.scmHookFn)
    {
        mdle->icd.scmHookFn(mdle, NIL);
    }

    AFX_ASSERT(module);
    *module = mdle;
    
    return err;
}

_AFX afxError _AfxAttachDriver(afxModule mdle, afxUri const* manifest, void* udd)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &mdle);

    if (!mdle->attached)
    {
        AfxLogEcho("Installing... <%.*s>", AfxPushString(&mdle->path.uri.str));

        if (!mdle->icd.icdHookFn) AfxThrowError();
        else
        {
            afxModuleFlags mdleType = mdle->flags;
            mdle->flags = afxModuleFlag_ICD;

            if (mdle->icd.icdHookFn(mdle, manifest)) AfxThrowError();
            else
            {
                afxString128 s, devDesc, providerName, providerUrl;
                AfxMakeString128(&s, NIL);
                AfxMakeString128(&devDesc, NIL);
                AfxMakeString128(&providerName, NIL);
                AfxMakeString128(&providerUrl, NIL);
                afxUnit verMajor, verMinor, verPatch;

                AfxGetModuleVersion(mdle, &verMajor, &verMinor, &verPatch);
                AfxGetModuleInfo(mdle, &s.str, &providerName.str, &devDesc.str);

                afxDevice dev;
                AFX_ITERATE_CHAIN_B2F(&mdle->devices, AFX_OBJ(afxDevice), icd, dev)
                {
                    AFX_ASSERT_OBJECTS(afxFcc_DEV, 1, &dev);

                    AfxLogY("\t<//./%.*s/>(\"%.*s\") %.*s %u.%u.%u %.*s",
                        AfxPushString(&dev->urn.uri.str), AfxPushString(&s.str), AfxPushString(&devDesc.str), verMajor, verMinor, verPatch, AfxPushString(&providerName.str));
                };

                mdle->attached = TRUE;
            }

            if (err)
                mdle->flags = mdleType;
        }
    }
    return err;
}
