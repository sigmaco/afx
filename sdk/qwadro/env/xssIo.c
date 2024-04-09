

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

void _XsFsysMount(afxShell sh)
{
    //AfxGetSlots(1, 3, );

    //AfxPullFromEnv(sh, 0, 3, );

    afxString mode;
    afxUri point, endpoint;
    XssPullUri(sh, 1, &point);
    XssPullUri(sh, 2, &endpoint);
    XssPullString(sh, 3, &mode);

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
    XssPushError(sh, 0, err);
}

void _XsFsysDismount(afxShell sh)
{
    //AfxGetSlots(1, 3, );

    afxString mode;
    afxUri point, endpoint;
    XssPullUri(sh, 1, &point);
    XssPullUri(sh, 2, &endpoint);
    XssPullString(sh, 3, &mode);

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
    XssPushError(sh, 0, err);
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

void _XsIobRead(afxShell sh)
{
    int dstCap;
    afxStream in = XssPullInstance(sh, 0);
    void* dst = (void*)lunaGetSlotBytes(sh, 1, &dstCap);
    afxNat range = XssPullNat(sh, 2);
    afxNat rate = XssPullNat(sh, 3);
    afxError err = AfxReadStream(in, range, rate, dst);
    XssPushError(sh, 0, err);
}

void _XsIobWrite(afxShell sh)
{
    int srcLen;
    afxStream out = XssPullInstance(sh, 0);
    void const* src = lunaGetSlotBytes(sh, 1, &srcLen);
    afxNat range = XssPullNat(sh, 2);
    afxNat rate = XssPullNat(sh, 3);
    afxError err = AfxWriteStream(out, range, rate, src);
    XssPushError(sh, 0, err);
}

void _XsIobSeek(afxShell sh)
{
    afxStream iob = XssPullInstance(sh, 0);
    afxInt offset = XssPullInt(sh, 1);
    afxSeekMode origin = XssPullInt(sh, 2);
    afxError err = AfxSeekStream(iob, offset, origin);
    XssPushError(sh, 0, err);
}

void _XsIobClose(afxShell sh)
{
    AfxCloseStream(XssPullInstance(sh, 0));
}

void _XsIobReloadFile(afxShell sh)
{
    afxStream iob = XssPullInstance(sh, 0);
    afxUri uri;
    XssPullUri(sh, 1, &uri);
    afxError err = AfxReloadFile(iob, &uri);
    XssPushError(sh, 0, err);
}

void _XsIobLoadFile(afxShell sh)
{
    afxUri uri;
    XssPullUri(sh, 1, &uri);
    afxStream file = AfxLoadFile(&uri);
    XssPushInstance(sh, 0, &file);
}

void _XsIobOpenFile(afxShell sh)
{
    afxUri uri;
    XssPullUri(sh, 1, &uri);
    afxString mode;
    XssPullString(sh, 2, &mode);

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
    XssPushInstance(sh, 0, file);
}

void _XsIobAcquire(afxShell sh)
{
    afxString mode;
    XssPullString(sh, 1, &mode);
    afxSize siz = XssPullSize(sh, 2);

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
    XssPushInstance(sh, 0, file);
}

void _XsIobOpen(afxShell sh)
{
    afxString mode;
    XssPullString(sh, 1, &mode);
    int dataLen;
    void* start = (void*)lunaGetSlotBytes(sh, 2, &dataLen);
    afxSize siz = XssPullSize(sh, 3);

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
    XssPushInstance(sh, 0, file);
}

void _XsIobOpenInput(afxShell sh)
{
    int dataLen;
    void* data = (void*)lunaGetSlotBytes(sh, 1, &dataLen);
    afxSize len = XssPullSize(sh, 2);

    afxStream file = AfxOpenInputStream(data, len);
    XssPushInstance(sh, 0, file);
}

void _XsIobOpenOutput(afxShell sh)
{
    afxSize cap = XssPullSize(sh, 1);
    int dataLen;
    void* data = (void*)lunaGetSlotBytes(sh, 2, &dataLen);

    afxStream file = AfxOpenOutputStream(data, cap);
    XssPushInstance(sh, 0, file);
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

void _XsMseReacquire(afxShell sh)
{
    afxNat port = XssPullNat(sh, 1);
    afxMouse mse;
    AfxGetMouse(port, &mse);
    AfxReacquireObjects(1, (void*[]) { mse });
}

void _XsMseRelease(afxShell sh)
{
    afxNat port = XssPullNat(sh, 1);
    afxMouse mse;
    AfxGetMouse(port, &mse);
    AfxReleaseObjects(1, (void*[]) { mse });
}

void _XsMseLmbIsPressed(afxShell sh)
{
    afxNat port = XssPullNat(sh, 1);
    XssPushBool(sh, 0, AfxLmbIsPressed(port));
}

void _XsMseRmbIsPressed(afxShell sh)
{
    afxNat port = XssPullNat(sh, 1);
    XssPushBool(sh, 0, AfxRmbIsPressed(port));
}

void _XsMseGetMotionX(afxShell sh)
{
    afxNat port = XssPullNat(sh, 1);
    afxV2d delta;
    AfxGetMouseMotion(port, delta);
    XssPushReal(sh, 0, delta[0]);
}

void _XsMseGetMotionY(afxShell sh)
{
    afxNat port = XssPullNat(sh, 1);
    afxV2d delta;
    AfxGetMouseMotion(port, delta);
    XssPushReal(sh, 0, delta[1]);
}

void _XsMseGetWheelDelta(afxShell sh)
{
    afxNat port = XssPullNat(sh, 1);
    XssPushReal(sh, 0, AfxGetMouseWheelDelta(port));
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

void _XsKbdReacquire(afxShell sh)
{
    afxNat port = XssPullNat(sh, 1);
    afxKeyboard kbd;
    AfxGetKeyboard(port, &kbd);
    AfxReacquireObjects(1, (void*[]) { kbd });
}

void _XsKbdRelease(afxShell sh)
{
    afxNat port = XssPullNat(sh, 1);
    afxKeyboard kbd;
    AfxGetKeyboard(port, &kbd);
    AfxReleaseObjects(1, (void*[]) { kbd });
}

void _XsKbdGetKeyPressure(afxShell sh)
{
    afxNat port = XssPullNat(sh, 1);
    afxNat code = XssPullNat(sh, 2);
    XssPushReal64(sh, 0, AfxGetKeyPressure(port, code));
}

void _XsKbdGetKeyCombo(afxShell sh)
{
    afxNat port = XssPullNat(sh, 1);
    afxNat code = XssPullNat(sh, 2);
    afxNat codeB = XssPullNat(sh, 3);
    XssPushReal64(sh, 0, AfxGetCombinedKeyPressure(port, code, codeB));
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

_AFX afxBool GetFfiClass(afxShell sh, afxString const* class, LunaForeignClassMethods* methods)
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

_AFX void* GetFfiMethod(afxShell sh, afxString const* class, afxString const* signature, afxBool firm)
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
