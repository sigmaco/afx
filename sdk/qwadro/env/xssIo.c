

#define _AFX_CORE_C
#define _AFX_SYSTEM_C
#include "qwadro/../_dep/luna.h"
#include "qwadro/core/afxSystem.h"

_AFXINL void AfxGetSlots(afxNat first, afxNat cnt, ...)
{
    va_list va;
    va_start(va, cnt);
    va_arg(va, int);
    va_end(va);
}

// FSYS ////////////////////////////////////////////////////////////////////////

void _XsFsysMount(afxEnvironment env)
{
    //AfxGetSlots(1, 3, );

    //AfxPullFromEnv(env, 0, 3, );

    afxString mode;
    afxUri point, endpoint;
    XssPullUri(env, 1, &point);
    XssPullUri(env, 2, &endpoint);
    XssPullString(env, 3, &mode);

    //afxChar mode[9] = { NIL }, *modePtr = mode;
    afxFileFlags ioFlags = NIL;

    if (AFX_INVALID_INDEX != AfxFindFirstChar(&mode, 0, 'r'))
    {
        ioFlags |= afxFileFlag_R;
        //*modePtr++ = 'r';
    }

    if (AFX_INVALID_INDEX != AfxFindFirstChar(&mode, 0, 'w'))
    {
        ioFlags |= afxFileFlag_W;
        //*modePtr++ = 'w';
    }
    
    if (AFX_INVALID_INDEX != AfxFindFirstChar(&mode, 0, 'x'))
    {
        ioFlags |= afxFileFlag_X;
        //*modePtr++ = 'x';
    }

    afxError err = AfxMountStorageUnit(&point, &endpoint, ioFlags);
    XssPushError(env, 0, err);
}

void _XsFsysDismount(afxEnvironment env)
{
    //AfxGetSlots(1, 3, );

    afxString mode;
    afxUri point, endpoint;
    XssPullUri(env, 1, &point);
    XssPullUri(env, 2, &endpoint);
    XssPullString(env, 3, &mode);

    //afxChar mode[9] = { NIL }, *modePtr = mode;
    afxFileFlags ioFlags = NIL;

    if (AFX_INVALID_INDEX != AfxFindFirstChar(&mode, 0, 'r'))
    {
        ioFlags |= afxFileFlag_R;
        //*modePtr++ = 'r';
    }

    if (AFX_INVALID_INDEX != AfxFindFirstChar(&mode, 0, 'w'))
    {
        ioFlags |= afxFileFlag_W;
        //*modePtr++ = 'w';
    }

    if (AFX_INVALID_INDEX != AfxFindFirstChar(&mode, 0, 'x'))
    {
        ioFlags |= afxFileFlag_X;
        //*modePtr++ = 'x';
    }

    afxError err = AfxDismountStorageUnit(&point, &endpoint, ioFlags);
    XssPushError(env, 0, err);
}

afxString const fsysVmtNames[] =
{
    AFX_STRING("Mount(_,_,_)"),
    AFX_STRING("Dismount(_,_,_)")
};

void* const fsysVmtPtrs[] =
{
    _XsFsysMount,
    _XsFsysDismount
};

// IOB /////////////////////////////////////////////////////////////////////////

void _XsIobRead(afxEnvironment env)
{
    int dstCap;
    afxStream in = XssPullInstance(env, 0);
    void* dst = (void*)lunaGetSlotBytes(env, 1, &dstCap);
    afxNat range = XssPullNat(env, 2);
    afxNat rate = XssPullNat(env, 3);
    afxError err = AfxReadStream(in, range, rate, dst);
    XssPushError(env, 0, err);
}

void _XsIobWrite(afxEnvironment env)
{
    int srcLen;
    afxStream out = XssPullInstance(env, 0);
    void const* src = lunaGetSlotBytes(env, 1, &srcLen);
    afxNat range = XssPullNat(env, 2);
    afxNat rate = XssPullNat(env, 3);
    afxError err = AfxWriteStream(out, range, rate, src);
    XssPushError(env, 0, err);
}

void _XsIobSeek(afxEnvironment env)
{
    afxStream iob = XssPullInstance(env, 0);
    afxInt offset = XssPullInt(env, 1);
    afxSeekMode origin = XssPullInt(env, 2);
    afxError err = AfxSeekStream(iob, offset, origin);
    XssPushError(env, 0, err);
}

void _XsIobClose(afxEnvironment env)
{
    AfxCloseStream(XssPullInstance(env, 0));
}

void _XsIobReloadFile(afxEnvironment env)
{
    afxStream iob = XssPullInstance(env, 0);
    afxUri uri;
    XssPullUri(env, 1, &uri);
    afxError err = AfxReloadFile(iob, &uri);
    XssPushError(env, 0, err);
}

