////////////////////////////////////////////////////////////////////////////////
//                     The Standard QWADRO Implementation                     //
////////////////////////////////////////////////////////////////////////////////

#ifndef AFX_SOUND_INPUT_H
#define AFX_SOUND_INPUT_H

#include "afxSoundDefs.h"

AFX_DEFINE_HANDLE(afxSoundInput);
AFX_DECLARE_STRUCT(afxSoundInputImplementation);
AFX_DECLARE_STRUCT(afxSoundInputImplementationData);

struct afxSoundInputImplementation
{
    afxFcc          fcc;
    afxChar const   *name;
    afxResult       (*dtor)(afxSoundInput);
};

AFX_OBJECT(afxSoundInput)
{
    afxObject                           obj;
    afxSoundInputImplementation const   *impl;
    afxSoundInputImplementationData     *data; // implementation-defined data
    
};

#endif//AFX_SOUND_INPUT_H