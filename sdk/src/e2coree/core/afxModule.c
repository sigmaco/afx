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
#define _AFX_MODULE_C
#include "qwadro/core/afxClass.h"
#include "qwadro/core/afxSystem.h"

_AFX afxResult AfxFindModuleSymbols(afxModule mdle, afxNat cnt, afxChar const *name[], void *sym[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdle, afxFcc_MDLE);

    AfxAssert(cnt);
    AfxAssert(name);
    afxResult hitCnt = 0;

    for (afxNat i = 0; i < cnt; i++)
    {
        if ((sym[i] = (void*)GetProcAddress(mdle->osHandle, name[i]))) hitCnt++;
        else
        {
            //AfxError("Symbol %s not found.", name[i]);
        }
    }
    return hitCnt;
}

_AFX void* AfxFindModuleSymbol(afxModule mdle, afxChar const *name)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdle, afxFcc_MDLE);

    AfxAssert(name);
    return (void*)GetProcAddress(mdle->osHandle, name);
}

_AFX afxUri const* AfxGetModulePath(afxModule mdle)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdle, afxFcc_MDLE);
    return &mdle->path.uri;
}

_AFX afxError _AfxMdleCtor(afxModule mdle, afxCookie const* cookie)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdle, afxFcc_MDLE);

    afxUri const* uri = ((afxUri const*)cookie->udd[1]) + cookie->no;

    AfxEntry("uri:%.*s", AfxPushString(uri ? AfxGetUriString(uri) : &AFX_STR_EMPTY));

    afxBool isEmpty = !uri || AfxUriIsBlank(uri);
    AfxAssert(!isEmpty);
    afxResult success = FALSE;
    afxResult loaded = FALSE;
    
    afxFixedUri128 absUri;
    AfxMakeFixedUri128(&absUri, NIL);

    AfxResolveUri(afxFileFlag_R, uri, &absUri.uri);

    //afxString1024 str;
    //AfxString1024(&str, 0, 0);
    afxUri path;
    //AfxCopyString(AfxGetUriPath(&absUri, &tmp), &str.str);
    AfxGetUriPath(&path, &absUri.uri);
    afxChar const *pathStr = AfxGetUriData(&path, 0);

    HMODULE oshandle;// = GetModuleHandleA(AfxGetStringData(pathStr, 0));

    //if (!oshandle)
    {
        if (!(loaded = !!(oshandle = LoadLibraryA(pathStr))))
            AfxThrowError();
    }

    if (oshandle)
    {
        success = TRUE;
        AfxMakeFixedUri128(&mdle->path, uri);
        mdle->osHandle = oshandle;
        mdle->hasBeenLoaded = loaded;

        if (FALSE == success)
        {
            if (loaded)
                FreeLibrary(oshandle);
        }
    }
    return err;
}

_AFX afxError _AfxMdleDtor(afxModule mdle)
{
    AfxEntry("mdle=%p", mdle);
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdle, afxFcc_MDLE);

    //if (mdle->osHandle != AfxApplicationGet()->e2coree->osHandle)

    if (mdle->hasBeenLoaded)
        FreeLibrary(mdle->osHandle);

    return err;
}

_AFX afxClassConfig const _AfxMdleClsConfig =
{
    .fcc = afxFcc_MDLE,
    .name = "Executable & Linkable Module",
    .unitsPerPage = 2,
    .size = sizeof(AFX_OBJECT(afxModule)),
    .mmu = NIL,
    .ctor = (void*)_AfxMdleCtor,
    .dtor = (void*)_AfxMdleDtor
};

////////////////////////////////////////////////////////////////////////////////

_AFX afxError AfxMountLoadableModules(afxNat cnt, afxUri const uri[], afxModule mdle[])
{
    AfxEntry("cnt=%u,mdle=%p,uri=%p", cnt, mdle, uri);
    afxError err = AFX_ERR_NONE;

    afxClass* cls = AfxGetModuleClass();
    AfxAssertClass(cls, afxFcc_MDLE);

    for (afxNat i = 0; i < cnt; i++)
    {
        afxModule tmp = AfxFindModule(&uri[i]);

        if (tmp)
        {
            if (AfxReacquireObjects(1, (afxObject*)&tmp)) AfxThrowError();
            else
            {
                mdle[i] = tmp;
            }
        }
        else
        {
            if (AfxAcquireObjects(cls, 1, (afxObject*)&mdle[i], (void const*[]) { NIL, &uri[i] }))
                AfxThrowError();
        }

        if (err)
        {
            AfxReleaseObjects(i, (afxObject*)mdle);
            break;
        }
    }
    return err;
}

_AFX afxNat AfxInvokeModules(afxNat first, afxNat cnt, afxBool(*f)(afxModule, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(f);
    afxClass* cls = AfxGetModuleClass();
    AfxAssertClass(cls, afxFcc_MDLE);
    return cnt ? AfxInvokeInstances(cls, first, cnt, (void*)f, udd) : 0;
}

_AFX afxNat AfxEnumerateModules(afxNat first, afxNat cnt, afxModule mdle[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(mdle);
    afxClass* cls = AfxGetModuleClass();
    AfxAssertClass(cls, afxFcc_MDLE);
    return cnt ? AfxEnumerateInstances(cls, first, cnt, (afxObject*)mdle) : 0;
}

_AFX afxNat AfxCountModules(void)
{
    afxError err = AFX_ERR_NONE;
    afxClass* cls = AfxGetModuleClass();
    AfxAssertClass(cls, afxFcc_MDLE);
    return AfxCountInstances(cls);
}

_AFX afxModule AfxFindModule(afxUri const *uri)
{
    afxError err = AFX_ERR_NONE;
    
    afxNat i = 0;
    afxModule mdle;
    while (AfxEnumerateModules(i, 1, &mdle))
    {
        AfxAssertObjects(1, &mdle, afxFcc_MDLE);

        if (0 == AfxCompareUri(AfxGetModulePath(mdle), uri))
            return mdle;

        ++i;
    }
    return NIL;
}
