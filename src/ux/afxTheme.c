/*
 *           ::::::::    :::::::::::    ::::::::    ::::     ::::       :::
 *          :+:    :+:       :+:       :+:    :+:   +:+:+: :+:+:+     :+: :+:
 *          +:+              +:+       +:+          +:+ +:+:+ +:+    +:+   +:+
 *          +#++:++#++       +#+       :#:          +#+  +:+  +#+   +#++:++#++:
 *                 +#+       +#+       +#+   +#+#   +#+       +#+   +#+     +#+
 *          #+#    #+#       #+#       #+#    #+#   #+#       #+#   #+#     #+#
 *           ########    ###########    ########    ###       ###   ###     ###
 *
 *                     S I G M A   T E C H N O L O G Y   G R O U P
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This software is part of Advanced User Experiences Extensions & Experiments.

#define _AUX_UX_C
#define _AUX_THEME_C
#include "src/ux/impl/auxImplementation.h"

#ifdef _AUX_THEME_C
#ifndef _AUX_THEME_IMPL
AFX_OBJECT(afxTheme)
#else
AFX_OBJECT(_auxTheme)
#endif
{
    afxUri32 name;
    afxModule mdle;
};
#endif//_AUX_THEME_C

_AUX afxError _AuxThemDtorCb(afxTheme them)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_THEM, 1, &them);


    return err;
}

_AUX afxError _AuxThemCtorCb(afxTheme them, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_THEM, 1, &them);

    afxEnvironment env = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_ENV, 1, &env);
    AFX_ASSERT(args[1]);
    //afxThemeInfo const* cfg = AFX_CAST(afxThemeInfo const*, args[1]) + invokeNo;

    
    return err;
}

_AUX afxClassConfig const _AUX_THEM_CLASS_CONFIG =
{
    .fcc = afxFcc_THEM,
    .name = "Theme",
    .desc = "Multimedia UX Theme",
    .fixedSiz = sizeof(AFX_OBJECT(afxTheme)),
    .ctor = (void*)_AuxThemCtorCb,
    .dtor = (void*)_AuxThemDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AUX afxError AfxAcquireThemes(afxUnit cnt, afxThemeInfo const info[], afxFont fonts[])
{
    afxError err = AFX_ERR_NONE;

    afxEnvironment env;
    if (!AfxGetEnvironment(&env))
    {
        AfxThrowError();
        return err;
    }
    AFX_ASSERT_OBJECTS(afxFcc_ENV, 1, &env);

    if (!info)
    {
        AfxThrowError();
        return err;
    }

    afxClass* cls = (afxClass*)_AuxEnvGetThemClass(env);
    AFX_ASSERT_CLASS(cls, afxFcc_THEM);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)fonts, (void const*[]) { env, info }))
    {
        AfxThrowError();
        return err;
    }

    return err;
}

_AUX afxError AfxLoadThemes(afxUnit cnt, afxUri const uri[], afxTheme themes[])
{
    afxError err = AFX_ERR_NONE;

    afxUri uri2;
    AfxMakeUri(&uri2, 0, "//./z/video/font-256.tga", 0);
    //AvxLoadRasters(dsys, avxRasterUsage_TEXTURE, NIL, 1, &uri2, &dsys->fntRas);

}
