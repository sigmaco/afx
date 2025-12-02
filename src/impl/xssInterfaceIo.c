


#define _AFX_CORE_C
#define _AFX_SCRIPT_C
#include "../exec/afxSystemDDK.h"
#include "../qwadro_xss/src/xss.h"
#include "qwadro/ux/afxScript.h"

AFX void GenAcqObj(xssVm vm);
AFX void GenRelObj(void* data);
AFX void XssPushError(xssVm vm, afxUnit slot, afxError err);
AFX void XssPushBool(xssVm vm, afxUnit slot, afxBool val);
AFX void XssPushReal(xssVm vm, afxUnit slot, afxReal val);
AFX void XssPushReal64(xssVm vm, afxUnit slot, afxReal64 val);
AFX void XssPushInt(xssVm vm, afxUnit slot, afxInt val);
AFX void XssPushNat(xssVm vm, afxUnit slot, afxUnit val);
AFX void XssPushSize(xssVm vm, afxUnit slot, afxSize val);
AFX afxBool XssPullBool(xssVm vm, afxUnit slot);
AFX afxReal XssPullReal(xssVm vm, afxUnit slot);
AFX afxReal64 XssPullReal64(xssVm vm, afxUnit slot);
AFX void XssPullReal3(xssVm vm, afxUnit slot, afxV3d v);
AFX afxInt XssPullInt(xssVm vm, afxUnit slot);
AFX afxUnit XssPullNat(xssVm vm, afxUnit slot);
AFX afxSize XssPullSize(xssVm vm, afxUnit slot);
AFX afxHandle XssPullHandle(xssVm vm, afxUnit slot);
AFX void XssPushHandle(xssVm vm, afxUnit slot, afxHandle han);
AFX void* XssPullInstance(xssVm vm, afxUnit slot);
AFX void XssPushInstance(xssVm vm, afxUnit slot, void* obj);
AFX void XssPullUri(xssVm vm, afxUnit slot, afxUri* uri);
AFX void XssPullString(xssVm vm, afxUnit slot, afxString* s);
AFX void XssUnhandle(xssVm vm, afxUnit cnt, afxHandle han[]);
AFX void XssHandleEachCall(xssVm vm, afxUnit cnt, afxString const signatures[], afxHandle handles[]);
AFX afxUnit XssHandleAnyCall(xssVm vm, afxUnit cnt, afxString const signatures[], afxHandle handles[]);
AFX afxUnit XssHandleAnyVar(xssVm vm, afxString const* domain, afxUnit baseSlot, afxUnit cnt, afxString const name[], afxHandle handles[]);
AFX afxError XssCall(xssVm vm, afxHandle method);

_AFXINL void AfxGetSlots(afxUnit first, afxUnit cnt, ...)
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

    afxUnit chPos;
    if (AfxFindChar(&mode, 0, 'r', FALSE, FALSE, &chPos))
    {
        ioFlags |= afxFileFlag_R;
        //*modePtr++ = 'r';
    }

    if (AfxFindChar(&mode, 0, 'w', FALSE, FALSE, &chPos))
    {
        ioFlags |= afxFileFlag_W;
        //*modePtr++ = 'w';
    }
    
    if (AfxFindChar(&mode, 0, 'x', FALSE, FALSE, &chPos))
    {
        ioFlags |= afxFileFlag_X;
        //*modePtr++ = 'x';
    }

    afxError err = AfxMountStorageUnit(point.s.start[0], &endpoint, ioFlags);
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

    afxUnit chPos;
    if (AfxFindChar(&mode, 0, 'r', FALSE, FALSE, &chPos))
    {
        ioFlags |= afxFileFlag_R;
        //*modePtr++ = 'r';
    }

    if (AfxFindChar(&mode, 0, 'w', FALSE, FALSE, &chPos))
    {
        ioFlags |= afxFileFlag_W;
        //*modePtr++ = 'w';
    }

    if (AfxFindChar(&mode, 0, 'x', FALSE, FALSE, &chPos))
    {
        ioFlags |= afxFileFlag_X;
        //*modePtr++ = 'x';
    }

    afxError err = AfxDismountStorageUnit(point.s.start[0], &endpoint, ioFlags);
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
    void* dst = (void*)xssGetSlotBytes(vm, 1, &dstCap);
    afxUnit range = XssPullNat(vm, 2);
    afxUnit rate = XssPullNat(vm, 3);
    afxError err = AfxReadStream(in, range, rate, dst);
    XssPushError(vm, 0, err);
}

void _XsIobWrite(xssVm vm)
{
    int srcLen;
    afxStream out = XssPullInstance(vm, 0);
    void const* src = xssGetSlotBytes(vm, 1, &srcLen);
    afxUnit range = XssPullNat(vm, 2);
    afxUnit rate = XssPullNat(vm, 3);
    afxError err = AfxWriteStream(out, range, rate, src);
    XssPushError(vm, 0, err);
}

