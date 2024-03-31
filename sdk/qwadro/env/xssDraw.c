
#define _AFX_CORE_C
#define _AFX_SYSTEM_C
#include "qwadro/env/afxEnvironment.h"
#include "qwadro/draw/afxDrawSystem.h"
#include "qwadro/../_dep/luna.h"

// CAM /////////////////////////////////////////////////////////////////////////

void _XsCamReset(afxEnvironment env)
{
    afxCamera cam = XssPullInstance(env, 0);
    AfxResetCamera(cam);
}

void _XsCamApplyMotion(afxEnvironment env)
{
    afxCamera cam = XssPullInstance(env, 0);
    afxV3d v;
    XssPullReal3(env, 1, v);
    AfxApplyCameraMotion(cam, v);
}

void _XsCamApplyOrientation(afxEnvironment env)
{
    afxCamera cam = XssPullInstance(env, 0);
    afxV3d v;
    XssPullReal3(env, 1, v);
    AfxApplyCameraElevAzimRoll(cam, v);
}

void _XsCamApplyOffset(afxEnvironment env)
{
    afxCamera cam = XssPullInstance(env, 0);
    afxV3d v;
    XssPullReal3(env, 1, v);
    AfxApplyCameraOffset(cam, v);
}

void _XsCamApplyDistance(afxEnvironment env)
{
    afxCamera cam = XssPullInstance(env, 0);
    AfxApplyCameraDistance(cam, XssPullReal(env, 1));
}

void _XsCamGet(afxEnvironment env)
{
    afxNat id = XssPullNat(env, 1);
    afxCamera cam = AfxGetObjectAt(AfxGetCameraClass(), id);
    XssPushInstance(env, 0, cam);
}

void _XsCamAcquire(afxEnvironment env)
{
    afxUri uri;
    afxDrawInput din = XssPullInstance(env, 1);
    afxV3d v;
    XssPullReal3(env, 2, v);
    afxCamera cam;
    AfxAcquireCameras(din, 1, &cam);
    AfxApplyCameraMotion(cam, v);
    XssPushInstance(env, 0, cam);
}

afxString const camVmtNames[] =
{
    AFX_STRING("Acquire(_,_,_,_)"),
    AFX_STRING("Get(_)"),

    AFX_STRING("Reset()"),

    AFX_STRING("ApplyMotion(_,_,_)"),
    AFX_STRING("ApplyOrientation(_,_,_)"),
    AFX_STRING("ApplyOffset(_,_,_)"),
    AFX_STRING("ApplyDistance(_)"),
};

void const* camVmtPtrs[] =
{
    _XsCamAcquire,
    _XsCamGet,

    _XsCamReset,

    _XsCamApplyMotion,
    _XsCamApplyOrientation,
    _XsCamApplyOffset,
    _XsCamApplyDistance
};

////////////////////////////////////////////////////////////////////////////////

afxString const ffiClasses[] =
{
    AFX_STRING("Camera")
};

struct
{
    afxNat          cnt;
    afxString const*names;
    void const**    ptrs;
}
const ffiSym[] =
{
    {
        AFX_COUNTOF(camVmtNames),
        camVmtNames,
        camVmtPtrs,
    }
};

_AVX afxBool GetFfiClass(afxEnvironment env, afxString const* class, LunaForeignClassMethods* methods)
{
    afxBool rslt = FALSE;
    afxNat clsCnt = AFX_COUNTOF(ffiClasses);

    for (afxNat i = 0; i < clsCnt; i++)
    {
        if (0 == AfxCompareString(class, &ffiClasses[i]))
        {
            methods->allocate = GenAcqObj;
            methods->finalize = GenRelObj;
            rslt = TRUE;
            break;
        }
    }
    return rslt;
}

_AVX void* GetFfiMethod(afxEnvironment env, afxString const* class, afxString const* signature, afxBool firm)
{
    void* sym = NIL;
    afxNat clsCnt = AFX_COUNTOF(ffiClasses);

    for (afxNat i = 0; i < clsCnt; i++)
    {
        if (0 == AfxCompareString(class, &ffiClasses[i]))
        {
            for (afxNat j = 0; j < ffiSym[i].cnt; j++)
            {
                if (0 == AfxCompareString(signature, &ffiSym[i].names[j]))
                {
                    sym = (void*)ffiSym[i].ptrs[j];
                    break;
                }
            }
            break;
        }
    }
    return sym;
}