void _XsIobLoadFile(afxEnvironment env)
{
    afxUri uri;
    XssPullUri(env, 1, &uri);
    afxStream file = AfxLoadFile(&uri);
    XssPushInstance(env, 0, &file);
}

void _XsIobOpenFile(afxEnvironment env)
{
    afxUri uri;
    XssPullUri(env, 1, &uri);
    afxString mode;
    XssPullString(env, 2, &mode);

    afxIoFlags ioFlags = NIL;

    if (AFX_INVALID_INDEX != AfxFindFirstChar(&mode, 0, 'r'))
    {
        ioFlags |= afxFileFlag_R;
        //*modePtr++ = 'r';
    }

    if (AFX_INVALID_INDEX != AfxFindFirstChar(&mode, 0, 'w'))
    {
        ioFlags |= afxFileFlag_W;
        //*modePtr++ = 'w';
    }

    if (AFX_INVALID_INDEX != AfxFindFirstChar(&mode, 0, 'x'))
    {
        ioFlags |= afxFileFlag_X;
        //*modePtr++ = 'x';
    }


    afxStream file= AfxOpenFile(&uri, ioFlags);
    XssPushInstance(env, 0, file);
}

void _XsIobAcquire(afxEnvironment env)
{
    afxString mode;
    XssPullString(env, 1, &mode);
    afxSize siz = XssPullSize(env, 2);

    afxIoFlags ioFlags = NIL;
    
    if (AfxFindFirstChar(&mode, 0, 'r'))
    {
        ioFlags |= afxIoFlag_R;
        //*modePtr++ = 'r';
    }

    if (AfxFindFirstChar(&mode, 0, 'w'))
    {
        ioFlags |= afxIoFlag_W;
        //*modePtr++ = 'w';
    }

    if (AfxFindFirstChar(&mode, 0, 'x'))
    {
        ioFlags |= afxIoFlag_X;
        //*modePtr++ = 'x';
    }

    afxStream file = AfxAcquireStream(ioFlags, siz);
    XssPushInstance(env, 0, file);
}

void _XsIobOpen(afxEnvironment env)
{
    afxString mode;
    XssPullString(env, 1, &mode);
    int dataLen;
    void* start = (void*)lunaGetSlotBytes(env, 2, &dataLen);
    afxSize siz = XssPullSize(env, 3);

    afxIoFlags ioFlags = NIL;
    if (AfxFindFirstChar(&mode, 0, 'r'))
    {
        ioFlags |= afxIoFlag_R;
        //*modePtr++ = 'r';
    }

    if (AfxFindFirstChar(&mode, 0, 'w'))
    {
        ioFlags |= afxIoFlag_W;
        //*modePtr++ = 'w';
    }
    
    
    if (AfxFindFirstChar(&mode, 0, 'x'))
    {
        ioFlags |= afxIoFlag_X;
        //*modePtr++ = 'x';
    }

    afxStream file = AfxOpenStream(ioFlags, start, siz);
    XssPushInstance(env, 0, file);
}

void _XsIobOpenInput(afxEnvironment env)
{
    int dataLen;
    void* data = (void*)lunaGetSlotBytes(env, 1, &dataLen);
    afxSize len = XssPullSize(env, 2);

    afxStream file = AfxOpenInputStream(data, len);
    XssPushInstance(env, 0, file);
}

void _XsIobOpenOutput(afxEnvironment env)
{
    afxSize cap = XssPullSize(env, 1);
    int dataLen;
    void* data = (void*)lunaGetSlotBytes(env, 2, &dataLen);

    afxStream file = AfxOpenOutputStream(data, cap);
    XssPushInstance(env, 0, file);
}

afxString const iobVmtNames[] =
{
    AFX_STRING("Acquire(_,_)"),
    AFX_STRING("Open(_,_,_)"),
    AFX_STRING("OpenInput(_,_)"),
    AFX_STRING("OpenOutput(_,_)"),
    AFX_STRING("OpenFile(_,_)"),
    AFX_STRING("LoadFile(_)"),

    AFX_STRING("ReloadFile(_)"),
    AFX_STRING("Close()"),

    AFX_STRING("Seek(_,_)"),
    AFX_STRING("Read(_,_,_)"),
    AFX_STRING("Write(_,_,_)")
};

void* const iobVmtPtrs[] =
{
    _XsIobAcquire,
    _XsIobOpen,
    _XsIobOpenInput,
    _XsIobOpenOutput,
    _XsIobOpenFile,
    _XsIobLoadFile,

    _XsIobReloadFile,
    _XsIobClose,

    _XsIobSeek,
    _XsIobRead,
    _XsIobWrite
};

// MSE /////////////////////////////////////////////////////////////////////////

void _XsMseReacquire(afxEnvironment env)
{
    afxNat port = XssPullNat(env, 1);
    afxMouse mse;
    AfxGetMouse(port, &mse);
    AfxReacquireObjects(1, (void*[]) { mse });
}

