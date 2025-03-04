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

afxByte _afxMdleData[sizeof(AFX_OBJ(afxModule)) + sizeof(afxObjectBase)] = { NIL };
afxByte _avxMdleData[sizeof(AFX_OBJ(afxModule)) + sizeof(afxObjectBase)] = { NIL };
afxByte _amxMdleData[sizeof(AFX_OBJ(afxModule)) + sizeof(afxObjectBase)] = { NIL };
afxByte _auxMdleData[sizeof(AFX_OBJ(afxModule)) + sizeof(afxObjectBase)] = { NIL };
afxByte _asxMdleData[sizeof(AFX_OBJ(afxModule)) + sizeof(afxObjectBase)] = { NIL };
_AFX afxModule TheAfxMdl = { NIL };
_AFX afxModule TheAvxMdl = { NIL };
_AFX afxModule TheAmxMdl = { NIL };
_AFX afxModule TheAuxMdl = { NIL };
_AFX afxModule TheAsxMdl = { NIL };

_AFX afxError _AfxAttachCoreModule(afxModule* e2coree)
{
    afxError err = NIL;

    if (TheAfxMdl)
    {
        AfxThrowError();
        return err;
    }

    afxSystem sys;
    AfxGetSystem(&sys);

    afxUri* name;
    name = AfxUri("e2coree.dll");

    afxClass* cls = AfxGetModuleClass();

    TheAfxMdl = (void*)_afxMdleData;
    if (_AfxConstructObjects(cls, 1, (afxObject*)&TheAfxMdl, (void*[]) { sys, name, name })) AfxThrowError();
    else
    {
        AFX_ASSERT(TheAfxMdl != (void*)_afxMdleData);
        AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &TheAfxMdl);

        if (err)
        {
            if (_AfxDestructObjects(cls, 1, (afxObject*)&TheAfxMdl))
                AfxThrowError();

            AFX_ASSERT(TheAfxMdl == (void*)_afxMdleData);
        }
    }

    AFX_ASSERT(e2coree);
    *e2coree = err ? NIL : TheAfxMdl;
    return err;
}

_AFX afxError _AfxAttachTargaModule(afxModule* e2targa)
{
    afxError err = NIL;

    if (TheAvxMdl)
    {
        AfxThrowError();
        return err;
    }

    afxSystem sys;
    AfxGetSystem(&sys);

    afxUri* name;
    name = AfxUri("e2targa.dll");

    afxClass* cls = AfxGetModuleClass();

    TheAvxMdl = (void*)_avxMdleData;
    if (_AfxConstructObjects(cls, 1, (afxObject*)&TheAvxMdl, (void*[]) { sys, name, name })) AfxThrowError();
    else
    {
        AFX_ASSERT(TheAvxMdl != (void*)_avxMdleData);
        AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &TheAvxMdl);

        if (TheAvxMdl->icd.scmHookFn(TheAvxMdl, name))
            AfxThrowError();

        if (err)
        {
            if (_AfxDestructObjects(cls, 1, (afxObject*)&TheAvxMdl))
                AfxThrowError();

            AFX_ASSERT(TheAvxMdl == (void*)_avxMdleData);
        }
    }

    AFX_ASSERT(e2targa);
    *e2targa = err ? NIL : TheAvxMdl;
    return err;
}

_AFX afxError _AfxAttachMixModule(afxModule* e2mixva)
{
    afxError err = NIL;

    if (TheAmxMdl)
    {
        AfxThrowError();
        return err;
    }

    afxSystem sys;
    AfxGetSystem(&sys);

    afxUri* name;
    name = AfxUri("e2mixva.dll");

    afxClass* cls = AfxGetModuleClass();

    TheAmxMdl = (void*)_amxMdleData;
    if (_AfxConstructObjects(cls, 1, (afxObject*)&TheAmxMdl, (void*[]) { sys, name, name })) AfxThrowError();
    else
    {
        AFX_ASSERT(TheAmxMdl != (void*)_amxMdleData);
        AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &TheAmxMdl);

        if (TheAmxMdl->icd.scmHookFn(TheAmxMdl, name))
            AfxThrowError();

        if (err)
        {
            if (_AfxDestructObjects(cls, 1, (afxObject*)&TheAmxMdl))
                AfxThrowError();

            AFX_ASSERT(TheAmxMdl == (void*)_amxMdleData);
        }
    }

    AFX_ASSERT(e2mixva);
    *e2mixva = err ? NIL : TheAmxMdl;
    return err;
}

_AFX afxError _AfxAttachUxModule(afxModule* e2mmux)
{
    afxError err = NIL;

    if (TheAuxMdl)
    {
        AfxThrowError();
        return err;
    }

    afxSystem sys;
    AfxGetSystem(&sys);

    afxUri* name;
    name = AfxUri("e2mmux.dll");

    afxClass* cls = AfxGetModuleClass();

    TheAuxMdl = (void*)_auxMdleData;
    if (_AfxConstructObjects(cls, 1, (afxObject*)&TheAuxMdl, (void*[]) { sys, name, name })) AfxThrowError();
    else
    {
        AFX_ASSERT(TheAuxMdl != (void*)_auxMdleData);
        AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &TheAuxMdl);

        if (err)
        {
            if (_AfxDestructObjects(cls, 1, (afxObject*)&TheAuxMdl))
                AfxThrowError();

            AFX_ASSERT(TheAuxMdl == (void*)_auxMdleData);
        }
    }

    AFX_ASSERT(e2mmux);
    *e2mmux = err ? NIL : TheAuxMdl;
    return err;
}

_AFX afxError _AfxAttachSimModule(afxModule* e2synerg)
{
    afxError err = NIL;

    if (TheAsxMdl)
    {
        AfxThrowError();
        return err;
    }

    afxSystem sys;
    AfxGetSystem(&sys);

    afxUri* name;
    name = AfxUri("e2synerg.dll");

    afxClass* cls = AfxGetModuleClass();

    TheAsxMdl = (void*)_asxMdleData;
    if (_AfxConstructObjects(cls, 1, (afxObject*)&TheAsxMdl, (void*[]) { sys, name, name })) AfxThrowError();
    else
    {
        AFX_ASSERT(TheAsxMdl != (void*)_asxMdleData);
        AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &TheAsxMdl);

        if (err)
        {
            if (_AfxDestructObjects(cls, 1, (afxObject*)&TheAsxMdl))
                AfxThrowError();

            AFX_ASSERT(TheAsxMdl == (void*)_asxMdleData);
        }
    }

    AFX_ASSERT(e2synerg);
    *e2synerg = err ? NIL : TheAsxMdl;
    return err;
}
