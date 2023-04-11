////////////////////////////////////////////////////////////////////////////////
//                     The Standard QWADRO Implementation                     //
////////////////////////////////////////////////////////////////////////////////

#ifndef AFX_SOUND_OUTPUT_H
#define AFX_SOUND_OUTPUT_H

#include "afxSoundDefs.h"

#define AFX_SOUT_NAME_LEN   (2048)

AFX_DEFINE_HANDLE(afxSoundOutput);
AFX_DECLARE_STRUCT(afxSoundOutputImplementation);
AFX_DECLARE_STRUCT(afxSoundOutputImplementationData);

struct afxSoundOutputImplementation
{
    afxFcc          fcc;
    afxChar const   *name;
    afxResult       (*dtor)(afxSoundOutput);
};

AFX_OBJECT(afxSoundOutput)
{
    afxObject                           obj;
    afxSoundOutputImplementation const  *impl;
    afxSoundOutputImplementationData    *data;

};

#endif//AFX_SOUND_OUTPUT_H