void _XsMseRelease(afxEnvironment env)
{
    afxNat port = XssPullNat(env, 1);
    afxMouse mse;
    AfxGetMouse(port, &mse);
    AfxReleaseObjects(1, (void*[]) { mse });
}

void _XsMseLmbIsPressed(afxEnvironment env)
{
    afxNat port = XssPullNat(env, 1);
    XssPushBool(env, 0, AfxLmbIsPressed(port));
}

void _XsMseRmbIsPressed(afxEnvironment env)
{
    afxNat port = XssPullNat(env, 1);
    XssPushBool(env, 0, AfxRmbIsPressed(port));
}

void _XsMseGetMotionX(afxEnvironment env)
{
    afxNat port = XssPullNat(env, 1);
    afxV2d delta;
    AfxGetMouseMotion(port, delta);
    XssPushReal(env, 0, delta[0]);
}

void _XsMseGetMotionY(afxEnvironment env)
{
    afxNat port = XssPullNat(env, 1);
    afxV2d delta;
    AfxGetMouseMotion(port, delta);
    XssPushReal(env, 0, delta[1]);
}

void _XsMseGetWheelDelta(afxEnvironment env)
{
    afxNat port = XssPullNat(env, 1);
    XssPushReal(env, 0, AfxGetMouseWheelDelta(port));
}

afxString const mseVmtNames[] =
{
    AFX_STRING("Release(_)"),
    AFX_STRING("Reacquire(_)"),

    AFX_STRING("LmbIsPressed(_)"),
    AFX_STRING("RmbIsPressed(_)"),

    AFX_STRING("GetMotionX(_)"),
    AFX_STRING("GetMotionY(_)"),
    AFX_STRING("GetWheelDelta(_)")

};

void* const mseVmtPtrs[] =
{
    _XsMseRelease,
    _XsMseReacquire,

    _XsMseLmbIsPressed,
    _XsMseRmbIsPressed,

    _XsMseGetMotionX,
    _XsMseGetMotionY,
    _XsMseGetWheelDelta
};

// KBD /////////////////////////////////////////////////////////////////////////

void _XsKbdReacquire(afxEnvironment env)
{
    afxNat port = XssPullNat(env, 1);
    afxKeyboard kbd;
    AfxGetKeyboard(port, &kbd);
    AfxReacquireObjects(1, (void*[]) { kbd });
}

void _XsKbdRelease(afxEnvironment env)
{
    afxNat port = XssPullNat(env, 1);
    afxKeyboard kbd;
    AfxGetKeyboard(port, &kbd);
    AfxReleaseObjects(1, (void*[]) { kbd });
}

void _XsKbdGetKeyPressure(afxEnvironment env)
{
    afxNat port = XssPullNat(env, 1);
    afxNat code = XssPullNat(env, 2);
    XssPushReal64(env, 0, AfxGetKeyPressure(port, code));
}

void _XsKbdGetKeyCombo(afxEnvironment env)
{
    afxNat port = XssPullNat(env, 1);
    afxNat code = XssPullNat(env, 2);
    afxNat codeB = XssPullNat(env, 3);
    XssPushReal64(env, 0, AfxGetCombinedKeyPressure(port, code, codeB));
}

afxString const kbdVmtNames[] =
{
    AFX_STRING("Release(_)"),
    AFX_STRING("Reacquire(_)"),

    AFX_STRING("GetKeyPressure(_,_)"),
    AFX_STRING("GetKeyCombo(_,_,_)")
};

void* const kbdVmtPtrs[] =
{
    _XsKbdRelease,
    _XsKbdReacquire,

    _XsKbdGetKeyPressure,
    _XsKbdGetKeyCombo
};

afxString const ffiClasses[] =
{
    AFX_STRING("Stream"),
    AFX_STRING("Storage"),
    AFX_STRING("Keyboard"),
    AFX_STRING("Mouse"),
};

struct
{
    afxNat          cnt;
    afxString const*names;
    void**          ptrs;
}
const ffiSym[] =
{
    {
        AFX_COUNTOF(iobVmtNames),
        iobVmtNames,
        iobVmtPtrs,
    },
    {
        AFX_COUNTOF(fsysVmtNames),
        fsysVmtNames,
        fsysVmtPtrs,
    },
    {
        AFX_COUNTOF(kbdVmtNames),
        kbdVmtNames,
        kbdVmtPtrs,
    },
    {
        AFX_COUNTOF(mseVmtNames),
        mseVmtNames,
        mseVmtPtrs,
    }
};

_AFX afxBool GetFfiClass(afxEnvironment env, afxString const* class, LunaForeignClassMethods* methods)
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

_AFX void* GetFfiMethod(afxEnvironment env, afxString const* class, afxString const* signature, afxBool firm)
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
                    sym = ffiSym[i].ptrs[j];
                    break;
                }
            }
            break;
        }
    }
    return sym;
}
