

#define _AUX_UX_C
#define _AFX_SYSTEM_C
#include "qwadro/../../_luna/luna.h"
#include "qwadro/ux/afxShell.h"

_AUXINL void AfxGetSlots(afxNat first, afxNat cnt, ...)
{
    va_list va;
    va_start(va, cnt);
    va_arg(va, int);
    va_end(va);
}

// FSYS ////////////////////////////////////////////////////////////////////////

void _XsFsysMount(xssVm vm)
{
    //AfxGetSlots(1, 3, );

    //AfxPullFromEnv(vm, 0, 3, );

    afxString mode;
    afxUri point, endpoint;
    XssPullUri(vm, 1, &point);
    XssPullUri(vm, 2, &endpoint);
    XssPullString(vm, 3, &mode);

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
    XssPushError(vm, 0, err);
}

void _XsFsysDismount(xssVm vm)
{
    //AfxGetSlots(1, 3, );

    afxString mode;
    afxUri point, endpoint;
    XssPullUri(vm, 1, &point);
    XssPullUri(vm, 2, &endpoint);
    XssPullString(vm, 3, &mode);

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
    XssPushError(vm, 0, err);
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

void _XsIobRead(xssVm vm)
{
    int dstCap;
    afxStream in = XssPullInstance(vm, 0);
    void* dst = (void*)lunaGetSlotBytes(vm, 1, &dstCap);
    afxNat range = XssPullNat(vm, 2);
    afxNat rate = XssPullNat(vm, 3);
    afxError err = AfxReadStream(in, range, rate, dst);
    XssPushError(vm, 0, err);
}

void _XsIobWrite(xssVm vm)
{
    int srcLen;
    afxStream out = XssPullInstance(vm, 0);
    void const* src = lunaGetSlotBytes(vm, 1, &srcLen);
    afxNat range = XssPullNat(vm, 2);
    afxNat rate = XssPullNat(vm, 3);
    afxError err = AfxWriteStream(out, range, rate, src);
    XssPushError(vm, 0, err);
}

void _XsIobSeek(xssVm vm)
{
    afxStream iob = XssPullInstance(vm, 0);
    afxInt offset = XssPullInt(vm, 1);
    afxSeekMode origin = XssPullInt(vm, 2);
    afxError err = AfxSeekStream(iob, offset, origin);
    XssPushError(vm, 0, err);
}

void _XsIobClose(xssVm vm)
{
    AfxCloseStream(XssPullInstance(vm, 0));
}

void _XsIobReloadFile(xssVm vm)
{
    afxStream iob = XssPullInstance(vm, 0);
    afxUri uri;
    XssPullUri(vm, 1, &uri);
    afxError err = AfxReloadFile(iob, &uri);
    XssPushError(vm, 0, err);
}

void _XsIobLoadFile(xssVm vm)
{
    afxUri uri;
    XssPullUri(vm, 1, &uri);
    afxStream file = AfxLoadFile(&uri);
    XssPushInstance(vm, 0, &file);
}

void _XsIobOpenFile(xssVm vm)
{
    afxUri uri;
    XssPullUri(vm, 1, &uri);
    afxString mode;
    XssPullString(vm, 2, &mode);

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
    XssPushInstance(vm, 0, file);
}

void _XsIobAcquire(xssVm vm)
{
    afxString mode;
    XssPullString(vm, 1, &mode);
    afxSize siz = XssPullSize(vm, 2);

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
    XssPushInstance(vm, 0, file);
}

void _XsIobOpen(xssVm vm)
{
    afxString mode;
    XssPullString(vm, 1, &mode);
    int dataLen;
    void* start = (void*)lunaGetSlotBytes(vm, 2, &dataLen);
    afxSize siz = XssPullSize(vm, 3);

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
    XssPushInstance(vm, 0, file);
}

void _XsIobOpenInput(xssVm vm)
{
    int dataLen;
    void* data = (void*)lunaGetSlotBytes(vm, 1, &dataLen);
    afxSize len = XssPullSize(vm, 2);

    afxStream file = AfxOpenInputStream(data, len);
    XssPushInstance(vm, 0, file);
}

void _XsIobOpenOutput(xssVm vm)
{
    afxSize cap = XssPullSize(vm, 1);
    int dataLen;
    void* data = (void*)lunaGetSlotBytes(vm, 2, &dataLen);

    afxStream file = AfxOpenOutputStream(data, cap);
    XssPushInstance(vm, 0, file);
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

// HID /////////////////////////////////////////////////////////////////////////

void _XsHidReacquire(xssVm vm)
{
    afxNat port = XssPullNat(vm, 1);
    afxHid hid;
    AfxGetHid(port, &hid);
    AfxReacquireObjects(1, (void*[]) { hid });
}

void _XsHidRelease(xssVm vm)
{
    afxNat port = XssPullNat(vm, 1);
    afxHid hid;
    AfxGetHid(port, &hid);
    AfxReleaseObjects(1, (void*[]) { hid });
}

void _XsHidGetKeyPressure(xssVm vm)
{
    afxNat port = XssPullNat(vm, 1);
    afxNat code = XssPullNat(vm, 2);
    XssPushReal64(vm, 0, AfxGetKeyPressure(port, code));
}

void _XsHidGetKeyCombo(xssVm vm)
{
    afxNat port = XssPullNat(vm, 1);
    afxNat code = XssPullNat(vm, 2);
    afxNat codeB = XssPullNat(vm, 3);
    XssPushReal64(vm, 0, AfxGetCombinedKeyPressure(port, code, codeB));
}

void _XsHidLmbIsPressed(xssVm vm)
{
    afxNat port = XssPullNat(vm, 1);
    XssPushBool(vm, 0, AfxLmbIsPressed(port));
}

void _XsHidRmbIsPressed(xssVm vm)
{
    afxNat port = XssPullNat(vm, 1);
    XssPushBool(vm, 0, AfxRmbIsPressed(port));
}

void _XsHidGetMotionX(xssVm vm)
{
    afxNat port = XssPullNat(vm, 1);
    afxV2d delta;
    AfxGetMouseMotion(port, delta);
    XssPushReal(vm, 0, delta[0]);
}

void _XsHidGetMotionY(xssVm vm)
{
    afxNat port = XssPullNat(vm, 1);
    afxV2d delta;
    AfxGetMouseMotion(port, delta);
    XssPushReal(vm, 0, delta[1]);
}

void _XsHidGetWheelDelta(xssVm vm)
{
    afxNat port = XssPullNat(vm, 1);
    XssPushReal(vm, 0, AfxGetMouseWheelDelta(port));
}

afxString const hidVmtNames[] =
{
    AFX_STRING("Release(_)"),
    AFX_STRING("Reacquire(_)"),

    AFX_STRING("GetKeyPressure(_,_)"),
    AFX_STRING("GetKeyCombo(_,_,_)"),

    AFX_STRING("LmbIsPressed(_)"),
    AFX_STRING("RmbIsPressed(_)"),

    AFX_STRING("GetMotionX(_)"),
    AFX_STRING("GetMotionY(_)"),
    AFX_STRING("GetWheelDelta(_)")

};

void* const hidVmtPtrs[] =
{
    _XsHidRelease,
    _XsHidReacquire,

    _XsHidGetKeyPressure,
    _XsHidGetKeyCombo,

    _XsHidLmbIsPressed,
    _XsHidRmbIsPressed,

    _XsHidGetMotionX,
    _XsHidGetMotionY,
    _XsHidGetWheelDelta
};

// CAM /////////////////////////////////////////////////////////////////////////

void _XsCamReset(xssVm vm)
{
    afxCamera cam = XssPullInstance(vm, 0);
    AfxResetCamera(cam);
}

void _XsCamApplyMotion(xssVm vm)
{
    afxCamera cam = XssPullInstance(vm, 0);
    afxV3d v;
    XssPullReal3(vm, 1, v);
    AfxApplyCameraMotion(cam, v);
}

void _XsCamApplyOrientation(xssVm vm)
{
    afxCamera cam = XssPullInstance(vm, 0);
    afxV3d v;
    XssPullReal3(vm, 1, v);
    AfxApplyCameraElevAzimRoll(cam, v);
}

void _XsCamApplyOffset(xssVm vm)
{
    afxCamera cam = XssPullInstance(vm, 0);
    afxV3d v;
    XssPullReal3(vm, 1, v);
    AfxApplyCameraOffset(cam, v);
}

void _XsCamApplyDistance(xssVm vm)
{
    afxCamera cam = XssPullInstance(vm, 0);
    AfxApplyCameraDistance(cam, XssPullReal(vm, 1));
}

void _XsCamGet(xssVm vm)
{
    afxNat id = XssPullNat(vm, 1);
    afxCamera cam = AfxGetObject(AfxGetCameraClass(), id);
    XssPushInstance(vm, 0, cam);
}

void _XsCamAcquire(xssVm vm)
{
    afxUri uri;
    afxDrawInput din = XssPullInstance(vm, 1);
    afxV3d v;
    XssPullReal3(vm, 2, v);
    afxCamera cam;
    AfxAcquireCameras(din, 1, &cam);
    AfxApplyCameraMotion(cam, v);
    XssPushInstance(vm, 0, cam);
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

AFX_DEFINE_STRUCT(afxScriptedEvent)
{
    afxString32     name;
    afxHandle       fn;
};

void _XsUxEvRegister(xssVm vm)
{
    afxString name;
    XssPullString(vm, 1, &name);

    afxHandle fn;
    XssHandleAnyCall(vm, 1, &name, &fn);

}

void _XsUxEvAddHandler(xssVm vm)
{
    afxString mode;
    XssPullString(vm, 1, &mode);
    afxHandle tar = XssPullInstance(vm, 2);
    afxHandle fn = XssPullInstance(vm, 3);
    afxNat priority = XssPullNat(vm, 4);
}

void _XsUxEvTrigger(xssVm vm)
{
    afxString mode;
    XssPullString(vm, 1, &mode);
    afxHandle tar = XssPullInstance(vm, 2);
}

afxString const uxEvVmtNames[] =
{
    AFX_STRING("Register(_)"),
    AFX_STRING("AddHandler(_,_,_,_)"),
    AFX_STRING("Trigger(_,_)"),
};

void const* uxEvVmtPtrs[] =
{
    _XsUxEvRegister,
    _XsUxEvAddHandler,
    _XsUxEvTrigger,
};

////////////////////////////////////////////////////////////////////////////////

afxString const ffiClasses[] =
{
    AFX_STRING("Stream"),
    AFX_STRING("Storage"),
    AFX_STRING("Hid"),

    AFX_STRING("Camera"),
    AFX_STRING("UxEvent"),
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
        AFX_COUNTOF(hidVmtNames),
        hidVmtNames,
        hidVmtPtrs,
    },
    
    {
        AFX_COUNTOF(camVmtNames),
        camVmtNames,
        camVmtPtrs,
    },
    {
        AFX_COUNTOF(uxEvVmtNames),
        uxEvVmtNames,
        uxEvVmtPtrs,
    }
};

_AUX afxBool GetFfiClass(xssVm vm, afxString const* class, LunaForeignClassMethods* methods)
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

_AUX void* GetFfiMethod(xssVm vm, afxString const* class, afxString const* signature, afxBool firm)
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
