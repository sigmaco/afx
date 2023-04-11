////////////////////////////////////////////////////////////////////////////////
//                     The Standard QWADRO Implementation                     //
////////////////////////////////////////////////////////////////////////////////

#ifndef AFX_SOUND_CONTEXT_H
#define AFX_SOUND_CONTEXT_H

#include "afxSoundInput.h"
#include "afxSoundOutput.h"
#include "afx/core/async/afxThread.h"

AFX_DEFINE_HANDLE(afxSoundContext);
AFX_DECLARE_STRUCT(afxSoundContextImplementation);
AFX_DECLARE_STRUCT(afxSoundContextImplementationData);
AFX_DECLARE_STRUCT(afxSoundIoConnection);

struct afxSoundIoConnection
{
    afxSoundOutput          sout;
    afxArray(afxSoundInput) sin;
};

struct afxSoundContextImplementation
{
    afxResult   (*dtor)(afxSoundContext);
    afxResult   (*process)(afxSoundContext);
    afxResult   (*input)(afxSoundContext,afxSoundInput);
    afxResult   (*output)(afxSoundContext,afxSoundOutput);
};

AFX_OBJECT(afxSoundContext)
{
    afxObject                           obj;
    afxSoundContextImplementation const *impl;
    afxSoundContextImplementationData   *data;
    
    afxArray(afxSoundIoConnection)      connections;

    afxReal                             deltaTime;

    afxOffset                           shouldStop, running;
    afxThread                           *thread;
    afxSize                             tid;
};

#endif//AFX_SOUND_CONTEXT_H