/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                  Q W A D R O   E X E C U T I O N   E C O S Y S T E M
 *
 *                                   Public Test Build
 *                       (c) 2017 SIGMA, Engineering In Technology
 *                             <https://sigmaco.org/qwadro/>
 */

// Advanced User Experience Extensions for Qwadro

#ifndef AFX_WIDGET_H
#define AFX_WIDGET_H

#include "qwadro/ux/afxUxDefs.h"
#include "qwadro/draw/afxColor.h"
#include "qwadro/core/afxChain.h"
#include "qwadro/core/afxObject.h"
#include "qwadro/io/afxUri.h"
#include "qwadro/math/afxTransform.h"
//#include "afxViewport.h"
//#include "qwadro/draw/dev/afxDrawInput.h"

typedef enum afxWidgetType
{
    //NIL
    AFX_WIDG_PANEL          = AFX_MAKE_FCC('w', 'p', 'a', 'n'),
    AFX_WIDG_LABEL          = AFX_MAKE_FCC('w', 'l', 'a', 'b'),
    AFX_WIDG_BUTTON         = AFX_MAKE_FCC('w', 'b', 'u', 't'),
    AFX_WIDG_CHECKBOX       = AFX_MAKE_FCC('w', 'c', 'h', 'k'),
    AFX_WIDG_EDITBOX        = AFX_MAKE_FCC('w', 'e', 'd', 't'),
    AFX_WIDG_SLIDER         = AFX_MAKE_FCC('w', 's', 'l', 'd'),
    AFX_WIDG_DROPDOWN       = AFX_MAKE_FCC('w', 'd', 'r', 'p'),
    AFX_WIDG_PROGRESSBAR    = AFX_MAKE_FCC('w', 'p', 'r', 'o'),
    AFX_WIDG_PICTURE        = AFX_MAKE_FCC('w', 'p', 'i', 'c'),
    AFX_WIDG_VIDEO          = AFX_MAKE_FCC('w', 'v', 'i', 'd'),
    AFX_WIDG_GROUP          = AFX_MAKE_FCC('w', 'g', 'r', 'p'),

    AFX_WIDG_TYPE_TOTAL
} afxWidgetType;

AFX_DEFINE_STRUCT( afxWidgetStyle)
{
    afxRect     rect;
};

// screen overlay is a draw input device that has been idealized to be an widget layer.

AFX_DEFINE_STRUCT( afxWidgetVertex)
{
    afxV2d      xy, uv;
};

AFX_DECLARE_STRUCT(afxWidgetImplementation);
AFX_DECLARE_STRUCT(afxWidgetImplementationData);

AFX_DEFINE_STRUCT(afxWidgetConfig)
{
    afxString const *name;
    afxWidget parent;
    afxUri const *uri;
    afxResult(*f)(afxWidget, afxUri const*, void *data);
};

#ifdef _AFX_UX_C
#ifdef _AFX_WIDGET_C
AFX_OBJECT(afxWidget)
{
    afxLinkage                      parent;
    afxChain                        children;

    afxTransform                    local;
    afxM4d                          iw;

    afxResult                       (*update)(afxWidget wid, afxReal dt);

    afxWidgetImplementation const   *impl;
    afxWidgetImplementationData     *data;

    // common
    afxV2d                          anchor; // -1 ... 0 ... 1, left-center-right, top-middle-bottom
    
    afxString*                      text; // 2048
    afxString*                      hint; // 2048
    afxByte                         extra[512];
    struct
    {
        afxConnection               bgTexture;
        afxColor                    color, bgColor;

        afxNat                      visibility;
        afxNat                      cursor; // 'arrow', 'pointer' for links

        // padding, marging
        // border length, border radius

        afxReal                     lineHeight;
        afxReal                     letterSpacing;
        afxNat                      wordBreak;

        struct
        {
            afxString*              face; // 32 // "sans"
            afxReal                 size; // 18.0 (px)
            afxNat                  style; // normal
            afxNat                  weight; // 400
        }                           font;

    };
};
#endif
#endif

AUX afxError    AfxAcquireWidget(afxOverlay ovy, afxViewport const* vp);

AUX afxError    AfxAcquireWidgets(afxOverlay ovy, afxNat cnt, afxWidgetConfig config[], afxWidget widgets[]);

#endif//AFX_WIDGET_H
