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

afxByte _e2coreeMdlObj[sizeof(AFX_OBJ(afxModule)) + sizeof(afxObjectBase)] = { NIL };
_AFX afxModule _e2coreeMdl = { NIL };

_AFX afxError _AfxAttachCoreModule(afxModule* e2coree)
{
    afxError err = NIL;

    if (_e2coreeMdl)
    {
        AfxThrowError();
        return err;
    }

    afxSystem sys;
    AfxGetSystem(&sys);

    afxUri* name;
    name = AfxUri("e2coree.dll");

    afxClass* cls = _AfxGetModuleClass();

    _e2coreeMdl = (void*)_e2coreeMdlObj;
    if (_AfxConstructObjects(cls, 1, (afxObject*)&_e2coreeMdl, (void*[]) { sys, name, name })) AfxThrowError();
    else
    {
        AFX_ASSERT(_e2coreeMdl != (void*)_e2coreeMdlObj);
        AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &_e2coreeMdl);

        if (err)
        {
            if (_AfxDestructObjects(cls, 1, (afxObject*)&_e2coreeMdl))
                AfxThrowError();

            AFX_ASSERT(_e2coreeMdl == (void*)_e2coreeMdlObj);
        }
    }

    AFX_ASSERT(e2coree);
    *e2coree = err ? NIL : _e2coreeMdl;
    return err;
}
