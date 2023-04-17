#ifndef AFX_SOUND_DEFS_H
#define AFX_SOUND_DEFS_H

#include "afx/core/base/afxObject.h"
#include "afx/core/base/afxLinkedList.h"
#include "afx/core/async/AfxCondition.h"
#include "afx/core/base/afxEvent.h"
#include "afx/core/mem/afxAllocator.h"

typedef enum
{
    AFX_SSYS_EVENT_SIN_SETTLE,
    AFX_SSYS_EVENT_SIN_DISMANTLE,
    AFX_SSYS_EVENT_SIN_RECONNECT,
    AFX_SSYS_EVENT_SIN_DISCONNECT,

    AFX_SSYS_EVENT_SOUT_SETTLE,
    AFX_SSYS_EVENT_SOUT_DISMANTLE,
    AFX_SSYS_EVENT_SOUT_RECONNECT,
    AFX_SSYS_EVENT_SOUT_DISCONNECT,
    AFX_SSYS_EVENT_SOUT_ACTIVATESIN,

    AFX_SSYS_EVENT_SCTX_SETTLE,
    AFX_SSYS_EVENT_SCTX_DISMANTLE,
    AFX_SSYS_EVENT_SCTX_PROCESS,
    AFX_SSYS_EVENT_SCTX_ACTIVATESOUT,
} afxSoundEvent;

#endif//AFX_SOUND_DEFS_H