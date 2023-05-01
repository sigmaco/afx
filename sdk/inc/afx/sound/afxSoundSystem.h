////////////////////////////////////////////////////////////////////////////////
//                     The Standard QWADRO Implementation                     //
////////////////////////////////////////////////////////////////////////////////

#ifndef AFX_SOUND_SYSTEM_H
#define AFX_SOUND_SYSTEM_H

#include "afxSoundContext.h"
#include "afx/core/afxClass.h"

// Criar um formato de música tracker do Qwadro? Possível extensão seria ".mix"

//#define _AFX_SOUND_MULTITHREADED

AFX_DEFINE_HANDLE(afxSoundSystem);
AFX_DECLARE_STRUCT(afxSoundSystemImplementation);
AFX_DECLARE_STRUCT(afxSoundSystemImplementationData);

struct afxSoundSystemImplementation
{
    afxResult       (*dtor)(afxSoundSystem);
    afxResult       (*process)(afxSoundSystem);
    afxSoundInput   (*sinFind)(afxSoundSystem, void*);
    afxSoundOutput  (*soutFind)(afxSoundSystem, void*);
    afxSoundContext (*sctxFind)(afxSoundSystem, void*);
    afxResult       (*sinImplCtor)(afxSoundInput, afxUri const*);
    afxResult       (*soutImplCtor)(afxSoundOutput, afxUri const*);
    afxResult       (*sctxImplCtor)(afxSoundContext, afxUri const*);
};

AFX_OBJECT(afxSoundSystem)
{
    afxObject                           obj;
    afxModule                           drv;
    afxSoundSystemImplementation const  *impl;
    afxSoundSystemImplementationData    *data;

    afxClass                       soutMeta;
    afxClass                       sctxMeta;
    afxClass                       sinMeta;
};

SFXSYM(afxClass*,  AfxGetSoundSystemMetaObject)(void);

SFXSYM(afxResult,       AfxSoundSystem_Acquire)(afxApplication app, afxUri const *driver, afxSoundSystem *ssys);

SFXSYM(afxResult,       AfxSoundSystem_Process)(afxSoundSystem ssys);

SFXSYM(afxResult,       AfxSoundSystem_ForEachInput)(afxSoundSystem ssys, afxResult (*f)(afxSoundInput, void*), void *data);

SFXSYM(afxResult,       AfxSoundSystem_ForEachOutput)(afxSoundSystem ssys, afxResult (*f)(afxSoundOutput, void*), void *data);

SFXSYM(afxResult,       AfxSoundSystem_ForEachContext)(afxSoundSystem ssys, afxResult (*f)(afxSoundContext, void*), void *data);

SFXSYM(afxSoundInput,   AfxSoundSystem_FindInput)(afxSoundSystem ssys, void *osHandle);

SFXSYM(afxSoundOutput,  AfxSoundSystem_FindOutput)(afxSoundSystem ssys, void *osHandle);

SFXSYM(afxSoundContext, AfxSoundSystem_FindContext)(afxSoundSystem ssys, void *osHandle);

SFXSYM(afxClass*,  AfxSoundSystem_GetInputMetaObject)(afxSoundSystem ssys);

SFXSYM(afxClass*,  AfxSoundSystem_GetOutputMetaObject)(afxSoundSystem ssys);

SFXSYM(afxClass*,  AfxSoundSystem_GetContextMetaObject)(afxSoundSystem ssys);

#endif//AFX_SOUND_SYSTEM_H