void _XsIobSeek(xssVm vm)
{
    afxStream iob = XssPullInstance(vm, 0);
    afxInt offset = XssPullInt(vm, 1);
    afxSeekOrigin origin = XssPullInt(vm, 2);
    afxError err = AfxSeekStream(iob, offset, origin);
    XssPushError(vm, 0, err);
}

void _XsIobClose(xssVm vm)
{
    AfxDisposeObjects(1, XssPullInstance(vm, 0));
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
    afxStream file;
    AfxLoadFile(&uri, &file);
    XssPushInstance(vm, 0, &file);
}

void _XsIobOpenFile(xssVm vm)
{
    afxUri uri;
    XssPullUri(vm, 1, &uri);
    afxString mode;
    XssPullString(vm, 2, &mode);

    afxIoFlags ioFlags = NIL;

    afxUnit chPos;
    if (AfxFindChar(&mode, 0, 'r', FALSE, FALSE, &chPos))
    {
        ioFlags |= afxFileFlag_R;
        //*modePtr++ = 'r';
    }

    if (AfxFindChar(&mode, 0, 'w', FALSE, FALSE, &chPos))
    {
        ioFlags |= afxFileFlag_W;
        //*modePtr++ = 'w';
    }

    if (AfxFindChar(&mode, 0, 'x', FALSE, FALSE, &chPos))
    {
        ioFlags |= afxFileFlag_X;
        //*modePtr++ = 'x';
    }


    afxStream file;
    AfxOpenFile(&uri, (afxFileFlags)ioFlags, &file);
    XssPushInstance(vm, 0, file);
}

void _XsIobOpen(xssVm vm)
{
    afxString mode;
    XssPullString(vm, 1, &mode);
    int dataLen;
    void* start = (void*)xssGetSlotBytes(vm, 2, &dataLen);
    afxSize siz = XssPullSize(vm, 3);

    afxIoFlags ioFlags = NIL;

    afxUnit chPos;
    if (AfxFindChar(&mode, 0, 'r', FALSE, FALSE, &chPos))
    {
        ioFlags |= afxIoFlag_R;
        //*modePtr++ = 'r';
    }

    if (AfxFindChar(&mode, 0, 'w', FALSE, FALSE, &chPos))
    {
        ioFlags |= afxIoFlag_W;
        //*modePtr++ = 'w';
    }
    
    
    if (AfxFindChar(&mode, 0, 'x', FALSE, FALSE, &chPos))
    {
        ioFlags |= afxIoFlag_X;
        //*modePtr++ = 'x';
    }

    afxStream file;
    AfxOpenStream(0, start, siz, ioFlags, &file);
    XssPushInstance(vm, 0, file);
}

void _XsIobOpenInput(xssVm vm)
{
    int dataLen;
    void* data = (void*)xssGetSlotBytes(vm, 1, &dataLen);
    afxSize len = XssPullSize(vm, 2);

    afxStream file;
    AfxOpenStream(0, data, len, afxIoFlag_RX, &file);
    XssPushInstance(vm, 0, file);
}

void _XsIobOpenOutput(xssVm vm)
{
    afxSize cap = XssPullSize(vm, 1);
    int dataLen;
    void* data = (void*)xssGetSlotBytes(vm, 2, &dataLen);

    afxStream file;
    AfxOpenStream(0, data, cap, afxIoFlag_WX, &file);
    XssPushInstance(vm, 0, file);
}

afxString const iobVmtNames[] =
{
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
    afxUnit port = XssPullNat(vm, 1);
    afxHid hid;
    AfxGetHid(port, &hid);
    AfxReacquireObjects(1, (void*[]) { hid });
}

void _XsHidRelease(xssVm vm)
{
    afxUnit port = XssPullNat(vm, 1);
    afxHid hid;
    AfxGetHid(port, &hid);
    AfxDisposeObjects(1, (void*[]) { hid });
}

void _XsHidGetKeyPressure(xssVm vm)
{
    afxUnit port = XssPullNat(vm, 1);
    afxUnit code = XssPullNat(vm, 2);
    XssPushReal64(vm, 0, AfxGetKeyPressure(port, code));
}

void _XsHidGetKeyCombo(xssVm vm)
{
    afxUnit port = XssPullNat(vm, 1);
    afxUnit code = XssPullNat(vm, 2);
    afxUnit codeB = XssPullNat(vm, 3);
    XssPushReal64(vm, 0, AfxGetCombinedKeyPressure(port, code, codeB));
}

void _XsHidLmbIsPressed(xssVm vm)
{
    afxUnit port = XssPullNat(vm, 1);
    XssPushBool(vm, 0, AfxIsMousePressed(port, AFX_LMB));
}

