#define _CRT_SECURE_NO_WARNINGS 1
#include <stdio.h>
#define WIN32_LEAN_AND_MEAN 1
#include <Windows.h>

#include "afxHid.h"
#include "afxKeyboard.h"
#include "afxMouse.h"
#include "../../mem/afxMemory.h"
#include "../../base/afxString.h"
#include "../../base/afxLinkedList.h"

// HID API /////////////////////////////////////////////////////////////////////

afxResult AfxUpdateHid(afxHid *hid, afxBool state[], afxInt extra[])
{
    AfxExpectType(hid, AFX_FCC_HID);
    return hid->update(hid, state, extra);
}

void _AfxUnregisterHid(afxHid *hid)
{
    AfxDropLink(&hid->appLink);
    hid->app = NIL;
}

void _AfxRegisterHid(afxHid *hid, afxNat port, afxResult(*update)(afxHid*, afxBool*, afxInt*))
{
    AfxPrependLink(&hid->appLink, NIL);
    hid->app = NIL;
    hid->port = port;
    hid->update = update;
}