void _XsHidRmbIsPressed(xssVm vm)
{
    afxUnit port = XssPullNat(vm, 1);
    XssPushBool(vm, 0, AfxIsMousePressed(port, AFX_RMB));
}

void _XsHidGetMotionX(xssVm vm)
{
    afxUnit port = XssPullNat(vm, 1);
    afxV2d delta;
    AfxGetMouseMotion(port, delta, NIL, NIL);
    XssPushReal(vm, 0, delta[0]);
}

void _XsHidGetMotionY(xssVm vm)
{
    afxUnit port = XssPullNat(vm, 1);
    afxV2d delta;
    AfxGetMouseMotion(port, delta, NIL, NIL);
    XssPushReal(vm, 0, delta[1]);
}

void _XsHidGetWheelDelta(xssVm vm)
{
    afxUnit port = XssPullNat(vm, 1);
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

#if 0
// CAM /////////////////////////////////////////////////////////////////////////

void _XsCamReset(xssVm vm)
{
    arxCamera cam = XssPullInstance(vm, 0);
    ArxResetCamera(cam);
}

void _XsCamApplyMotion(xssVm vm)
{
    arxCamera cam = XssPullInstance(vm, 0);
    afxV3d v;
    XssPullReal3(vm, 1, v);
    ArxTranslateCamera(cam, v);
}

void _XsCamApplyOrientation(xssVm vm)
{
    arxCamera cam = XssPullInstance(vm, 0);
    afxV3d v;
    XssPullReal3(vm, 1, v);
    ArxOrbitCamera(cam, v);
}

void _XsCamApplyOffset(xssVm vm)
{
    arxCamera cam = XssPullInstance(vm, 0);
    afxV3d v, r;
    XssPullReal3(vm, 1, v);
    ArxGetCameraDisplacement(cam, v, avxBlendOp_ADD, r);
    ArxDisplaceCamera(cam, r);
}

void _XsCamApplyDistance(xssVm vm)
{
    arxCamera cam = XssPullInstance(vm, 0);
    ArxApplyCameraDistance(cam, XssPullReal(vm, 1));
}

void _XsCamGet(xssVm vm)
{
    afxUnit id = XssPullNat(vm, 1);
    arxCamera cam = AfxGetInstance(AfxGetCameraClass(0), id);
    XssPushInstance(vm, 0, cam);
}

void _XsCamAcquire(xssVm vm)
{
    afxUri uri;
    arxScenario scio = XssPullInstance(vm, 1);
    afxV3d v;
    XssPullReal3(vm, 2, v);
    arxCamera cam;
    ArxAcquireCameras(scio, 1, &cam);
    ArxTranslateCamera(cam, v);
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
#endif

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
    afxUnit priority = XssPullNat(vm, 4);
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
    afxUnit          cnt;
    afxString const*names;
    void**          ptrs;
}
const ffiSym[] =
{
    {
        ARRAY_SIZE(iobVmtNames),
        iobVmtNames,
        iobVmtPtrs,
    },
    {
        ARRAY_SIZE(fsysVmtNames),
        fsysVmtNames,
        fsysVmtPtrs,
    },
    {
        ARRAY_SIZE(hidVmtNames),
        hidVmtNames,
        hidVmtPtrs,
    },
#if 0
    {
        ARRAY_SIZE(camVmtNames),
        camVmtNames,
        camVmtPtrs,
    },
#endif
    {
        ARRAY_SIZE(uxEvVmtNames),
        uxEvVmtNames,
        uxEvVmtPtrs,
    }
};

_AFX afxBool GetFfiClass(xssVm vm, afxString const* class, LunaForeignClassMethods* methods)
{
    afxBool rslt = FALSE;
    afxUnit clsCnt = ARRAY_SIZE(ffiClasses);

    for (afxUnit i = 0; i < clsCnt; i++)
    {
        afxUnit mId;
        if (AfxCompareStrings(class, 0, FALSE, 1, &ffiClasses[i], &mId))
        {
            methods->allocate = GenAcqObj;
            methods->finalize = GenRelObj;
            rslt = TRUE;
            break;
        }
    }
    return rslt;
}

_AFX void* GetFfiMethod(xssVm vm, afxString const* class, afxString const* signature, afxBool firm)
{
    void* sym = NIL;
    afxUnit clsCnt = ARRAY_SIZE(ffiClasses);

    for (afxUnit i = 0; i < clsCnt; i++)
    {
        afxUnit mId;
        if (AfxCompareStrings(class, 0, FALSE, 1, &ffiClasses[i], &mId))
        {
            for (afxUnit j = 0; j < ffiSym[i].cnt; j++)
            {
                afxUnit mId2;
                if (AfxCompareStrings(signature, 0, FALSE, 1, &ffiSym[i].names[j], &mId2))
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
